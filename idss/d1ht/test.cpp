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
 * @file test.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

#ifndef DEBUG
bool 	routing_table::xeq_table(bool succxeq, const char* file, const uint line) { return true; }
void* 	routing_table::test_table(const char* file, const uint line) { return NULL; }
#else

#define	TABLE_ERR(err) { if (err) {VERB(stderr, file, line, "ERROR Cause: xeq_table() %s. Detail: bucket=%d, bucketn=%d, slot=%d, bucket_peers=%d, FILE=%s, LINE=%d \n\n", #err, bucket, bucketn, slot, bpeers, __FILE__, __LINE__); print_table(); Exit(EXIT_FAILURE);}}

#define	TABLE_ERR2(err) { 																												\
	if (err) {																															\
			pos_t	dummy1, dummy2, dummy3;																								\
			G_xeq_find = true;																											\
			KEYID tempID = G_myID - 1;																									\
			find(tempID,  &dummy1, &dummy2, &dummy3);																					\
			G_xeq_find = false;																											\
			VERB(stderr, file, line, "ERROR Cause: xeq_table() %s. Detail: (bucket,slot,IP), succ=(%u,%u,%s:%u), pred=(%u,%u,%s:%u) FILE=%s, LINE=%d\n", \
			#err, succpos.bucket, succpos.slot, IPtoP(table[succpos.bucket].slot[succpos.slot]),  										\
			pred.bucket, pred.slot, IPtoP(table[pred.bucket].slot[pred.slot]), __FILE__, __LINE__); 									\
			print_table(); 																												\
			Exit(EXIT_FAILURE);																													\
	}																																	\
}

bool routing_table::pub_xeq_table(bool succxeq, const char* file, const uint line) {
	LOCK(rt_mutex);
	bool retcode = xeq_table(succxeq, file, line);
	UNLOCK(rt_mutex);
	return retcode;
}

bool routing_table::xeq_table(bool succxeq, const char* file, const uint line) {
	if (npeers() == 0) return(false);
	
	ASSERT(rt_mutex.xeq_locked_byme(), "rt_mutex must be hold before calling xeq_table.");
	
  	ASSERT(G_myID == ip2id(G_parms.myIP));

	if (succxeq  && (npeers() > 2)) {

		pos_t	pred, succpos, dummy1, dummy2;
		KEYID	tempID;
		
		tempID = G_myID - 1;
		find(tempID, &pred, &dummy1, &dummy2);
		
		tempID = G_myID + 1;
		find(tempID, &dummy2, &dummy1, &succpos);

//	   	VERBO(stderr,"pred=%s:%u, succ=%s:%u\n", IPtoP(pred_IP()), IPtoP(succ_IP()));
	   	
		TABLE_ERR2(succ_IP().ip.s_addr != table[succpos.bucket].slot[succpos.slot].ip.s_addr);
		TABLE_ERR2(succ_ID() != ip2id(succ_IP()));
		TABLE_ERR2(pred_IP().ip.s_addr != table[pred.bucket].slot[pred.slot].ip.s_addr);
		TABLE_ERR2(pred_ID() != ip2id(pred_IP()));
		TABLE_ERR2(G_myID==succ_ID());
		TABLE_ERR2(G_myID==pred_ID());
		TABLE_ERR2(npeers()==2 && pred_ID()!=succ_ID());
		TABLE_ERR2(pred_ID()==0);
		TABLE_ERR2(succ_ID()==0);

		if (G_noport0) TABLE_ERR2(ntohs(pred_IP().port) == 0);
		if (G_noport0) TABLE_ERR2(ntohs(succ_IP().port) == 0);
		
//		fprintf(stderr, "myIP=%s:%u, myID=%u, succ(G_myID)=%s:%u, idsucc=%u\n", IPtoP(G_parms.myIP), G_myID, IPtoP(succ(G_myID)), ip2id(succ(G_myID)));

		TABLE_ERR2(G_parms.myIP != succ(G_myID));
		TABLE_ERR2(G_parms.myIP != succn(G_myID,0));
		TABLE_ERR2(succ_IP() != succn(G_myID,1));
	}
 		
  	int xeq_npeers = 0;
 	for ( int bucket=0; bucket<rt_size; bucket++) { // xeq the the entries in the original bucket
		KEYID 	ida = 0, idb;
		int		slot = 0;
		int 	bucketn = bucket;
		bucket_t *entry = &table[bucket];
		int		bpeers	= bucket_peers[bucket];
		
		xeq_npeers += bpeers;

		if (bpeers) {
			TABLE_ERR(entry->slot[0].ip.s_addr == 0) ;
			ida	= ip2id(entry->slot[0]);
			int 	bpeers0 = MIN(bpeers,BUCKETSIZE); 
			if (G_noport0) TABLE_ERR (ntohs(entry->slot[0].port) == 0);
			bpeers0--;
			for	(slot=0; slot < bpeers0 ; slot++) {
				TABLE_ERR (entry->slot[slot+1].ip.s_addr == 0) ;
				TABLE_ERR ((idb = ip2id(entry->slot[slot+1])) <= ida) ; 
				if (G_noport0) TABLE_ERR (ntohs(entry->slot[slot+1].port) == 0);
				ida = idb;
			}
		}

		// xeq if there are "holes" or additional slots in the original bucket
		for (slot=bpeers; slot<BUCKETSIZE-1; slot++) {
			if (entry->slot[slot].ip.s_addr  == 0 ) continue;
			TABLE_ERR (slot && entry->slot[slot-1].ip.s_addr== 0 ) ;
			TABLE_ERR (ip2id(entry->slot[slot]) / basepos == (uint) bucket) ;
		}

		for (int peers=BUCKETSIZE; peers<bpeers; ) { // xeq for the entries in the other buckets
			bucketn = (bucketn+1) % rt_size;
			TABLE_ERR (bucketn == bucket) ; // if I didn't find all the bpeers entries, it is a error
			
			bucket_t *entryn = &table[bucketn];	
			
			for (slot=bucket_peers[bucketn]; slot < BUCKETSIZE && entryn->slot[slot].ip.s_addr && peers<bpeers; slot++) {
				idb = ip2id(entryn->slot[slot]);
				if ( idb/basepos == (uint) bucket) {
					peers++;
					TABLE_ERR (idb <= ida) ;
				}
			}
		} 

		do { // xeq if there is still any entry for bucket==bucket
			bucket_t *entryn = &table[bucketn];	

			for (; slot < BUCKETSIZE ; slot++) TABLE_ERR (entryn->slot[slot].ip.s_addr && ip2id(entryn->slot[slot])/basepos == (uint) bucket ) ;

			bucketn = (bucketn+1) % rt_size;
			slot = 0;
		} while (bucketn != bucket);
	}

 	if (xeq_npeers!=npeers()) EXITFL(file, line, "npeers didnt match. Detail: xeq_npeers=%d, npeers()=%d", (int) xeq_npeers, (int) npeers());
 	
//	return(false);

// 	print_table();
	KEYID	id0, id;

	id0 = 0;
	for (int j=0; j < npeers(); j++) {
		
		id0 = ip2id(succn(id0,1));
		id 	= id0;
		
		for (int i=1; i < npeers(); i++) { 
			
			IPaddr 	IP1 = succn(id0,i);
			IPaddr 	IP2 = succn(id,1);

   			KEYID 	id1 = ip2id(IP1); 
   			KEYID 	id2 = ip2id(IP2);

   			if (id1!=id2) {
   				G_parms.VERBOSE = true;
   				VERB(stderr, file, line,"ERROR Cause: xeq_table() SUCC. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2));  
   				ASSERT(IP1!=IP2);
   				EXIT("xeq_table() SUCC");
   				G_succ_pred_err = true;  				
   				IPaddr 	IP1 = succn(id0,i);
   				IPaddr 	IP2 = succn(id,1);
   				G_succ_pred_err = false;
   				
   	   			KEYID 	id1 = ip2id(IP1);
   	   			KEYID 	id2 = ip2id(IP2);
   	   			if (id1!=id2) {   				
	   				VERB(stderr,  file, line,"ERROR Cause: xeq_table() SUCC. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2)); 
	   				print_table(); 
	   				Exit(EXIT_FAILURE);
   	   			} else break;

   			};
   			id = id1;
		}
	}
	
	id0 = 0;
	for (int j=0; j < npeers() && j < 8; j++) {
		
		id0 = ip2id(predn(id0,1));
		id 	= id0;
		
		for (int i=1; i < npeers(); i++) { 
			
			IPaddr 	IP1 = predn(id0,i);
			IPaddr 	IP2 = predn(id,1);

   			KEYID 	id1 = ip2id(IP1); 
   			KEYID 	id2 = ip2id(IP2);

   			if (id1!=id2) {
   				G_parms.VERBOSE = true;
   				VERB(stderr,  file, line,"ERROR Cause: xeq_table() PRED. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2));   				
   				ASSERT(IP1!=IP2);
   				EXIT("xeq_table() PRED");
   				G_succ_pred_err = true; 
   				IPaddr 	IP1 = predn(id0,i);
   				IPaddr 	IP2 = predn(id,1);
   				G_succ_pred_err = false;
   				
   				 
   	   			KEYID 	id1 = ip2id(IP1); 
   				 
   	   			KEYID 	id2 = ip2id(IP2);
   	   			if (id1!=id2) {   
	   				VERB(stderr, file, line,"ERROR Cause: xeq_table() PRED. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2)); 
	   				print_table(); 
	   				Exit(EXIT_FAILURE);
   	   			} else break;
   			};
   			id = id1;
		}
	}
	
	id0 = ip2id(succn(0,0));
	id 	= id0;
	do { 
		for (int i=0; i < 16 && i < npeers(); i++) { 
			IPaddr 	IP1 = succn(id,i);
			KEYID 	id1 = ip2id(IP1);
			
			IPaddr 	IP2 = predn(id1,i);
   			KEYID 	id2 = ip2id(IP2);

   			if (id!=id2) {   	
   				G_parms.VERBOSE = true;
   				VERB(stderr, file, line,"ERROR Cause: xeq_table() SUCC-PRED. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2));
   				ASSERT(IP1!=IP2);
   				EXIT("xeq_table() SUCC-PRED");  				
   				G_succ_pred_err = true;
   				IPaddr 	IP1 = succn(id,i);
   				IPaddr 	IP2 = predn(id1,i);
   				G_succ_pred_err = false;
   				
   	   			 
   	   			KEYID 	id1 = ip2id(IP1);
  	   			 
   	   			KEYID 	id2 = ip2id(IP2);
  	   			if (id!=id2) {   
	   				VERB(stderr, file, line,"ERROR Cause: xeq_table() SUCC-PRED. Detail: id=%u, id0=%u, id1=%u, id2=%u, i=%d, IP1=%s:%u, IP2=%s:%u\n", id, id0, id1, id2, i, IPtoP(IP1), IPtoP(IP2)); 
	   				print_table(); 
	   				Exit(EXIT_FAILURE);
	   			};

   			}
		}

		id = ip2id(succn(id,1));
   	} while (id!=id0); 

	return(false);
}

#define XPREDSUCC(aa,nn) { 										\
 	int		n	= nn;											\
 	pos_t	pred, predl, succ;									\
	find(aa, &pred, &predl, &succ);								\
	KEYID	a 	= ip2id(table[succ.bucket].slot[succ.slot]);	\
 	KEYID 	id 	= ip2id(succn(a,n)); 							\
	KEYID 	id2 = ip2id(predn(id,n)); 							\
	if (a!=id2) {	 											\
		VERB(stderr, file, line,"ERROR Cause: xeq_table() SUCCN/PREDN. Detail:  key=%u, n=%d, succn(%u,%d)=%u, predn(%u,%d)=%u\n", a, n, a, n, id, id, n, id2);	\
		print_table(); Exit(EXIT_FAILURE);								\
	}															\
}
 	
#define PFIND(key) { 																							\
	found = find(key, &pred, &predl, &succ); 																	\
	VERB(stderr, file, line, "\n =====> FIND key=%u, found=%d, pred=[%u,%u], predl=[%u,%u], succ=[%u,%u]\n",	\
					key, found, pred.bucket, pred.slot, predl.bucket, predl.slot, succ.bucket, succ.slot); 		\
	}
	
#define PFINDP(bucketzz) { 																						\
	find_pred(bucketzz, &pred, &predl); 																		\
	VERB(stderr, file, line, "FIND PRED  bucket=%u, pred=[%u,%u], predl=[%u,%u]\n",								\
					bucketzz, pred.bucket, pred.slot, predl.bucket, predl.slot); 								\
	}
					
#define  T(a, b)  { 																							\
	if(a!=b) {																									\
		VERB(stderr, file, line, "ERROR Cause: xeq_table(). Detail: %s!=%s, %u!=%u, file=%s, line=%u\n", #a, #b,  a, b, __FILE__, __LINE__  ); 	\
		print_table();																							\
		fflush(stderr); 																						\
		Exit(EXIT_FAILURE); 																								\
	} }
					
#define PPOS(a, b, c, d)  { 																					\
	pos_t posl, posr; posl.bucket=a; posl.slot=b; posr=posl2pos(posl); 											\
	/* fprintf(stderr," posl2pos logical=%d,%d, real=%d,%d\n", a, b, posr.bucket, posr.slot);	*/				\
	if (c != posr.bucket) { VERB(stderr, file, line,"ERROR Cause: posl2pos bucket returned %d instead of %d, file=%s, line=%d\n", posr.bucket, c, __FILE__, __LINE__); print_table(); Exit(EXIT_FAILURE); }	\
	if (d != posr.slot) { VERB(stderr, file, line,"ERROR Cause: posl2pos slot returned %d instead of %d, file=%s, line=%d\n", posr.slot, d, __FILE__, __LINE__); print_table(); Exit(EXIT_FAILURE); } }
					
#define PPRED(a, n, b)  { 																								\
	KEYID k = a; IPaddr z = predn(k,n); KEYID d = ip2id(z); 															\
	if (d!=b) { VERB(stderr, file, line,"ERROR Cause: pre(%lu,%lu) returned %lu instead of %lu, file=%s, line=%d\n", 	\
			(ulong) a, (ulong) n, (ulong) d, (ulong) b, __FILE__, __LINE__); Exit(EXIT_FAILURE); };};

void* routing_table::test_table(const char* file, const uint line) { 
	pos_t	pred, predl, succ;
	bool 	found;
	
  	LOCK(rt_mutex);

    IPaddr ipx;
    ipx.port = 0; 

    ipx.ip.s_addr=3; add(ipx);    
    ipx.ip.s_addr=2; add(ipx);     
    ipx.ip.s_addr=2; add(ipx);  
  //  print_table(); // PFIND(4); Exit(EXIT_FAILURE);
    ipx.ip.s_addr=4; add(ipx);   
    ipx.ip.s_addr=5; add(ipx);    
    ipx.ip.s_addr=7; add(ipx);     
    ipx.ip.s_addr=8; add(ipx);    
    ipx.ip.s_addr=9; add(ipx);    
    ipx.ip.s_addr=8; add(ipx);    
    ipx.ip.s_addr=13; add(ipx);    
    ipx.ip.s_addr=11; add(ipx);   
 
    ipx.ip.s_addr=12; add(ipx);    
    ipx.ip.s_addr=5; add(ipx);     
    ipx.ip.s_addr=6; add(ipx);     
    ipx.ip.s_addr=  134217730; add(ipx);   
    ipx.ip.s_addr=11; add(ipx);     
    ipx.ip.s_addr=  134217731; add(ipx);     
    ipx.ip.s_addr=  134217733; add(ipx);    
    ipx.ip.s_addr=15; add(ipx);   	  
    ipx.ip.s_addr=13; add(ipx);        
    ipx.ip.s_addr=14; add(ipx);     
    ipx.ip.s_addr=10; add(ipx);     
    ipx.ip.s_addr=  134217732; add(ipx);    
    ipx.ip.s_addr=10; add(ipx);     
    ipx.ip.s_addr=1; add(ipx);    
 
    ipx.ip.s_addr=  134217741; add(ipx);   
    ipx.ip.s_addr=  134217742; add(ipx);   
    ipx.ip.s_addr=  134217738; add(ipx);   
    ipx.ip.s_addr=  134217743; add(ipx);   

    ipx.ip.s_addr=  134217739; add(ipx);   
    ipx.ip.s_addr=  134217740; add(ipx);   

    ipx.ip.s_addr=	16; add(ipx);     
    ipx.ip.s_addr=  134217737; add(ipx);   
 
    ipx.ip.s_addr=  134217739; add(ipx);    
    ipx.ip.s_addr=  134217743; add(ipx);   
    ipx.ip.s_addr=  16; add(ipx);   
    ipx.ip.s_addr=  14; add(ipx);   
        
    ipx.ip.s_addr=1000000021; add(ipx);  
    ipx.ip.s_addr=1000000023; add(ipx);  
    ipx.ip.s_addr=1000000022; add(ipx);   
    ipx.ip.s_addr=1000000023; add(ipx);  
    ipx.ip.s_addr=1000000024; add(ipx);  
    
    print_table();
    
    del(1000000022);
    del(1000000025);
    del(15);
    del(2);
    del(10);
   	del(2);
    del(10);
    ipx.ip.s_addr=2;add(ipx);
    del(15);
    del(3);
    
    print_table();
    
    T(ip2id(succn(0,0)),1);
    
     T(ip2id(succn(0,1)),2);
     T(ip2id(succn(0,2)),4); 
     T(ip2id(succn(0,3)),5);
     T(ip2id(succn(0,4)),6);
     T(ip2id(succn(0,5)),7);
     T(ip2id(succn(0,6)),8);
      
     T(ip2id(succn(0,7)),9);
     T(ip2id(succn(0,8)),11);
     T(ip2id(succn(0,9)),12);
     T(ip2id(succn(0,10)),13); 

     T(ip2id(succn(0,11)),14);
     T(ip2id(succn(0,12)),16);
//    print_table();

#if ! defined(PETROBRAS)   
     T(ip2id(succn(0,13)),G_myID);
     T(ip2id(succn(1,13)),G_myID);
     T(ip2id(succn(2,12)),G_myID);    
   
     T(ip2id(succn(0,14)),134217730);
     T(ip2id(succn(0,15)),134217731);
 
     T(ip2id(succn(1,14)),134217730);
     T(ip2id(succn(1,15)),134217731);
 
     T(ip2id(succn(2,13)),134217730);
     T(ip2id(succn(2,14)),134217731);
    
   	 T(ip2id(succn(134217730,12)),1000000023);
    
   	 T(ip2id(succn(134217730,15)),2);
   	
     T(ip2id(succn(0,25)),1000000021);
     T(ip2id(succn(0,26)),1000000023);
     T(ip2id(succn(0,27)),1000000024);
#endif
 
     T(ip2id(succn(1,0)),1);
     T(ip2id(succn(1,11)),14);
     T(ip2id(succn(1,12)),16);
 
     T(ip2id(succn(2,0)),2); 
     T(ip2id(succn(2,10)),14);
     T(ip2id(succn(2,11)),16);
    
        
     T(ip2id(succn(1,0)),1);
     T(ip2id(succn(1,1)),2);
     T(ip2id(succn(1,2)),4); 
     T(ip2id(succn(1,3)),5);
     T(ip2id(succn(1,4)),6);
     T(ip2id(succn(1,5)),7);
     T(ip2id(succn(1,6)),8);
    
     T(ip2id(succn(4,0)),4);
     T(ip2id(succn(4,1)),5);
     T(ip2id(succn(4,2)),6); 
     T(ip2id(succn(4,3)),7);
     T(ip2id(succn(4,4)),8); 
     
     T(ip2id(succn(3,0)),4);
     T(ip2id(succn(3,1)),5);
     T(ip2id(succn(3,2)),6); 
     T(ip2id(succn(3,3)),7);
     T(ip2id(succn(3,4)),8); 
     
     T(ip2id(succn(1000000023,0)),1000000023);
     T(ip2id(succn(1000000023,1)),1000000024);
     
     T(ip2id(succn(1000000022,0)),1000000023);
     T(ip2id(succn(1000000022,1)),1000000024);

    ipx.ip.s_addr=__INT_MAX__; add(ipx);
    ipx.ip.s_addr-=2; add(ipx);    
    ipx.ip.s_addr+=6; add(ipx);  
    ipx.ip.s_addr+=6; add(ipx);
    
    XEQ_TRUE(xeq_table(false,__FILE__,__LINE__), "RT corrupted", XEQ_ERROR);

    ipx.ip.s_addr+=(__INT_MAX__ - 500); add(ipx);  
    
    for (int kk=1; kk<BUCKETSIZE+5; kk++) {   ipx.ip.s_addr+=6; add(ipx);  };
     
    XEQ_TRUE(xeq_table(false,__FILE__,__LINE__), "RT corrupted", XEQ_ERROR);
    
	PPOS(0,0,0,0);
	PPOS(0,6,0,6);        
	PPOS(0,7,2,0);    
	PPOS(0,8,2,1);    
	PPOS(1,1,1,1);    
	   
#if ! defined(PETROBRAS) 
	PPOS(1,8,3,3);    
	PPOS(1,10,3,5);   
	PPOS(0,11,2,4);    
	PPOS(0,12,3,0);
	PPOS(31,9,3,6); 
	PPOS(31,11,4,1); 
   	PPRED(134217730,1,G_myID);
   	PPRED(134217730,2,16);
   	PPRED(134217730,3,14);
   	PPRED(134217730,4,13);
   	PPRED(134217730,5,12);
  	PPRED(1000000023,14,16);
   	PPRED(2147483651,25,7);
   	PPRED(2,4,4294966858);    
#endif 
	PPOS(31,6,31,6);    
	PPOS(31,7,2,5);    
	PPOS(31,8,2,6);    

    XEQ_TRUE(xeq_table(false,__FILE__,__LINE__), "RT corrupted", XEQ_ERROR);
 
   	PPRED(2,0,2);
   	PPRED(2,1,1);
   	PPRED(3,0,2);
   	PPRED(3,1,1);
   
   	PPRED(134217730,0,134217730);    
   
    XEQ_TRUE(xeq_table(false,__FILE__,__LINE__), "RT corrupted", XEQ_ERROR);
   
   	PFIND(9);
   	PFIND(10);
   
   	PPRED(11,7,2);
   	PPRED(9,5,4);
   	PPRED(9,7,1);
   	PPRED(9,6,2);
     
    for (int jj=0; jj < 20; jj++)  for (int kk=0; kk<npeers(); kk++) XPREDSUCC(jj,kk);
 
    XEQ_TRUE(xeq_table(true,__FILE__,__LINE__), "RT corrupted", XEQ_ERROR);
 
  	UNLOCK(rt_mutex);
  	return NULL;
}
#endif

} // namespace d1ht
