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
 * @file parameters.h
 * @brief 
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

class parameters
{
public:

	IPaddr 		myIP, peer_address;
	int     	f, NMAX, rho_max;
	KEYID		systemid;
	char* 		program_name;
	char*		RT_file;
	char*		IB_suffix;
	char		display_name[D1HT_HOSTNAME_MAXLENGTH];
	char		my_host_name[D1HT_HOSTNAME_MAXLENGTH];
	char		start_time_string[20];
	char		base_filename[2*D1HT_HOSTNAME_MAXLENGTH];
	int			retries;
	int			look_for_ports;
	float		RTOMIN;
	int			TCP_con_timeo;
	bool		verbose, VERBOSE, daemonize, delayed_acks, outfile, check, selfmsgs, test, etchost;
	bool		print_stats, dump_stats, display_hostname, display_IPaddress, local_output, infiniband;
	PORTNO 		default_udp_port, original_port;
	bool		input_RT;	
	bool		wait_CONT;
    sigset_t	sigmask_original;

	struct timeval program_start;

	int	get(int argc, char** argv);

	void parameters_init() {
  		myIP.ip.s_addr			= htonl(INADDR_ANY);
		myIP.port				= 0;
		systemid				= 0;
		peer_address.ip.s_addr	= 0;
		peer_address.port		= 0;
		NMAX 					= 0;
		default_udp_port 		= (PORTNO) htons(D1HT_DEFAULT_PORT);
		look_for_ports			= 0;
		delayed_acks			= false;// If delayed_ack is true, at the end of each \Theta interval D1HT will send one ack for each UDP msg received
		verbose 				= false;// Don't print verbose messages.
		VERBOSE 				= false;// Don't print VERBOSE messages.   
		daemonize				= false;// Don't daemonize.
		outfile					= false;// Don't send out and err messages to files
		local_output			= false;// Don't put stderr in /tmp
		check					= false;// Don't perform debug checks
		test					= false;// Don't test the routing table algorithms
		selfmsgs				= false;// Don't send self messages
		print_stats				= false;// Don't print statistics
#if defined(DEBUG)
		dump_stats				= false;// Don't dump statistics if debugging
#else
		dump_stats				= true; // Dump statistics if not debugging
#endif
		infiniband				= false;// Don't use Infiniband
		etchost					= false;// Don't read the IP address from /etc/D1HT.IPaddr
		f						= 1;	// Acceptable fraction of lookups solved with more than on hop, Default=1%.
		RTOMIN					= (float) D1HT_RTT_RXTMIN; 	// UDP min msg timeout in milliseconds
		retries					= D1HT_MAX_RETRIES;	// number of UDP msg transmission retries
		input_RT				= false;
		display_hostname		= false;
		display_IPaddress		= false;
		wait_CONT				= false;
		RT_file					= NULL;
		TCP_con_timeo			= D1HT_TCP_TIMEO;   // TCP connection timeout in seconds
		program_start.tv_sec	= 0;
		program_start.tv_usec	= 0;
		IB_suffix				= NULL;
		
		strncpy(base_filename, 		"    ", 3);
		strncpy(start_time_string, 	"    ", 3);
		strncpy(my_host_name,	 	"    ", 3);
		strncpy(display_name, 		"    ", 3);
	}

	parameters() {
		parameters_init();
		program_name = NULL;
	}
};

extern 	parameters	G_parms;

#endif /*PARAMETERS_H_*/
