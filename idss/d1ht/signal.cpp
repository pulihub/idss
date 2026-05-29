/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             *
 *  No title to or ownership of D1HT is hereby transferred.	             *
 *									     *
 *  D1HT is free software; you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License version 3 as        * 
 *  as published by the Free Software Foundation. Besides, the               *
 *  recipient of D1HT acknowledges and agrees to                             *
 *  strictly respect the following guidelines: 	                             *
 *                                                                           *
 *  (1) This whole copyright notice shall not be removed or modified,        *
 *      although it can and should be appended with informations about       *
 *      modifications included in the software                               *
 *  (2) No results or statistics obtained with the software compiled         *
 *      with the DEBUG flag turned on should be considered as valid          *
 *  (3) Any modified version of this software should be made publicy         *
 *      available BEFORE the results or statistics obtained with this        *
 *      respective version are published.                                    *
 *  (4) Errors and problems with the software (including its results and     *
 *      statistics) should be immediately reported to support.D1HT@gmail.com *
 *  (5) After modifying the source code, it is necessary to compile D1HT     *
 *      with the DEBUG flag on and run it with the flags “–c –V –T” with at  *
 *      least a few dozen peers before considering the modifications as valid*
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with D1HT. If not, see <http://www.gnu.org/licenses/>.              *
 *                                                                           *
 * D1HT is distributed in the hope that it will be usefull, but              *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU GPL      *
 * for more details.                                                         *
 *                                                                           *
 * The information in this software is subject to change without notice      *
 * and should not be construed as a commitment by its authors.               *
 *                                                                           *
 * The authors assume no responsibility or liability for                     *
 * the use, reliability, performance, and results of D1HT.                   *
 *                                                                           *
 *     ################  ==> Please see ChangeLog <==  ################      *
 *                                                                           *
 *****************************************************************************/

/*************************************************************************
 *                    INITIAL VERSION REMARKS                            *
 *                                                                       *
 * Author:        Luiz Rodolpho Monnerat, Petrobras                      *
 * Advisor:       Prof. Claudio L Amorim, COPPE Systems Engineering,     *
 *                Federal University of Rio de Janeiro.                  *
 * Documentation: L. Monnerat and C. Amorim, Peer-to-Peer Single Hop     *
 *                Distributed Hash Tables, in Proceedings of the IEEE    *
 *                Global Communications Conference (GLOBECOM), Nov/2009. *
 *                Available from http://www.cos.ufrj.br/~monnerat        *
 *                                                                       *
 *************************************************************************
 *                                                                       *
 * The msg headers used in this implementation are slightly different(and*
 * bigger) than the ones used in the experiments presented in the        *
 * GLOBECOM 2009 paper. This is because we included a 4 bytes “systemid” *
 * field in the message headers presented in Figures 1.a and 1.b of      *
 * the GLOBECOM paper, and so all messages will have 4 more bytes (these *
 * 4 bytes should be added to all D1HT, CALOT and OneHop msgs, including *
 * acks). This was done because in some tests that we made after         *
 * uploading the final paper version, we observed that when              *
 * running 2 distinct D1HT and/or CALOT  peers in the same node, a peer  *
 * from one system could (by mistake or software bug) send a msg to the  *
 * peer of the other system, which would pollute both peers Routing      *
 * Tables.                                                               *
 *                                                                       *
 *************************************************************************/
/*********************** D1HT COPYRIGHT NOTICE END **********************/
/**
 * @file signal.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

typedef void sigfunc_t(int, siginfo_t*, void*);

sigset_t g_blockmask;

PTHREAD(SIGUSR1_thread_handler) {
	PTHREAD_DETACH;
	PTHREAD_INIT;
	PTHREAD_MAYLOCK;
	
	G_RT.dump_table(); 
	return NULL;
}

void *SIGUSR1_thread() {
	PTHREAD_CREATE(SIGUSR1_thread_handler, NULL, &G_detached);
	return NULL;
} 

void SIG_handler(int signo, siginfo_t *siginfo, void *context) { 
	
 (void) context;

 G_xeq = false;
 
 if (G_exited) return;

 VERBOSE(stderr, "Signal %d catched\n", signo);

 sigset_t previous_blockmask;

 PTXEQ(pthread_sigmask(SIG_BLOCK, NULL, &previous_blockmask), XEQ_ERROR);

 PTXEQ(pthread_sigmask(SIG_BLOCK, &g_blockmask, NULL), XEQ_ERROR);
 
 char msgerr[128];

 if (signo != SIGINT && signo != SIGTERM) {
	 if (G_LINE != 0) {
		 if (G_PTHREAD != 0) {
			 sprintf(msgerr,"Signal caught (signo=%d, siginfo_t errno=%d code=%d) during malloc or free call done in file %s line %d. Detail:tid=%u",
					 signo, siginfo->si_errno, siginfo->si_code, G_FILE, G_LINE, (uint) G_PTHREAD);
		 } else {
			 sprintf(msgerr,"Signal caught (signo=%d, siginfo_t errno=%d code=%d) during malloc or free call done in file %s line %d",
					 signo, siginfo->si_errno, siginfo->si_code, G_FILE, G_LINE);
		 }
	 } else {
		sprintf(msgerr,"Signal caught (signo=%d, siginfo_t errno=%d code=%d)", signo, siginfo->si_errno, siginfo->si_code);
	 }

#if defined(XEQP)
	 if (G_started && !G_exiting) {
#if defined(XEQP_THREAD)
		pthread_data_t	*pdata = (pthread_data_t * ) pthread_getspecific(G_pthread_data_key);
		if (pdata) {
			fprintf(stderr, "\nXEQPOINT signal=%d, ", signo);
			pdata->xeq_print(stderr);
		}
#endif
		if (G_xeqpoint.line != 0) {
			fprintf(stderr, "\nXEQPOINT GLOBAL signal=%d, ", signo);
			G_xeqpoint.print(stderr);
			fprintf(stderr, "\n");
		}
	 }
#endif
 }
 switch (signo) {
	
	case SIGABRT: 
	case SIGBUS: 
    case SIGSEGV:
    case SIGILL: 
	  {   

		  G_parms.verbose = 1;
	
		  if (G_exiting) { 
			  fprintf(stderr,"\n%s FILE=%s LINE=%u Secondary ERROR while Exiting Cause: G_exited=%d, signal %s\n", 
					  G_parms.program_name, __FILE__, __LINE__, (int) G_exited, msgerr);
			  if (G_LINE_PREV != 0) {
				 fprintf(stderr, "Secondary ERROR Cause: Signal caught (signo=%d) and the previous malloc or free call was done in file %s line %d.\n", signo, G_FILE_PREV, G_LINE_PREV);
			  }
		  } else {
			  G_exiting = true;
			  fprintf(stderr,"\n%s tid=%u FILE=%s LINE=%u ERROR Cause: %s - Exiting\n", G_parms.program_name, (uint) pthread_self(), __FILE__, __LINE__, msgerr);
			  if (G_LINE_PREV != 0) {
				 fprintf(stderr, "ERROR Cause: Signal caught (signo=%d) and the previous malloc or free call was done in file %s line %d.\n", signo, G_FILE_PREV, G_LINE_PREV);
			  }
		  }
		  G_exited  = true;
		  
		  exit(-1);
		  break;
	  }
   	
    case SIGPIPE: 
      { 
		  fprintf(stderr,"\n%s FILE=%s LINE=%u WARNING Cause: SIGFPE Caught G_exited=%d, G_exiting=%d, signal %s\n", 
				  G_parms.program_name, __FILE__, __LINE__, (int) G_exited, (int) G_exiting, msgerr);

    	  EXIT("Signal SIGPIPE %s", msgerr);
    	  break;
      }
      	
    case SIGFPE: 
      { 
    	  EXIT("Signal SIGFPE %s", msgerr);
    	  break;
      }
     	
    case SIGALRM: 
      { 
    	  EXIT("Signal SIGALRM %s", msgerr);
    	  break;
      }
     	      
    case SIGTERM:   	
    case SIGINT: 
      {
    	  VERBO(stderr, "%s cancelled. Signal %u caught - Exiting...\n", G_parms.program_name, signo);
    	  if (!G_started || G_exited || G_exiting) {
    		  mysleep(10);
    		  Exit(0);
    	  }
    	  D1HT_exit_nodump(__FILE__, __LINE__, "program cancelled");
     	  Exit(0);
    	  break;
      }
      
    case SIGUSR1: 
    case SIGUSR2:
      { 
    	  // if (!G_exiting) SIGUSR1_thread();
    	  break;
      }
      
    case SIGCONT: 
      { 
    // 	  nothing to do here
    	  break;
      }
     
    default:
      { 
    	  verbose(stderr, "WARNING Cause: Unknown Signal %s\n", msgerr);
    	  break;
      }
 }
 G_xeq = true;

 PTXEQ(pthread_sigmask(SIG_SETMASK, &previous_blockmask, NULL), XEQ_ERROR);
}

struct sigaction signal_handler(const int signo, const sigfunc_t *func, const bool override)
{ 
	struct sigaction	act, oact;

	VERBOSE(stderr, "Setting handler for signal %d\n", signo);

	if (sigaction(signo, NULL, &oact) != 0) EXIT("sigaction");
	
	if (oact.sa_handler != SIG_DFL && oact.sa_handler != SIG_IGN && oact.sa_handler != NULL) {
		verbose(stderr, "WARNING Cause: Handler for signal %d has already been set by the application", signo);
		if (override) {
			if (G_parms.verbose) fprintf(stderr,  " - we will override the signal %d handler already defined with the D1HT signal handler\n", signo);
		} else {
			if (G_parms.verbose) fprintf(stderr,  " - we will not override signal %d handler already defined with the D1HT signal handler\n", signo);
			return oact;
		}
	} else if ((oact.sa_flags | SA_SIGINFO) && oact.sa_sigaction != NULL) {
		verbose(stderr, "WARNING Cause: Handler for signal %d has already been set by the application", signo);
		if (override) {
			if (G_parms.verbose) fprintf(stderr,  " - we will override signal %d handler already defined with the D1HT signal handler\n", signo);
		} else {
			if (G_parms.verbose) fprintf(stderr,  " - we will not override signal %d handler already defined with the D1HT signal handler\n", signo);
			return oact;
		}
	}

	BZERO(act);
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;

	if (func == (sigfunc_t *) SIG_IGN) {
		act.sa_handler = SIG_IGN;
	} else {
		act.sa_sigaction = func;
		act.sa_flags |= SA_SIGINFO;
	}
	
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}

	if (sigaction(signo, &act, NULL) != 0) EXIT("sigaction");
	
	return oact;
}

void signal_handler(const int signo, const sigfunc_t *func) {  
	signal_handler(signo, func, false);
}

void signal_ignore(const int signo, const bool override)
{ 
	signal_handler(signo, (sigfunc_t *) SIG_IGN, override);
}

void signal_ignore(const int signo)
{ 
	signal_handler(signo, (sigfunc_t *) SIG_IGN, true);
}

PTHREAD(signal_handler_thread)
{
	PTHREAD_DETACH;
	PTHREAD_INIT;
	PTHREAD_MAYLOCK;

	sigset_t blockmask;

	XEQ_EXEC(sigfillset(&blockmask), XEQ_ERROR);
	XEQ_EXEC(sigdelset(&blockmask, SIGHUP), XEQ_ERROR);
	XEQ_EXEC(sigdelset(&blockmask, SIGCONT), XEQ_ERROR);

    XEQ_EXEC(pthread_sigmask(SIG_UNBLOCK, &blockmask, NULL), XEQ_ERROR);

	XEQ_EXEC(sigemptyset(&blockmask), XEQ_ERROR);
	XEQ_EXEC(sigaddset(&blockmask, SIGHUP), XEQ_ERROR);
	XEQ_EXEC(sigaddset(&blockmask, SIGCONT), XEQ_ERROR);

	// loop forever handling interrupts, specially SIGUSR1, SIGUSR2, and SIGALARM, with the exception of SIGHUP and SIGCONT
	while (!G_exited) { sigsuspend(&blockmask); }

	return NULL;
}

void block_SIGCONT() {
	  if (G_parms.wait_CONT) {
		  sigset_t local_blockmask;

		  XEQ_EXEC(sigemptyset(&local_blockmask), XEQ_ERROR);

		  XEQ_EXEC(sigaddset(&local_blockmask, SIGCONT), XEQ_ERROR);

		  PTXEQ(pthread_sigmask(SIG_BLOCK, &local_blockmask, NULL), XEQ_ERROR);

		  signal_handler(SIGCONT, SIG_handler, G_parms.wait_CONT);
	  }
}

void unblock_SIGCONT() {
	  if (G_parms.wait_CONT) PTXEQ(pthread_sigmask(SIG_SETMASK, &G_parms.sigmask_original, NULL), XEQ_ERROR);
}

void *set_signal_handlers() {

  VERBOSE(stderr, "Setting signal handlers\n");

  XEQ_EXEC(sigemptyset(&g_blockmask), XEQ_ERROR);

  XEQ_EXEC(sigaddset(&g_blockmask, SIGTERM), 	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGINT), 	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGALRM), 	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGUSR1), 	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGUSR2),	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGCHLD),	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGHUP), 	XEQ_ERROR);
  XEQ_EXEC(sigaddset(&g_blockmask, SIGCONT), 	XEQ_ERROR);

  signal_ignore(SIGPIPE,	true);

  signal_handler(SIGSEGV, 	SIG_handler);
  signal_handler(SIGFPE, 	SIG_handler);
  signal_handler(SIGILL, 	SIG_handler);
  signal_handler(SIGINT, 	SIG_handler);
  signal_handler(SIGTERM, 	SIG_handler);
  signal_handler(SIGABRT, 	SIG_handler);
  signal_handler(SIGBUS, 	SIG_handler);
  signal_handler(SIGHUP, 	SIG_handler);
  signal_handler(SIGUSR1, 	SIG_handler);
  signal_handler(SIGUSR2, 	SIG_handler);

  // only signal_handler_thread will receive SIGALRM, SIGTERM, etc
  G_SIG_thread_id = PTHREAD_CREATE(signal_handler_thread, NULL, &G_detached);

  // all other threads will not receive SIGALRM, SIGTERM, etc
  PTXEQ(pthread_sigmask(SIG_BLOCK, &g_blockmask, NULL), XEQ_ERROR);

  return NULL;
}

} // namespace d1ht
