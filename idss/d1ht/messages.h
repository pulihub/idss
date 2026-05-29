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
 * @file messages.h
 * @brief definizione dei messagggi e delle macro
 *
 *
 * @note Sono stati introdotti 2 nuovi tipi di messaggi D1HT:
 * - D1HT_QUERY_REQUEST: consente di disseminare una query distribuita sull'overlay;
 * - D1HT_QUERY_REPLY:  consente di aggregare i risultati della query distribuita da rimandare al nodo origine.
 *
 */
#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <netinet/tcp.h>   // for TCP_NODELAY definition
#include <sys/select.h>
#include <sys/uio.h>
#include <fcntl.h>

/**
 * @TODO: la dimensione delle stringhe di messaggio e del risultato va confrontato con requisiti esterni
 */
//#define MAXQUERYLEN 4096       /**< lunghezza massima di una query */
//#define MAXREPLYLEN 2560000    /**< lunghezza massima di una reply */

#define MAXQUERYLEN 8192         /**< lunghezza massima di una query */
#define MAXREPLYLEN 5120000   /**< lunghezza massima di una reply */

/**
 *
 * @param hostname
 * @param address
 * @return
 */
extern int get_IPaddr(const char *hostname, IPaddr *address);

/**
 *
 * @param hostname
 * @param address
 * @return
 */
extern int get_IP(const char* hostname, IP *address);

/**
 *
 * @param hostname
 * @param hostname_len
 * @param address
 * @return
 */
extern int get_myName(char* hostname, const int hostname_len, IP *address);

/**
 *
 * @param hostname
 * @param hostlen
 * @param address
 * @return
 */
extern int get_Name(char *hostname, const socklen_t hostlen, IP address);

extern ssize_t readn(const int fd, const void *vptr, const size_t n);
extern ssize_t readN(const int fd, const void *vptr, const size_t n);
extern ssize_t writen(const int fd, const void *vptr, const size_t n);

/**
 * All messages have a header with:
 * - the type of the message,
 * - a seq id
 * - and the port of the sender
 */
class msg_hd {
public:
    NUM_8 type;         /**< if type is less than 127, it is in fact the TTL of the maintenance message */
    NUM_8 SeqNo;        /**< to assure message delivery over UDP */
    PORTNO sender_port; /**< Port number */
    KEYID systemid;     /**< to assure that peers from two different D1HT systems won't mixed up their networks */

    /** uguaglianza tra classi */
    bool operator==(const msg_hd header) {
        if (type != header.type) return false;
        if (SeqNo != header.SeqNo) return false;
        if (sender_port != header.sender_port) return false;
        if (systemid != header.systemid) return false;
        return true;
    }
};

#define D1HT_MSG_HEADER_size (sizeof(NUM_8)+sizeof(NUM_8)+sizeof(PORTNO)+sizeof(KEYID))

/*
 * types of messages:
 *
 *      message types   0 to 126    are used for maintenance msgs thru UDP (in fact the msg type means the TTL of the msg)
 *    	message type	127         is a msg ith TTL=0 and no events
 * 	message type	128         used to forward events to a joining node, thru TCP
 * 	message types	128+        are used for other msgs, typically thru TCP (D1HT_LOOKUP_EXTERN is handled by a dedicated thread)
 */

#define D1HT_TTL_0_NOEVENTS	127
#define D1HT_FORWARD_EVENTS     128

#define D1HT_LEAVE			140
#define D1HT_PROBE			141
#define D1HT_STAB			142

#define D1HT_LOOKUP_EXTERN	150
#define D1HT_LOOKUP		151
#define D1HT_LOOKUP_SUC		152  	/**< Used to reply to D1HT_LOOKUP_EXTERN and D1HT_ROUTE queries */
#define D1HT_LOOKUP_FAILED	153 	/**< Used to reply to D1HT_LOOKUP_EXTERN and D1HT_ROUTE queries */
#define D1HT_LOOKUP_RETURN	154 	/**< Used to reply to D1HT_LOOKUP queries */
#define D1HT_ROUTE		155
#define D1HT_SUCC		156
#define D1HT_DUMP_TABLE		157

#define D1HT_STAT_REPLY		160
#define D1HT_STAT_TOT		161
#define D1HT_STAT_DIFF		162

// ATTENTION. msg types in between 170 an 199 are reserved for applications use

/** @note custom message types for HPC Lab */
#define D1HT_QUERY_REQUEST  180     /**< usato per disseminare un messaggio nell'overlay */
#define D1HT_QUERY_REPLY    181     /**< usato per inviare una reply ad un mesaggio */


#define D1HT_JOIN_FINISHED	240
#define D1HT_JOIN_REQUEST	241
#define D1HT_JOIN_REPLY		242
#define D1HT_JOIN_STOP_TTL	243

#define D1HT_INVALID		253 	/**< invalid type of message, normally used as a flag */
#define D1HT_DUMMY		254


/**< Headers of the msgs */
#define	DEFAULT_PORT	0
#define	OTHER_PORT      1

/** Eventi contemplati dalla libreria */
#define	JOINS			0
#define LEAVES  		1



class D1HT_MAINTENANCE_body {
public:
    NUM_8 nevents[2][2];
    #define D1HT_MAINTENANCE_body_size (4*sizeof(NUM_8))

    bool operator==(const D1HT_MAINTENANCE_body body) {
        if (nevents[0][0] != body.nevents[0][0]) return false;
        if (nevents[0][1] != body.nevents[0][1]) return false;
        if (nevents[1][0] != body.nevents[1][0]) return false;
        if (nevents[1][1] != body.nevents[1][1]) return false;
        return true;
    }
};

//tipi di messaggi

/**
 * @brief messaggio per disseminare una query sull'overlay D1HT.
 * Tutti i nodi che ricevono un messaggio di questo tipo lo inoltreranno, con TTL decrescente, ai loro rho successori, entro lo scadere della deadline assegnata.
 *
 * @note custom message for HPC Lab Extension
*/
struct D1HT_QUERY_REQUEST_body {
    int idQuery;            /**< l'id della query */
    char uqi[MAXQUERYLEN];  /**< uqi generato da IDSS */
    char msg[MAXQUERYLEN];  /**< il messaggio da elaborare e disseminare */
    int deadline;           /**< tempo residuo per elaborare la query */
    double reductor;        /**< reduction factor del TTL IDSS */

    int TTL;                /**< EDRA-TTL: necessario per la disseminazione */

    //serve per marcare l'origin node dell'evento e applicare rule 8 di EDRA
    IPaddr originNode;      /**< nodo che ha immesso la query nell'overlay */
    KEYID originKey;        /**< key dell'origin node */
};

/**
@note custom message for HPC Lab Extension
*/
struct D1HT_QUERY_REPLY_body {
    int subq_n;                 /**< subquery number ??? */
    char uqi[MAXQUERYLEN];      /**< uqi della query */
    char response[MAXREPLYLEN]; /**< risultato della query */
};




struct D1HT_JOIN_REPLY_body_part1 {
    IPaddr succ;
    IPaddr next;
    KEYID id;
    bool busy_or_err;
};

struct D1HT_JOIN_REPLY_body_part2 {
    NUM_32 NMAX;
    NUM_32 Npeers;
    NUM_32 f;
    NUM_32 bucketsize;      /**< bucketize is sent just for checking */
    NUM_32 e_rate;          /**< event rate */
    NUM_32 i_srtt_msg;      /**< rttvar of maint msgs in usecs */
    NUM_32 i_srtt_lookup;   /**< rttcar of lookup msgs in usecs */
    NUM_32 i_rttvar_msg;    /**< rtt of maint msgs in usecs */
    NUM_32 i_rttvar_lookup; /**< rtt of lookup msgs in usecs */
    PORTNO default_port;    /**< default port is sent just for checking */
    KEYID systemid;         /**< bucketize is sent just for checking */
};

struct D1HT_JOIN_STOP_TTL_body {
    NUM_8 ttl;
};

struct D1HT_STAB_body {
    IPaddr pred_IP;
    IPaddr pre2_IP;
    IPaddr succ_IP;
    IPaddr suc2_IP;
};

struct D1HT_SUCC_body {
    IPaddr peer;
    int32_t count;
};

struct D1HT_LOOKUP_EXTERN_body {
    NUM_32 payload_len;
    NUM_32 max_reply_payload_len;
    char key[D1HT_MAX_KEY_SIZE];
};

struct D1HT_LOOKUP_body {
    KEYID id;
    NUM_32 payload_len;
    NUM_32 max_reply_payload_len;
    IPaddr prev_target;
    bool alive; // whether prev_target was alive or not
};

struct D1HT_LOOKUP_reply_body {
    NUM_32 reply_payload_len;
    IPaddr home;
    unsigned char hops;
};

struct D1HT_STAT_body {
    int new_savg;
};

/** Macros for sending and receiving Msgs */



/** GET_SENDER, di un socket connesso mi restituisce ip e porta del nodo remoto
 */
#define GET_SENDER(connection, XEQ_FLAG)	({																			\
		socklen_t 			address_length;																				\
		struct sockaddr_in 	socket_address;																				\
		address_length 	= sizeof(socket_address);																		\
		int _retcode;																									\
		XEQ_EXEC((_retcode = getpeername (connection, (struct sockaddr *) &socket_address, &address_length)), XEQ_FLAG);\
		IPaddr sender;																									\
		sender.port = socket_address.sin_port;																			\
		sender.ip 	= socket_address.sin_addr;																			\
		if (_retcode != 0) sender.port = 0;																				\
		sender;																											\
	})

/** SEND_BODY invia un messaggio su di un socket (connesso)
 */
#define SEND_BODY( connection, body, XEQ_FLAG )	({																		\
		ssize_t			_nbytes;																						\
  		XEQ_EXEC((_nbytes = writen(connection, (const void*) &body, sizeof(body))), XEQ_FLAG);							\
		int errsave = errno;																							\
		if (_nbytes != sizeof(body) && _nbytes != -1) {																	\
			D1HT_ERROR_function("SEND_BODY", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);							\
			_nbytes = -1;																								\
		}																												\
		errno = errsave;																								\
		_nbytes;																										\
	})

/** SEND_HEADER_SEQNO invia un D1HT message header su di un socket (connesso) , specificando tipo di messaggio e Seq Number
 */
#define SEND_HEADER_SEQNO( connection, typeX, SEQNO, XEQ_FLAG )	({														\
		ssize_t			_nbytes;																						\
		struct msg_hd 	_msg_header;																					\
  		_msg_header.type		= typeX;																				\
  		_msg_header.SeqNo		= SEQNO;																				\
  		_msg_header.sender_port	= G_parms.myIP.port;																	\
  		_msg_header.systemid	= G_parms.systemid;																		\
  		XEQ_EXEC((_nbytes = writen(connection, (const void*) &_msg_header, D1HT_MSG_HEADER_size)), XEQ_FLAG);			\
		int errsave = errno;																							\
		if (_nbytes != D1HT_MSG_HEADER_size && _nbytes != -1) {															\
			D1HT_ERROR_function("SEND_HEADER_SEQNO", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);					\
			_nbytes = -1;																								\
		}																												\
		errno = errsave;																								\
		_nbytes;																										\
	})

/** SEND_HEADER invoca la macro *_SEQNO con SeqNo pari a 1
 */
#define SEND_HEADER( connection, typeX, XEQ_FLAG )	SEND_HEADER_SEQNO( connection, typeX, 1, XEQ_FLAG )

#define SEND_MSG_SEQNO( connection, typeX, SEQNO, bodyX, XEQ_FLAG )	({													\
    	if (G_parms.check && !G_started && !G_exiting && typeX < D1HT_JOIN_FINISHED) {									\
			VERBO(stderr, "ATTENTION Cause: sending msg type=%u before started\n", typeX);							 	\
		}																												\
		struct 	msg_hd 	_msg_header;																					\
		struct 	iovec 	iov[2];																							\
		int 	iovcnt 	= 2; 																							\
		ssize_t	_nbytes;																								\
		iov[0].iov_base	= (void *) &_msg_header;																		\
		iov[0].iov_len	= sizeof(_msg_header);																			\
		iov[1].iov_base	= (void *) &bodyX;																				\
		iov[1].iov_len	= sizeof(bodyX);																				\
		_msg_header.type		= typeX;																				\
		_msg_header.SeqNo		= SEQNO;																				\
		_msg_header.sender_port	= G_parms.myIP.port;																	\
  		_msg_header.systemid	= G_parms.systemid;																		\
		XEQ_EXEC(_nbytes = writev( connection, (iovec *) &iov, iovcnt), XEQ_FLAG);										\
		int errsave = errno;																							\
 		if (_nbytes != sizeof(bodyX)+sizeof(_msg_header) && _nbytes != -1) {											\
			D1HT_ERROR_function("SEND_MSG_SEQNO", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);						\
			_nbytes = -1;																								\
		}																												\
		errno = errsave;																								\
		_nbytes;																										\
	})

/** SEND_MSG invoca la macro *_SEQNO con SeqNo pari a 1
 */
#define SEND_MSG( connection, typeX, bodyX, XEQ_FLAG )	SEND_MSG_SEQNO( connection, typeX, 1, bodyX, XEQ_FLAG )

/** RECV_MSG usa la recvmsg() BSD-call per ricevere un messaggio composto da un header e da un body
 */
#define RECV_MSG( connection, typeX, SEQNO, bodyX, bodyXsize, XEQ_FLAG )	({											\
		XEQPOINT_LEVEL1;																								\
		struct 	sockaddr_in	sender_address;																				\
		struct 	msg_hd 	_msg_header;																					\
		struct 	msghdr	msg;	BZERO(msg);																				\
		struct 	cmsghdr	cmsg;	BZERO(cmsg);																			\
		struct 	iovec 	iov[2];																							\
		msg.msg_name	= (void *) &sender_address;																		\
		msg.msg_namelen	= sizeof(sender_address);																		\
		msg.msg_iov		= iov;																							\
		msg.msg_iovlen	= 2;																							\
		msg.msg_control	= &cmsg;																						\
		msg.msg_controllen= sizeof(cmsg);																				\
		msg.msg_flags	= 0;																							\
		iov[0].iov_base	= (void *) &_msg_header;																		\
		iov[0].iov_len	= sizeof(_msg_header);																			\
		iov[1].iov_base	= (void *) bodyX;																				\
		iov[1].iov_len	= bodyXsize;																					\
		ssize_t	 _nbytes = recvmsg( connection, &msg, 0);																\
		int errsave = errno;																							\
		if ((_nbytes 				!= bodyXsize+sizeof(_msg_header)) ||												\
			(_msg_header.type		!= typeX) ||																		\
			(_msg_header.systemid	!= G_parms.systemid && _msg_header.systemid	!= 0 && G_parms.systemid != 0 ) || 		\
			((_msg_header.SeqNo		!= SEQNO) && (SEQNO != 0)) ) {														\
			if (XEQ_FLAG != XEQ_SILENT) {																				\
				char msgerr[160];																						\
				sprintf(msgerr, "errmsg=%s, sender_port=%d, type=(%u,%u), SeqNo=(%u,%u), systemid=(%u,%u), nbytes=(%d,%d)\n",	\
						strerror(errsave), _msg_header.sender_port, typeX, _msg_header.type, SEQNO, _msg_header.SeqNo, 			\
						G_parms.systemid, (uint)_msg_header.systemid, (int) (bodyXsize+sizeof(_msg_header)), (int) _nbytes);	\
				D1HT_ERROR_function("RECV_MSG", msgerr, XEQ_FLAG, __FILE__, __LINE__);									\
				if (!G_parms.VERBOSE && (_nbytes == bodyXsize+sizeof(_msg_header)) && 									\
					_msg_header.systemid != G_parms.systemid && _msg_header.systemid != 0 && G_parms.systemid != 0) {	\
					VERBO(stderr, "ATTENTION Cause: systemid mismatch %s\n", msgerr);									\
				}																										\
			}																											\
			_nbytes = -1;																								\
		}																												\
		errno = errsave;																								\
		_nbytes;																										\
	})

/** READ_HEADER effettua la lettura da un socket connesso di un message header
 */
#define READ_HEADER( connection, XEQ_FLAG )	({																			\
		ssize_t			_nbytes;																						\
		struct msg_hd 	_msg_header;																					\
		XEQ_EXEC((_nbytes = readn(connection, &_msg_header, D1HT_MSG_HEADER_size)), XEQ_FLAG);							\
		int errsave = errno;																							\
  		if ((_nbytes 				!= D1HT_MSG_HEADER_size) ||															\
  			(_msg_header.systemid	!= G_parms.systemid && _msg_header.systemid	!= 0 && G_parms.systemid != 0 ) ) {		\
	  		if (_nbytes != -1) {																						\
				if ( G_parms.VERBOSE) {	/*if(_nbytes==-1) XEQ_EXEC would have already printed the errror msg */			\
					char msgerr[160];																					\
					if (_nbytes != D1HT_MSG_HEADER_size) {																\
						sprintf(msgerr, "Incomplete header sender_port=%d, type=%u, SeqNo=%u, nbytes=(%d,%d)\n",		\
								ntohs(_msg_header.sender_port), _msg_header.type, _msg_header.SeqNo, 				 	\
								(int) D1HT_MSG_HEADER_size, (int) _nbytes);												\
					} else {																							\
						sprintf(msgerr, "System id mismatch sender_port=%d, type=%u, SeqNo=%u, systemid=(%u,%u)\n",		\
						ntohs(_msg_header.sender_port), _msg_header.type, _msg_header.SeqNo, G_parms.systemid, 			\
						_msg_header.systemid);																			\
					} 																									\
					D1HT_ERROR_function("READ_HEADER", msgerr, XEQ_FLAG, __FILE__, __LINE__);							\
				}																										\
				if (errsave == EAGAIN || errsave == EINTR || errsave == EWOULDBLOCK) errsave = ENODATA;					\
	  		}																											\
 			_msg_header.type = D1HT_INVALID;																			\
  		}																												\
		errno = errsave;																								\
  		_msg_header;																									\
	})

/** READ_HEADER_TYPE oltre a ricevere su un socket connesso un message header, verifica che sia del tipo tyepX fornito in input
 */
#define READ_HEADER_TYPE( connection, typeX, XEQ_FLAG )	({																\
		msg_hd _msg_header = READ_HEADER( connection, XEQ_FLAG );														\
		int errsave = errno;																							\
		if (typeX && _msg_header.type != typeX && _msg_header.type != D1HT_INVALID) {                              		\
			char msgerr[160];																							\
			sprintf(msgerr, "Read header failed. Header type is %u but it should be "#typeX, _msg_header.type);         \
			D1HT_ERROR_function("READ_HEADER_TYPE", msgerr, XEQ_FLAG, __FILE__, __LINE__);								\
			_msg_header.type = D1HT_INVALID;																			\
			errno = errsave;																							\
		}                                                                                                               \
		errno = errsave;																								\
		_msg_header;																									\
	})

/**
 READ_BODY legge un message body da un socket connesso */
#define READ_BODY( connection, msg_body, XEQ_FLAG )	({																	\
		ssize_t	_nbytes;																								\
		XEQ_EXEC(_nbytes = readn(connection, &msg_body, sizeof(msg_body)), XEQ_FLAG);									\
		int errsave = errno;																							\
 		if (_nbytes != sizeof(msg_body) && _nbytes != -1) {																\
			D1HT_ERROR_function("READ_BODY", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);							\
			errsave = ENODATA;																							\
			_nbytes = -1;																								\
		}																												\
		errno = errsave;																								\
		_nbytes;																										\
	})

/**
 * legge un messaggio D1HT, prelevando dapprima l'header, del tipo specificato, quindi leggendo il body
 */
#define READ_MSG( connection, typeX, msg_body, XEQ_FLAG )	({															\
		msg_hd _msg_header = READ_HEADER_TYPE( connection, typeX, XEQ_FLAG );											\
		int errsave = errno; 																							\
		ssize_t	_retcode = -1;																							\
		if (_msg_header.type != D1HT_INVALID) {                              											\
			_retcode = READ_BODY( connection, msg_body, XEQ_FLAG );														\
			errsave = errno; 																							\
		}																												\
		errno = errsave;																								\
		_retcode;																										\
	})


/** apertura di un socket */
#define	SOCKET( connection, TYPE, PROTOCOL, mys_addr, myport, XEQ_FLAG)	({													\
		XEQ_EXEC( (connection = socket(AF_INET, TYPE, PROTOCOL)), XEQ_FLAG); 												\
		int errsave = errno; 																								\
		if (connection >= 0) {																								\
			int ret;																										\
			int turn_on;																									\
			if (PROTOCOL == IPPROTO_TCP) {																					\
				turn_on = 1;																								\
				XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_REUSEADDR, &turn_on, sizeof(turn_on)), XEQ_WARNING);		\
				turn_on = 1;																								\
				XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_KEEPALIVE, &turn_on, sizeof(turn_on)), XEQ_WARNING);		\
				turn_on = 1;																								\
				XEQ_EXEC( setsockopt(connection, IPPROTO_TCP, TCP_NODELAY, &turn_on, sizeof(turn_on)), XEQ_WARNING);		\
				int buffer_size; socklen_t buflen = sizeof(buffer_size); 													\
				XEQ_EXEC( getsockopt(connection, SOL_SOCKET, SO_RCVBUF, &buffer_size, &buflen), XEQ_FLAG);					\
				if (D1HT_TCP_BUFFER_SIZE > buffer_size) {																	\
					buffer_size = D1HT_TCP_BUFFER_SIZE;																		\
					XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)),XEQ_WARNING);\
					buffer_size = D1HT_TCP_BUFFER_SIZE;																		\
					XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size)),XEQ_WARNING);\
				}																											\
			} else if (PROTOCOL == IPPROTO_UDP) {																			\
				int buffer_size; socklen_t buflen = sizeof(buffer_size); 													\
				XEQ_EXEC( getsockopt(connection, SOL_SOCKET, SO_RCVBUF, &buffer_size, &buflen), XEQ_FLAG);					\
				if (D1HT_UDP_BUFFER_SIZE > buffer_size) {																	\
					buffer_size = D1HT_UDP_BUFFER_SIZE;																		\
					XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)),XEQ_WARNING);\
					buffer_size = D1HT_UDP_BUFFER_SIZE;																		\
					XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size)),XEQ_WARNING);\
				}																											\
			} else {																										\
				EXIT("SOCKET Invalid protocol: should be IPPROTO_UDP or IPPROTO_TCP");										\
			}																												\
	 	    struct sockaddr_in socket_address;																				\
			turn_on = 1;																									\
			XEQ_EXEC( setsockopt(connection, SOL_SOCKET, SO_DONTROUTE, &turn_on, sizeof(turn_on)), XEQ_WARNING);			\
			BZERO(socket_address);																							\
			socket_address.sin_family 		= AF_INET;																		\
			socket_address.sin_port 		= myport;																		\
			socket_address.sin_addr.s_addr 	= mys_addr; 																	\
			XEQ_EXEC( ret = bind(connection, (struct sockaddr *) &socket_address, sizeof(socket_address)), XEQ_FLAG);		\
			errsave = errno; 																								\
			if (ret < 0) { D1HT_CLOSE(connection); connection = ret; }														\
		}																													\
		errno = errsave;																									\
		connection;																											\
	})


/** macro for select() */
#define	SELECT_CONNECTION(connection, timeo_secs, XEQ_FLAG)	({																\
		int errsave = EBADF; 																								\
		int	ret 	= -1;																									\
		if (connection >= 0) {																								\
			fd_set	rset, eset;																								\
			struct timeval tval;																							\
			FD_ZERO(&rset);																									\
			FD_SET(connection, &rset);																						\
			eset = rset;																									\
	 		tval.tv_sec = timeo_secs;																						\
	 		tval.tv_usec= 0;																								\
	 		XEQ_EXEC((ret = select(connection+1, &rset, NULL, &eset, &tval)), XEQ_FLAG);									\
	 		errsave = errno;																								\
	 		if (ret == -1 && errsave == EINTR) {																			\
	 			ret 	= 0; 																								\
	 			errsave = 0; 																								\
	 		}																												\
	 	}																													\
		errno = errsave;																									\
		ret;																												\
	})

/** non-blocking connect */
#define	CONNECTION_TIMEO(connection, TYPE, PROTOCOL, mys_addr, myport, peerIP, timeo_secs, XEQ_FLAG)	({					\
 	    VERBOSEX( stderr, "Trying %s connection to %s:%u, timeout=%d secs\n",												\
				#PROTOCOL, NTOP(peerIP.ip), ntohs(peerIP.port), (int) timeo_secs ); 										\
		SOCKET( connection, TYPE, PROTOCOL, mys_addr, myport, XEQ_FLAG);													\
		int errsave = errno; 																								\
		int	ret 	= connection;																							\
		if (connection >= 0) {																								\
	 	    struct sockaddr_in socket_address;																				\
			BZERO(socket_address);																							\
			socket_address.sin_family 	= AF_INET;																			\
			socket_address.sin_port 	= peerIP.port;																		\
			socket_address.sin_addr 	= peerIP.ip; 																		\
			int cflags = fcntl(connection, F_GETFL, 0);																		\
			XEQ_EXEC( (ret = fcntl(connection, F_SETFL, cflags | O_NONBLOCK)), XEQ_FLAG);									\
			errsave = EBADF;																								\
			if (ret != -1) {																								\
	 			XEQ_EXEC((ret=connect(connection,(struct sockaddr *) &socket_address, sizeof(socket_address))), XEQ_SILENT);\
	 			errsave = errno;																							\
	 		}																												\
	 		if (errsave == ETIMEDOUT || errsave == EINTR) errsave = EINPROGRESS;											\
	 		if (ret < 0 && errsave == EINPROGRESS && timeo_secs > 0) {														\
	 			XEQ_EXEC((ret = fcntl(connection, F_SETFL, cflags)), XEQ_FLAG);												\
		 		errsave = errno;																							\
				if (ret != -1) {																							\
					fd_set	rset, wset, eset;																				\
					struct timeval tval;																					\
					FD_ZERO(&rset);																							\
					FD_SET(connection, &rset);																				\
					wset = rset;																							\
					eset = rset;																							\
		 			tval.tv_sec = timeo_secs;																				\
		 			tval.tv_usec= 0;																						\
		 			XEQ_EXEC((ret = select(connection+1, &rset, &wset, &eset, &tval)), XEQ_FLAG);							\
		 			errsave = errno;																						\
		 			if (ret == 0) {																							\
						ret 	= -1;																						\
						errsave = EINPROGRESS;																				\
					} else if (FD_ISSET(connection, &eset)) {																\
		 				ret 	= -1; 																						\
						errsave = EBADF; 																					\
					} else if (ret == -1 && errno == EINTR) {																\
						errsave = EINPROGRESS;																				\
		 			} else if (ret >= 1) {																					\
		 				socklen_t errlen = sizeof(errsave); 																\
						XEQ_EXEC( getsockopt(connection, SOL_SOCKET, SO_ERROR, &errsave, &errlen), XEQ_FLAG);				\
						if (errsave != 0) {																					\
							ret = -1;																						\
						} else {																							\
							ret = 0;																						\
						}																									\
					} 																										\
				}																											\
	 		} else fcntl(connection, F_SETFL, cflags);																		\
			if (ret < 0 && errsave != EINPROGRESS && errsave != EINTR && errsave != ETIMEDOUT) { 							\
				D1HT_CLOSE(connection); 																					\
				connection 	= -1;																							\
				ret 		= -1;																							\
				D1HT_ERROR_function("CONNECTION_TIMEO", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);					\
		   		if (XEQ_FLAG == XEQ_ERROR) EXIT("CONNECTION_TIMEO failed - %s", strerror(errsave));							\
			} else if (ret < 0) errsave = EINPROGRESS;																		\
		}																													\
		errno = errsave;																									\
		ret;																												\
	})

/** connect */
#define	CONNECTION( connection, TYPE, PROTOCOL, myip, myport, peerIP, XEQ_FLAG)	({													\
		int ret = CONNECTION_TIMEO( connection, TYPE, PROTOCOL, myip.s_addr, myport, peerIP, G_parms.TCP_con_timeo, XEQ_FLAG);		\
		int errsave = errno; 																										\
		if (ret != 0 && connection >= 0) {																							\
			D1HT_CLOSE(connection); 																								\
			connection 	= -1;																										\
			ret 		= -1;																										\
			D1HT_ERROR_function("CONNECTION failed", strerror(errsave), XEQ_FLAG, __FILE__, __LINE__);								\
			if (XEQ_FLAG == XEQ_ERROR) EXIT("CONNECTION failed - %s", strerror(errsave));											\
		}																															\
		errno = errsave;																											\
		ret;																														\
	})

/** apertura di socket TCP locale */
#define	TCP_SOCKET( connection, myport, XEQ_FLAG) 	SOCKET( connection, SOCK_STREAM,IPPROTO_TCP, G_parms.myIP.ip.s_addr, myport, XEQ_FLAG)
/** apertura di socket UDP locale */
#define	UDP_SOCKET( connection, myport, XEQ_FLAG)	SOCKET( connection, SOCK_DGRAM, IPPROTO_UDP, G_parms.myIP.ip.s_addr, myport, XEQ_FLAG)
/** apertura di connessione TCP verso peer */
#define	TCP_CONNECTION( connection, myport, peerIP, XEQ_FLAG) CONNECTION( connection, SOCK_STREAM, 	IPPROTO_TCP, G_parms.myIP.ip, myport, peerIP, XEQ_FLAG)
/** apertura di connessione UDP verso peer */
#define	UDP_CONNECTION( connection, myport, peerIP, XEQ_FLAG) CONNECTION( connection, SOCK_DGRAM, 	IPPROTO_UDP, G_parms.myIP.ip, myport, peerIP, XEQ_FLAG)

#ifdef DEBUG
//non usata
class msgsent_t {
    struct msg_hd msg_header;
    struct D1HT_MAINTENANCE_body msg_body;
    IP *ips[2][2];
    PORTNO *ports[2];
};
#endif

#endif /*MESSAGES_H_*/
