#include <unistd.h>
#include "idssH.h"
#include "idss_client.h"
#include <stdio.h>
#include <string.h>
#include <syslog.h>

extern char *optarg;

static int verbose = 0;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting the" \
          " iDSS web service\n");
  printf ("\t-p web_service_port: port number where the iDSS web" \
          " service is listening on\n");
  printf ("\t -d <level> sets debug level (0-2)\n");
  printf ("\t-q query value: set the query value; default: 'select * from tb_user'\n");
  printf ("\t-t ttl value: set the ttl, in sec; default: 60\n");
  printf ("\t-v : verbose mode\n");
  printf ("\t-u : usage \n");
}



int verify_callback(int ok, X509_STORE_CTX *store)
{
    char data[256];


	if(verbose)
		fprintf(stderr, "Entering verify_callback...\n");

	// The ok parameter of this callback indicates whether the verification of the certificate in question passed
	// (ok == 1) or failed (ok == 0)
	// If the callback returns 1 then the handshake is continued and the connection maybe established
	// To return 1 when ok == 0 requires resetting the error state with X509_STORE_CTX_set_error(store, X509_V_OK)
	// If the callback returns 0 then the handshake is immediately terminated with "verification failed"
	//  and a verification failure alert is sent to the peer.

	if (!ok){


		if(X509_STORE_CTX_get_error(store) == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN){

			if(verbose){
				fprintf(stderr, "This certificate is self-signed...\n");
				fprintf(stderr, "Allowing self-signed certificate for testing purposes\n");
				fprintf(stderr, "Self-signed certificates must not be used in production\n");
			}

			// allow self-signed certificates for testing;
			// self-signed certificates must not be used for production
    			X509_STORE_CTX_set_error(store, X509_V_OK);
    			ok = 1;
		}
		else {
			X509 *cert = X509_STORE_CTX_get_current_cert(store);
			int depth = X509_STORE_CTX_get_error_depth(store);
			int err = X509_STORE_CTX_get_error(store);

			if(verbose)
				fprintf(stderr, "Error with certificate at depth: %i\n", depth);

			X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
			if(verbose)
				fprintf(stderr, " issuer = %s\n", data);

			X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
			if(verbose){
				fprintf(stderr, " subject = %s\n", data);
				fprintf(stderr, " err %i:%s\n", err, X509_verify_cert_error_string(err));
			}

		}

  	}
	else {

		if(verbose){
			fprintf(stderr, "The following certificate has been successfully verified:\n");
			X509 *cert = X509_STORE_CTX_get_current_cert(store);
			X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
			fprintf(stderr, " issuer = %s\n", data);
			X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
			fprintf(stderr, " subject = %s\n", data);
			fprintf(stderr, "Exiting verify_callback...\n");
		}

	}

	return ok;
}


int main (int argc, char **argv)
{
  struct soap         			soap;
  unsigned short int  			port;
  char                			*web_service;
  char                			contact_string[256];
  int                 			j;
  char		      			*query;
  int		      			ttl;
  struct idss__query_submit_response   response;
  int					res;

  port = IDSS_SERVICE_LISTENING_PORT;
  web_service = strdup (IDSS_SERVICE_HOSTNAME);
  query = strdup ("select * from tb_user");
  ttl = 60;

  while ((j = getopt (argc, argv, "h:p:q:t:vu")) > 0)
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
      case 'q':
	free (query);
	query = strdup (optarg);
	break;
      case 't':
	ttl = atoi(optarg);
	break;
      case 'v':
        verbose = 1;
        break;
      case 'u':
      default:
        usage (argv[0]);
	if (web_service)
		{
		 free(web_service);
		 web_service = NULL;
		}
	if (query)
		{
		 free(query);
		 query = NULL;
		}
        exit (1);
    }
  }

  if (!web_service)
	{
	 printf("Not enough memory.\n");
	 if(query)
	  free(query);
	 return 0;
	}

  if (!query)
	{
	 printf("Not enough memory.\n");
	 if(web_service)
	  free(web_service);
	 return 0;
	}




  soap_init(&soap);
  soap_ssl_init(); /* init OpenSSL */
  ttl *= 1000;

  soap.fsslverify = verify_callback;

  if (soap_ssl_client_context(&soap, SOAP_SSL_REQUIRE_SERVER_AUTHENTICATION | SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE | SOAP_SSL_SKIP_HOST_CHECK | SOAP_TLSv1, "client.pem", "InnoCyPES2024", "cacert.pem", "/etc/ssl/certs", NULL))
  {
        soap_print_fault(&soap, stderr);
        exit(EXIT_FAILURE);
  }


  sprintf (contact_string, "https://%s:%d", web_service, port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Contacting the web service listening on... %s\n",
            contact_string);
  }

  res = soap_call_idss__query_submit(&soap, contact_string, "", query, ttl, &response);
  if ((res) || (response.result))
	{
	 if (res)
		soap_print_fault (&soap, stderr);
	        printf ("Query submission failed.\n");
	}
  else
	printf ("UQI:  %s\n", response.uqi);

  if (query)
	{
	 free (query);
	 query = NULL;
	}
  if (web_service)
	{
	 free (web_service);
	 web_service = NULL;
	}
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
