/** @file idss-webservice-unregister-client
 * 
 * Removes an existing web service, a single instance of a web service or all
 * of the instances of a web service.
 * 
 * To run
 *       ./idss-webservice-unregister-client <options>
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
  printf ("\t-s web service name: name of the web service you want" \
          " to unregister\n");
  printf ("\t-r web service access URL: access URL of the web service" \
          " instance you want to unregister\n");
  printf ("\t-n web service hostname: FQDN where the web service that" \
          " you want to unregister is running\n");
  printf ("\t-a : unregister all the web service instances\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : two cases \n\t - If you want to unregister all of the instances of a web service, please specify the web service name and -a option \n\t - If you want to unregister a web service instance, please specify the web service accessurl of the service to be unregistered\n ");
}

static void usage_inst (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-r <web service access URL>: access URL of the web service" \
          " instance you want to unregister\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : three cases: \n\t - If you want to unregister all of the instances of a web service, please specify the web service name and -a option \n\t - If you want to unregister a web service instance, please specify the web service accessurl of the service to be unregistered\n\t - If you want to unregister all of the web service instances running on a host, please specify the web service name, -r option where you should specify the hostname and -a option \n ");
}

int main (int argc, char **argv)
{
  struct soap                    soap;
  char                           *service;
  char                           *ws_hostname;
  char                           contact_string[256];
  char                           *url;
  char                           *msg;
  unsigned short int             port;
  int                            verbose;
  int                            allinst;
  int                            result;
  int                            i;

  /* Setting default values */
  service = NULL;
  ws_hostname = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  url = NULL;
  msg = NULL;
  allinst = 0;


  while ((i = getopt (argc, argv, "h:p:s:r:n:avu")) > 0)
  {
    switch (i)
    {
      case 'h':
        free (ws_hostname);
        ws_hostname = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 's':
        service = strdup (optarg);
        break;
      case 'r':
        url = strdup (optarg);
        break;
      case 'n':
        url = strdup (optarg);
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
    printf ("Unregistering web service... %s\n", service);
    printf ("Contacting service on... %s\n", contact_string);
  }

  /*
   * soap_call_idss__unregister_webservice
   *
   * removes an existing web service, a single interface of a web service
   * or all of the instances of a service
   *
   * service: the logical name of the web service to be removed.
   * url: the access URL of the web service to be removed.
   * allinst: set to 1 to remove all of the instances of the specified
   * service, set to 0 to remove a single
   * result: set to a value < 0 if an error occurs, 0 on success.
   * If the web service does not exist the method returns -1.
   */
  if (soap_call_idss__unregister_webservice(&soap, contact_string, "",
          service, url, allinst, &result))
  {
    soap_print_fault (&soap, stderr);
    if((!strcmp(soap.fault->faultstring,
      "You must provide the web service name")))
    {
      usage(argv[0]);
    }
    if (allinst == 0)
    {
      if((!strcmp(soap.fault->faultstring,
         "You must provide the web service access URL")))
      {
        usage_inst(argv[0]);
      }
    }
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
      else if(result == idss_err_accessurl)
      {
        usage_inst(argv[0]);
      }
      else if(result == idss_err_missed_service)
      {
        usage(argv[0]);
      }
    }      
    /*if (result == -1)
    {
      printf ("Query uncorrectly executed\n");
    }
    else if (result == -2)
    {
      printf ("The FQDN specified has not correct! You must specify a valid" \
              " FQDN of the server that provides the web service\n");
      
    }
    else if (result == -3)
    {
      printf ("The web service access URL specified is not correct. Please" \
              " specify the web service access URL in the following format:" \
              " <prot>://<hostname>:<port>/<path>\n");
    }
    else if (result == -4)
    {
      printf ("Error during the translation from MQ file to SQL string\n");
    }
    else if(result == -5)
    {
      printf("The Web Service you specified is not registered\n");
    }
    else
    {
      printf ("Your Web Service has been unregistered\n");
    }*/
    
  }

  if (service)
  {
    free(service);
  }
  if (url)
  {
    free (url);
  }
  if(ws_hostname)
  {
    free (ws_hostname);
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
