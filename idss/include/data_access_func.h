#ifndef __DATA_ACCESS_FUNC_H__
#define __DATA_ACCESS_FUNC_H__

#define TABLES				"TABLES"
#define TABLE				"TABLE"
#define RELATIONS			"RELATIONS"
#define RELATION			"RELATION"
#define ALL 				"ALL"
#define INSERT_INTO 			"INSERT INTO "
#define NAME				"NAME"
#define RECORDS				"RECORDS"
#define RECORD				"RECORD"
#define ATTRIBUTES                      "ATTRIBUTES"
#define ATTRIBUTE			"ATTRIBUTE"
#define FOREIGNKEY			"FOREIGNKEY"
#define TYPE				"TYPE"
#define STRING				"STRING"
#define VALUES 				" VALUES ("
#define SELECT 				"SELECT "
#define REFERENCETABLE      		"REFERENCETABLE"
#define REFERENCEKEY			"REFERENCEKEY"
#define FROM 				" FROM "
#define WHERE 				" WHERE "
#define REFERENCEFIELDS			"REFERENCEFIELDS"
#define AND 				" AND "

#define MAXBUFLEN			4096
#define BUFLEN				1024


#define SQLITE_OK 0
#define SQLITE_OPEN_DB_ERROR 1
#define SQLITE_QUERY_SUBMISSION_ERROR 2
#define SQLITE_RETRIEVE_QUERY_INFORMATION_ERROR 3
#define SQLITE_PARSING_MQ_FILE_ERROR 4
#define SQLITE_MEMORY_ERROR 5
#define SQLITE_GET_VALUE_ERROR 6

int init_connection(sqlite_handle* db_handle);
int open_connection(char* dbname, sqlite_handle* db_handle);
int exec_query(sqlite_handle* db_handle, char* query);
int n_tuples(sqlite_handle* db_handle);
int init_answer(query_answer *answer);
int single_query_retrieve_information(sqlite_handle* db_handle, query_answer *answer);
int free_answer (query_answer *answer);
int close_connection(sqlite_handle* db_handle);
int get_sql (char *docname, sqlite_handle* db_handle, int xml_flag);
char *return_type(int num_type);

#endif