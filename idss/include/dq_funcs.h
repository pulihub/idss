#ifndef _DQ_FUNCS_H
#define _DQ_FUNCS_H

#include "db.h"
#include "parser_result.h"

#define DQF_OK	0
#define DQF_ERROR	-1
#define DQF_NO_MEMORY	-2

int getAllQueries(parser_result e, char **subqs);

int freeResult(parser_result e);

int insertQueryResults(parser_result e, double *values);

int evaluate(DB *dbp, parser_result e, int *result);

int getOtherFields(parser_result *e, char ***fields);

#endif
