/*
 * This software copyright 2022-2023
 * University of Salento, Italy
 *
 * Multi free: frees multiple arguments
 *
 *
 *
 */


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stdsoap2.h"
#include "utils.h"

void mfree (int count, ...)
{
  va_list ap;
  char* ptr;
  int i;

  va_start(ap, count);
  if (!count)
  {
    while ((ptr = va_arg(ap, char*)))
    {
      free(ptr);
    }
  }
  else
  {
    for (i = 0; i < count; i++)
    {
      ptr = va_arg(ap, char*);
      if (ptr) free(ptr);
    }
  }
  va_end(ap);
}

//void ptr_destroy (ptr_mng reg)
void ptr_destroy_dbg (ptr_mng reg, char* file, int line)
{
  int i, j;
  
  if (!reg)
    return;

  for (i = 0; i < reg->len; i++) {
    if (*(reg->ptr[i])){ 
      
	  //single free to dealloc a string
	  if (reg->type[i] == 0){ 
		free(*(reg->ptr[i]));
		*(reg->ptr[i]) = NULL;
	  }

      //multiple free to dealloc an array null termined of string
	  if (reg->type[i] == 1){ 
	    for (j = 0; (*(reg->ptr[i]))[j]; j++) 
	      free((*(reg->ptr[i]))[j]);
	    free(*(reg->ptr[i]));
	    *(reg->ptr[i]) = NULL;
	  }	

      //closing file
	  if (reg->type[i] == 2){ 
	    fclose((FILE*)*(reg->ptr[i]));
	    *(reg->ptr[i]) = NULL;
	  }	

      //destroying and closing soap structure
	  if (reg->type[i] == 3){ 
            soap_end ((struct soap *)*(reg->ptr[i]));
	    soap_destroy ((struct soap *)*(reg->ptr[i]));
            soap_done ((struct soap *)*(reg->ptr[i]));
            free (*(reg->ptr[i]));
	    *(reg->ptr[i]) = NULL;
	  }
  	  
	}
	 //Grelc unbind 
	/*if (reg->type[i] == 4){ 
	      grelc_std_access_clear_result((Grelc_Handle *)(reg->ptr[i]));
		grelc_std_access_unbind((Grelc_Handle *)(reg->ptr[i]));
		init_grelc_handle((Grelc_Handle *)(reg->ptr[i]));
	  }	*/
	  
	  //destroying Grelc_Answer
	 /* if (reg->type[i] == 5){ 
		grelc_free_data((Grelc_Answer *)reg->ptr[i]);
		grelc_init_data((Grelc_Answer *)reg->ptr[i]);
	  }*/

	  if (reg->type[i] == 6){ 
                pthread_mutex_unlock ((pthread_mutex_t *)reg->ptr[i]);
	  }
	  
  }
  free (reg->ptr);
  free (reg->type);
  free(reg);
}

int ptr_register (ptr_mng* reg, void ** ptr, int type)
{
  void ****tmp;
  int *tmp_i;
  
  if (!(*reg)){
    *reg = (ptr_mng) calloc (1, sizeof(struct _ptr_mng));
	if (!*reg)
	   return -1;

	(*reg)->len = 0;
	(*reg)->ptr = (void****)calloc (1, sizeof(void*));
    if (!(*reg)->ptr)
      return -1;
	(*reg)->type = (int*)calloc(1, sizeof(int));
    if (!(*reg)->type)
      return -1;
  }

  (*reg)->len++;
  tmp = (void****) realloc((*reg)->ptr, (*reg)->len * sizeof(void*));
  if (!tmp)
	return -1;	
  (*reg)->ptr = tmp;
    
  tmp_i = (int*) realloc((*reg)->type, (*reg)->len * sizeof(int));
  if (!tmp_i)
	return -1;	
  (*reg)->type = tmp_i;
  
  (*reg)->ptr[(*reg)->len - 1] = (void***) ptr;
  
  (*reg)->type[(*reg)->len - 1] = type;

  return 0;  
}
