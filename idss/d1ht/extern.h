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
 @file extern.h
 @brief funzioni di utilità, calcolo delle Keys, variabili globali
 */

#ifndef EXTERN_H_
#define EXTERN_H_

class D1HT_LOOKUP_EXTERN_body;
class stat_part_t;

extern char* w_dir; //working directory

extern int G_Savg;

extern void* mysleep(const int sleep_sec, const int sleep_usec);
extern void* mysleep(const int sleep_sec);
extern void* mysleep_msecs(const int sleep_msecs);

extern int Accept(const int fd, struct sockaddr *sa, socklen_t *salenptr);
extern int Connect(const int fd, const struct sockaddr *sa, const socklen_t salen);

extern void* TCP_server(void* dummy);
extern void* UDP_server(void* dummy);
extern void* UDP_lookup_server(void* dummy);

extern void close_pthread_data(void* pthread_data);

extern void print_pthread_chain(FILE* std, char* file, uint line);
extern void print_pthread_name(FILE* std);

extern void D1HT_exit(const bool err, const char* file, const uint line, const char* cause);
extern void D1HT_exit_nodump(const char* file, const uint line, const char* cause);
extern void D1HT_exit_dump(const char* file, const uint line, const char* cause);
extern void Exit(const int retcode);

extern void block_SIGCONT();
extern void unblock_SIGCONT();

extern KEYID ip2id(const IPaddr ip);
extern KEYID ip2id(const IP ip);
extern KEYID ip2id(const IP address, const PORTNO port);
extern KEYID key2id(D1HT_LOOKUP_EXTERN_body lookup_body);
extern KEYID key2id(const char* key);
extern KEYID G_myID;

extern int G_UDP_socket, G_TCP_socket, G_lookup_socket;
extern PORTNO G_lookup_port;

extern std::stack<IPaddr> G_known_peers;

extern bool G_started;
extern bool G_exiting;
extern bool G_exited;

extern int G_argc;
extern char** G_argv;
extern char G_additional_parms[4][12];

extern void my_terminate();
extern bool BigEndian();

#if defined(DEBUG)
extern bool G_noport0;
extern bool G_succ_pred_err;
extern bool G_xeq_find;
#endif

#endif /*EXTERN_H_*/
