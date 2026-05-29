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
 * @file close_interval.c
 * @brief gestione della segnalazione allo scadere di theta
 */

#include "D1HT.h"
#include "include.h"

using namespace std;

namespace d1ht {
    rtt_info G_rtt_msg;

    extern join_t G_joining_peers;
    extern void *start_interval(const int itime);

    /**
     * richiamata da close_interval, invia i messaggi
     * @param temp_interval
     * @return
     */
    void *send_maint_msg(const temp_interval_t *temp_interval) {

        ASSERT(temp_interval != NULL);
        ASSERT(temp_interval->target.port != 0);

        stack<IPaddr> IPs_to_probe;

        NUM_32 ttl = temp_interval->ttl;
        interval_t *interval = temp_interval->interval;
        IPaddr target = temp_interval->target;

        msg_hd msg_header;
        BZERO(msg_header);
        D1HT_MAINTENANCE_body msg_body;
        BZERO(msg_body);
        struct iovec iov[D1HT_EVENT_MAX_IOVCNT];
        BVZERO(iov, sizeof (iov));
        int iovcnt = 0, bytes_sent = 0, bytes_to_send = 0;

        iov[iovcnt].iov_base = (void *) &msg_header;
        iov[iovcnt].iov_len = (size_t) D1HT_MSG_HEADER_size;
        bytes_to_send = D1HT_MSG_HEADER_size;
        iovcnt++;

        msg_header.type = ttl;
        msg_header.sender_port = G_parms.myIP.port;
        msg_header.systemid = G_parms.systemid;
        msg_header.SeqNo = interval->SeqNo;
#if defined(DEBUG)
        if (interval->SeqNo == 0) ASSERT(G_exiting);
#endif
        //se sto uscendo, metto SeqNo a 0 per segnalare anche il mio LEAVE
        if (G_exiting) msg_header.SeqNo = 0;

        iov[iovcnt].iov_base = (void *) &msg_body;
        iov[iovcnt].iov_len = (size_t) D1HT_MAINTENANCE_body_size;
        bytes_to_send += D1HT_MAINTENANCE_body_size;

        iovcnt++;
        int nevents[2][2];
        IP * ips[2][2];
        PORTNO * ports[2];

        KEYID targetID = ip2id(target);

        if (interval->E > 0) {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                ports[itype] = CALLOC((interval->E + 1), PORTNO);
                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                    ips[iport][itype] = CALLOC((interval->E + 1), IP);
                    nevents[iport][itype] = 0;
                }
            }
        } else {
            Assert(ttl == 0, XEQ_ERROR);
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                ports[itype] = NULL;
                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                    ips[iport][itype] = NULL;
                    nevents[iport][itype] = 0;
                }
            }
        }

        for (events_t *next_event = interval->events; next_event != NULL; next_event = next_event->next) {
            //
            //	The statement below is to enforce rule 3 from the IPDPS paper,
            //
            if (next_event->TTL <= ttl) {
                //			VERBOSE(stderr,"event id=%u, ip=%s:%u, type=%u, ttl=%u discharged because its ttl is less than or equal the msg ttl (%u)\n",
                //					next_event->id, IPtoP(next_event->peer), next_event->type, next_event->TTL, ttl);
                continue;
            }
            //
            //	The statement below is to enforce rule 8 from the IPDPS paper, i.e., we will remove from the message
            //  any event that is in strech(myself,target)
            //
            if (INSTRETCH(next_event->id, G_myID, targetID)) {
                //			VERBOSE(stderr,"event id=%u, ip=%s:%u, type=%u, ttl=%u discharged because its id is between mine (%u) and the target (%u)\n",
                //					next_event->id, IPtoP(next_event->peer), next_event->type, next_event->TTL, G_myID, targetID);
                continue;
            }

            int itype = next_event->type;
            int iport = OTHER_PORT;

            if (next_event->peer.port == G_parms.default_udp_port && nevents[DEFAULT_PORT][itype] < 253) iport = DEFAULT_PORT;

            if (nevents[iport][itype] > 253) {
                VERBO(stderr, "WARNING Cause: Too many events per type. Detail: type=%d\n", itype);
                continue;
            }

            if (iport == OTHER_PORT) ports[itype][nevents[iport][itype]] = next_event->peer.port;

            ips[iport][itype][nevents[iport][itype]] = next_event->peer.ip;

            nevents[iport][itype]++;

            ASSERT(nevents[iport][itype] <= (NUM_8) interval->E);
            ASSERT(nevents[iport][itype] <= 254);
        }

        // set the IP without ports of all events to be xmited
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                if (nevents[iport][itype] != 0) {
                    ASSERT(nevents[iport][itype] <= (NUM_8) interval->E);
                    iov[iovcnt].iov_base = (void *) ips[iport][itype];
                    iov[iovcnt].iov_len = (size_t) nevents[iport][itype] * sizeof (IP);
                    bytes_to_send += iov[iovcnt].iov_len;
                    iovcnt++;
                }
            }
        }
        // set the port of the events not in the default port to be xmited
        for (int itype = JOINS; itype <= LEAVES; itype++) {
            if (nevents[OTHER_PORT][itype] != 0) {
                iov[iovcnt].iov_base = (void *) ports[itype];
                iov[iovcnt].iov_len = (size_t) nevents[OTHER_PORT][itype] * sizeof (PORTNO);
                bytes_to_send += iov[iovcnt].iov_len;
                iovcnt++;
            }
        }
        Assert(iovcnt < D1HT_EVENT_MAX_IOVCNT + 1, XEQ_ERROR);
        Assert(iovcnt > 1, XEQ_ERROR);

        uint tot_events = (uint) (nevents[0][0] + nevents[0][1] + nevents[1][0] + nevents[1][1]);

#if defined(DEBUG)
        if (tot_events > interval->E) EXIT("Wrong events count. Detail: nevents=(%u,%u,%u,%u), tot_events=%u, E=%u\n",
                (uint) nevents[0][0], (uint) nevents[0][1], (uint) nevents[1][0], (uint) nevents[1][1], (uint) tot_events, (uint) interval->E);
        ASSERT(tot_events <= interval->E);
        if (bytes_to_send > interval->T_bytes) EXIT("bytes_to_send > interval->T_bytes. Detail: bytes_to_send=%d, interval->T_bytes=%d", bytes_to_send, interval->T_bytes);
#endif

        if (iovcnt == 2 && ttl != 0) { // besides the msg_header and msg_body, is there something else to send??
            VERBOSE(stderr, "All events discharged.\n");
            //		G_RT.pub_print_table(stderr);
        } else {
            Assert((ttl == 0) || (tot_events != 0), XEQ_ERROR);

            if (tot_events == 0) {
                ASSERT(iovcnt == 2);
                msg_header.type = D1HT_TTL_0_NOEVENTS;
                iovcnt = 1;
                bytes_to_send = D1HT_MSG_HEADER_size;
                VERBOSEX(stderr, "Sending D1HT_TTL_0_NOEVENTS msg to %s:%u, SeqNo=%u\n", IPtoP(target), msg_header.SeqNo);
            } else {
#if	defined(DEBUG)
                if (G_parms.VERBOSE) {
                    PORTNO port = G_parms.default_udp_port;
                    for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                        for (int itype = JOINS; itype <= LEAVES; itype++) {
                            if (nevents[iport][itype]) {
                                VERBOSE(stderr, "Events to xmit to %s:%u, SeqNo=%u, ttl=%u, port=%u, type=%u, qty=%u: ", IPtoP(target), msg_header.SeqNo, ttl, iport, itype, nevents[iport][itype]);
                                for (int ievent = 0; ievent < (int) nevents[iport][itype]; ievent++) {
                                    if (iport != DEFAULT_PORT) port = ports[itype][ievent];
                                    fprintf(stderr, "(%u, %s:%u) ", ip2id(ips[iport][itype][ievent], port),
                                            NTOP(ips[iport][itype][ievent]), ntohs(port));
                                    ASSERT(ntohs(port) != 0)
                                }
                                fprintf(stderr, "\n");
                            }
                        }
                    }
                }
#endif
                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++)
                    for (int itype = JOINS; itype <= LEAVES; itype++)
                        msg_body.nevents[iport][itype] = (NUM_8) nevents[iport][itype];
            }
            int connection = -1;
            int tries = 0, targets = 0, maxtries = G_parms.retries;
            struct timeval tv_begin, tv_end, tv_rto, prev_tv;
            bool ack_received = false;
            bool rxmited = false;
            IPaddr peer_to_probe;
            BZERO(peer_to_probe);
            stat_part_t local_stats;

            Assert(iovcnt < D1HT_EVENT_MAX_IOVCNT + 1, XEQ_ERROR);

            ssize_t nbytes = 0;

            do { // loop to send msg and get ack
                if (target == G_parms.myIP) break;

                VERBOSEX(stderr, "Sending maintenance msg with TTL=%d to %s:%u. ID=%u. \n", ttl, IPtoP(target), ip2id(target));
                targets++;
                tries = 0;
                rxmited = false;
                ack_received = false;

                KEYID target_id = ip2id(target);

                if (connection >= 0) {
                    D1HT_CLOSE(connection);
                    connection = -1;
                }

                tv_rto = G_rtt_msg.rto();

                gettimeofday(&tv_begin, NULL);

                do {
                    tries++;
                    prev_tv = tv_rto;

                    if (connection < 0) {
                        UDP_CONNECTION(connection, 0, target, XEQ_WARNING);
                        if (connection < 0) continue;
                    }

                    // bellow we will spawn a thread to check if target is still alive
                    //	 			if (tries==2) G_probe.probe(target, __FILE__, __LINE__);

                    ASSERT(tv_rto.tv_sec + tv_rto.tv_usec > 0);
                    ASSERT(prev_tv.tv_sec + prev_tv.tv_usec > 0);

                    if (G_exiting) {
                        if (msg_header.type == D1HT_TTL_0_NOEVENTS) return NULL;
                        msg_header.SeqNo = 0; // if we are exiting, the SeqNo must be 0
                    }

                    XEQ_EXEC(bytes_sent = writev(connection, (iovec *) & iov, iovcnt), XEQ_SILENT);
                    int errsave = errno;

                    if (peer_to_probe.port != 0) {
                        G_probe.probe(peer_to_probe, __FILE__, __LINE__);
                        peer_to_probe.port = 0;
                    }

                    // bellow we are just checking if target is still in our RT
                    if (G_RT.pub_succ(target_id) != target) break;

                    if (bytes_sent < bytes_to_send) {

                        VERBOSE(stderr, "WARNING Cause: failed sending maint msg. Detail: target %s:%u: retcode=%d, bytes_to_send=%u, try=%u, Cause=%s\n", IPtoP(target), bytes_sent, bytes_to_send, tries, strerror(errsave));
                        if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                            D1HT_CLOSE(connection);
                            connection = -1;
                        }
                        continue;
                    } else {

                        struct msg_hd ack_header;

                        VERBOSEX(stderr, "Maintenance msg with %u events, TTL=%d sent to %s:%u, bytes=%u, SeqNo=%u, try=%u. Events=%u,%u,%u,%u\n",
                                tot_events, ttl, IPtoP(target), bytes_sent, msg_header.SeqNo, tries,
                                nevents[0][0], nevents[0][1], nevents[1][0], nevents[1][1]);

                        if (msg_header.type == D1HT_TTL_0_NOEVENTS) { // msg with TTL==0 and no event
                            if (!rxmited) {
                                local_stats.stats[stat_noevt_msgs_sent].count++;
                                local_stats.stats[stat_noevt_bytes_sent].count += (bytes_sent + D1HT_UDP_HEADER_SIZE);
                            } else {
                                local_stats.stats[stat_noevt_msgs_rxmited].count++;
                                local_stats.stats[stat_noevt_bytes_rxmited].count += (bytes_sent + D1HT_UDP_HEADER_SIZE);
                            }
                        } else {
                            if (!rxmited) {
                                local_stats.stats[stat_maint_msgs_sent].count++;
                                local_stats.stats[stat_maint_bytes_sent].count += (bytes_sent + D1HT_UDP_HEADER_SIZE);
                                local_stats.stats[stat_events_sent].count += tot_events;
                            } else {
                                local_stats.stats[stat_maint_msgs_rxmited].count++;
                                local_stats.stats[stat_maint_bytes_rxmited].count += (bytes_sent + D1HT_UDP_HEADER_SIZE);
                            }
                        }
                        rxmited = true;

                        XEQ_EXEC((nbytes = setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, &tv_rto, sizeof (tv_rto))), XEQ_WARNING);
                        if (nbytes == -1) {
                            if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                                D1HT_CLOSE(connection);
                                connection = -1;
                            }
                            continue;
                        }

                        tv_rto = USEC2TV(2.0 * TV2USEC(prev_tv));

                        XEQ_EXEC(nbytes = readn(connection, &ack_header, D1HT_MSG_HEADER_size), XEQ_SILENT);
                        errsave = errno;

                        if (nbytes != D1HT_MSG_HEADER_size && errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                            ack_received = false;
                            break;
                        } else if (nbytes == D1HT_MSG_HEADER_size && ack_header.systemid != G_parms.systemid) {
                            ack_received = false;
                            tries = maxtries + 1;
                            break;
                        } else if (nbytes != D1HT_MSG_HEADER_size && nbytes != -1) {
                            ack_received = false;
                            tries = maxtries + 1;
                            break;
                        } else if (ack_header.type == D1HT_LEAVE && ack_header.SeqNo == D1HT_LEAVE) {
                            ack_received = false;
                            tries = maxtries + 1;
                            G_probe.probed(target);
                            G_RT.pub_del(target, false);
                            if (peer_to_probe == target) peer_to_probe.port = 0;
                            break;
                        } else {
                            ack_received = (nbytes == D1HT_MSG_HEADER_size &&
                                    ack_header.type == msg_header.type &&
                                    ack_header.SeqNo == msg_header.SeqNo);
                        }
                    }
                } while (!ack_received && tries < maxtries);

                D1HT_CLOSE(connection);
                connection = -1;

                if (!ack_received) {
                    if (peer_to_probe.port != 0) G_probe.probe(peer_to_probe, __FILE__, __LINE__);
                    peer_to_probe = target;
                    target = G_RT.pub_succ(target_id + 1);
                }
            } while (!ack_received && targets <= 5);

            if (ack_received) {
                if (tries == 1) {
                    gettimeofday(&tv_end, NULL);
                    G_rtt_msg.update_rtt(tv_begin, tv_end);
                } else {
                    G_rtt_msg.update_rtt(prev_tv);
                }
                G_probe.probed(target);
                VERBOSEX(stderr, "Ack received OK from %s:%u ttl=%u, SeqNo=%u\n", IPtoP(target), msg_header.type, msg_header.SeqNo);
            }

            LOCK(G_stats.stat_mutex);
            G_stats.total.stats[stat_events_sent].count += local_stats.stats[stat_events_sent].count;
            G_stats.total.stats[stat_noevt_msgs_sent].count += local_stats.stats[stat_noevt_msgs_sent].count;
            G_stats.total.stats[stat_noevt_bytes_sent].count += local_stats.stats[stat_noevt_bytes_sent].count;

            G_stats.total.stats[stat_noevt_msgs_rxmited].count += local_stats.stats[stat_noevt_msgs_rxmited].count;
            G_stats.total.stats[stat_noevt_bytes_rxmited].count += local_stats.stats[stat_noevt_bytes_rxmited].count;
            G_stats.total.stats[stat_maint_msgs_sent].count += local_stats.stats[stat_maint_msgs_sent].count;
            G_stats.total.stats[stat_maint_bytes_sent].count += local_stats.stats[stat_maint_bytes_sent].count;

            G_stats.total.stats[stat_maint_msgs_rxmited].count += local_stats.stats[stat_maint_msgs_rxmited].count;
            G_stats.total.stats[stat_maint_bytes_rxmited].count += local_stats.stats[stat_maint_bytes_rxmited].count;
            UNLOCK(G_stats.stat_mutex);

            if (peer_to_probe.port != 0) G_probe.probe(peer_to_probe, __FILE__, __LINE__);
        }

#if defined(DEBUG)
        if (G_parms.selfmsgs) {
            int self_connection;
            selfmsg_t *selfmsg = CALLOC(1, selfmsg_t);
            selfmsg->msg_header = msg_header;
            selfmsg->msg_body = msg_body;

            for (int itype = JOINS; itype <= LEAVES; itype++) {
                selfmsg->ports[itype] = ports[itype];
                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) selfmsg->ips[iport][itype] = ips[iport][itype];
            }
            LOCK(G_selfmsgs_mutex);
            selfmsg->next = G_selfmsgs.next;
            G_selfmsgs.next = selfmsg;
            UNLOCK(G_selfmsgs_mutex);

            UDP_CONNECTION(self_connection, 0, G_parms.myIP, XEQ_WARNING);
            XEQ_EXEC(bytes_sent = writev(self_connection, (iovec *) & iov, iovcnt), XEQ_WARNING);
            D1HT_CLOSE(self_connection);
        } else
#endif
        {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                if (ports[itype] != NULL) FREE(ports[itype]);
                for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                    if (ips[iport][itype] != NULL) FREE(ips[iport][itype]);
                }
            }
        }

        if (bytes_to_send >= D1HT_MTU) {
            int now_time = (int) elapsed_msecs();
            int delta_time = now_time - interval->interval_start_time;

            VERBO(stderr, "WARNING Cause: bytes_to_send >= D1HT_MTU - Detail: bytes_to_send=%d, bytes_sent=%d, T_bytes=%d, E=%d, TTL=%d, iovcnt=%d, target=%s:%u, delta=%d msec, Theta=%d\n",
                    (int) bytes_to_send, (int) bytes_sent, (int) interval->T_bytes, (int) interval->E, (int) ttl, (int) iovcnt, IPtoP(target), delta_time, interval->theta);
        }

        return NULL;
    }

    PTHREAD(send_main_msg_attached) {
        temp_interval_t *temp_interval;

        PTHREAD_INIT;
        PTHREAD_MAYLOCK;
        PTHREAD_PARM_ASSIGN(temp_interval);

        send_maint_msg(temp_interval);

        return NULL;
    }

    /**
     *
     * @param npeers
     * @param mutexed
     * @param exiting
     * @param istimeout
     * @param forced
     * @return
     */
    void *close_interval(const int npeers, const bool mutexed, const bool exiting, const bool istimeout, const bool forced) {

#ifdef DEBUG
        if (mutexed) {
            ASSERT(G_interval_mutex.xeq_locked_byme(), "G_interval_mutex is not locked.");
        } else {
            ASSERT(!G_interval_mutex.xeq_locked_byme(), "G_interval_mutex is locked.");
        }
        if (exiting) {
            ASSERT(G_exiting);
            ASSERT(!istimeout);
        }
        ASSERT(exiting || npeers > 0);
#endif
        if (npeers < 2) {
            if (mutexed) UNLOCK(G_interval_mutex);
            if (!exiting && istimeout) start_interval(D1HT_MAX_THETA);
            return NULL;
        }

        if (!exiting && istimeout) {
            start_interval(D1HT_MIN_THETA);
        }

        int Theta = G_theta.new_theta(npeers);

        if (!mutexed) LOCK(G_interval_mutex);

        if (G_inter_current == NULL) { // we are already exiting
            ASSERT(G_exiting);
            UNLOCK(G_interval_mutex);
            return NULL;
        }

        ASSERT(G_inter_current->SeqNo > 0);
        ASSERT(forced || exiting || istimeout || G_inter_current->E > 0);

        if (!istimeout && G_inter_current->E == 0) {
            if (exiting || G_exiting) G_inter_current = NULL;
            UNLOCK(G_interval_mutex);
            return NULL;
        }

        if (forced || exiting || G_inter_current->should_close(npeers, istimeout, Theta)) {

#if defined(DEBUG)
            VERBOSE(stderr, "Closing interval=%lu, nevents=%u, SeqNo=%u, bigger_ttl=%d, interval_start_time=%d, forced=%d, exiting=%d, G_exiting=%d, npeers=%d, istimeout=%d, Theta=%d\n",
                    (ulong) G_inter_current, (uint) G_inter_current->E, G_inter_current->SeqNo, G_inter_current->bigger_ttl, G_inter_current->interval_start_time,
                    (int) forced, (int) exiting, (int) G_exiting, (int) npeers, (int) istimeout, (int) Theta);

            ASSERT(G_inter_current->bigger_ttl <= D1HT_RHO_MAX);
            ASSERT(!(G_inter_current->bigger_ttl > 0 && G_inter_current->E == 0));
#endif

            interval_t* new_interval = NULL;
            interval_t* interval;

            NUM_8 SeqN = G_inter_current->SeqNo + 1;

            if (exiting || G_exiting) {
                SeqN = 0;
            } else {
                start_interval(Theta);

                if (SeqN == 0) SeqN = 1;
                new_interval = CALLOC(1, interval_t);
                new_interval->interval_init(SeqN, G_inter_current->interval_start_time, istimeout);
            }

            interval = G_inter_current;
            G_inter_current = new_interval;
            UNLOCK(G_interval_mutex);

#if defined(DEBUG)
            uint tot_event_count = 0;
            for (events_t *next_event = interval->events; next_event != NULL; next_event = next_event->next) tot_event_count++;
            if (tot_event_count != ((uint) interval->E)) EXIT("Wrong event (E) count. Detail: E=%u, event count=%u", (uint) interval->E, tot_event_count);
#endif

            interval->theta = Theta;
            ASSERT(Theta > 0);

            temp_interval_t *temp_interval = CALLOC(D1HT_RHO_MAX + 1, temp_interval_t);

            pthread_t ptid[D1HT_RHO_MAX + 1];

            interval->bigger_ttl = MIN((int) interval->bigger_ttl, G_RT.pub_rho());

            G_RT.get_targets(temp_interval, interval->bigger_ttl);

            NUM_32 last_ttl;
            if (interval->bigger_ttl == 0)
                last_ttl = 0;
            else
                last_ttl = interval->bigger_ttl - 1;

            bool local_exiting = false;

            for (int ttl = last_ttl; ttl > 0; ttl--) {

                if (temp_interval[ttl].target.port == 0) {
                    ptid[ttl] = 0;
                    continue;
                }

                VERBOSE(stderr, "Creating thread to send msg with ttl=%u to %s:%u\n", ttl, IPtoP(temp_interval[ttl].target));

                temp_interval[ttl].interval = interval;
                temp_interval[ttl].ttl = ttl;

                ptid[ttl] = PTHREAD_TEST_CREATE(send_main_msg_attached, &(temp_interval[ttl]), &G_joinable, XEQ_WARNING);

                if (ptid[ttl] <= 0) {
                    VERBO(stderr, "ERROR Cause: Problem creating thread - I will exit!\n");
                    G_exiting = true;
                    local_exiting = true;
                    ttl++;
                    mysleep_msecs(100);
                    EXIT("Unable to create pthread");
                }
            }

            ASSERT(G_inter_current == NULL || !G_interval_mutex.xeq_locked_byme(), "G_interval_mutex is locked.");

            if ((local_exiting || exiting) && interval->E == 0) temp_interval[0].target.port = 0;

            if (temp_interval[0].target.port != 0) {
                temp_interval[0].interval = interval;
                temp_interval[0].ttl = 0;
                send_maint_msg(&(temp_interval[0]));
            }

            if (local_exiting) {
                G_exiting = false; // To avoid generating a "Secondary ERROR" msg
                EXIT("Problem creating thread");
            }

            if (!exiting) {

                if (istimeout) {
                    G_RT.stabilize_close_interval();
                    G_joining_peers.close_interval();
                    G_probe.probe_close_interval(Theta);
                }

                if (interval->E > 0) {
                    G_stats.total.stats[stat_E_realised].update_avg(((double) interval->E));
                    ;
                }

                for (int ttl = last_ttl; ttl > 0; ttl--) {
                    if (ptid[ttl] != 0) PTXEQ(pthread_join(ptid[ttl], NULL), XEQ_WARNING);
                }

                FREE_DESTROY(interval);
                FREE(temp_interval);
            }

        } else {
            UNLOCK(G_interval_mutex);
            if (!exiting && istimeout) {
                G_RT.stabilize_close_interval();
                G_joining_peers.close_interval();
                G_probe.probe_close_interval(Theta);
            }
        }
        return NULL;
    }

    void *close_interval(const int npeers, const bool mutexed, const bool exiting, const bool istimeout) {
        return close_interval(npeers, mutexed, exiting, istimeout, false);
    }

    void *close_interval(const bool mutexed, const bool exiting, const bool istimeout, const bool forced) {
        int npeers = 0;
        if (!exiting) npeers = G_RT.pub_npeers();
        return close_interval(npeers, mutexed, exiting, istimeout, forced);
    }

    void *close_interval(const bool mutexed, const bool exiting, const bool istimeout) {
        return close_interval(mutexed, exiting, istimeout, false);
    }

    void *close_interval() {
        return close_interval(false, false, true);
    }

    PTHREAD(close_interval_exit) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        close_interval(false, true, false);

        for (int i = 0; (!G_exited && i < 10); i++) {
            mysleep(1);
        }

        Exit(EXIT_SUCCESS);

        return NULL;
    }

    bool interval_t::should_close(const int np, const bool istimeout, const int Theta) {
#ifdef DEBUG
        ASSERT(G_interval_mutex.xeq_locked_byme(), "G_interval_mutex is not locked.");
#endif

        if (G_exiting) {
            if (E == 0) return false;
            return true;
        }

        // we may not risk to overflow our MTU
        if (T_bytes > (D1HT_MTU / (2 + (int) bigger_ttl))) {
            return true;
        }

        // we may not risk to overflow our event counters in the msg_hd
        if (E > (NUM_32) (500 / (2 + (int) bigger_ttl))) {
            return true;
        }

        if (!istimeout && np < 3 && E == 0) return false;

        int now_time = (int) elapsed_msecs();
        int delta_time = now_time - interval_start_time;

        if (delta_time < 0) { // it should never happen, but it sometimes occurs in planetlab
            VERBOSE(stderr, "WARNING Cause: delta_time < 0. Detail: delta=%d, now=%d, start=%d\n", delta_time, now_time, interval_start_time);
            return true;
        }

        NUM_32 E_tried = (NUM_32) G_theta.E_MAX(np);
        NUM_32 rho_now = (NUM_32) G_RT.rho(np);

        if (delta_time < D1HT_MIN_THETA) {
            if (E == 0) return false;
            // we've just closed another interval
            if (E > E_tried || bigger_ttl >= rho_now) start_interval(D1HT_MIN_THETA);
            return false;
        } else if (delta_time > Theta) {
            G_stats.total.stats[stat_theta_xceeded].count++;
            G_stats.total.stats[stat_theta_xceeded].value += (double) delta_time;
            // we can not risk to stay more than Theta secs without closing an interval
            if (E > 0) return true;
        }

        if (E > E_tried) {
            // we can not accumulate more than E_MAX events before flushing the events buffer (see the Globecom 2009 paper)
            return true;
        }

        VERBOSEX(stderr, "should close? istimeout=%u, Theta=%d, delta_time=%d, E=%u, E_MAX=%u, bigger_ttl=%u\n",
                istimeout, Theta, delta_time, E, G_theta.E_MAX(np), bigger_ttl);

        if (E > 0) {

            if (bigger_ttl >= rho_now) {
                // let's forward the event with our predecessor
                return true;
            }

            if ((time_first_event > 0) && (now_time - time_first_event) >= (Theta - D1HT_MIN_THETA)) {
                // no event can be hold for more than Theta secs before being rxmited, and the next inteval should take at least D1HT_MIN_THETA msecs
                return true;
            }

            if (delta_time >= (Theta - D1HT_MIN_THETA)) {
                // we can not risk to stay more than Theta secs without closing an interval
                return true;
            }
        }

        int probed_delta;

        if (!G_probe.xeq_succ_probed(Theta, &probed_delta)) {
            // we can not risk to stay more than Theta msecs without sending a msg to our successor
            if (E == 0) {
                G_stats.total.stats[stat_theta_noevt].count++;
                G_stats.total.stats[stat_theta_noevt].value += (double) delta_time;
            }
            return true;
        }

        ASSERT(probed_delta >= 0);
        ASSERT(Theta > probed_delta);

        if ((Theta - probed_delta) > D1HT_MIN_THETA / 10) {
            start_interval(Theta - probed_delta);
            return false;
        } else {
            if (E == 0) {
                G_stats.total.stats[stat_theta_noevt].count++;
                G_stats.total.stats[stat_theta_noevt].value += (double) delta_time;
            }
            return true;
        }
    }

} // namespace d1ht
