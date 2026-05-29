/** @file idss_dbms_manager.c
 *
 * This module is in charge of managing the IDSS DBMS
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "data_access_structures.h"
#include "data_access_func.h"

extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;

void *idss_dbms_manager(void *arg)
{

int rc;
char sql[1024] = {0};

//sprintf(sql, "VACUUM rl_hos_service; VACUUM rl_hos_ws; VACUUM tb_cpu_dynamic; VACUUM tb_cpu_static; VACUUM tb_device_dynamic; VACUUM tb_device_static; VACUUM tb_fir_ports; VACUUM tb_firewall; VACUUM tb_host; VACUUM tb_job; VACUUM tb_lrms; VACUUM tb_memory_dynamic; VACUUM tb_memory_static; VACUUM tb_net; VACUUM tb_queue; VACUUM tb_service; VACUUM tb_system; VACUUM tb_web_service; VACUUM tb_ws_wsdl_loc;");

sprintf(sql, "VACUUM;");

while(1){

pthread_mutex_lock(&db_handle_mutex);
rc = exec_query(&db_handle, sql);
pthread_mutex_unlock(&db_handle_mutex);

if (rc != SQLITE_OK)
{
      if(rc == SQLITE_MEMORY_ERROR)
      {
         if (get_debug_flg () >= 1)
        	idss_errmsg_r ("Not enough memory");
	 xmlCleanupParser ();
	 pthread_mutex_lock(&db_handle_mutex);
	 close_connection(&db_handle);
	 pthread_mutex_unlock(&db_handle_mutex);
	 exit (EXIT_FAILURE);
      }
      else
      {
      	if (get_debug_flg () >= 1)
        {
		idss_errmsg_r("An error occurred trying to perform VACUUM maintenance operation");
        }
	sleep(60);
	continue;
      }
}


if (get_debug_flg () >= 1)
{
	idss_errmsg_r("VACUUM maintenance operation SUCCESFUL");
}

sleep(600);

}

}
