#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "libxml/xmlreader.h"
#include "libxml/xmlwriter.h"
#include "help_p2p_thrs.h"
#include "data_access_structures.h"
#include "data_access_func.h"
#include "utils.h"
#include "parser_result.h"
#include "dq_funcs.h"
#include "db.h"
#include "p2p_comm.h"



#include "wrapper/d1ht_wrapper.h"
extern d1ht_h* d1ht_overlay;


extern pthread_mutex_t db_handle_mutex, merge_mutex;
extern sqlite_handle db_handle;

static char *mystrndup(const char *s, size_t n) {
    size_t len = strlen(s);
    char *ret;

    if (len <= n)
        return strdup(s);

    ret = malloc(n + 1);
    if (ret) {
        strncpy(ret, s, n);
        ret[n] = '\0';
    }
    return ret;
}

/* Splits an avg function in original_query into sum and count functions
 */
int remove_avg(char *original_query, char **result_query) {
    regex_t r[4];
    char *pattern[4];
    int i = 0, j = 0;
    int status[4];
    size_t nmatch = 1;
    regmatch_t pmatch[4];
    char *save_ptr = NULL;
    char delims[] = ",";
    char *from = NULL; //"from tb where...." in original_query
    char *original_query2 = NULL; //"select c1,...,cn" in original_query
    char *start = NULL; //memory for original_query2 address
    char *originalfield = NULL; //fieldname in original_query
    char *originalfieldname = NULL; //fieldname on which we compute avg
    char newfieldname[256] = {0};
    int return_value = OK;
    char errbuf[BUF_SIZE] = {0};

    if (get_debug_flg() >= 1){
        idss_errmsg_r("Entering remove_avg function");
        snprintf(errbuf, BUF_SIZE, "Original query: \"%s\"!", original_query);
        idss_errmsg_r(errbuf);
    }

    for (i = 0; i < 4; i++) {
        pattern[i] = NULL;
        status[i] = 1;
    }

    *result_query = (char *) (calloc(1024, sizeof (char)));
    if (!*result_query)
        return MEMORY_ERROR;

    pattern[0] = strdup("select[[:space:]]+"); //select
    pattern[1] = strdup("[[:space:]]*avg[[:space:]]*[(][[:space:]]*"); //avg(
    pattern[2] = strdup("[[:space:]]+from[[:space:]]"); //from
    pattern[3] = strdup("[[:space:]]*[)]"); //)

    for (i = 0; i < 4; i++) {
        if (pattern[i] == NULL) {
            for (j = 0; j < 4; j++) {
                if (pattern[j]) {
                    free(pattern[j]);
                    pattern[j] = NULL;
                }
            }

            return MEMORY_ERROR;
        }
    }

    //Registering regular expressions
    for (i = 0; i < 4; i++) {
        if (regcomp(&r[i], pattern[i], REG_EXTENDED | REG_ICASE) != 0) {

            if (get_debug_flg() >= 1)
                idss_errmsg_r("Error while registering regular expressions!");

            for (j = 0; j < i; j++)
                regfree(&r[j]);

            for (j = 0; j < 4; j++) {
                free(pattern[j]);
                pattern[j] = NULL;
            }

            return GENERIC_ERROR;
        }
    }

    //Getting from clause
    status[2] = regexec(&r[2], original_query, nmatch, &pmatch[2], 0);
    if (status[2] == 0) {
        from = strdup(original_query + pmatch[2].rm_so);
        original_query2 = mystrndup(original_query, (size_t) pmatch[2].rm_so);
        if ((!from) || (!original_query2)) {
            return_value = MEMORY_ERROR;
            goto exit;
        }
    } else {
        original_query2 = strdup(original_query);
        if (!original_query2) {
            return_value = MEMORY_ERROR;
            goto exit;
        }
    }
    start = original_query2;

    //Cutting select clause
    status[0] = regexec(&r[0], original_query2, nmatch, &pmatch[0], 0);
    if (status[0] == 0) {
        strncat(*result_query, original_query2, pmatch[0].rm_eo); //result_query = "select"
        original_query2 += pmatch[0].rm_eo;
    }

    i = 0;
    originalfield = strtok_r(original_query2, delims, &save_ptr);
    while (originalfield != NULL) {
        if (i > 0)
            strcat(*result_query, ", ");

        //Looking for avg
        status[1] = regexec(&r[1], originalfield, nmatch, &pmatch[1], 0);
        if (status[1] == 0) {
            //Found avg field, we have to split
            originalfield += pmatch[1].rm_eo; //Let's move after "avg("
            //Looking for )
            status[3] = regexec(&r[3], originalfield, nmatch, &pmatch[3], 0);
            if (status[3] != 0) {
                if (get_debug_flg() >= 1) {
                    snprintf(errbuf, BUF_SIZE, "Error while analizing query: closed bracket related to avg function not found in query %s", original_query);
                    idss_errmsg_r(errbuf);
                }
                return_value = GENERIC_ERROR;
                goto exit;
            }
            originalfieldname = mystrndup(originalfield, (size_t) (pmatch[3].rm_so)); //fieldname on which we compute avg
            if (!originalfieldname) {
                return_value = MEMORY_ERROR;
                goto exit;
            }
            snprintf(newfieldname, sizeof (newfieldname), "SUM(%s) AS sumforavg, COUNT(%s) AS countforavg", originalfieldname, originalfieldname);
        } else {
            snprintf(newfieldname, sizeof (newfieldname), "%s", originalfield);
        }

        if (originalfieldname) {
            free(originalfieldname);
            originalfieldname = NULL;
        }

        strcat(*result_query, newfieldname);
        originalfield = strtok_r(NULL, delims, &save_ptr);
        i++;
    }

exit:

    if (from) {
        if (*result_query)
            strcat(*result_query, from);
        free(from);
        from = NULL;
    }

    if (start) {
        free(start);
        start = NULL;
        original_query2 = NULL;
    }

    for (j = 0; j < 4; j++) {
        regfree(&r[j]);
        free(pattern[j]);
        pattern[j] = NULL;
    }

    if (originalfield) {
        free(originalfield);
        originalfield = NULL;
    }

    if (originalfieldname) {
        free(originalfieldname);
        originalfieldname = NULL;

    }

    if (get_debug_flg() >= 1){
        idss_errmsg_r("Exiting remove_avg function");
        snprintf(errbuf, BUF_SIZE, "Rewritten query: \"%s\"!", *result_query);
        idss_errmsg_r(errbuf);
    }

    return return_value;
}

/* Local execution */
int local_exec(char *query, char *filename) {
    int result_query = 0;
    query_answer answer;
    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr cur = NULL;
    xmlNodePtr cur2 = NULL;
    xmlNodePtr cur3 = NULL;
    int n_rows = 0;
    int j = 0, k = 0;
    char temp[20] = {0};
    char errbuf[BUF_SIZE] = {0};
    int return_value = OK;

    if (get_debug_flg() >= 1){
        idss_errmsg_r("Entering local_exec function");
        snprintf(errbuf, BUF_SIZE, "Trying executing query: \"%s\"!", query);
        idss_errmsg_r(errbuf);
    }

    init_answer(&answer);

    pthread_mutex_lock(&db_handle_mutex);
    result_query = exec_query(&db_handle, query);
    if (result_query == SQLITE_OK) {
        n_rows = n_tuples(&db_handle);
        single_query_retrieve_information(&db_handle, &answer);
    }
    pthread_mutex_unlock(&db_handle_mutex);

    if (result_query != SQLITE_OK) {
        if (result_query == SQLITE_MEMORY_ERROR)
            return MEMORY_ERROR;
        else {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "exec_query error: an error occurred while trying executing query: \"%s\"!", query);
                idss_errmsg_r(errbuf);
            }
            return GENERIC_ERROR;
        }
    }

    doc = xmlNewDoc(BAD_CAST "1.0");
    root = xmlNewNode(NULL, (const xmlChar *) "RECORDSET");
    xmlDocSetRootElement(doc, root);
    cur = xmlNewChild(root, NULL, (const xmlChar *) "SCH", NULL);
    for (j = 0; answer.schema[j] != NULL; j++) {
        cur2 = xmlNewChild(cur, NULL, (const xmlChar *) "ATT", (const xmlChar*) (answer.schema[j]));
        snprintf(temp, sizeof (temp), "%d", answer.type[j]);
        switch (answer.type[j]) {
            case SQLITE_INTEGER:
                snprintf(temp, sizeof (temp), "integer");
                break;
            case SQLITE_FLOAT:
                snprintf(temp, sizeof (temp), "float");
                break;
            case SQLITE_BLOB:
                snprintf(temp, sizeof (temp), "blob");
                break;
            case SQLITE_NULL:
                snprintf(temp, sizeof (temp), "null");
                break;
            case SQLITE_TEXT:
                snprintf(temp, sizeof (temp), "string");
                break;
            default:
                snprintf(temp, sizeof (temp), "unknown");
                break;
        }
        xmlNewProp(cur2, (const xmlChar *) "TYPE", (const xmlChar*) temp);
    }

    cur = xmlNewChild(root, NULL, (const xmlChar *) "RECS", NULL);
    snprintf(temp, sizeof (temp), "%d", answer.num_record);
    xmlNewProp(cur, (const xmlChar *) "N", (const xmlChar *) temp);
    for (k = 0; k < answer.num_record; k++) {
        cur2 = xmlNewChild(cur, NULL, (const xmlChar *) "R", NULL);
        for (j = 0; j < answer.num_field; j++)
            cur3 = xmlNewChild(cur2, NULL, (const xmlChar *) "F", (const xmlChar*) (answer.records[(k * answer.num_field) + j]));
    }

    if (root) {
        if (xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1) == -1) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while saving query result file: %s! Query: \"%s\"", filename, query);
                idss_errmsg_r(errbuf);
            }
            return_value = GENERIC_ERROR;
            goto exit;
        } else {
            return_value = OK;
        }
    } else {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error while creating query result doc! Query: \"%s\"", query);
            idss_errmsg_r(errbuf);
        }
        return_value = GENERIC_ERROR;
    }

exit:
    xmlFreeDoc(doc);
    free_answer(&answer);
    return return_value;
}

/*	Merges all files related to a (sub)query
 */
int merge_files(char *directory, char *finalfilename, char *uqi)
{
   DIR *dirp = NULL;
   struct dirent *dp = NULL;
   char filename[1024] = {0};
   char errbuf[BUF_SIZE] = {0};
   int sc = 0;
   int n_rec = 0;
   char temp[15] = {0};

   xmlDocPtr doc = NULL;
   xmlDocPtr result = NULL;
   xmlNodePtr root = NULL;
   xmlNodePtr sch = NULL;
   xmlNodePtr recs = NULL;
   xmlNodePtr r = NULL;
   xmlChar *f = NULL;
   xmlNodePtr cur = NULL;
   xmlNodePtr cur2 = NULL;
   xmlNodePtr cur3 = NULL;
   xmlNodePtr cur_r = NULL;
   xmlChar *att = NULL;
   xmlChar *type = NULL;
   xmlChar *n = NULL;

	pthread_mutex_lock(&merge_mutex);

   if (get_debug_flg () >= 1)
        		idss_errmsg_r ("Entering merge_files function");

   result = xmlNewDoc(BAD_CAST "1.0");
   root = xmlNewNode (NULL, (const xmlChar *) "RECORDSET");
   xmlDocSetRootElement (result, root);
   sch = xmlNewChild (root, NULL, (const xmlChar *) "SCH", NULL);
   recs = xmlNewChild (root, NULL, (const xmlChar *) "RECS", NULL);

   dirp = opendir (directory);
   if (dirp) {
		while ((dp = readdir (dirp)) != NULL)
    		{
			snprintf(filename, sizeof(filename), "%s/%s", directory, dp->d_name);
			if ((strstr(filename, uqi)) && (strcmp(filename, finalfilename)))
			{
	 			//Found a file that must be parsed
	 			if (get_debug_flg () >= 1)
				{
		 			snprintf(errbuf, BUF_SIZE, "Parsing file: %s", filename);
        	 			idss_errmsg_r (errbuf);
				}

				doc = xmlParseFile(filename);

	 			if (doc == NULL )
				{
					if (get_debug_flg () >= 1)
					{
		 	 		 	snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", filename);
        	 	 		 	idss_errmsg_r (errbuf);
					}

		 			xmlFreeDoc(result);
					closedir(dirp);
					pthread_mutex_unlock(&merge_mutex);
		 			return GENERIC_ERROR;
				}

				cur = xmlDocGetRootElement(doc);	//cur = RECORDSET
	 			if (cur == NULL)
				{
					if (get_debug_flg () >= 1)
					{
		 	 			snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", filename);
        	 	 			idss_errmsg_r (errbuf);
					}
					xmlFreeDoc(result);
					xmlFreeDoc(doc);
					closedir(dirp);
					pthread_mutex_unlock(&merge_mutex);
					return GENERIC_ERROR;
				}

	 			cur = cur->xmlChildrenNode;
	 			while (cur != NULL)
				{
		 			if ((!xmlStrcmp(cur->name, (const xmlChar *) "SCH")) && (sc == 0))
					{
		 				cur2 = cur->xmlChildrenNode;	//ATTs
		 				while (cur2 != NULL)
						{
			 				if (!xmlStrcmp(cur2->name, (const xmlChar *)"ATT"))
							{
			 	 				att = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
				 				type = xmlGetProp(cur2, (const xmlChar *)"TYPE");
				 				cur_r = xmlNewChild (sch, NULL, (const xmlChar *) "ATT", att);
				 				xmlNewProp (cur_r, (const xmlChar *)"TYPE", type);
								if (att)
								{
				 				 	xmlFree(att);
						 			att = NULL;
								}
								if (type)
								{
				 		 			xmlFree(type);
						 			type = NULL;
								}
							}
				 			cur2 = cur2->next;
						}
		 				sc = 1;	//SCH copied
	 				}

					if (!xmlStrcmp(cur->name, (const xmlChar *) "RECS"))
					{
						n = xmlGetProp(cur, (const xmlChar *)"N");
						if (n)
						{
							n_rec += (int) strtol((char *)n, (char **)NULL, 10);
				 			xmlFree(n);
				 			n = NULL;
						}
						cur2 = cur->xmlChildrenNode;	//Rs
						while (cur2 != NULL)
						{
			 				if (!xmlStrcmp(cur2->name, (const xmlChar *)"R"))
							{
				 				r = xmlNewChild (recs, NULL, (const xmlChar *) "R", NULL);
								cur3 = cur2->xmlChildrenNode;	//Fs
				 				while (cur3 != NULL)
								{
					 				if (!xmlStrcmp(cur3->name, (const xmlChar *)"F"))
									{
						 				f = xmlNodeListGetString(doc, cur3->xmlChildrenNode, 1);
						 				cur_r = xmlNewChild (r, NULL, (const xmlChar *) "F", f);
						 				if (f)
										{
							 				xmlFree(f);
							 				f = NULL;
										}
									}
									cur3 = cur3->next;
								}
							}
							cur2 = cur2->next;
						}
					}

					cur = cur->next;
				}

				xmlFreeDoc(doc);
			}
		}

		closedir(dirp);

		snprintf(temp, sizeof(temp), "%d", n_rec);
                xmlNewProp (recs, (const xmlChar *)"N", (xmlChar *)temp);

   		if (xmlSaveFormatFileEnc(finalfilename, result, "UTF-8", 1) == -1)
		{
			if (get_debug_flg () >= 1)
			{
		 		snprintf(errbuf, BUF_SIZE, "Error while saving query result file: %s", finalfilename);
        	 		idss_errmsg_r (errbuf);
			}
			xmlFreeDoc (result);
			pthread_mutex_unlock(&merge_mutex);
	 		return GENERIC_ERROR;
		}

		xmlFreeDoc (result);

		//Deleting
		dirp = opendir (directory);
   		if (dirp)
		{
		 	while ((dp = readdir (dirp)) != NULL)
    				{
				 snprintf(filename, sizeof(filename), "%s/%s", directory, dp->d_name);
			 	 if ((strstr(filename, uqi)) && (strcmp(filename, finalfilename)))
					{
				 	 if(remove(filename))
	      					{
                			 	if (get_debug_flg() >= 1){
                  					sprintf(errbuf, "Error! Remove error: file %s", filename);
		  			 		idss_errmsg_r(errbuf);
							}
	      					}
					else {
						if (get_debug_flg() >= 1){
                                                        sprintf(errbuf, "Removed query temp file: %s", filename);
                                                        idss_errmsg_r(errbuf);
                                                        }

					}
					}
				}

		    closedir(dirp);
		}
		else
		{
                	 if (get_debug_flg() >= 1){
                  		snprintf(errbuf, BUF_SIZE, "Permission denied: can not open directory %s for deleting files", directory);
		  		idss_errmsg_r(errbuf);
						}
		}

		pthread_mutex_unlock(&merge_mutex);
		return OK;
	}
   else
  	{
		if (get_debug_flg () >= 1)
		{
		 	snprintf(errbuf, BUF_SIZE, "Permission denied: can not open directory %s", directory);
        	 	idss_errmsg_r (errbuf);
		}
		xmlFreeDoc(result);
		pthread_mutex_unlock(&merge_mutex);
		return GENERIC_ERROR;
  	}
}

int apply_aggr(char *filename, char *finalfilename, char *query) {
    struct stat sb;
    char *pattern[10];
    regmatch_t pmatch[10];
    regex_t r[10];
    int i = 0, j = 0, k = 0;
    size_t nmatch = 1;
    int status[10];
    xmlDocPtr doc = NULL;
    xmlDocPtr result = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr cur = NULL;
    xmlNodePtr cur2 = NULL;
    xmlNodePtr cur_r = NULL;
    xmlNodePtr cur_r2 = NULL;
    xmlNodePtr sch = NULL;
    xmlNodePtr recs = NULL;
    xmlNodePtr sch_r = NULL;
    xmlNodePtr recs_r = NULL;
    xmlNodePtr root_r = NULL;
    int aggr = 0; //1 if we have one or more aggr functions
    int no_aggr = 0; //1 if we have one or more fields without aggr function
    char *query2 = NULL;
    char *query3 = NULL;
    char *save_ptr = NULL;
    char delims[] = ",";
    char *field = NULL;
    char errbuf[BUF_SIZE] = {0};
    int n_attrs = 0; //Number of attributes in query
    int n_attrs2 = 0; //Number of attributes after splitting every avg field
    double *memory = NULL; //Memory for agg function temp results (memory[i] = temp result of aggr func on i-th field in filename)
    char *operations = NULL; //Memory for agg functions (operation[i] = aggr function on i-th field in filename)
    char **fieldnames = NULL; //memory for field names in query (avg fields have been splitted, so we've lost their alias in filename)
    xmlChar *att = NULL;
    xmlChar *type = NULL;
    int return_value = OK;
    xmlChar *tempc = NULL;
    double tempv = 0;
    char t[30] = {0};

    if (get_debug_flg() >= 1) {
        snprintf(errbuf, BUF_SIZE, "Entering apply_aggr function, source filename: %s; destination filename: %s", filename, finalfilename);
        idss_errmsg_r(errbuf);
    }

    for (i = 0; i < 10; i++) {
        pattern[i] = NULL;
        status[i] = 1;
    }

    if (!stat(filename, &sb) && sb.st_size > 0) {

        pattern[0] = strdup("max[[:space:]]*[(]"); //max
        pattern[1] = strdup("min[[:space:]]*[(]"); //min
        pattern[2] = strdup("sum[[:space:]]*[(]"); //sum
        pattern[3] = strdup("count[[:space:]]*[(]"); //count
        pattern[4] = strdup("avg[[:space:]]*[(]"); //avg
        pattern[5] = strdup("select[[:space:]]+"); //select
        pattern[6] = strdup("[[:space:]]+from[[:space:]]"); //from
        pattern[7] = strdup("[[:space:]]+as[[:space:]]+"); //as
        pattern[8] = strdup("([a-zA-z]|[_])([[:alnum:]]|[_])*");
        pattern[9] = strdup("avg[[:space:]]*[(][[:space:]]*([a-zA-z]|[_])([[:alnum:]]|[_])*[[:space:]]*[)]");

        for (i = 0; i < 10; i++) {
            if (pattern[i] == NULL) {
                for (j = 0; j < 10; j++) {
                    if (pattern[j]) {
                        free(pattern[j]);
                        pattern[j] = NULL;
                    }
                }
                return MEMORY_ERROR;
            }
        }

        for (i = 0; i < 10; i++) {
            if (regcomp(&r[i], pattern[i], REG_EXTENDED | REG_ICASE) != 0) {
                if (get_debug_flg() >= 1)
                    idss_errmsg_r("Error while registering regular expressions!");

                for (j = 0; j < i; j++)
                    regfree(&r[j]);

                for (j = 0; j < 10; j++) {
                    free(pattern[j]);
                    pattern[j] = NULL;
                }

                return GENERIC_ERROR;
            }
        }

        doc = xmlParseFile(filename);

        if (doc == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while processing file: %s", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        root = xmlDocGetRootElement(doc); //root = RECORDSET
        if (root == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        cur = root->xmlChildrenNode;
        while (cur != NULL) {
            if (!xmlStrcmp(cur->name, (const xmlChar *) "SCH"))
                sch = cur;
            if (!xmlStrcmp(cur->name, (const xmlChar *) "RECS"))
                recs = cur;
            cur = cur->next;
        }

        if ((sch == NULL) || (recs == NULL)) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s! SCH and / or RECS nodes not found!", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        //Looking for select clause
        status[5] = regexec(&r[5], query, nmatch, &pmatch[5], 0);
        if (status[5] == 0)
            query += pmatch[5].rm_eo - 1; //query = "c1,...,cn from...."

        //Looking for from clause
        status[6] = regexec(&r[6], query, nmatch, &pmatch[6], 0);
        if (status[6] == 0)
            query2 = mystrndup(query, (size_t) pmatch[6].rm_so); //query2="c1,...,cn"
        else
            query2 = strdup(query);

        if (!query2) {
            return_value = MEMORY_ERROR;
            goto exit;
        }

        query3 = strdup(query2);
        if (!query3) {
            return_value = MEMORY_ERROR;
            goto exit;
        }

        //Looking for aggr functions and counting number of attributes
        field = strtok_r(query2, delims, &save_ptr);
        while (field != NULL) {
            n_attrs++; //Number of attributes in query
            n_attrs2++; //Number of attributes in file

            //Looking for aggr function on this field
            for (i = 0; i < 5; i++) {
                status[i] = regexec(&r[i], field, nmatch, &pmatch[i], 0);
                if (status[i] == 0) {
                    aggr = 1;
                    if (i == 4) //Avg field
                        n_attrs2++; //An avg field in query is splitted into two fields in filename
                    break;
                }
            }

            if (i == 5) {
                //No aggr function
                no_aggr = 1;
            }

            field = strtok_r(NULL, delims, &save_ptr);
        }

        if ((aggr == 1) && (no_aggr == 1)) {
            //We have found aggr functions and no aggr functions in the same query
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while executing query: aggregation functions and no aggregation functions  into the same query!");
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        if (aggr == 1) {
            //We have only aggr functions
            memory = (double *) calloc(n_attrs2, sizeof (double));
            operations = (char *) calloc(n_attrs2, sizeof (char));
            fieldnames = (char **) calloc(n_attrs, sizeof (char *));
            if ((!memory) || (!operations) || (!fieldnames)) {

                return_value = MEMORY_ERROR;
                goto exit;
            }

            j = 0; //index on file fields
            k = 0; //index on query fields

            //Looking for field names and aggr functions
            field = strtok_r(query3, delims, &save_ptr);
            while (field != NULL) {
                operations[j] = -1;
                for (i = 0; i < 5; i++) {
                    status[i] = regexec(&r[i], field, nmatch, &pmatch[i], 0);
                    if (status[i] == 0) {
                        //Found aggr function
                        operations[j] = i;
                        if (i == 4) //Avg field
                        {
                            //splitted field, we record again the avg func into operations
                            j++;
                            operations[j] = i;
                            //Looking for AS
                            status[7] = regexec(&r[7], field, nmatch, &pmatch[7], 0);
                            if (status[7] == 0) {
                                //Found AS
                                field += pmatch[7].rm_eo;
                                //Looking for field name
                                status[8] = regexec(&r[8], field, nmatch, &pmatch[8], 0);
                                field += pmatch[8].rm_so;
                                if (status[8] != 0) {
                                    if (get_debug_flg() >= 1)
                                        idss_errmsg_r("Error while executing query: found AS clause but no alias!");

                                    return_value = GENERIC_ERROR;
                                    goto exit;
                                }

                                *(fieldnames + k) = mystrndup(field, (size_t) (pmatch[8].rm_eo - pmatch[8].rm_so));
                                if (!*(fieldnames + k)) {
                                    return_value = MEMORY_ERROR;
                                    goto exit;
                                }
                            } else {
                                //No AS
                                status[9] = regexec(&r[9], field, nmatch, &pmatch[9], 0);
                                if (status[9] != 0) {
                                    if (get_debug_flg() >= 1)
                                        idss_errmsg_r("Error while executing query: expected AVG function!");
                                    return_value = GENERIC_ERROR;
                                    goto exit;
                                }
                                *(fieldnames + k) = mystrndup(field + pmatch[9].rm_so, (size_t) (pmatch[9].rm_eo - pmatch[9].rm_so));
                                if (!*(fieldnames + k)) {
                                    return_value = MEMORY_ERROR;
                                    goto exit;
                                }
                            }
                        }
                        break;
                    }
                }
                j++;
                k++;
                field = strtok_r(NULL, delims, &save_ptr);
            }

            result = xmlNewDoc(BAD_CAST "1.0");
            root_r = xmlNewNode(NULL, (const xmlChar *) "RECORDSET");
            xmlDocSetRootElement(result, root_r);
            sch_r = xmlNewChild(root_r, NULL, (const xmlChar *) "SCH", NULL);
            recs_r = xmlNewChild(root_r, NULL, (const xmlChar *) "RECS", NULL);
            xmlNewProp(recs_r, (const xmlChar *) "N", (xmlChar *) "1");
            //Copying the SCH
            cur = sch->xmlChildrenNode;
            j = -1; //index for operations array
            k = -1; //index for fieldnames array
            while (cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar *) "ATT")) {
                    j++;
                    k++;

                    if (operations[j] == 4) {
                        //Field in filename related to an avg field in query, also the next field in filename
                        // is related to the avg field
                        cur = cur->next;
                        while (cur != NULL) {
                            if (!xmlStrcmp(cur->name, (const xmlChar *) "ATT")) {
                                j++;
                                break;
                            }
                            cur = cur->next;
                        }

                        //The two field become a single avg field in finalfilename
                        cur_r = xmlNewChild(sch_r, NULL, (const xmlChar *) "ATT", (const xmlChar *) *(fieldnames + k));
                        xmlNewProp(cur_r, (const xmlChar *) "TYPE", (const xmlChar *) "float");
                    } else {
                        //No avg field; we can simply copy
                        att = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                        type = xmlGetProp(cur, (const xmlChar *) "TYPE");
                        if ((!att) || (!type)) {

                            return_value = MEMORY_ERROR;
                            goto exit;
                        }
                        cur_r = xmlNewChild(sch_r, NULL, (const xmlChar *) "ATT", att);
                        xmlNewProp(cur_r, (const xmlChar *) "TYPE", type);
                        if (att) {
                            xmlFree(att);
                            att = NULL;
                        }
                        if (type) {
                            xmlFree(type);
                            type = NULL;
                        }
                    }
                }
                cur = cur->next;
            }

            //only one record into finalfilename
            cur = recs->xmlChildrenNode;
            k = -1; //index on <R>
            while (cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar *) "R")) {
                    k++;
                    j = -1; //index for operations array
                    cur2 = cur->xmlChildrenNode;
                    while (cur2 != NULL) {
                        if (!xmlStrcmp(cur2->name, (const xmlChar *) "F")) {
                            j++;
                            tempc = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
                            if (tempc) {
                                tempv = strtod((char *) tempc, (char **) NULL);
                                switch (operations[j]) {
                                    case 0: //MAX
                                        if (k == 0) {
                                            memory[j] = tempv;
                                        } else {
                                            if (tempv > memory[j])
                                                memory[j] = tempv;
                                        }
                                        break;

                                    case 1: //MIN
                                        if (k == 0) {
                                            memory[j] = tempv;
                                        } else {
                                            if (tempv < memory[j])
                                                memory[j] = tempv;
                                        }
                                        break;

                                    case 2: //SUM
                                    case 3: //COUNT
                                        memory[j] += tempv;
                                        break;

                                    case 4: //AVG
                                        //We have to elaborate two fields
                                        memory[j] += tempv;
                                        xmlFree(tempc);
                                        tempc = NULL;
                                        cur2 = cur2->next;
                                        while (cur2 != NULL) {
                                            if (!xmlStrcmp(cur2->name, (const xmlChar *) "F")) {
                                                tempc = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
                                                j++;
                                                break;
                                            }
                                            cur2 = cur2->next;
                                        }
                                        if (tempc) {
                                            tempv = strtod((char *) tempc, (char **) NULL);
                                            memory[j] += tempv;
                                        }
                                        break;
                                }

                                if (tempc) {
                                    xmlFree(tempc);
                                    tempc = NULL;
                                }
                            }
                        }
                        cur2 = cur2->next;
                    }
                }
                cur = cur->next;
            }

            cur_r = xmlNewChild(recs_r, NULL, (const xmlChar *) "R", NULL);
            for (j = 0; j < n_attrs2; j++) {
                if (operations[j] != 4) {
                    snprintf(t, 30, "%f", memory[j]);
                } else { //Found the first field related to the avg
                    if ((memory[j + 1]) > 0)
                        snprintf(t, 30, "%f", memory[j] / memory[j + 1]);
                    else
                        snprintf(t, 30, "(NULL)");
                    j++;
                }
                cur_r2 = xmlNewChild(cur_r, NULL, (const xmlChar *) "F", (const xmlChar *) t);
            }

            if (xmlSaveFormatFileEnc(finalfilename, result, "UTF-8", 1) == -1) {
                if (get_debug_flg() >= 1) {
                    snprintf(errbuf, BUF_SIZE, "Error while saving query result file %s", finalfilename);
                    idss_errmsg_r(errbuf);
                }

                return_value = GENERIC_ERROR;
                goto exit;
            }

            if (remove(filename)) {
                if (get_debug_flg() >= 1) {
                    sprintf(errbuf, "Error! Remove error: file %s", filename);
                    idss_errmsg_r(errbuf);
                }
                return_value = OK;
                goto exit;
            }

            return_value = OK;
            goto exit;
        } else {
            //We have only to rename the file
            if (rename(filename, finalfilename) == -1) {
                if (get_debug_flg() >= 1) {
                    sprintf(errbuf, "Error while renaming file %s to %s", filename, finalfilename);
                    idss_errmsg_r(errbuf);
                }

                return_value = GENERIC_ERROR;
                goto exit;
            }

            return_value = OK;
            goto exit;
        }
    }
    else {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Unable to find file %s", filename);
            idss_errmsg_r(errbuf);
        }

        return_value = GENERIC_ERROR;
        goto exit;
    }

exit:

    for (j = 0; j < 10; j++) {
        regfree(&r[j]);
        free(pattern[j]);
        pattern[j] = NULL;
    }

    if (query2) {
        free(query2);
        query2 = NULL;
    }
    if (query3) {
        free(query3);
        query3 = NULL;
    }
    if (memory) {
        free(memory);
        memory = NULL;
    }
    if (operations) {
        free(operations);
        operations = NULL;
    }
    if (fieldnames) {
        for (k = 0; k < n_attrs; k++)
            if (*(fieldnames + k)) {
                free(*(fieldnames + k));
                *(fieldnames + k) = NULL;
            }

        free(fieldnames);
        fieldnames = NULL;
    }
    if (att) {
        xmlFree(att);
        att = NULL;
    }
    if (type) {
        xmlFree(type);
        type = NULL;
    }
    if (tempc) {
        xmlFree(tempc);
        tempc = NULL;
    }
    if (doc) {
        xmlFreeDoc(doc);
        doc = NULL;
    }
    if (result) {
        xmlFreeDoc(result);
        result = NULL;
    }

    return return_value;
}

/*	Returns the final result of a subquery
 */
int getSubqValue(char *filename, double *result) {
    struct stat sb;
    xmlDocPtr doc = NULL;
    int return_value = OK;
    xmlChar *tempc = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr recs = NULL;
    xmlNodePtr cur = NULL;
    xmlNodePtr cur2 = NULL;
    char errbuf[BUF_SIZE] = {0};

    if (get_debug_flg() >= 1) {
        snprintf(errbuf, BUF_SIZE, "Entering getSubqValue function, filename: %s", filename);
        idss_errmsg_r(errbuf);
    }

    if (!stat(filename, &sb) && sb.st_size > 0) {
        doc = xmlParseFile(filename);

        if (doc == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while processing file: %s", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }
        root = xmlDocGetRootElement(doc); //root = RECORDSET
        if (root == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        cur = root->xmlChildrenNode;
        while (cur != NULL) {
            if (!xmlStrcmp(cur->name, (const xmlChar *) "RECS")) {
                recs = cur;
                break;
            }
            cur = cur->next;
        }
        if (recs == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s! RECS node not found!", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        cur = recs->xmlChildrenNode;
        while (cur != NULL) {
            if (!xmlStrcmp(cur->name, (const xmlChar *) "R")) {
                cur2 = cur->xmlChildrenNode;
                while (cur2 != NULL) {
                    if (!xmlStrcmp(cur2->name, (const xmlChar *) "F")) {
                        tempc = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
                        break;
                    }
                    cur2 = cur2->next;
                }
                break;
            }
            cur = cur->next;
        }

        if (tempc == NULL) {
            if (get_debug_flg() >= 1) {
                snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s! F node value error!", filename);
                idss_errmsg_r(errbuf);
            }

            return_value = GENERIC_ERROR;
            goto exit;
        }

        *result = strtod((char *) tempc, (char **) NULL);
        return_value = OK;
    } else {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error! File %s not found!", filename);
            idss_errmsg_r(errbuf);
        }

        return_value = GENERIC_ERROR;
    }
exit:
    if (doc != NULL) {
        xmlFreeDoc(doc);
        doc = NULL;
    }
    if (tempc) {
        xmlFree(tempc);
        tempc = NULL;
    }
    return return_value;
}

static int getAttrsInFile(xmlDocPtr doc, xmlNodePtr sch, char ***attrs, int *attrNum) {
    int i = 0;
    int j = 0;
    xmlNodePtr cur = NULL;
    xmlChar *tempc = NULL;

    if (get_debug_flg() >= 1)
        idss_errmsg_r("Entering getAttrsInFile function");

    //Getting attributes
    *attrNum = 0;
    *attrs = NULL;
    if (sch != NULL) {
        //Counting number of fields
        cur = sch->xmlChildrenNode;
        while (cur != NULL) {
            if (!xmlStrcmp(cur->name, (const xmlChar *) "ATT")) {
                (*attrNum)++;
            }
            cur = cur->next;
        }

        if (*attrNum) {
            *attrs = (char **) calloc(*attrNum, sizeof (char *));
            if (!(*attrs))
                return MEMORY_ERROR;
        } else {
            *attrs = NULL;
            // this is not a memory allocation error
            // simply, there are no attributes
            // note that we do not return OK
            // because OK means the attributes have been retrieved correctly
            // and there is at least one attribute
            return MEMORY_ERROR;
        }

        cur = sch->xmlChildrenNode;
        while (cur != NULL) {
            if (!xmlStrcmp(cur->name, (const xmlChar *) "ATT")) {
                tempc = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                *(*attrs + i) = strdup((char *) tempc);
                if (!*(*attrs + i)) {
                    for (j = 0; j < i; j++) {
                        if (*(*attrs + j)) {
                            free(*(*attrs + j));
                            *(*attrs + j) = NULL;
                        }
                    }
                    *attrs = NULL;
                    if (tempc) {
                        free(tempc);
                        tempc = NULL;
                    }
                    return MEMORY_ERROR;
                }
                xmlFree(tempc);
                tempc = NULL;
                i++;
            }
            cur = cur->next;
        }
    }

    return OK;
}

int executeNested(char *firstfilename, parser_result e, char *resultfile) {
    xmlDocPtr doc = NULL;
    xmlDocPtr result = NULL;
    xmlNodePtr cur = NULL;
    xmlNodePtr cur2 = NULL;
    xmlNodePtr sch = NULL;
    xmlNodePtr recs = NULL;
    xmlNodePtr root_r = NULL;
    xmlNodePtr sch_r = NULL;
    xmlNodePtr recs_r = NULL;
    xmlNodePtr cur_r = NULL;
    xmlNodePtr cur_r2 = NULL;
    xmlChar *attname = NULL;
    xmlChar *atttype = NULL;
    xmlChar *tempc = NULL;
    char errbuf[BUF_SIZE] = {0};
    int rc = 0;
    int return_value = OK;
    int index = 0;
    DB *dbp = NULL; //Hash Map
    DBT key, data; //For BerkeleyDB
    int ret = 0;
    int n_attrs = 0; //number of fields in firstfilename
    char **attrs = NULL; //field names
    int evaluation = 0; //Result of a single record evaluation
    char n[20] = {0};
    int num_of_recs = 0;

    if (get_debug_flg() >= 1) {
        snprintf(errbuf, BUF_SIZE, "entering executeNested, filename: %s", firstfilename);
        idss_errmsg_r(errbuf);
    }

    doc = xmlParseFile(firstfilename);
    if (doc == NULL) {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", firstfilename);
            idss_errmsg_r(errbuf);
        }

        return_value = GENERIC_ERROR;
        goto exit;
    }

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s", firstfilename);
            idss_errmsg_r(errbuf);
        }

        return_value = GENERIC_ERROR;
        goto exit;
    }

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "SCH")) {
            sch = cur;
        }
        if (!xmlStrcmp(cur->name, (const xmlChar *) "RECS")) {
            recs = cur;
        }
        cur = cur->next;
    }

    if ((!sch) || (!recs)) {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error while parsing file: %s!  SCH and / or RECS nodes not found!", firstfilename);
            idss_errmsg_r(errbuf);
        }

        return_value = GENERIC_ERROR;
        goto exit;
    }

    rc = getAttrsInFile(doc, sch, &attrs, &n_attrs);
    if (rc != OK) {
        return_value = rc;
        goto exit;
    }

    //Creating the result doc
    result = xmlNewDoc(BAD_CAST "1.0");
    root_r = xmlNewNode(NULL, (const xmlChar *) "RECORDSET");
    xmlDocSetRootElement(result, root_r);
    sch_r = xmlNewChild(root_r, NULL, (const xmlChar *) "SCH", NULL);

    //Copying the schema (only requested fields)
    cur = sch->xmlChildrenNode;
    index = 0;
    while ((cur != NULL) && (index < (n_attrs - e.numOfLeafs))) {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "ATT")) {
            attname = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            atttype = xmlGetProp(cur, (const xmlChar *) "TYPE");
            cur_r = xmlNewChild(sch_r, NULL, (const xmlChar *) "ATT", attname);
            xmlNewProp(cur_r, (const xmlChar *) "TYPE", atttype);
            if (attname) {
                xmlFree(attname);
                attname = NULL;
            }
            if (atttype) {
                xmlFree(atttype);
                atttype = NULL;
            }
            index++;
        }
        cur = cur->next;
    }

    //Creating RECS node
    recs_r = xmlNewChild(root_r, NULL, (const xmlChar *) "RECS", NULL);

    ret = db_create(&dbp, NULL, 0);
    ret = dbp->open(dbp, /* DB structure pointer */
            NULL, /* Transaction pointer */
            NULL, /* On-disk file that holds the database. */
            NULL, /* Optional logical database name */
            DB_HASH, /* Database access method */
            DB_CREATE, /* Open flags */
            0); /* File mode (using defaults) */

    cur = recs->xmlChildrenNode;
    while (cur != NULL) {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "R")) {
            //We must copy this record in the Hash Map
            index = 0;
            cur2 = cur->xmlChildrenNode;
            while (cur2 != NULL) {
                if (!xmlStrcmp(cur2->name, (const xmlChar *) "F")) {
                    tempc = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
                    memset(&key, 0, sizeof (DBT));
                    memset(&data, 0, sizeof (DBT));
                    //creating key
                    key.size = strlen(*(attrs + index)) + 1;
                    key.data = *(attrs + index);
                    //creating data
                    data.data = (char *) tempc;
                    data.size = strlen((char *) tempc) + 1;
                    //By default the db doesn't allow duplicate entries
                    ret = dbp->get(dbp, NULL, &key, &data, 0);
                    if (ret == DB_NOTFOUND) { //key not present
                        dbp->put(dbp, NULL, &key, &data, 0);
                    }
                    index++;
                    if (tempc) {
                        xmlFree(tempc);
                        tempc = NULL;
                    }
                }
                cur2 = cur2->next;
            }

            rc = evaluate(dbp, e, &evaluation);
            if (rc != DQF_OK) {
                if (rc == DQF_NO_MEMORY)
                    return_value = MEMORY_ERROR;
                else
                    return_value = GENERIC_ERROR;
                goto exit;
            }
            sprintf(errbuf, "valore valutazione: %d", evaluation);
            idss_errmsg_r(errbuf);

            //Cleaning up the Hash Map
            for (index = 0; index < n_attrs; index++) {
                memset(&key, 0, sizeof (DBT));
                key.size = strlen(*(attrs + index)) + 1;
                key.data = *(attrs + index);
                dbp->del(dbp, NULL, &key, 0);
            }

            if (evaluation == 1) {
                //We must insert this record in the result (only requested fields)
                cur_r = xmlNewChild(recs_r, NULL, (const xmlChar *) "R", NULL);
                cur2 = cur->xmlChildrenNode;
                index = 0;
                while ((cur2 != NULL) && (index < (n_attrs - e.numOfLeafs))) {
                    if (!xmlStrcmp(cur2->name, (const xmlChar *) "F")) {
                        tempc = xmlNodeListGetString(doc, cur2->xmlChildrenNode, 1);
                        cur_r2 = xmlNewChild(cur_r, NULL, (const xmlChar *) "F", tempc);
                        if (tempc) {
                            xmlFree(tempc);
                            tempc = NULL;
                        }
                        index++;
                    }
                    cur2 = cur2->next;
                }
                num_of_recs++;
            }
        }
        cur = cur->next;
    }

    //Remiving the Hash Map
    //dbp->remove(dbp, NULL, NULL, 0);
    dbp->close(dbp, 0);

    //Adding the number of records in the result file
    snprintf(n, 20, "%d", num_of_recs);
    xmlNewProp(recs_r, (const xmlChar *) "N", (const xmlChar *) n);

    if (xmlSaveFormatFileEnc(resultfile, result, "UTF-8", 1) == -1) {
        if (get_debug_flg() >= 1) {
            snprintf(errbuf, BUF_SIZE, "Error while saving query result file: %s", resultfile);
            idss_errmsg_r(errbuf);
        }
        return_value = GENERIC_ERROR;
        goto exit;
    } else {
        return_value = OK;
    }
exit:
    if (attname) {
        xmlFree(attname);
        attname = NULL;
    }
    if (atttype) {
        xmlFree(atttype);
        atttype = NULL;
    }
    if (tempc) {
        xmlFree(tempc);
        tempc = NULL;
    }
    if (doc) {
        xmlFreeDoc(doc);
        doc = NULL;
    }
    if (result) {
        xmlFreeDoc(result);
        result = NULL;
    }
    if (attrs) {
        for (index = 0; index < n_attrs; index++) {
            if (*(attrs + index)) {
                free(*(attrs + index));
                *(attrs + index) = NULL;
            }
        }
        free(attrs);
        attrs = NULL;
    }
    return return_value;
}

int sendbackfile(char *filename, char *myhostname, char *uqi, int i, char *destkey) {
    FILE *file_a = NULL;
    char *buf = NULL;
    char *start = NULL;
    struct stat sst;
    size_t nbyte = 0;
    int return_value = OK;
    char *response = NULL;
    //Key dest_key;
    char errbuf[BUF_SIZE] = {0};

    if (get_debug_flg() >= 1) {
        snprintf(errbuf, BUF_SIZE, "Entering sendbackfile function, filename: %s destination key: %s", filename, destkey);
        idss_errmsg_r(errbuf);
    }

    if (!stat(filename, &sst) && sst.st_size > 0) {
        file_a = fopen(filename, "r");
        buf = (char *) calloc(sst.st_size + 1, sizeof (char));
        if (!buf) {
            return_value = MEMORY_ERROR;
            goto exit;
        }

        start = buf;
        while (!feof(file_a)) {
            nbyte = fread(buf, 1, 1024, file_a);
            buf = buf + nbyte;
        }
        fclose(file_a);
        file_a = NULL;
        *buf = 0;
        buf = start;
        response = (char *) calloc(256 + strlen(buf), sizeof (char));
        if (!response) {
            return_value = MEMORY_ERROR;
            goto exit;
        }

        sprintf(response, "%s%c%s%c%d%c%s", get_ig_hostname(), CH_DELIMETER, uqi, CH_DELIMETER, i, CH_DELIMETER, buf);


        /**
         * D1HT send reply to destination
         */
        snprintf(errbuf, BUF_SIZE-1,"D1HT send response to %s for uqi %s:\n\tresponse %s\n", destkey, uqi, buf);
        idss_errmsg_r(errbuf);
        d1ht_send_response(d1ht_overlay, uqi, destkey, buf);


        return_value = OK;
    } else
        return_value = GENERIC_ERROR;

exit:
    if (buf) {
        free(buf);
        buf = NULL;
        start = NULL;
    }
    if (response) {
        free(response);
        response = NULL;
    }
    if (file_a) {
        fclose(file_a);
        file_a = NULL;
    }
    return return_value;
}

int addFields(parser_result *e) {
    char **addfields = NULL;
    char *temp = NULL;
    int return_value = OK;
    regex_t r[2];
    char* pattern[2];
    size_t nmatch = 1;
    regmatch_t pmatch[2];
    int i = 0, j = 0;
    int rc = 0;
    int status[2];
    char *resultq = NULL;
    char *tempq = NULL;
    int s1 = 512, s2 = 512; //string sizes
    char *resultq_n = NULL; //used in realloc
    char *tempq_n = NULL; //used in realloc

    if (get_debug_flg() >= 1)
        idss_errmsg_r("Entering addFields function");

    for (i = 0; i < 2; i++) {
        pattern[i] = NULL;
        status[i] = 1;
    }

    pattern[0] = strdup("select[[:space:]]+"); //select
    pattern[1] = strdup("[[:space:]]+from[[:space:]]"); //from

    for (i = 0; i < 2; i++) {
        if (pattern[i] == NULL) {
            for (j = 0; j < 2; j++) {
                if (pattern[j]) {
                    free(pattern[j]);
                    pattern[j] = NULL;
                }
            }

            return MEMORY_ERROR;
        }
    }

    //Registering regular expressions
    for (i = 0; i < 2; i++) {
        if (regcomp(&r[i], pattern[i], REG_EXTENDED | REG_ICASE) != 0) {
            if (get_debug_flg() >= 1)
                idss_errmsg_r("Error while registering regular expressions!");

            for (j = 0; j < i; j++)
                regfree(&r[j]);

            for (j = 0; j < 2; j++) {
                free(pattern[j]);
                pattern[j] = NULL;
            }

            return GENERIC_ERROR;
        }
    }

    addfields = (char **) calloc(e->numOfLeafs, sizeof (char *));
    resultq = (char *) calloc(s1, sizeof (char));
    tempq = (char *) calloc(s2, sizeof (char));

    if ((!addfields) || (!resultq) || (!tempq)) {
        return_value = MEMORY_ERROR;
        goto exit;
    }

    rc = getOtherFields(e, &addfields);
    if (rc != DQF_OK) {
        if (rc == DQF_ERROR)
            return_value = GENERIC_ERROR;
        if (rc == DQF_NO_MEMORY)
            return_value = MEMORY_ERROR;

        goto exit;
    }

    if (e->parentq) {
        temp = e->parentq;
        //Looking for select clause
        status[0] = regexec(&r[0], temp, nmatch, &pmatch[0], 0);
        if (status[0] == 0) {
            //Found select clause
            //Checking (and adjusting) string size
            if (pmatch[0].rm_eo >= s1) {
                s1 = pmatch[0].rm_eo + 1;

                resultq_n = (char *) realloc(resultq, s1 * sizeof (char));
                if (resultq_n != NULL)
                    resultq = resultq_n;
                else {
                    return_value = MEMORY_ERROR;
                    goto exit;
                }
            }
            strncat(resultq, temp, pmatch[0].rm_eo);
            temp += pmatch[0].rm_eo;
        }
        //Looking for from clause
        status[1] = regexec(&r[1], temp, nmatch, &pmatch[1], 0);
        if (status[1] == 0) {
            //Found from clause
            //Checking (and adjusting) string size
            if (pmatch[1].rm_so >= s2) {
                s2 = pmatch[1].rm_so + 1;

                tempq_n = (char *) realloc(tempq, s2 * sizeof (char));
                if (tempq_n != NULL)
                    tempq = tempq_n;
                else {
                    return_value = MEMORY_ERROR;
                    goto exit;
                }
            }
            strncat(tempq, temp, pmatch[1].rm_so);
            temp += pmatch[1].rm_so;
        }

        for (i = 0; i < e->numOfLeafs; i++) {
            if ((strlen(tempq) + 1 + strlen(*(addfields + i))) >= s2) {
                s2 = strlen(tempq) + 1 + strlen(*(addfields + i)) + 1;

                tempq_n = (char *) realloc(tempq, s2 * sizeof (char));
                if (tempq_n != NULL)
                    tempq = tempq_n;
                else {
                    return_value = MEMORY_ERROR;
                    goto exit;
                }
            }
            strcat(tempq, ",");
            strcat(tempq, *(addfields + i));
        }

        if ((strlen(resultq) + strlen(tempq) + strlen(temp)) >= s1) {
            s1 = strlen(resultq) + strlen(tempq) + strlen(temp) + 1;

            resultq_n = (char *) realloc(resultq, s1 * sizeof (char));
            if (resultq_n != NULL)
                resultq = resultq_n;
            else {
                return_value = MEMORY_ERROR;
                goto exit;
            }
        }

        strcat(resultq, tempq);
        strcat(resultq, temp);

        free(e->parentq);
        e->parentq = strdup(resultq);

        return_value = OK;
    }
exit:
    if (addfields) {
        for (i = 0; i < e->numOfLeafs; i++)
            if (*(addfields + i)) {
                free(*(addfields + i));
                *(addfields + i) = NULL;
            }
        free(addfields);
        addfields = NULL;
    }
    if (tempq) {
        free(tempq);
        tempq = NULL;
    }
    if (resultq) {
        free(resultq);
        resultq = NULL;
    }
    for (j = 0; j < 2; j++) {
        regfree(&r[j]);
        free(pattern[j]);
        pattern[j] = NULL;
    }
    return return_value;
}
