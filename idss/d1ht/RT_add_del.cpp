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
 * @file RT_add_del.c
 * @brief funzioni di modifica della RT (add e delete di nodi)
 * 
 */

#include "D1HT.h"

namespace d1ht {

    extern join_t G_joining_peers;

    /**
     * metodo pubblico per aggiungere un nodo joining
     * @param IP
     * @return 
     */
    bool routing_table::pub_add(const IPaddr IP) {
        KEYID id = ip2id(IP);
        return pub_add(IP, id);
    };

    /**
     * metodo pubblico per aggiungere un nodo dati IP e Key
     * @param IP
     * @param id
     * @return 
     */
    bool routing_table::pub_add(const IPaddr IP, const KEYID id) {
        if (IP.port == 0) return false;
        LOCK(rt_mutex);
        bool retcode = add(IP, id, false);
        UNLOCK(rt_mutex);
        return retcode;
    }

    /**
     * rimozione di un nodo dalla routing table
     * @param IP
     */
    void routing_table::pub_del(const IPaddr IP) {
        pub_del(IP, false);
    }

    /**
     * rimozione di un nodo dalla routing table:
     * Se il nodo è mio predecessore, oltre ad eliminarlo dalla routing table faccio partire EDRA
     * @param IP
     * @param pred_for_sure
     */
    void routing_table::pub_del(const IPaddr IP, const bool pred_for_sure) {

        if (IP.port == 0) return;

        LOCK(rt_mutex);
        if (pred_for_sure || pred_IP() == IP) {
            del(IP);
            stabilized(IP);
            UNLOCK(rt_mutex);
            G_probe.probed(IP);
            //ack_event(IP, D1HT_RHO_MAX, LEAVES);
            ack_event_pred(IP, LEAVES);
            G_joining_peers.stop_forward(IP);
        } else {
            del(IP);
            UNLOCK(rt_mutex);
        }
    }

    /**
     * Elimino il nodo dalle mie strutture dati. In più, 
     * - se il nodo è mio successore, aggiorno l'elenco dei miei succ
     * - se il nodo è mio predecessore, avviso il sistema (EDRA)
     * @param id
     * @return 
     * // the caller MUST hold rt_mutex
     */
    void* routing_table::del(const KEYID id) 
    {
        ASSERT(basepos > 0);
        int bucketx = (int) (id / basepos);
        int bucketo = bucketx;
        int bpeers = bucket_peers[bucketx];
        int slot;
        pos_t peer, peerl, dummy;
        IPaddr addr_removed;

        if (id == G_myID) return NULL;
#ifdef DEBUG
        VERBOSE(stderr, "XXX del %u\n ", id);
        if (G_parms.check) {
            //		XEQ_TRUE(xeq_table(false,__FILE__,__LINE__), "RT corrupted before del.", XEQ_ERROR);
            //		PE(id);PE(bucketx);PE(bpeers);
            ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling del.");
        }
#endif
        if (find(id, &peer, &peerl, &dummy)) {
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
            XEQ_TRUE((peerl.bucket != bucketx), "G_RT.del error.", XEQ_ERROR);
#endif
            bucket_t *entry = &table[peer.bucket];
            addr_removed = entry->slot[peer.slot];

            //	fprintf(stderr," ZZZZ npeers=%u, G_myID=%u, pred_ID()=%u, succ_ID()=%u, id=%u\n", npeers(), G_myID, pred_ID(), succ_ID(), id);
            //	fprintf(stderr, "\n =====> DEL FIND id=%u, pred=[%u,%u], predl=[%u,%u]\n",
            //			id, peer.bucket, peer.slot, peerl.bucket, peerl.slot, dummy.bucket, dummy.slot); 

            for (slot = peer.slot + 1; slot < BUCKETSIZE; slot++) entry->slot[slot - 1] = entry->slot[slot];
            entry->slot[BUCKETSIZE - 1].ip.s_addr = 0;
            bucket_peers[bucketx]--;

            if (bpeers > BUCKETSIZE && peer.bucket == bucketo) {
                int count = 0;
                bool todo = true;

                while (todo && count != rt_size) {
                    count++;
                    bucketx = (bucketx + 1) % rt_size;
                    entry = &table[bucketx];

                    for (slot = bucket_peers[bucketx]; todo && slot < BUCKETSIZE; slot++) {
                        if (ip2id(entry->slot[slot]) / basepos == (uint) bucketo) {
                            todo = false;
                            table[bucketo].slot[BUCKETSIZE - 1] = entry->slot[slot];
                            for (slot = slot + 1; slot < BUCKETSIZE; slot++) entry->slot[slot - 1] = entry->slot[slot];
                            entry->slot[BUCKETSIZE - 1].ip.s_addr = 0;
                        }
                    }
                }
#if defined(XEQP_THREAD) && defined(XEQP_LEVEL3)
                XEQ_TRUE(todo, "Spin loop in G_RT.del.", XEQ_ERROR);
#endif
            }

            //aggiorno il numero di peers del sistema
            npeers(npeers() - 1);
            if (npeers() == 1) {
                succIP_ = predIP_ = G_parms.myIP;
                succID_ = predID_ = G_myID; //ci sono solo io
            } else if (id == succID_) { 
                //era il mio succ: cerco il nuovo succ (per key)
                pos_t pred, succ, dummy;
                KEYID tempID = G_myID + 1;
                find(tempID, &pred, &dummy, &succ);
                succIP_ = table[succ.bucket].slot[succ.slot];
                succID_ = ip2id(succIP_);
            } else if (id == predID_) {
                //era il mio predecessore: invoco stabilize()
                pos_t pred, succ, dummy;
                KEYID tempID = G_myID - 1;
                find(tempID, &pred, &dummy, &succ);
                predIP_ = table[pred.bucket].slot[pred.slot];
                predID_ = ip2id(predIP_);
                //			stabilize(predIP_,__FILE__,__LINE__);
                //			stabilize_force(&predIP_, &G_joining_peers);
                stabilize_force(NULL, &G_joining_peers);
                stabilize(predIP_, __FILE__, __LINE__);
            }
        }

#if defined(DEBUG)
        //	PE(G_stats.total.stats[stat_dels_dup);
        //	if (G_stats.total.stats[stat_dels == 5) {fprintf(stderr, "bp = %u \n", bucket_peers[7]);bucket_peers[7]++; print_table();}
        //	if (G_parms.check) XEQ_TRUE(xeq_table(true,__FILE__,__LINE__), "RT corrupted after del.", XEQ_ERROR);
        //  print_table();
        if (find(id, &peer, &peerl, &dummy)) {
            bucket_t *entryx = &table[peer.bucket];
            IPaddr IPx = entryx->slot[peer.slot];
            KEYID idx = ip2id(IPx);
            // print_table();
            fprintf(stderr, "DEL FIND2 id deleted=%u, id found=%u, IP=%s:%u, bucket=%u, slot=%u\n", (uint) id, (uint) idx, IPtoP(IPx), (uint) peer.bucket, (uint) peer.slot);
            D1HT_ERROR("RT del error", "entry not deleted", XEQ_ERROR);
        }
#endif
        return NULL;
    }

    
    
    /**
     * Aggiunge un nodo nella routing table, alla posizione data dal calcolo del bucket
     * @param IP
     * @param id
     * @param reinsert
     * @return 
    // the caller MUST hold rt_mutex 
     */
    bool routing_table::add(const IPaddr IP, const KEYID id, bool reinsert) {

#ifdef DEBUG	
        if (IP.port == 0 && G_noport0) return false;
        VERBOSEX(stderr, "Adding %s:%d, id=%u, reinsert=%u\n", IPtoP(IP), id, reinsert);
        if (G_noport0) ASSERT(IP.port != 0 && IP.ip.s_addr != 0);
        if (G_parms.check) {
            ASSERT(ip2id(IP) == id);
            ASSERT(basepos > 0);
            //		if (!reinsert) XEQ_TRUE(xeq_table(true,__FILE__,__LINE__), "RT corrupted before add.", XEQ_ERROR); 
            ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling add.");
        }
#else
        if (IP.port == 0 || IP.ip.s_addr == 0) return false;
#endif

        int bucketx = (int) (id / basepos);
        int bucketo = bucketx;
        int bpeers = bucket_peers[bucketx];
        bucket_t *entry = &table[bucketx];
        int slot;
        pos_t pred, predl, succ, ins;
        KEYID id2;
        bool retcode = true;

        ins.bucket = bucketo;

        if (npeers() == 0) {
            ins.slot = 0;

        } else if (!reinsert && id == pred_ID()) {
            G_probe.probed(pred_IP());
            return false;

        } else if (!reinsert && id == succ_ID()) {
            return false;

        } else if (bpeers == 0) {
            ins.slot = 0;

        } else if (bpeers <= BUCKETSIZE) {
            for (slot = 0; slot < bpeers && ip2id(entry->slot[slot]) < id; slot++);

            if (slot == BUCKETSIZE) {
                ins = get_next_slot(bucketx);
            } else if (id == ip2id(entry->slot[slot])) {
                return false;
            } else {
                ins.slot = slot;
            }

        } else if (find(id, &pred, &predl, &succ)) {
            return false;

        } else if ((predl.slot != bpeers - 1) && ((id2 = ip2id(table[succ.bucket].slot[succ.slot])) / basepos == (uint) bucketo) && (id2 > id)) {
            ins.bucket = succ.bucket;
            ins.slot = succ.slot;

        } else if (((id2 = ip2id(table[succ.bucket].slot[succ.slot])) / basepos == (uint) bucketo) && (id2 > id)) {
            ins.bucket = succ.bucket;
            ins.slot = succ.slot;

        } else {
            ins = get_next_slot(bucketx);
        }

        //	fprintf(stderr," ZZZZ npeers=%u, G_myID=%u, pred_ID()=%u, succ_ID()=%u, id=%u\n", npeers(), G_myID, pred_ID(), succ_ID(), id);
        //	fprintf(stderr, "\n =====> ADD FIND id=%u, pred=[%u,%u], predl=[%u,%u], succ=[%u,%u], ins=[%d,%d]\n",
        //			id, pred.bucket, pred.slot, predl.bucket, predl.slot, succ.bucket, succ.slot, ins.bucket, ins.slot); 

        ASSERT(ins.slot >= 0 && ins.slot < BUCKETSIZE);
        bucket_peers[bucketo]++;
        entry = &table[ins.bucket];
        IPaddr IPant = entry->slot[BUCKETSIZE - 1];
        for (slot = BUCKETSIZE - 1; slot > ins.slot; slot--) entry->slot[slot] = entry->slot[slot - 1];
        entry->slot[ins.slot] = IP;

        if (IPant.ip.s_addr) {
            VERBOSE(stderr, "reinsert IP=%s:%u, IPant=%s:%u, reinsert=%u\n", IPtoP(IP), IPtoP(IPant), reinsert);
            KEYID id_ant = ip2id(IPant);
            int bucketx_i = (int) (id_ant / basepos);
            bucket_peers[bucketx_i]--;
            add(IPant, id_ant, true);
        }
        //   	VERBO(stderr,"Adding IP=%s:%u, pred=%s:%u, succ=%s:%u\n", IPtoP(IP), IPtoP(predIP_), IPtoP(succIP_));
        if (reinsert == false) {

            //aggiorno il numero di peers del sistema
            npeers(npeers() + 1);

            Assert((npeers() <= G_parms.NMAX), XEQ_ERROR);
            if (npeers() == 2) {
                //se siamo in 2, il nuovo nodo è il mio succ e prec
                succID_ = id; 
                succIP_ = IP;
                predID_ = id;
                predIP_ = IP;
            } else if (INSTRETCH(id, G_myID, succID_)) {
                succID_ = id;
                succIP_ = IP;
            } else if (INSTRETCH(id, predID_, G_myID)) {
                predID_ = id;
                predIP_ = IP;
                stabilize_force(NULL, &G_joining_peers);
                stabilize(predIP_, __FILE__, __LINE__);
            }
        }
        //  VERBO(stderr,"Adding IP=%s:%u, pred=%s:%u, succ=%s:%u\n", IPtoP(IP), IPtoP(predIP_), IPtoP(succIP_));

        // 	if (callback_added) {
        //		IPaddr succ = succn(ip2id(IP), 1);
        // 		callback_added(IP, succ);
        //	}

#ifdef DEBUG
        ASSERT(bucketx == bucketo);
        if (G_parms.check && !reinsert) {

            //		if (id!=G_myID) XEQ_TRUE(xeq_table(true,__FILE__,__LINE__), "RT corrupted after add.", XEQ_ERROR);
            XEQ_TRUE((!find(id, &pred, &predl, &succ)), "G_RT.add error.", XEQ_ERROR);
            //	print_table(); 
            if (G_noport0) ASSERT(ntohs(IP.port) != 0);
        }
#endif

        return retcode;
    }

} // namespace d1ht
