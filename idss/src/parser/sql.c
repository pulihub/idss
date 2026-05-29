/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "sql.y"


/*#define YYDEBUG 1*/
/*-------------------------------------------------------------------------
 *
 * gram.y
 *	  POSTGRES SQL YACC rules/actions
 *
 *
 * This software belongs to PostgreSQL parsing code
 * It has been modified to handle only SELECT statements
 * rejecting anything else
 *
 * Code maintainer: Massimo Cafaro
 *
 * Portions Copyright (c) 1996-2003, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * NOTES
 *	  CAPITALS are used to represent terminal symbols.
 *	  non-capitals are used to represent non-terminals.
 *	  SQL92-specific syntax is separated from plain SQL/Postgres syntax
 *	  to help isolate the non-extensible portions of the parser.
 *
 *	  In general, nothing in this file should initiate database accesses
 *	  nor depend on changeable state (such as SET variables).  If you do
 *	  database accesses, your code will fail when we have aborted the
 *	  current transaction and are just parsing commands to find the next
 *	  ROLLBACK or COMMIT.  If you make use of SET variables, then you
 *	  will do the wrong thing in multi-query strings like this:
 *			SET SQL_inheritance TO off; SELECT * FROM foo;
 *	  because the entire string is parsed by gram.y before the SET gets
 *	  executed.  Anything that depends on the database or changeable state
 *	  should be handled inside parse_analyze() so that it happens at the
 *	  right time not the wrong time.  The handling of SQL_inheritance is
 *	  a good example.
 *
 * WARNINGS
 *	  If you use a list, make sure the datum is a node so that the printing
 *	  routines work.
 *
 *	  Sometimes we assign constants to makeStrings. Make sure we don't free
 *	  those.
 *
 *-------------------------------------------------------------------------
 */


/*
 * If you need access to certain yacc-generated variables and find that
 * they're static by default, uncomment the next line.  (this is not a
 * problem, yet.)
 */
/*#define __YYSCLASS*/

void yyerror(const char *message);
int yylex(void);

int idss_parse(char *str);


#line 134 "sql.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "sql.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ABORT_P = 3,                    /* ABORT_P  */
  YYSYMBOL_ABSOLUTE_P = 4,                 /* ABSOLUTE_P  */
  YYSYMBOL_ACCESS = 5,                     /* ACCESS  */
  YYSYMBOL_ACTION = 6,                     /* ACTION  */
  YYSYMBOL_ADD = 7,                        /* ADD  */
  YYSYMBOL_AFTER = 8,                      /* AFTER  */
  YYSYMBOL_AGGREGATE = 9,                  /* AGGREGATE  */
  YYSYMBOL_ALL = 10,                       /* ALL  */
  YYSYMBOL_ALTER = 11,                     /* ALTER  */
  YYSYMBOL_ANALYSE = 12,                   /* ANALYSE  */
  YYSYMBOL_ANALYZE = 13,                   /* ANALYZE  */
  YYSYMBOL_AND = 14,                       /* AND  */
  YYSYMBOL_ANY = 15,                       /* ANY  */
  YYSYMBOL_ARRAY = 16,                     /* ARRAY  */
  YYSYMBOL_AS = 17,                        /* AS  */
  YYSYMBOL_ASC = 18,                       /* ASC  */
  YYSYMBOL_ASSERTION = 19,                 /* ASSERTION  */
  YYSYMBOL_ASSIGNMENT = 20,                /* ASSIGNMENT  */
  YYSYMBOL_AT = 21,                        /* AT  */
  YYSYMBOL_AUTHORIZATION = 22,             /* AUTHORIZATION  */
  YYSYMBOL_BACKWARD = 23,                  /* BACKWARD  */
  YYSYMBOL_BEFORE = 24,                    /* BEFORE  */
  YYSYMBOL_BEGIN_P = 25,                   /* BEGIN_P  */
  YYSYMBOL_BETWEEN = 26,                   /* BETWEEN  */
  YYSYMBOL_BIGINT = 27,                    /* BIGINT  */
  YYSYMBOL_BINARY = 28,                    /* BINARY  */
  YYSYMBOL_BIT = 29,                       /* BIT  */
  YYSYMBOL_BOOLEAN_P = 30,                 /* BOOLEAN_P  */
  YYSYMBOL_BOTH = 31,                      /* BOTH  */
  YYSYMBOL_BY = 32,                        /* BY  */
  YYSYMBOL_CACHE = 33,                     /* CACHE  */
  YYSYMBOL_CALLED = 34,                    /* CALLED  */
  YYSYMBOL_CASCADE = 35,                   /* CASCADE  */
  YYSYMBOL_CASE = 36,                      /* CASE  */
  YYSYMBOL_CAST = 37,                      /* CAST  */
  YYSYMBOL_CHAIN = 38,                     /* CHAIN  */
  YYSYMBOL_CHAR_P = 39,                    /* CHAR_P  */
  YYSYMBOL_CHARACTER = 40,                 /* CHARACTER  */
  YYSYMBOL_CHARACTERISTICS = 41,           /* CHARACTERISTICS  */
  YYSYMBOL_CHECK = 42,                     /* CHECK  */
  YYSYMBOL_CHECKPOINT = 43,                /* CHECKPOINT  */
  YYSYMBOL_CLASS = 44,                     /* CLASS  */
  YYSYMBOL_CLOSE = 45,                     /* CLOSE  */
  YYSYMBOL_CLUSTER = 46,                   /* CLUSTER  */
  YYSYMBOL_COALESCE = 47,                  /* COALESCE  */
  YYSYMBOL_COLLATE = 48,                   /* COLLATE  */
  YYSYMBOL_COLUMN = 49,                    /* COLUMN  */
  YYSYMBOL_COMMENT = 50,                   /* COMMENT  */
  YYSYMBOL_COMMIT = 51,                    /* COMMIT  */
  YYSYMBOL_COMMITTED = 52,                 /* COMMITTED  */
  YYSYMBOL_CONSTRAINT = 53,                /* CONSTRAINT  */
  YYSYMBOL_CONSTRAINTS = 54,               /* CONSTRAINTS  */
  YYSYMBOL_CONVERSION_P = 55,              /* CONVERSION_P  */
  YYSYMBOL_CONVERT = 56,                   /* CONVERT  */
  YYSYMBOL_COPY = 57,                      /* COPY  */
  YYSYMBOL_CREATE = 58,                    /* CREATE  */
  YYSYMBOL_CREATEDB = 59,                  /* CREATEDB  */
  YYSYMBOL_CREATEUSER = 60,                /* CREATEUSER  */
  YYSYMBOL_CROSS = 61,                     /* CROSS  */
  YYSYMBOL_CURRENT_DATE = 62,              /* CURRENT_DATE  */
  YYSYMBOL_CURRENT_TIME = 63,              /* CURRENT_TIME  */
  YYSYMBOL_CURRENT_TIMESTAMP = 64,         /* CURRENT_TIMESTAMP  */
  YYSYMBOL_CURRENT_USER = 65,              /* CURRENT_USER  */
  YYSYMBOL_CURSOR = 66,                    /* CURSOR  */
  YYSYMBOL_CYCLE = 67,                     /* CYCLE  */
  YYSYMBOL_DATABASE = 68,                  /* DATABASE  */
  YYSYMBOL_DAY_P = 69,                     /* DAY_P  */
  YYSYMBOL_DEALLOCATE = 70,                /* DEALLOCATE  */
  YYSYMBOL_DEC = 71,                       /* DEC  */
  YYSYMBOL_DECIMAL_P = 72,                 /* DECIMAL_P  */
  YYSYMBOL_DECLARE = 73,                   /* DECLARE  */
  YYSYMBOL_DEFAULT = 74,                   /* DEFAULT  */
  YYSYMBOL_DEFAULTS = 75,                  /* DEFAULTS  */
  YYSYMBOL_DEFERRABLE = 76,                /* DEFERRABLE  */
  YYSYMBOL_DEFERRED = 77,                  /* DEFERRED  */
  YYSYMBOL_DEFINER = 78,                   /* DEFINER  */
  YYSYMBOL_DELETE_P = 79,                  /* DELETE_P  */
  YYSYMBOL_DELIMITER = 80,                 /* DELIMITER  */
  YYSYMBOL_DELIMITERS = 81,                /* DELIMITERS  */
  YYSYMBOL_DESC = 82,                      /* DESC  */
  YYSYMBOL_DISTINCT = 83,                  /* DISTINCT  */
  YYSYMBOL_DO = 84,                        /* DO  */
  YYSYMBOL_DOMAIN_P = 85,                  /* DOMAIN_P  */
  YYSYMBOL_DOUBLE_P = 86,                  /* DOUBLE_P  */
  YYSYMBOL_DROP = 87,                      /* DROP  */
  YYSYMBOL_EACH = 88,                      /* EACH  */
  YYSYMBOL_ELSE = 89,                      /* ELSE  */
  YYSYMBOL_ENCODING = 90,                  /* ENCODING  */
  YYSYMBOL_ENCRYPTED = 91,                 /* ENCRYPTED  */
  YYSYMBOL_END_P = 92,                     /* END_P  */
  YYSYMBOL_ESCAPE = 93,                    /* ESCAPE  */
  YYSYMBOL_EXCEPT = 94,                    /* EXCEPT  */
  YYSYMBOL_EXCLUDING = 95,                 /* EXCLUDING  */
  YYSYMBOL_EXCLUSIVE = 96,                 /* EXCLUSIVE  */
  YYSYMBOL_EXECUTE = 97,                   /* EXECUTE  */
  YYSYMBOL_EXISTS = 98,                    /* EXISTS  */
  YYSYMBOL_EXPLAIN = 99,                   /* EXPLAIN  */
  YYSYMBOL_EXTERNAL = 100,                 /* EXTERNAL  */
  YYSYMBOL_EXTRACT = 101,                  /* EXTRACT  */
  YYSYMBOL_FALSE_P = 102,                  /* FALSE_P  */
  YYSYMBOL_FETCH = 103,                    /* FETCH  */
  YYSYMBOL_FIRST_P = 104,                  /* FIRST_P  */
  YYSYMBOL_FLOAT_P = 105,                  /* FLOAT_P  */
  YYSYMBOL_FOR = 106,                      /* FOR  */
  YYSYMBOL_FORCE = 107,                    /* FORCE  */
  YYSYMBOL_FOREIGN = 108,                  /* FOREIGN  */
  YYSYMBOL_FORWARD = 109,                  /* FORWARD  */
  YYSYMBOL_FREEZE = 110,                   /* FREEZE  */
  YYSYMBOL_FROM = 111,                     /* FROM  */
  YYSYMBOL_FULL = 112,                     /* FULL  */
  YYSYMBOL_FUNCTION = 113,                 /* FUNCTION  */
  YYSYMBOL_GLOBAL = 114,                   /* GLOBAL  */
  YYSYMBOL_GRANT = 115,                    /* GRANT  */
  YYSYMBOL_GROUP_P = 116,                  /* GROUP_P  */
  YYSYMBOL_HANDLER = 117,                  /* HANDLER  */
  YYSYMBOL_HAVING = 118,                   /* HAVING  */
  YYSYMBOL_HOLD = 119,                     /* HOLD  */
  YYSYMBOL_HOUR_P = 120,                   /* HOUR_P  */
  YYSYMBOL_ILIKE = 121,                    /* ILIKE  */
  YYSYMBOL_IMMEDIATE = 122,                /* IMMEDIATE  */
  YYSYMBOL_IMMUTABLE = 123,                /* IMMUTABLE  */
  YYSYMBOL_IMPLICIT_P = 124,               /* IMPLICIT_P  */
  YYSYMBOL_IN_P = 125,                     /* IN_P  */
  YYSYMBOL_INCLUDING = 126,                /* INCLUDING  */
  YYSYMBOL_INCREMENT = 127,                /* INCREMENT  */
  YYSYMBOL_INDEX = 128,                    /* INDEX  */
  YYSYMBOL_INHERITS = 129,                 /* INHERITS  */
  YYSYMBOL_INITIALLY = 130,                /* INITIALLY  */
  YYSYMBOL_INNER_P = 131,                  /* INNER_P  */
  YYSYMBOL_INOUT = 132,                    /* INOUT  */
  YYSYMBOL_INPUT_P = 133,                  /* INPUT_P  */
  YYSYMBOL_INSENSITIVE = 134,              /* INSENSITIVE  */
  YYSYMBOL_INSERT = 135,                   /* INSERT  */
  YYSYMBOL_INSTEAD = 136,                  /* INSTEAD  */
  YYSYMBOL_INT_P = 137,                    /* INT_P  */
  YYSYMBOL_INTEGER = 138,                  /* INTEGER  */
  YYSYMBOL_INTERSECT = 139,                /* INTERSECT  */
  YYSYMBOL_INTERVAL = 140,                 /* INTERVAL  */
  YYSYMBOL_INTO = 141,                     /* INTO  */
  YYSYMBOL_INVOKER = 142,                  /* INVOKER  */
  YYSYMBOL_IS = 143,                       /* IS  */
  YYSYMBOL_ISNULL = 144,                   /* ISNULL  */
  YYSYMBOL_ISOLATION = 145,                /* ISOLATION  */
  YYSYMBOL_JOIN = 146,                     /* JOIN  */
  YYSYMBOL_KEY = 147,                      /* KEY  */
  YYSYMBOL_LANCOMPILER = 148,              /* LANCOMPILER  */
  YYSYMBOL_LANGUAGE = 149,                 /* LANGUAGE  */
  YYSYMBOL_LAST_P = 150,                   /* LAST_P  */
  YYSYMBOL_LEADING = 151,                  /* LEADING  */
  YYSYMBOL_LEFT = 152,                     /* LEFT  */
  YYSYMBOL_LEVEL = 153,                    /* LEVEL  */
  YYSYMBOL_LIKE = 154,                     /* LIKE  */
  YYSYMBOL_LIMIT = 155,                    /* LIMIT  */
  YYSYMBOL_LISTEN = 156,                   /* LISTEN  */
  YYSYMBOL_LOAD = 157,                     /* LOAD  */
  YYSYMBOL_LOCAL = 158,                    /* LOCAL  */
  YYSYMBOL_LOCALTIME = 159,                /* LOCALTIME  */
  YYSYMBOL_LOCALTIMESTAMP = 160,           /* LOCALTIMESTAMP  */
  YYSYMBOL_LOCATION = 161,                 /* LOCATION  */
  YYSYMBOL_LOCK_P = 162,                   /* LOCK_P  */
  YYSYMBOL_MATCH = 163,                    /* MATCH  */
  YYSYMBOL_MAXVALUE = 164,                 /* MAXVALUE  */
  YYSYMBOL_MINUTE_P = 165,                 /* MINUTE_P  */
  YYSYMBOL_MINVALUE = 166,                 /* MINVALUE  */
  YYSYMBOL_MODE = 167,                     /* MODE  */
  YYSYMBOL_MONTH_P = 168,                  /* MONTH_P  */
  YYSYMBOL_MOVE = 169,                     /* MOVE  */
  YYSYMBOL_NAMES = 170,                    /* NAMES  */
  YYSYMBOL_NATIONAL = 171,                 /* NATIONAL  */
  YYSYMBOL_NATURAL = 172,                  /* NATURAL  */
  YYSYMBOL_NCHAR = 173,                    /* NCHAR  */
  YYSYMBOL_NEW = 174,                      /* NEW  */
  YYSYMBOL_NEXT = 175,                     /* NEXT  */
  YYSYMBOL_NO = 176,                       /* NO  */
  YYSYMBOL_NOCREATEDB = 177,               /* NOCREATEDB  */
  YYSYMBOL_NOCREATEUSER = 178,             /* NOCREATEUSER  */
  YYSYMBOL_NONE = 179,                     /* NONE  */
  YYSYMBOL_NOT = 180,                      /* NOT  */
  YYSYMBOL_NOTHING = 181,                  /* NOTHING  */
  YYSYMBOL_NOTIFY = 182,                   /* NOTIFY  */
  YYSYMBOL_NOTNULL = 183,                  /* NOTNULL  */
  YYSYMBOL_NULL_P = 184,                   /* NULL_P  */
  YYSYMBOL_NULLIF = 185,                   /* NULLIF  */
  YYSYMBOL_NUMERIC = 186,                  /* NUMERIC  */
  YYSYMBOL_OF = 187,                       /* OF  */
  YYSYMBOL_OFF = 188,                      /* OFF  */
  YYSYMBOL_OFFSET = 189,                   /* OFFSET  */
  YYSYMBOL_OIDS = 190,                     /* OIDS  */
  YYSYMBOL_OLD = 191,                      /* OLD  */
  YYSYMBOL_ON = 192,                       /* ON  */
  YYSYMBOL_ONLY = 193,                     /* ONLY  */
  YYSYMBOL_OPERATOR = 194,                 /* OPERATOR  */
  YYSYMBOL_OPTION = 195,                   /* OPTION  */
  YYSYMBOL_OR = 196,                       /* OR  */
  YYSYMBOL_ORDER = 197,                    /* ORDER  */
  YYSYMBOL_OUT_P = 198,                    /* OUT_P  */
  YYSYMBOL_OUTER_P = 199,                  /* OUTER_P  */
  YYSYMBOL_OVERLAPS = 200,                 /* OVERLAPS  */
  YYSYMBOL_OVERLAY = 201,                  /* OVERLAY  */
  YYSYMBOL_OWNER = 202,                    /* OWNER  */
  YYSYMBOL_PARTIAL = 203,                  /* PARTIAL  */
  YYSYMBOL_PASSWORD = 204,                 /* PASSWORD  */
  YYSYMBOL_PATH_P = 205,                   /* PATH_P  */
  YYSYMBOL_PENDANT = 206,                  /* PENDANT  */
  YYSYMBOL_PLACING = 207,                  /* PLACING  */
  YYSYMBOL_POSITION = 208,                 /* POSITION  */
  YYSYMBOL_PRECISION = 209,                /* PRECISION  */
  YYSYMBOL_PRESERVE = 210,                 /* PRESERVE  */
  YYSYMBOL_PREPARE = 211,                  /* PREPARE  */
  YYSYMBOL_PRIMARY = 212,                  /* PRIMARY  */
  YYSYMBOL_PRIOR = 213,                    /* PRIOR  */
  YYSYMBOL_PRIVILEGES = 214,               /* PRIVILEGES  */
  YYSYMBOL_PROCEDURAL = 215,               /* PROCEDURAL  */
  YYSYMBOL_PROCEDURE = 216,                /* PROCEDURE  */
  YYSYMBOL_READ = 217,                     /* READ  */
  YYSYMBOL_REAL = 218,                     /* REAL  */
  YYSYMBOL_RECHECK = 219,                  /* RECHECK  */
  YYSYMBOL_REFERENCES = 220,               /* REFERENCES  */
  YYSYMBOL_REINDEX = 221,                  /* REINDEX  */
  YYSYMBOL_RELATIVE_P = 222,               /* RELATIVE_P  */
  YYSYMBOL_RENAME = 223,                   /* RENAME  */
  YYSYMBOL_REPLACE = 224,                  /* REPLACE  */
  YYSYMBOL_RESET = 225,                    /* RESET  */
  YYSYMBOL_RESTART = 226,                  /* RESTART  */
  YYSYMBOL_RESTRICT = 227,                 /* RESTRICT  */
  YYSYMBOL_RETURNS = 228,                  /* RETURNS  */
  YYSYMBOL_REVOKE = 229,                   /* REVOKE  */
  YYSYMBOL_RIGHT = 230,                    /* RIGHT  */
  YYSYMBOL_ROLLBACK = 231,                 /* ROLLBACK  */
  YYSYMBOL_ROW = 232,                      /* ROW  */
  YYSYMBOL_ROWS = 233,                     /* ROWS  */
  YYSYMBOL_RULE = 234,                     /* RULE  */
  YYSYMBOL_SCHEMA = 235,                   /* SCHEMA  */
  YYSYMBOL_SCROLL = 236,                   /* SCROLL  */
  YYSYMBOL_SECOND_P = 237,                 /* SECOND_P  */
  YYSYMBOL_SECURITY = 238,                 /* SECURITY  */
  YYSYMBOL_SELECT = 239,                   /* SELECT  */
  YYSYMBOL_SEQUENCE = 240,                 /* SEQUENCE  */
  YYSYMBOL_SERIALIZABLE = 241,             /* SERIALIZABLE  */
  YYSYMBOL_SESSION = 242,                  /* SESSION  */
  YYSYMBOL_SESSION_USER = 243,             /* SESSION_USER  */
  YYSYMBOL_SET = 244,                      /* SET  */
  YYSYMBOL_SETOF = 245,                    /* SETOF  */
  YYSYMBOL_SHARE = 246,                    /* SHARE  */
  YYSYMBOL_SHOW = 247,                     /* SHOW  */
  YYSYMBOL_SIMILAR = 248,                  /* SIMILAR  */
  YYSYMBOL_SIMPLE = 249,                   /* SIMPLE  */
  YYSYMBOL_SMALLINT = 250,                 /* SMALLINT  */
  YYSYMBOL_SOME = 251,                     /* SOME  */
  YYSYMBOL_STABLE = 252,                   /* STABLE  */
  YYSYMBOL_START = 253,                    /* START  */
  YYSYMBOL_STATEMENT = 254,                /* STATEMENT  */
  YYSYMBOL_STATISTICS = 255,               /* STATISTICS  */
  YYSYMBOL_STDIN = 256,                    /* STDIN  */
  YYSYMBOL_STDOUT = 257,                   /* STDOUT  */
  YYSYMBOL_STORAGE = 258,                  /* STORAGE  */
  YYSYMBOL_STRICT_P = 259,                 /* STRICT_P  */
  YYSYMBOL_SUBSTRING = 260,                /* SUBSTRING  */
  YYSYMBOL_SYSID = 261,                    /* SYSID  */
  YYSYMBOL_TABLE = 262,                    /* TABLE  */
  YYSYMBOL_TEMP = 263,                     /* TEMP  */
  YYSYMBOL_TEMPLATE = 264,                 /* TEMPLATE  */
  YYSYMBOL_TEMPORARY = 265,                /* TEMPORARY  */
  YYSYMBOL_THEN = 266,                     /* THEN  */
  YYSYMBOL_TIME = 267,                     /* TIME  */
  YYSYMBOL_TIMESTAMP = 268,                /* TIMESTAMP  */
  YYSYMBOL_TO = 269,                       /* TO  */
  YYSYMBOL_TOAST = 270,                    /* TOAST  */
  YYSYMBOL_TRAILING = 271,                 /* TRAILING  */
  YYSYMBOL_TRANSACTION = 272,              /* TRANSACTION  */
  YYSYMBOL_TREAT = 273,                    /* TREAT  */
  YYSYMBOL_TRIGGER = 274,                  /* TRIGGER  */
  YYSYMBOL_TRIM = 275,                     /* TRIM  */
  YYSYMBOL_TRUE_P = 276,                   /* TRUE_P  */
  YYSYMBOL_TRUNCATE = 277,                 /* TRUNCATE  */
  YYSYMBOL_TRUSTED = 278,                  /* TRUSTED  */
  YYSYMBOL_TYPE_P = 279,                   /* TYPE_P  */
  YYSYMBOL_UNENCRYPTED = 280,              /* UNENCRYPTED  */
  YYSYMBOL_UNION = 281,                    /* UNION  */
  YYSYMBOL_UNIQUE = 282,                   /* UNIQUE  */
  YYSYMBOL_UNKNOWN = 283,                  /* UNKNOWN  */
  YYSYMBOL_UNLISTEN = 284,                 /* UNLISTEN  */
  YYSYMBOL_UNTIL = 285,                    /* UNTIL  */
  YYSYMBOL_UPDATE = 286,                   /* UPDATE  */
  YYSYMBOL_USAGE = 287,                    /* USAGE  */
  YYSYMBOL_USER = 288,                     /* USER  */
  YYSYMBOL_USING = 289,                    /* USING  */
  YYSYMBOL_VACUUM = 290,                   /* VACUUM  */
  YYSYMBOL_VALID = 291,                    /* VALID  */
  YYSYMBOL_VALIDATOR = 292,                /* VALIDATOR  */
  YYSYMBOL_VALUES = 293,                   /* VALUES  */
  YYSYMBOL_VARCHAR = 294,                  /* VARCHAR  */
  YYSYMBOL_VARYING = 295,                  /* VARYING  */
  YYSYMBOL_VERBOSE = 296,                  /* VERBOSE  */
  YYSYMBOL_VERSION = 297,                  /* VERSION  */
  YYSYMBOL_VIEW = 298,                     /* VIEW  */
  YYSYMBOL_VOLATILE = 299,                 /* VOLATILE  */
  YYSYMBOL_WHEN = 300,                     /* WHEN  */
  YYSYMBOL_WHERE = 301,                    /* WHERE  */
  YYSYMBOL_WITH = 302,                     /* WITH  */
  YYSYMBOL_WITHOUT = 303,                  /* WITHOUT  */
  YYSYMBOL_WORK = 304,                     /* WORK  */
  YYSYMBOL_WRITE = 305,                    /* WRITE  */
  YYSYMBOL_YEAR_P = 306,                   /* YEAR_P  */
  YYSYMBOL_ZONE = 307,                     /* ZONE  */
  YYSYMBOL_UNIONJOIN = 308,                /* UNIONJOIN  */
  YYSYMBOL_IDENT = 309,                    /* IDENT  */
  YYSYMBOL_FCONST = 310,                   /* FCONST  */
  YYSYMBOL_SCONST = 311,                   /* SCONST  */
  YYSYMBOL_BCONST = 312,                   /* BCONST  */
  YYSYMBOL_XCONST = 313,                   /* XCONST  */
  YYSYMBOL_Op = 314,                       /* Op  */
  YYSYMBOL_ICONST = 315,                   /* ICONST  */
  YYSYMBOL_PARAM = 316,                    /* PARAM  */
  YYSYMBOL_317_ = 317,                     /* '='  */
  YYSYMBOL_318_ = 318,                     /* '<'  */
  YYSYMBOL_319_ = 319,                     /* '>'  */
  YYSYMBOL_POSTFIXOP = 320,                /* POSTFIXOP  */
  YYSYMBOL_321_ = 321,                     /* '+'  */
  YYSYMBOL_322_ = 322,                     /* '-'  */
  YYSYMBOL_323_ = 323,                     /* '*'  */
  YYSYMBOL_324_ = 324,                     /* '/'  */
  YYSYMBOL_325_ = 325,                     /* '%'  */
  YYSYMBOL_326_ = 326,                     /* '^'  */
  YYSYMBOL_UMINUS = 327,                   /* UMINUS  */
  YYSYMBOL_328_ = 328,                     /* '['  */
  YYSYMBOL_329_ = 329,                     /* ']'  */
  YYSYMBOL_330_ = 330,                     /* '('  */
  YYSYMBOL_331_ = 331,                     /* ')'  */
  YYSYMBOL_TYPECAST = 332,                 /* TYPECAST  */
  YYSYMBOL_333_ = 333,                     /* '.'  */
  YYSYMBOL_334_ = 334,                     /* ';'  */
  YYSYMBOL_335_ = 335,                     /* ','  */
  YYSYMBOL_336_ = 336,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 337,                 /* $accept  */
  YYSYMBOL_stmtmulti = 338,                /* stmtmulti  */
  YYSYMBOL_stmt = 339,                     /* stmt  */
  YYSYMBOL_SelectStmt = 340,               /* SelectStmt  */
  YYSYMBOL_select_with_parens = 341,       /* select_with_parens  */
  YYSYMBOL_select_no_parens = 342,         /* select_no_parens  */
  YYSYMBOL_select_clause = 343,            /* select_clause  */
  YYSYMBOL_simple_select = 344,            /* simple_select  */
  YYSYMBOL_into_clause = 345,              /* into_clause  */
  YYSYMBOL_OptTempTableName = 346,         /* OptTempTableName  */
  YYSYMBOL_qualified_name = 347,           /* qualified_name  */
  YYSYMBOL_opt_table = 348,                /* opt_table  */
  YYSYMBOL_opt_all = 349,                  /* opt_all  */
  YYSYMBOL_opt_distinct = 350,             /* opt_distinct  */
  YYSYMBOL_opt_sort_clause = 351,          /* opt_sort_clause  */
  YYSYMBOL_sort_clause = 352,              /* sort_clause  */
  YYSYMBOL_sortby_list = 353,              /* sortby_list  */
  YYSYMBOL_sortby = 354,                   /* sortby  */
  YYSYMBOL_select_limit = 355,             /* select_limit  */
  YYSYMBOL_opt_select_limit = 356,         /* opt_select_limit  */
  YYSYMBOL_select_limit_value = 357,       /* select_limit_value  */
  YYSYMBOL_select_offset_value = 358,      /* select_offset_value  */
  YYSYMBOL_group_clause = 359,             /* group_clause  */
  YYSYMBOL_having_clause = 360,            /* having_clause  */
  YYSYMBOL_for_update_clause = 361,        /* for_update_clause  */
  YYSYMBOL_opt_for_update_clause = 362,    /* opt_for_update_clause  */
  YYSYMBOL_update_list = 363,              /* update_list  */
  YYSYMBOL_from_clause = 364,              /* from_clause  */
  YYSYMBOL_from_list = 365,                /* from_list  */
  YYSYMBOL_table_ref = 366,                /* table_ref  */
  YYSYMBOL_joined_table = 367,             /* joined_table  */
  YYSYMBOL_alias_clause = 368,             /* alias_clause  */
  YYSYMBOL_join_type = 369,                /* join_type  */
  YYSYMBOL_join_outer = 370,               /* join_outer  */
  YYSYMBOL_join_qual = 371,                /* join_qual  */
  YYSYMBOL_relation_expr = 372,            /* relation_expr  */
  YYSYMBOL_func_table = 373,               /* func_table  */
  YYSYMBOL_where_clause = 374,             /* where_clause  */
  YYSYMBOL_TableFuncElementList = 375,     /* TableFuncElementList  */
  YYSYMBOL_TableFuncElement = 376,         /* TableFuncElement  */
  YYSYMBOL_Typename = 377,                 /* Typename  */
  YYSYMBOL_opt_array_bounds = 378,         /* opt_array_bounds  */
  YYSYMBOL_SimpleTypename = 379,           /* SimpleTypename  */
  YYSYMBOL_ConstTypename = 380,            /* ConstTypename  */
  YYSYMBOL_GenericType = 381,              /* GenericType  */
  YYSYMBOL_Numeric = 382,                  /* Numeric  */
  YYSYMBOL_opt_float = 383,                /* opt_float  */
  YYSYMBOL_opt_numeric = 384,              /* opt_numeric  */
  YYSYMBOL_opt_decimal = 385,              /* opt_decimal  */
  YYSYMBOL_Bit = 386,                      /* Bit  */
  YYSYMBOL_ConstBit = 387,                 /* ConstBit  */
  YYSYMBOL_BitWithLength = 388,            /* BitWithLength  */
  YYSYMBOL_BitWithoutLength = 389,         /* BitWithoutLength  */
  YYSYMBOL_Character = 390,                /* Character  */
  YYSYMBOL_ConstCharacter = 391,           /* ConstCharacter  */
  YYSYMBOL_CharacterWithLength = 392,      /* CharacterWithLength  */
  YYSYMBOL_CharacterWithoutLength = 393,   /* CharacterWithoutLength  */
  YYSYMBOL_character = 394,                /* character  */
  YYSYMBOL_opt_varying = 395,              /* opt_varying  */
  YYSYMBOL_opt_charset = 396,              /* opt_charset  */
  YYSYMBOL_ConstDatetime = 397,            /* ConstDatetime  */
  YYSYMBOL_ConstInterval = 398,            /* ConstInterval  */
  YYSYMBOL_opt_timezone = 399,             /* opt_timezone  */
  YYSYMBOL_opt_interval = 400,             /* opt_interval  */
  YYSYMBOL_r_expr = 401,                   /* r_expr  */
  YYSYMBOL_row = 402,                      /* row  */
  YYSYMBOL_row_descriptor = 403,           /* row_descriptor  */
  YYSYMBOL_sub_type = 404,                 /* sub_type  */
  YYSYMBOL_all_Op = 405,                   /* all_Op  */
  YYSYMBOL_MathOp = 406,                   /* MathOp  */
  YYSYMBOL_qual_Op = 407,                  /* qual_Op  */
  YYSYMBOL_any_operator = 408,             /* any_operator  */
  YYSYMBOL_qual_all_Op = 409,              /* qual_all_Op  */
  YYSYMBOL_a_expr = 410,                   /* a_expr  */
  YYSYMBOL_b_expr = 411,                   /* b_expr  */
  YYSYMBOL_c_expr = 412,                   /* c_expr  */
  YYSYMBOL_any_name = 413,                 /* any_name  */
  YYSYMBOL_opt_indirection = 414,          /* opt_indirection  */
  YYSYMBOL_expr_list = 415,                /* expr_list  */
  YYSYMBOL_extract_list = 416,             /* extract_list  */
  YYSYMBOL_type_list = 417,                /* type_list  */
  YYSYMBOL_array_expr_list = 418,          /* array_expr_list  */
  YYSYMBOL_array_expr = 419,               /* array_expr  */
  YYSYMBOL_extract_arg = 420,              /* extract_arg  */
  YYSYMBOL_overlay_list = 421,             /* overlay_list  */
  YYSYMBOL_overlay_placing = 422,          /* overlay_placing  */
  YYSYMBOL_position_list = 423,            /* position_list  */
  YYSYMBOL_substr_list = 424,              /* substr_list  */
  YYSYMBOL_substr_from = 425,              /* substr_from  */
  YYSYMBOL_substr_for = 426,               /* substr_for  */
  YYSYMBOL_trim_list = 427,                /* trim_list  */
  YYSYMBOL_in_expr = 428,                  /* in_expr  */
  YYSYMBOL_case_expr = 429,                /* case_expr  */
  YYSYMBOL_when_clause_list = 430,         /* when_clause_list  */
  YYSYMBOL_when_clause = 431,              /* when_clause  */
  YYSYMBOL_case_default = 432,             /* case_default  */
  YYSYMBOL_case_arg = 433,                 /* case_arg  */
  YYSYMBOL_columnref = 434,                /* columnref  */
  YYSYMBOL_dotted_name = 435,              /* dotted_name  */
  YYSYMBOL_attrs = 436,                    /* attrs  */
  YYSYMBOL_target_list = 437,              /* target_list  */
  YYSYMBOL_target_el = 438,                /* target_el  */
  YYSYMBOL_relation_name = 439,            /* relation_name  */
  YYSYMBOL_name_list = 440,                /* name_list  */
  YYSYMBOL_name = 441,                     /* name  */
  YYSYMBOL_attr_name = 442,                /* attr_name  */
  YYSYMBOL_func_name = 443,                /* func_name  */
  YYSYMBOL_AexprConst = 444,               /* AexprConst  */
  YYSYMBOL_Iconst = 445,                   /* Iconst  */
  YYSYMBOL_Sconst = 446,                   /* Sconst  */
  YYSYMBOL_ColId = 447,                    /* ColId  */
  YYSYMBOL_type_name = 448,                /* type_name  */
  YYSYMBOL_function_name = 449,            /* function_name  */
  YYSYMBOL_ColLabel = 450,                 /* ColLabel  */
  YYSYMBOL_unreserved_keyword = 451,       /* unreserved_keyword  */
  YYSYMBOL_col_name_keyword = 452,         /* col_name_keyword  */
  YYSYMBOL_func_name_keyword = 453,        /* func_name_keyword  */
  YYSYMBOL_reserved_keyword = 454,         /* reserved_keyword  */
  YYSYMBOL_SpecialRuleRelation = 455       /* SpecialRuleRelation  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   14051

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  337
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  119
/* YYNRULES -- Number of rules.  */
#define YYNRULES  754
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1149

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   574


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   325,     2,     2,
     330,   331,   323,   321,   335,   322,   333,   324,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   336,   334,
     318,   317,   319,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   328,     2,   329,   326,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   320,   327,   332
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   218,   218,   219,   223,   271,   272,   276,   277,   287,
     288,   290,   292,   297,   298,   325,   329,   331,   333,   338,
     339,   347,   349,   351,   353,   355,   357,   359,   361,   367,
     368,   371,   372,   375,   376,   377,   384,   385,   386,   387,
     391,   392,   396,   400,   401,   404,   406,   408,   410,   416,
     418,   420,   422,   424,   429,   430,   435,   436,   441,   453,
     454,   458,   459,   463,   464,   468,   469,   473,   474,   486,
     487,   491,   492,   502,   504,   506,   508,   510,   512,   514,
     516,   531,   533,   535,   558,   560,   562,   564,   566,   568,
     570,   575,   577,   579,   581,   585,   586,   587,   588,   592,
     593,   605,   606,   611,   613,   615,   617,   622,   624,   630,
     631,   636,   638,   642,   656,   658,   660,   662,   667,   669,
     671,   684,   685,   686,   687,   688,   689,   691,   693,   707,
     708,   709,   710,   711,   715,   724,   726,   728,   730,   732,
     734,   736,   738,   740,   742,   744,   748,   750,   755,   757,
     759,   764,   766,   768,   777,   779,   785,   787,   792,   797,
     806,   808,   812,   814,   818,   822,   826,   828,   830,   832,
     834,   836,   841,   842,   846,   847,   851,   853,   855,   857,
     862,   866,   867,   868,   872,   873,   874,   875,   876,   877,
     878,   880,   882,   884,   886,   888,   890,   892,   907,   909,
     911,   914,   917,   920,   922,   924,   926,   935,   936,   937,
     938,   941,   944,   945,   946,   949,   950,   953,   954,   955,
     956,   957,   958,   959,   960,   961,   964,   966,   970,   971,
     975,   977,   996,   997,   999,  1010,  1012,  1014,  1016,  1018,
    1020,  1022,  1024,  1026,  1028,  1030,  1032,  1034,  1036,  1038,
    1041,  1043,  1045,  1048,  1050,  1052,  1055,  1057,  1059,  1061,
    1063,  1065,  1067,  1069,  1072,  1074,  1076,  1078,  1090,  1092,
    1094,  1096,  1098,  1100,  1102,  1104,  1106,  1108,  1110,  1112,
    1114,  1116,  1118,  1120,  1122,  1124,  1126,  1128,  1130,  1143,
    1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,  1163,
    1165,  1167,  1169,  1171,  1173,  1175,  1177,  1179,  1181,  1183,
    1185,  1187,  1199,  1200,  1201,  1203,  1205,  1207,  1209,  1211,
    1213,  1215,  1217,  1219,  1221,  1223,  1225,  1227,  1229,  1231,
    1233,  1235,  1237,  1239,  1241,  1243,  1245,  1247,  1249,  1251,
    1253,  1255,  1257,  1259,  1261,  1263,  1265,  1267,  1269,  1271,
    1273,  1277,  1278,  1286,  1288,  1290,  1294,  1296,  1301,  1303,
    1306,  1308,  1312,  1314,  1318,  1320,  1329,  1330,  1331,  1332,
    1333,  1334,  1335,  1336,  1345,  1347,  1352,  1359,  1360,  1375,
    1377,  1379,  1381,  1383,  1385,  1390,  1393,  1396,  1397,  1398,
    1401,  1403,  1425,  1427,  1429,  1435,  1436,  1440,  1445,  1446,
    1449,  1450,  1458,  1460,  1465,  1469,  1471,  1473,  1487,  1488,
    1492,  1494,  1496,  1515,  1516,  1521,  1523,  1528,  1531,  1533,
    1535,  1542,  1544,  1546,  1548,  1550,  1552,  1554,  1556,  1558,
    1560,  1562,  1564,  1568,  1569,  1584,  1585,  1586,  1591,  1592,
    1598,  1599,  1600,  1606,  1607,  1608,  1609,  1610,  1626,  1627,
    1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,  1637,
    1638,  1639,  1640,  1641,  1642,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1697,
    1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,
    1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,
    1718,  1719,  1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,
    1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,
    1738,  1739,  1740,  1741,  1742,  1743,  1744,  1745,  1746,  1747,
    1748,  1749,  1750,  1751,  1752,  1753,  1754,  1755,  1756,  1757,
    1758,  1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,  1767,
    1768,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,
    1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,  1786,  1787,
    1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,  1796,  1797,
    1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,
    1808,  1809,  1823,  1824,  1825,  1826,  1827,  1828,  1829,  1830,
    1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,  1839,  1840,
    1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,
    1851,  1852,  1853,  1867,  1868,  1869,  1870,  1871,  1872,  1873,
    1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,  1882,  1883,
    1884,  1885,  1886,  1887,  1897,  1898,  1899,  1900,  1901,  1902,
    1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,  1911,  1912,
    1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,
    1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,  1931,  1932,
    1933,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,  1942,
    1943,  1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1952,
    1953,  1954,  1955,  1956,  1957,  1958,  1959,  1960,  1961,  1962,
    1963,  1964,  1965,  1970,  1972
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ABORT_P",
  "ABSOLUTE_P", "ACCESS", "ACTION", "ADD", "AFTER", "AGGREGATE", "ALL",
  "ALTER", "ANALYSE", "ANALYZE", "AND", "ANY", "ARRAY", "AS", "ASC",
  "ASSERTION", "ASSIGNMENT", "AT", "AUTHORIZATION", "BACKWARD", "BEFORE",
  "BEGIN_P", "BETWEEN", "BIGINT", "BINARY", "BIT", "BOOLEAN_P", "BOTH",
  "BY", "CACHE", "CALLED", "CASCADE", "CASE", "CAST", "CHAIN", "CHAR_P",
  "CHARACTER", "CHARACTERISTICS", "CHECK", "CHECKPOINT", "CLASS", "CLOSE",
  "CLUSTER", "COALESCE", "COLLATE", "COLUMN", "COMMENT", "COMMIT",
  "COMMITTED", "CONSTRAINT", "CONSTRAINTS", "CONVERSION_P", "CONVERT",
  "COPY", "CREATE", "CREATEDB", "CREATEUSER", "CROSS", "CURRENT_DATE",
  "CURRENT_TIME", "CURRENT_TIMESTAMP", "CURRENT_USER", "CURSOR", "CYCLE",
  "DATABASE", "DAY_P", "DEALLOCATE", "DEC", "DECIMAL_P", "DECLARE",
  "DEFAULT", "DEFAULTS", "DEFERRABLE", "DEFERRED", "DEFINER", "DELETE_P",
  "DELIMITER", "DELIMITERS", "DESC", "DISTINCT", "DO", "DOMAIN_P",
  "DOUBLE_P", "DROP", "EACH", "ELSE", "ENCODING", "ENCRYPTED", "END_P",
  "ESCAPE", "EXCEPT", "EXCLUDING", "EXCLUSIVE", "EXECUTE", "EXISTS",
  "EXPLAIN", "EXTERNAL", "EXTRACT", "FALSE_P", "FETCH", "FIRST_P",
  "FLOAT_P", "FOR", "FORCE", "FOREIGN", "FORWARD", "FREEZE", "FROM",
  "FULL", "FUNCTION", "GLOBAL", "GRANT", "GROUP_P", "HANDLER", "HAVING",
  "HOLD", "HOUR_P", "ILIKE", "IMMEDIATE", "IMMUTABLE", "IMPLICIT_P",
  "IN_P", "INCLUDING", "INCREMENT", "INDEX", "INHERITS", "INITIALLY",
  "INNER_P", "INOUT", "INPUT_P", "INSENSITIVE", "INSERT", "INSTEAD",
  "INT_P", "INTEGER", "INTERSECT", "INTERVAL", "INTO", "INVOKER", "IS",
  "ISNULL", "ISOLATION", "JOIN", "KEY", "LANCOMPILER", "LANGUAGE",
  "LAST_P", "LEADING", "LEFT", "LEVEL", "LIKE", "LIMIT", "LISTEN", "LOAD",
  "LOCAL", "LOCALTIME", "LOCALTIMESTAMP", "LOCATION", "LOCK_P", "MATCH",
  "MAXVALUE", "MINUTE_P", "MINVALUE", "MODE", "MONTH_P", "MOVE", "NAMES",
  "NATIONAL", "NATURAL", "NCHAR", "NEW", "NEXT", "NO", "NOCREATEDB",
  "NOCREATEUSER", "NONE", "NOT", "NOTHING", "NOTIFY", "NOTNULL", "NULL_P",
  "NULLIF", "NUMERIC", "OF", "OFF", "OFFSET", "OIDS", "OLD", "ON", "ONLY",
  "OPERATOR", "OPTION", "OR", "ORDER", "OUT_P", "OUTER_P", "OVERLAPS",
  "OVERLAY", "OWNER", "PARTIAL", "PASSWORD", "PATH_P", "PENDANT",
  "PLACING", "POSITION", "PRECISION", "PRESERVE", "PREPARE", "PRIMARY",
  "PRIOR", "PRIVILEGES", "PROCEDURAL", "PROCEDURE", "READ", "REAL",
  "RECHECK", "REFERENCES", "REINDEX", "RELATIVE_P", "RENAME", "REPLACE",
  "RESET", "RESTART", "RESTRICT", "RETURNS", "REVOKE", "RIGHT", "ROLLBACK",
  "ROW", "ROWS", "RULE", "SCHEMA", "SCROLL", "SECOND_P", "SECURITY",
  "SELECT", "SEQUENCE", "SERIALIZABLE", "SESSION", "SESSION_USER", "SET",
  "SETOF", "SHARE", "SHOW", "SIMILAR", "SIMPLE", "SMALLINT", "SOME",
  "STABLE", "START", "STATEMENT", "STATISTICS", "STDIN", "STDOUT",
  "STORAGE", "STRICT_P", "SUBSTRING", "SYSID", "TABLE", "TEMP", "TEMPLATE",
  "TEMPORARY", "THEN", "TIME", "TIMESTAMP", "TO", "TOAST", "TRAILING",
  "TRANSACTION", "TREAT", "TRIGGER", "TRIM", "TRUE_P", "TRUNCATE",
  "TRUSTED", "TYPE_P", "UNENCRYPTED", "UNION", "UNIQUE", "UNKNOWN",
  "UNLISTEN", "UNTIL", "UPDATE", "USAGE", "USER", "USING", "VACUUM",
  "VALID", "VALIDATOR", "VALUES", "VARCHAR", "VARYING", "VERBOSE",
  "VERSION", "VIEW", "VOLATILE", "WHEN", "WHERE", "WITH", "WITHOUT",
  "WORK", "WRITE", "YEAR_P", "ZONE", "UNIONJOIN", "IDENT", "FCONST",
  "SCONST", "BCONST", "XCONST", "Op", "ICONST", "PARAM", "'='", "'<'",
  "'>'", "POSTFIXOP", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS",
  "'['", "']'", "'('", "')'", "TYPECAST", "'.'", "';'", "','", "':'",
  "$accept", "stmtmulti", "stmt", "SelectStmt", "select_with_parens",
  "select_no_parens", "select_clause", "simple_select", "into_clause",
  "OptTempTableName", "qualified_name", "opt_table", "opt_all",
  "opt_distinct", "opt_sort_clause", "sort_clause", "sortby_list",
  "sortby", "select_limit", "opt_select_limit", "select_limit_value",
  "select_offset_value", "group_clause", "having_clause",
  "for_update_clause", "opt_for_update_clause", "update_list",
  "from_clause", "from_list", "table_ref", "joined_table", "alias_clause",
  "join_type", "join_outer", "join_qual", "relation_expr", "func_table",
  "where_clause", "TableFuncElementList", "TableFuncElement", "Typename",
  "opt_array_bounds", "SimpleTypename", "ConstTypename", "GenericType",
  "Numeric", "opt_float", "opt_numeric", "opt_decimal", "Bit", "ConstBit",
  "BitWithLength", "BitWithoutLength", "Character", "ConstCharacter",
  "CharacterWithLength", "CharacterWithoutLength", "character",
  "opt_varying", "opt_charset", "ConstDatetime", "ConstInterval",
  "opt_timezone", "opt_interval", "r_expr", "row", "row_descriptor",
  "sub_type", "all_Op", "MathOp", "qual_Op", "any_operator", "qual_all_Op",
  "a_expr", "b_expr", "c_expr", "any_name", "opt_indirection", "expr_list",
  "extract_list", "type_list", "array_expr_list", "array_expr",
  "extract_arg", "overlay_list", "overlay_placing", "position_list",
  "substr_list", "substr_from", "substr_for", "trim_list", "in_expr",
  "case_expr", "when_clause_list", "when_clause", "case_default",
  "case_arg", "columnref", "dotted_name", "attrs", "target_list",
  "target_el", "relation_name", "name_list", "name", "attr_name",
  "func_name", "AexprConst", "Iconst", "Sconst", "ColId", "type_name",
  "function_name", "ColLabel", "unreserved_keyword", "col_name_keyword",
  "func_name_keyword", "reserved_keyword", "SpecialRuleRelation", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1011)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-606)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -166,    69,  -166,    10, -1011,  -249,  -240, -1011,     4,   -97,
   -1011,   -87,  3301,  -216,  -194, -1011, -1011, -1011,    73,    73,
     129,    73,   -11,    15,  -161, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011,   -58, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011,  -118, -1011,  -204,  -102, -1011, -1011, -1011, -1011,
    4941,  -114, -1011,    42,    42, -1011, -1011, -1011, -1011, -1011,
     -91, -1011, -1011, -1011, -1011, -1011,   -86, -1011, -1011, -1011,
   -1011, -1011,   -59,   -41, -1011, -1011, -1011, -1011, -1011, -1011,
    -184,  -184, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
     103, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,     1,
   -1011, -1011,    14, -1011, -1011, -1011,  -156, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011,    17,    56,  -146, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,    24,
      44, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011,   212, -1011,    42, -1011, -1011, -1011, -1011, -1011, -1011,
    4941, -1011, -1011, -1011, -1011,    55,  -144, -1011, -1011, -1011,
      75, -1011, -1011, -1011, -1011,    78, -1011, -1011, -1011, -1011,
   -1011,    83, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
      68, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011,    92, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
      86, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,   153,
   -1011, -1011, -1011, -1011,  -199,   -75, -1011, -1011,   158, -1011,
     175, -1011, -1011, -1011, -1011, -1011,     1, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011,    31, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,  -116,
   -1011, -1011, -1011, -1011, -1011, -1011,   116,  4941,  4941, -1011,
    4941,  4941,  3629, -1011,   199, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011,   -19, -1011,  -115, -1011,   902,  4941, 10013,
   -1011, -1011, -1011,   183,   -77, -1011,   116,   193, -1011, -1011,
   -1011, -1011, -1011, -1011,  -113, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011,  -166,  -166,  4941,  -166,  -133,  3957,  4941,   425,
     -53,  4941,  4285, -1011, -1011, -1011,   203, 10768,   234,  4941,
   -1011, -1011,  4941,  4941,   220,   220,   220, -1011, -1011, -1011,
   -1011,    -2,   220, -1011,   220,   220,   245,   245, -1011, 10976,
    4941,   220, -1011,  9077,  4941,  5269,  2002,  4941,   274,   275,
     220, -1011,   220, -1011,  4941,  2973, -1011,  9685,   215, -1011,
     213,   213,    -9,    26,   -38,   219, 10043,   211, -1011,   309,
     220, -1011,   220,    37,     1,    -3,   430,   227,  -208, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011,    25,   604,  4941, 11593,   291,  5269,  4941,   229,   327,
   -1011,  4941,    33, -1011,   231,  4941,   295,    40,  4941,  4941,
    4941,  4941,  4941,  4941,  4941,  6568,  6896, 13435,  5597,    43,
     215, 11900,  3301,   456,   215, -1011,  1673, -1011,   429, -1011,
   -1011,   237, -1011,  9979,   429,   376,   386, -1011,  -132, 10768,
     421, 10768, -1011, -1011, -1011, -1011, 10768,  -186,  -219,  -135,
   -1011,   220,  4941,   -37, -1011, 10077,   -36, 10227,   -29,   246,
     248,   -15, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
     249,   470,   251,   252,   254, -1011, -1011,  1336,   -10, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011,   255,   256, -1011, 10261,   260,  5269,  5269,
    5269,  5269,  3629,  5269,   684, -1011,   262, -1011,   264, 10328,
    9996,   261,   267,   283,   293,   271,   273, 10414,  4613,  4941,
    4613,  4613, 10478,   261,   278, -1011,   116, -1011,  4941,   215,
   -1011,   116,  4941, 13128,   280,   281,   344,   345,   346, -1011,
   -1011,   350, -1011, -1011,   509,   437, -1011,     1,  9077,    92,
    4941, -1011, -1011, -1011, -1011,  3629, -1011, -1011,     1, 10976,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011,   316,   830, 11018,  3629, -1011,
   -1011,   513, -1011,    39, -1011,   302, -1011, -1011, 11048,  5269,
    4941,   229,  4941,   372,  9077, 10857,  4941,  1150, 11003, 11003,
     152,   152,    -9,    -9,    -9,    26, -1011,   245, -1011,   245,
     245,   314,   314,   322, -1011, -1011, -1011,   245,   323, -1011,
   13742, -1011,  -131,   -90, -1011, -1011, -1011,   631, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011,   -33,   116, -1011,
     604,   324,    45,   253, 12207,  9303, 11164, -1011, -1011, -1011,
     116, -1011,  8153,   355,  4941,  4941,   332, -1011,    47,  4941,
   -1011, -1011,   725, -1011, 13128, -1011,  4941,  4941,  3957, -1011,
    4941, -1011, -1011,   339,   340, 10512,  4941, -1011,   578, 13435,
   -1011, 12207, -1011, -1011, -1011, -1011,   220, -1011,  4941, -1011,
   -1011, -1011,  4941, -1011,   220, -1011,  9077,  4941,   561, -1011,
     343,   343,   -72,   343, 10043,   -57,  5269,    -5,    75,  5269,
    5269,  5269,  5269,  5269,  5269,  5269,  7211,  7527, 13435,  6240,
   -1011, -1011, -1011,  4941,  4941,   570,   561, -1011, -1011, -1011,
      16,    16, 13435,   347,   261,   349,   351,  4941, -1011, -1011,
    9332,   215, -1011,  9352, -1011,   637,   199,   -54,   -69,   449,
     519,  -208, -1011, -1011,   358, -1011,  5925,  5269,  4941,    49,
    4941, -1011, -1011,   360, -1011, -1011, 13435,  4941,  1101, 11129,
   -1011, 11193,  4941,   364, 11238,   675,   371,   374,   220, -1011,
   -1011,  3629, -1011,   445,   445,   445,   445, -1011, -1011, 12207,
   12207,  8461,   378,  7841, 12514,   389,   380,   -22, -1011, 12514,
   12821,   183,   387,   390,  4941,   608,    63,    65, -1011, -1011,
   -1011, -1011,   392, -1011, -1011, -1011, -1011, -1011, 10768, -1011,
   -1011,  4941, 10768, -1011,   397,   398, -1011,   116,   400,   399,
   10768, 10662,   404, -1011, 10768,   570,  1117,   620,   550,   408,
    1117,  9122,  9122,   -94,   -94,   -72,   -72,   -72,   343, -1011,
     -57, 10768, 10768, -1011, -1011, -1011, -1011,   409, -1011, -1011,
   -1011,   261, -1011,  4941,   215, -1011,    37, -1011, -1011, -1011,
   -1011, -1011, -1011, -1011, -1011, -1011, -1011,   991,    67, -1011,
     204, 13435, -1011,    71,    67,  5269,  4941,  4941, 11263,    50,
    4941,   411,   374,   220,  -190,   412, 10696, 12207, 12207, 12207,
   12207, -1011, -1011, 12207, -1011,  2660,   -22,   414, 13128, -1011,
     416, -1011,  8153,   596,   554, -1011,  8153,   554,   -23,   554,
    8153,   609, -1011,  8769, -1011,   427,  2331, 10768,   728,   645,
   -1011, -1011, 13128, 10768, -1011, -1011, -1011, -1011, -1011, -1011,
    5269,   434, 13435, -1011, 10741, -1011,    76, -1011, 13435,   991,
      67,    67,  4941,    67,   220,   436, -1011,   438,    37, -1011,
   -1011, -1011, -1011, -1011,   435, 12514,   443, 13128,   -22,  8153,
   -1011, -1011,   -12, -1011,  8153,   630, -1011, -1011,  8153, 13128,
     447, 13128, -1011,    89,  4941,  4941, -1011, -1011,   336, 13435,
     112, -1011, -1011, -1011,    67,   450, -1011, -1011, -1011, -1011,
   -1011, 13128,   119, -1011,  4941,   452, -1011, -1011,  8153,   -12,
     121, -1011, 13435, 13128,   127, 13435, -1011,   261, 10768,   128,
   -1011, -1011,   133, -1011, 10768, 13128, -1011, -1011, -1011, 13128,
   -1011,   150, -1011, -1011, -1011,   151, -1011, -1011, -1011
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    39,     0,     0,     2,     0,    14,     5,    41,    13,
      38,    36,     0,    14,     0,     1,     3,     4,    35,    35,
       0,    35,     0,    40,     0,   448,   449,   450,   451,   452,
     453,   454,   455,     0,   456,   457,   458,   663,   459,   460,
     461,   664,   632,   665,   633,   634,   462,   463,   464,   465,
     401,     0,   466,   635,   636,   467,   468,   469,   470,   471,
     637,   472,   473,   474,   475,   476,   638,   477,   478,   479,
     666,   323,   324,   326,   332,   480,   481,   482,   483,   484,
     639,   640,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   641,
     502,   503,   642,   431,   504,   505,   643,   506,   507,   667,
     668,   508,   509,   510,   511,   512,   669,   513,   514,   515,
     670,   516,   517,   518,   519,   671,   520,   521,   522,   523,
     524,   644,   645,   646,   525,   672,   673,   526,   674,   527,
     528,   529,   530,   675,   531,   676,   532,   533,   534,   328,
     330,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   677,   647,   754,   546,   547,   548,   549,   648,
       0,   550,   551,   678,   432,   649,   650,   552,   553,   753,
     554,   555,   556,   679,   680,   651,   557,   558,   559,   560,
     561,   652,   562,   564,   563,   565,   566,   567,   568,   569,
     653,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   681,   580,   654,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   333,   590,   655,   591,   592,   682,   593,
     656,   594,   595,   596,   597,   598,   599,   600,   602,   657,
     601,   603,   604,   605,   658,   659,   606,   607,   660,   608,
     661,   430,   609,   610,   611,   612,     0,   613,   614,   615,
     616,   617,   334,   618,   619,   620,   621,   662,   622,   683,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   435,
     422,   434,   424,   425,   226,   433,   355,     0,     0,   412,
       0,     0,     0,   347,     0,   129,   130,   131,   156,   157,
     132,   162,   163,   175,   133,     0,   288,     0,     0,   411,
     232,   317,   312,   355,    20,   408,   355,     0,   313,   421,
     423,   414,   134,   419,   436,   437,   442,   413,     8,     7,
      33,    34,     0,     0,     0,     0,     0,     0,     0,    66,
      55,     0,     0,   349,   350,   172,   159,   400,     0,     0,
     167,   166,     0,     0,     0,     0,     0,   143,   142,   141,
     348,   359,     0,   140,     0,     0,   173,   173,   171,   255,
       0,     0,   144,     0,     0,   378,     0,   384,     0,     0,
       0,   179,     0,   177,     0,     0,   287,     0,   429,   355,
     235,   236,   237,   238,   347,     0,   356,     0,   426,     0,
       0,   165,     0,   197,     0,     0,     0,     0,     0,   215,
     225,   223,   224,   217,   218,   219,   220,   221,   222,   230,
     216,     0,   251,     0,     0,     0,     0,     0,     0,     0,
     268,     0,     0,   270,     0,     0,     0,   226,   225,   223,
     224,   217,   218,   219,   220,   239,   240,     0,   252,     0,
     403,     0,     0,    70,   402,   404,     0,    14,    18,    13,
      17,    42,    43,    48,    16,     0,    68,    57,    51,    56,
      52,    58,    65,    12,    54,    11,   356,     0,     0,     0,
     362,     0,     0,   399,   395,     0,     0,   356,     0,     0,
       0,     0,   369,   370,   371,   368,   372,   367,   366,   373,
       0,     0,     0,     0,     0,   170,   169,     0,     0,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   493,   641,
     642,   643,   644,   645,   646,   545,   647,   649,   650,   554,
     651,   652,   653,   654,   656,   657,   658,   659,   660,   661,
     662,   435,   228,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,   209,     0,   356,
     356,   383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   389,     0,   406,   405,   418,     0,   314,
     210,   355,     0,     0,     0,     0,   186,   187,   188,   185,
     189,   184,   427,   198,     0,     0,   203,     0,     0,     0,
       0,   205,   214,   212,   213,     0,   201,   202,     0,   253,
     684,   685,   686,   687,   688,   689,   690,   691,   692,   693,
     694,   695,   696,   697,   698,   699,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,   710,   711,   712,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,   723,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   443,
     410,   444,   445,   446,   447,     0,     0,   260,     0,   390,
     283,     0,   274,     0,   269,     0,   272,   276,   256,     0,
       0,     0,     0,     0,     0,   254,     0,   249,   247,   248,
     241,   242,   243,   244,   245,   246,   138,   173,   145,   173,
     173,   153,   153,   147,   135,   136,   180,   173,   150,   139,
       0,   137,   183,   183,   168,   438,   233,   120,   121,   122,
     123,   154,   155,   124,   160,   161,   125,   197,   134,   439,
     250,     0,   509,   534,     0,    32,    32,    19,    28,    30,
      29,   409,     0,   110,     0,     0,     0,   318,     0,     0,
      46,    47,     0,    64,     0,    63,     0,     0,     0,    37,
       0,   364,   365,     0,     0,     0,     0,   396,     0,     0,
     394,     0,   346,   325,   327,   152,     0,   336,     0,   146,
     329,   331,     0,   149,     0,   227,     0,     0,     0,   337,
     291,   292,   293,   294,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,   296,     0,   308,
     338,   207,   208,     0,     0,   381,   382,   339,   181,   182,
     183,   183,     0,     0,   388,     0,     0,     0,   344,   407,
       0,   316,   355,   211,   174,   175,     0,     0,     0,     0,
       0,     0,   204,   199,     0,   200,     0,     0,     0,     0,
       0,   275,   271,     0,   273,   277,     0,     0,     0,   262,
     284,   258,     0,     0,   264,   120,     0,   114,     0,   126,
     128,     0,   285,    32,    32,    32,    32,    27,    31,     0,
       0,     0,   435,     0,    80,   103,    69,    71,    82,    73,
      75,    30,     0,   436,     0,    60,     0,     0,   322,   319,
      44,    45,    67,   415,   417,    49,    53,    50,   357,   363,
     158,     0,   398,   392,     0,     0,   352,     0,   351,     0,
     358,     0,     0,   229,   376,   375,   377,     0,     0,     0,
     305,   303,   304,   297,   298,   299,   300,   301,   302,   290,
     306,   386,   385,   379,   380,   178,   176,     0,   341,   342,
     343,   387,   353,     0,   315,   164,   197,   191,   192,   193,
     194,   195,   196,   190,   206,   231,   234,   281,   261,   391,
     278,     0,   361,     0,   257,     0,     0,     0,   266,   227,
       0,     0,   115,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    21,     0,   105,    80,     0,    82,     0,    81,
      94,   104,     0,     0,   100,    98,     0,   100,     0,   100,
       0,     0,    74,     0,    76,    94,     0,   109,     0,    62,
     320,   321,     0,   397,   335,   345,   151,   393,   148,   374,
       0,     0,     0,   340,     0,   428,     0,   279,     0,   282,
     263,   259,     0,   265,     0,     0,   118,     0,   197,   286,
      26,    25,    24,    23,     0,    84,    92,     0,    72,     0,
      99,    95,     0,    96,     0,     0,    97,    86,     0,     0,
      92,     0,   107,     0,     0,     0,    15,   416,   309,     0,
       0,   354,   280,   360,   267,     0,   116,   119,   127,   106,
      83,     0,     0,    85,     0,     0,    88,    90,     0,     0,
       0,   111,     0,     0,     0,   417,   108,    59,    61,     0,
     310,   117,     0,    93,   102,     0,    89,    87,    77,     0,
     113,     0,    79,   311,    91,     0,   112,    78,   101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1011, -1011,   777, -1011,     0,   259,   -45,   169, -1011, -1011,
    -379,  -705,   317, -1011, -1011, -1011, -1011,    19,   444, -1011,
       7,  -305, -1011, -1011,   464, -1011, -1011, -1011, -1011,  -167,
    -117,  -834,  -239,  -510,  -328, -1011, -1011, -1011, -1010,  -335,
    -441,   -80,    80, -1011,  -425,  -416, -1011, -1011,   -74, -1011,
   -1011,  -403,  -373, -1011, -1011,  -277,  -250, -1011,   -16,   -43,
    -241,   -84,  -234,  -734, -1011,  -394,   439,   375,  -345, -1011,
     247,  -528,  -306,    -8,   587,  -329, -1011,  -268,  -323, -1011,
    -914, -1011,  -337, -1011, -1011, -1011, -1011, -1011,  -688,  -766,
     -62,   120, -1011, -1011,   330, -1011, -1011, -1011,   242,  -278,
   -1011,   368,   586,  -740,  -214, -1011,  -670, -1011,  -339,  -285,
    -259,   107, -1011, -1011,   492,   405,   407, -1011, -1011
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     5,   293,    14,     8,     9,   453,   757,
     905,   899,   332,    12,    22,    23,   461,   462,   339,   475,
     468,   470,  1039,  1096,   340,   473,   775,   763,   906,   907,
     908,  1019,  1031,  1081,  1116,   909,   910,   915,  1120,  1121,
     992,   887,   737,   294,   295,   296,   363,   372,   357,   740,
     297,   298,   299,   743,   300,   301,   302,   303,   346,   401,
     304,   305,   381,   592,   306,   307,   395,   608,   419,   420,
     308,   543,   449,   476,   554,   310,   935,   388,   397,   500,
     993,   479,   344,   501,   547,   808,   556,   562,   835,   836,
     574,   690,   311,   483,   484,   788,   348,   312,   313,   455,
     314,   315,   316,  1112,   923,   576,   317,   318,   319,   320,
     321,   322,   323,   680,   324,   325,   326,   684,   327
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       6,   421,    13,     6,   309,   480,   736,   358,   389,   398,
      15,   383,   425,   889,   601,   489,   490,   491,   477,   478,
     403,   399,   738,   502,   599,   503,   504,   607,   542,   486,
     488,   739,   508,   343,   922,   602,   586,   350,   351,  1023,
     603,   565,   347,   566,   741,   450,   555,   425,   454,  1023,
    -215,   900,   786,   602,   561,  -215,   -14,   776,   603,   699,
    -231,   584,   573,   585,   451,  -231,   977,   492,   -14,   963,
     864,  -168,   758,     1,   742,  1032,  1034,  1056,   947,    10,
     594,  1124,  -173,   330,   465,    17,   817,   587,   425,  1024,
    1024,   345,   912,   426,    -6,   336,   980,   555,    18,   360,
    1024,   -14,   337,   378,   379,    24,   586,  -173,  1025,  1025,
     781,   978,  -183,  1141,   544,   328,   780,   -14,   493,  1025,
     945,   579,   600,  1084,  1026,   285,  -173,  -153,   577,  1027,
    1027,   380,   588,   768,  1026,   589,   338,   329,  1100,  1066,
    1027,   871,   784,    19,   337,   779,   356,   368,   964,   780,
    1028,   -14,    11,   466,   700,  -147,   331,   587,   701,   -14,
    1028,   334,   369,   494,     2,  -180,   495,  -150,   981,   341,
     744,   378,   379,   425,   362,   948,   883,   595,   338,  1049,
    1114,   596,   949,   979,  -180,  1129,   371,   702,  1007,  1008,
    1009,  1010,   428,  -138,   782,  -438,   281,   745,  -439,   380,
     783,    20,   588,   777,   590,   589,   746,  1029,  1029,  -145,
     429,   430,   378,   379,  -440,   402,   349,  -441,  1029,   555,
     555,   555,   555,   872,   555,   425,   873,   378,   379,   824,
     825,   826,   827,   912,    -9,   496,  -183,    -9,   828,   352,
     382,  1110,  1055,   -14,   353,   573,   844,   573,   573,     1,
     433,   366,   367,   542,   827,   382,   386,   599,   452,     7,
     828,   434,     7,   482,   822,   823,   824,   825,   826,   827,
     342,   354,     2,   591,   590,   828,   604,  1115,   943,   390,
     391,   703,   392,   393,   396,    21,  1030,   458,   460,   355,
     464,  -215,   394,   328,   604,   790,  1030,   888,   849,   780,
     422,  -231,   792,   852,   497,   738,   780,   498,   893,   499,
     894,   400,   359,   851,   739,   874,   795,   446,   378,   379,
     796,   803,   875,   447,   854,   804,   463,   741,  -135,   469,
     471,     2,   457,   457,  1108,   457,   333,   345,   335,   544,
       2,   485,  -168,   591,   361,   487,   -10,  -606,   934,   -10,
     505,   506,   912,  -173,   364,   605,   912,   742,   447,   542,
     912,  -168,   507,   747,   738,   869,   546,  -136,   559,   560,
     555,  1132,  -173,   739,   365,   897,   567,   572,   919,  -139,
     989,   437,   780,  1132,   780,   370,   741,   959,   441,   442,
     443,   444,   445,   446,  1040,  1145,  1041,  -137,   780,   447,
     780,   967,  1057,   738,   593,   373,  1058,  1102,   374,   912,
     691,  1058,   739,   375,   912,   609,   742,   738,   912,   687,
    1126,   606,   376,   698,   780,   741,   739,   705,   689,   692,
     707,   708,   709,   710,   711,   712,   713,   714,   715,   741,
     750,   916,   917,  1130,   309,   544,   929,  1058,   912,   387,
    1133,   738,  1138,   744,  1042,   742,  1139,   939,  1142,  1143,
     739,   542,  1139,  1058,  1144,   942,   921,   984,  1042,   742,
     890,   925,   926,   741,   785,   443,   444,   445,   446,  -606,
     745,  1147,  1148,   377,   447,  1139,  1042,   555,   384,   746,
     555,   555,   555,   555,   555,   555,   555,   555,   555,   383,
     555,   459,   459,   742,   459,   385,   843,   693,   845,   846,
     281,   694,   744,  -420,   695,   924,   895,  1083,   896,  1086,
    1011,  1012,  1014,   456,   971,   441,   442,   443,   444,   445,
     446,   336,   938,   481,   482,   285,   447,   986,   555,   745,
     345,   563,   564,   578,   814,   447,   582,   544,   746,  1005,
     580,   744,   394,   583,   748,   597,   448,   598,   685,   688,
     572,   704,   572,   572,   706,   744,   738,   762,    19,   773,
     850,   976,   769,   774,   853,   739,   778,   793,   745,   794,
     797,   798,   799,   800,   974,   801,   805,   746,   741,   806,
     838,   809,   745,   830,   448,   831,   780,   863,   837,   744,
     839,   746,   840,   696,   841,   394,   965,   966,   865,   848,
     697,   855,   856,   857,   858,   859,   448,  1103,   742,   860,
     861,   862,   553,   866,   870,   425,   745,   738,  1070,  1071,
    1072,  1073,   876,   738,  1074,   746,   739,   448,   448,   448,
     448,   882,   739,   448,   356,  1020,   747,   886,   358,   741,
    1020,  1035,   362,   371,   891,   741,   914,   822,   823,   824,
     825,   826,   827,   918,  1065,  1067,   555,   342,   828,   448,
     933,   930,   834,   553,   738,   828,   833,   399,   968,   742,
     969,  1140,   970,   739,  1140,   742,   982,   983,   394,   985,
     991,  1001,   879,   759,   881,   999,   741,   738,   884,  1003,
     738,   689,  1004,   350,   351,   747,   739,   898,  -440,   739,
     448,   368,  1021,  1093,   744,  1022,   448,  1036,   448,   741,
    -441,   555,   741,   448,  1038,  1105,   742,  1042,  1044,  1045,
    1046,  1050,   448,  -414,   448,  1048,  1016,  1051,  1052,  1064,
    1053,   745,  1079,  1068,   747,  1075,  1077,   429,   430,   742,
     746,   892,   742,  1080,   448,  1088,  1020,  1091,   747,  1076,
    1094,   463,   904,  1095,  1099,  1106,  1109,  1107,   471,   471,
     469,  1127,   928,  1111,  1090,   744,  1118,  1123,   932,  1131,
      16,   744,  1135,   924,   474,   927,  1017,   433,   920,  1085,
     940,  1137,   747,   448,   941,   553,   553,   553,   553,   944,
     553,   829,   745,   472,  1146,  1002,   448,   448,   745,   816,
     885,   746,   975,   787,   448,   558,  1020,   746,   924,   448,
     761,   880,   744,     0,   751,   961,   962,   817,  1097,   682,
    1122,   683,  1125,     0,     0,     0,     0,   748,     0,     0,
       0,     0,     0,     0,   867,   744,     0,     0,   744,   745,
       0,     0,   924,     0,     0,  1078,   448,     0,   746,  1082,
     988,     0,   990,  1087,  1125,   545,     0,     0,     0,   994,
       0,     0,   745,     0,   998,   745,   924,     0,   818,   545,
    1122,   746,     0,  1006,   746,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,   748,     0,     0,     0,
       0,     0,     0,  1015,     0,     0,  1037,   747,     0,     0,
       0,     0,  1113,     0,     0,     0,   681,  1117,     0,   407,
       0,  1119,     0,  1043,     0,   441,   442,   443,   444,   445,
     446,     0,     0,   829,   448,   748,   447,     0,     0,   749,
       0,     0,     0,   545,     0,   448,   553,     0,     0,   748,
       0,  1136,   448,     0,   448,   448,   448,   448,   448,   448,
     448,   448,   448,     0,     0,  1054,     0,     0,   747,     0,
       0,     0,     0,   817,   747,     0,     0,     0,     0,     0,
       0,     0,     0,   748,     0,     0,     0,     0,  1060,  1061,
       0,     0,  1063,     0,     0,     0,   759,   448,   284,     0,
       0,   819,   820,   821,   911,   822,   823,   824,   825,   826,
     827,     0,     0,   686,     0,   747,   828,     0,     0,     0,
       0,     0,   904,     0,   818,     0,   904,   404,     0,     0,
     904,     0,   448,   936,     0,     0,     0,   760,   747,   409,
       0,   747,   410,   411,   412,   405,   413,   414,   415,   416,
     417,   418,     0,     0,  1104,     0,     0,   829,   829,   829,
     829,   448,   829,   553,     0,     0,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   545,   553,     0,     0,   904,
       0,     0,   406,     0,   904,     0,     0,  1128,   904,     0,
     545,     0,     0,     0,     0,     0,   407,   448,   748,     0,
     448,     0,   408,     0,     0,     0,  1134,     0,     0,     0,
       0,     0,     0,     0,   553,   995,     0,     0,   904,     0,
       0,     0,     0,     0,     0,   829,   448,     0,   448,     0,
       0,   448,     0,     0,   817,   810,   811,   812,   813,     0,
     815,   759,   759,   759,   284,   911,     0,   819,   820,   821,
       0,   822,   823,   824,   825,   826,   827,     0,     0,   748,
       0,     0,   828,     0,     0,   748,     0,     0,     0,     0,
       0,   425,     0,     0,     0,   448,   426,     0,     0,   448,
       0,     0,     0,     0,     0,   818,     0,   448,   448,     0,
       0,   448,     0,   829,     0,     0,   545,   829,   829,   829,
     829,   829,   829,   829,   829,   829,   748,   829,   448,   448,
       0,     0,     0,     0,     0,     0,   409,     0,     0,   410,
     411,   412,   749,   413,   414,   415,   416,   417,   418,   748,
       0,     0,   748,     0,   829,   448,     0,   448,     0,     0,
       0,   448,   553,     0,   817,   448,   545,     0,     0,   759,
     759,   759,   759,   448,   913,   759,     0,     0,     0,     0,
     817,     0,     0,     0,   911,     0,   545,     0,   911,     0,
       0,   427,   911,     0,     0,   428,     0,     0,     0,     0,
       0,   749,     0,   545,   448,     0,   878,     0,     0,     0,
     448,     0,     0,   429,   430,   818,     0,   553,   545,     0,
       0,   448,     0,     0,   431,   284,   829,   448,   448,     0,
     448,   818,   822,   823,   824,   825,   826,   827,     0,     0,
     749,   911,     0,   828,     0,     0,   911,     0,     0,     0,
     911,     0,     0,   433,   749,     0,     0,     0,     0,     0,
     760,     0,     0,     0,   434,   829,     0,     0,   760,     0,
     423,   448,     0,     0,     0,     0,     0,   425,     0,     0,
     911,     0,   426,     0,     0,     0,     0,     0,   749,     0,
       0,     0,     0,     0,     0,   448,     0,   937,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   545,   545,   545,     0,   913,   545,     0,   436,     0,
       0,   545,   545,   946,     0,     0,   950,   951,   952,   953,
     954,   955,   956,   957,   958,   284,   960,     0,   819,   820,
     821,     0,   822,   823,   824,   825,   826,   827,     0,     0,
       0,   284,     0,   828,   819,   820,   821,     0,   822,   823,
     824,   825,   826,   827,     0,     0,     0,     0,     0,   828,
       0,     0,     0,     0,   987,     0,     0,   427,     0,     0,
       0,   428,     0,     0,   437,     0,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,     0,     0,   429,
     430,     0,   447,   749,     0,   760,   760,   760,     0,   760,
     431,     0,     0,     0,     0,     0,     0,     0,     0,   545,
     545,   545,   545,     0,     0,   545,     0,   545,     0,     0,
     545,     0,     0,     0,   913,     0,   432,     0,   913,   433,
       0,     0,   913,     0,     0,   545,     0,     0,     0,     0,
     434,     0,   435,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,     0,     0,     0,     0,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   545,     0,   545,
       0,   913,     0,     0,     0,     0,   913,     0,     0,     0,
     913,   545,  1059,   545,   436,     0,     0,     0,     0,     0,
       0,   749,     0,   760,   760,   760,   760,     0,     0,   760,
       0,     0,     0,   545,     0,     0,     0,     0,   760,     0,
     913,     0,   760,     0,   749,   545,   760,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   545,     0,     0,
       0,   545,     0,     0,     0,     0,     0,  1098,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,     0,     0,   760,     0,     0,   447,     0,
     760,   802,     0,     0,   760,     0,    25,    26,    27,    28,
      29,    30,    31,   764,    32,     0,     0,     0,     0,    33,
       0,     0,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,   760,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,    58,    59,
      60,     0,     0,    61,    62,    63,     0,    64,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,    83,     0,
      84,    85,    86,    87,    88,     0,   765,     0,    89,    90,
      91,    92,     0,    93,    94,     0,    95,     0,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
     107,     0,   108,   109,     0,   110,   111,   112,     0,     0,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,   125,   126,   127,   128,   129,   130,
     131,   132,     0,   133,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,     0,   143,   144,   145,     0,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   178,   179,     0,     0,   180,   181,     0,
       0,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   191,   192,   193,   194,     0,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,     0,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,     0,   246,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,   256,   257,   258,   259,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,     0,     0,   273,   274,   275,   276,   277,
     278,     0,   279,   280,   281,   282,   283,   284,   285,   286,
       0,     0,     0,     0,   287,   288,   766,     0,   290,   291,
       0,     0,     0,   292,   767,    25,    26,    27,    28,    29,
      30,    31,     0,    32,     0,     0,     0,     0,    33,     0,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,     0,    56,    57,    58,    59,    60,
       0,     0,    61,    62,    63,     0,    64,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,     0,    83,     0,    84,
      85,    86,    87,    88,     0,     0,     0,    89,    90,    91,
      92,     0,    93,    94,     0,    95,     0,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   107,
       0,   108,   109,     0,   110,   111,   112,     0,     0,   113,
       0,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,     0,   125,   126,   127,   128,   129,   130,   131,
     132,     0,   133,     0,   134,   135,   136,   137,   138,   139,
     140,   141,   142,     0,   143,   144,   145,     0,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,   178,   179,     0,     0,   180,   181,     0,     0,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     191,   192,   193,   194,     0,   195,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,     0,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,     0,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,     0,   246,     0,   247,   248,   249,   250,   251,   252,
     253,   254,   255,     0,   256,   257,   258,   259,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,     0,     0,   273,   274,   275,   276,   277,   278,
       0,   279,   280,   281,   282,   283,   284,   285,   286,     0,
       0,     0,     0,   287,   288,     0,     0,   290,   291,     0,
       0,     0,   292,   557,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,    33,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,    58,    59,    60,     0,
       0,    61,    62,    63,     0,    64,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,    90,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,     0,
     108,   109,     0,   110,   111,   112,     0,     0,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,   125,   126,   127,   128,   129,   130,   131,   132,
       0,   133,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,     0,   143,   144,   145,     0,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,   178,   179,     0,     0,   180,   181,     0,     0,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   191,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
       0,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   256,   257,   258,   259,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     279,   280,   281,   282,   283,   284,   285,   286,     0,     0,
       0,     0,   287,   288,     0,     0,   290,   291,     0,     0,
       0,   292,  1092,    25,    26,    27,    28,    29,    30,    31,
       0,    32,     0,     0,     0,     0,     0,  1018,     0,    34,
      35,    36,     0,    38,    39,    40,     0,   509,     0,   510,
     511,     0,    46,    47,    48,    49,     0,     0,    52,   512,
     513,    55,     0,    56,    57,    58,    59,   514,     0,     0,
      61,    62,    63,     0,    64,    65,   515,    67,     0,    68,
      69,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   516,   517,    82,     0,    83,     0,    84,    85,    86,
      87,    88,     0,     0,     0,    89,   518,    91,    92,     0,
      93,    94,     0,    95,   -14,    96,    97,    98,   519,   100,
     101,   520,     0,   104,   105,   521,   -14,   107,     0,   108,
       0,     0,     0,   111,   112,     0,     0,   113,     0,   114,
     115,     0,   117,   118,   119,     0,   121,   122,   123,   124,
       0,     0,   126,   127,   128,   129,   130,   522,   523,   -14,
     524,     0,   134,     0,     0,   137,     0,   139,   140,   141,
     142,     0,     0,   144,     0,   -14,   146,   147,   148,     0,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   525,     0,   526,     0,   165,   166,   167,   168,   169,
       0,   171,   172,     0,     0,   527,   528,   177,     0,   -14,
     178,     0,     0,     0,   529,   181,     0,   -14,   182,     0,
       0,   530,   186,   187,   188,   189,   190,     0,   531,   192,
     193,   194,     0,   195,   196,   197,   198,   199,   532,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,   212,   533,   214,   215,   216,   217,   218,   219,     0,
     220,   221,   222,     0,   224,   225,   226,   227,     0,   229,
     534,     0,   231,   232,   233,   234,   235,   236,   237,   238,
     535,   240,     0,   241,   242,   243,     0,   536,   537,     0,
     246,     0,   247,   538,   249,   539,     0,   252,   253,   254,
     255,   -14,     0,   257,   258,   259,   260,   261,     0,     0,
     263,   264,   265,   266,   540,   268,     0,   270,   271,   272,
       0,     0,   273,   274,   275,   276,   277,   278,     0,   541,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,    30,    31,     0,    32,     0,     0,     0,     0,    33,
       0,   328,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,   568,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,    58,    59,
      60,     0,     0,    61,    62,    63,     0,    64,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,    83,     0,
      84,    85,    86,    87,    88,     0,     0,     0,    89,    90,
      91,    92,     0,    93,    94,     0,    95,     0,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
     107,     0,   108,   109,   569,   110,   111,   112,     0,     0,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,   125,   126,   127,   128,   129,   130,
     131,   132,     0,   133,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   570,   143,   144,   145,     0,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   178,   179,     0,     0,   180,   181,     0,
       0,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   191,   192,   193,   194,     0,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,     0,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,     0,   246,   571,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,   256,   257,   258,   259,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,     0,     0,   273,   274,   275,   276,   277,
     278,     0,   279,   280,   281,   282,   283,   284,   285,   286,
       0,     0,     0,     0,   287,   288,     0,     0,   290,   291,
       0,     0,     0,   292,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,    33,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,    58,    59,    60,     0,
       0,    61,    62,    63,     0,    64,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,    90,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,     0,
     108,   109,     0,   110,   111,   112,     0,     0,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,   125,   126,   127,   128,   129,   130,   131,   132,
       0,   133,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,     0,   143,   144,   145,     0,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,   178,   179,     0,     0,   180,   181,     0,     0,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   191,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
       0,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   256,   257,   258,   259,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     279,   280,   281,   282,   283,   284,   285,   286,     0,     0,
       0,     0,   287,   288,   289,     0,   290,   291,     0,     0,
       0,   292,    25,    26,    27,    28,    29,    30,    31,     0,
      32,     0,     0,     0,     0,    33,     0,     0,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,     0,    56,    57,    58,    59,    60,     0,     0,    61,
      62,    63,     0,    64,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,    83,     0,    84,    85,    86,    87,
      88,     0,     0,     0,    89,    90,    91,    92,     0,    93,
      94,     0,    95,     0,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,     0,   107,     0,   108,   109,
       0,   110,   111,   112,     0,     0,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,     0,
     125,   126,   127,   128,   129,   130,   131,   132,     0,   133,
       0,   134,   135,   136,   137,   138,   139,   140,   141,   142,
       0,   143,   144,   145,     0,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,   178,
     179,     0,     0,   180,   181,     0,     0,   182,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   191,   192,   193,
     194,     0,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,     1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
       0,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   241,   242,   243,     0,   244,   245,     0,   246,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
       0,   256,   257,   258,   259,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,     0,
       0,   273,   274,   275,   276,   277,   278,     0,   279,   280,
     281,   282,   283,   284,   285,   286,     0,     0,     0,     0,
     287,   288,     0,     0,   290,   291,     0,     0,     0,   292,
      25,    26,    27,    28,    29,    30,    31,   467,    32,     0,
       0,     0,     0,    33,     0,     0,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,     0,
      56,    57,    58,    59,    60,     0,     0,    61,    62,    63,
       0,    64,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,    83,     0,    84,    85,    86,    87,    88,     0,
       0,     0,    89,    90,    91,    92,     0,    93,    94,     0,
      95,     0,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   107,     0,   108,   109,     0,   110,
     111,   112,     0,     0,   113,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,     0,   125,   126,
     127,   128,   129,   130,   131,   132,     0,   133,     0,   134,
     135,   136,   137,   138,   139,   140,   141,   142,     0,   143,
     144,   145,     0,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,   178,   179,     0,
       0,   180,   181,     0,     0,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   191,   192,   193,   194,     0,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,     0,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     241,   242,   243,     0,   244,   245,     0,   246,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,   256,
     257,   258,   259,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,     0,     0,   273,
     274,   275,   276,   277,   278,     0,   279,   280,   281,   282,
     283,   284,   285,   286,     0,     0,     0,     0,   287,   288,
       0,     0,   290,   291,     0,     0,     0,   292,    25,    26,
      27,    28,    29,    30,    31,     0,    32,     0,     0,     0,
       0,    33,     0,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,     0,    56,    57,
      58,    59,    60,     0,     0,    61,    62,    63,     0,    64,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
      83,     0,    84,    85,    86,    87,    88,     0,     0,     0,
      89,    90,    91,    92,     0,    93,    94,     0,    95,     0,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   107,     0,   108,   109,     0,   110,   111,   112,
       0,     0,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,   125,   126,   127,   128,
     129,   130,   131,   132,     0,   133,     0,   134,   135,   136,
     137,   138,   139,   140,   141,   142,     0,   143,   144,   145,
       0,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,   178,   179,     0,     0,   180,
     181,     0,     0,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   191,   192,   193,   194,     0,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,     0,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,     0,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,     0,   246,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,     0,   256,   257,   258,
     259,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,     0,     0,   273,   274,   275,
     276,   277,   278,     0,   279,   280,   281,   282,   283,   284,
     285,   286,     0,     0,     0,     0,   287,   288,     0,     0,
     290,   291,     0,   342,     0,   292,    25,    26,    27,    28,
      29,    30,    31,     0,    32,     0,     0,     0,     0,    33,
       0,     0,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,     0,    56,    57,    58,    59,
      60,     0,     0,    61,    62,    63,     0,    64,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,    83,     0,
      84,    85,    86,    87,    88,     0,     0,     0,    89,    90,
      91,    92,     0,    93,    94,     0,    95,     0,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
     107,     0,   108,   109,   569,   110,   111,   112,     0,     0,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,   125,   126,   127,   128,   129,   130,
     131,   132,     0,   133,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,     0,   143,   144,   145,     0,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   178,   179,     0,     0,   180,   181,     0,
       0,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   191,   192,   193,   194,     0,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,     0,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,     0,   246,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,   256,   257,   258,   259,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,     0,     0,   273,   274,   275,   276,   277,
     278,     0,   279,   280,   281,   282,   283,   284,   285,   286,
       0,     0,     0,     0,   287,   288,     0,     0,   290,   291,
       0,     0,     0,   292,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,    33,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,    58,    59,    60,     0,
       0,    61,    62,    63,     0,    64,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,    90,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,     0,
     108,   109,     0,   110,   111,   112,     0,     0,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,   125,   126,   127,   128,   129,   130,   131,   132,
       0,   133,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,     0,   143,   144,   145,     0,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,   178,   179,     0,     0,   180,   181,     0,     0,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   191,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
       0,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,   256,   257,   258,   259,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     279,   280,   281,   282,   283,   284,   285,   286,     0,     0,
       0,     0,   287,   288,     0,     0,   290,   291,     0,     0,
       0,   292,    25,    26,    27,    28,    29,    30,    31,     0,
      32,     0,     0,     0,     0,    33,     0,     0,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,     0,    56,    57,    58,    59,    60,     0,     0,    61,
      62,    63,     0,    64,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,    83,     0,    84,    85,    86,    87,
      88,     0,     0,     0,    89,    90,    91,    92,     0,    93,
      94,     0,    95,     0,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,     0,   107,     0,   108,   109,
       0,   110,   111,   112,     0,     0,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,     0,
     125,   126,   127,   128,   129,   130,   131,   132,     0,   133,
       0,   134,   135,   136,   137,   138,   139,   140,   141,   142,
       0,   143,   144,   145,     0,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,     0,
     171,   172,   173,   174,   175,   176,   177,     0,     0,   178,
     179,     0,     0,   180,   181,     0,     0,   182,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   191,   192,   193,
     194,     0,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   533,   214,   215,   216,   217,   218,   219,     0,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
       0,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   241,   242,   243,     0,   244,   245,     0,   246,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
       0,     0,   257,   258,   259,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,     0,
       0,   273,   274,   275,   276,   277,   278,     0,   279,   280,
     281,   282,   283,   284,   285,   286,     0,     0,     0,     0,
     548,   549,     0,     0,   550,   551,     0,     0,     0,   552,
      25,    26,    27,    28,    29,    30,    31,     0,    32,     0,
       0,     0,     0,    33,     0,     0,    34,    35,    36,    37,
      38,    39,    40,     0,    42,    43,    44,    45,     0,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,     0,
      56,    57,    58,    59,    60,     0,     0,    61,    62,    63,
       0,    64,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,    83,     0,    84,    85,    86,    87,    88,     0,
       0,     0,    89,    90,    91,    92,     0,    93,    94,     0,
       0,     0,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   107,     0,   108,   109,     0,   110,
     111,   112,     0,     0,   113,     0,   114,   115,     0,   117,
     118,   119,     0,   121,   122,   123,   124,     0,   125,   126,
     127,   128,   129,   130,   131,   132,     0,   133,     0,   134,
     135,   136,   137,   138,   139,   140,   141,   142,     0,   143,
     144,     0,     0,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,     0,   171,   172,
     173,   174,   175,   176,   177,     0,     0,   178,   179,     0,
       0,   180,   181,     0,     0,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   191,   192,   193,   194,     0,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,   220,   221,   222,
     223,   224,   225,   226,   227,     0,   229,   230,     0,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     241,   242,   243,     0,   244,   245,     0,   246,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,   256,
     257,   258,   259,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,     0,     0,   273,
     274,   275,   276,   277,   278,     0,   279,   280,   281,   282,
     283,   284,   285,   286,     0,     0,     0,     0,   287,   288,
       0,     0,   290,   291,     0,     0,     0,   292,    25,    26,
      27,    28,    29,    30,    31,     0,    32,     0,     0,     0,
       0,    33,     0,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,     0,    56,    57,
      58,    59,    60,     0,     0,    61,    62,    63,     0,    64,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
      83,     0,    84,    85,    86,    87,    88,     0,     0,     0,
      89,    90,    91,    92,     0,    93,    94,     0,    95,     0,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   107,     0,   108,   109,     0,   110,   111,   112,
       0,     0,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,   125,   126,   127,   128,
     129,   130,   131,   132,     0,   133,     0,   134,   135,   136,
     137,   138,   139,   140,   141,   142,     0,   143,   144,   145,
       0,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,     0,   171,   172,   173,   174,
     175,   176,   177,     0,     0,   178,   179,     0,     0,   529,
     181,     0,     0,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   191,   192,   193,   194,     0,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   533,   214,   215,
     216,   217,   218,   219,     0,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,     0,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,     0,   246,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,     0,     0,   257,   258,
     259,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,     0,     0,   273,   274,   275,
     276,   277,   278,     0,   279,   280,   281,   282,   283,     0,
     285,   286,     0,    25,    26,    27,    28,    29,    30,    31,
       0,    32,     0,     0,     0,   552,    33,     0,     0,    34,
      35,    36,    37,    38,    39,    40,     0,    42,    43,    44,
      45,     0,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,     0,    56,    57,    58,    59,    60,     0,     0,
      61,    62,    63,     0,    64,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,     0,    83,     0,    84,    85,    86,
      87,    88,     0,     0,     0,    89,    90,    91,    92,     0,
      93,    94,     0,     0,     0,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   107,     0,   108,
     109,     0,   110,   111,   112,     0,     0,   113,     0,   114,
     115,     0,   117,   118,   119,     0,   121,   122,   123,   124,
       0,   125,   126,   127,   128,   129,   130,   131,   132,     0,
     133,     0,   134,   135,   136,   137,   138,   139,   140,   141,
     142,     0,   143,   144,     0,     0,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       0,   171,   172,   173,   174,   175,   176,   177,     0,     0,
     178,   179,     0,     0,   180,   181,     0,     0,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   191,   192,
     193,   194,     0,   195,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   533,   214,   215,   216,   217,   218,   219,     0,
     220,   221,   222,   223,   224,   225,   226,   227,     0,   229,
     230,     0,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,     0,
     246,     0,   247,   248,   249,   250,   251,   252,   253,   254,
     255,     0,     0,   257,   258,   259,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
       0,     0,   273,   274,   275,   276,   277,   278,     0,   279,
     280,   281,   282,   283,   284,   285,   286,     0,     0,     0,
       0,   548,   549,     0,     0,   550,   551,     0,     0,     0,
     552,    25,    26,    27,    28,    29,    30,    31,  -221,    32,
       0,     0,     0,  -221,    33,     0,     0,    34,    35,    36,
      37,    38,    39,    40,     0,    42,    43,    44,    45,     0,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
       0,    56,    57,    58,    59,    60,     0,     0,    61,    62,
      63,     0,    64,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,     0,    83,     0,    84,    85,    86,    87,    88,
       0,     0,     0,    89,    90,    91,    92,     0,    93,    94,
       0,     0,     0,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   107,     0,   108,   109,     0,
     110,   111,   112,     0,     0,   113,     0,   114,   115,     0,
     117,   118,   119,     0,   121,   122,   123,   124,     0,   125,
     126,   127,   128,   129,   130,   131,   132,     0,   133,     0,
     134,     0,     0,   137,   138,   139,   140,   141,   142,     0,
     143,   144,     0,     0,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,     0,   171,
     172,     0,   174,   175,   176,   177,     0,     0,   178,   179,
       0,     0,     0,   181,     0,     0,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   191,   192,   193,   194,
       0,   195,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,     0,   220,   221,
     222,   223,   224,   225,   226,   227,     0,   229,   230,  -221,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,     0,   246,     0,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
     256,   257,   258,   259,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,     0,     0,
     273,   274,   275,   276,   277,   278,     0,   279,   280,   281,
     282,   283,     0,   285,   286,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,   292,    25,
      26,    27,    28,    29,    30,    31,  -222,    32,     0,     0,
       0,  -222,    33,     0,     0,    34,    35,    36,    37,    38,
      39,    40,     0,    42,    43,    44,    45,     0,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,     0,    56,
      57,    58,    59,    60,     0,     0,    61,    62,    63,     0,
      64,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       0,    83,     0,    84,    85,    86,    87,    88,     0,     0,
       0,    89,    90,    91,    92,     0,    93,    94,     0,     0,
       0,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   107,     0,   108,   109,     0,   110,   111,
     112,     0,     0,   113,     0,   114,   115,     0,   117,   118,
     119,     0,   121,   122,   123,   124,     0,   125,   126,   127,
     128,   129,   130,   131,   132,     0,   133,     0,   134,     0,
       0,   137,   138,   139,   140,   141,   142,     0,   143,   144,
       0,     0,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,     0,   171,   172,     0,
     174,   175,   176,   177,     0,     0,   178,   179,     0,     0,
       0,   181,     0,     0,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,     0,   195,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,     0,   220,   221,   222,   223,
     224,   225,   226,   227,     0,   229,   230,  -222,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,     0,   246,     0,   247,   248,
     249,   250,   251,   252,   253,   254,   255,     0,   256,   257,
     258,   259,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,     0,     0,   273,   274,
     275,   276,   277,   278,     0,   279,   280,   281,   282,   283,
       0,   285,   286,     0,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,   292,    33,     0,     0,
      34,    35,    36,    37,    38,    39,    40,     0,    42,    43,
      44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,    56,    57,    58,    59,    60,     0,
       0,    61,    62,    63,     0,    64,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,    90,    91,    92,
       0,    93,    94,     0,     0,     0,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,     0,
     108,   109,     0,   110,   111,   112,     0,     0,   113,     0,
     114,   115,     0,   117,   118,   119,     0,   121,   122,   123,
     124,     0,   125,   126,   127,   128,   129,   130,   131,   132,
       0,   133,     0,   134,     0,     0,   137,   138,   139,   140,
     141,   142,     0,   143,   144,     0,     0,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,     0,   171,   172,     0,   174,   175,   176,   177,     0,
       0,   178,   179,     0,     0,     0,   181,     0,     0,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   191,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   533,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,   223,   224,   225,   226,   227,     0,
     229,   230,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
       0,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     0,     0,   257,   258,   259,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     279,   280,   281,   282,   283,     0,   285,   286,     0,     0,
      25,    26,    27,    28,    29,    30,    31,   551,    32,     0,
       0,   552,     0,    33,     0,     0,    34,    35,    36,    37,
      38,    39,    40,     0,    42,    43,    44,    45,     0,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,     0,
      56,    57,    58,    59,    60,     0,     0,    61,    62,    63,
       0,    64,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,    83,     0,    84,    85,    86,    87,    88,     0,
       0,     0,    89,    90,    91,    92,     0,    93,    94,     0,
       0,     0,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   107,     0,   108,   109,     0,   110,
     111,   112,     0,     0,   113,     0,   114,   115,     0,   117,
     118,   119,     0,   121,   122,   123,   124,     0,   125,   126,
     127,   128,   129,   130,   131,   132,     0,   133,     0,   134,
       0,     0,   137,   138,   139,   140,   141,   142,     0,   143,
     144,     0,     0,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,     0,   171,   172,
       0,   174,   175,   176,   177,     0,     0,   178,   179,     0,
       0,     0,   181,     0,     0,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   191,   192,   193,   194,     0,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   533,
     214,   215,   216,   217,   218,   219,     0,   220,   221,   222,
     223,   224,   225,   226,   227,     0,   229,   230,     0,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,     0,
     241,   242,   243,     0,   244,   245,     0,   246,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,     0,     0,
     257,   258,   259,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,     0,     0,   273,
     274,   275,   276,   277,   278,     0,   279,   280,   281,   282,
     283,     0,   285,   286,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,   552,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,   509,    43,
     510,   511,     0,    46,    47,    48,    49,     0,     0,    52,
     512,   513,    55,     0,    56,    57,    58,    59,   514,     0,
       0,    61,    62,    63,     0,    64,    65,   515,    67,     0,
      68,    69,    70,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   516,   517,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,   518,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,   519,
     100,   101,   520,     0,   104,   105,   521,     0,   107,     0,
     108,   109,     0,   110,   111,   112,     0,     0,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,   125,   126,   127,   128,   129,   130,   522,   523,
       0,   524,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,     0,   143,   144,   145,     0,   146,   147,   148,
       0,     0,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   525,   162,   526,   164,   165,   166,   167,   168,
     169,     0,   171,   172,   173,     0,   527,   528,   177,     0,
       0,   178,   179,     0,   901,   529,   181,     0,     0,   182,
     183,   184,   530,   186,   187,   188,   189,   190,     0,   531,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   532,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   533,   214,   215,   216,   217,   218,   219,
       1,   220,   221,   222,     0,   224,   225,   226,   227,   228,
     229,   534,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   535,   240,     0,   241,   242,   243,     0,   536,   537,
       0,   246,     0,   247,   538,   249,   539,     0,   252,   253,
     254,   255,     0,     0,   257,   258,   259,   260,   261,     0,
       0,   263,   264,   265,   266,   540,   268,   269,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     902,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,    30,    31,     0,    32,     0,     0,     0,     0,     0,
       0,   903,    34,    35,    36,    37,    38,    39,    40,    41,
     509,    43,   510,   511,     0,    46,    47,    48,    49,     0,
       0,    52,   512,   513,    55,     0,    56,    57,    58,    59,
     514,     0,     0,    61,    62,    63,     0,    64,    65,   515,
      67,     0,    68,    69,    70,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   516,   517,    82,     0,    83,     0,
      84,    85,    86,    87,    88,     0,     0,     0,    89,   518,
      91,    92,     0,    93,    94,     0,    95,     0,    96,    97,
      98,   519,   100,   101,   520,     0,   104,   105,   521,     0,
     107,     0,   108,   109,     0,   110,   111,   112,     0,     0,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,   125,   126,   127,   128,   129,   130,
     522,   523,     0,   524,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,     0,   143,   144,   145,     0,   146,
     147,   148,     0,     0,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   525,   162,   526,   164,   165,   166,
     167,   168,   169,     0,   171,   172,   173,     0,   527,   528,
     177,     0,     0,   178,   179,     0,   901,   529,   181,     0,
       0,   182,   183,   184,   530,   186,   187,   188,   189,   190,
       0,   531,   192,   193,   194,     0,   195,   196,   197,   198,
     199,   532,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   533,   214,   215,   216,   217,
     218,   219,     0,   220,   221,   222,     0,   224,   225,   226,
     227,   228,   229,   534,     0,   231,   232,   233,   234,   235,
     236,   237,   238,   535,   240,     0,   241,   242,   243,     0,
     536,   537,     0,   246,     0,   247,   538,   249,   539,     0,
     252,   253,   254,   255,     0,     0,   257,   258,   259,   260,
     261,     0,     0,   263,   264,   265,   266,   540,   268,   269,
     270,   271,   272,     0,     0,   273,   274,   275,   276,   277,
     278,     0,   902,     0,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,     0,     0,     0,
      34,    35,    36,   903,    38,    39,    40,     0,   509,     0,
     510,   511,     0,    46,    47,    48,    49,     0,     0,    52,
     512,   513,    55,     0,    56,    57,    58,    59,   514,     0,
       0,    61,    62,    63,     0,    64,    65,   515,    67,     0,
      68,    69,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   516,   517,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,   518,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,   519,
     100,   101,   520,     0,   104,   105,   521,     0,   107,     0,
     108,     0,     0,     0,   111,   112,     0,     0,   113,     0,
     114,   115,     0,   117,   118,   119,     0,   121,   122,   123,
     124,     0,     0,   126,   127,   128,   129,   130,   522,   523,
       0,   524,     0,   134,     0,     0,   137,     0,   139,   140,
     141,   142,     0,     0,   144,     0,     0,   146,   147,   148,
       0,     0,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   525,     0,   526,   164,   165,   166,   167,   168,
     169,     0,   171,   172,     0,     0,   527,   528,   177,     0,
       0,   178,   179,     0,     0,   529,   181,     0,     0,   182,
       0,     0,   530,   186,   187,   188,   189,   190,     0,   531,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   532,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   533,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,     0,   224,   225,   226,   227,     0,
     229,   534,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   535,   240,     0,   241,   242,   243,     0,   536,   537,
       0,   246,     0,   247,   538,   249,   539,     0,   252,   253,
     254,   255,     0,     0,   257,   258,   259,   260,   261,     0,
       0,   263,   264,   265,   266,   540,   268,     0,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     541,     0,    25,    26,    27,    28,    29,    30,    31,     0,
      32,     0,     0,     0,     0,     0,     0,     0,    34,    35,
      36,  1013,    38,    39,    40,     0,   509,     0,   510,   511,
       0,    46,    47,    48,    49,     0,     0,    52,   512,   513,
      55,     0,    56,    57,    58,    59,   514,     0,     0,    61,
      62,    63,     0,    64,    65,   515,    67,     0,    68,    69,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     516,   517,    82,     0,    83,     0,    84,    85,    86,    87,
      88,     0,     0,     0,    89,   518,    91,    92,     0,    93,
      94,     0,    95,     0,    96,    97,    98,   519,   100,   101,
     520,     0,   104,   105,   521,     0,   107,     0,   108,     0,
       0,     0,   111,   112,     0,     0,   113,     0,   114,   115,
       0,   117,   118,   119,     0,   121,   122,   123,   124,     0,
       0,   126,   127,   128,   129,   130,   522,   523,     0,   524,
       0,   134,     0,     0,   137,     0,   139,   140,   141,   142,
       0,     0,   144,     0,     0,   146,   147,   148,     0,     0,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     525,     0,   526,     0,   165,   166,   167,   168,   169,     0,
     171,   172,     0,     0,   527,   528,   177,     0,     0,   178,
       0,     0,     0,   529,   181,     0,     0,   182,     0,     0,
     530,   186,   187,   188,   189,   190,     0,   531,   192,   193,
     194,     0,   195,   196,   197,   198,   199,   532,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
     212,   533,   214,   215,   216,   217,   218,   219,     0,   220,
     221,   222,     0,   224,   225,   226,   227,     0,   229,   534,
       0,   231,   232,   233,   234,   235,   236,   237,   238,   535,
     240,     0,   241,   242,   243,     0,   536,   537,     0,   246,
       0,   247,   538,   249,   539,     0,   252,   253,   254,   255,
       0,     0,   257,   258,   259,   260,   261,     0,     0,   263,
     264,   265,   266,   540,   268,     0,   270,   271,   272,     0,
       0,   273,   274,   275,   276,   277,   278,     0,   541,     0,
      25,    26,    27,    28,    29,    30,    31,     0,    32,     0,
       0,     0,     0,     0,     0,     0,    34,    35,    36,  1089,
      38,    39,    40,     0,   509,     0,   510,   511,     0,    46,
      47,    48,    49,     0,     0,    52,   512,   513,    55,     0,
      56,    57,    58,    59,   514,     0,     0,    61,    62,    63,
       0,    64,    65,   515,    67,     0,    68,    69,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   516,   517,
      82,     0,    83,     0,    84,    85,    86,    87,    88,     0,
       0,     0,    89,   518,    91,    92,     0,    93,    94,     0,
      95,     0,    96,    97,    98,   519,   100,   101,   520,     0,
     104,   105,   521,     0,   107,     0,   108,     0,     0,     0,
     111,   112,     0,     0,   113,     0,   114,   115,     0,   117,
     118,   119,     0,   121,   122,   123,   124,     0,     0,   126,
     127,   128,   129,   130,   522,   523,     0,   524,     0,   134,
       0,     0,   137,     0,   139,   140,   141,   142,     0,     0,
     144,     0,     0,   146,   147,   148,     0,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   525,     0,
     526,     0,   165,   166,   167,   168,   169,     0,   171,   172,
       0,     0,   527,   528,   177,   817,     0,   178,     0,     0,
       0,   529,   181,     0,     0,   182,     0,     0,   530,   186,
     187,   188,   189,   190,     0,   531,   192,   193,   194,     0,
     195,   196,   197,   198,   199,   532,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   533,
     214,   215,   216,   217,   218,   219,   818,   220,   221,   222,
       0,   224,   225,   226,   227,     0,   229,   534,     0,   231,
     232,   233,   234,   235,   236,   237,   238,   535,   240,     0,
     241,   242,   243,     0,   536,   537,   423,   246,     0,   247,
     538,   249,   539,   425,   252,   253,   254,   255,   426,     0,
     257,   258,   259,   260,   261,     0,   423,   263,   264,   265,
     266,   540,   268,   425,   270,   271,   272,     0,   426,   273,
     274,   275,   276,   277,   278,     0,   541,     0,     0,     0,
       0,   409,     0,     0,   410,   411,   412,  -603,   413,   414,
     415,   416,   417,   418,     0,     0,     0,     0,     0,  -603,
       0,     0,     0,     0,  -603,     0,     0,     0,     0,  -603,
       0,  -603,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   284,     0,     0,     0,
    -606,  -606,  -603,   822,   823,   824,   825,   826,   827,     0,
       0,     0,     0,   427,   828,     0,     0,   428,  -603,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,   429,   430,   428,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,  -603,     0,     0,   429,   430,     0,     0,     0,
    -603,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   432,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,   435,     0,
       0,     0,   432,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   898,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,  -603,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,  -603,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -603,     0,  -603,  -603,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,     0,
       0,   972,     0,     0,   447,     0,   437,     0,   973,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,     0,  -357,    25,    26,
      27,    28,    29,    30,    31,     0,    32,     0,     0,     0,
       0,     0,     0,     0,    34,    35,    36,     0,    38,    39,
      40,     0,   509,     0,   510,   511,     0,    46,    47,    48,
      49,     0,     0,    52,   512,   513,    55,     0,    56,    57,
      58,    59,   514,     0,     0,    61,    62,    63,     0,    64,
      65,   515,    67,     0,    68,    69,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   516,   517,    82,     0,
      83,     0,    84,    85,    86,    87,    88,     0,     0,     0,
      89,   518,    91,    92,     0,    93,    94,     0,    95,     0,
      96,    97,    98,   519,   100,   101,   520,     0,   104,   105,
     521,     0,   107,     0,   108,     0,     0,     0,   111,   112,
       0,     0,   113,     0,   114,   115,     0,   117,   118,   119,
       0,   121,   122,   123,   124,     0,     0,   126,   127,   128,
     129,   130,   522,   523,     0,   524,     0,   134,     0,     0,
     137,     0,   139,   140,   141,   142,     0,     0,   144,     0,
       0,   146,   147,   148,     0,     0,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   525,     0,   526,     0,
     165,   166,   167,   168,   169,     0,   171,   172,     0,     0,
     527,   528,   177,     0,     0,   178,     0,     0,     0,   529,
     181,     0,     0,   182,     0,     0,   530,   186,   187,   188,
     189,   190,     0,   531,   192,   193,   194,     0,   195,   196,
     197,   198,   199,   532,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   533,   214,   215,
     216,   217,   218,   219,     0,   220,   221,   222,     0,   224,
     225,   226,   227,     0,   229,   534,     0,   231,   232,   233,
     234,   235,   236,   237,   238,   535,   240,     0,   241,   242,
     243,     0,   536,   537,     0,   246,     0,   247,   538,   249,
     539,     0,   252,   253,   254,   255,     0,     0,   257,   258,
     259,   260,   261,     0,     0,   263,   264,   265,   266,   540,
     268,     0,   270,   271,   272,     0,     0,   273,   274,   275,
     276,   277,   278,   423,   541,     0,     0,   770,     0,     0,
     425,     0,     0,     0,     0,   426,     0,     0,   575,     0,
     423,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,   426,     0,     0,     0,     0,   423,     0,     0,
     424,     0,     0,     0,   425,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,   771,     0,     0,   425,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   789,     0,     0,     0,   425,     0,
     427,     0,   833,   426,   428,     0,     0,   834,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,   428,   429,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   427,     0,     0,     0,   428,   429,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,   429,   430,     0,   432,
       0,     0,   433,     0,   427,     0,     0,   431,   428,     0,
       0,     0,     0,   434,     0,   435,   432,     0,     0,   433,
       0,     0,     0,     0,     0,     0,   429,   430,     0,     0,
     434,     0,   435,   432,     0,     0,   433,   431,   427,     0,
       0,     0,   428,     0,     0,     0,     0,   434,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   430,     0,   432,     0,     0,   433,   436,     0,     0,
       0,   431,     0,     0,     0,     0,     0,   434,     0,   435,
       0,   423,     0,     0,   436,     0,     0,     0,   425,     0,
       0,     0,     0,   426,     0,     0,     0,   432,     0,     0,
     433,   436,     0,     0,     0,     0,     0,     0,   772,     0,
       0,   434,     0,   435,     0,   423,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,   426,     0,     0,
       0,   436,     0,   437,     0,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,     0,     0,     0,     0,
     437,   447,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,     0,     0,   436,     0,   437,   447,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
       0,     0,   423,     0,     0,   447,     0,     0,   427,   425,
       0,     0,   428,     0,   426,     0,     0,   437,     0,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     429,   430,     0,     0,   581,   447,     0,     0,     0,     0,
       0,   431,   427,     0,     0,     0,   428,     0,     0,     0,
       0,   437,     0,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   429,   430,     0,   432,     0,   447,
     433,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,   434,     0,   435,     0,     0,     0,     0,   423,     0,
       0,   842,     0,     0,     0,   425,     0,     0,     0,     0,
     426,   432,     0,     0,   433,     0,     0,     0,     0,   427,
       0,     0,     0,   428,     0,   434,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   807,     0,
       0,   429,   430,     0,     0,   436,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,     0,     0,     0,     0,     0,     0,   425,
       0,     0,     0,     0,   426,     0,     0,     0,   432,   436,
       0,   433,     0,     0,     0,     0,   791,     0,     0,     0,
       0,     0,   434,     0,   435,     0,   423,     0,     0,     0,
       0,     0,     0,   425,     0,   427,     0,     0,   426,   428,
       0,   437,     0,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,     0,     0,     0,   429,   430,   447,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   437,   436,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,     0,   847,
       0,     0,     0,   447,   432,     0,     0,   433,     0,   427,
       0,     0,     0,   428,     0,     0,     0,     0,   434,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   427,     0,     0,     0,   428,     0,     0,
       0,     0,   437,     0,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   429,   430,     0,   432,   832,
     447,   433,   436,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   434,     0,   435,     0,   423,     0,     0,     0,
       0,     0,     0,   425,     0,     0,     0,     0,   426,     0,
       0,     0,   432,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,   435,     0,
     423,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,   426,     0,     0,     0,   436,     0,   437,     0,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
     436,     0,   425,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   931,     0,
       0,     0,   423,   427,     0,     0,     0,   428,     0,   425,
       0,     0,   437,     0,   426,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   429,   430,     0,     0,     0,
     447,     0,     0,     0,     0,     0,   431,   427,     0,     0,
       0,   428,     0,     0,     0,     0,   437,     0,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   429,
     430,     0,   432,     0,   447,   433,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,   434,     0,   435,     0,
       0,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,   423,     0,     0,     0,     0,   432,     0,   425,   433,
       0,     0,     0,   426,   429,   430,     0,     0,     0,   427,
     434,     0,   435,   428,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   429,   430,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   431,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,   435,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,   432,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,   427,   438,
     439,   440,   428,   441,   442,   443,   444,   445,   446,   436,
       0,     0,     0,  1047,   447,     0,     0,   425,     0,     0,
     429,   430,   426,     0,     0,     0,     0,     0,     0,     0,
     437,   431,     0,   438,   439,   440,   436,   441,   442,   443,
     444,   445,   446,     0,   425,     0,     0,  1069,   447,   426,
       0,     0,     0,     0,     0,     0,     0,   432,     0,   425,
     433,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,   437,     0,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,     0,   425,
    1101,     0,     0,   447,   426,     0,     0,     0,     0,     0,
       0,     0,   437,     0,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,     0,     0,   427,     0,     0,
     447,   428,     0,     0,     0,   436,     0,     0,     0,     0,
       0,   868,     0,     0,     0,     0,     0,     0,     0,   429,
     430,     0,     0,     0,   427,     0,     0,     0,   428,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,  -606,
       0,   877,     0,   428,     0,     0,   429,   430,     0,     0,
     425,     0,     0,     0,     0,   426,   432,   431,     0,   433,
       0,   429,   430,     0,     0,     0,     0,     0,     0,  -606,
     434,   437,  -606,   428,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,     0,     0,   433,     0,     0,   447,
       0,   429,   430,     0,     0,     0,     0,   434,     0,     0,
       0,   433,  -606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,   425,     0,     0,     0,     0,   426,
       0,     0,   996,     0,   436,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
    -606,   436,     0,     0,   428,     0,     0,     0,  -605,   425,
       0,     0,     0,     0,   426,     0,  -606,     0,     0,     0,
    -605,     0,   429,   430,     0,  -605,     0,     0,     0,     0,
    -605,     0,  -605,  -606,   425,     0,   997,     0,     0,   426,
     437,     0,     0,   438,   439,   440,  -606,   441,   442,   443,
     444,   445,   446,  -605,     0,     0,     0,     0,   447,     0,
       0,     0,   433,     0,  -606,     0,     0,   437,   428,  -605,
       0,  -606,  -606,   434,   441,   442,   443,   444,   445,   446,
       0,  1000,   437,     0,     0,   447,   429,   430,     0,   441,
     442,   443,   444,   445,   446,     0,     0,  -606,     0,     0,
     447,     0,     0,  -605,     0,     0,  1062,     0,     0,  -606,
       0,  -605,   437,   428,     0,     0,     0,     0,     0,   441,
     442,   443,   444,   445,   446,     0,   433,  -606,     0,     0,
     447,   429,   430,     0,  -606,     0,     0,   434,   428,     0,
       0,     0,  -606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -606,     0,     0,
       0,   433,     0,     0,     0,     0,   898,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,  -606,     0,   437,     0,  -605,   433,     0,     0,     0,
     441,   442,   443,   444,   445,   446,     0,   434,     0,     0,
       0,   447,     0,     0,     0,  -605,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -606,     0,     0,     0,
       0,     0,     0,     0,     0,  -605,     0,  -605,  -605,     0,
       0,     0,     0,     0,     0,     0,     0,   437,     0,     0,
       0,  -606,     0,     0,   441,   442,   443,   444,   445,   446,
       0,     0,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,   441,
     442,   443,   444,   445,   446,     0,     0,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,   437,     0,     0,
       0,     0,     0,     0,   441,   442,   443,   444,   445,   446,
       0,     0,     0,     0,     0,   447,    25,    26,    27,    28,
      29,    30,    31,   610,    32,   611,   612,   613,   614,   615,
     616,   617,    34,    35,    36,    37,    38,    39,    40,    41,
     509,    43,   510,   511,   618,    46,    47,    48,    49,   619,
     620,    52,   512,   513,    55,   621,    56,    57,    58,    59,
     514,   622,   623,    61,    62,    63,   624,    64,    65,   515,
      67,   625,    68,    69,    70,   626,   627,   628,   629,    75,
      76,    77,    78,    79,   516,   517,    82,   630,    83,   631,
      84,    85,    86,    87,    88,   632,   633,   634,    89,   518,
      91,    92,   635,    93,    94,   636,    95,   637,    96,    97,
      98,   519,   100,   101,   520,   638,   104,   105,   521,   639,
     107,   640,   108,   109,   641,   110,   111,   112,   642,   643,
     113,   644,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   645,   125,   126,   127,   128,   129,   130,
     522,   523,   646,   524,   647,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   648,   143,   144,   145,   649,   146,
     147,   148,   650,   651,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   525,   162,   526,   652,   165,   166,
     167,   168,   169,   653,   171,   172,   173,   654,   527,   528,
     177,   655,   656,   178,   657,   658,   659,   529,   181,   660,
     661,   182,   183,   184,   530,   186,   187,   188,   189,   190,
     662,   531,   192,   193,   194,   663,   195,   196,   197,   198,
     199,   532,   201,   664,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   533,   214,   215,   216,   217,
     218,   219,   665,   220,   221,   222,   666,   224,   225,   226,
     227,   228,   229,   534,   667,   231,   232,   233,   234,   235,
     236,   237,   238,   535,   240,   668,   241,   242,   243,   669,
     536,   537,   670,   246,   671,   247,   538,   249,   539,   672,
     252,   253,   254,   255,   673,   674,   257,   258,   259,   260,
     261,   675,   676,   263,   264,   265,   266,   540,   268,   269,
     270,   271,   272,   677,   678,   273,   274,   275,   276,   277,
     278,     0,   679,    25,    26,    27,    28,    29,    30,    31,
       0,    32,     0,     0,     0,     0,     0,     0,     0,    34,
      35,    36,     0,    38,    39,    40,     0,   509,     0,   510,
     511,     0,    46,    47,    48,    49,     0,     0,    52,   512,
     513,    55,     0,    56,    57,    58,    59,   514,     0,     0,
      61,    62,    63,     0,    64,    65,   515,    67,     0,    68,
      69,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   516,   517,    82,     0,    83,     0,    84,    85,    86,
      87,    88,     0,     0,     0,    89,   518,    91,    92,     0,
      93,    94,     0,    95,     0,    96,    97,    98,   519,   100,
     101,   520,     0,   104,   105,   521,     0,   107,     0,   108,
       0,     0,     0,   111,   752,     0,     0,   113,     0,   114,
     115,     0,   117,   118,   119,     0,   121,   122,   123,   124,
       0,     0,   126,   127,   128,   129,   130,   522,   523,     0,
     524,     0,   134,     0,     0,   137,     0,   139,   140,   141,
     142,     0,     0,   144,     0,     0,   146,   147,   753,     0,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   525,     0,   526,   164,   165,   166,   167,   168,   169,
       0,   171,   172,     0,     0,   527,   528,   177,     0,     0,
     178,   179,     0,     0,   529,   181,     0,     0,   182,     0,
       0,   530,   186,   187,   188,   189,   190,     0,   531,   192,
     193,   194,     0,   195,   196,   197,   198,   199,   532,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,   212,   533,   214,   215,   216,   217,   218,   219,     0,
     220,   221,   222,     0,   224,   225,   226,   227,     0,   229,
     534,     0,   231,   232,   233,   234,   235,   236,   237,   238,
     535,   240,   754,   755,   242,   756,     0,   536,   537,     0,
     246,     0,   247,   538,   249,   539,     0,   252,   253,   254,
     255,     0,     0,   257,   258,   259,   260,   261,     0,     0,
     263,   264,   265,   266,   540,   268,     0,   270,   271,   272,
       0,     0,   273,   274,   275,   276,   277,   278,     0,   541,
      25,    26,    27,    28,    29,    30,    31,     0,    32,     0,
       0,     0,     0,     0,     0,     0,    34,    35,    36,     0,
      38,    39,    40,     0,   509,     0,   510,   511,     0,    46,
      47,    48,    49,     0,     0,    52,   512,   513,    55,     0,
      56,    57,    58,    59,   514,     0,     0,    61,    62,    63,
       0,    64,    65,   515,    67,     0,    68,    69,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   516,   517,
      82,     0,    83,     0,    84,    85,    86,    87,    88,     0,
       0,     0,    89,   518,    91,    92,     0,    93,    94,     0,
      95,     0,    96,    97,    98,   519,   100,   101,   520,     0,
     104,   105,   521,     0,   107,     0,   108,     0,     0,     0,
     111,   112,     0,     0,   113,     0,   114,   115,     0,   117,
     118,   119,     0,   121,   122,   123,   124,     0,     0,   126,
     127,   128,   129,   130,   522,   523,     0,   524,     0,   134,
       0,     0,   137,     0,   139,   140,   141,   142,     0,     0,
     144,     0,     0,   146,   147,   148,     0,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   525,     0,
     526,   164,   165,   166,   167,   168,   169,     0,   171,   172,
       0,     0,   527,   528,   177,     0,     0,   178,   179,     0,
       0,   529,   181,     0,     0,   182,     0,     0,   530,   186,
     187,   188,   189,   190,     0,   531,   192,   193,   194,     0,
     195,   196,   197,   198,   199,   532,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   533,
     214,   215,   216,   217,   218,   219,     0,   220,   221,   222,
       0,   224,   225,   226,   227,     0,   229,   534,     0,   231,
     232,   233,   234,   235,   236,   237,   238,   535,   240,     0,
     241,   242,   243,     0,   536,   537,     0,   246,     0,   247,
     538,   249,   539,     0,   252,   253,   254,   255,     0,     0,
     257,   258,   259,   260,   261,     0,     0,   263,   264,   265,
     266,   540,   268,     0,   270,   271,   272,     0,     0,   273,
     274,   275,   276,   277,   278,     0,   541,    25,    26,    27,
      28,    29,    30,    31,     0,    32,     0,     0,     0,     0,
       0,  1018,     0,    34,    35,    36,     0,    38,    39,    40,
       0,   509,     0,   510,   511,     0,    46,    47,    48,    49,
       0,     0,    52,   512,   513,    55,     0,    56,    57,    58,
      59,   514,     0,     0,    61,    62,    63,     0,    64,    65,
     515,    67,     0,    68,    69,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   516,   517,    82,     0,    83,
       0,    84,    85,    86,    87,    88,     0,     0,     0,    89,
     518,    91,    92,     0,    93,    94,     0,    95,     0,    96,
      97,    98,   519,   100,   101,   520,     0,   104,   105,   521,
       0,   107,     0,   108,     0,     0,     0,   111,   112,     0,
       0,   113,     0,   114,   115,     0,   117,   118,   119,     0,
     121,   122,   123,   124,     0,     0,   126,   127,   128,   129,
     130,   522,   523,     0,   524,     0,   134,     0,     0,   137,
       0,   139,   140,   141,   142,     0,     0,   144,     0,     0,
     146,   147,   148,     0,     0,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   525,     0,   526,     0,   165,
     166,   167,   168,   169,     0,   171,   172,     0,     0,   527,
     528,   177,     0,     0,   178,     0,     0,     0,   529,   181,
       0,     0,   182,     0,     0,   530,   186,   187,   188,   189,
     190,     0,   531,   192,   193,   194,     0,   195,   196,   197,
     198,   199,   532,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,   212,   533,   214,   215,   216,
     217,   218,   219,     0,   220,   221,   222,     0,   224,   225,
     226,   227,     0,   229,   534,     0,   231,   232,   233,   234,
     235,   236,   237,   238,   535,   240,     0,   241,   242,   243,
       0,   536,   537,     0,   246,     0,   247,   538,   249,   539,
       0,   252,   253,   254,   255,     0,     0,   257,   258,   259,
     260,   261,     0,     0,   263,   264,   265,   266,   540,   268,
       0,   270,   271,   272,     0,     0,   273,   274,   275,   276,
     277,   278,     0,   541,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,     0,     0,  1033,     0,
      34,    35,    36,     0,    38,    39,    40,     0,   509,     0,
     510,   511,     0,    46,    47,    48,    49,     0,     0,    52,
     512,   513,    55,     0,    56,    57,    58,    59,   514,     0,
       0,    61,    62,    63,     0,    64,    65,   515,    67,     0,
      68,    69,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   516,   517,    82,     0,    83,     0,    84,    85,
      86,    87,    88,     0,     0,     0,    89,   518,    91,    92,
       0,    93,    94,     0,    95,     0,    96,    97,    98,   519,
     100,   101,   520,     0,   104,   105,   521,     0,   107,     0,
     108,     0,     0,     0,   111,   112,     0,     0,   113,     0,
     114,   115,     0,   117,   118,   119,     0,   121,   122,   123,
     124,     0,     0,   126,   127,   128,   129,   130,   522,   523,
       0,   524,     0,   134,     0,     0,   137,     0,   139,   140,
     141,   142,     0,     0,   144,     0,     0,   146,   147,   148,
       0,     0,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   525,     0,   526,     0,   165,   166,   167,   168,
     169,     0,   171,   172,     0,     0,   527,   528,   177,     0,
       0,   178,     0,     0,     0,   529,   181,     0,     0,   182,
       0,     0,   530,   186,   187,   188,   189,   190,     0,   531,
     192,   193,   194,     0,   195,   196,   197,   198,   199,   532,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   533,   214,   215,   216,   217,   218,   219,
       0,   220,   221,   222,     0,   224,   225,   226,   227,     0,
     229,   534,     0,   231,   232,   233,   234,   235,   236,   237,
     238,   535,   240,     0,   241,   242,   243,     0,   536,   537,
       0,   246,     0,   247,   538,   249,   539,     0,   252,   253,
     254,   255,     0,     0,   257,   258,   259,   260,   261,     0,
       0,   263,   264,   265,   266,   540,   268,     0,   270,   271,
     272,     0,     0,   273,   274,   275,   276,   277,   278,     0,
     541,    25,    26,    27,    28,    29,    30,    31,     0,    32,
       0,     0,     0,     0,     0,     0,     0,    34,    35,    36,
       0,    38,    39,    40,     0,   509,     0,   510,   511,     0,
      46,    47,    48,    49,     0,     0,    52,   512,   513,    55,
       0,    56,    57,    58,    59,   514,     0,     0,    61,    62,
      63,     0,    64,    65,   515,    67,     0,    68,    69,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   516,
     517,    82,     0,    83,     0,    84,    85,    86,    87,    88,
       0,     0,     0,    89,   518,    91,    92,     0,    93,    94,
       0,    95,     0,    96,    97,    98,   519,   100,   101,   520,
       0,   104,   105,   521,     0,   107,     0,   108,     0,     0,
       0,   111,   112,     0,     0,   113,     0,   114,   115,     0,
     117,   118,   119,     0,   121,   122,   123,   124,     0,     0,
     126,   127,   128,   129,   130,   522,   523,     0,   524,     0,
     134,     0,     0,   137,     0,   139,   140,   141,   142,     0,
       0,   144,     0,     0,   146,   147,   148,     0,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   525,
       0,   526,     0,   165,   166,   167,   168,   169,     0,   171,
     172,     0,     0,   527,   528,   177,     0,     0,   178,     0,
       0,     0,   529,   181,     0,     0,   182,     0,     0,   530,
     186,   187,   188,   189,   190,     0,   531,   192,   193,   194,
       0,   195,   196,   197,   198,   199,   532,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,   212,
     533,   214,   215,   216,   217,   218,   219,     0,   220,   221,
     222,     0,   224,   225,   226,   227,     0,   229,   534,     0,
     231,   232,   233,   234,   235,   236,   237,   238,   535,   240,
       0,   241,   242,   243,     0,   536,   537,     0,   246,     0,
     247,   538,   249,   539,     0,   252,   253,   254,   255,     0,
       0,   257,   258,   259,   260,   261,     0,     0,   263,   264,
     265,   266,   540,   268,     0,   270,   271,   272,     0,     0,
     273,   274,   275,   276,   277,   278,     0,   541,    25,    26,
      27,    28,    29,    30,    31,     0,    32,     0,     0,     0,
       0,     0,     0,     0,    34,    35,    36,     0,    38,    39,
      40,     0,   716,     0,   717,   718,     0,    46,    47,    48,
      49,     0,     0,    52,   719,   720,    55,     0,    56,    57,
      58,    59,     0,     0,     0,    61,    62,    63,     0,    64,
      65,     0,    67,     0,    68,    69,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   721,   722,    82,     0,
      83,     0,    84,    85,    86,    87,    88,     0,     0,     0,
      89,    90,    91,    92,     0,    93,    94,     0,    95,     0,
      96,    97,    98,     0,   100,   101,     0,     0,   104,   105,
     723,     0,   107,     0,   108,     0,     0,     0,   111,   112,
       0,     0,   113,     0,   114,   115,     0,   117,   118,   119,
       0,   121,   122,   123,   124,     0,     0,   126,   127,   128,
     129,   130,   724,   725,     0,   726,     0,   134,     0,     0,
     137,     0,   139,   140,   141,   142,     0,     0,   144,     0,
       0,   146,   147,   148,     0,     0,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,     0,   727,     0,
     165,   166,   167,   168,     0,     0,   171,   172,     0,     0,
       0,   728,   177,     0,     0,   178,     0,     0,     0,   529,
     181,     0,     0,   182,     0,     0,     0,   186,   187,   188,
     189,   190,     0,     0,   192,   193,   194,     0,   195,   196,
     197,   198,   199,   729,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,     0,   214,   215,
     216,   217,   218,   219,     0,   220,   221,   222,     0,   224,
     730,   226,   227,     0,   229,   731,     0,   231,   232,   233,
     234,   235,   236,   237,   238,     0,   240,     0,   241,   242,
     243,     0,   732,   733,     0,   246,     0,   247,     0,   249,
       0,     0,   252,   253,   254,   255,     0,     0,   257,   258,
     259,   260,   261,     0,     0,   263,   264,   265,   266,   734,
     268,     0,   270,   271,   272,     0,     0,   273,   274,   275,
     276,   277,   278,     0,   735,    25,    26,    27,    28,    29,
      30,    31,     0,    32,     0,     0,     0,     0,     0,     0,
       0,    34,    35,    36,     0,    38,    39,    40,     0,   716,
       0,   717,   718,     0,    46,    47,    48,    49,     0,     0,
      52,   719,   720,    55,     0,    56,    57,    58,    59,     0,
       0,     0,    61,    62,    63,     0,    64,    65,     0,    67,
       0,    68,    69,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   721,   722,    82,     0,    83,     0,    84,
      85,    86,    87,    88,     0,     0,     0,    89,    90,    91,
      92,     0,    93,    94,     0,    95,     0,    96,    97,    98,
       0,   100,   101,     0,     0,   104,   105,   723,     0,   107,
       0,   108,     0,     0,     0,   111,   112,     0,     0,   113,
       0,   114,   115,     0,   117,   118,   119,     0,   121,   122,
     123,   124,     0,     0,   126,   127,   128,   129,   130,   724,
     725,     0,   726,     0,   134,     0,     0,   137,     0,   139,
     140,   141,   142,     0,     0,   144,     0,     0,   146,   147,
     148,     0,     0,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   727,     0,   165,   166,   167,
     168,     0,     0,   171,   172,     0,     0,     0,   728,   177,
       0,     0,   178,     0,     0,     0,   529,   181,     0,     0,
     182,     0,     0,     0,   186,   187,   188,   189,   190,     0,
       0,   192,   193,   194,     0,   195,   196,   197,   198,   199,
     729,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,     0,   212,     0,   214,   215,   216,   217,   218,
     219,     0,   220,   221,   222,     0,   224,     0,   226,   227,
       0,   229,   731,     0,   231,   232,   233,   234,   235,   236,
     237,   238,     0,   240,     0,   241,   242,   243,     0,   732,
     733,     0,   246,     0,   247,     0,   249,     0,     0,   252,
     253,   254,   255,     0,     0,   257,   258,   259,   260,   261,
       0,     0,   263,   264,   265,   266,   734,   268,     0,   270,
     271,   272,     0,     0,   273,   274,   275,   276,   277,   278,
       0,   735
};

static const yytype_int16 yycheck[] =
{
       0,   307,     2,     3,    12,   342,   447,    81,   286,   294,
       0,   245,    21,   747,   408,   354,   355,   356,   341,   342,
     305,    40,   447,   362,   232,   364,   365,   421,   373,   352,
     353,   447,   371,    33,   774,    10,    69,    53,    54,    61,
      15,   380,    50,   382,   447,   313,   375,    21,   316,    61,
      10,   756,    89,    10,   377,    15,    94,   189,    15,    26,
      10,   400,   385,   402,   141,    15,   120,    69,   106,   835,
     598,    40,   451,   239,   447,   909,   910,   991,    83,    10,
      83,  1091,    40,    10,   217,   334,   143,   120,    21,   112,
     112,   295,   762,    26,   334,   106,   165,   426,    94,    99,
     112,   139,   155,   302,   303,   192,    69,   311,   131,   131,
     329,   165,   311,  1123,   373,   331,   335,   155,   120,   131,
     808,   389,   330,   146,   146,   315,   330,   311,   387,   152,
     152,   330,   165,   456,   146,   168,   189,   331,  1052,   329,
     152,   102,   481,   139,   155,   331,   330,   163,   836,   335,
     172,   189,    83,   286,   121,   311,    83,   120,   125,   197,
     172,    32,   170,   165,   330,   311,   168,   311,   237,   330,
     447,   302,   303,    21,   330,   180,   704,   180,   189,   945,
     192,   184,   187,   237,   330,  1099,   330,   154,   893,   894,
     895,   896,   125,   311,   329,   311,   311,   447,   311,   330,
     335,   197,   165,   335,   237,   168,   447,   230,   230,   311,
     143,   144,   302,   303,   330,   330,   330,   330,   230,   548,
     549,   550,   551,   184,   553,    21,   187,   302,   303,   323,
     324,   325,   326,   903,   331,   237,   311,   334,   332,   330,
     330,  1075,   976,   281,   330,   568,   569,   570,   571,   239,
     183,    39,    40,   598,   326,   330,   256,   232,   335,     0,
     332,   194,     3,   300,   321,   322,   323,   324,   325,   326,
     328,   330,   330,   306,   237,   332,   251,   289,   806,   287,
     288,   248,   290,   291,   292,   281,   308,   332,   333,   330,
     335,   251,   292,   331,   251,   331,   308,   330,   576,   335,
     308,   251,   331,   581,   306,   730,   335,   309,   263,   311,
     265,   330,   209,   581,   730,   276,   331,   326,   302,   303,
     335,   331,   283,   332,   583,   335,   334,   730,   311,   337,
     338,   330,   332,   333,  1068,   335,    19,   295,    21,   598,
     330,   349,   311,   306,   330,   353,   331,   143,   789,   334,
     366,   367,  1022,   311,   330,   330,  1026,   730,   332,   704,
    1030,   330,   370,   447,   789,   688,   374,   311,   376,   377,
     699,  1111,   330,   789,   330,   754,   384,   385,   331,   311,
     331,   314,   335,  1123,   335,   330,   789,   828,   321,   322,
     323,   324,   325,   326,   331,  1135,   331,   311,   335,   332,
     335,   842,   331,   828,   404,   330,   335,   331,   330,  1079,
      83,   335,   828,   330,  1084,   423,   789,   842,  1088,   427,
     331,   421,   330,   431,   335,   828,   842,   435,   428,   102,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   842,
     448,   764,   765,   331,   452,   704,   783,   335,  1118,   333,
     331,   876,   331,   730,   335,   828,   335,   796,   331,   331,
     876,   806,   335,   335,   331,   804,   772,   861,   335,   842,
     748,   776,   777,   876,   482,   323,   324,   325,   326,   143,
     730,   331,   331,   330,   332,   335,   335,   816,   330,   730,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   733,
     829,   332,   333,   876,   335,   330,   568,   180,   570,   571,
     311,   184,   789,   330,   187,   774,   263,  1027,   265,  1029,
     899,   900,   901,   330,   847,   321,   322,   323,   324,   325,
     326,   106,   791,   330,   300,   315,   332,   866,   867,   789,
     295,   267,   267,   328,   552,   332,   335,   806,   789,   888,
     331,   828,   552,   244,   447,   125,   309,   330,   267,   330,
     568,   330,   570,   571,   269,   842,   991,   111,   139,   193,
     578,   856,   335,   187,   582,   991,   155,   331,   828,   331,
     331,   111,   331,   331,   852,   331,   331,   828,   991,   333,
     307,   331,   842,   331,   347,   331,   335,   597,   331,   876,
     307,   842,   331,   276,   331,   605,   840,   841,   608,   331,
     283,   331,   331,   269,   269,   269,   369,  1058,   991,   269,
     111,   184,   375,   307,   111,    21,   876,  1052,  1007,  1008,
    1009,  1010,   330,  1058,  1013,   876,  1052,   390,   391,   392,
     393,   269,  1058,   396,   330,   904,   730,    16,   722,  1052,
     909,   910,   330,   330,   330,  1058,   301,   321,   322,   323,
     324,   325,   326,   331,  1003,  1004,   995,   328,   332,   422,
      92,   331,   111,   426,  1099,   332,   106,    40,   331,  1052,
     331,  1122,   331,  1099,  1125,  1058,   237,   168,   688,   331,
     330,    16,   700,   451,   702,   331,  1099,  1122,   706,   328,
    1125,   701,   328,   719,   720,   789,  1122,   262,   330,  1125,
     463,   727,   323,  1036,   991,   335,   469,   330,   471,  1122,
     330,  1050,  1125,   476,   116,  1064,  1099,   335,   331,   331,
     331,   111,   485,   333,   487,   331,   903,   187,   330,   328,
     331,   991,   146,   331,   828,   331,   330,   143,   144,  1122,
     991,   751,  1125,   199,   507,   146,  1015,   330,   842,  1018,
      32,   769,   762,   118,   330,   329,   331,   329,   776,   777,
     778,  1094,   780,   330,  1033,  1052,   146,   330,   786,   329,
       3,  1058,   330,  1042,   340,   778,   903,   183,   769,  1028,
     798,  1119,   876,   546,   802,   548,   549,   550,   551,   807,
     553,   554,  1052,   339,  1139,   885,   559,   560,  1058,   125,
     730,  1052,   855,   483,   567,   376,  1075,  1058,  1077,   572,
     452,   701,  1099,    -1,   449,   833,   834,   143,  1042,   424,
    1089,   424,  1091,    -1,    -1,    -1,    -1,   730,    -1,    -1,
      -1,    -1,    -1,    -1,    14,  1122,    -1,    -1,  1125,  1099,
      -1,    -1,  1111,    -1,    -1,  1022,   609,    -1,  1099,  1026,
     868,    -1,   870,  1030,  1123,   373,    -1,    -1,    -1,   877,
      -1,    -1,  1122,    -1,   882,  1125,  1135,    -1,   194,   387,
    1139,  1122,    -1,   891,  1125,    -1,    -1,    -1,    -1,    -1,
      -1,   891,    -1,    -1,    -1,    -1,   789,    -1,    -1,    -1,
      -1,    -1,    -1,   903,    -1,    -1,   914,   991,    -1,    -1,
      -1,    -1,  1079,    -1,    -1,    -1,   424,  1084,    -1,   194,
      -1,  1088,    -1,   931,    -1,   321,   322,   323,   324,   325,
     326,    -1,    -1,   686,   687,   828,   332,    -1,    -1,   447,
      -1,    -1,    -1,   451,    -1,   698,   699,    -1,    -1,   842,
      -1,  1118,   705,    -1,   707,   708,   709,   710,   711,   712,
     713,   714,   715,    -1,    -1,   973,    -1,    -1,  1052,    -1,
      -1,    -1,    -1,   143,  1058,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,   996,   997,
      -1,    -1,  1000,    -1,    -1,    -1,   754,   750,   314,    -1,
      -1,   317,   318,   319,   762,   321,   322,   323,   324,   325,
     326,    -1,    -1,   426,    -1,  1099,   332,    -1,    -1,    -1,
      -1,    -1,  1022,    -1,   194,    -1,  1026,   125,    -1,    -1,
    1030,    -1,   785,   791,    -1,    -1,    -1,   451,  1122,   314,
      -1,  1125,   317,   318,   319,   143,   321,   322,   323,   324,
     325,   326,    -1,    -1,  1062,    -1,    -1,   810,   811,   812,
     813,   814,   815,   816,    -1,    -1,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   583,   829,    -1,    -1,  1079,
      -1,    -1,   180,    -1,  1084,    -1,    -1,  1095,  1088,    -1,
     598,    -1,    -1,    -1,    -1,    -1,   194,   850,   991,    -1,
     853,    -1,   200,    -1,    -1,    -1,  1114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   867,    14,    -1,    -1,  1118,    -1,
      -1,    -1,    -1,    -1,    -1,   878,   879,    -1,   881,    -1,
      -1,   884,    -1,    -1,   143,   548,   549,   550,   551,    -1,
     553,   899,   900,   901,   314,   903,    -1,   317,   318,   319,
      -1,   321,   322,   323,   324,   325,   326,    -1,    -1,  1052,
      -1,    -1,   332,    -1,    -1,  1058,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,   928,    26,    -1,    -1,   932,
      -1,    -1,    -1,    -1,    -1,   194,    -1,   940,   941,    -1,
      -1,   944,    -1,   946,    -1,    -1,   704,   950,   951,   952,
     953,   954,   955,   956,   957,   958,  1099,   960,   961,   962,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,   317,
     318,   319,   730,   321,   322,   323,   324,   325,   326,  1122,
      -1,    -1,  1125,    -1,   987,   988,    -1,   990,    -1,    -1,
      -1,   994,   995,    -1,   143,   998,   754,    -1,    -1,  1007,
    1008,  1009,  1010,  1006,   762,  1013,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,  1022,    -1,   774,    -1,  1026,    -1,
      -1,   121,  1030,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   789,    -1,   791,  1037,    -1,   699,    -1,    -1,    -1,
    1043,    -1,    -1,   143,   144,   194,    -1,  1050,   806,    -1,
      -1,  1054,    -1,    -1,   154,   314,  1059,  1060,  1061,    -1,
    1063,   194,   321,   322,   323,   324,   325,   326,    -1,    -1,
     828,  1079,    -1,   332,    -1,    -1,  1084,    -1,    -1,    -1,
    1088,    -1,    -1,   183,   842,    -1,    -1,    -1,    -1,    -1,
     754,    -1,    -1,    -1,   194,  1098,    -1,    -1,   762,    -1,
      14,  1104,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
    1118,    -1,    26,    -1,    -1,    -1,    -1,    -1,   876,    -1,
      -1,    -1,    -1,    -1,    -1,  1128,    -1,   791,    -1,    -1,
      -1,  1134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   899,   900,   901,    -1,   903,   904,    -1,   248,    -1,
      -1,   909,   910,   816,    -1,    -1,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   314,   829,    -1,   317,   318,
     319,    -1,   321,   322,   323,   324,   325,   326,    -1,    -1,
      -1,   314,    -1,   332,   317,   318,   319,    -1,   321,   322,
     323,   324,   325,   326,    -1,    -1,    -1,    -1,    -1,   332,
      -1,    -1,    -1,    -1,   867,    -1,    -1,   121,    -1,    -1,
      -1,   125,    -1,    -1,   314,    -1,    -1,   317,   318,   319,
      -1,   321,   322,   323,   324,   325,   326,    -1,    -1,   143,
     144,    -1,   332,   991,    -1,   899,   900,   901,    -1,   903,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1007,
    1008,  1009,  1010,    -1,    -1,  1013,    -1,  1015,    -1,    -1,
    1018,    -1,    -1,    -1,  1022,    -1,   180,    -1,  1026,   183,
      -1,    -1,  1030,    -1,    -1,  1033,    -1,    -1,    -1,    -1,
     194,    -1,   196,    -1,  1042,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1052,    -1,    -1,    -1,    -1,    -1,
    1058,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1075,    -1,  1077,
      -1,  1079,    -1,    -1,    -1,    -1,  1084,    -1,    -1,    -1,
    1088,  1089,   995,  1091,   248,    -1,    -1,    -1,    -1,    -1,
      -1,  1099,    -1,  1007,  1008,  1009,  1010,    -1,    -1,  1013,
      -1,    -1,    -1,  1111,    -1,    -1,    -1,    -1,  1022,    -1,
    1118,    -1,  1026,    -1,  1122,  1123,  1030,  1125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,    -1,    -1,
      -1,  1139,    -1,    -1,    -1,    -1,    -1,  1050,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,    -1,    -1,   317,   318,   319,    -1,   321,   322,   323,
     324,   325,   326,    -1,    -1,  1079,    -1,    -1,   332,    -1,
    1084,   335,    -1,    -1,  1088,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,  1118,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    83,    -1,    85,    86,
      87,    88,    -1,    90,    91,    -1,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
     107,    -1,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,    -1,    -1,   190,   191,    -1,    -1,   194,   195,    -1,
      -1,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,   208,   209,   210,   211,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,    -1,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,    -1,   263,   264,   265,    -1,
     267,   268,    -1,   270,    -1,   272,   273,   274,   275,   276,
     277,   278,   279,   280,    -1,   282,   283,   284,   285,   286,
     287,   288,    -1,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,    -1,    -1,   302,   303,   304,   305,   306,
     307,    -1,   309,   310,   311,   312,   313,   314,   315,   316,
      -1,    -1,    -1,    -1,   321,   322,   323,    -1,   325,   326,
      -1,    -1,    -1,   330,   331,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      -1,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,   107,
      -1,   109,   110,    -1,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,   140,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
      -1,    -1,   190,   191,    -1,    -1,   194,   195,    -1,    -1,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
     208,   209,   210,   211,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,    -1,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,    -1,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,    -1,   263,   264,   265,    -1,   267,
     268,    -1,   270,    -1,   272,   273,   274,   275,   276,   277,
     278,   279,   280,    -1,   282,   283,   284,   285,   286,   287,
     288,    -1,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,    -1,    -1,   302,   303,   304,   305,   306,   307,
      -1,   309,   310,   311,   312,   313,   314,   315,   316,    -1,
      -1,    -1,    -1,   321,   322,    -1,    -1,   325,   326,    -1,
      -1,    -1,   330,   331,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,   107,    -1,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,    -1,
      -1,   190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,   276,   277,   278,
     279,   280,    -1,   282,   283,   284,   285,   286,   287,   288,
      -1,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,   310,   311,   312,   313,   314,   315,   316,    -1,    -1,
      -1,    -1,   321,   322,    -1,    -1,   325,   326,    -1,    -1,
      -1,   330,   331,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    19,
      20,    21,    -1,    23,    24,    25,    -1,    27,    -1,    29,
      30,    -1,    32,    33,    34,    35,    -1,    -1,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    -1,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
      -1,    -1,    -1,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,    -1,   142,    -1,    -1,   145,    -1,   147,   148,   149,
     150,    -1,    -1,   153,    -1,   155,   156,   157,   158,    -1,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,   173,    -1,   175,   176,   177,   178,   179,
      -1,   181,   182,    -1,    -1,   185,   186,   187,    -1,   189,
     190,    -1,    -1,    -1,   194,   195,    -1,   197,   198,    -1,
      -1,   201,   202,   203,   204,   205,   206,    -1,   208,   209,
     210,   211,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,   231,   232,   233,   234,   235,   236,   237,   238,    -1,
     240,   241,   242,    -1,   244,   245,   246,   247,    -1,   249,
     250,    -1,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,    -1,   263,   264,   265,    -1,   267,   268,    -1,
     270,    -1,   272,   273,   274,   275,    -1,   277,   278,   279,
     280,   281,    -1,   283,   284,   285,   286,   287,    -1,    -1,
     290,   291,   292,   293,   294,   295,    -1,   297,   298,   299,
      -1,    -1,   302,   303,   304,   305,   306,   307,    -1,   309,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    16,
      -1,   331,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    85,    86,
      87,    88,    -1,    90,    91,    -1,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
     107,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,    -1,    -1,   190,   191,    -1,    -1,   194,   195,    -1,
      -1,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,   208,   209,   210,   211,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,    -1,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,    -1,   263,   264,   265,    -1,
     267,   268,    -1,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,    -1,   282,   283,   284,   285,   286,
     287,   288,    -1,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,    -1,    -1,   302,   303,   304,   305,   306,
     307,    -1,   309,   310,   311,   312,   313,   314,   315,   316,
      -1,    -1,    -1,    -1,   321,   322,    -1,    -1,   325,   326,
      -1,    -1,    -1,   330,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,   107,    -1,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,    -1,
      -1,   190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,   276,   277,   278,
     279,   280,    -1,   282,   283,   284,   285,   286,   287,   288,
      -1,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,   310,   311,   312,   313,   314,   315,   316,    -1,    -1,
      -1,    -1,   321,   322,   323,    -1,   325,   326,    -1,    -1,
      -1,   330,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    -1,    -1,    50,
      51,    52,    -1,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    -1,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,
      91,    -1,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,   107,    -1,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,    -1,    -1,   190,
     191,    -1,    -1,   194,   195,    -1,    -1,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,   208,   209,   210,
     211,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,    -1,   263,   264,   265,    -1,   267,   268,    -1,   270,
      -1,   272,   273,   274,   275,   276,   277,   278,   279,   280,
      -1,   282,   283,   284,   285,   286,   287,   288,    -1,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,    -1,
      -1,   302,   303,   304,   305,   306,   307,    -1,   309,   310,
     311,   312,   313,   314,   315,   316,    -1,    -1,    -1,    -1,
     321,   322,    -1,    -1,   325,   326,    -1,    -1,    -1,   330,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    85,    86,    87,    88,    -1,    90,    91,    -1,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,   107,    -1,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,    -1,    -1,   190,   191,    -1,
      -1,   194,   195,    -1,    -1,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,   208,   209,   210,   211,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,    -1,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,    -1,
     263,   264,   265,    -1,   267,   268,    -1,   270,    -1,   272,
     273,   274,   275,   276,   277,   278,   279,   280,    -1,   282,
     283,   284,   285,   286,   287,   288,    -1,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,    -1,    -1,   302,
     303,   304,   305,   306,   307,    -1,   309,   310,   311,   312,
     313,   314,   315,   316,    -1,    -1,    -1,    -1,   321,   322,
      -1,    -1,   325,   326,    -1,    -1,    -1,   330,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      85,    86,    87,    88,    -1,    90,    91,    -1,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,   107,    -1,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,    -1,    -1,   190,   191,    -1,    -1,   194,
     195,    -1,    -1,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,   208,   209,   210,   211,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,    -1,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,    -1,   263,   264,
     265,    -1,   267,   268,    -1,   270,    -1,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    -1,   282,   283,   284,
     285,   286,   287,   288,    -1,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    -1,    -1,   302,   303,   304,
     305,   306,   307,    -1,   309,   310,   311,   312,   313,   314,
     315,   316,    -1,    -1,    -1,    -1,   321,   322,    -1,    -1,
     325,   326,    -1,   328,    -1,   330,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    85,    86,
      87,    88,    -1,    90,    91,    -1,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
     107,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,    -1,    -1,   190,   191,    -1,    -1,   194,   195,    -1,
      -1,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,   208,   209,   210,   211,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,    -1,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,    -1,   263,   264,   265,    -1,
     267,   268,    -1,   270,    -1,   272,   273,   274,   275,   276,
     277,   278,   279,   280,    -1,   282,   283,   284,   285,   286,
     287,   288,    -1,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,    -1,    -1,   302,   303,   304,   305,   306,
     307,    -1,   309,   310,   311,   312,   313,   314,   315,   316,
      -1,    -1,    -1,    -1,   321,   322,    -1,    -1,   325,   326,
      -1,    -1,    -1,   330,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,   107,    -1,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,    -1,
      -1,   190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,   276,   277,   278,
     279,   280,    -1,   282,   283,   284,   285,   286,   287,   288,
      -1,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,   310,   311,   312,   313,   314,   315,   316,    -1,    -1,
      -1,    -1,   321,   322,    -1,    -1,   325,   326,    -1,    -1,
      -1,   330,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    -1,    -1,    50,
      51,    52,    -1,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    -1,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,
      91,    -1,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,   107,    -1,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,    -1,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
     181,   182,   183,   184,   185,   186,   187,    -1,    -1,   190,
     191,    -1,    -1,   194,   195,    -1,    -1,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,   208,   209,   210,
     211,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,    -1,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,    -1,   263,   264,   265,    -1,   267,   268,    -1,   270,
      -1,   272,   273,   274,   275,   276,   277,   278,   279,   280,
      -1,    -1,   283,   284,   285,   286,   287,   288,    -1,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,    -1,
      -1,   302,   303,   304,   305,   306,   307,    -1,   309,   310,
     311,   312,   313,   314,   315,   316,    -1,    -1,    -1,    -1,
     321,   322,    -1,    -1,   325,   326,    -1,    -1,    -1,   330,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    85,    86,    87,    88,    -1,    90,    91,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,   107,    -1,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,   181,   182,
     183,   184,   185,   186,   187,    -1,    -1,   190,   191,    -1,
      -1,   194,   195,    -1,    -1,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,   208,   209,   210,   211,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,    -1,   240,   241,   242,
     243,   244,   245,   246,   247,    -1,   249,   250,    -1,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,    -1,
     263,   264,   265,    -1,   267,   268,    -1,   270,    -1,   272,
     273,   274,   275,   276,   277,   278,   279,   280,    -1,   282,
     283,   284,   285,   286,   287,   288,    -1,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,    -1,    -1,   302,
     303,   304,   305,   306,   307,    -1,   309,   310,   311,   312,
     313,   314,   315,   316,    -1,    -1,    -1,    -1,   321,   322,
      -1,    -1,   325,   326,    -1,    -1,    -1,   330,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      85,    86,    87,    88,    -1,    90,    91,    -1,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,   107,    -1,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,    -1,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,    -1,   181,   182,   183,   184,
     185,   186,   187,    -1,    -1,   190,   191,    -1,    -1,   194,
     195,    -1,    -1,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,   208,   209,   210,   211,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,    -1,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,    -1,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,    -1,   263,   264,
     265,    -1,   267,   268,    -1,   270,    -1,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    -1,    -1,   283,   284,
     285,   286,   287,   288,    -1,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    -1,    -1,   302,   303,   304,
     305,   306,   307,    -1,   309,   310,   311,   312,   313,    -1,
     315,   316,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,   330,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    28,    29,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    -1,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,
      90,    91,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,   107,    -1,   109,
     110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
     140,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,   181,   182,   183,   184,   185,   186,   187,    -1,    -1,
     190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,   208,   209,
     210,   211,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,    -1,
     240,   241,   242,   243,   244,   245,   246,   247,    -1,   249,
     250,    -1,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,    -1,   263,   264,   265,    -1,   267,   268,    -1,
     270,    -1,   272,   273,   274,   275,   276,   277,   278,   279,
     280,    -1,    -1,   283,   284,   285,   286,   287,   288,    -1,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
      -1,    -1,   302,   303,   304,   305,   306,   307,    -1,   309,
     310,   311,   312,   313,   314,   315,   316,    -1,    -1,    -1,
      -1,   321,   322,    -1,    -1,   325,   326,    -1,    -1,    -1,
     330,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    -1,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,    91,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,   107,    -1,   109,   110,    -1,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,   140,    -1,
     142,    -1,    -1,   145,   146,   147,   148,   149,   150,    -1,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,   181,
     182,    -1,   184,   185,   186,   187,    -1,    -1,   190,   191,
      -1,    -1,    -1,   195,    -1,    -1,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,   208,   209,   210,   211,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    -1,   240,   241,
     242,   243,   244,   245,   246,   247,    -1,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
      -1,   263,   264,   265,    -1,   267,   268,    -1,   270,    -1,
     272,   273,   274,   275,   276,   277,   278,   279,   280,    -1,
     282,   283,   284,   285,   286,   287,   288,    -1,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,    -1,    -1,
     302,   303,   304,   305,   306,   307,    -1,   309,   310,   311,
     312,   313,    -1,   315,   316,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   326,    -1,    -1,    -1,   330,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    -1,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    85,    86,    87,    88,    -1,    90,    91,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,   107,    -1,   109,   110,    -1,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,   140,    -1,   142,    -1,
      -1,   145,   146,   147,   148,   149,   150,    -1,   152,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,   181,   182,    -1,
     184,   185,   186,   187,    -1,    -1,   190,   191,    -1,    -1,
      -1,   195,    -1,    -1,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,   208,   209,   210,   211,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,    -1,   240,   241,   242,   243,
     244,   245,   246,   247,    -1,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,    -1,   263,
     264,   265,    -1,   267,   268,    -1,   270,    -1,   272,   273,
     274,   275,   276,   277,   278,   279,   280,    -1,   282,   283,
     284,   285,   286,   287,   288,    -1,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,    -1,    -1,   302,   303,
     304,   305,   306,   307,    -1,   309,   310,   311,   312,   313,
      -1,   315,   316,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,   330,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,   107,    -1,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,    -1,    -1,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    -1,   181,   182,    -1,   184,   185,   186,   187,    -1,
      -1,   190,   191,    -1,    -1,    -1,   195,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,   243,   244,   245,   246,   247,    -1,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,   276,   277,   278,
     279,   280,    -1,    -1,   283,   284,   285,   286,   287,   288,
      -1,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,   310,   311,   312,   313,    -1,   315,   316,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,   326,    11,    -1,
      -1,   330,    -1,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    85,    86,    87,    88,    -1,    90,    91,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,   107,    -1,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
      -1,    -1,   145,   146,   147,   148,   149,   150,    -1,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,   181,   182,
      -1,   184,   185,   186,   187,    -1,    -1,   190,   191,    -1,
      -1,    -1,   195,    -1,    -1,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,   208,   209,   210,   211,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,    -1,   240,   241,   242,
     243,   244,   245,   246,   247,    -1,   249,   250,    -1,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,    -1,
     263,   264,   265,    -1,   267,   268,    -1,   270,    -1,   272,
     273,   274,   275,   276,   277,   278,   279,   280,    -1,    -1,
     283,   284,   285,   286,   287,   288,    -1,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,    -1,    -1,   302,
     303,   304,   305,   306,   307,    -1,   309,   310,   311,   312,
     313,    -1,   315,   316,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,   330,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    -1,    -1,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,   105,    -1,   107,    -1,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,   154,    -1,   156,   157,   158,
      -1,    -1,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    -1,   181,   182,   183,    -1,   185,   186,   187,    -1,
      -1,   190,   191,    -1,   193,   194,   195,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,    -1,   277,   278,
     279,   280,    -1,    -1,   283,   284,   285,   286,   287,    -1,
      -1,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,   330,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    34,    35,    -1,
      -1,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    85,    86,
      87,    88,    -1,    90,    91,    -1,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,   105,    -1,
     107,    -1,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,    -1,   156,
     157,   158,    -1,    -1,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    -1,   181,   182,   183,    -1,   185,   186,
     187,    -1,    -1,   190,   191,    -1,   193,   194,   195,    -1,
      -1,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,   208,   209,   210,   211,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,    -1,   240,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,    -1,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,    -1,   263,   264,   265,    -1,
     267,   268,    -1,   270,    -1,   272,   273,   274,   275,    -1,
     277,   278,   279,   280,    -1,    -1,   283,   284,   285,   286,
     287,    -1,    -1,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,    -1,    -1,   302,   303,   304,   305,   306,
     307,    -1,   309,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,   330,    23,    24,    25,    -1,    27,    -1,
      29,    30,    -1,    32,    33,    34,    35,    -1,    -1,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,   105,    -1,   107,    -1,
     109,    -1,    -1,    -1,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,   173,   174,   175,   176,   177,   178,
     179,    -1,   181,   182,    -1,    -1,   185,   186,   187,    -1,
      -1,   190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,
      -1,    -1,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,    -1,   244,   245,   246,   247,    -1,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,    -1,   277,   278,
     279,   280,    -1,    -1,   283,   284,   285,   286,   287,    -1,
      -1,   290,   291,   292,   293,   294,   295,    -1,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,   330,    23,    24,    25,    -1,    27,    -1,    29,    30,
      -1,    32,    33,    34,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    -1,    -1,    50,
      51,    52,    -1,    54,    55,    56,    57,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    -1,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,
      91,    -1,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,   105,    -1,   107,    -1,   109,    -1,
      -1,    -1,   113,   114,    -1,    -1,   117,    -1,   119,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,    -1,    -1,   145,    -1,   147,   148,   149,   150,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,   173,    -1,   175,   176,   177,   178,   179,    -1,
     181,   182,    -1,    -1,   185,   186,   187,    -1,    -1,   190,
      -1,    -1,    -1,   194,   195,    -1,    -1,   198,    -1,    -1,
     201,   202,   203,   204,   205,   206,    -1,   208,   209,   210,
     211,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
     231,   232,   233,   234,   235,   236,   237,   238,    -1,   240,
     241,   242,    -1,   244,   245,   246,   247,    -1,   249,   250,
      -1,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,    -1,   263,   264,   265,    -1,   267,   268,    -1,   270,
      -1,   272,   273,   274,   275,    -1,   277,   278,   279,   280,
      -1,    -1,   283,   284,   285,   286,   287,    -1,    -1,   290,
     291,   292,   293,   294,   295,    -1,   297,   298,   299,    -1,
      -1,   302,   303,   304,   305,   306,   307,    -1,   309,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,   330,
      23,    24,    25,    -1,    27,    -1,    29,    30,    -1,    32,
      33,    34,    35,    -1,    -1,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    85,    86,    87,    88,    -1,    90,    91,    -1,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,   105,    -1,   107,    -1,   109,    -1,    -1,    -1,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
      -1,    -1,   145,    -1,   147,   148,   149,   150,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
     173,    -1,   175,   176,   177,   178,   179,    -1,   181,   182,
      -1,    -1,   185,   186,   187,   143,    -1,   190,    -1,    -1,
      -1,   194,   195,    -1,    -1,   198,    -1,    -1,   201,   202,
     203,   204,   205,   206,    -1,   208,   209,   210,   211,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,   231,   232,
     233,   234,   235,   236,   237,   238,   194,   240,   241,   242,
      -1,   244,   245,   246,   247,    -1,   249,   250,    -1,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,    -1,
     263,   264,   265,    -1,   267,   268,    14,   270,    -1,   272,
     273,   274,   275,    21,   277,   278,   279,   280,    26,    -1,
     283,   284,   285,   286,   287,    -1,    14,   290,   291,   292,
     293,   294,   295,    21,   297,   298,   299,    -1,    26,   302,
     303,   304,   305,   306,   307,    -1,   309,    -1,    -1,    -1,
      -1,   314,    -1,    -1,   317,   318,   319,    94,   321,   322,
     323,   324,   325,   326,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,    -1,
     318,   319,   139,   321,   322,   323,   324,   325,   326,    -1,
      -1,    -1,    -1,   121,   332,    -1,    -1,   125,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   143,   144,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,   143,   144,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    -1,
      -1,    -1,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   262,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     248,    -1,    -1,    -1,   281,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     248,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   331,    -1,   333,   334,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,   317,
     318,   319,    -1,   321,   322,   323,   324,   325,   326,    -1,
      -1,   329,    -1,    -1,   332,    -1,   314,    -1,   336,   317,
     318,   319,    -1,   321,   322,   323,   324,   325,   326,    -1,
      -1,    -1,    -1,    -1,   332,    -1,    -1,   335,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    23,    24,
      25,    -1,    27,    -1,    29,    30,    -1,    32,    33,    34,
      35,    -1,    -1,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      85,    86,    87,    88,    -1,    90,    91,    -1,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
     105,    -1,   107,    -1,   109,    -1,    -1,    -1,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,    -1,   142,    -1,    -1,
     145,    -1,   147,   148,   149,   150,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,   173,    -1,
     175,   176,   177,   178,   179,    -1,   181,   182,    -1,    -1,
     185,   186,   187,    -1,    -1,   190,    -1,    -1,    -1,   194,
     195,    -1,    -1,   198,    -1,    -1,   201,   202,   203,   204,
     205,   206,    -1,   208,   209,   210,   211,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,   231,   232,   233,   234,
     235,   236,   237,   238,    -1,   240,   241,   242,    -1,   244,
     245,   246,   247,    -1,   249,   250,    -1,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,    -1,   263,   264,
     265,    -1,   267,   268,    -1,   270,    -1,   272,   273,   274,
     275,    -1,   277,   278,   279,   280,    -1,    -1,   283,   284,
     285,   286,   287,    -1,    -1,   290,   291,   292,   293,   294,
     295,    -1,   297,   298,   299,    -1,    -1,   302,   303,   304,
     305,   306,   307,    14,   309,    -1,    -1,    18,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,    -1,   323,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      17,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    82,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    17,    -1,    -1,    -1,    21,    -1,
     121,    -1,   106,    26,   125,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,   125,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   121,    -1,    -1,    -1,   125,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,   180,
      -1,    -1,   183,    -1,   121,    -1,    -1,   154,   125,    -1,
      -1,    -1,    -1,   194,    -1,   196,   180,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,    -1,
     194,    -1,   196,   180,    -1,    -1,   183,   154,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,   194,    -1,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,    -1,   180,    -1,    -1,   183,   248,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,
      -1,    14,    -1,    -1,   248,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,   180,    -1,    -1,
     183,   248,    -1,    -1,    -1,    -1,    -1,    -1,   289,    -1,
      -1,   194,    -1,   196,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,   248,    -1,   314,    -1,    -1,   317,   318,   319,    -1,
     321,   322,   323,   324,   325,   326,    -1,    -1,    -1,    -1,
     314,   332,    -1,   317,   318,   319,    -1,   321,   322,   323,
     324,   325,   326,    -1,    -1,   248,    -1,   314,   332,    -1,
     317,   318,   319,    -1,   321,   322,   323,   324,   325,   326,
      -1,    -1,    14,    -1,    -1,   332,    -1,    -1,   121,    21,
      -1,    -1,   125,    -1,    26,    -1,    -1,   314,    -1,    -1,
     317,   318,   319,    -1,   321,   322,   323,   324,   325,   326,
     143,   144,    -1,    -1,   331,   332,    -1,    -1,    -1,    -1,
      -1,   154,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,   314,    -1,    -1,   317,   318,   319,    -1,   321,   322,
     323,   324,   325,   326,   143,   144,    -1,   180,    -1,   332,
     183,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,   194,    -1,   196,    -1,    -1,    -1,    -1,    14,    -1,
      -1,    17,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,   194,    -1,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,
      -1,   143,   144,    -1,    -1,   248,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,   180,   248,
      -1,   183,    -1,    -1,    -1,    -1,   289,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,   121,    -1,    -1,    26,   125,
      -1,   314,    -1,    -1,   317,   318,   319,    -1,   321,   322,
     323,   324,   325,   326,    -1,    -1,    -1,   143,   144,   332,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,   314,   248,    -1,   317,   318,
     319,    -1,   321,   322,   323,   324,   325,   326,    -1,   111,
      -1,    -1,    -1,   332,   180,    -1,    -1,   183,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,   194,    -1,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,   314,    -1,    -1,   317,   318,   319,    -1,   321,
     322,   323,   324,   325,   326,   143,   144,    -1,   180,   331,
     332,   183,   248,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,   248,    -1,   314,    -1,
      -1,   317,   318,   319,    -1,   321,   322,   323,   324,   325,
     326,    -1,    -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
     248,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   266,    -1,
      -1,    -1,    14,   121,    -1,    -1,    -1,   125,    -1,    21,
      -1,    -1,   314,    -1,    26,   317,   318,   319,    -1,   321,
     322,   323,   324,   325,   326,   143,   144,    -1,    -1,    -1,
     332,    -1,    -1,    -1,    -1,    -1,   154,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,   314,    -1,    -1,   317,
     318,   319,    -1,   321,   322,   323,   324,   325,   326,   143,
     144,    -1,   180,    -1,   332,   183,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    -1,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,   180,    -1,    21,   183,
      -1,    -1,    -1,    26,   143,   144,    -1,    -1,    -1,   121,
     194,    -1,   196,   125,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     248,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   154,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    -1,    -1,
      -1,    -1,    -1,    -1,   248,    -1,    -1,    -1,   180,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,   121,   317,
     318,   319,   125,   321,   322,   323,   324,   325,   326,   248,
      -1,    -1,    -1,   331,   332,    -1,    -1,    21,    -1,    -1,
     143,   144,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   154,    -1,   317,   318,   319,   248,   321,   322,   323,
     324,   325,   326,    -1,    21,    -1,    -1,   331,   332,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    21,
     183,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    -1,    -1,   314,    -1,    -1,   317,   318,
     319,    -1,   321,   322,   323,   324,   325,   326,    -1,    21,
     329,    -1,    -1,   332,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,    -1,    -1,   317,   318,   319,    -1,   321,
     322,   323,   324,   325,   326,    -1,    -1,   121,    -1,    -1,
     332,   125,    -1,    -1,    -1,   248,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    93,    -1,   125,    -1,    -1,   143,   144,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,   180,   154,    -1,   183,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     194,   314,   154,   125,   317,   318,   319,    -1,   321,   322,
     323,   324,   325,   326,    -1,    -1,   183,    -1,    -1,   332,
      -1,   143,   144,    -1,    -1,    -1,    -1,   194,    -1,    -1,
      -1,   183,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    93,    -1,   248,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   248,    -1,    -1,   125,    -1,    -1,    -1,    94,    21,
      -1,    -1,    -1,    -1,    26,    -1,   248,    -1,    -1,    -1,
     106,    -1,   143,   144,    -1,   111,    -1,    -1,    -1,    -1,
     116,    -1,   118,   154,    21,    -1,    93,    -1,    -1,    26,
     314,    -1,    -1,   317,   318,   319,   248,   321,   322,   323,
     324,   325,   326,   139,    -1,    -1,    -1,    -1,   332,    -1,
      -1,    -1,   183,    -1,   121,    -1,    -1,   314,   125,   155,
      -1,   318,   319,   194,   321,   322,   323,   324,   325,   326,
      -1,    93,   314,    -1,    -1,   332,   143,   144,    -1,   321,
     322,   323,   324,   325,   326,    -1,    -1,   154,    -1,    -1,
     332,    -1,    -1,   189,    -1,    -1,    93,    -1,    -1,   121,
      -1,   197,   314,   125,    -1,    -1,    -1,    -1,    -1,   321,
     322,   323,   324,   325,   326,    -1,   183,   248,    -1,    -1,
     332,   143,   144,    -1,   121,    -1,    -1,   194,   125,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,   262,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   248,    -1,   314,    -1,   281,   183,    -1,    -1,    -1,
     321,   322,   323,   324,   325,   326,    -1,   194,    -1,    -1,
      -1,   332,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   248,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   331,    -1,   333,   334,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,
      -1,   248,    -1,    -1,   321,   322,   323,   324,   325,   326,
      -1,    -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,    -1,    -1,    -1,    -1,    -1,    -1,   321,
     322,   323,   324,   325,   326,    -1,    -1,    -1,    -1,    -1,
     332,    -1,    -1,    -1,    -1,    -1,    -1,   314,    -1,    -1,
      -1,    -1,    -1,    -1,   321,   322,   323,   324,   325,   326,
      -1,    -1,    -1,    -1,    -1,   332,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,    -1,   309,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    23,    24,    25,    -1,    27,    -1,    29,
      30,    -1,    32,    33,    34,    35,    -1,    -1,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    -1,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    85,    86,    87,    88,    -1,
      90,    91,    -1,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,   105,    -1,   107,    -1,   109,
      -1,    -1,    -1,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,   138,    -1,
     140,    -1,   142,    -1,    -1,   145,    -1,   147,   148,   149,
     150,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,   173,   174,   175,   176,   177,   178,   179,
      -1,   181,   182,    -1,    -1,   185,   186,   187,    -1,    -1,
     190,   191,    -1,    -1,   194,   195,    -1,    -1,   198,    -1,
      -1,   201,   202,   203,   204,   205,   206,    -1,   208,   209,
     210,   211,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,   231,   232,   233,   234,   235,   236,   237,   238,    -1,
     240,   241,   242,    -1,   244,   245,   246,   247,    -1,   249,
     250,    -1,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,    -1,   267,   268,    -1,
     270,    -1,   272,   273,   274,   275,    -1,   277,   278,   279,
     280,    -1,    -1,   283,   284,   285,   286,   287,    -1,    -1,
     290,   291,   292,   293,   294,   295,    -1,   297,   298,   299,
      -1,    -1,   302,   303,   304,   305,   306,   307,    -1,   309,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      23,    24,    25,    -1,    27,    -1,    29,    30,    -1,    32,
      33,    34,    35,    -1,    -1,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    85,    86,    87,    88,    -1,    90,    91,    -1,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,   105,    -1,   107,    -1,   109,    -1,    -1,    -1,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
      -1,    -1,   145,    -1,   147,   148,   149,   150,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
     173,   174,   175,   176,   177,   178,   179,    -1,   181,   182,
      -1,    -1,   185,   186,   187,    -1,    -1,   190,   191,    -1,
      -1,   194,   195,    -1,    -1,   198,    -1,    -1,   201,   202,
     203,   204,   205,   206,    -1,   208,   209,   210,   211,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,   231,   232,
     233,   234,   235,   236,   237,   238,    -1,   240,   241,   242,
      -1,   244,   245,   246,   247,    -1,   249,   250,    -1,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,    -1,
     263,   264,   265,    -1,   267,   268,    -1,   270,    -1,   272,
     273,   274,   275,    -1,   277,   278,   279,   280,    -1,    -1,
     283,   284,   285,   286,   287,    -1,    -1,   290,   291,   292,
     293,   294,   295,    -1,   297,   298,   299,    -1,    -1,   302,
     303,   304,   305,   306,   307,    -1,   309,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    19,    20,    21,    -1,    23,    24,    25,
      -1,    27,    -1,    29,    30,    -1,    32,    33,    34,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    -1,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    75,
      -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,    85,
      86,    87,    88,    -1,    90,    91,    -1,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,   105,
      -1,   107,    -1,   109,    -1,    -1,    -1,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
     136,   137,   138,    -1,   140,    -1,   142,    -1,    -1,   145,
      -1,   147,   148,   149,   150,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,   173,    -1,   175,
     176,   177,   178,   179,    -1,   181,   182,    -1,    -1,   185,
     186,   187,    -1,    -1,   190,    -1,    -1,    -1,   194,   195,
      -1,    -1,   198,    -1,    -1,   201,   202,   203,   204,   205,
     206,    -1,   208,   209,   210,   211,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,   231,   232,   233,   234,   235,
     236,   237,   238,    -1,   240,   241,   242,    -1,   244,   245,
     246,   247,    -1,   249,   250,    -1,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,    -1,   263,   264,   265,
      -1,   267,   268,    -1,   270,    -1,   272,   273,   274,   275,
      -1,   277,   278,   279,   280,    -1,    -1,   283,   284,   285,
     286,   287,    -1,    -1,   290,   291,   292,   293,   294,   295,
      -1,   297,   298,   299,    -1,    -1,   302,   303,   304,   305,
     306,   307,    -1,   309,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      19,    20,    21,    -1,    23,    24,    25,    -1,    27,    -1,
      29,    30,    -1,    32,    33,    34,    35,    -1,    -1,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    91,    -1,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,   105,    -1,   107,    -1,
     109,    -1,    -1,    -1,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,   173,    -1,   175,   176,   177,   178,
     179,    -1,   181,   182,    -1,    -1,   185,   186,   187,    -1,
      -1,   190,    -1,    -1,    -1,   194,   195,    -1,    -1,   198,
      -1,    -1,   201,   202,   203,   204,   205,   206,    -1,   208,
     209,   210,   211,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,   231,   232,   233,   234,   235,   236,   237,   238,
      -1,   240,   241,   242,    -1,   244,   245,   246,   247,    -1,
     249,   250,    -1,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,    -1,   263,   264,   265,    -1,   267,   268,
      -1,   270,    -1,   272,   273,   274,   275,    -1,   277,   278,
     279,   280,    -1,    -1,   283,   284,   285,   286,   287,    -1,
      -1,   290,   291,   292,   293,   294,   295,    -1,   297,   298,
     299,    -1,    -1,   302,   303,   304,   305,   306,   307,    -1,
     309,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    25,    -1,    27,    -1,    29,    30,    -1,
      32,    33,    34,    35,    -1,    -1,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    -1,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,    91,
      -1,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,   105,    -1,   107,    -1,   109,    -1,    -1,
      -1,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,   136,   137,   138,    -1,   140,    -1,
     142,    -1,    -1,   145,    -1,   147,   148,   149,   150,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,   173,    -1,   175,   176,   177,   178,   179,    -1,   181,
     182,    -1,    -1,   185,   186,   187,    -1,    -1,   190,    -1,
      -1,    -1,   194,   195,    -1,    -1,   198,    -1,    -1,   201,
     202,   203,   204,   205,   206,    -1,   208,   209,   210,   211,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,   231,
     232,   233,   234,   235,   236,   237,   238,    -1,   240,   241,
     242,    -1,   244,   245,   246,   247,    -1,   249,   250,    -1,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
      -1,   263,   264,   265,    -1,   267,   268,    -1,   270,    -1,
     272,   273,   274,   275,    -1,   277,   278,   279,   280,    -1,
      -1,   283,   284,   285,   286,   287,    -1,    -1,   290,   291,
     292,   293,   294,   295,    -1,   297,   298,   299,    -1,    -1,
     302,   303,   304,   305,   306,   307,    -1,   309,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    23,    24,
      25,    -1,    27,    -1,    29,    30,    -1,    32,    33,    34,
      35,    -1,    -1,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    -1,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    57,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      85,    86,    87,    88,    -1,    90,    91,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,    -1,    -1,   103,   104,
     105,    -1,   107,    -1,   109,    -1,    -1,    -1,   113,   114,
      -1,    -1,   117,    -1,   119,   120,    -1,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,    -1,   142,    -1,    -1,
     145,    -1,   147,   148,   149,   150,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,   173,    -1,
     175,   176,   177,   178,    -1,    -1,   181,   182,    -1,    -1,
      -1,   186,   187,    -1,    -1,   190,    -1,    -1,    -1,   194,
     195,    -1,    -1,   198,    -1,    -1,    -1,   202,   203,   204,
     205,   206,    -1,    -1,   209,   210,   211,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,   231,    -1,   233,   234,
     235,   236,   237,   238,    -1,   240,   241,   242,    -1,   244,
     245,   246,   247,    -1,   249,   250,    -1,   252,   253,   254,
     255,   256,   257,   258,   259,    -1,   261,    -1,   263,   264,
     265,    -1,   267,   268,    -1,   270,    -1,   272,    -1,   274,
      -1,    -1,   277,   278,   279,   280,    -1,    -1,   283,   284,
     285,   286,   287,    -1,    -1,   290,   291,   292,   293,   294,
     295,    -1,   297,   298,   299,    -1,    -1,   302,   303,   304,
     305,   306,   307,    -1,   309,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    23,    24,    25,    -1,    27,
      -1,    29,    30,    -1,    32,    33,    34,    35,    -1,    -1,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    -1,
      -1,    -1,    50,    51,    52,    -1,    54,    55,    -1,    57,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    85,    86,    87,
      88,    -1,    90,    91,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,    -1,    -1,   103,   104,   105,    -1,   107,
      -1,   109,    -1,    -1,    -1,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,   136,   137,
     138,    -1,   140,    -1,   142,    -1,    -1,   145,    -1,   147,
     148,   149,   150,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,   173,    -1,   175,   176,   177,
     178,    -1,    -1,   181,   182,    -1,    -1,    -1,   186,   187,
      -1,    -1,   190,    -1,    -1,    -1,   194,   195,    -1,    -1,
     198,    -1,    -1,    -1,   202,   203,   204,   205,   206,    -1,
      -1,   209,   210,   211,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,   231,    -1,   233,   234,   235,   236,   237,
     238,    -1,   240,   241,   242,    -1,   244,    -1,   246,   247,
      -1,   249,   250,    -1,   252,   253,   254,   255,   256,   257,
     258,   259,    -1,   261,    -1,   263,   264,   265,    -1,   267,
     268,    -1,   270,    -1,   272,    -1,   274,    -1,    -1,   277,
     278,   279,   280,    -1,    -1,   283,   284,   285,   286,   287,
      -1,    -1,   290,   291,   292,   293,   294,   295,    -1,   297,
     298,   299,    -1,    -1,   302,   303,   304,   305,   306,   307,
      -1,   309
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   239,   330,   338,   339,   340,   341,   342,   343,   344,
      10,    83,   350,   341,   342,     0,   339,   334,    94,   139,
     197,   281,   351,   352,   192,     3,     4,     5,     6,     7,
       8,     9,    11,    16,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    45,    46,
      47,    50,    51,    52,    54,    55,    56,    57,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    75,    77,    78,    79,    80,    81,    85,
      86,    87,    88,    90,    91,    93,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   107,   109,   110,
     112,   113,   114,   117,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   131,   132,   133,   134,   135,
     136,   137,   138,   140,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   152,   153,   154,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   190,   191,
     194,   195,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   208,   209,   210,   211,   213,   214,   215,   216,   217,
     218,   219,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   263,   264,   265,   267,   268,   270,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   282,   283,   284,   285,
     286,   287,   288,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   302,   303,   304,   305,   306,   307,   309,
     310,   311,   312,   313,   314,   315,   316,   321,   322,   323,
     325,   326,   330,   341,   380,   381,   382,   387,   388,   389,
     391,   392,   393,   394,   397,   398,   401,   402,   407,   410,
     412,   429,   434,   435,   437,   438,   439,   443,   444,   445,
     446,   447,   448,   449,   451,   452,   453,   455,   331,   331,
      10,    83,   349,   349,    32,   349,   106,   155,   189,   355,
     361,   330,   328,   341,   419,   295,   395,   410,   433,   330,
     395,   395,   330,   330,   330,   330,   330,   385,   385,   209,
     341,   330,   330,   383,   330,   330,    39,    40,   395,   410,
     330,   330,   384,   330,   330,   330,   330,   330,   302,   303,
     330,   399,   330,   399,   330,   330,   341,   333,   414,   436,
     410,   410,   410,   410,   341,   403,   410,   415,   446,    40,
     330,   396,   330,   446,   125,   143,   180,   194,   200,   314,
     317,   318,   319,   321,   322,   323,   324,   325,   326,   405,
     406,   409,   410,    14,    17,    21,    26,   121,   125,   143,
     144,   154,   180,   183,   194,   196,   248,   314,   317,   318,
     319,   321,   322,   323,   324,   325,   326,   332,   407,   409,
     414,   141,   335,   345,   414,   436,   330,   341,   343,   344,
     343,   353,   354,   410,   343,   217,   286,    10,   357,   410,
     358,   410,   361,   362,   355,   356,   410,   415,   415,   418,
     419,   330,   300,   430,   431,   410,   415,   410,   415,   445,
     445,   445,    69,   120,   165,   168,   237,   306,   309,   311,
     416,   420,   445,   445,   445,   395,   395,   410,   445,    27,
      29,    30,    39,    40,    47,    56,    71,    72,    86,    98,
     101,   105,   137,   138,   140,   171,   173,   185,   186,   194,
     201,   208,   218,   232,   250,   260,   267,   268,   273,   275,
     294,   309,   405,   408,   447,   451,   410,   421,   321,   322,
     325,   326,   330,   407,   411,   412,   423,   331,   403,   410,
     410,   415,   424,   267,   267,   445,   445,   410,    31,   111,
     151,   271,   410,   415,   427,   323,   442,   447,   328,   414,
     331,   331,   335,   244,   445,   445,    69,   120,   165,   168,
     237,   306,   400,   341,    83,   180,   184,   125,   330,   232,
     330,   402,    10,    15,   251,   330,   341,   402,   404,   410,
      10,    12,    13,    14,    15,    16,    17,    18,    31,    36,
      37,    42,    48,    49,    53,    58,    62,    63,    64,    65,
      74,    76,    82,    83,    84,    89,    92,    94,   102,   106,
     108,   111,   115,   116,   118,   130,   139,   141,   151,   155,
     159,   160,   174,   180,   184,   188,   189,   191,   192,   193,
     196,   197,   207,   212,   220,   239,   243,   251,   262,   266,
     269,   271,   276,   281,   282,   288,   289,   300,   301,   309,
     450,   451,   452,   453,   454,   267,   411,   410,   330,   341,
     428,    83,   102,   180,   184,   187,   276,   283,   410,    26,
     121,   125,   154,   248,   330,   410,   269,   410,   410,   410,
     410,   410,   410,   410,   410,   410,    27,    29,    30,    39,
      40,    71,    72,   105,   137,   138,   140,   173,   186,   218,
     245,   250,   267,   268,   294,   309,   377,   379,   381,   382,
     386,   388,   389,   390,   392,   393,   397,   398,   448,   451,
     410,   404,   114,   158,   262,   263,   265,   346,   347,   435,
     439,   438,   111,   364,    10,    83,   323,   331,   415,   335,
      18,    82,   289,   193,   187,   363,   189,   335,   155,   331,
     335,   329,   329,   335,   445,   410,    89,   431,   432,    17,
     331,   289,   331,   331,   331,   331,   335,   331,   111,   331,
     331,   331,   335,   331,   335,   331,   333,   207,   422,   331,
     411,   411,   411,   411,   410,   411,   125,   143,   194,   317,
     318,   319,   321,   322,   323,   324,   325,   326,   332,   407,
     331,   331,   331,   106,   111,   425,   426,   331,   307,   307,
     331,   331,    17,   427,   415,   427,   427,   111,   331,   436,
     410,   414,   436,   410,   447,   331,   331,   269,   269,   269,
     269,   111,   184,   341,   408,   341,   307,    14,    93,   415,
     111,   102,   184,   187,   276,   283,   330,    93,   411,   410,
     428,   410,   269,   408,   410,   379,    16,   378,   330,   400,
     436,   330,   341,   263,   265,   263,   265,   347,   262,   348,
     348,   193,   309,   330,   341,   347,   365,   366,   367,   372,
     373,   435,   443,   451,   301,   374,   415,   415,   331,   331,
     354,   409,   440,   441,   447,   358,   358,   357,   410,   419,
     331,   266,   410,    92,   377,   413,   435,   439,   447,   445,
     410,   410,   445,   408,   410,   425,   411,    83,   180,   187,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   377,
     411,   410,   410,   426,   425,   399,   399,   377,   331,   331,
     331,   415,   329,   336,   414,   396,   446,   120,   165,   237,
     165,   237,   237,   168,   402,   331,   412,   411,   410,   331,
     410,   330,   377,   417,   410,    14,    93,    93,   410,   331,
      93,    16,   378,   328,   328,   445,   410,   348,   348,   348,
     348,   347,   347,   330,   347,   341,   366,   367,    17,   368,
     447,   323,   335,    61,   112,   131,   146,   152,   172,   230,
     308,   369,   368,    17,   368,   447,   330,   410,   116,   359,
     331,   331,   335,   410,   331,   331,   331,   331,   331,   426,
     111,   187,   330,   331,   410,   400,   417,   331,   335,   411,
     410,   410,    93,   410,   328,   445,   329,   445,   331,   331,
     347,   347,   347,   347,   347,   331,   447,   330,   366,   146,
     199,   370,   366,   370,   146,   369,   370,   366,   146,   330,
     447,   330,   331,   415,    32,   118,   360,   441,   411,   330,
     417,   329,   331,   377,   410,   445,   329,   329,   400,   331,
     368,   330,   440,   366,   192,   289,   371,   366,   146,   366,
     375,   376,   447,   330,   375,   447,   331,   415,   410,   417,
     331,   329,   440,   331,   410,   330,   366,   371,   331,   335,
     377,   375,   331,   331,   331,   440,   376,   331,   331
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   337,   338,   338,   339,   340,   340,   341,   341,   342,
     342,   342,   342,   343,   343,   344,   344,   344,   344,   345,
     345,   346,   346,   346,   346,   346,   346,   346,   346,   347,
     347,   348,   348,   349,   349,   349,   350,   350,   350,   350,
     351,   351,   352,   353,   353,   354,   354,   354,   354,   355,
     355,   355,   355,   355,   356,   356,   357,   357,   358,   359,
     359,   360,   360,   361,   361,   362,   362,   363,   363,   364,
     364,   365,   365,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   367,   367,   367,   367,   367,   367,
     367,   368,   368,   368,   368,   369,   369,   369,   369,   370,
     370,   371,   371,   372,   372,   372,   372,   373,   373,   374,
     374,   375,   375,   376,   377,   377,   377,   377,   378,   378,
     378,   379,   379,   379,   379,   379,   379,   379,   379,   380,
     380,   380,   380,   380,   381,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   383,   383,   384,   384,
     384,   385,   385,   385,   386,   386,   387,   387,   388,   389,
     390,   390,   391,   391,   392,   393,   394,   394,   394,   394,
     394,   394,   395,   395,   396,   396,   397,   397,   397,   397,
     398,   399,   399,   399,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   401,   401,
     401,   401,   401,   401,   401,   401,   401,   402,   402,   402,
     402,   403,   404,   404,   404,   405,   405,   406,   406,   406,
     406,   406,   406,   406,   406,   406,   407,   407,   408,   408,
     409,   409,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   413,   413,   414,   414,   414,   415,   415,   416,   416,
     417,   417,   418,   418,   419,   419,   420,   420,   420,   420,
     420,   420,   420,   420,   421,   421,   422,   423,   423,   424,
     424,   424,   424,   424,   424,   425,   426,   427,   427,   427,
     428,   428,   429,   429,   429,   430,   430,   431,   432,   432,
     433,   433,   434,   434,   435,   436,   436,   436,   437,   437,
     438,   438,   438,   439,   439,   440,   440,   441,   442,   443,
     443,   444,   444,   444,   444,   444,   444,   444,   444,   444,
     444,   444,   444,   445,   446,   447,   447,   447,   448,   448,
     449,   449,   449,   450,   450,   450,   450,   450,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   452,   452,   452,   452,   452,   452,   452,   452,
     452,   452,   452,   452,   452,   452,   452,   452,   452,   452,
     452,   452,   452,   452,   452,   452,   452,   452,   452,   452,
     452,   452,   452,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   455,   455
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     3,     3,     1,
       2,     4,     4,     1,     1,     8,     4,     4,     4,     2,
       0,     3,     3,     4,     4,     4,     4,     2,     1,     1,
       1,     1,     0,     1,     1,     0,     1,     5,     1,     0,
       1,     0,     3,     1,     3,     3,     2,     2,     1,     4,
       4,     2,     2,     4,     1,     0,     1,     1,     1,     3,
       0,     2,     0,     3,     3,     1,     0,     2,     0,     2,
       0,     1,     3,     1,     2,     1,     2,     5,     6,     5,
       1,     2,     1,     4,     3,     4,     3,     5,     4,     5,
       4,     5,     2,     4,     1,     2,     2,     2,     1,     1,
       0,     4,     2,     1,     2,     2,     4,     3,     4,     2,
       0,     1,     3,     2,     2,     3,     5,     6,     3,     4,
       0,     1,     1,     1,     1,     1,     2,     5,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     5,     3,
       0,     5,     3,     0,     1,     1,     1,     1,     5,     2,
       1,     1,     1,     1,     5,     2,     2,     2,     1,     3,
       3,     2,     1,     0,     3,     0,     5,     2,     5,     2,
       1,     3,     3,     0,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     0,     3,     4,
       4,     3,     3,     3,     4,     3,     5,     4,     4,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       1,     4,     1,     3,     5,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     2,     3,     5,     4,     6,
       3,     5,     4,     6,     4,     6,     5,     7,     2,     3,
       2,     4,     3,     4,     3,     4,     3,     4,     5,     6,
       7,     5,     6,     3,     4,     4,     6,     2,     1,     1,
       3,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     5,
       6,     7,     1,     1,     3,     5,     4,     1,     3,     4,
       5,     5,     4,     1,     1,     4,     1,     4,     1,     4,
       1,     4,     1,     1,     1,     6,     4,     4,     4,     4,
       6,     5,     5,     5,     4,     6,     4,     1,     2,     2,
       2,     1,     1,     4,     6,     0,     1,     3,     3,     0,
       3,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     2,     3,     0,     3,
       3,     2,     2,     1,     0,     2,     2,     3,     2,     1,
       1,     3,     5,     6,     4,     1,     2,     4,     2,     0,
       1,     0,     2,     2,     2,     2,     2,     3,     1,     3,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     6,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 80: /* table_ref: select_with_parens  */
#line 517 "sql.y"
                                {
					/*
					 * The SQL spec does not permit a subselect
					 * (<derived_table>) without an alias clause,
					 * so we don't either.  This avoids the problem
					 * of needing to invent a unique refname for it.
					 * That could be surmounted if there's sufficient
					 * popular demand, but for now let's just implement
					 * the spec and see if anyone complains.
					 * However, it does seem like a good idea to emit
					 * an error message that's better than "syntax error".
					 */

				}
#line 5082 "sql.c"
    break;


#line 5086 "sql.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1976 "sql.y"

