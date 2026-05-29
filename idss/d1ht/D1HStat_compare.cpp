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
 * @file D1HStat_compare.c
 * @brief Sembra un file di test per le elaborazione delle statistiche (?)
 */
#include "D1HWrap.h"

using namespace d1ht;

void usagemsg(const int argc, char** argv) {
    fprintf(stderr, "Running command=");
    for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
    fprintf(stderr, "\n");
    fprintf(stderr, "usage %s perc dumpfile1 dumpfile2\n", argv[0]);
}

int main(int argc, char** argv) {

    G_parms.program_name = "D1HStat_compare";
    fclose(stdout);

    if (argc != 4) {
        usagemsg(argc, argv);
        EXIT("%s requires tree (and only tree) parameters: the difference threshold and the names of the dump files to compare", G_parms.program_name);
    }

    G_parms.verbose = 1;

    float diff = 0.10;

    int fstats_info_1;

    if ((fstats_info_1 = D1HT_OPEN(argv[2], O_RDONLY)) < 0) {
        int errsave = errno;
        EXIT("Unable to open input file. Detail: file=%s, msg=%s", argv[3], strerror(errsave));
    }

    int fstats_info_2;

    if ((fstats_info_2 = D1HT_OPEN(argv[3], O_RDONLY)) < 0) {
        int errsave = errno;
        EXIT("Unable to open input file. Detail: file=%s, msg=%s", argv[4], strerror(errsave));
    }

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

    int phases_count_1, phases_count_2;
    char hostname[D1HT_HOSTNAME_MAXLENGTH + 1];

    stat_part_t stat_1, stat_2;

    stat_1.stat_dump_read_begin(fstats_info_1, &phases_count_1, hostname);
    stat_2.stat_dump_read_begin(fstats_info_2, &phases_count_2, hostname);

    if (phases_count_1 != phases_count_2) EXIT("different phase counts. Detail: phases_count_1=%d, phases_count_2=%d/n", phases_count_1, phases_count_2);

    fprintf(stderr, "\n     Dump files=             %-70.70s -- %-70.70s\n\n", argv[2], argv[3]);

    for (int iphase = 0; iphase < phases_count_1; iphase++) {
        PXEQ

        stat_1.stat_dump_read(fstats_info_1);
        PXEQ
        stat_2.stat_dump_read(fstats_info_2);
        PXEQ

        for (int i = 0; i < stat_LAST; i++) {

            float comp_1;
            float comp_2;

            int ccount = (int) stat_1.stats[i].count;
            if (ccount == 0) ccount = (int) stat_1.stats[i].tried;

            if (stat_1.stats[i].value > 0.0 && ccount > 0) {
                comp_1 = ((float) stat_1.stats[i].value) / ((float) ccount);
                ccount = (int) stat_2.stats[i].count;
                if (ccount == 0) ccount = (int) stat_2.stats[i].tried;
                comp_2 = ((float) stat_2.stats[i].value) / ((float) ccount);
            } else if (stat_1.stats[i].value > 0.0 && stat_1.stats[i].OK > 0) {
                comp_1 = ((float) stat_1.stats[i].value) / ((float) stat_1.stats[i].OK);
                comp_2 = ((float) stat_2.stats[i].value) / ((float) stat_2.stats[i].OK);
            } else if (stat_1.stats[i].value > 0.0) {
                comp_1 = (float) stat_1.stats[i].value;
                comp_2 = (float) stat_2.stats[i].value;
            } else if (stat_1.stats[i].max_count != 0) {
                comp_1 = (float) stat_1.stats[i].max_count;
                comp_2 = (float) stat_2.stats[i].max_count;
            } else {
                comp_1 = (float) ccount;
                ccount = (int) stat_2.stats[i].count;
                if (ccount == 0) ccount = (int) stat_2.stats[i].tried;
                comp_2 = (float) ccount;
            }

            float sdiff;

            if (comp_1 == 0.0) {
                if (comp_2 != 0.0) {
                    sdiff = 100000.0;
                } else {
                    sdiff = 0.0;
                }
            } else {
                sdiff = (comp_2 - comp_1) / comp_1;
            }

            if (sdiff > diff || sdiff < (-diff)) {

                if (phases_count_1 > 1) fprintf(stderr, "phase%1u", iphase);

                int idiff = (int) (100.0 * sdiff);

                if (idiff > 999 || idiff < -99) {
                    fprintf(stderr, "dif=XXX%%, %-19s", stat_1.stats[i].name);
                } else {
                    fprintf(stderr, "dif=%3d%%, %-19s", idiff, stat_1.stats[i].name);
                }

                char sstats[256];
                stat_1.stats[i].print_counter_str(sstats, NULL, 0);
                fprintf(stderr, "%-70.70s -- ", sstats);
                stat_2.stats[i].print_counter_str(sstats, NULL, 0);
                fprintf(stderr, "%-70.70s\n", sstats);
            }
        }
    }

    D1HT_CLOSE(fstats_info_1);
    D1HT_CLOSE(fstats_info_2);

    VERBO(stderr, "\n\nCommand %s ended OK\n", argv[0]);
    return 0;
}
