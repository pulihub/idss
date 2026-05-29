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
 * @file memory.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht
{
pthread_t	G_PTHREAD 	= 0;
char 		*G_FILE;
int 		G_LINE 		= 0;
char 		*G_FILE_PREV;
int 		G_LINE_PREV	= 0;

#if defined(DEBUG)
FILE*		G_statm	= NULL;
uint		G_max_pages = 0;

void* print_memory_used(const char* header1, const char* header2, const char* filename, const uint linenumber) { 
	
	return NULL;
	
	if (!G_started || G_exiting || G_statm == NULL) return NULL;
	XEQ_EXEC(fseek(G_statm, 0, SEEK_SET), XEQ_ERROR);
	uint tsize, rss, shared, code, lib, stack, dirty;
	int nread = fscanf(G_statm, "%u %u %u %u %u %u %u", &tsize, &rss, &shared, &code, &lib, &stack, &dirty);
	if (nread != 7) {
		VERBO( stderr, "ATTENTION Cause: reading /proc/self/statm, return code=%d (should be 7), message=%s\n", nread, strerror(errno));
		fclose(G_statm);
		G_statm = fopen("/proc/self/statm", "r");
		return NULL;
	}
	if (tsize > (4 * G_max_pages / 5) ) { 
		VERB(stderr, filename, linenumber, "memory used %s %s: %u %u %u %u %u %u %u\n", header1, header2, tsize, rss, shared, code, lib, stack, dirty);
	}
	return NULL;
}
#else
void* print_memory_used(const char* header1, const char* header2, const char* filename, const uint linenumber) { return NULL; };
#endif

#if defined(MCHECK)
void print_mem_status(char* filename, const uint linenumber, enum mcheck_status status, char* var, void* pointer) {
  	char	msg[512];

  	switch (status) {
   	
	    case MCHECK_DISABLED: {     	
	    	sprintf( msg, "WARNING Cause: malloced memory: var=%s, pointer=%x: mcheck disabled - you should link with -lmcheck", var, (unsigned int) pointer );
	    	VERB(stderr, filename, linenumber, "%s\n", msg);
	    	break;
	    }
	  	
	    case MCHECK_OK: {  // everything is ok
//	    	sprintf( msg, "malloced memory OK: var=%s, pointer=%x", var, pointer );
//	    	VERB(stderr, filename, linenumber, "%s\n", msg);  	
	      	break;
	    }
	   	
	    case MCHECK_FREE: { 
	    	sprintf( msg, "ERROR Cause: Problem in malloced memory: var=%s, pointer=%x: block already freed", var, (unsigned int) pointer );
	    	VERB(stderr, filename, linenumber, "%s\n", msg);
    		break;
	      	EXITFL( filename, linenumber, msg);
	      	break;
	    }
	    
	    case MCHECK_HEAD: { 
	    	sprintf( msg, "ERROR Cause: Problem in malloced memory: var=%s, pointer=%x: The data immediately before the block was modified. For recently allocated memory it can be due to a memory shortage. Otherwise, this commonly happens when an array index or pointer is decremented too far." , var, (unsigned int) pointer);
	    	VERB(stderr, filename, linenumber, "%s\n", msg);
    		break;
	      	EXITFL( filename, linenumber, msg);
	    	break;
	    }
	    
	    case MCHECK_TAIL: { 
	    	sprintf( msg, "ERROR Cause: Problem in malloced memory: var=%s, pointer=%x: The data immediately after the block was modified. For recently allocated memory it can be due to a memory shortage. Otherwise, this commonly happens when an array index or pointer is incremented too far." , var, (unsigned int) pointer);
	    	VERB(stderr, filename, linenumber, "%s\n", msg);
    		break;
	      	EXITFL( filename, linenumber, msg);
	    	break;
	    }
	    
		default: {
	      	sprintf( msg, "ERROR Cause: Problem in malloced memory: var=%s, pointer=%x, unknow return code=%d", var, (unsigned int) pointer, status);
	      	VERB(stderr, filename, linenumber, "%s\n", msg);
	      	break;
	      	EXITFL( filename, linenumber, msg);
	      	break;
		}
	}
}

bool g_bugged = false;

void abortfn(enum mcheck_status status) { 
	if (status != MCHECK_OK) {
		if (G_exiting || g_bugged) Exit(-2); 
		VERBO(stderr, "ERROR Cause: Problem in alloced memory: mcheck_status=%d\n", status);
		print_mem_status( __FILE__, __LINE__, status, "unknown", NULL);
#if defined(DEBUG_MALLOC)
//		VERBO(stderr, "Checking all malloced memory\n");
//		dmalloc_verify(NULL);
#endif
//		EXIT("Problem with malloced memory");
//		unlock_try_all(__FILE__,__LINE__); 
		g_bugged = true;
//		pthread_exit(NULL);
	} 
}

void* probefn(char* filename, const uint linenumber, char* var, void* pointer) { 
	enum mcheck_status status = mprobe(pointer);
	if (status != MCHECK_OK) {
		print_mem_status( filename, linenumber, status, var, pointer); 
		EXIT("Problem with malloced memory");
	}
	return NULL;
}
#endif

} // namespace d1ht
