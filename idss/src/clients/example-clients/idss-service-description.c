/** @file idss-service-description 
 * Returns description and status of the service
 *
 * To run
 *      ./idss-service-description
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

//#include "config.h"
#include <unistd.h>
#include "idssH.h"
#include <stdio.h>
#include <syslog.h>

extern char *optarg;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap         soap;
  char                *web_service;
  char                *result;
  char                contact_string[256];
  unsigned short int  port;
  int                 verbose;
  int                 c;
  
/* Setting default values */
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  

  while ((c = getopt (argc, argv, "h:p:vu")) > 0)
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
 * soap_call_idss__getServiceDescription
 *  
 * returns description and status of the service
 * 
 * result: the string of description
 */
  if (soap_call_idss__getServiceDescription
          (&soap, contact_string, "", NULL, &result))
  {
    soap_print_fault (&soap, stderr);
    soap_print_fault_location (&soap, stderr);
  }
  else
  {
    if (result)
      printf ("%s\n", result);
  }

  free (web_service);
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);


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
