/** @file idss-system-lookup-client
 * searches for ystem info
 *
 * To run
 *      ./idss-system-lookup-client
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 * 
 */

//#include "config.h"
#include <unistd.h>
#include "idssH.h"
#include "idss_client.h"
#include <stdio.h>
#include <syslog.h>

extern char *optarg;


static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-s system name: name of the system" \
          " you want to lookup\n");
   printf ("\t-r release number: release number of the system" \
          " you want to lookup\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the" \
          " iDSS web service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");

}

int main (int argc, char **argv)
{
  struct soap        soap;
  xsd__string         result;
  char               *name;
  char               *release;
  char               contact_string[256];
  char               *web_service;
  unsigned short int port;
  int                verbose;
  int                c;

/* Setting default values */
  name = NULL;
  release = NULL;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;

  
  while ((c = getopt (argc, argv, "h:p:s:r:vu")) > 0)
  {
    switch (c)
    {
      case 'h':
        free (web_service);
        web_service = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 's':
        name = strdup (optarg);
        break;
      case 'r':
        release = strdup (optarg);
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
  
  soap_init(&soap);
		          
  sprintf (contact_string, "http://%s:%d", web_service, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Contacting the web service listening on... %s\n",
            contact_string);
    if (name)
    {
      printf ("Looking up firewall... %s\n", name);
    }    
    else
    {
      printf ("Looking up any available firewall\n");
    }    
  }

/*
 * soap_call_idss__lookup_system
 * 
 * lookups the information of the registered firewall. 
   It contacts the supplied MDS server on the default port
 * 
 * name: the name of the firewall to be searched
 * result: query result
 */
  if (soap_call_idss__lookup_system(&soap, contact_string, "", name, release, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    if (verbose)
    {
      printf ("Your system has been looked up\n");
    }
    if(result)
     save_dime_data(result, "system.xml");
  }
  
  if(web_service)
     free (web_service);
     
  if (name)
     free(name);
  if (release)  
     free(release);
     
     
  soap_end (&soap);
  soap_done (&soap);
  
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
