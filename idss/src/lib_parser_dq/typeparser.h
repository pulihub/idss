/*
 * typeparser.h
 * Definition of the structure used internally by the parser and lexer
 * to exchange data.
 */
 
#ifndef __TYPE_PARSER_H__
#define __TYPE_PARSER_H__
 
#include "tree.h"
 
/**
 * The structure used by flex and bison
 */
typedef union tagTypeParser
{
        SNode *tree;
        char *svalue;
	double dvalue;
}STypeParser;
 
// define the type for flex and bison
#define YYSTYPE STypeParser
 
 
#endif // __TYPE_PARSER_H__
 
