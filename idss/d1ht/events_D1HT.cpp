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
 * @file events_D1HT.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht
{
void eventmsg_t::set_ids() {
	ASSERT(initiated);
//	check(__FILE__,__LINE__);
	for (int iport=DEFAULT_PORT; iport<=OTHER_PORT; iport++) {
		for (int itype=JOINS; itype<=LEAVES; itype++) {
			for (int ievent=0; ievent < (int) body.nevents[iport][itype]; ievent++) {
				ASSERT((iport==OTHER_PORT) || (eventlist[iport][itype].ports[ievent]==G_parms.default_udp_port));
				eventlist[iport][itype].ids[ievent] = ip2id(eventlist[iport][itype].ips[ievent], eventlist[iport][itype].ports[ievent]);
			}
		}
	}
//	check(__FILE__,__LINE__);
} 

void* eventmsg_t::eventmsg_init() { 

	ASSERT(!initiated);
	initiated 	= true;
	next 		= NULL;

	ASSERT(JOINS < LEAVES);
	ASSERT(DEFAULT_PORT < OTHER_PORT);
	
	for (int iport=DEFAULT_PORT; iport<=OTHER_PORT; iport++) {
		for (int itype=JOINS; itype<=LEAVES; itype++) { 
			if (body.nevents[iport][itype] != 0) { 
				unsigned int count = (unsigned int) body.nevents[iport][itype];
				eventlist[iport][itype].ips 	= CALLOC(count, IP);
				eventlist[iport][itype].ids 	= CALLOC(count, KEYID);
				eventlist[iport][itype].ports 	= CALLOC(count, PORTNO);
				if (iport == DEFAULT_PORT) {
					for (uint ievent=0; ievent < count; ievent++) 
	 					eventlist[DEFAULT_PORT][itype].ports[ievent] = G_parms.default_udp_port;
				}
			} else {
				eventlist[iport][itype].ips 	= NULL;
				eventlist[iport][itype].ids 	= NULL;
				eventlist[iport][itype].ports 	= NULL;					
			}
		}
	} 
	return NULL;
}
  
void* eventmsg_t::fill(const IPaddr ip, const int ttl, const int itype) {

	BZERO(msg_header);
	BZERO(body)
	BZERO(sender_address);

    int     iport;
    if (ip.port == G_parms.default_udp_port) {
    	iport = DEFAULT_PORT;
    } else {
    	iport = OTHER_PORT;
    }

    body.nevents[iport][itype]      = 1;

    eventmsg_init();

    eventlist[iport][itype].ips[0]  = ip.ip;
    eventlist[iport][itype].ports[0]= ip.port;
 
	TTL = (NUM_8) ttl;	
	
	whosent.port= 0;
	whosent.ip 	= G_parms.myIP.ip;
	tot_events 	= 1;
	
	msg_header.type			= ttl;
	msg_header.sender_port	= 0;
	msg_header.systemid		= G_parms.systemid; 
	msg_header.SeqNo		= 1; 
	SeqNo					= 1;
	
	iovcnt = 0;

	iov[iovcnt].iov_base 	= (void *) &msg_header;
	iov[iovcnt].iov_len 	= (size_t) D1HT_MSG_HEADER_size;
	iovcnt++;

	iov[iovcnt].iov_base 	= (void *) &body;
	iov[iovcnt].iov_len 	= (size_t) D1HT_MAINTENANCE_body_size;
	iovcnt++;
	
	iov[iovcnt].iov_base 	= (void *) eventlist[iport][itype].ips;
	iov[iovcnt].iov_len 	= (size_t) sizeof(IP);
	iovcnt++;
	
	if (iport != DEFAULT_PORT) {
		iov[iovcnt].iov_base 	= (void *) eventlist[iport][itype].ports;
		iov[iovcnt].iov_len 	= (size_t) sizeof(PORTNO);
		iovcnt++;
	}
	
	return NULL;
}
	
#if defined(DEBUG)
void eventmsg_t::eventmsg_check(char* filename, const uint linenumber) { 
	
	M_CHECKFL(filename, linenumber, this);
	if (!initiated) return;

	for (int iport=DEFAULT_PORT; iport<=OTHER_PORT; iport++) {
		for (int itype=JOINS; itype<=LEAVES; itype++) { 
			if(body.nevents[iport][itype] != 0) { 
				
				ASSERTFL(eventlist[iport][itype].ips 	!= NULL, "Problem 1", filename, linenumber);
				ASSERTFL(eventlist[iport][itype].ids 	!= NULL, "Problem 2", filename, linenumber);
				ASSERTFL(eventlist[iport][itype].ports 	!= NULL, "Problem 3", filename, linenumber);
			
				M_CHECKFL(filename, linenumber, eventlist[iport][itype].ips);
				M_CHECKFL(filename, linenumber, eventlist[iport][itype].ids);
				M_CHECKFL(filename, linenumber, eventlist[iport][itype].ports);
				
				for (uint ievent=0; ievent < body.nevents[iport][itype]; ievent++) {
 					ASSERT(eventlist[iport][itype].ports[ievent] != 0);
 					ASSERT(eventlist[iport][itype].ips[ievent].s_addr != 0);
 					if (iport==OTHER_PORT) {
 						ASSERT(eventlist[iport][itype].ports[ievent] != G_parms.default_udp_port);	
 					} else {
 						ASSERT(eventlist[iport][itype].ports[ievent] == G_parms.default_udp_port);	
 					}
 					ASSERT((eventlist[iport][itype].ids[ievent] == 0) || (eventlist[iport][itype].ids[ievent] == ip2id(eventlist[iport][itype].ips[ievent], eventlist[iport][itype].ports[ievent])));
				}
			}
		}
	}
};
#endif

void eventmsg_t::destroy() { 
	if (!initiated) return;
//		check(__FILE__,__LINE__);
	for (int iport=DEFAULT_PORT; iport<=OTHER_PORT; iport++) {
		for (int itype=JOINS; itype<=LEAVES; itype++) {
			if (body.nevents[iport][itype] != 0) { 
				body.nevents[iport][itype] = 0;
				if (eventlist[iport][itype].ips!=NULL) 	FREE(eventlist[iport][itype].ips);
				if (eventlist[iport][itype].ids!=NULL) 	FREE(eventlist[iport][itype].ids);
				if (eventlist[iport][itype].ports!=NULL)FREE(eventlist[iport][itype].ports);
			}
		}
	} 		
	initiated = false;
};

} // namespace d1ht
