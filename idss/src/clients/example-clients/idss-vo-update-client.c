/** @file idss-vo-update-client
 * Updates some information about VO.
 *
 * To run
 *       ./idss-vo-update-client <options>
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

#include <unistd.h>
#include "idssStub.h"
#include <stdio.h>
#include <syslog.h>
#include "gsi.h"
#include "idss_client.h"

extern char *optarg;


/* authorization callback prototype */
int gsi_authorization_callback (struct soap *soap);

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-o <VO name>: set the name of" \
          " the Virtual Organization that you want to update\n");
  printf ("\t-t VO resource type: set updated resource type\n");
  printf ("\t-j VO job manager: set updated job manager\n");
  printf ("\t-q VO queue: set updated queue\n");
  printf ("\t-k VO HelpDeskURL: set updated help-desk url\n");
  printf ("\t-a VO AdminName: set updated administrator name\n");
  printf ("\t-n <VO host name>: set the name of the host belonging to " \
          "the Virtual Organization\n");
  printf ("\t-m VO validity time: set updated validity time of the Virtual" \
	  " Organization information\n");
  printf ("\t-u usage: two cases\n\t - if you want to update VO resource type, VO job manager, VO queue, VO HelpDeskURL, VO AdminName please specify the name of the VO that you want to update\n\t - if you want to update the VO validity time, please specify the host belonging to the Virtual Organization.\n");
  printf ("\t-v verbose mode\n");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  static struct gsi_plugin_data *data;
  int                            result;
  char                           *server;
  char                           *vo;
  char                           *helpDeskURL;
  char                           *adminname;
  char                           *restype;
  char                           *jobm;
  char                           *queue;
  char                           *hostname;
  char                           *msg;
  char                           contact_string[256];
  unsigned short int             port;
  int                            i, rc;
  int                            verbose;
  unsigned int                   valtime;

  /* Setting default values */
  vo = NULL;
  helpDeskURL = NULL;
  adminname = NULL;
  restype = NULL;
  jobm = NULL;
  queue = NULL;
  hostname = NULL;
  msg = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  valtime = 0;

  while ((i = getopt (argc, argv, "h:p:o:t:j:q:k:a:n:m:uv")) != EOF)
  {
    switch (i)
    {
      case 'h':
        server = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 'o':
        vo = strdup (optarg);
        break;
      case 't':
        restype = strdup (optarg);
        break;
      case 'j':
        jobm = strdup (optarg);
        break;
      case 'q':
        queue = strdup (optarg);
        break;
      case 'k':
        helpDeskURL = strdup (optarg);
        break;
      case 'a':
        adminname = strdup (optarg);
        break;
      case 'n':
        hostname = strdup (optarg);
        break;
      case 'm':
        valtime = atoi (optarg);
        break;
      case 'v':
        verbose = 1;
        break;
      case 'u':
      default:
        usage (argv[0]);
        exit (1);
    }
  }
  if (verbose)
  {
    printf ("Updating the following Virtual Organization:\n");
    printf ("VO name:%s\n", vo);
    printf ("VO resource type:%s\n", restype);
    printf ("VO job manager:%s\n", jobm);
    printf ("VO queue:%s\n", queue);
    printf ("VO Help Desk URL:%s\n", helpDeskURL);
    printf ("VO admin name:%s\n", adminname);
  }

  
  globus_module_activate (GLOBUS_GSI_GSSAPI_MODULE);

  soap_init1(&soap, SOAP_IO_CHUNK);


  /* register the gsoap-gsi plugin */
  if (soap_register_plugin (&soap, globus_gsi))
  {
    soap_print_fault (&soap, stderr);
    exit (1);
  }

  /* setup of authorization callback */
  data = (struct gsi_plugin_data *) soap_lookup_plugin (&soap, GSI_PLUGIN_ID);
  data->gsi_authorization_callback = NULL;

  /* we begin acquiring our credential */
  rc = gsi_acquire_credential(&soap);
  if(rc < 0)
  {
	 if (vo)
	  {
		free(vo);
	  }
	  if (restype)
	  {
		free(restype);
	  }
	  if (jobm)
	  {
		free(jobm);
	  }
	  if (queue)
	  {
		free(queue);
	  }
	  if (helpDeskURL)
	  {
		free(helpDeskURL);
	  }
	  if (adminname)
	  {
		free(adminname);
	  }
	
	  soap_end (&soap);
	  soap_done (&soap);
	  
	  
	  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);
	exit(1);
  }
   /* setup of GSI channel */
  gsi_set_replay(&soap, GLOBUS_TRUE);
  gsi_set_sequence(&soap, GLOBUS_TRUE);
  gsi_set_confidentiality(&soap, GLOBUS_TRUE);
  gsi_set_integrity(&soap, GLOBUS_TRUE);


  sprintf (contact_string, "https://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to web service on... %s\n", contact_string);
    printf ("Updating VO resource type...%s\n", restype);
    printf ("Updating VO job manager...%s\n", jobm);
    printf ("Updating VO queue...%s\n", queue);
    printf ("Updating VO Help Desk URL...%s\n", helpDeskURL);
    printf ("Updating VO admin name...%s\n", adminname);
  }

  /*
   * soap_call_idss__update_vo
   *
   * updates resource type, job manager, queue, help desk
   * url, admin DN and validity time of a VO
   *
   * name: name of the Virtual Organization to be updated
   * restype: resource type of the Virtual Organization to be updated
   * jobm: jobmanager used by the Virtual Organization to be updated
   * queue: job queue of the Virtual Organization to be updated.
   * helpDeskURL: the URL of the help-Desk of the Virtual Organization
   * to be updated
   * adminname: Distinguished Name of the administrator of the Virtual
   * Organization to be updated
   * host: the name of the host belonging to the Virtual Organization
   * validity_time: validity time of the Virtual Organization information
   * to be updated
   * result: set to -1 if an error occurs, 0 on success.
   */

  if (soap_call_idss__update_vo (&soap, contact_string, "",
          vo, restype, jobm, queue, helpDeskURL, adminname, hostname,
          valtime, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);			
      if ((result != idss_str_vo_up_ok) && (result != idss_str_vo_instance_up_ok))
      {
        usage(argv[0]);
      }
      exit (1);
    }
    else
    {
      printf ("Error during service update\n");
    }
    
  }
  if (vo)
  {
    free(vo);
  }
  if (restype)
  {
    free(restype);
  }
  if (jobm)
  {
    free(jobm);
  }
  if (queue)
  {
    free(queue);
  }
  if (helpDeskURL)
  {
    free(helpDeskURL);
  }
  if (adminname)
  {
    free(adminname);
  }

  soap_end (&soap);
  soap_done (&soap);
  
  
  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);
  
  return 1;
}

/*
 * gsi_authorization_callback
 *
 * checks the received distinguished name against an access control list
 * stored in the file called authorized_dn. If the distinguished name 
 * received belongs 
 * to the access control list then the client can safely access 
 * the service
 *
 */
 
int
gsi_authorization_callback (struct soap *soap)
{
  char buf[512] = {'\0'}; 
  char *auth;
  char *auth_file;
  FILE *fd;
  
  struct gsi_plugin_data *data;
  
  data = (struct gsi_plugin_data *) soap_lookup_plugin (soap, GSI_PLUGIN_ID);

  auth = globus_libc_getenv("AUTHORIZATION_FILE");
  if(auth != NULL)
  {
    auth_file = strdup(auth);
  }
  else
  {
    auth_file = strdup (AUTHORIZATION_FILE);
  }
  
  fd = fopen (auth_file, "r");
  
  if (!fd)
  {

    globus_libc_printf ("Can not read file: %s\n", AUTHORIZATION_FILE);
    syslog(LOG_ALERT | LOG_USER, "Cannot read authorized_dn file");
    return -1;
  }
  while (fgets (buf, 512, fd) != NULL)
  {
    buf[strlen (buf) - 1] = '\0';
    if (!strcmp (data->server_identity, buf))
    {
      syslog(LOG_NOTICE | LOG_USER, "You are connecting to %s", 
              data->server_identity);
      if(auth)
      {
        free(auth);
      }
      if(auth_file)
      {
        free(auth_file);
      }

      fclose (fd);

      return 0;
    }
  }
  if(auth)
  {
    free(auth);
  }
  if(auth_file)
  {
    free(auth_file);
  }
  fclose (fd);

  globus_libc_printf ("Sorry, service %s is not authorized\n",
          data->server_identity);
  syslog(LOG_NOTICE | LOG_USER, "Service %s not authorized\n",
          data->server_identity);
  return -1;
}


struct Namespace namespaces[] = {
  {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/"},
  {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/"},
  {"xsi", "http://www.w3.org/1999/XMLSchema-instance"},
  {"xsd", "http://www.w3.org/1999/XMLSchema"},
  {"idss", "urn:idss"},
  {NULL, NULL}
};
