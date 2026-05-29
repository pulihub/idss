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
 * @file D1HWrap_routines.c
 * @brief gestione stats
 */

#include "D1HWrap.h"
#include <wait.h>

namespace d1ht {

    char G_new_port[12];

    void* close_stats_connection() {

        if (G_stats_connection >= 0) {
            D1HT_CLOSE(G_stats_connection);
            G_stats_connection = -1;
        }

        return NULL;
    };

    void* open_stats_connection() {
        if (G_stats_connection >= 0) {
            close_stats_connection();
            if (G_phase >= 0) mysleep(5);
        }

        if (G_phase >= 0) {
            G_stats_connection = D1HT_connect(G_parms.myIP.ip, G_parms.myIP.port);
            VERBOSE(stderr, "Connection to peer %s:%u established\n", IPtoP(G_parms.myIP));
        }
        return NULL;
    };

    void* add_stats(const int phase) {

        if (phase < 0) return NULL;
        stat_part_t partial_stats;
        verbose(stderr, "getting and adding peer stats IP=%s:%u, phase=%u, G_phase=%u, G_killed=%u, G_stats_connection=%u\n",
                IPtoP(G_parms.myIP), phase, G_phase, G_killed, G_stats_connection);

        //	LOCK(G_wrap_stats[phase].wrap_stats_mutex); 
        if (G_stats_connection == -2) { // the connection had previoulsy failed, so we discard the first stats
            verbose(stderr, "Discharging previous stats connection\n");
            open_stats_connection();
            if (G_stats_connection < 0) {
                verbose(stderr, "WARNING Cause: Failed to stablish stats connection. Detail: IP=%s:%u, G_killed=%u\n", IPtoP(G_parms.myIP), G_killed);
            } else {
                if (D1HT_get_stats_partial(G_stats_connection, &partial_stats, G_Savg) != 0) {
                    close_stats_connection();
                    G_stats_connection = -1;
                    verbose(stderr, "WARNING Cause: Failed to get D1HT peer stats. Detail: IP=%s:%u, G_killed=%u\n", IPtoP(G_parms.myIP), G_killed);
                }
            }
        } else {
            if (G_stats_connection < 0) open_stats_connection();
            if (G_stats_connection < 0) {
                G_stats_connection = -2;
                verbose(stderr, "WARNING Cause: Failed to stablish stats connection. Detail: IP=%s:%u, G_killed=%u\n", IPtoP(G_parms.myIP), G_killed);
            } else {
                if (D1HT_get_stats_partial(G_stats_connection, &partial_stats, G_Savg) != 0) {
                    close_stats_connection();
                    G_stats_connection = -2;
                    verbose(stderr, "WARNING Cause: Failed to get D1HT peer stats. Detail: IP=%s:%u, G_killed=%u\n", IPtoP(G_parms.myIP), G_killed);
                } else {
                    G_wrap_stats[phase] = G_wrap_stats[phase] + partial_stats;
                    //				G_wrap_stats[phase].stat_print(stderr, "D1HT partial"); 
                }
            }
        }
        //	UNLOCK(G_wrap_stats[phase].wrap_stats_mutex); 
        return NULL;
    }

    void *D1HT_kill(const int signum, const char* file, const uint line) {

        LOCK(G_event_mutex);
        pid_t pid = G_D1HT_pid;
        bool ended = false;
        verbosefl(stderr, file, line, "I will kill the D1HT peer pid=%u, signal=%u\n", pid, signum);
        if (G_killed || pid == 0) {
            UNLOCK(G_event_mutex);
            verbosefl(stderr, file, line, "D1HT peer is not running pid=%u, signal=%u\n", pid, signum);
            return NULL;
        }

        if (signum == SIGTERM || signum == SIGKILL || signum == SIGINT) {
            G_wrap_stats[G_phase].stats[stat_kill].count++;
            G_wrap_stats[G_phase].stats[stat_leaves].count++;
            if (signum == SIGKILL) G_wrap_stats[G_phase].stats[stat_kill_9].count++;
            G_D1HT_pid = 0;
            G_killed = true;
            G_never_died = false;
            ended = true;
            if (G_phase >= 0) {
                add_stats(G_phase);
                G_wrap_stats[G_phase].new_event();
            }
            close_stats_connection();
        }

        XEQ_EXEC(kill(pid, signum), XEQ_SILENT);

        if (ended) {
            int i = 0;
            while (true) {
                if ((int) wait4(pid, NULL, WNOHANG, NULL) > 0) break;
                if (i >= 10) {
                    kill(pid, SIGKILL);
                    break;
                }
                UNLOCK(G_event_mutex);
                i++;
                mysleep(1);
                LOCK(G_event_mutex);
            }
            if ((int) wait4(pid, NULL, WNOHANG, NULL) <= 0) kill(pid, SIGKILL);
            verbosefl(stderr, file, line, "killed the D1HT peer pid=%u, signal=%u\n", pid, signum);
        } else {
            verbosefl(stderr, file, line, "sent %u signal to D1HT peer pid=%u\n", signum, pid);
        }
        UNLOCK(G_event_mutex);
        return NULL;
    }

    void* D1HT_fork(const char* file, const uint line) {

        if (G_stats_dumped) {
            verbosefl(stderr, file, line, "Forking D1HT peer AVOIDED as we are exiting\n");
            return NULL;
        }
        verbosefl(stderr, file, line, "Forking D1HT peer\n");

        LOCK(G_event_mutex);

        if (!G_killed) {
            UNLOCK(G_event_mutex);
            return NULL;
        }
        close_stats_connection();

        if (G_phase == -1) {
            UNLOCK(G_event_mutex);
            verbosefl(stderr, file, line, "D1HT peer not forked - benchmark has already ended\n");
            return NULL;
        }

        G_D1HT_pid = 99999;

        G_D1HT_pid = fork();

        if (G_D1HT_pid < 0) {
            EXIT_WRAP("failed to fork peer D1HT process");
        } else if (G_D1HT_pid == 0) { // I am the child D1HT process
            XEQ_EXEC(execvp(G_argv[0], &(G_argv[0])), XEQ_ERROR);
            EXIT_WRAP("failed to exec D1HT program"); // if execv return, we had an error
        }

        // I am the original wrapper process (the parent)

        G_killed = false;

        if (G_phase >= 0) {
            G_wrap_stats[G_phase].new_event();
        }

        UNLOCK(G_event_mutex);
        verbosefl(stderr, file, line, "forked the D1HT peer, pid=%u, IP=%s:%u\n", (int) G_D1HT_pid, IPtoP(G_parms.myIP));
        mysleep(10);

        return NULL;
    }

} // namespace d1ht
