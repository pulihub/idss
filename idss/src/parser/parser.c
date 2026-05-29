#include <stdio.h>
#include <string.h>
#include "idss_parser.h"



int main(int argc, char **argv)
{

        int rc;
        char *str = strdup(argv[1]);

        rc = idss_parse(str);

        if(rc != -1)
	      {
                fprintf(stdout,"SQL SELECT statement has been parsed succesfully\n");
	      }
	      else
	      {
                fprintf(stderr, "Can not parse this SQL SELECT statement\n");
	      }
        return 0;

}
