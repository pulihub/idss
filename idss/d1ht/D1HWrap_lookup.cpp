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
 * @file D1HWrap_lookup.c
 * @brief Implementazione del thread lookup_thread 
 */
#include "D1HWrap.h"

namespace d1ht {

    PTHREAD(lookup_thread) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        struct lookup_parms_t *lookup_parms;

        PTHREAD_PARM_ASSIGN(lookup_parms);

        int phase = G_phase;

        unsigned short xsubi[3];

        xsubi[0] = (unsigned short) ((unsigned int) getpid() % 65535);
        xsubi[1] = (unsigned short) ((unsigned int) G_myID % 65535);
        xsubi[2] = (unsigned short) ((unsigned int) G_parms.myIP.ip.s_addr % 65535);

        int sleep_sec = 0, sleep_usec = 0;

        if (lookup_parms->lookup_rate == 1) {
            sleep_sec = 1;
            sleep_usec = 0;
        } else if (lookup_parms->lookup_rate > 0) {
            sleep_sec = 0;
            sleep_usec = (int) ((1e+6) / lookup_parms->lookup_rate);
            Assert((sleep_usec < 1000000), XEQ_ERROR);
            Assert((sleep_usec > 0), XEQ_ERROR);
        }

        mysleep(30); // waits for a while in order to allow the peer to start

        int D1HT_connection = -1;
        struct in_addr home_ip;
        in_port_t home_port;
        KEYID id;

        char key[D1HT_MAX_KEY_SIZE];
        BVZERO(key, sizeof (key));

        size_t nbytes = MIN(sizeof (id), sizeof (key));

        bool stat_connection_tried = false;

        if (G_stats_connection < 0) stat_connection_tried = true;

        while (1) {
            int lookup_retcode;

            while (G_killed) {
                mysleep(60);
            }

            while (D1HT_connection < 0) {
                mysleep(10);
                if (!G_killed) D1HT_connection = D1HT_connect(lookup_parms->peer.ip, lookup_parms->peer.port);
            }

            if (sleep_sec == 1) {
                mysleep(1, 0);
            } else if (sleep_usec != 0) {
                int sleep_usecx = (int) ((uint) sleep_usec);
                ASSERT((sleep_usecx < 1000000), XEQ_ERROR);
                if (sleep_usecx > 0) mysleep(0, sleep_usecx);
            }

            if (!stat_connection_tried && D1HT_connection >= 0) {
                stat_connection_tried = true;
                open_stats_connection();
            }

            id = (KEYID) nrand48(xsubi);
            memcpy(key, &id, nbytes);

            if (G_phase != phase) break;

            lookup_retcode = D1HT_lookup(key, sizeof (key), &D1HT_connection, &home_ip, &home_port);

            if (lookup_retcode < 0) {
                //		VERBOSE(stderr, "WARNING Cause: lookup failed, peer=%s:%u, killed=%u\n", IPtoP(G_parms.myIP), G_killed);
                D1HT_close(&D1HT_connection);
                D1HT_connection = -1;
            }
        }

        D1HT_close(&D1HT_connection);
        D1HT_connection = -1;
        PTHREAD_PARM_FREE;
        return NULL;
    }

} // namespace d1ht
