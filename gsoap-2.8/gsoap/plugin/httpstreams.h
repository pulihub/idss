/*
	httpstreams.h

	HTTP(S) streams plug-in

        Persistent http(s) connections ("streams") to accelerate client threads.

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

#include "stdsoap2.h"
#include "threads.h"

#define HTTP_STREAMS_ID "SOAP-HTTP-STREAMS/1.0" /* plugin identification */

#ifdef __cplusplus
extern "C" {
#endif

extern const char http_streams_id[];

struct soap_http_shared_streams {
  int max_host;
  int max_open;
  struct soap_http_stream_state *streams;
  MUTEX_TYPE init;
};

struct soap_http_stream_state {
  struct soap_http_shared_streams *hub;
  struct soap *ctx;
  MUTEX_TYPE init;
  MUTEX_TYPE lock;
};

/* allocate and deallocate a shared persistent streams hub to manage your own (pass to plugin as arg) */
SOAP_FMAC1 struct soap_http_shared_streams * SOAP_FMAC2 soap_new_http_streams(int max_host, int max_open);
SOAP_FMAC1 void SOAP_FMAC2 soap_free_http_streams(struct soap_http_shared_streams *hub);

/* when using the default global static shared streams, hub you can set the parameters */
SOAP_FMAC1 void SOAP_FMAC2 soap_http_streams_max_host(int max_host); /* max number of distinct host:port servers */
SOAP_FMAC1 void SOAP_FMAC2 soap_http_streams_max_open(int max_open); /* max concurrent open socket streams per server */

/* registry */
SOAP_FMAC1 int SOAP_FMAC2 http_streams(struct soap *soap, struct soap_plugin *plugin, void *arg);

/* return stream id 0 to max_host*max_open-1 or -1 when no http stream is used */
SOAP_FMAC1 int SOAP_FMAC2 soap_http_streams_stream_id(struct soap *soap);

#ifdef __cplusplus
}
#endif

