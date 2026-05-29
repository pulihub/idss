#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "idss_data.h"
#include "idss_unix.h"
#include "utils.h"
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "libxml/xmlreader.h"
#include "libxml/xmlwriter.h"
#include "data_access_structures.h"
#include "data_access_func.h"
#include "idss_p2p.h"
#include "regex.h"
#include "p2p_comm.h"
#include "p2p_utils.h"
#include "parser_result.h"
#include "idss_parser_dq.h"
#include "dq_funcs.h"
#include "help_p2p_thrs.h"
#include "p2p_comm.h"


extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;
extern char ip_address[25];

int ttl_sleep(double ttl_seconds);


/*	Thread that allows query local execution
 */
void *query_execution(void *arg)
{
  char sql[4096] = {0};
  int result, result_query, rc = 0;
  char *query = NULL;
  char filename[2048] = {0};
  query_s *qs = NULL;
  char **subqs = NULL;
  int stop = 0;
  char errbuf[BUF_SIZE] = {0};
  int i = 0;
  int limit = 0;
  long long start_exec = 0, end_exec = 0;

  long secs, usecs;
  struct timeval tt;

  if (get_debug_flg() >= 1)
        idss_errmsg_r("Entering query_execution thread");

  qs = (query_s *) arg;

  if(!qs){

	if (get_debug_flg () >= 1)
	{
	 	snprintf(errbuf, BUF_SIZE, "WARNING: query argument of query_execution() is NULL!");
       	 	idss_errmsg_r (errbuf);
        	idss_errmsg_r("Exiting query_execution thread");
		return NULL;
	}

  }

  start_exec = get_current_time();

/*
  idss_errmsg_r("start TH query_execution for: ");
  idss_errmsg_r(qs->value);
  idss_errmsg_r("\n");
*/

  snprintf(errbuf, BUF_SIZE-1,"start TH query_execution for: %s", qs->value);
  idss_errmsg_r(errbuf);

  gettimeofday(&tt,NULL);
  secs=tt.tv_sec;
  usecs=tt.tv_usec;

  if ((qs->e.root == NULL) && (qs->e.parentq == NULL))
  {
	//We aren't the originator node, we have to build the AST
	  rc = buildAST(qs->value, &(qs->e));

  	 if (rc != 0)
   	 {
		if (get_debug_flg () >= 1)
		{
		 	snprintf(errbuf, BUF_SIZE, "Cannot build the tree associated to query %s! Error in buildAST function!", qs->uqi);
        	 	idss_errmsg_r (errbuf);
		}

		//Updating query status
   		sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   		pthread_mutex_lock(&db_handle_mutex);
   		result_query = exec_query(&db_handle, sql);
   		pthread_mutex_unlock(&db_handle_mutex);
   		memset(sql, 0, sizeof(sql));
   		if (result_query != SQLITE_OK)
   		{
			if(result_query == SQLITE_MEMORY_ERROR)
      			{
        			if (get_debug_flg () >= 1)
        				idss_errmsg_r ("exec_query error: not enough memory!");

				      stop = 1;
      			}
      			else
      			{
      	 			if (get_debug_flg () >= 1)
				      {
		 			      snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
					       idss_errmsg_r(errbuf);
				      }
      		  }
   		}

		goto exit;
   	 }
  }

  if ((qs->e.parentIsAgg > 0) && (qs->e.numOfAggSubs > 0))
   {
	//Our parent query contains agg functions and also subqueries with agg functions
	if (get_debug_flg () >= 1)
        	idss_errmsg_r ("This version of iDSS cannot execute this SQL SELECT statement: aggregate function in parent query and subqueries!");

	//Updating query status
   	sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   	pthread_mutex_lock(&db_handle_mutex);
   	result_query = exec_query(&db_handle, sql);
   	pthread_mutex_unlock(&db_handle_mutex);
   	memset(sql, 0, sizeof(sql));
   	if (result_query != SQLITE_OK)
   	{
		if(result_query == SQLITE_MEMORY_ERROR)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("exec_query error: not enough memory!");

			stop = 1;
      		}
      		else
      		{
      	 		if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
				idss_errmsg_r(errbuf);
			}
      		}
   	}

	goto exit;
   }

   if ((qs->e.numOfAggSubs > 0) && (qs->e.numOfSubs > qs->e.numOfAggSubs))
   {
	//We have subqueries with agg functions and subqueries without
	if (get_debug_flg () >= 1)
        	idss_errmsg_r ("This version of iDSS cannot execute this SQL SELECT statement: subqueries with aggregate functions and subqueries without aggregate functions inside the same nested query!");

	//Updating query status
   	sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   	pthread_mutex_lock(&db_handle_mutex);
   	result_query = exec_query(&db_handle, sql);
   	pthread_mutex_unlock(&db_handle_mutex);
   	memset(sql, 0, sizeof(sql));
   	if (result_query != SQLITE_OK)
   	{
		if(result_query == SQLITE_MEMORY_ERROR)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("exec_query error: not enough memory!");

			stop = 1;
      		}
      		else
      		{
      	 		if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
				idss_errmsg_r(errbuf);
			}
      		}
   	}

	goto exit;
   }

  if (qs->e.numOfAggSubs == 0)
  {
	//We have a query that must be executed only once (not nested)
	if (get_debug_flg () >= 1)
  {
    snprintf(errbuf, BUF_SIZE, "This query is not a nested query: there are %d subqueries and %d subqueries with aggregation operators", qs->e.numOfSubs, qs->e.numOfAggSubs);
		idss_errmsg_r(errbuf);
	}
	limit = 1;
	subqs = (char **)calloc(limit, sizeof(char *));
	if (!subqs)
   	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
	*subqs = strdup(qs->value);
	if (!*subqs)
	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
  }
  else
  {
	//We must decompose the query
	if (get_debug_flg () >= 1)
  {
    snprintf(errbuf, BUF_SIZE, "We must decompose the query! There are %d subqueries with aggregation operators", qs->e.numOfAggSubs);
		idss_errmsg_r(errbuf);
	}

	limit = qs->e.numOfSubs + 1;

  //Getting parent query and subqueries
	rc = addFields(&(qs->e));
	if (rc != DQF_OK)
	{
		if (rc == DQF_NO_MEMORY)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("addFields error: not enough memory!");

			stop = 1;
      		}
		else
		{
			if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occourred while executing query %s! Error while adding fields for nested query execution! Function: addFields", qs->uqi);
				idss_errmsg_r(errbuf);
			}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
                        result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
                        memset(sql, 0, sizeof(sql));
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory");

					stop = 1;
      				}
      			else
      				{
      	 				if (get_debug_flg () >= 1)
					{
		 				snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
		}
		goto exit;
	}
  	subqs = (char **)calloc(limit, sizeof(char *));
  	if (!subqs)
   	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
	rc = getAllQueries(qs->e, subqs);
  	if (rc != DQF_OK)
   	{
		if (rc == DQF_NO_MEMORY)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("getAllQueries error: not enough memory!");

			stop = 1;
      		}
		else
		{
			if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occourred while executing query %s! Error while decomposing query for nested execution! Function: getAllQueries", qs->uqi);
				idss_errmsg_r(errbuf);
			}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
                        memset(sql, 0, sizeof(sql));
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      			else
      				{
      	 				if (get_debug_flg () >= 1)
					{
		 				snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
		}
		goto exit;
   	}
  }

  //Local execution
  for (i = 0; i < limit; i++)
   {
	if (*(subqs + i))
	{


   	if (get_debug_flg () >= 1){
      idss_errmsg_r ("Parsed subqueries:");
		  snprintf(errbuf, BUF_SIZE, "subquery %s\"!", *(subqs + i));
		  idss_errmsg_r(errbuf);
    }

		snprintf(filename, 1024, "%s/%s_%d_%s", get_query_tempfiles_sd(), qs->uqi, i, get_ig_hostname());
		rc = remove_avg(*(subqs + i), &query);
		if (rc != OK)
		{
			if (rc == MEMORY_ERROR)
			{
				if (get_debug_flg () >= 1)
        				idss_errmsg_r ("remove_avg error: not enough memory!");

				stop = 1;
			}
			else
			{
				if (get_debug_flg () >= 1)
				{
					snprintf(errbuf, BUF_SIZE, "An error occurred while trying executing query %s! Error while removing avg functions! Function: remove_avg; subquery: \"%s\"", qs->uqi, *(subqs + i));
					idss_errmsg_r(errbuf);
				}

				//Updating query status
   				sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   				pthread_mutex_lock(&db_handle_mutex);
   				result_query = exec_query(&db_handle, sql);
   				pthread_mutex_unlock(&db_handle_mutex);
                                memset(sql, 0, sizeof(sql));
   				if (result_query != SQLITE_OK)
   				{
					if(result_query == SQLITE_MEMORY_ERROR)
      					{
        					if (get_debug_flg () >= 1)
        						idss_errmsg_r ("exec_query error: not enough memory!");

						stop = 1;
      					}
      					else
      					{
      	 					if (get_debug_flg () >= 1)
						{
		 					snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
							idss_errmsg_r(errbuf);
						}
      					}
   				}
			}
			goto exit;
		}

		rc = local_exec(query, filename);
		if (rc != OK)
		{
			if (rc == MEMORY_ERROR)
			{
				if (get_debug_flg () >= 1)
        				idss_errmsg_r ("local_exec error: not enough memory!");

				stop = 1;
			}
			else
			{
				if (get_debug_flg () >= 1)
				{
					snprintf(errbuf, BUF_SIZE, "An error occurred while trying executing query %s! Function: local_exec; subquery: \"%s\"", qs->uqi, query);
					idss_errmsg_r(errbuf);
				}

				//Updating query status
   				sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   				pthread_mutex_lock(&db_handle_mutex);
   				result_query = exec_query(&db_handle, sql);
   				pthread_mutex_unlock(&db_handle_mutex);
                                memset(sql, 0, sizeof(sql));
   				if (result_query != SQLITE_OK)
   				{
					if(result_query == SQLITE_MEMORY_ERROR)
      					{
        					if (get_debug_flg () >= 1)
        						idss_errmsg_r ("exec_query error: not enough memory!");

						stop = 1;
      					}
      					else
      					{
      	 					if (get_debug_flg () >= 1)
						{
		 					snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
							idss_errmsg_r(errbuf);
						}
      					}
   				}
			}
			goto exit;
		}

		if (query)
		{
			free(query);
			query = NULL;
		}
	}
    }

   //At this point everything should be ok
   //Updating query status
   sprintf(sql, "UPDATE tb_query SET local_exec = 1 WHERE uqi = '%s'", qs->uqi);
   pthread_mutex_lock(&db_handle_mutex);
   result_query = exec_query(&db_handle, sql);
   pthread_mutex_unlock(&db_handle_mutex);
   memset(sql, 0, sizeof(sql));
   if (result_query != SQLITE_OK)
   {
	if(result_query == SQLITE_MEMORY_ERROR)
      	{
        	if (get_debug_flg () >= 1)
        		idss_errmsg_r ("exec_query error: not enough memory!");

		stop = 1;
      	}
      	else
      	{
      	 	if (get_debug_flg () >= 1)
		{
		 	snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"locally executed\"!", qs->uqi);
			idss_errmsg_r(errbuf);
		}

		//Updating query status
   		sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   		pthread_mutex_lock(&db_handle_mutex);
   		result_query = exec_query(&db_handle, sql);
   		pthread_mutex_unlock(&db_handle_mutex);
                memset(sql, 0, sizeof(sql));
   		if (result_query != SQLITE_OK)
   		{
			if(result_query == SQLITE_MEMORY_ERROR)
      			{
        			if (get_debug_flg () >= 1)
        				idss_errmsg_r ("exec_query error: not enough memory!");

				stop = 1;
      			}
      			else
      			{
      	 			if (get_debug_flg () >= 1)
				{
					snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
					idss_errmsg_r(errbuf);
				}
      			}
   		}
      	}
   }

  end_exec = get_current_time();
/*
  snprintf(errbuf, BUF_SIZE, "Query %s: local execution started at time %lld - local execution ended at time %lld - elapsed time %lld", qs->uqi, start_exec, end_exec, end_exec - start_exec);
  idss_errmsg_r(errbuf);
*/

  snprintf(errbuf, BUF_SIZE, "%s%s%lld%s%s%lld%s%s%lld", SEP, LOCEXSTART, start_exec, SEP, LOCEXEND, end_exec, SEP, LOCEXDUR, end_exec - start_exec);
  print_result(qs->uqi, ip_address, errbuf);

  gettimeofday(&tt,NULL);
  secs=tt.tv_sec-secs;
  usecs=tt.tv_usec-usecs;
  double elapsed = secs+((double)usecs/1000000.0);

  double diff =  (qs->ttl - elapsed) / 1000.0;
  //snprintf(errbuf, BUF_SIZE, "DIFF: %f", diff);
  //idss_errmsg_r(errbuf);
  int retval = ttl_sleep(diff);

  query_termination(qs);

exit:
  if (qs->value)
  {
	free(qs->value);
	qs->value = NULL;
  }
  if (qs->key)
  {
	free(qs->key);
	qs->key = NULL;
  }
  if (qs->uqi)
  {
	free(qs->uqi);
	qs->uqi = NULL;
  }
  if (query)
  {
 	free (query);
	query = NULL;
  }
  freeResult(qs->e);

  free(qs);
  qs = NULL;

  if (subqs)
  {
	for (i = 0; i < limit; i++)
		if (*(subqs + i))
		{
			free(*(subqs + i));
			*(subqs + i) = NULL;
		}

	free(subqs);
	subqs = NULL;
  }

  if (stop)
  {
	xmlCleanupParser ();
	idss_clean_up();
	pthread_mutex_lock(&db_handle_mutex);
	close_connection(&db_handle);
	pthread_mutex_unlock(&db_handle_mutex);

 	exit (EXIT_FAILURE);
  }
  return NULL;
}


int ttl_sleep(double ttl_seconds)
{
 struct timespec tv;
 /* Construct the timespec from the number of whole seconds... */
 tv.tv_sec = (time_t) ttl_seconds;
 /* ... and the remainder in nanoseconds. */
 tv.tv_nsec = (long) ((ttl_seconds - tv.tv_sec) * 1e+9);

 while (1)
 {
  /* Sleep for the time specified in tv. If interrupted by a
 *     signal, place the remaining time left to sleep back into tv. */
  int rval = nanosleep (&tv, &tv);
  if (rval == 0)
   /* Completed the entire sleep time; all done. */
   return 0;
  else if (errno == EINTR)
   /* Interrupted by a signal. Try again. */
   continue;
  else
   /* Some other error; bail out. */
   return rval;
 }

 return 0;

}


/*	Thread that looks for queries that must be completed on the local node
 */

#if 0
void *query_check(void *arg)
{
  char errbuf[BUF_SIZE] = "";
  char sql[4096] = {0};
  query_answer answer;
  int rows;
  int result_query = 0;
  int i;
  /* id of the generated thread */
  pthread_t tid;
  query_exec_s *qs = NULL;
  long long current_time;
  int unslept;

  if (get_debug_flg() >= 1)
        idss_errmsg_r("Entering query_check thread");

  while(1)
	{
		init_answer(&answer);
		rows = 0;
		current_time = get_current_time();
		sprintf(sql, "SELECT uqi, value, sender_key FROM tb_query WHERE (arrive_time + ttl) < %lld AND completed = 0 AND sent_back = 0 AND failed = 0 AND local_exec = 1 ORDER BY (arrive_time + ttl) ASC", current_time);
		pthread_mutex_lock(&db_handle_mutex);

                result_query = exec_query(&db_handle, sql);
		if (result_query == SQLITE_OK)
			{
			 rows = n_tuples(&db_handle);
/*
                         snprintf(errbuf, BUF_SIZE-1, "n rows = %d", rows);
                         idss_errmsg_r(errbuf);
*/

		 	 if (rows > 0)
				single_query_retrieve_information(&db_handle, &answer);
			}

		pthread_mutex_unlock(&db_handle_mutex);

		if (result_query == SQLITE_OK)
			{

			 for (i = 0; i < rows; i++)
				{
				 qs = (query_exec_s *)calloc(1, sizeof(query_exec_s));
			 	 if (!qs)
				 	{
				 	 if (get_debug_flg() >= 1)
						idss_errmsg_r("Not enough memory!");
					  goto exit;
					}
				 qs->value = strdup(answer.records[(i * answer.num_field) + 1]);
				 qs->uqi = strdup(answer.records[i * answer.num_field]);
				 qs->key = strdup(answer.records[(i * answer.num_field) + 2]);

				 if ((!qs->value) || (!qs->uqi) || (!qs->key))
				 {
					if (get_debug_flg() >= 1)
						idss_errmsg_r("Not enough memory!");
					goto exit;
				 }

                                pthread_create(&tid, NULL, &query_termination, (void *)qs);
				}
			}
		else
			{
			 	if(result_query == SQLITE_MEMORY_ERROR)
      				{
         		 	 	if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					 goto exit;
      				}
      		 	 	else
      				{
      			 		if (get_debug_flg () >= 1)
						idss_errmsg_r("Thread query_check: an error occurred while trying getting queries for merging!");
      				}
			}

		free_answer(&answer);
		//waiting for 1/2 second
		unslept = 500000;
		while(unslept)
     			unslept = usleep (unslept);
	}
  exit:
	free_answer(&answer);
	idss_clean_up();
	xmlCleanupParser();
	pthread_mutex_lock(&db_handle_mutex);
	close_connection(&db_handle);
	pthread_mutex_unlock(&db_handle_mutex);

	exit(EXIT_FAILURE);
}
#endif


/*	This thread merges all files related to a query, and sends back the results to the previous node or executes the query
	if we are the originator node
 */
void *query_termination(void *arg)
{
   int rc = 0;
   int result = 0;
   query_s *qs = NULL;
   char finalfilename[1024] = {0};
   char finalfilename2[1024] = {0};
   int result_query = 0;
   char errbuf[BUF_SIZE] = {0};
   char sql[4096] = {0};
   int stop = 0;
   char **subqs = NULL;
   int i = 0;
   char temp[50] = {0};
   int j = 0;
   char resultfile[1024] = {0};	//name of result (single) file on the originator node
   char *firstf = NULL;		//name of the parent query result file (useful for nested execution)
   double *subqvalues = NULL;	//subqvalues[i] = final result of i-th subquery (useful on the originator node, with nested esecution, parent query is ignored)
   int limit = 0;
   long long start = 0, start2 = 0, start3 =0, end = 0, end2 = 0, end3 = 0;

   if (get_debug_flg() >= 1)
        	idss_errmsg_r("Entering query_termination thread");

   qs = (query_s *) arg;

  if(!qs){
    if (get_debug_flg () >= 1)
	 {
	   snprintf(errbuf, BUF_SIZE, "WARNING: query argument of query_termination() is NULL!");
       	 	idss_errmsg_r (errbuf);
        	idss_errmsg_r("Exiting query_termination thread");
		return NULL;
	 }
  }


  snprintf(errbuf, BUF_SIZE -1, "TH query_termination for: %s, uqi %s, key %s\n", qs->value, qs->uqi, qs->key);
  idss_errmsg_r(errbuf);

   start = get_current_time();
   start3 = start;

   snprintf(errbuf, BUF_SIZE, "%s%s%lld", SEP, COMPSTART, start3);
   print_result(qs->uqi, ip_address, errbuf);

   if ((qs->e.parentIsAgg > 0) && (qs->e.numOfAggSubs > 0))
   {
	//Our parent query contains agg functions and also subqueries with agg functions
	if (get_debug_flg () >= 1)
        	idss_errmsg_r ("This version of iDSS cannot execute this SQL SELECT statement: aggregate function in parent query and subqueries!");

	//Updating query status
   	sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   	pthread_mutex_lock(&db_handle_mutex);
   	result_query = exec_query(&db_handle, sql);
   	pthread_mutex_unlock(&db_handle_mutex);
   	if (result_query != SQLITE_OK)
   	{
		if(result_query == SQLITE_MEMORY_ERROR)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("exec_query error: not enough memory!");

			stop = 1;
      		}
      		else
      		{
      	 		if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
				idss_errmsg_r(errbuf);
			}
      		}
   	}

	goto exit;
   }

   if ((qs->e.numOfAggSubs > 0) && (qs->e.numOfSubs > qs->e.numOfAggSubs))
   {
	//We have subqueries with agg functions and subqueries without
	if (get_debug_flg () >= 1)
        	idss_errmsg_r ("This version of iDSS cannot execute this SQL SELECT statement: subqueries with aggregate functions and subqueries without aggregate functions inside the same nested query!");

	//Updating query status
   	sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   	pthread_mutex_lock(&db_handle_mutex);
   	result_query = exec_query(&db_handle, sql);
   	pthread_mutex_unlock(&db_handle_mutex);
   	if (result_query != SQLITE_OK)
   	{
		if(result_query == SQLITE_MEMORY_ERROR)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("exec_query error: not enough memory!");

			stop = 1;
      		}
      		else
      		{
      	 		if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
				idss_errmsg_r(errbuf);
			}
      		}
   	}

	goto exit;
   }

  if (qs->e.numOfAggSubs == 0)
   {
	//We have a query that must be executed only once
	limit = 1;
	subqs = (char **)calloc(limit, sizeof(char *));
	if (!subqs)
   	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
	*subqs = strdup(qs->value);
	if (!*subqs)
	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
  }
  else
  {
	//We have to decompose
	limit = qs->e.numOfSubs + 1;
  	//Getting parent query and subqueries
  	subqs = (char **)calloc(limit, sizeof(char *));
  	if (!subqs)
   	{
		if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Not enough memory!");

		stop = 1;
		goto exit;
   	}
	rc = getAllQueries(qs->e, subqs);
  	if (rc != DQF_OK)
   	{
		if (rc == DQF_NO_MEMORY)
      		{
        		if (get_debug_flg () >= 1)
        			idss_errmsg_r ("getAllQueries error: not enough memory!");

			stop = 1;
      		}
		else
		{
			if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "An error occourred while executing query %s! Error while decomposing query for nested execution! Function: getAllQueries", qs->uqi);
				idss_errmsg_r(errbuf);
			}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      			else
      				{
      	 				if (get_debug_flg () >= 1)
					{
		 				snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
		}
		goto exit;
   	}

	//Subquery results on the originator node (useful for nested execution)
        if (!strcmp(qs->key, "(NULL)"))
   	{
		subqvalues = (double *)calloc(limit - 1, sizeof(double));
		if (!subqvalues)
   		{
			if (get_debug_flg () >= 1)
        			idss_errmsg_r ("Not enough memory!");

			stop = 1;
			goto exit;
   		}
   	}
  }

   j = 0;
   for (i = 0; i < limit; i++)
   {
	if (*(subqs + i))
	{
		snprintf(finalfilename, 1024, "%s/%s_%d_temp", get_query_tempfiles_sd(), qs->uqi, i);
   		snprintf(finalfilename2, 1024, "%s/%s_%d", get_query_tempfiles_sd(), qs->uqi, i);

		if ((i == 0) && (!strcmp(qs->key, "(NULL)")))
		{
			 firstf = strdup(finalfilename2);	//Useful for nested execution
			 if (!firstf)
			 {
				if (get_debug_flg () >= 1)
        				idss_errmsg_r ("Not enough memory!");

				stop = 1;
				goto exit;
			 }
			 snprintf(resultfile, 1024, "%s/%s_f", get_query_results_sd(), qs->uqi); 	//name of result (single) file on the originator node
		}

		//temp file search key
		snprintf(temp, 50, "%s_%d", qs->uqi, i);

		//Merging all files related to this (sub)query
   		rc = merge_files(get_query_tempfiles_sd(), finalfilename, temp);

		if (rc != OK)
		{
			if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, sizeof(errbuf), "An error occurred while merging files for query %s (subquery %d)! Function: merge_files", qs->uqi, i);
		 		idss_errmsg_r(errbuf);
			}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      				else
      				{
      	 				if (get_debug_flg () >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
			goto exit;
		}

		//Applying aggregate functions (only if we are the originator node)
		if (!strcmp(qs->key, "(NULL)"))
 			rc = apply_aggr(finalfilename, finalfilename2, *(subqs + i));

		if (rc != OK)
		{
			if (rc == MEMORY_ERROR)
			{
				if (get_debug_flg () >= 1)
        				idss_errmsg_r ("apply_aggr error: not enough memory!");

				stop = 1;
			}
			else
			{
				if (get_debug_flg () >= 1)
	   	  		{
					snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing query %s (subquery %d)! Function: apply_aggr; subquery: \"%s\"", qs->uqi, i, *(subqs + i));
					idss_errmsg_r(errbuf);
		 		}
				//Updating query status
   				sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   				pthread_mutex_lock(&db_handle_mutex);
   				result_query = exec_query(&db_handle, sql);
   				pthread_mutex_unlock(&db_handle_mutex);
   				if (result_query != SQLITE_OK)
   				{
					if(result_query == SQLITE_MEMORY_ERROR)
      					{
        					if (get_debug_flg () >= 1)
        						idss_errmsg_r ("exec_query error: not enough memory!");

						stop = 1;
      					}
      					else
      					{
      	 					if (get_debug_flg () >= 1)
						{
		 					snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
							idss_errmsg_r(errbuf);
						}
      					}
   				}
			}
			goto exit;
		}

		//We are not the originator node, we have only to rename
		if (strcmp(qs->key, "(NULL)"))
			rc = rename(finalfilename, finalfilename2);

		if (rc != 0)
		{
			if (get_debug_flg () >= 1)
	   	  	{
				snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing query %s (subquery %d)! Error while renaming file %s to %s!", qs->uqi, i, finalfilename, finalfilename2);
				idss_errmsg_r(errbuf);
		 	}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      				else
      				{
      	 				if (get_debug_flg () >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
			goto exit;
		}

		if ((i > 0) && (!strcmp(qs->key, "(NULL)")) && (subqvalues != NULL))
		{
			//We have to populate the tree with subqueries final values
			rc = getSubqValue(finalfilename2, subqvalues + j);
			j++;
			if (rc != OK)
			{
				if (get_debug_flg () >= 1)
	   	  		{
					snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing query %s (subquery %d)! Error occourred while getting subquery final result! Function: getSubqValue", qs->uqi, i);
					idss_errmsg_r(errbuf);
		 		}
				//Updating query status
   				sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   				pthread_mutex_lock(&db_handle_mutex);
   				result_query = exec_query(&db_handle, sql);
   				pthread_mutex_unlock(&db_handle_mutex);
   				if (result_query != SQLITE_OK)
   				{
					if(result_query == SQLITE_MEMORY_ERROR)
      					{
        					if (get_debug_flg () >= 1)
        						idss_errmsg_r ("exec_query error: not enough memory!");

						stop = 1;
      					}
      					else
      					{
      	 					if (get_debug_flg () >= 1)
						{
							snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
							idss_errmsg_r(errbuf);
						}
      					}
   				}
				goto exit;
			}
		}
	}
   }

   //Now we have to do nested execution
   if ((!strcmp(qs->key, "(NULL)")) && (subqvalues != NULL))
   {
	//We are the originator node, and we must get a single file

	start2 = get_current_time();

	insertQueryResults(qs->e, subqvalues);
	rc = executeNested(firstf, qs->e, resultfile);
	if (rc != OK)
	{
		if (rc == MEMORY_ERROR)
		{
			if (get_debug_flg () >= 1)
				idss_errmsg_r ("executeNested error: not enough memory!");

			stop = 1;
		}
		else
		{
			if (get_debug_flg () >= 1)
	   	  	{
				snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing nested query %s! Function: executeNested", qs->uqi);
				idss_errmsg_r(errbuf);
		 	}
			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      				else
      				{
      	 				if (get_debug_flg () >= 1)
					{
		 				snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
		}
		goto exit;
	}

	end2 = get_current_time();
	snprintf(errbuf, BUF_SIZE, "Query %s: nested execution started at time %lld - nested execution ended at time %lld - elapsed time %lld", qs->uqi, start2, end2, end2 - start2);
	idss_errmsg_r(errbuf);

	snprintf(errbuf, BUF_SIZE, "%s%s%lld", SEP, NESTEDDUR, end2-start2);
	print_result(qs->uqi, ip_address, errbuf);

   }
   else if (!strcmp(qs->key, "(NULL)"))
   {
	//We are the originator node, but we don't have nested execution
	rc = rename(firstf, resultfile);
	if (rc != 0)
		{
			if (get_debug_flg () >= 1)
	   	  	{
				snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing query %s (subquery %d)! Error occourred while renaming file %s to %s!", qs->uqi, i, firstf, resultfile);
				idss_errmsg_r(errbuf);
		 	}

			//Updating query status
   			sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   			pthread_mutex_lock(&db_handle_mutex);
   			result_query = exec_query(&db_handle, sql);
   			pthread_mutex_unlock(&db_handle_mutex);
   			if (result_query != SQLITE_OK)
   			{
				if(result_query == SQLITE_MEMORY_ERROR)
      				{
        				if (get_debug_flg () >= 1)
        					idss_errmsg_r ("exec_query error: not enough memory!");

					stop = 1;
      				}
      				else
      				{
      	 				if (get_debug_flg () >= 1)
					{
						snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
						idss_errmsg_r(errbuf);
					}
      				}
   			}
			goto exit;
		}
   }

   //Updating query status
   sprintf(sql, "UPDATE tb_query SET completed = 1 WHERE uqi = '%s'", qs->uqi);
   pthread_mutex_lock(&db_handle_mutex);
   result_query = exec_query(&db_handle, sql);
   pthread_mutex_unlock(&db_handle_mutex);
   if (result_query != SQLITE_OK)
   {
	if(result_query == SQLITE_MEMORY_ERROR)
   	{
       		if (get_debug_flg () >= 1)
       			idss_errmsg_r ("exec_query error: not enough memory!");
		stop = 1;
   	}
   	else
      	{
      		if (get_debug_flg () >= 1)
		{
		 	snprintf(errbuf, BUF_SIZE, "An error occurred while updating status for query %s to \"completed\"!", qs->uqi);
		 	idss_errmsg_r(errbuf);
		}
		//Updating query status
   		sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   		pthread_mutex_lock(&db_handle_mutex);
   		result_query = exec_query(&db_handle, sql);
   		pthread_mutex_unlock(&db_handle_mutex);
   		if (result_query != SQLITE_OK)
   		{
			if(result_query == SQLITE_MEMORY_ERROR)
      			{
        			if (get_debug_flg () >= 1)
        				idss_errmsg_r ("exec_query error: not enough memory!");

				stop = 1;
      			}
      			else
      			{
      	 			if (get_debug_flg () >= 1)
				{
					snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
					idss_errmsg_r(errbuf);
				}
      			}
   		}
      	}
	goto exit;
   }

    end = get_current_time();
    //snprintf(errbuf, BUF_SIZE, "Query %s: merging started at time %lld - merging ended at time %lld - elapsed time %lld", qs->uqi, start, end, end - start);
    //idss_errmsg_r(errbuf);

    snprintf(errbuf, BUF_SIZE, "%s%s%lld", SEP, MERGINGDUR, end-start);
    print_result(qs->uqi, ip_address, errbuf);

   if (strcmp(qs->key, "(NULL)"))
   {
	//We are not the originator node, so we have to send back
	for (i = 0; i < limit; i++)
   	{
		if (*(subqs + i))
		{
			start2 = get_current_time();
			snprintf(finalfilename2, 1024, "%s/%s_%d", get_query_tempfiles_sd(), qs->uqi, i);

/*
                        snprintf(errbuf, BUF_SIZE -1, "...sending back file to %s, for uqi %s, with i=%d\n", qs->key, qs->uqi, i);
                        idss_errmsg_r(errbuf);
*/

                        rc = sendbackfile(finalfilename2, get_ig_hostname(), qs->uqi, i, qs->key);
			if (rc != OK)
			{
				if (rc == MEMORY_ERROR)
				{
					if (get_debug_flg () >= 1)
        					idss_errmsg_r ("sendbackfile error: not enough memory!");

					stop = 1;
				}
				else
				{
					if (get_debug_flg () >= 1)
	   	  			{
						snprintf(errbuf, sizeof(errbuf), "An error occurred while trying executing query %s (subquery %d)! Error while sending file %s to node %s!", qs->uqi, i, finalfilename2, qs->key);
						idss_errmsg_r(errbuf);
		 			}

					//Updating query status
   					sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   					pthread_mutex_lock(&db_handle_mutex);
   					result_query = exec_query(&db_handle, sql);
   					pthread_mutex_unlock(&db_handle_mutex);
   					if (result_query != SQLITE_OK)
   					{
						if(result_query == SQLITE_MEMORY_ERROR)
      						{
        						if (get_debug_flg () >= 1)
        							idss_errmsg_r ("exec_query error: not enough memory!");

							stop = 1;
      						}
      						else
      						{
      	 						if (get_debug_flg () >= 1)
							{
								snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
								idss_errmsg_r(errbuf);
							}
      						}
   					}
				}
				goto exit;
			}
			end2 = get_current_time();
			snprintf(errbuf, BUF_SIZE, "Query %s: sending file %d started at time %lld - ended at time %lld - elapsed time %lld", qs->uqi, i, start2, end2, end2 - start2);
			idss_errmsg_r(errbuf);
		}
	}

   	//Updating query status
   	sprintf(sql, "UPDATE tb_query SET sent_back = 1 WHERE uqi = '%s'", qs->uqi);
   	pthread_mutex_lock(&db_handle_mutex);
   	result_query = exec_query(&db_handle, sql);
   	pthread_mutex_unlock(&db_handle_mutex);
   	if (result_query != SQLITE_OK)
   		{
		 if(result_query == SQLITE_MEMORY_ERROR)
   			{
       				if (get_debug_flg () >= 1)
       					idss_errmsg_r ("exec_query error: not enough memory!");
				stop = 1;
   			}
   		 else
      			{
      				if (get_debug_flg () >= 1)
				{
		 			snprintf(errbuf, BUF_SIZE, "An error occurred while updating status for query %s to \"sent back\"!", qs->uqi);
		 			idss_errmsg_r(errbuf);
				}
				//Updating query status
   				sprintf(sql, "UPDATE tb_query SET failed = 1 WHERE uqi = '%s'", qs->uqi);
   				pthread_mutex_lock(&db_handle_mutex);
   				result_query = exec_query(&db_handle, sql);
   				pthread_mutex_unlock(&db_handle_mutex);
   				if (result_query != SQLITE_OK)
   				{
					if(result_query == SQLITE_MEMORY_ERROR)
      					{
        					if (get_debug_flg () >= 1)
        						idss_errmsg_r ("exec_query error: not enough memory!");

						stop = 1;
      					}
      					else
      					{
      	 					if (get_debug_flg () >= 1)
						{
							snprintf(errbuf, BUF_SIZE, "An error occurred while updating query %s status to \"failed\"!", qs->uqi);
							idss_errmsg_r(errbuf);
						}
      					}
   				}
      			}
		 goto exit;
   		}
   }

//Now deleting
   for (i = 1; i < limit; i++)
   {
	if (*(subqs + i))
	{
		snprintf(finalfilename2, 1024, "%s/%s_%d", get_query_tempfiles_sd(), qs->uqi, i);
		rc = remove(finalfilename2);
		if (rc)
		{
			if (get_debug_flg() >= 1){
                  		sprintf(errbuf, "Error! Remove error: file %s", finalfilename2);
		  		idss_errmsg_r(errbuf);
				}
		}
	}
   }

   if ((strcmp(qs->key, "(NULL)")) || ((!strcmp(qs->key, "(NULL)")) && (subqvalues != NULL)))
   {
	//We aren't the originator node, or we are the originator node but we have nested execution
	snprintf(finalfilename2, 1024, "%s/%s_%d", get_query_tempfiles_sd(), qs->uqi, 0);
	rc = remove(finalfilename2);
	if (rc)
	{
		if (get_debug_flg() >= 1){
               		sprintf(errbuf, "Error! Remove error: file %s", finalfilename2);
	  		idss_errmsg_r(errbuf);
			}
	}
   }

   end3 = get_current_time();
   snprintf(errbuf, BUF_SIZE, "Query %s: completed; started at time %lld - ended at time %lld - Total elapsed: %lld", qs->uqi, start3, end3, end3 - start3);
   idss_errmsg_r(errbuf);

   snprintf(errbuf, BUF_SIZE, "%s%s%lld%s%s%lld", SEP, COMPEND, end3, SEP, COMPDUR, end3-start3);
   print_result(qs->uqi, ip_address, errbuf);

exit:

  if (subqs)
  	{
	 for (i = 0; i < limit; i++)
		if (*(subqs + i))
		{
			free(*(subqs + i));
			*(subqs + i) = NULL;
		}

	 free(subqs);
	 subqs = NULL;
  	}

  if (subqvalues)
	{
	 free(subqvalues);
	 subqvalues = NULL;
	}
  if (firstf)
	{
		free(firstf);
		firstf = NULL;
	}
   if (stop)
   	{
		idss_clean_up();
		xmlCleanupParser();
		pthread_mutex_lock(&db_handle_mutex);
		close_connection(&db_handle);
		pthread_mutex_unlock(&db_handle_mutex);

		exit(EXIT_FAILURE);
	}
   return NULL;
}
