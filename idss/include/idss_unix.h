/* This software provides common unix
* application functionalities, including
* signal handling, becomin a daemon etc.
*
* Part of this code is strongly based on 
* Secure programming Cookbook
* by John Viega & matt Messier
* O'Reilly & Associates inc
* ISBN 0-596-00394-3
*
* The source code has been modified
* as needed by iDSS, for instance
* the environment handling functions
* have been modified to make them thread-safe,
* etc.
* 
* 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <paths.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>


#ifndef IDSS_OPENMAX
#define IDSS_OPENMAX 256
#endif

/* our clean up function */
void idss_clean_up(void);



/* This handler deals with the SIGCHLD signal
*
* Remember to write only async-signal-safe code inside
* a signal handler.
*
* input parameters:
*
* int signum An integer corresponding to the signal that has been caught
*
* returns: void
* 
*/

void idss_child_signal_handler(int signum);

/* This handler deals with the following signals:
* SIGBUS, SIGFPE, SIGILL, SIGSEGV
* SIGSYS, SIGXCPU, SIGXFSZ
*
* Remember to write only async-signal-safe code inside
* a signal handler.
*
* input parameters:
*
* int signum An integer corresponding to the signal that has been caught
*
* returns: void
* 
*/

void idss_signal_handler(int signum);


/*
 * This function is used for
 * setting up signal dispositions
 *
 * NOTE that SIGTERM is handled by a dedicated thread
 * using sigwait()
 * 
 * input parameters: void 
 * returns: 0 on success, -1 on error;
 *
 */

int idss_handle_signals(void);

/* This function daemonize the application
*
* input parameters: void
* returns: 0 on success, -1 on error;
*
*/

int idss_daemonize(void);


/* The idss_env_sanitize() is used to make sure that an attacker does not set
* environment variables to malicious values. The function uses the following arrays: 
* idss_restricted_environ and idss_preserve_environ.
* These arrays are both NULL-terminated.
* idss_restricted_environ contains only the following 
* two environment variables: IFS and PATH;
* IFS is used by many shells to determine which character separates command-line arguments.
* While modern unix shells use a reasonable default value for IFS if it is not set, we must
* defensively assume that a shell does nothing instead. Therefore, we set IFS to a sane value.
* PATH is used by the shell and by some of the exec() system calls to locate an executable
* if a path is not explicitly specified. The search path must never include relative paths, 
* especially the current directorry as denoted by a single period. To be safe, we force PATH
* to _PATH_STDPATH, which is defined in paths.h to contain the standard paths for the system
* you are running on (this is of course platform specific).
*
* idss_preserve_environ contains the environment values you need for your application;
* note that we add also the TZ environment variable that denotes the time zone that 
* the application should use, if relevant. Indeed, users may not be in the same time zone of the 
* machine running the application, so we preserve this variable, if present.
*
* Any other environment variables that are defined in the environment before the application begins
* must be removed. Moreover, the environment variables that are preserved (i.e., stored in the  
																		   * idss_preserve_environ array) must always be treated as untrusted user input.
*
* input parameters:
* 
* int preservec This is the number of entries in the array preservev;
* char **preservev This is the array of environment variables that must be preserved,
* i.e., that must appear in the new environment
* 
* returns: void
*
*/

/* This function set properly the stderr stream
 *
 * input paramters:   
 * 
 * char *err //file name for stderr
 *
 * it returns 0 on success, -1 on error.
 */
void idss_set_err_descr(char * err);

void idss_env_sanitize(int preservec, char **preservev);

/* Obtaining the value of an environment variable
* Do not use getenv(), since it simply returns a pointer 
* into the environment, rather than returning a copy of
* the environment variable's value
* If you do not immediately make a copy of the value returned by getenv()
* but instead store the pointer somewhere for later use, you could end up
* with a dangling pointer or a different value altogether, if the environment
* is modified between the time you called getenv() and the time you use the pointer 
* it returns. The function idss_env_getenv() is safer
* Moreover, never make any assumption about the length or the contents of an environment
* variable's value. Do not copy the value into a statically allocated buffer or even
* a dynamically allocated buffer that was not allocated based on the actual size of 
* the environment variable's value. 
* Another problem is that it is possible to have two or more environment variables
* with the same name in your process's environment. Most, if not all of the implementations,
* will always return the first occurrence of an environment variable.
*
* input parameters:
*
* const char *name The name of the environment variable whose value must be retrieved
*
* returns: a copy of the environment variable's value that must be freed using free()
* on success, NULL on error
*
*/

char *idss_env_getenv(const char *name);


/* Changing the value of an environment variable
* putenv() is normally used to modify the value of an 
* environment variable. Some implementations also allows
* using putenv() to delete environment variables, but this 
* behaviour is nonstandard, therefore not portable.
* If the environment has been sanitized using idss_env_sanitize()
* you can not use putenv() because of the way putenv() manages the memory
* allocated to the environment.
* Moreover, an attacker could have manipulated the environment before spawning 
* your application, in such a way that two or more environment variables 
* share the same name. Using idss_env_sanitize() make reasonably sure that
* there is only one environment variable for each name.
* The function idss_env_putenv() will properly handle an environment built using
* idss_env_sanitize(), as well as an unaltered environment.
* In addition to modifying the value of an environment variable, idss_env_putenv()
* is also capable of adding new environment variables.
*
* input parameters:
*
* const char *name The name of an environment variable to modify or add
* const char *value The value to assign to the environment variable
*
* returns: -1 on error, 0 on success
*
*/  

/* Changing the value of an environment variable */


int idss_env_putenv(const char *name, const char *value);


/* No method for deleting an environment variable is defined in any standard.
* Some implementations of putenv() will delete environment variables if the
* assigned value is a zero length string. Other systems provide a function called
* unsetenv(), but this is nonstandard an thus nonportable.
* None of these methods of deleting environment variables take into account the
* possibility that multiple occurrences of the same environment variable may
* exist in the environment. Usually, only the first occurrence will be deleted,
* rather than all of them. This implies that the environment variable will not
* actually be deleted, because calling getenv() will return the next occurrence
* of the environment variable. If idss_env_sanitize() has been used to
* sanitize the environment, or if idss_env_putenv() has been used, then you must use
* idss_env_delenv to delete an environment variable. The function depends on the static
* variable idss_environ declared at global scope; the two functions must share the
* same instance of that variable to work properly.
*
* input parameters:
*
* const char *name The name of the environment variable to be deleted
*
* returns: 1 on success, 0 on error
*
*/


int idss_env_delenv(const char *name);


/* The function idss_env_finalize() must be called
* when environment handling has been done in order
* to destroy the mutex used to make thread-safe the
* functions idss_env_putenv() and idss_env_delenv()
* Note that after calling this function you can not 
* call again idss_env_putenv() or idss_env_delenv()
* since the mutex these functions rely on has been
* destroyed.
*
* input parameters: void
*
* returns: -1 on error, 0 on success
*
*/

int idss_env_finalize(void);



/* idss_sanitize_files() is used to make sure that only
* the stdin, stdout and stderr file descriptors are open
* when the application starts up. This is needed to avoid
* denial of service attacks and to avoid having an attacker place
* untrusted files on special hardcoded file descriptors.
* The function uses idss_open_devnull() to try to open 
* stdin, stdout and stderr using /dev/null if these
* descriptors are not open
* 
* input parameters: void
*
* returns: void
*
*/

void idss_sanitize_files(void);


/* The function idss_limit_core() is used to make sure that 
* if the application crashes, data will not be written to disk.
* Indeed, an application may store potentially sensitive data in
* memory, and local attackers may be able to examine a core dump and use 
* the application information
*
* input parameters: void
* returns: void
*
*/

void idss_limit_core(void);



/* The function idss_is_safedir() is used to make sure that a directory
* used to store potentially sensitive information can not be modified by 
* other users (except of course the administrator). No future permission checks
* are needed after calling this function.
* The function requires a single argument specifying the directory to check.
* 
* input parameters:
*
* const char *dir The path specification for the directory to be checked
* returns:  -1 on error, 0 if the path specified is not safe, and 1 if 
* the path specified is safe
*
*/ 

int idss_is_safedir(const char *dir);

/* String handling functions
* The functions strlcpy() and strlcat() are not available on all platforms
* (they seem to be available on Darwin (Mac OS X), FreeBSD, NetBSD and OpenBSD).
* Do not use strcpy() ot strcat(): no effort is made to prevent buffer overflows;
* even if you can use strncpy() instead of strcpy() (or strncat()), this is dangerous:
* for strncpy() if the source buffer contains more data than the limit imposed by the
* len argument (here is the prototype:  char *strncpy(char *dst, const char *src, size_t len);)
* the destination buffer will not be NULL terminated.
* This means that the developer must ensure that the destination buffer is NULL terminated.
* The functions strlcpy() and strlcat() are consistent in their behaviour, and provide 
* an indication back to the caller of how much space in the destination buffer would be
* required to succesfully complete their respective operations without truncating the results.
* For both functions, the length limit indicates the maximum size of the destination buffer,
* and the destination buffer is always NULL terminated, even if the destination buffer
* must be truncated.
*/

/* from the Darwin 7.5 man page:

The strlcpy() and strlcat() functions copy and concatenate strings
respectively.  They are designed to be safer, more consistent, and less
error prone replacements for strncpy(3) and strncat(3).  Unlike those
functions, strlcpy() and strlcat() take the full size of the buffer (not
																	 just the length) and guarantee to NUL-terminate the result (as long as
																																 size is larger than 0 or, in the case of strlcat(), as long as there is
																																 at least one byte free in dst).  Note that you should include a byte for
the NUL in size.  Also note that strlcpy() and strlcat() only operate on
true ``C'' strings.  This means that for strlcpy() src must be NUL-termi-
nated and for strlcat() both src and dst must be NUL-terminated.

The strlcpy() function copies up to size - 1 characters from the NUL-ter-
minated string src to dst, NUL-terminating the result.

The strlcat() function appends the NUL-terminated string src to the end
of dst.  It will append at most size - strlen(dst) - 1 bytes, NUL-termi-
nating the result.

RETURN VALUES
The strlcpy() and strlcat() functions return the total length of the
string they tried to create.  For strlcpy() that means the length of src.
For strlcat() that means the initial length of dst plus the length of
src.  While this may seem somewhat confusing it was done to make trunca-
tion detection simple.

Note however, that if strlcat() traverses size characters without finding
a NUL, the length of the string is considered to be size and the destina-
tion string will not be NUL-terminated (since there was no space for the
										NUL).  This keeps strlcat() from running off the end of a string.  In
practice this should not happen (as it means that either size is incor-
								 rect or that dst is not a proper ``C'' string).  The check exists to pre-
vent potential security problems in incorrect code.


EXAMPLES
The following code fragment illustrates the simple case:

char *s, *p, buf[BUFSIZ];

...

(void)strlcpy(buf, s, sizeof(buf));
(void)strlcat(buf, p, sizeof(buf));

To detect truncation, perhaps while building a pathname, something like
the following might be used:

char *dir, *file, pname[MAXPATHLEN];

...

if (strlcpy(pname, dir, sizeof(pname)) >= sizeof(pname))
goto toolong;
if (strlcat(pname, file, sizeof(pname)) >= sizeof(pname))
goto toolong;

Since we know how many characters we copied the first time, we can speed
things up a bit by using a copy instead of an append:

char *dir, *file, pname[MAXPATHLEN];
size_t n;

...

n = strlcpy(pname, dir, sizeof(pname));
if (n >= sizeof(pname))
goto toolong;
if (strlcpy(pname + n, file, sizeof(pname) - n) >= sizeof(pname) - n)
goto toolong;

However, one may question the validity of such optimizations, as they
defeat the whole purpose of strlcpy() and strlcat().  As a matter of
fact, the first version of this manual page got it wrong.

*/

size_t idss_strlcpy(char *dst, const char *src, size_t size);
size_t idss_strlcat(char *dst, const char *src, size_t len);

