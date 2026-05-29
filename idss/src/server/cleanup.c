/** @file cleanup.c
 * threaded server
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

/***********
* Includes *
************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "idss_data.h"
#include "idss_unix.h"
#include "utils.h"
#include "data_access_structures.h"
#include "data_access_func.h"


extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;

/***********************
* Function Declaration *
************************/

static void remove_element(void *** list, int k);
static int add_element(void *** list, void* el);


/**********************
* Function Definition *
***********************/

static int add_element(void *** list, void* el)
{
  int count;
  void **new_list = NULL;
  char errbuff[1024];

  if (get_debug_flg() >= 1)
  {
                  sprintf (errbuff,"entering add_element");
                  idss_errmsg_r(errbuff);
  }

  if (!list)
    return 1;

  if (!el)
    return 0;

  for (count = 0; (*list)[count]; count++)
    ;

  new_list = (void**)realloc(*list, (count+2) * sizeof(void*));

  if(new_list){
  	*list = new_list;
  	(*list)[count] = el;
  	(*list)[count + 1] = NULL;
  }
  else {
  	return -1;

  }

  if (get_debug_flg() >= 1)
  {
                  sprintf (errbuff,"exiting add_element");
                  idss_errmsg_r(errbuff);
  }
  return 0;

}

static void remove_element(void *** list, int k)
{
  int count;
  void **membuf;
  char errbuff[1024];

  if (get_debug_flg() >= 1)
  {
                  sprintf (errbuff,"entering remove_element");
                  idss_errmsg_r(errbuff);
  }

  if (!list){
  	if (get_debug_flg() >= 1)
	{
                  sprintf (errbuff,"exiting remove_element");
                  idss_errmsg_r(errbuff);
	}
    	return;
  }

  for (count = 0; (*list)[count]; count ++)
    ;

  if (k >= count || k < 0)
    return;

  membuf = (void **)malloc( count * sizeof(void*));
  if(!membuf){
  	if (get_debug_flg() >= 1)
	{
                  sprintf (errbuff,"Not enough memory, now exiting iDSS");
                  idss_errmsg_r(errbuff);
                  sprintf (errbuff,"exiting remove_element");
                  idss_errmsg_r(errbuff);
	}
 	idss_clean_up();
	pthread_mutex_lock(&db_handle_mutex);
	close_connection(&db_handle);
	pthread_mutex_unlock(&db_handle_mutex);
	exit(EXIT_FAILURE);
  }

  memcpy(membuf, *list, k * sizeof(void*));
  memcpy(&membuf[k], &((*list)[k+1]), (count - k) * sizeof(void*));

  free(*list);

  *list = membuf;

  	if (get_debug_flg() >= 1)
	{
                  sprintf (errbuff,"exiting remove_element");
                  idss_errmsg_r(errbuff);
	}
}
