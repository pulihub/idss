/** @file idss-sed
 *
 *
 *
 *  Copyright (C)
 *  University of Salento, Italy
 *
 */


//gsoap idss service name: idss
//gsoap idss service style: rpc
//gsoap idss service encoding: encoded
//gsoap idss service location: https://ubuntu:19000
//gsoap idss service namespace: urn:idss

struct arr_str
{
  int __size;
  char **__ptr;
};

typedef int   xsd__int;
typedef char  *xsd__string;
typedef float  xsd__float;

struct dimeData
{
   unsigned char *__ptr;
   int __size;
   char *id;
   char *type;
   char *options;
};

struct idss__query_submit_response
{
  xsd__string uqi;
  xsd__int result;
};

struct idss__get_file_response
{
  xsd__string result;
  xsd__int status;
};

int idss__istore(struct dimeData *data, xsd__int * result);
int idss__istore2(xsd__string file, xsd__string host, xsd__int * result);
//int idss__search (xsd__string filter, xsd__string *result);
int idss__search_sql (xsd__string filter, xsd__string *result);
int idss__getServiceDescription(void *_,xsd__string *result);
int idss__query_submit(xsd__string value, xsd__int ttl, struct idss__query_submit_response *res);
int idss__get_file(xsd__string uqi, struct idss__get_file_response *res);
int idss__query_test(xsd__string query, xsd__string *result);
