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
 * @file stab.c
 * @brief procedure di stabilizzazione del sistema: 
 * un joining peer richiede info sui succ e prec di un altro nodo 
 * e compone una lista di peer da probare.
 * D1HT_STAB body è il messaggio usato
 *  
 */

#include "D1HT.h"

using namespace std;

namespace d1ht {

    struct STAB_msg_to_send {
        IPaddr target;
        struct D1HT_STAB_body body;
    };

    PTHREAD(stab_detached) {
        PTHREAD_DETACH;
        if (G_exiting) return NULL;

        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        STAB_msg_to_send *msg;
        PTHREAD_PARM_ASSIGN(msg);

        int connection;
        struct D1HT_STAB_body reply; /**< contiene gli IP di 4 nodi: 2 prec e 2 succ */

        VERBOSE(stderr, "sending stabilize request to %s:%u\n", IPtoP(msg->target));

        if (TCP_CONNECTION(connection, 0, msg->target, XEQ_SILENT) == 0) {
            ssize_t nbytes = SEND_MSG(connection, D1HT_STAB, msg->body, XEQ_SILENT);
            if (nbytes != -1) {
                msg_hd header = READ_HEADER(connection, XEQ_SILENT);

                if (header.type == D1HT_LEAVE && header.SeqNo == D1HT_LEAVE) {
                    G_probe.probed(msg->target);
                    G_RT.pub_del(msg->target, false);
                    D1HT_CLOSE(connection);
                    PTHREAD_PARM_FREE;
                    return NULL;
                }

                if (header.type == D1HT_STAB && header.systemid == G_parms.systemid) {
                    nbytes = READ_BODY(connection, reply, XEQ_SILENT);
                    if (nbytes != -1) {
                        G_probe.probed(msg->target);
                        G_RT.stabilize_final(msg->target, reply);
                        D1HT_CLOSE(connection);
                        PTHREAD_PARM_FREE;
                        return NULL;
                    }
                }
            }
        }
        G_probe.probe_not_detached(msg->target, __FILE__, __LINE__);
        D1HT_CLOSE(connection);
        PTHREAD_PARM_FREE;
        return NULL;
    }

    /**
     * Dopo aver ricevuto il messaggio D1HT_STAB in risposta alla richiesta fatta, aggiorno la lista di peer to probe
     * @param peerIP
     * @param reply
     * @return 
     */
    void* routing_table::stabilize_final(const IPaddr peerIP, const D1HT_STAB_body reply) {

        if (G_exiting || !G_started) return NULL;

        stack<IPaddr> IPs_to_probe;

        LOCK(rt_mutex);

        if (G_parms.myIP != reply.succ_IP && !find(reply.succ_IP)) IPs_to_probe.push(reply.succ_IP);

        if (G_parms.myIP != reply.suc2_IP && !find(reply.suc2_IP)) IPs_to_probe.push(reply.suc2_IP);

        if (G_parms.myIP != reply.pred_IP && !find(reply.pred_IP)) IPs_to_probe.push(reply.pred_IP);

        if (G_parms.myIP != reply.pre2_IP && !find(reply.pre2_IP)) IPs_to_probe.push(reply.pre2_IP);

        UNLOCK(rt_mutex);

        while (!IPs_to_probe.empty()) {
            G_probe.probe_not_detached(IPs_to_probe.top(), __FILE__, __LINE__);
            IPs_to_probe.pop();
        }

        IPaddr temp_IP;

        LOCK(rt_mutex);

        if (G_parms.myIP != reply.pred_IP) {
            temp_IP = succn(reply.pred_IP, 1);
            if (temp_IP != peerIP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != reply.pre2_IP) {
            temp_IP = succn(reply.pre2_IP, 2);
            if (temp_IP != peerIP) IPs_to_probe.push(temp_IP);
            temp_IP = succn(reply.pre2_IP, 1);
            if (temp_IP != reply.pred_IP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != reply.succ_IP) {
            temp_IP = succn(peerIP, 1);
            if (temp_IP != reply.succ_IP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != reply.suc2_IP) {
            temp_IP = succn(peerIP, 2);
            if (temp_IP != reply.suc2_IP) IPs_to_probe.push(temp_IP);
            temp_IP = succn(reply.succ_IP, 1);
            if (temp_IP != reply.suc2_IP) IPs_to_probe.push(temp_IP);
        }

        UNLOCK(rt_mutex);

        while (!IPs_to_probe.empty()) {
            G_probe.probe_not_detached(IPs_to_probe.top(), __FILE__, __LINE__);
            IPs_to_probe.pop();
        }

        VERBOSE(stderr, "stabilize ended ip=%s:%u\n", IPtoP(peerIP));

        return NULL;
    }

    /**
     * 
     * @param connection
     * @param peerIP
     * @param sent
     * @return 
     */
    void* routing_table::stabilize_reply(int *connection, const IPaddr peerIP, const D1HT_STAB_body sent) {

        if (G_exiting || !G_started) return NULL;

        stack<IPaddr> IPs_to_probe;
        D1HT_STAB_body reply;

        VERBOSE(stderr, "stabilizing request received from ip=%s:%u\n", IPtoP(peerIP));
        //    VERBOSE(stderr,"sent.succ_IP=%s:%u\n", IPtoP(sent.succ_IP));
        //    VERBOSE(stderr,"sent.suc2_IP=%s:%u\n", IPtoP(sent.suc2_IP));
        //    VERBOSE(stderr,"sent.pred_IP=%s:%u\n", IPtoP(sent.pred_IP));
        //    VERBOSE(stderr,"sent.pre2_IP=%s:%u\n", IPtoP(sent.pre2_IP));

        LOCK(rt_mutex);

        //    print_table(stderr);

        stabilized(peerIP);

        if (G_parms.myIP != sent.succ_IP && !find(sent.succ_IP)) IPs_to_probe.push(sent.succ_IP);

        if (G_parms.myIP != sent.suc2_IP && !find(sent.suc2_IP)) IPs_to_probe.push(sent.suc2_IP);

        if (G_parms.myIP != sent.pred_IP && !find(sent.pred_IP)) IPs_to_probe.push(sent.pred_IP);

        if (G_parms.myIP != sent.pre2_IP && !find(sent.pre2_IP)) IPs_to_probe.push(sent.pre2_IP);

        if (succ_IP() != peerIP && !find(peerIP)) add(peerIP);

        //    print_table(stderr);

        reply.pred_IP = pred_IP();
        reply.pre2_IP = predn(ip2id(pred_IP()), 1);
        reply.succ_IP = succ_IP();
        reply.suc2_IP = succn(succ_IP(), 1);

        UNLOCK(rt_mutex);

        if (SEND_MSG(*connection, D1HT_STAB, reply, XEQ_WARNING) != -1) G_probe.probed(peerIP);

        while (!IPs_to_probe.empty()) {
            G_probe.probe_not_detached(IPs_to_probe.top(), __FILE__, __LINE__);
            IPs_to_probe.pop();
        }

        IPaddr temp_IP;

        LOCK(rt_mutex);

        if (G_parms.myIP != sent.pred_IP) {
            temp_IP = succn(sent.pred_IP, 1);
            if (temp_IP != peerIP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != sent.pre2_IP) {
            temp_IP = succn(sent.pre2_IP, 2);
            if (temp_IP != peerIP) IPs_to_probe.push(temp_IP);
            temp_IP = succn(sent.pre2_IP, 1);
            if (temp_IP != sent.pred_IP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != sent.succ_IP) {
            temp_IP = succn(peerIP, 1);
            if (temp_IP != sent.succ_IP) IPs_to_probe.push(temp_IP);
        }

        if (G_parms.myIP != sent.suc2_IP) {
            temp_IP = succn(peerIP, 2);
            if (temp_IP != sent.suc2_IP) IPs_to_probe.push(temp_IP);
            temp_IP = succn(sent.succ_IP, 1);
            if (temp_IP != sent.suc2_IP) IPs_to_probe.push(temp_IP);
        }

        UNLOCK(rt_mutex);

        while (!IPs_to_probe.empty()) {
            G_probe.probe_not_detached(IPs_to_probe.top(), __FILE__, __LINE__);
            IPs_to_probe.pop();
        }
        VERBOSE(stderr, "stabilizing ended with ip=%s:%u\n", IPtoP(peerIP));

        return NULL;
    }

    void* routing_table::stabilize(const IPaddr peer, const char* file, const uint line) {

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling stabilize.");

        if (npeers() < 3 || stabilized(peer) || G_exiting || !G_started) {
            VERBOSEFL(stderr, file, line, "Stabilize avoided ip=%s:%u\n", IPtoP(peer));
            return NULL;
        }
        VERBOSEFL(stderr, file, line, "Stabilize not avoided ip=%s:%u\n", IPtoP(peer));

        struct STAB_msg_to_send *msg;

        msg = CALLOC(1, STAB_msg_to_send);

        msg->target = peer;

        msg->body.pred_IP = pred_IP();
        msg->body.pre2_IP = predn(ip2id(pred_IP()), 1);
        msg->body.succ_IP = succ_IP();
        msg->body.suc2_IP = succn(succ_IP(), 1);

        ASSERT(msg->body.succ_IP != msg->body.suc2_IP);

        PTHREAD_CREATE(stab_detached, msg, &G_detached);

        return NULL;
    }

    void* routing_table::pub_stabilize(const char* file, const uint line) {

        LOCK(rt_mutex);
        IPaddr peer = pred_IP();
        stabilize(peer, file, line);
        UNLOCK(rt_mutex);
        return NULL;
    }

    void* routing_table::pub_stabilize(const IPaddr peer, const char* file, const uint line) {

        LOCK(rt_mutex);
        stabilize(peer, file, line);
        UNLOCK(rt_mutex);
        return NULL;
    }

    void* routing_table::stop_stab() {
        no_stab = true;
        return NULL;
    }

    void* routing_table::stabilize_close_interval() {

        LOCK(rt_mutex);

        if (no_stab || G_exiting) {
            UNLOCK(rt_mutex);
            return NULL;
        }

        list<stab_peers_t>::iterator p = stab_peers.begin();

        while (p != stab_peers.end()) {
            p->count++;
            if (p->count >= 2) {
                p = stab_peers.erase(p);
            } else {
                p++;
            }
        }
        UNLOCK(rt_mutex);
        return NULL;
    }

    void* routing_table::stabilize_force(IPaddr *skip, join_t *joining_peers) {
        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling stabilize_force.");

        if (1 || no_stab || G_exiting) return NULL;

        list<stab_peers_t>::iterator ps = stab_peers.begin();
        ;
        list<join_events_t> *peers;
        list<join_events_t>::iterator pp;

        while (ps != stab_peers.end()) {
            ps = stab_peers.erase(ps);
        }

        LOCK(joining_peers->join_mutex);

        peers = &(joining_peers->peers);
        for (pp = peers->begin(); pp != peers->end(); pp++) {
            if (skip != NULL && pp->ip != *(skip)) {
                stabilize(pp->ip, __FILE__, __LINE__);
            }
        }
        UNLOCK(joining_peers->join_mutex);
        return NULL;
    }

    bool routing_table::pub_stabilized(const IPaddr host) {

        bool already_stabed;
        LOCK(rt_mutex);
        already_stabed = stabilized(host);
        UNLOCK(rt_mutex);
        return already_stabed;
    }

    bool routing_table::stabilized(const IPaddr host) {

        bool already_stabed = false;

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling stabilized.");

        if (no_stab || G_exiting) return true;

        list<stab_peers_t>::iterator p = stab_peers.begin();

        while (p != stab_peers.end()) {
            if (p->ip == host) {
                already_stabed = true;
                break;
            }
            p++;
        }

        if (!already_stabed) {
            stab_peers_t new_peer(host);
            stab_peers.push_back(new_peer);
        }
        return already_stabed;
    }

} // namespace d1ht
