/*  builds an Abstract Syntax Tree for the input query
 *
 * input parameters:
 *
 * const char *query      The query to be parsed
 *
 * parser_result *e       the ABS that has been built
 *
 * returns: 0 on success, -1 on error
 *
 * 
 *
 */

#include "parser_result.h"

int buildAST(char *query, parser_result *e);
