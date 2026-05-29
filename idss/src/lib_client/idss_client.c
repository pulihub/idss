/*
 * This software copyright 2022-2023
 * University of Salento, Italy
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include "idss_client.h"
#include "utils.h"

/*****************************
* Static Function Definition *
******************************/

static void add_data_element(void *** list, void* el);


/**********************
* Function Definition *
***********************/



static void add_data_element(void *** list, void* el)
{
  int count;

  if (!list)
    return;

  for (count = 0; (*list)[count]; count ++)
    ;

  *list = (void**)realloc(*list, (count + 2) * sizeof(void*));

  (*list)[count] = el;
  (*list)[count + 1] = NULL;
}

