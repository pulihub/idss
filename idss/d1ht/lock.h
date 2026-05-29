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
 * @file lock.h
 * @brief 
 */

#ifndef LOCK_H_			
#define LOCK_H_

extern void* xeq_all_locks(const char* file, const uint line);
extern void* unlock_try_all(const char* file, const uint line);
extern void* print_lock_chain(FILE* std, const char* file, const uint line);
extern void* print_lock_stats(FILE* std, const char* file, const uint line);

class lock_t {
	pthread_t 	 	tid;
	pthread_data_t	*thread_data;
	
	bool			initiated;
	bool			locked;
	bool			may_chain;
	unsigned short	rank; 
	
public:	
	pthread_mutex_t	mutex;
	xeqpoint_t		xeqpoint;
	lock_t			*next_chain;
	lock_t			*next_lock;
	NUM_32			locks;
	NUM_32			lock_held;
	NUM_32			queuesize;
	char			name[20];

private:
	int try_lock() {
		int retcode = pthread_mutex_trylock(&mutex); 
		if (retcode == 0) { 
			PTXEQ(pthread_mutex_unlock(&mutex), XEQ_ERROR);
		}  else { 
			retcode = pthread_mutex_lock(&mutex);
			if (retcode == 0) pthread_mutex_unlock(&mutex);
		} 
		return retcode;
	}

public:		
	void *init_base(const char *mutex_name, const bool chain, const unsigned short irank);
	void *init(const char *mutex_name, const bool chain, const unsigned short irank);
	void *init(const char *mutex_name, const bool chain) { return init(mutex_name, chain, 0); }
	void* destroy();
	
	lock_t() { 
		tid 		= 0;
		thread_data	= NULL;
		next_chain	= NULL;
		next_lock	= NULL;
		may_chain	= false;
		locked 		= false;
		initiated 	= false;
		rank		= 0;
		locks		= 0;
		lock_held	= 0;
	}

	~lock_t() { if (initiated) destroy(); }
	
	void *lock(const char* filename, const uint linenumber, bool forced);
	
	void *unlock(const char* filename, const uint linenumber);
	
	void *try_unlock(const char* filename, const uint linenumber);
	
	void* xeq(const char* filename, const uint linenumber, const bool locked);
	
	void* lock_print(FILE* std);
	
	bool xeq_locked() 		{ return (try_lock() != 0); }
	
	bool xeq_unlocked() 	{ return (try_lock() == 0); }
	
	bool xeq_locked_byme() 	{ 
		if (!G_exiting && tid != pthread_self()) return false;
		return (try_lock() == EDEADLK); 
	}
	
	bool assert_locked_byme(const char* filename, const uint linenumber) { 

		if (!xeq_locked_byme()) EXITFL(filename, linenumber, "assert_locked_byme failed, lock=%s", name); 
	
		return true;
	}
};

#define	LOCK_INIT(LOCKNAME, MAYCHAIN, RANK) { bool MAYCHAIN; LOCKNAME.init(#LOCKNAME, maychain, RANK); }

#define LOCK(mutex) 		mutex.lock( __FILE__, __LINE__, false )	

#define LOCK_FORCED(mutex) 	mutex.lock( __FILE__, __LINE__, true )	
	
#define UNLOCK(mutex) 		mutex.unlock( __FILE__, __LINE__ )

#define UNLOCK_TRY(mutex) 	mutex.try_unlock( __FILE__, __LINE__ )

extern lock_t 			G_interval_mutex, G_thread_data_mutex, G_header_mutex, G_malloc_mutex;

#if defined(DEBUG)
extern lock_t				G_selfmsgs_mutex;
#endif

#endif /*LOCK_H_*/
