/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SQL_H_INCLUDED
# define YY_YY_SQL_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ABORT_P = 258,                 /* ABORT_P  */
    ABSOLUTE_P = 259,              /* ABSOLUTE_P  */
    ACCESS = 260,                  /* ACCESS  */
    ACTION = 261,                  /* ACTION  */
    ADD = 262,                     /* ADD  */
    AFTER = 263,                   /* AFTER  */
    AGGREGATE = 264,               /* AGGREGATE  */
    ALL = 265,                     /* ALL  */
    ALTER = 266,                   /* ALTER  */
    ANALYSE = 267,                 /* ANALYSE  */
    ANALYZE = 268,                 /* ANALYZE  */
    AND = 269,                     /* AND  */
    ANY = 270,                     /* ANY  */
    ARRAY = 271,                   /* ARRAY  */
    AS = 272,                      /* AS  */
    ASC = 273,                     /* ASC  */
    ASSERTION = 274,               /* ASSERTION  */
    ASSIGNMENT = 275,              /* ASSIGNMENT  */
    AT = 276,                      /* AT  */
    AUTHORIZATION = 277,           /* AUTHORIZATION  */
    BACKWARD = 278,                /* BACKWARD  */
    BEFORE = 279,                  /* BEFORE  */
    BEGIN_P = 280,                 /* BEGIN_P  */
    BETWEEN = 281,                 /* BETWEEN  */
    BIGINT = 282,                  /* BIGINT  */
    BINARY = 283,                  /* BINARY  */
    BIT = 284,                     /* BIT  */
    BOOLEAN_P = 285,               /* BOOLEAN_P  */
    BOTH = 286,                    /* BOTH  */
    BY = 287,                      /* BY  */
    CACHE = 288,                   /* CACHE  */
    CALLED = 289,                  /* CALLED  */
    CASCADE = 290,                 /* CASCADE  */
    CASE = 291,                    /* CASE  */
    CAST = 292,                    /* CAST  */
    CHAIN = 293,                   /* CHAIN  */
    CHAR_P = 294,                  /* CHAR_P  */
    CHARACTER = 295,               /* CHARACTER  */
    CHARACTERISTICS = 296,         /* CHARACTERISTICS  */
    CHECK = 297,                   /* CHECK  */
    CHECKPOINT = 298,              /* CHECKPOINT  */
    CLASS = 299,                   /* CLASS  */
    CLOSE = 300,                   /* CLOSE  */
    CLUSTER = 301,                 /* CLUSTER  */
    COALESCE = 302,                /* COALESCE  */
    COLLATE = 303,                 /* COLLATE  */
    COLUMN = 304,                  /* COLUMN  */
    COMMENT = 305,                 /* COMMENT  */
    COMMIT = 306,                  /* COMMIT  */
    COMMITTED = 307,               /* COMMITTED  */
    CONSTRAINT = 308,              /* CONSTRAINT  */
    CONSTRAINTS = 309,             /* CONSTRAINTS  */
    CONVERSION_P = 310,            /* CONVERSION_P  */
    CONVERT = 311,                 /* CONVERT  */
    COPY = 312,                    /* COPY  */
    CREATE = 313,                  /* CREATE  */
    CREATEDB = 314,                /* CREATEDB  */
    CREATEUSER = 315,              /* CREATEUSER  */
    CROSS = 316,                   /* CROSS  */
    CURRENT_DATE = 317,            /* CURRENT_DATE  */
    CURRENT_TIME = 318,            /* CURRENT_TIME  */
    CURRENT_TIMESTAMP = 319,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 320,            /* CURRENT_USER  */
    CURSOR = 321,                  /* CURSOR  */
    CYCLE = 322,                   /* CYCLE  */
    DATABASE = 323,                /* DATABASE  */
    DAY_P = 324,                   /* DAY_P  */
    DEALLOCATE = 325,              /* DEALLOCATE  */
    DEC = 326,                     /* DEC  */
    DECIMAL_P = 327,               /* DECIMAL_P  */
    DECLARE = 328,                 /* DECLARE  */
    DEFAULT = 329,                 /* DEFAULT  */
    DEFAULTS = 330,                /* DEFAULTS  */
    DEFERRABLE = 331,              /* DEFERRABLE  */
    DEFERRED = 332,                /* DEFERRED  */
    DEFINER = 333,                 /* DEFINER  */
    DELETE_P = 334,                /* DELETE_P  */
    DELIMITER = 335,               /* DELIMITER  */
    DELIMITERS = 336,              /* DELIMITERS  */
    DESC = 337,                    /* DESC  */
    DISTINCT = 338,                /* DISTINCT  */
    DO = 339,                      /* DO  */
    DOMAIN_P = 340,                /* DOMAIN_P  */
    DOUBLE_P = 341,                /* DOUBLE_P  */
    DROP = 342,                    /* DROP  */
    EACH = 343,                    /* EACH  */
    ELSE = 344,                    /* ELSE  */
    ENCODING = 345,                /* ENCODING  */
    ENCRYPTED = 346,               /* ENCRYPTED  */
    END_P = 347,                   /* END_P  */
    ESCAPE = 348,                  /* ESCAPE  */
    EXCEPT = 349,                  /* EXCEPT  */
    EXCLUDING = 350,               /* EXCLUDING  */
    EXCLUSIVE = 351,               /* EXCLUSIVE  */
    EXECUTE = 352,                 /* EXECUTE  */
    EXISTS = 353,                  /* EXISTS  */
    EXPLAIN = 354,                 /* EXPLAIN  */
    EXTERNAL = 355,                /* EXTERNAL  */
    EXTRACT = 356,                 /* EXTRACT  */
    FALSE_P = 357,                 /* FALSE_P  */
    FETCH = 358,                   /* FETCH  */
    FIRST_P = 359,                 /* FIRST_P  */
    FLOAT_P = 360,                 /* FLOAT_P  */
    FOR = 361,                     /* FOR  */
    FORCE = 362,                   /* FORCE  */
    FOREIGN = 363,                 /* FOREIGN  */
    FORWARD = 364,                 /* FORWARD  */
    FREEZE = 365,                  /* FREEZE  */
    FROM = 366,                    /* FROM  */
    FULL = 367,                    /* FULL  */
    FUNCTION = 368,                /* FUNCTION  */
    GLOBAL = 369,                  /* GLOBAL  */
    GRANT = 370,                   /* GRANT  */
    GROUP_P = 371,                 /* GROUP_P  */
    HANDLER = 372,                 /* HANDLER  */
    HAVING = 373,                  /* HAVING  */
    HOLD = 374,                    /* HOLD  */
    HOUR_P = 375,                  /* HOUR_P  */
    ILIKE = 376,                   /* ILIKE  */
    IMMEDIATE = 377,               /* IMMEDIATE  */
    IMMUTABLE = 378,               /* IMMUTABLE  */
    IMPLICIT_P = 379,              /* IMPLICIT_P  */
    IN_P = 380,                    /* IN_P  */
    INCLUDING = 381,               /* INCLUDING  */
    INCREMENT = 382,               /* INCREMENT  */
    INDEX = 383,                   /* INDEX  */
    INHERITS = 384,                /* INHERITS  */
    INITIALLY = 385,               /* INITIALLY  */
    INNER_P = 386,                 /* INNER_P  */
    INOUT = 387,                   /* INOUT  */
    INPUT_P = 388,                 /* INPUT_P  */
    INSENSITIVE = 389,             /* INSENSITIVE  */
    INSERT = 390,                  /* INSERT  */
    INSTEAD = 391,                 /* INSTEAD  */
    INT_P = 392,                   /* INT_P  */
    INTEGER = 393,                 /* INTEGER  */
    INTERSECT = 394,               /* INTERSECT  */
    INTERVAL = 395,                /* INTERVAL  */
    INTO = 396,                    /* INTO  */
    INVOKER = 397,                 /* INVOKER  */
    IS = 398,                      /* IS  */
    ISNULL = 399,                  /* ISNULL  */
    ISOLATION = 400,               /* ISOLATION  */
    JOIN = 401,                    /* JOIN  */
    KEY = 402,                     /* KEY  */
    LANCOMPILER = 403,             /* LANCOMPILER  */
    LANGUAGE = 404,                /* LANGUAGE  */
    LAST_P = 405,                  /* LAST_P  */
    LEADING = 406,                 /* LEADING  */
    LEFT = 407,                    /* LEFT  */
    LEVEL = 408,                   /* LEVEL  */
    LIKE = 409,                    /* LIKE  */
    LIMIT = 410,                   /* LIMIT  */
    LISTEN = 411,                  /* LISTEN  */
    LOAD = 412,                    /* LOAD  */
    LOCAL = 413,                   /* LOCAL  */
    LOCALTIME = 414,               /* LOCALTIME  */
    LOCALTIMESTAMP = 415,          /* LOCALTIMESTAMP  */
    LOCATION = 416,                /* LOCATION  */
    LOCK_P = 417,                  /* LOCK_P  */
    MATCH = 418,                   /* MATCH  */
    MAXVALUE = 419,                /* MAXVALUE  */
    MINUTE_P = 420,                /* MINUTE_P  */
    MINVALUE = 421,                /* MINVALUE  */
    MODE = 422,                    /* MODE  */
    MONTH_P = 423,                 /* MONTH_P  */
    MOVE = 424,                    /* MOVE  */
    NAMES = 425,                   /* NAMES  */
    NATIONAL = 426,                /* NATIONAL  */
    NATURAL = 427,                 /* NATURAL  */
    NCHAR = 428,                   /* NCHAR  */
    NEW = 429,                     /* NEW  */
    NEXT = 430,                    /* NEXT  */
    NO = 431,                      /* NO  */
    NOCREATEDB = 432,              /* NOCREATEDB  */
    NOCREATEUSER = 433,            /* NOCREATEUSER  */
    NONE = 434,                    /* NONE  */
    NOT = 435,                     /* NOT  */
    NOTHING = 436,                 /* NOTHING  */
    NOTIFY = 437,                  /* NOTIFY  */
    NOTNULL = 438,                 /* NOTNULL  */
    NULL_P = 439,                  /* NULL_P  */
    NULLIF = 440,                  /* NULLIF  */
    NUMERIC = 441,                 /* NUMERIC  */
    OF = 442,                      /* OF  */
    OFF = 443,                     /* OFF  */
    OFFSET = 444,                  /* OFFSET  */
    OIDS = 445,                    /* OIDS  */
    OLD = 446,                     /* OLD  */
    ON = 447,                      /* ON  */
    ONLY = 448,                    /* ONLY  */
    OPERATOR = 449,                /* OPERATOR  */
    OPTION = 450,                  /* OPTION  */
    OR = 451,                      /* OR  */
    ORDER = 452,                   /* ORDER  */
    OUT_P = 453,                   /* OUT_P  */
    OUTER_P = 454,                 /* OUTER_P  */
    OVERLAPS = 455,                /* OVERLAPS  */
    OVERLAY = 456,                 /* OVERLAY  */
    OWNER = 457,                   /* OWNER  */
    PARTIAL = 458,                 /* PARTIAL  */
    PASSWORD = 459,                /* PASSWORD  */
    PATH_P = 460,                  /* PATH_P  */
    PENDANT = 461,                 /* PENDANT  */
    PLACING = 462,                 /* PLACING  */
    POSITION = 463,                /* POSITION  */
    PRECISION = 464,               /* PRECISION  */
    PRESERVE = 465,                /* PRESERVE  */
    PREPARE = 466,                 /* PREPARE  */
    PRIMARY = 467,                 /* PRIMARY  */
    PRIOR = 468,                   /* PRIOR  */
    PRIVILEGES = 469,              /* PRIVILEGES  */
    PROCEDURAL = 470,              /* PROCEDURAL  */
    PROCEDURE = 471,               /* PROCEDURE  */
    READ = 472,                    /* READ  */
    REAL = 473,                    /* REAL  */
    RECHECK = 474,                 /* RECHECK  */
    REFERENCES = 475,              /* REFERENCES  */
    REINDEX = 476,                 /* REINDEX  */
    RELATIVE_P = 477,              /* RELATIVE_P  */
    RENAME = 478,                  /* RENAME  */
    REPLACE = 479,                 /* REPLACE  */
    RESET = 480,                   /* RESET  */
    RESTART = 481,                 /* RESTART  */
    RESTRICT = 482,                /* RESTRICT  */
    RETURNS = 483,                 /* RETURNS  */
    REVOKE = 484,                  /* REVOKE  */
    RIGHT = 485,                   /* RIGHT  */
    ROLLBACK = 486,                /* ROLLBACK  */
    ROW = 487,                     /* ROW  */
    ROWS = 488,                    /* ROWS  */
    RULE = 489,                    /* RULE  */
    SCHEMA = 490,                  /* SCHEMA  */
    SCROLL = 491,                  /* SCROLL  */
    SECOND_P = 492,                /* SECOND_P  */
    SECURITY = 493,                /* SECURITY  */
    SELECT = 494,                  /* SELECT  */
    SEQUENCE = 495,                /* SEQUENCE  */
    SERIALIZABLE = 496,            /* SERIALIZABLE  */
    SESSION = 497,                 /* SESSION  */
    SESSION_USER = 498,            /* SESSION_USER  */
    SET = 499,                     /* SET  */
    SETOF = 500,                   /* SETOF  */
    SHARE = 501,                   /* SHARE  */
    SHOW = 502,                    /* SHOW  */
    SIMILAR = 503,                 /* SIMILAR  */
    SIMPLE = 504,                  /* SIMPLE  */
    SMALLINT = 505,                /* SMALLINT  */
    SOME = 506,                    /* SOME  */
    STABLE = 507,                  /* STABLE  */
    START = 508,                   /* START  */
    STATEMENT = 509,               /* STATEMENT  */
    STATISTICS = 510,              /* STATISTICS  */
    STDIN = 511,                   /* STDIN  */
    STDOUT = 512,                  /* STDOUT  */
    STORAGE = 513,                 /* STORAGE  */
    STRICT_P = 514,                /* STRICT_P  */
    SUBSTRING = 515,               /* SUBSTRING  */
    SYSID = 516,                   /* SYSID  */
    TABLE = 517,                   /* TABLE  */
    TEMP = 518,                    /* TEMP  */
    TEMPLATE = 519,                /* TEMPLATE  */
    TEMPORARY = 520,               /* TEMPORARY  */
    THEN = 521,                    /* THEN  */
    TIME = 522,                    /* TIME  */
    TIMESTAMP = 523,               /* TIMESTAMP  */
    TO = 524,                      /* TO  */
    TOAST = 525,                   /* TOAST  */
    TRAILING = 526,                /* TRAILING  */
    TRANSACTION = 527,             /* TRANSACTION  */
    TREAT = 528,                   /* TREAT  */
    TRIGGER = 529,                 /* TRIGGER  */
    TRIM = 530,                    /* TRIM  */
    TRUE_P = 531,                  /* TRUE_P  */
    TRUNCATE = 532,                /* TRUNCATE  */
    TRUSTED = 533,                 /* TRUSTED  */
    TYPE_P = 534,                  /* TYPE_P  */
    UNENCRYPTED = 535,             /* UNENCRYPTED  */
    UNION = 536,                   /* UNION  */
    UNIQUE = 537,                  /* UNIQUE  */
    UNKNOWN = 538,                 /* UNKNOWN  */
    UNLISTEN = 539,                /* UNLISTEN  */
    UNTIL = 540,                   /* UNTIL  */
    UPDATE = 541,                  /* UPDATE  */
    USAGE = 542,                   /* USAGE  */
    USER = 543,                    /* USER  */
    USING = 544,                   /* USING  */
    VACUUM = 545,                  /* VACUUM  */
    VALID = 546,                   /* VALID  */
    VALIDATOR = 547,               /* VALIDATOR  */
    VALUES = 548,                  /* VALUES  */
    VARCHAR = 549,                 /* VARCHAR  */
    VARYING = 550,                 /* VARYING  */
    VERBOSE = 551,                 /* VERBOSE  */
    VERSION = 552,                 /* VERSION  */
    VIEW = 553,                    /* VIEW  */
    VOLATILE = 554,                /* VOLATILE  */
    WHEN = 555,                    /* WHEN  */
    WHERE = 556,                   /* WHERE  */
    WITH = 557,                    /* WITH  */
    WITHOUT = 558,                 /* WITHOUT  */
    WORK = 559,                    /* WORK  */
    WRITE = 560,                   /* WRITE  */
    YEAR_P = 561,                  /* YEAR_P  */
    ZONE = 562,                    /* ZONE  */
    UNIONJOIN = 563,               /* UNIONJOIN  */
    IDENT = 564,                   /* IDENT  */
    FCONST = 565,                  /* FCONST  */
    SCONST = 566,                  /* SCONST  */
    BCONST = 567,                  /* BCONST  */
    XCONST = 568,                  /* XCONST  */
    Op = 569,                      /* Op  */
    ICONST = 570,                  /* ICONST  */
    PARAM = 571,                   /* PARAM  */
    POSTFIXOP = 572,               /* POSTFIXOP  */
    UMINUS = 573,                  /* UMINUS  */
    TYPECAST = 574                 /* TYPECAST  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 66 "sql.y"

	int				ival;
	char				chr;
	char				*str;
	const char			*keyword;

#line 390 "sql.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SQL_H_INCLUDED  */
