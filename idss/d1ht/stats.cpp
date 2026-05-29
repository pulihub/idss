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
 * @file stats.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht {

stat_t	G_stats;

double stat_counter_t::add_time(const struct timeval *begin, const struct timeval *end) {

	ASSERT(end->tv_sec > begin->tv_sec || end->tv_usec >= begin->tv_usec);

	double new_value = TVDIFF2MSEC((*begin),(*end));

	add_time(new_value);

	return value;
}

double stat_counter_t::add_time(const double new_value) {

	ASSERT(new_value >= 0.0);

	value += new_value;
	
	count++;
	
	return value;
}

void stat_counter_t::start_time() {
	gettimeofday(&temptv, NULL);
	tried++;
}

void stat_counter_t::start_time(const struct timeval *tv_time) {
#if defined(DEBUG)
	Assert((temptv.tv_sec == 0), XEQ_WARNING, "time has not been closed since last start");
#endif
	temptv.tv_sec = tv_time->tv_sec;
	temptv.tv_usec= tv_time->tv_usec;
	tried++;
}

double stat_counter_t::stop_time(const bool _OK, const struct timeval *now) {
	
//	ASSERT(temptv.tv_sec != 0);
	
	double dt = TVDIFF2MSEC(temptv,(*now));
	
	value += dt;
	
	count++;
	
	if (_OK) {
		OK++;
		count++;
	} else {
		lost_time += dt;
	}
	
	temptv.tv_sec = 0;
	
	return dt;
}

double stat_counter_t::stop_time(const bool _OK) {

	struct timeval	now;  
	
	gettimeofday(&now, NULL);

	return stop_time(_OK, &now);
}

char *print_uint(const NUM_64 count)
{
    static char str[6];
	if (count < 100000) {
		sprintf(str, "%5u",  (uint) count);
	} else if (count < 10000000) {
		sprintf(str, "%4uK", (uint) (count / 1000));
	} else {
		sprintf(str, "%4uM", (uint) (count / 1000000));
	} 
    return(str);
}

char *print_double(const double value)
{
	static char	svalue[6];
	if (value < 10.0) {
		sprintf(svalue, "%5.3f", value);
	} else if (value < 1000.0) {
		sprintf(svalue, "%5.1f", value);
	} else if (value >= 10000000000.0) {
		sprintf(svalue, "%4uG", (uint) (((NUM_64) value) / 1000000000));
	} else {
		return (print_uint((NUM_64) value));
	}		
	return(svalue);
}

void stat_counter_t::print_counter(FILE* fstats, const char *header, const char *tot_name, const float ttotal) {

	if (header != NULL) fprintf(fstats, "%s.", header);
	fprintf(fstats, "%-19s", name);

	char sstats[256];
	print_counter_str(sstats, tot_name, ttotal);
	fprintf(fstats, "%s\n", sstats);
}

void stat_counter_t::print_counter_str(char sstats[256], const char *tot_name, const float ttotal) {

	int ttried = MAX(OK,tried);
	int ccount = (int) count;
	if (ccount == 0) ccount = tried;

//	VERBO( stderr, "printing %s.%s stats: value=%12.4f, count=%10lu, max_count=%10lu, total=%10lu, calculate_min_max=%d, min_value=%12.4f, max_value=%12.4f\n",
//		header, name, value, count, max_count, total, (int) calculate_min_max, min_value, max_value);

	float 	vvalue = value;
	char* 	tstats = sstats;
	int   	bytes;
	int   	bytes_value = 0;
	int   	bytes_count = 0;
	char 	str_value[40];
	char 	str_count[40];

#define SPRINTF( ... ) { bytes = sprintf(tstats, __VA_ARGS__); tstats+=bytes; }

	if (value > 0.0 && ccount > 0) {
		float avg = ((float) value) / ((float) ccount);
		SPRINTF("(AVG)= %-5s %s", print_double(avg), unit);
		bytes_value = sprintf(str_value, ", value=%-5s", print_double(value));
		bytes_count = sprintf(str_count, ", count=%-5s", print_uint(ccount));
	} else if (value > 0.0 && OK > 0) {
		float avg = ((float) value) / ((float) OK);
		SPRINTF("(AVG)= %-5s %s",  print_double(avg), unit);
		bytes_value = sprintf(str_value, ", value=%-5s", print_double(value));
		bytes_count = sprintf(str_count, ", OK=%-5s", print_uint(OK));
	} else if (value > 0.0) {
		SPRINTF("     = %-5s %s",  print_double(value), unit);
	} else if (max_count != 0) {
		ASSERT(tot_index== 0);
		ASSERT(ttotal 	== 0.0);
		SPRINTF("(MAX)= %-5s %s",  print_uint(max_count), unit);
	} else {
		vvalue = (float) ccount;
		SPRINTF("     = %-5s %s",  print_uint(ccount), unit);
	}
	if (ttotal != 0.0) {
		float perc = 100.0 * vvalue / ttotal;
		SPRINTF(", %3u%% of %-6s", (NUM_32) perc, tot_name);
	}
	if (ttried != 0) {
		SPRINTF(", OK=%3u%%", (uint) (100*(OK) / (ttried)));
	}
	ASSERT((!(lost_time > 0.0 && vvalue == 0.0)))

	if (bytes_value > 0) SPRINTF("%s", str_value);
	if (bytes_count > 0) SPRINTF("%s", str_count);

	if (vvalue > 0.0 && lost_time > 0.0) {
		float perc = 100.0 * ((float) lost_time) / vvalue;
		SPRINTF(", lost_time=%3u%%", (NUM_32) perc);
	}

	if (value > 0.0 && calculate_min_max == 1) {
		if (min_value > 0.0)	SPRINTF(", min=%-5s", print_double(min_value));
		if (max_value > 0.0) 	SPRINTF(", max=%-5s (%lu times)", print_double(max_value), (ulong) max_count);
	}
};

void pass_counter_t::print_pass_counter(FILE* fstats, const char *header, const stat_counter_t *stats_vector) {
	
	if (count > 0) {
	
		if (header != NULL) fprintf(fstats, "%s.", header);
  	
		fprintf(fstats, "FILE=%-20s LINE=%4d count=%10u", FILEname, LINE, count); 
		
		if (tot_index != 0) {
			
			float max_count = (float) MAX(stats_vector->count, stats_vector->tried);

			if (max_count > 0.0) {
				float perc = 100.0 * ((float) count) / max_count;
				fprintf(fstats,", %3u%% of %-6s", (NUM_32) perc, stats_vector->name);
			}
		}
		
		fprintf(fstats,"\n");
	}
}

void stat_counter_t::max_update(const NUM_64 new_max_count) {
	ASSERT(value== 0.0);
	ASSERT(count== 0);
	ASSERT(tried== 0);
	ASSERT(OK 	== 0);
	
	ASSERT(tot_index == 0);
	
	calculate_min_max = 0;

	max_count = MAX(new_max_count, max_count); 
}

#define	DUMP_COUNTER(OP, VAR) {												\
	int nbytes;																\
	XEQ_EXEC( (nbytes=OP(dumpfile, &VAR, sizeof(VAR))), XEQ_WARNING);		\
	if (nbytes <= 0) {														\
		D1HT_CLOSE(dumpfile);													\
		return;																\
	}																		\
	hash += ((double) VAR);													\
 /* fprintf(stderr,"%s val=%ld,%f hash=%f\n", #VAR, ((long) VAR), ((double) VAR), hash); */ \
}

void stat_counter_t::dump_counter_write(const int dumpfile) {

#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not dumping statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	XEQ_EXEC( writen(dumpfile, name, sizeof(name)), XEQ_WARNING);
	XEQ_EXEC( writen(dumpfile, unit, sizeof(unit)), XEQ_WARNING);
	
	double hash = 0.0;
	DUMP_COUNTER(writen, count);
	DUMP_COUNTER(writen, tot_index);
	DUMP_COUNTER(writen, total);
	DUMP_COUNTER(writen, OK);
	DUMP_COUNTER(writen, tried);
	DUMP_COUNTER(writen, value);
	DUMP_COUNTER(writen, lost_time);
	DUMP_COUNTER(writen, min_value);
	DUMP_COUNTER(writen, max_value);
	DUMP_COUNTER(writen, max_count);
	DUMP_COUNTER(writen, calculate_min_max);
	DUMP_COUNTER(writen, xeq);
	
	DUMP_COUNTER(writen, hash);
}

void stat_counter_t::dump_counter_read(const int dumpfile) {

	char	xeq_name[D1HT_STAT_NAME_SIZE];
	char	xeq_unit[D1HT_STAT_UNIT_SIZE];
	
	ASSERT(sizeof(xeq_name) == sizeof(name));
	ASSERT(sizeof(xeq_unit) == sizeof(unit));

	XEQ_EXEC( readn(dumpfile, xeq_name, sizeof(xeq_name)), XEQ_ERROR);
		
	if (strncmp(name, xeq_name, sizeof(name))) {
		EXIT("unknown stat %s (it should be %s)\n", xeq_name, name);
	}

	XEQ_EXEC( readn(dumpfile, xeq_unit, sizeof(xeq_unit)), XEQ_ERROR);
	
	if (strncmp(unit, xeq_unit, sizeof(unit))) {
		EXIT("invalid unit (%s) for stat %s (it should be %s)\n", xeq_unit, name, unit);
	}

	double hash = 0.0;
	
	DUMP_COUNTER(readn, count);
	DUMP_COUNTER(readn, tot_index);
	DUMP_COUNTER(readn, total);
	DUMP_COUNTER(readn, OK);
	DUMP_COUNTER(readn, tried);
	DUMP_COUNTER(readn, value);
	DUMP_COUNTER(readn, lost_time);
	DUMP_COUNTER(readn, min_value);
	DUMP_COUNTER(readn, max_value);
	DUMP_COUNTER(readn, max_count);
	DUMP_COUNTER(readn, calculate_min_max);
	DUMP_COUNTER(readn, xeq);
	
	double hash_test = 0;
	
	XEQ_EXEC( readn(dumpfile, &hash_test, sizeof(hash_test)), XEQ_ERROR);
	
	// fprintf(stderr,"read %s count=%lu, hash=%f, hash_test=%f\n", name, (ulong) count, hash, hash_test);

	Assert(hash == hash_test, XEQ_ERROR);
}

void stat_part_t::stat_print(FILE* fstats, const char* header) {

#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not prining statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	if (!G_parms.print_stats) return;

 	times.time_print(fstats, header);

 	for (int i = 0; i < stat_LAST; i++) {  PE(i)
 		
#if defined(D1HF_STATS)
 		if ( i > stat_memshort && i < stat_Ttotal) continue;
#endif
 		char	tot_name[24];
 		float 	ttotal;
 		ttotal = 0.0;

#if defined(DEBUG)
 		if (stats[i].tot_index >= stat_LAST) EXIT("Invalid tot_index=%ld for stats=%s\n", (long) stats[i].tot_index, stats[i].name)
#endif

 		if (i==stat_lookup_rate && stats[i].value==0.0) {
 			stats[i].value = (double) stats[stat_lookups_total].count ;
 			if (stats[i].value==0.0) {
 				stats[i].count = 0;
 			} else {
 	 			stats[i].count = times.secs_real;
 			}
 		} else if (stats[i].tot_index) {
 			strncpy(tot_name, stats[stats[i].tot_index].name, sizeof(tot_name)-1);
 			if (stats[i].value > 0.0) {
 				ttotal = (float) stats[stats[i].tot_index].value;
 			} else {
 				ttotal = (float) MAX(stats[stats[i].tot_index].count,stats[stats[i].tot_index].tried);			
 			}
 		} else if (stats[i].total != 0) {
 			sprintf(tot_name, "%ld", (long) stats[i].total);
 		}
 		VERBOSE(stderr,"stat name=%s, count=%ld, tried=%ld, OK=%lu, value=%12.4f, tot_index=%ld, ttotal=%12.4f, max_count=%ld\n",
 				stats[i].name, (long) stats[i].count, (long) (long) stats[i].tried, (ulong) stats[i].OK, stats[i].value, (long) stats[i].tot_index, ttotal, (long) stats[i].max_count);

 		stats[i].print_counter(fstats, header, tot_name, ttotal);
 	}

 	for (int i = 0; i < pass_counter_last; i++) { PE(i)
  		pass_counter[i].print_pass_counter(fstats, header, &(stats[pass_counter[i].tot_index])); 
 	}
 	
#if defined(D1HF_STATS)
	if (stats[stat_Fread].value > 0.0 && times.secs_real > 0) {
		float MBS  = ((float) stats[stat_Fread].value) / ((float) times.secs_real);
		float IOPS = ((float) stats[stat_Fread].count) / ((float) times.secs_real);
		fprintf(fstats, "filesystem peer avg read = %5.0f IOPS, %5.1f MB/s\n", IOPS, MBS);

		MBS  = MBS  * ((float) stats[stat_peers].count);
		IOPS = IOPS * ((float) stats[stat_peers].count);
		fprintf(fstats, "filesystem TOT read      = %5.0f IOPS, %5.1f MB/s\n", IOPS, MBS);
	}
	
	if (stats[stat_Fread_base].value > 0.0 && times.secs_real > 0) {
		float MBS  = ((float) stats[stat_Fread_base].value) / ((float) times.secs_real);
		float IOPS = ((float) stats[stat_Fread_base].count) / ((float) times.secs_real);
		fprintf(fstats, "BASE filesystem peer avg read = %5.0f IOPS, %5.1f MB/s\n", IOPS, MBS);

		MBS  = MBS  * ((float) stats[stat_peers].count);
		IOPS = IOPS * ((float) stats[stat_peers].count);
		fprintf(fstats, "BASE filesystem TOT read      = %5.0f IOPS, %5.1f MB/s\n", IOPS, MBS);
	}
#endif
}

int stat_part_t::stat_dump_write_begin(const int phase_count, const char* hostname) { PXEQ
	
#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not dumping statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	if (!G_parms.dump_stats) return -1;

	int file_dump_stats;

	char filename[256];
	PXEQ
	sprintf( filename, "stats_%s.dump", G_parms.base_filename);
	    
	XEQ_EXEC((file_dump_stats = D1HT_OPEN_MODE(filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)), XEQ_WARNING);
	
	if (file_dump_stats < 0) {
		sprintf( filename, "/tmp/stats_%s.dump", G_parms.base_filename);
		XEQ_EXEC((file_dump_stats = D1HT_OPEN_MODE(filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)), XEQ_ERROR);
	}

	verbose(stderr, "Dumping statistics for host %s on file %s, phase_count=%d, sizeof(int)=%d, sizeof(double)=%d\n",
			hostname, filename, phase_count, (int) sizeof(int), (int) sizeof(double));
  		
	stat_dump_write_begin(file_dump_stats, phase_count, hostname);
   	
	return file_dump_stats;
}

void stat_part_t::stat_dump_write_begin(const int file_dump_stats, const int phase_count, const char* hostname) {

#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not dumping statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	if (!G_parms.dump_stats) return;

  	XEQ_EXEC( writen(file_dump_stats, &phase_count, sizeof(phase_count)), XEQ_WARNING);
  		
  	XEQ_EXEC( writen(file_dump_stats, hostname, D1HT_HOSTNAME_MAXLENGTH), XEQ_WARNING);

//  	VERBO(stderr, "timestampxxx=%s, size=%d\n", __TIMESTAMP__, (int) sizeof(#__TIMESTAMP__));

//  	XEQ_EXEC( writen(file_dump_stats, #__TIMESTAMP__, sizeof(#__TIMESTAMP__)), XEQ_WARNING);
}

void stat_part_t::stat_dump_write(const int file_dump_stats) {
	
#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not dumping statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	if (!G_parms.dump_stats) return;

	Assert(times.time_got, XEQ_ERROR, "!times.time_got");
	
  	times.time_dump_write(file_dump_stats);
	
	XEQ_EXEC( writen(file_dump_stats, &pass_counter_last, sizeof(pass_counter_last)), XEQ_ERROR); 
 
	for (int i=0; i < stat_LAST; i++) stats[i].dump_counter_write(file_dump_stats);

#ifdef D1HT_PASS_COUNT
	for (int i=0; i < pass_counter_last; i++) pass_counter[i].dump_pass_counter_write(file_dump_stats);
#endif 
}

int stat_part_t::stat_dump_read_begin(const int file_dump_stats, int *phase_count, char* hostname) {
	
	ssize_t nbytes;
	
	XEQ_EXEC( nbytes = readN(file_dump_stats, phase_count, sizeof(int)), XEQ_ERROR); PE(nbytes)
	
	if (nbytes == 0) return 0;
	
	Assert(nbytes==sizeof(int), XEQ_ERROR);

	XEQ_EXEC( nbytes = readn(file_dump_stats, hostname, D1HT_HOSTNAME_MAXLENGTH), XEQ_ERROR);
	
	verbose(stderr, "reading statistics for host %s, phase_count=%d, sizeof(int)=%d, sizeof(double)=%d\n", 
			hostname, *phase_count, (int) sizeof(int), (int) sizeof(double));
	
	Assert(nbytes==D1HT_HOSTNAME_MAXLENGTH, XEQ_ERROR);
	
	return nbytes;
}

void stat_part_t::stat_dump_read(const int file_dump_stats) {
	
  	times.time_dump_read(file_dump_stats);
  	times.time_got = true;
  	
	XEQ_EXEC( readn(file_dump_stats, &pass_counter_last, sizeof(pass_counter_last)), XEQ_ERROR); PE(pass_counter_last)
 	
	for (int i=0; i < stat_LAST; i++) {
		VERBOSEX(stderr,"reading stats %d (last = %d) %s\n", i, stat_LAST, stats[i].name);
		stats[i].dump_counter_read(file_dump_stats);
	}
#ifdef D1HT_PASS_COUNT
	for (int i=0; i < pass_counter_last; i++) pass_counter[i].dump_pass_counter_read(file_dump_stats);
#endif
}

stat_part_t stat_part_t::operator-(const stat_part_t minus) {
	
	stat_part_t retcode;

	for (int i=0; i < stat_LAST; i++) {
 		ASSERT(stats[i].count 		>= minus.stats[i].count);
 		ASSERT(stats[i].value 		>= minus.stats[i].value);
		ASSERT(stats[i].total 		>= minus.stats[i].total);
 		ASSERT(stats[i].OK	  		>= minus.stats[i].OK);
 		ASSERT(stats[i].tried 		>= minus.stats[i].tried);
 		ASSERT(stats[i].lost_time 	>= minus.stats[i].lost_time);
 		
 		retcode.stats[i].count		= stats[i].count 	- minus.stats[i].count;
 		retcode.stats[i].value		= stats[i].value 	- minus.stats[i].value;
 		retcode.stats[i].total		= stats[i].total 	- minus.stats[i].total;
 		retcode.stats[i].OK			= stats[i].OK 	 	- minus.stats[i].OK;
 		retcode.stats[i].tried		= stats[i].tried 	- minus.stats[i].tried;
 		retcode.stats[i].lost_time	= stats[i].lost_time- minus.stats[i].lost_time;
 		
 		retcode.stats[i].calculate_min_max	= 0; 
 		retcode.stats[i].max_count 			= 0; 
 	}
	retcode.times = times - minus.times;
	
	retcode.pass_counter_last = pass_counter_last;
	
	for (int i=0; i < minus.pass_counter_last; i++) {
		int j;
		for (j=0; j < retcode.pass_counter_last; j++) {
			if (retcode.pass_counter[j] == minus.pass_counter[i]) break;
		}
		if (j < retcode.pass_counter_last) {
			retcode.pass_counter[j] = retcode.pass_counter[j] - minus.pass_counter[i];
		} 
	}

	return retcode;
} 

stat_part_t stat_part_t::operator+(const stat_part_t plus) {
	
	stat_part_t retcode;
 
	for (int i=0; i < stat_LAST; i++) {	
		retcode.stats[i].count		= stats[i].count 	+ plus.stats[i].count;
		retcode.stats[i].value		= stats[i].value 	+ plus.stats[i].value; 
		retcode.stats[i].total		= stats[i].total 	+ plus.stats[i].total;
		retcode.stats[i].OK			= stats[i].OK 	 	+ plus.stats[i].OK;
		retcode.stats[i].tried		= stats[i].tried 	+ plus.stats[i].tried;
		retcode.stats[i].lost_time	= stats[i].lost_time+ plus.stats[i].lost_time;
	 
 		if (stats[i].calculate_min_max == 1 && plus.stats[i].calculate_min_max == 1 && stats[i].max_value > 0.0) {
 			
 			retcode.stats[i].calculate_min_max	= 1;
 			
 			if ( retcode.stats[i].min_value < 0.0 ) {
 				retcode.stats[i].min_value = plus.stats[i].min_value;
 			} else if ( plus.stats[i].min_value >= 0.0 ) {
 				retcode.stats[i].min_value = MIN(stats[i].min_value, plus.stats[i].min_value);
 			}

 			double var = (plus.stats[i].max_value - stats[i].max_value) / stats[i].max_value;
 			
 			if (var > 0.05) {
 				retcode.stats[i].max_count = plus.stats[i].max_count;
 				retcode.stats[i].max_value = plus.stats[i].max_value;
 			} else if (var > -0.05) {
 				retcode.stats[i].max_count = plus.stats[i].max_count + stats[i].max_count;
 				if (stats[i].max_value >= plus.stats[i].max_value) {
 	 				retcode.stats[i].max_value = stats[i].max_value;
 	  			} else {
 	 				retcode.stats[i].max_value = plus.stats[i].max_value;
 	 			}
 			} else {
 				retcode.stats[i].max_count = stats[i].max_count;
 				retcode.stats[i].max_value = stats[i].max_value;
 			}
  		} else {
			retcode.stats[i].max_count 			= MAX(stats[i].max_count, plus.stats[i].max_count);
			retcode.stats[i].calculate_min_max 	= 0;
  		}
  	}
	retcode.times = times + plus.times;
#ifdef D1HT_PASS_COUNT

	retcode.pass_counter_last = pass_counter_last;
	
	for (int i=0; i < pass_counter_last; i++) retcode.pass_counter[i] = pass_counter[i];
	
	for (int i=0; i < plus.pass_counter_last; i++) {
		int j;
		for (j=0; j < retcode.pass_counter_last; j++) {
			if (retcode.pass_counter[j] == plus.pass_counter[i]) break;
		}
		if (j < retcode.pass_counter_last) {
			retcode.pass_counter[j] = retcode.pass_counter[j] + plus.pass_counter[i];
		} else if (retcode.pass_counter_last < MAX_PASS_COUNTERS) {
			retcode.pass_counter[retcode.pass_counter_last] = plus.pass_counter[i];
			retcode.pass_counter_last++;
		} else {
			VERBO(stderr,"WARNING Cause: Exceeded MAX_PASS_COUNTERS (%d)\n", (int) MAX_PASS_COUNTERS);
		}
	}
#endif 

	return retcode;
} 

void stat_part_t::compare(const stat_part_t other) {

	for (int i=0; i < stat_LAST; i++) {
		NUM_32 min, max;			
 		
 		if (stats[i].count == other.stats[i].count) {
 			continue;
 		} else if (stats[i].count > other.stats[i].count) {
 			max = stats[i].count;
 			min = other.stats[i].count;
 		} else {
 			max = other.stats[i].count;
 			min = stats[i].count;
 		}	
 		int diff = max - min;	
 		Assert(max>0, XEQ_ERROR);

 		int perc = 100*diff / max;

 		if (diff>2 && perc>0) { 
 			VERBO(stderr, "stats %s did not match %lu, %lu (%3u%%)\n", stats[i].name, stats[i].count, other.stats[i].count, perc );
 		}
 	}
}

void* stat_t::start_stats(FILE* std) {
  	
	if (G_parms.print_stats) {
	  	if (fstats != NULL) {
	  		print_stat(std);
	  		fclose(fstats);
	  		fstats = NULL;
	  	}
	  	
//	  	sprintf( filename, "stats_%s_Version%u.txt", G_parms.base_filename, version);
//	  	XEQ_EXEC_NULL(fstats = fopen(filename, "w"), XEQ_ERROR);
	}

  	version++;
  	
  	total.times.time_start();
  	last.times.time_start();
  	last.times.time_get();
	
  	return NULL;
}

void stat_t::print_stat(FILE* std) { 
	
#if defined(DEBUG)
	verbose(stderr, "WARNING Cause: Not dumping statistics because D1HT was compiled with the DEBUG flag on\n")
#endif

	if (!G_parms.print_stats) return;

	FILE* std_stats;
	
	if (std != 0) {
		std_stats = std;
		VERBO(std_stats, "Printing statistics for program %s\n", G_parms.program_name);
	} else {
		VERBO(stderr, "Printing statistics for program %s\n", G_parms.program_name);
		std_stats = fstats;
	}
	
	if (std != 0) {
		Assert(total.times.time_got, XEQ_ERROR);
		// total.times.time_get();
		
		total.stat_print(std_stats, "total");
		
	} else {
		VERBO(stderr, "ATTENTION Cause: Failed to print stats\n");
	}
}

void stat_t::diff(stat_part_t* diff) { 
	LOCK(stat_mutex);
	total.times.time_get();
  	*diff 	= total - last;
  	last 	= total;
  	UNLOCK(stat_mutex);
}

void* stat_t::exiting_stats(FILE* std) { 
	if (!G_started) return NULL;

	if (stats_connection >= 0) {
		stat_part_t diff;
		total.times.time_get();
	  	diff 	= total - last;
		SEND_MSG( stats_connection, D1HT_STAT_REPLY, diff, XEQ_SILENT );
		D1HT_CLOSE(stats_connection);
		stats_connection = -1;
	}
	
	if (G_parms.print_stats && std!=NULL) {
/*		
		int fstats;

		char filename[256];
	 		
		sprintf( filename, "stats_%s_%d.txt", G_parms.base_filename, (int) getpid());
		    
		XEQ_EXEC((fstats = D1HT_OPEN_MODE(filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)), XEQ_WARNING);
		
		if (fstats < 0) {
			sprintf( filename, "/tmp/stats_%s_%d.dump", G_parms.base_filename, (int) getpid());
			XEQ_EXEC((fstats = D1HT_OPEN_MODE(filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)), XEQ_ERROR);
		}

		VERBOSE(stderr, "Dumping statistics for host %s on file %s, sizeof(int)=%d, sizeof(double)=%d\n", 
				hostname, filename, (int) sizeof(int), (int) sizeof(double));
*/		

		print_stat(std);	
		if (fstats != 0 && fstats != NULL) {
			fflush(fstats);
			fclose(fstats);
			fstats = NULL;
		}
	}
	
	if (G_parms.dump_stats) {
		int dump_stats = total.stat_dump_write_begin(1, G_parms.display_name);
		if (dump_stats >= 0) {
			total.stat_dump_write(dump_stats);
			D1HT_CLOSE(dump_stats);
		}
	}
	
	return NULL;
}

void stat_t::shutdown() { 
	if (stats_connection >= 0) {
		D1HT_CLOSE(stats_connection); 
		stats_connection = -1; 
	}
	if (fstats != NULL) { 
		fclose(fstats); 
		fstats = NULL; 
	}
}

int D1HT_get_stats(int connection, const NUM_8 stats_type, stat_part_t * stats, const int Savg) {
	int retcode = -1;

	msg_hd 	reply_hd;	
	int 	severity = XEQ_SILENT;severity = XEQ_WARNING;
	 
	for (int tries=0; retcode <= 0 && tries <= G_parms.retries; tries++) { 
		retcode = -1; 
		
		if (connection < 0) return retcode;
		
		if (tries == G_parms.retries-1) severity = XEQ_WARNING;
		
		mysleep(tries); 
		
		if (SEND_HEADER( connection, stats_type, severity ) < 0) continue;	
		
		D1HT_STAT_body body;
		body.new_savg = Savg;
		
		if (SEND_BODY(connection, body, severity) < 0) continue; 
		
		reply_hd = READ_HEADER_TYPE(connection, D1HT_STAT_REPLY, severity);
		int errsave = errno;
		
		if (reply_hd.type != D1HT_INVALID) { 
			retcode = READ_BODY(connection, (*stats), severity);
		} else {
			if (errsave != EAGAIN && errsave != EINTR && errsave != EWOULDBLOCK) break;
			if (tries > G_parms.retries) break;
		}
	}
	if (retcode > 0) {
		retcode = 0;
		stats->times.time_got = true;
	}
	return retcode;
}

int D1HT_get_stats_partial(const int connection, stat_part_t * stats, const int Savg) { 
	return D1HT_get_stats(connection, D1HT_STAT_DIFF, stats, Savg); 
}

int D1HT_get_stats_total(const int connection, stat_part_t * stats, const int Savg) { 
	return D1HT_get_stats(connection, D1HT_STAT_TOT, stats, Savg); 
}

void stat_part_t::stat_time_start3(const char* filename, const uint linenumber, const int stat_index) {
	
	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d, stat_index=%d, prevtimer=%d\n", timer_index, stats[timer_index].prevtimer, stat_index, stats[stat_index].prevtimer);

	if (stats[stat_index].prevtimer != 0) EXITFL(filename, linenumber, "prevtimer stat name=%s, new stat name=%s\n", stats[stats[stat_index].prevtimer].name, stats[stat_index].name);
	
	if (timer_index != 0) EXITFL(filename, linenumber, "timer_index stat name=%s, new stat name=%s\n", stats[timer_index].name, stats[stat_index].name);

	AssertFL(stats[stat_index].prevtimer == 0, XEQ_ERROR, filename, linenumber);
	AssertFL(timer_index == 0, XEQ_ERROR, filename, linenumber);

	gettimeofday(&timer, NULL);
	timer_index = stat_index;

	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d, stat_index=%d, prevtimer=%d\n", timer_index, stats[timer_index].prevtimer, stat_index, stats[stat_index].prevtimer);
}

void stat_part_t::stat_time_switch3(const char* filename, const uint linenumber, const int stat_index, const bool _OK) {
	
	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d, stat_index=%d, prevtimer=%d\n", timer_index, stats[timer_index].prevtimer, stat_index, stats[stat_index].prevtimer);

	if (stats[stat_index].prevtimer != 0)EXITFL(filename, linenumber, "prevtimer stat name=%s, new stat name=%s\n", stats[stats[stat_index].prevtimer].name, stats[stat_index].name);
	ASSERTFL(stats[stat_index].prevtimer == 0, "prevtimer is not zero", filename, linenumber);
	
	if (timer_index != 0) {
		ASSERTFL(timer.tv_sec != 0, "timer.tv_sec != 0", filename, linenumber);
		
		struct timeval	now;  
		gettimeofday(&now, NULL);
		
		double dt = TVDIFF2MSEC(timer, now);
				
		stat_time_stop3(_OK, dt);
		
		timer.tv_sec 	= now.tv_sec;
		timer.tv_usec 	= now.tv_usec;
		
		stats[stat_index].prevtimer = timer_index;
	} else {
		ASSERTFL(timer_index == 0, "timer_index == 0",filename, linenumber);
		gettimeofday(&timer, NULL);
	}
	timer_index = stat_index;

	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d, stat_index=%d, prevtimer=%d\n", timer_index, stats[timer_index].prevtimer, stat_index, stats[stat_index].prevtimer);
}

double stat_part_t::stat_time_restore3(const char* filename, const uint linenumber, const bool _OK) {

	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	struct timeval	now;  
	gettimeofday(&now, NULL);
		
	double dt = TVDIFF2MSEC(timer, now);	
	
	if (timer_index != 0) {
		
		AssertFL((timer.tv_sec != 0 || timer.tv_usec != 0), XEQ_ERROR, filename, linenumber);

		stat_time_stop3(_OK, dt);
	
		timer.tv_sec 	= now.tv_sec;
		timer.tv_usec 	= now.tv_usec;
		
		int stat_index 	= timer_index;
			
		timer_index 	= stats[stat_index].prevtimer;
		
		stats[stat_index].prevtimer 		= 0;
		stats[stat_index].calculate_min_max	= 0;
	}
	
	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	return dt;
}

NUM_32 stat_counter_t::stop_time3(const bool _OK, const double dt, const NUM_32 maxcount) {
		
	value += dt;
	
	if (maxcount != 0) {
		if (maxcount > count) count = maxcount;
	} else {
		count++;
	}
	
	tried++;
	
	if (_OK) {
		
		OK++;
		
		if (max_value > 0.0) {
			
			double var = (dt - max_value) / max_value;
		
			if (var > 0.05) {
				max_count = 1;
			} else if (var > -0.05) {
				max_count++;
			}
		}
		
		max_value = MAX(max_value, dt);
		
		if (min_value < 0.0) {
			min_value = dt;
		} else {
			min_value = MIN(min_value, dt);
		}

	} else {
		lost_time += dt;
	}
	return count;
}

double stat_part_t::stat_time_stop3(const bool _OK, const bool xeq) {

	if (xeq && timer_index==0) return 0;
	
	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	ASSERT(timer.tv_sec!= 0);
	ASSERT(timer_index != 0);

	struct timeval	now;  
	gettimeofday(&now, NULL);
		
	double dt = TVDIFF2MSEC(timer, now);
	
	stat_time_stop3(_OK, dt);
	
	stats[timer_index].prevtimer = 0;

	timer_index = 0;
	BZERO(timer);
	
	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	return dt;
}

void stat_part_t::stat_time_stop3(const bool _OK, const double dt) {

	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	NUM_32 maxcount = 0;
	for (int index = timer_index; index != 0; index = stats[index].tot_index) {
		maxcount = stats[index].stop_time3(_OK, dt, maxcount);
	}

	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);
}

void stat_part_t::stat_time_clean3() {

	VERBOSEX(stderr,"OLD timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);

	int next_index;
	for (int index = timer_index; index != 0; index = next_index) {
		next_index = stats[index].prevtimer;
		stats[index].prevtimer = 0;
	}
	timer_index = 0;

	VERBOSEX(stderr,"NEW timer_index=%d, [timer_index].prev_timer=%d\n", timer_index, stats[timer_index].prevtimer);
}

void pass_counter_t::dump_pass_counter_write(const int dumpfile) { 

#ifndef D1HT_PASS_COUNT
		return ;
#endif 

	//VERBO(stderr,"counters for FILE=%s, LINE=%d, count=%lu\n", FILEname, LINE, count);

	XEQ_EXEC( writen(dumpfile, FILEname, pass_counter_t_FILE_size), XEQ_WARNING); 
	
	double hash = 0.0;
	
	DUMP_COUNTER(writen, LINE);
	DUMP_COUNTER(writen, count);
	DUMP_COUNTER(writen, tot_index);
	
	DUMP_COUNTER(writen, hash);
}

void pass_counter_t::dump_pass_counter_read(const int dumpfile) {

#ifndef D1HT_PASS_COUNT
		return ;
#endif 

	XEQ_EXEC( readn(dumpfile, FILEname, pass_counter_t_FILE_size), XEQ_ERROR);
	
	double hash = 0.0;

	DUMP_COUNTER(readn, LINE);
	DUMP_COUNTER(readn, count);
	DUMP_COUNTER(readn, tot_index);
	
	double hash_test = 0.0;

	XEQ_EXEC( readn(dumpfile, &hash_test, sizeof(hash_test)), XEQ_ERROR);
	
	// fprintf(stderr,"read %s count=%lu, hash=%f, hash_test=%f\n", FILEname, (ulong) count, hash, hash_test);

	Assert(hash == hash_test, XEQ_ERROR);
}

} // namespace d1ht
