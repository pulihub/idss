/*
	httpstreamstest.h

	Interfece header definisions for soapcpp2 for httpstreamstest demo
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

/******************************************************************************/
/*      calc service                                                          */
/******************************************************************************/

//gsoap ns service name:	calc Simple calculator service described at https://www.genivia.com/dev.html
//gsoap ns service protocol:	SOAP
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://websrv.cs.fsu.edu/~engelen/calc.wsdl
//gsoap ns service location:	http://localhost:8080

//gsoap ns schema namespace:	urn:calc

//gsoap ns service method: add Sums two values
int ns__add(double a, double b, double *result);

//gsoap ns service method: sub Subtracts two values
int ns__sub(double a, double b, double *result);

//gsoap ns service method: mul Multiplies two values
int ns__mul(double a, double b, double *result);

//gsoap ns service method: div Divides two values
int ns__div(double a, double b, double *result);

//gsoap ns service method: pow Raises a to b
int ns__pow(double a, double b, double *result);

/******************************************************************************/
/*      SOAP/XML with MTOM service                                            */
/******************************************************************************/

// #import "soap12.h"
#import "xop.h"
#import "xmime5.h"

//gsoap m service name:		mtom_test
//gsoap m service namespace:	http://www.genivia.com/wsdl/mtom_test.wsdl

//gsoap x schema namespace:	http://www.genivia.com/schemas/mtom_test.xsd
//gsoap x schema elementForm:	qualified

struct xsd__base64Binary {
  unsigned char *__ptr;
  int __size;
};

//gsoap x schema type-documentation: DataType a union of an MIME attachment or a base64 binary data type
struct x__DataType {
  int __union;
  union x__data {
    _xop__Include xop__Include;
    struct xsd__base64Binary base64;
  } choice;
  @char *xmime5__contentType;
};

//gsoap x schema type-documentation: WrapperType wraps a sequence of data elements with MIME attachments or base64 binary data
struct x__WrapperType {
  int __size;
  struct x__DataType *Data;
};

/* m:EchoTestSingle has a single in/out attachment of text/xml */
//gsoap m service method-input-mime-type: EchoTestSingle text/xml
//gsoap m service method-output-mime-type: EchoTestSingle text/xml

//gsoap m service method-documentation: EchoTestSingle echo a single MIME attachment or base64 binary data element
int m__EchoTestSingle(
  struct x__DataType *x__Data,
  struct m__EchoTestSingleResponse {
    struct x__DataType *x__Data;
  } *
);

/* m:EchoTestSingle has at least two in/out attachments of any MIME type */
//gsoap m service method-mime-type: EchoTestMultiple */*
//gsoap m service method-mime-type: EchoTestMultiple */*

//gsoap m service method-documentation: EchoTestMultiple echo a sequence of MIME attachments or base64 binary data elements
int m__EchoTestMultiple(
  struct x__WrapperType *x__EchoTest,
  struct m__EchoTestMultipleResponse {
    struct x__WrapperType *x__EchoTest;
  } *
);

/******************************************************************************/
/*      JSON REST service                                                     */
/******************************************************************************/

#import "xml-rpc.h" /* XML-RPC and JSON data definitions */

/******************************************************************************/
