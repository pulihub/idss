/** @file idss-search-client 
 * Query an iStore server 
 *
 * To run
 *      ./idss-search-client
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
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the" \
          " iDSS web service is listening on\n");
  printf ("\t-f host: returns all of the information related only to the specified host name\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap          soap;
  xsd__string       result;
  
  char                 contact_string[256];
  char                 *ws_hostname;
  unsigned short int   ws_port;
  char 		       *host;
  int                  verbose;
  int                  c;
  
/* Setting default values */
  ws_hostname = strdup (IDSS_SERVICE_HOSTNAME);
  ws_port = IDSS_SERVICE_LISTENING_PORT;
  host = NULL;
  
  verbose = 0;

  
  while ((c = getopt (argc, argv, "h:p:f:vu")) > 0)
  {
    switch (c)
    {
      case 'h':
        free (ws_hostname);
        ws_hostname = strdup (optarg);
        break;  
      case 'p':
        ws_port = atoi (optarg);
        break;
      case 'f':
        host = strdup (optarg);  
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
 
  sprintf (contact_string, "http://%s:%d", ws_hostname, ws_port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to service on... %s\n", contact_string);
    
    {	    
      printf ("Display the value of all of the attributes\n");
    }
  }

/*
 * soap_call_idss__search
 * 
 * query an arbitrary iStore server
 * 
 * host: select only the informations about the host specified
 * result: query result
 */
 
  
  if (soap_call_idss__search(&soap, contact_string, "", host, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  
  if (result)
  {
   save_dime_data(result, "search.xml");
  }  
  
  
  if(host)
    free (host);
  if(ws_hostname)  
    free (ws_hostname);
  
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
