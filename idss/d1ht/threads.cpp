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
 * @file threads.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

typedef void 	sigfunc_t(int, siginfo_t*, void*);
extern 	void	signal_handler(int signo, sigfunc_t *func);

pthread_attr_t 	G_joinable, G_detached;
pthread_key_t 	G_pthread_data_key;	
pthread_data_t	*G_pthread_data_header;
pthread_t		G_TCP_thread_id = 0, G_UDP_thread_id = 0, G_SIG_thread_id = 0;

#ifndef D1HT_PTHREAD_LIGHT

void pthread_data_t::pthread_init(pthread_data_t *G_pthread_data_header) { 
	pthread_init();
#if defined(D1HT_PTHREAD_STATS)
	if (G_exiting) return;
	LOCK_FORCED(G_thread_data_mutex);
	G_stats.total.stats[stat_pthread_created].count++;
	next = G_pthread_data_header->next;
	prev = G_pthread_data_header;
	G_pthread_data_header->next = this;
	UNLOCK(G_thread_data_mutex);
#endif
}

void *pthread_data_t::pthread_destroy(pthread_data_t *G_pthread_data_header) {
#if defined(D1HT_PTHREAD_STATS)
	pthread_data_t	*pdata; 
	if (G_exiting || this == NULL) return NULL;
	LOCK_FORCED(G_thread_data_mutex);
	if (G_exiting) {
		UNLOCK(G_thread_data_mutex);
		return NULL;
	}
	G_stats.total.stats[stat_pthread_ended].count++;
	if (this->stat_ended > 0) G_stats.total.stats[this->stat_ended].count++;
	for (pdata=G_pthread_data_header; pdata!=NULL && pdata->next!=this; pdata=pdata->next);
	ASSERT(pdata!=NULL && !G_exiting);
	if (pdata != NULL && !G_exiting) pdata->next = this->next;
	UNLOCK(G_thread_data_mutex);
#endif
	return NULL;
}

void pthread_data_t::pthread_print(FILE* std) {
	fprintf(std, "thread name=%s tid=%u", pthread_name, (uint) tid);
	if (started)  fprintf(std, ", started");
	if (detached) fprintf(std, ", detached");
	if (may_lock) fprintf(std, ", may lock");
#if defined(XEQP_THREAD)
	fprintf(std, "\n          last xeq  =");
	xeq_last.print(std);
	
	fprintf(std, "\n          created in=");
	xeq_caller.print(std);
#endif
	if (holding_locks || waiting_lock) {
		fprintf(std, "\n          ");
		if (waiting_lock) fprintf(std, "Waiting for lock: %s, ", waiting_lock->name);
		if (holding_locks) {
			fprintf(std, "Holding locks: ");
			for (lock_t* lock=holding_locks; lock!=NULL; lock=lock->next_chain) fprintf(std, "%s,", lock->name);
		}
	}
	fprintf(std, "\n\n");
}

void print_pthread_name(FILE* std) {
	pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
	if (pdata) fprintf (std, "thread=%s, ", pdata->pthread_name);
}

void print_pthread_chain(FILE* std, char* file, uint line) {
	LOCK_FORCED(G_thread_data_mutex);
	VERB(std, file, line, "Printing pthread chain:\n\n");
	
	for (pthread_data_t	*pdata=G_pthread_data_header->next; pdata!=NULL; pdata=pdata->next) {
		ASSERT(pdata!=pdata->next);
		pdata->pthread_print(std);
	}
	
	UNLOCK(G_thread_data_mutex);
	fprintf(std, "\n");
	fflush(std);
}
#if defined(DEBUG)
void xeq_all_pthreads(const char* file, const uint line) {
	LOCK_FORCED(G_thread_data_mutex);
	
	for (pthread_data_t	*pdata=G_pthread_data_header->next; pdata!=NULL; pdata=pdata->next) {
		ASSERTFL(pdata!=pdata->next, pdata->pthread_name, file, line);
//		ASSERTFL(pdata->zero1==0, pdata->pthread_name, file, line);
//		ASSERTFL(pdata->zero2==0, pdata->pthread_name, file, line);
	}
	
	UNLOCK(G_thread_data_mutex);
}
#else
void xeq_all_pthreads(const char* file, const uint line) { }
#endif

void close_pthread_data(void* pthread_data) { 

	if (G_exiting) return;
#if defined(DEBUG)
    if (G_parms.check && !0) {
    	xeq_all_locks(__FILE__,__LINE__);
    	xeq_all_pthreads(__FILE__,__LINE__);
    }
#endif 
	pthread_data_t* p_data = (pthread_data_t *) pthread_data;
	
	if (p_data == NULL) return;
	
	if (p_data->holding_locks != NULL) {
		p_data->holding_locks->lock_print(stderr);
		EXIT("pthread exiting while holding lock %s\n", p_data->holding_locks->name);
	}
	if (p_data->waiting_lock != NULL) {
		p_data->holding_locks->lock_print(stderr);
		EXIT("pthread exiting while waiting for lock %s\n", p_data->waiting_lock->name);
	}

	p_data->pthread_destroy(G_pthread_data_header);
	
	ASSERT(p_data != NULL);
	
	if (G_exiting) return;

	FREE(p_data);
}
#else

void print_pthread_name(FILE* std) { }
void print_pthread_chain(FILE* std, char* file, uint line) { }
void xeq_all_pthreads(const char* file, const uint line) { }

void close_pthread_data(void* pthread_data) { 
	print_memory_used("close_pthread_data", "before", __FILE__, __LINE__);
	if (!G_exiting & pthread_data != NULL) FREE(pthread_data);
	print_memory_used("close_pthread_data", "after", __FILE__, __LINE__);
}

#endif

#if defined(XEQP_THREAD)
void pthread_data_t::xeq_print(FILE* std) {
	if (xeq_last.line != 0 || xeq_caller.line != 0) {
		fprintf(std, "thread=%s", pthread_name);
		if (xeq_last.line != 0) {
			fprintf(std, ", last xeq ");
			xeq_last.print(std);
		}
		if (xeq_caller.line != 0) {
			fprintf(std, ", caller xeq ");
			xeq_caller.print(std);
		}
		fprintf(std, "\n");
	}
}
#endif

} // namespace d1ht
