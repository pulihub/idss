/** @file idss-vo-lookup-client.c
 * Lookups an existing virtual organization
 * 
 * To run
 *         ./idss-vo-lookup-client <options>
 * 
 * Copyright 2022-2023 University of Salento, Italy.
 * All rights reserved.
 *
 */

//#include "config.h"
#include <unistd.h>
#include "idssH.h"
#include <stdio.h>
#include "gsi.h"
#include <syslog.h>
#include "idss_client.h"

extern char *optarg;

/* authorization callback prototype */
int gsi_authorization_callback (struct soap *soap);

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-o VO name: name of the Virtual Organization" \
          " you want to lookup\n");
  printf ("\t-h web_service_hostname: FQDN of the server" \
          " hosting the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS" \
          " web service is listening on\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}

int main (int argc, char **argv)
{
  struct soap         soap;
  xsd__string         result;
  char                *name;
  char                *web_service;
  char                contact_string[256];
  unsigned short int  port;
  int                 i;
  int                 verbose;
  int                 j;
  
  static struct gsi_plugin_data *data;
  int rc;
  
  /* Setting default values */
  name = NULL;
  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  verbose = 0;
  
  
  globus_module_activate (GLOBUS_GSI_GSSAPI_MODULE);
  
  while ((j = getopt (argc, argv, "h:p:o:vu")) > 0)
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
      case 'o':
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

  /* register the gsoap-gsi plugin */
  if (soap_register_plugin (&soap, globus_gsi))
  {
    soap_print_fault (&soap, stderr);
    exit (1);
  }

  
  /* setup of authorization callback */
  data = (struct gsi_plugin_data *) soap_lookup_plugin (&soap, GSI_PLUGIN_ID);
  data->gsi_authorization_callback = NULL;
  
  /* we begin acquiring our credential */
  rc = gsi_acquire_credential(&soap);
  if(rc < 0)
  {
	 free (web_service);
 
	  if(name)
	  {
		free(name);
	  }
	  soap_end (&soap);
	  soap_done (&soap);
	  
	  
	  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);
	exit(1);
  }	
   /* setup of GSI channel */
  gsi_set_replay(&soap, GLOBUS_TRUE);
  gsi_set_sequence(&soap, GLOBUS_TRUE);
  gsi_set_confidentiality(&soap, GLOBUS_TRUE);
  gsi_set_integrity(&soap, GLOBUS_TRUE);

  sprintf (contact_string, "https://%s:%d", web_service, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Contacting the web service listening on... %s\n",
	    contact_string);
    if (name)
    {
      printf ("Looking up Virtual Organization... %s\n", name);
    }
    else
    {
      printf ("Looking up any available Virtual Organization\n");
    }
  }

  /*
   * soap_call_idss__lookup_vo
   * 
   * returns all the information about Virtual Organizations. It contacts
   * the supplied iDSS server on the default port
   * 
   * name: the name of the Virtual Organization to be searched
   * result: query result.
   */
  if (soap_call_idss__lookup_vo(&soap, contact_string, "",
              name, &result))
  {
    soap_print_fault (&soap, stderr);
  }
  else
  {
    if (verbose)
    {
      printf ("Your Virtual Organization has been looked up\n");
    }
    if(result)
	{
	  idss_vo_data * vo_data;
	  
      save_dime_data(result, "vo.xml");

      vo_data = idss_get_vo_data(result);
      if(vo_data)
      {
        for (i = 0; vo_data[i]; i++)
        {
		  printf("\n...... VO # %d\n", i + 1);
		  printf("VO name\t\t\t= %s\n", vo_data[i]->name); 
          printf("VO admin\t\t= %s\n", vo_data[i]->admin);
          printf("VO resource type\t= %s\n", vo_data[i]->res_type);
          printf("VO held desk URL\t= %s\n", vo_data[i]->hdu);
          printf("VO job manager\t\t= %s\n", vo_data[i]->jobm);
          printf("VO queue\t\t= %s\n", vo_data[i]->queue);
		  printf("VO phone numbers:\n");
          for (j = 0; vo_data[i]->hdpn[j]; j++)
            printf("\t%s\n", vo_data[i]->hdpn[j]);

          printf("\rVO File systems:\n");
		  for (j = 0; vo_data[i]->fs[j]; j++)
		    printf("\t%s:%s\n", vo_data[i]->fs[j]->type, vo_data[i]->fs[j]->path);
		
          printf("\rHosts:\n");
		  for (j = 0; vo_data[i]->hostname_l[j]; j++)
		    printf("\t%s\t(%s)\t(cd: %lu - vt: %u)\n", vo_data[i]->hostname_l[j]->hostname, vo_data[i]->hostname_l[j]->publisher, vo_data[i]->hostname_l[j]->cd, vo_data[i]->hostname_l[j]->vt);		  
        }
		idss_delete_vo_data(vo_data);
      } 
	}
    
  }
  
  
 
  free (web_service);
 
  if(name)
  {
    free(name);
  }
  soap_end (&soap);
  soap_done (&soap);
  
  
  globus_module_deactivate (GLOBUS_GSI_GSSAPI_MODULE);
  
  return 0;
}

/*
 * gsi_authorization_callback
 *
 * checks the received distinguished name against an access control list
 * stored in the file called authorized_dn. If the distinguished name 
 * received belongs 
 * to the access control list then the client can safely access 
 * the service
 *
 */
 
int
gsi_authorization_callback (struct soap *soap)
{
  char buf[512] = {'\0'}; 
  char *auth;
  char *auth_file;
  FILE *fd;
  
  struct gsi_plugin_data *data;
  
  data = (struct gsi_plugin_data *) soap_lookup_plugin (soap, GSI_PLUGIN_ID);

  auth = globus_libc_getenv("AUTHORIZATION_FILE");
  if(auth != NULL)
  {
    auth_file = strdup(auth);
  }
  else
  {
    auth_file = strdup (AUTHORIZATION_FILE);
  }
  
  fd = fopen (auth_file, "r");
  
  if (!fd)
  {

    globus_libc_printf ("Can not read file: %s\n", AUTHORIZATION_FILE);
    syslog(LOG_ALERT | LOG_USER, "Cannot read authorized_dn file");
    return -1;
  }
  while (fgets (buf, 512, fd) != NULL)
  {
    buf[strlen (buf) - 1] = '\0';
    if (!strcmp (data->server_identity, buf))
    {
     
      syslog(LOG_NOTICE | LOG_USER, "You are connecting to %s", data->server_identity);
      if(auth)
        free(auth);
      if(auth_file)
        free(auth_file);

      fclose (fd);

      return 0;
    }
  }
  if(auth)
    free(auth);
  if(auth_file)
    free(auth_file);
  fclose (fd);

  globus_libc_printf ("Sorry, service %s is not authorized\n",
          data->server_identity);
  syslog(LOG_NOTICE | LOG_USER, "Service %s not authorized\n",
          data->server_identity);
  return -1;
}

struct Namespace namespaces[] = {
  {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/"},
  {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/"},
  {"xsi", "http://www.w3.org/1999/XMLSchema-instance"},
  {"xsd", "http://www.w3.org/1999/XMLSchema"},
  {"idss", "urn:idss"},
  {NULL, NULL}
};
