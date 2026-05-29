#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include "p2p_utils.h"


void get_random_value (char **value)
{
	struct timeval tv;
	char buffer[30] = {'\0'};

	gettimeofday(&tv, NULL);

	snprintf (buffer, sizeof (buffer), "%ld%ld", tv.tv_sec, (long) tv.tv_usec);

	*value = strdup (buffer);
}


//ritorna il tempo in millisecondi
long long get_current_time ()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv, &tz);
	return ((long long) tv.tv_sec * 1000) + (long long) (tv.tv_usec / 1000);
}

int print_result(char *uqi, char *ip, char *value)
{
	FILE *fp;
	char filename[50] = {'\0'};

	snprintf(filename, 50, "%s_%s", ip, uqi);

	fp = fopen(filename, "a+");
	if (fp != NULL)
		{
			fprintf(fp, "%s", value);
			fclose(fp);
			return 0;
		}
	else
		return 1;
}
