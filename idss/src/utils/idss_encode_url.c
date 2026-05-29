/*
 * ! fn char *idss_encode_url(const char *url)
 *
 * \brief this function encode URLs (RFC 1738)
 *
 * \param const char *url: The URL to be encoded
 * Returns coded URL
 *
 *
 */

#include <string.h>

char *idss_encode_url(const char *url)
{
  char ptr[769];
  const char *t;
  int j = 0;
  char *h = "0123456789abcdef";
  char *out = NULL;
  int i = 0;
  ptr[0]= '\0';
  
  for (t = url; *t; t++)
  {
    j = (int) (unsigned char) *t;
    if(('a' <= j && j <= 'z') 
       || ('A' <= j && j <= 'Z') 
       || ('0' <= j && j <= '9') 
       || j == '-' || j == '_' || j == '.' || j == '$' || j == '+')
    {
      ptr[i] = *t;
    }
    else if (*t == 'Ã' || *t == 'Ã' || *t == 'â' || *t == 'Ã' || *t == 'Ë' || *t == 'Ã' || *t == 'â' || *t == 'Ã')
    {
      return NULL;
    }
    else
    {
      ptr[i] = '%';
      ptr[(i += 1)] = h[j >> 4];
      ptr[(i += 1)] = h[j & 0x0f];
    }
    i++;
  }
  ptr[i] = '\0';
  return out = strdup(ptr);
}

