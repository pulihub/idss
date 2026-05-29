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
 * @file messages.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht
{
char *ip2p(const IP ip) {
    static char str[128];
    if (inet_ntop(AF_INET, &ip, str, sizeof(str)) == NULL) return(NULL);
    return(str);
}

int get_IPaddr(const char *hostname, IPaddr *address) { // while get_IPaddr is a routine, IPtoP is a macro defined in base.h
	int		retcode = -1;
	char    buffer[80];
	char	*match, *match1;

	strncpy(buffer, hostname, sizeof(buffer));

	match = buffer;

	if ((match1 = strstr( match, ":" )) == NULL) { 
		address->port = (PORTNO) 0;
	} else { 
		int port;
		
		match1[0] = '\0';	
		match1++;	
		if (sscanf(match1, "%u", &port ) != 1) EXIT("D1HT _peer address should have the format ip_address:port");
		address->port = (PORTNO) htons(port);
	}

	retcode = get_IP(match, &(address->ip));
	return retcode;
}

int get_IP(const char* hostname, IP *address) {

	struct 	addrinfo *res, hints;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family		= AF_INET;
	hints.ai_socktype 	= SOCK_STREAM;
	hints.ai_protocol	= IPPROTO_TCP;

	if (G_parms.infiniband) {
		char hostIB[256];
		ASSERT(G_parms.IB_suffix != NULL, XEQ_ERROR);
		sprintf( hostIB, "%s%s", hostname, G_parms.IB_suffix);

		if (getaddrinfo(hostIB, NULL, &hints, &res) != 0) {
			int h_errsave = h_errno;
			EXIT("invalid hostname or IP address. Detail hostname=%s, msg=%s", hostIB, hstrerror(h_errsave));
		}

	} else {

		if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
			int h_errsave = h_errno;
			EXIT("invalid hostname or IP address. Detail hostname=%s, msg=%s", hostname, hstrerror(h_errsave));
		}
	}

	*address = ((struct sockaddr_in *) (res->ai_addr))->sin_addr;

	freeaddrinfo(res);

	return 0;
}

int get_myName(char* hostname, const int hostname_len, IP *address) {

	XEQ_EXEC(gethostname(hostname, hostname_len), XEQ_ERROR);
	
	XEQ_EXEC(get_IP(hostname, address), XEQ_ERROR);

	return 0;
}

int get_Name(char *hostname, const socklen_t hostlen, IP address) {
	
	struct 	sockaddr_in socket_address;
	
	bzero(&socket_address, sizeof(socket_address));
	
	socket_address.sin_addr 	= address;
	socket_address.sin_family 	= AF_INET;
	
	int ecode = getnameinfo((sockaddr *) &socket_address, sizeof(socket_address), hostname, hostlen, NULL, 0, (NI_DGRAM || NI_NOFQDN));

	if  ( ecode != 0 ) EXIT("Failed to resolve hostname. Detail: hostname=%s, msg=%s", hostname, gai_strerror(ecode));
	
	return 0;
}
/* 
 *  These readn and writen routines come from the Unix Network Programming Book (Stevens, Fenner, Rudoff)
 */
ssize_t readn(const int fd, const void *vptr, const size_t n) 
//  Read "n" bytes from a descriptor and return -1 if the file ends
{
	ssize_t retcode = readN(fd, vptr, n);
	int errsave = errno;
	
	if (retcode != (ssize_t) n && errsave == 0) errsave = ENODATA;
	
	errno = errsave;
	return(retcode);
}

ssize_t readN(const int fd, const void *vptr, const size_t n)
//  Read "n" bytes from a descriptor and return 0 if the file ends
{
        size_t          nleft 	= n;
        ssize_t         nread	= 0;
        int             errsave = 0;
        char            *ptr 	= (char *) vptr;

        while (nleft > 0) {
                errsave = 0;
                if ( (nread = D1HT_READ(fd, ptr, nleft)) < 0) {
                	errsave = errno;
                	if (errno == EINTR)                             	/* and call D1HT_READ() again */
                		nread = 0;
                	else {                                                  /* error */
                        break;
                	}
                } else if (nread == 0) {
                	errsave = errno;
                	if (errsave == 0) errsave = ENODATA;
                	break;                          /* EOF */
                }

                nleft -= nread;
                ptr   += nread;
        }

        ssize_t retcode = n - nleft;
        
        if (retcode == 0 && errsave != 0) retcode = nread;

        VERBOSEX(stderr,"readN bytes=%ld\n", (long int) retcode);

        errno = errsave;
        return(retcode);
}

ssize_t writen(const int fd, const void *vptr, const size_t n) 
// Write "n" bytes to a descriptor.
{
	ssize_t 	nwritten;
	size_t 		nleft 	= n;
	const char	*ptr 	= (char *) vptr;
	int 		errsave = 0;

	while (nleft > 0) { 
		errsave = 0;
	    if ( (nwritten = write(fd, ptr, nleft)) <= 0) { 
	    	errsave = errno;
	        if (nwritten < 0 && errno == EINTR)	{		/* and call write() again */
	        	nwritten= 0;           
	        } else {									/* error */
	        	if (nleft != n) {
	        		errsave = ENODATA;
		        	verbose(stderr, "WARNING: Cause: Incomplete write. Detail: writen bytes=%ld, n=%ld, errno=%ld\n", (long) (n-nleft), (long) n, (long) errsave);
	        	}
	        	errno = errsave;
	        	return(-1);
	        }
	    }
		
	    nleft -= nwritten;
	    ptr   += nwritten;
	}
	
	VERBOSEX(stderr,"writen bytes=%ld, n=%ld, errno=%ld\n", (long) n, (long) n, (long) errsave);

	errno	= errsave;
	return(n);
}

int Connect(const int fd, const struct sockaddr *sa, const socklen_t salen)
{
	int retcode ;
	if ((retcode = connect(fd, sa, salen)) <  0 ) {
    	int errsave = errno;
//		G_probe.probe_connection(fd, __FILE__, __LINE__);
    	errno = errsave;
	}
	return retcode;
}

ssize_t Sendto(const int fd, const void *ptr, size_t nbytes, const int flags, const struct sockaddr *sa, const socklen_t salen)
{
	ssize_t retcode;
	if ( (retcode = sendto(fd, ptr, nbytes, flags, sa, salen)) != (ssize_t) nbytes) {
    	int errsave = errno;
//		G_probe.probe_connection(fd, __FILE__, __LINE__);
    	errno = errsave;
	}
	return retcode;
}

int Accept(const int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int 	n = 0, tries = 0;
	int 	errsave;
again:
    errsave = 0;
	if ( (tries < G_parms.retries) && ((n = accept(fd, sa, salenptr)) < 0)) 
	{
		errsave = errno;
// the retries based on the errno's bellow are in accordance with the Linux "man 2 accept" manual page
		if (errno == EPROTO || errno == ENETDOWN || errno == ENOPROTOOPT || errno == EHOSTDOWN 	  || errno == ECONNABORTED || errno == EPERM || errno == ETIMEDOUT ||
			errno == ENONET || errno == EAGAIN   || errno == ENETUNREACH || errno == EWOULDBLOCK  || errno == EHOSTUNREACH || errno == EINTR || errno == EOPNOTSUPP) { 
//			int errnosave=errno;
			mysleep_msecs(tries*100); 
			tries++; 
//			VERBOSEX(stderr, "Accept try=%d, previous error=%s (%d)\n", tries, strerror(errnosave), errnosave);
			errno = errsave;
			goto again;
		}
		errno = errsave;
	}
	return(n);
}

} // namespace d1ht
