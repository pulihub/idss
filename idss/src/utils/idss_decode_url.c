/* Evaluating URL encodings
 * The RFC 1738 defines the synatx for URLs. Section 2.2 of that document also
 * defines the rules for encoding characters in a URL. While some characters must
 * always be encoded, any character my be encoded. To encode a character, you must use
 * its hexadecimal digits: for instance, the character A has the value 0x41 and when
 * encoded in a URL it becomes %41
 * You need to decode an URL before using it; the problem is that you must make
 * certain that you never decode a URL that has already been decoded, or you will be vulnerable to double * encoding attacks. Suppose that the URL contains the sequence %25%34%31. Decoded once the result is %41 * because  %25 is the encoding for the percent symbol, %34 is the encoding for the number 4, and %31 is
 * the encoding for the number 1. Decoded twice, the result is A.
 * What happens if you decode repeatedly until there are no more escaped characters?
 * You will end up with certain sequences of characters that are impossible to represent.
 * The purpose of encoding in the first place is to allow the use of characters that have special
 * meaning or that can not be represented visually.
 * Another problem is that a NULL terminator can be encoded anywhere in the URL. There are several
 * approaches to dealing with this problem. One is to treat the decoded string as a binary array rather
 * than as a C string. Another is the use of the SafeStr library because it gives no special
 * significance to any one character.
 *
 * The function idss_decode_url() can be used to decode URLs.
 * input parameters:
 *
 * const char *url The URL to be decoded
 * size_t *nbytes  On return, this parameter contains the number of bytes of the decoded URL
 *
 * returns: the decoded URL, and the number of bytes of the decoded URL in the nbytes parameter
 *
 * The result will be NULL terminated but it may contain embedded NULLs as well.
 * You can determine whether it contains embedded NULLs by comparing the number of bytes
 * the function returns in the nbytes parameter with the result of calling strlen() on the
 * decoded URL. If the URL conatins embedded NULLs, the result from strlen() will be less
 * than the number of bytes returned by idss_decode_url() in the nbytes parameter.
 *
 */

#include "utils.h"
#include <string.h>
#include <ctype.h>

char *idss_decode_url(const char *url, size_t *nbytes)
{
  char       *out, *ptr;
  const char *c;

  if (!(out = ptr = strdup(url))) return 0;
  for (c = url;  *c;  c++) {
#ifdef TRU64
    if (*c != '%' || !isxdigit((int)c[1]) || !isxdigit((int)c[2])) *ptr++ = *c;
#else
    if (*c != '%' || !isxdigit(c[1]) || !isxdigit(c[2])) *ptr++ = *c;
#endif
    else {
      *ptr++ = (BASE16_TO_10(c[1]) * 16) + (BASE16_TO_10(c[2]));
      c += 2;
    }
  }
  *ptr = 0;
  if (nbytes) *nbytes = (ptr - out); /* does not include null byte */
  return out;
}

