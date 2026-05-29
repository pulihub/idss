/** @file domainname
 * Checks the FQDN specified to verify if it is correct and returns the domain.
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

/*
 * domainname checks the FQDN specified to verify if it is correct and returns 
   the domain.
 *
 */

char *domainname(char *hostname)
{
  char *str;
  
  str = strchr(hostname, '.');
  if (!str)
  {
    return NULL;
  }
  return strdup(str + 1);
}
