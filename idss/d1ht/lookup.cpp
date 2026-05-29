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
 * @file lookup.c
 * @brief Implementazione delle funzioni di libreria in <e>D1HT.h</e>

 */
#include "D1HT.h"
#include <sys/uio.h>

namespace d1ht {

    rtt_info G_rtt_lookup;

    void *g_reply_payload = NULL;
    char *g_dummy_buff = NULL;

    struct D1HT_LOOKUP_reply_body routing_table::lookup(D1HT_LOOKUP_body *lookup_bodyp) {
        return lookup(lookup_bodyp, NULL, NULL);
    }

    struct D1HT_LOOKUP_reply_body routing_table::lookup(D1HT_LOOKUP_body *lookup_bodyp, void* payload, void* reply_payload) {

        ASSERT(lookup_bodyp->payload_len == 0 || payload != NULL);
        ASSERT(lookup_bodyp->max_reply_payload_len == 0 || reply_payload != NULL);

        struct timeval lookup_begin, lookup_end;
        gettimeofday(&lookup_begin, NULL);

        D1HT_LOOKUP_reply_body retcode;
        BZERO(retcode);

        IPaddr peer_to_add;
        peer_to_add.port = 0;
        IPaddr peer_to_probe;
        BZERO(peer_to_probe);

        ssize_t bytes_sent, bytes_to_send = 0, bytes_rcvd = 0;

        timeval tv_rto, prev_tv;

        //HPC Lab: cerca il successore dell'id in input
        IPaddr target = pub_succ(lookup_bodyp->id);

        if (target == G_parms.myIP) {
            //sono io: lookup terminata
            gettimeofday(&lookup_end, NULL);

            double latency = TVDIFF2MSEC(lookup_begin, lookup_end);

            G_stats.total.stats[stat_LookLatency].add_time(latency);
            G_stats.total.stats[stat_lookups_total].count++;
            G_stats.total.stats[stat_lookup_hops].value += 0;
            G_stats.total.stats[stat_lookup_hops].count++;

            retcode.home = target;
            size_t reply_len = 0;

            if (reply_len > (size_t) lookup_bodyp->max_reply_payload_len) {
                VERBO(stderr, "WARNING Cause: reply payload length (%u) is bigger than the size of the designated memory area (%u) - reply payload discharged",
                        (uint) reply_len, (uint) lookup_bodyp->max_reply_payload_len);
                retcode.reply_payload_len = 0;
            } else {
                retcode.reply_payload_len = (NUM_32) reply_len;
            }

            VERBOSEX(stderr, "lookup successful localy solved (i.e., zero hopes) id=%.10u homeID=%.10u myID=%.10u\n", lookup_body.id, ip2id(retcode.home), G_myID);

            retcode.hops = 0;
            errno = 0;
            return retcode;
        }

        // now we are going to set the msg to send (lookup request)
        msg_hd lookup_req_header;
        lookup_req_header.systemid = G_parms.systemid;
        lookup_req_header.sender_port = G_parms.myIP.port;
        lookup_req_header.type = D1HT_LOOKUP;
        lookup_req_header.SeqNo = 0;

        struct iovec iovs[3]; // iovec for sending the UDP lookup request

        iovs[0].iov_base = (void *) &lookup_req_header;
        iovs[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        bytes_to_send += (ssize_t) iovs[0].iov_len;

        iovs[1].iov_base = (void *) lookup_bodyp;
        iovs[1].iov_len = (size_t) sizeof (struct D1HT_LOOKUP_body);
        bytes_to_send += (ssize_t) iovs[1].iov_len;

        int iovcnts;
        if (payload != NULL && lookup_bodyp->payload_len != 0) {
            iovcnts = 3;
            iovs[2].iov_base = payload;
            iovs[2].iov_len = lookup_bodyp->payload_len;
            bytes_to_send += (ssize_t) iovs[2].iov_len;
        } else {
            iovcnts = 2;
            iovs[2].iov_base = NULL;
            iovs[2].iov_len = 0;
        }

        // now we are going to set the msg to receive (lookup reply)
        D1HT_LOOKUP_reply_body reply_body;
        msg_hd reply_header;

        struct iovec iovr[3]; // iovec for receiveing the UDP lookup reply
        ssize_t bytes_to_receive = 0;

        iovr[0].iov_base = (void *) &reply_header;
        iovr[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        bytes_to_receive += (ssize_t) iovr[0].iov_len;

        iovr[1].iov_base = (void *) &reply_body;
        iovr[1].iov_len = (size_t) sizeof (struct D1HT_LOOKUP_reply_body);
        bytes_to_receive += (ssize_t) iovr[1].iov_len;

        int max_payload_size = D1HT_MTU - (D1HT_UDP_HEADER_SIZE * 2 + bytes_to_receive);

        if (D1HT_MAX_PAYLOAD_SIZE > max_payload_size)
            EXIT("D1HT_MAX_PAYLOAD_SIZE value (%d) is too big - it should be smaller than %d", D1HT_MAX_PAYLOAD_SIZE, max_payload_size);

        int iovcntr;
        if (reply_payload != NULL && lookup_bodyp->max_reply_payload_len != 0) {
            iovcntr = 3;
            iovr[2].iov_base = reply_payload;
            iovr[2].iov_len = lookup_bodyp->max_reply_payload_len;
        } else {
            iovcntr = 2;
            iovr[2].iov_base = NULL;
            iovr[2].iov_len = 0;
        }

        int connection = -1;
        int tries = 0;
        bool alive = false;
        NUM_8 SeqNo = 0;

        retcode.hops = 0;

        do {
            if (connection >= 0) {
                D1HT_CLOSE(connection);
                connection = -1;
            }

            retcode.hops++;
            tries = 0;

            IPaddr target_UDP;
            target_UDP.ip = target.ip;
            target_UDP.port = htons(ntohs(target.port) + 1);

#if defined(DEBUG)
            if (retcode.hops >= 2) {
                if (retcode.hops == 2) VERBOSE(stderr, "lookup for id=%.10u will take more than 2 hops\n", lookup_bodyp->id);
                if (tries == 0) VERBOSE(stderr, "Lookup for id=%.10u, hops=%u, prev_target=%s:%u, alive=%u\n",
                        lookup_bodyp->id, retcode.hops, IPtoP(lookup_bodyp->prev_target), lookup_bodyp->alive);
            }
#endif

            lookup_bodyp->alive = alive;
            alive = false;

            if (lookup_bodyp->prev_target == target) lookup_bodyp->prev_target.port = 0;

            SeqNo++;
            lookup_req_header.SeqNo = SeqNo;

            tv_rto = (timeval) G_rtt_lookup.rto_lookup();

            bytes_sent = 0;
            int errsave = 0;

            do {
                tries++;
                prev_tv = tv_rto;

                if (connection < 0) {
                    UDP_CONNECTION(connection, 0, target_UDP, XEQ_WARNING);
                    if (connection < 0) continue;
                }

                if (G_exiting) {
                    errno = ESHUTDOWN;
                    retcode.hops = 10;
                    return retcode;
                }

                XEQ_EXEC(bytes_sent = writev(connection, (iovec *) & iovs, iovcnts), XEQ_SILENT);

                errsave = errno;

                VERBOSE(stderr, "Lookup for id=%.10u, hops=%u, tries=%u, sent to %s:%u (target_UDP=%s:%u), bytes_to_send=%d, bytes_sent=%d, bytes_to_receive=%d, tv_rto=(%lu sec, %lu usec)\n",
                        lookup_bodyp->id, retcode.hops, tries, IPtoP(target), IPtoP(target_UDP), (int) bytes_to_send, (int) bytes_sent, (int) bytes_to_receive, (ulong) tv_rto.tv_sec, (ulong) tv_rto.tv_usec);

                bytes_rcvd = 0;
                reply_body.home.port = 0;

                if (peer_to_probe.port != 0) {
                    ASSERT(peer_to_probe != peer_to_add);
                    G_probe.force_probe(peer_to_probe, __FILE__, __LINE__);
                    peer_to_probe.port = 0;
                }

                if (peer_to_add.port != 0) {
                    pub_add(peer_to_add);
                    peer_to_add.port = 0;
                }

                if (bytes_sent <= 0) {
                    if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                        D1HT_CLOSE(connection);
                        connection = -1;
                    }
                    continue;
                }

                if (bytes_sent != bytes_to_send) {
                    D1HT_CLOSE(connection);
                    connection = -1;
                    continue;
                }

                XEQ_EXEC((bytes_rcvd = setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, &tv_rto, sizeof (tv_rto))), XEQ_WARNING);

                errsave = errno;

                if (bytes_rcvd < 0) {
                    if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) {
                        D1HT_CLOSE(connection);
                        connection = -1;
                    }
                    continue;
                }

                tv_rto = USEC2TV(((2.0 + (((float) retcode.hops) / 2.0)) * TV2USEC(prev_tv)));

                XEQ_EXEC(bytes_rcvd = readv(connection, (iovec *) & iovr, iovcntr), XEQ_SILENT);
                errsave = errno;

#if defined(DEBUG)
                if (true || (bytes_rcvd > 0 && target != reply_body.home)) {
                    VERBOSE(stderr, "lookup reply for id=%.10u, hops=%u, tries=%u, received, home=%s:%u, header type=%d, reply_body.reply_payload_len=%d\n",
                            lookup_bodyp->id, retcode.hops, tries, IPtoP(reply_body.home), (int) reply_header.type, (int) reply_body.reply_payload_len);
                    //					pub_print_table();
                }
#endif
                if (bytes_rcvd == -1) {
                    if (errsave == EAGAIN || errsave == EINTR || errsave == EWOULDBLOCK) {
                        continue;
                    } else {
                        break;
                    }
                } else if (bytes_rcvd >= (ssize_t) sizeof (reply_header) && reply_header.systemid != G_parms.systemid) {
                    bytes_rcvd = -1;
                    tries = G_parms.retries + 1;
                    alive = false;
                    break;
                } else if (bytes_rcvd != (ssize_t) (bytes_to_receive + reply_body.reply_payload_len) || reply_header.type != D1HT_LOOKUP_RETURN) {
                    D1HT_CLOSE(connection);
                    connection = -1;
                    bytes_rcvd = -1;
                    continue;
                } else {
                    alive = true;
                }
            } while (bytes_rcvd <= 0 && tries < G_parms.retries);

            lookup_bodyp->prev_target = target;

            if (connection >= 0) D1HT_CLOSE(connection);
            connection = -1;

            if (bytes_rcvd > 0) {
                if (bytes_rcvd < bytes_to_receive) bytes_rcvd = -1;
                if (reply_header.type != D1HT_LOOKUP_RETURN) bytes_rcvd = -1;
                if (reply_body.reply_payload_len > iovr[2].iov_len) bytes_rcvd = -1;
                if (lookup_req_header.SeqNo != reply_header.SeqNo) bytes_rcvd = -1;

                if (bytes_rcvd != (ssize_t) (bytes_to_receive + reply_body.reply_payload_len)) bytes_rcvd = -1;
            }

            if (bytes_rcvd <= 0 || bytes_sent <= 0) {

                peer_to_probe = target;
                if (retcode.hops >= 9) {
                    G_probe.probe(target, __FILE__, __LINE__);
                    break;
                }

                LOCK(rt_mutex);
                IPaddr new_target = succ(lookup_bodyp->id);

                if (new_target == target) new_target = succ((ip2id(target) + 1));

                UNLOCK(rt_mutex);

                target = new_target;
            } else {

                retcode.home = target;

                if (target != reply_body.home) {
                    peer_to_add = reply_body.home;
                    target = reply_body.home;
                }
            }

            if (target == G_parms.myIP) retcode.home = target;
            if (target == retcode.home) {
                IPaddr true_succ = pub_succ(lookup_bodyp->id);
                if (true_succ != target) {
                    G_probe.force_probe(true_succ, __FILE__, __LINE__);
                    if (peer_to_probe == true_succ) peer_to_probe.port = 0;
                    target = pub_succ(lookup_bodyp->id);
                }
            }
            if (target == G_parms.myIP) {
                retcode.home = target;
                retcode.reply_payload_len = reply_body.reply_payload_len;
            }

        } while (target != retcode.home && retcode.hops < 2 * G_parms.retries);

        if (target != retcode.home) {
            retcode.hops = 10;
            reply_payload = NULL;
            retcode.reply_payload_len = 0;

            LOCK(G_stats.stat_mutex);
            G_stats.total.stats[stat_lookups_total].count++;
            G_stats.total.stats[stat_lookups_failed].count++;
            UNLOCK(G_stats.stat_mutex);

            if (G_exiting) {
                errno = ESHUTDOWN;
            } else {
                errno = EAGAIN;
            }
        } else {
            retcode.reply_payload_len = reply_body.reply_payload_len;

            gettimeofday(&lookup_end, NULL);

            double latency = TVDIFF2MSEC(lookup_begin, lookup_end);

            LOCK(G_stats.stat_mutex);

            G_stats.total.stats[stat_LookLatency].add_time(latency);
            G_stats.total.stats[stat_lookups_total].count++;
            G_stats.total.stats[stat_lookup_hops].count++;
            G_stats.total.stats[stat_lookup_hops].value += (double) retcode.hops;

            if (retcode.hops > 1) {
                G_stats.total.stats[stat_lookups_not1hop].count++;
                G_stats.total.stats[stat_LookLatency_hops].add_time(latency);
                VERBOSEX(stderr, "WARNING Cause: lookup successful with more than one hop. Detail: id=%.10u homeID=%.10u target id=%.10u myID=%.10u home=%s:%u hops=%u\n", lookup_body.id, ip2id(retcode.home), ip2id(target), G_myID, IPtoP(retcode.home), retcode.hops);
            } else {
                G_stats.total.stats[stat_LookLatency_1hop].add_time(latency);
                VERBOSEX(stderr, "lookup successful id=%.10u homeID=%.10u myID=%.10u home=%s:%u hops=%u\n", lookup_body.id, ip2id(retcode.home), G_myID, IPtoP(retcode.home), retcode.hops);
            }

            if (retcode.hops == 1) {
                if (tries <= 1) {
                    G_stats.total.stats[stat_LookLatency_1try].add_time(latency);
                    G_rtt_lookup.update_rtt(lookup_begin, lookup_end);
                } else {
                    G_stats.total.stats[stat_LookLatency_tries].add_time(latency);
                }
            }

            UNLOCK(G_stats.stat_mutex);

            errno = 0;
        }

        return retcode;
    }

    IPaddr routing_table::lookup_local(msg_hd *lookup_req_header, const D1HT_LOOKUP_body *lookup_req_body,
            int connection, const IPaddr *peer, struct sockaddr_in *saddr, const void* payload_buff) {

        ASSERT(peer != NULL);
        ASSERT(saddr != NULL);
        ASSERT(lookup_req_body != NULL);
        ASSERT(lookup_req_header != NULL);
        ASSERT(connection >= 0);

        IPaddr retcode;
        BZERO(retcode);

        if (peer == NULL || saddr == NULL || lookup_req_body == NULL || lookup_req_header == NULL) {
            errno = EINVAL;
            return retcode;
        }

        if (connection < 0) {
            errno = EBADF;
            return retcode;
        }

        D1HT_LOOKUP_reply_body reply_body;
        BZERO(reply_body)

        IPaddr peer_probed;
        BZERO(peer_probed);

        LOCK(rt_mutex);

        reply_body.home = succ(lookup_req_body->id);

        if (reply_body.home != G_parms.myIP && reply_body.home == lookup_req_body->prev_target) {

            IPaddr new_home = succ(ip2id(reply_body.home) + 1);

            if (new_home == G_parms.myIP) {
                UNLOCK(rt_mutex);
                G_probe.force_probe(reply_body.home, __FILE__, __LINE__);
                peer_probed = reply_body.home;
                LOCK(rt_mutex);
                reply_body.home = succ(lookup_req_body->id);
            } else {
                reply_body.home = new_home;
            }
        }

        UNLOCK(rt_mutex);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return retcode;
        }

        size_t reply_len = 0;
        reply_body.reply_payload_len = 0;

        if (reply_body.home != G_parms.myIP) {
            reply_body.hops = 10;
        } else {
            if (g_reply_payload == NULL) g_reply_payload = MALLOC(D1HT_MAX_PAYLOAD_SIZE, char);

            if (reply_len > (size_t) lookup_req_body->max_reply_payload_len) {
                VERBO(stderr, "WARNING Cause: reply payload length (%u) is bigger than the size of the designated memory area (%u) - reply payload discharged",
                        (uint) reply_len, (uint) lookup_req_body->max_reply_payload_len);
            } else {
                reply_body.reply_payload_len = (NUM_32) reply_len;
            }
        }

        lookup_req_header->systemid = G_parms.systemid;
        lookup_req_header->sender_port = G_parms.myIP.port;
        lookup_req_header->type = D1HT_LOOKUP_RETURN;

        struct iovec iov[3];
        ssize_t bytes_to_send = 0;

        iov[0].iov_base = (void *) lookup_req_header;
        iov[0].iov_len = (size_t) D1HT_MSG_HEADER_size;
        bytes_to_send += iov[0].iov_len;

        iov[1].iov_base = (void *) &reply_body;
        iov[1].iov_len = (size_t) sizeof (reply_body);
        bytes_to_send += iov[1].iov_len;

        struct msghdr msg;
        BZERO(msg);

        if (reply_len == 0) {
            msg.msg_iovlen = 2;
        } else {
            msg.msg_iovlen = 3;
            iov[2].iov_base = g_reply_payload;
            iov[2].iov_len = reply_len;
            bytes_to_send += iov[2].iov_len;
        }

        msg.msg_iov = iov;
        msg.msg_name = (void *) saddr;
        msg.msg_namelen = sizeof (struct sockaddr_in);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return retcode;
        }

        ssize_t nbytes = 0;

        XEQ_EXEC((nbytes = sendmsg(connection, &msg, 0)), XEQ_WARNING);

        LOCK(rt_mutex);

        if (nbytes == bytes_to_send && peer != NULL) add(*peer);

        if (lookup_req_body->prev_target.port == 0) {

            UNLOCK(rt_mutex);

        } else {

            if (lookup_req_body->alive &&
                    pred_IP() != lookup_req_body->prev_target &&
                    succ_IP() != lookup_req_body->prev_target) {

                add(lookup_req_body->prev_target);

                UNLOCK(rt_mutex);

            } else {

                UNLOCK(rt_mutex);

                if (reply_body.home == lookup_req_body->prev_target &&
                        reply_body.home != peer_probed &&
                        reply_body.home != G_parms.myIP) {

                    G_probe.force_probe(reply_body.home, __FILE__, __LINE__);
                }
            }
        }
#if defined(DEBUG)
        if (peer != NULL) {
            if (reply_body.home != G_parms.myIP) {
                VERBOSE(stderr, "Lookup locally unsucessfull for id=%.10u, from %s:%u, nbytes=%d\n", lookup_req_body->id, IPtoP((*peer)), (int) nbytes);
                ASSERT(reply_body.hops == 10);
            } else {
                VERBOSE(stderr, "Lookup locally solved for id=%.10u, from %s:%u, nbytes=%d\n", lookup_req_body->id, IPtoP((*peer)), (int) nbytes);
                ASSERT(reply_body.hops == 0);
            }
            if (lookup_req_body->prev_target.port != 0) {
                VERBOSE(stderr, "Lookup for id=%.10u, prev_target=%s:%u, alive=%u\n", lookup_req_body->id, IPtoP(lookup_req_body->prev_target), lookup_req_body->alive);
                ASSERT(G_parms.myIP != lookup_req_body->prev_target);
            }
        }
#endif
        return reply_body.home;
    }

    /**
     * apre la connessione al nodo indicato
     * @param target_ip dest ip
     * @param target_port dest port
     * @return connected socket (dopo una non-blocking connect)
     */
    int D1HT_connect(const in_addr target_ip, const in_port_t target_port) {

        int connection = -1;
        IPaddr target;

        target.ip = target_ip;
        target.port = target_port;

        if (G_parms.systemid != 0) EXIT("Applications performing D1HT_connect should not set G_parms.systemid");

        VERBOSE(stderr, "Connecting to peer %s:%u\n", inet_ntoa(target_ip), ntohs(target_port));

        TCP_CONNECTION(connection, 0, target, XEQ_SILENT);

        return connection;
    }

    int D1HT_close(int *connection) {

        if (g_dummy_buff != NULL) {
            FREE(g_dummy_buff);
            g_dummy_buff = NULL;
        }
        if (*connection >= 0) D1HT_CLOSE(*connection);
        *connection = -1;
        errno = 0;
        return 0;
    }


    /**
     *
     * @param key
     * @param key_len: size of key. Should be less than D1HT_MAX_KEY_SIZE
     * @param connection:  connection returned by D1HT_connect
     * @param home_ip:   Return values with the IP and port of the peer associated with key (home peer)
     * @param home_port: Return values with the IP and port of the peer associated with key (home peer)
     * @param payload: it will be passed to the user_call_target routine at the home peer
     * @param payload_len: Should be less than D1HT_MAX_PAYLOAD_SIZE
     * @param reply_payload: result returned from the user_call_target routine at the home peer
     * @param reply_len :It is a value/result field. When D1HT_route is called *reply_len must contain the maximum size of the reply_payload.
        				  When D1HT_route sucessfully returns, reply_len contains the actual size of the reply pointed by reply_payload
        				  In any case, *reply_len should be less than D1HT_MAX_PAYLOAD_SIZE
     * @return
     */
    int D1HT_route(const char *key, const int key_len, int *connection, struct in_addr *home_ip, in_port_t *home_port,
            const void* payload, const ssize_t payload_len, void* reply_payload, ssize_t *reply_len) {

        errno = 0;

        if (key_len > D1HT_MAX_KEY_SIZE || payload_len > D1HT_MAX_PAYLOAD_SIZE) {
            errno = EINVAL;
            return -1;
        }

        if (key_len <= 0 || payload_len < 0 || *connection < 0) {
            errno = EINVAL;
            return -1;
        }

        if (reply_len != NULL && (*reply_len < 0 || *reply_len > D1HT_MAX_PAYLOAD_SIZE)) {
            errno = EINVAL;
            return -1;
        }

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (G_parms.systemid != 0) EXIT("Applications performing D1HT_route should not set G_parms.systemid");

        D1HT_LOOKUP_EXTERN_body lookup_extern_body;
        BZERO(lookup_extern_body);

        memcpy(lookup_extern_body.key, key, key_len);

        lookup_extern_body.payload_len = (NUM_32) payload_len;
        if (reply_len != NULL) lookup_extern_body.max_reply_payload_len = (NUM_32) * reply_len;

        if (SEND_MSG(*connection, D1HT_ROUTE, lookup_extern_body, XEQ_WARNING) < 0) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        if (payload_len != 0) {
            ssize_t nbytes;
            XEQ_EXEC((nbytes = writen(*connection, payload, (size_t) payload_len)), XEQ_WARNING);
            if (nbytes != payload_len) {
                if (errno == 0) errno = EBADF;
                return -1;
            }
        }

        msg_hd reply_hd;
        D1HT_LOOKUP_reply_body reply_bd;

        reply_hd = READ_HEADER(*connection, XEQ_WARNING);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (reply_hd.type != D1HT_LOOKUP_SUC) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        if (READ_BODY(*connection, reply_bd, XEQ_WARNING) < 0) return -1;

        home_ip->s_addr = reply_bd.home.ip.s_addr;
        *home_port = reply_bd.home.port;

        if (reply_bd.reply_payload_len == 0) {
            *reply_len = 0;
            errno = 0;
            return 0;
        }

        if (reply_payload == NULL) {
            errno = EINVAL;
            return -1;
        }

        if ((ssize_t) reply_bd.reply_payload_len > *reply_len) {
            *reply_len = 0;
            if (g_dummy_buff == NULL) g_dummy_buff = (char *) MALLOC(D1HT_MAX_PAYLOAD_SIZE, char);
            READ_BODY(*connection, *(g_dummy_buff), XEQ_SILENT);
            errno = ENOBUFS;
            return -1;
        }

        XEQ_EXEC((*reply_len = readn(*connection, reply_payload, (size_t) reply_bd.reply_payload_len)), XEQ_WARNING);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (*reply_len != (ssize_t) reply_bd.reply_payload_len) {
            if (*reply_len != -1 || errno == 0) errno = EBADF;
            return -1;
        }

        errno = 0;
        return 0;
    }

    int D1HT_lookup(const char *key, const int key_len, int *connection, struct in_addr *home_ip, in_port_t *home_port) {

        errno = 0;

        if (key_len > D1HT_MAX_KEY_SIZE) {
            verbose(stderr, "Problem in %s Cause: Key lengh exceeds D1HT_MAX_KEY_SIZE (which is %d bytes)\n", __FUNCTION__, (int) D1HT_MAX_KEY_SIZE);
            errno = EINVAL;
            return -1;
        }

        if (*connection < 0) {
            errno = EBADF;
            return -1;
        }

        if (G_parms.systemid != 0) EXIT("Applications performing D1HT_lookup should not set G_parms.systemid");

        D1HT_LOOKUP_EXTERN_body lookup_extern_body;
        BZERO(lookup_extern_body);
        memcpy(lookup_extern_body.key, key, key_len);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (SEND_MSG(*connection, D1HT_LOOKUP_EXTERN, lookup_extern_body, XEQ_SILENT) < 0) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        msg_hd reply_hd;
        D1HT_LOOKUP_reply_body reply_bd;

        reply_hd = READ_HEADER(*connection, XEQ_SILENT);

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (reply_hd.type != D1HT_LOOKUP_SUC) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        if (READ_BODY(*connection, reply_bd, XEQ_WARNING) < 0) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        home_ip->s_addr = reply_bd.home.ip.s_addr;
        *home_port = reply_bd.home.port;

        errno = 0;
        return 0;
    }


    /**
     *
     * @param peer
     * @param count
     * @param connection
     * @param succ: succ address is returned according to the local routing table, without any msg exchange
     * @return
     //  if count >=0 , succ will be the count_th successor of peer
     //  if count < 0 , succ will be the count_th predecessor of peer
     //  note that if count == 0 and peer exists in the D1HT system (according to the local routing table), succ = peer

     */
    int D1HT_succ(const IPaddr peer, const int count, int *connection, IPaddr *succ) {


        if (*connection < 0) {
            errno = EBADF;
            return -1;
        }

        if (peer.port <= 0) {
            errno = EINVAL;
            return -1;
        }

        if (G_exiting) {
            errno = ESHUTDOWN;
            return -1;
        }

        if (G_parms.systemid != 0) EXIT("Applications performing D1HT_succ should not set G_parms.systemid");

        struct D1HT_SUCC_body body;

        body.peer = peer;
        body.count = count;

        if (SEND_MSG(*connection, D1HT_SUCC, body, XEQ_SILENT) < 0) {
            if (errno == 0) errno = EBADF;
            return -1;
        }

        if (READ_BODY(*connection, body, XEQ_WARNING) < 0) {
            if (errno == 0) errno = EBADF;
            return -1;
        }
#if defined(DEBUG)
        //	if (count < G_RT.pub_npeers() && body.peer == peer) EXIT("Invalid succ");
#endif
        *succ = body.peer;

        errno = 0;
        return 0;
    }

} // namespace d1ht
