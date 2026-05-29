/** @file idss-wsdlloc-unregister-client
 * Unregisters one or more new wsdl locations url associated to a web service.
 *
 * To run
 *       ./idss-wsdlloc-unregister-client <options>
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
  printf ("\t-s <web service name>: web service name related to the web service WSDL location urls that you want to unregister\n");
  printf ("\t-w <web service WSDL location>: list multi-valued of the web service WSDL location urls" \
          " that you want to unregister\n");
  printf ("\t-v verbose mode");
  printf ("\t-u : If you want to unregister one or more WSDL location URL, please specify the name of the web service related to specified WSDL location URL\n");
}

int main (int argc, char **argv)
{
  struct soap                    soap;
  char                           *server;
  char                           *service;
  char                           *wsdllocation;
  char                           *msg;
  char                           contact_string[256];
  int                            i;
  int                            verbose;
  int                            result;
  unsigned short int             port;

  service = NULL;
  wsdllocation = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  msg = NULL;

  while ((i = getopt (argc, argv, "h:p:s:w:uv")) != EOF)
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
      case 'w':
        wsdllocation = strdup (optarg);
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
    printf ("Unregistering the following wsdl location URL:\n");
    printf ("web service wsdl location:%s\n", wsdllocation);
  }


  soap_init1(&soap, SOAP_IO_CHUNK);

  sprintf (contact_string, "http://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to web service on... %s\n", contact_string);
    printf ("Unregistering a new wsdl location URL of the Web " \
            "Service... %s", service);
  }

  /*
   * soap_call_idss__unregister_wsdlloc
   *
   * allows the user to unregister a wsdl location URL of a web service
   *
   * service: the logical name of the web service related to the wsdlloc list
   * to be unregistered
   * wsdllocation: a list, space-separated, of the web service WSDL location 
   * URL variables
   * result: set to a value < 0 if an error occurs during the unregister.
   *
   */

 if (soap_call_idss__unregister_wsdlloc (&soap, contact_string, "",
         service, wsdllocation, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if(msg)
    {
      printf("%s\n", msg);
      if(result != idss_str_del_wsdl_ok)
      {
        usage(argv[0]);
      }
      exit (1);
    }	
    else
    {
      printf ("Error during web service unregistration\n");
    }
   
  }
  if (server)
  {
    free(server);
  }
  if (service)
  {
    free(service);
  }
  if (wsdllocation)
  {
    free(wsdllocation);
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
