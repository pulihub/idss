/*
 * This software copyright 2022-2023
 * University of Salento, Italy
 *
 * Readfile: reads the contents of a file in a buffer
 *
 *
 *
 */



#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"


#define BUFLEN 16384

int readfile(char *filename, char **buffer)
{
   int  fd, read_len;
   char errbuf[1024];
 
   fd = open(filename, O_RDONLY);
   if ( fd < 0 )
      {
         sprintf(errbuf, "An error occurred trying to open %s", filename);
	 idss_errmsg_r(errbuf);
         return -1;
      }

  *buffer = (char *) calloc(BUFLEN, sizeof(char));
 
  read:
   read_len = read(fd, *buffer, BUFLEN);
   if (read_len <= 0)
      {
         if (errno == EINTR)
            goto read;
         sprintf(errbuf, "An error occurred while reading the contents of %s", filename);
	 idss_errmsg_r(errbuf);
         close(fd);
         return -1;
      }
   close(fd);

   return 0;
}   

ssize_t idss_readn(int fd, void *vptr, size_t n)
{
        size_t  nleft;
        ssize_t nread;
        char    *ptr;

        ptr = vptr;
        nleft = n;
        while (nleft > 0) {
                if ( (nread = read(fd, ptr, nleft)) < 0) {
                        if (errno == EINTR)
                                nread = 0;              /* and call read() again */
                        else
                                return(-1);
                } else if (nread == 0)
                        break;                          /* EOF */

                nleft -= nread;
                ptr   += nread;
        }
        return(n - nleft);              /* return >= 0 */
}

ssize_t idss_writen(int fd, const void *vptr, size_t n)
{
        size_t          nleft;
        ssize_t         nwritten;
        const char      *ptr;

        ptr = vptr;
        nleft = n;
        while (nleft > 0) {
                if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
                        if (nwritten < 0 && errno == EINTR)
                                nwritten = 0;           /* and call write() again */
                        else
                                return(-1);                     /* error */
                }

                nleft -= nwritten;
                ptr   += nwritten;
        }
        return(n);
}

