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

int yyerror(const char *msg);

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"


#include "typeparser.h"
#include "parserparam.h"
#include "tree.h"
#include <string.h>



/* Line 268 of yacc.c  */
#line 81 "parser.c"

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
     SELECT = 258,
     FROM = 259,
     WHERE = 260,
     NAME = 261,
     COMMA = 262,
     SPACE = 263,
     ALIAS = 264,
     DOT = 265,
     OR = 266,
     AND = 267,
     OP = 268,
     CP = 269,
     EOL = 270,
     SEMICOLON = 271,
     NUMBER = 272,
     BOOL = 273,
     EQ = 274,
     GT = 275,
     LT = 276,
     GTE = 277,
     LTE = 278,
     NE = 279,
     ASTERISK = 280,
     UNKNOWN = 281,
     STRING = 282,
     JOIN = 283,
     ON = 284,
     AGGF = 285,
     NOT = 286,
     IN = 287
   };
#endif
/* Tokens.  */
#define SELECT 258
#define FROM 259
#define WHERE 260
#define NAME 261
#define COMMA 262
#define SPACE 263
#define ALIAS 264
#define DOT 265
#define OR 266
#define AND 267
#define OP 268
#define CP 269
#define EOL 270
#define SEMICOLON 271
#define NUMBER 272
#define BOOL 273
#define EQ 274
#define GT 275
#define LT 276
#define GTE 277
#define LTE 278
#define NE 279
#define ASTERISK 280
#define UNKNOWN 281
#define STRING 282
#define JOIN 283
#define ON 284
#define AGGF 285
#define NOT 286
#define IN 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 186 "parser.c"

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   142

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNRULES -- Number of states.  */
#define YYNSTATES  128

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,    12,    14,    16,    18,    22,    24,    28,
      30,    34,    38,    46,    48,    52,    54,    58,    60,    64,
      66,    70,    72,    76,    77,    79,    84,    90,    98,   106,
     110,   112,   116,   120,   124,   128,   132,   134,   136,   138,
     140,   142,   144,   146,   148,   150,   153,   164,   175,   176,
     181,   189,   197,   201,   203,   207,   211
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      34,     0,    -1,     3,     8,    35,     8,     4,     8,    38,
      43,    -1,    36,    -1,    37,    -1,    40,    -1,    36,     7,
      40,    -1,    41,    -1,    37,     7,    41,    -1,    39,    -1,
      38,     7,    39,    -1,    38,    28,    39,    -1,    38,    28,
      39,    29,    42,    46,    42,    -1,     6,    -1,     6,     9,
       6,    -1,    42,    -1,    42,     9,     6,    -1,    30,    -1,
      30,     9,     6,    -1,     6,    -1,     6,    10,     6,    -1,
      25,    -1,     6,    10,    25,    -1,    -1,    16,    -1,     8,
       5,     8,    44,    -1,     8,     5,     8,    44,    16,    -1,
      44,     8,    11,     8,    13,    44,    14,    -1,    44,     8,
      12,     8,    13,    44,    14,    -1,    13,    44,    14,    -1,
      45,    -1,    42,    46,    17,    -1,    42,    47,    27,    -1,
      42,    47,    18,    -1,    42,    46,    50,    -1,    42,    48,
      49,    -1,    19,    -1,    20,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,    19,    -1,    24,    -1,    32,    -1,
      31,    32,    -1,    13,     3,     8,    40,     8,     4,     8,
      38,    51,    14,    -1,    13,     3,     8,    41,     8,     4,
       8,    38,    51,    14,    -1,    -1,     8,     5,     8,    52,
      -1,    52,     8,    11,     8,    13,    52,    14,    -1,    52,
       8,    12,     8,    13,    52,    14,    -1,    13,    52,    14,
      -1,    53,    -1,    42,    46,    17,    -1,    42,    47,    27,
      -1,    42,    47,    18,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    53,    53,    57,    58,    62,    63,    67,    68,    72,
      73,    74,    75,    79,    80,    84,    85,    89,    90,    94,
      95,    96,    97,   100,   101,   102,   103,   107,   108,   109,
     110,   114,   115,   116,   117,   118,   122,   123,   124,   125,
     126,   127,   131,   132,   136,   137,   141,   145,   148,   149,
     153,   154,   155,   156,   160,   161,   162
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SELECT", "FROM", "WHERE", "NAME",
  "COMMA", "SPACE", "ALIAS", "DOT", "OR", "AND", "OP", "CP", "EOL",
  "SEMICOLON", "NUMBER", "BOOL", "EQ", "GT", "LT", "GTE", "LTE", "NE",
  "ASTERISK", "UNKNOWN", "STRING", "JOIN", "ON", "AGGF", "NOT", "IN",
  "$accept", "entire_clause", "fieldlist", "fieldlistnoagg",
  "fieldlistagg", "tablelist", "tablename", "fieldname", "aggname",
  "dottedfieldname", "whereclause", "conditions", "condition",
  "completeoperator", "simpleoperator", "inclusionoperator",
  "innerquerynoagg", "innerqueryagg", "innerquerywhereclause",
  "innerqueryconditions", "innerquerycondition", 0
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
     285,   286,   287
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    37,    37,    38,
      38,    38,    38,    39,    39,    40,    40,    41,    41,    42,
      42,    42,    42,    43,    43,    43,    43,    44,    44,    44,
      44,    45,    45,    45,    45,    45,    46,    46,    46,    46,
      46,    46,    47,    47,    48,    48,    49,    50,    51,    51,
      52,    52,    52,    52,    53,    53,    53
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     8,     1,     1,     1,     3,     1,     3,     1,
       3,     3,     7,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     0,     1,     4,     5,     7,     7,     3,
       1,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,    10,    10,     0,     4,
       7,     7,     3,     1,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,    19,    21,    17,     0,     3,
       4,     5,     7,    15,     0,     0,     0,     0,     0,     0,
      20,    22,    18,     0,     6,     8,    16,     0,    13,    23,
       9,     0,     0,     0,    24,     0,     2,    14,    10,     0,
      11,     0,     0,     0,     0,    25,    30,     0,     0,    36,
      37,    38,    39,    40,    41,     0,    44,     0,     0,     0,
       0,    26,    36,    41,     0,    29,    45,     0,    31,    34,
      33,    32,     0,    35,     0,     0,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,    28,     0,     0,     0,     0,    48,    48,     0,
       0,     0,     0,    47,    46,     0,     0,     0,    49,    53,
       0,     0,     0,     0,    52,    54,    56,    55,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     8,     9,    10,    29,    30,    11,    12,    44,
      36,    45,    46,    57,    58,    59,    73,    69,   100,   108,
     109
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -101
static const yytype_int16 yypact[] =
{
      22,     3,    18,    -1,  -101,    28,  -101,    31,    72,    74,
      89,  -101,  -101,    88,     9,    92,    95,    11,    70,    98,
    -101,  -101,  -101,    93,  -101,  -101,  -101,    99,    97,     0,
    -101,   101,    99,   103,  -101,    99,  -101,  -101,  -101,   102,
      80,     6,    11,     6,    26,    48,  -101,    63,    13,     8,
    -101,  -101,  -101,  -101,    15,    79,  -101,    24,    33,   100,
      66,  -101,  -101,  -101,    11,  -101,  -101,   109,  -101,  -101,
    -101,  -101,   111,  -101,   107,   108,  -101,   110,   113,   104,
     112,    70,    11,     6,     6,   114,   115,    45,    57,   120,
     122,  -101,  -101,   119,   121,    99,    99,     2,     2,   123,
     116,   117,   124,  -101,  -101,     7,     7,    69,   125,  -101,
      60,   118,    36,    83,  -101,  -101,  -101,  -101,   126,   128,
     127,   129,     7,     7,    61,    62,  -101,  -101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -101,  -101,  -101,  -101,  -101,   -23,    20,   -16,   -14,    -3,
    -101,   -40,  -101,   -45,    30,  -101,  -101,  -101,    40,  -100,
    -101
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -44
static const yytype_int8 yytable[] =
{
      13,    24,    64,    48,    25,     5,   110,    32,    33,    32,
      99,     3,     5,     5,    13,    20,    34,     5,     4,    43,
     106,    60,   124,   125,     6,     1,   -42,    65,    35,     7,
      35,     6,     6,   -43,    21,   -42,     6,    67,    14,    47,
      15,    68,   -43,    87,    88,    49,    50,    51,    52,    53,
      54,    70,    38,    60,   116,    40,    60,    55,    56,    91,
      71,    76,   111,   117,    61,    60,    86,    85,   113,   113,
     113,    92,    97,    98,   114,   126,   127,    74,    75,    13,
      16,    17,    62,    50,    51,    52,    53,    63,    49,    50,
      51,    52,    53,    54,   118,   119,    18,    19,    22,    23,
       7,    27,   107,   107,    26,    28,    31,    37,    39,    42,
      41,    66,    77,    72,    78,    79,    80,    83,    81,   107,
     107,    82,    89,    90,    93,    84,    94,    95,   102,    96,
     103,   104,   105,   113,   120,   115,   121,   112,   101,     0,
     122,     0,   123
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-101))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       3,    17,    47,    43,    18,     6,   106,     7,     8,     7,
       8,     8,     6,     6,    17,     6,    16,     6,     0,    13,
      13,     8,   122,   123,    25,     3,    18,    14,    28,    30,
      28,    25,    25,    18,    25,    27,    25,    13,    10,    42,
       9,    17,    27,    83,    84,    19,    20,    21,    22,    23,
      24,    18,    32,     8,    18,    35,     8,    31,    32,    14,
      27,    64,   107,    27,    16,     8,    82,    81,     8,     8,
       8,    14,    95,    96,    14,    14,    14,    11,    12,    82,
       8,     7,    19,    20,    21,    22,    23,    24,    19,    20,
      21,    22,    23,    24,    11,    12,     7,     9,     6,     4,
      30,     8,   105,   106,     6,     6,     9,     6,     5,    29,
       8,    32,     3,    13,     3,     8,     8,    13,     8,   122,
     123,     8,     8,     8,     4,    13,     4,     8,     5,     8,
      14,    14,     8,     8,     8,    17,     8,   107,    98,    -1,
      13,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    34,     8,     0,     6,    25,    30,    35,    36,
      37,    40,    41,    42,    10,     9,     8,     7,     7,     9,
       6,    25,     6,     4,    40,    41,     6,     8,     6,    38,
      39,     9,     7,     8,    16,    28,    43,     6,    39,     5,
      39,     8,    29,    13,    42,    44,    45,    42,    44,    19,
      20,    21,    22,    23,    24,    31,    32,    46,    47,    48,
       8,    16,    19,    24,    46,    14,    32,    13,    17,    50,
      18,    27,    13,    49,    11,    12,    42,     3,     3,     8,
       8,     8,     8,    13,    13,    41,    40,    44,    44,     8,
       8,    14,    14,     4,     4,     8,     8,    38,    38,     8,
      51,    51,     5,    14,    14,     8,    13,    42,    52,    53,
      52,    46,    47,     8,    14,    17,    18,    27,    11,    12,
       8,     8,    13,    13,    52,    52,    14,    14
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
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
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
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

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
        case 2:

/* Line 1806 of yacc.c  */
#line 53 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (8)].svalue)) + strlen((yyvsp[(2) - (8)].svalue)) + strlen((yyvsp[(3) - (8)].svalue)) + strlen((yyvsp[(4) - (8)].svalue)) + strlen((yyvsp[(5) - (8)].svalue)) + strlen((yyvsp[(6) - (8)].svalue)) + strlen((yyvsp[(7) - (8)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (8)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (8)].svalue)); ((SParserParam*)data)->result.parentquery = (yyval.svalue); free((yyvsp[(1) - (8)].svalue)); free((yyvsp[(2) - (8)].svalue)); free((yyvsp[(3) - (8)].svalue)); free((yyvsp[(4) - (8)].svalue)); free((yyvsp[(5) - (8)].svalue)); free((yyvsp[(6) - (8)].svalue)); free((yyvsp[(7) - (8)].svalue));}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 57 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue); ((SParserParam*)data)->result.parentIsAgg = 0;}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 58 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue); ((SParserParam*)data)->result.parentIsAgg = 1;}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 62 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 63 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 67 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 68 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 72 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 73 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 74 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 75 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (7)].svalue)) + strlen((yyvsp[(2) - (7)].svalue)) + strlen((yyvsp[(3) - (7)].svalue)) + strlen((yyvsp[(4) - (7)].svalue)) + strlen((yyvsp[(5) - (7)].svalue)) + strlen((yyvsp[(6) - (7)].svalue)) + strlen((yyvsp[(7) - (7)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (7)].svalue)); free((yyvsp[(1) - (7)].svalue)); free((yyvsp[(2) - (7)].svalue)); free((yyvsp[(3) - (7)].svalue)); free((yyvsp[(4) - (7)].svalue)); free((yyvsp[(5) - (7)].svalue)); free((yyvsp[(6) - (7)].svalue)); free((yyvsp[(7) - (7)].svalue));}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 79 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 80 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 84 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 85 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 89 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 90 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 94 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 95 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 96 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 97 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 100 "parser.y"
    {((SParserParam*)data)->result.root = NULL;}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 101 "parser.y"
    {((SParserParam*)data)->result.root = NULL;}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 102 "parser.y"
    {((SParserParam*)data)->result.root = (yyvsp[(4) - (4)].tree); free((yyvsp[(1) - (4)].svalue)); free((yyvsp[(2) - (4)].svalue)); free((yyvsp[(3) - (4)].svalue));}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 103 "parser.y"
    {((SParserParam*)data)->result.root = (yyvsp[(4) - (5)].tree); free((yyvsp[(1) - (5)].svalue)); free((yyvsp[(2) - (5)].svalue)); free((yyvsp[(3) - (5)].svalue));}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 107 "parser.y"
    {(yyval.tree) = createOperation(eOR, (yyvsp[(1) - (7)].tree), (yyvsp[(6) - (7)].tree)); free((yyvsp[(2) - (7)].svalue)); free((yyvsp[(3) - (7)].svalue)); free((yyvsp[(4) - (7)].svalue)); free((yyvsp[(5) - (7)].svalue)); free((yyvsp[(7) - (7)].svalue));}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 108 "parser.y"
    {(yyval.tree) = createOperation(eAND, (yyvsp[(1) - (7)].tree), (yyvsp[(6) - (7)].tree)); free((yyvsp[(2) - (7)].svalue)); free((yyvsp[(3) - (7)].svalue)); free((yyvsp[(4) - (7)].svalue)); free((yyvsp[(5) - (7)].svalue)); free((yyvsp[(7) - (7)].svalue));}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 109 "parser.y"
    {(yyval.tree) = (yyvsp[(2) - (3)].tree); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 110 "parser.y"
    {(yyval.tree) = (yyvsp[(1) - (1)].tree);}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 114 "parser.y"
    {(yyval.tree) = createLeaf(eCOND_NUM, (yyvsp[(1) - (3)].svalue), (yyvsp[(2) - (3)].svalue), NULL, (yyvsp[(3) - (3)].dvalue), NULL); ((SParserParam*)data)->result.numOfLeafs++; free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue));}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 115 "parser.y"
    {(yyval.tree) = createLeaf(eCOND_STR, (yyvsp[(1) - (3)].svalue), (yyvsp[(2) - (3)].svalue), NULL, 0, (yyvsp[(3) - (3)].svalue)); ((SParserParam*)data)->result.numOfLeafs++; free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 116 "parser.y"
    {(yyval.tree) = createLeaf(eCOND_BOOL, (yyvsp[(1) - (3)].svalue), (yyvsp[(2) - (3)].svalue), NULL, (yyvsp[(3) - (3)].dvalue), NULL); ((SParserParam*)data)->result.numOfLeafs++; free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue));}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 117 "parser.y"
    {(yyval.tree) = createLeaf(eSUBQ, (yyvsp[(1) - (3)].svalue), (yyvsp[(2) - (3)].svalue), (yyvsp[(3) - (3)].svalue), 0, NULL); ((SParserParam*)data)->result.numOfLeafs++; free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 118 "parser.y"
    {(yyval.tree) = createLeaf(eSUBQNOAGG, (yyvsp[(1) - (3)].svalue), (yyvsp[(2) - (3)].svalue), (yyvsp[(3) - (3)].svalue), 0, NULL); ((SParserParam*)data)->result.numOfLeafs++; free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 122 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 123 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 124 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 125 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 126 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 127 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 131 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 132 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 136 "parser.y"
    {(yyval.svalue) = (yyvsp[(1) - (1)].svalue);}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 137 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (2)].svalue)) + strlen((yyvsp[(2) - (2)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (2)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (2)].svalue)); free((yyvsp[(1) - (2)].svalue)); free((yyvsp[(2) - (2)].svalue));}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 141 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(2) - (10)].svalue)) + strlen((yyvsp[(3) - (10)].svalue)) + strlen((yyvsp[(4) - (10)].svalue)) + strlen((yyvsp[(5) - (10)].svalue)) + strlen((yyvsp[(6) - (10)].svalue)) + strlen((yyvsp[(7) - (10)].svalue)) + strlen((yyvsp[(8) - (10)].svalue)) + strlen((yyvsp[(9) - (10)].svalue)) + 1, sizeof(char));  strcat((yyval.svalue), (yyvsp[(2) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(8) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(9) - (10)].svalue));   ((SParserParam*)data)->result.numOfSubs++; free((yyvsp[(1) - (10)].svalue)); free((yyvsp[(2) - (10)].svalue)); free((yyvsp[(3) - (10)].svalue)); free((yyvsp[(4) - (10)].svalue)); free((yyvsp[(5) - (10)].svalue)); free((yyvsp[(6) - (10)].svalue)); free((yyvsp[(7) - (10)].svalue)); free((yyvsp[(8) - (10)].svalue)); free((yyvsp[(9) - (10)].svalue)); free((yyvsp[(10) - (10)].svalue));}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 145 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(2) - (10)].svalue)) + strlen((yyvsp[(3) - (10)].svalue)) + strlen((yyvsp[(4) - (10)].svalue)) + strlen((yyvsp[(5) - (10)].svalue)) + strlen((yyvsp[(6) - (10)].svalue)) + strlen((yyvsp[(7) - (10)].svalue)) + strlen((yyvsp[(8) - (10)].svalue)) + strlen((yyvsp[(9) - (10)].svalue)) + 1, sizeof(char));  strcat((yyval.svalue), (yyvsp[(2) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(8) - (10)].svalue)); strcat((yyval.svalue), (yyvsp[(9) - (10)].svalue));  ((SParserParam*)data)->result.numOfAggSubs++; ((SParserParam*)data)->result.numOfSubs++; free((yyvsp[(1) - (10)].svalue)); free((yyvsp[(2) - (10)].svalue)); free((yyvsp[(3) - (10)].svalue)); free((yyvsp[(4) - (10)].svalue)); free((yyvsp[(5) - (10)].svalue)); free((yyvsp[(6) - (10)].svalue)); free((yyvsp[(7) - (10)].svalue)); free((yyvsp[(8) - (10)].svalue)); free((yyvsp[(9) - (10)].svalue)); free((yyvsp[(10) - (10)].svalue));}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 148 "parser.y"
    {(yyval.svalue) = strdup("");}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 149 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (4)].svalue)) + strlen((yyvsp[(2) - (4)].svalue)) + strlen((yyvsp[(3) - (4)].svalue)) + strlen((yyvsp[(4) - (4)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (4)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (4)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (4)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (4)].svalue)); free((yyvsp[(1) - (4)].svalue)); free((yyvsp[(2) - (4)].svalue)); free((yyvsp[(3) - (4)].svalue)); free((yyvsp[(4) - (4)].svalue));}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 153 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (7)].svalue)) + strlen((yyvsp[(2) - (7)].svalue)) + strlen((yyvsp[(3) - (7)].svalue)) + strlen((yyvsp[(4) - (7)].svalue)) + strlen((yyvsp[(5) - (7)].svalue)) + strlen((yyvsp[(6) - (7)].svalue)) + strlen((yyvsp[(7) - (7)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (7)].svalue)); free((yyvsp[(1) - (7)].svalue)); free((yyvsp[(2) - (7)].svalue)); free((yyvsp[(3) - (7)].svalue)); free((yyvsp[(4) - (7)].svalue)); free((yyvsp[(5) - (7)].svalue)); free((yyvsp[(6) - (7)].svalue)); free((yyvsp[(7) - (7)].svalue));}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 154 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (7)].svalue)) + strlen((yyvsp[(2) - (7)].svalue)) + strlen((yyvsp[(3) - (7)].svalue)) + strlen((yyvsp[(4) - (7)].svalue)) + strlen((yyvsp[(5) - (7)].svalue)) + strlen((yyvsp[(6) - (7)].svalue)) + strlen((yyvsp[(7) - (7)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(4) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(5) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(6) - (7)].svalue)); strcat((yyval.svalue), (yyvsp[(7) - (7)].svalue)); free((yyvsp[(1) - (7)].svalue)); free((yyvsp[(2) - (7)].svalue)); free((yyvsp[(3) - (7)].svalue)); free((yyvsp[(4) - (7)].svalue)); free((yyvsp[(5) - (7)].svalue)); free((yyvsp[(6) - (7)].svalue)); free((yyvsp[(7) - (7)].svalue));}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 155 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 156 "parser.y"
    {(yyval.svalue) = strdup((yyvsp[(1) - (1)].svalue)); free((yyvsp[(1) - (1)].svalue));}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 160 "parser.y"
    {char temp[30]; snprintf(temp, 30, "%f", (yyvsp[(3) - (3)].dvalue)); (yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen(temp) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), temp); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue));}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 161 "parser.y"
    {(yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen((yyvsp[(3) - (3)].svalue)) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(3) - (3)].svalue)); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue)); free((yyvsp[(3) - (3)].svalue));}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 162 "parser.y"
    {char temp[30]; snprintf(temp, 30, "%f", (yyvsp[(3) - (3)].dvalue)); (yyval.svalue) = (char *)calloc(strlen((yyvsp[(1) - (3)].svalue)) + strlen((yyvsp[(2) - (3)].svalue)) + strlen(temp) + 1, sizeof(char)); strcat((yyval.svalue), (yyvsp[(1) - (3)].svalue)); strcat((yyval.svalue), (yyvsp[(2) - (3)].svalue)); strcat((yyval.svalue), temp); free((yyvsp[(1) - (3)].svalue)); free((yyvsp[(2) - (3)].svalue));}
    break;



/* Line 1806 of yacc.c  */
#line 1902 "parser.c"
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



