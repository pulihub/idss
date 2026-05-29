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
 * @file join.c
 * @brief gestione del join di un nodo
 */
#include <sys/wait.h>
#include "D1HT.h"

namespace d1ht {


    extern join_t G_joining_peers;

    ttls_received_t G_ttls_received;
    int G_forward_connection = -1;

#ifdef D1HT_JOIN_COPY_RT
    int g_rt_skip_copy = 0;
    int g_rt_skip_next = 1;
#endif

    bool routing_table::pub_send(const struct msg_hd msg_header, int *connection) {

        Assert(msg_header.systemid == 0 || msg_header.systemid == G_parms.systemid, XEQ_ERROR);

        IPaddr temp_joining_IP;
        struct D1HT_JOIN_REPLY_body_part1 msg_body_reply_1;
        BZERO(msg_body_reply_1);
        struct D1HT_JOIN_REPLY_body_part2 msg_body_reply_2;
        BZERO(msg_body_reply_2);

        temp_joining_IP = GET_SENDER(*connection, XEQ_WARNING);

        if (temp_joining_IP.port == 0) return false;

        ASSERT(temp_joining_IP.port != msg_header.sender_port && temp_joining_IP.port != G_parms.default_udp_port);

        if (msg_header.sender_port)
            temp_joining_IP.port = msg_header.sender_port;
        else
            temp_joining_IP.port = G_parms.default_udp_port;

        verbose(stderr, "Peer asking to join. Detail: peer=%s:%u\n", IPtoP(temp_joining_IP));

        msg_body_reply_1.id = ip2id(temp_joining_IP);

        if (!Assert((msg_body_reply_1.id != G_myID), XEQ_WARNING)) return false;

        LOCK(rt_mutex);

        msg_body_reply_1.succ = succ(msg_body_reply_1.id);

        if (msg_body_reply_1.succ == temp_joining_IP) { // I should have missed its leave notification
            VERBOSEX(stderr, "Peer seems to be rejoining. Detail: peer=%s:%u (id=%u)\n", IPtoP(temp_joining_IP), msg_body_reply_1.id);
            del(msg_body_reply_1.id);
            msg_body_reply_1.succ = succ(msg_body_reply_1.id);
        }

        UNLOCK(rt_mutex);

        ASSERT(msg_body_reply_1.succ != temp_joining_IP);
        ASSERT(ntohs(msg_body_reply_1.succ.port) != 0);

        D1HT_LOOKUP_reply_body reply_body;
        D1HT_LOOKUP_body lookup_body;
        BZERO(lookup_body);

        lookup_body.prev_target.port = 0;
        lookup_body.id = msg_body_reply_1.id;
        reply_body = lookup(&lookup_body);
        int errsave = errno;

#if defined(DEBUG)
        if (ntohs(reply_body.home.port) == 0 && (reply_body.hops < 10)) {
            // pub_print_table();
            EXIT("Invalid IP returned. Detail: %s:%u", IPtoP(reply_body.home));
        }
#endif

        if (errsave != 0) {
            verbose(stderr, "WARNING Cause: Lookup join failed, msg=%s. Detail: peer %s:%u\n", strerror(errsave), IPtoP(temp_joining_IP));
            mysleep(1);
            return false;
        } else if ((temp_joining_IP == reply_body.home) || (ntohs(reply_body.home.port) == 0)) {
            verbose(stderr, "WARNING Cause: Invalid join lookup. Detail: peer %s:%u\n", IPtoP(temp_joining_IP));
            mysleep(1);
            return false;
        } else if (reply_body.hops < 10) {
            msg_body_reply_1.succ = reply_body.home;
        } else {
            verbose(stderr, "WARNING Cause: Lookup join failed. Detail: peer %s:%u\n", IPtoP(temp_joining_IP));
            mysleep(1);
            return false;
        }

        KEYID join_succID = ip2id(msg_body_reply_1.succ);

        ASSERT(D1HT_DEFAULT_PORT == ntohs(G_parms.default_udp_port));

        if (join_succID == G_myID) {

            ASSERT(msg_body_reply_1.succ == G_parms.myIP);

            msg_body_reply_1.busy_or_err = !G_joining_peers.join_start(temp_joining_IP);

            ssize_t nbytes;

            msg_body_reply_2.e_rate = (NUM_32) (100.0 * G_theta.erate()); // we can not do this while locking rt_mutex

            LOCK(rt_mutex);

            //      check if I still am the right succ after calling G_joining_peers.join_start

            msg_body_reply_1.succ = succ(msg_body_reply_1.id);
            join_succID = ip2id(msg_body_reply_1.succ);
            msg_body_reply_1.next = succ(join_succID + 1);

            if (msg_body_reply_1.busy_or_err || join_succID != G_myID) {
                UNLOCK(rt_mutex);
                nbytes = SEND_MSG(*connection, D1HT_JOIN_REPLY, msg_body_reply_1, XEQ_WARNING);
                G_joining_peers.join_abort();
                if (join_succID != G_myID) G_probe.force_probe(msg_body_reply_1.succ, __FILE__, __LINE__);
                return true;
            } else {
                // I am the actual successor and I am not busy, so send the RT

                msg_body_reply_2.Npeers = (NUM_32) npeers();

#ifdef DEBUG
                if (G_parms.check) xeq_table(__FILE__, __LINE__);
#endif
                struct timeval begin, end;
                float copy_time = 0.0;
                bool RT_locked = true;

                bucket_t *table_send = table;
                unsigned char *bucket_peers_send = bucket_peers;

                gettimeofday(&begin, NULL);

#ifdef D1HT_JOIN_COPY_RT
                if (g_rt_skip_copy <= 0) {
                    memcpy(table_copy, table, (rt_size * sizeof (bucket_t)));
                    memcpy(bucket_peers_copy, bucket_peers, (rt_size * sizeof (unsigned char)));

                    UNLOCK(rt_mutex);

                    table_send = table_copy;
                    bucket_peers_send = bucket_peers_copy;

                    RT_locked = false;

                    struct timeval end_copy;
                    gettimeofday(&end_copy, NULL);

                    copy_time = TVDIFF2MSEC(begin, end_copy);

                    G_stats.total.stats[stat_rt_copy].value += copy_time;
                    G_stats.total.stats[stat_rt_copy].count++;

                } else {
                    g_rt_skip_copy--;
                }
#endif
                nbytes = SEND_MSG(*connection, D1HT_JOIN_REPLY, msg_body_reply_1, XEQ_WARNING);

                if (nbytes < 0 || G_exiting) {
                    if (RT_locked) UNLOCK(rt_mutex);
                    G_joining_peers.join_abort();
                    return false;
                }

                msg_body_reply_2.NMAX = (NUM_32) G_parms.NMAX;
                msg_body_reply_2.f = (NUM_32) G_parms.f;
                msg_body_reply_2.bucketsize = (NUM_32) BUCKETSIZE;
                msg_body_reply_2.i_srtt_msg = (NUM_32) G_rtt_msg.rtt();
                msg_body_reply_2.i_srtt_lookup = (NUM_32) G_rtt_lookup.rtt();
                msg_body_reply_2.i_rttvar_msg = (NUM_32) G_rtt_msg.rttv();
                msg_body_reply_2.i_rttvar_lookup = (NUM_32) G_rtt_lookup.rttv();
                msg_body_reply_2.default_port = (PORTNO) G_parms.default_udp_port;
                msg_body_reply_2.systemid = (KEYID) G_parms.systemid;

                nbytes = SEND_MSG(*connection, D1HT_JOIN_REPLY, msg_body_reply_2, XEQ_WARNING);

                if (nbytes < 0 || G_exiting) {
                    if (RT_locked) UNLOCK(rt_mutex);
                    G_joining_peers.join_abort();
                    return false;
                }

                XEQ_EXEC(nbytes = writen(*connection, (const void*) table_send, rt_size * sizeof (bucket_t)), XEQ_WARNING);
                errsave = errno;

                if (G_exiting || (nbytes != (ssize_t) (rt_size * sizeof (bucket_t)))) {
                    if (RT_locked) UNLOCK(rt_mutex);
                    G_joining_peers.join_abort();
                    return false;
                }

                XEQ_EXEC(nbytes = writen(*connection, (const void*) bucket_peers_send, rt_size * sizeof (unsigned char)), XEQ_WARNING);
                errsave = errno;

                if (RT_locked) UNLOCK(rt_mutex);

                if (G_exiting || (nbytes != (ssize_t) (rt_size * sizeof (unsigned char)))) {
                    G_joining_peers.join_abort();
                    return false;
                }

                gettimeofday(&end, NULL);

                float send_time = TVDIFF2MSEC(begin, end);

                G_stats.total.stats[stat_rt_send].value += send_time;
                G_stats.total.stats[stat_rt_send].count++;

#ifdef D1HT_JOIN_COPY_RT
                if (copy_time > (0.3 * send_time)) {

                    g_rt_skip_copy = g_rt_skip_next * ((int) (copy_time / (0.25 * send_time)));
                    g_rt_skip_next = 2 * g_rt_skip_next;

                    verbose(stderr, "WARNING Cause: Big RT copy time. Detail: npeers=%u, copy_time=%.3f msecs, send_time=%.3f msecs, g_rt_skip_next=%u\n",
                            (unsigned) msg_body_reply_2.Npeers, copy_time, send_time, g_rt_skip_next);
                }
#endif
                verbose(stderr, "Routing table sent to peer. Detail: joining peer=%s:%u, npeers=%u, copy_time=%.3f msecs, send_time=%.3f msecs\n",
                        IPtoP(temp_joining_IP), (unsigned) msg_body_reply_2.Npeers, copy_time, send_time);

                msg_hd msg_header;

                do {
                    msg_header = READ_HEADER(*connection, XEQ_WARNING);

                } while (msg_header.type == D1HT_JOIN_REQUEST);

                if (msg_header.type != D1HT_JOIN_FINISHED) {
                    G_joining_peers.join_abort();
                    return false;
                }

                // JOIN FINISHED OK!!!
                LOCK(rt_mutex);
                stabilized(temp_joining_IP);
                add(temp_joining_IP);
                UNLOCK(rt_mutex);
                G_joining_peers.join_complete(temp_joining_IP, pub_rho());
                G_probe.probed(temp_joining_IP);
                verbose(stderr, "Finished join. Detail: joining peer=%s:%u, npeers=%u\n", IPtoP(temp_joining_IP), (msg_body_reply_2.Npeers + 1));
                ack_event_pred(temp_joining_IP, JOINS);
                //	  	  	  	  		ack_event(temp_joining_IP, D1HT_RHO_MAX, JOINS);
                return true;
            }
        } else {
            msg_body_reply_1.busy_or_err = false;

            G_probe.force_probe(msg_body_reply_1.succ, __FILE__, __LINE__);

            LOCK(rt_mutex);
            msg_body_reply_1.succ = succ(msg_body_reply_1.id);
            msg_body_reply_1.next = succ(ip2id(msg_body_reply_1.succ) + 1);
            if (npeers() > 4 && msg_body_reply_1.succ == msg_body_reply_1.next) {
                print_table();
                UNLOCK(rt_mutex);
                EXIT("joining with msg_body_reply_1.succ == msg_body_reply_1.next. Detail: succ=%s:%u, next=%s:%u, join_succID=%lu",
                        IPtoP(msg_body_reply_1.succ), IPtoP(msg_body_reply_1.next), (long unsigned int) join_succID);
            }
            uint local_npeers = (uint) npeers();
            UNLOCK(rt_mutex);

            SEND_MSG(*connection, D1HT_JOIN_REPLY, msg_body_reply_1, XEQ_WARNING);
            if (G_parms.verbose) {
                verbose(stderr, "Join forwarded. Detail: peer %s:%u join forwarded to %s:%u", IPtoP(temp_joining_IP), IPtoP(msg_body_reply_1.succ));
                fprintf(stderr, " and %s:%u, npeers=%u\n", IPtoP(msg_body_reply_1.next), local_npeers);
            }

            return true;
        }
    }

#define	TIMEOUT_MICROSECS 	000000
#define	TIMEOUT_SECS 		2

    bool routing_table::get() {
        static int connection = -1;
        static uint timeouts = 0;
        ssize_t nbytes;

        D1HT_JOIN_REPLY_body_part1 msg_body_reply_1;
        D1HT_JOIN_REPLY_body_part2 msg_body_reply_2;

        bool retcode = false;

        if (connection >= 0) {
            D1HT_CLOSE(connection);
            connection = -1;
        }

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling RT get.");

        IPaddr IPreq = G_known_peers.top();
        G_known_peers.pop();

        if (IPreq.port == 0 || IPreq.port == ntohs(0)) IPreq.port = G_parms.default_udp_port;
        if (IPreq == G_parms.myIP) return false;

        verbose(stderr, "Contacting peer to get the routing table. Detail: peer %s:%d, ID=%u, G_myID=%u\n", IPtoP(IPreq), ip2id(IPreq), G_myID);

        struct timeval tv;
        msg_hd msg_header;

        int errsave;
        int tries = 0;
        int maxtries = 3;

        connection = -1;

        do {
            mysleep_msecs(tries);

            tries++;

            msg_header.type = D1HT_INVALID;

            if (connection < 0) {
                TCP_CONNECTION(connection, 0, IPreq, XEQ_SILENT);
                errsave = errno;

                if (connection < 0) {
                    VERBOSE(stderr, "WARNING Cause: Failed to connect to ask RT. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
                    D1HT_CLOSE(connection);
                    connection = -1;
                    return false;
                    ;
                }
            }

            tv.tv_sec = TIMEOUT_SECS * tries;
            tv.tv_usec = TIMEOUT_MICROSECS;
            XEQ_EXEC(setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)), XEQ_WARNING);

            tv.tv_sec = TIMEOUT_SECS * tries;
            tv.tv_usec = TIMEOUT_MICROSECS;
            XEQ_EXEC(setsockopt(connection, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof (tv)), XEQ_WARNING);

            nbytes = SEND_HEADER(connection, D1HT_JOIN_REQUEST, XEQ_SILENT); // ask for the RT
            errsave = errno;

            if (nbytes <= 0) {
                VERBOSE(stderr, "WARNING Cause: Failed to send request RT msg. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
                D1HT_CLOSE(connection);
                connection = -1;
                continue;
            }

            msg_header = READ_HEADER_TYPE(connection, D1HT_JOIN_REPLY, XEQ_SILENT);
            errsave = errno;

            if (G_exiting) {
                D1HT_CLOSE(connection);
                return false;
            }

            if (msg_header.type == D1HT_INVALID) {
                VERBOSE(stderr, "WARNING Cause: Failed to read RT msg header. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
                if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                    D1HT_CLOSE(connection);
                    connection = -1;
                } else {
                    timeouts++;
                }
            }

        } while (msg_header.type == D1HT_INVALID && tries < maxtries);

        if (msg_header.type == D1HT_INVALID) {
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        nbytes = READ_BODY(connection, msg_body_reply_1, XEQ_WARNING);
        errsave = errno;

        if (nbytes <= 0) {
            VERBOSE(stderr, "WARNING Cause: Failed to read RT msg body. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        if (ntohs(msg_body_reply_1.succ.port) == 0) {
            VERBOSE(stderr, "WARNING Cause: Invalid successor IP address. Detail peer=%s:%u\n", IPtoP(IPreq));
            G_known_peers.push(IPreq);
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        if (msg_body_reply_1.succ != IPreq) {
            D1HT_CLOSE(connection);
            G_known_peers.push(IPreq);
            G_known_peers.push(msg_body_reply_1.next);
            G_known_peers.push(msg_body_reply_1.succ);
            verbose(stderr, "Join forwarded. Detail: to %s:%u and %s:%u\n", IPtoP(msg_body_reply_1.succ), IPtoP(msg_body_reply_1.next));
            return false;
        }

        G_known_peers.push(msg_body_reply_1.next);
        G_known_peers.push(IPreq);

        if (msg_body_reply_1.busy_or_err) {
            D1HT_CLOSE(connection);
            int sleep_busy_join = 5 + (int) (G_parms.NMAX / 1000);
            verbose(stderr, "My join was postponed. Detail: Postponed by %s:%u, retrying in %u seconds. Please wait.\n", IPtoP(IPreq), sleep_busy_join);
            mysleep(sleep_busy_join);
            return false;
        }

        nbytes = READ_MSG(connection, D1HT_JOIN_REPLY, msg_body_reply_2, XEQ_WARNING); // receive reply with NMAX, etc..
        errsave = errno;

        if (nbytes <= 0) {
            VERBOSE(stderr, "WARNING Cause: Failed to read RT msg body. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        Assert(msg_body_reply_2.bucketsize == (NUM_32) BUCKETSIZE, XEQ_ERROR, "bucket sizes do not match!");

        ASSERT(D1HT_DEFAULT_PORT == ntohs(G_parms.default_udp_port))

        if (msg_body_reply_2.default_port != (NUM_32) G_parms.default_udp_port) {
            EXIT("Default ports do not match. Detail: my default port=%u, system default port=%u\n", ntohs(msg_body_reply_2.default_port), ntohs(G_parms.default_udp_port));
        }

        if (G_myID != msg_body_reply_1.id) {
            D1HT_CLOSE(connection);
            return false;
        }

        npeers((int) msg_body_reply_2.Npeers);

        G_parms.NMAX = (int) msg_body_reply_2.NMAX;
        G_parms.rho_max = (int) log2((float) G_parms.NMAX);
        G_parms.f = msg_body_reply_2.f;
        G_parms.systemid = msg_body_reply_2.systemid;

        if (msg_body_reply_2.e_rate > 0) {
            float erate = ((float) msg_body_reply_2.e_rate) / 100.0;
            G_theta.update_erate(erate);
        }

        // receive the RT from the successor
        if (!G_exiting && RT_init(connection) && G_ttls_received.start_notify(IPreq) && !G_exiting) {

            nbytes = SEND_HEADER(connection, D1HT_JOIN_FINISHED, XEQ_WARNING);
            errsave = errno;

            if (nbytes <= 0) {
                G_ttls_received.stop_notify(__FILE__, __LINE__);
                VERBOSE(stderr, "WARNING Cause: Failed to send D1HT_JOIN_FINISHED msg. Detail peer=%s:%u, msg=%s\n", IPtoP(IPreq), strerror(errsave));
            } else {
                // As soon as we make G_started = true we will start acceping all TCP connections. We should make G_started = true just after sending the
                // D1HT_JOIN_FINISHED otherwise we may deny PROBE and other connections, which will make other peers thing that we are not alive
                G_started = true;

                G_rtt_msg.rtt_init(msg_body_reply_2.i_srtt_msg, msg_body_reply_2.i_rttvar_msg);
                G_rtt_lookup.rtt_init(msg_body_reply_2.i_srtt_lookup, msg_body_reply_2.i_rttvar_lookup);

                verbose(stderr, "Finished receiving the Routing table. Detail: my sucessor=%s:%u, npeers=%u, systemid=%u, timeouts=%u\n",
                        IPtoP(IPreq), npeers(), G_parms.systemid, timeouts);

                IPaddr finger = pred_IP();
                KEYID id = pred_ID();
                int RHO = rho();
                int offset = 1;

                for (int i = 0; i < RHO; i++) {

                    stabilize(finger, __FILE__, __LINE__);
                    finger = predn(id, offset);
#if defined(DEBUG)
                    if (id == ip2id(finger)) {
                        print_table();
                        xeq_table(__FILE__, __LINE__);
                        EXIT("id == ip2id(finger). Detail: id=%lu, finger=%s:%u\n", (long unsigned int) id, IPtoP(finger));
                    }
#endif
                    id = ip2id(finger);
                    offset = 2 * offset;
                }

                G_stats.total.stats[stat_joins].count = 1;
                retcode = true;

            }
        } else {
            if (G_forward_connection >= 0) {
                D1HT_CLOSE(G_forward_connection);
                G_forward_connection = -1;
            }
            G_ttls_received.stop_notify(__FILE__, __LINE__);
        }

        D1HT_CLOSE(connection);

        return retcode;
    }

} // namespace d1ht
