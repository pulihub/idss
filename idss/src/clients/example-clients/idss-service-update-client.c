/** @file idss-service-update-client
 * Updates a service.
 *
 * To run
 *       ./idss-service-update-client <options>
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

#include <unistd.h>
#include "idssStub.h"
#include <stdio.h>
#include <syslog.h>
#include "idss_client.h"

extern char *optarg;


static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-r <service access URL>: service access URL of" \
          " the service that you want to update\n");
  printf ("\t-s <service name>: service name of the service to be modified\n");
  printf ("\t-d service description: updated service description\n");
  printf ("\t-k service keywords: updated service keywords\n");
  printf ("\t-f service default port: updated service default port\n");
  printf ("\t-l service validity time: updated service validity time\n");
  printf ("\t-u usage: two cases\n\t - if you want to update the service description, keywords and default port, please specify the name of the service that you want to update\n\t - if you want to update the service instance validity time, please specify the service instance access URL\n");
  printf ("\t-v verbose mode\n");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  char                          *server;
  char                          *service;
  char                          *url;
  char                          *description;
  char                          contact_string[256];
  char                          *keywords;
  char                          *msg;
  int                           i;
  int                           verbose;
  int                           result;
  unsigned short int            port;
  int                           dport;
  unsigned int                  valtime;
  
  service = NULL;
  url = NULL;
  description = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  valtime = 0;
  keywords = NULL;
  msg = NULL;
  dport = -1;
  
  
  while ((i = getopt (argc, argv, "h:p:s:d:k:r:f:l:uv")) != EOF)
  {
    switch (i)
    {
      case 'h':
        server = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 's':
        service = strdup (optarg);
        break;
      case 'd':
        description = strdup (optarg);
        break;
      case 'k':
        keywords = strdup (optarg);
        break;
      case 'r':
        url = strdup (optarg);
        break;
      case 'f':
        dport = atoi (optarg);
        break;
      case 'l':
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
    printf ("Updating the following web service:\n");
    printf ("service name:%s\n", service);
    printf ("service description:%s\n", description);
    printf ("service keywords:%s\n", keywords);
    printf ("service access URL:%s\n", url);
    printf ("service default port:%d\n", dport);
    printf ("service validity time:%lu\n", (unsigned long)valtime);
  }
  
  
  soap_init1(&soap, SOAP_IO_CHUNK);
 
  
  sprintf (contact_string, "http://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to web service on... %s\n", contact_string);
    printf ("Registering Web Service... %s", service);
    printf ("Service Description... %s\n", description);
    printf ("Service Keywords... %s\n", keywords);
    printf ("Service access URL... %s\n", url);
    printf ("Service default port... %d\n", dport);
    printf ("service validity time:%d\n", valtime);
  }

  /*
   * soap_call_idss__update_service
   *  
   * updates name, description, keywords, default port and validity time
   * of a service
   * 
   * service: the logical name of the service to be updated
   * keywords: the key words of the service to be updated 
   * description: the description of the service to be updated
   * url: the access URL of the service instance to be registered
   * dport: the default port where the service to be updated is listening on
   * valtime: validity time of the service instance information
   * to be updated
   * 
   * result: set to a value < 0 if an error occurs during the registration,
   * 0 on success when updating the web service;  
   */

  if (soap_call_idss__update_service (&soap, contact_string, "", 
          service, keywords, description, url, dport, valtime, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
      if(result != idss_str_up_ok)
      {
        usage(argv[0]);
      }
    }
    else
    {
      printf ("Error during service update\n");
    }
    
  }
  if (service)
  {
    free(service);
  }
  if (url)
  {
    free(url);
  }
  if (description)
  {
    free(description);
  }
  if (keywords)
  {
    free(keywords); 
  }
 
  
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
