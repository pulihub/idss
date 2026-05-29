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
 * @file RT_other.c
 * @brief implementazione delle funzioni di gestione della RT
 */
#include "D1HT.h"

namespace d1ht {

    /**
     * invocata dal nodo quando esegue il leave dal sistema: invia un LEAVE al suo successore
     * @return 
     */
    void *routing_table::leave() {
        int connection, iret;
        IPaddr succ;

        // when leaving, for our first try we won't even bother to get rt_mutex
        succ = succIP_;

        iret = TCP_CONNECTION(connection, 0, succ, XEQ_SILENT);
        if (iret == 0 && connection >= 0) {

            SEND_HEADER_SEQNO(connection, D1HT_LEAVE, D1HT_LEAVE, XEQ_WARNING);

        } else {

            bool should_unlock = false;
            if (!rt_mutex.xeq_locked_byme()) {
                LOCK_FORCED(rt_mutex);
                should_unlock = true;
            }
            succ = succ_IP();
            if (should_unlock) UNLOCK(rt_mutex);

            TCP_CONNECTION(connection, 0, succ, XEQ_SILENT);
            if (connection >= 0) {
                SEND_HEADER_SEQNO(connection, D1HT_LEAVE, D1HT_LEAVE, XEQ_WARNING);
            } else { // try twice
                TCP_CONNECTION(connection, 0, succ, XEQ_WARNING);
                SEND_HEADER_SEQNO(connection, D1HT_LEAVE, D1HT_LEAVE, XEQ_WARNING);
            }
        }
        D1HT_CLOSE(connection);
        return NULL;
    }

    /**
     * Dalla connection riceve la routing table e la bucket_peers
     * @param connection
     * @return 
     */
    bool routing_table::RT_init(const int connection) {

        if (G_parms.NMAX == 0) EXIT("N should be greater than 0");

        int avg_bucket_occupance = (int) exp2((float) floor(log2((float) BUCKETSIZE)));

        rt_size = G_parms.NMAX / avg_bucket_occupance;
        basepos = (int) exp2((float) (8 * sizeof (KEYID) - log2((float) rt_size)));

        //	PE(basepos);PE((8*sizeof(KEYID)-log2((float) rt_size)));
        //	PE(BUCKETSIZE);PE(avg_bucket_occupance);PE(rt_size);
        ASSERT(basepos > 0);

        if (bucket_peers != NULL) FREE(bucket_peers);
        if (table != NULL) FREE(table);

        table = CALLOC(rt_size, bucket_t);
        bucket_peers = CALLOC(rt_size, unsigned char);

#ifdef D1HT_JOIN_COPY_RT
        if (bucket_peers_copy != NULL) FREE(bucket_peers_copy);
        if (table_copy != NULL) FREE(table_copy);

        table_copy = CALLOC((rt_size + 1), bucket_t);
        bucket_peers_copy = CALLOC((rt_size + 1), unsigned char);
#endif

        if (connection) {
            ssize_t bytes_read;

            XEQ_EXEC(bytes_read = readn(connection, table, rt_size * sizeof (bucket_t)), XEQ_WARNING); // Receive the RT

            if (bytes_read != (ssize_t) (rt_size * sizeof (bucket_t))) return false;

            if (G_exiting) return false;

            XEQ_EXEC(bytes_read = readn(connection, bucket_peers, rt_size * sizeof (unsigned char)), XEQ_WARNING); // Receive the RT

            if (bytes_read != (ssize_t) (rt_size * sizeof (unsigned char))) return false;
            // print_table();
        } else if (G_parms.input_RT) {
            read_table();
        } else {
            npeers(0);
        }

        add(G_parms.myIP);

        set_succ_pred();
#ifdef DEBUG
        if (G_parms.check) XEQ_TRUE(xeq_table(true, __FILE__, __LINE__), "RT corrupted at initialization.", XEQ_ERROR);
#endif
        return true;
    }

    /**
     * legge la routing table da file
     * @return 
     */
    void* routing_table::read_table() {
        FILE* std;
        char filename[256];

        if (G_parms.RT_file == NULL) {
            sprintf(filename, "table_%s%s.txt", G_parms.program_name, G_parms.display_name);
            G_parms.RT_file = filename;
        }

        VERBO(stderr, "Reading the routing table from file %s\n", G_parms.RT_file);

        if ((std = fopen(G_parms.RT_file, "r")) == NULL) {
            EXIT("Unable to open the file with the IP addresses of the initial routing table (%s).", G_parms.RT_file);
        }

        bool myself = false;
        int np;
        int iret;

        if ((iret = fscanf(std, "npeers=%u", &np)) != 1) {
            if (iret < 0) {
                EXIT("Problem while reading npeers from initial routing table, retcode=%d (%s).", iret, strerror(errno));
            } else {
                EXIT("Problem while reading npeers from initial routing table.");
            }
        }
        npeers(np);

        for (int i = 0; i < np; i++) {
            IPaddr peer;
            char IP[30];

            iret = fscanf(std, "%s", IP);

            if (iret < 0) EXIT("Problem while reading the initial routing table, retcode=%d (%s).", iret, strerror(errno));

            if (iret != 1) EXIT("Problem while reading the file with the IP addresses of the initial routing table.")
                if (get_IPaddr(IP, &peer) != 0) EXIT("Malformed peer address in the file with the IP addresses of the initial routing table.");
            if (peer.port == (PORTNO) 0) peer.port = G_parms.default_udp_port;

            add(peer, ip2id(peer), true);
            if (peer == G_parms.myIP) myself = true;
        }
        if (!myself) {
            add(G_parms.myIP, G_myID, true);
            npeers(np + 1);
        }

        if (G_parms.wait_CONT) {

            sigset_t wait_SIGCONT;

            XEQ_EXEC(sigfillset(&wait_SIGCONT), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGCONT), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGTERM), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGINT), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGILL), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGBUS), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGSEGV), XEQ_ERROR);
            XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGALRM), XEQ_ERROR);

            verbose(stderr, "Routing table read and initialized. Waiting for SIGCONT to start\n");
            sigsuspend(&wait_SIGCONT); // block waiting SIGCONT
            verbose(stderr, "SIGCONT catched. Continuing\n");

            PTXEQ(pthread_sigmask(SIG_SETMASK, &G_parms.sigmask_original, NULL), XEQ_ERROR);

        } else {
            verbose(stderr, "Routing table read and initialized.\n");
        }

        return NULL;
    }

    /**
     * stampa la routing table sul file std
     * @param std
     */
    void routing_table::print_table(FILE* std) {

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling print_table.");

        VERBO(std, "PRINT TABLE npeers=%d, rho=%d, G_myID=%.10u, myIP=%s:%u\n", npeers(), rho(), G_myID, IPtoP(G_parms.myIP));
//#if defined(DEBUG)
        VERBO(std, "PRINT TABLE pred=%.10u (%s:%u), succ=%.10u (%s:%u)\n", pred_ID(), IPtoP(pred_IP()), succ_ID(), IPtoP(succ_IP()));
        for (uint k = 0; k < (uint) rt_size; k++) {
            bucket_t *entry = &table[k];

            if (entry->slot[0].ip.s_addr == 0) continue;

            VERBO(std, "bucket=%d, bpeers=%d: ", k, bucket_peers[k]);

            for (uint j = 0; j < BUCKETSIZE; j++) {
                if ((entry->slot[j].ip.s_addr)) {
                    KEYID key = ip2id(entry->slot[j]);
                    fprintf(std, "(%.10u, %s:%u) ", key, IPtoP(entry->slot[j]));
                    ASSERT((j >= bucket_peers[k]) || (key / basepos == k));
                }
            }
            fprintf(std, "\n");
        }
//#endif
    }

    /**
     * esegue il dump della routing table su file
     */
    void routing_table::dump_table() {

        FILE *fdump;
        char filename[256];

        sprintf(filename, "table_%s%sdump.txt", G_parms.program_name, G_parms.display_name);

        if ((fdump = fopen(filename, "w")) != NULL) {

            VERBOSE(stderr, "Dumping the routing table to %s\n", filename);
            //		VERBO(fdump, "DUMP TABLE G_myID=%u, myIP=%s:%u\n", G_myID, IPtoP(G_parms.myIP));

            LOCK(rt_mutex);

            //		VERBO(fdump, "DUMP TABLE npeers=%d, rho=%d\n", npeers(), rho());
            fprintf(fdump, "npeers=%d\n", npeers());

            for (int k = 0; k < rt_size; k++) {

                if (bucket_peers[k] == 0) continue;

                bucket_t *entry = &table[k];
                KEYID key = 0;
                int j;

                for (j = 0; j < bucket_peers[k] && j < BUCKETSIZE; j++) {
                    ASSERT(entry->slot[j].ip.s_addr);
                    key = ip2id(entry->slot[j]);
                    fprintf(fdump, "%u, %.10u, %s:%u\n", key / basepos, key, IPtoP(entry->slot[j]));
                }
                for (; j < bucket_peers[k]; j++) {
                    IPaddr next = succn(key, 1);
                    ASSERT(next.port);
                    key = ip2id(next);
                    fprintf(fdump, "%u, %.10u, %s:%u\n", key / basepos, key, IPtoP(next));
                }
            }

            UNLOCK(rt_mutex);
            fprintf(fdump, "\n");
            fclose(fdump);
        } else {
            EXIT("Unable to dump the routing table\n");
        }
    }

    /**
     * find()
     * @param id
     * @param pred
     * @param predl
     * @param succ
     * @return 
     * 
     * // ** ATTENTION **: the caller MUST hold rt_mutex, and the positions returned are only valid while  rt_mutex is hold!!!
     * 
    // This function returns:
    //
    // 		(a) if there is a peer with ID == id
    //			(a.1) the function returns true
    //			(a.2) both pred and succ will have the physical position in the RT of this peer (i.e. pred will be equal to succ)
    //			(a.3) predl will have the logical position in the RT of this peer
    // 		(b) if there is NOT a peer with ID == id
    //			(b.1) the function returns false
    //			(b.2) pred will have the physical position of the peer with the biggest ID  | ID < id 
    //			(b.3) predl will have the logical position of the peer with the biggest ID  | ID < id 
    //			(b.4) succ will have the physical position of the peer with the smallest ID | ID > id 
    //
    // Note that by logical position we mean the position in the RT the peer would have if we had infinite buckets
     */
    bool routing_table::find(const KEYID id, pos_t *pred, pos_t *predl, pos_t *succ) {

        bool retcode = false;
        int slot;
        int bucketx = (int) (id / basepos);
        int bucketo = bucketx;
        int bpeers = bucket_peers[bucketx];

        KEYID id2 = 0;
        bucket_t *entry = &table[bucketx];

#ifdef DEBUG
        //if (G_xeq_find) { VERBO(stderr, "find id=%u, bucketx=%u, basepos=%u\n", id, bucketx, basepos); }
        if (G_parms.check) ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling find.");
#endif

        int bpeers0 = MIN(bpeers, BUCKETSIZE);
        for (slot = 0; slot < bpeers0 && id > (id2 = ip2id(entry->slot[slot])); slot++);
        //	if (G_xeq_find) { VERBO(stderr, "find id2=%u, bucketx=%u, slot=%u\n", id2, bucketx, slot); }

        if (bpeers == 0) {
            //		if (G_xeq_find) {  }
            retcode = false;
            find_pred(bucketx, pred, predl);
            do bucketx = (bucketx + 1) % rt_size; while (bucket_peers[bucketx] == 0);
            succ->slot = 0;
            succ->bucket = bucketx;
        } else if (id == id2) {
            //		if (G_xeq_find) {  }
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
            XEQ_TRUE(((slot >= BUCKETSIZE) || (bpeers == 0)), "Error in G_RT.find.", XEQ_ERROR);
#endif
            retcode = true;
            pred->bucket = predl->bucket = succ->bucket = bucketx;
            pred->slot = predl->slot = succ->slot = slot;
        } else if (id < id2) {
            //		if (G_xeq_find) {  }
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
            XEQ_TRUE((slot >= BUCKETSIZE), "Error in G_RT.find.", XEQ_ERROR);
#endif
            retcode = false;
            succ->bucket = bucketx;
            succ->slot = slot;
            if (slot) {
                //			if (G_xeq_find) {  }
                pred->bucket = predl->bucket = bucketx;
                pred->slot = predl->slot = slot - 1;
            } else {
                //			if (G_xeq_find) {  }
                find_pred(bucketx, pred, predl);

            }
        } else if (bpeers <= BUCKETSIZE) {
            //		if (G_xeq_find) {  }
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
            XEQ_TRUE((slot != bpeers), "Error in G_RT.find.", XEQ_ERROR);
#endif
            retcode = false;
            pred->bucket = predl->bucket = bucketx;
            pred->slot = predl->slot = bpeers - 1;
            do bucketx = (bucketx + 1) % rt_size; while (bucket_peers[bucketx] == 0);
            succ->slot = 0;
            succ->bucket = bucketx;
        } else {
            //		if (G_xeq_find) {  }
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
            XEQ_TRUE(((slot != BUCKETSIZE) || (id <= id2)), "Error in G_RT.find.", XEQ_ERROR);
#endif
            int peers_found = BUCKETSIZE;
            int bucketx_o = bucketx;
            int slot_o = BUCKETSIZE - 1;

            while ((id > id2) && (peers_found != bpeers)) {
                bucketx = (bucketx + 1) % rt_size;
                for (slot = bucket_peers[bucketx]; id > id2 && slot < BUCKETSIZE && peers_found != bpeers; slot++) {
                    //				if (G_xeq_find) { VERBO(stderr, "bucketx=%u, slot=%u, peers_found=%u, bucketx_o=%u, slot_o=%u \n", bucketx, slot, peers_found, bucketx_o, slot_o); }
                    if (table[bucketx].slot[slot].ip.s_addr == 0) continue;
                    if ((uint) bucketo == ip2id(table[bucketx].slot[slot]) / basepos) {
                        id2 = ip2id(table[bucketx].slot[slot]);
                        peers_found++;
                        if (id == id2) {
                            retcode = true;
                            pred->bucket = succ->bucket = bucketx;
                            pred->slot = succ->slot = slot;
                            predl->bucket = bucketo;
                            predl->slot = peers_found - 1;
                        } else if (id < id2) {
                            retcode = false;
                            succ->bucket = bucketx;
                            succ->slot = slot;
                            predl->bucket = bucketo;
                            predl->slot = peers_found - 2;
                            pred->bucket = bucketx_o;
                            pred->slot = slot_o;
                        } else if (peers_found == bpeers) {
                            retcode = false;
                            predl->bucket = bucketo;
                            predl->slot = peers_found - 2;
                            pred->bucket = bucketx_o;
                            pred->slot = slot_o;

                            bucketx = bucketo;
                            do bucketx = (bucketx + 1) % rt_size; while (bucket_peers[bucketx] == 0);

                            succ->slot = 0;
                            succ->bucket = bucketx;
                        }
                        bucketx_o = bucketx;
                        slot_o = slot;
                    }
                }
            }
        }
        return retcode;
    }

    /**
     * 
     * @param posl
     * @return 
     *  
     * this function converts a "logical" position in the RT to a "real" position
        Note that by logical position we mean the position in the RT the peer would have if we had infinite buckets
     */
    pos_t routing_table::posl2pos(const pos_t posl) {

        pos_t retcode;
#ifdef DEBUG
        if (G_parms.check) {
            ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling posl.");
            XEQ_TRUE((posl.bucket >= rt_size), "Wrong bucket number.", XEQ_ERROR);
            XEQ_TRUE((posl.slot >= bucket_peers[posl.bucket]), "Wrong slot number.", XEQ_ERROR);
        }
#endif
        // the logical position only differs from the real one when the bucket is not big enough to store all collisions
        if (posl.slot < BUCKETSIZE) return posl;

        int bucket = posl.bucket;
        int slot = posl.slot;
        int peers_found = BUCKETSIZE;

        while (posl.slot >= peers_found) {
            bucket = (bucket + 1) % rt_size;
            bucket_t *entry = &table[bucket];

            for (slot = bucket_peers[bucket]; slot < BUCKETSIZE && posl.slot >= peers_found && entry->slot[slot].ip.s_addr; slot++)
                if ((ip2id(entry->slot[slot]) / basepos) == (uint) posl.bucket) peers_found++;
            slot--;
        }
        retcode.bucket = bucket;
        retcode.slot = slot;
        return retcode;
    }

    
    /**
     * calcola i rho target cui EDRA è destinata
     * @param targets
     * @param n
     */
    void routing_table::get_targets(temp_interval_t *targets, const int n) {

        int offset = 1;
        KEYID id;
        int nloop = n;

        LOCK(rt_mutex);
#if defined(DEBUG)
        // if (G_parms.VERBOSE) print_table(stderr);
#endif

        if (nloop > rho()) nloop = rho();

        targets[0].target = succ_IP();
        id = succ_ID();

        for (int i = 1; i < nloop; i++) {
            targets[i].target = succn(id, offset);
            id = ip2id(targets[i].target);
            offset = 2 * offset;
        }
        UNLOCK(rt_mutex);
    }

} // namespace d1ht

