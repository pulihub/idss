/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.                     *
 *  No title to or ownership of D1HT is hereby transferred.	             *
 *									     *
 *  D1HT is free software; you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License version 3 as        *
 *  as published by the Free Software Foundation. Besides, the               *
 *  recipient of D1HT acknowledges and agrees to                             *
 *  strictly respect the following guidelines:                                *
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
 * @file TCP_server.c
 * @brief implementazione del server TCP responsabile della ricezione di Join e Leave
 *
 */

#include <sys/wait.h>

#include "D1HT.h"
#include "wrapper/query_defs.h"
using namespace std;

namespace d1ht{

    extern void *event_msg_handler(void* dummy);
    extern join_t G_joining_peers;
    extern ttls_received_t G_ttls_received;

    /**< user_call ??? */
    extern void user_call(msg_hd *msg_header, IPaddr sender, int connection);

    struct TCP_parms_t {
        int connection;
        IPaddr peer;
    };



    /**
     * Apre un TCP listening socket sulla porta indicata
     * @param port
     * @return
     */
    int get_tcp_socket(const PORTNO port) {
        int severity = XEQ_SILENT;
        ;
        int retcode = -1;

        for (int tries = 0; retcode <= 0; tries++) {
            D1HT_CLOSE(retcode);
            retcode = -1;
            mysleep(tries);
            if (tries == G_parms.retries) severity = XEQ_ERROR;
            TCP_SOCKET(retcode, port, severity);
            if (retcode >= 0) {
                int rlisten = -1;
                XEQ_EXEC((rlisten = listen(retcode, 100)), severity);
                if (rlisten < 0) retcode = -1;
            }
        }
        if (retcode < 0) EXIT("Unable to get TCP port. Detail: port=%u", ntohs(port));

        return retcode;
    }

    /**
     * TH dedicato alla gestione di una connessione TCP e dei messaggi che su di essa possono arrivare
     * @param _parm_temp
     * @return
     */
    PTHREAD(TCP_general_connection_handler) {

        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        struct TCP_parms_t *TCP_parms;
        //i dati della connessione TCP (connection e sender address) sono assegnati al thread e castati al tipo TCP_parms_t
        PTHREAD_PARM_ASSIGN(TCP_parms);

        if (G_exiting) {
            SEND_HEADER_SEQNO(TCP_parms->connection, D1HT_LEAVE, D1HT_LEAVE, XEQ_SILENT);
            D1HT_CLOSE(TCP_parms->connection);
            TCP_parms->connection = -1;
            return NULL;
        }


        struct msg_hd msg_header;
        //tutti i messaggi hanno un header:
        //class msg_hd (NUM_8 type, NUM_8 SeqNo, PORTNO sender_port, KEYID systemid)

        msg_header = READ_HEADER(TCP_parms->connection, XEQ_SILENT);

        if (msg_header.type == D1HT_INVALID) {
            VERBOSEX(stderr, "WARNING Cause: Failed to receive TCP msg from %s:%u\n", IPtoP(TCP_parms->peer));
            D1HT_CLOSE(TCP_parms->connection);
            TCP_parms->connection = -1;
            PTHREAD_PARM_FREE;
            return NULL;
        }

        //sender è l'indirizzo del mittente (ritornato da Accept())
        IPaddr sender = TCP_parms->peer;
        sender.port = msg_header.sender_port;

        VERBOSE(stderr, "TCP connection accepted on fd %d from %s:%u, msg type=%u, ephemeral port=%u.\n",
        TCP_parms->connection, IPtoP(sender), msg_header.type, TCP_parms->peer.port);

        if (G_started) {
            if (msg_header.type != D1HT_LOOKUP_EXTERN && msg_header.type != D1HT_STAT_TOT && msg_header.type != D1HT_SUCC && msg_header.type != D1HT_DUMP_TABLE &&
                    msg_header.type != D1HT_STAT_DIFF && msg_header.type != D1HT_ROUTE && msg_header.type < D1HT_JOIN_REQUEST) {
                if (G_parms.systemid != 0 && msg_header.systemid != G_parms.systemid) {
                    VERBO(stderr, "WARNING Cause: System id mismatch. Detail: Mine=%lu. Msg from %s:%u, type=%lu, systemid=%lu\n",
                            (ulong) G_parms.systemid, IPtoP(sender), (ulong) msg_header.type, (ulong) msg_header.systemid);
                }
                //va intesa come inizializzazione del probe del nodo sender
                G_probe.probed(sender);
            }
        } else if (msg_header.type > D1HT_FORWARD_EVENTS) {
            //se type > 128, è un forward message: chiudo la connessione e chiudo il thread
            D1HT_CLOSE(TCP_parms->connection);
            TCP_parms->connection = -1;
            PTHREAD_PARM_FREE;
            return NULL;
        }

        switch (msg_header.type) {

            case D1HT_JOIN_REQUEST:
            {
                //fa partire la procedura di join: il nodo invia una REPLY con le info sui succ e prec, aggiorna la RT e aiuta il nodo a terminare il join
                //se il join va a buon fine, aggiunge il nodo alla RT (da cui parte EDRA)
                if (!G_RT.pub_send(msg_header, &(TCP_parms->connection))) {
                    verbose(stderr, "WARNING Cause: failed to process join request. Detail: peer %s:%u\n", IPtoP(sender));
                }
                break;
            }

            case D1HT_JOIN_FINISHED:
            { // it should due to a broken join. Normally, this message should be ignorated
                verbose(stderr, "D1HT_JOIN_FINISHED received from %s:%u\n", IPtoP(sender));
                if (G_parms.check) EXIT("message D1HT_JOIN_FINISHED received");
                break;
            }

            case D1HT_JOIN_STOP_TTL:
            {
                struct msg_hd msg_header;
                struct D1HT_JOIN_STOP_TTL_body msg_body;
                ssize_t nbytes;
                while (true) {
                    msg_header = READ_HEADER_TYPE(TCP_parms->connection, D1HT_JOIN_STOP_TTL, XEQ_SILENT);
                    if (msg_header.type == D1HT_INVALID) break;
                    nbytes = READ_BODY(TCP_parms->connection, msg_body, XEQ_SILENT);
                    if (nbytes <= 0) break;
                    G_joining_peers.ttl_arrived(msg_body.ttl, sender);
                }
                VERBOSE(stderr, "Finished receiving STOP_TTL from %s:%u\n", IPtoP(sender));
                break;
            }

            case D1HT_FORWARD_EVENTS:
            { // I should receive the events forwarded by my successor
                // the loop bellow is almost the same as in UDP_server


                //se ho una forward connection attiva, la chiudo e la aggiorno con quella attuale
                if (G_forward_connection >= 0) D1HT_CLOSE(G_forward_connection);
                G_forward_connection = TCP_parms->connection;

                VERBOSE(stderr, "Waiting for forwarded events from %s:%u\n", IPtoP(sender));

                while (!G_exiting) {
                    eventmsg_t *msg;

                    msg = CALLOC(1, eventmsg_t);

                    //su questa connessione mi predispongo a ricevere un eventmsg
                    if (msg->eventmsg_receive(G_forward_connection)) {
                        //  PTHREAD_CREATE(event_msg_handler, msg, &G_detached);
                        //
                        G_RT.ack_msg(msg);
                        FREE_DESTROY(msg);
                    } else {
                        FREE_DESTROY(msg);
                        break;
                    }
                }
                G_ttls_received.stop_notify(__FILE__, __LINE__);
                VERBOSE(stderr, "Finished receiving forwarded events from %s:%u\n", IPtoP(sender));
                if (G_started && !G_exiting) G_probe.probe_not_detached(sender, __FILE__, __LINE__);
                break;
            }

            case D1HT_LEAVE:
            {
                G_RT.pub_del(sender, true);
                break;
            }

            case D1HT_DUMMY:
            {
                break;
            }

            //****************************************************************************
            //************************************************              HPC Lab Extension
            case D1HT_QUERY_REQUEST:
            {
                struct D1HT_QUERY_REQUEST_body body;
                ssize_t nbytes = READ_BODY(TCP_parms->connection, body, XEQ_WARNING);

                //G_QM.debug_print_query(body.msg, body.uqi, body.deadline, body.reductor, ip2p( body.originNode.ip ), ntohs( body.originNode.port ), body.originKey, ip2p( sender.ip ), ntohs( sender.port ), body.TTL );

                char sender_node[INET6_ADDRSTRLEN+6] = {0};
                snprintf(sender_node, INET6_ADDRSTRLEN+5, "%s:%d", IPtoP(sender));

                query_received_args *args;
                args = (query_received_args*) malloc(sizeof(query_received_args));
                args->sql = strdup(body.msg);
                args->uqi = strdup(body.uqi);
                args->ttl = body.deadline;
                args->origin_key = body.originKey;
                args->sender = strdup(sender_node);
                G_QM.exec_query_received_callback(args);


                Query q = Query(body.uqi, body.msg, body.deadline,  body.reductor, body.TTL, body.originNode,  body.originKey, false, sender);
                q.relay_query();

                break;
            }


            case D1HT_QUERY_REPLY:
            {
                struct D1HT_QUERY_REPLY_body body;
                ssize_t nbytes = READ_BODY(TCP_parms->connection, body, XEQ_WARNING);

                char sender_node[INET6_ADDRSTRLEN+6] = {0};
                snprintf(sender_node, INET6_ADDRSTRLEN+5, "%s:%d", IPtoP(sender));

                response_received_args *args;
                args = (response_received_args*) malloc(sizeof(response_received_args));
                args->msg = strdup(body.response);
                args->uqi = strdup(body.uqi);
                args->subq_n = body.subq_n;
                args->sender = strdup(sender_node);

                G_QM.exec_response_received_callback(args);

                break;
            }

            //************************************************              HPC Lab Extension
            //****************************************************************************

            case D1HT_PROBE:
            {
                SEND_HEADER(TCP_parms->connection, D1HT_PROBE, XEQ_SILENT);
                G_RT.pub_add(sender);
                break;
            }

            case D1HT_STAB:
            {
                struct D1HT_STAB_body msg_body;

                ssize_t nbytes = READ_BODY(TCP_parms->connection, msg_body, XEQ_WARNING);

                if (nbytes == sizeof (msg_body)) {
                    G_RT.stabilize_reply(&(TCP_parms->connection), sender, msg_body);
                }
                break;
            }

            case D1HT_LOOKUP_EXTERN:
            case D1HT_SUCC:
            case D1HT_ROUTE:
            {
                void* payload = NULL;
                void* reply_payload = NULL;

                while (!G_exiting) {

                    if (msg_header.type != D1HT_LOOKUP_EXTERN && msg_header.type != D1HT_SUCC && msg_header.type != D1HT_ROUTE) break;

                    ssize_t nbytes;

                    if (msg_header.type == D1HT_SUCC) {

                        struct D1HT_SUCC_body succ_body;

                        nbytes = READ_BODY(TCP_parms->connection, succ_body, XEQ_WARNING);

                        if (G_exiting) break;
                        if (nbytes != sizeof (succ_body)) break;

                        IPaddr temp_ip = succ_body.peer;

                        if (succ_body.count >= 0) {
                            succ_body.peer = G_RT.pub_succn(ip2id(temp_ip), succ_body.count);
                        } else {
                            succ_body.peer = G_RT.pub_predn(ip2id(temp_ip), (-succ_body.count));
                        }
                        if (SEND_BODY(TCP_parms->connection, succ_body, XEQ_WARNING) < 0) break;

                    } else {

                        D1HT_LOOKUP_EXTERN_body lookup_extern_body;
                        D1HT_LOOKUP_body lookup_body;
                        BZERO(lookup_body);
                        D1HT_LOOKUP_reply_body reply_body;
                        BZERO(reply_body);

                        nbytes = READ_BODY(TCP_parms->connection, lookup_extern_body, XEQ_WARNING);

                        if (G_exiting) break;
                        if (nbytes != sizeof (lookup_extern_body)) break;

                        lookup_body.id = key2id(lookup_extern_body);
                        lookup_body.prev_target.port = 0;

                        if (msg_header.type == D1HT_LOOKUP_EXTERN) {

                            ASSERT(lookup_extern_body.payload_len == 0 && lookup_body.max_reply_payload_len == 0);

                            if (lookup_extern_body.payload_len != 0 || lookup_body.max_reply_payload_len != 0) break;

                            reply_body = G_RT.lookup(&lookup_body);

                            //VERBOSE(stderr, "Lookup for id=%.10u, from %s:%u, home host=%s:%u, myIP=%s:%u, hops=%u\n", lookup_body.id, IPtoP(sender), IPtoP(reply_body.home), IPtoP(G_parms.myIP), reply_body.hops);

                            if (reply_body.hops >= 10) {
                                SEND_MSG_SEQNO(TCP_parms->connection, D1HT_LOOKUP_FAILED, msg_header.SeqNo, reply_body, XEQ_WARNING);
                                VERBOSE(stderr, "WARNING Cause: lookup failed. Detail: id=%.10u\n", lookup_body.id);
                            } else {
                                SEND_MSG_SEQNO(TCP_parms->connection, D1HT_LOOKUP_SUC, msg_header.SeqNo, reply_body, XEQ_WARNING);
                            }
                        } else {
                            ASSERT(msg_header.type == D1HT_ROUTE);

                            if (payload == NULL) {
                                payload = MALLOC(D1HT_MAX_PAYLOAD_SIZE, char);
                                reply_payload = MALLOC(D1HT_MAX_PAYLOAD_SIZE, char);
                            }
                            lookup_body.payload_len = lookup_extern_body.payload_len;
                            lookup_body.max_reply_payload_len = lookup_extern_body.max_reply_payload_len;

                            if (lookup_body.payload_len > 0) {
                                XEQ_EXEC((nbytes = readn(TCP_parms->connection, payload, (size_t) lookup_body.payload_len)), XEQ_WARNING);
                                if (nbytes != (ssize_t) lookup_body.payload_len) break;
                            }

                            reply_body = G_RT.lookup(&lookup_body, payload, reply_payload);

                            //VERBOSE(stderr, "Lookup for id=%.10u, from %s:%u, home host=%s:%u, myIP=%s:%u, hops=%u\n", lookup_body.id, IPtoP(sender), IPtoP(reply_body.home), IPtoP(G_parms.myIP), reply_body.hops);

                            if (reply_body.hops >= 10) {
                                if (SEND_MSG_SEQNO(TCP_parms->connection, D1HT_LOOKUP_FAILED, msg_header.SeqNo, reply_body, XEQ_WARNING) < 0) break;
                                VERBOSE(stderr, "WARNING Cause: lookup failed. Detail: id=%.10u\n", lookup_body.id);
                            } else {
                                if (SEND_MSG_SEQNO(TCP_parms->connection, D1HT_LOOKUP_SUC, msg_header.SeqNo, reply_body, XEQ_WARNING) < 0) {
                                    break;
                                } else {
                                    ASSERT(lookup_body.id, ip2id(retcode.home), G_myID);
                                    if (reply_body.reply_payload_len != 0) {
                                        XEQ_EXEC((nbytes = writen(TCP_parms->connection, reply_payload, (size_t) reply_body.reply_payload_len)), XEQ_WARNING);
                                        if (nbytes != (ssize_t) reply_body.reply_payload_len) break;
                                    }
                                }
                            }
                        }
                    }
                    msg_header = READ_HEADER(TCP_parms->connection, XEQ_SILENT);
                }

                if (payload != NULL) {
                    FREE(payload);
                    ASSERT(reply_payload != NULL);
                    FREE(reply_payload);
                }

                break;
            }

            case D1HT_STAT_TOT:
            {
#if !defined(DEBUG) || 1

                stat_part_t temp_stat;

                LOCK(G_stats.stat_mutex);
                G_stats.total.times.time_get();
                temp_stat = G_stats.total;
                UNLOCK(G_stats.stat_mutex);

                SEND_MSG(TCP_parms->connection, D1HT_STAT_REPLY, temp_stat, XEQ_WARNING);
#endif
                break;
            }

            case D1HT_STAT_DIFF:
            {
#if !defined(DEBUG) || 1
                stat_part_t diff;

                VERBOSE(stderr, "D1HT_STAT_DIFF connection on fd %d from %s:%u, msg type=%u, ephemeral port=%u.\n",
                        TCP_parms->connection, IPtoP(sender), msg_header.type, TCP_parms->peer.port);

                // we don't allow concurrent D1HT_STAT_DIFF connections

                if (!G_stats.stat_connect(TCP_parms->connection)) {
                    D1HT_CLOSE(TCP_parms->connection);
                    EXIT("Problem transmitting stats");
                }

                int xeq_flag = XEQ_WARNING;

                struct timeval tv_timeo;
                tv_timeo.tv_usec = 0;
                tv_timeo.tv_sec = 0;
                XEQ_EXEC(setsockopt(TCP_parms->connection, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof (tv_timeo)), XEQ_WARNING);

                while (1) {

                    D1HT_STAT_body body;
                    ssize_t nbytes;

                    nbytes = READ_BODY(TCP_parms->connection, body, xeq_flag);
                    int errsave = errno;

                    if (nbytes <= 0) {
                        if (errsave == EAGAIN || errsave == EINTR || errsave == EWOULDBLOCK) {
                            XEQ_EXEC(setsockopt(TCP_parms->connection, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof (tv_timeo)), XEQ_WARNING);
                            continue;
                        }
                        G_stats.stat_connect(-1);
                        break;
                    }

                    G_stats.diff(&diff);

                    nbytes = SEND_MSG(TCP_parms->connection, D1HT_STAT_REPLY, diff, XEQ_WARNING);

                    if (nbytes <= 0) {
                        G_stats.stat_connect(-1);
                        break;
                    }

                    if (body.new_savg > 0) G_theta.new_Savg(body.new_savg);

                    msg_header = READ_HEADER(TCP_parms->connection, XEQ_SILENT);

                    if (msg_header.type != D1HT_STAT_DIFF) {
                        G_stats.stat_connect(-1);
                        break;
                    }

                    xeq_flag = XEQ_SILENT;
                }

                VERBO(stderr, "Finishing transmiting stats on fd %d to %s:%u, msg type=%u, ephemeral port=%u.\n",
                        TCP_parms->connection, IPtoP(sender), msg_header.type, TCP_parms->peer.port);
#endif
                break;
            }

            default:
            {
#if defined(DEBUG)
                char msg[256];
                sprintf(msg, "message type=%u, from=%s:%u", msg_header.type, IPtoP(sender));
                D1HT_ERROR("Unknown message type received", msg, XEQ_WARNING);
#endif
                break;
            }
        }
        if (TCP_parms->connection > 0) {
            D1HT_CLOSE(TCP_parms->connection);
            TCP_parms->connection = -1;
        }

        ASSERT(!G_RT.rt_mutex.xeq_locked_byme(), "TCP_general_connection_handler may not be holding any LOCK when leaving.");

        PTHREAD_PARM_FREE;
        return NULL;
    }

    /**
     * TH server: accetta le connessioni TCP da parte dei peers e delega ad un th la gestione dei messaggi
     */
    PTHREAD(TCP_server) {
        PTHREAD_DETACH;
        PTHREAD_INIT;

        int tries = 0;
        int severity = XEQ_WARNING;
        int iretcode = 1;
        struct sockaddr_in remote_address;
        socklen_t address_length = sizeof (remote_address);

        VERBOSE(stderr, "%s TCP server listening. Detail: address=%s:%d\n", G_parms.program_name, IPtoP(G_parms.myIP));

        while (!G_exited && (iretcode >= 0 || tries <= G_parms.retries)) {
            BZERO(remote_address);

            XEQ_EXEC(iretcode = Accept(G_TCP_socket, (struct sockaddr *) &remote_address, &address_length), severity);

            //alla chiusura del processo, un messaggio di leave è inviato sul socket iretcode (avvalorato dalla Accept())
            if (G_exiting) {
                SEND_HEADER_SEQNO(iretcode, D1HT_LEAVE, D1HT_LEAVE, XEQ_SILENT);
                D1HT_CLOSE(iretcode);
                iretcode = -1;
                continue;
            }

            if (iretcode >= 0) {

                struct TCP_parms_t *TCP_parms = CALLOC(1, struct TCP_parms_t);

                TCP_parms->connection = iretcode;
                TCP_parms->peer.ip = remote_address.sin_addr;
                TCP_parms->peer.port = remote_address.sin_port;
#if defined(DEBUG)
                pthread_t tid = PTHREAD_CREATE(TCP_general_connection_handler, TCP_parms, &G_detached);
                VERBOSE(stderr, "Thread tid=%lu will deal with msg received from %s\n", (long uint) tid, NTOP(remote_address.sin_addr));
#else
                PTHREAD_CREATE(TCP_general_connection_handler, TCP_parms, &G_detached);
#endif
            } else {
                D1HT_CLOSE(G_TCP_socket);
                G_TCP_socket = -1;
                mysleep(tries);
                if (G_exiting) break;
                G_TCP_socket = get_tcp_socket(G_parms.myIP.port);
                if (tries == G_parms.retries) severity = XEQ_ERROR;
                tries++;
            }
        }
        G_TCP_thread_id = 0;
        return NULL;
    }

} // namespace d1ht
