/** @file idss-firewall-unregister-client
 * Removes an existing firewall.
 *
 * To run
 *      ./idss-firewall-unregister-client
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
  printf ("\t-f <firewall hostname>: hostname of the firewall" \
          " you want to unregister\n");
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS" \
          " web service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage\n");
}

int main (int argc, char **argv)
{
  struct soap                   soap;
  char                          *ws_hostname = NULL;
  char                          *name = NULL;
  char                          *msg = NULL;
  char                          contact_string[256];
  int                           result;
  int                           c;
  unsigned short int            port;
  int                           verbose;

/* Setting default values */
  name = NULL;
  ws_hostname = strdup(IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  verbose = 0;
  msg = NULL;

  while ((c = getopt (argc, argv, "h:p:f:vu")) > 0)
  {
    switch (c)
    {
      case 'h':
        free (ws_hostname);
        ws_hostname = strdup (optarg);
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
  if (!name)
  {
    printf ("You must provide the firewall's name you want to unregister\n");
    free (ws_hostname);
    usage (argv[0]);
    return 1;
  }

  soap_init1(&soap, SOAP_IO_CHUNK);

  sprintf (contact_string, "http://%s:%d", ws_hostname, port);
  contact_string[strlen(contact_string)] = '\0';


  if (verbose)
  {
    printf ("Unregistering firewall... %s\n", name);
    printf ("Contacting service on... %s\n", contact_string);
  }

/*
 * soap_call_idss__unregister_firewall
 *
 * removes information about the registered firewall
 *
 * name: host name of the machine where the firewall is installed;
 * the corresponding information will be removed
 * result: set to -1 if an error occurs, 0 on success. If the firewall
 * information does not exist the method sets result to -1.
 */
  if (soap_call_idss__unregister_firewall
          (&soap, contact_string, "", name, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
      if (result != idss_str_fir_del_ok)
      {
        usage(argv[0]);
      }
    }
  }

  soap_end (&soap);
  soap_done (&soap);

  if(ws_hostname)
    free(ws_hostname);
  if(name)
  {
    free(name);
  }

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
