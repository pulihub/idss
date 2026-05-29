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
 */



#include <pthread.h>
#include "idss_unix.h"
#include "idss_data.h"
#include "utils.h"
#include "data_access_structures.h"
#include "data_access_func.h"

#ifdef LINUX_IA64
#define SIZE_T_MAX UINT_MAX
#endif
#ifdef LINUX
#define SIZE_T_MAX UINT_MAX
#endif
#ifdef TRU64
#define SIZE_T_MAX UINT_MAX
#endif

/* this mutex is used to make thread-safe the functions idss_env_putenv() and idss_env_delenv() */
static pthread_mutex_t mutex;

/* default environment */
extern char **environ;

static char *idss_restricted_environ[] = {
	"IFS= \t\n",
	"PATH= _PATH_STDPATH",
	NULL
};

/* this variable is used by idss_env_putenv() and idss_env_delenv()
* do not modify the value of this variable directly
*/

static int idss_environ;

void idss_clean_up(void);
static size_t idss_strlen_safe(const char *s);
static void *sigterm_thread(void *arg);


/* This must perform clean_up as needed by iDSS, before exiting
 * from a signal handler. Thus, the code must be async-signal-safe.
 *
 * input parameters: void
 * returns: void
 *
 */

void idss_clean_up(void)
{
	/*
		Clean-up code goes here --
		WARNING: the code must be async-signal-safe.
	*/

	pid_t pid;
	int fds;
	int fd;
	info_provider_node ipnode = NULL;

    	fclose(logfd);
	logfd = NULL;

	if (get_debug_flg() >= 1)
		write(STDERR_FILENO, "entering idss_clean_up\n", idss_strlen_safe("entering idss_clean_up\n"));

  	if ((fds = getdtablesize()) == -1) /* get number of open file descriptors */
		  fds = IDSS_OPENMAX;         /* if getdtablesize() fails, assume 256 */


	pid = getpid();

	/* terminating all of the processes related to information providers */
    	for (get_next_info_provider_node(&ipnode); ipnode; get_next_info_provider_node(&ipnode))
    	{
		if (ipnode->pid > 0 && ipnode->pid != pid)
		{
			kill(ipnode->pid, SIGKILL);
		}
    	}

    	idss_data_dealloc();


	errno = 0;
	while (wait(NULL) > 0)  /* now wait */
		;

	if (errno != ECHILD)
		write(STDERR_FILENO, "An error occurred waiting\n", idss_strlen_safe("An error occurred waiting\n"));

	write(STDERR_FILENO, "The iDSS server is shutting down\n", idss_strlen_safe("The iDSS server is shutting down\n"));

	if (get_debug_flg() >= 1)
		write(STDERR_FILENO, "exiting idss_clean_up\n", idss_strlen_safe("exiting idss_clean_up\n"));


  	/* closing all file descriptors */
	for (fd = 0;  fd < fds;  fd++)     /* close open file descriptors including stdin, stdout, and stderr */
		  (void) close(fd);

}

/* This is an  async-signal-safe version of strlen()
 * indeed, the C library strlen() function is NOT async-signal-safe!
 *
 * input parameters
 * const char *s A C style string
 * returns: the length of the string
 */

static size_t idss_strlen_safe(const char *s)
{

	size_t n = 0;
	while (*s++ != '\0')
		n++;
	return n;

}

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

void idss_child_signal_handler(int signum)
{

pid_t child_pid;
int status;

if (get_debug_flg() >= 1)
	write(STDERR_FILENO, "entering idss_child_signal_handler\n", idss_strlen_safe("entering idss_child_signal_handler\n"));

/* wait for any child terminate
 * and get its status; wait if no status is available immediately
 */

while((child_pid = waitpid(-1, &status, WNOHANG)))
   if (!((child_pid == -1) && (errno != EINTR)))
      break;


/* do what is needed after termination of child process
 * remember: only async-signal-safe code
 */
if (get_debug_flg() >= 1)
	write(STDERR_FILENO, "exiting idss_child_signal_handler\n", idss_strlen_safe("exiting idss_child_signal_handler\n"));

}

/* This function set properly the stdout and
 * stderr descriptors
 *
 * input paramters:
 *
 * char * out //file name for the stdout descriptor
 * char * err //file name for the stderr descriptor
 *
 */

void idss_set_err_descr(char * err)
{
  char errbuff[1024];
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  int flags = O_RDWR | O_CREAT | O_APPEND;

   if (get_debug_flg() >= 1)
   {
		sprintf (errbuff, "entering idss_set_err_descr");
                idss_errmsg_r(errbuff);
   }

  if (err){
	close(STDERR_FILENO);
    if (open (err, flags, mode)  < 0)
      open (_PATH_DEVNULL, O_RDWR);
    else
	setbuf(stderr, NULL);

  }

   if (get_debug_flg() >= 1)
   {
		sprintf (errbuff, "exiting idss_set_err_descr");
                idss_errmsg_r(errbuff);
   }
}


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

void idss_signal_handler(int signum)
{
	int i;

	struct {
		int signum;
		char *msg;
	} sigmsg[] = {
		{ SIGBUS, "Access to undefined portion of a memory object" },
		{ SIGFPE, "Erroneous arithmetic operation" },
		{ SIGILL, "Illegal instruction" },
		{ SIGSEGV, "Invalid memory reference" },
		{ SIGSYS, "Bad system call" },
		{ SIGXCPU, "CPU-time limit exceeded" },
		{ SIGXFSZ, "File-size limit exceeded" },
		{ 0, NULL}
	};

if (get_debug_flg() >= 1)
	write(STDERR_FILENO, "entering idss_signal_handler\n", idss_strlen_safe("entering idss_signal_handler\n"));

	 /* write on standard error
	 * an appropriate message related to the
	 * signal that has been delivered
	 * note that we must use write() because
	 * printf() and fprintf() are NOT async-signal-safe
	 */

	for (i = 0; sigmsg[i].signum > 0; i++)
		if (sigmsg[i].signum == signum) {
			(void) write(STDERR_FILENO, sigmsg[i].msg,
			  idss_strlen_safe(sigmsg[i].msg));
			(void) write(STDERR_FILENO, "\n", 1);
			break;
		}




	if (get_debug_flg() >= 1)
		write(STDERR_FILENO, "exiting idss_signal_handler\n", idss_strlen_safe("exiting idss_signal_handler\n"));

	/* perform clean_up as needed */
	idss_clean_up();

        /* call _exit() to terminate the application;
         * note that we can NOT call the ordinary exit()
         * from a signal handler, since we must not execute
         * clean_up handlers associated to exit() that may have been
         * registered using atexit()
         * when terminating an application from a signal handler
         */
	_exit(EXIT_FAILURE);

}

/*
 * This function is used for
 * setting up signal dispositions
 *
 * input parameters: void
 * returns: 0 on success, -1 on error;
 *
 */

int idss_handle_signals(void)
{


	int rc;
	sigset_t set;
        char errbuff[1024] = {'\0'};
	struct sigaction act;
	pthread_t tid;

        if (get_debug_flg() >= 1)
	{
		sprintf (errbuff, "entering idss_handle_signals");
                idss_errmsg_r(errbuff);
	}

	/* we begin by blocking ALL of the signals */
	rc = sigfillset(&set);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set current thread signal mask */
	rc = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* initialize the struct sigaction act */
	memset(&act, 0, sizeof(act));
	rc = sigfillset(&act.sa_mask);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set the signal disposition to SIG_IGN
	 * to ignore the following signals:
	 * SIGHUP this signal (terminal line hangup) is traditionally used to ask the application
         * to read again its configuration files
	 * SIGINT we want to ignore users pressing ^C since the correct way to terminate the application
	 * is by receiving SIGTERM when the administrator wants to shut down the process
	 * SIGQUIT we want to ignore users pressing ^\ for the same reason
	 * SIGPIPE we want to ignore this so that write() will instead return an error if it is writing to an empty pipe
	 * this is more convenient than receiving a signal
	 */

	act.sa_handler = SIG_IGN;

	rc = sigaction(SIGHUP, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGINT, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGQUIT, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGPIPE, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set the signal disposition for the other signal we are interested in;
	 * this time we want our signal handler to execute when one of these signals
	 * will be delivered:
	 * SIGBUS
	 * SIGFPE
	 * SIGILL
	 * SIGSEGV
	 * SIGSYS
	 * SIGXCPU
	 * SIGXFSZ
	 */

	act.sa_handler = idss_signal_handler;


	rc = sigaction(SIGBUS, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGFPE, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGILL, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGSEGV, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGSYS, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGXCPU, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaction(SIGXFSZ, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	act.sa_handler = idss_child_signal_handler;

	rc = sigaction(SIGCHLD, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* now unblock all signals but SIGTERM that must be blocked in all of the threads except our sigterm_thread thread */
	rc = sigemptyset(&set);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = sigaddset(&set, SIGTERM);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set current thread signal mask */
	rc = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* create the thread that will handle SIGTERM using sigwait() */
	pthread_create(&tid, NULL, sigterm_thread, NULL);

	/* done, return 0 to indicate success */
       	if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "exiting idss_handle_signals");
                  idss_errmsg_r(errbuff);
	}

	return 0;

}


static void *sigterm_thread(void *arg)
{

	int rc;
	sigset_t set;
        char errbuff[1024] = {'\0'};
	int signum;

	sprintf (errbuff, "entering sigterm_thread");
        idss_errmsg_r(errbuff);

	rc = sigemptyset(&set);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting sigterm_thread");
                  idss_errmsg_r(errbuff);
		}
		return (void *) NULL;
        }

	rc = sigaddset(&set, SIGTERM);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting sigterm_thread");
                  idss_errmsg_r(errbuff);
		}
		return (void *) NULL;
        }

	rc = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting sigterm_thread");
                  idss_errmsg_r(errbuff);
		}
		return (void *) NULL;
        }

	while(1){
		rc = sigwait(&set, &signum);
		if(rc != 0)
			continue;

		if(signum == SIGTERM){
			fprintf(stderr, "The iDSS Service received the SIGTERM signal and is shutting down\n");

			/* idss cleanup code: this time the code does not need to be async-signal safe */
			/* however, we already have coded an async-signal-safe cleanup function, let's use this one */
		  	sprintf (errbuff, "exiting sigterm_thread");
                  	idss_errmsg_r(errbuff);
			idss_clean_up();

			_exit(EXIT_SUCCESS);
		}

	}

	return (void *) NULL; /* will never get here */

}



/* This function daemonize the application
 *
 * input parameters: void
 * returns: 0 on success, -1 on error;
 *
 */

int idss_daemonize(void)
{
	int fd, fds;
	pid_t	pid;
	int rc;
	sigset_t set;
	struct sigaction act;
  char errbuff[1024];

       	if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "entering idss_daemonize");
                  idss_errmsg_r(errbuff);
	}

	/* fork a child */
	if ( (pid = fork()) < 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return (-1);
	}
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */

	if (setsid() < 0){			/* become session leader */
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return (-1);
	}

	/* here we need to set the signal disposition of SIGHUP to SIG_IGN */
    	/* we begin by blocking ALL of the signals */
	rc = sigfillset(&set);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set current thread signal mask */
	rc = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* initialize the struct sigaction act */
	memset(&act, 0, sizeof(act));
	rc = sigfillset(&act.sa_mask);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* set the signal disposition to SIG_IGN
	 * to ignore SIGHUP: this signal is traditionally used to ask the application to read again its configuration files
	 *
	 */

	act.sa_handler = SIG_IGN;

	rc = sigaction(SIGHUP, &act, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* now unblock all signals */
	rc = sigemptyset(&set);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	rc = pthread_sigmask(SIG_SETMASK, &set, NULL);
	if(rc != 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return -1;
        }

	/* done with SIGHUP */


	/*fork again */
	if ( (pid = fork()) < 0){
        	if (get_debug_flg() >= 1)
		{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
		}
		return (-1);
        }
	else if (pid)
		_exit(0);			/* child 1 terminates */

	/* child 2 continues... */

	/*chdir("/");*/				/* change working directory */


	if ((fds = getdtablesize()) == -1) /* get number of open file descriptors */
		  fds = IDSS_OPENMAX;         /* if getdtablesize() fails, assume 256 */
	for (fd = 0;  fd < fds;  fd++)     /* close open file descriptors including stdin, stdout, and stderr */
		  (void) close(fd);

	/* redirect stdin, stdout, and stderr to /dev/null
	 * since we closed all of the file descriptors,
	 * the first open will return 0, thus actually redirecting stdin,
	 * the second call to open will return 1 etc
	 * the key point is that open always returns the lowest unused file descriptor
	 */

	open(_PATH_DEVNULL, O_RDONLY);
	open(_PATH_DEVNULL, O_RDWR);
	open(_PATH_DEVNULL, O_RDWR);

        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "exiting idss_daemonize");
                  idss_errmsg_r(errbuff);
	}
	return (0);				/* success */
}







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





void idss_env_sanitize(int preservec, char **preservev)
{
  int    i, rc;
  char   **new_environ, *ptr, *value, *var;
  size_t arr_size = 1, arr_ptr = 0, len, new_size = 0;
  char errbuff[1024];

  /* initialize the mutex that will be used
   * by idss_env_putenv() and idss_env_delenv()
   *
   * remember to call idss_env_finalize() when you are done with
   * environment management, to destroy the mutex
   *
   */

  rc = pthread_mutex_init(&mutex, NULL);
  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to initialize mutex");
                  idss_errmsg_r(errbuff);
	}
	  return;
  }

  /* determine how much memory we need to allocate to build the new environment
   * with the IFS, PATH, TZ and user's supplied variables to be preserved
   * if the amount of memory we need can not be alloctaed, call abort() and
   * terminate the application
   */

  for (i = 0;  i >= 0 && (var = idss_restricted_environ[i]) != 0;  i++) {
    if (arr_size == SIZE_T_MAX || new_size > SIZE_T_MAX - strlen(var))
		abort();
    new_size += strlen(var) + 1;
    arr_size++;
  }
  if (i < 0)
	  abort();
  for (i = 0;  i >= 0 && (var = preservev[i]) != 0;  i++) {
    if (!(value = getenv(var)))
		continue;
    if (arr_size == SIZE_T_MAX || new_size > SIZE_T_MAX - strlen(var) - strlen(value) - 1)
		abort();
    new_size += strlen(var) + strlen(value) + 2; /* include the '=' */
    arr_size++;
  }
  if (i < 0)
	  abort();
  if (preservec && preservev) {
    for (i = 0;  i < preservec && (var = preservev[i]) != 0;  i++) {
      if (!(value = getenv(var)))
		  continue;
      if (arr_size == SIZE_T_MAX || new_size > SIZE_T_MAX - strlen(var) - strlen(value) - 1)
		  abort();
      new_size += strlen(var) + strlen(value) + 2; /* include the '=' */
      arr_size++;
    }
  }

  if (new_size > SIZE_T_MAX - (arr_size * sizeof(char *)) + 1)
	  abort();
  new_size += (arr_size * sizeof(char *));
  if (!(new_environ = (char **) malloc(new_size)))
	  abort();
  new_environ[arr_size - 1] = 0;

  /* We have got the memory we need, now build the new environment and replace
   * the old environ pointer with a pointer to the newly allocated one.
   * Note that memory is allocate in one chunk, rather than in smaller pieces for
   * the individual strings, since this is faster.
   */

  ptr = (char *) new_environ + (arr_size * sizeof(char *));
  for (i = 0;  (var = idss_restricted_environ[i]) != 0;  i++) {
    new_environ[arr_ptr++] = ptr;
    len = strlen(var);
    memcpy(ptr, var, len + 1);
    ptr += len + 1;
  }
  for (i = 0;  (var = preservev[i]) != 0;  i++) {
    if (!(value = getenv(var)))
		continue;
    new_environ[arr_ptr++] = ptr;
    len = strlen(var);
    memcpy(ptr, var, len);
    *(ptr + len + 1) = '=';
    memcpy(ptr + len + 2, value, strlen(value) + 1);
    ptr += len + strlen(value) + 2; /* include the '=' */
  }
  if (preservec && preservev) {
    for (i = 0;  i < preservec && (var = preservev[i]) != 0;  i++) {
      if (!(value = getenv(var)))
		  continue;
      new_environ[arr_ptr++] = ptr;
      len = strlen(var);
      memcpy(ptr, var, len);
      *(ptr + len + 1) = '=';
      memcpy(ptr + len + 2, value, strlen(value) + 1);
      ptr += len + strlen(value) + 2; /* include the '=' */
    }
  }

  environ = new_environ;
}



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

char *idss_env_getenv(const char *name)
{
  char *value;

  if (!(value = getenv(name)))
  	return NULL;
  return strdup(value);
}

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


int idss_env_putenv(const char *name, const char *value)
{
  int         del = 0, envc, i, mod = -1;
  char        *envptr, **new_environ;
  size_t      delsz = 0, envsz = 0, namelen, valuelen;
  extern char **environ;
  char errbuff[1024];
  int rc;


  /* lock the mutex to make thread safe this function
   * environment handling is done in a critical section
   */
  rc = pthread_mutex_lock(&mutex);
  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to lock mutex");
                  idss_errmsg_r(errbuff);
	}
	return -1;
  }

  /* First compute the amount of memory required for the new environment */
  namelen  = strlen(name);
  valuelen = strlen(value);
  for (envc = 0;  envc >= 0 && environ[envc];  envc++) {
	  if (!strncmp(environ[envc], name, namelen) && environ[envc][namelen] == '=') {
		  if (mod == -1)
			  mod = envc;
		  else {
			  if (++del <= 0 || delsz > SIZE_T_MAX - strlen(environ[envc])) {
				  rc = pthread_mutex_unlock(&mutex);
				  if(rc != 0) {
      if (get_debug_flg () >= 1)
      {
		  sprintf (errbuff, "Failed to lock mutex");
                  idss_errmsg_r(errbuff);
      }
					  return -1;
				  }
				  return 0;
			  }
			  delsz += strlen(environ[envc]) + 1;
		  }
	  }
	  if (envsz > SIZE_T_MAX - strlen(environ[envc])) {
		  rc = pthread_mutex_unlock(&mutex);
		  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to lock mutex");
                  idss_errmsg_r(errbuff);
	}
			  return -1;
		  }
		  return 0;
	  }
	  envsz += strlen(environ[envc]) + 1;
  }
  if (envc < 0) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }
	  return 0;
  }
  if (mod == -1) {
	  if (++envc <= 0 || envsz > SIZE_T_MAX - namelen - valuelen - 1) {
		  rc = pthread_mutex_unlock(&mutex);
		  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
			  return -1;
		  }
		  return 0;
	  }
	  envsz += (namelen + valuelen + 1 + 1);
  }
  envc  -= del;   /* account for duplicate entries of the same name */
  envsz -= delsz;

  /* allocate memory for the new environment */
  if (envsz > SIZE_T_MAX - (sizeof(char *) * (envc + 1)) + 1) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }
	  return 0;
  }
  envsz += (sizeof(char *) * (envc + 1));
  if (!(new_environ = (char **) malloc(envsz))) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }
	  return 0;
  }
  envptr = (char *) new_environ + (sizeof(char *) * (envc + 1));

  /* copy the old environment into the new environment,  eplacing the named
	  * environment variable if it already exists; otherwise, add it at the end.
	  */
  for (envc = i = 0;  environ[envc];  envc++) {
	  if (del && !strncmp(environ[envc], name, namelen) && environ[envc][namelen] == '=')
		  continue;
	  new_environ[i++] = envptr;
	  if (envc != mod) {
		  envsz = strlen(environ[envc]);
		  memcpy(envptr, environ[envc], envsz + 1);
		  envptr += (envsz + 1);
	  } else {
		  memcpy(envptr, name, namelen);
		  memcpy(envptr + namelen + 1, value, valuelen);
		  envptr[namelen] = '=';
		  envptr[namelen + valuelen + 1] = 0;
		  envptr += (namelen + valuelen + 1 + 1);
	  }
  }
  if (mod == -1) {
	  new_environ[i++] = envptr;
	  memcpy(envptr, name, namelen);
	  memcpy(envptr + namelen + 1, value, valuelen);
	  envptr[namelen] = '=';
	  envptr[namelen + valuelen + 1] = 0;
  }
  new_environ[i] = 0;

  /* possibly free the old environment, then replace it with the new one */
  if (idss_environ) free(environ);
  environ = new_environ;
  idss_environ = 1;

  rc = pthread_mutex_unlock(&mutex);
  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
	  return -1;
  }
  return 1;

}


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


int idss_env_delenv(const char *name)
{
  int         del = 0, envc, i, idx = -1;
  size_t      delsz = 0, envsz = 0, namelen;
  char        *envptr, **new_environ;
  extern int  idss_environ;
  extern char **environ;
  char errbuff[1024];
  int rc;

  /* lock the mutex to make thread safe this function
   * environment handling is done in a critical section
   */
  rc = pthread_mutex_lock(&mutex);
  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to lock mutex");
                  idss_errmsg_r(errbuff);
	}
	  return -1;
  }

  /* first compute the size of the new environment */
  namelen = strlen(name);
  for (envc = 0;  envc >= 0 && environ[envc];  envc++) {
    if (!strncmp(environ[envc], name, namelen) && environ[envc][namelen] == '=') {
      if (idx == -1)
		  idx = envc;
      else {
		  if (++del <= 0 || delsz > SIZE_T_MAX - strlen(environ[envc])) {
			  rc = pthread_mutex_unlock(&mutex);
			  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
				  return -1;
			  }

			  return 0;
		  }
        delsz += strlen(environ[envc]) + 1;
      }
    }
	  if (envsz > SIZE_T_MAX - strlen(environ[envc])) {
		  rc = pthread_mutex_unlock(&mutex);
		  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
			  return -1;
		  }

		return 0;
	  }
    envsz += strlen(environ[envc]) + 1;
  }
  if (envc < 0 || idx == -1) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }

	  return (idx == -1);
  }
  envc  -= del;   /* account for duplicate entries of the same name */
  envsz -= delsz;

  /* allocate memory for the new environment */
  if (envsz > SIZE_T_MAX - (sizeof(char *) * (envc + 1)) + 1) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }

	  return 0;
  }
  envsz += (sizeof(char *) * (envc + 1));
  if (!(new_environ = (char **) malloc(envsz))) {
	  rc = pthread_mutex_unlock(&mutex);
	  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
		  return -1;
	  }

	  return 0;
  }
  envptr = (char *) new_environ + (sizeof(char *) * (envc + 1));

  /* copy the old environment into the new environment, ignoring any
   * occurrences of the environment variable that we want to delete.
   */
  for (envc = i = 0;  environ[envc];  envc++) {
    if (envc == idx || (del && !strncmp(environ[envc], name, namelen) && environ[envc][namelen] == '='))
		continue;
    new_environ[i++] = envptr;
    envsz = strlen(environ[envc]);
    memcpy(envptr, environ[envc], envsz + 1);
    envptr += (envsz + 1);
  }

  /* possibly free the old environment, then replace it with the new one */
  if (idss_environ) free(environ);
  environ = new_environ;
  idss_environ = 1;

  rc = pthread_mutex_unlock(&mutex);
  if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to unlock mutex");
                  idss_errmsg_r(errbuff);
	}
	  return -1;
  }
  return 1;
}

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

int idss_env_finalize(void)
{
	int rc;
  char errbuff[1024];

	rc = pthread_mutex_destroy(&mutex);
	if(rc != 0) {
        if (get_debug_flg() >= 1)
	{
		  sprintf (errbuff, "Failed to destroy mutex");
                  idss_errmsg_r(errbuff);
	}
		return -1;
	}

	return 0;
}



/* this function is used internally by idss_sanitize_files()
 * input paramters:
 *
 * int fd The file descriptor we are going to open using /dev/null
 *
 * returns: 0 on error, 1 on success
 *
 */

static int idss_open_devnull(int fd) {
  FILE *f = 0;

  if (!fd) f = freopen(_PATH_DEVNULL, "rb", stdin);
  else if (fd == 1) f = freopen(_PATH_DEVNULL, "wb", stdout);
  else if (fd == 2) f = freopen(_PATH_DEVNULL, "wb", stderr);
  return (f && fileno(f) == fd);
}

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

void idss_sanitize_files(void)
{
  char errbuff[1024];
  int         fd, fds;
  struct stat st;

  if (get_debug_flg() >= 1)
  {
		  sprintf (errbuff, "entering idss_sanitize_files");
                  idss_errmsg_r(errbuff);
  }

  /* Make sure all open descriptors other than the standard ones are closed:
   * first get number of file descriptors using getdtablesize(),
   * then close all but stdin, stdout and stderr
   */

  if ((fds = getdtablesize()) == -1)
  	fds = IDSS_OPENMAX;
  for (fd = 3;  fd < fds;  fd++)
  	(void) close(fd);

  /* Verify that the standard descriptors are open.  If they're not, attempt to
   * open them using /dev/null.  If any are unsuccessful, abort.
   */

  for (fd = 0;  fd < 3;  fd++)
    if (fstat(fd, &st) == -1 && (errno != EBADF || !idss_open_devnull(fd))) abort();

  if (get_debug_flg() >= 1)
  {
		  sprintf (errbuff, "exiting idss_sanitize_files");
                  idss_errmsg_r(errbuff);
  }
}


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

void idss_limit_core(void)
{
  struct rlimit rlim;
  char errbuff[1024];

  if (get_debug_flg() >= 1)
  {
		  sprintf (errbuff, "entering idss_limit_core");
                  idss_errmsg_r(errbuff);
  }

  rlim.rlim_cur = rlim.rlim_max = 0;
  setrlimit(RLIMIT_CORE, &rlim);

  if (get_debug_flg() >= 1)
  {
		  sprintf (errbuff, "exiting idss_limit_core");
                  idss_errmsg_r(errbuff);
  }
}


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
#ifndef TRU64
int idss_is_safedir(const char *dir)
{
  DIR         *fd, *start;
  int         rc = -1;
  char        new_dir[PATH_MAX + 1] = {'0'};
  uid_t       uid;
  struct stat f, l;

  /* Check all of the directories in the path;
   * traverse the directory tree from the bottom to the root,
   * ensuring that only the owner or superuser have write access
   * to each directory
   *
   */

  if (!(start = opendir("."))) return -1;
  if (lstat(dir, &l) == -1) {
    closedir(start);
    return -1;
  }
  uid = geteuid();

  do {
    if (chdir(dir) == -1) break;
    if (!(fd = opendir("."))) break;
    if (fstat(dirfd(fd), &f) == -1) {
      closedir(fd);
      break;
    }
    closedir(fd);

    if (l.st_mode != f.st_mode || l.st_ino != f.st_ino || l.st_dev != f.st_dev)
      break;
    if ((f.st_mode & (S_IWOTH | S_IWGRP)) || (f.st_uid && f.st_uid != uid)) {
      rc = 0;
      break;
    }
    dir = "..";
    if (lstat(dir, &l) == -1) break;
    if (!getcwd(new_dir, PATH_MAX + 1)) break;
  } while (new_dir[1]); /* new_dir[0] will always be a slash */
  if (!new_dir[1]) rc = 1;

  fchdir(dirfd(start));
  closedir(start);
  return rc;
}
#endif

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

size_t idss_strlcpy(char *dst, const char *src, size_t size) {
  char       *dstptr = dst;
  size_t     tocopy  = size;
  const char *srcptr = src;

  if (tocopy && --tocopy) {
    do {
      if (!(*dstptr++ = *srcptr++)) break;
    } while (--tocopy);
  }
  if (!tocopy) {
    if (size) *dstptr = 0;
    while (*srcptr++);
  }

  return (srcptr - src - 1);
}

size_t idss_strlcat(char *dst, const char *src, size_t len) {
  char       *dstptr = dst;
  size_t     dstlen, tocopy;
  const char *srcptr = src;

  tocopy = 0;

  while (tocopy-- && *dstptr) dstptr++;
  dstlen = dstptr - dst;
  if (!(tocopy = len - dstlen)) return (dstlen + strlen(src));
  while (*srcptr) {
    if (tocopy != 1) {
      *dstptr++ = *srcptr;
      tocopy--;
    }
    srcptr++;
  }
  *dstptr = 0;

  return (dstlen + (srcptr - src));
}


