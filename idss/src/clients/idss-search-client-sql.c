/** @file idss-search-client -sql
 * Query an idss-store server
 *
 * To run
 *      ./idss-search-client-sql
 *
 * Copyright 2022-2024 University of Salento, Italy.
 * All rights reserved.
 *
 */


#include <unistd.h>
#include "idssH.h"
#include "idss_client.h"
#include <stdio.h>
#include <syslog.h>


extern char *optarg;

static int verbose = 0;

static void usage (char *name)
{
  printf ("%s: Usage:\n", name);
  printf ("\t-h web_service_hostname: FQDN of the server hosting" \
          " the iDSS web service\n");
  printf ("\t-p web_service_port: port number where the" \
          " iDSS web service is listening on\n");
  printf ("\t-q <SELECT statement>: query\n");
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
  struct soap          soap;
  xsd__string       result = NULL;

  char                 contact_string[256];
  char                 *ws_hostname;
  unsigned short int   ws_port;
  char 		       *sql;
  int                  c;

/* Setting default values */
  ws_hostname = strdup (IDSS_SERVICE_HOSTNAME);
  ws_port = IDSS_SERVICE_LISTENING_PORT;
  sql = strdup("select * from tb_host;");



  while ((c = getopt (argc, argv, "h:p:q:vu")) > 0)
  {
    switch (c)
    {
      case 'h':
        free (ws_hostname);
        ws_hostname = strdup (optarg);
        break;
      case 'p':
        ws_port = atoi (optarg);
        break;
      case 'q':
        sql = strdup (optarg);
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
  soap_ssl_init(); /* init OpenSSL */

  soap.fsslverify = verify_callback;

  if (soap_ssl_client_context(&soap, SOAP_SSL_REQUIRE_SERVER_AUTHENTICATION | SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE | SOAP_SSL_SKIP_HOST_CHECK | SOAP_TLSv1, "client.pem", "InnoCyPES2024", "cacert.pem", "/etc/ssl/certs", NULL))
  {
        soap_print_fault(&soap, stderr);
        exit(EXIT_FAILURE);
  }



  sprintf (contact_string, "https://%s:%d", ws_hostname, ws_port);
  contact_string[strlen(contact_string)] = '\0';

  if (verbose)
  {
    printf ("Connecting to service on... %s\n", contact_string);

    {
      printf ("Display the value of all of the attributes\n");
    }
  }

/*
 * soap_call_idss__search
 *
 * query an arbitrary idss-store server
 *
 * host: select only the informations about the host specified
 * result: query result
 */

if (soap_call_idss__search_sql(&soap, contact_string, "", sql, &result))
{
   soap_print_fault (&soap, stderr);
   return 1;
}
else
{
   if(result)
   {
            save_dime_data(result, "search-sql.xml");
    }
}


  if (sql)
    free (sql);
  if(ws_hostname)
    free (ws_hostname);

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
