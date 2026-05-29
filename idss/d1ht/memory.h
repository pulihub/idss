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
 * @file memory.h
 * @brief 
 */

#ifndef MEM_H_
#define MEM_H_

#if defined(DEBUG)
extern FILE* G_statm;
extern uint	 G_max_pages;
#endif

extern pthread_t	G_PTHREAD;
extern char 		*G_FILE;
extern int 			G_LINE;
extern char 		*G_FILE_PREV;
extern int 			G_LINE_PREV;


extern void* print_memory_used(const char* header1, const char* header2, const char* filename, const uint linenumber);

#define BZERO(arg) { 																			\
	if (memset( (void *) &arg, 0, sizeof(arg)) == NULL ) { 										\
		D1HT_ERROR_function("memset failed", "retcode==NULL", XEQ_ERROR, __FILE__, __LINE__);	\
  		EXIT("memset failed");																	\
	}																							\
}

#define BVZERO(arg, sizeofarg) { 																\
	if (memset( arg, 0, sizeofarg) == NULL ) { 													\
		D1HT_ERROR_function("memset failed", "retcode==NULL", XEQ_ERROR, __FILE__, __LINE__);	\
  		EXIT("memset failed");																	\
	}																							\
}

#if defined(DEBUG_MALLOC)
#include <dmalloc.h>

extern void* 			probefn(char* filename, const uint linenumber, char* var, void* pointer); 
extern void 			abortfn(enum mcheck_status status);

#define M_CHECKFL(_file, _line, VAR) { 															\
	if (VAR!=NULL && !G_exiting) { 																\
		ASSERTFL((dmalloc_verify(VAR)==DMALLOC_VERIFY_NOERROR), #VAR, _file, _line ) ;			\
	}																							\
}

#else 
#if defined(MCHECK)
#include <mcheck.h>
#define M_CHECKFL(_file, _line, VAR) { 															\
	if (VAR!=NULL && !G_exiting && G_UDP_thread_id !=0) { 										\
		probefn( _file, _line, #VAR, (void*) VAR); 												\
	}																							\
}

#else
#define M_CHECKFL(_file, _line, VAR) 
#endif
#endif

#define M_CHECK(VAR) M_CHECKFL(__FILE__, __LINE__, VAR)

#if defined(MEMLOCK)
// the CALLOC macros malloc and zero a region of RAM
#define	CALLOC(N, TYPE)  	({																	\
	G_FILE 		= __FILE__;																		\
	G_LINE 		= __LINE__;																		\
	size_t _N = (size_t) N;																		\
	size_t _csize = _N * sizeof(TYPE);															\
	TYPE	*_region = NULL;																	\
	if (!G_exiting) LOCK_FORCED(G_malloc_mutex);												\
	XEQ_EXEC_NULL(_region = (TYPE *) malloc(_csize), XEQ_ERROR );								\
	G_FILE_PREV = G_FILE;																		\
	G_LINE_PREV = G_LINE;																		\
	G_LINE 		= 0;																			\
	XEQ_EXEC_NULL(memset((void *) _region, 0, _csize), XEQ_ERROR );								\
	if (!G_exiting) {																			\
		VERBOSE(stderr,"CALLOC FILE=%s, LINE=%u, type=%s, N=%u, sizeof(TYPE)=%u, size=%u, ALLOCED=%x\n", __FILE__, __LINE__, #TYPE, _N, sizeof(TYPE), _csize, (uint) _region);	\
		M_CHECK(_region);																		\
		UNLOCK(G_malloc_mutex); 																\
	} else { UNLOCK_TRY(G_malloc_mutex); }														\
	/* size_t _next = _csize + (size_t) _region; */ \
	/* fprintf(stderr,"- CALLOC FILE=%s, LINE=%u, type=%s, N=%u, sizeof(TYPE)=%u, size=%u, ALLOCED=%x, next=%x -", __FILE__, __LINE__, #TYPE, _N, sizeof(TYPE), _csize, _region, _next); */		\
	(TYPE *) _region; 		})
	
#define FREE(VAR) if (!G_exiting) {																\
	G_FILE 		= __FILE__;																		\
	G_LINE 		= __LINE__;																		\
	VERBOSE(stderr,"FREE %s=%x\n", #VAR, (uint) VAR); 											\
	LOCK_FORCED(G_malloc_mutex); 																\
	if (VAR!=NULL && !G_exiting) { 																\
		M_CHECK(VAR); 																			\
		free((void *) VAR); 																	\
		UNLOCK(G_malloc_mutex);																	\
		VERBOSE(stderr,"FRED %s=%x\n", #VAR, (uint) VAR);										\
		VAR = NULL;  																			\
	} else if (VAR==NULL && !G_exiting) {														\
		UNLOCK(G_malloc_mutex);																	\
		EXIT("FREE NULL pointer %s=%x", #VAR, (uint) VAR););									\
	} else { UNLOCK(G_malloc_mutex);  }															\
	G_FILE_PREV = G_FILE;																		\
	G_LINE_PREV = G_LINE;																		\
	G_LINE 		= 0;																			\
}

#else

#define	MALLOC(N, TYPE)  	({																	\
	G_FILE = __FILE__;																			\
	G_LINE = __LINE__;																			\
	if (G_parms.verbose && !G_exiting) G_PTHREAD = pthread_self();								\
	size_t _N = (size_t) N;																		\
	size_t _csize = _N * sizeof(TYPE);															\
	TYPE *_region = (TYPE *) malloc(_csize);													\
	if (_region == NULL) EXIT("mallock failed"); 												\
	G_FILE_PREV = G_FILE;																		\
	G_LINE_PREV = G_LINE;																		\
	G_LINE 		= 0;																			\
	(TYPE *) _region; 		})

#define	CALLOC(N, TYPE)  	({																	\
	size_t 	_N 		= (size_t) N;																\
	size_t 	_csize 	= _N * sizeof(TYPE);														\
	TYPE	*_region= MALLOC(N, TYPE);															\
	XEQ_EXEC_NULL(memset((void *) _region, 0, _csize), XEQ_ERROR );								\
	(TYPE *) _region; 		})
	
#define FREE(VAR) 	({																			\
	G_FILE = __FILE__;																			\
	G_LINE = __LINE__;																			\
	if (G_parms.verbose && !G_exiting) G_PTHREAD = pthread_self();								\
	if (VAR!=NULL) { 																			\
		free((void *) VAR); 																	\
		VAR = NULL;  																			\
	} else if (!G_exiting) {																	\
		EXIT("FREE NULL pointer %s\n", #VAR);													\
	} 																							\
	G_FILE_PREV = G_FILE;																		\
	G_LINE_PREV = G_LINE;																		\
	G_LINE 		= 0;																			\
   })
#endif

#define ALLOC(N, VAR) 		VAR = CALLOC(N, typeof(*VAR))

#define	ALLOC_INIT(TYPE)  	({																	\
	TYPE	*region = CALLOC(1, TYPE);															\
	if( region != NULL ) region->init();														\
	region; 				})

#define FREE_DESTROY(VAR) { ASSERT(VAR!=NULL); if (VAR!=NULL && !G_exiting) { VAR->destroy(); FREE(VAR); } }

#endif /*MEM_H_*/

