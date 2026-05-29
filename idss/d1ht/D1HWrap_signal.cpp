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
 * running 2 distinct D1HT and/or CALOT  peers in the samWARNING Cause: e node, a peer  *
 * from one system could (by mistake or software bug) send a msg to the  *
 * peer of the other system, which would pollute both peers Routing      *
 * Tables.                                                               *
 *                                                                       *
 *************************************************************************/
/*********************** D1HT COPYRIGHT NOTICE END **********************/
/**
 * @file D1HWrap_signal.c
 * @brief gestione dei segnali
 */

#include "D1HWrap.h"
#include <wait.h>

namespace d1ht {

    bool G_stats_dumped = false;

    typedef void sigfunc_t(int);

    sigset_t g_blockmask;

    void SIG_handler(int signo) {

        PTXEQ(pthread_sigmask(SIG_BLOCK, &g_blockmask, NULL), XEQ_ERROR);
        char msgerr[128];

        if (G_LINE != 0) {
            sprintf(msgerr, "(signo=%d) caught, probably due to an error in the malloc or free call done in FILE=%s LINE=%d", signo, G_FILE, G_LINE);
        } else {
            sprintf(msgerr, "(signo=%d) caught", signo);
        }

        switch (signo) {

            case SIGABRT:
            case SIGBUS:
            {
                if (!G_killed && G_D1HT_pid != 0) {
                    G_killed = true;
                    pid_t pid = G_D1HT_pid;
                    G_D1HT_pid = 0;
                    XEQ_EXEC(kill(pid, SIGKILL), XEQ_SILENT);
                }
                if (G_exiting) {
                    G_exited = true;
                    fprintf(stderr, "\n%s FILE=%s LINE=%u Secondary ERROR while Exiting Cause: Signal SIGABRT or SIGBUS %s\n", G_parms.program_name, __FILE__, __LINE__, msgerr);
                    fflush(stderr);
                    mysleep(10);
                } else {
                    G_exiting = true;
                    G_exited = true;
                    fprintf(stderr, "\n%s FILE=%s LINE=%u ERROR Cause: Signal SIGABRT or SIGBUS %s - Exiting\n", G_parms.program_name, __FILE__, __LINE__, msgerr);
                }
                fflush(stderr);
                exit(-1);
                break;
            }

            case SIGILL:
            {
                EXIT_WRAP("Signal SIGILL %s", msgerr);
                break;
            }

            case SIGPIPE:
            {
                EXIT_WRAP("Signal SIGPIPE %s", msgerr);
                break;
            }

            case SIGSEGV:
            {
                EXIT_WRAP("Signal SIGSEGV %s", msgerr);
                break;
            }

            case SIGFPE:
            {
                EXIT_WRAP("Signal SIGFPE %s", msgerr);
                break;
            }

            case SIGTERM:
            case SIGINT:
            {
                VERBO(stderr, "%s caught Signal SIGTERM or SIGINT (signo=%u) - Exiting...\n", G_parms.program_name, (uint) signo);
                if (!G_killed && G_D1HT_pid != 0) {
                    G_killed = true;
                    pid_t pid = G_D1HT_pid;
                    G_D1HT_pid = 0;
                    XEQ_EXEC(kill(pid, SIGTERM), XEQ_SILENT);
                    mysleep(10);
                    XEQ_EXEC(kill(pid, SIGKILL), XEQ_SILENT);
                }

                if (G_stats_dumped) {
                    EXITOK;
                } else {
                    EXIT_WRAP("D1HWrap exit forced");
                }
                break;
            }

            case SIGCHLD:
            {
                wait(NULL);
                if (G_phase != -1 && !G_killed) {

                    bool already_killed = G_killed;

                    verbose(stderr, "WARNING Cause: D1HT peer failed, phase=%d, failed previoulsy=%u - restarting\n", G_phase, G_failed);

                    add_stats(G_phase);
                    close_stats_connection();

                    if (G_phase == -1) break;

                    D1HT_kill(SIGTERM, __FILE__, __LINE__);

                    if (G_parms.input_RT && G_phase == 0) {

                        mysleep(60);
                        D1HT_fork(__FILE__, __LINE__);
                    } else {
                        LOCK(G_event_mutex);

                        if (G_phase == -1) break;
                        if (!already_killed) {
                            G_wrap_stats[G_phase].new_event();
                            G_wrap_stats[G_phase].failure();
                        }
                        UNLOCK(G_event_mutex);
                        PTXEQ(pthread_sigmask(SIG_UNBLOCK, &g_blockmask, NULL), XEQ_ERROR);

                        mysleep(60);
                        G_never_died = false;
                    }
                    G_failed = true; // we allow only one restart per phase in order to avoid recurring errors   	
                }
                break;
            }

            case SIGHUP:
            {
                break;
            }

            default:
            {
                EXIT_WRAP("Signal Unknown %s", msgerr);
                break;
            }
        }
        PTXEQ(pthread_sigmask(SIG_UNBLOCK, &g_blockmask, NULL), XEQ_ERROR);
    }

    PTHREAD(signal_handler_thread) {
        PTHREAD_DETACH;
        PTHREAD_INIT;
        PTHREAD_MAYLOCK;

        sigset_t blockmask;
        //	XEQ_EXEC(sigemptyset(&g_blockmask), XEQ_ERROR);
        //	XEQ_EXEC(sigaddset(&blockmask, SIGHUP), XEQ_ERROR);
        //	XEQ_EXEC(pthread_sigmask(SIG_BLOCK, &blockmask, NULL), XEQ_ERROR);

        XEQ_EXEC(sigfillset(&blockmask), XEQ_ERROR);
        XEQ_EXEC(sigdelset(&blockmask, SIGHUP), XEQ_ERROR);

        XEQ_EXEC(pthread_sigmask(SIG_UNBLOCK, &blockmask, NULL), XEQ_ERROR);

        XEQ_EXEC(sigemptyset(&blockmask), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&blockmask, SIGHUP), XEQ_ERROR);

        // loop forever handling interrupts, specially SIGUSR1, SIGUSR2, and SIGALARM, with the exception of SIGHUP
        while (1) {
            sigsuspend(&blockmask);
        }

        return NULL;
    }

    sigfunc_t *signal_handler(int signo, sigfunc_t *func) {
        struct sigaction act, oact;

        BZERO(act);
        sigemptyset(&act.sa_mask);
        act.sa_handler = func;

        act.sa_flags = 0;
        if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT; /* SunOS 4.x */
#endif
        } else {
#ifdef	SA_RESTART
            act.sa_flags |= SA_RESTART; /* SVR4, 44BSD */
#endif
        }

        if (sigaction(signo, &act, &oact) != 0) EXIT_WRAP("sigaction");
        return (oact.sa_handler);
    }

    void signal_ignore(const int signo) {
        struct sigaction act, oact;

        BZERO(act);
        sigemptyset(&act.sa_mask);
        act.sa_handler = SIG_IGN;

        act.sa_flags = 0;

        if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT; /* SunOS 4.x */
#endif
        } else {
#ifdef	SA_RESTART
            act.sa_flags |= SA_RESTART; /* SVR4, 44BSD */
#endif
        }

        if (sigaction(signo, &act, &oact) != 0) EXIT("sigaction");
    }

    void *set_signal_handlers() {

        XEQ_EXEC(sigemptyset(&g_blockmask), XEQ_ERROR);

        XEQ_EXEC(sigaddset(&g_blockmask, SIGTERM), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&g_blockmask, SIGINT), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&g_blockmask, SIGALRM), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&g_blockmask, SIGUSR1), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&g_blockmask, SIGUSR2), XEQ_ERROR);
        XEQ_EXEC(sigaddset(&g_blockmask, SIGCHLD), XEQ_ERROR);
        //XEQ_EXEC(sigaddset(&g_blockmask, SIGHUP), 	XEQ_ERROR);
        //XEQ_EXEC(sigaddset(&g_blockmask, SIGCONT), 	XEQ_ERROR);

        signal_handler(SIGPIPE, SIG_IGN);
        signal_handler(SIGSEGV, SIG_handler);
        signal_handler(SIGFPE, SIG_handler);
        signal_handler(SIGILL, SIG_handler);
        signal_handler(SIGINT, SIG_handler);
        signal_handler(SIGTERM, SIG_handler);
        signal_handler(SIGABRT, SIG_handler);
        signal_handler(SIGBUS, SIG_handler);
        signal_handler(SIGCHLD, SIG_handler);

        signal_handler(SIGUSR1, SIG_IGN);
        signal_handler(SIGUSR2, SIG_IGN);
        signal_handler(SIGHUP, SIG_handler);
        signal_handler(SIGALRM, SIG_handler);
        signal_handler(SIGCONT, SIG_IGN);

        // only signal_handler_thread will receive SIGALRM, SIGHUP, etc
        G_SIG_thread_id = PTHREAD_CREATE(signal_handler_thread, NULL, &G_detached);

        XEQ_EXEC(pthread_sigmask(SIG_BLOCK, &g_blockmask, NULL), XEQ_ERROR);
        return NULL;
    }

    void block_signals() {

        XEQ_EXEC(pthread_sigmask(SIG_BLOCK, NULL, &g_blockmask), XEQ_ERROR);

        XEQ_EXEC(sigaddset(&g_blockmask, SIGCONT), XEQ_ERROR);

        XEQ_EXEC(pthread_sigmask(SIG_BLOCK, &g_blockmask, NULL), XEQ_ERROR);
    }

} // namespace d1ht
