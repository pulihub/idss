/*
 * This software copyright 2022-2025
 * University of Salento, Italy
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "idss_client.h"


void save_dime_data(char *xml_buf, char *name)
{
  //char *buf = (char*)data->__ptr;
  int len = strlen(xml_buf);
  FILE *fd = fopen(name, "wb");
  if (!fd)
  { fprintf(stderr, "Cannot save file %s\n", name);
    return;
  }
  while (len)
  { size_t nwritten = fwrite(xml_buf, 1, len, fd);
    if (!nwritten)
    { fprintf(stderr, "Cannot write to %s\n", name);
      return;
    }
    len -= nwritten;
    xml_buf += nwritten;
  }
  fclose(fd);
}










