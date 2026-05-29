#include <string.h>
#include <stdlib.h>

/* tokenize_string_r
 * This function is thread-safe; it parses a string and returns
 * -1 in case of error, or the number of tokens found in the string s.
 * The string s will be split in tokens that you will find in tokenp.
 *
 * parameters:
 *
 * char *s               is the string to be parsed;
 *
 * char *delimiters      is a a string containing the delimiters
 *                       to be used during the parsing phase;
 *
 * char ***tokenp        a pointer to an array of strings
 *                       obtained parsing the input string;
 *
 * The function returns the number of tokens found on success,
 * -1 on error, 0 if no tokens are found
 *
 *
 */

int tokenize_string_r(char *s, char *delimiters, char ***tokenp)
{
   char *t = NULL;
   char *snew;
   int  numtokens;
   int  i;
   char *saveptr = NULL;
   char *ptr;

    /* snew is real start of string after skipping leading delimiters */
   snew = s + strspn(s, delimiters);
                              /* create space for a copy of snew in t */
   if (!(t = (char*)calloc(strlen(snew) + 1, sizeof(char))))
   {
     *tokenp = NULL;
     numtokens = -1;
   }
   else
   {                     /* count the number of tokens in snew */
     strcpy(t, snew);
     if ( !(ptr=strtok_r(t, delimiters, &saveptr)))
     {
        free(t);
        numtokens = 0;
     }
     else
     {

	   for (numtokens = 1; (ptr=strtok_r(NULL, delimiters, &saveptr)) != NULL;
              numtokens++)
              ;
                /* create an array to contain ptrs to tokens */
       if (!(*tokenp = (char**)calloc(numtokens + 1, sizeof(char *))))
       {
         free(t);
         numtokens = -1;
       }
       else
       {            /* insert pointers to tokens into the array */
         if (numtokens > 0)
         {
           strcpy(t, snew);
           **tokenp = strdup(strtok_r(t, delimiters, &saveptr));
           for (i = 1; i < numtokens + 1; i++)
           {
		     ptr = strtok_r(NULL, delimiters, &saveptr);
             if (ptr)
			   *((*tokenp) + i) = strdup(ptr);
			 else
			   *((*tokenp) + i) = NULL;
           }

           free(t);
         }
         else
         {
           *tokenp = NULL;
           free(t);
         }
       }
     }
   }

   return numtokens;
}
