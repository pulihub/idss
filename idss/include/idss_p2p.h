#ifndef __IDSS_P2P_H__
#define __IDSS_P2P_H__

#include "parser_result.h"

void *query_execution(void *arg);
void *query_check(void *arg);
void *query_termination(void *arg);

struct _query_s
{
	char *uqi;
	char *value;
	char *key;
	double ttl;
	parser_result e;
};

typedef struct _query_s query_s;

/*
struct _query_exec_s
{
	char *key;
	char *value;
	char *uqi;
	double ttl;
};

typedef struct _query_exec_s query_exec_s;
*/

#endif
