#include <pthread.h>
#include "parserparam.h"
#include "parser.h"
#include "lexer.h"
#include "parser_result.h"

 
extern int yyparse(void *param);
extern pthread_mutex_t parser_mutex;
 
static int initParserParam(SParserParam* param)
{
        int ret = 0;
        param->result.root = NULL;
	param->result.parentIsAgg = 0;
	param->result.numOfAggSubs = 0;
	param->result.numOfSubs = 0;
	param->result.numOfLeafs = 0;
	param->result.parentquery = NULL;
        ret = yylex_init(&param->scanner);
 
        return ret;
}


static int destroyParserParam(SParserParam* param)
{
        return yylex_destroy(param->scanner);
}

 
int buildAST(char *query, parser_result *e)
{
        SParserParam p;
        YY_BUFFER_STATE state;

	pthread_mutex_lock(&parser_mutex);

        if ( initParserParam(&p) )
        {
                // couldn't initialize
                pthread_mutex_unlock(&parser_mutex);
                return 1;
        }
 
        state = yy_scan_string(query, p.scanner);
 
        if ( yyparse(&p) )
        {
                // error parsing
                pthread_mutex_unlock(&parser_mutex);
                return 1;
        }
 
        yy_delete_buffer(state, p.scanner);
 
        destroyParserParam(&p);
	e->root = p.result.root;
	e->parentq = p.result.parentquery;
	e->parentIsAgg = p.result.parentIsAgg;
	e->numOfAggSubs = p.result.numOfAggSubs;
	e->numOfSubs = p.result.numOfSubs;
	e->numOfLeafs = p.result.numOfLeafs;

	pthread_mutex_unlock(&parser_mutex);
        return 0;
} 
