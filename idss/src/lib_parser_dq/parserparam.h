/*
 * parserparam.h
 * Definitions of the parameters for the reentrant functions
 * of flex (yylex) and bison (yyparse)
 */
 
#ifndef __PARSERPARAM_H__
#define __PARSERPARAM_H__
 
#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H 1
#endif // YY_NO_UNISTD_H
 
#include "typeparser.h"
#include "lexer.h"
#include "tree.h"


typedef struct parseRResult
{
	SNode *root;
	char *parentquery;
	int parentIsAgg;
	int numOfAggSubs;
	int numOfSubs;
	int numOfLeafs;
}RResult;

/**
 * structure given as argument to the reentrant 'yyparse' function.
 */
typedef struct tagSParserParam
{
        yyscan_t scanner;
        RResult result;
}SParserParam;
 
// the parameter name (of the reentrant 'yyparse' function)
// data is a pointer to a 'SParserParam' structure
#define YYPARSE_PARAM data
 
// the argument for the 'yylex' function
#define YYLEX_PARAM   ((SParserParam*)data)->scanner
 
 
 
#endif // __PARSERPARAM_H__
 
