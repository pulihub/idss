/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             		 *
 *  No title to or ownership of D1HT is hereby transferred.	                 *
 *									     									 *
 *  D1HT is free software; you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License version 3 as        * 
 *  as published by the Free Software Foundation. Besides, the               *
 *  recipient of D1HT acknowledges and agrees to                             *
 *  strictly respect the following guidelines: 	                             *
 *                                                                           *
 *  (1) This whole copyright notice shall not be removed or modified,        *
 *      although it can and should be appended with informations about       *
 *      modifications included in the software                               *
 *  (2) No results or statistics obtained with the software compiled         *
 *      with the DEBUG flag turned on should be considered as valid          *
 *  (3) Any modified version of this software should be made publicy         *
 *      available BEFORE the results or statistics obtained with this        *
 *      respective version are published.                                    *
 *  (4) Errors and problems with the software (including its results and     *
 *      statistics) should be immediately reported to support.D1HT@gmail.com *
 *  (5) After modifying the source code, it is necessary to compile D1HT     *
 *      with the DEBUG flag on and run it with the flags “–c –V –T” with at  *
 *      least a few dozen peers before considering the modifications as valid*
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with D1HT. If not, see <http://www.gnu.org/licenses/>.              *
 *                                                                           *
 * D1HT is distributed in the hope that it will be usefull, but              *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU GPL      *
 * for more details.                                                         *
 *                                                                           *
 * The information in this software is subject to change without notice      *
 * and should not be construed as a commitment by its authors.               *
 *                                                                           *
 * The authors assume no responsibility or liability for                     *
 * the use, reliability, performance, and results of D1HT.                   *
 *                                                                           *
 *     ################  ==> Please see ChangeLog <==  ################      *
 *                                                                           *
 *****************************************************************************/

/*************************************************************************
 *                    INITIAL VERSION REMARKS                            *
 *                                                                       *
 * Author:        Luiz Rodolpho Monnerat, Petrobras                      *
 * Advisor:       Prof. Claudio L Amorim, COPPE Systems Engineering,     *
 *                Federal University of Rio de Janeiro.                  *
 * Documentation: L. Monnerat and C. Amorim, Peer-to-Peer Single Hop     *
 *                Distributed Hash Tables, in Proceedings of the IEEE    *
 *                Global Communications Conference (GLOBECOM), Nov/2009. *
 *                Available from http://www.cos.ufrj.br/~monnerat        *
 *                                                                       *
 *************************************************************************
 *                                                                       *
 * The msg headers used in this implementation are slightly different(and*
 * bigger) than the ones used in the experiments presented in the        *
 * GLOBECOM 2009 paper. This is because we included a 4 bytes “systemid” *
 * field in the message headers presented in Figures 1.a and 1.b of      *
 * the GLOBECOM paper, and so all messages will have 4 more bytes (these *
 * 4 bytes should be added to all D1HT, CALOT and OneHop msgs, including *
 * acks). This was done because in some tests that we made after         *
 * uploading the final paper version, we observed that when              *
 * running 2 distinct D1HT and/or CALOT  peers in the same node, a peer  *
 * from one system could (by mistake or software bug) send a msg to the  *
 * peer of the other system, which would pollute both peers Routing      *
 * Tables.                                                               *
 *                                                                       *
 *************************************************************************/
/*********************** D1HT COPYRIGHT NOTICE END **********************/
/**
 * @file events.c
 * @brief 
 */

#include "D1HT.h"
#include <sys/socket.h>

namespace d1ht {

    extern int get_udp_socket(const PORTNO port);

    char g_buffer[D1HT_MTU];

    class msgs_received_t {
#define MAXMSGS	5*D1HT_RHO_MAX
        msg_hd headers[MAXMSGS];
        IPaddr senders[MAXMSGS];
        D1HT_MAINTENANCE_body bodies[MAXMSGS];
        int next;
        lock_t msgs_received_mutex;

    public:

        msgs_received_t() {
            BVZERO(headers, sizeof (headers));
            BVZERO(senders, sizeof (senders));
            BVZERO(bodies, sizeof (bodies));
            next = 0;
            LOCK_INIT(msgs_received_mutex, maychain = false, 0);
        }

        bool check(const IPaddr sender, const msg_hd header, const D1HT_MAINTENANCE_body body) {
            int i;
            bool retcode;

            LOCK(msgs_received_mutex);

            for (i = 0; i < MAXMSGS; i++) {
                if (headers[i] == header && senders[i] == sender && bodies[i] == body) break;
            }

            if (i != MAXMSGS) {
                retcode = true;
            } else {
                headers[next] = header;
                senders[next] = sender;
                bodies[next] = body;
                next = (next + 1) % MAXMSGS;
                retcode = false;
            }

            UNLOCK(msgs_received_mutex);
            return retcode;
        }
    };

    msgs_received_t g_msgs_received;

    // this routine should quickly read the whole maintenance msg in order to allow UDP_server to receive other msgs

    bool eventmsg_t::eventmsg_receive(const int socket) {
        char msgerr[256];
        bool retcode = false;
        ssize_t nbytes;
        struct msghdr msg;
        BZERO(msg);
        struct cmsghdr cmsg;
        BZERO(cmsg);

        msg.msg_name = (void *) &sender_address;
        msg.msg_namelen = sizeof (sender_address);
        msg.msg_iov = iov;
        msg.msg_iovlen = 2;
        msg.msg_control = &cmsg;
        msg.msg_controllen = sizeof (cmsg);
        msg.msg_flags = 0;

        iov[0].iov_base = (void *) &msg_header;
        iov[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        iov[1].iov_base = (void *) &body;
        iov[1].iov_len = (size_t) D1HT_MAINTENANCE_body_size;

        XEQ_EXEC(nbytes = recvmsg(socket, &msg, MSG_PEEK), XEQ_WARNING);

        int errsave = errno;

        if (nbytes == 0) { // it seems that the connection was closed
            return false;
        }

        if (nbytes < 0) {
            if (G_exiting) pthread_exit(NULL);
            if (errsave == EBADF || errsave == ENOTCONN || errsave == ENOTSOCK) {
                D1HT_CLOSE(socket);
                if (socket == G_UDP_socket) {
                    verbose(stderr, "WARNING Cause: reopening local UDP port. Detail: %s\n", strerror(errsave));
                    G_UDP_socket = get_udp_socket(G_parms.myIP.port);
                }
            }
            return false;
        }

        msg_hd nack_header;

        nack_header.systemid = G_parms.systemid;

        if (G_exiting) {
            nack_header.type = D1HT_LEAVE;
            nack_header.SeqNo = D1HT_LEAVE;
            sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL);
            return false;
        }

        nack_header.type = D1HT_INVALID;

        if (nbytes < (int) D1HT_MSG_HEADER_size) {
            VERBOSE(stderr, "WARNING Cause: Incomplete msg peeked. Detail: nbytes=%d, headersize=%d\n", (int) nbytes, (int) D1HT_MSG_HEADER_size);
            sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL);
            return false;
        }

        // we must register the time we received thess events to ensure that we will not hold any event for more than Theta secs 
        time_received = (int) elapsed_msecs();
        ;

        SeqNo = msg_header.SeqNo;
        whosent.ip = sender_address.sin_addr;
        whosent.port = msg_header.sender_port;
        TTL = msg_header.type;

        if (nbytes >= (ssize_t) (D1HT_MSG_HEADER_size + D1HT_MAINTENANCE_body_size)) {
            sprintf(msgerr, "From %s:%u, TTL=%u, SeqNo=%u, totbytes=%d", IPtoP(whosent), TTL, SeqNo, (int) nbytes);
        } else {
            sprintf(msgerr, "From %s:%u, TTL=%u, SeqNo=%u, totbytes=%d, events=(%u,%u,%u,%u),", IPtoP(whosent), TTL, SeqNo, (int) nbytes,
                    (uint) body.nevents[0][0], (uint) body.nevents[0][1], (uint) body.nevents[1][0], (uint) body.nevents[1][1]);
        }

        if ((sender_address.sin_addr.s_addr == 0) && (msg_header.sender_port != 0)) {
            VERBOSE(stderr, "WARNING Cause: Invalid IP in UDP event msg. Detail: %s\n", msgerr);
            if (recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL) > 0) // cleanning the input UDP queue
                sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        if (msg_header.type > D1HT_TTL_0_NOEVENTS) {
            VERBOSE(stderr, "WARNING Cause: Invalid type (TTL) of msg in event msg. Detail: %s\n", msgerr);
            if (recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL) > 0 && whosent.port != 0) // cleanning the input UDP queue
                sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        if (G_parms.systemid != 0 && msg_header.systemid != G_parms.systemid) {
            VERBO(stderr, "WARNING Cause: System id mismatch. Detail: %s, mine=%lu, systemid sent=%lu\n", msgerr, (ulong) G_parms.systemid, (ulong) msg_header.systemid);
            if (recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL) > 0 && whosent.port != 0) // cleanning the input UDP queue
                sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        if (whosent.port != 0) {

            VERBOSEX(stderr, "D1HT maintenance msg peeked. %s nbytes=%d\n", msgerr, (int) nbytes);

            G_probe.probed(whosent);

            if (TTL == D1HT_TTL_0_NOEVENTS) {

                recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL); // cleanning the input UDP queue

                if (G_exiting) {
                    sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                    pthread_exit(NULL);
                }

                if (SeqNo == 0) {
                    VERBOSE(stderr, "WARNING Cause: Invalid SeqNo in D1HT_TTL_0_NOEVENTS maintenance msg. Detail: %s nbytes=%d\n", msgerr, (int) nbytes);
                    sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                    return false;
                }

                if (nbytes != (int) D1HT_MSG_HEADER_size) {
                    VERBOSE(stderr, "WARNING Cause: invalid D1HT_TTL_0_NOEVENTS msg received. Detail: %s nbytes=%d (should be %lu bytes) \n", msgerr, (int) nbytes, (ulong) D1HT_MSG_HEADER_size);
                    sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                    return false;
                }

                if (!G_parms.delayed_acks && whosent.port != 0) {
                    XEQ_EXEC(sendto(socket, (void *) &(msg_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address)), XEQ_WARNING);
                    LOCK(G_stats.stat_mutex);
                    G_stats.total.stats[stat_ack_msgs_sent].count++;
                    G_stats.total.stats[stat_ack_bytes_sent].count += (D1HT_MSG_HEADER_size + D1HT_UDP_HEADER_SIZE);
                    UNLOCK(G_stats.stat_mutex);
                }

                IPaddr peer_to_probe;
                peer_to_probe.port = 0;

                LOCK(G_RT.rt_mutex);

                IPaddr old_pred = G_RT.pred_IP();
                G_RT.add(whosent);

                if (whosent != G_RT.pred_IP() && whosent != old_pred) {

                    VERBOSE(stderr, "WARNING Cause: msg received from wrong peer. Detail: %s (should be %s:%u)\n", msgerr, IPtoP(G_RT.pred_IP()));

                    if (INSTRETCH((G_RT.pred_ID()), G_myID, ip2id(whosent))) peer_to_probe = G_RT.pred_IP();

                    G_RT.stabilize(whosent, __FILE__, __LINE__);
                }
                UNLOCK(G_RT.rt_mutex);

                if (peer_to_probe.port != 0) G_probe.force_probe(peer_to_probe, __FILE__, __LINE__);

                return false;
            }

            if (g_msgs_received.check(whosent, msg_header, body)) {

                if (G_exiting) {
                    sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                    pthread_exit(NULL);
                }

                XEQ_EXEC(sendto(socket, (void *) &(msg_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address)), XEQ_WARNING);

                LOCK(G_stats.stat_mutex);
                G_stats.total.stats[stat_duplicated_msgs].count++;
                G_stats.total.stats[stat_ack_msgs_sent].count++;
                G_stats.total.stats[stat_ack_bytes_sent].count += (D1HT_MSG_HEADER_size + D1HT_UDP_HEADER_SIZE);
                UNLOCK(G_stats.stat_mutex);

                VERBOSE(stderr, "Duplicated maint msgs received: Detail: %s\n", msgerr);
                recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL); // cleanning the input UDP queue
                return false;
            }
        } else {
            VERBOSE(stderr, "Forwarded events peeked. Detail: %s nbytes=%d\n", msgerr, (int) nbytes);
            whosent.ip.s_addr = 0;
        }

        if (nbytes < (int) (D1HT_MSG_HEADER_size + D1HT_MAINTENANCE_body_size)) {
            VERBOSE(stderr, "WARNING Cause: Incomplete msg peeked. Detail: %s, nbytes=%d (shoul be %u bytes)\n", msgerr, (int) nbytes, (uint) (D1HT_MSG_HEADER_size + D1HT_MAINTENANCE_body_size));
            if (whosent.port != 0) sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        iov[0].iov_base = (void *) &msg_header;
        iov[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        iov[1].iov_base = (void *) &body;
        iov[1].iov_len = (size_t) D1HT_MAINTENANCE_body_size;

        this->eventmsg_init();
        iovcnt = 2;

        ssize_t bytes_to_receive = D1HT_MSG_HEADER_size + D1HT_MAINTENANCE_body_size;

        tot_events = 0;
        // set the pointers to receive the IPs (without ports) of all the events 
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                if (body.nevents[iport][itype] != 0) {
                    if (eventlist[iport][itype].ips == NULL) {
                        if (recvfrom(socket, &g_buffer, sizeof (g_buffer), 0, NULL, NULL) > 0 && whosent.port != 0) // cleanning the input UDP queue
                            sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                        return false;
                    }
                    iov[iovcnt].iov_base = (void *) eventlist[iport][itype].ips;
                    iov[iovcnt].iov_len = (size_t) body.nevents[iport][itype] * sizeof (IP);
                    bytes_to_receive += iov[iovcnt].iov_len;
                    tot_events += (int) body.nevents[iport][itype];
                    iovcnt++;
                }
            }
        }

        // set the pointers to receive the ports of the events not in the default port 
        for (int itype = JOINS; itype <= LEAVES; itype++) {
            if (body.nevents[OTHER_PORT][itype] != 0) {
                ASSERT(iovcnt < D1HT_EVENT_MAX_IOVCNT);
                iov[iovcnt].iov_base = (void *) eventlist[OTHER_PORT][itype].ports;
                iov[iovcnt].iov_len = (size_t) body.nevents[OTHER_PORT][itype] * sizeof (PORTNO);
                bytes_to_receive += iov[iovcnt].iov_len;
                iovcnt++;
            }
        }

        Assert(iovcnt < D1HT_EVENT_MAX_IOVCNT + 1, XEQ_ERROR);

        BZERO(cmsg);
        BZERO(msg);
        BZERO(sender_address);

        msg.msg_name = (void *) &sender_address;
        msg.msg_namelen = sizeof (sender_address);
        msg.msg_iov = iov;
        msg.msg_iovlen = iovcnt;
        msg.msg_control = &cmsg;
        msg.msg_controllen = sizeof (cmsg);
        msg.msg_flags = 0;

        XEQ_EXEC(nbytes = recvmsg(socket, &msg, 0), XEQ_WARNING);
        VERBOSE(stderr, "Maintenance msg received. %s, nbytes=%d\n", msgerr, (int) nbytes);

        if (nbytes != bytes_to_receive || G_exiting) {
            if (whosent.port != 0) sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        int tot_events_n = 0;
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                if (body.nevents[iport][itype] > 0) {
                    tot_events_n += (int) body.nevents[iport][itype];
                    if (eventlist[iport][itype].ips == NULL || eventlist[iport][itype].ports == NULL || eventlist[iport][itype].ids == NULL) {
                        VERBOX(stderr, "WARNING Cause: body.nevents does not match. Detail: tot_events=%d, iport=%d, itype=%d, body.nevents=%d\n",
                                (int) tot_events, (int) iport, (int) itype, (int) body.nevents[iport][itype]);
                        return false;
                    }
                }

            }
        }

        if (tot_events_n != tot_events) {
            VERBOSE(stderr, "WARNING Cause: Invalid msg received. Detail: %s, tot_events=%d (shoul be %d)\n", msgerr, tot_events_n, tot_events);
            if (whosent.port != 0) sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
            return false;
        }

        if (whosent.ip.s_addr == 0) {
            VERBOSE(stderr, "Forwarded events received. %s nbytes=%d, tot_events=%d.\n", msgerr, (int) nbytes, tot_events);
        } else {
            // The msg arrived ok, so send the ack, which is just the msg header

            if (G_exiting) {
                sendto(socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                pthread_exit(NULL);
            }

            if (!G_parms.delayed_acks && whosent.port != 0) {
                XEQ_EXEC(sendto(socket, (void *) &(msg_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address)), XEQ_WARNING);
                LOCK(G_stats.stat_mutex);
                G_stats.total.stats[stat_ack_msgs_sent].count++;
                G_stats.total.stats[stat_ack_bytes_sent].count += (D1HT_MSG_HEADER_size + D1HT_UDP_HEADER_SIZE);
                UNLOCK(G_stats.stat_mutex);
            }

            VERBOSE(stderr, "%s maintenance msg received. %s nbytes=%d, tot_events=%d\n", G_parms.program_name, msgerr, (int) nbytes, tot_events);

            IPaddr whosentN;
            whosentN.port = msg_header.sender_port;
            whosentN.ip = sender_address.sin_addr;
            if (!Assert(whosent == whosentN, XEQ_WARNING)) return retcode;
            if (!Assert(SeqNo == msg_header.SeqNo, XEQ_WARNING)) return retcode;
            if (msg.msg_flags && MSG_TRUNC) {
                VERBO(stderr, "WARNING Cause: MSG_TRUNC - Detail: %s nbytes=%d, tot_events=%d\n", msgerr, (int) nbytes, tot_events);
                return retcode;
            }
        }

#if defined(DEBUG)
        //	check(__FILE__, __LINE__);
        if (G_parms.selfmsgs && whosent == G_parms.myIP) {
            selfmsg_t *selfmsg, *selfmsgp;
            VERBOSE(stderr, "Self msg received, ttl=%u, SeqNo=%u\n", TTL, SeqNo);

            LOCK_FORCED(G_selfmsgs_mutex);
            for (selfmsgp = &G_selfmsgs; selfmsgp->next != NULL &&
                    (selfmsgp->next->msg_header.SeqNo != SeqNo || selfmsgp->next->msg_header.type != TTL); selfmsgp = selfmsgp->next);

            ASSERT(selfmsgp->next != NULL, "Self msg ERROR - could not find this msg");

            selfmsg = selfmsgp->next;
            selfmsgp->next = selfmsg->next;
            UNLOCK(G_selfmsgs_mutex);

            ASSERT(selfmsg->msg_body == body, "Self msg ERROR - body of the msg did not match");

            for (int itype = JOINS; itype <= LEAVES; itype++) {

                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                    if (selfmsg->msg_body.nevents[iport][itype] != 0) {

                        if (iport == OTHER_PORT) {

                            for (int ievent = 0; ievent < (int) selfmsg->msg_body.nevents[OTHER_PORT][itype]; ievent++)
                                ASSERT(selfmsg->ports[itype][ievent] == eventlist[OTHER_PORT][itype].ports[ievent], "Self msg ERROR");

                            FREE(selfmsg->ports[itype]);
                        }

                        for (int ievent = 0; ievent < (int) selfmsg->msg_body.nevents[iport][itype]; ievent++)
                            ASSERT(selfmsg->ips[iport][itype][ievent].s_addr == eventlist[iport][itype].ips[ievent].s_addr, "Self msg ERROR");

                        FREE(selfmsg->ips[iport][itype]);
                    }
                }
            }
            FREE(selfmsg);
            VERBOSEX(stderr, "Self msg received OK!! ttl=%u, SeqNo=%u\n", TTL, SeqNo);
            return false;
        }
        if (G_parms.VERBOSE)eventmsg_print();
        if (G_parms.check) eventmsg_check(__FILE__, __LINE__);
#endif

        return true;
    };

} // namespace d1ht
