/**
 * @file d1ht.c
 * @brief
 * p2p_d1ht.cc
 * This class encapsulates all D1HT infrastructure.
 *
 *  Created on: Nov 23, 2010
 *      Author: anderson
 *
 *  Modified on: Aug 17, 2012 by Luiz Monnerat
 */

#include "d1ht_h.h"
#include "d1ht_main_thread.h"


#include "QueryManager.h"
#include "Query.h"

using namespace d1ht;

#include <sys/stat.h>
#include <exception>
#include <sys/sysinfo.h>

namespace d1ht{

    extern void print_usage(const int is_error, const int argc, char** argv);
    extern void D1HT_exit(const bool err, const char* file, const uint line, const char* cause, const bool should_exit);

    /*
     * This constructor must start all D1HT threads.
     */

    d1ht_t::d1ht_t(int argc, char** argv) {
        start_d1ht(argc, argv, false);
    }

    d1ht_t::d1ht_t(int argc, char** argv, const bool _wait_join) {
        start_d1ht(argc, argv, _wait_join);
    }

   /*
    * This constructor also initializes callbacks for "query" messages.
    */
    //d1ht_t::d1ht_t(int argc, char** argv, void (*response_received)(void *p), void (*query_received)(char *sql, char *uqi, int ttl, long origin_key, char *sender) ){
    d1ht_t::d1ht_t(int argc, char** argv, void (*response_received)(void *args), void (*query_received)(void *args)) { 
        G_QM.set_query_received_callback(query_received);
        G_QM.set_response_received_callback(response_received);

        start_d1ht(argc, argv, false);        
    }

    /*
     * This destructor must stop all D1HT threads.
     */
    d1ht_t::~d1ht_t() {
        stop_d1ht(false, "D1HT peer leaving normally", false);
    }

    void d1ht_t::stop_d1ht(const bool err, const char* cause, const bool should_exit) {

        D1HT_exit(err, __FILE__, __LINE__, cause, should_exit);

        unblock_SIGCONT();

        if (d1ht_ptid != 0) pthread_cancel(d1ht_ptid);
        if (G_SIG_thread_id != 0) pthread_cancel(G_SIG_thread_id);
        if (G_TCP_thread_id != 0) pthread_cancel(G_TCP_thread_id);
        if (G_UDP_thread_id != 0) pthread_cancel(G_UDP_thread_id);
    }

    void d1ht_t::start_d1ht(int argc, char** argv, const bool _wait_join) {

        pthread_key_create(&G_pthread_data_key, close_pthread_data);

        d1ht_ptid = 0;

#if defined(DEBUG)
#if defined(MCHEK)
        mcheck(&abortfn);
#endif
        G_statm = fopen("/proc/self/statm", "r");
#endif

        pthread_attr_init(&G_joinable);
        pthread_attr_setdetachstate(&G_joinable, PTHREAD_CREATE_JOINABLE);

        pthread_attr_init(&G_detached);
        pthread_attr_setdetachstate(&G_detached, PTHREAD_CREATE_DETACHED);

        pthread_data_t *pthread_data_mine;

        LOCK_INIT(G_interval_mutex, maychain = false, 80);
        LOCK_INIT(G_thread_data_mutex, maychain = true, 13);

#if defined(DEBUG)
        LOCK_INIT(G_selfmsgs_mutex, maychain = true, 90);
#if defined(MEMLOCK)
        LOCK_INIT(G_malloc_mutex, maychain = true, 10);
#endif
#endif

        G_pthread_data_header = CALLOC(1, pthread_data_t);

        pthread_data_mine = CALLOC(1, pthread_data_t);

        pthread_key_create(&G_pthread_data_key, close_pthread_data);
        pthread_setspecific(G_pthread_data_key, pthread_data_mine);

#ifndef D1HT_PTHREAD_LIGHT
        G_pthread_data_header->next = pthread_data_mine;
        pthread_data_mine->prev = G_pthread_data_header;
        pthread_data_mine->tid = pthread_self();

        strncpy(pthread_data_mine->pthread_name, "d1ht_main_thread", D1HT_PTHREAD_NAME_SIZE - 1);
        pthread_data_mine->started = true;

#if defined(XEQP)
        XEQ(pthread_data_mine->xeq_caller);
        pthread_data_mine->xeq_last = pthread_data_mine->xeq_caller;
#endif
        pthread_data_mine->pthread_init();
#endif

        G_parms.program_name = "D1HT";

        G_parms.get(argc, argv);

        set_terminate(my_terminate);

        G_parms.program_name = "D1HT";

        if (G_parms.daemonize) { // daemonize the process
            VERBO(stdout, "I will run as a daemon - closing files (including stdin, stdout and stderr)\n");
            VERBO(stderr, "I will run as a daemon - closing files (including stdin, stdout and stderr)\n")
            for (int i = 0; i < 16; i++) D1HT_CLOSE(i);
            openlog("D1HT ", LOG_PERROR | LOG_PID | LOG_CONS, LOG_USER); // open syslog
            XEQ_EXEC(daemon(1, 0), XEQ_ERROR);
        }

        if (G_parms.outfile) {
            char filename[256];
            if (G_parms.local_output) {
                sprintf(filename, "/tmp/e_%s.txt", G_parms.base_filename);
            } else {
                sprintf(filename, "e_%s.txt", G_parms.base_filename);
            }
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

        if (G_parms.NMAX) {
            G_parms.rho_max = (int) log2((float) G_parms.NMAX);

            if (!G_known_peers.empty() && !G_parms.input_RT) VERBO(stderr, "WARNING Cause: You specified both -N and -A.\n");

            if (G_parms.rho_max > D1HT_RHO_MAX) {
                VERBO(stderr, "ERROR Cause: NMAX should be smaller than 2^%d !\n", D1HT_RHO_MAX);
                print_usage(1, G_argc, G_argv);
            }

            if (G_parms.input_RT && G_parms.systemid == 0) {
                VERBO(stderr, "ERROR Cause: If you don't specify -s you may not specify --routing_table or -e!\n");
                print_usage(1, G_argc, G_argv);
            }
        } else {

            if (G_known_peers.empty()) {
                VERBO(stderr, "ERROR Cause: If you don't specify -N you must specify -A or -F !\n");
                print_usage(1, G_argc, G_argv);
            }
        }

        if (G_parms.NMAX != 0 && G_parms.systemid == 0) G_parms.systemid = (G_myID / 2) + ((KEYID) G_parms.program_start.tv_sec);

        pid_t procid = getpid();

        VERBO(stderr, "My address is %s:%u, my name is %s, procid=%u, NMAX=%d, myID=%u, default port=%u\n",
        IPtoP(G_parms.myIP), G_parms.my_host_name, procid, G_parms.NMAX, G_myID, ntohs(G_parms.default_udp_port));

        if (G_parms.original_port != G_parms.myIP.port) {
            VERBO(stderr, "My address would be %s:%u but I changed the port %u because the original port was occupied\n",
                    inet_ntoa(G_parms.myIP.ip), ntohs(G_parms.original_port), ntohs(G_parms.myIP.port));
        }

        Assert(((G_parms.default_udp_port != 0) && (G_parms.default_udp_port != ntohs(0))), XEQ_ERROR, "Invalid default UDP port");

        if (BigEndian()) {
            fprintf(stdout, "** ATTENTION ** - Cause: You are running D1HT in a machine with big-endian byte order and/or\n");
            fprintf(stdout, "                 short integers with size different than two bytes. D1HT may work OK\n");
            fprintf(stdout, "                 in this machine, but you should be aware that D1HT has been developed and\n");
            fprintf(stdout, "                 mostly debugged in machines with a different architecture.\n\n");

            VERBO(stderr, "** ATTENTION ** - Cause: You are running D1HT in a machine with big-endian byte order and/or\n"
                    "                  short integers with size different than two bytes. D1HT may work OK\n"
                    "                  in this machine, but you should be aware that D1HT has been developed and\n"
                    "                  mostly debugged in machines with a different architecture.\n\n");
        }

        block_SIGCONT();

#if defined(DEBUG)
        VERBO(stderr, "** ATTENTION ** - Cause: DEBUG flag ON. This execution MUST NOT not be use to gather statistics of any type!\n"
        "This program has being compiled with the DEBUG flag turned on, which severely hurts its performance and its\n"
        "ability to cope with workloads and to timely propagate the events. Because of that, this (and any other D1HT\n"
        "executable generated with the DEBUG option) MUST NEVER be used to gather ANY statistics of ANY type.\n\n");
#endif

        d1ht_ptid = PTHREAD_CREATE(d1ht_main_thread, NULL, &G_joinable);
        int errsave = errno;

        bool should_exit = _wait_join;

        if (d1ht_ptid == 0) {
            char msgerr[128];
            sprintf(msgerr, "Unable to create D1HT main thread. msg=%s", strerror(errsave));
            stop_d1ht(true, msgerr, should_exit);
            errno = errsave;
        } else if (_wait_join && !wait_join()) {
            errsave = errno;
            if (errno == 0) errsave = ESHUTDOWN;
            stop_d1ht(true, "Unable to start D1HT", should_exit);
        } else {
            ASSERT(errsave == 0);
        }
        errno = errsave;
    }

    // wait for D1HT to start and return if it is ok

    bool d1ht_t::wait_join() {
        if (d1ht_ptid == 0) {
            //		 wait_join should have already been called
            if (G_started && !G_exiting && !G_exited) {
                errno = 0;
                return true;
            } else {
                G_exiting = true;
                VERBO(stderr, "ERROR Cause: Invalid wait_join call\n");
                errno = EDEADLK;
                return false;
            }
        }

        int errsave = pthread_join(d1ht_ptid, NULL);

        unblock_SIGCONT();

        d1ht_ptid = 0;

        if (errsave != 0) {
            char msgerr[128];
            sprintf(msgerr, "Unable to join the D1HT main thread. msg=%s", strerror(errsave));
            stop_d1ht(true, msgerr, false);
            errno = errsave;
            return false;
        }

        if (G_exiting || !G_started) {
            stop_d1ht(true, "D1HT failed to join", false);
            errno = ESHUTDOWN;
            return false;
        }

        ASSERT(G_started);
        ASSERT(!G_exiting);

        errno = 0;
        return true;
    }

    /*
     * Returns the IP address associated to key.
     */
    bool d1ht_t::lookup(const char key[D1HT_MAX_KEY_SIZE], IPaddr & addr) {
        return lookup(key2id(key), addr);
    }

    /*
     * Returns the IP address associated to key.
     */
    bool d1ht_t::lookup(const KEYID key, IPaddr & addr) {
        // This function performas a lookup for (KEYID) key
        //
        // On success, it returns true and the IPaddr of the home peer in addr
        //
        // On error, it returns false, and errno is set as below:
        //
        //		ESHUTDOWN, strerror="Cannot send after transport endpoint shutdown": D1HT is leaving the network
        //
        //		ENOTCONN, strerror="Transport endpoint is not connected", D1HT hasnt yet finished its joining to the D1HT network
        //
        //		EAGAIN, strerror="Try again", D1HT could not solve the lookup after some atempts. This should e a transitory error.
        //
        //		Additional error codes may be available in the definition of lookup(D1HT_LOOKUP_body *lookup_bodyp, void* payload, void* reply_payload)
        //					in file RT.h
        //

        if (G_exiting) {
            errno = ESHUTDOWN;
            return false;
        }

        if (!G_started) {
            errno = ENOTCONN;
            return false;
        }

        D1HT_LOOKUP_body lookup_body;
        D1HT_LOOKUP_reply_body reply_body;

        lookup_body.id = key;
        lookup_body.prev_target.port = 0;
        lookup_body.payload_len = 0;
        lookup_body.max_reply_payload_len = 0;

        reply_body = G_RT.lookup(&lookup_body);

        if (reply_body.hops >= 10) {

            VERBOSE(stderr, "WARNING Cause: Lookup failed. Detail: id=%.10u\n", lookup_body.id);

            errno = EAGAIN;
            if (G_exiting) errno = ESHUTDOWN;
            return false;

        } else {

            addr.ip = reply_body.home.ip;
            addr.port = reply_body.home.port;
            errno = 0;
            return true;
        }
    }

    /*
     *
     */
    bool d1ht_t::route(const char key[D1HT_MAX_KEY_SIZE], IPaddr& addr, void* payload, const int payload_len, void* reply_payload, int reply_payload_len) {
        return route(key2id(key), addr, payload, payload_len, reply_payload, reply_payload_len);
    }

    /*
     *
     */
    bool d1ht_t::route(const KEYID key, IPaddr& addr, void* payload, const int payload_len, void* reply_payload, int reply_payload_len) {

        if (!G_started) {
            verbose(stderr, "WARNING Cause: d1ht_t::route failed because D1HT is not ready yet. Detail id=%u\n", key);
            errno = ENOTCONN;
            return false;
        }

        if (G_exiting) {
            verbose(stderr, "WARNING Cause: d1ht_t::route failed because D1HT is exiting. Detail id=%u\n", key);
            errno = ESHUTDOWN;
            return false;
        }

        D1HT_LOOKUP_body lookup_body;
        D1HT_LOOKUP_reply_body reply_body;

        lookup_body.id = key;
        lookup_body.prev_target.port = 0;
        lookup_body.payload_len = payload_len;
        lookup_body.max_reply_payload_len = reply_payload_len;

        reply_body = G_RT.lookup(&lookup_body, payload, reply_payload);

        if (reply_body.hops >= 10) {
            VERBOSE(stderr, "WARNING Cause: lookup failed. Detail id=%.10u\n", lookup_body.id);
            errno = EAGAIN;
            if (G_exiting) errno = ESHUTDOWN;
            return false;
        }

        addr.ip = reply_body.home.ip;
        addr.port = reply_body.home.port;

        errno = 0;

        return true;
    }

    bool d1ht_t::succ(const IPaddr& peer, const int count, IPaddr & succ) {

        if (!G_started) {
            verbose(stderr, "WARNING Cause: d1ht_t::succ failed because D1HT is not ready yet\n");
            errno = ENOTCONN;
            return false;
        }

        if (G_exiting) {
            verbose(stderr, "WARNING Cause: d1ht_t::succ failed because D1HT is exiting\n");
            errno = ESHUTDOWN;
            return false;
        }

        if (count >= 0) {
            succ = G_RT.pub_succn(ip2id(peer), count);
        } else {
            succ = G_RT.pub_predn(ip2id(peer), (-count));
        }
        return true;
    }
    

} // namespace d1ht
