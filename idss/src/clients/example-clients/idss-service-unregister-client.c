/** @file idss-service-unregister-client
 * Removes a service, a single instance of a service or all of the 
 * instances of a service.
 * 
 * To run
 *       ./idss-service-unregister-client <options>
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
  printf ("\t-s service name: name of the service you want to unregister\n");
  printf ("\t-n service hostname: FQDN of the server hosting the" \
          " service you want to unregister\n");
  printf ("\t-t service port: port of the service you want to unregister\n");
  printf ("\t-a : unregister all the service instances\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : three cases: \n\t - If you want to unregister all of the instance of a service, please specify the service name and -a option \n\t - If you want to unregister a service instance, please specify the service hostname and port of the service to be unregistered \n\t - If you want to unregister all of the service instances running on a host, please specify the service name, hostname and -a option \n");
}


static void usage_inst (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-n service hostname: FQDN of the server  hosting the" \
	  " service you want to unregister\n");
  printf ("\t-t service port: port of the service you want to unregister\n");
  printf ("\t-h web_service_hostname: FQDN of the server  hosting the" \
	  " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
	  " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : three cases: \n\t - If you want to unregister all of the instances of a service, please specify the service name and -a option \n\t - If you want to unregister a service instance, please specify the service hostname and port of the service to be unregistered \n\t - If you want to unregister all of the service instances running on a host, please specify the service name, hostname and -a option \n");
}


int main (int argc, char **argv)
{
  struct soap                   soap;
  char                          *service;
  char                          *ws_hostname;
  char                          contact_string[256];
  char                          *hostname;
  char                          *msg;
  int                           result;
  int                           i;
  unsigned short int            port;
  int                           verbose;
  int                           allinst;
  unsigned short int            service_port;
  
  /* Setting default values */
  service = NULL;
  msg = NULL;
  ws_hostname = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  allinst = 0;
  hostname = NULL;
  service_port = 0;
  
  while ((i = getopt (argc, argv, "h:n:p:s:t:avu")) > 0)
  {
    switch (i)
    {
      case 'h':
        free (ws_hostname);
        ws_hostname = strdup (optarg);
        break;
      case 'n':
        hostname = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 's':
        service = strdup (optarg);
        break;
      case 't':
        service_port = atoi (optarg);
        break;
      case 'a':
	allinst = 1;
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
  
  
  soap_init1(&soap, SOAP_IO_CHUNK);

  sprintf (contact_string, "http://%s:%d", ws_hostname, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Unregistering service... %s\n", service);
    printf ("Contacting service on... %s\n", contact_string);
  }

  /*
   * soap_call_idss__unregister_service
   *
   * removes a service, a single interface of a service
   * or all of the instances of a service
   *
   * service: the logical name of the service to be removed.
   * hostname: FQDN of the server that provides the service.
   * service_port: port number where the service will be listening on.
   * allinst: set to 0 if a service instance is removed,
   * 1 all the instances of a service.
   * result: set to a value < 0 if an error occurs, 0 on success. If the service
   * does not exist the method returns -1. 
   */
  if (soap_call_idss__unregister_service(&soap, contact_string, "", service, 
           hostname, service_port, allinst, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  { 
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
      if(result == idss_err_name)
      {
        usage(argv[0]);
      }
      else if(result == idss_err_hostname)
      {
        usage_inst(argv[0]);
      }
      else if(result == idss_err_port)
      {
        usage_inst(argv[0]);
      }
      else if(result == idss_err_missed_service)
      {
        usage(argv[0]);
      }
    }
    
  }
  
  if(ws_hostname)
    free (ws_hostname);
  
  if (hostname)
  {
    free (hostname);
  }
  
  if (service)
  {
    free (service);
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
