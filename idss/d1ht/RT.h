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
 * @file RT.h
 * @brief gestione della routing table del nodo
 */

#ifndef RT_H_
#define RT_H_

class join_t;
struct temp_interval_t;
class interval_t;


#define	BUCKETSIZE	7 /**< Routing Table buckets */

struct bucket_t {
    IPaddr slot[BUCKETSIZE];
};

class stab_peers_t {
public:
    IPaddr ip;
    unsigned int count;

    stab_peers_t() {
        EXIT("Invalid stab_peers_t initialization");
    }

    stab_peers_t(const IPaddr peer) {
        count = 0;
        ip = peer;
    }
};

/*** RING and ID manipulation MACROS ******************************/
#define INSTRETCH(who, start, end) ( (end >= start) ? (who >= start && who <= end) : (who <= end || who >= start) )
#define INSTRETCHX(who, start, end) ( (end >= start) ? (who > start && who < end) : (who < end || who > start) )

#define pstretch(who, start, end)  fprintf(stderr," who=%u, start=%u, end=%u, in=%d\n", who, start, end, INSTRETCH( who, start, end));

/*** Routing Table **************************************/

struct pos_t {
    int bucket;
    int slot;
};

class routing_table {
    /*
     *  The routing table (table) and bucket_peers are separeted structures in order to avoid alignment problems
     */
    bucket_t *table;
    unsigned char *bucket_peers;
#ifdef D1HT_JOIN_COPY_RT
    bucket_t *table_copy;
    unsigned char *bucket_peers_copy;
#endif
   
public:
    lock_t rt_mutex; /**< rt_mutex protects *table, *bucket_peers, rt_size, npeers_, succID_, succIP_, predID_, predIP_ */

private:
    IPaddr succIP_, predIP_;
    KEYID succID_, predID_;
    int npeers_;

    NUM_8 SeqNo_lookup;

    int basepos;
    
    bool find(const KEYID id, pos_t *pred, pos_t *predl, pos_t *succ);

    /**
     * data una key, esegue il find della key, ritornando 3 valori
     * @param id
     * @return value-result arguments: 
     */
    bool find(const KEYID id) {
        pos_t dummy1, dummy2, dummy3;
        return find(id, &dummy1, &dummy2, &dummy3);
    };

    /**
     * dato l'IP, ne ottiene la Key associata ed esegue il find()
     * @param IP
     * @return 
     */
    bool find(const IPaddr IP) {
        return find(ip2id(IP));
    };
   
    pos_t posl2pos(const pos_t posl);

    void *del(const KEYID id);

    void del(const IPaddr IP) {
        VERBOSE(stderr, "Deleting other port %s:%u\n", IPtoP(IP));
        if (IP.port == 0) return;
        del(ip2id(IP));
    };

    void del(const IP ip) {
        VERBOSE(stderr, "Deleting default port %s:%u\n", NTOP(ip), ntohs(G_parms.default_udp_port));
        del(ip2id(ip));
    };
    
    /** funzioni di aggiunta di un nodo al sistema */
    /**
     * 
     * @param IP
     * @param id
     * @param reinsert
     * @return 
     */
    bool add(const IPaddr IP, const KEYID id, bool reinsert);

    /**
     * aggiunge un nodo con IP e key - non reinserimento
     * @param IP
     * @param id
     * @return 
     */
    bool add(const IPaddr IP, const KEYID id) {
        return add(IP, id, false);
    };

    /**
     * dato un IP, aggiunge la default UDP port ed ottiene un IPaddr sul quale esegue add()
     * @param IP
     * @return 
     */
    bool add(const IP IP) {
        IPaddr IP2;
        IP2.ip = IP;
        IP2.port = G_parms.default_udp_port;
        return add(IP2);
    };

    /**
     * come sopra, con porta esplicitata
     * @param IP
     * @param port
     * @return 
     */
    bool add(const IP IP, const PORTNO port) {
        IPaddr IP2;
        IP2.ip = IP;
        IP2.port = port;
        return add(IP2);
    };

    /**
     * add di un nodo con IP, porta e key note
     * @param IP
     * @param port
     * @param id
     * @return 
     */
    bool add(const IP IP, const PORTNO port, const KEYID id) {
        IPaddr IP2;
        IP2.ip = IP;
        IP2.port = port;
        return add(IP2, id);
    };

    pos_t prednpos(const KEYID id, const int n);

    IPaddr predn(const KEYID id, const int n) {
        pos_t pred = prednpos(id, n);
        return table[pred.bucket].slot[pred.slot];
    };

    IPaddr succn(const KEYID id, const int n);

    IPaddr succn(const IPaddr IP, const int n) {
        return ( succn(ip2id(IP), n));
    };

    bool xeq_table(bool succ, const char* file, const uint line);

    bool xeq_table(const char* file, const uint line) {
        return xeq_table(true, file, line);
    };
    void print_table(FILE* std);

    void print_table() {
        print_table(stderr);
    };
    void *send(const struct msg_hd msg_header, const int connection);

    /**
     * get la variabile privata npeers_
     * @return 
     */
    int npeers() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return npeers_;
    };

    /**
     * set della variabile privata npeers_
     * @param new_npeers
     * @return 
     */
    int npeers(int new_npeers) {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        npeers_ = new_npeers;
        return new_npeers;
    };

    /**
     * 
     * @return il valore di rho, sulla base dei peers del sistema 
     */
    int rho() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return rho(npeers());
    };

    std::list<stab_peers_t> stab_peers;
    bool no_stab;

public:

    IPaddr pred_IP() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return predIP_;
    }

    KEYID pred_ID() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return predID_;
    }

    IPaddr succ_IP() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return succIP_;
    }

    KEYID succ_ID() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return succID_;
    }
    void set_succ_pred();

    //	D1HT_LOOKUP_reply_body 	lookup(const D1HT_LOOKUP_EXTERN_body lookup_extern_body);
    D1HT_LOOKUP_reply_body lookup(D1HT_LOOKUP_body *lookup_bodyp, void* payload, void* reply_payload);
    D1HT_LOOKUP_reply_body lookup(D1HT_LOOKUP_body *lookup_bodyp);

    IPaddr lookup_local(msg_hd *lookup_req_header, const D1HT_LOOKUP_body *lookup_req_body, int connection, const IPaddr *peer, struct sockaddr_in *saddr, const void* payload_buff);

    int pub_npeers() {
        int t_npeers;
        LOCK(rt_mutex);
        t_npeers = npeers_;
        UNLOCK(rt_mutex);
        return t_npeers;
    };

    int pub_locked_npeers() {
        if (G_parms.check) rt_mutex.assert_locked_byme(__FILE__, __LINE__);
        return npeers_;
    };

    /**
     * calcola il valore di rho
     * @param np
     * @return 
     */
    int rho(const int np) {
        return (int) ceilf(log2f((float) np));
    };

    void *ack_msg(eventmsg_t* msgp);
    void *ack_event(const IPaddr IP, const int TTL, const int TYPE);
    void *ack_event_pred(const IPaddr IP, const int TYPE);

    void *stabilize(const IPaddr peer, const char* file, const uint line);
    bool stabilized(const IPaddr host);
    void *pub_stabilize(const IPaddr peer, const char* file, const uint line);
    void *pub_stabilize(const char* file, const uint line);
    void *stabilize_final(const IPaddr peer, const D1HT_STAB_body reply);
    void *stabilize_reply(int *connection, const IPaddr peerIP, const D1HT_STAB_body sent);
    void *stop_stab();
    void *stabilize_close_interval();
    bool pub_stabilized(const IPaddr host);
    void *stabilize_force(IPaddr *skip, join_t *joining_peers);

    bool RT_init(const int connection);
    bool get();
    void *read_table();
    bool pub_send(const struct msg_hd msg_header, int *connection);
    int rt_size;

    void *leave();

    bool pub_xeq_table(bool succ, const char* file, const uint line);

    bool pub_xeq_table(const char* file, const uint line) {
        return pub_xeq_table(true, file, line);
    };
    void *test_table(const char* file, const uint line);
    void dump_table();

    void get_targets(temp_interval_t *targets, const int n);

    /**
     * 
     * @param IP
     * @return 
     */
    bool add(const IPaddr IP) {
        return add(IP, ip2id(IP));
    };
    bool pub_add(const IPaddr IP, const KEYID id);
    bool pub_add(const IPaddr IP);
    void pub_del(const IPaddr IP);
    void pub_del(const IPaddr IP, const bool pred);

    void pub_print_table() {
        return pub_print_table(stderr);
    };

    void pub_print_table(FILE* std) {
        LOCK(rt_mutex);
        print_table(std);
        UNLOCK(rt_mutex);
    }

    routing_table() {

        table = NULL;
        bucket_peers = NULL;

        SeqNo_lookup = 2;
        no_stab = false;
        stab_peers.clear();

        LOCK_INIT(rt_mutex, maychain = true, 30);
    }

    int Emax(const int np, const int rh) {

        ASSERT(np > rh);
        Assert((rh > 0), XEQ_ERROR);

        int retcode = (8 * G_parms.f * np) / (100 * (16 + 3 * rh));

        if (retcode == 0) retcode = 1;
        /*	
                retcode = (4 * G_parms.f * np) / (100 * (8 + rh));
	
                if (retcode == 0) retcode = 1;
         */
        // VERBO(stderr, "Npeers=%u, rho=%u, Emax = %u\n", (uint) np, (uint) rh, (uint) retcode);

        return retcode;
    }

    int Emax(const int np) {
        return Emax(np, rho(np));
    }

    int Emax() {
        int np;
        LOCK(rt_mutex);
        np = npeers();
        UNLOCK(rt_mutex);
        return Emax(np);
    }

    int pub_rho() {
        int retcode;
        LOCK(rt_mutex);
        retcode = rho();
        UNLOCK(rt_mutex);
        return retcode;
    };

    int aprox_rho() {
        return (int) ceilf(log2f((float) npeers_));
    };

    bool pub_find(const IPaddr IP) {
        bool retcode = false;
        LOCK(rt_mutex);
        retcode = find(ip2id(IP));
        UNLOCK(rt_mutex);
        return retcode;
    };

    IPaddr pub_succ(const KEYID id) {
        pos_t dummy1, dummy2, succ;
        LOCK(rt_mutex);
        find(id, &dummy1, &dummy2, &succ);
        UNLOCK(rt_mutex);
        return table[succ.bucket].slot[succ.slot];
    }

    IPaddr pub_succn(const KEYID id, const int n) {
        IPaddr retcode;
        LOCK(rt_mutex);
        retcode = succn(id, n);
        UNLOCK(rt_mutex);
        return retcode;
    }

    IPaddr pub_succ_IP() {
        IPaddr retcode;
        LOCK(rt_mutex);
        retcode = succIP_;
        UNLOCK(rt_mutex);
        return retcode;
    }

    IPaddr pub_succ_IP(const KEYID id) {
        return pub_succn(id, 0);
    }

    IPaddr pub_predn(const KEYID id, const int n) {
        IPaddr retcode;
        LOCK(rt_mutex);
        retcode = predn(id, n);
        UNLOCK(rt_mutex);
        return retcode;
    }

    IPaddr pub_pred_IP() {
        IPaddr retcode;
        LOCK(rt_mutex);
        retcode = predIP_;
        UNLOCK(rt_mutex);
        return retcode;
    }

    IPaddr pub_pred_IP(const KEYID id) {
        return pub_predn(id, 0);
    }

    KEYID pub_succ_ID() {
        KEYID retcode;
        LOCK(rt_mutex);
        retcode = succID_;
        UNLOCK(rt_mutex);
        return retcode;
    }

    KEYID pub_pred_ID() {
        KEYID retcode;
        LOCK(rt_mutex);
        retcode = predID_;
        UNLOCK(rt_mutex);
        return retcode;
    }

private:

    IPaddr succ(const KEYID id) {
        pos_t dummy1, dummy2, succ;

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling succ.");

        find(id, &dummy1, &dummy2, &succ);
        return table[succ.bucket].slot[succ.slot];
    }

    bool find(const KEYID id, pos_t *peer) {
        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling find.");
        pos_t dummy1, dummy2;
        return (find(id, &dummy1, &dummy2, peer));
    }

    pos_t get_next_slot(const int bucket) {
        pos_t retcode;
        int bucketx = bucket;
        int slot;
        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling get_next_slot.");

        while (table[bucketx].slot[BUCKETSIZE - 1].ip.s_addr != 0) bucketx = (bucketx + 1) % rt_size;

        for (slot = bucket_peers[bucketx]; table[bucketx].slot[slot].ip.s_addr != 0; slot++);

        retcode.bucket = bucketx;
        retcode.slot = slot;
        return retcode;
    }

    void find_pred(const int bucket, pos_t *pred, pos_t *predl) {
        int bucketx = bucket;

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling find_pred.");

        do {
            bucketx = (bucketx - 1);
            if (bucketx < 0) bucketx = rt_size - 1;
        } while (bucket_peers[bucketx] == 0);

        predl->bucket = bucketx;
        predl->slot = bucket_peers[bucketx] - 1;
        if (bucket_peers[bucketx] <= BUCKETSIZE) {
            pred->bucket = bucketx;
            pred->slot = bucket_peers[bucketx] - 1;
        } else {
            int slot = -1;
            int peers_found = BUCKETSIZE;
            int bpeers2 = bucket_peers[bucketx];
            int bucket2 = bucketx;
            while (peers_found != bpeers2) {
                bucket2 = (bucket2 + 1) % rt_size;
                for (slot = bucket_peers[bucket2]; peers_found != bpeers2 && slot < BUCKETSIZE; slot++)
                    if (table[bucket2].slot[slot].ip.s_addr && ((uint) bucketx == ip2id(table[bucket2].slot[slot]) / basepos)) peers_found++;
                slot--;
            }
            ASSERT(slot >= 0 && slot < BUCKETSIZE);
            pred->bucket = bucket2;
            pred->slot = slot;
        }
    }
};

extern routing_table G_RT;

#endif /*RT_H_*/
