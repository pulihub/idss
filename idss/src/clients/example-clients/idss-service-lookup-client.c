/** @file idss-service-lookup-client
 * searches for an existing service.
 *
 * To run
 *       ./idss-service-lookup-client <options>
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 * 
 */

//#include "config.h"
#include <unistd.h>
#include "idssH.h"
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "idss_client.h"


extern char *optarg;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-s service name: name of the service you want to lookup\n");
  printf ("\t-n hostname: name of the host that provides the service you want to lookup\n");
  printf ("\t-f service default port: default port of the service you " \
          "want to lookup\n");
  printf ("\t-t service port: service port of the service you want to lookup\n");
  printf ("\t-r service protocol: service protocol\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap         soap;
  xsd__string         result;
  char                *service;
  char                *hostname;
  int                 sport;
  int                 dport;
  char                *prot;
  unsigned short int  port;
  char                *web_service;
  int                 i, j;
  int                 verbose;
  char                contact_string[256];
  idss_ws_data*  ser_data;
  
  /* Setting default values */
  service = NULL;
  hostname = NULL;
  sport = -1;
  dport = -1;
  prot = NULL;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  while ((j = getopt (argc, argv, "h:p:s:n:f:t:r:vu")) > 0)
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
      case 's':
        service = strdup (optarg);
        break;
      case 'n':
        hostname = strdup (optarg);
        break;
      case 'f':
        dport = atoi (optarg);
        break;
      case 't':
        sport = atoi (optarg);
        break;
      case 'r':
        prot = strdup (optarg);
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

 /* Timeout after 5 minutes stall on recv */
//	gsi_set_recv_timeout(&soap, 20);
//	gsi_set_send_timeout(&soap, 60); 

  sprintf (contact_string, "http://%s:%d", web_service, port);
  contact_string[strlen(contact_string)] = '\0'; 
   
  if (verbose)
  {
    printf ("Contacting the web service listening on... %s\n",
            contact_string);
    if (service)
    {
      printf ("Looking up service... %s\n", service);
    }
    else
    {
      printf ("Looking up any service available\n");
    }
  }

  /*
   * soap_call_idss__lookup_service
   * 
   * lookups information about services and returns the query result.
   * It contacts the supplied iDSS server on the default port.
   * 
   * service: the logical name of the service to be searched
   * dport: the default port of the service to be searched
   * sport: the port number where the service to be searched is listening on
   * prot: the protocol of the service to be searched
   * result: query result
   */
  if (soap_call_idss__lookup_service(&soap, contact_string, "",
          service, hostname, dport, sport, prot, &result))
  {
    soap_print_fault (&soap, stderr);
    if((!strcmp(soap.fault->faultstring,
        "You must provide the service name")) ||
	(!strcmp(soap.fault->faultstring,
        "You must provide the service port")) ||
        (!strcmp(soap.fault->faultstring,
        "You must provide the service protocol")) ||
        (!strcmp(soap.fault->faultstring,
        "You must provide the service default port")))
    {
      usage (argv[0]);
    }

  }
  else
  {
    if (verbose)
    {
      printf ("Your service has been looked up\n");
    }
    if(result)
    {
	 save_dime_data(result, "service.xml");

	 ser_data = idss_get_ser_data(result);
	
	   if(ser_data)
	   {
		for (i = 0; ser_data[i]; i++)
		{
		  printf("service name = %s\n",ser_data[i]->name);
		  printf("description  = %s\n",ser_data[i]->description);
		  printf("keywords     = %s\n",ser_data[i]->keywords);
		  printf("default port = %d\n",ser_data[i]->def_port);
		 
		  for (j = 0; ser_data[i]->acurl[j]; j++)
		  {
			printf("access URLs #%d\n", j+1);
			printf("\thostname  = %s\n", ser_data[i]->acurl[j]->hostname);
			printf("\turl       = %s\n", ser_data[i]->acurl[j]->url);
			printf("\tpublisher = %s\n", ser_data[i]->acurl[j]->publisher);
			printf("\tcreated   = %lu\n", ser_data[i]->acurl[j]->cd);
			printf("\tvalid     = %u\n", ser_data[i]->acurl[j]->vt);
		  }
		  printf("\n");
		 }
		}
    }       
    
  }
  
  if(web_service)
     free (web_service);
    
    if(service)
     free (service);
     
    if(hostname)
     free(hostname);
        
    if(prot)
     free(prot);
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
