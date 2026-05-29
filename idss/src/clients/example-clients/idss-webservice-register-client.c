/** @file idss-webservice-register-client
 * Registers a new web service.
 *
 * To run
 *       ./idss-webservice-register-client <options>
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
#include "idss_client.h"

extern char *optarg;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-s <web service name>: set web service name\n");
  printf ("\t-d web service description: set web service description\n");
  printf ("\t-w <web service WSDL location>: set web service WSDL location\n");
  printf ("\t-k web service keywords: set web service keywords\n");
  printf ("\t-r <web service access URL>: set web service access URL\n");
  printf ("\t-n web service validity time: set web service validity time\n");
  printf ("\t-u usage\n");
  printf ("\t-v verbose mode\n");
}

int main (int argc, char **argv)
{
  struct soap                    soap;
  char                           *server;
  char                           *service;
  char                           *url;
  char                           *wsdllocation;
  char                           *description;
  char                           contact_string[256];
  char                           *keywords;
  char                           *msg;
  int                            i;
  int                            verbose;
  unsigned int                   valtime;
  int                            result;
  unsigned short int             port;

  service = NULL;
  url = NULL;
  wsdllocation = NULL;
  description = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  valtime = 0;
  keywords = NULL;


  while ((i = getopt (argc, argv, "h:p:s:d:w:k:r:n:uv")) != EOF)
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
      case 'w':
        wsdllocation = strdup (optarg);
        break;
      case 'k':
        keywords = strdup (optarg);
        break;
      case 'r':
        url = strdup (optarg);
        break;
      case 'n':
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
    printf ("Registering the following web service:\n");
    printf ("web service name:%s\n", service);
    printf ("web service description:%s\n", description);
    printf ("web service wsdl location:%s\n", wsdllocation);
    printf ("web service keywords:%s\n", keywords);
    printf ("web service access URL:%s\n", url);
    printf ("web service validity time:%lu\n", (unsigned long)valtime);
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
    printf ("Web Service validity time... %d\n", valtime);
  }

  /*
   * soap_call_idss__register_webservice
   *
   * allows the user to register a new web service 
   *
   * service: the logical name of the new web service to be registered
   * wsdllocation: a list, space-separated, of the web service WSDL location
   * description: human readable description of the web service
   * keywords: web service key words
   * url: the access URL of the web service instance to be registered
   * valtime: validity time of the information.
   * Default: 0 the information is always valid.
   * result: set to a value < 0 if an error occurs during the registration,
   * 0 on success when registering the first instance of a web service;
   * 1 on success when registering additional instances of the web service.
   * If the new web service being registered already exists,
   * the registration will fail.
   *
   */

 if (soap_call_idss__register_webservice (&soap, contact_string, "",
         service, wsdllocation, keywords, description, url, valtime,
         &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if(msg)
    {
      printf("%s\n", msg);
      if((result != idss_str_ok) && (result != idss_str_instance_ok))
      {
        usage(argv[0]);
      }
    }
    else
    {
      printf ("Error during web service registration\n");
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
  if (wsdllocation)
  {
    free(wsdllocation);
  }
  if (description)
  {
    free(description);
  }
  if (keywords)
  {
    free(keywords);
  }
  if(server)
    free(server);
  
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
