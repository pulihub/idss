/** @file idss-service-register-client
 * Registers a new service or an instance of an existing service.
 *
 * To run
 *       ./idss-service-register-client <options>
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
          " the IGrid web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-s <service name>: set service name\n");
  printf ("\t-n service hostname: set FQDN of the server hosting" \
          " the service\n");
  printf ("\t-r <service protocol>: set service protocol\n");
  printf ("\t-t <service port>: set service port\n");
  printf ("\t-f <service default port>: set service default port\n");
  printf ("\t-d service description: set service description\n");
  printf ("\t-k service keywords: set service keywords\n");
  printf ("\t-l service validity time: set service validity time\n");
  printf ("\t-u usage\n");
  printf ("\t-v verbose mode\n");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  int                           result;
  char                          *server;
  char                          *service;
  char                          *protocol;
  char                          *description;
  char                          contact_string[256];
  char                          *keywords;
  char                          *hostname;
  char                          *msg;
  int                           i;
  int                           verbose;
  int                           dport;
  unsigned short int            port;
  unsigned short int            serviceport;
  unsigned int                  valtime;
  
  service = NULL;
  serviceport = 0;
  protocol = NULL;
  description = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  dport = -1;
  keywords = NULL;
  hostname = NULL;
  valtime = 0;
 
  while ((i = getopt (argc, argv, "h:p:s:n:t:r:f:d:k:l:uv")) != EOF)
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
      case 'n':
        hostname = strdup (optarg);
        break;
      case 't':
        serviceport = atoi (optarg);
        break;
      case 'r':
        protocol = strdup (optarg);
        break;
      case 'f':
        dport = atoi (optarg);
        break;
      case 'd':
        description = strdup (optarg);
        break;
      case 'k':
        keywords = strdup (optarg);
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
    printf ("Registering the following service:\n");
    printf ("service name:%s\n", service);
    printf ("service hostname:%s\n", hostname);
    printf ("service port:%d\n", serviceport);
    printf ("service protocol:%s\n", protocol);
    printf ("service default port:%d\n", dport);
    printf ("service description:%s\n", description);
    printf ("service keywords:%s\n", keywords);
    printf ("service validity time:%lu\n", (unsigned long)valtime);
  }
  
  
  soap_init1(&soap, SOAP_IO_CHUNK);
  
  sprintf (contact_string, "http://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to web service on... %s\n", contact_string);
    printf ("Registering Service... %s://server:%d/%s\n", protocol,
           serviceport, service);
    printf ("service default port:%d\n", dport);
    printf ("Service Description... %s\n", description);
    printf ("service keywords:%s\n", keywords);
    printf ("service validity time:%d\n", valtime);
  }

  /*
   * soap_call_idss__register_service
   *  
   * allows the user to register a new service or an instance
   * of an existing service 
   * 
   * service: it is the logical name of the service to be registered
   * hostname: it is the FQDN of the server that provides the service           
   * serviceport: port number where the service will be listening on
   * protocol: communication protocol to be used to contact the service
   * dport: default port number where the service will be listening on
   * description: human readable description of the service
   * keywords: set of key words of a service
   * valtime: validity_time of the service instance information
   * result: set to a value < 0 if an error occurs during the registration, 0 on
   * success for the service first instance registration; set to 1 for successful
   * registration of additional instances of the service.
   * If the service being registered already exists, 
   * the registration will fail.
   */
  if (soap_call_idss__register_service (&soap, contact_string, "", service, 
          hostname, serviceport, protocol, dport, description, keywords, 
          valtime, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
      if((result != idss_str_ok) && (result != idss_str_instance_ok))
      {
        usage(argv[0]);
      }
    }
    else
    {
      printf ("Error during service registration\n");
    }
    
  }
  
  if(server)
    free (server);
  if (service)
  {
    free (service);
  }
  if (protocol)
  {
    free (protocol);
  }
  if (description)
  {
    free (description);
  }
  if (keywords)
  {
    free (keywords);  
  }
  if (hostname)
  {
    free (hostname);  
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
