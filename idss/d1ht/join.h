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
 * @file join.h
 * @brief gestione del join di un nodo 
 */

#ifndef JOIN_H_
#define JOIN_H_

extern int G_forward_connection;



class join_events_t {
public:
    std::bitset<D1HT_RHO_MAX + 2 > ttls;
    std::bitset<D1HT_RHO_MAX + 2 > ttls_to_stop;

    IPaddr ip;
    int connection;
    bool forward_stopped;
    bool joined;

    /**
     * apre una connessione TCP verso il peer indicato e gli invia un msg header di tipo FORWARD_EVENTS
     * @param peer
     * @return 
     */
    bool fw_start(const IPaddr peer) {

        forward_stopped = false;
        joined = false;
        ip = peer;
        ttls.reset();
        ttls_to_stop.reset();

        TCP_CONNECTION(connection, 0, peer, XEQ_WARNING);

        if (connection < 0) return false;

        int flags = fcntl(connection, F_GETFL, 0);

        int ret = -1;
        if (flags >= 0) {
            flags |= O_NONBLOCK;
            XEQ_EXEC((ret = fcntl(connection, F_SETFL, flags)), XEQ_WARNING);
        }

        if (ret < 0 || SEND_HEADER(connection, D1HT_FORWARD_EVENTS, XEQ_WARNING) < 0) {
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        return true;
    }

    /**
     * chiude la connessione TCP 
     * @param filename
     * @param linenumber
     */
    void fw_stop(char* filename, const uint linenumber) {
        D1HT_CLOSE(connection);
        connection = -1;
    }

    /**
     * marca il nodo joined
     */
    void finish_join() {
        joined = true;
    }

    /**
     * setta il ttl del nodo
     * @param ttl
     */
    void set_ttl(const int ttl) {
        ttls.set((size_t) ttl);
    }
};




/**< class join_t contiene una lista di peers che fanno il join (probabile transitoria: quelli che ora sono in fase join) */
class join_t {
    bool no_peer_to_forward;
public:
    bool handling_join;
    lock_t join_mutex;
    IPaddr joining_IP;
    std::list<join_events_t> peers;

    join_t() {
        handling_join = false;
        no_peer_to_forward = true;
        LOCK_INIT(join_mutex, maychain = true, 28);
    }

    bool start_forward(const IPaddr peer) {
        if (G_parms.check) join_mutex.assert_locked_byme(__FILE__, __LINE__);

        bool retcode = false;
        join_events_t forward_events;

        if (forward_events.fw_start(peer)) {
            VERBOSEX(stderr, "Starting to forward events to %s:%u\n", IPtoP(peer));
            peers.push_back(forward_events);
            retcode = true;
            no_peer_to_forward = false;
        }
        return retcode;
    }

    
    void *locked_stop_forward(const IPaddr peer) {
        std::list<join_events_t>::iterator p = peers.begin();

        if (joining_IP == peer) handling_join = false;

        VERBOSEX(stderr, "Stopping to forward ALL events to %s:%u\n", IPtoP(peer));

        while (p != peers.end()) {
            if (p->ip == peer) {
                p->fw_stop(__FILE__, __LINE__);
                peers.erase(p);
                break;
            }
            p++;
        }
        if (peers.empty()) no_peer_to_forward = true;
        return NULL;
    }

    void *stop_forward(const IPaddr peer) {
        LOCK(join_mutex);
        void* retcode = locked_stop_forward(peer);
        UNLOCK(join_mutex);
        return retcode;
    }

    void *close_interval() {
        LOCK(join_mutex);
        std::list<join_events_t>::iterator p = peers.begin();

        while (p != peers.end()) {
            p->ttls_to_stop = p->ttls;
            if (p->forward_stopped) {
                p->fw_stop(__FILE__, __LINE__);
                peers.erase(p);
                break;
            }
            if ((int) p->ttls_to_stop.count() >= G_RT.aprox_rho()) {
                p->forward_stopped = true;
            } else if (p->ttls_to_stop.test(D1HT_RHO_MAX + 1)) {
                p->forward_stopped = true;
            } else if (p->joined) {
                // for peers that have already completed their join, by now they should have already had enough time to run stabilize with
                // their first fingers				
                if (!p->ttls_to_stop.test(0)) {
                    p->set_ttl(0);
                } else {
                    p->set_ttl(1);
                }
            }
            p++;
        }
        if (peers.empty()) no_peer_to_forward = true;
        UNLOCK(join_mutex);
        return NULL;
    }

    bool join_start(const IPaddr ip) {
        bool retcode = false;
        LOCK(join_mutex);
        if (!handling_join) {
            if (start_forward(ip)) {
                handling_join = true;
                joining_IP = ip;
                retcode = true;
            }
        } else {
            verbose(stderr, "WARNING Cause: Join postponed. Detail: I am busy handling %s:%u join. Postponing %s:%u join\n", IPtoP(joining_IP), IPtoP(ip));
        }
        UNLOCK(join_mutex);
        return retcode;
    }

    void join_abort() {
        stop_forward(joining_IP);
    }

    void join_complete(const IPaddr peer, const int rho) {
        LOCK(join_mutex);

        handling_join = false;

        std::list<join_events_t>::iterator p = peers.begin();

        while (p != peers.end()) {
            if (p->ip == peer) {
                p->finish_join();
                break;
            }
            p++;
        }

        UNLOCK(join_mutex);
    }

    
    void *forward_msg(const eventmsg_t* msgp) {

        if (no_peer_to_forward) return NULL;

        ASSERT(msgp->iovcnt != 0);

        ssize_t nbytes;

        struct iovec iov[D1HT_EVENT_MAX_IOVCNT];
        for (int i = 1; i < msgp->iovcnt; i++) iov[i] = msgp->iov[i];

        struct msg_hd msg_header;
        iov[0].iov_base = (void *) &msg_header;
        iov[0].iov_len = (size_t) D1HT_MSG_HEADER_size;

        msg_header.type = msgp->TTL;
        msg_header.SeqNo = msgp->SeqNo;
        msg_header.sender_port = 0;
        msg_header.systemid = G_parms.systemid;

        struct msghdr msg;
        struct cmsghdr cmsg;
        BZERO(cmsg);
        BZERO(msg);

        msg.msg_iov = iov;
        msg.msg_iovlen = msgp->iovcnt;
        msg.msg_control = &cmsg;
        msg.msg_controllen = sizeof (cmsg);

        Assert(msg.msg_iovlen < D1HT_EVENT_MAX_IOVCNT + 1, XEQ_ERROR);
        Assert(msg.msg_iovlen > 0, XEQ_ERROR);

        LOCK(join_mutex);

        if (no_peer_to_forward) {
            UNLOCK(join_mutex);
            return NULL;
        }

        std::list<join_events_t>::iterator p = peers.begin();

        std::list<join_events_t>::iterator next;

        while (p != peers.end()) {
            next = p;
            next++;
            if ((p->ip != msgp->whosent) && !p->ttls_to_stop.test(msgp->TTL)) {

                nbytes = -1;

                if (p->connection >= 0) nbytes = sendmsg(p->connection, &msg, 0);

                if (nbytes <= 0) {
#if defined(DEBUG)
                    int errnumber = errno;
                    VERBOSE(stderr, "WARNING Cause: Failed to forward events. Detail: TTL=%u to %s:%u, SeqNo=%u, tot_events=%u, nbytes=%d, iovcnt=%um msg=%s\n",
                            (uint) msgp->TTL, IPtoP(p->ip), (uint) msgp->SeqNo, (uint) msgp->tot_events, (int) nbytes, (uint) msg.msg_iovlen, strerror(errnumber));
#endif
                    G_probe.probe(p->ip, __FILE__, __LINE__);
                    p->fw_stop(__FILE__, __LINE__);
                    peers.erase(p);
                } else {
                    VERBOSEX(stderr, "Forwarded events with TTL=%u to %s:%u, SeqNo=%u, tot_events=%u, nbytes=%d, iovcnt=%u\n",
                            (uint) msgp->TTL, IPtoP(p->ip), (uint) msgp->SeqNo, (uint) msgp->tot_events, (int) nbytes, (uint) msg.msg_iovlen);
                    G_probe.probed(p->ip);
                }
            }
            p = next;
        }
        if (peers.empty()) no_peer_to_forward = true;
        UNLOCK(join_mutex);
        return NULL;
    }

    void* locked_ttl_arrived(const int ttl, const IPaddr peer) {
        void *retcode = NULL;

        if (ttl >= D1HT_TTL_0_NOEVENTS) return retcode;

        std::list<join_events_t>::iterator p = peers.begin();

        while (p != peers.end()) {
            if (p->ip == peer) {
                p->set_ttl(ttl);
                break;
            }
            p++;
        }
        return retcode;
    }

    void* ttl_arrived(const int ttl, const IPaddr peer) {
        VERBOSEX(stderr, "Stopping to forward events with TTL %u to %s:%u\n", ttl, IPtoP(peer));

        LOCK(join_mutex);
        void *retcode = locked_ttl_arrived(ttl, peer);
        UNLOCK(join_mutex);
        return retcode;
    }

};

class ttls_received_t {
    std::bitset<D1HT_RHO_MAX> ttls;
    lock_t ttl_mutex;
    bool received_all;
    IPaddr succ;
    int connection;
    bool connected;

public:

    ttls_received_t() {
        received_all = true;
        LOCK_INIT(ttl_mutex, maychain = false, 40);
    }

    bool start_notify(const IPaddr peer) {

        succ = peer;
        received_all = false;
        connected = false;
        ttls.reset();
        VERBOSEX(stderr, "trying STOP_TTL connection to %s:%u\n", IPtoP(succ));

        TCP_CONNECTION(connection, 0, succ, XEQ_WARNING);

        if (connection < 0) return false;

        if (SEND_HEADER(connection, D1HT_JOIN_STOP_TTL, XEQ_WARNING) < 0) {
            D1HT_CLOSE(connection);
            connection = -1;
            return false;
        }

        VERBOSEX(stderr, "STOP_TTL connection %d stablished to %s:%u\n", connection, IPtoP(succ));
        connected = true;
        return true;
    }

    void *stop_notify(char* filename, const uint linenumber) {
        void *retcode = NULL;
        VERBOSEFLX(stderr, filename, linenumber, "STOP_TTL stop_notify count=%u, rho=%u\n", (uint) ttls.count(), G_RT.aprox_rho());

        D1HT_CLOSE(connection);
        connection = -1;
        received_all = true;
        connected = false;
        ttls.set();
        return retcode;
    }

    bool received_ttl(const int ttl) {
        if (received_all) return false;
        bool retcode;

        LOCK(ttl_mutex);
        if ((retcode = !ttls.test((size_t) ttl)) == true) {
            VERBOSEX(stderr, "Sending STOP_TTL to %s:%u, ttl=%u, connection=%u, connected=%u\n", IPtoP(succ), ttl, connection, connected);
            struct D1HT_JOIN_STOP_TTL_body msg_body;
            msg_body.ttl = ttl;
            ssize_t nbytes = SEND_MSG(connection, D1HT_JOIN_STOP_TTL, msg_body, XEQ_WARNING);
            if (nbytes == sizeof (msg_body)) {
                ttls.set((size_t) ttl);
                if ((int) ttls.count() >= G_RT.aprox_rho()) stop_notify(__FILE__, __LINE__);
            } else {
                stop_notify(__FILE__, __LINE__);
            }
        }
        UNLOCK(ttl_mutex);
        return retcode;
    }
};

#endif /*JOIN_H_*/
