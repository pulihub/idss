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
 * @file interval.h
 * @brief 
 */

#ifndef INTERVAL_H_
#define INTERVAL_H_

class events_t {
public:
	events_t	*next;
	KEYID		id;
	IPaddr		peer;

	NUM_8		TTL;
	short		type;	// LEAVE or JOIN

	bool operator==(const events_t event) {
		if (peer 	!= event.peer)	return false;
		if (TTL 	!= event.TTL) 	return false;
		if (type 	!= event.type) 	return false;
		return true;
	}
	
	events_t() {
		peer.port = 0;
	}
};

class interval_t {
	bool	initiated;
public:
	int		interval_start_time;
	int		time_first_event;
	int		theta;
 	events_t *events;
	NUM_32	E;
	NUM_32	bigger_ttl;
 	int		T_bytes;

	bool	timeout;
 	NUM_8	SeqNo;
#if defined(DEBUG)
 	bool	xeqtime;
#endif 	
	
	bool should_close(const int np, const bool istimeout, const int Theta);
	
	void interval_init(const NUM_8 SeqN, const int previous_time, const bool wastimeout) {
		
#if defined(DEBUG)
		ASSERT(!initiated);
		if (previous_time == 0) {
			xeqtime = true;
		} else {
			ASSERT(G_interval_mutex.xeq_locked_byme(), "G_interval_mutex is not locked.");
		}
#endif
		initiated 	= true;
			
		bigger_ttl 	= 0;
		timeout		= wastimeout;
		
		time_first_event = 0;
		
		SeqNo = SeqN;		
		if (SeqNo == 0) SeqNo = 1; // SeqNo == 0 is used when the peer is leaving
		
		E 		= 0;
		events	= NULL;
		T_bytes = D1HT_UDP_HEADER_SIZE+D1HT_MAINTENANCE_body_size;

		int new_interval_start_time 	= (int) elapsed_msecs(); 
		
#if defined(DEBUG)
		ASSERT(new_interval_start_time>=0)
		ASSERT(previous_time>=0)
		VERBOSEX(stderr, "new_interval_start_time=%d\n", new_interval_start_time);

		if (previous_time!=0) {
			if ((previous_time < 0) || (new_interval_start_time <= 0)) 	EXIT("previous_time or new_interval_start_time is/are less than zero. Detail: previous_time=%d, new_interval_start_time=%d", previous_time, new_interval_start_time);
			if (xeqtime && previous_time > new_interval_start_time) 	EXIT("previous_time > new_interval_start_time. Detail: previous_time=%d, new_interval_start_time=%d", previous_time, new_interval_start_time);	
		} else {
			Assert( (SeqN <= 1), XEQ_ERROR);
		}
#endif
		if (new_interval_start_time <= previous_time) {
#if defined(DEBUG)
			xeqtime = false;
#endif
			interval_start_time = previous_time+1;			
		} else {
#if defined(DEBUG)
			xeqtime = true;
#endif
			interval_start_time = new_interval_start_time;
		}
	};
	
	void destroy() {
		if (!initiated) return; 
//		check(__FILE__,__LINE__);
		events_t *next_event, *event;
		for (event = events; event!=NULL; event=next_event) {
			next_event = event->next;
			FREE(event);			
		}
		events = NULL;
	}; 
	
#if defined(DEBUG)	
	void* check(char* filename, const uint linenumber) {
		void *retcode = NULL;
		if (!initiated) 	return retcode; 
		if (!G_parms.check) return retcode;

		events_t *event;
		NUM_32	count 	= 0;
		char	msgerr[200];
		sprintf(msgerr, "ERROR Cause: in interval SeqNo=%d, E=%d,bigger_ttl=%d,  event=(id=%d,ip=%s:%u,ttl=%d,type=%d)", SeqNo, E,  bigger_ttl, event->id, IPtoP(event->peer), event->TTL, event->type);
		for (event = events; event!=NULL; event=event->next) {
			ASSERTFL(event->id == ip2id(event->peer), msgerr, filename, linenumber);
			ASSERTFL(event->TTL <= bigger_ttl, msgerr, filename, linenumber);
			count++;
		}
		sprintf(msgerr, "ERROR Cause: in interval SeqNo=%d, E=%d, bigger_ttl=%d, count=%d", SeqNo, E,  bigger_ttl, count);
		ASSERT(count == E, msgerr, filename, linenumber);
		return retcode;
	};
	
	void *print_check(char* filename, const uint linenumber) {
		void *retcode=NULL;
		VERBOSEFL(stderr, filename, linenumber, "Printing interval SeqNo=%d, E=%d, bigger_ttl=%d\n", SeqNo, E, bigger_ttl);
		if (events != NULL) {
			VERBOSEFL(stderr, filename, linenumber, "events=");
			events_t *event;
			NUM_32	count = 0;
			for (event = events; event!=NULL; event=event->next) {
				VERBOSE(stderr, "event=(id=%u,ip=%s:%u,ttl=%d,type=%d)\n", event->id, IPtoP(event->peer), event->TTL, event->type);
				ASSERT(event->id == ip2id(event->peer));
				ASSERT(event->TTL <= bigger_ttl);
				ASSERT(event->type <= MAX(JOINS,LEAVES));
				count++;
			}
			if (G_parms.VERBOSE) fprintf(stderr, "\n");
			ASSERT(count == E);
		}
		return retcode;
	}
#else
	void* check(char* filename, const uint linenumber) { return NULL; }
	void *print_check(char* filename, const uint linenumber) { return NULL; }
#endif
};

extern	interval_t *G_inter_current;

struct temp_interval_t {
	NUM_32		ttl;
	interval_t	*interval;
	IPaddr		target;
};

#ifdef DEBUG
class selfmsg_t {
public:
	struct 	msg_hd 					msg_header;
	struct 	D1HT_MAINTENANCE_body 	msg_body;
	IP		*ips[2][2];
	PORTNO	*ports[2];
	selfmsg_t *next;
	
	selfmsg_t () { next = NULL; }
};
extern selfmsg_t G_selfmsgs;
#endif

#endif /*INTERVAL_H_*/
