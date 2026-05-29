#ifndef __P2P_COMM_H__
#define __P2P_COMM_H__

#define BROADCAST_QUERY 22
#define BROADCAST_RESPONSE 23
#define USE_ACK	1
#define CH_DELIMETER '|'
#define BUF_SIZE 1024

void on_query_received(void * args);

void on_response_received(void * args);

#endif
