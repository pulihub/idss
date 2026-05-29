/** 
 * @file query_defs.h
 * 
 */
#ifndef __QUERYDEFS_H
#define __QUERYDEFS_H

/**
 * @brief Arguments for "query received" callback.
 */
typedef struct query_received_args {
  char *sql;
  char *uqi;
  double ttl;
  long origin_key;
  char *sender;
} query_received_args;

/**
 * @brief Arguments for "response received" callback.
 */
typedef struct response_received_args {
  char *sender;
  char *uqi;
  int subq_n;
  char *msg;
} response_received_args;

#endif /* __QUERYDEFS_H */
