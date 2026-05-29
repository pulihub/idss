/*
	calcserver.c

	Example calculator service in C

	Compilation in C (see samples/calc/calc.h):
	$ soapcpp2 -c calc.h
	$ cc -o calcserver calcserver.c stdsoap2.c soapC.c soapServer.c
	where stdsoap2.c is in the 'gsoap' directory, or use libgsoap:
	$ cc -o calcserver calcserver.c soapC.c soapServer.c -lgsoap
        $ ./calcserver 8080

	Compilation for concurrent multithreaded service:
	$ cc -DMULTITHREADED -o calcserver calcserver.c stdsoap2.c soapC.c soapServer.c -lpthread

	Compilation for secure HTTPS concurrent multithreaded service (requires server.pem to load):
	$ cc -DWITH_OPENSSL -DMULTITHREADED -o calcserver calcserver.c stdsoap2.c soapC.c soapServer.c -lssl -lcrypto -lpthread

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2001-2026, Robert van Engelen, Genivia, Inc. All Rights Reserved.
This software is released under one of the following two licenses:
GPL.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapH.h"
#include "calc.nsmap"

#ifndef WIN32
#include <signal.h>
void sigpipe_handle(int x) { (void)x; } /* ignore broken pipes */
#endif

/* optional, to enable concurrent multithreaded server */
#ifdef MULTITHREADED
#include "plugin/threads.h" /* Windows also requires plugin/threads.c */
#endif

/* optional, to enable HTTPS with OpenSSL prior to 1.1.0 */
int CRYPTO_thread_setup();
void CRYPTO_thread_cleanup();

void *process_request(void*);

int main(int argc, char **argv)
{
  SOAP_SOCKET m, s; /* sockets */
  struct soap soap;
  soap_init(&soap);

  if (argc < 2)
  {
    /* serve as CGI application */
    soap_serve(&soap);
  }
  else
  {
    int port = atoi(argv[1]);
    if (!port)
    {
      fprintf(stderr, "Usage: calcserver <port>\n");
      exit(EXIT_SUCCESS);
    }

#ifdef MULTITHREADED
    /* multithreaded with efficient HTTP chunking and keep-alive */
    THREAD_TYPE tid;
    soap_set_mode(&soap, SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
    m = soap_bind(&soap, NULL, port, 100); /* longer queue is OK */
#else
    /* single threaded can't use keep-alive since only one connection at a time */
    m = soap_bind(&soap, NULL, port, 1); /* very short queue: a long queue can starve clients (possible DoS) */
#endif

    soap.send_timeout = 5; /* never wait forever on sends, 5 second timeout */
    soap.recv_timeout = 5; /* never wait forever on receives, 5 second timeout */
    if (!soap_valid_socket(m))
    {
      soap_print_fault(&soap, stderr);
      exit(EXIT_FAILURE);
    }

#ifndef WIN32
    signal(SIGPIPE, sigpipe_handle); /* ignore broken pipes: */
#endif

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
    /* Enable optional HTTPS server */
    /* Uncomment to call this first before all else if SSL is initialized elsewhere, e.g. in application code */
    /* soap_ssl_noinit(); */
    /* Init SSL before any threads are started (do this just once) */
    soap_ssl_init();
    /* set up SSL locks (not needed for OpenSSL 1.1.0 and greater) */
    if (CRYPTO_thread_setup())
    {
      fprintf(stderr, "Cannot setup thread mutex for OpenSSL\n");
      exit(EXIT_FAILURE);
    }
    /* The supplied server certificate "server.pem" assumes that the server is
      running on 'localhost', so clients can only connect from the same host when
      verifying the server's certificate.
      To verify the certificates of third-party services, they must provide a
      certificate issued by Verisign or another trusted CA. At the client-side,
      the capath parameter should point to a directory that contains these
      trusted (root) certificates or the cafile parameter should refer to one
      file will all certificates. To help you out, the supplied "cacerts.pem"
      file contains the certificates issued by various CAs. You should use this
      file for the cafile parameter instead of "cacert.pem" to connect to trusted
      servers. Note that the client may fail to connect if the server's
      credentials have problems (e.g. expired).
      Note 1: the password and capath are not used with GNUTLS
      Note 2: setting capath may not work on Windows.
    */
    if (soap_ssl_server_context(&soap,
      SOAP_SSL_DEFAULT,	/* use SOAP_SSL_REQUIRE_CLIENT_AUTHENTICATION to verify clients: client must provide a key file e.g. "client.pem" and "password" */
      "server.pem",	/* keyfile (cert+key): see README.txt to create this file, use non-password protected gnutls-server.pem with GNUTLS and WolfSSL see README.txt */
      "password",	/* password to read the private key in the key file */
      NULL, 	        /* cacert file to store trusted certificates (to authenticate clients), see README.txt */
      NULL,		/* capath */
      NULL,       	/* DH file name (e.g. "dh2048.pem") or DH param key len bits in string (e.g. "2048"), if NULL then RSA with 2048 bits is used instead (bits defined by SOAP_SSL_RSA_BITS) */
      NULL,		/* if randfile!=NULL: use a file with random data to seed randomness */ 
      "calcserver"	/* server identification for SSL session cache (unique server name, e.g. use argv[0]) */
    )
    )
    {
      soap_print_fault(&soap, stderr);
      exit(EXIT_FAILURE);
    }
    /* enable CRL, may need SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE when certs have no CRL
    if (soap_ssl_crl(&soap, ""))
    {
      soap_print_fault(&soap, stderr);
      exit(EXIT_FAILURE);
    }
    */
#endif

    fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
    for ( ; ; )
    {
#ifdef MULTITHREADED
      struct soap *tsoap;
#endif

      s = soap_accept(&soap);
      fprintf(stderr, "Socket connection successful: socket = %d\n", s);
      if (!soap_valid_socket(s))
      {
        soap_print_fault(&soap, stderr);
        exit(1);
      }

#ifdef MULTITHREADED
      tsoap = soap_copy(&soap);
      if (!tsoap)
      {
        soap_closesock(&soap);
        continue;
      }
      while (THREAD_CREATE(&tid, (void*(*)(void*))&process_request, tsoap))
        sleep(1);
#else
      if (soap_serve(&soap))
        soap_print_fault(&soap, stderr);
      soap_destroy(&soap);
      soap_end(&soap);
#endif
    }

    CRYPTO_thread_cleanup(); /* OpenSSL before 1.1.0 */
  }

  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  return 0;
}

#ifdef MULTITHREADED
void *process_request(void *soap)
{
  THREAD_DETACH(THREAD_ID);

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
  if (soap_ssl_accept(soap) != SOAP_OK)
  {
    /* when soap_ssl_accept() fails, socket is closed and SSL data reset */
    soap_print_fault(soap, stderr);
    fprintf(stderr, "SSL request failed, continue with next...\n");
  }
  else
#endif
  {
    soap_serve((struct soap*)soap);
  }

  soap_destroy((struct soap*)soap);
  soap_end((struct soap*)soap);
  soap_free((struct soap*)soap);
  return NULL;
}
#endif

/******************************************************************************\
 *
 *	Service methods
 *
\******************************************************************************/

int ns__add(struct soap *soap, double a, double b, double *result)
{
  (void)soap;
  *result = a + b;
  return SOAP_OK;
}

int ns__sub(struct soap *soap, double a, double b, double *result)
{
  (void)soap;
  *result = a - b;
  return SOAP_OK;
}

int ns__mul(struct soap *soap, double a, double b, double *result)
{
  (void)soap;
  *result = a * b;
  return SOAP_OK;
}

int ns__div(struct soap *soap, double a, double b, double *result)
{
  if (b)
  {
    *result = a / b;
  }
  else
  {
    char *s = (char*)soap_malloc(soap, 1024);
    (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't divide %f by %f</error>", a, b);
    return soap_sender_fault(soap, "Division by zero", s);
  }
  return SOAP_OK;
}

int ns__pow(struct soap *soap, double a, double b, double *result)
{
  *result = pow(a, b);
  if (soap_errno == EDOM)	/* soap_errno is like errno, but compatible with Win32 */
  {
    char *s = (char*)soap_malloc(soap, 1024);
    (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't raise %f to %f</error>", a, b);
    return soap_sender_fault(soap, "Power function domain error", s);
  }
  return SOAP_OK;
}

/******************************************************************************\
 *
 *	OpenSSL
 *
\******************************************************************************/

#if defined(WITH_OPENSSL) && OPENSSL_VERSION_NUMBER < 0x10100000L

struct CRYPTO_dynlock_value
{
  MUTEX_TYPE mutex;
};

static MUTEX_TYPE *mutex_buf = NULL;

static struct CRYPTO_dynlock_value *dyn_create_function(const char *file, int line)
{
  struct CRYPTO_dynlock_value *value;
  (void)file; (void)line;
  value = (struct CRYPTO_dynlock_value*)OPENSSL_malloc(sizeof(struct CRYPTO_dynlock_value));
  if (value)
    MUTEX_SETUP(value->mutex);
  return value;
}

static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line)
{
  (void)file; (void)line;
  if (mode & CRYPTO_LOCK)
    MUTEX_LOCK(l->mutex);
  else
    MUTEX_UNLOCK(l->mutex);
}

static void dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line)
{
  (void)file; (void)line;
  MUTEX_CLEANUP(l->mutex);
  OPENSSL_free(l);
}

static void locking_function(int mode, int n, const char *file, int line)
{
  (void)file; (void)line;
  if (mode & CRYPTO_LOCK)
    MUTEX_LOCK(mutex_buf[n]);
  else
    MUTEX_UNLOCK(mutex_buf[n]);
}

static unsigned long id_function()
{
  return (unsigned long)THREAD_ID;
}

int CRYPTO_thread_setup()
{
  int i;
  mutex_buf = (MUTEX_TYPE*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(MUTEX_TYPE));
  if (!mutex_buf)
    return SOAP_EOM;
  for (i = 0; i < CRYPTO_num_locks(); i++)
    MUTEX_SETUP(mutex_buf[i]);
  CRYPTO_set_id_callback(id_function);
  CRYPTO_set_locking_callback(locking_function);
  CRYPTO_set_dynlock_create_callback(dyn_create_function);
  CRYPTO_set_dynlock_lock_callback(dyn_lock_function);
  CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);
  return SOAP_OK;
}

void CRYPTO_thread_cleanup()
{
  int i;
  if (!mutex_buf)
    return;
  CRYPTO_set_id_callback(NULL);
  CRYPTO_set_locking_callback(NULL);
  CRYPTO_set_dynlock_create_callback(NULL);
  CRYPTO_set_dynlock_lock_callback(NULL);
  CRYPTO_set_dynlock_destroy_callback(NULL);
  for (i = 0; i < CRYPTO_num_locks(); i++)
    MUTEX_CLEANUP(mutex_buf[i]);
  OPENSSL_free(mutex_buf);
  mutex_buf = NULL;
}

#else

/* OpenSSL not used or OpenSSL after 1.1.0 */

int CRYPTO_thread_setup()
{
  return SOAP_OK;
}

void CRYPTO_thread_cleanup()
{ }

#endif
