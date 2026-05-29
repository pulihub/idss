/** @file idss-firewall-register-client
 * Registers a firewall.
 *
 * To run
 *       ./idss-firewall-register-client <options>
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

#define MAX_BUF 1024
extern char *optarg;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t-u usage\n");
}

int main (int argc, char **argv)
{
  struct soap                    soap;
  int                            result;
  char                           *server;
  unsigned int                   valtime;
  int                            type_por = 0;

  int num_range = 0;
  char                           contact_string[256];
  char                           *msg;
  unsigned short int             port;
  int                            i, j;
 
  struct strcmn info_gen;
  struct arr_port_info range;
  int *num_port_range;
  int *num_prot_range;
  
  struct port_info *info_port=NULL;

  /* Setting default values */
  msg = NULL;
  server = strdup (IDSS_SERVICE_HOSTNAME);
  port = IDSS_SERVICE_LISTENING_PORT;
  valtime = 0;
  type_por = -1;
  
  /* allocation for general firewall info */

  while ((i = getopt (argc, argv, "h:p:u")) != EOF)
  {
    switch (i)
    {
      case 'h':
        server = strdup (optarg);
        break;
      case 'p':
        port = atoi (optarg);
        break;
      case 'u':
      default:
        usage (argv[0]);
        exit (1);
    }
  }
  printf ("Enter firewall hostname: \n");
  fgets(argv[0], MAX_BUF, stdin);
  argv[0][strlen(argv[0])-1]='\0';
  info_gen.hostname = strdup (argv[0]);
        
  printf ("Enter firewall administrator DN. Default NULL. Please digit c if you don't want specify any value: \n");
  fgets(argv[0], MAX_BUF, stdin);
  argv[0][strlen(argv[0])-1]='\0';
  
  if(strcmp(argv[0], "c") == 0)
  {
    info_gen.admindn  = NULL;
  }	  
  else
  {
    info_gen.admindn  = strdup (argv[0]);
  }	  
	
  printf ("Enter host name of the machine registered at the firewall. Default: host name of the server hosting the iDSS web service. Please digit c if you don't want specify any value: \n");
	  
  fgets(argv[0], MAX_BUF, stdin);
  argv[0][strlen(argv[0])-1]='\0';
  if(strcmp(argv[0], "c") == 0)
  {
    info_gen.host  = NULL;
  }	  
  else
  {
    info_gen.host  = strdup (argv[0]);
  }	  

  printf ("Enter firewall validity time. Default 0. Please digit c if you don't want specify any value: \n");
  fgets(argv[0], MAX_BUF, stdin);
  argv[0][strlen(argv[0])-1]='\0';
  if(strcmp(argv[0], "c") == 0)
  {
    info_gen.valtime=0;
  }	  
  else
  {
    info_gen.valtime=atoi(argv[0]);
  }	  
          	
  printf ("Enter number of open ports range: \n");
  fgets(argv[0], MAX_BUF, stdin);
  argv[0][strlen(argv[0])-1]='\0';
  num_range = atoi(argv[0]);
  
  /* allocation for ports firewall info */
  range.__size = num_range;
  
  info_port = (struct port_info *) calloc (num_range, sizeof(struct port_info));

  num_port_range = (int *) calloc (num_range, sizeof(int));
  num_prot_range = (int *) calloc (num_range, sizeof(int));

  for(i=0; i<num_range; i++)
  {
    printf ("Enter number of ports for open ports range #%d: \n", i+1);	
    fgets(argv[0], MAX_BUF, stdin);
    argv[0][strlen(argv[0])-1]='\0';
    num_port_range[i] = atoi(argv[0]);
    if(num_port_range[i] <= 0)
    {
      printf("Number of open ports for range #%d is not valid! Please specify a valid value\n", i+1);
      fgets(argv[0], MAX_BUF, stdin);
      argv[0][strlen(argv[0])-1]='\0';
      num_port_range[i] = atoi(argv[0]);
    }
    info_port[i].ports.__size = num_port_range[i];
    info_port[i].ports.__ptr = (char **) calloc (num_port_range[i], sizeof(char *));
    for(j=0; j<num_port_range[i]; j++)
    {
      printf ("Enter the port #%d for open ports range #%d in the following format <port-port> or <port>: \n", j + 1, i + 1);
      fgets(argv[0], MAX_BUF, stdin);
      argv[0][strlen(argv[0])-1]='\0';
      info_port[i].ports.__ptr[j] = strdup(argv[0]);
    }
    
    printf ("Enter number of protocol for open ports range #%d: \n", i+1);	
    fgets(argv[0], MAX_BUF, stdin);
    argv[0][strlen(argv[0])-1]='\0';
    num_prot_range[i] = atoi(argv[0]);
    
    if(num_prot_range[i] <= 0)
    {
      printf("Number of protocol for range #%d is not valid! Please specify a valid value\n", i+1);
      fgets(argv[0], MAX_BUF, stdin);
      argv[0][strlen(argv[0])-1]='\0';
      num_prot_range[i] = atoi(argv[0]);
    }
    
    info_port[i].prot.__size = num_prot_range[i];
    info_port[i].prot.__ptr = (char **) calloc (num_prot_range[i], sizeof(char *));
    for(j=0; j<num_prot_range[i]; j++)
    {
      printf ("Enter the protocol #%d for open ports range #%d: \n", j+1, i+1);
      fgets(argv[0], MAX_BUF, stdin);
      argv[0][strlen(argv[0])-1]='\0';
      info_port[i].prot.__ptr[j] = strdup(argv[0]);
    }
	  
    printf ("Enter creation date in the YYYYMMDD format for range #%d:\n", i+1);
    fgets(argv[0], MAX_BUF, stdin);
    argv[0][strlen(argv[0])-1]='\0';
    info_port[i].since=atoi(argv[0]);
	  
    printf ("Enter validity time in sec for range #%d. Default 0. Please digit c if you don't want specify any value: \n", i+1);
    fgets(argv[0], MAX_BUF, stdin);
    argv[0][strlen(argv[0])-1]='\0';
    if(strcmp(argv[0], "c") == 0)
    {
      info_port[i].upto=0;
    }  
    else
    {
      info_port[i].upto=atoi(argv[0]);
    }  
    printf ("Enter open ports range type for range #%d. Please digit 0 if open ports range is open, 1 otherwise. Default 0. Please digit c if you don't want specify any value: \n", i+1);
    fgets(argv[0], MAX_BUF, stdin);
    argv[0][strlen(argv[0])-1]='\0';
    if(strcmp(argv[0], "c") == 0)
    {
      info_port[i].type = 0;
    }  
    else
    {
      info_port[i].type=atoi(argv[0]);
    }  
  }

  range.__ptr = info_port;


  
  soap_init1(&soap, SOAP_IO_CHUNK);

  sprintf (contact_string, "http://%s:%d", server, port);
  contact_string[strlen(contact_string)] = '\0';

  
  /*
   * soap_call_idss__register_firewall
   *
   * allows the user to register the Firewall information
   *
   * info: struct storing general firewall info
   * pinfo: struct storing ports and protocols info
   * valtime: the firewall validity time
   * result: set to -1 if an error occurs, 0 on success.
   */

    if (soap_call_idss__register_firewall (&soap, contact_string, "",
            info_gen, range, &result))
    {
      soap_print_fault (&soap, stderr);
      usage(argv[0]);
      if(server)
        free(server);
      exit (1);
    }
    msg = client_message(result);
    if (msg)
    {
      printf("%s\n", msg);
    }
    
  
  
  if(info_gen.hostname)free(info_gen.hostname);
  if(info_gen.admindn)free(info_gen.admindn);
  if(info_gen.host)free(info_gen.host);

  for(i=0; i<num_range; i++)
  {
    for(j=0; j<num_port_range[i]; j++)
    {
      if(info_port[i].ports.__ptr[j])free(info_port[i].ports.__ptr[j]);
    }
    if(info_port[i].ports.__ptr)free(info_port[i].ports.__ptr);
    for(j=0; j<num_prot_range[i]; j++)
    {
      if(info_port[i].prot.__ptr[j])free(info_port[i].prot.__ptr[j]);
    }
    if(info_port[i].prot.__ptr)free(info_port[i].prot.__ptr);
  }
  if(num_port_range)free(num_port_range);
  if(num_prot_range)free(num_prot_range);
  if(info_port)free(info_port);

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
