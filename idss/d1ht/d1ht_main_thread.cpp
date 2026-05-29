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
 *  No title to or ownership SIGINTof D1HT is hereby transferred.	                 *
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
 * @file d1ht_main_thread.c
 * @brief 
 */

#include "D1HT.h"
#include "d1ht_main_thread.h"
#include <sys/stat.h>
#include <exception>
#include <sys/sysinfo.h>


//#include "broadcast.h"

namespace d1ht {

    extern int get_tcp_socket(const PORTNO port);
    extern int get_udp_socket(const PORTNO port);
    extern lock_t g_theta_interval_mutex;
    extern void* theta_interval(void* dummy);
    extern void* set_signal_handlers();

    extern ttls_received_t G_ttls_received;

} // namespace d1ht

using namespace d1ht;

PTHREAD(d1ht_main_thread) {

    PTHREAD_DETACH;
    PTHREAD_INIT;

    PTHREAD_MAYLOCK;

    LOCK_INIT(g_theta_interval_mutex, maychain = true, 20);

    set_signal_handlers();

#if defined(DEBUG)  
    if (G_parms.test) {
        IPaddr IPold = G_parms.myIP;

        G_parms.myIP.port = htons(8100);
        G_parms.myIP.ip.s_addr = 100575424;

        G_noport0 = false;
        G_myID = ip2id(G_parms.myIP);
        G_xeq_find = false;

        routing_table RTX;
        LOCK(RTX.rt_mutex);
        RTX.RT_init(0);
        UNLOCK(RTX.rt_mutex);
        RTX.test_table(__FILE__, __LINE__);

        G_parms.myIP = IPold;
        G_noport0 = true;
        G_myID = ip2id(G_parms.myIP);
        G_xeq_find = false;
        G_parms.test = false;
    }
#endif  
#if defined(XEQP)
    VERBO(stderr, "** ATTENTION ** - Cause: XEQP flag ON. This execution MUST NOT not be use to gather statistics of any type!\n");
#endif
    // Assert that we can use the defined sockets 
    G_TCP_socket = get_tcp_socket(G_parms.myIP.port);
    G_UDP_socket = get_udp_socket(G_parms.myIP.port);

    G_lookup_port = htons(ntohs(G_parms.myIP.port) + 1);
    G_lookup_socket = get_udp_socket(G_lookup_port);

    LIMIT_SET(RLIMIT_DATA, 512000000);
    LIMIT_SET(RLIMIT_STACK, 512000000);
    LIMIT_SET(RLIMIT_AS, 512000000);
    LIMIT_MAX(RLIMIT_NPROC);

#if defined(DEBUG)
    {
        struct rlimit rl;
        XEQ_EXEC(getrlimit(RLIMIT_AS, &rl), XEQ_WARNING);
        G_max_pages = rl.rlim_cur / (4 * 1024);
        VERBOSE(stderr, "G_max_pages=%u\n", (uint) G_max_pages);
    }
#endif

    G_inter_current = CALLOC(1, interval_t);
    G_inter_current->interval_init(1, 0, false);

    // Before getting the RT, I should in fact create the TCP_server thread 
    // Its is necessary because as soon as my pred knows about me, it will start sending me msgs and
    // so I should be able to receive them otherwise my pred will think I've failed
    // Note that the TCP_server thread will receive those msgs but will block in rt_mutex while I am initialising and receiving my RT
    //
    G_started = false;
    LOCK(G_RT.rt_mutex);

    G_TCP_thread_id = PTHREAD_CREATE(TCP_server, NULL, &G_detached);
    G_UDP_thread_id = PTHREAD_CREATE(UDP_server, NULL, &G_detached);
    PTHREAD_CREATE(UDP_lookup_server, NULL, &G_detached);

    //  G_parms.VERBOSE = 1;
    //  G_parms.verbose = 1;

    G_stats.start_stats();
    int tries = 1;
    if (G_parms.NMAX) {
        Assert(G_parms.systemid != 0, XEQ_ERROR);
        G_RT.RT_init(0);
        Assert(((G_RT.pub_locked_npeers() == 1) || G_parms.input_RT), XEQ_ERROR);
        G_started = true;
    } else {
        mysleep(1); // let's the TCP server initiate
        bool join_OK = false;
        G_known_peers.push(G_known_peers.top()); // let's try twice the first peer

        while (!(join_OK = G_RT.get()) && !G_known_peers.empty() && (tries < 10 * G_parms.retries)) {
            if (G_forward_connection >= 0) {

                D1HT_CLOSE(G_forward_connection);
                G_forward_connection = -1;
                G_ttls_received.stop_notify(__FILE__, __LINE__);
            }

            UNLOCK(G_RT.rt_mutex); // unlock rt_mutex in order to clear pending forwarded events


            if (G_exiting) break; // if already exiting, give up

            if (G_known_peers.empty()) break;

            tries++;
            verbose(stderr, "Join try=%d target=%s:%u, my systemid=%u\n", tries, IPtoP(G_known_peers.top()), G_parms.systemid);

            LOCK(G_RT.rt_mutex);
        }
        if (join_OK) {
            G_started = true;
            if (G_RT.pub_locked_npeers() <= 1) EXIT("Routing table received during join is corrupted")
            } else {
            VERBO(stderr, "ERROR Cause: D1HT join failed\n");
            G_started = false;
        }
    }

    UNLOCK(G_RT.rt_mutex);

    if (G_started) {
        PTHREAD_CREATE(theta_interval, NULL, &G_detached);

        verbose(stderr, "D1HT Join finished OK! npeers=%u, Max peers=%u, my systemid=%u, tries=%d\n", (unsigned) G_RT.pub_npeers(), G_parms.NMAX, G_parms.systemid, tries);
        Assert(G_parms.systemid != 0, XEQ_ERROR);
#if defined(DEBUG)
        XEQ_TRUE(G_RT.pub_xeq_table(true, __FILE__, __LINE__), "RT corrupted after join.", XEQ_ERROR);
#endif
    }


//    query_t miomess = query_t();
//    miomess.printQuery();

    return NULL;
}
