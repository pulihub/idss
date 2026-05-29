#ifndef __HELP_P2P_THRS_H__
#define __HELP_P2P_THRS_H__

#include "parser_result.h"

#define OK 			0
#define MEMORY_ERROR 		1
#define GENERIC_ERROR 		2

int remove_avg(char *original_query, char **result_query) ;

int local_exec(char *query, char *filename);

int merge_files(char *directory, char *finalfilename, char *uqi);

int apply_aggr(char *filename, char *finalfilename, char *query);

int getSubqValue(char *filename, double *result);

int executeNested(char *firstfilename, parser_result e, char *resultfile);

int sendbackfile(char *filename, char *myhostname, char *uqi, int i, char *destkey);

int addFields(parser_result *e);

#endif