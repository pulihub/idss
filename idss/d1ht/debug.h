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
 *  No title to or ownership of D1HT is hereby transferred.	             	 *
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
 * @file debug.h
 * @brief 
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef DEBUG
#define NODEBUG
#else
#undef  NODEBUG
#endif

#ifdef  NODEBUG
#undef  DEBUG
#endif

using namespace std;

extern void *dump(FILE *std, const char* file, const uint line, const char* cause);

#define VERBX(std, FILE, LINE, ...) 

#define VERB(std, FILE, LINE, ...) { 																\
	struct 	timeval tv_time;																		\
  	struct 	tm* 	ptm;																			\
  	char 	time_string[40];																		\
  	gettimeofday(&tv_time, NULL);																	\
  	ptm = localtime (&tv_time.tv_sec);																\
  	strftime(time_string, sizeof(time_string), D1HT_TIME_FORMAT, ptm);								\
	fprintf(std, "%s - %s tid=%u ", time_string, G_parms.program_name, (uint) pthread_self());		\
	print_pthread_name(std); 																		\
	fprintf(std, "FILE=%s LINE=%u ", FILE, (uint) LINE);											\
	fprintf(std, __VA_ARGS__ ) ; 																	\
	fflush(std); 																					\
  }

#define VERBO(std, ...) VERB(std, __FILE__, __LINE__, __VA_ARGS__ ) 

#define EXITFL( file, line, ... ) { 														\
	if (G_exited) {																			\
		mysleep(10);																		\
		Exit(EXIT_FAILURE); 																\
	}																						\
	fprintf(stderr,"\n"); 																	\
	char _cause[256];																		\
	sprintf(_cause, __VA_ARGS__ );															\
	if (G_exiting) {																		\
		G_exited = true;																	\
		VERB(stderr, file, line, "Secondary ERROR while Exiting Cause: %s\n", _cause);		\
		mysleep(10);																		\
		Exit(EXIT_FAILURE); 																\
	}																						\
	if(strcmp(G_parms.program_name,"D1HT")!=0 || strcmp(G_parms.program_name,"D1HFileS")!=0)\
	{ 																						\
		VERB(stderr,file,line,"Program %s ERROR Cause: %s\n",G_parms.program_name,_cause);	\
		Exit(EXIT_FAILURE); 																\
	}																						\
	VERB(stderr, file, line, "ERROR Cause: %s\n", _cause); 									\
	if (!G_started) {																		\
		G_exited  = true;																	\
		G_exiting = true;																	\
		if (G_parms.VERBOSE) dump(stderr, file, line, _cause); 								\
		Exit(EXIT_FAILURE); 																\
	}																						\
	D1HT_exit_dump( file, line, _cause); 													\
}

#define EXIT( ... ) EXITFL( __FILE__, __LINE__, __VA_ARGS__ )

#define EXITOK { 																			\
	if (!G_started) Exit(EXIT_SUCCESS); 													\
	if (G_exited || G_exiting) {															\
		G_exited  = true;																	\
		mysleep(10);																		\
		exit(0); 																			\
	}																						\
	fprintf(stderr,"\n"); 																	\
	if(strcmp(G_parms.program_name,"D1HT")!=0 || strcmp(G_parms.program_name,"D1HFileS")!=0)\
	{ 																						\
		VERB(stderr, __FILE__, __LINE__, "Program %s Ending OK.\n", G_parms.program_name);	\
		Exit(EXIT_SUCCESS); 																\
	}																						\
	D1HT_exit_nodump( __FILE__, __LINE__, "Ending OK.\n"); 									\
}

#define VERBOSEX(std, ...) 		// VERBOSE(std, __VA_ARGS__)
#define VERBOSEFLX(std, ...) 	// VERBOSEFL(std, __VA_ARGS__)

#if defined(DEBUG)
#define VERBOX(std, ...) VERBO(std, __VA_ARGS__ ) 
#define VERBOSEFL(std, FILE, LINE, ...) 													\
	if (G_parms.VERBOSE) { 																	\
		VERB( std, FILE, LINE, __VA_ARGS__ ); 												\
		if (std == stdout) VERB( stderr, FILE, LINE, __VA_ARGS__ ); 						\
	}
#define VERBOSE(std, ...) VERBOSEFL(std, __FILE__, __LINE__, __VA_ARGS__)
#else
#define VERBOX(std, ...)
#define VERBOSE(std, ...)
#define VERBOSEFL(std, ...)
#define VERBOSEX(std, ...)
#define VERBOSEFLX(std, ...)
#endif

#define verbosefl(std, FILE, LINE, ...) 													\
	if (G_parms.verbose) { 																	\
		VERB( std, FILE, LINE, __VA_ARGS__ ); 												\
		if (std == stdout) VERB( stderr, FILE, LINE, __VA_ARGS__ ); 						\
	}

#define verbose(std, ...) verbosefl(std, __FILE__, __LINE__, __VA_ARGS__)

#define XCALL(functioncall) { 																		\
	VERBOSEX(stderr, "Call routine=%s FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__);		\
	functioncall; 																					\
	VERBOSEX(stderr, "Return routine=%s FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__);		\
	} 

/*** ERROR HANDLING MACROS ************************************/

extern void* D1HT_ERROR_function(const char* cause, const char* message, const int XEQ_FLAG, const char *file, const uint line);

// the XEQ_XX macros should be in sync with the err_type and err_priority arrays defined in D1HT_ERROR_function
#define XEQ_SILENT 		0
#define XEQ_WARNING 	1+XEQ_SILENT
#define XEQ_ERROR 		1+XEQ_WARNING

#ifdef DEBUG
#define XXEQ	{ int __errsave__ = errno; XEQPOINT; errno = __errsave__; }
#define PXEQ 	{ int __errsave__ = errno; VERBOSE(stderr, "FUNCTION=%s \n", __FUNCTION__); fflush(stderr); errno = __errsave__; }
#define PXXEQ   { XXEQ; PXEQ; }
#define vXEQ 	{ int __errsave__ = errno; verbose(stderr, "YXEQ\n"); errno = __errsave__; }
#define PEE(X) 	{ int __errsave__ = errno; VERBO(stderr, "FUNCTION=%s PE ", __FUNCTION__); cerr << #X << "=" << X << endl; fflush(stderr); errno = __errsave__;}
#define PE(X) 	{ if (G_parms.verbose) {  int __errsave__ = errno; PEE(X); errno = __errsave__;} }
#else
#define XXEQ
#define PXEQ
#define PXXEQ
#define vXEQ
#define PEE(_XX) { (void) _XX; }
#define PE(_XX)	 { (void) _XX; }
#endif

#define XSTR(S) STR(S)
#define STR(S) 	#S

#define PTXEQFL(functioncall, XEQ_FLAG, FILE, LINE) ({																					\
	XEQPOINT_LEVEL2;																													\
	int errnumber = 0; 																													\
	VERBOSEX(stderr, "Call routine=%s FILE=%s LINE=%d\n", #functioncall, FILE, LINE);													\
	do {																																\
		if ((errnumber = functioncall) && (errnumber != EINTR)) {																		\
  			if (XEQ_FLAG == XEQ_SILENT) continue; 																						\
  			if (strcmp(#functioncall,XSTR(functioncall))) {																				\
				D1HT_ERROR_function(strerror(errnumber), #functioncall " - EXPANDED=" XSTR(functioncall), XEQ_FLAG, FILE, LINE);		\
			} else {																													\
				D1HT_ERROR_function(strerror(errnumber), #functioncall, XEQ_FLAG, FILE, LINE);											\
			}																															\
	   		if (XEQ_FLAG == XEQ_ERROR) EXIT("%s - %s", #functioncall, strerror(errnumber));												\
		}																																\
	} while (errnumber == EINTR);																										\
	errnumber;																															\
})

#define PTXEQ(functioncall, XEQ_FLAG) 	PTXEQFL(functioncall, XEQ_FLAG, __FILE__, __LINE__)	

#define D1HT_ERROR(cause, message, XEQ_FLAG)														\
{ 																									\
	XEQPOINT_LEVEL2;																				\
	if (XEQ_FLAG != XEQ_SILENT) { 																	\
   		if (XEQ_FLAG == XEQ_ERROR) EXIT("%s - %s", cause, message);									\
		D1HT_ERROR_function(cause, message, XEQ_FLAG, __FILE__, __LINE__);							\
	}																								\
}

#define XEQ_EXEC(functioncall, XEQ_FLAG) 		XEQ_EXECT(functioncall, XEQ_FLAG, -1)
#define XEQ_EXEC_NULL(functioncall, XEQ_FLAG) 	XEQ_EXECT(functioncall, XEQ_FLAG, NULL)

#define XEQ_EXECT(functioncall, XEQ_FLAG, RETCODE) 																					\
({ 																																	\
  XEQPOINT_LEVEL2;																													\
  int errnumber = 0; 																												\
  VERBOSEX(stderr, "Call routine=%s, FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__);										\
  do {																																\
  		if ((functioncall) == RETCODE && ((errnumber = errno) != EINTR)) {															\
  			ASSERT(errnumber != 0);																									\
  			if (XEQ_FLAG == XEQ_SILENT) break; 																						\
  			if (strcmp(#functioncall,XSTR(functioncall))) {																			\
  				D1HT_ERROR(strerror(errnumber), #functioncall " - EXPANDED=" XSTR(functioncall), XEQ_FLAG ); 						\
  			} else {																												\
				D1HT_ERROR(strerror(errnumber), #functioncall, XEQ_FLAG ); 															\
  			}																														\
  		}																															\
  } while (errnumber == EINTR);																										\
  VERBOSEX(stderr, "Return routine=%s FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__); 										\
  errno = errnumber;																												\
}) 

#define XEQ_EXEC_NZERO(functioncall, XEQ_FLAG) 																						\
({ 																																	\
  XEQPOINT_LEVEL2;																													\
  int errnumber = 0; 																												\
  VERBOSEX(stderr, "Call routine=%s FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__);											\
  do {																																\
  		if (functioncall && ((errnumber = errno) != EINTR)) {																		\
  			if (XEQ_FLAG == XEQ_SILENT) continue; 																					\
  			if (strcmp(#functioncall,XSTR(functioncall))) {																			\
  				D1HT_ERROR(strerror(errnumber), #functioncall " - EXPANDED=" XSTR(functioncall), XEQ_FLAG ); 						\
  			} else {																												\
				D1HT_ERROR(strerror(errnumber), #functioncall, XEQ_FLAG ); 															\
  			}																														\
  		}																															\
  } while (errnumber == EINTR);																										\
  VERBOSEX(stderr, "Return routine=%s FILE=%s LINE=%d\n", #functioncall, __FILE__, __LINE__); 										\
  errno = errnumber;																												\
}) 

#define XEQ_TRUE(condition, msg, XEQ_FLAG) { XEQPOINT_LEVEL2; if (condition && XEQ_FLAG != XEQ_SILENT) D1HT_ERROR(#condition, msg, XEQ_FLAG ) }

#define AssertFL(condition, XEQ_FLAG, FILE, LINE, ... ) ({																				\
	bool _retcode = true;																												\
	if (!G_exited && !(condition)) {																									\
		_retcode = false;																												\
		if (G_exiting || XEQ_FLAG == XEQ_WARNING) {																						\
			D1HT_ERROR_function("Assert( " XSTR(condition) " ) failed", #condition " failed - " #__VA_ARGS__ , XEQ_WARNING, FILE, LINE);\
		} else if (XEQ_FLAG == XEQ_ERROR) {																								\
			EXITFL(FILE, LINE, "Assert failed - " __VA_ARGS__);																			\
		}																																\
	} 																																	\
	_retcode;																															\
})

#define Assert(condition, XEQ_FLAG, ...) AssertFL(condition, XEQ_FLAG, __FILE__, __LINE__, __VA_ARGS__ )

#ifdef DEBUG

#define ASSERTFL(condition, msg, FILE, LINE) { 																				\
	if (!G_exited && !(condition)) {																						\
		if (G_exiting) {																									\
			D1HT_ERROR_function("ASSERT( "XSTR(condition)" ) failed - " #condition " failed", msg, XEQ_WARNING, FILE, LINE);\
		} else{ 																											\
			EXITFL(FILE, LINE, msg);																						\
		}																													\
	} }

#define ASSERT(condition, ...) ASSERTFL(condition, XSTR(condition) " failed " #__VA_ARGS__, __FILE__, __LINE__) 

#else

#define ASSERT(condition, ...)
#define ASSERTFL(condition, msg, FILE, LINE)

#endif

#endif /*DEBUG_H_*/
