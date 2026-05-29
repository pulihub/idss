/** 
 * @file d1ht_wrapper.cpp
 * 
 */

#include "d1ht_h.h"
#include <stdbool.h>
#include <stdio.h>

extern "C" {
#include "d1ht_wrapper.h"
}

d1ht_h *d1ht_new(int argc, char **argv, void (*response_received)(void *args),
                 void (*query_received)(void *args)) {
  return reinterpret_cast<d1ht_h *>(
      new d1ht::d1ht_t(argc, argv, response_received, query_received));
}

void d1ht_delete(d1ht_h *d1ht) {
  delete reinterpret_cast<d1ht::d1ht_t *>(d1ht);
}

void d1ht_dumpRT(d1ht_h *d1ht) {
  if (d1ht == NULL) {
    return;
  }
  //invocata solo dal demo
  printf("\nD1HT routing table:\n");
  d1ht::G_RT.pub_print_table(stdout);
  printf("\n\n");
}

int d1ht_broadcast_query(d1ht_h *d1ht, char *query, char *uqi, int timeout,
                         double reduction) {

  if (NULL == d1ht) {
    return -1;
  }

  return d1ht::G_QM.broadcast_query(query, uqi, timeout, reduction);
}

int d1ht_send_response(d1ht_h *d1ht, char *uqi, char *destination,
                       char *response) {
  if (NULL == d1ht) {
    return -1;
  }

  return d1ht::G_QM.send_query_reply(uqi, destination, response);
}
