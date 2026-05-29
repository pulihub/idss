/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             *
 *  No title to or ownership of D1HT is hereby transferred.	             *
 *									     *
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
 * @file UDP_server.c
 * @brief gestione della segnalazione over UDP
 */
#include <sys/wait.h>

#include "D1HT.h"

namespace d1ht {

    int get_udp_socket(const PORTNO port) {
        int severity = XEQ_SILENT;
        ;
        int retcode = -1;

        if (G_exiting) {
            mysleep(10);
            Exit(EXIT_SUCCESS);
        }

        for (int tries = 0; retcode <= 0; tries++) {
            mysleep(tries);
            if (tries == G_parms.retries) severity = XEQ_ERROR;
            UDP_SOCKET(retcode, port, severity);
        }

        if (retcode < 0) EXIT("Unable to get UDP  port. Detail: port=%u", ntohs(port));

        return retcode;
    }

    PTHREAD(event_msg_handler) {

        PTHREAD_DETACH;

        if (G_exiting) pthread_exit(NULL);

        PTHREAD_INIT;

        eventmsg_t* eventmsg;
        PTHREAD_PARM_ASSIGN(eventmsg);

        PTHREAD_MAYLOCK;

        G_RT.ack_msg((eventmsg_t*) eventmsg);

        FREE_DESTROY(eventmsg);

        pthread_exit(NULL);
    }

    PTHREAD(UDP_server) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        char dummy_buffer[D1HT_MTU];
        struct msghdr dummy_msg;
        BZERO(dummy_msg);
        struct cmsghdr dummy_cmsg;
        BZERO(dummy_cmsg);
        struct msg_hd dummy_msg_header;
        struct sockaddr_in dummy_sender_address;
        struct iovec dummy_iov[2];
        msg_hd dummy_header;

        dummy_header.systemid = G_parms.systemid;
        dummy_header.type = D1HT_LEAVE;
        dummy_header.SeqNo = D1HT_LEAVE;

        dummy_msg.msg_name = (void *) &dummy_sender_address;
        dummy_msg.msg_iov = dummy_iov;
        dummy_msg.msg_iovlen = 2;
        dummy_msg.msg_control = &dummy_cmsg;

        dummy_iov[0].iov_base = (void *) &dummy_msg_header;
        dummy_iov[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        dummy_iov[1].iov_base = (void *) dummy_buffer;
        dummy_iov[1].iov_len = (size_t) D1HT_MTU;

        while (!G_started) { // this is just to assure that our join has already finished
            mysleep_msecs(100);
            LOCK(G_RT.rt_mutex);
            UNLOCK(G_RT.rt_mutex);
        }

        VERBOSE(stderr, "Listening to maintenance messages\n");

        while (!G_exiting) {

            eventmsg_t *eventmsg = CALLOC(1, eventmsg_t);

            // We should read the whole msg quickly, and then spawn a thread to treat it, in order to be free to receive more msgs
            if (eventmsg->eventmsg_receive(G_UDP_socket) && !G_exiting) {
                //		msg arrived ok and complete, so creates a thread to add the msg to maintenance, and send the reply (ack)
                PTHREAD_CREATE(event_msg_handler, eventmsg, &G_detached);
            } else if (!G_exiting) {
                FREE_DESTROY(eventmsg);
            }
        }

        while (!G_exited) {
            dummy_msg.msg_flags = 0;
            dummy_msg.msg_namelen = sizeof (dummy_sender_address);
            dummy_msg.msg_controllen = sizeof (dummy_cmsg);
            if (recvmsg(G_UDP_socket, &dummy_msg, 0) < (ssize_t) D1HT_MSG_HEADER_size) break;
            sendto(G_UDP_socket, (void *) &(dummy_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(dummy_sender_address), sizeof (dummy_sender_address));
        }

        D1HT_CLOSE(G_UDP_socket);
        G_UDP_socket = -1;
        G_UDP_thread_id = 0;
        return NULL;
    }

    PTHREAD(UDP_lookup_server) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        ssize_t nbytes;

        struct msghdr msg;
        struct cmsghdr cmsg;
        struct iovec iov[3];
        struct sockaddr_in sender_address;

        msg_hd lookup_req_header;
        D1HT_LOOKUP_body lookup_req_body;

        BZERO(msg);
        msg.msg_name = (void *) &sender_address;
        msg.msg_iov = iov;
        msg.msg_iovlen = 3;
        msg.msg_control = &cmsg;

        ssize_t bytes_to_receive = 0;

        iov[0].iov_base = (void *) &lookup_req_header;
        iov[0].iov_len = sizeof (msg_hd);
        bytes_to_receive += (ssize_t) iov[0].iov_len;

        iov[1].iov_base = (void *) &lookup_req_body;
        iov[1].iov_len = sizeof (D1HT_LOOKUP_body);
        bytes_to_receive += (ssize_t) iov[1].iov_len;

        void* payload_buff = CALLOC(D1HT_MAX_PAYLOAD_SIZE, char);
        iov[2].iov_base = (void *) payload_buff;
        iov[2].iov_len = D1HT_MAX_PAYLOAD_SIZE;

        int max_payload_size = D1HT_MTU - (D1HT_UDP_HEADER_SIZE * 2 + bytes_to_receive);

        if (D1HT_MAX_PAYLOAD_SIZE > max_payload_size)
            EXIT("D1HT_MAX_PAYLOAD_SIZE value (%d) is too big - it should be smaller than %d", D1HT_MAX_PAYLOAD_SIZE, max_payload_size);

#if defined(DEBUG)
        IPaddr target_UDP;
        target_UDP.ip = G_parms.myIP.ip;
        target_UDP.port = G_lookup_port;
        VERBOSE(stderr, "UDP_lookup_server started. Target lookup IP=%s:%u\n", IPtoP(target_UDP));
#endif

        msg_hd nack_header;
        nack_header.type = D1HT_INVALID;
        nack_header.systemid = G_parms.systemid;

        msg_hd dummy_header;
        dummy_header.type = D1HT_LEAVE;
        dummy_header.SeqNo = D1HT_LEAVE;
        dummy_header.systemid = G_parms.systemid;

        while (!G_exited) {

            msg.msg_flags = 0;
            msg.msg_namelen = sizeof (struct sockaddr_in);
            msg.msg_controllen = sizeof (cmsg);

            XEQ_EXEC(nbytes = recvmsg(G_lookup_socket, &msg, 0), XEQ_WARNING);

            int errsave = errno;

            if (nbytes < 0 && errsave != EINTR) {
                if (G_exiting) break;
                D1HT_CLOSE(G_lookup_socket);
                G_lookup_socket = -1;
                verbose(stderr, "WARNING Cause: reopening lookup UDP port. Detail: port=%d, msg: %s\n", G_lookup_port, strerror(errsave));
                G_lookup_socket = get_udp_socket(G_lookup_port);
                continue;
            }

            if (nbytes <= 0) continue;

            if (!G_started || G_exiting) {
                sendto(G_lookup_socket, (void *) &(dummy_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));
                continue;
            }

            VERBOSE(stderr, "bytes_to_receive=%d, nbytes=%d, lookup_req_header.type=%d, payload_len=%d\n", (int) bytes_to_receive, (int) nbytes,
                    (int) lookup_req_header.type, (int) lookup_req_body.payload_len);

            if (nbytes < bytes_to_receive ||
                    lookup_req_header.type != D1HT_LOOKUP ||
                    lookup_req_header.systemid != G_parms.systemid ||
                    lookup_req_body.payload_len > D1HT_MAX_PAYLOAD_SIZE ||
                    nbytes != (ssize_t) (bytes_to_receive + lookup_req_body.payload_len)) {

                sendto(G_lookup_socket, (void *) &(nack_header), D1HT_MSG_HEADER_size, 0, (struct sockaddr *) &(sender_address), sizeof (sender_address));

                continue;
            }

            IPaddr sender;
            sender.ip = sender_address.sin_addr;
            sender.port = lookup_req_header.sender_port;

            VERBOSE(stderr, "Lookup received from %s:%u for id=%.10u, payload_len=%d\n", IPtoP(sender), lookup_req_body.id, (int) lookup_req_body.payload_len);

            G_RT.lookup_local(&lookup_req_header, &lookup_req_body, G_lookup_socket, &sender, &sender_address, payload_buff);
        }
        D1HT_CLOSE(G_lookup_socket);
        G_lookup_socket = -1;
        return NULL;
    }

} // namespace d1ht
