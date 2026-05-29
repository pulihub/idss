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
 * @file threads_debug.h
 * @brief 
 */

#ifndef THREADS_DEBUG_H_
#define THREADS_DEBUG_H_

class pthread_data_t { 
	public:	
#if defined(XEQP) 
	xeqpoint_t		xeq_last;
	xeqpoint_t		xeq_caller;
#endif
	lock_t			*waiting_lock;
	lock_t			*holding_locks;
	pthread_t 	 	tid;
	pthread_data_t	*caller;
	pthread_data_t	*next;
	pthread_data_t	*prev;
	void			*thread_parms;
	bool			may_lock;
	bool			detached;
	bool			started;
	bool			locked;
#if defined(D1HT_PTHREAD_STATS)
	int				stat_started;
	int				stat_ended;
#endif
#if defined(DEBUG)
#define D1HT_PTHREAD_NAME_SIZE 16
#else
#define D1HT_PTHREAD_NAME_SIZE 8
#endif
	char			pthread_name[D1HT_PTHREAD_NAME_SIZE];
	void pthread_init() { 
		BVZERO(pthread_name, D1HT_PTHREAD_NAME_SIZE);
		holding_locks	= NULL;
		waiting_lock	= NULL;
		caller			= NULL;
		may_lock		= false;	
		started			= false;
		detached		= false;
		locked			= false;
		next			= NULL;
		prev 			= NULL;
#if defined(D1HT_PTHREAD_STATS)
		stat_started	=-1;
		stat_ended		=-1;
#endif
	}

	void pthread_init(pthread_data_t *G_pthread_data_header);

	void *pthread_destroy(pthread_data_t *G_pthread_data_header);
	
	void pthread_print(FILE* std);
	
	pthread_data_t() 	{ pthread_init(); }
};

#if defined(XEQP)						
#define PTHREAD_CREATE_XEQP																			\
		XEQ(caller_data->xeq_last);																	\
		called_data->xeq_caller		= caller_data->xeq_last;										\
		called_data->xeq_last		= caller_data->xeq_last;										
#else
#define PTHREAD_CREATE_XEQP
#endif

#if defined(D1HT_PTHREAD_STATS)
#define	D1HT_PTHREAD_STATS_CREATE(THREAD_ROUTINE) {													\
	called_data->stat_started 	=  stat_started_ ## THREAD_ROUTINE;									\
	called_data->stat_ended 	=  stat_ended_   ## THREAD_ROUTINE;									\
	G_stats.total.stats[stat_created_ ## THREAD_ROUTINE].count++;									\
}
#else
#define	D1HT_PTHREAD_STATS_CREATE(THREAD_ROUTINE)
#endif

#define PTHREAD_TEST_CREATE(THREAD_ROUTINE, PARM, STATE, XEQ_FLAG) ({								\
	print_memory_used("PTHREAD_CREATE", "before", __FILE__, __LINE__);								\
	pthread_t		thread_id;																		\
	pthread_data_t	*called_data = CALLOC( 1, pthread_data_t);										\
	void			*_PARM = PARM;																	\
	if (G_exiting) {																				\
		called_data->pthread_init();																\
	} else {																						\
		called_data->pthread_init(G_pthread_data_header);											\
		pthread_data_t	*caller_data = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);	\
		ASSERT(caller_data!=NULL, "Caller pthread should have been created with PTHREAD_CREATE\n"); \
		called_data->caller = caller_data;															\
		PTHREAD_CREATE_XEQP																			\
	}																								\
	strncpy(called_data->pthread_name, #THREAD_ROUTINE, D1HT_PTHREAD_NAME_SIZE-1);					\
	called_data->thread_parms	= (void *) _PARM;													\
	int errsave = PTXEQ(pthread_create( &thread_id, STATE, &THREAD_ROUTINE, called_data), XEQ_FLAG);\
	if (errsave != 0) {																				\
		thread_id = 0;																				\
	} else {																						\
		VERBOSE(stderr, "Thread %s created (tid=%u)\n", #THREAD_ROUTINE, (uint) thread_id);			\
		D1HT_PTHREAD_STATS_CREATE(THREAD_ROUTINE)													\
	}																								\
	print_memory_used("PTHREAD_CREATE", "after", __FILE__, __LINE__);								\
	errno = errsave;																				\
	thread_id;																						\
})

#if defined(D1HT_PTHREAD_STATS)
#define PTHREAD_INIT							 																\
	pthread_data_t	*pthread_data_mine 	= (pthread_data_t *) _parm_temp;										\
	pthread_data_mine->tid 				= pthread_self();														\
	pthread_setspecific(G_pthread_data_key, pthread_data_mine);													\
	pthread_data_mine->started 			= true;																	\
	XEQ(pthread_data_mine->xeq_last);																			\
	VERBOSE(stderr, "Thread %s started (tid=%u)\n", pthread_data_mine->pthread_name, (uint) pthread_data_mine->tid);	\
	ASSERT(pthread_data_mine->waiting_lock  == NULL);															\
	ASSERT(pthread_data_mine->holding_locks == NULL);															\
	LOCK_FORCED(G_thread_data_mutex);																			\
	G_stats.total.stats[stat_pthread_started].count++;															\
	if (pthread_data_mine->stat_started >= 0) G_stats.total.stats[pthread_data_mine->stat_started].count++;		\
	G_stats.total.stats[stat_pthread_conc].count = 1;															\
	G_stats.total.stats[stat_pthread_conc].value = MAX((double) (G_stats.total.stats[stat_pthread_started].count-G_stats.total.stats[stat_pthread_ended].count), G_stats.total.stats[stat_pthread_conc].value); \
	UNLOCK(G_thread_data_mutex);																				\
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
#else
#define PTHREAD_INIT							 																\
	pthread_data_t	*pthread_data_mine 	= (pthread_data_t *) _parm_temp;										\
	pthread_data_mine->tid 				= pthread_self();														\
	pthread_setspecific(G_pthread_data_key, pthread_data_mine);													\
	pthread_data_mine->started 			= true;																	\
	XEQ(pthread_data_mine->xeq_last);																			\
	VERBOSE(stderr, "Thread %s started (tid=%u)\n", pthread_data_mine->pthread_name, (uint) pthread_data_mine->tid);	\
	ASSERT(pthread_data_mine->waiting_lock == NULL);															\
	ASSERT(pthread_data_mine->holding_locks == NULL);															\
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
#endif

#define PTHREAD_MAYLOCK {						 														\
	pthread_data_t	*pthread_data_mine = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);	\
	if (pthread_data_mine!=NULL) pthread_data_mine->may_lock = true;						 			\
}

#define PTHREAD_DETACH  			pthread_detach(pthread_self());
#define PTHREAD(THREAD_ROUTINE) 	void* THREAD_ROUTINE(void* _parm_temp) 
#define PTHREAD_PARM_ASSIGN(PARM) 	PARM = (typeof(PARM)) pthread_data_mine->thread_parms
#define PTHREAD_PARM_FREE 			{ FREE(pthread_data_mine->thread_parms); }

#define PTHREAD_PARM_COPY(PARM) {																		\
	if (pthread_data_mine->thread_parms == NULL) EXIT("Invalid thread parm");							\
	PARM = *((typeof(PARM) *) pthread_data_mine->thread_parms);											\
}

#endif /*THREADS_DEBUG_H_*/
