#ifndef __DATA_ACCESS_STRUCTURES_H__
#define __DATA_ACCESS_STRUCTURES_H__

#include <sqlite3.h>

struct _sqlite_handle
{
        sqlite3* db_connection;
        sqlite3_stmt* result;
        char* db_name;
        int n_fields;
        int n_rows;
        int is_query;
        int last_row;
};


struct _query_answer
{
//! schema
/*! this variable is a pointer to the schema of the resultset */
        char    **schema;
//! type 
/*! this variable is a pointer to the type of field in the resultset */
	int	*type;
//! records
/*! this variable is a pointer to the records in the resultset */
        char    **records;
//! num_record
/*! this variable contains the number of records in the resultset */
        int     num_record;
//! num_field
/*! this variable contains the number of field in the schema */
        int     num_field;
//! index
/*! this variable is a pointer to the active record*/
        int     index;
};


struct _Relation
{
        char *type;
        char *foreignkey;
        int value;
};


typedef struct _sqlite_handle sqlite_handle;
typedef struct _query_answer query_answer;
typedef struct _Relation Relation;

#endif