/* Validating email addresses
 * RFC 822 defines the syntax for email addresses. 
 * The function idss_email_isvalid() checks the format of an email address.
 * It performs only a syntactical check (no attempt is made to verify the
 * authenticity of the address by delivering mail or by performing a DNS lookup
 * on the domain name portion of the address.
 * The function only validates an actual email address, and will not accept any associated data.
 * For example, it will fail to validate "Bob Bobson <bob@bobson.com>", but it will succesfully
 * validate "<bob@bobson.com>".
 *
 * input parameters:
 *
 * const char *address The address to be validated
 *
 * returns: 1 if the address is syntactically valid, 0 otherwise
 *
 * keep in mind that almost any character is legal in an email address if it is properly quoted.
 * So, if you are passing an email address to something that may be sensitive to certain characters
 * or character sequences (such as a command shell), you must be sure to properly escape those characters.
 *
 *
 */ 

#include <string.h>

int idss_email_isvalid(const char *address)
{
  int        count = 0;
  const char *c, *domain;
  static char *rfc822_specials = "()<>@,;:\\\"[]";

  /* first we validate the name portion (name@domain) */
  for (c = address;  *c;  c++) {
    if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
      while (*++c) {
        if (*c == '\"') break;
        if (*c == '\\' && (*++c == ' ')) continue;
        if (*c <= ' ' || *c >= 127) return 0;
      }
      if (!*c++) return 0;
      if (*c == '@') break;
      if (*c != '.') return 0;
      continue;
    }
    if (*c == '@') break;
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  }
  if (c == address || *(c - 1) == '.') return 0;

  /* next we validate the domain portion (name@domain) */
  if (!*(domain = ++c)) return 0;
  do {
    if (*c == '.') {
      if (c == domain || *(c - 1) == '.') return 0;
      count++;
    }
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  } while (*++c);

  return (count >= 1);
}

