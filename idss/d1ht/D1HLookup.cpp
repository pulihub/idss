/**
 * @file D1HLookup.c
 * @brief Sembra un file di test per le fasi dell'algoritmo (?)
 */

#include "D1HWrap.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sysinfo.h>

namespace d1ht {

    extern void* lookup_thread(void* lookup_rate);
    extern void* set_signal_handlers();

    class phase_t {
    public:
        char name[8];
        int lookup_rate; // lookups p/min
        int duration; // duration in seconds	
        bool wait_SIGCONT; // true (1) if we should wait for a SIGCONT signal to start the phase
        bool iterative; // true (1) if D1HWrap should loop waiting for new phases files
    };

    int G_phase;
    pid_t G_D1HT_pid;
    bool G_killed = true;
    bool G_failed = false;
    bool G_never_died = true;
    stat_part_t G_wrap_stats[MAX_PHASES];
    lock_t G_event_mutex;
    lock_t G_wrap_stats_mutex;
    pthread_t G_pthread_lookup_id = 0;
    int G_stats_connection = -1;
}

using namespace d1ht;

int main(int argc, char** argv) {
#if defined(MCHEK)
    mcheck(&abortfn);
#endif

    G_UDP_thread_id = 0;

    FILE* input;
    FILE* fstats = NULL;
    char buffer[8192];
    char* match;
    size_t bytes_read;

    phase_t phases[MAX_PHASES];
    phase_t default_parms;

    G_UDP_thread_id = 0;
    ASSERT(MAX_PHASES < 9);

    pthread_data_t *pthread_data_mine;

    LOCK_INIT(G_header_mutex, maychain = false, 15);
    LOCK_INIT(G_thread_data_mutex, maychain = true, 20);

#if defined(MEMLOCK)
    LOCK_INIT(G_malloc_mutex, maychain = true, 10);
#endif

    LOCK_INIT(G_wrap_stats_mutex, maychain = true, 21);

    G_pthread_data_header = CALLOC(1, pthread_data_t);
    pthread_data_mine = CALLOC(1, pthread_data_t);

    pthread_key_create(&G_pthread_data_key, close_pthread_data);
    pthread_setspecific(G_pthread_data_key, pthread_data_mine);

    G_pthread_data_header->next = pthread_data_mine;
#if defined(DEBUG)
    pthread_data_mine->prev = G_pthread_data_header;
    pthread_data_mine->tid = pthread_self();
    pthread_data_mine->started = true;
#endif
    strncpy(pthread_data_mine->pthread_name, "main", D1HT_PTHREAD_NAME_SIZE - 1);
    XEQ(pthread_data_mine->xeq_caller);
#if defined(XEQP)
    pthread_data_mine->xeq_last = pthread_data_mine->xeq_caller;
#endif

    pthread_data_mine->pthread_init();
    PTHREAD_MAYLOCK;

    G_parms.VERBOSE = true;
    G_parms.verbose = true;

    G_parms.program_name = "D1HLookup";

    IPaddr serverIP;

    get_IPaddr(argv[2], &(serverIP));

    char display_name[120];
    char myname[80];

    get_myName(myname, sizeof (myname), &(G_parms.myIP.ip));
    G_parms.myIP.port = 0;

    sprintf(display_name, D1HT_HOSTNAME_FORMAT, IPtoP(G_parms.myIP));
    Assert(G_parms.myIP.ip.s_addr != serverIP.ip.s_addr, XEQ_ERROR);

    for (int i = 0; i < 16; i++) D1HT_CLOSE(i);

    //	openlog( "D1HLookup ", LOG_PERROR|LOG_PID|LOG_CONS, LOG_USER); // open syslog  
    XEQ_EXEC(daemon(1, 0), XEQ_ERROR);

    char filename[256];
    char time_string[20];
    {
        struct timeval tnow;
        struct tm *ptm;

        gettimeofday(&tnow, NULL);
        ptm = localtime(&tnow.tv_sec);
        strftime(time_string, sizeof (time_string), D1HT_TIME_FORMAT, ptm);

        sprintf(filename, "e_%s:%s:%u%s%s_%u.txt", G_parms.program_name, IPtoP(serverIP), display_name, time_string, getpid());
        XEQ_EXEC_NULL(freopen(filename, "w", stderr), XEQ_ERROR);
    }

    if (G_argc != argc) {
        VERBO(stderr, "Submitted command=");
        for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
        fprintf(stderr, "\n");
    }
    VERBO(stderr, "Running command=");
    for (int i = 0; i < G_argc; i++) fprintf(stderr, "%s ", G_argv[i]);
    fprintf(stderr, "\n");

    VERBO(stderr, "My IP=%s, My name=%s, My pid=%u, Target Lookup IP = %s:%u\n", display_name, myname, getpid(), IPtoP(serverIP))

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

        VERBO(stderr, "Waiting for SIGHUP to read file %s and start\n", argv[1]);
        sigsuspend(&wait_SIGHUP); // block waiting SIGHUP
        VERBO(stderr, "SIGHUP catched\n");

        if (!(input = fopen(argv[1], "r"))) EXIT_WRAP("opening input file");

        if (!(bytes_read = fread(buffer, 1, sizeof (buffer), input))) EXIT_WRAP("reading input file");

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
        FIND(defp, wait_SIGCONT);
        FIND(defp, iterative);

        if (default_parms.duration != 0) EXIT_WRAP("You may not specify a default duration");

        verbose(stderr, "default parms: iterative=%u lookup_rate=%u lookups/min wait_SIGCONT=%u \n",
                defp->iterative, defp->lookup_rate, defp->wait_SIGCONT);

        int phase_count = 0;

        for (int i = 0; i < MAX_PHASES; i++) {

            phase_t* p = &phases[phase_count];

            sprintf(p->name, "%s%u", "phase", i);

            if (strstr(match, p->name) != NULL) {
                FIND(p, duration);
                FIND(p, lookup_rate);
                FIND(p, wait_SIGCONT);

                verbose(stderr, "phase %s: duration=%u secs, lookup_rate=%u lookups/min wait_SIGCONT=%u \n",
                        p->name, p->duration, p->lookup_rate, p->wait_SIGCONT);

                if (p->duration < 30) EXIT_WRAP("All phase durations should be bigger than 30 secs\n");

                phase_count++;
            }
        }

        Assert(phase_count != 0, XEQ_ERROR);
        G_killed = false;

        for (G_phase = 0; G_phase < phase_count; G_phase++) {

            phase_t* p;
            int time_left;
            pthread_t pthread_lookup_id = 0;

            p = &phases[G_phase];

            G_wrap_stats[G_phase].init();
            G_wrap_stats[G_phase].times.time_start();

            if (p->wait_SIGCONT) {
                VERBO(stderr, "Waiting for SIGCONT to start %s\n", p->name);
                fflush(stderr);
                sigsuspend(&wait_SIGCONT); // block waiting SIGCONT
            }

            struct sysinfo loadinfo;
            XEQ_EXEC(sysinfo(&loadinfo), XEQ_ERROR);
            G_wrap_stats[G_phase].stats[stat_loadavg15].count = 1;
            G_wrap_stats[G_phase].stats[stat_loadavg15].value = ((double) loadinfo.loads[2]) * 1000.0 / 65536.0;

            VERBO(stderr, "%s started: dur=%u lrate=%u \n", p->name, p->duration, p->lookup_rate);

            fflush(stderr);

            time_left = p->duration;

            if (p->lookup_rate != 0) {

                //				G_parms.VERBOSE = 0;
                //				G_parms.verbose = 0;

                struct lookup_parms_t *lookup_parms = CALLOC(1, struct lookup_parms_t);

                lookup_parms->lookup_rate = p->lookup_rate;
                lookup_parms->phase = G_phase;
                lookup_parms->peer = serverIP;
                pthread_lookup_id = PTHREAD_CREATE(lookup_thread, lookup_parms, &G_detached);
            }

            mysleep(time_left);

            if (pthread_lookup_id) {
                PTXEQ(pthread_cancel(pthread_lookup_id), XEQ_SILENT);
                pthread_lookup_id = 0;
            }

            G_wrap_stats[G_phase].times.time_get();

            G_parms.VERBOSE = 1;
            G_parms.verbose = 1;
        }
        G_phase = -1;

        verbose(stderr, "Final phase ended, G_D1HT_pid=%u\n", G_D1HT_pid);

        sprintf(filename, "stats_%s:%s:%u%s%s_%u.txt", G_parms.program_name, IPtoP(serverIP), display_name, time_string, getpid());
        XEQ_EXEC_NULL(fstats = fopen(filename, "w"), XEQ_ERROR);

        for (G_phase = 0; G_phase < phase_count; G_phase++) {
            G_wrap_stats[G_phase].stat_print(fstats, phases[G_phase].name);
        }
        fclose(fstats);

        if (!default_parms.iterative) break;
    }

    verbose(stderr, "Ending program\n");
}
