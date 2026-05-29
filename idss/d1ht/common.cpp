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
 * @file common.c
 * @brief 
 */
#include "D1HT.h"

namespace d1ht {

  // the g_err_type and g_err_priority arrays should be in sync with the XEQ_XX macros defined in debug.h
char 	g_err_type[3][10] = {"SILENT",		"WARNING",  "ERROR"};
int		g_err_priority[3] = {LOG_WARNING,	LOG_WARNING, LOG_ERR};

void *D1HT_ERROR_function(const char* cause, const char* message, const int XEQ_FLAG, const char* file, const uint line)
{ 
  if (XEQ_FLAG == XEQ_SILENT) return NULL;
  if ((!G_parms.verbose) && (XEQ_FLAG == XEQ_WARNING)) return NULL;
  if (G_exiting && (XEQ_FLAG != XEQ_ERROR)) return NULL;

  if (G_parms.daemonize ) {
#ifndef D1HT_PTHREAD_LIGHT	  
	pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
	 
	if (pdata) 	{ 
		syslog(g_err_priority[XEQ_FLAG], "tid=%u %s in program %s thread=%s FILE=%s LINE=%d myIP=%s:%u G_myID=%u myNAME=%s\n", (uint) pthread_self(),
  			g_err_type[XEQ_FLAG], G_parms.program_name, pdata->pthread_name, file, line, IPtoP(G_parms.myIP), G_myID, G_parms.my_host_name);
	} 
	else
#endif
	{
		syslog(g_err_priority[XEQ_FLAG], "tid=%u %s in program %s FILE=%s LINE=%d myIP=%s:%u G_myID=%u myNAME=%s\n", (uint) pthread_self(),
  			g_err_type[XEQ_FLAG], G_parms.program_name, file, line, IPtoP(G_parms.myIP), G_myID, G_parms.my_host_name);
	}
	if (G_exiting) {
		syslog(g_err_priority[XEQ_FLAG], "tid=%u Secondary ERROR while Exiting Cause: %s\n", (uint) pthread_self(), cause);
	} else {
		syslog(g_err_priority[XEQ_FLAG], "tid=%u Cause: %s\n", (uint) pthread_self(), cause);
	}
	if (strcmp(cause,message) != 0 ) syslog(g_err_priority[XEQ_FLAG], "tid=%u, Message: %s\n",(uint)  pthread_self(), message);
  }
  
  VERB(stderr, file, line, "%s in program %s myIP=%s:%u G_myID=%u myNAME=%s\n", g_err_type[XEQ_FLAG], G_parms.program_name, IPtoP(G_parms.myIP), G_myID, G_parms.my_host_name);
  if (G_exiting) {
	  VERB(stderr, file, line, "%s Secondary ERROR while Exiting Cause: %s\n", g_err_type[XEQ_FLAG], cause);
  } else {
	  VERB(stderr, file, line, "%s Cause: %s\n", g_err_type[XEQ_FLAG], cause);
  }
  if ((message!=NULL) && (strcmp(cause,message) != 0)) VERB(stderr, file, line, "%s Message: %s\n", g_err_type[XEQ_FLAG], message);

  fflush(stderr);
  return NULL;
}

#if defined(DEBUG)
KEYID ip2idx(const IPaddr address) { return ((KEYID) address.ip.s_addr + (KEYID) address.port); }
#endif

#define SHA_OFF (SHA_DIGEST_LENGTH-sizeof(long int)-1)

KEYID key2id(D1HT_LOOKUP_EXTERN_body lookup_body) {

   	unsigned char sha_result[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char *) &lookup_body.key, (unsigned long) sizeof(lookup_body.key), sha_result);
    unsigned char* sha_temp = &(sha_result[SHA_OFF]);
    
   	KEYID 	id;
    memcpy(&id, sha_temp, sizeof(id));
   	return 	id;
}

KEYID key2id(const char key[D1HT_MAX_KEY_SIZE]) {

	unsigned char sha_result[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char *) key, D1HT_MAX_KEY_SIZE, sha_result);
    unsigned char* sha_temp = &(sha_result[SHA_OFF]);

	KEYID   id;
	memcpy(&id, sha_temp, sizeof(id));
	return  id;
}

KEYID ip2id(const IPaddr address) {
#if defined(DEBUG) && 0
    if (!G_noport0) {
        if (!G_parms.test) EXIT("G_noport0=false");
        return ip2idx(address);
    }
#endif
   	KEYID 	id;
   	int off = SHA_DIGEST_LENGTH - sizeof(id)-1;
   	unsigned char sha_result[SHA_DIGEST_LENGTH];
    // note that, due to alignment issues, for IPv4 we have that sizeof(address)=8 while sizeof(address.ip)+sizeof(address.port)=6
    SHA1((const unsigned char *) &address, (sizeof(address.ip)+sizeof(address.port)), sha_result);
    unsigned char* sha_temp = &(sha_result[off]);
    memcpy(&id, sha_temp, sizeof(id));
   	return 	id;
}

KEYID ip2id(const IP address, const PORTNO port) {
	IPaddr address2;
	
	address2.ip 	= address;
	address2.port	= port;
	
	return ip2id(address2);
}

void my_terminate() { EXIT("Unknown exception caught\n"); }

bool BigEndian() {
        union {
          short  s;
      char   c[sizeof(short)];
    } un;

        un.s = 0x0102;
        if (sizeof(short) == 2) {
                if (un.c[0] == 1 && un.c[1] == 2) return true;
                else if (un.c[0] == 2 && un.c[1] == 1) return false;
                else return true;
        } else return false;
}

} // namespace d1ht
