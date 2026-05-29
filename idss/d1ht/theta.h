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
 * @file theta.h 
 * @brief gestione della durata degli intervalli theta 
 */

#ifndef THETA_H_
#define THETA_H_

class theta_t {
    int last_theta;     /**< duration of the current theta interval (in msecs) */

    lock_t theta_mutex;

    float s_erate;      /**< smoothed erate estimator, events/seconds */
    float erate_var;    /**< smoothed erate deviation, events/seconds */
    int start_erate;    /**< moment when we started to count events */
    int events;

    /**
     * 
     * @param np
     * @return 
     */
    int Savg(const int np) { // average session length (in secs)
        //		return 3600;

        int retcode;

        Assert(np > 0, XEQ_ERROR);
        if (s_erate <= 0.0) EXIT("s_erate <=0. Detail: s_erate=%f", (float) s_erate);

        float S;
        S = (float) (2.0 * (float) np);
        S = S / s_erate;

        if (S < 300.0) S = 300.0;
        if (S > 36000.0) S = 36000.0;

        VERBOSEX(stderr, "Savg=%6.0f, erate=%6.3f, npeers=%u\n", S, s_erate, np);

        G_stats.total.stats[stat_Savg].value += (double) S;
        G_stats.total.stats[stat_Savg].count++;

        retcode = (int) S;

        return retcode;
    }

    /**
     * 
     * @param np
     * @return 
     */
    uint E_MAX_locked(const int np) {

        uint retcode = (uint) (((float) (8.0 * G_parms.f * np)) / ((float) 100.0 * (16.0 + 3.0 * G_RT.rho(np)))); // E Equ. 3.8

        G_stats.total.stats[stat_E_tried].count++;
        G_stats.total.stats[stat_E_tried].value += (double) retcode;
        //		VERBO(stderr,"E_tried value=(%f,%f), count=(%d)\n", (float) retcode, (float) G_stats.total.stats[stat_E_tried].value, (int) G_stats.total.stats[stat_E_tried].count);
        return retcode;
    }

public:

    theta_t() {

        last_theta = D1HT_INIT_THETA;
        s_erate = 1.0;
        erate_var = 0.25;
        events = 0;
        start_erate = 0;

        LOCK_INIT(theta_mutex, maychain = true, 79);
    }

    
    /**
     * Calcola la durata di theta sulla base dei nodi presenti nel sistema
     * @param np
     * @return 
     */
    int new_theta(const int np) { // duration of the next theta interval (in msecs)
        int retcode = -1;

        Assert(np > 0, XEQ_ERROR);

        int rho = G_RT.rho(np);

        Assert(rho > 0, XEQ_ERROR);

        LOCK(theta_mutex);

        //		last_theta = (2*G_parms.f*Savg(np)*10 - 2*rho*rto) / (8+rho);	// Theta Equ. 3.6
        last_theta = (4 * G_parms.f * Savg(np)*10) / (16 + 3 * rho); // Theta Equ. 3.7
        //		fprintf(stderr, "theta=%d, np=%u, rho=%u, f=%u, part1=%u, part2=%d, rto=%d\n", theta, np, rho, G_parms.f, 2*G_parms.f*Savg(np)/100, 2*rho*rto)/1000, rto);

        if (last_theta < D1HT_MIN_THETA) {
            last_theta = D1HT_MIN_THETA;
        } else if (last_theta > D1HT_MAX_THETA) {
            last_theta = D1HT_MAX_THETA;
        }

        retcode = last_theta;

        if (retcode > 0.0) {
            G_stats.total.stats[stat_theta].count++;
            G_stats.total.stats[stat_theta].value += (double) retcode;
        }
        //		VERBO(stderr,"stat_theta value=(%f,%f), count=(%d)\n", (float) retcode, (float) G_stats.total.stats[stat_theta].value, (int) G_stats.total.stats[stat_theta].count);

        UNLOCK(theta_mutex);
        return retcode;
    }

    /**
     * 
     * @return 
     */
    int new_theta() {
        int npeers = G_RT.pub_npeers();
        return new_theta(npeers);
    }

    
    /**
     * 
     * @param np
     * @param E
     */
    void update_erate(const int np, const int E) {

        if (E == 0 || !G_started || G_exiting) return;

        NUM_32 rho = (NUM_32) G_RT.rho(np);

        LOCK(theta_mutex);

        int now_time = (int) elapsed_msecs();
        int delta_time = now_time - start_erate;

        ASSERT(delta_time >= 0);

        if (delta_time <= 0) {
            UNLOCK(theta_mutex);
            return; // it shouldnt happen .......
        }

        events += E;

        if (start_erate == 0) {
            start_erate = now_time;
            //VERBO(stderr,"erate initial1 = %7.4f\n", s_erate)
            Assert(events == E, XEQ_ERROR);
            events = 0;
            //		} else if ((events > (int) (rho * E_MAX_locked(np)/2)) && (delta_time > (int) (rho * last_theta)/2)) {
        } else if (delta_time >= (int) (rho * last_theta) / 2) {

            float f_delta_time = (float) delta_time;
            float f_events = (float) events;

            Assert(f_delta_time > 0.0, XEQ_ERROR);

            float temp_rate = f_events * 1000.0 / f_delta_time;
            //VERBO(stderr, "temp_rate=%f, events=%u, tempo=%f msecs\n", temp_rate, events,  ((float) delta_time));

            float delta = temp_rate - s_erate;
            s_erate += delta / 8.0;
            if (delta < 0.0) delta = -delta; /* |delta| */
            erate_var += (delta - erate_var) / 4.0; /* h = 1/4 */

            G_stats.total.stats[stat_erate].count++;
            G_stats.total.stats[stat_erate].value += (double) s_erate;

            start_erate = now_time;
            events = 0;
        }
        UNLOCK(theta_mutex);
    }

    /**
     * 
     * @param erate
     */
    void update_erate(const float erate) {
        Assert(erate > 0.0, XEQ_ERROR);
        if (G_started) {
            LOCK(theta_mutex);
            s_erate = erate;
            UNLOCK(theta_mutex);
        } else {
            s_erate = erate;
        }
    }

    /**
     * 
     * @param nsavg
     */
    void new_Savg(const int nsavg) {
        //if (nsavg < 3600) VERBO("WARNING Cause: D1HT is not intended to run with average session timers smaller than one hour (observed Savg=%d secs)\n", nsavg);
        Assert((nsavg > 0), XEQ_ERROR);
        float new_rate = (float) (2.0 * (float) G_RT.pub_npeers());
        new_rate = new_rate / ((float) nsavg);
        Assert(new_rate > 0.0, XEQ_ERROR);
        LOCK(theta_mutex);
        s_erate = new_rate;
        UNLOCK(theta_mutex);
        VERBOSE(stderr, "new Savg=%d, new erate=%7.4f\n", nsavg, new_rate);
    }

    /**
     * 
     * @return 
     */
    float erate() {
        float retcode;
        LOCK(theta_mutex);
        retcode = s_erate;
        UNLOCK(theta_mutex);
        return retcode;
    }

    /**
     * 
     * @return 
     */
    int theta() {
        int retcode = -1;
        ;
        LOCK(theta_mutex);
        retcode = last_theta;
        UNLOCK(theta_mutex);
        return retcode;
    }

    /**
     * 
     * @param np
     * @return 
     */
    uint E_MAX(const int np) {
        uint retcode;
        LOCK(theta_mutex);
        retcode = E_MAX_locked(np);
        UNLOCK(theta_mutex);
        return retcode;
    }
};

extern theta_t G_theta;

#endif /*THETA_H_*/
