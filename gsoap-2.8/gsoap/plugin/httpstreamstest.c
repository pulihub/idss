/*
	httpstreamstest.h

	httpstreamstest demo
	Combines calculator demo services, MTOM attachment demo services, JSON time services
	Servers to test against are located in samples/calc, samples/mtom, samples/xml-rpc-json

	Compile:
	$ soapcpp2 -Iimport -c -C httpstreamstest.h
	$ cc -o httpstreamstest httpstreamstest.c soapC.c soapClient.c stdsoap2.c httpstreams.c xml-rpc.c json.c threads.c -lpthread

	Compile for HTTPS demo with OpenSSL (or -DWITH_GNUTLS or -DWITH_WOLFSSL or -DSYSTEMSSL):
	$ soapcpp2 -Iimport -c -C httpstreamstest.h
	$ cc -DWITH_OPENSSL -o sslhttpstreamstest httpstreamstest.c soapC.c soapClient.c stdsoap2.c httpstreams.c xml-rpc.c json.c threads.c -lssl -lcrypto -lpthread

	Run servers (separately compiled):
	$ ./calcserver 8080 &
	$ ./calcserver 8081 &
	$ ./mtom-test 8082 &
	$ ./mtom-test 8083 &
	$ ./json-currentTimeServer 8084 &
	when using HTTPS with SSL/TLS also start TLS/SSL services:
	$ ./sslcalcserver 8090 &
	$ ./sslcalcserver 8091 &

	Run the http streams test demo:
	$ ./httpstreamstest

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2000-2026, Robert van Engelen, Genivia, Inc. All Rights Reserved.
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

#include "soapH.h"		/* client-side generated definitions */
#include "calc.nsmap"		/* client-side generated definitions */
#include "plugin/httpstreams.h"	/* HTTP streams plugin */
#include "json.h"		/* JSON REST */

#ifndef WIN32
#include <signal.h>		/* defines SIGPIPE */
void sigpipe_handle(int x) { (void)x; }
#endif

#define N 100	/* number of clients for testing */
#define K 7	/* number of calls per calc client for testing */

#define MAX_HOST 5	/* max number of distinct host:port servers */
#define MAX_OPEN 4	/* max number of open streams per server */

#ifdef WITH_OPENSSL
const char calc_server0[] = "https://localhost:8090";
const char calc_server1[] = "https://localhost:8091";
#else
const char calc_server0[] = "http://localhost:8080";
const char calc_server1[] = "http://localhost:8081";
#endif
const char mtom_server0[] = "http://localhost:8082";
const char mtom_server1[] = "http://localhost:8083";
const char json_server0[] = "http://localhost:8084";

int CRYPTO_thread_setup();
void CRYPTO_thread_cleanup();

/******************************************************************************\
 *
 *	Clients
 *
\******************************************************************************/

void *run_calc_client(void *arg)
{
  struct soap *ctx = arg;
  double factorial = 1;
  int i, k;

  for (k = 1; k <= K; ++k)
  {
    const char *endpoint = (k % 2 ? calc_server0 : calc_server1);

    if (soap_call_ns__mul(ctx, endpoint, NULL, factorial, k, &factorial))
    {
      soap_print_fault(ctx, stderr);
      return NULL;
    }

    /*
       i = soap_http_streams_stream_id(ctx);
       fprintf(stderr, "Thread %p stream %d multiply by %d = %g\n", THREAD_ID, i, k, factorial);
    */

    soap_end(ctx);
  }

  i = soap_http_streams_stream_id(ctx);
  fprintf(stderr, "Thread %p stream %d result %d! = %g\n", THREAD_ID, i, K, factorial);

  return NULL;
}

void *run_json_client(void *arg)
{
  struct soap *ctx = arg;
  struct value *request = new_value(ctx);
  struct value response;

  /* make the JSON REST POST request and get response */
  *string_of(request) = "getCurrentTime";
  if (json_call(ctx, json_server0, request, &response))
  {
    soap_print_fault(ctx, stderr);
  }
  else if (is_string(&response)) /* JSON does not support a dateTime value: this is a string */
  {
    int i = soap_http_streams_stream_id(ctx);
    fprintf(stderr, "Thread %p stream %d time = %s\n", THREAD_ID, i, *string_of(&response));
  }
  else /* error? */
  {
    int i = soap_http_streams_stream_id(ctx);
    fprintf(stderr, "Thread %p stream %d error:\n", THREAD_ID, i);
    ctx->sendfd = 2; /* stderr */
    json_write(ctx, &response);
    ctx->sendfd = 1; /* stdout */
    fprintf(stderr, "\n");
  }

  /* clean up JSON data */
  soap_end(ctx);

  return NULL;
}

void *run_mtom_client(void *arg)
{
  const char *raw = "abcdefghijklmnopqrstuvwxyz01234567890\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
  struct soap *ctx = arg;
  struct x__DataType data;
  struct x__WrapperType wrap;
  struct m__EchoTestSingleResponse single;
  int i;

  soap_default_x__DataType(ctx, &data);
  soap_default_x__WrapperType(ctx, &wrap);

  data.__union = SOAP_UNION_x__data_base64;
  data.choice.base64.__ptr = (unsigned char*)raw;
  data.choice.base64.__size = (int)strlen(raw) + 1;
  if (soap_call_m__EchoTestSingle(ctx, mtom_server0, NULL, &data, &single))
  {
    soap_print_fault(ctx, stderr);
    return NULL;
  }

  i = soap_http_streams_stream_id(ctx);

  if (!single.x__Data
      || single.x__Data->__union != SOAP_UNION_x__data_xop__Include
      || !single.x__Data->choice.xop__Include.__ptr
      || single.x__Data->choice.xop__Include.__size != data.choice.base64.__size
      || strcmp((char*)single.x__Data->choice.xop__Include.__ptr, (char*)data.choice.base64.__ptr))
    fprintf(stderr, "Thread %p stream %d EchoTestSingle 1: data transcription error\n", THREAD_ID, i);
  else
    fprintf(stderr, "Thread %p stream %d EchoTestSingle 1: OK\n", THREAD_ID, i);

  data.__union = SOAP_UNION_x__data_xop__Include;
  data.choice.xop__Include.__ptr = (unsigned char*)raw;
  data.choice.xop__Include.__size = (int)strlen(raw) + 1;
  data.choice.xop__Include.id = NULL;
  data.choice.xop__Include.type = "text/xml";
  data.choice.xop__Include.options = NULL;
  data.xmime5__contentType = "text/xml";
#ifdef WITH_NOIDREF
  /* compiling with WITH_NOIDREF removes auto-detection of attachments */
  soap_set_mime(ctx, NULL, NULL); /* so we explicitly set MIME attachments */
#endif
  if (soap_call_m__EchoTestSingle(ctx, mtom_server1, NULL, &data, &single))
  {
    soap_print_fault(ctx, stderr);
    return NULL;
  }

  i = soap_http_streams_stream_id(ctx);

  if (!single.x__Data
      || single.x__Data->__union != SOAP_UNION_x__data_base64
      || !single.x__Data->choice.base64.__ptr
      || single.x__Data->choice.base64.__size != data.choice.xop__Include.__size
      || strcmp((char*)single.x__Data->choice.base64.__ptr, (char*)data.choice.xop__Include.__ptr))
    fprintf(stderr, "Thread %p stream %d EchoTestSingle 2: data transcription error\n", THREAD_ID, i);
  else
    fprintf(stderr, "Thread %p stream %d EchoTestSingle 2: OK\n", THREAD_ID, i);

  soap_end(ctx);

  return NULL;
}

/******************************************************************************\
 *
 *	Main
 *
\******************************************************************************/

int main()
{
  THREAD_TYPE tid[N];
  struct soap *clients[N];
  int i;

  soap_http_streams_max_host(MAX_HOST);
  soap_http_streams_max_open(MAX_OPEN);

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
  /* Uncomment to call this first before all else if SSL is initialized elsewhere, e.g. in application code */
  /* soap_ssl_noinit(); */
  /* Init SSL before any threads are started (do this just once) */
  soap_ssl_init();
  /* set up SSL locks (not needed for OpenSSL 1.1.0 and greater) */
  if (CRYPTO_thread_setup())
  {
    fprintf(stderr, "Cannot setup thread mutex for OpenSSL\n");
    exit(1);
  }
#endif

#ifndef WIN32
  /* ignore broken pipes */
  signal(SIGPIPE, sigpipe_handle); /* or use SIG_IGN */
#endif

  for (i = 0; i < N; i++)
  {
    clients[i] = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
    if (soap_ssl_client_context(clients[i],
      /* SOAP_SSL_NO_AUTHENTICATION, */ /* for encryption w/o authentication */
      /* SOAP_SSL_DEFAULT | SOAP_SSL_SKIP_HOST_CHECK, */ /* if we don't want the host name checks since these will change from machine to machine */
      SOAP_SSL_DEFAULT | SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE, /* allow self-signed, expired, and certificates w/o CRL */
      /* SOAP_SSL_DEFAULT, */ /* use SOAP_SSL_DEFAULT in production code */
      NULL,		/* keyfile (cert+key): required only when client must authenticate to server (see SSL docs to create this file) */
      NULL,		/* password to read the keyfile */
      "cacert.pem",	/* optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs, use gnutls-cacert.pem with GNUTLS and WolfSSL see README.txt */
      NULL,		/* optional capath to directory with trusted certificates */
      NULL		/* if randfile!=NULL: use a file with random data to seed randomness */ 
      ))
    {
      soap_print_fault(clients[i], stderr);
      exit(EXIT_FAILURE);
    }
    /* Consider using soap_ssl_client_setup() instead of soap_ssl_client_context() above:
     - automatically uses certificates stored in Unix/Linux common locations
     - automatically uses Windows system certificate store
     - when -DWITH_WININET is defined, uses the gSOAP WinInet plugin with WinInet system certificate store
     - when -DWITH_CURL is defined, uses the gSOAP CURL plugin with CURL certificate store
    if (soap_ssl_client_setup(&soap,
      // SOAP_SSL_DEFAULT | SOAP_SSL_SKIP_HOST_CHECK, if we don't want the host name checks since these will change from machine to machine
      SOAP_SSL_DEFAULT | SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE, // allow self-signed, expired, and certificates w/o CRL
      NULL,               // keyfile (cert+key): required only when client must authenticate to server (see SSL docs to create this file)
      NULL,               // password to read the keyfile
      "cacert.pem",       // optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs
      NULL                // optional capath to directory with trusted certificates
    ))
    {
      soap_print_fault(&soap, stderr);
      exit(1);
    }
    */
#endif
    /* example timeout settings, this depends on client application requirements */
    clients[i]->connect_retry = 2;
    clients[i]->connect_timeout = 5;
    clients[i]->send_timeout = 5;
    clients[i]->recv_timeout = 5;
    clients[i]->transfer_timeout = 10;
    
    /* register http streams plugin with an internal static shared streams management hub */
    if (soap_register_plugin(clients[i], http_streams))
      soap_print_fault(clients[i], stderr);
  }

#ifdef TIMED
  struct timeval t;
  gettimeofday(&t, NULL);
#endif

  for (i = 0; i < N; i++)
    THREAD_CREATE(&tid[i], &run_calc_client, clients[i]);

  for (i = 0; i < N; i++)
  {
    THREAD_JOIN(tid[i]);
    soap_end(clients[i]);
  }

  for (i = 0; i < N; i++)
    THREAD_CREATE(&tid[i], &run_json_client, clients[i]);

  for (i = 0; i < N; i++)
  {
    THREAD_JOIN(tid[i]);
    soap_end(clients[i]);
  }

  for (i = 0; i < N; i++)
    THREAD_CREATE(&tid[i], &run_mtom_client, clients[i]);

  for (i = 0; i < N; i++)
  {
    THREAD_JOIN(tid[i]);
    soap_end(clients[i]);
  }

#ifdef TIMED
  struct timeval now;
  gettimeofday(&now, NULL);
  float ms = now.tv_usec;
  ms -= t.tv_usec;
  // t.tv_usec = now.tv_usec;
  ms = 1000.0 * (now.tv_sec - t.tv_sec) + ms/1000.0;
  // t.tv_sec = now.tv_sec;
  if (ms < 0.0)
    ms += 60000.0;
  printf("elapsed %g ms\n", ms);
#endif

  for (i = 0; i < N; i++)
    soap_free(clients[i]);

  CRYPTO_thread_cleanup();
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
