#ifndef _PARSER_RESULT_H
#define _PARSER_RESULT_H

#include "tree.h"


typedef struct _parser_result
{
	SNode *root;
	char *parentq;
	int parentIsAgg;
	int numOfSubs;
	int numOfAggSubs;
	int numOfLeafs;
} parser_result;

#endif