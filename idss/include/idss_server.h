/** @file idss_server.h
 *
 *
 * Copyright 2022-2025 University of Salento, Italy.
 * All rights reserved.
 *
 *
 *
 */




/* Streaming DIME attachment content handlers */

void *dime_read_open(struct soap*, void*, const char*, const char*, const char*);
void dime_read_close(struct soap*, void*);
size_t dime_read(struct soap*, void*, char*, size_t);
void *dime_write_open(struct soap*, const char*, const char*, const char*);
void dime_write_close(struct soap*, void*);
int dime_write(struct soap*, void*, const char*, size_t);

/* COLLECTOR */

/* periodically calls the collect function */
//void* collect_main (void* myarg);

/* collect collects information to be stored in the DB into an xml file */
//void collect(struct soap *soap);


/* PROVIDER MANAGER */

void *idss_dbms_manager(void *);
int* provider_thr (void* ip_data);
void provider_manager();


/* CLEANUP FUNCTIONS */

int system_cleanup();
