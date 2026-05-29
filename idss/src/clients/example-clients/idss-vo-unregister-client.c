/** @file idss-vo-unregister-client.c
 * Removes an existing virtual organization
 *
 * To run
 *        ./idss-vo-unregister-client <options>
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

static void usage(char* name)
{
  printf("%s: Usage:\n",name);
  printf("\t-o <VO name>: name of the Virtual Organization" \
         " you want to unregister\n");
  printf("\t-n VO hostname: host name of the machine belonging to " \
         "the Virtual Organization you want to unregister\n");
  printf ("\t-a : unregister all the Virtual Organization instances\n");
  printf("\t-h web_service_hostname: FQDN of the server hosting" \
         " the iDSS web service\n");
  printf("\t-p web_service_port: port number where the iDSS" \
         " web service is listening on\n");
  printf("\t-v : verbose mode\n");
  printf("\t-u : usage\n");
  printf ("\t-u : two cases \n\t - If you want to unregister all of the hosts beloging to a VO, please specify the VO name and -a option \n\t - If you want to unregister a host beloging to a VO, please specify the VO name and hostname of the VO to be unregistered. Default hostname is the server hosting the iDSS web service\n");
}


int main(int argc, char **argv)
{
  struct soap                    soap;
  static struct gsi_plugin_data  *data;
  char                           *name;
  char                           *hostname;
  char                           *msg;
  char                           *ws_hostname;
  char                           contact_string[256];
  unsigned short int             port;
  int                            verbose;
  int                            allinst;
  int                            result;
  int                            i, rc;

  /* Setting default values */
  name = NULL;
  hostname = NULL;
  msg = NULL;
  ws_hostname = strdup(IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  allinst = 0;

  while ((i=getopt(argc,argv,"h:p:o:n:avu")) > 0)
  {
    switch(i)
    {
      case 'h':
        free(ws_hostname);
        ws_hostname = strdup(optarg);
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'o':
        name = strdup(optarg);
        break;
      case 'n':
        hostname = strdup(optarg);
        break;
      case 'a':
        allinst = 1;
        break;
      case 'v':
        verbose = 1;
        break;
      case 'u':
      default:
        usage(argv[0]);
        exit(1);
    }
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
    if(name)
	  {
		free(name);
	  }
	  if(hostname)
	  {
		free(hostname);
	  }
	  
	  soap_end (&soap);
	  soap_done (&soap);
	  
	  
	  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);
    exit(1);
  }

  /* setup of GSI channel */
  gsi_set_sequence(&soap, GLOBUS_TRUE);
  gsi_set_confidentiality(&soap, GLOBUS_TRUE);
  gsi_set_integrity(&soap, GLOBUS_TRUE);

  sprintf (contact_string, "https://%s:%d", ws_hostname, port);
  contact_string[strlen(contact_string)] = '\0';
  if (verbose)
  {
    printf("Unregistering Virtual Organization... %s\n",name);
    printf("Contacting service on... %s\n",contact_string);
  }

/*
 * soap_call_idss__unregister_vo
 *
 * removes information about a Virtual Organization
 *
 * name: name of the Virtual Organization whose information must be removed
 * hostname: host name of the machine belonging to the Virtual Organization
 * allinst: set to 1 to remove specified Virtual Organization information and 
 * the relations between it and all of the registered hosts, set to 0 to 
 * remove a single relation
 * result: set to -1 if an error occurs, 0 on success. If the VO information
 * does not exist the method sets result to -1.
 */

  if (soap_call_idss__unregister_vo
          (&soap, contact_string, "", name, hostname, allinst, &result ))
  {
    soap_print_fault(&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
      if((result != idss_str_vo_instance_del_ok) && (result != idss_str_vo_del_ok))
      {
        usage(argv[0]);
      }
    }     
    
  }

  free(ws_hostname);

  if(name)
  {
    free(name);
  }
  if(hostname)
  {
    free(hostname);
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
