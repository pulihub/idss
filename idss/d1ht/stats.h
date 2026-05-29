/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             		 *
 *  No title to or ownership of D1HT is hereby transferred.	             	 *
 *									     									 *
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
 * @file stats.h
 * @brief 
 */

#ifndef STATS_H_
#define STATS_H_

#include <values.h>

#if defined(D1HT_PASS_COUNT)

#define MAX_PASS_COUNTERS 64
			
#define PASS_COUNT(_TOT_INDEX) {																				\
	static int local_counter = -1;																				\
	if (local_counter < 0) {																					\
		local_counter = G_stats.total.pass_counter_last;														\
		G_stats.total.pass_counter_last++;																		\
		if (G_stats.total.pass_counter_last < MAX_PASS_COUNTERS) {												\
			strncpy(G_stats.total.pass_counter[local_counter].FILEname, __FILE__, pass_counter_t_FILE_size);	\
			G_stats.total.pass_counter[local_counter].LINE 		= __LINE__;										\
			G_stats.total.pass_counter[local_counter].count 	= 0;											\
			G_stats.total.pass_counter[local_counter].tot_index = _TOT_INDEX;									\
			VERBOSE(stderr,"PASS COUNTER local_counter=%d _TOT_INDEX=%d\n", (int) local_counter, (int) _TOT_INDEX); \
		} else {																								\
			verbose(stderr,"WARNING Cause: Exceeded MAX_PASS_COUNTERS (%d)\n", (int) MAX_PASS_COUNTERS); 		\
		}																										\
	}																											\
	if (local_counter < MAX_PASS_COUNTERS) G_stats.total.pass_counter[local_counter].count++;					\
}
#else
#define MAX_PASS_COUNTERS 0
#define PASS_COUNT(_TOT_INDEX) { (void) _TOT_INDEX; }
#endif

class stat_counter_t {

public:
#define D1HT_STAT_NAME_SIZE	24
	char	name[D1HT_STAT_NAME_SIZE];
	
#define D1HT_STAT_UNIT_SIZE	8
	char	unit[D1HT_STAT_UNIT_SIZE];
	
	NUM_64	count;
	NUM_64	tot_index;
	NUM_64	total;
	NUM_64	OK;
	NUM_64	tried;
	double	value;
	double	lost_time;
	double	min_value;
	double 	max_value;
	NUM_64	max_count;
	int		calculate_min_max;
	bool	xeq;
// temptv and prevtimer MUST be the last 2 variables in stat_counter_t. DONT MOVE THEM!!!!
	struct 	timeval	temptv;
	int 	prevtimer;
	
	void init(const char *sname, const char *sunit, const NUM_32 _tot_index, const bool _xeq) {
		BZERO(unit)
		BZERO(name)
		strncpy(name, sname, sizeof(name)-1);
		strncpy(unit, sunit, sizeof(unit));
		count 		= 0;
		value		= 0.0;
		lost_time	= 0.0;
		tot_index	= _tot_index;
		total 		= 0;
		OK			= 0;
		tried		= 0;
		prevtimer	= 0;;
		temptv.tv_sec= 0;
		xeq			= _xeq;
		
		max_count	= 0;
		max_value	= 0.0;
		min_value	= -1.0;
		calculate_min_max = 1;
	};
	
	void 	max_update(const NUM_64 new_max_count);
	void 	start_time();	
	void 	start_time(const struct timeval *tv_stat);
	double 	stop_time(const bool _OK);
	double 	stop_time(const bool _OK, const struct timeval *now);
	void 	stop_time2(const bool _OK, const double dt);
	double 	add_time(const double new_value);
	double 	add_time(const struct timeval *begin, const struct timeval *end);
	void 	print_counter_str(char sstats[256], const char *tot_name, const float ttotal);
	void 	print_counter(FILE* fstats, const char *header, const char *tot_name, const float ttotal);
	void	dump_counter_write(const int dumpfile);
	void 	dump_counter_read(const int dumpfile);
	
	NUM_32 	stop_time3(const bool _OK, const double dt, const NUM_32 maxcount);
	void 	stop_time3(const bool _OK, const double dt) { stop_time3(_OK, dt, 0); };
	
	double	update_avg(const double _value) {
		value += _value;
		count++;
		return (value / ((double) count));
	}
};

class pass_counter_t {
	
#define 	pass_counter_t_FILE_size 19
	
public:

	char	FILEname[pass_counter_t_FILE_size+1];
	int		LINE;
	int		tot_index;
	uint	count;

	void dump_pass_counter_write(const int dumpfile);
	void dump_pass_counter_read(const int dumpfile);
	void print_pass_counter(FILE* fstats, const char *header, const stat_counter_t *stats_vector);

	pass_counter_t operator+(const pass_counter_t plus) {
#ifndef D1HT_PASS_COUNT
		return plus;
#endif 

		ASSERT(strncmp(FILEname, plus.FILEname, pass_counter_t_FILE_size) == 0);
		ASSERT(LINE == plus.LINE);
		
		pass_counter_t retcode = plus;
		
		retcode.count = count + plus.count;
		
		return retcode;
	}
	
	pass_counter_t operator-(const pass_counter_t minus) {
		
#ifndef D1HT_PASS_COUNT
		return minus;
#endif 

		ASSERT(strncmp(FILEname, minus.FILEname, pass_counter_t_FILE_size) == 0);
		ASSERT(LINE == minus.LINE);
		
		pass_counter_t retcode = minus;	
		
		retcode.count = count - minus.count;
		
		return retcode;
	}
	
	pass_counter_t operator=(const pass_counter_t other) {
#ifndef D1HT_PASS_COUNT
		return other;
#endif 
			
		strncpy(FILEname, other.FILEname, pass_counter_t_FILE_size);
		
		count 		= other.count;
		LINE 		= other.LINE;
		tot_index 	= other.tot_index;
		
		return *this;
	}
	
	bool operator==(const pass_counter_t other) {

#ifndef D1HT_PASS_COUNT
		return true;
#endif 	
		
		if (strncmp(FILEname, other.FILEname, pass_counter_t_FILE_size) != 0) return false;
		
		if (LINE != other.LINE) return false;
		
		return true;		
	}
};

enum estats {
	stat_peers,
	stat_loadavg15,
	stat_memshort,
	stat_duplicated_msgs,
	stat_ack_msgs_sent, 		
	stat_ack_bytes_sent, 		
	stat_maint_msgs_sent, 		
	stat_maint_bytes_sent, 	
	stat_maint_msgs_rxmited, 	
	stat_maint_bytes_rxmited, 		
	stat_noevt_msgs_sent, 		
	stat_noevt_bytes_sent, 		
	stat_noevt_msgs_rxmited, 	
	stat_noevt_bytes_rxmited,
	stat_maintmsgdelays,
	stat_rt_send,
	stat_rt_copy,
	stat_lookup_rate, 		
	stat_lookups_total, 		
	stat_lookups_not1hop,
	stat_lookups_failed,  
	stat_lookup_hops, 
	stat_LookLatency,
    stat_LookLatency_1hop,
    stat_LookLatency_hops,
    stat_LookLatency_1try,
    stat_LookLatency_tries,
	stat_rtt,    			 	
	stat_rto, 
	stat_peer_failures,
	stat_theta,
	stat_theta_xceeded,
	stat_theta_noevt,
	stat_E_tried,
	stat_E_realised,
	stat_erate,
	stat_Savg,
	stat_events_detected,
	stat_joins_acked,
	stat_leaves_acked,
	stat_joins,
	stat_leaves,
	stat_events_generated,
	stat_kill,
	stat_kill_9,
	stat_events_sent, 
#if defined(D1HF_STATS)
	stat_Ttotal,	
	stat_Twait,
	stat_Tread_original,
	stat_Tread,
	stat_Tread_begin,
	stat_Tread_wait_first,
	stat_Tread_wait_other,
	stat_Tread_wait_preftd,
	stat_Tread_data,
	stat_Tread_end,
	stat_Topen,
	stat_Topen_connect,	
	stat_Topen_reconnect,
	stat_Topen_lookup,
	stat_Topen_begin,
	stat_Topen_queue,
	stat_Topen_chunck,
	stat_Topen_cache_xeq,
	stat_Topen_cache_creat,
	stat_Topen_final,
	stat_Topen_final_ahead,
	stat_Topen_wait_first,
	stat_Topen_wait_other,
	stat_Topen_delay,
	stat_Fread,
	stat_Fread_original,
	stat_Fread_skipped,
	stat_Fread_forced_local,
	stat_Fread_big,
	stat_Fread_old_pended,
	stat_Fread_open_delay,
	stat_Fread_chunck,
	stat_Fread_old,
	stat_Fread_old_pending,
	stat_Fread_not_created,
	stat_Fread_ahead,
	stat_Fread_hits,
	stat_Fread_misses,
	stat_checksum_failed,
	stat_Fread_timeout_maj,
	stat_Fread_timeout_1st,
	stat_Fread_timeout_min,
	stat_Fread_timeout_avg,
	stat_Fopen,
	stat_chuncks_failed,
	stat_Fopen_reopen,
	stat_Fopen_delegated,
	stat_Fopen_self,
	stat_Fopen_final,
	stat_Fopen_final_ahead,
	stat_Fopen_timeout,
	stat_Fopen_skipped,
	stat_Fopen_cold,
	stat_Fopen_rtried,
	stat_Fopen_ahead,
	stat_Fopen_unused,
	stat_Fopen_connecting,
	stat_Fopen_connectingRe,
	stat_Fopen_delay,
	stat_Fopen_delay_queue,
	stat_fileforcelocal,
	stat_Flookup,
	stat_Freconnect,
	stat_Freconnect_back,
	stat_connections,
	stat_Fclose,
	stat_Topen_base,
	stat_Tread_base,
	stat_Fread_base,
	stat_temp0,
	stat_temp1,
	stat_temp2,
	stat_temp3,
	stat_temp4,
	stat_temp5,
	stat_time_temp1,
	stat_time_temp2,
	stat_time_temp3,
	stat_time_temp4,
#else
	stat_temp0,
	stat_temp1,
	stat_temp2,
	stat_temp3,
#endif
#if defined(XEQ_WRITE_BLOCKING)
	stat_writes_block_check,
#endif
#if defined(D1HT_PTHREAD_STATS)
	stat_pthread_created,
	stat_pthread_started,	
	stat_pthread_ended,	
	stat_pthread_conc,
	stat_created_d1ht_main_thread,
	stat_created_close_interval_exit,
	stat_created_event_msg_handler,
	stat_created_file_connection_handler,
	stat_created_lookup_thread,
	stat_created_probe_and_propagate,
	stat_created_probe_pthread,
	stat_created_send_main_msg_attached,
	stat_created_signal_handler_thread,
	stat_created_SIGUSR1_thread_handler,
	stat_created_stab_detached,
	stat_created_TCP_general_connection_handler,
	stat_created_TCP_server,
	stat_created_theta_interval,
	stat_created_UDP_lookup_server,
	stat_created_UDP_server,
	stat_started_d1ht_main_thread,
	stat_started_close_interval_exit,
	stat_started_event_msg_handler,
	stat_started_file_connection_handler,
	stat_started_lookup_thread,
	stat_started_probe_and_propagate,
	stat_started_probe_pthread,
	stat_started_send_main_msg_attached,
	stat_started_signal_handler_thread,
	stat_started_SIGUSR1_thread_handler,
	stat_started_stab_detached,
	stat_started_TCP_general_connection_handler,
	stat_started_TCP_server,
	stat_started_theta_interval,
	stat_started_UDP_lookup_server,
	stat_started_UDP_server,
	stat_ended_d1ht_main_thread,
	stat_ended_close_interval_exit,
	stat_ended_event_msg_handler,
	stat_ended_file_connection_handler,
	stat_ended_lookup_thread,
	stat_ended_probe_and_propagate,
	stat_ended_probe_pthread,
	stat_ended_send_main_msg_attached,
	stat_ended_signal_handler_thread,
	stat_ended_SIGUSR1_thread_handler,
	stat_ended_stab_detached,
	stat_ended_TCP_general_connection_handler,
	stat_ended_TCP_server,
	stat_ended_theta_interval,
	stat_ended_UDP_lookup_server,
	stat_ended_UDP_server,
#endif
	stat_LAST
};

#define	MAX_STATS	stat_LAST+1

#define STATS_MACRO(STATS_NAME, STATS_UNIT, STATS_XEQ, STATS_TOT) {									\
	if (stat_check_init[STATS_NAME] != 0) {															\
		fprintf(stderr, "ERROR Cause: Statistics %s is being initialized twice\n",  #STATS_NAME);	\
		exit(EXIT_FAILURE);																			\
	} else {																						\
		stat_check_init[STATS_NAME] = 1;															\
		stat_initialized++;																			\
	} 																								\
	char temp[80];																					\
	strncpy(temp, #STATS_NAME, sizeof(temp));														\
	stats[STATS_NAME].init(&(temp[5]), STATS_UNIT, STATS_TOT, STATS_XEQ);							\
}
#if defined(D1HT_PTHREAD_STATS)
#define STATS_THREAD(THREAD_NAME) {																	\
	STATS_MACRO(stat_created_ ## THREAD_NAME, " ", stat_pthread_created, false );					\
	STATS_MACRO(stat_started_ ## THREAD_NAME, " ", stat_created_ ## THREAD_NAME, false);			\
	STATS_MACRO(stat_ended_   ## THREAD_NAME, " ", stat_started_ ## THREAD_NAME, false);			\
}
#endif 

class stat_part_t {
public:

	int				timer_index;
	struct timeval	timer;  
	stat_counter_t 	stats[MAX_STATS];
	times_t			times;													// protected by stat_mutex
	
	pass_counter_t  pass_counter[MAX_PASS_COUNTERS + 2];
	int				pass_counter_last;

	void 	stat_print(FILE* std, const char* header);	
	
	int  	stat_dump_write_begin(const int phase_count, const char* hostname); 
	void 	stat_dump_write_begin(const int file_dump_stats, const int phase_count, const char* hostname);
	void 	stat_dump_write(const int file_dump_stats);
	int  	stat_dump_read_begin(const int file_dump_stats, int *phase_count, char* hostname);
	void 	stat_dump_read(const int file_dump_stats);	

	void 	stat_time_start3(const char* filename, const uint linenumber, const int stat_index);
	void 	stat_time_switch3(const char* filename, const uint linenumber, const int stat_index, const bool _OK);
	double 	stat_time_restore3(const char* filename, const uint linenumber, const bool _OK);
	
	void 	stat_time_stop3(const bool _OK, const double dt);
	double 	stat_time_stop3(const bool _OK, const bool xeq);
	double 	stat_time_stop3(const bool _OK) 	{ return stat_time_stop3(_OK, false); }
	double 	stat_time_stop3_try(const bool _OK)	{ return stat_time_stop3(_OK, true);  }
	void 	stat_time_clean3();

	stat_part_t operator-(const stat_part_t minus);
	stat_part_t operator+(const stat_part_t plus);	
	void compare(const stat_part_t other);
	
	stat_part_t() 		{ init(); }
	
	void new_event() 	{ stats[stat_events_generated].count++; }
	void failure() 		{ stats[stat_peer_failures].count++; 	}
	
	void* init() {
		
		Assert(MAX_STATS > stat_LAST, XEQ_ERROR);

		int stat_check_init[MAX_STATS];
		BZERO(stat_check_init);
		int	stat_initialized = 0;
		
		BZERO(pass_counter);
		pass_counter_last = 0;
		
		BZERO(timer);
		timer_index 	= 0;
		times.time_got 	= true;

		STATS_MACRO(stat_LAST, 				"     ", 	false, 0);   					//
		STATS_MACRO(stat_peers, 			"peers", 	false, 0);   					//

		stats[stat_peers].count = 1;
		
		STATS_MACRO(stat_loadavg15, 		"tasks", 	false, 0);   					//
		STATS_MACRO(stat_memshort, 			"     ", 	false, 0);   					// 
		STATS_MACRO(stat_E_tried, 			"evnts", 	false, 0);   					// protected by theta_mutex
		STATS_MACRO(stat_E_realised, 		"evnts", 	false, stat_E_tried);   		// unprotected
        STATS_MACRO(stat_theta,				"msec ",	false, 0);            			// protected by G_interval_mutex
        STATS_MACRO(stat_theta_xceeded,		"msec ",	false, stat_theta);          	// protected by theta_mutex
        STATS_MACRO(stat_theta_noevt,		"msec ",	false, stat_theta);          	// protected by G_interval_mutex
		STATS_MACRO(stat_erate,				"e/sec", 	false, 0);   					// protected by theta_mutex
		STATS_MACRO(stat_Savg,				"secs ", 	false, 0);   					// protected by theta_mutex
		STATS_MACRO(stat_rtt, 				"msecs", 	false, 0);   			 		// protected by rtt_mutex
		STATS_MACRO(stat_rto, 				"msecs", 	false, 0);   			 		// protected by rtt_mutex
		STATS_MACRO(stat_duplicated_msgs, 	"     ", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_ack_msgs_sent, 	"     ", 	false, 0);						// protected by stat_mutexopen
		STATS_MACRO(stat_ack_bytes_sent, 	"bytes", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_maint_msgs_sent, 	"     ", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_maint_msgs_rxmited,"     ", 	false, stat_maint_msgs_sent);	// protected by stat_mutex
		STATS_MACRO(stat_maint_bytes_sent, 	"bytes", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_maint_bytes_rxmited,"bytes", 	false, stat_maint_bytes_sent);	// protected by stat_mutex
		STATS_MACRO(stat_noevt_msgs_sent, 	"     ", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_noevt_msgs_rxmited,"     ", 	false, stat_noevt_msgs_sent);	// protected by stat_mutex
		STATS_MACRO(stat_noevt_bytes_sent, 	"bytes", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_noevt_bytes_rxmited,"bytes", 	false, stat_noevt_bytes_sent);	// protected by stat_mutex
		STATS_MACRO(stat_maintmsgdelays,	"msec ",	false, 0);						// unprotected

		STATS_MACRO(stat_rt_send, 			"msecs", 	true,  0);    					// unprotected
		STATS_MACRO(stat_rt_copy, 			"msecs", 	true,  stat_rt_send);    		// unprotected
		
		STATS_MACRO(stat_lookup_hops, 		"     ", 	true,  0);   					// protected by stat_mutex
		STATS_MACRO(stat_lookups_total, 	"     ", 	true,  0);						// protected by stat_mutex
		STATS_MACRO(stat_lookup_rate, 		"lkps ", 	true,  0);						// protected by stat_mutex
        STATS_MACRO(stat_LookLatency,		"msec ",	false, 0);                     	// protected by stat_mutex
        STATS_MACRO(stat_LookLatency_hops,	"msec ",	false, stat_LookLatency);   	// protected by stat_mutex
        STATS_MACRO(stat_LookLatency_1hop,	"msec ",	false, stat_LookLatency);   	// protected by stat_mutex
        STATS_MACRO(stat_LookLatency_1try,	"msec ",	false, stat_LookLatency_1hop);	// unprotected
        STATS_MACRO(stat_LookLatency_tries,	"msec ",	false, stat_LookLatency_1hop);	// unprotected

		STATS_MACRO(stat_lookups_not1hop,	"     ", 	true,  stat_lookups_total); 	// protected by stat_mutex
		STATS_MACRO(stat_lookups_failed, 	"     ", 	false, stat_lookups_total);  	// protected by stat_mutex
		
		STATS_MACRO(stat_events_detected, 	"     ", 	false, stat_events_generated);	// protected by rt_mutex
		STATS_MACRO(stat_joins_acked, 		"     ", 	false, stat_events_detected);	// protected by rt_mutex
		STATS_MACRO(stat_leaves_acked, 		"     ", 	false, stat_events_detected);	// protected by rt_mutex
		STATS_MACRO(stat_joins, 			"     ", 	false, stat_joins_acked);		// protected by rt_mutex
		STATS_MACRO(stat_leaves, 			"     ", 	false, stat_leaves_acked);		// unprotected
			
		STATS_MACRO(stat_events_generated, 	"evnts", 	false, stat_events_detected);  	// protected by wrap_stats_mutex
		STATS_MACRO(stat_kill, 				"evnts", 	false, stat_events_generated); 	// protected by wrap_stats_mutex
		STATS_MACRO(stat_kill_9, 			"evnts", 	false, stat_events_generated); 	// protected by wrap_stats_mutex
		STATS_MACRO(stat_events_sent, 		"evnts", 	false, 0);						// protected by stat_mutex
		STATS_MACRO(stat_peer_failures, 	"     ", 	false, stat_events_generated); 	// protected by wrap_stats_mutex
#if defined(D1HF_STATS)			
		STATS_MACRO(stat_Topen_connect, 	"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_reconnect, 	"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_lookup, 		"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_begin, 		"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_queue, 		"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_chunck, 		"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_cache_xeq,	"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_cache_creat, "msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_final, 		"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_final_ahead,	"msecs", 	true,  stat_Topen);    			// unprotected
		STATS_MACRO(stat_Topen_wait_first,	"msecs", 	true,  0);    					// unprotected
		STATS_MACRO(stat_Topen_wait_other,	"msecs", 	true,  0); 						// unprotected
		STATS_MACRO(stat_Topen_delay,		"msecs", 	true,  stat_Topen); 			// unprotected
		STATS_MACRO(stat_Topen, 			"msecs", 	true,  stat_Ttotal);    		// unprotected
		STATS_MACRO(stat_Ttotal, 			"msecs", 	false, 0);   			 		// unprotected
		STATS_MACRO(stat_Twait, 			"msecs", 	true,  stat_Ttotal);    		// unprotected
		STATS_MACRO(stat_Tread_original, 	"msecs", 	true,  stat_Ttotal);   			// unprotected
		STATS_MACRO(stat_Tread, 			"msecs", 	true,  stat_Ttotal);    		// unprotected
		STATS_MACRO(stat_Tread_begin,		"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Tread_wait_first,	"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Tread_wait_other,	"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Tread_wait_preftd,	"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Tread_data,		"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Tread_end,			"msecs", 	true,  stat_Tread);    			// unprotected
		STATS_MACRO(stat_Fread_chunck,		"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fread_original, 	"     ", 	false, stat_Fread_chunck);  	// unprotected
		STATS_MACRO(stat_Fread_skipped, 	"     ", 	false, stat_Fread_original);  	// unprotected
		STATS_MACRO(stat_Fread_forced_local,"     ", 	false, stat_Fread_original);  	// unprotected
		STATS_MACRO(stat_Fread_big,			"     ", 	false, stat_Fread_original);  	// unprotected
		STATS_MACRO(stat_Fread_old_pended,	"     ", 	false, stat_Fread_original);  	// unprotected
		STATS_MACRO(stat_Fread_open_delay,	"     ", 	false, stat_Fread_original);  	// unprotected
		STATS_MACRO(stat_Fread_ahead,		"     ", 	false, stat_Fread_chunck);    	// unprotected
		STATS_MACRO(stat_Fread_hits, 		"     ", 	false, stat_Fread_ahead);  		// unprotected
		STATS_MACRO(stat_Fread_misses, 		"     ", 	false, stat_Fread_ahead);  		// unprotected
		STATS_MACRO(stat_Fread_not_created, "     ", 	false, stat_Fread_chunck);  	// unprotected
		STATS_MACRO(stat_Fread_timeout_avg,	"msecs", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fread_timeout_1st,	"     ", 	false, stat_Fread_timeout_maj); // unprotected
		STATS_MACRO(stat_Fread_timeout_maj,	"     ", 	false, stat_Fread_chunck);    	// unprotected
		STATS_MACRO(stat_Fread_timeout_min,	"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fread_old,			"     ", 	false, stat_Fread_chunck);    	// unprotected
		STATS_MACRO(stat_Fread_old_pending,	"     ", 	false, stat_Fread_chunck);  	// unprotected
		STATS_MACRO(stat_checksum_failed,	"     ", 	false, stat_Fread_chunck);    	// unprotected
		STATS_MACRO(stat_Fread,				"MB	  ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fopen,				"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fopen_delegated,	"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_self,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_final,		"     ", 	false, stat_Fopen);    			// unprotected
		STATS_MACRO(stat_Fopen_reopen,		"     ", 	false, stat_Fopen);    			// unprotected
		STATS_MACRO(stat_Fopen_ahead,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_final_ahead,	"     ", 	false, stat_Fopen_ahead);    	// unprotected
		STATS_MACRO(stat_Fopen_unused,		"     ", 	false, stat_Fopen_ahead);		// unprotected
		STATS_MACRO(stat_Fopen_connecting,	"     ", 	false, stat_Fopen_ahead); 		// unprotected
		STATS_MACRO(stat_Fopen_connectingRe,"     ", 	false, stat_Fopen_connecting); 	// unprotected
		STATS_MACRO(stat_Fopen_timeout,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_skipped,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_cold,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_rtried,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_delay,		"     ", 	false, stat_Fopen); 			// unprotected
		STATS_MACRO(stat_Fopen_delay_queue,	"     ", 	false, stat_Fopen_delay); 		// unprotected
		STATS_MACRO(stat_chuncks_failed,	"     ", 	false, stat_Fopen);  			// unprotected
		STATS_MACRO(stat_fileforcelocal,	"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Fclose, 			"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Flookup, 			"     ", 	false, stat_Fopen);    			// unprotected
		STATS_MACRO(stat_Freconnect, 		"     ", 	false, stat_Fopen);    			// unprotected
		STATS_MACRO(stat_Freconnect_back, 	"     ", 	false, stat_Freconnect);    	// unprotected
		STATS_MACRO(stat_connections, 		"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_Topen_base,		"msecs", 	false, 0);   			 		// unprotected
		STATS_MACRO(stat_Tread_base,		"msecs", 	false, 0);   			 		// unprotected
		STATS_MACRO(stat_Fread_base,		"MB	  ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_temp0,				"     ", 	false, 0);						// unprotected
		STATS_MACRO(stat_temp1,				"     ", 	false, stat_Fread_skipped);		// unprotected
		STATS_MACRO(stat_temp2,				"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_temp3,				"     ", 	false, stat_Fread_skipped);    	// unprotected
		STATS_MACRO(stat_temp4,				"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_temp5,				"     ", 	false, stat_Fopen_delegated);   // unprotected
		STATS_MACRO(stat_time_temp1,		"msecs", 	false, 0);    					// unprotected
		STATS_MACRO(stat_time_temp2,		"msecs", 	false, 0);    					// unprotected
		STATS_MACRO(stat_time_temp3,		"msecs", 	false, 0);    					// unprotected
		STATS_MACRO(stat_time_temp4,		"msecs", 	false, 0);    					// unprotected
#else
		STATS_MACRO(stat_temp0,				"     ", 	false, 0);    					// unprotected
		STATS_MACRO(stat_temp1,				"     ", 	false, stat_temp0);    			// unprotected
		STATS_MACRO(stat_temp2,				"     ", 	false, stat_temp0);    			// unprotected
		STATS_MACRO(stat_temp3,				"     ", 	false, stat_lookups_total);    	// unprotected
#endif
#if defined(XEQ_WRITE_BLOCKING)
		STATS_MACRO(stat_writes_block_check, 	"     ", 	false, 0);					// unprotected
#endif
#if defined(D1HT_PTHREAD_STATS)
		STATS_MACRO(stat_pthread_created, 	"     ", 	false, 0);                 		// protected by G_thread_data_mutex
		STATS_MACRO(stat_pthread_started, 	"     ", 	false, stat_pthread_created);  	// protected by G_thread_data_mutex
		STATS_MACRO(stat_pthread_ended, 	"     ", 	false, stat_pthread_created);  	// protected by G_thread_data_mutex
		STATS_MACRO(stat_pthread_conc, 		"     ", 	false, 0);                 		// protected by G_thread_data_mutex	

		STATS_THREAD(d1ht_main_thread);
		STATS_THREAD(close_interval_exit);
		STATS_THREAD(event_msg_handler);
		STATS_THREAD(file_connection_handler);
		STATS_THREAD(lookup_thread);
		STATS_THREAD(probe_and_propagate);
		STATS_THREAD(probe_pthread);
		STATS_THREAD(send_main_msg_attached);
		STATS_THREAD(signal_handler_thread);
		STATS_THREAD(SIGUSR1_thread_handler);
		STATS_THREAD(stab_detached);
		STATS_THREAD(TCP_general_connection_handler);
		STATS_THREAD(TCP_server);
		STATS_THREAD(theta_interval);
		STATS_THREAD(UDP_lookup_server);
		STATS_THREAD(UDP_server);
#endif

		if (stat_initialized != (stat_LAST+1)) {
			if (stat_initialized > (stat_LAST+1)) {
				fprintf(stderr, "\n FILE=%s LINE=%d ERROR Cause: Too many statistics were initialised\n", __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			}
			int count_error = stat_LAST - stat_initialized + 1; PE(count_error)
			int count = 0;
			for (int i=0; i < stat_LAST && count < count_error; i++) { PE(i)
				if (stat_check_init[i] == 0) {
					fprintf(stderr, "FILE=%s LINE=%d ERROR Cause: Statistic not initialised - previous=%s, next=%s\n",
							__FILE__, __LINE__, stats[i-1].name, stats[i+1].name);
					count++;
					if (count_error == count) {
						fprintf(stderr, "FILE=%s LINE=%d ERROR Cause: %d statistics not initialised\n", __FILE__, __LINE__, count_error);
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		return NULL;
	}
};

class stat_t {
	FILE 			*fstats;
	unsigned		version;
	char			filename[256];
	stat_part_t		last;
	
public:
	int 			stats_connection;
	stat_part_t		total;
	lock_t			stat_mutex;
	
	void print_stat(FILE* std);
	void diff(stat_part_t* diff);
	void *start_stats() {return start_stats(0);}
	void *start_stats(FILE* std);
	void *exiting_stats(FILE* std);
	void shutdown();
	
	stat_t() {
		fstats 				= NULL;
		version 			= 0;
		stats_connection 	= -1;
		LOCK_INIT(stat_mutex, maychain=false, 5); 
	}

	bool stat_connect(const int new_connection) {
		bool retcode = false;
		LOCK(stat_mutex);
		
		if (new_connection != -1 && stats_connection != new_connection) mysleep(5);
		
		if (new_connection == -1) {
			stats_connection 	= -1;
			retcode 			= true;
		} else if (stats_connection == -1) {
			stats_connection 	= new_connection;
			retcode 	= true;
		} else if (stats_connection == new_connection) {
			retcode 	= true;
		} 
		UNLOCK(stat_mutex);
		return retcode;
	}
};

#if defined(XEQ_WRITE_BLOCKING)
#include <poll.h>
#define XEQ_WRITE_BLOCK(_socket) {														\
	STAT_TRIED(stat_writes_block_check);												\
	pollfd pollwrite;																	\
	pollwrite.fd 	= _socket;															\
	pollwrite.events= POLLOUT;															\
	poll(&pollwrite, 1, 0);																\
	if ((pollwrite.revents & POLLOUT) || (pollwrite.revents & POLLWRNORM) || 			\
		(pollwrite.revents & POLLERR) || (pollwrite.revents & POLLHUP) 	  || 			\
		(pollwrite.revents & POLLNVAL) ) {												\
		STAT_OK(stat_writes_block_check);												\
	} else {																			\
		PASS_COUNT(stat_writes_block_check);											\
	}																					\
}
#else
#define XEQ_WRITE_BLOCK(_socket) { (void) _socket; }
#endif

extern 	stat_t	G_stats;

#endif /*STATS_H_*/
