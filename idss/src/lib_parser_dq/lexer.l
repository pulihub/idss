%{

#include "typeparser.h"
#include "parser.h"
 
%}
 
%option reentrant noyywrap never-interactive nounistd case-insensitive
%option bison-bridge

%%

select									{yylval->svalue = strdup(yytext); return SELECT;}
from									{yylval->svalue = strdup(yytext); return FROM;}
where									{yylval->svalue = strdup(yytext); return WHERE;}
or									{yylval->svalue = strdup(yytext); return OR;}
and									{yylval->svalue = strdup(yytext); return AND;}
"."									{yylval->svalue = strdup(yytext); return DOT;}
[ \t]*[,][ \t]*								{yylval->svalue = strdup(yytext); return COMMA;}
[(][ \t]*								{yylval->svalue = strdup(yytext); return OP;}
[ \t]*[)]								{yylval->svalue = strdup(yytext); return CP;}
"*"									{yylval->svalue = strdup(yytext); return ASTERISK;}
true									{yylval->dvalue = 1; return BOOL;}
false									{yylval->dvalue = 0; return BOOL;}
[ \t]+join[ \t]+							{yylval->svalue = strdup(yytext); return JOIN;}
avg[ \t]*[(][ \t]*[a-zA-Z][a-zA-Z0-9_]*[ \t]*[)]			{yylval->svalue = strdup(yytext); return AGGF;}
sum[ \t]*[(][ \t]*[a-zA-Z][a-zA-Z0-9_]*[ \t]*[)]			{yylval->svalue = strdup(yytext); return AGGF;}
count[ \t]*[(][ \t]*(([a-zA-Z][a-zA-Z0-9_]*)|[*])[ \t]*[)]		{yylval->svalue = strdup(yytext); return AGGF;}
max[ \t]*[(][ \t]*[a-zA-Z][a-zA-Z0-9_]*[ \t]*[)]			{yylval->svalue = strdup(yytext); return AGGF;}
min[ \t]*[(][ \t]*[a-zA-Z][a-zA-Z0-9_]*[ \t]*[)]			{yylval->svalue = strdup(yytext); return AGGF;}
[ \t]+not								{yylval->svalue = strdup(" not"); return NOT;}
[ \t]+in[ \t]*								{yylval->svalue = strdup(" in"); return IN;}
[a-zA-Z][a-zA-Z0-9_]*							{yylval->svalue = strdup(yytext); return NAME;}
[ \t]+as[ \t]+								{yylval->svalue = strdup(yytext); return ALIAS;}
-?[0-9]+	|
-?[0-9]+"."[0-9]* |
-?"."[0-9]+      |
-?[0-9]+E[-+]?[0-9]+     |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+ |
-?"."[0-9]+E[-+]?[0-9]+ 						{yylval->dvalue = strtod(yytext, (char **)NULL); return NUMBER;}
'(\\.|''|[^'\n])*'   |
\"(\\.|\"\"|[^"\n])*\"							{yylval->svalue = strdup(yytext); return STRING;}

[ \t]*[=][=]?[ \t]*							{yylval->svalue = strdup(yytext); return EQ;}
[ \t]*[>][ \t]*								{yylval->svalue = strdup(yytext); return GT;}
[ \t]*[<][ \t]*								{yylval->svalue = strdup(yytext); return LT;}
[ \t]*[>][=][ \t]*							{yylval->svalue = strdup(yytext); return GTE;}
[ \t]*[<][=][ \t]*							{yylval->svalue = strdup(yytext); return LTE;}
[ \t]*[!][=][ \t]*							{yylval->svalue = strdup(yytext); return NE;}
[ \t]*[;]								return SEMICOLON;
\n                      						return EOL;
[ \t]+                  						{yylval->svalue = strdup(yytext); return SPACE;}
.        								return UNKNOWN;

%%

int yyerror(const char *msg) { fprintf(stderr,"Error:%s\n",msg); return 0; }
