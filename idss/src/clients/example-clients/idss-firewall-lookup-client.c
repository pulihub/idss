/** @file idss-firewall-lookup-client
 * searches for an existing firewall.
 *
 * To run
 *      ./idss-firewall-lookup-client
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
#include <string.h>
#include <syslog.h>
 
extern char *optarg;


static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-f firewall hostname: hostname of the firewall" \
          " you want to lookup\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the" \
          " iDSS web service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");

}

int main (int argc, char **argv)
{
  struct soap        soap;
  xsd__string         result;
  char               *name;
  char               contact_string[256];
  char               *web_service;
  unsigned short int port;
  int                i;
  int                j;
  int                verbose;
  int                c;
  idss_firewall      *fir_data;

/* Setting default values */
  name = NULL;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  while ((c = getopt (argc, argv, "h:p:f:vu")) > 0)
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
      case 'f':
        name = strdup (optarg);
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
    if (name)
    {
      printf ("Looking up firewall... %s\n", name);
    }    
    else
    {
      printf ("Looking up any available firewall\n");
    }    
  }

/*
 * soap_call_idss__lookup_firewall
 * 
 * lookups the information of the registered firewall. 
   It contacts the supplied MDS server on the default port
 * 
 * name: the name of the firewall to be searched
 * result: query result
 */
  if (soap_call_idss__lookup_firewall(&soap, contact_string, "", name, &result))
  {
    soap_print_fault (&soap, stderr);
    if((!strcmp(soap.fault->faultstring,
        "You must provide the firewall hostname")))
    {
      usage (argv[0]);
    }
  }
  else
  {
    if (verbose)
    {
      printf ("Your firewall has been looked up\n");
    }
    if(result)
    {
      save_dime_data(result, "firewall.xml");
      fir_data = idss_get_fir_data(result);
      if(fir_data)
      {
        for (i = 0; fir_data[i]; i++)
        {
          for (j = 0; fir_data[i]->hostname[j]; j++)
          {
            printf("hostname = %s\n", fir_data[i]->hostname[j]);
          }
          printf("\n");
          printf("firewall name = %s\n", fir_data[i]->name); 
          printf("firewall admin  = %s\n", fir_data[i]->admin);
          printf("firewall publisher   = %s\n", fir_data[i]->publisher);
          printf("firewall validity time   = %u\n", fir_data[i]->vt);
          printf("firewall creation date   = %lu\n", fir_data[i]->cd);
          for (j = 0; fir_data[i]->ports[j]; j++)
          {
            printf("firewall ports #%d\n", j + 1);
            printf("\tprotocol = %s\n", fir_data[i]->ports[j]->protocol);
            printf("\tcreated   = %lu\n", fir_data[i]->ports[j]->cd);
            /* Commented lines due to functionality not supported by the server */
			//printf("\tvalid     = %u\n", fir_data[i]->ports[j]->vt);
            //printf("\tmin   = %d\n", fir_data[i]->ports[j]->port_range.min);
            printf("\tmax     = %d\n", fir_data[i]->ports[j]->port_range.max);
          }
          printf("\n");
        }
		idss_delete_fir_data(fir_data);
      } 
    }
    else
	{
	    if (verbose)
    {
      printf ("No firewall has been found\n");
    }

	} 
   
     
  }
   if(web_service)
     free (web_service);
     
    if (name)
     free(name);
     
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
