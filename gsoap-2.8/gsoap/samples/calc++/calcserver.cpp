/*
	calcserver.cpp

	Example calculator service in C++

	$ soapcpp2 -i calc.h
	$ c++ -o calcserver++ calcserver.cpp stdsoap2.cpp soapC.cpp soapcalcService.cpp
	where stdsoap2.cpp is in the 'gsoap' directory, or use libgsoap++:
	$ c++ -o calcserver++ calcserver.cpp soapC.cpp soapcalcService.cpp -lgsoap++
        $ ./calcserver 8080

	Compilation for concurrent multithreaded service:
	$ c++ -DMULTITHREADED -o calcserver++ calcserver.cpp stdsoap2.cpp soapC.cpp soapcalcService.cpp -lpthread

	Compilation for secure HTTPS concurrent multithreaded service (requires server.pem to load):
	$ c++ -DWITH_OPENSSL -DMULTITHREADED -o calcserver++ calcserver.cpp stdsoap2.cpp soapC.cpp soapcalcService.cpp -lssl -lcrypto -lpthread

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

#include "soapcalcService.h"
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
  if (argc < 2)
  {
    /* serve as CGI application */
    calcService calc;
    calc.serve();
    calc.destroy();
  }
  else
  {
    int port = atoi(argv[1]);
    if (!port)
    {
      fprintf(stderr, "Usage: calcserver++ <port>\n");
      exit(EXIT_SUCCESS);
    }

#ifdef MULTITHREADED

    /* multithreaded with efficient HTTP chunking and keep-alive */
    calcService calc(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
    SOAP_SOCKET m = calc.bind(NULL, port, 100); /* longer queue is OK */
    calc.send_timeout = 5; /* never wait forever on sends, 5 second timeout */
    calc.recv_timeout = 5; /* never wait forever on receives, 5 second timeout */
    if (!soap_valid_socket(m))
    {
      calc.soap_stream_fault(std::cerr);
      exit(EXIT_FAILURE);
    }

#ifndef WIN32
    signal(SIGPIPE, sigpipe_handle); /* ignore broken pipes */
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
    if (soap_ssl_server_context(&calc,
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
      calc.soap_stream_fault(std::cerr);
      exit(EXIT_FAILURE);
    }
    /* enable CRL, may need SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE when certs have no CRL
    if (soap_ssl_crl(&calc, ""))
    {
      calc.soap_stream_fault(std::cerr);
      exit(EXIT_FAILURE);
    }
    */
#endif

    for ( ; ; )
    {
      SOAP_SOCKET s = calc.accept();
      if (!soap_valid_socket(s))
      {
        calc.soap_stream_fault(std::cerr);
        exit(EXIT_FAILURE);
      }
      THREAD_TYPE tid;
      calcService *worker = calc.copy();
      if (!worker)
      {
        calc.soap_force_close_socket();
        continue;
      }
      while (THREAD_CREATE(&tid, (void*(*)(void*))&process_request, worker))
        sleep(1);
    }
    calc.destroy();

#else

    /* run iterative server on port until fatal error */
    calcService calc;
    if (calc.run(port))
    {
      calc.soap_stream_fault(std::cerr);
      exit(EXIT_FAILURE);
    }
    calc.destroy();

#endif

  }
  return 0;
} 


#ifdef MULTITHREADED
void *process_request(void *arg)
{
  calcService *calc = reinterpret_cast<calcService*>(arg);
  THREAD_DETACH(THREAD_ID);

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
  if (calc->ssl_accept() != SOAP_OK)
  {
    /* when soap_ssl_accept() fails, socket is closed and SSL data reset */
    calc->soap_stream_fault(std::cerr);
    fprintf(stderr, "SSL request failed, continue with next...\n");
  }
  else
#endif
  {
    calc->serve();
  }

  calc->destroy();
  delete calc;
  return NULL;
}
#endif

int calcService::add(double a, double b, double *result)
{
  *result = a + b;
  return SOAP_OK;
} 

int calcService::sub(double a, double b, double *result)
{
  *result = a - b;
  return SOAP_OK;
} 

int calcService::mul(double a, double b, double *result)
{
  *result = a * b;
  return SOAP_OK;
} 

int calcService::div(double a, double b, double *result)
{
  if (b)
    *result = a / b;
  else
  {
    char *s = (char*)soap_malloc(this, 1024);
    (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't divide %f by %f</error>", a, b);
    return soap_senderfault("Division by zero", s);
  }
  return SOAP_OK;
} 

int calcService::pow(double a, double b, double *result)
{
  *result = ::pow(a, b);
  if (soap_errno == EDOM)	/* soap_errno is like errno, but compatible with Win32 */
  {
    char *s = (char*)soap_malloc(this, 1024);
    (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't take power of %f to %f</error>", a, b);
    return soap_senderfault("Power function domain error", s);
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
