/** @file idss-lrms-lookup-client 
 * Searches for local resource management system of a specified GRIS server.
 *
 * To run
 *      ./idss-lrms-lookup-client
 *
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

#include <unistd.h>
#include "idssH.h"
#include "idss_client.h"
#include <stdio.h>
#include <syslog.h>

extern char *optarg;


static void usage ()
{
  printf ("Usage:\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS" \
          " web service is listening on\n");
  printf ("\t-n hostname: name of the host where the lrms is running on\n");
  printf ("\t-s type: type of the lrms to be searched\n");
  printf ("\t-q queue: name of the queue to be searched\n");
  printf ("\t-j job: name of the job to be searched\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap        soap;
  xsd__string         result;
  char               *name;
  char               *host;
  char               *queue;
  char               *job;
  char               *web_service;
  char               contact_string[256];
  unsigned short int port;
  int                verbose;
  int                c;

/* Setting default values */
  name = NULL;
  host = NULL;
  queue = NULL;
  job = NULL;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  
  while ((c = getopt (argc, argv, "h:p:s:n:q:j:vu")) > 0)
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
      case 's':
        free (name);
        name = strdup (optarg);
        break;
      case 'n':
        free (host);
        host = strdup (optarg);
        break;
      case 'q':
        free (queue);
        queue = strdup (optarg);
        break;
      case 'j':
        free (job);
        job = strdup (optarg);
        break;
      case 'v':
        verbose = 1;
        break;
      case 'u':
      default:
        usage ();
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
 * soap_call_idss__lrms_search
 * 
 * searches the information about lrms of a specified GRIS server.
 * It contacts the supplied MDS server on the default port
 * 
 * name: the name of the lrms to be searched
 * result: query result
 */
  if (soap_call_idss__lookup_lrms
          (&soap, contact_string, "", host, name, queue, job, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    if (verbose)
    {
      printf ("lrms information has been searched\n");
    }
    if(result)
      save_dime_data(result, "lrms.xml");
    
  }
  
    if(web_service)
     free (web_service);
    if(name)
     free(name);
    if(host)
     free(host);
    if(queue)
     free(queue);
    if(job)
     free(job);
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
