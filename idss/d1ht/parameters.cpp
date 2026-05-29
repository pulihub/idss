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
 * @file parameters.c
 * @brief 
 */

#define XXXXX {fprintf(stderr,"program=%s, FILE=%s, LINE=%d, IP=%s:%u, hostname=%s\n", argv[0], __FILE__, __LINE__, IPtoP(myIP), my_host_name); fflush(stderr); }

#include "D1HT.h"
#include <getopt.h>
#include <unistd.h>

using namespace std;

namespace d1ht
{
parameters	G_parms;

static const struct option long_options[] = {

  { "local_address",    1, NULL, 'a' },
  { "peer_address",     1, NULL, 'A' },
  { "wait_SIGCONT",     0, NULL, 'C' },
  { "routing_table",    2, NULL, 'e' },
  { "use_etc_IP",    	0, NULL, 'E' },
  { "Daemonize",        0, NULL, 'D' },
  { "failures",         1, NULL, 'f' },
  { "look_for_ports",   1, NULL, 'F' },
  { "help",             0, NULL, 'h' },
  { "display_hostname", 0, NULL, 'H' },
  { "display_IPaddress",0, NULL, 'I' },
  { "infiniband	",		1, NULL, 'i' },
  { "delayed_acks",     0, NULL, 'k' },
  { "Known_hosts",      1, NULL, 'K' },
  { "NMAX",       		1, NULL, 'N' },
  { "outfile",          0, NULL, 'o' },
  { "local_output",     0, NULL, 'l' },
  { "local_port",       1, NULL, 'p' },
  { "retries",        	1, NULL, 'r' },
  { "RTOMIN",      	 	1, NULL, 'R' },
  { "systemid",    	 	1, NULL, 's' },
  { "verbose",          0, NULL, 'v' },
  { "VERBOSE",          0, NULL, 'V' },
#if defined(DEBUG)
  { "check",   			0, NULL, 'c' },
  { "Self_Send_Msgs",   0, NULL, 'M' },
  { "test",   			0, NULL, 'T' },
#else
  { "no_dump_stats",   	0, NULL, 'd' },
  { "stats",          	0, NULL, 'S' },
#endif
  { "dummy",    		0, NULL, 'U' },
  { NULL,   			0, NULL,  0  }
};
#if defined(DEBUG)
static const char* const short_options = "la:p:A:N:f:K:R:r:s:F:i:cCTEeohdkvSVMDIH";
#else
static const char* const short_options = "la:p:A:N:f:K:R:r:s:F:i:cCdEeohkvSVDIH";
#endif

static const char* const usage_template = 
  "\nUsage: %s -a local_address  { -A peer_address -P peer_port | -p local_port -N Nmax [-f f] } [ options ]\n\n"
  "  -a, --local_address IP     	Local IP address.\n"
  "  -p, --local_port PORT      	Local IP port.\n"
  "  -A, --peer_address ip      	IP address of any peer already in the system.\n"
  "  -N, --NMAX Nmax            	Maximum number of peers in the system (it must be a power of 2).\n"
  "  -C, --wait_SIGCONT             If -e (or --routing_table) is specified, -C forces the peer to wait for a SIGCONT signal\n"
  "  -D, --Daemonize            	Daemonize the process (includes closing stdin, stdout and stderr).\n"  
  "  -e, --routing_table=file   	File with the IP addresses of the initial routing table.\n"
  "  -E, --use_etc_IP   			Read the IP address (without port number) from /etc/D1HT.IPaddr.\n"
  "  -f, --failures             	Acceptable fraction of lookups solved with more than on hop, in %% (should be an integer) Default=1%%\n"
  "  -F  --look_for_ports ntries  	Tries ntries distinct UDP & TCP ports\n"
  "  -h, --help                 	Print this information.\n"
  "  -H, --display_hostname     	Use hostname instead of IP address when creating outfiles and displaying some other information.\n"
  "  -I, --display_IPaddress     	Use IP address instead of hostname when creating outfiles and displaying some other information.\n"
  "  -i, --infiniband     			Supply an IB suffix to be added to the hostname.\n"
  "  -k, --delayed_acks         	UDP delayed acks.\n"
  "  -K, --Known_hosts filename 	Specifies file with IP address of known peers for bootstrap\n"
  "  -l, --local_file				if -o is specified, directs the stderr file to /tmp.\n"
  "  -r, --retries					UDP msg retries.\n"
  "  -R, --RTOMIN					UDP msg min timeout.\n"
  "  -o, --outfile					Send out and err messages to files.\n"
  "  -s, --systemid sid 			Set the system id.\n"
#if defined(DEBUG)
  "  -c, --check       				Perform debug cheks.\n"
  "  -M, --Self_Send_Msgs       	Self send maintenance messages to check their integrity.\n"
  "  -T, --test       				Tests the routing table routines.\n"
  "  -V, --VERBOSE              	Print LOTS of verbose messages.\n"
#else 
  "  -d, --no_dump_stats           	Dont dump statistics when leaving\n"
  "  -S, --stats       				Print statistics when leaving.\n"
#endif
  "  -v, --verbose              	Print verbose messages.\n";
   
void print_usage(const int is_error, const int argc, char** argv)
{
#if defined(BWR4)
  if (is_error) {
	  fprintf(stderr, "\n Parameters used:");
	  for (int i=0; i<argc; i++) fprintf(stderr, "%s ", argv[i]);
	  fprintf(stderr, "\nusage ERROR\n");
  } else {
	  fprintf(stderr, "\nusage ERROR\n");
  }
#else	
  if (is_error) {
	  fprintf(stderr, "\n Parameters used:");
	  for (int i=0; i<argc; i++) fprintf(stderr, "%s ", argv[i]);
	  fprintf(stderr, usage_template, G_parms.program_name);
  } else {
	  fprintf(stderr, "\nusage ERROR\n");
	  fprintf(stderr, usage_template, G_parms.program_name);
  }
#endif
  exit (is_error ? 1 : 0);
}

int parameters::get(int argc, char** argv)
{
  int 		next_option;
  bool		specified_f = false;
  char		temp_display_name[160];

  parameters_init();  // reset the parameters

//  for (int i=0; i<argc; i++) fprintf(stderr, "argv[%u]=%s\n", i, argv[i]);

  gettimeofday(&program_start, NULL); // store when we started
  struct tm	*ptm;
  ptm = localtime(&program_start.tv_sec);	
  strftime(start_time_string, sizeof(start_time_string), D1HT_TIME_FORMAT, ptm);

  Assert(((default_udp_port != 0) && (default_udp_port != ntohs(0))), XEQ_ERROR, "Invalid default UDP port");

  bool systemid_is_portno = false;
  optind = 0;
  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL); 
    
//   if (next_option != -1 ) fprintf(stderr,"next_option=%s optarg=%s optind=%d optopt=%d opterr=%d\n", &next_option, optarg, optind, optopt, opterr );
     
    switch (next_option) {
      	
    case 'a': 
      { 
    	Assert((myIP.ip.s_addr == htonl(INADDR_ANY)), XEQ_WARNING, "my IP address (-a) should be specified only once");

    	PORTNO port = myIP.port;
    	
    	get_IPaddr(optarg, &myIP);
    	
    	if (myIP.port == 0) {
    		myIP.port = port;
    		sprintf(temp_display_name, "%s", optarg);
    	} else {
    		Assert((port==0 || myIP.port==port), XEQ_ERROR, "conflicting (-p and -a) port specifications\n");
    		char    buffer[80];
    		char	*match1;
    				
    		strncpy(buffer, optarg, sizeof(buffer));

    		if ((match1 = strstr( buffer, ":" )) != NULL) match1[0] = '\0';	

      	  	sprintf(temp_display_name, "%s", buffer);
    	}
      }
      break;      
  
    case 'A':  
      { 
    	IPaddr known_peer;
     	
     	get_IPaddr(optarg, &known_peer);

     	if (known_peer.port == ntohs(0) || known_peer.port == 0) known_peer.port = default_udp_port;

     	G_known_peers.push(known_peer);
      }
      break;
      
    case 'C':  
    	wait_CONT = true;
    	PTXEQ(pthread_sigmask(SIG_BLOCK, NULL, &sigmask_original), XEQ_ERROR);
  		break;
      
    case 'D':  
    	daemonize = true;
  		break;
 
    case 'e': 
       	RT_file = optarg;
    	input_RT = true;
    	break;

    case 'E': 
     	etchost = true;
     	break;

    case 'f': 
      {
    	char* end;

		f = strtol (optarg, &end, 10);
		if (*end != '\0') {
		  	VERBO(stderr, "\nERROR Cause: -f (or --failures) should only contain digits\n");
			print_usage(1, argc, argv);  /* The user specified non-digits.  */
		}
		specified_f = true;
		if (f==0) { 
		  	VERBO(stderr, "\nERROR Cause: -f (or --failures) should be greater than 0.\n");
  			print_usage(1, argc, argv);
  		}	
      }
      break;

    case 'F':
	    {
			int value;
			char* end;
			value = strtol (optarg, &end, 10);
			if (*end != '\0') {
			  	VERBO(stderr, "\nERROR Cause: -F should only contain digits\n");
				print_usage(1, argc, argv);  /* The user specified non-digits.  */
			}
			look_for_ports = value;
	    }
	    break;
      
    case 'H':  
    	display_hostname = true;
    	display_IPaddress= false;
  		break;
        
    case 'I':  
    	display_hostname = false;
    	display_IPaddress= true;
  		break;

    case 'i':
    	infiniband 	= true;
    	IB_suffix 	= optarg;
  		break;

    case 'k':  
    	delayed_acks = true;
  		break;
  	     
    case 'K':
     {
    	ifstream 	input;

    	input.open(optarg);
    	if (!input) EXIT("Unable to open the file with the IP addresses of the known peers (-K) (%s).", optarg);

    	while (true) {
        	IPaddr 	known_peer;
    		char	buffer[200];
    		
    		input.getline(buffer, sizeof(buffer));	
    		if (input.eof()) break;

    		get_IPaddr(buffer, &known_peer);
    		G_known_peers.push(known_peer);
    	}
     }
     break;

    case 'l':  
    	local_output = true;
  		break;
  	     
     case 'N': 
      {
		int value; char* end;
		
		value = strtol (optarg, &end, 10);
		if (*end != '\0') print_usage(1, argc, argv);  /* The user specified non-digits */
		if ((int) ceilf(log2((float) value)) != (int) floor(log2((float) value))) { 
		  	fprintf(stderr, "\nERROR Cause: Nmax should be a power of 2!\n");
  			print_usage(1, argc, argv);
  		}	
				
		NMAX 	= value;

		if (NMAX != 0 && NMAX < (int) exp2((float) ceilf(log2((float) BUCKETSIZE))) ) {
			NMAX = (int) exp2((float) ceilf(log2((float) BUCKETSIZE)));
		}

      }
      break;

    case 'o':  
      outfile = true;
      break;
      
    case 'p':
      {
		int value; char* end;

		value = strtol (optarg, &end, 10);
		if (*end != '\0') {
		  	VERBO(stderr, "\nERROR Cause: -p should only contain digits\n");
			print_usage(1, argc, argv);  /* The user specified non-digits.  */
		}
		/* The port number needs to be converted to network (big endian) byte order.  */
		myIP.port = (PORTNO) htons(value);
      }
      break;
    
    case 'r':
     {
		int value; char* end;
		
    	value = strtol (optarg, &end, 10);
		if (*end != '\0') {
		  	VERBO(stderr, "\nERROR Cause: -r should only contain digits\n");
			print_usage(1, argc, argv);  /* The user specified non-digits.  */
		}
		retries = value;
		if (f==0) { 
		  	fprintf(stderr, "\nERROR Cause: -r (or --retries) should be greater than 0.\n");
  			print_usage(1, argc, argv);
  		}	
      }
      break;
      
    case 'R':
     {
		int value; char* end;
		
    	value = strtol (optarg, &end, 10);
		if (*end != '\0') {
		  	VERBO(stderr, "\nERROR Cause: -R should only contain digits\n");
			print_usage(1, argc, argv);  /* The user specified non-digits.  */
		}
		RTOMIN = (float) value;
		if (RTOMIN==0.0) { 
		  	fprintf(stderr, "\nERROR Cause: -R (or --RTO) should be greater than 0.\n");
  			print_usage(1, argc, argv);
  		}	
      }
      break;
      
    case 's':
     {
		int value; char* end;
		
    	value = strtol (optarg, &end, 10);
		if (*end != '\0') {
		  	VERBO(stderr, "\nERROR Cause: -s should only contain digits\n");
			print_usage(1, argc, argv);  /* The user specified non-digits.  */
		}
		systemid = value;
		
		if (systemid == 0 ) {
			systemid_is_portno = true;
		} else {
			systemid_is_portno = false;
		}
      }
      break;
   	
    case 'U':
      // dummy parameter: do nothing
      break;      
 
    case 'v':  
      verbose = true;
      break;
      
    case 'V':  
       verbose = true;
#if defined(DEBUG)
       VERBOSE = true;
#endif
       break;

#if defined(DEBUG)
   case 'c':  
     check = true;
     break;

   case 'M':  
     selfmsgs = true;
     break;

   case 'T':  
     test = true;
     break;

   case 'd':
	  VERBO(stderr, "WARNING Cause: parameter -d has no effect with DEBUG flag on\n");
      break;

   case 'S':
     VERBO(stderr, "WARNING Cause: parameter -S has no effect with DEBUG flag on\n");
     break;

#else
   case 'd':  
      dump_stats = false;
      break;
      
   case 'S':  
     print_stats = true;
     break;
     
   case 'c':  
	 VERBO(stderr, "WARNING Cause: parameter -c has no effect if D1HT was not compiled with DEBUG flag on\n");
     break;
#endif

    case '?':  
      print_usage(1, argc, argv);

    case 'h':  
      print_usage(0, argc, argv);

    case -1:  
      break;

    default:
      VERBO(stderr,"Unknown %d parameter=%s\n", next_option, optarg );
      print_usage(1, argc, argv);
    }
  } while (next_option != -1);
  
  /* This program takes no additional arguments. Issue an error if the user specified any.  */
  if (optind != argc) {
	  VERBO(stderr,"Unknown parameter\n");
	  print_usage(1, argc, argv);
  }
  
#if ! defined(DEBUG)
	if (check || VERBOSE) {
  	VERBO(stderr, "\nERROR Cause: The -c (--check), -S (--Self_Send_Msgs) and -V (--VERBOSE) options are only available when D1HT is compiled with the -DEBUG flag!\n");
  	print_usage(1, argc, argv);
  }
#endif
	
  if (specified_f && G_known_peers.empty()) {
  	VERBO(stderr, "\nERROR Cause: You should not specify both -f and -A.\n");
  	print_usage(1, argc, argv);
  }
  
  if (NMAX==0 && (specified_f || input_RT)) {
  	VERBO(stderr, "\nERROR Cause: If you don't specify -N you may not specify -f or --routing_table or -e!\n");
  	print_usage(1, argc, argv);
  }

  if (G_parms.etchost) { 

	FILE*	input 	= fopen("/etc/D1HT.IPaddr", "r");
	int 	errsave = errno;

	if (input == NULL) EXIT("ERROR Cause: -E was specified but we could not open the file /etc/D1HT.IPaddr, msg=%s", strerror(errsave));
	
	get_myName(my_host_name, sizeof(my_host_name), &(myIP.ip));
	
	char local_my_host_name[D1HT_HOSTNAME_MAXLENGTH];
	BZERO(local_my_host_name);

	if (fread(local_my_host_name, 1, sizeof(local_my_host_name), input) <= 7) {
		errsave = errno;
		EXIT("ERROR Cause: -E was specified but we could not read the file /etc/D1HT.IPaddr, msg=%s", strerror(errsave));
	}

	fclose(input);

	char* _match = strstr( local_my_host_name, " " );

 	if (_match == NULL) EXIT("ERROR Cause: -E was specified but the first line in /etc/D1HT.IPaddr does not end with a blank space");

 	strncpy(_match, "\0", 1);

	get_IP(local_my_host_name, &(myIP.ip));

  } else if (myIP.ip.s_addr == htonl(INADDR_ANY)) {
	  get_myName(my_host_name, sizeof(my_host_name), &(myIP.ip));
	  sprintf(temp_display_name, "%s", my_host_name);
  } else {
	  get_Name(my_host_name, sizeof(my_host_name), myIP.ip);
  }
  
  if (!myIP.port) myIP.port = default_udp_port;
  
  original_port = myIP.port;
  
  if (look_for_ports) {
	  
	  unsigned short int try_port	= htons(ntohs(myIP.port)-1);
	  int		try_socket 	= 0;

	  for (int i=0; i<=look_for_ports; i++) { 
		  D1HT_CLOSE(try_socket);  
		  try_port = htons(ntohs(try_port)+1); 

		  UDP_SOCKET(try_socket, try_port, XEQ_SILENT);
		  if (try_socket < 0) continue;
		  D1HT_CLOSE(try_socket);
		  
		  TCP_SOCKET(try_socket, try_port, XEQ_SILENT);
		  if (try_socket >= 0) break;
	  }
	  D1HT_CLOSE(try_socket);
	  myIP.port = try_port;
	  
	  G_argv = CALLOC(argc+5, char*);
	  for (G_argc=0; argv[G_argc]!=NULL; G_argc++) {
		 G_argv[G_argc] = strdup(argv[G_argc]);
	  }
	  for (int i=0; i<2; i++) {
		  G_argv[G_argc] = G_additional_parms[i];
		  G_argc++;		  
	  }
	  G_argv[G_argc] = NULL; 
	  sprintf(G_additional_parms[0], "-F0");
	  sprintf(G_additional_parms[1], "-p%u", ntohs(try_port));

   } else {
	  G_argc = argc;
	  G_argv = argv;
  }
 
  G_myID = ip2id(myIP);

  if (G_parms.etchost) {
	  sprintf(display_name, D1HT_HOSTNAME_FORMAT, my_host_name, ntohs(myIP.port));
  } else if (display_hostname) {
	  sprintf(display_name, D1HT_HOSTNAME_FORMAT, my_host_name, ntohs(myIP.port));
  } else if (display_IPaddress) {
	  sprintf(display_name, D1HT_HOSTNAME_FORMAT, IPtoP(myIP));
  } else {
	  sprintf(display_name, D1HT_HOSTNAME_FORMAT, temp_display_name, ntohs(myIP.port));
  }

  if (systemid_is_portno) systemid = default_udp_port;

  Assert(((default_udp_port != 0) && (default_udp_port != ntohs(0))), XEQ_ERROR, "Invalid default UDP port");
    
  sprintf( base_filename, "%s%s%s", G_parms.program_name, G_parms.display_name, G_parms.start_time_string);

  return 0;
}

} // namespace d1ht
