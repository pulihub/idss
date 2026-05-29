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
 * @file D1HT.h
 * @brief file di interfaccia alla libreria. 
 * Le implementazioni sono in lookup.c e le macro in messages.h
 */

#ifndef D1HT_H
#define D1HT_H

#include "base.h"

namespace d1ht {

#define	D1HT_STATS_ON_ERR		true		

#define	D1HT_RHO_MAX			20
#define	D1HT_MTU			1500
#define	D1HT_MAX_THETA			20000	 	/**< max duration of the theta intervals in miliseconds */
#define	D1HT_MIN_THETA			300		/**< min duration of the theta intervals in miliseconds */
#define	D1HT_INIT_THETA			2000		/**< default initial duration of the theta intervals in milliseconds */
#define D1HT_UDP_HEADER_SIZE            28		/**< size in bytes of the IP+UDP headers. It is 28 bytes for IPv4 (20 bytes for the IP header plus the 8 bytes for the UDP header) */
#define D1HT_MAX_KEY_SIZE		40
#define D1HT_UDP_BUFFER_SIZE            64*1024
#define D1HT_TCP_BUFFER_SIZE            512*1024
#define D1HT_STAB_INTERVAL		20              /**< minimum interval in seconds between 2 stabilizations against the same peer */
#define D1HT_MAX_PAYLOAD_SIZE           1024

#define D1HT_DEFAULT_PORT		8100

#define D1HT_TIME_FORMAT		"%F-%H-%M-%S"
#define D1HT_HOSTNAME_FORMAT            ":%s:%u:"
#define D1HT_HOSTNAME_MAXLENGTH         80

#define D1HT_MAX_RETRIES		3
//#define D1HT_JOIN_COPY_RT

    // the 3 msecs RXTMIN is OK for LANs, but for WANs it should be properly tuned
#define	D1HT_RTT_RXTMIN         100		/**< min retransmit timeout value, microseconds */ 
#define	D1HT_RTT_RXTMAX     	500000          /**< max retransmit timeout value, microseconds */
#define	D1HT_RTT_VARMULT     	2		/**< RTT var multiplier - you should increase it if there are to many retransmissions */
#define	D1HT_TCP_TIMEO     	2               /**< TCP timeoout connection, in seconds */

#if defined(DEBUG)
#define D1HT_PTHREAD_STATS
#undef 	D1HT_PTHREAD_LIGHT
#define XEQP
#else
#if defined(MEMLOCK) || defined(MCHECK) || defined(DEBUG_MALLOC)
#error "The flags XEQP, MEMLOCK, MCHECK and DEBUG_MALLOC should only be used in conjunction with the flag DEBUG"
#endif
#undef 	D1HT_PTHREAD_STATS
#define	D1HT_PTHREAD_LIGHT
#endif

#if defined(D1HT_PTHREAD_LIGHT)
#undef 	D1HT_PTHREAD_STATS
#endif

#include "include.h"

    /**
     * D1HT_connect esegue la connessione TCP verso IP e porta specificati nella chiamata. 
     * Definita in lookup.c
     * @param target_ip
     * @param target_port
     * @return intero, pari al socket connesso
     */
    extern int D1HT_connect(const in_addr target_ip, const in_port_t target_port);
    
    /**
     * chiusura di una connessione TCP
     * @param connection
     * @return 
     */
    extern int D1HT_close(int *connection);
    
    /**
     * esecuzione di un lookup
     * @param key
     * @param key_len
     * @param connection
     * @param home_ip
     * @param home_port
     * @return 
     */
    extern int D1HT_lookup(const char *key, const int key_len, int *connection, struct in_addr *home_ip, in_port_t *home_port);
    
    /**
     * ricerca del successore del nodo indicato
     * @param peer
     * @param count
     * @param connection
     * @param succ
     * @return 
     */
    extern int D1HT_succ(const IPaddr peer, const int count, int *connection, IPaddr *succ);

    /**
     * routing di un messaggio
     * @param key
     * @param key_len
     * @param connection
     * @param home_ip
     * @param home_port
     * @param payload
     * @param payload_len
     * @param reply_payload
     * @param reply_len
     * @return 
     */
    extern int D1HT_route(const char *key, const int key_len, int *connection, struct in_addr *home_ip, in_port_t *home_port,
            const void* payload, const ssize_t payload_len, void* reply_payload, ssize_t *reply_len);

    extern int D1HT_get_stats(int connection, stat_part_t *stats, const int Savg);
    extern int D1HT_get_stats_partial(int connection, stat_part_t *stats, const int Savg);
    extern int D1HT_get_stats_total(int connection, stat_part_t *stats, const int Savg);

} // namespace d1ht

#endif  /* D1HT_H */
