/** @file strtrim
 *
 * Removes (trims) leading and trailing blank characters from
 * a string
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char *strtrim(char *str2parse)
{

  int len;
  char *ptr;
  char *lastsp;

  if (!str2parse)
  {
    return NULL;
  }

  len = strlen(str2parse);

  /* check if input string is type ""  or null */
  if (!len)
  {
    return NULL;
  }

  for (ptr = str2parse; *ptr == ' '; ptr ++)
    ;

  for (lastsp = str2parse + len -1; *lastsp == ' ' && lastsp != ptr; lastsp --)
    ;

  *(lastsp+1) = 0;

  if (!strlen(ptr))
  {
    return NULL;
  }
  return ptr;
}
