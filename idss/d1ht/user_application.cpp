/**
 * @file user_application.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

int main(int argc, char** argv)
{ 
	G_parms.VERBOSE = 1;
	G_parms.verbose = 1;
	
	IPaddr peer;
	char hostname[80]; 

	unsigned short 	xsubi[3];
	xsubi[0] = (unsigned short) ((unsigned int) getpid() % 65535);
	xsubi[1] = (unsigned short) ((unsigned int) G_myID % 65535);
	xsubi[2] = (unsigned short) ((unsigned int) G_parms.myIP.ip.s_addr % 65535);
	
	get_myName(hostname, sizeof(hostname), &(peer.ip));
	
	peer.port = (PORTNO) htons(8100);
	
	int D1HT_connection = D1HT_connect(peer.ip, peer.port);
	
	if (D1HT_connection < 0) {
		VERBO(stderr, "ERROR Cause: Failed to connect to %s:%u\n", IPtoP(peer));
		return -1;
	}

	IPaddr 			home;
   	struct in_addr 	home_ip;
   	in_port_t 		home_port;

   	KEYID			id;
	char 			key[D1HT_MAX_KEY_SIZE];	
   	BVZERO(key, sizeof(key));  
   	
   	size_t 			nbytes 	= MAX(sizeof(id),sizeof(key));
	
	for (int i=2; i < 10; i++) {
		
		id = (KEYID) nrand48(xsubi);
		memcpy(key, &id, nbytes);
		
		int		reply;
		ssize_t	sreply 	= sizeof(reply);
		int 	payload = i;
		
		if (D1HT_route(key, sizeof(key), &D1HT_connection, &home_ip, &home_port, &payload, sizeof(payload), &reply, &sreply) < 0) {
			D1HT_close(&D1HT_connection);
			// lookup failed - if we want to continue, we should close(D1HT_connection) an call D1HT_connect again
			VERBO(stderr, "ERROR Cause: D1HT_route failed\n");
			return -1;
		}
			
		home.ip		= home_ip;
		home.port	= home_port;
	
		VERBO(stderr, "Reply from %s:%u received (sreply = %ld): 2*%d=%ld\n", IPtoP(home), (long) sreply, i, (long) reply);
		
		Assert((sreply == sizeof(reply)), XEQ_ERROR);
		Assert((reply == 2*i), XEQ_ERROR); 
	}
	
	D1HT_CLOSE(D1HT_connection);
	
	VERBO(stderr, "Program %s ending OK.\n", argv[0]);
	
	exit(0);
}

} // namespace d1ht
