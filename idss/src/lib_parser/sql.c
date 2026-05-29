/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
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



/* Line 268 of yacc.c  */
#line 133 "sql.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ABORT_P = 258,
     ABSOLUTE_P = 259,
     ACCESS = 260,
     ACTION = 261,
     ADD = 262,
     AFTER = 263,
     AGGREGATE = 264,
     ALL = 265,
     ALTER = 266,
     ANALYSE = 267,
     ANALYZE = 268,
     AND = 269,
     ANY = 270,
     ARRAY = 271,
     AS = 272,
     ASC = 273,
     ASSERTION = 274,
     ASSIGNMENT = 275,
     AT = 276,
     AUTHORIZATION = 277,
     BACKWARD = 278,
     BEFORE = 279,
     BEGIN_P = 280,
     BETWEEN = 281,
     BIGINT = 282,
     BINARY = 283,
     BIT = 284,
     BOOLEAN_P = 285,
     BOTH = 286,
     BY = 287,
     CACHE = 288,
     CALLED = 289,
     CASCADE = 290,
     CASE = 291,
     CAST = 292,
     CHAIN = 293,
     CHAR_P = 294,
     CHARACTER = 295,
     CHARACTERISTICS = 296,
     CHECK = 297,
     CHECKPOINT = 298,
     CLASS = 299,
     CLOSE = 300,
     CLUSTER = 301,
     COALESCE = 302,
     COLLATE = 303,
     COLUMN = 304,
     COMMENT = 305,
     COMMIT = 306,
     COMMITTED = 307,
     CONSTRAINT = 308,
     CONSTRAINTS = 309,
     CONVERSION_P = 310,
     CONVERT = 311,
     COPY = 312,
     CREATE = 313,
     CREATEDB = 314,
     CREATEUSER = 315,
     CROSS = 316,
     CURRENT_DATE = 317,
     CURRENT_TIME = 318,
     CURRENT_TIMESTAMP = 319,
     CURRENT_USER = 320,
     CURSOR = 321,
     CYCLE = 322,
     DATABASE = 323,
     DAY_P = 324,
     DEALLOCATE = 325,
     DEC = 326,
     DECIMAL_P = 327,
     DECLARE = 328,
     DEFAULT = 329,
     DEFAULTS = 330,
     DEFERRABLE = 331,
     DEFERRED = 332,
     DEFINER = 333,
     DELETE_P = 334,
     DELIMITER = 335,
     DELIMITERS = 336,
     DESC = 337,
     DISTINCT = 338,
     DO = 339,
     DOMAIN_P = 340,
     DOUBLE_P = 341,
     DROP = 342,
     EACH = 343,
     ELSE = 344,
     ENCODING = 345,
     ENCRYPTED = 346,
     END_P = 347,
     ESCAPE = 348,
     EXCEPT = 349,
     EXCLUDING = 350,
     EXCLUSIVE = 351,
     EXECUTE = 352,
     EXISTS = 353,
     EXPLAIN = 354,
     EXTERNAL = 355,
     EXTRACT = 356,
     FALSE_P = 357,
     FETCH = 358,
     FIRST_P = 359,
     FLOAT_P = 360,
     FOR = 361,
     FORCE = 362,
     FOREIGN = 363,
     FORWARD = 364,
     FREEZE = 365,
     FROM = 366,
     FULL = 367,
     FUNCTION = 368,
     GLOBAL = 369,
     GRANT = 370,
     GROUP_P = 371,
     HANDLER = 372,
     HAVING = 373,
     HOLD = 374,
     HOUR_P = 375,
     ILIKE = 376,
     IMMEDIATE = 377,
     IMMUTABLE = 378,
     IMPLICIT_P = 379,
     IN_P = 380,
     INCLUDING = 381,
     INCREMENT = 382,
     INDEX = 383,
     INHERITS = 384,
     INITIALLY = 385,
     INNER_P = 386,
     INOUT = 387,
     INPUT_P = 388,
     INSENSITIVE = 389,
     INSERT = 390,
     INSTEAD = 391,
     INT_P = 392,
     INTEGER = 393,
     INTERSECT = 394,
     INTERVAL = 395,
     INTO = 396,
     INVOKER = 397,
     IS = 398,
     ISNULL = 399,
     ISOLATION = 400,
     JOIN = 401,
     KEY = 402,
     LANCOMPILER = 403,
     LANGUAGE = 404,
     LAST_P = 405,
     LEADING = 406,
     LEFT = 407,
     LEVEL = 408,
     LIKE = 409,
     LIMIT = 410,
     LISTEN = 411,
     LOAD = 412,
     LOCAL = 413,
     LOCALTIME = 414,
     LOCALTIMESTAMP = 415,
     LOCATION = 416,
     LOCK_P = 417,
     MATCH = 418,
     MAXVALUE = 419,
     MINUTE_P = 420,
     MINVALUE = 421,
     MODE = 422,
     MONTH_P = 423,
     MOVE = 424,
     NAMES = 425,
     NATIONAL = 426,
     NATURAL = 427,
     NCHAR = 428,
     NEW = 429,
     NEXT = 430,
     NO = 431,
     NOCREATEDB = 432,
     NOCREATEUSER = 433,
     NONE = 434,
     NOT = 435,
     NOTHING = 436,
     NOTIFY = 437,
     NOTNULL = 438,
     NULL_P = 439,
     NULLIF = 440,
     NUMERIC = 441,
     OF = 442,
     OFF = 443,
     OFFSET = 444,
     OIDS = 445,
     OLD = 446,
     ON = 447,
     ONLY = 448,
     OPERATOR = 449,
     OPTION = 450,
     OR = 451,
     ORDER = 452,
     OUT_P = 453,
     OUTER_P = 454,
     OVERLAPS = 455,
     OVERLAY = 456,
     OWNER = 457,
     PARTIAL = 458,
     PASSWORD = 459,
     PATH_P = 460,
     PENDANT = 461,
     PLACING = 462,
     POSITION = 463,
     PRECISION = 464,
     PRESERVE = 465,
     PREPARE = 466,
     PRIMARY = 467,
     PRIOR = 468,
     PRIVILEGES = 469,
     PROCEDURAL = 470,
     PROCEDURE = 471,
     READ = 472,
     REAL = 473,
     RECHECK = 474,
     REFERENCES = 475,
     REINDEX = 476,
     RELATIVE_P = 477,
     RENAME = 478,
     REPLACE = 479,
     RESET = 480,
     RESTART = 481,
     RESTRICT = 482,
     RETURNS = 483,
     REVOKE = 484,
     RIGHT = 485,
     ROLLBACK = 486,
     ROW = 487,
     ROWS = 488,
     RULE = 489,
     SCHEMA = 490,
     SCROLL = 491,
     SECOND_P = 492,
     SECURITY = 493,
     SELECT = 494,
     SEQUENCE = 495,
     SERIALIZABLE = 496,
     SESSION = 497,
     SESSION_USER = 498,
     SET = 499,
     SETOF = 500,
     SHARE = 501,
     SHOW = 502,
     SIMILAR = 503,
     SIMPLE = 504,
     SMALLINT = 505,
     SOME = 506,
     STABLE = 507,
     START = 508,
     STATEMENT = 509,
     STATISTICS = 510,
     STDIN = 511,
     STDOUT = 512,
     STORAGE = 513,
     STRICT_P = 514,
     SUBSTRING = 515,
     SYSID = 516,
     TABLE = 517,
     TEMP = 518,
     TEMPLATE = 519,
     TEMPORARY = 520,
     THEN = 521,
     TIME = 522,
     TIMESTAMP = 523,
     TO = 524,
     TOAST = 525,
     TRAILING = 526,
     TRANSACTION = 527,
     TREAT = 528,
     TRIGGER = 529,
     TRIM = 530,
     TRUE_P = 531,
     TRUNCATE = 532,
     TRUSTED = 533,
     TYPE_P = 534,
     UNENCRYPTED = 535,
     UNION = 536,
     UNIQUE = 537,
     UNKNOWN = 538,
     UNLISTEN = 539,
     UNTIL = 540,
     UPDATE = 541,
     USAGE = 542,
     USER = 543,
     USING = 544,
     VACUUM = 545,
     VALID = 546,
     VALIDATOR = 547,
     VALUES = 548,
     VARCHAR = 549,
     VARYING = 550,
     VERBOSE = 551,
     VERSION = 552,
     VIEW = 553,
     VOLATILE = 554,
     WHEN = 555,
     WHERE = 556,
     WITH = 557,
     WITHOUT = 558,
     WORK = 559,
     WRITE = 560,
     YEAR_P = 561,
     ZONE = 562,
     UNIONJOIN = 563,
     IDENT = 564,
     FCONST = 565,
     SCONST = 566,
     BCONST = 567,
     XCONST = 568,
     Op = 569,
     ICONST = 570,
     PARAM = 571,
     POSTFIXOP = 572,
     UMINUS = 573,
     TYPECAST = 574
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 64 "sql.y"

	int				ival;
	char				chr;
	char				*str;
	const char			*keyword;



/* Line 293 of yacc.c  */
#line 497 "sql.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 509 "sql.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
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
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1149

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   574

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    13,    15,    19,    23,
      25,    28,    33,    38,    40,    42,    51,    56,    61,    66,
      69,    70,    74,    78,    83,    88,    93,    98,   101,   103,
     105,   107,   109,   110,   112,   114,   115,   117,   123,   125,
     126,   128,   129,   133,   135,   139,   143,   146,   149,   151,
     156,   161,   164,   167,   172,   174,   175,   177,   179,   181,
     185,   186,   189,   190,   194,   198,   200,   201,   204,   205,
     208,   209,   211,   215,   217,   220,   222,   225,   231,   238,
     244,   246,   249,   251,   256,   260,   265,   269,   275,   280,
     286,   291,   297,   300,   305,   307,   310,   313,   316,   318,
     320,   321,   326,   329,   331,   334,   337,   342,   346,   351,
     354,   355,   357,   361,   364,   367,   371,   377,   384,   388,
     393,   394,   396,   398,   400,   402,   404,   407,   413,   416,
     418,   420,   422,   424,   426,   428,   430,   432,   434,   436,
     438,   441,   444,   447,   450,   453,   455,   459,   460,   466,
     470,   471,   477,   481,   482,   484,   486,   488,   490,   496,
     499,   501,   503,   505,   507,   513,   516,   519,   522,   524,
     528,   532,   535,   537,   538,   542,   543,   549,   552,   558,
     561,   563,   567,   571,   572,   574,   576,   578,   580,   582,
     584,   588,   592,   596,   600,   604,   608,   612,   613,   617,
     622,   627,   631,   635,   639,   644,   648,   654,   659,   664,
     668,   672,   676,   678,   680,   682,   684,   686,   688,   690,
     692,   694,   696,   698,   700,   702,   704,   706,   711,   713,
     717,   719,   724,   726,   730,   736,   739,   742,   745,   748,
     751,   754,   758,   762,   766,   770,   774,   778,   782,   786,
     790,   794,   797,   800,   804,   808,   811,   815,   821,   826,
     833,   837,   843,   848,   855,   860,   867,   873,   881,   884,
     888,   891,   896,   900,   905,   909,   914,   918,   923,   929,
     936,   944,   950,   957,   961,   966,   971,   978,   981,   983,
     985,   989,   992,   995,   998,  1001,  1004,  1007,  1011,  1015,
    1019,  1023,  1027,  1031,  1035,  1039,  1043,  1047,  1050,  1053,
    1059,  1066,  1074,  1076,  1078,  1082,  1088,  1093,  1095,  1099,
    1104,  1110,  1116,  1121,  1123,  1125,  1130,  1132,  1137,  1139,
    1144,  1146,  1151,  1153,  1155,  1157,  1164,  1169,  1174,  1179,
    1184,  1191,  1197,  1203,  1209,  1214,  1221,  1226,  1228,  1231,
    1234,  1237,  1239,  1241,  1246,  1253,  1254,  1256,  1260,  1264,
    1265,  1269,  1271,  1273,  1277,  1281,  1285,  1287,  1289,  1291,
    1293,  1295,  1297,  1299,  1301,  1306,  1310,  1313,  1317,  1318,
    1322,  1326,  1329,  1332,  1334,  1335,  1338,  1341,  1345,  1348,
    1350,  1352,  1356,  1362,  1369,  1374,  1376,  1379,  1384,  1387,
    1388,  1390,  1391,  1394,  1397,  1400,  1403,  1406,  1410,  1412,
    1416,  1420,  1422,  1424,  1426,  1428,  1430,  1434,  1436,  1438,
    1440,  1442,  1444,  1446,  1448,  1450,  1452,  1455,  1459,  1466,
    1469,  1471,  1473,  1475,  1477,  1479,  1481,  1483,  1485,  1487,
    1489,  1491,  1493,  1495,  1497,  1499,  1501,  1503,  1505,  1507,
    1509,  1511,  1513,  1515,  1517,  1519,  1521,  1523,  1525,  1527,
    1529,  1531,  1533,  1535,  1537,  1539,  1541,  1543,  1545,  1547,
    1549,  1551,  1553,  1555,  1557,  1559,  1561,  1563,  1565,  1567,
    1569,  1571,  1573,  1575,  1577,  1579,  1581,  1583,  1585,  1587,
    1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,
    1609,  1611,  1613,  1615,  1617,  1619,  1621,  1623,  1625,  1627,
    1629,  1631,  1633,  1635,  1637,  1639,  1641,  1643,  1645,  1647,
    1649,  1651,  1653,  1655,  1657,  1659,  1661,  1663,  1665,  1667,
    1669,  1671,  1673,  1675,  1677,  1679,  1681,  1683,  1685,  1687,
    1689,  1691,  1693,  1695,  1697,  1699,  1701,  1703,  1705,  1707,
    1709,  1711,  1713,  1715,  1717,  1719,  1721,  1723,  1725,  1727,
    1729,  1731,  1733,  1735,  1737,  1739,  1741,  1743,  1745,  1747,
    1749,  1751,  1753,  1755,  1757,  1759,  1761,  1763,  1765,  1767,
    1769,  1771,  1773,  1775,  1777,  1779,  1781,  1783,  1785,  1787,
    1789,  1791,  1793,  1795,  1797,  1799,  1801,  1803,  1805,  1807,
    1809,  1811,  1813,  1815,  1817,  1819,  1821,  1823,  1825,  1827,
    1829,  1831,  1833,  1835,  1837,  1839,  1841,  1843,  1845,  1847,
    1849,  1851,  1853,  1855,  1857,  1859,  1861,  1863,  1865,  1867,
    1869,  1871,  1873,  1875,  1877,  1879,  1881,  1883,  1885,  1887,
    1889,  1891,  1893,  1895,  1897,  1899,  1901,  1903,  1905,  1907,
    1909,  1911,  1913,  1915,  1917,  1919,  1921,  1923,  1925,  1927,
    1929,  1931,  1933,  1935,  1937,  1939,  1941,  1943,  1945,  1947,
    1949,  1951,  1953,  1955,  1957,  1959,  1961,  1963,  1965,  1967,
    1969,  1971,  1973,  1975,  1977,  1979,  1981,  1983,  1985,  1987,
    1989,  1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,  2007,
    2009,  2011,  2013,  2015,  2017,  2019,  2021,  2023,  2025,  2027,
    2029,  2031,  2033,  2035,  2037,  2039,  2041,  2043,  2045,  2047,
    2049,  2051,  2053,  2055,  2057,  2059,  2061,  2063,  2065,  2067,
    2069,  2071,  2073,  2075,  2077,  2079,  2081,  2083,  2085,  2087,
    2089,  2091,  2093,  2095,  2097,  2099,  2101,  2103,  2105,  2107,
    2109,  2111,  2113,  2115,  2117
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     338,     0,    -1,   339,    -1,   338,   339,    -1,   340,   334,
      -1,   342,    -1,   341,    -1,   330,   342,   331,    -1,   330,
     341,   331,    -1,   344,    -1,   343,   352,    -1,   343,   351,
     361,   356,    -1,   343,   351,   355,   362,    -1,   344,    -1,
     341,    -1,   239,   350,   437,   345,   364,   374,   359,   360,
      -1,   343,   281,   349,   343,    -1,   343,   139,   349,   343,
      -1,   343,    94,   349,   343,    -1,   141,   346,    -1,    -1,
     265,   348,   347,    -1,   263,   348,   347,    -1,   158,   265,
     348,   347,    -1,   158,   263,   348,   347,    -1,   114,   265,
     348,   347,    -1,   114,   263,   348,   347,    -1,   262,   347,
      -1,   347,    -1,   439,    -1,   435,    -1,   262,    -1,    -1,
      10,    -1,    83,    -1,    -1,    83,    -1,    83,   192,   330,
     415,   331,    -1,    10,    -1,    -1,   352,    -1,    -1,   197,
      32,   353,    -1,   354,    -1,   353,   335,   354,    -1,   410,
     289,   409,    -1,   410,    18,    -1,   410,    82,    -1,   410,
      -1,   155,   357,   189,   358,    -1,   189,   358,   155,   357,
      -1,   155,   357,    -1,   189,   358,    -1,   155,   357,   335,
     358,    -1,   355,    -1,    -1,   410,    -1,    10,    -1,   410,
      -1,   116,    32,   415,    -1,    -1,   118,   410,    -1,    -1,
     106,   286,   363,    -1,   106,   217,   193,    -1,   361,    -1,
      -1,   187,   440,    -1,    -1,   111,   365,    -1,    -1,   366,
      -1,   365,   335,   366,    -1,   372,    -1,   372,   368,    -1,
     373,    -1,   373,   368,    -1,   373,    17,   330,   375,   331,
      -1,   373,    17,   447,   330,   375,   331,    -1,   373,   447,
     330,   375,   331,    -1,   341,    -1,   341,   368,    -1,   367,
      -1,   330,   367,   331,   368,    -1,   330,   367,   331,    -1,
     366,    61,   146,   366,    -1,   366,   308,   366,    -1,   366,
     369,   146,   366,   371,    -1,   366,   146,   366,   371,    -1,
     366,   172,   369,   146,   366,    -1,   366,   172,   146,   366,
      -1,    17,   447,   330,   440,   331,    -1,    17,   447,    -1,
     447,   330,   440,   331,    -1,   447,    -1,   112,   370,    -1,
     152,   370,    -1,   230,   370,    -1,   131,    -1,   199,    -1,
      -1,   289,   330,   440,   331,    -1,   192,   410,    -1,   347,
      -1,   347,   323,    -1,   193,   347,    -1,   193,   330,   347,
     331,    -1,   443,   330,   331,    -1,   443,   330,   415,   331,
      -1,   301,   410,    -1,    -1,   376,    -1,   375,   335,   376,
      -1,   447,   377,    -1,   379,   378,    -1,   245,   379,   378,
      -1,   379,    16,   328,   445,   329,    -1,   245,   379,    16,
     328,   445,   329,    -1,   378,   328,   329,    -1,   378,   328,
     445,   329,    -1,    -1,   381,    -1,   382,    -1,   386,    -1,
     390,    -1,   397,    -1,   398,   400,    -1,   398,   330,   445,
     331,   400,    -1,   448,   436,    -1,   381,    -1,   382,    -1,
     387,    -1,   391,    -1,   397,    -1,   448,    -1,   137,    -1,
     138,    -1,   250,    -1,    27,    -1,   218,    -1,   105,   383,
      -1,    86,   209,    -1,    72,   385,    -1,    71,   385,    -1,
     186,   384,    -1,    30,    -1,   330,   445,   331,    -1,    -1,
     330,   445,   335,   445,   331,    -1,   330,   445,   331,    -1,
      -1,   330,   445,   335,   445,   331,    -1,   330,   445,   331,
      -1,    -1,   388,    -1,   389,    -1,   388,    -1,   389,    -1,
      29,   395,   330,   445,   331,    -1,    29,   395,    -1,   392,
      -1,   393,    -1,   392,    -1,   393,    -1,   394,   330,   445,
     331,   396,    -1,   394,   396,    -1,    40,   395,    -1,    39,
     395,    -1,   294,    -1,   171,    40,   395,    -1,   171,    39,
     395,    -1,   173,   395,    -1,   295,    -1,    -1,    40,   244,
     447,    -1,    -1,   268,   330,   445,   331,   399,    -1,   268,
     399,    -1,   267,   330,   445,   331,   399,    -1,   267,   399,
      -1,   140,    -1,   302,   267,   307,    -1,   303,   267,   307,
      -1,    -1,   306,    -1,   168,    -1,    69,    -1,   120,    -1,
     165,    -1,   237,    -1,   306,   269,   168,    -1,    69,   269,
     120,    -1,    69,   269,   165,    -1,    69,   269,   237,    -1,
     120,   269,   165,    -1,   120,   269,   237,    -1,   165,   269,
     237,    -1,    -1,   402,   125,   341,    -1,   402,   180,   125,
     341,    -1,   402,   409,   404,   341,    -1,   402,   409,   341,
      -1,   402,   409,   402,    -1,   402,   143,   184,    -1,   402,
     143,   180,   184,    -1,   402,   200,   402,    -1,   402,   143,
      83,   111,   402,    -1,   232,   330,   403,   331,    -1,   232,
     330,   410,   331,    -1,   232,   330,   331,    -1,   330,   403,
     331,    -1,   415,   335,   410,    -1,    15,    -1,   251,    -1,
      10,    -1,   314,    -1,   406,    -1,   321,    -1,   322,    -1,
     323,    -1,   324,    -1,   325,    -1,   326,    -1,   318,    -1,
     319,    -1,   317,    -1,   314,    -1,   194,   330,   408,   331,
      -1,   405,    -1,   447,   333,   408,    -1,   405,    -1,   194,
     330,   408,   331,    -1,   412,    -1,   410,   332,   377,    -1,
     410,    21,   267,   307,   412,    -1,   321,   410,    -1,   322,
     410,    -1,   325,   410,    -1,   326,   410,    -1,   410,   325,
      -1,   410,   326,    -1,   410,   321,   410,    -1,   410,   322,
     410,    -1,   410,   323,   410,    -1,   410,   324,   410,    -1,
     410,   325,   410,    -1,   410,   326,   410,    -1,   410,   318,
     410,    -1,   410,   319,   410,    -1,   410,   317,   410,    -1,
     410,   407,   410,    -1,   407,   410,    -1,   410,   407,    -1,
     410,    14,   410,    -1,   410,   196,   410,    -1,   180,   410,
      -1,   410,   154,   410,    -1,   410,   154,   410,    93,   410,
      -1,   410,   180,   154,   410,    -1,   410,   180,   154,   410,
      93,   410,    -1,   410,   121,   410,    -1,   410,   121,   410,
      93,   410,    -1,   410,   180,   121,   410,    -1,   410,   180,
     121,   410,    93,   410,    -1,   410,   248,   269,   410,    -1,
     410,   248,   269,   410,    93,   410,    -1,   410,   180,   248,
     269,   410,    -1,   410,   180,   248,   269,   410,    93,   410,
      -1,   410,   144,    -1,   410,   143,   184,    -1,   410,   183,
      -1,   410,   143,   180,   184,    -1,   410,   143,   276,    -1,
     410,   143,   180,   276,    -1,   410,   143,   102,    -1,   410,
     143,   180,   102,    -1,   410,   143,   283,    -1,   410,   143,
     180,   283,    -1,   410,   143,    83,   111,   410,    -1,   410,
     143,   187,   330,   417,   331,    -1,   410,   143,   180,   187,
     330,   417,   331,    -1,   410,    26,   411,    14,   411,    -1,
     410,   180,    26,   411,    14,   411,    -1,   410,   125,   428,
      -1,   410,   180,   125,   428,    -1,   410,   409,   404,   341,
      -1,   410,   409,   404,   330,   410,   331,    -1,   282,   341,
      -1,   401,    -1,   412,    -1,   411,   332,   377,    -1,   321,
     411,    -1,   322,   411,    -1,   325,   411,    -1,   326,   411,
      -1,   411,   325,    -1,   411,   326,    -1,   411,   321,   411,
      -1,   411,   322,   411,    -1,   411,   323,   411,    -1,   411,
     324,   411,    -1,   411,   325,   411,    -1,   411,   326,   411,
      -1,   411,   318,   411,    -1,   411,   319,   411,    -1,   411,
     317,   411,    -1,   411,   407,   411,    -1,   407,   411,    -1,
     411,   407,    -1,   411,   143,    83,   111,   411,    -1,   411,
     143,   187,   330,   417,   331,    -1,   411,   143,   180,   187,
     330,   417,   331,    -1,   434,    -1,   444,    -1,   316,   436,
     414,    -1,   330,   410,   331,   436,   414,    -1,   330,   410,
     331,   414,    -1,   429,    -1,   443,   330,   331,    -1,   443,
     330,   415,   331,    -1,   443,   330,    10,   415,   331,    -1,
     443,   330,    83,   415,   331,    -1,   443,   330,   323,   331,
      -1,    62,    -1,    63,    -1,    63,   330,   445,   331,    -1,
      64,    -1,    64,   330,   445,   331,    -1,   159,    -1,   159,
     330,   445,   331,    -1,   160,    -1,   160,   330,   445,   331,
      -1,    65,    -1,   243,    -1,   288,    -1,    37,   330,   410,
      17,   377,   331,    -1,   101,   330,   416,   331,    -1,   201,
     330,   421,   331,    -1,   208,   330,   423,   331,    -1,   260,
     330,   424,   331,    -1,   273,   330,   410,    17,   377,   331,
      -1,   275,   330,    31,   427,   331,    -1,   275,   330,   151,
     427,   331,    -1,   275,   330,   271,   427,   331,    -1,   275,
     330,   427,   331,    -1,    56,   330,   410,   289,   413,   331,
      -1,    56,   330,   415,   331,    -1,   341,    -1,    98,   341,
      -1,    16,   341,    -1,    16,   419,    -1,   447,    -1,   435,
      -1,   414,   328,   410,   329,    -1,   414,   328,   410,   336,
     410,   329,    -1,    -1,   410,    -1,   415,   335,   410,    -1,
     420,   111,   410,    -1,    -1,   417,   335,   377,    -1,   377,
      -1,   419,    -1,   418,   335,   419,    -1,   328,   415,   329,
      -1,   328,   418,   329,    -1,   309,    -1,   306,    -1,   168,
      -1,    69,    -1,   120,    -1,   165,    -1,   237,    -1,   311,
      -1,   410,   422,   425,   426,    -1,   410,   422,   425,    -1,
     207,   410,    -1,   411,   125,   411,    -1,    -1,   410,   425,
     426,    -1,   410,   426,   425,    -1,   410,   425,    -1,   410,
     426,    -1,   415,    -1,    -1,   111,   410,    -1,   106,   410,
      -1,   410,   111,   415,    -1,   111,   415,    -1,   415,    -1,
     341,    -1,   330,   415,   331,    -1,    36,   433,   430,   432,
      92,    -1,   185,   330,   410,   335,   410,   331,    -1,    47,
     330,   415,   331,    -1,   431,    -1,   430,   431,    -1,   300,
     410,   266,   410,    -1,    89,   410,    -1,    -1,   410,    -1,
      -1,   439,   414,    -1,   435,   414,    -1,   439,   436,    -1,
     333,   442,    -1,   333,   323,    -1,   333,   442,   436,    -1,
     438,    -1,   437,   335,   438,    -1,   410,    17,   450,    -1,
     410,    -1,   323,    -1,   455,    -1,   447,    -1,   441,    -1,
     440,   335,   441,    -1,   447,    -1,   447,    -1,   449,    -1,
     435,    -1,   445,    -1,   310,    -1,   446,    -1,   312,    -1,
     313,    -1,   380,   446,    -1,   398,   446,   400,    -1,   398,
     330,   445,   331,   446,   400,    -1,   316,   414,    -1,   276,
      -1,   102,    -1,   184,    -1,   315,    -1,   311,    -1,   309,
      -1,   451,    -1,   452,    -1,   309,    -1,   451,    -1,   309,
      -1,   451,    -1,   453,    -1,   309,    -1,   451,    -1,   452,
      -1,   453,    -1,   454,    -1,     3,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    11,
      -1,    19,    -1,    20,    -1,    21,    -1,    23,    -1,    24,
      -1,    25,    -1,    32,    -1,    33,    -1,    34,    -1,    35,
      -1,    38,    -1,    41,    -1,    43,    -1,    44,    -1,    45,
      -1,    46,    -1,    50,    -1,    51,    -1,    52,    -1,    54,
      -1,    55,    -1,    57,    -1,    59,    -1,    60,    -1,    66,
      -1,    67,    -1,    68,    -1,    69,    -1,    70,    -1,    73,
      -1,    75,    -1,    77,    -1,    78,    -1,    79,    -1,    80,
      -1,    81,    -1,    85,    -1,    86,    -1,    87,    -1,    88,
      -1,    90,    -1,    91,    -1,    93,    -1,    95,    -1,    96,
      -1,    97,    -1,    99,    -1,   100,    -1,   103,    -1,   104,
      -1,   107,    -1,   109,    -1,   113,    -1,   114,    -1,   117,
      -1,   119,    -1,   120,    -1,   122,    -1,   123,    -1,   124,
      -1,   126,    -1,   127,    -1,   128,    -1,   129,    -1,   132,
      -1,   133,    -1,   134,    -1,   135,    -1,   136,    -1,   142,
      -1,   145,    -1,   147,    -1,   148,    -1,   149,    -1,   150,
      -1,   153,    -1,   156,    -1,   157,    -1,   158,    -1,   161,
      -1,   162,    -1,   163,    -1,   164,    -1,   165,    -1,   166,
      -1,   167,    -1,   168,    -1,   169,    -1,   170,    -1,   171,
      -1,   175,    -1,   176,    -1,   177,    -1,   178,    -1,   181,
      -1,   182,    -1,   187,    -1,   190,    -1,   194,    -1,   195,
      -1,   198,    -1,   202,    -1,   203,    -1,   204,    -1,   205,
      -1,   206,    -1,   209,    -1,   211,    -1,   210,    -1,   213,
      -1,   214,    -1,   215,    -1,   216,    -1,   217,    -1,   219,
      -1,   221,    -1,   222,    -1,   223,    -1,   224,    -1,   225,
      -1,   226,    -1,   227,    -1,   228,    -1,   229,    -1,   231,
      -1,   233,    -1,   234,    -1,   235,    -1,   236,    -1,   237,
      -1,   238,    -1,   240,    -1,   241,    -1,   242,    -1,   244,
      -1,   246,    -1,   247,    -1,   249,    -1,   252,    -1,   253,
      -1,   254,    -1,   255,    -1,   256,    -1,   257,    -1,   258,
      -1,   261,    -1,   259,    -1,   263,    -1,   264,    -1,   265,
      -1,   270,    -1,   272,    -1,   274,    -1,   277,    -1,   278,
      -1,   279,    -1,   280,    -1,   283,    -1,   284,    -1,   285,
      -1,   286,    -1,   287,    -1,   290,    -1,   291,    -1,   292,
      -1,   293,    -1,   295,    -1,   297,    -1,   298,    -1,   299,
      -1,   302,    -1,   303,    -1,   304,    -1,   305,    -1,   306,
      -1,   307,    -1,    27,    -1,    29,    -1,    30,    -1,    39,
      -1,    40,    -1,    47,    -1,    56,    -1,    71,    -1,    72,
      -1,    98,    -1,   101,    -1,   105,    -1,   137,    -1,   138,
      -1,   140,    -1,   173,    -1,   179,    -1,   185,    -1,   186,
      -1,   201,    -1,   208,    -1,   218,    -1,   232,    -1,   245,
      -1,   250,    -1,   260,    -1,   267,    -1,   268,    -1,   273,
      -1,   275,    -1,   294,    -1,    22,    -1,    26,    -1,    28,
      -1,    61,    -1,   110,    -1,   112,    -1,   121,    -1,   125,
      -1,   131,    -1,   143,    -1,   144,    -1,   146,    -1,   152,
      -1,   154,    -1,   172,    -1,   183,    -1,   199,    -1,   200,
      -1,   230,    -1,   248,    -1,   296,    -1,    10,    -1,    12,
      -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,    18,    -1,    31,    -1,    36,    -1,    37,    -1,    42,
      -1,    48,    -1,    49,    -1,    53,    -1,    58,    -1,    62,
      -1,    63,    -1,    64,    -1,    65,    -1,    74,    -1,    76,
      -1,    82,    -1,    83,    -1,    84,    -1,    89,    -1,    92,
      -1,    94,    -1,   102,    -1,   106,    -1,   108,    -1,   111,
      -1,   115,    -1,   116,    -1,   118,    -1,   130,    -1,   139,
      -1,   141,    -1,   151,    -1,   155,    -1,   159,    -1,   160,
      -1,   174,    -1,   180,    -1,   184,    -1,   188,    -1,   189,
      -1,   191,    -1,   192,    -1,   193,    -1,   196,    -1,   197,
      -1,   207,    -1,   212,    -1,   220,    -1,   239,    -1,   243,
      -1,   251,    -1,   262,    -1,   266,    -1,   269,    -1,   271,
      -1,   276,    -1,   281,    -1,   282,    -1,   288,    -1,   289,
      -1,   300,    -1,   301,    -1,   191,    -1,   174,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   216,   216,   217,   221,   269,   270,   274,   275,   285,
     286,   288,   290,   295,   296,   323,   327,   329,   331,   336,
     337,   345,   347,   349,   351,   353,   355,   357,   359,   365,
     366,   369,   370,   373,   374,   375,   382,   383,   384,   385,
     389,   390,   394,   398,   399,   402,   404,   406,   408,   414,
     416,   418,   420,   422,   427,   428,   433,   434,   439,   451,
     452,   456,   457,   461,   462,   466,   467,   471,   472,   484,
     485,   489,   490,   500,   502,   504,   506,   508,   510,   512,
     514,   529,   531,   533,   556,   558,   560,   562,   564,   566,
     568,   573,   575,   577,   579,   583,   584,   585,   586,   590,
     591,   603,   604,   609,   611,   613,   615,   620,   622,   628,
     629,   634,   636,   640,   654,   656,   658,   660,   665,   667,
     669,   682,   683,   684,   685,   686,   687,   689,   691,   705,
     706,   707,   708,   709,   713,   722,   724,   726,   728,   730,
     732,   734,   736,   738,   740,   742,   746,   748,   753,   755,
     757,   762,   764,   766,   775,   777,   783,   785,   790,   795,
     804,   806,   810,   812,   816,   820,   824,   826,   828,   830,
     832,   834,   839,   840,   844,   845,   849,   851,   853,   855,
     860,   864,   865,   866,   870,   871,   872,   873,   874,   875,
     876,   878,   880,   882,   884,   886,   888,   890,   905,   907,
     909,   912,   915,   918,   920,   922,   924,   933,   934,   935,
     936,   939,   942,   943,   944,   947,   948,   951,   952,   953,
     954,   955,   956,   957,   958,   959,   962,   964,   968,   969,
     973,   975,   994,   995,   997,  1008,  1010,  1012,  1014,  1016,
    1018,  1020,  1022,  1024,  1026,  1028,  1030,  1032,  1034,  1036,
    1039,  1041,  1043,  1046,  1048,  1050,  1053,  1055,  1057,  1059,
    1061,  1063,  1065,  1067,  1070,  1072,  1074,  1076,  1088,  1090,
    1092,  1094,  1096,  1098,  1100,  1102,  1104,  1106,  1108,  1110,
    1112,  1114,  1116,  1118,  1120,  1122,  1124,  1126,  1128,  1141,
    1143,  1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,
    1163,  1165,  1167,  1169,  1171,  1173,  1175,  1177,  1179,  1181,
    1183,  1185,  1197,  1198,  1199,  1201,  1203,  1205,  1207,  1209,
    1211,  1213,  1215,  1217,  1219,  1221,  1223,  1225,  1227,  1229,
    1231,  1233,  1235,  1237,  1239,  1241,  1243,  1245,  1247,  1249,
    1251,  1253,  1255,  1257,  1259,  1261,  1263,  1265,  1267,  1269,
    1271,  1275,  1276,  1284,  1286,  1288,  1292,  1294,  1299,  1301,
    1304,  1306,  1310,  1312,  1316,  1318,  1327,  1328,  1329,  1330,
    1331,  1332,  1333,  1334,  1343,  1345,  1350,  1357,  1358,  1373,
    1375,  1377,  1379,  1381,  1383,  1388,  1391,  1394,  1395,  1396,
    1399,  1401,  1423,  1425,  1427,  1433,  1434,  1438,  1443,  1444,
    1447,  1448,  1456,  1458,  1463,  1467,  1469,  1471,  1485,  1486,
    1490,  1492,  1494,  1513,  1514,  1519,  1521,  1526,  1529,  1531,
    1533,  1540,  1542,  1544,  1546,  1548,  1550,  1552,  1554,  1556,
    1558,  1560,  1562,  1566,  1567,  1582,  1583,  1584,  1589,  1590,
    1596,  1597,  1598,  1604,  1605,  1606,  1607,  1608,  1624,  1625,
    1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,
    1636,  1637,  1638,  1639,  1640,  1641,  1642,  1643,  1644,  1645,
    1646,  1647,  1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,
    1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,
    1666,  1667,  1668,  1669,  1670,  1671,  1672,  1673,  1674,  1675,
    1676,  1677,  1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,
    1686,  1687,  1688,  1689,  1690,  1691,  1692,  1693,  1694,  1695,
    1696,  1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,
    1706,  1707,  1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,
    1716,  1717,  1718,  1719,  1720,  1721,  1722,  1723,  1724,  1725,
    1726,  1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,
    1736,  1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,  1745,
    1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,  1755,
    1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,  1764,  1765,
    1766,  1767,  1768,  1769,  1770,  1771,  1772,  1773,  1774,  1775,
    1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,
    1786,  1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,
    1796,  1797,  1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,
    1806,  1807,  1821,  1822,  1823,  1824,  1825,  1826,  1827,  1828,
    1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,
    1849,  1850,  1851,  1865,  1866,  1867,  1868,  1869,  1870,  1871,
    1872,  1873,  1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,
    1882,  1883,  1884,  1885,  1895,  1896,  1897,  1898,  1899,  1900,
    1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,
    1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,
    1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,  1940,
    1941,  1942,  1943,  1944,  1945,  1946,  1947,  1948,  1949,  1950,
    1951,  1952,  1953,  1954,  1955,  1956,  1957,  1958,  1959,  1960,
    1961,  1962,  1963,  1968,  1970
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ABORT_P", "ABSOLUTE_P", "ACCESS",
  "ACTION", "ADD", "AFTER", "AGGREGATE", "ALL", "ALTER", "ANALYSE",
  "ANALYZE", "AND", "ANY", "ARRAY", "AS", "ASC", "ASSERTION", "ASSIGNMENT",
  "AT", "AUTHORIZATION", "BACKWARD", "BEFORE", "BEGIN_P", "BETWEEN",
  "BIGINT", "BINARY", "BIT", "BOOLEAN_P", "BOTH", "BY", "CACHE", "CALLED",
  "CASCADE", "CASE", "CAST", "CHAIN", "CHAR_P", "CHARACTER",
  "CHARACTERISTICS", "CHECK", "CHECKPOINT", "CLASS", "CLOSE", "CLUSTER",
  "COALESCE", "COLLATE", "COLUMN", "COMMENT", "COMMIT", "COMMITTED",
  "CONSTRAINT", "CONSTRAINTS", "CONVERSION_P", "CONVERT", "COPY", "CREATE",
  "CREATEDB", "CREATEUSER", "CROSS", "CURRENT_DATE", "CURRENT_TIME",
  "CURRENT_TIMESTAMP", "CURRENT_USER", "CURSOR", "CYCLE", "DATABASE",
  "DAY_P", "DEALLOCATE", "DEC", "DECIMAL_P", "DECLARE", "DEFAULT",
  "DEFAULTS", "DEFERRABLE", "DEFERRED", "DEFINER", "DELETE_P", "DELIMITER",
  "DELIMITERS", "DESC", "DISTINCT", "DO", "DOMAIN_P", "DOUBLE_P", "DROP",
  "EACH", "ELSE", "ENCODING", "ENCRYPTED", "END_P", "ESCAPE", "EXCEPT",
  "EXCLUDING", "EXCLUSIVE", "EXECUTE", "EXISTS", "EXPLAIN", "EXTERNAL",
  "EXTRACT", "FALSE_P", "FETCH", "FIRST_P", "FLOAT_P", "FOR", "FORCE",
  "FOREIGN", "FORWARD", "FREEZE", "FROM", "FULL", "FUNCTION", "GLOBAL",
  "GRANT", "GROUP_P", "HANDLER", "HAVING", "HOLD", "HOUR_P", "ILIKE",
  "IMMEDIATE", "IMMUTABLE", "IMPLICIT_P", "IN_P", "INCLUDING", "INCREMENT",
  "INDEX", "INHERITS", "INITIALLY", "INNER_P", "INOUT", "INPUT_P",
  "INSENSITIVE", "INSERT", "INSTEAD", "INT_P", "INTEGER", "INTERSECT",
  "INTERVAL", "INTO", "INVOKER", "IS", "ISNULL", "ISOLATION", "JOIN",
  "KEY", "LANCOMPILER", "LANGUAGE", "LAST_P", "LEADING", "LEFT", "LEVEL",
  "LIKE", "LIMIT", "LISTEN", "LOAD", "LOCAL", "LOCALTIME",
  "LOCALTIMESTAMP", "LOCATION", "LOCK_P", "MATCH", "MAXVALUE", "MINUTE_P",
  "MINVALUE", "MODE", "MONTH_P", "MOVE", "NAMES", "NATIONAL", "NATURAL",
  "NCHAR", "NEW", "NEXT", "NO", "NOCREATEDB", "NOCREATEUSER", "NONE",
  "NOT", "NOTHING", "NOTIFY", "NOTNULL", "NULL_P", "NULLIF", "NUMERIC",
  "OF", "OFF", "OFFSET", "OIDS", "OLD", "ON", "ONLY", "OPERATOR", "OPTION",
  "OR", "ORDER", "OUT_P", "OUTER_P", "OVERLAPS", "OVERLAY", "OWNER",
  "PARTIAL", "PASSWORD", "PATH_P", "PENDANT", "PLACING", "POSITION",
  "PRECISION", "PRESERVE", "PREPARE", "PRIMARY", "PRIOR", "PRIVILEGES",
  "PROCEDURAL", "PROCEDURE", "READ", "REAL", "RECHECK", "REFERENCES",
  "REINDEX", "RELATIVE_P", "RENAME", "REPLACE", "RESET", "RESTART",
  "RESTRICT", "RETURNS", "REVOKE", "RIGHT", "ROLLBACK", "ROW", "ROWS",
  "RULE", "SCHEMA", "SCROLL", "SECOND_P", "SECURITY", "SELECT", "SEQUENCE",
  "SERIALIZABLE", "SESSION", "SESSION_USER", "SET", "SETOF", "SHARE",
  "SHOW", "SIMILAR", "SIMPLE", "SMALLINT", "SOME", "STABLE", "START",
  "STATEMENT", "STATISTICS", "STDIN", "STDOUT", "STORAGE", "STRICT_P",
  "SUBSTRING", "SYSID", "TABLE", "TEMP", "TEMPLATE", "TEMPORARY", "THEN",
  "TIME", "TIMESTAMP", "TO", "TOAST", "TRAILING", "TRANSACTION", "TREAT",
  "TRIGGER", "TRIM", "TRUE_P", "TRUNCATE", "TRUSTED", "TYPE_P",
  "UNENCRYPTED", "UNION", "UNIQUE", "UNKNOWN", "UNLISTEN", "UNTIL",
  "UPDATE", "USAGE", "USER", "USING", "VACUUM", "VALID", "VALIDATOR",
  "VALUES", "VARCHAR", "VARYING", "VERBOSE", "VERSION", "VIEW", "VOLATILE",
  "WHEN", "WHERE", "WITH", "WITHOUT", "WORK", "WRITE", "YEAR_P", "ZONE",
  "UNIONJOIN", "IDENT", "FCONST", "SCONST", "BCONST", "XCONST", "Op",
  "ICONST", "PARAM", "'='", "'<'", "'>'", "POSTFIXOP", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", "TYPECAST",
  "'.'", "';'", "','", "':'", "$accept", "stmtmulti", "stmt", "SelectStmt",
  "select_with_parens", "select_no_parens", "select_clause",
  "simple_select", "into_clause", "OptTempTableName", "qualified_name",
  "opt_table", "opt_all", "opt_distinct", "opt_sort_clause", "sort_clause",
  "sortby_list", "sortby", "select_limit", "opt_select_limit",
  "select_limit_value", "select_offset_value", "group_clause",
  "having_clause", "for_update_clause", "opt_for_update_clause",
  "update_list", "from_clause", "from_list", "table_ref", "joined_table",
  "alias_clause", "join_type", "join_outer", "join_qual", "relation_expr",
  "func_table", "where_clause", "TableFuncElementList", "TableFuncElement",
  "Typename", "opt_array_bounds", "SimpleTypename", "ConstTypename",
  "GenericType", "Numeric", "opt_float", "opt_numeric", "opt_decimal",
  "Bit", "ConstBit", "BitWithLength", "BitWithoutLength", "Character",
  "ConstCharacter", "CharacterWithLength", "CharacterWithoutLength",
  "character", "opt_varying", "opt_charset", "ConstDatetime",
  "ConstInterval", "opt_timezone", "opt_interval", "r_expr", "row",
  "row_descriptor", "sub_type", "all_Op", "MathOp", "qual_Op",
  "any_operator", "qual_all_Op", "a_expr", "b_expr", "c_expr", "any_name",
  "opt_indirection", "expr_list", "extract_list", "type_list",
  "array_expr_list", "array_expr", "extract_arg", "overlay_list",
  "overlay_placing", "position_list", "substr_list", "substr_from",
  "substr_for", "trim_list", "in_expr", "case_expr", "when_clause_list",
  "when_clause", "case_default", "case_arg", "columnref", "dotted_name",
  "attrs", "target_list", "target_el", "relation_name", "name_list",
  "name", "attr_name", "func_name", "AexprConst", "Iconst", "Sconst",
  "ColId", "type_name", "function_name", "ColLabel", "unreserved_keyword",
  "col_name_keyword", "func_name_keyword", "reserved_keyword",
  "SpecialRuleRelation", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,    61,    60,    62,
     572,    43,    45,    42,    47,    37,    94,   573,    91,    93,
      40,    41,   574,    46,    59,    44,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
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

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
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

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
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

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,   293,    14,     8,     9,   453,   757,
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

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1011
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

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -606
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

#define yypact_value_is_default(yystate) \
  ((yystate) == (-1011))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-606))

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

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
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

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 80:

/* Line 1806 of yacc.c  */
#line 515 "sql.y"
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
    break;



/* Line 1806 of yacc.c  */
#line 5564 "sql.tab.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1974 "sql.y"


