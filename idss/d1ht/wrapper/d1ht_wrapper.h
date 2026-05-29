/**
 * @file d1ht_wrapper.h
 *
 *
 */

#ifndef __D1HTWRAPPER_H
#define __D1HTWRAPPER_H

#include <stdbool.h>

struct d1ht_h; // An opaque type that we'll use as a handle
typedef struct d1ht_h d1ht_h;

/**
 * @brief Instantiate a new D1HT overlay.
 * @param argc
 * @param argv
 * @return d1ht_h*
 */
d1ht_h *d1ht_new(int argc, char **argv, void (*response_received)(void *args),
                 void (*query_received)(void *args));

/**
 * @brief Destroy D1HT overlay.
 * @param d1ht
 */
void d1ht_delete(d1ht_h *d1ht);

/**
 * @brief Broadcast a query message.
 */
int d1ht_broadcast_query(d1ht_h *d1ht, char *query, char *uqi, int timeout,
                         double reduction);

/**
 * @brief Send a query response.
 */
int d1ht_send_response(d1ht_h *d1ht, char *uqi, char *destination,
                       char *response);

/**
 * @brief routing table dump
 * @param d1ht
 */
void d1ht_dumpRT(d1ht_h *d1ht);

#endif /* __D1HTWRAPPER_H */
