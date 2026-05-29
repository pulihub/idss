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
 * @file base.h
 * @brief definizioni base e gestione di IP e porta (definizione di IPaddr)
 */


#ifndef BASE_H_
#define BASE_H_

#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//stl libs
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <syslog.h>
#include <cmath>

//HPC Lab: aggiungo le map
#include <map>

#include <netdb.h>
#include <sys/types.h>
#include <asm/types.h>
#include <time.h>
#include <sys/time.h>

#include <stack>
#include <bitset>
#include <list>

#include <signal.h>
#include <sys/resource.h>

namespace d1ht {

    // we used the NUM types to assure that all numbers exchanged among systems have the same size, even for 64 bits systems
    typedef uint64_t NUM_64;
    typedef uint32_t NUM_32;
    typedef uint16_t NUM_16;
    typedef uint8_t NUM_8;
    typedef uint32_t KEYID;

    // D1HT was developed to run with IPv4. Defining the macro IPv6 (instead of the IPv4 bellow) is not sufficient to make it run with IPv6,
    // but will help with this porting
#define IPv4

    // structs in_addr and sockaddr_in are defined in /usr/include/linux/in.h

#if defined(IPv4)
#undef 	IPv6
#define IPVERS 			AF_INET
#define	NTOP(addr)		inet_ntoa( addr )
    typedef struct in_addr IP;
    typedef in_port_t PORTNO;

#endif

#if defined(IPv6)
#undef 	IPv4
#define IPVERS 			AF_INET6
#define	NTOP(addr)		TO_BE_DEFINED
#define	IPtoP(IP)		TO_BE_DEFINED
    typedef struct in6_addr IP;
    typedef in6_port_t PORTNO;
#endif

#if !defined(IPv6) && !defined(IPv4)
#error Please define the IP version used (IPv4 or IPv6)
#endif

#define MAX(a,b)        std::max(a,b)
#define MIN(a,b)        std::min(a,b)

    class IPaddr {
    public:
        IP ip; // Internet address
        PORTNO port; // Port number

        IPaddr() : port(0) {
            memset(&ip, 0, sizeof (ip));
        };

        IPaddr(in_addr i, in_port_t p) : ip(i), port(p) {
        }

        bool operator==(const IPaddr IP) {
            return (port == IP.port && ip.s_addr == IP.ip.s_addr);
        }

        bool operator!=(const IPaddr IP) {
            return (port != IP.port || ip.s_addr != IP.ip.s_addr);
        }

        std::string get_str_ip() const {
            return inet_ntoa(ip);
        }

        std::string get_str_port() const {
            char temp[10];
            sprintf(temp, "%d", ntohs(port));
            return temp;
        }
    };

    // while IPtoP is a macro, ip2p is a routine defined in messages.c
#define	IPtoP(IP) ip2p( IP.ip ), ntohs( IP.port )
    extern char *ip2p(const IPaddr ip);
    extern char *ip2p(const IP ip);

#define LIMIT_GET(_resource) ({            																\
	struct rlimit rl;                                    												\
    int retcode;																						\
    XEQ_EXEC(retcode = getrlimit(_resource, &rl), XEQ_WARNING);  										\
    int errsave = errno;                               													\
    if (retcode == 0) retcode = (int) rl.rlim_cur;  													\
    errno = errsave;	                               													\
    retcode;																							\
})

#define LIMIT_SET(_resource, _value) {            														\
	struct rlimit rl;                                    												\
    int retcode;																						\
    XEQ_EXEC(retcode = getrlimit(_resource, &rl), XEQ_WARNING);  										\
    int errsave = errno;                               													\
    if (retcode == 0 && _value > rl.rlim_cur) {															\
    	int old_limit = rl.rlim_cur;																	\
	    if (_value <= rl.rlim_max) {																	\
	    	rl.rlim_cur = _value;                            											\
	    } else {																						\
	    	rl.rlim_cur = rl.rlim_max;                            										\
	    }																								\
	    VERBO(stderr , "resource "#_resource" old=%u, new=%u, max=%u\n", (uint) old_limit, (uint) rl.rlim_cur, (uint) rl.rlim_max); \
	    XEQ_EXEC(retcode = setrlimit(_resource, &rl), XEQ_WARNING);                             		\
	    errsave = errno;																				\
    }                               																	\
	if (retcode != 0) VERBO(stderr, "WARNING Cause: getrlimit/setrlimit failed, message=%s\n", strerror(errsave));		\
}

#define LIMIT_MAX(_resource) {   																		\
	struct rlimit rl;  																					\
    int retcode;																						\
    XEQ_EXEC(retcode = getrlimit(_resource, &rl), XEQ_WARNING);  										\
	int errsave = errno;																				\
	if (retcode == 0) {																					\
	    VERBO(stderr , "resource "#_resource" old=%u, new=%u\n", (uint) rl.rlim_cur, (uint) rl.rlim_max);\
	    rl.rlim_cur = rl.rlim_max;  																	\
	    XEQ_EXEC(retcode = setrlimit(_resource, &rl), XEQ_WARNING);  									\
	    errsave = errno;																				\
    }                               																	\
	if (retcode != 0) VERBO(stderr, "WARNING Cause: getrlimit/setrlimit failed, message=%s\n", strerror(errsave));		\
}


#define D1HT_OPEN_MODE	open
#define D1HT_OPEN		open
#define D1HT_READ		read
#define D1HT_LSEEK		lseek
#define D1HT_CLOSE		close
#define D1HT_FSTAT		fstat

} // namespace d1ht

#endif /*BASE_H_*/
