/*
	httpstreams.c

	HTTP(S) streams plug-in

        Persistent http(s) connections ("streams") to accelerate client threads.

	When HTTPS is required, compile with WITH_OPENSSL, WITH_GNUTLS,
	WITH_WOLFSSL, or WITH_SYSTEMSSL e.g. cc -DWITH_OPENSSL -c httpstreams.c

	Usage (static shared streams case with one internal managing hub):

	soap_http_streams_max_host(2); // max 2 distinct http://host:port servers
	soap_http_streams_max_open(4); // max 4 concurrent open socket streams per server
	for (i = 0; i < N; i++)
	  THREAD_CREATE(&tid[i], &start_client, NULL);
	...
	for (i = 0; i < N; i++)
	  THREAD_JOIN(tid[i]);
	...
	// worker client thread:
	void *start_client(void *arg)
	{
	  struct soap *soap = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
	  soap_register_plugin(soap, http_streams);
	  // THREAD_DETACH(THREAD_ID); // use if we don't THREAD_JOIN()
	  ... // optional: soap_ssl_client_context(soap, ...) for HTTPS
	  ... // do client stuff, i.e. requesting services...
	  soap_destroy(soap);
	  soap_end(soap);
	  soap_free(soap);
	  return NULL;
	}

	Note:
        - a SOAP_TCP_ERROR "http_streams plugin error" with detail "Out of
          host:post server slots" is generated when the number of distinct
          host:port server combinations exceed soap_streams_max_host(), make
          sure to specify number of distinct host:port that clients use to
          connect.
        - a SOAP_SSL_ERROR is generated when attempting to connect with HTTPS
          over a prior plain HTTP connection or when HTTPS is not supported.
	- a shared streams array is dynamically allocated once for the static
	  shared streams hub, which is never deallocated (may be reported as a
	  memory leak).  By contrast, a user-defined managing hub (see below)
	  does not use the static hub and can clean everything up afterwards.

	Usage (user-defined local managing hub, multiple hubs are possible):

	// define a managing hub with max 2 distinct http://host:port servers
	// and max 4 concurrent open socket streams per connected server
	struct soap_http_shared_streams *hub = soap_new_http_streams(2, 4);
	for (i = 0; i < N; i++)
	  THREAD_CREATE(&tid[i], &start_client, hub);
	...
	for (i = 0; i < N; i++)
	  THREAD_JOIN(tid[i]);
	...
	// release hub and free all streams
	soap_free_http_streams(hub);
	...
	// worker client thread with user-defined hub:
	void *start_client(void *arg)
	{
	  struct soap_http_shared_streams *hub = arg;
	  struct soap *soap = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
	  soap_register_plugin_arg(soap, http_streams, hub);
	  // THREAD_DETACH(THREAD_ID); // use if we don't THREAD_JOIN()
	  ... // optional: soap_ssl_client_context(soap, ...) for HTTPS
	  ... // do client stuff, i.e. requesting services...
	  soap_destroy(soap);
	  soap_end(soap);
	  soap_free(soap);
	  return NULL;
	}

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

/**

@mainpage

- @ref httpstreams documents the HTTP streams plugin

*/

/**

@page httpstreams The HTTP streams plugin

[TOC]

@section httpstreams_0 Overview

The HTTP streams plugin manages concurrent shared persistent http(s)
connections ("streams") to accelerate client threads.  The HTTP streams plugin
keeps a server connections alive as long as possible and shares this connection
(or more than one connection in case of connection contention) with multiple
client threads.

The HTTP streams plugin is useful when multiple client threads connect to the
same server (the same host:port) repeatedly and frequently.

A client application with a single client thread instead of multiple threads
may not benefit from this plugin when it already keeps connections alive by
creating a gSOAP context or client proxy only once and using it as a long as
possible with keeo-alive enabled.  Destroying a gSOAP context or client proxy
closes the connection, which gets in the way of keeping connections alive.  If
this is the case, then this plugin helps to keep the connection alive, even
after the gSOAP context or client proxy is destroyed.

- supports http and also https when all source code is compiled with the
  `WITH_OPENSSL` or `WITH_GNUTLS` or `WITH_WOLFSSL` or `WITH_SYSTEMSSL` flag
- supports http/s gzip compression when all source code is compiled with the
  `WITH_GZIP` flag
- supports all transports over http/s such as SOAP/XML, SOAP/XML with
  attachments (MTOM, MIME, DIME), REST, JSON REST, XML-RPC
- client-side contexts should register the plugin as explained in this
  document, but no other client-side source code changes are necessary
- no limit on the number of client threads
- configurable parameters for the maximum number of host:port servers to share
  among client threads and the maximum number of open connections permitted to
  each server (maximum level of concurrent connections permitted per server)
- uses HTTP/1.1 without upgrading to HTTP/2 or HTTP/3, supporting legacy
  systems without introducing additional overhead and complexity while
  effectively mitigating the dreaded TIME_WAIT state and head-of-line-blocking
  among concurrent client threads

@section httpstreams_1 Introduction

When client applications repeatedly and frequently connect to the same server,
problems with TCP/IP sockets become apparent when connections are starting to
fail due to server connection overload.  Even when the server can handle
hundreds of simultaneous connections, connections that are closed afterwards
must be recycled for reuse.  When this happens, the TCP socket lingers in
TIME_WAIT state before it accepts a new connection.  This effectively reduces
the number of connections that can be established to a server in a given time.
This can be mitigated by tinkering with the TCP socket linger time (time in
TIME_WAIT state).  In gSOAP this can be done using the `soap.linger_time`
variable and by setting `soap.connect_flags = SO_LINGER`.  Setting the linger
time to zero can bypass the TIME_WAIT state.  However, this approach is not
always possible or even disarable because of other effects this has on the
server.

@subsection httpstreams_1_2 Multiplexing and pipelining

One solution is to use multiplexing or pipelining.  In this scenario with
multiplexing/pipelining, multiple client threads talk to the server over the
same TCP/IP connection that persists during a session.  With multiplexing,
clients send requests independently and recieve responses independently over
the same connection.  Multiple requests may be sent first before the responses
are received.  Pipelining (e.g. with the [HTTP pipelining
plugin](../../guide/html/group__group__io.html#details)) means that all
responses are received in the same order that requests were send.  Multiplexing
means that responses may be received out-of-order, but are reordered by the
control layer.  These mechanisms require inclusion of a request message ID to
match the ID of the response message (i.e. IDs are carried in HTTP headers).

Multiplexing eliminates the "head-of-line blocking" problem, where a single
transaction can prevent other transactions from taking place by allowing for
multiple requests and responses to be in transit "simultaneously" (to be more
clear: message transport is not concurrent or simultaneous, but message
ordering is independent so that a slow response on a request message does not
block another request message on the same connection).  While HTTP/2
multiplexing, has merits, especially for browsers that typically fetch many
images and other static data from a server with a burst of HTTP GET requests,
it has some drawbacks that researchers and developers have reported on,
including application coding requirements and network knowledge to exploit the
mechanism fully.

The HTTP/2 and HTTP/3 protocols support multiplexing.  This is done by
negotiating the HTTP/1.1 request by a client to upgrade the protocol to HTTP/2
when agreed to by the server in a roundtrip handshake.  Besides the additional
roundtrip, additional complexity is involved to establish secure HTTPS
transfers for clients over a multiplexed connection, using ALPN to extend TLS
avoiding additional roundtrips.

Multiplexing and pipelining require a managing layer to keep track of IDs and
to store the messages received, buffering the messages to deliver them to
clients in the right order.  When serving time is low (short response times) or
when client requests are more spaced apart in time (i.e. no bulk sent
requests), then multiplexing/pipelining becomes less effective.  Message
exchanges over the same connection no longer overlap (do not compete for
network access).

> Related subjects:
>
> - Server-side HTTP pipelining on the server side using the http_pipe plugin
>   in the gSOAP documentation.
> - Message queueing using the inbound message queue mq plugin in the gSOAP
>   documentation.
>
> See also the gSOAP documentation of [plugins and plugin registry functions](group__group__plugin.html)

@subsection httpstreams_1_3 Shared streams

Is multiplexing worth it?  Yes, but insofar "burst-mode" web request are made
to the same server potentially causing "head-of-line" requests with slow
responses that may block subsequent traffic on the same connection.  When
implementing SOAP/XML and JSON web services, these concerns are still present,
but the message exchange behavior differs from the typical bursty web browser
traffic.  Furthermore, web services may not be stateless, meaning that a
sequence of message exchanges must be performed in a pre-determined order and
each must be completed before the next can commence.  Such web service client
threads won't benefit from multiplexing.

To optimize web services connectivity and message exchanges without overloaidng
the server and risking TIME_WAIT states, the HTTP streams plugin establishes
one or more persistent connections (bidirectional "streams") to a server for up
to a given maximum number of active open connections.  The number of active
persistent connections depends on the concurrent demands by the client threads
sharing these connections.  Message exchanges are guaranteed to be sequenced as
before without the plugin.  Rather than multiplexing, another available
connection is automatically selected to prevent "head-of-line-blocking".  In
this way, a blocking client-server exchange is bypassed effectively by another
connection to the server.  A limited number of shared connections is
established on demand.  Each connection is closed when inactive (by the server
when timing out), preventing a server overload while maximizing throughput with
concurrency.  This allows for hundreds of client threads to share a limited
number of connections to a server without overloading the server and avoid the
dreaded TIME_WAIT state.

Another benefit of the HTTP streams plugin is that the overhead to set up and
tear down a secure SSL/TLS connection (for HTTPS) by each client thread that
wants to connect securely to a server can be amortized, since this is done only
once to establish the secure persistent connection that is shared among
multiple client threads.  This keeps the complexity and overhead of shared HTTP
and HTTPS persistent keep-alive connections low.

The lifetime of a persistent connection to a server is determined by the
maximum number of HTTP keep-alive roundtrips permitted by the server and by its
connection timeout policies.  This may vary across server implementations.  The
default maximum number of HTTP keep-alive roundtrips are 100
(`SOAP_MAXKEEPALIVE`) for gSOAP multi-threaded services enabled with the
`SOAP_IO_KEEPALIVE` flag.  The server-side roundtrip max count can be modified
by setting `soap.max_keep_alive` to the desired value.  Timeouts on the client
and server sides can be set to `soap.send_timeout` and `soap.recv_timeout`
measured in seconds, or in microseconds (10^-6 sec) that are expressed as a
negative value.  A zero value disables timeouts.  A connection is closed by the
server when the max keep-alive rountrips are reached or sooner when a
connection timeout occurs (which means that nothing was sent to the server
within a timeout time span.)

Connections shared by client threads are managed by a HTTP streams plugin
"shared streams hub" that optimally activates and manages connectivity to one
or more servers used by the clients.  Each distinct host:port is considered a
server (i.e. the path part in a URL does not matter).  To use the HTTP streams
plugin, the maximum number of distinct host:port servers must be specified in
advance (the specific host and port info comes from the client threads).  The
maximum number of open connections to each server must also be specified in
advance, which is used by the hub to manage the shared connections.

Both HTTP and HTTPS traffic is supported, assuming that distinct server ports
support HTTP and HTTPS (such as the default HTTP and HTTPS ports 80 and 443,
respectively).  The HTTP streams plugin blocks HTTPS requests attempted over
shared HTTP connections, when an application's client thread switches from
HTTPS to HTTP by accident (but only when HTTPS was used before to connect to a
server, since the hub has no knowledge of the server HTTP versus HTTPS
capabilities.)

@section httpstreams_2 Using the default global HTTP streams hub

By default when registering client threads with the HTTP streams plugin, a
single global static shared http streams hub is created that manages persistent
connections.  This supports most scenarios in which all client application
threads share connections to one or more servers.

When a shared streams hub is initialized, the maximum number of host:port
servers must be specified in advance by the application.  When not specified,
this defaults to eight (8) distinct host:port servers that can be reached by an
application's client threads.  The server host:port addresses do not need to be
specified in advance as these are picked up from the client thread requests
automatically.

In addition, when a shared streams hub is initialized, the maximum number of
persisten connections opened to each server must be specified in advance.  When
not specified, this default to up to four (4) open connections per host:port
server.  The maximum number of open connections should be less than or equal to
the number of active client threads.  A low number (1 to 3) increases
connection contention, but may not necessarily decrease overall performance
when server responses are quick enough on average and when client threads
connect less frequently (not in bursts).  In the best case, only one of the
client threads connects and exchanges a message at a time, which means that
only one persistent connection is activated and maintained by the hub (no
connection contention means no connection concurrency is required).  However,
if many client threads are created (tens to hundreds or more) that all make
frequent requests where server responses to client requests are slow, then the
maximum number of open connections may have to be increased to increase
connection concurrency.  This increases the overall performance, but up to the
point when network traffic bandwidth is exhausted.

To specify the maximum number of host:port servers and maximum number of open
connections per server, invoke `soap_http_streams_max_host()` and
`soap_http_streams_max_open()` functions, respectively:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    #include "plugin/httpstreams.h"
    ...
    soap_http_streams_max_host(2); // max 2 distinct http/s host:port servers
    soap_http_streams_max_open(4); // max 4 concurrent open socket streams per server
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As a simple example we define a function to start a new client thread that
makes one or more web service requests:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    void *start_client(void *arg)
    {
      struct soap *soap = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
      soap_register_plugin(soap, http_streams);
      // THREAD_DETACH(THREAD_ID); // use if we don't THREAD_JOIN()
      ... // optional: soap_ssl_client_context(soap, ...) for HTTPS ...
      ... // ... or use soap_ssl_client_setup() from ssl_setup.c
      ... // do client stuff, i.e. requesting services...
      soap_destroy(soap);
      soap_end(soap);
      soap_free(soap);
      return NULL;
    }
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The run-time flags `SOAP_IO_CHUNK` and `SOAP_IO_KEEPALIVE` are specified for
optimal HTTP communications and other non-`SOAP_IO_` flags are supported, such
as `SOAP_ENC_ZLIB` compression, but the HTTP streams plugin always sets
`SOAP_IO_CHUNK` and `SOAP_IO_KEEPALIVE` anyway when registering.

The usual client-side settings for timeouts are also supported and can be used,
for example after creating the context we may set some or all of these:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    soap->connect_retry = 3;        // retry 3 times to connect with exponential backoff
    soap->connect_timeout = 5;      // connecting times out after 5s (not on Linux)
    soap->send_timeout = 5;         // each send should not block longer than 5s
    soap->recv_timeout = 5;         // each recv should not block longer than 5s
    soap->recv_maxlength = 1048576; // allow up to 1MB received, fail if too large
    soap->transfer_timeout = 10;    // allow up to 10s message transfer time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that a connection retry uses exponential backoff, so that the first retry
takes 1 second delay after 5 second connect timed out, the second retry takes 2
seconds delay after connect timed out again, then 4, 8, 16, 32, 32, 32 seconds
delay with 32 seconds delay max.  For example, with the settings above we get
5+(5+1)+(5+2)+(5+4)=27 seconds delay (at least) for each failed client thread
request to the server when the server is non-responsive or does not run at all,
causing failed connection attempts.

Client threads are created with:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    THREAD_TYPE tid[N];
    for (i = 0; i < N; i++)
      THREAD_CREATE(&tid[i], &start_client, NULL);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

and joined with:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    for (i = 0; i < N; i++)
      THREAD_JOIN(tid[i]);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The plugin/httpstreams.c and plugin/threads.c code must be compiled and
linked with the application.  An example application plugin/httpstreamstest.c
is included.

To support HTTPS, all source code (application, gSOAP source code and the
HTTP streams plugin sources) must be compiled with one of the following four
compile-time flags:

- `WITH_OPENSSL` to compile and link with OpenSSL, or
- `WITH_GNUTLS` to compile and link with GNU TLS, or
- `WITH_WOLFSSL` to compile and link with WolfSSL, or
- `WITH_SYSTEMSSL` to compile and link with System SSL.

@note A `SOAP_TCP_ERROR` "http_streams plugin cerror" with detail "Out of
host:post server slots" is generated for a client call when the number of
distinct host:port server combinations exceed `soap_streams_max_host()`; please
make sure to specify number of distinct host:port that clients use to connect.

@note A `SOAP_SSL_ERROR` is generated when attempting to connect with HTTPS
over a prior plain HTTP connection or when HTTPS is not supported.

@note A shared streams array is dynamically allocated once for the static
shared streams hub, which is never deallocated (may be reported as a memory
leak).  By contrast, a user-defined managing hub (see below) does not use
the static hub and can clean everything up afterwards.

@section httpstreams_3 Defining a local HTTP streams hub

Instead of using the default single global static shared HTTP streams hub, you
can define one or more local hubs.  Each local hub is responsible to manage its
streams independently of the other hubs.

For example, to create a new hub to support two host:port servers used by
client threads with up to four persisten connections each:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    #include "plugin/httpstreams.h"
    ...
    // a new hub with max 2 distinct http/s host:port servers with
    // up to 4 concurrent open socket streams per server
    struct soap_http_shared_streams *hub = soap_new_http_streams(2, 4);
    ...
    THREAD_TYPE tid[N];
    for (i = 0; i < N; i++)
      THREAD_CREATE(&tid[i], &start_client, hub);
    ...
    for (i = 0; i < N; i++)
      THREAD_JOIN(tid[i]);
    ...
    // release hub and free all streams (use only after client threads terminated!)
    soap_free_http_streams(hub);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that the local hub is passed on to `start_client` to register the hub with
the client thread.  Otherwise, the client thread would use the global hub.
A local hub is registered with the client thread's context using
`soap_register_plugin_arg(soap, http_streams, hub)` as follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    void *start_client(void *arg)
    {
      struct soap_http_shared_streams *hub = arg; // use the local hub
      struct soap *soap = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
      soap_register_plugin_arg(soap, http_streams, hub);
      // THREAD_DETACH(THREAD_ID); // use if we don't THREAD_JOIN()
      ... // optional: soap_ssl_client_context(soap, ...) for HTTPS ...
      ... // ... or use soap_ssl_client_setup() from ssl_setup.c
      ... // do client stuff, i.e. requesting services...
      soap_destroy(soap);
      soap_end(soap);
      soap_free(soap);
      return NULL;
    }
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The usual client-side settings for timeouts are also supported and can be used,
for example after creating the context we may set some or all of these:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
    soap->connect_retry = 3;        // retry 3 times to connect with exponential backoff
    soap->connect_timeout = 5;      // connecting times out after 5s (not on Linux)
    soap->send_timeout = 5;         // each send should not block longer than 5s
    soap->recv_timeout = 5;         // each recv should not block longer than 5s
    soap->recv_maxlength = 1048576; // allow up to 1MB received, fail if too large
    soap->transfer_timeout = 10;    // allow up to 10s message transfer time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that a connection retry uses exponential backoff, so that the first retry
takes 1 second delay after 5 second connect timed out, the second retry takes 2
seconds delay after connect timed out again, then 4, 8, 16, 32, 32, 32 seconds
delay with 32 seconds delay max.  For example, with the settings above we get
5+(5+1)+(5+2)+(5+4)=27 seconds delay (at least) for each failed client thread
request to the server when the server is non-responsive or does not run at all,
causing failed connection attempts.

The plugin/httpstreams.c and plugin/threads.c code must be compiled and
linked with the application.  An example application plugin/httpstreamstest.c
is included.

To support HTTPS, all source code (application, gSOAP source code and the
HTTP streams plugin sources) must be compiled with one of the following four
compile-time flags:

- `WITH_OPENSSL` to compile and link with OpenSSL, or
- `WITH_GNUTLS` to compile and link with GNU TLS, or
- `WITH_WOLFSSL` to compile and link with WolfSSL, or
- `WITH_SYSTEMSSL` to compile and link with System SSL.

@note A `SOAP_TCP_ERROR` "http_streams plugin error" with detail "Out of
host:post server slots" is generated for a client call when the number of
distinct host:port server combinations exceed `soap_streams_max_host()`; please
make sure to specify number of distinct host:port that clients use to connect.

@note A `SOAP_SSL_ERROR` is generated when attempting to connect with HTTPS
over a prior plain HTTP connection or when HTTPS is not supported.

@note A shared streams array is dynamically allocated once for the static
shared streams hub, which is never deallocated (may be reported as a memory
leak).  By contrast, a user-defined managing hub (see below) does not use
the static hub and can clean everything up afterwards.
*/

#include "httpstreams.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define strcasecmp(s,t) _stricmp(s,t)
#endif

const char http_streams_id[] = HTTP_STREAMS_ID;

/******************************************************************************\
 *
 *      Default global static shared streams hub
 *
\******************************************************************************/

static struct soap_http_shared_streams http_streams_hub = { 8, 4, NULL, MUTEX_INITIALIZER };

/******************************************************************************\
 *
 *      Plugin registry
 *
\******************************************************************************/

static int http_streams_init(struct soap *soap, struct soap_plugin *p);
static void http_streams_delete(struct soap *soap, struct soap_plugin *p);
static int http_streams_connect(struct soap *soap, const char *endpoint, const char *host, int port);
static int http_streams_disconnect(struct soap *soap);

SOAP_FMAC1
int
SOAP_FMAC2
http_streams(struct soap *soap, struct soap_plugin *p, void *arg)
{
  p->id = http_streams_id;
  p->data = (arg ? arg : (void*)&http_streams_hub);
  p->fcopy = NULL;
  p->fdelete = http_streams_delete;
  return http_streams_init(soap, p);
}

/******************************************************************************/

static int
http_streams_init(struct soap *soap, struct soap_plugin *p)
{
  struct soap_http_shared_streams *hub = (struct soap_http_shared_streams*)p->data;
  MUTEX_LOCK(hub->init);
  if (!hub->streams)
  {
    int i;
    hub->streams = (struct soap_http_stream_state*)malloc(sizeof(struct soap_http_stream_state) * hub->max_host * hub->max_open);
    if (!hub->streams)
    {
      MUTEX_UNLOCK(hub->init);
      return soap->error = SOAP_EOM;
    }
    for (i = 0; i < hub->max_host * hub->max_open; ++i)
    {
      struct soap_http_stream_state *stream = &hub->streams[i];
      stream->hub = hub;
      stream->ctx = NULL;
      MUTEX_SETUP(stream->init);
      MUTEX_SETUP(stream->lock);
    }
  }
  MUTEX_UNLOCK(hub->init);
  soap_set_mode(soap, SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
  soap->data[2] = (void*)&hub->streams[0];
  soap->fconnect = http_streams_connect;
  soap->fdisconnect = http_streams_disconnect;
  soap->fopen = NULL;
  soap->fclose = NULL;
  return SOAP_OK;
}

/******************************************************************************/

static void
http_streams_delete(struct soap *soap, struct soap_plugin *p)
{
  struct soap_http_stream_state *stream = (struct soap_http_stream_state*)soap->data[2];
  if (stream)
  {
    MUTEX_UNLOCK(stream->init);
    MUTEX_UNLOCK(stream->lock);
  }
  (void)p;
}

/******************************************************************************\
 *
 *      User-defined shared stream hub allocation/deallocation
 *
\******************************************************************************/

SOAP_FMAC1
struct soap_http_shared_streams *
SOAP_FMAC2
soap_new_http_streams(int max_host, int max_open)
{
  struct soap_http_shared_streams *hub = (struct soap_http_shared_streams*)malloc(sizeof(struct soap_http_shared_streams));
  if (hub)
  {
    hub->max_host = (max_host > 0 ? max_host : 4);
    hub->max_open = (max_open > 0 ? max_open : 2);
    hub->streams = NULL;
    MUTEX_SETUP(hub->init);
  }
  return hub;
}

/******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_free_http_streams(struct soap_http_shared_streams *hub)
{
  if (hub)
  {
    if (hub->streams)
    {
      int i;
      for (i = 0; i < hub->max_host * hub->max_open; ++i)
      {
        struct soap_http_stream_state *stream = &hub->streams[i];
        if (stream->ctx)
        {
          soap_end(stream->ctx);
          soap_free(stream->ctx);
        }
        MUTEX_CLEANUP(stream->lock);
        MUTEX_CLEANUP(stream->init);
      }
      free(hub->streams);
    }
    MUTEX_CLEANUP(hub->init);
    free(hub);
  }
}

/******************************************************************************\
 *
 *      Static shared stream hub config
 *
\******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_http_streams_max_host(int max_host)
{
  MUTEX_LOCK(http_streams_hub.init);
  if (!http_streams_hub.streams)
    http_streams_hub.max_host = max_host;
  MUTEX_UNLOCK(http_streams_hub.init);
}

/******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_http_streams_max_open(int max_open)
{
  MUTEX_LOCK(http_streams_hub.init);
  if (!http_streams_hub.streams)
    http_streams_hub.max_open = max_open;
  MUTEX_UNLOCK(http_streams_hub.init);
}

/******************************************************************************\
 *
 *      Get shared stream id
 *
\******************************************************************************/

SOAP_FMAC1
int
SOAP_FMAC2
soap_http_streams_stream_id(struct soap *soap)
{
  struct soap_http_stream_state *stream = (struct soap_http_stream_state*)soap->data[2];
  if (!stream)
    return -1;
  return (int)(stream - stream->hub->streams);
}

/******************************************************************************\
 *
 *      Shared stream connection initialization
 *
\******************************************************************************/

static int
http_streams_https_check(struct soap *ctx, struct soap *soap, const char *endpoint)
{
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
  /* active SSL/TLS connection */
  if ((ctx->ssl_flags & SOAP_SSL_CLIENT))
    return SOAP_OK;
#else
  (void)ctx;
#endif
  /* don't allow https over existing plain http */
  if (!soap_tag_cmp(endpoint, "https:*"))
  {
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "http_streams plugin error: thread %p attempts https over http connection to %s\n", THREAD_ID, endpoint));
    return soap_set_sender_error(soap, "http_streams plugin error", "Attempt https over http stream", SOAP_SSL_ERROR);
  }
  return SOAP_OK;
}

static int
http_streams_ssl_init(struct soap *ctx, struct soap *soap, const char *endpoint)
{
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS) || defined(WITH_WOLFSSL) || defined(WITH_SYSTEMSSL)
  /* require https, if not SSL/TLS initialized yet then initialize this stream */
  if (!(ctx->ssl_flags & SOAP_SSL_CLIENT) && (soap->ssl_flags & SOAP_SSL_CLIENT) && !soap_tag_cmp(endpoint, "https:*"))
  {
    if (soap_ssl_client_context(ctx, soap->ssl_flags, soap->keyfile, soap->password, soap->cafile, soap->capath, soap->randfile))
      return ctx->error;
    if (soap_ssl_crl(ctx, soap->crlfile))
      return ctx->error;
  }
#else
  (void)ctx; (void)soap;
#endif
  return http_streams_https_check(ctx, soap, endpoint);;
}

static int
http_streams_connect_init(struct soap *ctx, struct soap *soap, const char *endpoint)
{
  int err = http_streams_ssl_init(ctx, soap, endpoint);
  if (!err)
  {
    soap_set_omode(ctx, SOAP_ENC_PLAIN);
    ctx->connect_flags = soap->connect_flags;
    ctx->connect_retry = soap->connect_retry;
    ctx->connect_timeout = soap->connect_timeout;
    ctx->send_timeout = soap->send_timeout;
    ctx->recv_timeout = soap->recv_timeout;
    ctx->transfer_timeout = soap->transfer_timeout;
    err = soap_connect_command(ctx, soap->status, endpoint, soap->action);
    soap_clr_omode(ctx, SOAP_ENC_PLAIN);
    soap->errnum = ctx->errnum;
    soap->errmode = ctx->errmode;
  }
  if (err)
  {
    soap_end(ctx);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "http_streams plugin: thread %p stream %d cannot connect error %d\n", THREAD_ID, soap_http_streams_stream_id(soap), err));
    return err;
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "http_streams plugin: thread %p stream %d lock & connected %s %d socket %d\n", THREAD_ID, soap_http_streams_stream_id(soap), ctx->host, ctx->port, ctx->socket));
  return SOAP_OK;
}

/******************************************************************************\
 *
 *      Callbacks
 *
\******************************************************************************/

static int
http_streams_connect(struct soap *soap, const char *endpoint, const char *host, int port)
{
  struct soap_http_stream_state *stream = (struct soap_http_stream_state*)soap->data[2];
  struct soap_http_shared_streams *hub = stream->hub;
  for (;;)
  {
    int i, serving = 0;
    for (i = 0; i < hub->max_host * hub->max_open; i += hub->max_open)
    {
      struct soap_http_stream_state *host_stream = &hub->streams[i];
      MUTEX_LOCK(host_stream->init);
      if (host_stream->ctx)
      {
        ++serving;
        if (strcasecmp(host, host_stream->ctx->host) == 0 && port == host_stream->ctx->port)
        {
          int k, err;
          MUTEX_UNLOCK(host_stream->init);
          for (k = 0; k < hub->max_open; ++k)
          {
            struct soap_http_stream_state *open_stream = &hub->streams[i + k];
            if (MUTEX_TRYLOCK(open_stream->lock) == 0)
            {
              if (!open_stream->ctx)
              {
                open_stream->ctx = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
                if (!open_stream->ctx)
                  break;
              }
              soap->data[2] = (void*)open_stream;
              if (!soap_valid_socket(open_stream->ctx->socket) || open_stream->ctx->keep_alive == 0)
              {
                MUTEX_LOCK(open_stream->init);
                err = http_streams_connect_init(open_stream->ctx, soap, endpoint);
                MUTEX_UNLOCK(open_stream->init);
                if (err)
                  return err;
              }
              else
              {
                err = http_streams_https_check(open_stream->ctx, soap, endpoint);
                if (err)
                  return err;
              }
              soap_copy_stream(soap, open_stream->ctx);
              open_stream->ctx->error = SOAP_OK;
              return SOAP_OK;
            }
          }
          MUTEX_LOCK(host_stream->lock);
          soap->data[2] = (void*)host_stream;
          if (!soap_valid_socket(host_stream->ctx->socket) || host_stream->ctx->keep_alive == 0)
          {
            MUTEX_LOCK(host_stream->init);
            err = http_streams_connect_init(host_stream->ctx, soap, endpoint);
            MUTEX_UNLOCK(host_stream->init);
            if (err)
              return err;
          }
          else
          {
            err = http_streams_https_check(host_stream->ctx, soap, endpoint);
            if (err)
              return err;
          }
          soap_copy_stream(soap, host_stream->ctx);
          host_stream->ctx->error = SOAP_OK;
          return SOAP_OK;
        }
      }
      MUTEX_UNLOCK(host_stream->init);
    }
    if (serving == hub->max_host)
    {
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "http_streams plugin error: need more server host slots than allocated with max_host=%d\n", hub->max_host));
      return soap_set_sender_error(soap, "http_streams plugin error", "Out of host:post server slots, increase max_host", SOAP_TCP_ERROR);
    }
    for (i = 0; i < hub->max_host * hub->max_open; i += hub->max_open)
    {
      struct soap_http_stream_state *host_stream = &hub->streams[i];
      MUTEX_LOCK(host_stream->init);
      if (host_stream->ctx)
      {
        MUTEX_UNLOCK(host_stream->init);
        if (strcasecmp(host, host_stream->ctx->host) == 0 && port == host_stream->ctx->port)
          break;
      }
      else
      {
        struct soap *ctx = soap_new1(SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
        if (ctx)
        {
          int err;
          soap->data[2] = (void*)host_stream;
          err = http_streams_connect_init(ctx, soap, endpoint);
          if (err)
          {
            soap_free(ctx);
            MUTEX_UNLOCK(host_stream->init);
            return err;
          }
          host_stream->ctx = ctx;
          MUTEX_LOCK(host_stream->lock);
          MUTEX_UNLOCK(host_stream->init);
          soap_copy_stream(soap, ctx);
          return SOAP_OK;
        }
      }
    }
  }
}

static int
http_streams_disconnect(struct soap *soap)
{
  struct soap_http_stream_state *stream = (struct soap_http_stream_state*)soap->data[2];
  int err = soap->error;
  if (soap_valid_socket(soap->socket))
  {
    MUTEX_LOCK(stream->init);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "http_streams plugin: thread %p stream %d disconnect\n", THREAD_ID, (int)(stream - stream->hub->streams)));
    soap_end(stream->ctx);
    soap_copy_stream(stream->ctx, soap);
    soap_set_mode(stream->ctx, SOAP_IO_CHUNK | SOAP_IO_KEEPALIVE);
    err = (soap->error ? soap->error : stream->ctx->error);
    MUTEX_UNLOCK(stream->init);
    soap_free_stream(soap);
    MUTEX_UNLOCK(stream->lock);
  }
  return err;
}

#ifdef __cplusplus
}
#endif
