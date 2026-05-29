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
 * @file D1HWrap.c
 * @brief test file (?)
 */

#include "D1HWrap.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sysinfo.h>

namespace d1ht {

    extern void* lookup_thread(void* lookup_rate);
    extern void* set_signal_handlers();

    int G_phase;
    pid_t G_D1HT_pid;
    bool G_killed = true;
    bool G_failed = false;
    bool G_never_died = true;
    stat_part_t G_wrap_stats[MAX_PHASES];
    lock_t G_event_mutex;
    lock_t G_wrap_stats_mutex;
    FILE* g_random;
    pthread_t G_pthread_lookup_id = 0;
    int G_stats_connection = -1;
    char* G_D1HT = "D1HT";

} // namespace d1ht

using namespace d1ht;

#define FRANDOM(_file) ({ 													\
	float 		_retcode; 													\
	unsigned 	_dummy; 													\
	fread(&_dummy, sizeof(_dummy), 1, _file); 								\
	_retcode = (float) _dummy;												\
	_retcode = _retcode / (powf(2.0, ((float) sizeof(_dummy)) * 8.0));		\
	ASSERT(_retcode >= 0.0);												\
	_retcode; 																\
})

#define URANDOM(_file) ({ uint _retcode; fread(&_retcode, sizeof(_retcode), 1, _file); _retcode; })

int now() {
    struct timeval nowtv;
    gettimeofday(&nowtv, NULL);
    return (int) (nowtv.tv_sec - G_parms.program_start.tv_sec);
}

int get_cpu_count() {
    FILE *fproc;
    char buffer[10240];
    size_t bytes_read;
    char* match;
    int retcode = 0;

    XEQ_EXEC_NULL((fproc = fopen("/proc/cpuinfo", "r")), XEQ_ERROR);
    bytes_read = fread(buffer, 1, sizeof (buffer), fproc);
    fclose(fproc);

    Assert(bytes_read > 0, XEQ_ERROR, "Error reading /proc/cpuinfo");
    Assert(bytes_read < sizeof (buffer), XEQ_ERROR, "buffer size too small to read /proc/cpuinfo");

    buffer[bytes_read] = '\0';

    match = buffer;

    while ((match = strstr(match, "processor")) != NULL) {
        retcode++;
        match++;
    }

    VERBO(stderr, "cpu count = %u\n", retcode);

    return retcode;
}

class phase_t {
    bool started;
    bool finished;
public:
    char name[8];
    int lookup_rate; // lookups p/min
    int Savg; // average sesion time, in seconds
    int duration; // duration in seconds	
    float kill_fraction; // % of nodes that should die in the beggining of the phase
    float SIGKILL_fraction; // % of the departures that are not voluntary
    bool restart; // true (1) if we should restart a node that has been chosen to be G_killed in a previous phase (due to the kill_fraction)
    bool wait_SIGCONT; // true (1) if we should wait for a SIGCONT signal to start the phase
    bool dump_table; // true (1) if we should dump the routing table at the end of the phase
    bool iterative; // true (1) if D1HWrap should loop waiting for new phases files
    bool start_immediatly; // 

    int session_time() {
        int stime = (int) MAX((uint) 60, (URANDOM(g_random) % Savg));
        return stime;
    }
};

int main(int argc, char** argv) {
#if defined(DEBUG)
    G_statm = fopen("/proc/self/statm", "r");
#if defined(MCHEK)
    mcheck(&abortfn);
#endif	
#endif
    G_UDP_thread_id = 0;

    G_D1HT = "D1HT";

    FILE* input;
    char buffer[8192];
    char* match;
    size_t bytes_read;

    phase_t phases[MAX_PHASES];
    phase_t default_parms;

    G_UDP_thread_id = 0;
    G_started = false;
    ASSERT(MAX_PHASES < 9);

    pthread_data_t *pthread_data_mine;

    LOCK_INIT(G_header_mutex, maychain = false, 15);
    LOCK_INIT(G_interval_mutex, maychain = true, 80);
    LOCK_INIT(G_event_mutex, maychain = true, 22);
    LOCK_INIT(G_thread_data_mutex, maychain = true, 20);

#if defined(MEMLOCK)
    LOCK_INIT(G_malloc_mutex, maychain = true, 10);
#endif

    LOCK_INIT(G_wrap_stats_mutex, maychain = true, 21);

    G_pthread_data_header = CALLOC(1, pthread_data_t);
    pthread_data_mine = CALLOC(1, pthread_data_t);

    pthread_key_create(&G_pthread_data_key, close_pthread_data);
    pthread_setspecific(G_pthread_data_key, pthread_data_mine);

#ifndef D1HT_PTHREAD_LIGHT	
    G_pthread_data_header->next = pthread_data_mine;
    pthread_data_mine->prev = G_pthread_data_header;
    pthread_data_mine->tid = pthread_self();
    strncpy(pthread_data_mine->pthread_name, "main", D1HT_PTHREAD_NAME_SIZE - 1);
    pthread_data_mine->started = true;
    XEQ(pthread_data_mine->xeq_caller);
#if defined(XEQP)
    pthread_data_mine->xeq_last = pthread_data_mine->xeq_caller;
#endif

    pthread_data_mine->pthread_init();
    PTHREAD_MAYLOCK;
#endif

    G_parms.program_name = "D1HWrap";

    G_parms.get(argc - 2, &(argv[2]));

    G_parms.program_name = "D1HWrap";

    G_parms.VERBOSE = 1;
    G_parms.verbose = 1;
    G_started = true;

    //	for (int i=0; i<16; i++) D1HT_CLOSE(i);   
    //openlog( "D1HWrap ", LOG_PERROR|LOG_PID|LOG_CONS, LOG_USER); // open syslog  
    //	XEQ_EXEC(daemon(1,0), XEQ_ERROR);

    g_random = fopen("/dev/urandom", "rb");

    stat_part_t wrap_tot_stats;

    char WrapedName[20];

    if ((strstr(argv[2], "D1HT")) != NULL) {
        strncpy(WrapedName, "D1HT", sizeof (WrapedName));
    } else {
        VERBO(stderr, "Wrap command submitted=");
        for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
        fprintf(stderr, "\n");

        char msgerr[120];
        sprintf(msgerr, "ERROR Cause: Invalid program name (%s)- it should be D1HT", argv[2]);
        EXIT_WRAP(msgerr);
    }

    char filename[256];
    sprintf(filename, "e_%s_%s%s%s.txt", G_parms.program_name, WrapedName, G_parms.display_name, G_parms.start_time_string);
    XEQ_EXEC_NULL(freopen(filename, "w", stderr), XEQ_ERROR);

    VERBO(stderr, "Wrap command=");
    for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
    fprintf(stderr, "\n");

    VERBO(stderr, "Running command=");
    for (int i = 0; i < G_argc; i++) fprintf(stderr, "%s ", G_argv[i]);
    fprintf(stderr, "\n");

    ASSERT(G_argv[G_argc] == NULL);

    char** argvn = CALLOC((G_argc + 2), char*);

    for (int i = 0; argv[i] != NULL && i < G_argc; i++) {
        argvn[i] = G_argv[i];
    }

    G_argv = argvn;
    G_argv[G_argc] = G_new_port;
    G_argv[G_argc + 1] = NULL;
    argvn = NULL;
    G_argc++;

    sprintf(G_new_port, "-p%u", ntohs(G_parms.myIP.port));
    set_signal_handlers();

    sigset_t wait_SIGCONT, wait_SIGHUP;

    XEQ_EXEC(sigfillset(&wait_SIGCONT), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGCONT), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGTERM), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGINT), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGILL), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGBUS), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGSEGV), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGCONT, SIGALRM), XEQ_ERROR);

    XEQ_EXEC(sigfillset(&wait_SIGHUP), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGHUP), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGTERM), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGINT), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGILL), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGBUS), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGSEGV), XEQ_ERROR);
    XEQ_EXEC(sigdelset(&wait_SIGHUP, SIGALRM), XEQ_ERROR);

    while (true) {
        struct tm *ptm;
        char time_string[20];
        struct timeval phase_start;

        gettimeofday(&phase_start, NULL);
        ptm = localtime(&phase_start.tv_sec);
        strftime(time_string, sizeof (time_string), D1HT_TIME_FORMAT, ptm);
        if (G_parms.daemonize) EXIT_WRAP("You cannot specify --Daemonize or -D with D1HWrap");

        VERBO(stderr, "Waiting for SIGHUP to read file %s and start\n", argv[1]);
        sigsuspend(&wait_SIGHUP); // block waiting SIGHUP
        VERBO(stderr, "SIGHUP catched\n");

        if (!(input = fopen(argv[1], "r"))) EXIT_WRAP("Problem opening input file");

        if (!(bytes_read = fread(buffer, 1, sizeof (buffer), input))) EXIT_WRAP("Problem reading input file");

        fclose(input);

        if (bytes_read == sizeof (buffer)) EXIT_WRAP("input file too big - need to increase internal buffer file");

        buffer[bytes_read] = '\0';

        match = buffer;

        if (strstr(match, "phase8") != NULL) EXIT_WRAP("there should be at most 8 phases (phase0 through phase7)");
        if (strstr(match, "phase9") != NULL) EXIT_WRAP("there should be at most 8 phases (phase0 through phase7)");

        BVZERO(phases, sizeof (phases));

        BZERO(default_parms);

        phase_t* defp = &(default_parms);

        sprintf(defp->name, "default");

        FIND(defp, duration);
        FIND(defp, lookup_rate);
        FIND(defp, Savg);
        FIND(defp, restart);
        FIND(defp, wait_SIGCONT);
        FIND(defp, dump_table);
        FIND(defp, SIGKILL_fraction);
        FIND(defp, kill_fraction);
        FIND(defp, iterative);

        if (default_parms.duration != 0) EXIT_WRAP("You may not specify a default duration");
        if (default_parms.kill_fraction != 0.0) EXIT_WRAP("You may not specify a default kill_fraction");

        verbose(stderr, "default parms: iterative=%u lookup_rate=%u lookups/min Savg=%u secs wait_SIGCONT=%u restart=%u SIGKILL_fraction=%u %%\n",
                defp->iterative, defp->lookup_rate, defp->Savg, defp->wait_SIGCONT, defp->restart, (uint) defp->SIGKILL_fraction);

        int phase_count = 0;

        for (int i = 0; i < MAX_PHASES; i++) {

            phase_t* p = &phases[phase_count];

            sprintf(p->name, "%s%u", "phase", i);

            if (strstr(match, p->name) != NULL) {
                FIND(p, duration);
                FIND(p, lookup_rate);
                FIND(p, Savg);
                FIND(p, restart);
                FIND(p, wait_SIGCONT);
                FIND(p, dump_table);
                FIND(p, kill_fraction);
                FIND(p, SIGKILL_fraction);
                FIND(p, start_immediatly);

                verbose(stderr, "phase %s: duration=%u secs, lookup_rate=%u lookups/min Savg=%u secs wait_SIGCONT=%u restart=%u kill_fraction=%u SIGKILL_fraction=%u\n",
                        p->name, p->duration, p->lookup_rate, p->Savg, p->wait_SIGCONT, p->restart, (uint) p->kill_fraction, (uint) p->SIGKILL_fraction);

                if (p->duration < 30) EXIT_WRAP("All phase durations should be bigger than 30 secs\n");

                if (p->start_immediatly) {
                    if (phase_count != 0) EXIT_WRAP("start_immediatly may only be specified for the initial phase\n");
                    if (G_parms.input_RT) EXIT_WRAP("you may not specify start_immediatly with -e or --routing_table\n");
                }

#if defined(DEBUG) && !defined(BWR4) && !defined(BENCHPETRO)		
                if (true && p->Savg) {
                    int TOT = 0;
                    int iter = 1000;
                    int max = 0;
                    int min = 99999;
                    for (int i = 0; i < iter; i++) {
                        int s = p->session_time();
                        TOT += s;
                        min = MIN(min, s);
                        max = MAX(max, s);
                    }
                    TOT = TOT / iter;
                    VERBO(stderr, "%s: Estimated Savg=%d, min=%d, max=%d, Savg=%d\n", p->name, TOT, min, max, p->Savg);
                }
#endif			 			
                phase_count++;
            }
        }

        Assert(phase_count != 0, XEQ_ERROR);

        for (G_phase = 0; G_phase < phase_count; G_phase++) {

            phase_t* p;
            G_wrap_stats[G_phase].init();
            p = &phases[G_phase];

            G_Savg = p->Savg;

            if (p->wait_SIGCONT) {
                VERBO(stderr, "Waiting for SIGCONT to start %s\n", p->name);
                fflush(stderr);
                sigsuspend(&wait_SIGCONT); // block waiting SIGCONT
            }

            struct sysinfo loadinfo;
            XEQ_EXEC(sysinfo(&loadinfo), XEQ_ERROR);
            G_wrap_stats[G_phase].stats[stat_loadavg15].count = get_cpu_count();
            G_wrap_stats[G_phase].stats[stat_loadavg15].value = ((double) loadinfo.loads[2])*1000.0 / 65536.0;

#if defined(BWR4)
            VERBO(stderr, "%s started: dur=%u lrate=%u Savg=%u kill_f=%f restart=%u\n",
                    p->name, p->duration, p->lookup_rate, p->Savg, p->kill_fraction, p->restart);
#else
            VERBO(stderr, "%s started: dur=%u lrate=%u Savg=%u kill_f=%.1f %% restart=%u\n",
                    p->name, p->duration, p->lookup_rate, p->Savg, p->kill_fraction, p->restart);
#endif		
            fflush(stderr);

            int start_time = now();
            int end_time = start_time + p->duration;

            if (G_phase == 0) {

                Assert(G_killed, XEQ_ERROR);
                Assert(!G_failed, XEQ_ERROR);

                if (p->kill_fraction != 0.0) EXIT_WRAP("kill_fraction is invalid in the initial phase");
                if (p->restart) {
                    VERBOSEX(stderr, "WARNING Cause: Restart has no effect in the initial phase\n");
                    p->restart = false;
                }
                if (G_parms.input_RT) {
                    if (p->Savg) EXIT_WRAP("When reading the routing table, you should not specify Savg in the initial phase");
                    if (p->lookup_rate) EXIT_WRAP("When reading the routing table, you should not specify lookup_rate in the initial phase");
                }

                float prob = FRANDOM(g_random);

                if (G_parms.input_RT || p->start_immediatly) {
                    mysleep(5);
                } else {
                    int wait_period = (int) (prob * ((float) (p->duration - 30)));
                    mysleep(wait_period);
                    D1HT_fork(__FILE__, __LINE__);
                }

                if (G_killed) D1HT_fork(__FILE__, __LINE__);

            } else if (G_killed) {
                if (p->restart) {
                    D1HT_fork(__FILE__, __LINE__);
                    G_failed = false;
                    verbose(stderr, "I was restarted\n");
                }
            } else {
                if (p->kill_fraction != 0.0) {
                    if (p->restart) EXIT_WRAP("restart and kill_fraction must not be specified in the same phase");
                    float prob = FRANDOM(g_random)*100.0;
                    if (prob < p->kill_fraction) D1HT_kill(SIGTERM, __FILE__, __LINE__);
                }
            }

            if (!G_killed) {
                if (p->lookup_rate != 0) {

                    struct lookup_parms_t *lookup_parms = CALLOC(1, struct lookup_parms_t);

                    lookup_parms->lookup_rate = p->lookup_rate;
                    lookup_parms->phase = G_phase;
                    lookup_parms->peer = G_parms.myIP;

                    G_pthread_lookup_id = PTHREAD_CREATE(lookup_thread, lookup_parms, &G_detached);
                }

                if (p->Savg != 0) {

                    int stime = p->session_time();
                    while ((stime + now() + 60) <= end_time) {
                        mysleep(stime);

                        if (G_pthread_lookup_id) {
                            PTXEQ(pthread_cancel(G_pthread_lookup_id), XEQ_SILENT);
                            G_pthread_lookup_id = 0;
                        }

                        float prob = FRANDOM(g_random)*100.0;

                        if (prob < p->SIGKILL_fraction) {
                            D1HT_kill(SIGKILL, __FILE__, __LINE__);
                        } else {
                            D1HT_kill(SIGTERM, __FILE__, __LINE__);
                        }
                        mysleep(180);
                        D1HT_fork(__FILE__, __LINE__);

                        if (p->lookup_rate != 0) {

                            struct lookup_parms_t *lookup_parms = CALLOC(1, struct lookup_parms_t);

                            lookup_parms->lookup_rate = p->lookup_rate;
                            lookup_parms->phase = G_phase;
                            lookup_parms->peer = G_parms.myIP;

                            G_pthread_lookup_id = PTHREAD_CREATE(lookup_thread, lookup_parms, &G_detached);
                        }

                        int remain = p->Savg - stime - 60;

                        if (remain > (end_time - now())) break;

                        if (remain > 0) mysleep(remain);

                        stime = p->session_time();
                    }
                }
                Assert(!G_killed, XEQ_WARNING, "D1HT peer should have failed");
            }

            int time_to_end = end_time - now();
            if (time_to_end > 0) mysleep(time_to_end);

            time_to_end = end_time - now();
            if (time_to_end > 0) mysleep(time_to_end);

            // when especifying -e, it should be use only in the first phase, so we replace id by -U
            if (G_phase == 0 && G_parms.input_RT) {
                for (int i = 3; i < G_argc; i++) {
                    if (strncmp(G_argv[i], "-e", 2) == 0) strncpy(G_argv[i], "-U", 2);
                }
            }

            if (G_pthread_lookup_id) {
                PTXEQ(pthread_cancel(G_pthread_lookup_id), XEQ_SILENT);
                G_pthread_lookup_id = 0;
            }

            if (!G_killed) {
                Assert(G_D1HT_pid != 0, XEQ_ERROR);
                if (p->dump_table) D1HT_kill(SIGUSR1, __FILE__, __LINE__);

                if ((int) G_phase < (int) (phase_count - 1) && phases[G_phase + 1].Savg > 0) {
                    G_Savg = phases[G_phase + 1].Savg;
                }

                if (G_parms.input_RT && G_phase == 0) {
                    D1HT_kill(SIGCONT, __FILE__, __LINE__);
                } else {
                    add_stats(G_phase);
                }
            }
            ASSERT(now() >= end_time);
        }
        G_phase = -1;
        close_stats_connection();

        if (G_pthread_lookup_id) {
            PTXEQ(pthread_cancel(G_pthread_lookup_id), XEQ_SILENT);
            G_pthread_lookup_id = 0;
        }
        if (G_never_died) {
            if (G_stats_connection < 0) open_stats_connection();

            if (G_stats_connection >= 0) {
                stat_part_t tot_stats;
                if (D1HT_get_stats_total(G_stats_connection, &tot_stats, G_Savg) == 0) {
                    wrap_tot_stats.compare(tot_stats);
                }
                D1HT_CLOSE(G_stats_connection);
                G_stats_connection = -1;
            }
        }

        VERBO(stderr, "Final phase ended, G_killed=%u, G_D1HT_pid=%u\n", G_killed, G_D1HT_pid);

        if (G_parms.print_stats) {

            FILE* fstats = NULL;
            sprintf(filename, "stats_%s_%s%s%s.txt", G_parms.program_name, "D1HT", G_parms.display_name, time_string);
            XEQ_EXEC_NULL(fstats = fopen(filename, "w"), XEQ_ERROR);

            for (int iphase = 0; iphase < phase_count; iphase++) {
                G_wrap_stats[iphase].stat_print(fstats, phases[iphase].name);
            }

            fclose(fstats);
        }

        int dump_stats = G_wrap_stats[0].stat_dump_write_begin(phase_count, G_parms.display_name);

        if (dump_stats >= 0) {
            for (int iphase = 0; iphase < phase_count; iphase++) {
                G_wrap_stats[iphase].stat_dump_write(dump_stats);
            }
        }

        D1HT_CLOSE(dump_stats);

        G_stats_dumped = true;

        verbose(stderr, "Stats dumped (iterative=%d)\n", (int) default_parms.iterative);

        mysleep(300);
        G_never_died = true;

        if (!G_killed && G_D1HT_pid != 0) {
            G_killed = true;
            pid_t pid = G_D1HT_pid;
            G_D1HT_pid = 0;
            XEQ_EXEC(kill(pid, SIGTERM), XEQ_SILENT);
            mysleep(10);
            XEQ_EXEC(kill(pid, SIGKILL), XEQ_SILENT);
        }

        if (!default_parms.iterative) break;

        G_failed = 0;
    }

    VERBO(stderr, "Ending program\n");

    EXITOK;
}
