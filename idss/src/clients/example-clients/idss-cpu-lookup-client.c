/** @file idss-cpu-lookup-client
 * searches for cpu info 
 *
 * To run
 *       ./idss-cpu-lookup-client <options>
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
#include <string.h>
#include <syslog.h>


extern char *optarg;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-c cpu cache: minimum cache (MBytes) of the cpu you " \
          "want to lookup\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-m cpu mhz: minimum frequency value of the cpu\n");    
  printf ("\t-n cpu number: minimum number of cpus on the resource\n");    
  printf ("\t-l cpu load: minimum cpu load\n");    
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap         soap;
  xsd__string         result;
  
  int                 mhz;
  int 				  cache;
  int 		          number;
  char                *load = NULL;
  
  unsigned short int  port;
  char                *web_service;
  int                 j;
  int                 verbose;
  char                contact_string[256];

  
  /* Setting default values */
  
  mhz = cache = number = 0;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  while ((j = getopt (argc, argv, "h:p:m:c:n:l:vu")) > 0)
  {
    switch (j)
    {
      case 'h':
        free (web_service);
        web_service = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 'm':
        mhz = atoi (optarg);
        break;
      case 'c':
        cache = atoi (optarg);
        break;
      case 'n':
        number = atoi (optarg);
        break; 
      case 'l':
        load = strdup (optarg);
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
    
  }

  /*
   * soap_call_idss__lookup_cpu
   * 
   * lookups information about cpu status and returns the query result.
   * It contacts the supplied iDSS server on the default port.
   * 
   * result: query result
   */
  if (soap_call_idss__lookup_cpu(&soap, contact_string, "", mhz, cache, number, load, &result))
  {
    soap_print_fault (&soap, stderr);
    
  }
  else
  {
    if(result)
      save_dime_data(result, "cpu.xml");
    
  }
 
  if(load)
    free(load);
  if(web_service)
    free(web_service);
    
  soap_end (&soap);
  soap_done (&soap);
  
  return 0;
}

struct Namespace namespaces[] = {
  {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/"},
  {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/"},
  {"xsi", "http://www.w3.org/1999/XMLSchema-instance"},
  {"xsd", "http://www.w3.org/1999/XMLSchema"},
  {"idss", "urn:idss"},
  {NULL, NULL}
};
