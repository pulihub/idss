/** @file idss-webservice-update-client
 * Updates a web service.
 *
 * To run
 *       ./idss-webservice-update-client <options>
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
  printf ("\t-r <web service access URL>: set web service access URL of" \
          " the web service that you want to update\n");
  printf ("\t-s <web service name>: set updated web service name\n");
  printf ("\t-d web service description: set updated web service" \
          " description\n");
  printf ("\t-k web service keywords: set updated web service keywords\n");
  printf ("\t-l web service validity time: set updated web service " \
          "validity time\n");
  printf ("\t-u usage: two cases:\n\t - if you want to update the web service description, and keywords, please specify the name of the web service that you want to update\n\t - if you want to update the web service validity time, please specify the web service access URL of the web service instance.\n");
  printf ("\t-v verbose mode");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  char                          *server;
  char                          *service;
  char                          *url;
  char                          *msg;
  char                          *wsdllocation;
  char                          *description;
  char                          contact_string[256];
  char                          *keywords;
  int                           i;
  int                           verbose;
  int                           result;
  unsigned int                  valtime;
  unsigned short int            port;
  
  service = NULL;
  url = NULL;
  wsdllocation = NULL;
  description = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  valtime = 0;
  keywords = NULL;
  msg = NULL;
  
  while ((i = getopt (argc, argv, "h:p:s:d:k:r:l:uv")) != EOF)
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
    printf ("web service name:%s\n", service);
    printf ("web service description:%s\n", description);
    printf ("web service keywords:%s\n", keywords);
    printf ("web service access URL:%s\n", url);
  }
  
  
  soap_init1(&soap, SOAP_IO_CHUNK);
  
  sprintf (contact_string, "http://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to web service on... %s\n", contact_string);
    printf ("Registering Web Service... %s", service);
    printf ("Web Service Description... %s\n", description);
    printf ("Web Service Keywords... %s\n", keywords);
    printf ("Web Service access URL... %s\n", url);
  }

  /*
   * soap_call_idss__update_webservice
   *  
   * allows the user to register a new web service
   * 
   * service: the logical name of the new web service to be updated
   * keywords: the key words of the web service to be updated
   * description: the description of the web service to be updated
   * url: the access url of the web service instance to be updated 
   * valtime: validity time of the web service instance information 
   * to be updated
   * result: set to a value < 0 if an error occurs during the registration,
   * 0 on success when updating the web service;  
   */

  if (soap_call_idss__update_webservice (&soap, contact_string, "", 
          service, keywords, description, url, valtime, &result))
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
      printf ("Error during web service update\n");
    }
    
  }
  if(server)
  {
    free(server);
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
