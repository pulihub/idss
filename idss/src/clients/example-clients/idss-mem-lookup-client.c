/** @file idss-mem-lookup-client
 * seraches for memory info 
 *
 * To run
 *       ./idss-mem-lookup-client <options>
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
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");        
  printf ("\t-m total ram: minimum total ram of the host\n");    
  printf ("\t-n total swap: minimum total swap memory of the host\n");    
  printf ("\t-r free ram: minimum available ram of the host\n"); 
  printf ("\t-s free swap: minimum available swap memory of the host\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap         soap;
  xsd__string         result;
  
  int                 total_ram;
  int 				  total_swap;
  int 		          free_ram;
  int 				  free_swap;
  
  unsigned short int  port;
  char                *web_service;
  int                 j;
  int                 verbose;
  char                contact_string[256];
  
  /* Setting default values */
  
  total_ram = total_swap = free_ram = free_swap = 0;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  while ((j = getopt (argc, argv, "h:p:m:n:r:s:vu")) > 0)
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
        total_ram = atoi (optarg);
        break;
      case 'n':
        total_swap = atoi (optarg);
        break;
      case 'r':
        free_ram = atoi (optarg);
        break; 
      case 's':
        free_swap = atoi (optarg);
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
   * soap_call_idss__lookup_memory
   * 
   * lookups information about cpu status and returns the query result.
   * It contacts the supplied iDSS server on the default port.
   * 
   * result: query result
   */
  if (soap_call_idss__lookup_memory(&soap, contact_string, "", total_ram, total_swap, free_ram, free_swap, &result))
  {
    soap_print_fault (&soap, stderr);
    
  }
  else
  {
    if(result)
      save_dime_data(result, "mem.xml");
  }
  
  if(web_service)
    free(web_service);
    
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
