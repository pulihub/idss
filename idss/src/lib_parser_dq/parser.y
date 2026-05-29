%{
 
#include "typeparser.h"
#include "parserparam.h"
#include "tree.h"
#include <string.h>
 
%}
 
%define api.pure
 
%token <svalue> SELECT
%token <svalue> FROM
%token <svalue> WHERE
%token <svalue> NAME
%token <svalue> COMMA
%token <svalue> SPACE
%token <svalue> ALIAS
%token <svalue> DOT
%token <svalue> OR
%token <svalue> AND
%token <svalue> OP
%token <svalue> CP
%token EOL
%token SEMICOLON
%token <dvalue> NUMBER
%token <dvalue> BOOL
%token <svalue> EQ
%token <svalue> GT
%token <svalue> LT
%token <svalue> GTE
%token <svalue> LTE
%token <svalue> NE
%token <svalue> ASTERISK
%token UNKNOWN
%token <svalue> STRING
%token <svalue> JOIN
%token <svalue> ON
%token <svalue> AGGF
%token <svalue> NOT
%token <svalue> IN

%left		OR
%left		AND
%left		COMMA JOIN

%type <tree> condition conditions
%type <svalue> dottedfieldname innerqueryconditions innerquerycondition innerquerywhereclause innerquerynoagg innerqueryagg tablelist tablename fieldname aggname fieldlist fieldlistnoagg fieldlistagg entire_clause completeoperator simpleoperator inclusionoperator

%%

entire_clause:
	SELECT SPACE fieldlist SPACE FROM SPACE tablelist whereclause	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); ((SParserParam*)data)->result.parentquery = $$; free($1); free($2); free($3); free($4); free($5); free($6); free($7);}
	;

fieldlist:
	fieldlistnoagg		{$$ = $1; ((SParserParam*)data)->result.parentIsAgg = 0;}
	| fieldlistagg		{$$ = $1; ((SParserParam*)data)->result.parentIsAgg = 1;}
	;

fieldlistnoagg:
	fieldname				{$$ = $1;}
	| fieldlistnoagg COMMA fieldname	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

fieldlistagg:
	aggname					{$$ = $1;}	
	| fieldlistagg COMMA aggname		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

tablelist:
	tablename					{$$ = $1;}	
	| tablelist COMMA tablename			{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	| tablelist JOIN tablename			{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	| tablelist JOIN tablename ON dottedfieldname completeoperator dottedfieldname		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); free($1); free($2); free($3); free($4); free($5); free($6); free($7);}
	;

tablename:
	NAME				{$$ = $1;}
	| NAME ALIAS NAME		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

fieldname:
	dottedfieldname			{$$ = $1;}
	| dottedfieldname ALIAS NAME	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

aggname:
	AGGF				{$$ = $1;}
	| AGGF ALIAS NAME		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

dottedfieldname:
	NAME				{$$ = $1;}
	| NAME DOT NAME			{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	| ASTERISK			{$$ = $1;}
	| NAME DOT ASTERISK		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	;

whereclause:							{((SParserParam*)data)->result.root = NULL;}
	| SEMICOLON						{((SParserParam*)data)->result.root = NULL;}
	| SPACE WHERE SPACE conditions				{((SParserParam*)data)->result.root = $4; free($1); free($2); free($3);}
	| SPACE WHERE SPACE conditions SEMICOLON		{((SParserParam*)data)->result.root = $4; free($1); free($2); free($3);}
	;

conditions:
	conditions SPACE OR SPACE OP conditions CP		{$$ = createOperation(eOR, $1, $6); free($2); free($3); free($4); free($5); free($7);}
	| conditions SPACE AND SPACE OP conditions CP		{$$ = createOperation(eAND, $1, $6); free($2); free($3); free($4); free($5); free($7);}
	| OP conditions CP					{$$ = $2; free($1); free($3);}
	| condition						{$$ = $1;}
	;

condition:
	dottedfieldname completeoperator NUMBER		{$$ = createLeaf(eCOND_NUM, $1, $2, NULL, $3, NULL); ((SParserParam*)data)->result.numOfLeafs++; free($1); free($2);}
	| dottedfieldname simpleoperator STRING	{$$ = createLeaf(eCOND_STR, $1, $2, NULL, 0, $3); ((SParserParam*)data)->result.numOfLeafs++; free($1); free($2); free($3);}
	| dottedfieldname simpleoperator BOOL		{$$ = createLeaf(eCOND_BOOL, $1, $2, NULL, $3, NULL); ((SParserParam*)data)->result.numOfLeafs++; free($1); free($2);}
	| dottedfieldname completeoperator innerqueryagg	{$$ = createLeaf(eSUBQ, $1, $2, $3, 0, NULL); ((SParserParam*)data)->result.numOfLeafs++; free($1); free($2); free($3);}
	| dottedfieldname inclusionoperator innerquerynoagg	{$$ = createLeaf(eSUBQNOAGG, $1, $2, $3, 0, NULL); ((SParserParam*)data)->result.numOfLeafs++; free($1); free($2); free($3);}
	;

completeoperator:
	EQ	{$$ = $1;}
	| GT	{$$ = $1;}
	| LT	{$$ = $1;}
	| GTE	{$$ = $1;}
	| LTE	{$$ = $1;}
	| NE	{$$ = $1;}
	;

simpleoperator:
	EQ	{$$ = $1;}
	| NE	{$$ = $1;}
	;

inclusionoperator:
	IN		{$$ = $1;}
	| NOT IN	{$$ = (char *)calloc(strlen($1) + strlen($2) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); free($1); free($2);}
	;

innerquerynoagg:
	OP SELECT SPACE fieldname SPACE FROM SPACE tablelist innerquerywhereclause CP		{$$ = (char *)calloc(strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + strlen($8) + strlen($9) + 1, sizeof(char));  strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); strcat($$, $8); strcat($$, $9);   ((SParserParam*)data)->result.numOfSubs++; free($1); free($2); free($3); free($4); free($5); free($6); free($7); free($8); free($9); free($10);}
	;

innerqueryagg:
	OP SELECT SPACE aggname SPACE FROM SPACE tablelist innerquerywhereclause CP		{$$ = (char *)calloc(strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + strlen($8) + strlen($9) + 1, sizeof(char));  strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); strcat($$, $8); strcat($$, $9);  ((SParserParam*)data)->result.numOfAggSubs++; ((SParserParam*)data)->result.numOfSubs++; free($1); free($2); free($3); free($4); free($5); free($6); free($7); free($8); free($9); free($10);}
	;

innerquerywhereclause:					{$$ = strdup("");}
	| SPACE WHERE SPACE innerqueryconditions	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + strlen($4) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); strcat($$, $4); free($1); free($2); free($3); free($4);}
	;
	
innerqueryconditions:
	innerqueryconditions SPACE OR SPACE OP innerqueryconditions CP		{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); free($1); free($2); free($3); free($4); free($5); free($6); free($7);}
	| innerqueryconditions SPACE AND SPACE OP innerqueryconditions CP	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + strlen($4) + strlen($5) + strlen($6) + strlen($7) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); strcat($$, $4); strcat($$, $5); strcat($$, $6); strcat($$, $7); free($1); free($2); free($3); free($4); free($5); free($6); free($7);}
	| OP innerqueryconditions CP	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	| innerquerycondition	{$$ = strdup($1); free($1);}
	;

innerquerycondition:
	dottedfieldname completeoperator NUMBER		{char temp[30]; snprintf(temp, 30, "%f", $3); $$ = (char *)calloc(strlen($1) + strlen($2) + strlen(temp) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, temp); free($1); free($2);}
	| dottedfieldname simpleoperator STRING	{$$ = (char *)calloc(strlen($1) + strlen($2) + strlen($3) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, $3); free($1); free($2); free($3);}
	| dottedfieldname simpleoperator BOOL		{char temp[30]; snprintf(temp, 30, "%f", $3); $$ = (char *)calloc(strlen($1) + strlen($2) + strlen(temp) + 1, sizeof(char)); strcat($$, $1); strcat($$, $2); strcat($$, temp); free($1); free($2);}
	;
