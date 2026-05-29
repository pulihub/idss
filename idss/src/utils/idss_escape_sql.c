/* Escaping SQL SELECT statement
 * This is done before submitting a query string to the iDSS parser
 * which prevents SQL injection attacks; after escaping the input string
 * we parse it to make sure the string is indeed a valid SELECT statement.
 * 
 * input parameters
 *
 *  const char *input	This is the input query string
 * 
 *  char quote		This is the quoting character to be used:
 *			it must be either a single or double quote.
			For iDSS we use the single quote.
 *
 *  int wildcards	This is a boolean flag, indicating if wildcards
 *			(such as percent symbol, underscore and square brackets)
 * 			recognized by the LIKE operator in a WHERE clause must be
 *			escaped or not: passing 0 will NOT escape these characters.
 * 			Passing 1 WILL escape the wildcards. 
 * 			In general, wildcards should be escaped only when the input string
 *			is going to be used as the right hand side for the LIKE operator.
 *			In iDSS there is no need to escape wildcards, so we always pass 0.
 *
 * returns: a properly escaped query string
 *
 *
 */


#include <stdlib.h>
#include <string.h>


char *idss_escape_sql(const char *input, char quote, int wildcards) {
  char       *out, *ptr;
  const char *c;

  /* If every character in the input needs to be escaped, the resulting string
   * would at most double in size.  Also, include room for the surrounding quotes.
   */
  if (quote != '\'' && quote != '\"') 
  	return 0;

  if (strlen(input) > ((~(unsigned int)0)>>1)-3)
  	return 0;

  if (!(out = ptr = (char *) malloc(strlen(input) * 2 + 2 + 1)))
  	return 0;

  *ptr++ = quote;

  for (c = input;  *c;  c++) {
    switch (*c) {

      case '\'': case '\"':
        if (quote == *c)
        	*ptr++ = *c;

        *ptr++ = *c;
        break;

      case '%': case '_': case '[': case ']':
        if (wildcards)
        	*ptr++ = '\\';

        *ptr++ = *c;
        break;

      case '\\': *ptr++ = '\\'; *ptr++ = '\\'; break;
      case '\b': *ptr++ = '\\'; *ptr++ = 'b';  break;
      case '\n': *ptr++ = '\\'; *ptr++ = 'n';  break;
      case '\r': *ptr++ = '\\'; *ptr++ = 'r';  break;
      case '\t': *ptr++ = '\\'; *ptr++ = 't';  break;

      default:
        *ptr++ = *c;
        break;
    }
  }

  *ptr++ = quote;
  *ptr = 0;
  return out;
}

