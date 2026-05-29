/** @file utils.h
 * functionalities to be used to develop a client.
 *
 * Copyright 2022-2025 University of Salento, Italy.
 * All rights reserved.
 *
 *
 *
 */

/* The structure stores information about web service */
#ifndef __UTILS_H
#define __UTILS_H

#include <libxml/parser.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#ifndef __PROVIDERS
#include "idss_data.h"
#endif

#define LOG_FILE_LIMIT 0x10000000


#ifndef __PROVIDERS
#define idss_errmsg_r(string) { \
char buf[128] = {0}; \
struct tm tm; \
time_t now; \
char *timestamp; \
now = time(NULL); \
timestamp = asctime_r(gmtime_r(&now, &tm), buf); \
timestamp[strlen(timestamp)-1] = 0; \
pthread_mutex_lock (&log_mutex); \
if (logfd) { \
  if (ftell(logfd) > LOG_FILE_LIMIT) \
	idss_set_logfile(NULL); \
fprintf(logfd, "[%s][File %s:%d function %s] %s\n", timestamp, __FILE__, __LINE__, __func__, string); \
fflush(logfd); \
} \
else \
fprintf(stderr, "[%s][File %s:%d function %s] %s\n", timestamp, __FILE__, __LINE__, __func__, string); \
pthread_mutex_unlock (&log_mutex); \
}
#endif

#ifdef __PROVIDERS

#define idss_errmsg_r(string) \
{ \
	char buf[128] = {0}; \
	struct tm tm; \
	time_t now; \
	char *timestamp; \
	now = time(NULL); \
	timestamp = asctime_r(gmtime_r(&now, &tm), buf); \
	timestamp[strlen(timestamp)-1] = 0; \
	fprintf(stderr, "[%s][File %s:%d function %s] %s\n", timestamp, __FILE__, __LINE__, __func__, string); \
}


#endif



#define ptr_destroy(h) ptr_destroy_dbg(h, __FILE__, __LINE__)

struct _ptr_mng {
  void **** ptr;
  int *type; // 0 = string; 1 = array null termined
  int len;
};
typedef struct _ptr_mng* ptr_mng;


/*
 * code_str codes the "& _ ~ ?" characters with the '?' character followed
 * by the related ascii code and removes all other invalid characters for MDS
 *
 */
char *code_str (char **text_src);

/*
 * decode_str codes the '?' character followed by the related ascii code
 * with "& _ ~ ?" characters
 *
 */
char *decode_str (char **text_src);

/* domainname checks the FQDN specified to verify if it is correct and returns
 * the domain.
 */
char *domainname(char *hostname);

/*
 * get_time takes the information creation date in the
 * following format: MM/DD/YYYY hh:mm:ss
 *
 */
char * get_time(void);

/*
 * Removes (trims) leading and trailing blank characters from
 * a string
 *
 */
char *strtrim(char *str2parse);

/*
 * Free a list of parameters
 *
 */
void mfree (int count, ...);

//void ptr_destroy (ptr_mng reg);
void ptr_destroy_dbg (ptr_mng reg, char*, int);
int ptr_register (ptr_mng* reg, void ** ptr, int type);


/* tokenize_string_r parses a string and returns -1 in case of error,
 * or the number of tokens found in the string s.
 * this version IS THREAD SAFE
 */
int tokenize_string_r(char *s, char *delimiters, char ***tokenp);

/* tokenize_string parses a string and returns -1 in case of error,
 * or the number of tokens found in the string s.
 * this version is NOT THREAD SAFE
 */
int tokenize_string(char *s, char *delimiters, char ***tokenp);

/* readfile puts the content of a file into a buffer
 *
 */
int readfile(char *filename, char **buffer);

ssize_t idss_readn(int fd, void *vptr, size_t n);

ssize_t idss_writen(int fd, const void *vptr, size_t n);

/* Evaluating URL encodings* The RFC 1738 defines the synatx for URLs. Section 2.2 of that document also* defines the rules for encoding characters in a URL. While some characters must* always be encoded, any character my be encoded. To encode a character, you must use* its hexadecimal digits: for instance, the character A has the value 0x41 and when* encoded in a URL it becomes %41 * You need to decode an URL before using it; the problem is that you must make* certain that you never decode a URL that has already been decoded, or you will be vulnerable to double * encoding attacks. Suppose that the URL contains the sequence %25%34%31. Decoded once the result is %41 * because  %25 is the encoding for the percent symbol, %34 is the encoding for the number 4, and %31 is
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

#define BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? ((x) - '0') : \
                                                 (toupper((x)) - 'A' + 10))

char *idss_decode_url(const char *url, size_t *nbytes);


/*
 * ! fn char *idss_code_url(const char *url)
 *
 * \brief this function encode URLs (RFC 1738)
 *
 * \param const char *url: The URL to be coded
 * Returns coded URL
 */

char *idss_code_url(const char *url);

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
*/

int idss_email_isvalid(const char *address);


/* Escaping SQL SELECT statement
 * This is done before submitting a query string to the iDSS parser
 * which prevents SQL injection attacks; after escaping the input string
 * we parse it to make sure the string is indeed a valid SELECT statement.
 *
 * input parameters
 *
 *  const char *input   This is the input query string
 *
 *  char quote          This is the quoting character to be used:
 *                      it must be either a single or double quote.
                        For iDSS we use the single quote.
 *
 *  int wildcards       This is a boolean flag, indicating if wildcards
 *                      (such as percent symbol, underscore and square brackets)
 *                      recognized by the LIKE operator in a WHERE clause must be
 *                      escaped or not: passing 0 will NOT escape these characters.
 *                      Passing 1 WILL escape the wildcards.
 *                      In general, wildcards should be escaped only when the input string
 *                      is going to be used as the right hand side for the LIKE operator.
 *                      In iDSS there is no need to escape wildcards, so we always pass 0.
 *
 * returns: a properly escaped query string
 */



char *idss_escape_sql(const char *input, char quote, int wildcards);


#endif
