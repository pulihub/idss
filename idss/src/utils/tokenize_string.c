#include <string.h>
#include <stdlib.h>

/* tokenize_string
 * This function is NOT thread-safe; it parses a string and returns 
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
 * NOTE: you MUST free the individual tokens argvp[i] and then
 * the argument argvp
 *
 *
 */


int tokenize_string(char *s, char *delimiters, char ***argvp)
{
   char *t;
   char *snew;
   int numtokens;
   int i;
    /* snew is real start of string after skipping leading delimiters */
   snew = s + strspn(s, delimiters);
                              /* create space for a copy of snew in t */
   if (!(t = (char *) calloc(strlen(snew) + 1, sizeof(char)))) {
      *argvp = NULL;
      numtokens = -1;
      return numtokens;
   }

  /* count the number of tokens in snew */


      strcpy(t, snew);
      if ( !strtok(t, delimiters))
         numtokens = 0;
      else
         for (numtokens = 1; strtok(NULL, delimiters);
              numtokens++)
              ;

      /* create an argument array to contain ptrs to tokens */
      if (!(*argvp = (char **) calloc(numtokens + 1, sizeof(char *))) ) {
         numtokens = -1;
      } else {            /* insert pointers to tokens into the array */
         if (numtokens > 0) {
            strcpy(t, snew);
            **argvp = strdup(strtok(t, delimiters));
            for (i = 1; i < numtokens + 1; i++){
                char *ret = strtok(NULL, delimiters);
                if(ret)
                        *((*argvp) + i) = strdup(ret);
                else
                        *((*argvp) + i) = NULL;
            }
         } else {
           **argvp = NULL;
         }
      }

   free(t);
   return numtokens;
}

