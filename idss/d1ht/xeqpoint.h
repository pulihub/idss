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
 * WITHOUT ANY WARRANTY; withoutpthread_init even the implied warranty of                *
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
 * @file xeqpoint.h
 * @brief 
 */

#ifndef XEQPOINT_H_
#define XEQPOINT_H_

#define XEQ_NAME_SIZE 15

#if defined(XEQP) || defined(XEQP_THREAD) || defined(XEQP_LEVEL0) || defined(XEQP_LEVEL1) || defined(XEQP_LEVEL2) || defined(XEQP_LEVEL3)
#undef 	XEQP
#define XEQP
#undef 	XEQP_LEVEL0
#define XEQP_LEVEL0
#endif

#if defined(XEQP) 
class xeqpoint_t {
	timeval tv_xeq;
	char	file[XEQ_NAME_SIZE+1];
public:
	int		line;

	void xeq(const char* filename, const uint linenumber);
	void xeq_notime(const char* filename, const uint linenumber);
	
	void xeqpoint_init() {
		line 			= 0;
		tv_xeq.tv_sec	= 0;
	}
	
	xeqpoint_t() { xeqpoint_init(); }

	xeqpoint_t operator=(xeqpoint_t pxeq) {
		line 	= pxeq.line;
		tv_xeq	= pxeq.tv_xeq;
		strncpy(file, pxeq.file, XEQ_NAME_SIZE-1);
		return *this;
	} 
	
	void print(FILE* std) {
		if (line == 0) return;
		if (tv_xeq.tv_sec != 0) {
			struct tm ptm;
			localtime_r(&tv_xeq.tv_sec, &ptm);

			char 	time_string[21];
			strftime (time_string, sizeof(time_string)-1, D1HT_TIME_FORMAT, &ptm);

			fprintf(std,"FILE=%s LINE=%u time=%s", file, line, time_string);
		} else {
			fprintf(std,"FILE=%s LINE=%u", file, line);
		}
	}
};

#define XEQ(xeqpoint) xeqpoint.xeq( __FILE__, __LINE__)

#if defined(XEQP_THREAD)
#define XEQPOINT_FL( FILE, LINE ) {																\
		int _errsave = errno;																	\
		pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);	\
		if (pdata) {																			\
			pdata->xeq_last.xeq( FILE, LINE );													\
		} else {																				\
			G_xeqpoint.xeq( FILE, LINE );														\
		}																						\
		errno = _errsave;																		\
	}
#else
#define XEQPOINT_FL( FILE, LINE ) {																\
		int _errsave = errno;																	\
		G_xeqpoint.xeq( FILE, LINE );															\
		errno = _errsave;																		\
	}
#endif

#define XEQPOINT XEQPOINT_FL( __FILE__, __LINE__ )

#else // defined(XEQP)

class xeqpoint_t {
public:

	void* xeq_notime(const char* filename, const uint linenumber) { (void) filename; (void) linenumber; return NULL; }

	void* xeq(const char* filename, const uint linenumber) { (void) filename; (void) linenumber; return NULL; }
	
	xeqpoint_t operator=(xeqpoint_t pxeq) { (void) pxeq; return *this; } 
	
	void* print(FILE* std) { (void) std; return NULL; }
};

#define XEQ(xeqpoint) 

#define XEQPOINT 

#define XEQPOINT_FL( FILE, LINE ) 
#endif // defined(XEQP)

#define XEQPOINT_LEVEL0 XEQPOINT

#if defined(XEQP_LEVEL3)
#define XEQPOINT_LEVEL3 XEQPOINT
#undef 	XEQP_LEVEL2
#define XEQP_LEVEL2
#else
#define XEQPOINT_LEVEL3
#endif

#if defined(XEQP_LEVEL2)
#define XEQPOINT_LEVEL2 XEQPOINT
#undef 	XEQP_LEVEL1
#define XEQP_LEVEL1
#else
#define XEQPOINT_LEVEL2
#endif

#if defined(XEQP_LEVEL1)
#define XEQPOINT_LEVEL1 XEQPOINT
#else
#define XEQPOINT_LEVEL1
#endif

extern xeqpoint_t G_xeqpoint;
extern bool		  G_xeq;

#endif /*XEQPOINT_H_*/
