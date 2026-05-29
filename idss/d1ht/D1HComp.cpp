/**
 * @file D1HComp.c
 * @brief Sembra essere un eseguibile per l'elaborazione di file di statistiche
 */

#include "D1HWrap.h"
#include <gsl/gsl_sort.h>
#include <gsl/gsl_statistics.h>
#include <locale.h>

#define EXIT_STATS( ... ) { fprintf(stderr,"\n"); VERBO(stderr, "Exiting - " __VA_ARGS__); fprintf(stderr,"\n"); exit(-1); }

#define EXIT_STATSV( msgerr ) { fprintf(stderr,"\n"); VERBO(stderr, "Exiting - ERROR: %s\n", msgerr); exit(-1); }

#define EXTRACT( DEF )  ({                                                             		\
        int             _val = -1;                                                      	\
        char*   _match;                                                                	 	\
        if (((_match = strstr( buffer, #DEF )) != NULL ) ) {                            	\
                _match += (sizeof(#DEF) - 1);                                            	\
                if  (strncmp(" ", _match, 1) == 0) {                                    	\
                        _match = strstr( _match, "=" );                                 	\
                        sscanf(_match, "=%u", &_val);                                   	\
                }                                                                      		\
        }                                                                              		\
        _val;                                                                           	\
})

#define SUM_TIMESTATS(_stat) {                                                          	\
        int _value = EXTRACT(_stat);                                                    	\
        if (_value >= 0) {                                                              	\
                Tstats[phasenumber].times._stat += _value;                   				\
                continue;                                                               	\
        } }

using namespace d1ht;

int main(int argc, char** argv) {

    ifstream input1, input2, input3;
    FILE* outfile;

    G_parms.program_name = "D1HComp";

    char filename[200];

    fclose(stdout);
    sprintf(filename, "e_%s_%s.txt", G_parms.program_name, argv[1]);
    freopen(filename, "w", stderr);

    VERBO(stderr, "Running command=");
    for (int i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
    fprintf(stderr, "\n");

    Assert(argc == 2, XEQ_ERROR, "ERROR: D1HStats requires 1 (and only 1) parameter: the benchmark filespec");

    sprintf(filename, "D1HT.%s.stats_to_xls.txt", argv[1]);
    input1.open(filename);
    if (!input1) EXIT_STATS("ERROR Opening input file %s", filename);
    VERBO(stderr, "Opened file %s\n", filename);

    sprintf(filename, "look.%s.stats_to_xls.txt", argv[1]);
    input3.open(filename);
    if (!input3) EXIT_STATS("ERROR Opening input file %s", filename);
    VERBO(stderr, "Opened file %s\n", filename);

    sprintf(filename, "stats_%s.%s.txt", G_parms.program_name, argv[1]);
    outfile = fopen(filename, "w");
    Assert(outfile != NULL, XEQ_ERROR, "ERROR opening output file");

    fprintf(outfile, "#%s\n", argv[1]);
    fprintf(outfile, "#                                       D1HT            CALOT              look\n");

    while (!input1.eof()) {
        char buffer[200];
        char statsname1[80], statsname2[80], statsname3[80];
        float stats1, stats2, stats3;

        stats1 = 0.0;
        input1.getline(buffer, sizeof (buffer));

        if (input1.eof()) break;

        VERBO(stderr, "buffer=%s\n", buffer)
        sscanf(buffer, "%s %f", statsname1, &stats1);
        VERBO(stderr, "%s %16.4f\n", statsname1, stats1)

        Assert((!input2.eof()), XEQ_ERROR);
        Assert((!input3.eof()), XEQ_ERROR);

        stats2 = 0.0;
        input2.getline(buffer, sizeof (buffer));
        VERBO(stderr, "buffer=%s\n", buffer)
        sscanf(buffer, "%s %f", statsname2, &stats2);
        VERBO(stderr, "%s %16.4f\n", statsname2, stats2)
        Assert((strncmp(statsname1, statsname2, sizeof (statsname1)) == 0), XEQ_ERROR);

        stats3 = 0.0;
        input3.getline(buffer, sizeof (buffer));
        VERBO(stderr, "buffer=%s\n", buffer)
        sscanf(buffer, "%s %f", statsname3, &stats3);
        VERBO(stderr, "%s %16.4f\n", statsname3, stats3)
        Assert((strncmp(statsname1, statsname3, sizeof (statsname1)) == 0), XEQ_ERROR);

        if (setlocale(LC_NUMERIC, "pt_BR") == NULL) EXIT_STATS("ERROR in setlocale pt_BR");
        fprintf(outfile, "%-31s %16.4f %16.4f %16.4f\n", statsname1, stats1, stats2, stats3);
        if (setlocale(LC_NUMERIC, "en_US") == NULL) EXIT_STATS("ERROR in setlocale en_US");
    }

    input1.close();
    input2.close();
    input3.close();
    fclose(outfile);
    return 0;
}
