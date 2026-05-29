/** @file collector.c
 * functionalities to be used to collect information about iServe.
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
//#include "config.h"
#include "idssH.h"
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "idss_data.h"
#include "idss_server.h"
#include "utils.h"
#include "data_access_structures.h"
#include "data_access_func.h"

extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;

/*******************
 * Macro Definition
 *******************/
#define IDSS_DUMP_FILENAME "dump"
#define BUF_SIZE 1024

/*************
 * Variables
 *************/
struct dime_write_handle
{
  char *name;
  FILE *fd;
};

static pthread_mutex_t mulock;

/*****************************
 * Static Function Declaration
 *****************************/

/* putData prototype */
static void putData(char *db_name, char* hostname);


/**********************
 * Functions definition
 **********************/

/*
 * ! \fn void *collect_main()
 *
 * \brief This function manages the collection of iServe information
 * and periodically updates the list of registered iStores to which
 * information has to be sent
 *
 */

#if 0
void* collect_main (void* myarg)
{
  int              i;			/* internal index */
  int              period;		/* purge period(sec) for registration of iServe to iStores */
  int              steps;		/* internal index */
  char             *hostname;		/* points to a registered iStore hostname */
  char             dump_filename[128];	/* MQ dump filename */
  struct host_node *hnode;		/* index for iStores list */
  struct soap *soap;
  char errbuf[BUF_SIZE];

  	if (get_debug_flg() >= 1)
	        idss_errmsg_r("entering collect_main");

  pthread_detach(pthread_self());

  soap = (struct soap*) myarg;

  /* init the mutex */
  pthread_mutex_init (&mulock, NULL);

  period = get_registration_ttl();
  steps = get_purge_period()/period + 1;
  i = steps;

  /* IDSS debug section */
  if (get_debug_flg() >= 1)
  {
    sprintf(errbuf, "Thread collector created: %lu", (unsigned long)pthread_self());
    idss_errmsg_r(errbuf);
  }

  /* first writing of the MQ dump file containing old iServe info and sending to registered iStores */
  sprintf(dump_filename, "%s_%s", IDSS_SERVICE_HOSTNAME, IDSS_DUMP_FILENAME);
  if(create_dump(dump_filename) == 0)
  {

    for(hnode = NULL, hostname = get_next_registered_host(&hnode); hostname; hostname = get_next_registered_host(&hnode))
    {
      putData(dump_filename, hostname);
    }
    unlink(dump_filename);
  }

  /* loop for the collection of iServe information */
  while(1)
  {
	i--;

    /* dump of old iServe info after steps time */
    if(i == 0 && get_numof_hidle_hosts() > 0)
    {
      /* IDSS debug section */
      if (get_debug_flg() >= 1)
      {
	 idss_errmsg_r("Restoring idle nodes");
      }
      /*
       * periodical writing of the MQ dump file containing old iServe info
       * and sending to previously unreached iStores
       */
      if(create_dump(dump_filename) == 0)
      {
        hnode = NULL;
	for(get_next_registered_host(&hnode); hnode; hnode = hnode->next)
        {
          /* the dump file will be sent to each previously unreached iStore */
          if (!(hnode->in_flag))
          {
            putData(dump_filename, hnode->hostname);
          }
        }
        unlink(dump_filename);
      }
      /* reset original list of iSores */
      reset_registered_host_list();
    }
    if (!i)
    {
      i = steps;
    }
    sleep(period);

    /* IDSS debug section */
    if (get_debug_flg() >= 1)
    {
      idss_errmsg_r("Collecting Data");
    }

    /* collection of new iServe info to be sent to registered iStores */
    collect(soap);

  }
}

/*
 * ! \fn void *collect(struct soap *soap)
 *
 * \brief This function collects new iServe information stored into
 * three different spool directories, writes them into a file
 * and sends it to the list of reached registered iStores
 *
 */
void collect(struct soap *soap)
{
  DIR                           *host_dp;		/* points to the inode info dir */
  DIR                           *dp;			/* points to the */
  FILE                          *tmpxml;		/* points to the MQ temporary iServe new info file */
  FILE                          *fxml;			/* points to a MQ source file in a spool dir */
  struct dirent                 *dirp;			/* internal pointer */
  struct stat                   sst;			/* internal variable */
  struct host_node              *current;		/* index for iStores list */
  char                          **bufile = NULL;	/* temporary buffer */
  char                          fxmlpath[256];		/* final MQ info file name */
  char                          txmlpath[256];		/* temporary MQ info file name */
  char                          xmlfile[256];		/* MQ source info file name */
  char                          *spooldir[3];		/* array of spool dir */
  char                          *hostname;		/* points to a registered iStore hostname */
  int                           j;			/* internal index */
  int                           h;			/* internal index */
  int                           n;			/* internal index */
  int                           flag;			/* flag indicating if the collection has to be done */
  time_t now = time (0);
  char errbuf[BUF_SIZE];

  	if (get_debug_flg() >= 1)
	        idss_errmsg_r("entering collect");

  /* block until the mutual exclusion lock, mutex, is acquired */
  pthread_mutex_lock(&mulock);

  /* setting of the spool directories path */
  spooldir[0] = get_inode_info_sd();
  spooldir[1] = get_system_info_sd();
  spooldir[2] = get_user_supplied_info_sd();

  /* creation the MQ temporary iServe new info file and writing of the first two lines */
  host_dp = opendir(spooldir[0]);
  if(host_dp)
  {
    sprintf(txmlpath, "%s/%s", spooldir[0], "tmpxml");
    tmpxml = fopen(txmlpath, "w");
    fprintf(tmpxml, "%s\n", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    fprintf(tmpxml, "<TABLES grelcdb_name=\"%s\">\n", get_db_name());
    closedir(host_dp);
  }
  else
  {

    /* internal logging */
    if (get_debug_flg() >= 1){
      sprintf(errbuf, "Error! Permission denied: can not create directory %s", spooldir[0]);
      idss_errmsg_r(errbuf);
    }
    pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting collect");
    return;
  }

  /* writing of the MQ temporary iServe new info file */
  flag = 0;
  for(n = 0; n <= 2; n++)
  {
    dp = opendir(spooldir[n]);
    if(dp)
    {
      /* reading spool dir omitting hidden files */
      while((dirp = readdir(dp)))
      {
        if (dirp->d_name[0] != '.')
        {
          sprintf(xmlfile, "%s/%s", spooldir[n], dirp->d_name);
          if (stat(xmlfile, &sst) < 0)
          {

	    /* internal logging */
            if (get_debug_flg() >= 1){
              sprintf(errbuf, "Error! Stat error for file %s", xmlfile);
	      idss_errmsg_r(errbuf);
	    }
	    pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
	        idss_errmsg_r("exiting collect");
            return;
          }
          if (!(sst.st_mode & S_IXUSR))
          {
            if((n != 0) || ((n == 0) && strcmp(dirp->d_name, "tmpxml")))
            {
              sprintf(xmlfile, "%s/%s", spooldir[n], dirp->d_name);
              flag = 1; /* new info has to be stored */
              fxml = fopen(xmlfile, "r");
              j = 2;

	      /* reading new info from each MQ file stored in the spool dir */
              bufile = (char **) malloc ((j + 1) * sizeof (char *));
              for(h = 0; h <= j; h++)
              {
                bufile[h] = (char *) calloc (1024, sizeof (char));
              }
              for(h = 0; h < j; h++)
              {
                fgets(bufile[h], 1024, fxml);
              }
              /* writing new info into the MQ temporary iServe new info file */
			    while ((fgets(bufile[j], 1024, fxml) != NULL) && (strcmp(bufile[j], "</TABLES>\n")))
              {
                fprintf(tmpxml, "%s", bufile[j]);
                j++;
                bufile = (char **) realloc (bufile, (j + 1) * sizeof(char *));
                bufile[j] = (char *) calloc (1024, sizeof (char));
              }

              /* free temporary buffer */
			   for(h = 0; h <= j; h++)
              {
                free(bufile[h]);
              }
              free(bufile);
	      /* closing the MQ source info file */
              if(fclose(fxml))
              {
		/* internal logging */
                if (get_debug_flg() >= 1){
                  sprintf(errbuf, "Error! Close error: file %s", xmlfile);
		  idss_errmsg_r(errbuf);
		}
	      }
	      /* removal of the MQ source info file */

              if(remove(xmlfile))
	      {
		/* internal logging */
                if (get_debug_flg() >= 1){
                  sprintf(errbuf, "Error! Remove error: file %s", xmlfile);
		  idss_errmsg_r(errbuf);
		}
	      }
            }
          }
        }
      }
      free(dirp);
      closedir(dp);
    }
    else
    {
      /* internal logging */
      if (get_debug_flg() >= 1){
        sprintf(errbuf, "Permission denied: can not open directory %s", spooldir[n]);
        idss_errmsg_r(errbuf);
      }
      pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting collect");
      return;
    }
  }

  /* sending of the MQ iServe new info file to registered iStores */
  host_dp = opendir(spooldir[0]);
  if(host_dp)
  {
    /* writing of the last line into the final MQ file */
    fprintf(tmpxml, "%s", "</TABLES>\n");
    fclose(tmpxml);
    if(flag == 1)
    {
      sprintf(fxmlpath, "%s/%s_%ld", spooldir[0], IDSS_SERVICE_HOSTNAME, (long)mktime(gmtime(&now)));
      rename(txmlpath, fxmlpath);
      /* the MQ file will be sent to each reached iStore */
      for(current = NULL, hostname = get_next_registered_host(&current); hostname; hostname = get_next_registered_host(&current))
      {
        putData(fxmlpath, hostname);
      }

      /* removal of the MQ iServe new info file */
      if(unlink(fxmlpath) < 0)
      {
        closedir(host_dp);
	pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
		idss_errmsg_r("exiting collect");
        return;
      }
    }
    else
    {
      /* removal of the temporary MQ iServe new info file */

      if(unlink(txmlpath) < 0)
      {
        closedir(host_dp);
	pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
	   idss_errmsg_r("exiting collect");
        return;
      }
    }
    closedir(host_dp);
  }

  pthread_mutex_unlock(&mulock);
  	if (get_debug_flg() >= 1)
	   idss_errmsg_r("exiting collect");
  return;
}

/*
 * ! \fn void putData(char *name, char* hostname)
 *
 * \brief This function sends the info stored into a file to a remote host
 * calling the idss_istore web service method
 *
 * \param name The full path of the file where info are stored
 * \param hostname The FQDN of the remote host where info has to be sent
 *
 */
void putData(char *name, char* hostname)
{
  struct stat            sb;			/* internal variable */
  // struct dimeData        *dime_data;		/* points to the dime struct containing info to be sent */
  struct soap            soap1;			/* gSOAP struct */
  int                    result;		/* result of the call to the idss_istore */
  FILE                   *sxml = NULL;			/* points to the source file */
  FILE                   *dxml = NULL;			/* points to the destination file */
  size_t                 nbyte = 0;		/* number of byte read from the source file */
  char                   *option_name = NULL;	/* points to the file name */
  char                   *cbyte = NULL;		/* internal buffer */
  char                   *buf = NULL;		/* internal buffer */
  char                   *domain_istore = NULL; /* interval variable */
  char                   *domain_iserve = NULL; /* interval variable */
  char                   buff[1024] = {'\0'};  /* internal buffer */
  char                   txml[256] = {'\0'};	/* stores the name of the temporary file */
  char                   fxml[256] = {'\0'};		/* stores the name of the final file */
  char			 		 rfile[256] = {'\0'};
  char 				     errbuf[BUF_SIZE] = {'\0'};

  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("entering putData");

  soap_init(&soap1);

  /* check if the iDSS Istore server is in the same domain of the iDSS iServe server.
   * Compression is enabled only if the iStore isn't in the same domain of the iServe. */

  domain_istore = domainname(hostname);
  domain_iserve = domainname(IDSS_SERVICE_HOSTNAME);

if(domain_istore && domain_iserve)

if(strcmp(domain_istore, domain_iserve))
  {
      soap_set_omode(&soap1, SOAP_ENC_ZLIB);
      soap1.z_level = 9; /* best compression */
      if (get_debug_flg() >= 1)
        idss_errmsg_r("Enabled Compression");
  }

  if(domain_istore)
    free(domain_istore);
  if(domain_iserve)
    free(domain_iserve);

  /*soap1.fdimereadopen = dime_read_open;
  soap1.fdimereadclose = dime_read_close;
  soap1.fdimeread = dime_read;
 */
  /* IDSS debug section */
  if(get_debug_flg() >= 1)
  {
    sprintf(errbuf, "Sending data to %s", hostname);
    idss_errmsg_r(errbuf);
  }


  /* removal of the reference to the iServe (local) db name into the MQ file to be sent */
	if (get_debug_flg() >= 1){
        sprintf(errbuf,"name to send:%s\n", name);
        idss_errmsg_r(errbuf);
      }

  // dime_data = (struct dimeData*)soap_malloc(&soap1, sizeof(struct dimeData));
  if (!stat(name, &sb) && sb.st_size > 0)
  {
    sxml = fopen(name, "r");
    if (!sxml)
    {

      if (get_debug_flg() >= 1){
        sprintf(errbuf,"Error! File %s not found", name);
        idss_errmsg_r(errbuf);
      }
  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting putData");
      return;
    }
    buf = (char *) calloc (sb.st_size + 1, sizeof (char));
    if (!buf)
    {
  	if (get_debug_flg() >= 1)
          idss_errmsg_r("Memory allocation error");
  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting putData");
      return;
    }
    cbyte = buf;
    fgets(buff, 1024, sxml);
    fgets(buff, 1024, sxml);

    /* reading of file lenght (byte) */
    while(!feof(sxml))
    {
      nbyte = fread(buf, 1, 1024, sxml);
      buf = buf + nbyte;
    }

    /* writing of new MQ file without reference to iServe local db name */
    *buf = 0;
    buf = cbyte;
    fclose(sxml);
    sprintf(txml, "%s/.tmpfile", get_inode_info_sd());
    dxml = fopen(txml, "w");
    fprintf(dxml, "%s", buf);
    fclose(dxml);

    option_name = strrchr(name, '/');
    if (!option_name || *option_name == 0)
    {
      option_name = name;
	  sprintf(fxml, ".%s", name);
 	//  dime_data->options = soap_dime_option(&soap1, 0, fxml);
    }
    else
    {
	  option_name++;
	  sprintf(rfile, ".%s", option_name);
	//  dime_data->options = soap_dime_option(&soap1, 0, rfile);
	  sprintf(fxml, "%s/%s", get_inode_info_sd(), rfile);
    }

    rename(txml, fxml);
   // free(buf);
  }

  /* writing of MQ file name obtained from the full path */
  if (!stat(fxml, &sb) && sb.st_size > 0)
  {
//  dime_data->__ptr = buf;
//	dime_data->__ptr = (unsigned char*) fxml;
  /*      dime_data->__size = sb.st_size;
        dime_data->type = strdup("text/xml");
	dime_data->id = strdup("1");
*/
//dime_data->__size = 0;
//dime_data->__size = sb.st_size;
//dime_data->type ="";


    /* call to the idss_istore method */
    if (soap_call_idss__istore2(&soap1, hostname, NULL, buf, IDSS_SERVICE_HOSTNAME, &result))
    {
      soap_print_fault(&soap1, stderr);
      delete_registered_host(hostname);
    }
    unlink(fxml);
  }
  else
  {
    if (get_debug_flg() >= 1){
      sprintf(errbuf, "Error! fstat error: file %s", name);
      idss_errmsg_r(errbuf);
    }
  }

  if(buf)
    free(buf);

  // free(dime_data->type);
// free(dime_data->id);

    soap_destroy(&soap1);
    soap_end(&soap1);
    soap_done(&soap1);

  	if (get_debug_flg() >= 1)
	  idss_errmsg_r("exiting putData");

   return;
}


/*
 * ! \fn int create_dump(char *dump_filename)
 *
 * \brief This function collects old valid info stored into iServe db
 * to be sent to each previously unreached iStore
 *
 * \param dump_filename The full path of the file where info will be collect
 *
 */


//static int create_dump(char *dump_filename)

#endif

