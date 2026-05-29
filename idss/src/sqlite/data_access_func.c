#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "libxml/xmlmemory.h"
#include "libxml/parser.h"
#include "data_access_structures.h"
#include "data_access_func.h"

static void clear_result(sqlite_handle* db_handle);
static int sqlite_get_value (char** field, sqlite_handle *db_handle, int row, int column);
static int parseSize (xmlDocPtr doc, xmlNodePtr cur, int *sizeRelation);
static int parseRecord (xmlDocPtr doc, xmlNodePtr cur, Relation *** relation, sqlite_handle *db_handle);
static int parseRecordAttribute (xmlDocPtr doc, xmlNodePtr cur, sqlite_handle *db_handle, char sqlInit[], char sqlUpdate[], char sqlDelete[], char *table, Relation *** relation, int oper);
static int fill_relation (xmlDocPtr doc, xmlNodePtr cur, Relation *** relation, char *flag, int index, sqlite_handle *db_handle);
static int get_operation_type (char *oper);
static int my_strcat (char *, const char *);
static char * _lltoa (long long value, char *string);
static int relation_destroy (Relation *** relation);
static long value_relation (xmlDocPtr doc, xmlNodePtr cur, sqlite_handle *db_handle);


static void clear_result(sqlite_handle* db_handle)
{
	if (db_handle->result)
		{
			sqlite3_finalize(db_handle->result);
			db_handle->result = NULL;
		}

	db_handle->n_fields = 0;
	db_handle->n_rows = 0;
	db_handle->last_row = -1;
	db_handle->is_query = 0;
}


int init_connection(sqlite_handle* db_handle)
{
	db_handle->db_connection = NULL;
	db_handle->result = NULL;
	db_handle->db_name = NULL;
	db_handle->n_fields = 0;
	db_handle->n_rows = 0;
	db_handle->last_row = -1;
	db_handle->is_query = 0;
	return SQLITE_OK;
}


int open_connection(char* dbname, sqlite_handle* db_handle)
{
	int rc = 0;

	if (db_handle->db_connection)
		{
			sqlite3_close(db_handle->db_connection);
			db_handle->db_connection = NULL;
		}

	if (db_handle->result)
		{
			sqlite3_finalize(db_handle->result);
			db_handle->result = NULL;
		}

	if (db_handle->db_name)
		{
			free(db_handle->db_name);
			db_handle->db_name = NULL;
		}

	db_handle->db_name = strdup(dbname);
	if (!db_handle->db_name)
		return SQLITE_MEMORY_ERROR;

	rc = sqlite3_open(dbname, &(db_handle->db_connection));
	if (!rc)
		return SQLITE_OK;
	else
		{
			db_handle->db_connection = NULL;
			if (db_handle->db_name)
				{
				 free(db_handle->db_name);
				 db_handle->db_name = NULL;
				}
			return SQLITE_OPEN_DB_ERROR;
		}
}


int exec_query(sqlite_handle* db_handle, char* query)
{
	int rc = 0;
	int n_rows = 0;
	char *query_tmp = NULL;
	char *token = NULL;
	char *save_ptr = NULL;

	clear_result(db_handle);

	if (db_handle->db_connection == NULL)
	{
		clear_result(db_handle);
		return SQLITE_QUERY_SUBMISSION_ERROR;
	}

	if (sqlite3_prepare(db_handle->db_connection, query, strlen(query), &(db_handle->result), NULL) != SQLITE_OK)
        {
                clear_result(db_handle);
		return SQLITE_QUERY_SUBMISSION_ERROR;
        }

	rc = sqlite3_step(db_handle->result);

	while (rc == SQLITE_ROW)
		{
                	n_rows++;
			rc = sqlite3_step(db_handle->result);
		}

        if (rc != SQLITE_DONE)
        	{
                	clear_result(db_handle);
                	return SQLITE_QUERY_SUBMISSION_ERROR;
        	}

	if (!(query_tmp = strdup (query)))
        	{
                	clear_result(db_handle);
                	return SQLITE_MEMORY_ERROR;
        	}

        token = strtok_r (query_tmp, " ", &save_ptr);
        if ((strcasecmp (token, "SELECT") == 0))
        	{
                	db_handle->n_rows = n_rows;
                	db_handle->n_fields = sqlite3_column_count(db_handle->result);
			db_handle->last_row = n_rows;
			db_handle->is_query = 1;
        	}
        else if ((strcasecmp (token, "INSERT") == 0) || (strcasecmp (token, "DELETE") == 0) || (strcasecmp (token, "UPDATE") == 0))
        	{
               		db_handle->n_rows = sqlite3_total_changes(db_handle->db_connection);
                	db_handle->n_fields = 0;
			db_handle->last_row = -1;
			db_handle->is_query = 0;
        	}
        else 	{
			db_handle->n_rows = 0;
			db_handle->n_fields = 0;
			db_handle->last_row = -1;
			db_handle->is_query = 0;
		}

        if (query_tmp)
                free (query_tmp);

	return SQLITE_OK;
}


int n_tuples(sqlite_handle* db_handle)
{
	return db_handle->n_rows;
}


int init_answer(query_answer *answer)
{
 	answer->schema = NULL;
 	answer->records = NULL;
	answer->type = NULL;
	return SQLITE_OK;
}


int single_query_retrieve_information (sqlite_handle* db_handle, query_answer *answer)
{
	int rows;
  	int columns;
  	int i;
  	int j;
  	int cont;
  	int rec;
  	char *tmp_buffer = NULL;
	int rc = SQLITE_OK;

  	if (!db_handle->result)
        	return SQLITE_RETRIEVE_QUERY_INFORMATION_ERROR;

  	if(!db_handle->is_query)
		return SQLITE_RETRIEVE_QUERY_INFORMATION_ERROR;

  	rows = db_handle->n_rows;
  	columns = db_handle->n_fields;

  	answer->schema = (char **) malloc (sizeof (char *) * (columns + 1));
  	if(!(answer->schema))
		{
		 rc = SQLITE_MEMORY_ERROR;
		 goto exit_error;
		}

  	answer->num_record = rows;
  	answer->num_field = columns;

  	for (i = 0; i < columns; i++)
    		{

        		tmp_buffer = (char *)sqlite3_column_name(db_handle->result, i);

			if (!tmp_buffer)
				{
					rc = SQLITE_MEMORY_ERROR;
					goto exit_error;
				}

        		cont = strlen (tmp_buffer) - 1;
      			while (tmp_buffer[cont] == ' ')
				{
	  	 		 tmp_buffer[cont] = '\0';
	  	 		 cont--;
				}
        		answer->schema[i] = strdup (tmp_buffer);
        		if(!(answer->schema[i]))
				{
					rc = SQLITE_MEMORY_ERROR;
					goto exit_error;
      				}

		}

   	answer->schema[i] = NULL;

	answer->type = (int *) malloc (sizeof (int) * columns );
	if (!(answer->type))
		{
		 rc = SQLITE_MEMORY_ERROR;
		 goto exit_error;
		}

	if (rows > 0)
		{
		 sqlite3_reset(db_handle->result);
        	 db_handle->last_row = -1;
		 if (sqlite3_step(db_handle->result) != SQLITE_ROW)
			{
		 	 rc = SQLITE_RETRIEVE_QUERY_INFORMATION_ERROR;
		 	 goto exit_error;
			}
		 (db_handle->last_row)++;

		 for (i = 0; i < columns; i++)
			answer->type[i] = sqlite3_column_type(db_handle->result, i);
		}
	else
		for (i = 0; i < columns; i++)
			answer->type[i] = SQLITE_NULL;

   	answer->records = (char **) malloc (sizeof (char *) * (columns * rows + 1));
   	if(!(answer->records))
		{
			rc = SQLITE_MEMORY_ERROR;
			goto exit_error;
  		}

	rec = 0;
   	for (i = 0; i < rows; i++)
  		{
		 for (j = 0; j < columns; j++)
			{
			 sqlite_get_value (&tmp_buffer, db_handle, i, j);
			 if (tmp_buffer != NULL)
				{
				 if (strlen (tmp_buffer) > 0)
					{
						cont = strlen (tmp_buffer) - 1;
	  					while (tmp_buffer[cont] == ' ')
	    					{
	      			 		 tmp_buffer[cont] = '\0';
	      			 		 cont--;
	    					}
	  		 			answer->records[rec] = strdup (tmp_buffer);
	  		 			if(!(answer->records[rec]))
						{
				 		 rc = SQLITE_MEMORY_ERROR;
				 		 goto exit_error;
	  					}
					}
				else
					{
						answer->records[rec] = strdup ("(NULL)");
						if(!(answer->records[rec]))
						{
				 		 rc = SQLITE_MEMORY_ERROR;
				 		 goto exit_error;
	  					}
					}
			 	}
			else
				{
				  answer->records[rec] = strdup ("(NULL)");
				  if(!(answer->records[rec]))
					{
				 	 rc = SQLITE_MEMORY_ERROR;
				 	 goto exit_error;
	  				}
				}
			rec++;
			}
  		}
  	answer->records[rec] = NULL;
  	answer->index = 1;
	rc = SQLITE_OK;
	goto exit_success;

exit_error:
	free_answer(answer);
exit_success:
  	return rc;
}


static int sqlite_get_value (char** field, sqlite_handle *db_handle, int row, int column)
{
        int records = -1;
        int fields = -1;
        int value_int = 0;
        int blob_size = 0;
        double value_double = 0;
	      unsigned char *value_text = NULL;
        char value[1024] = {0};
        int error_code = 0;

        if (!db_handle->result)
                return SQLITE_GET_VALUE_ERROR;

        if (!db_handle->is_query)
                return -SQLITE_GET_VALUE_ERROR;

        records = db_handle->n_rows;
        fields = db_handle->n_fields;

        if ((row < 0) || (column < 0) || (records<=row) || (fields<=column))
                return SQLITE_GET_VALUE_ERROR;

        if (row<(db_handle->last_row))
        {
                sqlite3_reset(db_handle->result);
                db_handle->last_row = -1;
        }

        while (db_handle->last_row != row)
        {
                error_code = sqlite3_step(db_handle->result);
                if (error_code != SQLITE_ROW)
                        return SQLITE_GET_VALUE_ERROR;
                db_handle->last_row++;
        }

        switch ( sqlite3_column_type(db_handle->result, column) )
        {
                case SQLITE_INTEGER:
                        value_int = sqlite3_column_int(db_handle->result, column);
                        sprintf(value, "%d", value_int);
                        if (!(*field = strdup(value)))
                                return SQLITE_MEMORY_ERROR;
                        break;
                case SQLITE_FLOAT:
                        value_double = sqlite3_column_double(db_handle->result, column);
                        sprintf(value, "%f", value_double);
                        if (!(*field = strdup(value)))
                                return SQLITE_MEMORY_ERROR;
                        break;
                case SQLITE_TEXT:
                        value_text = (unsigned char *) sqlite3_column_text(db_handle->result, column);
                        sprintf(value, "%s", value_text);
                        if (!(*field = strdup(value)))
                                return SQLITE_MEMORY_ERROR;
                        break;
                case SQLITE_BLOB:
                        value_text = (unsigned char *) sqlite3_column_blob(db_handle->result, column);
                        blob_size = sqlite3_column_bytes(db_handle->result, column);
                        *field = malloc(blob_size*sizeof(char));
                        if (!(*field))
                                return SQLITE_MEMORY_ERROR;
                        memcpy(*field, value_text, blob_size);
                        break;
                case SQLITE_NULL:
                        *field = NULL;
                        break;
                default:
                        return SQLITE_GET_VALUE_ERROR;
                        break;
        }

        return SQLITE_OK;
}


int free_answer (query_answer* answer)
{
  int i;

  if (answer->schema)
    {
      i = 0;
      while (answer->schema[i] != NULL)
        {
          free (answer->schema[i]);
          i++;
        }
      free (answer->schema);
      answer->schema = NULL;
    }

  if (answer->records)
    {
      i = 0;
      while (answer->records[i] != NULL)
        {
          free (answer->records[i]);
          i++;
        }
      free (answer->records);
      answer->records = NULL;
    }

  if (answer->type)
	{
      	  free (answer->type);
      	   answer->type = NULL;
    	}

  return SQLITE_OK;
}


int close_connection(sqlite_handle* db_handle)
{
	clear_result(db_handle);

	if (db_handle->db_name)
		{
			free(db_handle->db_name);
			db_handle->db_name = NULL;
		}

	if (db_handle->db_connection != NULL)
		{
			sqlite3_close(db_handle->db_connection);
			db_handle->db_connection = NULL;
		}
	return SQLITE_OK;
}


int get_sql (char *docname, sqlite_handle* db_handle, int xml_flag)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    int retparserec;
    int sizeRelation = 0;
    int i = 0;
    Relation **relation = NULL;;
    doc = NULL;

    if (xml_flag == 0)
      {
	doc = xmlParseMemory (docname, strlen (docname) - 1);
      }
    else if (xml_flag == 1)
      {
	doc = xmlParseFile (docname);
      }
    if (doc == NULL)
      {
	return SQLITE_PARSING_MQ_FILE_ERROR;
      }

    cur = xmlDocGetRootElement (doc);

    if (cur == NULL)
      {
	xmlFreeDoc (doc);
	return SQLITE_PARSING_MQ_FILE_ERROR;
      }

    if (xmlStrcmp (cur->name, (const xmlChar *) TABLES))
      {
	xmlFreeDoc (doc);
	return SQLITE_PARSING_MQ_FILE_ERROR;
      }

    if (parseSize (doc, cur, &sizeRelation))
      {
	xmlFreeDoc (doc);
	return SQLITE_PARSING_MQ_FILE_ERROR;
      }

    if (!
	((relation) =
	 (Relation **) malloc ((1 + sizeRelation) * sizeof (Relation *))))
      {
	xmlFreeDoc (doc);
	return SQLITE_MEMORY_ERROR;
      }


    for (i = 0; i < sizeRelation; i++)
      {
	if (!(relation[i] = (Relation *) malloc (sizeof (Relation))))
	  {
      relation_destroy (&relation);
	    xmlFreeDoc (doc);
	    return SQLITE_MEMORY_ERROR;
	  }
      }
    relation[sizeRelation] = NULL;
    if ((retparserec = parseRecord (doc, cur, &relation, db_handle)) < 0)
      {
	xmlFreeDoc (doc);
	return retparserec;
      }
    relation_destroy (&relation);
    xmlFreeDoc (doc);
    return SQLITE_OK;
}


/* This function frees the dynamic memory allocated before for the array of Relation structures */
static int relation_destroy (Relation *** relation)
{
    int i = 0;

    for (i = 0; (*relation)[i]; i++)
      {
	free ((*relation)[i]->foreignkey);
	(*relation)[i]->foreignkey = NULL;
	free ((*relation)[i]->type);
	(*relation)[i]->type = NULL;
	free ((*relation)[i]);
	(*relation)[i] = NULL;
      }
    free (*relation);
    *relation = NULL;

    return 1;
}


static int parseSize (xmlDocPtr doc, xmlNodePtr cur, int *sizeRelation)
{
    xmlNodePtr cur1, cur2, cur3;
    cur1 = cur->xmlChildrenNode;

    while (cur1 != NULL)
      {

	if ((!xmlStrcmp (cur1->name, (const xmlChar *) RELATIONS)))
	  {

	    cur2 = cur1->xmlChildrenNode;

	    while (cur2 != NULL)
	      {
		if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATION)))
		  {
		    (*sizeRelation)++;
		  }
		cur2 = cur2->next;
	      }
	  }

	else if ((!xmlStrcmp (cur1->name, (const xmlChar *) TABLE)))
	  {

	    cur2 = cur1->xmlChildrenNode;

	    while (cur2 != NULL)
	      {

		if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATIONS)))
		  {

		    cur3 = cur2->xmlChildrenNode;

		    while (cur3 != NULL)
		      {
			if ((!xmlStrcmp
			     (cur3->name, (const xmlChar *) RELATION)))
			  {
			    (*sizeRelation)++;
			  }
			cur3 = cur3->next;
		      }
		  }
		cur2 = cur2->next;
	      }
	  }
	cur1 = cur1->next;
      }

    return 0;
}


static int parseRecord (xmlDocPtr doc, xmlNodePtr cur, Relation *** relation, sqlite_handle *db_handle)
{

    xmlNodePtr cur1, cur2, cur3;
    char sqlInit[MAXBUFLEN] = {0};
    char sqlUpdate[MAXBUFLEN] = {0};
    char sqlDelete[MAXBUFLEN] = {0};
    int indexRelation = 0;
    int oper;

    cur1 = cur->xmlChildrenNode;

    while (cur1 != NULL)
      {

	if ((!xmlStrcmp (cur1->name, (const xmlChar *) RELATIONS)))
	  {

	    cur2 = cur1->xmlChildrenNode;

	    while (cur2 != NULL)
	      {
		if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATION)))
		  {
		    if ((fill_relation
			 (doc, cur2, relation, ALL, indexRelation,
			  db_handle)) < 0)
		      return -1;
		    indexRelation++;
		  }
		cur2 = cur2->next;
	      }
	  }

	else if ((!xmlStrcmp (cur1->name, (const xmlChar *) TABLE)))
	  {
	    char *ptr = NULL;

	    strcpy (sqlInit, INSERT_INTO);
	    ptr = (char *) xmlGetProp (cur1, BAD_CAST NAME);
	    if (ptr)
	      {
		my_strcat (sqlInit, ptr);
		free (ptr);
		ptr = NULL;
	      }
	    ptr = (char *) xmlGetProp (cur1, BAD_CAST NAME);
	    sprintf (sqlUpdate, "UPDATE %s set ", ptr);
	    if (ptr){
	      free (ptr);
 	      ptr = NULL;
   	    }
	    ptr = (char *) xmlGetProp (cur1, BAD_CAST NAME);
	    sprintf (sqlDelete, "DELETE from %s ", ptr);
	    if (ptr){
	      free (ptr);
 	      ptr = NULL;
   	    }
	    cur2 = cur1->xmlChildrenNode;

	    while (cur2 != NULL)
	      {

		if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATIONS)))
		  {

		    cur3 = cur2->xmlChildrenNode;

		    while (cur3 != NULL)
		      {
			if ((!xmlStrcmp
			     (cur3->name, (const xmlChar *) RELATION)))
			  {
			    char *ptr = NULL;

			    ptr = (char *) xmlGetProp (cur1, BAD_CAST NAME);
			    if ((fill_relation (doc, cur3, relation,
						ptr,
						indexRelation, db_handle)) < 0)
			      return -1;
			    indexRelation++;
			    if (ptr){
			      free (ptr);
			      ptr = NULL;
			    }
			  }
			cur3 = cur3->next;
		      }
		  }

		else if ((!xmlStrcmp (cur2->name, (const xmlChar *) RECORDS)))
		  {

		    cur3 = cur2->xmlChildrenNode;

		    while (cur3 != NULL)
		      {
			if ((!xmlStrcmp
			     (cur3->name, (const xmlChar *) RECORD)))
			  {
			    ptr = (char *) xmlGetProp (cur3, BAD_CAST "OPER");

			    if (ptr == NULL)
			      oper = 0;
			    else
			      oper = get_operation_type (ptr);
			    free (ptr);
			    ptr = NULL;
			    ptr = (char *) xmlGetProp (cur1, BAD_CAST NAME);
			    if (ptr)
			      {
				if (parseRecordAttribute
				    (doc, cur3, db_handle, sqlInit, sqlUpdate,
				     sqlDelete, ptr, relation,
				     oper))
				  {
				    free (ptr);
				    ptr = NULL;
				    return -1;
				  }
			      }

			  }
			cur3 = cur3->next;

		      }
		  }
		cur2 = cur2->next;
	      }
	  }
	cur1 = cur1->next;
      }

    return 0;
}


static int parseRecordAttribute (xmlDocPtr doc, xmlNodePtr cur, sqlite_handle *db_handle,
		      char sqlInit[], char sqlUpdate[], char sqlDelete[],
		      char *table, Relation *** relation, int oper)
{
  xmlNodePtr cur1, cur2;
  char tmp_sqlInsert[MAXBUFLEN] = {0};
  char tmp_sqlDelete[MAXBUFLEN] = {0};
  char tmp_sqlSelect[MAXBUFLEN] = {0};
  char tmp_sqlUpdate[MAXBUFLEN] = {0};
  char condition[MAXBUFLEN+10] = {0};
  char update_set[MAXBUFLEN] = {0};
  char update_where[MAXBUFLEN] = {0};
  int where_set;		/* YES = 0 */
  int count_set;
  int tuples;
  int count_where;
  int i = 0;
  int j, k;
  long valueSelect = -1;
  char buffer[BUFLEN];
  int res;

  strcpy (update_set, "");
  strcpy (update_where, "");
  strcpy (tmp_sqlUpdate, sqlUpdate);
  strcpy (tmp_sqlInsert, sqlInit);
  strcpy (tmp_sqlDelete, sqlDelete);
  strcat (tmp_sqlInsert, " (");
  cur1 = cur->xmlChildrenNode;

  j = -1;
  k = -1;
  count_set = -1;
  count_where = -1;
  while (cur1 != NULL)
    {
      if ((!xmlStrcmp (cur1->name, (const xmlChar *) RELATIONS)))
	{

	  cur2 = cur1->xmlChildrenNode;

	  while (cur2 != NULL)
	    {
	      if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATION)))
		{
		  char *ptr = NULL;
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST FOREIGNKEY);
		  my_strcat (tmp_sqlInsert, ptr);
		  strcat (tmp_sqlInsert, ",");
		  free (ptr);
		  ptr = NULL;
		}
	      cur2 = cur2->next;
	    }
	}
      else if ((!xmlStrcmp (cur1->name, (const xmlChar *) ATTRIBUTES)))
	{
	  cur2 = cur1->xmlChildrenNode;
	  while (cur2 != NULL)
	    {
	      if ((!xmlStrcmp (cur2->name, (const xmlChar *) ATTRIBUTE)))
		{
		  char *ptr = NULL;
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST "WHERE");
		  where_set = 1;	/* default where_set  = NO */
		  if (ptr)
		    {
		      where_set = xmlStrcmp (BAD_CAST ptr, BAD_CAST "YES");
		      free (ptr);
		      ptr = NULL;
		    }
		  if (!where_set)
		    {		/* if YES */
		      count_where++;
		      if (count_where)
			strcat (update_where, " and ");
		    }
		  else
		    {
		      count_set++;
		      if (count_set)
			strcat (update_set, " , ");
		    }

		  k++;
		  if (k)
		    strcat (tmp_sqlDelete, " and ");
		  else
		    strcat (tmp_sqlDelete, " where ");

		  ptr = (char *) xmlGetProp (cur2, BAD_CAST TYPE);
		  if (ptr)
		    {
		      if ((!xmlStrcmp (BAD_CAST ptr, BAD_CAST STRING)))
			{
			  char *ptr_2 = NULL;
			  ptr_2 = (char *) xmlGetProp (cur2, BAD_CAST NAME);
			  if (ptr_2)  {
			  	strcpy (condition, ptr_2);
			    	free (ptr_2);
				ptr_2 = NULL;
			  }
			  strcat (condition, "=");
			  strcat (condition, "'");
			  ptr_2 =
			    (char *) xmlNodeListGetString (doc,
							   cur2->
							   xmlChildrenNode,
							   1);
			  if (!ptr_2)
			    ptr_2 = strdup ("");
			  if (!ptr_2){
			    if(ptr){
				free(ptr);
				ptr = NULL;
			    }
			    return -1;
			  }

			  my_strcat (condition, ptr_2);
			  if (ptr_2){
			    free (ptr_2);
			    ptr_2 = NULL;
 			  }
			  strcat (condition, "'");
			  strcat (condition, " ");
			  if (!where_set)
			    strcat (update_where, condition);
			  else
			    strcat (update_set, condition);
			  strcat (tmp_sqlDelete, condition);
			}
		      else
			{
			  char *ptr_2 = NULL;
			  ptr_2 = (char *) xmlGetProp (cur2, BAD_CAST NAME);
			  if (ptr_2) {
			  	strcpy (condition, ptr_2);
			    	free (ptr_2);
				ptr_2 = NULL;
			  }
			  strcat (condition, "=");
			  ptr_2 = (char *) xmlNodeListGetString (doc,
								 cur2->
								 xmlChildrenNode,
								 1);
			  my_strcat (condition, ptr_2);
			  if (ptr_2){
			    free (ptr_2);
			    ptr_2 = NULL;
		  	  }
			  strcat (condition, " ");
			  if (!where_set)
			    strcat (update_where, condition);
			  else
			    strcat (update_set, condition);
			  strcat (tmp_sqlDelete, condition);
			}
		      free (ptr);
		      ptr = NULL;
		    }
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST NAME);
		  my_strcat (tmp_sqlInsert, ptr);
		  if (ptr){
		    free (ptr);
		    ptr = NULL;
		  }
		  strcat (tmp_sqlInsert, ",");
		}
	      cur2 = cur2->next;
	    }
	}
      cur1 = cur1->next;
    }

  for (i = 0; (*relation)[i]; i++)
    {
      if (!(strcmp ((*relation)[i]->type, ALL))
	  || !(strcmp ((*relation)[i]->type, table)))
	{
	  my_strcat (tmp_sqlInsert, (*relation)[i]->foreignkey);
	  strcat (tmp_sqlInsert, ",");
	}
    }

  tmp_sqlInsert[strlen (tmp_sqlInsert) - 1] = '\0';
  strcat (tmp_sqlInsert, ")");
  strcat (tmp_sqlInsert, VALUES);
  cur1 = cur->xmlChildrenNode;

  while (cur1 != NULL)
    {
      if ((!xmlStrcmp (cur1->name, (const xmlChar *) RELATIONS)))
	{
	  cur2 = cur1->xmlChildrenNode;
	  while (cur2 != NULL)
	    {
	      if ((!xmlStrcmp (cur2->name, (const xmlChar *) RELATION)))
		{
		  char *ptr = NULL;
		  j++;
		  k++;
		  valueSelect = value_relation (doc, cur2, db_handle);
		  strcat (tmp_sqlInsert,
			     _lltoa (valueSelect, buffer));
		  strcat (tmp_sqlInsert, ",");

		  ptr = (char *) xmlGetProp (cur2, BAD_CAST "WHERE");
		  where_set=1;
		  if (ptr) {
		  	where_set = xmlStrcmp (BAD_CAST ptr, BAD_CAST "YES");
		        free (ptr);
			ptr = NULL;
		  }
		  if (!where_set)
		    {		/* if YES */
		      count_where++;
		      if (count_where)
			strcat (update_where, " and ");
		    }
		  else
		    {
		      count_set++;
		      if (count_set)
			strcat (update_set, " , ");
		    }
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST FOREIGNKEY);
		  if (ptr) {
		 	 sprintf (condition, " %s = %ld ", ptr, valueSelect);
		    	 free (ptr);
			 ptr = NULL;
		  }

		  if (!where_set)
		    strcat (update_where, condition);
		  else
		    strcat (update_set, condition);
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST FOREIGNKEY);
		  if (ptr) {
		  if (!k)
		    sprintf (tmp_sqlDelete, "%s where %s = %ld",
			     tmp_sqlDelete, ptr, valueSelect);
		  else
		    sprintf (tmp_sqlDelete, "%s and %s = %ld",
			     tmp_sqlDelete, ptr, valueSelect);

		    free (ptr);
		    ptr = NULL;
		  }
		}
	      cur2 = cur2->next;
	    }
	}
      else if ((!xmlStrcmp (cur1->name, (const xmlChar *) ATTRIBUTES)))
	{
	  cur2 = cur1->xmlChildrenNode;
	  while (cur2 != NULL)
	    {
	      if ((!xmlStrcmp (cur2->name, (const xmlChar *) ATTRIBUTE)))
		{
		  char *ptr = NULL;;
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST TYPE);
		  if (ptr) {
		  if ((!xmlStrcmp (BAD_CAST ptr, BAD_CAST STRING)))
		    {
		      char *ptr_2 = NULL;
		      strcat (tmp_sqlInsert, "'");
		      ptr_2 = (char *) xmlNodeListGetString (doc,
							     cur2->
							     xmlChildrenNode,
							     1);

		      my_strcat (tmp_sqlInsert, ptr_2);
		      if (ptr_2){
			free (ptr_2);
			ptr_2 = NULL;
		      }
		      strcat (tmp_sqlInsert, "'");
		    }
		  else
		    {
		      char *ptr_2 = NULL;

		      ptr_2 =
			(char *) xmlNodeListGetString (doc,
						       cur2->xmlChildrenNode,
						       1);
		      my_strcat (tmp_sqlInsert, ptr_2);
		      if (ptr_2){
			free (ptr_2);
			ptr_2 = NULL;
		      }
		    }
		    free (ptr);
		    ptr = NULL;
		  }
		  strcat (tmp_sqlInsert, ",");
		}
	      cur2 = cur2->next;
	    }
	}
      cur1 = cur1->next;
    }


  /* IMPO sulle relazioni esterne al record la update non fa nulla.
     funzionano solo insert e delete */

  for (i = 0; (*relation)[i]; i++)
    {
      if (!(strcmp ((*relation)[i]->type, "ALL"))
	  || !(strcmp ((*relation)[i]->type, table)))
	{
	  j++;
	  k++;
	  valueSelect = (*relation)[i]->value;
	  strcat (tmp_sqlInsert, _lltoa (valueSelect, buffer));
	  strcat (tmp_sqlInsert, ",");
	  if (!k)
	    sprintf (tmp_sqlDelete, "%s where %s = %d", tmp_sqlDelete,
		     (*relation)[i]->foreignkey, (*relation)[i]->value);
	  else
	    sprintf (tmp_sqlDelete, "%s and %s = %d", tmp_sqlDelete,
		     (*relation)[i]->foreignkey, (*relation)[i]->value);
	}
    }


  tmp_sqlInsert[strlen (tmp_sqlInsert) - 1] = '\0';
  strcat (tmp_sqlInsert, ")");
  strcat (tmp_sqlUpdate, update_set);
  if (count_where > -1)
    {
      strcat (tmp_sqlUpdate, " where ");
      strcat (tmp_sqlUpdate, update_where);
    }
  switch (oper)
    {
    case 1:
      res = exec_query (db_handle, tmp_sqlInsert);
      break;
    case 2:
      res = exec_query (db_handle, tmp_sqlDelete);
      break;
    case 3:
      res = exec_query (db_handle, tmp_sqlUpdate);
      break;
    case 4:
      res = exec_query (db_handle, tmp_sqlDelete);
      res = exec_query (db_handle, tmp_sqlInsert);
      break;
    case 5:
      res = exec_query (db_handle, tmp_sqlUpdate);
      sprintf (tmp_sqlSelect, "Select * from %s where %s", table,
	       update_where);

      res = exec_query (db_handle, tmp_sqlSelect);
      tuples = n_tuples (db_handle);
      if (tuples == 0)
	    {
	      res = exec_query (db_handle, tmp_sqlInsert);
	    }
      break;
    default:
      res = exec_query (db_handle, tmp_sqlInsert);
      break;
    }

    return res;
}


static char * _lltoa (long long value, char *string)
{

    char tmp[65];
    char *tp = tmp;
    int i;
    unsigned long long v;
    int sign;
    char *sp;
    int radix = 10;

    if (radix == 10 && value < 0LL) {
        sign = 1;
        v = -value;
    }
    else {
        sign = 0;
        v = (unsigned long long) value;
    }

    while (v || tp == tmp) {
        i = v % (long long) radix;
        v = v / (long long) radix;
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i - 10 + 'a';
    }

    if (string == NULL && (tmp - tp + sign + 1))
        if (!(string = (char *) malloc(tmp - tp + sign + 1)))
            return NULL;

    if(string)
      sp = string;
    else
      return NULL;

    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = '\0';

    return string;
}


static int get_operation_type (char *oper)
{
/* INSERT tuple */
  if (!strcmp (oper, "INSERT"))
    return 1;
/* DELETE tuple */
  if (!strcmp (oper, "DELETE"))
    return 2;
/* UPDATE tuple */
  if (!strcmp (oper, "UPDATE"))
    return 3;
/* DELETE+INSERT = FORCED UPDATE tuple */
  if (!strcmp (oper, "FORCED_UPDATE"))
    return 4;
  if (!strcmp (oper, "FORCED_UPDATE2"))
    return 5;
  return 0;
}


static int fill_relation (xmlDocPtr doc, xmlNodePtr cur, Relation *** relation, char *flag, int index, sqlite_handle * db_handle)
{
  char *ptr = NULL;

  ptr = (char *) xmlGetProp (cur, BAD_CAST FOREIGNKEY);
  if (ptr)
  	{
    	 (*relation)[index]->foreignkey = ptr;
  	}
  else
  	{
    	 return SQLITE_PARSING_MQ_FILE_ERROR;
  	}

  (*relation)[index]->value = value_relation (doc, cur, db_handle);
  if (flag)
    	{
      	 (*relation)[index]->type = strdup (flag);
      	 if (!((*relation)[index]->type)){
			free(ptr);
			ptr = NULL;
			return SQLITE_PARSING_MQ_FILE_ERROR;
      			}
    	}
  return SQLITE_OK;
}


static long value_relation (xmlDocPtr doc, xmlNodePtr cur, sqlite_handle *db_handle)
{
  xmlNodePtr cur1, cur2;
  char sqlSelect[MAXBUFLEN] = {0};
  int tuples;
  int res = 0;
  char *ptr = NULL;

  char *value = NULL;
  strcpy (sqlSelect, SELECT);

  ptr = (char *) xmlGetProp (cur, BAD_CAST REFERENCETABLE);
  my_strcat (sqlSelect, ptr);
  if (ptr){
    free (ptr);
    ptr = NULL;
  }
  strcat (sqlSelect, ".");

  ptr = (char *) xmlGetProp (cur, BAD_CAST REFERENCEKEY);
  my_strcat (sqlSelect, ptr);
  if (ptr){
    free (ptr);
    ptr = NULL;
  }

  strcat (sqlSelect, FROM);
  ptr = (char *) xmlGetProp (cur, BAD_CAST REFERENCETABLE);
  my_strcat (sqlSelect, ptr);
  if (ptr){
    free (ptr);
    ptr = NULL;
  }
  strcat (sqlSelect, WHERE);
  ptr = (char *) xmlGetProp (cur, BAD_CAST REFERENCETABLE);
  my_strcat (sqlSelect, ptr);
  if (ptr){
    free (ptr);
    ptr = NULL;
  }
  strcat (sqlSelect, ".");

  cur1 = cur->xmlChildrenNode;

  while (cur1 != NULL)
    {
      if ((!xmlStrcmp (cur1->name, BAD_CAST REFERENCEFIELDS)))
	{

	  cur2 = cur1->xmlChildrenNode;

	  while (cur2 != NULL)
	    {

	      if ((!xmlStrcmp (cur2->name, BAD_CAST ATTRIBUTE)))
		{
		  ptr = (char *) xmlGetProp (cur2, BAD_CAST NAME);
		  my_strcat (sqlSelect, ptr);
		  if (ptr){
		    free (ptr);
    		    ptr = NULL;
 		  }
		  strcat (sqlSelect, " = ");

		  ptr = (char *) xmlGetProp (cur2, BAD_CAST TYPE);
		  if (ptr) {
		  if ((!xmlStrcmp (BAD_CAST ptr, BAD_CAST STRING)))
		    {
		      char *ptr_2 = NULL;
		      strcat (sqlSelect, "'");
		      ptr_2 = (char *) xmlNodeListGetString (doc,
							     cur2->
							     xmlChildrenNode,
							     1);

		      my_strcat (sqlSelect, ptr_2);
		      if (ptr_2){
			free (ptr_2);
			ptr_2 = NULL;
		      }
		      strcat (sqlSelect, "'");
		    }
		  else
		    {
		      char *ptr_2 = NULL;
		      ptr_2 =
			(char *) xmlNodeListGetString (doc,
						       cur2->xmlChildrenNode,
						       1);
		      my_strcat (sqlSelect, ptr_2);
		      if (ptr_2){
			free (ptr_2);
			ptr_2 = NULL;
		      }
		    }
		    free (ptr);
		    ptr = NULL;
		  }
		  strcat (sqlSelect, AND);
		}
	      cur2 = cur2->next;
	    }
	  sqlSelect[strlen (sqlSelect) - 5] = '\0';
	}
      cur1 = cur1->next;
    }

  res = exec_query (db_handle, sqlSelect);
  tuples = n_tuples (db_handle);
  if (tuples == 0)
    {
      //grelc_std_access_clear_result (handle);
      return -1;
    }
  else if (tuples == 1)
    {
      sqlite_get_value (&value, db_handle, 0, 0);
      //grelc_std_access_clear_result (handle);
      long l = strtol(value, (char **)NULL, 10);
      free(value);
      return l;
    }
  else
    {
      //grelc_std_access_clear_result (handle);
      return -1;
    }

}


static int my_strcat (char *p1, const char *p2)
{
  if (!p2)
    	return 1;			/* 1 if p2=NULL */
  strcat (p1, p2);
  return 0;
}


char *return_type(int num_type)
{
  char *type = NULL;

  switch (num_type)
    {
	    case 1:
	      type = strdup("bigint");
	      break;
	    case 2:
	      type = strdup("float");
	      break;
	    case 3:
	      type = strdup("text");
	      break;
	    case 4:
	      type = strdup("blob");
	    default:
	      type = strdup("undefined");
	      break;
    }
   return type;
}
