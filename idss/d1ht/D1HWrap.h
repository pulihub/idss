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
 * @file D1HWrap.h
 * @brief definizione di macro
 */


#ifndef WRAPER_H_
#define WRAPER_H_
#include "D1HT.h"

namespace d1ht {
    // MAX_PHASES MUST not be bigger than 8 
#define MAX_PHASES 8   

#define EXIT_WRAP( ... ) { 																	\
	if (G_pthread_lookup_id) {																\
		PTXEQ(pthread_cancel(G_pthread_lookup_id), XEQ_SILENT);								\
		G_pthread_lookup_id = 0;															\
	}																						\
	if (!G_killed && G_D1HT_pid!=0) {														\
		G_killed	= true;																	\
		pid_t pid 	= G_D1HT_pid;															\
		G_D1HT_pid 	= 0;																	\
		XEQ_EXEC(kill(pid, SIGTERM), XEQ_SILENT);											\
		mysleep(10);																		\
		XEQ_EXEC(kill(pid, SIGKILL), XEQ_SILENT);											\
	}																						\
	EXIT( __VA_ARGS__ );																	\
}

    struct lookup_parms_t {
        IPaddr peer;
        int lookup_rate;
        int phase;
    };

    extern char G_new_port[12];
    extern int G_phase;
    extern char* G_D1HT;
    extern pid_t G_D1HT_pid;
    extern bool G_killed;
    extern bool G_failed;
    extern bool G_never_died;
    extern bool G_stats_dumped;
    extern lock_t G_event_mutex;
    extern pthread_t G_pthread_lookup_id;
    extern lock_t G_wrap_stats_mutex;
    extern int G_stats_connection;
    extern stat_part_t G_wrap_stats[MAX_PHASES];

    extern void* close_stats_connection();
    extern void* open_stats_connection();
    extern void* D1HT_fork(const char* file, const uint line);
    extern void* D1HT_kill(const int signum, const char* file, const uint line);
    extern void* add_stats(const int phase);

#define FIND( _phase, _parm ) {											\
	char* 	_match; 													\
	char	_def[80];													\
	_phase->_parm = default_parms._parm;								\
 	sprintf( _def, "%s.%s=", _phase->name, #_parm);						\
 	if ( (_match = strstr( buffer, _def )) != NULL ) {					\
 		uint _temp_val;													\
	 	_match = strstr( _match, "=" );									\
	 	sscanf(_match, "=%u", &_temp_val);								\
	 	_phase->_parm = (typeof(_phase->_parm)) _temp_val;				\
 	}																	\
}
} // namespace d1ht

#endif /*WRAPER_H_*/
