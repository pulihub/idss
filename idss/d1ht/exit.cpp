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
 * @file exit.c
 * @brief 
 */


#include "D1HT.h"
#include <wait.h>

namespace d1ht
{
extern	void 	signal_ignore(const int signo);
extern	void 	*close_interval_exit(void* dummy); 

pthread_once_t 	g_once	= PTHREAD_ONCE_INIT;
pthread_once_t 	g_dump 	= PTHREAD_ONCE_INIT;
char			g_cause[120];
char			g_file[120];
int 			g_line;

void Exit(const int retcode) { 
	G_exiting 		= true; 
	G_exited  		= true;
	G_parms.verbose = 0;
	G_parms.VERBOSE = 0;
#ifdef DEBUG_MALLOC
	dmalloc_shutdown(); 
#endif
	exit(retcode);
}

void Exit() { Exit(EXIT_SUCCESS); }

void D1HT_exit_nodump(const char* file, const uint line, const char* cause) {
	D1HT_exit(false, file, line, cause);
}

void D1HT_exit_dump(const char* file, const uint line, const char* cause) {
	D1HT_exit(true, file, line, cause);
}

void *close_and_exit() {
	void *retcode = NULL;
	PTHREAD_CREATE(close_interval_exit, NULL, &G_detached);
	return retcode;
}

void once_exit(void) {	
	
	G_stats.total.stats[stat_leaves].count = 1;
	
  	close_and_exit();
  	
	G_stats.exiting_stats(stderr);
	
    print_lock_stats(stderr,__FILE__,__LINE__);
  	
  	verbose(stderr,"D1HT peer leaving, cause=%s\n", g_cause);
}

#if defined(D1HT_PTHREAD_LIGHT)
void *dump(FILE *std, const char* file, const uint line, const char* cause) {
	(void) std;
	(void) file;
	(void) line;
	(void) cause;
	return NULL;
}
#else
void *dump(FILE *std, const char* file, const uint line, const char* cause) {
  	FILE 		*fdump;

  	if (std != 0) {
  		fdump = std;
  	} else { 
  	  	char		filename[256];
 		
	  	sprintf( filename, "dump_%s.txt", G_parms.base_filename);
	    
		XEQ_EXEC_NULL((fdump = fopen(filename, "w")), XEQ_WARNING);
		
		if (fdump == NULL) return NULL;

  		VERBO(stderr, "Printing dump of mutexes and active threads on %s\n", filename);
  	}
		
  	bool	should_unlock = false;
	
	VERB(fdump, file, line ,"Dumping status of threads and mutexes due to %s\n\n", cause);

	pthread_data_t	*pthread_data_mine 	= (pthread_data_t *) pthread_getspecific(G_pthread_data_key);	
	if (pthread_data_mine) {
		VERBO(fdump, "Interrupted thread: ");
		pthread_data_mine->pthread_print(fdump); 
	}
	fflush(fdump);				
	
	VERBO(fdump, "Printing lock chains:\n\n");
	
	if (!G_header_mutex.xeq_locked_byme()) { 
		LOCK_FORCED(G_header_mutex);
		should_unlock = true;
	}
	
	for (lock_t* lock=G_header_mutex.next_lock; lock!=NULL; lock=lock->next_lock) lock->lock_print(fdump); 
	
	if (should_unlock) UNLOCK(G_header_mutex);
	fprintf(fdump, "\n");
	fflush(fdump);				
									
	VERBO(fdump, "Printing other active pthreads:\n\n");
	
	should_unlock = false;
	if (!G_thread_data_mutex.xeq_locked_byme()) {
		LOCK_FORCED(G_thread_data_mutex);
		should_unlock = true;
	}
	
	for (pthread_data_t	*pdata=G_pthread_data_header->next; pdata!=NULL; pdata=pdata->next) {
		ASSERT(pdata!=pdata->next);
		if (pdata!=pthread_data_mine) pdata->pthread_print(fdump);
	}
	if (should_unlock) UNLOCK(G_thread_data_mutex);
	if (!G_thread_data_mutex.xeq_locked_byme()) {
		LOCK_FORCED(G_thread_data_mutex);
		should_unlock = true;
	}
	
	for (pthread_data_t	*pdata=G_pthread_data_header->next; pdata!=NULL; pdata=pdata->next) {
		ASSERT(pdata!=pdata->next);
		if (pdata!=pthread_data_mine) pdata->pthread_print(fdump);
	}
	if (should_unlock) UNLOCK(G_thread_data_mutex);

	if (std == 0) fclose(fdump);
	
	return NULL;
}
#endif
void dump(void) { 
	dump(0, g_file, g_line, g_cause); 
}

void D1HT_exit(const bool err, const char* file, const uint line, const char* cause, const bool should_exit) { PXEQ

	PTHREAD_MAYLOCK;

	ASSERT(! (err && !should_exit) );

	strncpy(g_cause, cause, sizeof(g_cause));;
	g_cause[sizeof(g_cause)-1] = 0;
	g_line	= line;

	strncpy(g_file, file, sizeof(g_file));;PXEQ

	g_file[sizeof(g_file)-1] = 0;

	G_parms.verbose		= 1;
	if (err) {PXEQ
		G_parms.VERBOSE		= 1;
		if (! D1HT_STATS_ON_ERR) G_parms.print_stats = false;
	}
	PXEQ
	if (G_exiting) {PXEQ
		if (err) pthread_once( &g_dump, dump );

  		unlock_try_all(__FILE__,__LINE__);

	} else {PXEQ

		G_exiting	= true;
		G_probe.stop_probing();PXEQ
		G_RT.stop_stab();PXEQ
		G_RT.leave();

#if defined(DEBUG)
		G_parms.selfmsgs 	= false;
#endif

        pthread_once( &g_once, once_exit );    PXEQ

		G_started	= false;

#ifndef D1HT_PTHREAD_LIGHT
        pthread_data_t  *pthread_data_mine = (pthread_data_t *) pthread_getspecific(G_pthread_data_key);
        PXEQ
        if (pthread_data_mine) {PXEQ
        	VERB(stderr, file, line ,"Interrupted thread: ");
        	pthread_data_mine->pthread_print(stderr);
        }
        PXEQ
        fflush(stderr);

        if (err) pthread_once( &g_dump, dump );PXEQ
#endif
        unlock_try_all(__FILE__,__LINE__);PXEQ

        G_exited = true;
	}PXEQ

	if (should_exit) {

		mysleep(10); // allow 10 seconds for close_interval_exit to finish
		closelog();PXEQ

		if (err) {
			Exit(EXIT_FAILURE);
		} else {
			Exit(EXIT_SUCCESS);
		}
	}
}

void D1HT_exit(const bool err, const char* file, const uint line, const char* cause) {
	D1HT_exit(err, file, line, cause, true);
}

} // namespace d1ht
