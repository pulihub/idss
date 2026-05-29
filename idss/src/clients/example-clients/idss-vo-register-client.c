/** @file idss-vo-register-client.c
 * Registers a Virtual Organization "owning" the grid resource
 *
 * To run
 *        ./idss-vo-register-client <options>
 *  
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

//#include "config.h"
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
  printf ("\t-p web_service_port: port number where the iDSS" \
          " web service is listening on\n");
  printf ("\t-o <VO name>: set Virtual Organization name\n");
  printf ("\t-d <VO HelpDeskPhoneNumber>: set multi-valued help-desk" \
          " phone number\n");
  printf ("\t-t VO resource type: set resource type\n");
  printf ("\t-j VO job manager: set job manager used by the " \
          "Virtual Organization\n");
  printf ("\t-q VO queue: set job queue of the Virtual Organization\n");
  printf ("\t-s <VO_filesystem_path>:VO_filesystem_type : set multi-valued " \
         "file systems (type:pathname) for Virtual Organization\n");
  printf ("\t-k VO HelpDeskURL: set the URL of the help-Desk\n");
  printf ("\t-a VO AdminName: set name of the Virtual Organization administrator\n");
  printf ("\t-n VO host name: set the name of the host to be registered to " \
          "the Virtual Organization\n");
  printf ("\t-m VO validity time: set validity time of the Virtual" \
	  " Organization information\n");
  printf ("\t-u usage\n");
  printf ("\t-v verbose\n");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  static struct gsi_plugin_data *data;
  int                            result;
  char                           *server;
  char                           *vo;
  char                           *helpPhoneN;
  unsigned short int             port;
  char                           *helpDeskURL;
  char                           *adminname;
  char                           *restype;
  char                           *jobm;
  char                           *queue;
  char                           *fs_path;
  char                           *msg;
  char                           contact_string[256];
  char                           *hostname;
  int                            i, rc;
  int                            verbose;
  unsigned int                   valtime;

  /* Setting default values */
  vo = NULL;
  helpPhoneN = NULL;
  helpDeskURL = NULL;
  adminname = NULL;
  restype = NULL;
  jobm = NULL;
  queue = NULL;
  fs_path = NULL;
  hostname = NULL;
  msg = NULL;
  valtime = 0;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;  
  verbose = 0;

  while ((i = getopt (argc, argv, "h:p:o:d:t:j:q:s:k:a:n:m:uv")) != EOF)
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
      case 'd':
        helpPhoneN = strdup (optarg);
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
      case 's':
        fs_path = strdup (optarg);
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
    printf ("registering the following Virtual Organization:\n");
    printf ("Virtual Organization name: %s\n", vo);
    printf ("Virtual Organization help-desk phone number :%s\n", helpPhoneN);
    printf ("Virtual Organization resource type :%s\n", restype);
    printf ("Virtual Organization job manager :%s\n", jobm);
    printf ("Virtual Organization queue :%s\n", queue);
    printf ("Virtual Organization multi-valued filesystem <path:type>:%s\n", fs_path);
    printf ("Virtual Organization help-desk URL:%s\n", helpDeskURL);
    printf ("Virtual Organization administrator's name:%s\n", adminname);
    printf ("Virtual Organization host name:%s\n", hostname);
    printf ("Virtual Organization validity time:%lu\n", (unsigned long)valtime);
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
    if(server)
    free (server);

	  if (vo)
	  {
		free (vo);
	  }
	  if (helpPhoneN)
	  {
		free (helpPhoneN);
	  }
	  if (restype)
	  {
		free (restype);
	  }
	  if (jobm)
	  {
		free (jobm);
	  }
	  if (queue)
	  {
		free (queue);
	  }
	  if (fs_path)
	  {
		free (fs_path);
	  }
	  if (helpDeskURL)
	  {
		free (helpDeskURL);
	  }
	  if (adminname)
	  {
		free (adminname);
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
    printf ("Registering Virtual Organization... %s\n", vo);
  }

  /*
   * soap_call_idss__register_vo
   *
   * allows the user to register the Virtual Organization "owning"
   * the grid resource
   *
   * vo: name of the Virtual Organization
   * helpDeskPN: a list, space-separated, of the Help-Desk Phone number of the
   * Virtual Organization
   * restype: Virtual Organization resource type
   * jobm: jobmanager used by the Virtual Organization
   * queue: job queue of the Virtual Organization
   * fs_path: a list, space-separated, of the Virtual Organization
   * file system (type:pathname)
   * helpDeskURL: the URL of the help-Desk
   * adminname: name of the VO administrator
   * hostname: name of the host to be registered to the Virtual Organization
   * valtime: validity time of the Virtual Organization information.
   * Default: 0 the information is always valid
   * result: set to -1 if an error occurs, 0 on success
   */
  if (soap_call_idss__register_vo (&soap, contact_string, "", vo,
              helpPhoneN, restype, jobm, queue, fs_path, helpDeskURL,
              adminname, hostname, valtime, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if(msg)
    {
      printf("%s\n", msg);
      if((result != idss_str_vo_ok) && (result != idss_str_vo_instance_ok))
      {
        usage(argv[0]);
      }
     }
     else
     {
       printf ("Error during Virtual Organization registration\n");
     }
    
  }

  if(server)
    free (server);

  if (vo)
  {
    free (vo);
  }
  if (helpPhoneN)
  {
    free (helpPhoneN);
  }
  if (restype)
  {
    free (restype);
  }
  if (jobm)
  {
    free (jobm);
  }
  if (queue)
  {
    free (queue);
  }
  if (fs_path)
  {
    free (fs_path);
  }
  if (helpDeskURL)
  {
    free (helpDeskURL);
  }
  if (adminname)
  {
    free (adminname);
  }
  
  soap_end (&soap);
  soap_done (&soap);
  
  
  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);

  return 1;
}

struct Namespace namespaces[] = {
  {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/"},
  {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/"},
  {"xsi", "http://www.w3.org/1999/XMLSchema-instance"},
  {"xsd", "http://www.w3.org/1999/XMLSchema"},
  {"idss", "urn:idss"},
  {NULL, NULL}
};
