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
 * @file lock.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

static pthread_mutexattr_t 	g_lock_attr;	          
static pthread_once_t 		g_lock_attr_once = PTHREAD_ONCE_INIT;

void g_lock_attr_init()
{ 
	PTXEQ(pthread_mutexattr_init(&g_lock_attr), XEQ_ERROR);
	PTXEQ(pthread_mutexattr_settype(&g_lock_attr, PTHREAD_MUTEX_ERRORCHECK_NP), XEQ_ERROR); 
	G_header_mutex.init_base("G_header_mutex", false, 15); 
}

#if defined(DEBUG)
void* lock_t::lock(const char* filename, const uint linenumber, bool forced) {
	
	ASSERT(filename!=NULL);
	ASSERT(name!=NULL);
	
	if (!G_exiting) {
		bool	washeld = false;
		if (locked) {
			washeld = true;
				
			if (G_parms.VERBOSE) {
#if defined(XEQP)
				VERBOSEFLX(stderr, filename, linenumber, "(%s LINE=%u) Locking %s (forced=%u), which was held in\n", __FILE__, __LINE__, name, (int) forced);
				xeqpoint.print(stderr);
				fprintf(stderr,"\n");
#else
				VERBOSEFLX(stderr, filename, linenumber, "(%s LINE=%u) Locking %s (forced=%u)\n", __FILE__, __LINE__, name, (int) forced);				
#endif
			}
			if (tid == pthread_self()) {
				if (xeq_locked_byme()) {
					lock_print(stderr);
					EXITFL(filename, linenumber, "(%s LINE=%u) lock %s (forced=%u) is already held by myself. Detail: tid=%u, pthread_self=%u", 
							__FILE__, __LINE__, name, (uint) forced, (int) tid, (int) pthread_self());
				} else {
					if (locked) {
						lock_print(stderr);
						EXITFL(filename, linenumber, "(%s LINE=%u) lock %s (forced=%u) is (or not) held by myself ? Detail: tid=%u, pthread_self=%u", 
								__FILE__, __LINE__, name, (uint) forced, (int) tid, (int) pthread_self());
					}
				}
			}	
		} else VERBOSEFLX(stderr, filename, linenumber, "Locking %s (forced=%u)\n", name, (int) forced);
		
		if (G_parms.check) xeq(filename, linenumber, false);
		if (!initiated) EXITFL(filename, linenumber, "lock %s is not initiated", name );
		pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
		
		if (pdata) {
			if (rank!=0 && pdata->holding_locks!=NULL && pdata->holding_locks->rank!=0 && rank >= pdata->holding_locks->rank) {
				EXITFL(filename, linenumber, "(%s LINE=%u) Invalid lock chain - lock %s (rank=%u) is chaining with lock %s (rank=%u)\n", 
						__FILE__, __LINE__, name, rank, pdata->holding_locks->name, pdata->holding_locks->rank);
			}		
			if (!forced) {
				if (pdata->holding_locks && !may_chain) {
					EXITFL(filename, linenumber, "(%s LINE=%u) Lock chain not permitted - lock %s (rank=%u) is chaining with lock %s (rank=%u)\n", 
							__FILE__, __LINE__, name, rank, pdata->holding_locks->name, pdata->holding_locks->rank);
				}		
				if (!pdata->may_lock) 			EXITFL(filename, linenumber, "this pthread may not lock" );
				if (pdata->waiting_lock != NULL)EXITFL(filename, linenumber, "lock %s - waiting_lock != NULL", name );
				pdata->waiting_lock = this;
			}
		}	
		
		PTXEQFL(pthread_mutex_lock(&mutex), XEQ_ERROR, filename, linenumber);
		VERBOSEFLX(stderr, filename, linenumber, "%s locked (forced=%u)\n", name, (uint) forced);
		locked = true;
		tid = pthread_self();
		
		if (pdata) {
			ASSERT(tid == pdata->tid);
			if (!forced) {
				ASSERTFL(pdata->waiting_lock == this, name, filename, linenumber);
				pdata->waiting_lock = NULL;
			}
			thread_data 		= pdata;
			next_chain			= pdata->holding_locks;
			pdata->holding_locks= this;
			if (pdata->waiting_lock != NULL)EXITFL(filename, linenumber, "lock %s - waiting_lock != NULL", name );
		}
		if (G_parms.check) xeq(filename, linenumber, true);
		locks++;
		if (washeld) lock_held++;
	} else {
		PTXEQFL(pthread_mutex_lock(&mutex), XEQ_ERROR, filename, linenumber);
	}
	return NULL;
}

void* lock_t::unlock(const char* filename, const uint linenumber) {
	if (!G_exiting) {
		if (next_chain!=NULL) {
			VERBOSEFLX(stderr, filename, linenumber, "Unlocking %s, next_chain=%s\n", name, next_chain->name);
		} else {
			VERBOSEFLX(stderr, filename, linenumber, "Unlocking %s\n", name);
		}
		if (G_parms.check) xeq(filename, linenumber, true);
		ASSERTFL(xeq_locked_byme(), name, filename, linenumber);
		ASSERTFL(initiated, name, filename, linenumber); 
		ASSERTFL(locked, name, filename, linenumber);
		ASSERTFL(tid == pthread_self(), name, filename, linenumber);
	}
	if (G_parms.check) xeqpoint.xeq(filename, linenumber);

	locked 	= false;
	tid 	= 0;
	
	pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
	
	if (pdata != NULL && thread_data != NULL) AssertFL((pdata == thread_data), XEQ_ERROR, filename, linenumber, "(pdata == thread_data) failed with lock=%s", name);
	
	if (pdata == NULL) pdata=thread_data;
	
	if (pdata != NULL) {
		if (!G_exiting) {
			AssertFL(pdata->waiting_lock == NULL, XEQ_ERROR, filename, linenumber, "Lock=%s is chaining with lock=%s", name, pdata->waiting_lock->name);
			ASSERTFL(thread_data == pdata, name, filename, linenumber);
			ASSERTFL(pdata->holding_locks == this, name, filename, linenumber);
		}
		pdata->holding_locks= next_chain;
		next_chain 			= NULL;
		if (next_chain!=NULL) ASSERTFL(next_chain->xeq_locked_byme(), next_chain->name, filename, linenumber);
	}
	thread_data = NULL;
	PTXEQFL(pthread_mutex_unlock(&mutex), XEQ_ERROR, filename, linenumber);
	return NULL;
}

void* lock_t::try_unlock(const char* filename, const uint linenumber) {
	VERBX(stderr, filename, linenumber, "Trying to unlock %s\n", name);
	xeqpoint.xeq(filename, linenumber);
	if (pthread_mutex_unlock(&mutex) == 0) {
		locked 	= false;
		tid 	= 0;
		pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
		if (pdata && pdata->holding_locks==this) pdata->holding_locks = next_chain;
		thread_data = 0;
	}
	return NULL;
}

void* print_lock_chain(FILE* std, const char* file, const uint line) {
	
	VERB(std, file, line, "Printing lock chain:\n");
	
	PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);					
	
	for (lock_t* lock=G_header_mutex.next_lock; lock!=NULL; lock=lock->next_lock) lock->lock_print(std); 
	
	pthread_mutex_unlock(&(G_header_mutex.mutex));
	fprintf(std, "\n");
	fflush(std);
	return NULL;
}

void* print_lock_stats(FILE* std, const char* file, const uint line) {
	
	VERB(std, file, line, "Printing lock stats:\n");
	
	PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);					
	
	for (lock_t* lock=G_header_mutex.next_lock; lock!=NULL; lock=lock->next_lock) { 
		VERB(std, file, line, "lock stats for %s: locks=%u, held=%u", lock->name, lock->locks, lock->lock_held);
		if (lock->locks != 0) fprintf(std, " (%3u%%)", 100*lock->lock_held/lock->locks); 
		fprintf(std,"\n");
	}
	
	pthread_mutex_unlock(&(G_header_mutex.mutex));
	fprintf(std, "\n");
	fflush(std);
	return NULL;
}

void*  lock_t::lock_print(FILE* std) { 
	ASSERTFL(initiated, name, __FILE__, __LINE__);
	fprintf(std, "     lock %s (rank=%u)", name, rank);

	if (next_chain)		fprintf(std, ", next chained=%s (rank=%u)", next_chain->name, next_chain->rank);
	
	if (thread_data) 	fprintf(std, ", holding thread=%s", thread_data->pthread_name);
	else 				fprintf(std, ", not held");
#if defined(XEQP) 
	if (xeqpoint.line) {
		fprintf(std, ", last xeq=");
		xeqpoint.print(std);
	}
#endif
	fprintf(std, "\n");
	return NULL;
}

void* lock_t::xeq(const char* filename, const uint linenumber, const bool locked) {
	if (!G_exiting) {
		ASSERTFL(initiated, name, filename, linenumber); 
		if (locked) {
			ASSERTFL(locked, name, filename, linenumber);
			ASSERTFL(tid == pthread_self(), name, filename, linenumber);
			ASSERTFL(xeq_locked_byme(), name, filename, linenumber);
		}
		pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
		if (pdata) {
			if (pdata->waiting_lock != NULL) VERB(stderr, filename, linenumber, "ERROR Cause: lock=%s, waiting_lock=%s\n", name, pdata->waiting_lock->name);
			ASSERTFL(pdata->waiting_lock == NULL, name, filename, linenumber);
			if (locked) {
				ASSERTFL(pdata->holding_locks == this, name, filename, linenumber);
				ASSERTFL(thread_data == pdata, name, filename, linenumber);
				ASSERTFL(pdata->holding_locks == this, name, filename, linenumber);
			}
		}
	}
	return NULL;
}

#else
void* lock_t::lock(const char* filename, const uint linenumber, bool forced) {
	PTXEQFL(pthread_mutex_lock(&mutex), XEQ_ERROR, filename, linenumber);
	return NULL;
}

void* lock_t::unlock(const char* filename, const uint linenumber) {
	PTXEQFL(pthread_mutex_unlock(&mutex), XEQ_ERROR, filename, linenumber);
	return NULL;
}

void* lock_t::try_unlock(const char* filename, const uint linenumber) {
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* lock_t::lock_print(FILE* std) { return NULL; }

void* lock_t::xeq(const char* filename, const uint linenumber, const bool locked) {	return NULL; }

void* print_lock_stats(FILE* std, const char* file, const uint line) { return NULL; }

void* print_lock_chain(FILE* std, const char* file, const uint line) { return NULL; }

#endif

void* lock_t::init_base(const char *mutex_name, const bool chain, const unsigned short irank) {

	xeqpoint.xeq(__FILE__, __LINE__);
	
	if (this != &G_header_mutex ) {
		
		ASSERT(!initiated);

		PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);
		
		next_lock				= G_header_mutex.next_lock;
		G_header_mutex.next_lock= this;
		
		PTXEQ(pthread_mutex_unlock(&(G_header_mutex.mutex)), XEQ_ERROR);
	} else {
		if (initiated) return NULL;
	}
	
	strncpy(name, mutex_name, sizeof(name)-1);
	tid 		= 0;
	thread_data	= NULL;
	next_chain	= NULL; 
	may_chain	= chain;
	locked 		= false;
	initiated 	= true;
	rank		= irank;

	PTXEQ(pthread_mutex_init(&mutex, &g_lock_attr), XEQ_ERROR); 
	//PTXEQ(pthread_mutex_init(&mutex, NULL), XEQ_ERROR); 
	
	return NULL;
}

void* lock_t::init(const char *mutex_name, const bool chain, const unsigned short irank) {
	
	PTXEQ(pthread_once(&g_lock_attr_once, g_lock_attr_init), XEQ_ERROR);
	
	Assert(!initiated, XEQ_ERROR);

	return init_base(mutex_name, chain, irank);
}

void* lock_t::destroy() {
	if (initiated) { 
		initiated = false;
		
		if (this==&G_header_mutex) {
			G_header_mutex.next_lock = NULL;
		} else {
			PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);
			
			lock_t* lock;
			for (lock=&G_header_mutex; lock!=NULL && lock->next_lock!=this; lock=lock->next_lock);
			if (lock!=NULL) lock->next_lock = this->next_lock;
			
			PTXEQ(pthread_mutex_unlock(&(G_header_mutex.mutex)), XEQ_ERROR);	
		}
	}
	return NULL;
}

void* xeq_all_locks(const char* file, const uint line) {
	
	PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);					
	
	for (lock_t* lock=G_header_mutex.next_lock; lock!=NULL; lock=lock->next_lock) lock->xeq(file, line, false); 
	
	pthread_mutex_unlock(&(G_header_mutex.mutex));
	return NULL;
}

void* unlock_try_all(const char* file, const uint line) {
	
	PTXEQ(pthread_mutex_lock(&(G_header_mutex.mutex)), XEQ_ERROR);					
	
	for (lock_t* lock=G_header_mutex.next_lock; lock!=NULL; lock=lock->next_lock) lock->try_unlock(file, line); 
	
	pthread_mutex_unlock(&(G_header_mutex.mutex));
	return NULL;
}

} // namespace d1ht
