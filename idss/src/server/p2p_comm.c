#include <stdlib.h>
#include <pthread.h>
#include "idss_data.h"
#include "idss_unix.h"
#include "utils.h"
#include "data_access_structures.h"
#include "data_access_func.h"
#include "idss_p2p.h"
#include "p2p_comm.h"
#include "p2p_utils.h"
#include "dq_funcs.h"

#include "wrapper/query_defs.h"

extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;
extern double redfact;
extern char ip_address[25];

/*
int
get_payload_params(char *payload, Key *sender_key, char **uqi, int *ttl, char **sql)
{
    int i, j;
    int l1 = strlen(payload);
    char *temp = NULL;

    //Discarding
    for (i = 0; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);

    //Getting the sender's key
    for (j = i+1; (j < l1) && (payload[j] != CH_DELIMETER); j++);
    temp = (char *) calloc((j - i + 1), sizeof(char));
    if (!temp)
	return 1;
    strncpy(temp, payload + i, (j - i));
    str_to_key(temp, sender_key);
    free(temp);
    temp = NULL;

    //Getting the uqi
    for (i = j+1; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);
    for (j = i+1; (j < l1) && (payload[j] != CH_DELIMETER); j++);
    *uqi = (char *) calloc((j - i + 1), sizeof(char));
    if (*uqi == NULL)
	 return 1;
    strncpy(*uqi, payload + i, (j - i));

    //Getting the ttl value
    temp = (char *) calloc(30, sizeof(char));
    if (!temp)
	{
	 free (*uqi);
	 *uqi = NULL;
	 return 1;
	}
    for (i = j+1; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);
    j = 0;
    for (; (i < l1) && (payload[i] != CH_DELIMETER); i++)
	{
	 temp[j] = payload[i];
	 j++;
	}
    temp[j] = '\0';
    *ttl = (int)strtol(temp, (char **)NULL, 10);
    free (temp);
    temp = NULL;

    //Getting the sql
    for (; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);

    if(l1 - i + 1){
      *sql = (char *) calloc(l1 - i + 1, sizeof(char));
      if (*sql == NULL)
	    {
	      free(*uqi);
	      *uqi = NULL;
	      return 1;
	    }
    }
    else{
	      free(*uqi);
	      *uqi = NULL;
	      return 1;
    }

    strncpy(*sql, payload + i, l1 - i);
    return 0;
}
*/

int
get_payload_params2(char *payload, char **sender_name, char **uqi, int *subq_n, char **result)
{
    int i, j;
    int l1 = strlen(payload);
    char *temp = NULL;

    //Discarding
    for (i = 0; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);

    //Getting the sender's hostname
    for (j = i+1; (j < l1) && (payload[j] != CH_DELIMETER); j++);
    *sender_name = (char *) calloc((j - i + 1), sizeof(char));
    if (*sender_name == NULL)
	return 1;
    strncpy(*sender_name, payload + i, (j - i));

    //Getting the query UQI
    for (i = j+1; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);
    for (j = i+1; (j < l1) && (payload[j] != CH_DELIMETER); j++);
    *uqi = (char *) calloc((j - i + 1), sizeof(char));
    if (*uqi == NULL)
	{
	 free (*sender_name);
	 *sender_name = NULL;
	 return 1;
	}
    strncpy(*uqi, payload + i, (j - i));

    //Getting subquery number
    temp = (char *) calloc(30, sizeof(char));
    if (!temp)
	{
	 free (*sender_name);
	 *sender_name = NULL;
	 free (*uqi);
	 *uqi = NULL;
	 return 1;
	}
    for (i = j+1; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);
    j = 0;
    for (; (i < l1) && (payload[i] != CH_DELIMETER); i++)
	{
	 temp[j] = payload[i];
	 j++;
	}
    temp[j] = '\0';
    *subq_n = (int)strtol(temp, (char **)NULL, 10);
    free (temp);
    temp = NULL;

    //Getting the query result
    for (; (i < l1) && ((payload[i] == ' ') || (payload[i] == CH_DELIMETER)); i++);

    if(l1 - i + 1){

      *result = (char *) calloc(l1 - i + 1, sizeof(char));
      if (*result == NULL)
	    {
	      free(*sender_name);
	      *sender_name = NULL;
	      free (*uqi);
	      *uqi = NULL;
	      return 1;
	    }

    }
    else{
	      free(*sender_name);
	      *sender_name = NULL;
	      free (*uqi);
	      *uqi = NULL;
	      return 1;
    }

    strncpy(*result, payload + i, l1 - i);
    return 0;
}


//This function returns 1 if the current node has already seen a msg with the specified query identifier; 0 otherwise
int
check_query_id(char *query_id)
{
    char sqlh[512] = {0};
    int rows = 0;
    int result_query;
    char errbuf[BUF_SIZE] = {0};

    sprintf(sqlh, "SELECT uqi FROM tb_query WHERE uqi = '%s'", query_id);
    pthread_mutex_lock(&db_handle_mutex);
    result_query = exec_query(&db_handle, sqlh);
    if (result_query == SQLITE_OK)
			rows = n_tuples(&db_handle);
    pthread_mutex_unlock(&db_handle_mutex);

    if (result_query != SQLITE_OK) {
		if (result_query == SQLITE_MEMORY_ERROR) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("exec_query error: not enough memory!");
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);

			exit(EXIT_FAILURE);
		} else {
			if (get_debug_flg() >= 1)
				{

				 snprintf(errbuf, BUF_SIZE, "Error %d submitting query to dbms! Query: \"%s\"", result_query,  sqlh);
				 idss_errmsg_r(errbuf);
				}
			return 1;
		}
	}

    if (rows > 0)
	return 1;
    else
  	return 0;
}




/**
 *
 * @param uqi
 * @param sql
 * @param ttl
 * @param sender - hostname ip:port del sender della query
 * @param my_key
 */
void on_query_received(void *args) {

    query_received_args *data;
    data = (query_received_args*) malloc(sizeof(query_received_args));
    data = (query_received_args*)args;

    char *sql = strdup(data->sql);
    char *uqi = strdup(data->uqi);
    double ttl = data->ttl;
    long origin_key = data->origin_key;
    char *sender = strdup(data->sender);

    char errbuf[BUF_SIZE] = {0};
    sprintf(errbuf, "=== ON QUERY WITH UQI %s RECEIVED CALLBACK. Msg: ", uqi);
    idss_errmsg_r(errbuf);
    idss_errmsg_r(sql);

    long long arrive_time; /**< tempo di arrivo della query */

    char sqlh[10000] = {0};
    int result_query;

    /* id of the generated thread */
    pthread_t tid;

    query_s *qs = (query_s *)calloc(1, sizeof(query_s));
    if(!qs){

   	if (get_debug_flg() >= 1)
        	idss_errmsg_r("on_query_recevied(): not enough memory!");

        idss_clean_up();
        pthread_mutex_lock(&db_handle_mutex);
        close_connection(&db_handle);
        pthread_mutex_unlock(&db_handle_mutex);

	return;
    }

    if (check_query_id(uqi) == 0)
    {
            arrive_time = get_current_time();
            snprintf(errbuf, BUF_SIZE, "Query with uqi='%s' received at time %lld by node %s", uqi, arrive_time, sender);
            idss_errmsg_r(errbuf);

            snprintf(errbuf, BUF_SIZE, "%s%s%ld%s%s%s%s%s%s%s%lld", MYKEY, SEP, origin_key, UQI, uqi, SEP, RCVFROM, sender, SEP, RCVAT, arrive_time);
            print_result(uqi, ip_address, errbuf);

            snprintf(sqlh, 10000,
                    "INSERT INTO tb_query (uqi, value, ttl, arrive_time, sender_key) VALUES ('%s', '%s', %f, %lld, '%s')",
                            uqi, sql, ttl, arrive_time, sender);

            pthread_mutex_lock(&db_handle_mutex);
                result_query = exec_query(&db_handle, sqlh);
            pthread_mutex_unlock(&db_handle_mutex);

            if (result_query != SQLITE_OK) {
                    if (result_query == SQLITE_MEMORY_ERROR) {
                            if (get_debug_flg() >= 1)
                                    idss_errmsg_r("exec_query error: not enough memory!");
                            idss_clean_up();
                            pthread_mutex_lock(&db_handle_mutex);
                                close_connection(&db_handle);
                            pthread_mutex_unlock(&db_handle_mutex);

                            if (uqi)
                            {
                                    free(uqi);
                                    uqi = NULL;
                            }
                            if (sql)
                            {
                                    free(sql);
                                    sql = NULL;
                            }
			    if(qs)
			    {
				free(qs);
				qs = NULL;
			    }
                            exit(EXIT_FAILURE);
                    } else {
                            if (get_debug_flg() >= 1) {
                                    sprintf(errbuf, "Error submitting query: %s", sqlh);
                                    idss_errmsg_r(errbuf);
                            }
                            if (uqi)
                            {
                                    free(uqi);
                                    uqi = NULL;
                            }
                            if (sql)
                            {
                                    free(sql);
                                    sql = NULL;
                            }
			    if(qs)
			    {
				free(qs);
				qs = NULL;
			    }
                    return;
                    }//fi SQLITE_MEMORY_ERROR
            }//fi !SQLITE_OK


            qs->uqi = strdup(uqi);
            qs->value = strdup(sql);
	    qs->key = strdup(sender);
	    qs->ttl = (double) ttl;
            if ((!qs->uqi) || (!qs->value) || (!qs->key))
            {
                    if (get_debug_flg() >= 1)
                            idss_errmsg_r("Not enough memory");
                    idss_clean_up();
                    pthread_mutex_lock(&db_handle_mutex);
                    close_connection(&db_handle);
                    pthread_mutex_unlock(&db_handle_mutex);

                    if (qs->uqi)
                    {
                            free(qs->uqi);
                            qs->uqi = NULL;
                    }
                    if (qs->key)
                    {
                            free(qs->key);
                            qs->key = NULL;
                    }
                    if (qs->value)
                    {
                            free(qs->value);
                            qs->value = NULL;
                    }
	  	    if(qs)
		    {
			free(qs);
			qs = NULL;
		    }
                    if (uqi != NULL)
                    {
                            free (uqi);
                             uqi = NULL;
                    }
                    if (sql)
                    {
                            free (sql);
                            sql = NULL;
                    }
                    exit(EXIT_FAILURE);
            }

            pthread_create(&tid, NULL, &query_execution, (void *)(qs));

    }//fi check_uqi

}


/**
 *
 * @param sender - hostname del mittente
 * @param uqi - uqi della query cui la risposta appartiene
 * @param subq_n - subquery_number(?)
 * @param msg - la risposta ricevuta
 */
void on_response_received(void *args) {
    /* idss_errmsg_r("\n\t=== ON RESPONSE RECEIVED CALLBACK. Msg: "); */
    /* idss_errmsg_r(msg); */

    response_received_args *data;
    data = (response_received_args*) malloc(sizeof(response_received_args));
    data = (response_received_args*)args;

    char *sender = strdup(data->sender);
    char *uqi = strdup(data->uqi);
    int subq_n = data->subq_n;
    char *msg = strdup(data->msg);

    FILE 	*file_a = NULL;
    char	*random_v = NULL;
    int		result = 0;
    char	filename[1024] = {0};
    int		error = 0;
    char	errbuf[1024] = {0};
    long long	start = 0, end = 0;

    start = get_current_time();

    get_random_value (&random_v);
    if (random_v == NULL)
    {
        if (get_debug_flg() >= 1)
            idss_errmsg_r("exec_query error: not enough memory!");
            error = 1;
            goto exit;
        }

        //1024-1 char per il carattere di terminazione delle C-string
        snprintf(filename, 1023, "%s/%s_%d_%s_%s", get_query_tempfiles_sd(), uqi, subq_n, sender, random_v);

        file_a = fopen(filename, "w");
        if (!file_a)
        {
            if (get_debug_flg() >= 1)
            {
               sprintf(errbuf, "Permission denied: can not create file %s", filename);
               idss_errmsg_r(errbuf);
            }
            goto exit;
        }

        fprintf(file_a, "%s", msg);

        if (fclose(file_a))
        {
            if (get_debug_flg() >= 1){
                sprintf(errbuf, "Error! Close error: file %s", filename);
                idss_errmsg_r(errbuf);
            }

            file_a = NULL;
            goto exit;
        }

        file_a = NULL;

         end = get_current_time();
         snprintf(errbuf, 1024, "Query %s: receving file %d started at time %lld - ended at time %lld - elapsed time: %lld", uqi, subq_n, start, end, end - start);
         idss_errmsg_r(errbuf);

exit:
        if (random_v)
                {
                 free(random_v);
                 random_v = NULL;
                }
        if (sender)
                {
                 free(sender);
                 sender = NULL;
                }
        if (uqi)
                {
                 free(uqi);
                 uqi = NULL;
                }
        if (msg)
                {
                 free(msg);
                 msg = NULL;
                }
        if (file_a)
                {
                 fclose(file_a);
                 file_a = NULL;
                }
        if (error)
                {
                 idss_clean_up();
                 xmlCleanupParser();
                 pthread_mutex_lock(&db_handle_mutex);
                    close_connection(&db_handle);
                 pthread_mutex_unlock(&db_handle_mutex);

                 exit(EXIT_FAILURE);
                }

}
