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
 * Documentation: L. Monnerat and C. Amorim, Peerstat_print-to-Peer Single Hop     *
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
 * @file mtime.c
 * @brief gestione degli intervalli EDRA ...
 */

#include "D1HT.h"

namespace d1ht {

    lock_t g_theta_interval_mutex;
    int g_next_interval_start; /**< variabile per la gestione degli istanti di fine attesa (quando parte EDRA), protetta da mutex */

    extern void *close_interval(const bool mutexed, const bool exiting, const bool istimeout);

    void times_t::time_dump_write(const int file_dump_stats) {
        VERBOSEX(stderr, "dump_write secs_real=%d, %u, %d\n", secs_real, secs_real, (int) secs_real);
        XEQ_EXEC(writen(file_dump_stats, &secs_real, sizeof (secs_real)), XEQ_WARNING);
        XEQ_EXEC(writen(file_dump_stats, &msecs_user, sizeof (msecs_user)), XEQ_WARNING);
        XEQ_EXEC(writen(file_dump_stats, &msecs_sys, sizeof (msecs_sys)), XEQ_WARNING);
    }

    void times_t::time_dump_read(const int file_dump_stats) {
        XEQ_EXEC(readn(file_dump_stats, &secs_real, sizeof (secs_real)), XEQ_ERROR);
        XEQ_EXEC(readn(file_dump_stats, &msecs_user, sizeof (msecs_user)), XEQ_ERROR);
        XEQ_EXEC(readn(file_dump_stats, &msecs_sys, sizeof (msecs_sys)), XEQ_ERROR);
        VERBOSEX(stderr, "dump_read secs_real=%d, %u, %d\n", secs_real, secs_real, (int) secs_real);
    }

    /**
     * calcola il tempo attuale e ritorna la differenza con il momento di avvio del processo
     * @return 
     */
    float elapsed_msecs() {
        struct timeval now;
        gettimeofday(&now, NULL);
        return TVDIFF2MSEC(G_parms.program_start, now);
        ;
    }

    /**
     * determina lo scadere del prossimo intervallo di segnalazione
     * @param msecs
     * @return 
     */
    void *start_interval(const int msecs) {

        int now_time = (int) elapsed_msecs();

        LOCK(g_theta_interval_mutex);

        g_next_interval_start = now_time + msecs;

        UNLOCK(g_theta_interval_mutex);

        VERBOSEX(stderr, "Next interval will expire in %d msecs\n", msecs);

        return NULL;
    }

    
    
    /**
     * 
     * @param _parm_temp
     * @return 
     */
    PTHREAD(theta_interval) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        ASSERT(!G_exiting);

        start_interval(D1HT_INIT_THETA);

        while (!G_exiting) {

            int now_time = (int) elapsed_msecs();

            LOCK(g_theta_interval_mutex);

            int delta = g_next_interval_start - now_time;

            if (delta <= 0) g_next_interval_start = now_time + D1HT_MIN_THETA;

            UNLOCK(g_theta_interval_mutex);

            if (G_exiting) break;

            if (!G_started) continue;

            //calcolo quanto manca al prossimo intervallo theta, e se è scaduto, chiamo close_interval() per l'invio di tutti gli eventi accumulati 
            if (delta <= D1HT_MIN_THETA / 10) {
                VERBOSEX(stderr, "Theta interval expired.\n");
                //parte la disseminazione EDRA
                close_interval(false, false, true);
            } else {
                if (delta > D1HT_MIN_THETA * 2) delta = D1HT_MIN_THETA * 2;
                VERBOSEX(stderr, "Next interval will expire in %d msecs\n", delta);
                mysleep_msecs(delta);
            }
        }
        return NULL;
    }

    
    /**
     * sembra una precedente versione del calcolo degli intervalli di segnalazione
     * @param msecs
     * @return 
     */
    void *start_interval_old(const int msecs) {

        struct itimerval theta;
        BZERO(theta);

        theta.it_value.tv_sec = msecs / 1000;
        theta.it_value.tv_usec = (msecs - (1000 * theta.it_value.tv_sec)) * 1000;
        //	 theta.it_interval 		= theta.it_value;

        setitimer(ITIMER_REAL, &theta, NULL);

        VERBOSE(stderr, "next interval will expire in %u msecs (tv_sec=%lu, tv_usec=%lu)\n", msecs, (ulong) theta.it_value.tv_sec, (ulong) theta.it_value.tv_usec);
        return NULL;
    }

    
    
    void* mysleep(const int sleep_sec, const int sleep_usec) {
        if (sleep_sec == 0 && sleep_usec == 0) return NULL;

        struct timespec tv;
        tv.tv_sec = (time_t) sleep_sec;
        tv.tv_nsec = (int) 1000 * sleep_usec;

        while (true) {
            if (nanosleep(&tv, &tv) == 0) break;
            if (errno == EINTR) continue;
            int errsave = errno;
            EXIT("Nanosleep failed, reason=%s. Detail: sleep_sec=%u, sleep_usec=%u\n", strerror(errsave), sleep_sec, sleep_usec);
        }
        return NULL;
    }

    void* mysleep(const int sleep_sec) {
        return mysleep(sleep_sec, 0);
    }

    void* mysleep_msecs(const int sleep_msecs) {
        int secs = sleep_msecs / 1000;
        int usecs = (sleep_msecs - (1000 * secs)) * 1000;
        return mysleep(secs, usecs);
    }

} // namespace d1ht
