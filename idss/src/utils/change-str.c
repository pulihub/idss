/** @file change_str
 * Codes and decodes the characters of a string.
 *
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define CODLEN 3


/*
 * code_str codes the ~_&?%{}|\\^[]`;@=$+!\' characters with the '?' character
 * followed by the related ascii code and removes all other invalid characters
 * for MDS
 *
 */

char *code_str (char **text_src)
{
  int          istnum;
  int          i;
  int          numtokens;
  unsigned int ccod;
  size_t       len;
  char         format[10];
  char         *sep_src;
  char         *sep_dest;
  char         chrcod[CODLEN + 1];
  char         *textde;
  char         **tok;
  char         *text;
  static char  del[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                       16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                       29, 30, 31, 35, 60, 62, 127, 128, 129, 130, 131, 132,
                       133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
                       144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
                       155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                       166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
                       177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                       188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                       199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
                       210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
                       221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
                       232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
                       243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
                       254, 255, 0 };
  char         delim[] = "~_&?%{}|\\^[]`;@=$+!\'";

  if(!*text_src)
    return NULL;

  istnum = 0;
  sprintf(format, "?%c0%dX", '%', CODLEN - 1);
  numtokens = tokenize_string_r(*text_src, del, &tok);
  text = (char *) calloc (strlen(*text_src) - numtokens + 2, sizeof(char));
  if(!text)
    return NULL;

  for(i = 0; i < numtokens; i++)
  {
    strcat(text, *(tok + i));
    free(*(tok + i));
  }
  sep_src = text;
  while (sep_src)
  {
    sep_src = strpbrk(sep_src, delim);
    if (sep_src)
    {
      istnum++;
      sep_src ++;
    }
  }
  sep_dest = (char *) calloc (strlen(text) + (CODLEN - 1) * istnum + 1,
          sizeof(char));
  textde = sep_dest;
  sep_src = text;
  while (*sep_src)
  {
    len = strcspn(sep_src, delim);
    memcpy(sep_dest, sep_src, len);
    sep_dest += len;
    sep_src += len;
    if(*sep_src)
    {
      ccod = (unsigned int) *sep_src;
      sprintf(chrcod, format, ccod);
      memcpy(sep_dest, chrcod, CODLEN);
      sep_dest += CODLEN;
      sep_src++;
    }
  }
  free(text);
  *text_src = textde;
  return *text_src;
}

/*
 * decode_str codes the '?' character followed  by the related ascii code
 * with "& _ ~ ?" characters
 *
 */

char *decode_str (char **text_src)
{
  int          istnum;
  //int          i;
  int          buff_len;
  size_t       len;
  char         *sep_src;
  char         *sep_dest;
  char         *textde;
  //char         *text_dest;
  char         del[] = "?";
  char         codec[CODLEN + 2];
  unsigned int ccode;

  if(*text_src)
    return NULL;

  istnum = 0;
  strcpy(codec,"0x");
  sep_src = *text_src;
  while (sep_src)
  {
    sep_src = strpbrk(sep_src, del);
    if (sep_src)
    {
      istnum++;
      sep_src ++;
    }
  }
  buff_len = strlen(*text_src) - (CODLEN - 1) * istnum + 1;
  sep_dest = (char *) calloc (buff_len, sizeof(char));
  textde = sep_dest;
  sep_src = *text_src;
  while (*sep_src)
  {
    len = strcspn(sep_src, del);
    memcpy(sep_dest, sep_src, len);
    sep_dest += len;
    sep_src += len;
    if(*sep_src)
    {
      memcpy(codec + 2, sep_src + 1, CODLEN - 1);
      codec[CODLEN + 1] = 0;
      sscanf(codec, "%x", &ccode);
      *sep_dest = (char)ccode;
      sep_dest ++;
      sep_src += CODLEN;
    }
  }
  *text_src = textde;
  return *text_src;
}
