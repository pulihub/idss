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
 * @file mtime.h
 * @brief gestione dei timestamp, dei tempi, ...
 */


#ifndef TIME_H_
#define TIME_H_

//#define PY(X) 	{ cerr << #X << " = " << X <<  ", file=" << __FILE__ << ", line=" << __LINE__ << endl;}

#define TV2USEC(_tv_time)    	((1000000.0 * ((float) _tv_time.tv_sec))+ ((float) _tv_time.tv_usec))

#define TV2MSEC(_tv_time)     	((1000.0 * ((float) _tv_time.tv_sec)) 	+ (((float) _tv_time.tv_usec) / 1000.0))

#define USEC2TV(_usecs)			({																							\
		timeval _tv_retcode;																								\
		_tv_retcode.tv_sec = (int) (((float) _usecs) / 1000000.0);															\
		_tv_retcode.tv_usec= (int) (((float) _usecs) - (1000000.0 * (float) _tv_retcode.tv_sec)) ;							\
		_tv_retcode;																										\
	})

#define MSEC2TV(_msecs)			({																							\
		timeval _tv_retcode =  USEC2TV((1000.0 * ((float) _msecs));															\
		_tv_retcode;																										\
	})

#define TVDIFF2MSEC(_begin,_end) 	({																						\
		ASSERT(_end.tv_sec >= _begin.tv_sec);																				\
		(float) ((1000.0 * ((float) (_end.tv_sec-_begin.tv_sec))) + (((float) (_end.tv_usec-_begin.tv_usec)) / 1000.0));	\
	})

#define TVDIFF2USEC(_begin,_end) 	({																						\
		ASSERT(_end.tv_sec >= _begin.tv_sec);																				\
		(float) ((1000000.0 * ((float) (_end.tv_sec-_begin.tv_sec))) + ((float) (_end.tv_usec-_begin.tv_usec)));			\
	})

extern float elapsed_msecs();

class times_t {
public:
    int secs_real;
    int msecs_user;
    int msecs_sys;
    int start_time;
    bool time_got;

    times_t() {
        secs_real = 0;
        msecs_user = 0;
        msecs_sys = 0;
        time_got = true;
        start_time = 0;
    }

    void time_start() {
        struct timeval now;
        gettimeofday(&now, NULL);
        start_time = now.tv_sec;
        time_got = false;
    }

    void time_dump_write(const int file_dump_stats);
    void time_dump_read(const int file_dump_stats);

    void time_get() {
        struct timeval now;
        struct rusage usage;
        time_got = true;

        getrusage(RUSAGE_SELF, &usage);
        gettimeofday(&now, NULL);

        secs_real = now.tv_sec - start_time; // VERBO(stderr, "secs_real=%d, now.tv_sec=%d, start_time=%d\n", (int) secs_real, (int) now.tv_sec, (int) start_time);
        msecs_user = (int) TV2MSEC(usage.ru_utime);
        msecs_sys = (int) TV2MSEC(usage.ru_stime);
    }

    times_t operator-(times_t minus) {
        Assert(time_got, XEQ_ERROR);
        Assert(minus.time_got, XEQ_ERROR);

        times_t retcode;

        if (secs_real < minus.secs_real) D1HT_exit_dump(__FILE__, __LINE__, "(secs_real  < minus.secs_real)");
        if (msecs_user < minus.msecs_user) D1HT_exit_dump(__FILE__, __LINE__, "(msecs_user < minus.msecs_user)");
        if (msecs_sys < minus.msecs_sys) D1HT_exit_dump(__FILE__, __LINE__, "(msecs_sys  < minus.msecs_sys)");

        retcode.secs_real = secs_real - minus.secs_real;
        retcode.msecs_user = msecs_user - minus.msecs_user;
        retcode.msecs_sys = msecs_sys - minus.msecs_sys;

        retcode.time_got = true;
        retcode.start_time = 0;

        return retcode;
    }

    times_t operator+(times_t plus) {
        Assert(time_got, XEQ_ERROR);
        Assert(plus.time_got, XEQ_ERROR);

        times_t retcode;

        retcode.secs_real = secs_real + plus.secs_real;
        retcode.msecs_user = msecs_user + plus.msecs_user;
        retcode.msecs_sys = msecs_sys + plus.msecs_sys;

        retcode.time_got = true;
        retcode.start_time = 0;

        return retcode;
    }

    void time_print(FILE* std, const char* header) {

        if (header != NULL) fprintf(std, "%s.", header);
        fprintf(std, "secs_real               =%12lu secs\n", (long unsigned int) secs_real);

        float perc = 0.0;
        float secs = (float) (10.0 * secs_real);

        if (secs > 0.0) perc = ((float) msecs_user) / ((float) secs);
        if (header != NULL) fprintf(std, "%s.", header);
        fprintf(std, "msecs_user              =%12lu msecs (%.2f%%)\n", (long unsigned int) msecs_user, perc);

        if (secs > 0.0) perc = ((float) msecs_sys) / ((float) secs);
        if (header != NULL) fprintf(std, "%s.", header);
        fprintf(std, "msecs_sys               =%12lu msecs (%.2f%%)\n", (long unsigned int) msecs_sys, perc);
    }
};

#endif /*TIME_H_*/
