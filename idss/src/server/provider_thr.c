/** @file provider_thr.c
 *
 * This module is in charge of managing system information providers.
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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include "utils.h"
#include "idss_data.h"
#include "data_access_structures.h"
#include "data_access_func.h"

#define STRLEN 512
#define MAX_REPETITIONS 5

extern pthread_mutex_t service_mutex;

extern pthread_mutex_t db_handle_mutex;
extern sqlite_handle db_handle;


int* provider_thr (void* arg)
{
  char vt[64], fl[STRLEN], db[STRLEN], hn[STRLEN];
  char *cmd = NULL;
  char *args = NULL;
  char *name = NULL;
  int l, rc;
  char *xmlresult = NULL;
  char **cmd_args = NULL;
  FILE *pipe = NULL;
  pid_t provider_pid = -1;
  char filename[STRLEN] = {'\0'};
  char tmp_fn[STRLEN] = {'\0'};
  info_provider_node ip_data;
  int unslept;
  char errbuf[4096] = {'\0'};
  sigset_t set, oldset;
  struct sigaction act;
  ptr_mng ptr_handle = NULL;

  static int launched = 0;

  ptr_register(&ptr_handle, (void**)(&cmd), 0);
  ptr_register(&ptr_handle, (void**)(&args), 0);
  ptr_register(&ptr_handle, (void**)(&xmlresult), 0);
  ptr_register(&ptr_handle, (void**)(&cmd_args), 1);


  if (get_debug_flg() >= 1)
                idss_errmsg_r("Entering Provider_thr\n");

  pthread_detach(pthread_self());

  ip_data = (info_provider_node) arg;

   /* just in this thread we need to change the SIGCHLD disposition we have inherited */

   /* we begin by blocking ALL of the signals */
   rc = sigfillset(&set);
   if(rc != 0){
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
   }

   /* set temporary thread signal mask and retrieve old signal mask */
   rc = pthread_sigmask(SIG_SETMASK, &set, &oldset);
   if(rc != 0){
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
   }

   /* initialize the struct sigaction act */
   memset(&act, 0, sizeof(act));
   rc = sigfillset(&act.sa_mask);
   if(rc != 0){
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
   }

   act.sa_handler = SIG_DFL;
   rc = sigaction(SIGCHLD, &act, NULL);
   if(rc != 0){
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
   }

   /* set the thread final signal mask */
   rc = pthread_sigmask(SIG_SETMASK, &oldset, NULL);
   if(rc != 0){
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
   }

  if (!ip_data->module)
  {
    if (get_debug_flg() >= 1){
      sprintf(errbuf, "Invalid Module %s Information Provider\n", ip_data->name);
      idss_errmsg_r(errbuf);
    }
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
	/* wrong configuration file. No provider specified */
  }
  /* figure out of the command line length */
  l = strlen(ip_data->module) + 1; /* executable name */

  sprintf(vt,"-v %d ",ip_data->ttl); /* validity time */
  l += strlen(vt);

  name = strrchr(ip_data->module,'/');
  if (!name || *name == 0)
    name = ip_data->module;
  else
    name ++;

  /* filename variable will also be used further in the function */
  sprintf(filename, "%s/%s.info", get_system_info_sd(), name);
  sprintf(tmp_fn, "%s/.%s.info", get_system_info_sd(), name);

  if (strlen(tmp_fn) + 4 >= STRLEN)
  {
    if (get_debug_flg() >= 1){
      sprintf(errbuf, "Filename <%s> too long. Size limit: %d\n", tmp_fn, STRLEN);
      idss_errmsg_r(errbuf);
    }
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
	/* wrong configuration file. No provider specified */
  }

  sprintf(fl, "-o %s ", tmp_fn); /* output filename */
  l += strlen(fl);

  if (strlen(get_db_name()) + 4 >= STRLEN)
  {
    if (get_debug_flg() >= 1){
      sprintf(errbuf, "DB name <%s> too long. Size limit: %d\n", get_db_name(), STRLEN);
      idss_errmsg_r(errbuf);
    }
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
	/* wrong configuration file. No provider specified */
  }
  sprintf(db,"-s %s ",get_db_name()); /* db name filename */
  l += strlen(db);

  if (get_ig_hostname())
  {
    if (strlen(get_ig_hostname()) + 4 >= STRLEN)
    {
      if (get_debug_flg() >= 1){
        sprintf(errbuf, "Hostname <%s> too long. Size limit: %d\n", get_ig_hostname(), STRLEN);
	idss_errmsg_r(errbuf);
      }
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
	/* wrong configuration file. No provider specified */
    }
    sprintf(hn,"-h %s ",get_ig_hostname()); /* hostname */
  }
  else
    sprintf(hn,"-h %s ",IDSS_SERVICE_HOSTNAME); /* hostname */

  l += strlen(hn);

  if (ip_data->arguments){
    	args = strdup(ip_data->arguments);
    	if(!args){
    		if (get_debug_flg() >= 1){
      			sprintf(errbuf, "Out of memory\n");
      			idss_errmsg_r(errbuf);
                	idss_errmsg_r("Exiting Provider_thr\n");
    		}
		pthread_exit(NULL);
    	}
  }
  else{
    	args = strdup("");
	if(!args){
    		if (get_debug_flg() >= 1){
      			sprintf(errbuf, "Out of memory\n");
      			idss_errmsg_r(errbuf);
                	idss_errmsg_r("Exiting Provider_thr\n");
    		}
		pthread_exit(NULL);
	}
  }

  l += strlen(args);

  cmd = (char*) malloc((l + 1) * sizeof(char));
  if (!cmd)
  {
    if (get_debug_flg() >= 1)
      idss_errmsg_r("Error! Not enough memory\n");
    ptr_destroy(ptr_handle);
  	if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting Provider_thr\n");
	pthread_exit(NULL);
  }

  sprintf(cmd, "%s %s%s%s%s%s", ip_data->module, vt, fl, db, hn, args);
  free(args);
  args = NULL;

  tokenize_string_r(cmd, " ", &cmd_args);


  /* main loop */

  while(1)
  {
    if(get_debug_flg() >= 1)
    {
      if (get_debug_flg() >= 1){
        sprintf(errbuf, "Provider '%s': executing\n%s\n", ip_data->name, cmd);
	idss_errmsg_r(errbuf);
      }
    }

    if ((provider_pid = fork()) < 0)
    {

	  launched++;
	  if(launched <= MAX_REPETITIONS){
	 	sleep(60);
      		if (get_debug_flg() >= 1){
        		idss_errmsg_r("Fork Error\n");
        		sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
			idss_errmsg_r(errbuf);
	  	}
	  	continue; /* start again, always try to execute the provider */
	  }
	  else{
		if (get_debug_flg() >= 1)
        		idss_errmsg_r("Fork Error\n");

      		ptr_destroy(ptr_handle);

	  	if (get_debug_flg() >= 1)
                	idss_errmsg_r("Exiting Provider_thr\n");
      		pthread_exit(NULL);
	  }
    }

    if (!provider_pid) /* Child process */
    {


	if (execvp(cmd_args[0], cmd_args) == -1){
       		if (get_debug_flg() >= 1){
       			sprintf(errbuf,"Execvp error for command: %s\n", cmd);
       			idss_errmsg_r(errbuf);
       			sprintf(errbuf,"Killing this process\n");
       			idss_errmsg_r(errbuf);
		}

		/* cleanup memory in this process */
        	ptr_destroy(ptr_handle);

		/* the process will now exit sending to itself  SIGKILL */
		//kill(getpid(), SIGKILL);
		raise(SIGKILL);

        }


    }
    else //parent process
    {
      int status;
      int byteread;
      struct stat ss;

      ip_data->pid = provider_pid;

      waitpid(provider_pid, &status, 0);
      ip_data->pid = -1;

      /* check if the forked process exited due to the receipt of a signal */
      /* if the signal is SIGKILL, then the process was not able to exec the provider */
      /* A status of 0 indicates normal termination */
      if(status) /* the process did not terminate correctly */
      	if (WIFSIGNALED(status))
      	{

		if(WTERMSIG(status) == SIGKILL){
        		if (get_debug_flg() >= 1){
	  			sprintf(errbuf, "Error! Provider '%s' exited with status %d: Execvp Error in child process\n", ip_data->name, status);
          			idss_errmsg_r(errbuf);
			}

			provider_pid = -1;

	  		launched++;
	  		if(launched <= MAX_REPETITIONS){
	 			sleep(60);
      				if (get_debug_flg() >= 1){
        				sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
					idss_errmsg_r(errbuf);
	  			}
	  			continue; /* start again, always try to execute the provider */
	  		}
	  		else{

      				ptr_destroy(ptr_handle);

	  			if (get_debug_flg() >= 1)
                			idss_errmsg_r("Exiting Provider_thr\n");

      				pthread_exit(NULL);
	  		}
		}

      	}

      if (stat(tmp_fn, &ss))
        continue;

      if ((xmlresult = (char*)malloc(ss.st_size + 1)) == NULL)
      {
  	if (get_debug_flg() >= 1)
          	idss_errmsg_r("Error! Not enough memory\n");
        	ptr_destroy(ptr_handle);
  	    	if (get_debug_flg() >= 1)
                	idss_errmsg_r("Exiting Provider_thr\n");
	    	pthread_exit(NULL);
     	}

     pipe = fopen(tmp_fn, "r");
     if (!pipe)
     {
       if (get_debug_flg() >= 1) {
         sprintf(errbuf, "Error! Can not open file %s\n", tmp_fn);
         idss_errmsg_r(errbuf);
       }


	provider_pid = -1;

	launched++;
	if(launched <= MAX_REPETITIONS){
		sleep(60);
      		if (get_debug_flg() >= 1){
       			sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
			idss_errmsg_r(errbuf);
		}
		continue; /* start again, always try to execute the provider */
	}
	else{

		ptr_destroy(ptr_handle);

		if (get_debug_flg() >= 1)
       			idss_errmsg_r("Exiting Provider_thr\n");

      		pthread_exit(NULL);
	}

     }

     byteread = fread(xmlresult, 1, ss.st_size, pipe);
     if (byteread != ss.st_size)
     {
          sprintf(errbuf, "Error reading file %s\n", tmp_fn);
          idss_errmsg_r(errbuf);
          fclose(pipe);
	  provider_pid = -1;

	launched++;
	if(launched <= MAX_REPETITIONS){
		sleep(60);
      		if (get_debug_flg() >= 1){
       			sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
			idss_errmsg_r(errbuf);
		}
		continue; /* start again, always try to execute the provider */
	}
	else{

		ptr_destroy(ptr_handle);

		if (get_debug_flg() >= 1)
       			idss_errmsg_r("Exiting Provider_thr\n");

      		pthread_exit(NULL);
	}



     }

     fclose(pipe);

     if (rename (tmp_fn, filename))
     {
       if (get_debug_flg() >= 1) {
         sprintf(errbuf, "Error renaming file from %s to %s\n", tmp_fn, filename);
	 idss_errmsg_r(errbuf);
       }

	provider_pid = -1;

	launched++;
	if(launched <= MAX_REPETITIONS){
		sleep(60);
      		if (get_debug_flg() >= 1){
       			sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
			idss_errmsg_r(errbuf);
		}
		continue; /* start again, always try to execute the provider */
	}
	else{

		ptr_destroy(ptr_handle);

		if (get_debug_flg() >= 1)
       			idss_errmsg_r("Exiting Provider_thr\n");

      		pthread_exit(NULL);
	}

     }

     xmlresult[byteread] = 0;

     if(!strcmp(ip_data->name, "SERVICE"))
      pthread_mutex_lock(&service_mutex);

     pthread_mutex_lock(&db_handle_mutex);
     rc = get_sql (xmlresult, &db_handle, 0);
     pthread_mutex_unlock(&db_handle_mutex);

     if (rc != SQLITE_OK)
     {
  	   if (get_debug_flg() >= 1){
	     sprintf(errbuf, "Error! Submitting query to DB %s for provider '%s':\nQuery: \n", db_handle.db_name, ip_data->name);
	     idss_errmsg_r(errbuf);
	   }

	   if(!strcmp(ip_data->name, "SERVICE"))
		  pthread_mutex_unlock(&service_mutex);


	   provider_pid = -1;


	launched++;
	if(launched <= MAX_REPETITIONS){
		sleep(60);
      		if (get_debug_flg() >= 1){
       			sprintf(errbuf, "Starting again Provider '%s'\n", ip_data->name);
			idss_errmsg_r(errbuf);
		}
		continue; /* start again, always try to execute the provider */
	}
	else{

		ptr_destroy(ptr_handle);

		if (get_debug_flg() >= 1)
       			idss_errmsg_r("Exiting Provider_thr\n");

      		pthread_exit(NULL);
	}




     }

	if(!strcmp(ip_data->name, "SERVICE"))
		pthread_mutex_unlock(&service_mutex);



        free(xmlresult);
		xmlresult = NULL;


     /* this is the proper way of sleeping
      * in a process or thread that can be
      * interrupted by a signal
      */

     unslept = ip_data->ttl;
     while(unslept)
     	unslept = sleep (unslept);

   }
 } //end while
}

void provider_manager()
{
  int res;
  pthread_t tid;
  info_provider_node ip_node, iterator = NULL;
  char errbuf[1024] = {'\0'};

 if (get_debug_flg() >= 1)
                idss_errmsg_r("Entering provider_manager\n");

  for (ip_node = get_next_info_provider_node(&iterator); ip_node; ip_node = get_next_info_provider_node(&iterator))
  {
    res = pthread_create (&tid, NULL, (void*(*)(void*))&provider_thr, (void *) ip_node);
	if (res == EAGAIN) {
  	   if (get_debug_flg() >= 1)
	     idss_errmsg_r("pthread_create failed: maximum threads in a process\n");
	}
	if (res == EINVAL) {
  	   if (get_debug_flg() >= 1)
	     idss_errmsg_r("pthread_create failed: The value specified by attr is invalid");
	}

    if (get_debug_flg() >= 1)
	{
	  sprintf(errbuf, "Thread for information provider created: Thread ID=%ld name=%s ttl=%d\n", (long)tid, ip_node->name, ip_node->ttl);
	  idss_errmsg_r(errbuf);
	}
  }

 if (get_debug_flg() >= 1)
                idss_errmsg_r("Exiting provider_manager\n");

}

