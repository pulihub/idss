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
 * @file RT_pred_succ.c
 * @brief gestione di successori e predecessori della routing table
 */
#include "D1HT.h"

namespace d1ht {

    void routing_table::set_succ_pred() {

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling set_succ_pred.");

        pos_t pred, succ, dummy;
        KEYID tempID;

        if (npeers() <= 1) {
            succIP_ = predIP_ = G_parms.myIP;
        } else {
            tempID = G_myID + 1;
            find(tempID, &pred, &dummy, &succ);
            succIP_ = table[succ.bucket].slot[succ.slot];

            tempID = G_myID - 1;
            find(tempID, &pred, &dummy, &succ);
            predIP_ = table[pred.bucket].slot[pred.slot];
        }

        succID_ = ip2id(succIP_);
        predID_ = ip2id(predIP_);

#if defined(DEBUG)
        // VERBOSEX(stderr, "New pred=%u (%s:%u), new succ=%u (%s:%u).\n", predID_, IPtoP(predIP_), succID_, IPtoP(succIP_));
        // if (G_parms.check) XEQ_TRUE(xeq_table(true,__FILE__,__LINE__), "RT corrupted at set_succ_pred.", XEQ_ERROR);
#endif
    }

    /**
     * calcolo del successore di ordine n, data una Key
     * @param id
     * @param n
     * @return
     * 
     * please note that if there's a peer with ID=id, then succn(id,0) will be this peer,
       but if there is not a peer with ID=id, then succn(id,0) will be the peer with the next ID after id in the ring  
     */
    IPaddr routing_table::succn(const KEYID id, const int n) {
        
        int bucketx = (int) (id / basepos);
        bucket_t *entry = &table[bucketx];
        pos_t pred, predl, succ;
        int offset = n % npeers();

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling succn.");

        bool found = find(id, &pred, &predl, &succ);

#if defined(DEBUG)
        if (G_succ_pred_err) {
            VERBOSE(stderr, "id = %u, n=%d, offset=%d\n", id, n, offset);
            VERBOSE(stderr, "succ bucket=%d, slot=%d\n", succ.bucket, succ.slot);
            VERBOSE(stderr, "predl bucket=%d, slot=%d\n", predl.bucket, predl.slot);
        }
#endif

        if (offset == 0) return table[succ.bucket].slot[succ.slot];

        int lbucket = predl.bucket;
        int lslot = offset + predl.slot;

        if (!found) lslot++;

        while (lslot > bucket_peers[lbucket] - 1) {
            lslot -= bucket_peers[lbucket];
            lbucket = (lbucket + 1) % rt_size;
        }

        int bucket = lbucket;
        int slot = lslot;
        lslot = lslot - BUCKETSIZE + 1;

        while (lslot > 0) {
            bucket = (bucket + 1) % rt_size;
            entry = &table[bucket];

            for (slot = bucket_peers[bucket]; slot < BUCKETSIZE && lslot && entry->slot[slot].ip.s_addr; slot++)
                if ((ip2id(entry->slot[slot]) / basepos) == (uint) lbucket) lslot--;
            slot--;
        }
        //	if (G_succ_pred_err) VERBOSE(stderr, "bucket=%d, slot=%d\n", bucket, slot); 
        return table[bucket].slot[slot];
    }

    
    /**
     * calcolo del predecessore di ordine n
     * @param id
     * @param n
     * @return 
     * 
     *  please note that if there's a peer with ID=id, then predpos(id,0) will be this peer,
        but if there is not a peer with ID=id, then predpos(id,0) will be the peer with the previous ID before id in the ring 
        in any case, for n>0, predpos(id,n) is the predecessor of predpos(id,n-1)
     */
    pos_t routing_table::prednpos(const KEYID id, const int n) {
              
        pos_t retcode;
        pos_t dummy1, predl, dummy2;
        int offset = n % npeers();

        ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling prednpos.");

        find(id, &dummy1, &predl, &dummy2);
        //	if (G_succ_pred_err) { VERBOSE(stderr, "id = %u, predl bucket=%d, slot=%d, n=%d, offset=%d\n", id, predl.bucket, predl.slot, n, offset); }

        retcode.bucket = predl.bucket;
        retcode.slot = predl.slot - offset;

        if (offset <= predl.slot) {
            retcode.bucket = predl.bucket;
            retcode.slot = predl.slot - offset;
            //		if (G_succ_pred_err) { VERBOSE(stderr,"X pred id=%u, n=%u = [%u,%u]\n", id, offset, retcode.bucket, retcode.slot); }
            return posl2pos(retcode);
        }

        int lslot = offset - predl.slot;
        int lbucket = predl.bucket - 1;
        if (lbucket < 0) lbucket = rt_size - 1;

        while (lslot > bucket_peers[lbucket]) {
            lslot -= bucket_peers[lbucket];
            lbucket = lbucket - 1;
            if (lbucket < 0) lbucket = rt_size - 1;
        }
        lslot = bucket_peers[lbucket] - lslot;

        //	if (G_succ_pred_err) { VERBOSE(stderr,"Y pred id=%u, n=%u = [%u,%u]\n", id, offset, lbucket, lslot); }
        retcode.bucket = lbucket;
        retcode.slot = lslot;

        return posl2pos(retcode);
    }

} // namespace d1ht
