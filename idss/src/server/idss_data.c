/** @file idss_data.c
 *
 * Inserts and deletes an iStore in the list of the iStores the host is registered to.
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <string.h>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "idss_unix.h"
#include "utils.h"
#include "idss_data.h"

#define BUF_SIZE 1024

/********************
 * Data Declaration
 ********************/

static int idss_debug = 0;
static char* configuration_file = NULL;

FILE* logfd = NULL;


/* the directory where information providers store data */
static char  *system_provider_sd = NULL;
/* the directory where the user supplied information are stored */
static char  *user_provider_sd = NULL;
/* the directory where information coming from authorized istore are stored */
static char  *istore_info_sd = NULL;
/* the directory where results about query are stored */
static char  *query_results_sd = NULL;
/* the directory where temporary files about query are stored */
static char  *query_tempfiles_sd = NULL;


/* wake up period */
static int collector_wp = -1;
static int purge_p = -1;

/* list of access url of the remote method belonging to the registered iStore hosts */
static host_list registered_host_l = NULL;
/* array of access url as like they will be in the configuration file
   this array is used to reset periodically the registered_host_l list.
   this array is NULL termined */
static char ** registered_host_a = NULL;

static provider_list provider_l = NULL;
static host_list allowed_inode_l = NULL;
static char *sqlite_db_path = NULL;
static char *hostname = NULL;
static char *server_identity = NULL;

pthread_mutex_t log_mutex;

/*******************************
 * Static Functions Declaration
 *******************************/

static struct host_node** get_registered_host_node (char* name);
static info_provider_node* get_info_provider_node (char* name);
//static struct host_node** get_allowed_inode (char* name);
static int rec_mkdir(struct soap *soap, char *spool_dir);


/***********************
 * Functions Definition
 ***********************/

void data_allocation()
{

	if (get_debug_flg() >= 1)
	  idss_errmsg_r("entering data_allocation");

	if(!idss_debug)
		if(getenv("IDSS_DEBUG"))
	  		idss_debug = atoi(getenv("IDSS_DEBUG"));

	registered_host_l = (host_list) calloc(1,sizeof(struct _host_list));
	registered_host_l->nhost = 0;
	registered_host_l->first = NULL;

	registered_host_a = (char**)calloc(1,sizeof(char*));
	registered_host_a[0] = NULL;

	provider_l = (provider_list) calloc(1,sizeof(struct _provider_list));
	provider_l->first = NULL;
	provider_l->nproviders = 0;

	allowed_inode_l = (host_list) calloc(1,sizeof(struct _host_list));
	allowed_inode_l->nhost = 0;
	allowed_inode_l->first = NULL;

	server_identity = strdup("Undefined");

	pthread_mutex_init (&log_mutex, NULL);

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting data_allocation");
}


/* f is the name of the log filename. if f is NULL this implies that
a log rotation will be activated
*/
int idss_set_logfile(char* f)
{
  static char* log_filename = NULL;

  if (!f && !log_filename)
  {
    fprintf(stderr, "\n[%s:%d] Error no log filename specified. Log will be redirect to /dev/null\n", __FILE__, __LINE__);
    logfd = fopen(_PATH_DEVNULL,"w");
    setbuf(logfd, NULL);
    return 0;
  }

  if (!log_filename)
    log_filename = strdup(f);

  /* log rotation */
  if (!f)
  {
      char buf[512];
	  if (logfd)
	    fclose(logfd);
	  sprintf(buf, "%s_%lu", log_filename, time(NULL));
	  if (rename (log_filename, buf))
	    fprintf(stderr, "Error! Rename failed from %s to %s\n", log_filename, buf); \
  }

  logfd = fopen(log_filename,"w");
  if (!logfd)
  {
    fprintf(stderr, "\n[%s:%d] Error opening file %s\n", __FILE__, __LINE__, f);
    logfd = fopen(_PATH_DEVNULL,"w");
  }
  setbuf(logfd, NULL);

  return 0;

}

int get_debug_flg()
{
  return idss_debug;
}

char* get_server_identity()
{
  if (!server_identity)
     return "Undefined";
  return server_identity;
}

int set_server_identity(char* id)
{
   if (!id)
     return -2;

   if (server_identity) free(server_identity);
   server_identity = strdup(id);
   if (!server_identity) {
    if (get_debug_flg() >= 1)
      idss_errmsg_r("Not Enough Memory\n");
	  return -1;
   }
   return 0;
}

void set_debug_flg(int level)
{
  idss_debug = level;
}


void set_conf_filename(char* name)
{
  configuration_file = strdup(name);
}

char* get_conf_filename()
{
  if (!configuration_file)
    return "none";

  return configuration_file;
}

void idss_data_dealloc()
{

if (get_debug_flg() >= 1)
  idss_errmsg_r("entering idss_data_dealloc");

  if (configuration_file)
    free(configuration_file);

  if (system_provider_sd)
    free(system_provider_sd);

  if (user_provider_sd)
    free(user_provider_sd);

  if (istore_info_sd)
    free(istore_info_sd);

  if (registered_host_l)
  {
    struct host_node* hn, *next;

    for (hn = registered_host_l->first; hn; hn = next)
	  {
	    if (hn && hn->hostname)
	      free(hn->hostname);

	    next = hn->next;

	    if(hn)
	      free(hn);
	  }

	free(registered_host_l);

  }

  if (registered_host_a)
  {
    int i;

    for(i=0; registered_host_a[i]; i++)
      if(registered_host_a[i])
	      free(registered_host_a[i]);

	free(registered_host_a);
  }

  if (provider_l)
  {
    info_provider_node pn, next;

	for(pn = provider_l->first; pn; pn = next)
	{
	  if (pn && pn->name)
	    free(pn->name);

	  if (pn && pn->module)
	    free(pn->module);

	  if (pn && pn->arguments)
	    free(pn->arguments);

	  next = pn->next;

	  if(pn)
	    free(pn);
	}
	free(provider_l);
  }

if (allowed_inode_l)
  {
    struct host_node* hn, *next;

    for (hn = allowed_inode_l->first; hn; hn = next)
	  {
	    if (hn && hn->hostname)
	      free(hn->hostname);

	    next = hn->next;

      if(hn)
	      free(hn);
	  }

	free(allowed_inode_l);
  }

  if (sqlite_db_path)
    free(sqlite_db_path);

  if (hostname)
    free(hostname);

  if (server_identity)
    free(server_identity);

  //if (get_debug_flg() >= 1)
    //idss_errmsg_r("exiting idss_data_dealloc");
}


/*
  this function reads the configuration file content and initialize
  the data structures.
  the function returns:
  0. on success
  1. if the configuration file can not be opened or parsed
  2. if a node af xml structure can not be parsed
  3. if configuration filename has not been defined
*/


int read_conf_file(struct soap *soap)
{
  int i;
  int return_value = 0;
  char *hname = NULL;
  struct host_node *hnode = NULL;
 char *ip_name = NULL;
 provider_type ip_type = ip_type_exe;
 char *ip_module = NULL;
 char *ip_arguments = NULL;
 int  ip_ttl = 0;
  xmlTextReaderPtr xmldoc = NULL;
  char *ptr = NULL;
  xmlChar* name = NULL;
  char logbuff[1024];
  int res;
  struct stat sst;


  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering read_conf_file");

  if (!configuration_file)
  {
    if (get_debug_flg() >= 1)
      idss_errmsg_r("Error! Please specify the iDSS configuration file using the IDSS_CONF_FILENAME environment variable");
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting read_conf_file");
    return 3;
  }


  xmldoc = xmlNewTextReaderFilename (configuration_file);
  if (!xmldoc)
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Read error for file %s", configuration_file);
      idss_errmsg_r(logbuff);
      idss_errmsg_r("exiting read_conf_file");
    }
    return 1;
  }

  res = xmlTextReaderRead(xmldoc);
  while (res) {
    int xmltype;

    if (res == -1)
    {
      if (get_debug_flg() >= 1){
        sprintf( logbuff, "Error parsing configuration file %s", configuration_file);
        idss_errmsg_r(logbuff);
      }
      return_value = 2;
      goto end_function;
    }

    xmltype = xmlTextReaderNodeType(xmldoc);
    if (name) free(name);
    name = xmlTextReaderName(xmldoc);

    if (xmltype == XML_READER_TYPE_ELEMENT) {
      if (!strcmp((char *) name, "SystemInfo_SD")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading SystemInfo_SD node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }
        system_provider_sd = (char *) xmlTextReaderValue(xmldoc);
      }
      else if (!strcmp((char *)name,"UserInfo_SD")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading UserInfo_SD node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }

        user_provider_sd = (char *) xmlTextReaderValue(xmldoc);
      }
      else if (!strcmp((char *)name,"iNodeInfo_SD")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading iNodeInfo_SD node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }

        istore_info_sd = (char *) xmlTextReaderValue(xmldoc);
      }
      else if (!strcmp((char *)name,"QueryTempFiles_SD")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading QueryTempFiles_SD node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }

        query_tempfiles_sd = (char *) xmlTextReaderValue(xmldoc);
      }
      else if (!strcmp((char *)name,"QueryResults_SD")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading QueryResults_SD node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }

        query_results_sd = (char *) xmlTextReaderValue(xmldoc);
      }
      else if (!strcmp((char *)name,"RegistrationPeriod")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
         if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: error reading RegistrationPeriod node", configuration_file);
           idss_errmsg_r(logbuff);
	 }
         return_value = 2;
         goto end_function;
        }

        ptr = (char *)xmlTextReaderValue(xmldoc);
        if (!ptr){
          if (get_debug_flg() >= 1){
	   sprintf( logbuff, "Error parsing configuration file %s: no value specified for collector RegistrationPeriod", configuration_file);
           idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }
        collector_wp = atoi(ptr);
	free(ptr);
        ptr = NULL;
      }
      else if (!strcmp((char *)name,"Hostname")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
          if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: error reading Hostname node", configuration_file);
            idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }

        hostname = (char *)xmlTextReaderValue(xmldoc);
	if(!domainname(hostname))
	{
	  if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: No valid FQDN Hostname node specified", configuration_file);
	  idss_errmsg_r(logbuff);
          }
          return_value = 2;
	  goto end_function;
        }

      }
      else if (!strcmp((char *)name,"Sqlite_Db_Path")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
          if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: error reading Sqlite_Db_Path node", configuration_file);
            idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }

        sqlite_db_path = (char *)xmlTextReaderValue(xmldoc);
	if (stat(sqlite_db_path, &sst) < 0)
         {
	  if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: No valid Sqlite database specified", configuration_file);
	  idss_errmsg_r(logbuff);
          }
          return_value = 2;
	  goto end_function;
	 }
      }
      else if (!strcmp((char *)name,"PurgePeriod")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
          if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: error reading PurgePeriod node", configuration_file);
            idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }

        ptr = (char *)xmlTextReaderValue(xmldoc);
        if (!ptr){
          if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: no value specified for Purge_Period node", configuration_file);
            idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }
        purge_p = atoi(ptr);
	free(ptr);
        ptr = NULL;
      }
      else if (!strcmp((char *)name,"InfoProviders")){

        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
        {
          if (get_debug_flg() >= 1){
	    sprintf( logbuff, "Error parsing configuration file %s: error reading InfoProviders  node", configuration_file);
            idss_errmsg_r(logbuff);
	  }
         return_value = 2;
         goto end_function;
        }
        xmltype = xmlTextReaderNodeType(xmldoc);
        if (name) free(name);
	name = xmlTextReaderName(xmldoc);

        while (!(xmltype == XML_READER_TYPE_END_ELEMENT && !strcmp((char *)name,"InfoProviders")) && res){

          if (xmltype == XML_READER_TYPE_ELEMENT) {
            if (!strcmp((char *)name,"Provider")){
              ip_name = (char *)xmlTextReaderGetAttribute(xmldoc, BAD_CAST "name");
              if (!ip_name)
              {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid name for InfoProviders", configuration_file);
                  idss_errmsg_r(logbuff);
		}
                return_value = 2;
                goto end_function;
              }
              ptr = (char *)xmlTextReaderGetAttribute(xmldoc, BAD_CAST "type");
              if (!ptr)
              {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid type for InfoProviders", configuration_file);
                  idss_errmsg_r(logbuff);
		}
                return_value = 2;
                goto end_function;
              }
              ip_type = (provider_type)atoi(ptr);
	      free(ptr);
              ptr = NULL;
            }
            else if (!strcmp((char *)name,"Module")){
              res = xmlTextReaderRead(xmldoc);
              if (res == -1)
              {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid Module for InfoProviders", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;
              }

              ip_module = (char *)xmlTextReaderValue(xmldoc);
            }
            else if (!strcmp((char *)name,"Arguments")){
			  char * str;
              res = xmlTextReaderRead(xmldoc);
              if (res == -1)
              {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid Arguments for InfoProviders", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;
              }
              ip_arguments = (char *)xmlTextReaderValue(xmldoc);
              if(ip_arguments)
	      {
              /* substitute \n character with ' ' character */
			  for (str = strchr(ip_arguments, '\n'); str; str = strchr(str + 1, '\n'))
			    *str = ' ';
              /* substitute \n character with ' ' character */
			  for (str = strchr(ip_arguments, '\t'); str; str = strchr(str + 1, '\t'))
			    *str = ' ';
              }
            }
            else if (!strcmp((char *)name,"InfoTTL")){
              res = xmlTextReaderRead(xmldoc);
              if (res == -1)
              {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid InfoTTL for InfoProviders", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;
              }
              ptr = (char *)xmlTextReaderValue(xmldoc);
              if (!ptr){
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid value for InfoTTL ", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;
              }
              ip_ttl = atoi(ptr);
	      free(ptr);
              ptr = NULL;
			}
          }
          if (xmltype == XML_READER_TYPE_END_ELEMENT && !strcmp((char *)name,"Provider")) {
            add_info_provider_node(create_info_provider_node(ip_name,ip_type,ip_module,ip_arguments,ip_ttl));

            if (ip_name)
              free(ip_name);
            if (ip_module)
              free(ip_module);
            if (ip_arguments)
              free (ip_arguments);

            ip_name = NULL;
            ip_type = ip_type_exe;
            ip_module = NULL;
            ip_arguments = NULL;
            ip_ttl = 0;
          }

          res = xmlTextReaderRead(xmldoc);
          if (res == -1)
          {
              if (get_debug_flg() >= 1){
	        sprintf( logbuff, "Error parsing configuration file %s", configuration_file);
                idss_errmsg_r(logbuff);
	      }
         return_value = 2;
         goto end_function;
		  }
          xmltype = xmlTextReaderNodeType(xmldoc);
          if (name) free(name);
	  name = xmlTextReaderName(xmldoc);
        }

        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
		{
                  if (get_debug_flg() >= 1){
	            sprintf( logbuff, "Error parsing configuration file %s", configuration_file);
                    idss_errmsg_r(logbuff);
		  }
         return_value = 2;
         goto end_function;
		}
        xmltype = xmlTextReaderNodeType(xmldoc);
        if (name) free(name);
	name = xmlTextReaderName(xmldoc);
      }
      /*else if (!strcmp((char *)name,"Allowed_iNodes")){
        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
		{
                  if (get_debug_flg() >= 1){
	            sprintf( logbuff, "Error parsing configuration file %s: error reading Allowed_iNodes node", configuration_file);
                    idss_errmsg_r(logbuff);
		  }
         return_value = 2;
         goto end_function;
		}

        xmltype = xmlTextReaderNodeType(xmldoc);
        if (name) free(name);
	name = xmlTextReaderName(xmldoc);

        while (!(xmltype == XML_READER_TYPE_END_ELEMENT && !strcmp((char *)name,"Allowed_iNodes")) && res){

          if (xmltype == XML_READER_TYPE_ELEMENT) {
            if (!strcmp((char *)name,"iNode_DN")){
              res = xmlTextReaderRead(xmldoc);
              if (res == -1)
  		      {
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: error reading iNode_DN node", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;
		      }

              ptr = (char *)xmlTextReaderValue(xmldoc);
              if (!ptr){
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: invalid value specified for iNode_DN node", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;

              }
              add_allowed_host(ptr);
	      free(ptr);
              ptr = NULL;
            }
          }
          res = xmlTextReaderRead(xmldoc);
          if (res == -1)
		  {
             if (get_debug_flg() >= 1){
	        sprintf( logbuff, "Error parsing configuration file %s", configuration_file);
                idss_errmsg_r(logbuff);
	     }
         return_value = 2;
         goto end_function;
		  }
          xmltype = xmlTextReaderNodeType(xmldoc);
          if (name) free(name);
	  name = xmlTextReaderName(xmldoc);
        }
      }*/
      else if (!strcmp((char *)name,"Registered_iStore")){

        res = xmlTextReaderRead(xmldoc);
        if (res == -1)
		{
            if (get_debug_flg() >= 1){
	        sprintf( logbuff, "Error parsing configuration file %s: error reading Registered_iSotre node", configuration_file);
                idss_errmsg_r(logbuff);
	    }
         return_value = 2;
         goto end_function;
		}

        xmltype = xmlTextReaderNodeType(xmldoc);
        if (name) free(name);
	name = xmlTextReaderName(xmldoc);

        while (!(xmltype == XML_READER_TYPE_END_ELEMENT && !strcmp((char *)name,"Registered_iStore")) && res){

          if (xmltype == XML_READER_TYPE_ELEMENT) {
            if (!strcmp((char *)name,"iStore_WS")){
              res = xmlTextReaderRead(xmldoc);
              if (res == -1)
		      {
               if (get_debug_flg() >= 1){
	         sprintf( logbuff, "Error parsing configuration file %s: error reading iStore_WS node", configuration_file);
                 idss_errmsg_r(logbuff);
	       }
         return_value = 2;
         goto end_function;
		      }

              ptr = (char *)xmlTextReaderValue(xmldoc);
              if (!ptr){
                if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: Bad value specified for iStore_WS node", configuration_file);
                  idss_errmsg_r(logbuff);
		}
         return_value = 2;
         goto end_function;

              }
	      if(domainname(ptr))
                add_registered_host(ptr);
	      else
	      {
	        if (get_debug_flg() >= 1){
	          sprintf( logbuff, "Error parsing configuration file %s: No valid FQDN iStore_WS node specified", configuration_file);
		  idss_errmsg_r(logbuff);
	       }
               return_value = 2;
	       goto end_function;
	      }
	      free(ptr);
              ptr = NULL;
            }
          }
          res = xmlTextReaderRead(xmldoc);
          if (res == -1)
		  {
              if (get_debug_flg() >= 1){
	        sprintf( logbuff, "Error parsing configuration file %s", configuration_file);
                idss_errmsg_r(logbuff);
	      }
         return_value = 2;
         goto end_function;
		  }
          xmltype = xmlTextReaderNodeType(xmldoc);
          if (name) free(name);
	  name = xmlTextReaderName(xmldoc);
        }

        //initialization of registered_host_a
        for (i = 0; registered_host_a[i]; i++)
          free(registered_host_a[i]);
        free(registered_host_a);

        registered_host_a = (char **)calloc(registered_host_l->nhost + 1,sizeof(char*));

        for (hname = get_next_registered_host(&hnode), i = 0; hname; hname = get_next_registered_host(&hnode), i++){
          registered_host_a[i] = strdup(hname);
        }
        registered_host_a[registered_host_l->nhost] = NULL;
      }
     }
    res = xmlTextReaderRead(xmldoc);
  }

  if (rec_mkdir(soap, system_provider_sd))
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Error creating directory  %s", system_provider_sd);
      idss_errmsg_r(logbuff);
    }
    return_value = 1;
    goto end_function;
  }
  if (rec_mkdir(soap, user_provider_sd))
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Error creating directory  %s", user_provider_sd);
      idss_errmsg_r(logbuff);
    }
    return_value = 1;
    goto end_function;
  }
  if (rec_mkdir(soap, istore_info_sd))
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Error creating directory  %s", istore_info_sd);
      idss_errmsg_r(logbuff);
    }
    return_value = 1;
    goto end_function;
  }
  if (rec_mkdir(soap, query_tempfiles_sd))
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Error creating directory  %s", query_tempfiles_sd);
      idss_errmsg_r(logbuff);
    }
    return_value = 1;
    goto end_function;
  }
  if (rec_mkdir(soap, query_results_sd))
  {
    if (get_debug_flg() >= 1){
      sprintf( logbuff, "Error creating directory  %s", query_results_sd);
      idss_errmsg_r(logbuff);
    }
    return_value = 1;
    goto end_function;
  }

  return_value = 0;

end_function:
 if (ip_name) free(ip_name);
 if (ip_module) free(ip_module);
 if (ip_arguments) free(ip_arguments);
 if (xmldoc) xmlFreeTextReader(xmldoc);
 if (ptr) free(ptr);
 if (name) free(name);

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting read_conf_file");
  return return_value;
}

static int rec_mkdir(struct soap * soap, char *spool_dir)
{
  char *path;
  char *path_c;
  int  path_len;
  DIR  *dp_spool;
  char errbuf[BUF_SIZE];

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering rec_mkdir");

  path_len = 0;
  dp_spool = opendir(spool_dir);
  if(!dp_spool)
  {
    if(errno == EACCES)
    {
      if (get_debug_flg() >= 1){
        sprintf(errbuf, "Error! Can not access directory: %s", spool_dir);
        idss_errmsg_r(errbuf);
        idss_errmsg_r("exiting rec_mkdir");
      }
      return -2;
    }
    else
    {
      path = (char *) calloc (strlen(spool_dir) + 1, sizeof (char));
      path_c = spool_dir;
      do
      {
        path_c = strchr(path_c + 1, '/');
        if(path_c)
        {
          strncpy(path, spool_dir, path_c - spool_dir);
          dp_spool = opendir(path);
          if(!dp_spool)
          {
            umask(0);
	    if (mkdir(path, S_IRWXU) == -1)
            {
              if (get_debug_flg() >= 1){
                sprintf(errbuf, "Error! mkdir error: %s", path);
	        idss_errmsg_r(errbuf);
	      }
		free(path);
    if (get_debug_flg() >= 1)
                idss_errmsg_r("exiting rec_mkdir");
              return -1;
            }
          }
          else
          {
            closedir(dp_spool);
	  }

          path_len = path_c - spool_dir;
        }
      } while (path_c);
      free(path);
      dp_spool = opendir(spool_dir);
      if(((path_len + 1) < (int)strlen(spool_dir)) && !dp_spool)
      {
        umask(0);
        if (mkdir(spool_dir, S_IRWXU) == -1)
        {
          if (get_debug_flg() >= 1){
            sprintf(errbuf, "Error! mkdir error: %s", spool_dir);
	    idss_errmsg_r(errbuf);
            idss_errmsg_r("exiting rec_mkdir");
	  }
          return -1;
        }
      }
      else
      {
        closedir(dp_spool);
      }
    }
  }
  else
  {
    closedir(dp_spool);
  }
  return 0;
}

void print_data()
{
  info_provider_node ip_node = NULL;
  struct host_node * hnode = NULL;
  char * name;

    if (get_debug_flg() >= 1)
                fprintf(stderr, "\n[%s:%d] entering print_data\n", __FILE__, __LINE__);

  fprintf(stderr, "Settings:\n");
  fprintf(stderr, "\tsystem_provider_sd:\t%s\n",system_provider_sd);
  fprintf(stderr, "\tuser_provider_sd:\t%s\n",user_provider_sd);
  fprintf(stderr, "\tistore_info_sd:\t\t%s\n",istore_info_sd);
  fprintf(stderr, "\tquery_tempfiles_sd:\t\t%s\n",query_tempfiles_sd);
  fprintf(stderr, "\tquery_results_sd:\t\t%s\n",query_results_sd);
  fprintf(stderr, "\tcollector_wp:\t\t%d\n",collector_wp);
  fprintf(stderr, "\tpurge_p:\t\t%d\n",purge_p);

  for (get_next_info_provider_node(&ip_node);ip_node;get_next_info_provider_node(&ip_node)){
    fprintf(stderr, "\nProvider:\n\tName:\t%s\n",ip_node->name);
    fprintf(stderr, "\tType:\t%d\n",ip_node->type);
    fprintf(stderr, "\tModule:\t%s\n",ip_node->module);
    fprintf(stderr, "\tArgs:\t%s\n",ip_node->arguments);
    fprintf(stderr, "\tTTL:\t%d\n",ip_node->ttl);
  }

  /*fprintf(stderr, "\nAllowed iNodes:\n");
  for (name = get_next_allowed_host(&hnode);name;name = get_next_allowed_host(&hnode)){
    fprintf(stderr, "\t%s\n",name);
  }*/

  hnode = NULL;
  fprintf(stderr, "\nRegistered iStore:\n");
  for (name = get_next_registered_host(&hnode);name;name = get_next_registered_host(&hnode)){
    fprintf(stderr, "\t%s\n",name);
  }

    if (get_debug_flg() >= 1)
                fprintf(stderr, "\n[%s:%d] exiting print_data\n", __FILE__, __LINE__);
}

/**********************
* Registered iStore
***********************/

void reset_registered_host_list()
{
  struct host_node* hnode;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering reset_registered_host_list");

  if (!registered_host_l)
    data_allocation();

  for(hnode = registered_host_l->first; hnode; hnode = hnode->next)
  {
    hnode->in_flag = 1;
  }
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting reset_registered_host_list");
}

/*
  this function returns a host. It uses parameter hnode as iterator
  for the internal host list.
  for the first invocation the parameter should be NULL, upon return
  the function will update the hnode parameter with a value that should
  be passed to get next host
*/
char * get_next_registered_host(struct host_node** hnode)
{
    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering get_next_registered_host");

  if (!registered_host_l)
    data_allocation();

  if (!*hnode)
    *hnode = registered_host_l->first;
  else
    *hnode = (*hnode)->next;

  for ( ;*hnode; *hnode = (*hnode)->next)
  {
    if ((*hnode)->in_flag)
      break;
  }


  if (!*hnode)
    return NULL;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting get_next_registered_host");

  return (*hnode)->hostname;
}

int get_numof_hidle_hosts()
{
  int numof = 0;
  struct host_node *hnode;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering get_numof_hidle_hosts");

  for(hnode = registered_host_l->first; hnode; hnode = hnode->next)
    if (hnode->in_flag == 0)
	  numof++;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting get_numof_hidle_hosts");
  return numof;
}

/*
  add new hostname on the top of the list.
  if the hostname already exixts it will not be added
  The function returns:
  1 if the hostnode already exists
  0 on success
*/
int add_registered_host(char *name)
{

  struct host_node *newhost = (struct host_node *) calloc (1, sizeof(struct host_node));

    if (get_debug_flg() >= 1)
                idss_errmsg_r("entering add_registered_host");
  if (!registered_host_l)
    data_allocation();

  if (get_registered_host_node(name) != NULL){
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting add_registered_host");
    free(newhost);
    return 1;
  }

  newhost->hostname = strdup(name);
  newhost->in_flag = 1;

  newhost->next = registered_host_l->first;
  registered_host_l->first = newhost;
  registered_host_l->nhost += 1;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting add_registered_host");
  return 0;
}

/*
  this function search the registered host list for 'name'
  it returns the handle of the node if the host exists otherwise
  returns NULL
*/
static struct host_node** get_registered_host_node (char* name)
{
  struct host_node** htmp;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering get_registered_host_node");

  if (!registered_host_l)
    data_allocation();

  if (!name)
    return NULL;

  htmp = &(registered_host_l->first);

  while(*htmp) {
    if (strcmp(name,(*htmp)->hostname) == 0)
      return htmp;
    htmp = &((*htmp)->next);
  }
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting get_registered_host_node");
  return NULL;
}

void delete_registered_host(char *name)
{
  struct host_node **todel;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering delete_registered_host");

  if (!registered_host_l)
  {
    data_allocation();
    return;
  }

  todel = get_registered_host_node(name);

  if (!todel)
    return;

  (*todel)->in_flag = 0;
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting delete_registered_host");
}

/**********************
* Allowed iNodes
***********************/

/*
  this function returns a host. It uses parameter hnode as iterator
  for the internal host list.
  for the first invocation the parameter should be NULL, upon return
  the function will update the hnode parameter with a value that should
  be passed to get next host
*/
/*
char * get_next_allowed_host(struct host_node** hnode)
{
    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering get_next_allowed_host");

  if (!allowed_inode_l)
    data_allocation();

  if (!*hnode) {
    *hnode = allowed_inode_l->first;
  }
  else {
    *hnode = (*hnode)->next;
  }

  for ( ; *hnode; *hnode = (*hnode)->next)
  {
    if ((*hnode)->in_flag)
      break;
  }

  if (!*hnode)
    return NULL;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting get_next_allowed_host");
  return (*hnode)->hostname;
}*/


/*
  add new hostname on the top of the list.
  if the hostname already exixts it will not be added
  The function returns:
  1 if the hostnode already exists
  0 on success
*/
/*int add_allowed_host(char *name)
{

  struct host_node *newhost = (struct host_node *) calloc (1, sizeof(struct host_node));

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering add_allowed_host");

  if (!allowed_inode_l)
    data_allocation();

  if (get_allowed_inode(name) != NULL){
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting add_allowed_host");
    return 1;
  }

  newhost->hostname = strdup(name);
  newhost->in_flag = 1;

  newhost->next = allowed_inode_l->first;
  allowed_inode_l->first = newhost;
  allowed_inode_l->nhost += 1;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting add_allowed_host");
  return 0;
}*/

/*int is_allowed_inode (char* name)
{
  return (get_allowed_inode(name) != NULL || get_allowed_inode("all") != NULL);
}*/

/*
  this function search the registered host list for 'name'
  it returns the handle of the node if the host exists otherwise
  returns NULL
*/
/*
static struct host_node** get_allowed_inode (char* name)
{
  struct host_node** htmp;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering get_allowed_inode");

  if (!allowed_inode_l)
    data_allocation();

  htmp = &(allowed_inode_l->first);

  while(*htmp) {
    if ((strcmp(name,(*htmp)->hostname) == 0))
      return htmp;
    htmp = &((*htmp)->next);
  }
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting get_allowed_inode");
  return NULL;
}*/
/*
void delete_allowed_inode(char *name)
{
  struct host_node **todel;

    if (get_debug_flg() >= 1)
      idss_errmsg_r("entering delete_allowed_inode");

  if (!allowed_inode_l)
  {
    data_allocation();
    return;
  }

  todel = get_allowed_inode(name);

  if (!todel)
    return;

  (*todel)->in_flag = 0;
    if (get_debug_flg() >= 1)
      idss_errmsg_r("exiting delete_allowed_inode");
}*/


/***********
* Settings
************/

int get_registration_ttl()
{
  return collector_wp;
}

int get_purge_period()
{
  return purge_p;
}

char* get_system_info_sd()
{
  return system_provider_sd;
}

char* get_user_supplied_info_sd()
{
  return user_provider_sd;
}

char* get_inode_info_sd()
{
  return istore_info_sd;
}

char* get_query_tempfiles_sd()
{
  return query_tempfiles_sd;
}

char* get_query_results_sd()
{
  return query_results_sd;
}

char* get_db_name()
{
  return sqlite_db_path;
}

char* get_ig_hostname()
{
  return hostname;
}

/**************************
* Providers Data Structure
***************************/

info_provider_node get_next_info_provider_node(info_provider_node* ip_node)
{

  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering get_next_info_provider_node");

  if (!provider_l)
    data_allocation();

  if (!*ip_node) {
    *ip_node = provider_l->first;
  }
  else {
  	*ip_node = (*ip_node)->next;
  }

  if (!*ip_node){
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting get_next_info_provider_node");
    return NULL;
  }

  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting get_next_info_provider_node");
  return *ip_node;
}


/*
  add new hostname on the top of the list.
  if the hostname already exixts it will not be added
  The function returns:
  1 if the hostnode already exists
  0 on success
*/
int add_info_provider_node(info_provider_node ip_node)
{

  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering add_info_provider_node");

  if (!provider_l)
    data_allocation();

  if (!ip_node){
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting add_info_provider_node");
    return 0;
  }

  if (get_info_provider_node(ip_node->name) != NULL){
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting add_info_provider_node");
    return 1;
  }

  ip_node->next = provider_l->first;
  provider_l->first = ip_node;
  provider_l->nproviders += 1;

  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting add_info_provider_node");
  return 0;
}

/*
  this function search the registered host list for 'name'
  it returns the handle of the node if the host exists otherwise
  returns NULL
*/
static info_provider_node* get_info_provider_node (char* name)
{
  info_provider_node* htmp;

  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering get_info_provider_node");

  if (!provider_l)
    data_allocation();

  htmp = &(provider_l->first);

  while(*htmp) {
    if (strcmp(name,(*htmp)->name) == 0)
      return htmp;
    htmp = &((*htmp)->next);
  }
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting get_info_provider_node");
  return NULL;
}

void delete_info_provider_node(char *name)
{
  info_provider_node *todel;
  info_provider_node htmp;

  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering delete_info_provider_node");

  if (!provider_l)
  {
    data_allocation();
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting delete_info_provider_node");
    return;
  }

  todel = get_info_provider_node(name);

  if (!todel){
  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting delete_info_provider_node");
    return;
  }

  if ((*todel)->name)
    free((*todel)->name);
  if ((*todel)->module)
    free((*todel)->module);
  if ((*todel)->arguments)
    free((*todel)->arguments);

  htmp = (*todel)->next;
  free(*todel);
  *todel = htmp;

  provider_l->nproviders -= 1;
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting delete_info_provider_node");
}

info_provider_node create_info_provider_node(char* name, provider_type type, char* module, char* args, int ttl)
{
  info_provider_node ip_node;

  if (get_debug_flg() >= 1)
    idss_errmsg_r("entering create_info_provider_node");

  if (!name || !module){
  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting create_info_provider_node");
    return NULL;
  }

  ip_node = (info_provider_node)calloc(1,sizeof(struct _info_provider_node));
  ip_node->name = strdup(name);
  ip_node->module = strdup(module);
  ip_node->ttl = ttl;

  if (!args)
    ip_node->arguments = NULL;
  else
    ip_node->arguments = strdup(args);

  ip_node->type = type;
  ip_node->pid = -1;

  if (get_debug_flg() >= 1)
    idss_errmsg_r("exiting create_info_provider_node");
  return ip_node;

}

