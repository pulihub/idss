/**
 * @file xeq_ports.c
 * @brief 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define EXIT { \
	fprintf(stderr, "The %s program should receive 2 positive integer parameters (first_port last_port), where first_port<last_port\n", argv[0]); \
	exit(-1); \
}

#define	SOCKET( connection, TYPE, PROTOCOL, myport)	{												\
	connection = socket(AF_INET, TYPE, PROTOCOL); 													\
	if (connection >= 0) {																			\
		int ret;																					\
 	    struct sockaddr_in socket_address;															\
		memset( (void *) &socket_address, 0, sizeof(socket_address));								\
		socket_address.sin_family 	= AF_INET;														\
		socket_address.sin_port 	= myport;														\
		socket_address.sin_addr 	= my_ip; 														\
		ret = bind(connection, (struct sockaddr *) &socket_address, sizeof(socket_address));		\
		if (ret < 0) { int errsave = errno; D1HT_CLOSE(connection); connection = ret; errno = errsave;}	\
	}																								\
}

#define	TCP_SOCKET( connection, myport) SOCKET( connection, SOCK_STREAM, 	IPPROTO_TCP, myport);
#define	UDP_SOCKET( connection, myport)	SOCKET( connection, SOCK_DGRAM, 	IPPROTO_UDP, myport);

int main(int argc, char** argv)
{ 
	struct in_addr my_ip;
	char* end;
	int  portI, portF;
	
	if (argc !=3 ) EXIT;

	portI = strtol (argv[1], &end, 10);	
	if (*end != '\0') 	EXIT;  /* The user specified non-digits.  */
	if (portI <= 0) 	EXIT;
	
	portF = strtol (argv[2], &end, 10);
	if (*end != '\0') 	EXIT;  /* The user specified non-digits.  */
	if (portF <= portI) EXIT;

	struct addrinfo *res, hints;
	char			myname[80];

	if (gethostname(myname, sizeof(myname)) != 0 ) {
		fprintf(stderr, "gethostname failed err=%s\n", strerror(errno));
		exit(-1);
	}
	
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family		= AF_INET;
	hints.ai_socktype 	= SOCK_STREAM;
	hints.ai_protocol	= IPPROTO_TCP;
	
	if (getaddrinfo(myname, NULL, &hints, &res) != 0 ) {
		fprintf(stderr, "getaddrinfo failed err=%s\n", strerror(errno));
		exit(-1);
	}
	
	do {
		my_ip = ((struct sockaddr_in *) (res->ai_addr))->sin_addr;
		fprintf(stderr,"my name=%s, my IP=%s\n", myname, inet_ntoa( my_ip ));
		res=res->ai_next;
	} while (res != NULL);

	freeaddrinfo(res);
	in_port_t 	try_port;
	int			try_socket = -1;
	int			next_port;
	
	for (int port=portI; port <= portF; port=next_port) {
		  
		  D1HT_CLOSE(try_socket);
		  next_port = port+1;
//		  fprintf(stderr, "trying %u, next=%u\n", port, next_port);
		  
		  try_port = (in_port_t) htons(port);
		  
		  TCP_SOCKET(try_socket, try_port);
		  if (try_socket < 0) continue;
		  if (listen(try_socket, 100) != 0) continue;
		  D1HT_CLOSE(try_socket);
	
		  UDP_SOCKET(try_socket, try_port);
		  if (try_socket < 0) continue;
		  D1HT_CLOSE(try_socket);
			  
		  try_port = (in_port_t) htons(next_port);	  
	
		  UDP_SOCKET(try_socket, try_port);
		  if (try_socket >= 0) { 
			  fprintf(stderr,"portOK=%u\n", port);
		  } else {
			  next_port++;
		  }
    }
	fprintf(stderr, "%s program ended\n", argv[0]);
}
