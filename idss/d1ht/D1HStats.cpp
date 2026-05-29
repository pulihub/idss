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
 *  No title to or ownership of D1HT is hereby transferred.	                 *
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
 * @file D1HStats.c
 * @brief Sembra un file di test per le elaborazione delle statistiche (a partire da dump file) (?)
 */
#include "D1HWrap.h"

#define NOGSL

#define MAX_PEERS 			8200 

#ifndef NOGSL
#include <gsl/gsl_sort.h>
#include <gsl/gsl_statistics.h>
#define MAX_XEQ_STATS 		6
#else
#define MAX_XEQ_STATS 		0
#endif

using namespace d1ht;

class stats_xeq_t {
public:
    int stat_i;
    int icount;
    bool xeq_small;
    bool xeq_big;
    double max;
    double min;
    double xeqed_value[MAX_PEERS];
    char hostname[MAX_PEERS][D1HT_HOSTNAME_MAXLENGTH];
};

int main(int argc, char** argv) {

    G_parms.program_name = "D1HStats";
    fclose(stdout);

    stats_xeq_t stats_xeq[MAX_XEQ_STATS];

    BZERO(stats_xeq);

    for (int j = 0; j < MAX_XEQ_STATS; j++) stats_xeq[j].stat_i = -1;

    ASSERT(MAX_PHASES < 9);

    int phase_to_check = 1;

#ifndef NOGSL
    stats_xeq[0].stat_i = stat_lookup_rate;
    stats_xeq[0].xeq_big = false;
    stats_xeq[0].xeq_small = true;

    stats_xeq[1].stat_i = stat_lookups_not1hop;
    stats_xeq[1].xeq_big = true;
    stats_xeq[1].xeq_small = false;

    stats_xeq[2].stat_i = stat_lookups_failed;
    stats_xeq[2].xeq_big = true;
    stats_xeq[2].xeq_small = false;
#else
    phase_to_check = -1;
#endif

    char time_string[20];
    {
        char filename[256];
        struct tm *ptm;
        struct timeval tnow;

        gettimeofday(&tnow, NULL);
        ptm = localtime(&tnow.tv_sec);
        strftime(time_string, sizeof (time_string), D1HT_TIME_FORMAT, ptm);

        sprintf(filename, "e_%s_%s.txt", G_parms.program_name, time_string);
        freopen(filename, "w", stderr);
    }

    VERBO(stderr, "Running command=");
    for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
    fprintf(stderr, "\n");

    ASSERT(MAX_PHASES < 9);

    if (argc != 2) EXIT("%s requires one (and only one) parameter: the name of the input (.dump) file", G_parms.program_name);

    G_parms.verbose = 1;
    G_parms.dump_stats = true;
    G_parms.print_stats = true;

    int fstats_info;

    if ((fstats_info = D1HT_OPEN(argv[1], O_RDONLY)) < 0) {
        int errsave = errno;
        EXIT("Unable to open input file. Detail: file=%s, msg=%s", argv[1], strerror(errsave));
    }

    if (strncmp(G_parms.start_time_string, "  ", 1) == 0) {
        struct timeval program_start;
        gettimeofday(&program_start, NULL);
        struct tm *ptm;
        ptm = localtime(&program_start.tv_sec);
        strftime(G_parms.start_time_string, sizeof ( G_parms.start_time_string), D1HT_TIME_FORMAT, ptm);
    }

    int new_phases_count;
    char hostname[D1HT_HOSTNAME_MAXLENGTH + 1];

    int phases_count = -1;
    int hosts = 0;

    stat_part_t Tstats[MAX_PHASES + 1];
    stat_part_t xeq_stat;
    stat_part_t temp_stat;

    while (temp_stat.stat_dump_read_begin(fstats_info, &new_phases_count, hostname) > 0) {

        if (phases_count == -1) phases_count = new_phases_count;

        if (phases_count != new_phases_count) EXIT("Invalid phases count. Detail: phases_count=%d, new_phases_count=%d\n", phases_count, new_phases_count);

        hosts++;

        for (int iphase = 0; iphase < phases_count; iphase++) {

            temp_stat.stat_dump_read(fstats_info);

            Tstats[iphase] = temp_stat + Tstats[iphase];

            if (iphase != phase_to_check) continue;

            for (int j = 0; j < MAX_XEQ_STATS; j++) {

                Assert(stats_xeq[j].icount < MAX_PEERS, XEQ_ERROR);

                int i = stats_xeq[0].stat_i;

                double xvalue = -1;

                if (temp_stat.stats[i].value > 0 && temp_stat.stats[i].count > 0) {
                    xvalue = temp_stat.stats[i].value / ((double) temp_stat.stats[i].count);
                } else if (temp_stat.stats[i].value > 0) {
                    xvalue = temp_stat.stats[i].value;
                } else if (temp_stat.stats[i].OK > 0) {
                    xvalue = (double) temp_stat.stats[i].value;
                } else {
                    xvalue = (double) temp_stat.stats[i].count;
                }

                int k = stats_xeq[j].icount;

                if (k == 0) stats_xeq[j].min = xvalue;

                strncpy(stats_xeq[j].hostname[k], hostname, D1HT_HOSTNAME_MAXLENGTH - 2);

                stats_xeq[j].xeqed_value[k] = xvalue;
                stats_xeq[j].max = MAX(stats_xeq[j].max, xvalue);
                stats_xeq[j].min = MIN(stats_xeq[j].min, xvalue);

                //				if (iphase==1) fprintf(stderr, "name=%s, value=%f, count=%u, xvalue=%f, xeqed_value=%f\n", temp_stat.stats[i].name, temp_stat.stats[i].value, temp_stat.stats[i].count, xvalue, stats_xeq[j].xeqed_value[k]);

                stats_xeq[j].icount++;
            }
        }
    }

    D1HT_CLOSE(fstats_info);

    if (hosts == 0) return 0;

    if (hosts > 1) {

        char filename[256];

        sprintf(filename, "SUM_%s", argv[1]);

        int total_filestats;

        XEQ_EXEC((total_filestats = D1HT_OPEN_MODE(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)), XEQ_WARNING);
        int errsave = errno;

        if (total_filestats < 0) {

            VERBO(stderr, "WARNING Cause: Unable do create dump summary file. Detail: filename=%s, msg=%s, \n", filename, strerror(errsave));

            sprintf(filename, "%s.SUM", argv[1]);

            VERBO(stderr, "Trying to create alternative dump summary fil %s\n", filename);

            XEQ_EXEC((total_filestats = D1HT_OPEN_MODE(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)), XEQ_WARNING);
            int errsave = errno;

            if (total_filestats < 0) VERBO(stderr, "WARNING Cause: Unable do create alternative dump summary file. Detail: filename=%s, msg=%s, \n", filename, strerror(errsave));
        }

        if (total_filestats >= 0) {

            VERBO(stderr, "dump summary file %s will have the sum of the stats of all %d hosts in file %s\n", filename, hosts, argv[1]);

            Tstats[0].stat_dump_write_begin(total_filestats, phases_count, "TOTAL");

            for (int iphase = 0; iphase < phases_count; iphase++) {
                Tstats[iphase].stat_dump_write(total_filestats);
            }

            D1HT_CLOSE(total_filestats);
        }
    }

    FILE* outfile;
    {
        char filename[256];
        sprintf(filename, "%s.stats.txt", argv[1]);
        outfile = fopen(filename, "w");
        int errsave = errno;
        if (outfile == NULL) EXIT("failed to open output file. Detail file=%s msg=%s", filename, strerror(errsave));
    }

#ifndef NOGSL
    for (int j = 0; j < MAX_XEQ_STATS; j++) {

        if (stats_xeq[j].stat_i == -1) continue;

        int k = stats_xeq[j].icount;

        if (k == 0) continue;

        char unit[8];
        char name[20];
        char _temp_string[200];

        strncpy(name, xeq_stat.stats[stats_xeq[j].stat_i].name, sizeof (name));
        strncpy(unit, xeq_stat.stats[stats_xeq[j].stat_i].unit, sizeof (unit));

        // gsl_sort(stats_xeq[j].sorted_count, 1, stats_xeq[j].icount);

        double mean = gsl_stats_mean(stats_xeq[j].xeqed_value, 1, stats_xeq[j].icount);

        double sdev = gsl_stats_sd(stats_xeq[j].xeqed_value, 1, stats_xeq[j].icount);

        double mmax = MIN(5 * mean, mean + 2 * sdev);

        double mmin = MAX(mean / 2, mean - 2 * sdev);

        int mcount = 0;

        for (int kk = 0; kk < stats_xeq[j].icount; kk++) {
            if (stats_xeq[j].xeq_big && stats_xeq[j].xeqed_value[kk] > mmax) {
                mcount++;
                sprintf(_temp_string, "HOST=%s - check big   value for phase%u.%s", stats_xeq[j].hostname[kk], phase_to_check, name);
                fprintf(stderr, "%s=%12.0f %s\n", _temp_string, stats_xeq[j].xeqed_value[kk], unit);
            } else if (stats_xeq[j].xeq_small && stats_xeq[j].xeqed_value[kk] < mmin) {
                mcount++;
                sprintf(_temp_string, "HOST=%s - check small value for phase%u.%s", stats_xeq[j].hostname[kk], phase_to_check, name);
                fprintf(stderr, "%s=%12.0f %s\n", _temp_string, stats_xeq[j].xeqed_value[kk], unit);
            } else if (stats_xeq[j].stat_i == stat_lookup_rate && stats_xeq[j].xeqed_value[kk] < (0.8 * mmin)) {
                mcount++;
                sprintf(_temp_string, " HOST=%s - check small value for phase%u.%s", stats_xeq[j].hostname[kk], phase_to_check, name);
                fprintf(stderr, "%s=%12.0f %s - unable do cope with required lookup rate\n", _temp_string, stats_xeq[j].xeqed_value[kk], unit);
            }
        }

        fprintf(stderr, "phase1.%-30s mean=%7.0f standard deviation=%7.0f, max=%7.0f, min=%7.0f, mcount=%u\n", name, mean, sdev, stats_xeq[j].max, stats_xeq[j].min, mcount);

        if (mcount * 10 > stats_xeq[j].icount) {
            fprintf(outfile, "\nATTENTION Cause: More than 10%% of the peers have unusual values for %s - "
                    "You MUST carefully check the peers listed in the stderr output before considering these satistics as valid\n\n", name);
            fprintf(stderr, "\nATTENTION Cause: More than 10%% of the peers have unusual values for %s - "
                    "You MUST carefully check the peers listed in the stderr output before considering these satistics as valid\n\n", name);
        }
    }
#endif

    if (phases_count == 1) {
        Tstats[0].stat_print(outfile, NULL);
    } else {
        for (int i = 0; i < phases_count; i++) {
            char header[12];
            sprintf(header, "phase%1u", i);
            Tstats[i].stat_print(outfile, header);
            //	Tstats[MAX_PHASES] += Tstats[i];
        }
    }

    //Tstats[MAX_PHASES].stat_print(outfile, "TOTAL");
    fclose(outfile);

    VERBO(stderr, "Command %s ended OK\n", argv[0]);
}
