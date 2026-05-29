#ifndef MYKEY
#define MYKEY "MYKEY|"
#endif

#ifndef UQI
#define UQI "UQI|"
#endif

#ifndef RCVFROM
#define RCVFROM "RCVFROM|"
#endif

#ifndef RCVAT
#define RCVAT "RCVAT|"
#endif

#ifndef LOCEXSTART
#define LOCEXSTART "LOCEXSTART|"
#endif

#ifndef LOCEXEND
#define LOCEXEND "LOCEXEND|"
#endif

#ifndef LOCEXDUR
#define LOCEXDUR "LOCEXDUR|"
#endif

#ifndef COMPSTART
#define COMPSTART "COMPSTART|"
#endif

#ifndef COMPEND
#define COMPEND "COMPEND|"
#endif

#ifndef COMPDUR
#define COMPDUR "COMPDUR|"
#endif

#ifndef NESTEDDUR
#define NESTEDDUR "NESTEDDUR|"
#endif

#ifndef MERGINGDUR
#define MERGINGDUR "MERGINGDUR|"
#endif

#ifndef SEP
#define SEP "|"
#endif

#ifndef __P2P_UTILS_H__
#define __P2P_UTILS_H__

//Functions used for P2P
void get_random_value (char **value);
long long get_current_time ();
int print_result(char *uqi, char *ip, char *value);

#endif
