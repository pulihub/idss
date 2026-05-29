/** @file idss_data.h
 *
 * functionalities to be used to develop the collector.
 *
 * Copyright 2022-2025, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */
#ifndef _IDSS_DATA_H
#define _IDSS_DATA_H

#include "stdsoap2.h"

#define IDSS_DEBUG_LOW 0
#define IDSS_DEBUG_PROVIDER 0
#define IDSS_DEBUG_COLLECTOR 0
//#define IDSS_DEBUG_GRELC 1
#define IDSS_DEBUG_PROVIDER_DETAILS 1
#define IDSS_DEBUG_MEDIUM 1
#define IDSS_DEBUG_HIGH 2

extern FILE* logfd;



/* the structure stores information about an iStore the host is register to */
struct host_node
{
  char              *hostname;
  char              in_flag; //if 0 the node has to be considered removed from the list
  struct host_node	*next;
};

/* the structure stores the list of iStores the host is register to */
struct _host_list
{
  int               nhost;
  struct host_node *first;
};

typedef struct _host_list * host_list;

enum _provider_type {ip_type_exe};

typedef enum _provider_type provider_type;

/* the structure stores information about an iStore the host is register to */
struct _info_provider_node
{
  char                      *name;
  provider_type             type;
  char                      *module;
  char                      *arguments;
  int                       ttl;
  pid_t						pid;
  struct _info_provider_node	*next;
};

typedef struct _info_provider_node * info_provider_node;

/* the structure stores the list of iStores the host is register to */
struct _provider_list
{
  int                       nproviders;
  info_provider_node        first;
};

typedef struct _provider_list * provider_list;

extern pthread_mutex_t log_mutex;

/************************
 * Functions Declaration
 ************************/

void print_data();

/*
  this function reads the configuration file content and initialize
  the data structures.
  the function returns:
  0. on success
  1. if the configuration file can not be opened or parsed
  2. if a node af xml structure can not be parsed
*/
int read_conf_file(struct soap *soap);

int idss_set_logfile(char* filename);

void set_conf_filename(char* name);

char* get_conf_filename(void);

void idss_data_dealloc(void);

void reset_registered_host_list(void);

void data_allocation(void);

/*
  this function returns a host. It uses parameter hnode as iterator
  for the internal host list.
  for the first invocation the parameter should be NULL, upon return
  the function will update the hnode parameter with a value that should
  be passed to get next host
*/
char * get_next_registered_host(struct host_node** hnode);

/*
  add new hostname on the top of the list.
  if the hostname already exixts it will not be added
  The function returns:
  1 if the hostnode already exists
  0 on success
*/
int add_registered_host(char *name);

void delete_registered_host(char *name);

int get_registration_ttl();

int get_numof_hidle_hosts();

int get_purge_period();

char * get_system_info_sd();

char* get_user_supplied_info_sd();

char* get_inode_info_sd();

char* get_query_tempfiles_sd();

char* get_query_results_sd();

char* get_db_name();

char* get_ig_hostname(); /* return the hostname as read from the idss_conf file */

info_provider_node get_next_info_provider_node(info_provider_node* ip_node);

/*
  add new hostname on the top of the list.
  if the hostname already exixts it will not be added
  The function returns:
  1 if the hostnode already exists
  0 on success
*/
int add_info_provider_node(info_provider_node ip_node);

void delete_info_provider_node(char *name);

info_provider_node create_info_provider_node(char* name, provider_type type, char* module, char* args, int ttl);

//char * get_next_allowed_host(struct host_node** hnode);

//int add_allowed_host(char *name);

//int is_allowed_inode (char* name);

//void delete_allowed_inode(char *name);

int get_debug_flg();
void set_debug_flg(int level);

int set_server_identity(char* id);
char* get_server_identity();

#endif

