%{

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

%}


%union
{
	int				ival;
	char				chr;
	char				*str;
	const char			*keyword;
}



%type <ival>	Iconst


/*
 * If you make any token changes, update the keyword table in
 * parser/keywords.c and add new keywords to the appropriate one of
 * the reserved-or-not-so-reserved keyword lists, below.
 */

/* ordinary key words in alphabetical order */
%token <keyword> ABORT_P ABSOLUTE_P ACCESS ACTION ADD AFTER
	AGGREGATE ALL ALTER ANALYSE ANALYZE AND ANY ARRAY AS ASC
	ASSERTION ASSIGNMENT AT AUTHORIZATION

	BACKWARD BEFORE BEGIN_P BETWEEN BIGINT BINARY BIT
	BOOLEAN_P BOTH BY

	CACHE CALLED CASCADE CASE CAST CHAIN CHAR_P
	CHARACTER CHARACTERISTICS CHECK CHECKPOINT CLASS CLOSE
	CLUSTER COALESCE COLLATE COLUMN COMMENT COMMIT
	COMMITTED CONSTRAINT CONSTRAINTS CONVERSION_P CONVERT COPY CREATE CREATEDB
	CREATEUSER CROSS CURRENT_DATE CURRENT_TIME
	CURRENT_TIMESTAMP CURRENT_USER CURSOR CYCLE

	DATABASE DAY_P DEALLOCATE DEC DECIMAL_P DECLARE DEFAULT DEFAULTS
	DEFERRABLE DEFERRED DEFINER DELETE_P DELIMITER DELIMITERS
    DESC DISTINCT DO DOMAIN_P DOUBLE_P DROP

	EACH ELSE ENCODING ENCRYPTED END_P ESCAPE EXCEPT EXCLUDING
	EXCLUSIVE EXECUTE EXISTS EXPLAIN EXTERNAL EXTRACT

	FALSE_P FETCH FIRST_P FLOAT_P FOR FORCE FOREIGN FORWARD
	FREEZE FROM FULL FUNCTION

	GLOBAL GRANT GROUP_P

	HANDLER HAVING HOLD HOUR_P

	ILIKE IMMEDIATE IMMUTABLE IMPLICIT_P IN_P INCLUDING INCREMENT
	INDEX INHERITS INITIALLY INNER_P INOUT INPUT_P
	INSENSITIVE INSERT INSTEAD INT_P INTEGER INTERSECT
	INTERVAL INTO INVOKER IS ISNULL ISOLATION

	JOIN

	KEY

	LANCOMPILER LANGUAGE LAST_P LEADING LEFT LEVEL LIKE LIMIT
	LISTEN LOAD LOCAL LOCALTIME LOCALTIMESTAMP LOCATION
	LOCK_P

	MATCH MAXVALUE MINUTE_P MINVALUE MODE MONTH_P MOVE

	NAMES NATIONAL NATURAL NCHAR NEW NEXT NO NOCREATEDB
	NOCREATEUSER NONE NOT NOTHING NOTIFY NOTNULL NULL_P
	NULLIF NUMERIC

	OF OFF OFFSET OIDS OLD ON ONLY OPERATOR OPTION OR
	ORDER OUT_P OUTER_P OVERLAPS OVERLAY OWNER

	PARTIAL PASSWORD PATH_P PENDANT PLACING POSITION
	PRECISION PRESERVE PREPARE PRIMARY 
	PRIOR PRIVILEGES PROCEDURAL PROCEDURE

	READ REAL RECHECK REFERENCES REINDEX RELATIVE_P RENAME REPLACE
	RESET RESTART RESTRICT RETURNS REVOKE RIGHT ROLLBACK ROW ROWS
	RULE

	SCHEMA SCROLL SECOND_P SECURITY SELECT SEQUENCE
	SERIALIZABLE SESSION SESSION_USER SET SETOF SHARE
	SHOW SIMILAR SIMPLE SMALLINT SOME STABLE START STATEMENT
	STATISTICS STDIN STDOUT STORAGE STRICT_P SUBSTRING SYSID

	TABLE TEMP TEMPLATE TEMPORARY THEN TIME TIMESTAMP
	TO TOAST TRAILING TRANSACTION TREAT TRIGGER TRIM TRUE_P
	TRUNCATE TRUSTED TYPE_P

	UNENCRYPTED UNION UNIQUE UNKNOWN UNLISTEN UNTIL
	UPDATE USAGE USER USING

	VACUUM VALID VALIDATOR VALUES VARCHAR VARYING
	VERBOSE VERSION VIEW VOLATILE

	WHEN WHERE WITH WITHOUT WORK WRITE

	YEAR_P

	ZONE

/* The grammar thinks these are keywords, but they are not in the keywords.c
 * list and so can never be entered directly.  The filter in parser.c
 * creates these tokens when required.
 */
%token			UNIONJOIN

/* Special keywords, not in the query language - see the "lex" file */
%token <str>	IDENT FCONST SCONST BCONST XCONST Op
%token <ival>	ICONST PARAM

/* precedence: lowest to highest */
%left		UNION EXCEPT
%left		INTERSECT
%left		OR
%left		AND
%right		NOT
%right		'='
%nonassoc	'<' '>'
%nonassoc	LIKE ILIKE SIMILAR
%nonassoc	ESCAPE
%nonassoc	OVERLAPS
%nonassoc	BETWEEN
%nonassoc	IN_P
%left		POSTFIXOP		/* dummy for postfix Op rules */
%left		Op OPERATOR		/* multi-character ops and user-defined operators */
%nonassoc	NOTNULL
%nonassoc	ISNULL
%nonassoc	IS NULL_P TRUE_P FALSE_P UNKNOWN /* sets precedence for IS NULL, etc */
%left		'+' '-'
%left		'*' '/' '%'
%left		'^'
/* Unary Operators */
%left		AT ZONE			/* sets precedence for AT TIME ZONE */
%right		UMINUS
%left		'[' ']'
%left		'(' ')'
%left		TYPECAST
%left		'.'
/*
 * These might seem to be low-precedence, but actually they are not part
 * of the arithmetic hierarchy at all in their use as JOIN operators.
 * We make them high-precedence to support their use as function names.
 * They wouldn't be given a precedence at all, were it not that we need
 * left-associativity among the JOIN rules themselves.
 */
%left		JOIN UNIONJOIN CROSS LEFT FULL RIGHT INNER_P NATURAL
%%

/*
 *	Handle comment-only lines, and ;; SELECT * FROM pg_class ;;;
 *	psql already handles such cases, but other interfaces don't.
 *	bjm 1999/10/05
 */

stmtmulti:  	stmt    
 		| stmtmulti stmt
		;


stmt:		SelectStmt ';'
		;

/*****************************************************************************
 *
 *		QUERY:
 *				SELECT STATEMENTS
 *
 *****************************************************************************/

/* A complete SELECT statement looks like this.
 *
 * The rule returns either a single SelectStmt node or a tree of them,
 * representing a set-operation tree.
 *
 * There is an ambiguity when a sub-SELECT is within an a_expr and there
 * are excess parentheses: do the parentheses belong to the sub-SELECT or
 * to the surrounding a_expr?  We don't really care, but yacc wants to know.
 * To resolve the ambiguity, we are careful to define the grammar so that
 * the decision is staved off as long as possible: as long as we can keep
 * absorbing parentheses into the sub-SELECT, we will do so, and only when
 * it's no longer possible to do that will we decide that parens belong to
 * the expression.	For example, in "SELECT (((SELECT 2)) + 3)" the extra
 * parentheses are treated as part of the sub-select.  The necessity of doing
 * it that way is shown by "SELECT (((SELECT 2)) UNION SELECT 2)".	Had we
 * parsed "((SELECT 2))" as an a_expr, it'd be too late to go back to the
 * SELECT viewpoint when we see the UNION.
 *
 * This approach is implemented by defining a nonterminal select_with_parens,
 * which represents a SELECT with at least one outer layer of parentheses,
 * and being careful to use select_with_parens, never '(' SelectStmt ')',
 * in the expression grammar.  We will then have shift-reduce conflicts
 * which we can resolve in favor of always treating '(' <select> ')' as
 * a select_with_parens.  To resolve the conflicts, the productions that
 * conflict with the select_with_parens productions are manually given
 * precedences lower than the precedence of ')', thereby ensuring that we
 * shift ')' (and then reduce to select_with_parens) rather than trying to
 * reduce the inner <select> nonterminal to something else.  We use UMINUS
 * precedence for this, which is a fairly arbitrary choice.
 *
 * To be able to define select_with_parens itself without ambiguity, we need
 * a nonterminal select_no_parens that represents a SELECT structure with no
 * outermost parentheses.  This is a little bit tedious, but it works.
 *
 * In non-expression contexts, we use SelectStmt which can represent a SELECT
 * with or without outer parentheses.
 */

SelectStmt: select_no_parens			%prec UMINUS
			| select_with_parens		%prec UMINUS
		;

select_with_parens:
			'(' select_no_parens ')'				
			| '(' select_with_parens ')'			
		;

/*
 *	FOR UPDATE may be before or after LIMIT/OFFSET.
 *	In <=7.2.X, LIMIT/OFFSET had to be after FOR UPDATE
 *	We now support both orderings, but prefer LIMIT/OFFSET before FOR UPDATE
 *	2002-08-28 bjm
 */
select_no_parens:
			simple_select						
			| select_clause sort_clause
				
			| select_clause opt_sort_clause for_update_clause opt_select_limit
				
			| select_clause opt_sort_clause select_limit opt_for_update_clause
				
		;

select_clause:
			simple_select							
			| select_with_parens					
		;

/*
 * This rule parses SELECT statements that can appear within set operations,
 * including UNION, INTERSECT and EXCEPT.  '(' and ')' can be used to specify
 * the ordering of the set operations.	Without '(' and ')' we want the
 * operations to be ordered per the precedence specs at the head of this file.
 *
 * As with select_no_parens, simple_select cannot have outer parentheses,
 * but can have parenthesized subclauses.
 *
 * Note that sort clauses cannot be included at this level --- SQL92 requires
 *		SELECT foo UNION SELECT bar ORDER BY baz
 * to be parsed as
 *		(SELECT foo UNION SELECT bar) ORDER BY baz
 * not
 *		SELECT foo UNION (SELECT bar ORDER BY baz)
 * Likewise FOR UPDATE and LIMIT.  Therefore, those clauses are described
 * as part of the select_no_parens production, not simple_select.
 * This does not limit functionality, because you can reintroduce sort and
 * limit clauses inside parentheses.
 *
 * NOTE: only the leftmost component SelectStmt should have INTO.
 * However, this is not checked by the grammar; parse analysis must check it.
 */
simple_select:
			SELECT opt_distinct target_list
			into_clause from_clause where_clause
			group_clause having_clause
				
			| select_clause UNION opt_all select_clause
				
			| select_clause INTERSECT opt_all select_clause
				
			| select_clause EXCEPT opt_all select_clause
				
		;

into_clause:
			INTO OptTempTableName					
			| /*EMPTY*/								
		;

/*
 * Redundancy here is needed to avoid shift/reduce conflicts,
 * since TEMP is not a reserved word.  See also OptTemp.
 */
OptTempTableName:
			TEMPORARY opt_table qualified_name
				
			| TEMP opt_table qualified_name
				
			| LOCAL TEMPORARY opt_table qualified_name
				
			| LOCAL TEMP opt_table qualified_name
				
			| GLOBAL TEMPORARY opt_table qualified_name
				
			| GLOBAL TEMP opt_table qualified_name
				
			| TABLE qualified_name
				
			| qualified_name
				
		;


qualified_name:
		relation_name
		| dotted_name
		;

opt_table:	TABLE									
			| /*EMPTY*/								
		;

opt_all:	ALL										
			| DISTINCT								
			| /*EMPTY*/								
		;

/* We use (NIL) as a placeholder to indicate that all target expressions
 * should be placed in the DISTINCT list during parsetree analysis.
 */
opt_distinct:
			DISTINCT								
			| DISTINCT ON '(' expr_list ')'			
			| ALL									
			| /*EMPTY*/								
		;

opt_sort_clause:
			sort_clause								
			| /*EMPTY*/								
		;

sort_clause:
			ORDER BY sortby_list					
		;

sortby_list:
			sortby									
			| sortby_list ',' sortby				
		;

sortby:		a_expr USING qual_all_Op
				
			| a_expr ASC
				
			| a_expr DESC
				
			| a_expr
				
		;


select_limit:
			LIMIT select_limit_value OFFSET select_offset_value
				
			| OFFSET select_offset_value LIMIT select_limit_value
				
			| LIMIT select_limit_value
				
			| OFFSET select_offset_value
				
			| LIMIT select_limit_value ',' select_offset_value
				
		;

opt_select_limit:
			select_limit							
			| /* EMPTY */
					
		;

select_limit_value:
			a_expr									
			| ALL
				
		;

select_offset_value:
			a_expr									
		;

/*
 *	jimmy bell-style recursive queries aren't supported in the
 *	current system.
 *
 *	...however, recursive addattr and rename supported.  make special
 *	cases for these.
 */

group_clause:
			GROUP_P BY expr_list					
			| /*EMPTY*/								
		;

having_clause:
			HAVING a_expr							
			| /*EMPTY*/								
		;

for_update_clause:
			FOR UPDATE update_list					
			| FOR READ ONLY							
		;

opt_for_update_clause:
			for_update_clause						
			| /* EMPTY */							
		;

update_list:
			OF name_list							
			| /* EMPTY */							
		;

/*****************************************************************************
 *
 *	clauses common to all Optimizable Stmts:
 *		from_clause		- allow list of both JOIN expressions and table names
 *		where_clause	- qualifications for joins or restrictions
 *
 *****************************************************************************/

from_clause:
			FROM from_list							
			| /*EMPTY*/								
		;

from_list:
			table_ref								
			| from_list ',' table_ref				
		;

/*
 * table_ref is where an alias clause can be attached.	Note we cannot make
 * alias_clause have an empty production because that causes parse conflicts
 * between table_ref := '(' joined_table ')' alias_clause
 * and joined_table := '(' joined_table ')'.  So, we must have the
 * redundant-looking productions here instead.
 */
table_ref:	relation_expr
				
			| relation_expr alias_clause
				
			| func_table
				
			| func_table alias_clause
				
			| func_table AS '(' TableFuncElementList ')'
				
			| func_table AS ColId '(' TableFuncElementList ')'
				
			| func_table ColId '(' TableFuncElementList ')'
				
			| select_with_parens
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
			| select_with_parens alias_clause
				
			| joined_table
				
			| '(' joined_table ')' alias_clause
				
		;


/*
 * It may seem silly to separate joined_table from table_ref, but there is
 * method in SQL92's madness: if you don't do it this way you get reduce-
 * reduce conflicts, because it's not clear to the parser generator whether
 * to expect alias_clause after ')' or not.  For the same reason we must
 * treat 'JOIN' and 'join_type JOIN' separately, rather than allowing
 * join_type to expand to empty; if we try it, the parser generator can't
 * figure out when to reduce an empty join_type right after table_ref.
 *
 * Note that a CROSS JOIN is the same as an unqualified
 * INNER JOIN, and an INNER JOIN/ON has the same shape
 * but a qualification expression to limit membership.
 * A NATURAL JOIN implicitly matches column names between
 * tables and the shape is determined by which columns are
 * in common. We'll collect columns during the later transformations.
 */

joined_table:
			'(' joined_table ')'
				
			| table_ref CROSS JOIN table_ref
				
			| table_ref UNIONJOIN table_ref
				
			| table_ref join_type JOIN table_ref join_qual
				
			| table_ref JOIN table_ref join_qual
				
			| table_ref NATURAL join_type JOIN table_ref
				
			| table_ref NATURAL JOIN table_ref
				
		;

alias_clause:
			AS ColId '(' name_list ')'
				
			| AS ColId
				
			| ColId '(' name_list ')'
				
			| ColId
				
		;

join_type:	FULL join_outer							
			| LEFT join_outer						
			| RIGHT join_outer						
			| INNER_P								
		;

/* OUTER is just noise... */
join_outer: OUTER_P									
			| /*EMPTY*/								
		;

/* JOIN qualification clauses
 * Possibilities are:
 *	USING ( column list ) allows only unqualified column names,
 *						  which must match between tables.
 *	ON expr allows more general qualifications.
 *
 * We return USING as a List node, while an ON-expr will not be a List.
 */

join_qual:	USING '(' name_list ')'					
			| ON a_expr								
		;


relation_expr:
			qualified_name
				
			| qualified_name '*'
				
			| ONLY qualified_name
				
			| ONLY '(' qualified_name ')'
				
		;


func_table: func_name '(' ')'
				
			| func_name '(' expr_list ')'
				
		;


where_clause:
			WHERE a_expr							
			| /*EMPTY*/								
		;


TableFuncElementList:
			TableFuncElement
				
			| TableFuncElementList ',' TableFuncElement
				
		;

TableFuncElement:	ColId Typename
				
		;

/*****************************************************************************
 *
 *	Type syntax
 *		SQL92 introduces a large amount of type-specific syntax.
 *		Define individual clauses to handle these cases, and use
 *		 the generic case to handle regular type-extensible Postgres syntax.
 *		- thomas 1997-10-10
 *
 *****************************************************************************/

Typename:	SimpleTypename opt_array_bounds
				
			| SETOF SimpleTypename opt_array_bounds
				
			| SimpleTypename ARRAY '[' Iconst ']'
				
			| SETOF SimpleTypename ARRAY '[' Iconst ']'
				
		;

opt_array_bounds:
			opt_array_bounds '[' ']'
					
			| opt_array_bounds '[' Iconst ']'
					
			| /*EMPTY*/
					
		;

/*
 * XXX ideally, the production for a qualified typename should be ColId attrs
 * (there's no obvious reason why the first name should need to be restricted)
 * and should be an alternative of GenericType (so that it can be used to
 * specify a type for a literal in AExprConst).  However doing either causes
 * reduce/reduce conflicts that I haven't been able to find a workaround
 * for.  FIXME later.
 */
SimpleTypename:
			GenericType								
			| Numeric								
			| Bit									
			| Character								
			| ConstDatetime							
			| ConstInterval opt_interval
				
			| ConstInterval '(' Iconst ')' opt_interval
				
			| type_name attrs
				
		;

/* We have a separate ConstTypename to allow defaulting fixed-length
 * types such as CHAR() and BIT() to an unspecified length.
 * SQL9x requires that these default to a length of one, but this
 * makes no sense for constructs like CHAR 'hi' and BIT '0101',
 * where there is an obvious better choice to make.
 * Note that ConstInterval is not included here since it must
 * be pushed up higher in the rules to accomodate the postfix
 * options (e.g. INTERVAL '1' YEAR).
 */
ConstTypename:
			GenericType								
			| Numeric								
			| ConstBit								
			| ConstCharacter						
			| ConstDatetime							
		;

GenericType:
			type_name
				
		;

/* SQL92 numeric data types
 * Check FLOAT() precision limits assuming IEEE floating types.
 * - thomas 1997-09-18
 * Provide real DECIMAL() and NUMERIC() implementations now - Jan 1998-12-30
 */
Numeric:	INT_P
				
			| INTEGER
				
			| SMALLINT
				
			| BIGINT
				
			| REAL
				
			| FLOAT_P opt_float
				
			| DOUBLE_P PRECISION
				
			| DECIMAL_P opt_decimal

			| DEC opt_decimal
				
			| NUMERIC opt_numeric
				
			| BOOLEAN_P
				
		;

opt_float:	'(' Iconst ')'
				
			| /*EMPTY*/
				
		;

opt_numeric:
			'(' Iconst ',' Iconst ')'
				
			| '(' Iconst ')'
				
			| /*EMPTY*/
				
		;

opt_decimal:
			'(' Iconst ',' Iconst ')'
				
			| '(' Iconst ')'
				
			| /*EMPTY*/
				
		;


/*
 * SQL92 bit-field data types
 * The following implements BIT() and BIT VARYING().
 */
Bit:		BitWithLength
				
			| BitWithoutLength
				
		;

/* ConstBit is like Bit except "BIT" defaults to unspecified length */
/* See notes for ConstCharacter, which addresses same issue for "CHAR" */
ConstBit:	BitWithLength
				
			| BitWithoutLength
				
		;

BitWithLength:
			BIT opt_varying '(' Iconst ')'
				
		;

BitWithoutLength:
			BIT opt_varying
				
		;


/*
 * SQL92 character data types
 * The following implements CHAR() and VARCHAR().
 */
Character:  CharacterWithLength
				
			| CharacterWithoutLength
				
		;

ConstCharacter:  CharacterWithLength
				
			| CharacterWithoutLength
				
		;

CharacterWithLength:  character '(' Iconst ')' opt_charset
				
		;

CharacterWithoutLength:	 character opt_charset
				
		;

character:	CHARACTER opt_varying
										
			| CHAR_P opt_varying
										
			| VARCHAR
										
			| NATIONAL CHARACTER opt_varying
										
			| NATIONAL CHAR_P opt_varying
										
			| NCHAR opt_varying
										
		;

opt_varying:
			VARYING									
			| /*EMPTY*/								
		;

opt_charset:
			CHARACTER SET ColId						
			| /*EMPTY*/								
		;

ConstDatetime:
			TIMESTAMP '(' Iconst ')' opt_timezone
				
			| TIMESTAMP opt_timezone
				
			| TIME '(' Iconst ')' opt_timezone
				
			| TIME opt_timezone
				
		;

ConstInterval:
			INTERVAL								
		;

opt_timezone:
			WITH TIME ZONE							
			| WITHOUT TIME ZONE						
			| /*EMPTY*/								
		;

opt_interval:
			YEAR_P									
			| MONTH_P								
			| DAY_P									
			| HOUR_P								
			| MINUTE_P								
			| SECOND_P								
			| YEAR_P TO MONTH_P
					
			| DAY_P TO HOUR_P
					
			| DAY_P TO MINUTE_P
				
			| DAY_P TO SECOND_P
					
			| HOUR_P TO MINUTE_P
					
			| HOUR_P TO SECOND_P
					
			| MINUTE_P TO SECOND_P
					
			| /*EMPTY*/								
		;


/*****************************************************************************
 *
 *	expression grammar
 *
 *****************************************************************************/

/* Expressions using row descriptors
 * Define row_descriptor to allow yacc to break the reduce/reduce conflict
 * with singleton expressions. Use SQL99's ROW keyword to allow rows of
 * one element.
 */
r_expr:  row IN_P select_with_parens
				
			| row NOT IN_P select_with_parens
				
			| row qual_all_Op sub_type select_with_parens
			%prec Op
				
			| row qual_all_Op select_with_parens
			%prec Op
				
			| row qual_all_Op row
			%prec Op
				
			| row IS NULL_P
				
			| row IS NOT NULL_P
				
			| row OVERLAPS row
				
			| row IS DISTINCT FROM row
			%prec IS
				
		;

/* Explicit row production.
 * SQL99 allows an optional ROW keyword, so we can now do single-element productions
 * without conflicting with the parenthesized a_expr production.
 */
row:  ROW '(' row_descriptor ')'					
			| ROW '(' a_expr ')'					
			| ROW '(' ')'							
			| '(' row_descriptor ')'				
		;

row_descriptor:  expr_list ',' a_expr				
		;

sub_type:	ANY										
			| SOME									
			| ALL									
		;

all_Op:		Op										
			| MathOp								
		;

MathOp:		 '+'									
			| '-'									
			| '*'									
			| '/'									
			| '%'									
			| '^'									
			| '<'									
			| '>'									
			| '='									
		;

qual_Op:	Op
					
			| OPERATOR '(' any_operator ')'			
		;

any_operator:           
                        all_Op  
                        | ColId '.' any_operator
                ;

qual_all_Op:
			all_Op
					
			| OPERATOR '(' any_operator ')'			
		;

/*
 * General expressions
 * This is the heart of the expression syntax.
 *
 * We have two expression types: a_expr is the unrestricted kind, and
 * b_expr is a subset that must be used in some places to avoid shift/reduce
 * conflicts.  For example, we can't do BETWEEN as "BETWEEN a_expr AND a_expr"
 * because that use of AND conflicts with AND as a boolean operator.  So,
 * b_expr is used in BETWEEN and we remove boolean keywords from b_expr.
 *
 * Note that '(' a_expr ')' is a b_expr, so an unrestricted expression can
 * always be used by surrounding it with parens.
 *
 * c_expr is all the productions that are common to a_expr and b_expr;
 * it's factored out just to eliminate redundant coding.
 */
a_expr:		c_expr									
			| a_expr TYPECAST Typename
					
			| a_expr AT TIME ZONE c_expr
				
		/*
		 * These operators must be called out explicitly in order to make use
		 * of yacc/bison's automatic operator-precedence handling.  All other
		 * operator names are handled by the generic productions using "Op",
		 * below; and all those operators will have the same precedence.
		 *
		 * If you add more explicitly-known operators, be sure to add them
		 * also to b_expr and to the MathOp list above.
		 */
			| '+' a_expr					%prec UMINUS
				
			| '-' a_expr					%prec UMINUS
				
			| '%' a_expr
				
			| '^' a_expr
				
			| a_expr '%'
				
			| a_expr '^'
				
			| a_expr '+' a_expr
				
			| a_expr '-' a_expr
				
			| a_expr '*' a_expr
				
			| a_expr '/' a_expr
				
			| a_expr '%' a_expr
				
			| a_expr '^' a_expr
				
			| a_expr '<' a_expr
				
			| a_expr '>' a_expr
				
			| a_expr '=' a_expr
				

			| a_expr qual_Op a_expr				%prec Op
				
			| qual_Op a_expr					%prec Op
				
			| a_expr qual_Op					%prec POSTFIXOP
				

			| a_expr AND a_expr
				
			| a_expr OR a_expr
				
			| NOT a_expr
				

			| a_expr LIKE a_expr
				
			| a_expr LIKE a_expr ESCAPE a_expr
				
			| a_expr NOT LIKE a_expr
				
			| a_expr NOT LIKE a_expr ESCAPE a_expr
				
			| a_expr ILIKE a_expr
				
			| a_expr ILIKE a_expr ESCAPE a_expr
				
			| a_expr NOT ILIKE a_expr
				
			| a_expr NOT ILIKE a_expr ESCAPE a_expr
				

			| a_expr SIMILAR TO a_expr				%prec SIMILAR
				
			| a_expr SIMILAR TO a_expr ESCAPE a_expr
				
			| a_expr NOT SIMILAR TO a_expr			%prec SIMILAR
				
			| a_expr NOT SIMILAR TO a_expr ESCAPE a_expr
				

			/* NullTest clause
			 * Define SQL92-style Null test clause.
			 * Allow two forms described in the standard:
			 *	a IS NULL
			 *	a IS NOT NULL
			 * Allow two SQL extensions
			 *	a ISNULL
			 *	a NOTNULL
			 */
			| a_expr ISNULL
				
			| a_expr IS NULL_P
				
			| a_expr NOTNULL
				
			| a_expr IS NOT NULL_P
				
			| a_expr IS TRUE_P
				
			| a_expr IS NOT TRUE_P
				
			| a_expr IS FALSE_P
				
			| a_expr IS NOT FALSE_P
				
			| a_expr IS UNKNOWN
				
			| a_expr IS NOT UNKNOWN
				
			| a_expr IS DISTINCT FROM a_expr			%prec IS
				
			| a_expr IS OF '(' type_list ')'			%prec IS
				
			| a_expr IS NOT OF '(' type_list ')'		%prec IS
				
			| a_expr BETWEEN b_expr AND b_expr			%prec BETWEEN
				
			| a_expr NOT BETWEEN b_expr AND b_expr		%prec BETWEEN
				
			| a_expr IN_P in_expr
				
			| a_expr NOT IN_P in_expr
				
			| a_expr qual_all_Op sub_type select_with_parens %prec Op
				
			| a_expr qual_all_Op sub_type '(' a_expr ')' %prec Op
				
			| UNIQUE select_with_parens %prec Op
				
			| r_expr
				
		;

/*
 * Restricted expressions
 *
 * b_expr is a subset of the complete expression syntax defined by a_expr.
 *
 * Presently, AND, NOT, IS, and IN are the a_expr keywords that would
 * cause trouble in the places where b_expr is used.  For simplicity, we
 * just eliminate all the boolean-keyword-operator productions from b_expr.
 */
b_expr:		c_expr
				
			| b_expr TYPECAST Typename
				
			| '+' b_expr					%prec UMINUS
				
			| '-' b_expr					%prec UMINUS
				
			| '%' b_expr
				
			| '^' b_expr
				
			| b_expr '%'
				
			| b_expr '^'
				
			| b_expr '+' b_expr
				
			| b_expr '-' b_expr
				
			| b_expr '*' b_expr
				
			| b_expr '/' b_expr
				
			| b_expr '%' b_expr
				
			| b_expr '^' b_expr
				
			| b_expr '<' b_expr
				
			| b_expr '>' b_expr
				
			| b_expr '=' b_expr
				
			| b_expr qual_Op b_expr				%prec Op
				
			| qual_Op b_expr					%prec Op
				
			| b_expr qual_Op					%prec POSTFIXOP
				
			| b_expr IS DISTINCT FROM b_expr	%prec IS
				
			| b_expr IS OF '(' type_list ')'	%prec IS
				
			| b_expr IS NOT OF '(' type_list ')'	%prec IS
				
		;

/*
 * Productions that can be used in both a_expr and b_expr.
 *
 * Note: productions that refer recursively to a_expr or b_expr mostly
 * cannot appear here.	However, it's OK to refer to a_exprs that occur
 * inside parentheses, such as function arguments; that cannot introduce
 * ambiguity to the b_expr syntax.
 */
c_expr:		columnref								
			| AexprConst							
			| PARAM attrs opt_indirection
				
			| '(' a_expr ')' attrs opt_indirection
				
			| '(' a_expr ')' opt_indirection
				
			| case_expr
				
			| func_name '(' ')'
				
			| func_name '(' expr_list ')'
				
			| func_name '(' ALL expr_list ')'
				
			| func_name '(' DISTINCT expr_list ')'
				
			| func_name '(' '*' ')'
				
			| CURRENT_DATE
				
			| CURRENT_TIME
				
			| CURRENT_TIME '(' Iconst ')'
				
			| CURRENT_TIMESTAMP
				
			| CURRENT_TIMESTAMP '(' Iconst ')'
				
			| LOCALTIME
				
			| LOCALTIME '(' Iconst ')'
				
			| LOCALTIMESTAMP
				
			| LOCALTIMESTAMP '(' Iconst ')'
				
			| CURRENT_USER
				
			| SESSION_USER
				
			| USER
				
			| CAST '(' a_expr AS Typename ')'
				
			| EXTRACT '(' extract_list ')'
				
			| OVERLAY '(' overlay_list ')'
				
			| POSITION '(' position_list ')'
				
			| SUBSTRING '(' substr_list ')'
				
			| TREAT '(' a_expr AS Typename ')'
				
			| TRIM '(' BOTH trim_list ')'
				
			| TRIM '(' LEADING trim_list ')'
				
			| TRIM '(' TRAILING trim_list ')'
				
			| TRIM '(' trim_list ')'
				
			| CONVERT '(' a_expr USING any_name ')'
				
			| CONVERT '(' expr_list ')'
				
			| select_with_parens			%prec UMINUS
				
			| EXISTS select_with_parens
				
			| ARRAY select_with_parens
				
			| ARRAY array_expr
				
		;

any_name:       ColId
                | dotted_name
                ; 

/*
 * Supporting nonterminals for expressions.
 */

opt_indirection:
			opt_indirection '[' a_expr ']'
				
			| opt_indirection '[' a_expr ':' a_expr ']'
				
			| /*EMPTY*/
				
		;

expr_list:	a_expr
				
			| expr_list ',' a_expr
				
		;

extract_list:
			extract_arg FROM a_expr
				
			| /*EMPTY*/								
		;

type_list:  type_list ',' Typename
				
			| Typename
				
		;

array_expr_list: array_expr
				
			| array_expr_list ',' array_expr
				
		;

array_expr: '[' expr_list ']'
				
			| '[' array_expr_list ']'
				
		;

/* Allow delimited string SCONST in extract_arg as an SQL extension.
 * - thomas 2001-04-12
 */

extract_arg:
			IDENT									
			| YEAR_P								
			| MONTH_P								
			| DAY_P									
			| HOUR_P								
			| MINUTE_P								
			| SECOND_P								
			| SCONST								
		;

/* OVERLAY() arguments
 * SQL99 defines the OVERLAY() function:
 * o overlay(text placing text from int for int)
 * o overlay(text placing text from int)
 */
overlay_list:
			a_expr overlay_placing substr_from substr_for
				
			| a_expr overlay_placing substr_from
				
		;

overlay_placing:
			PLACING a_expr
				
		;

/* position_list uses b_expr not a_expr to avoid conflict with general IN */

position_list:
			b_expr IN_P b_expr						
			| /*EMPTY*/								
		;

/* SUBSTRING() arguments
 * SQL9x defines a specific syntax for arguments to SUBSTRING():
 * o substring(text from int for int)
 * o substring(text from int) get entire string from starting point "int"
 * o substring(text from pattern) get entire string matching pattern
 * o substring(text for int) get first "int" characters of string
 * We also want to implement generic substring functions which accept
 * the usual generic list of arguments. So we will accept both styles
 * here, and convert the SQL9x style to the generic list for further
 * processing. - thomas 2000-11-28
 */
substr_list:
			a_expr substr_from substr_for
				
			| a_expr substr_for substr_from
				
			| a_expr substr_from
				
			| a_expr substr_for
				
			| expr_list
				
			| /*EMPTY*/
				
		;

substr_from:
			FROM a_expr								
		;

substr_for: FOR a_expr								
		;

trim_list:	a_expr FROM expr_list					
			| FROM expr_list						
			| expr_list								
		;

in_expr:	select_with_parens
				
			| '(' expr_list ')'						
		;

/* Case clause
 * Define SQL92-style case clause.
 * Allow all four forms described in the standard:
 * - Full specification
 *	CASE WHEN a = b THEN c ... ELSE d END
 * - Implicit argument
 *	CASE a WHEN b THEN c ... ELSE d END
 * - Conditional NULL
 *	NULLIF(x,y)
 *	same as CASE WHEN x = y THEN NULL ELSE x END
 * - Conditional substitution from list, use first non-null argument
 *	COALESCE(a,b,...)
 * same as CASE WHEN a IS NOT NULL THEN a WHEN b IS NOT NULL THEN b ... END
 * - thomas 1998-11-09
 * 
 * NULLIF and COALESCE have become first class nodes to
 * prevent double evaluation of arguments.
 * - Kris Jurka 2003-02-11
 */
case_expr:	CASE case_arg when_clause_list case_default END_P
				
			| NULLIF '(' a_expr ',' a_expr ')'
				
			| COALESCE '(' expr_list ')'
				
		;

when_clause_list:
			/* There must be at least one */
			when_clause								
			| when_clause_list when_clause			
		;

when_clause:
			WHEN a_expr THEN a_expr
				
		;

case_default:
			ELSE a_expr								
			| /*EMPTY*/								
		;

case_arg:	a_expr									
			| /*EMPTY*/								
		;

/*
 * columnref starts with relation_name not ColId, so that OLD and NEW
 * references can be accepted.	Note that when there are more than two
 * dotted names, the first name is not actually a relation name...
 */
columnref:	relation_name opt_indirection
				
			| dotted_name opt_indirection
				
		;

dotted_name:
			relation_name attrs
					
		;

attrs:		'.' attr_name
					
			| '.' '*'
					
			| '.' attr_name attrs
					
		;


/*****************************************************************************
 *
 *	target lists
 *
 *****************************************************************************/

/* Target lists as found in SELECT ... and INSERT VALUES ( ... ) */

target_list:
			target_el								
			| target_list ',' target_el				
		;

/* AS is not optional because shift/red conflict with unary ops */
target_el:	a_expr AS ColLabel
				
			| a_expr
				
			| '*'
				
		;

/* Target list as found in UPDATE table SET ...
| '(' row_ ')' = '(' row_ ')'
{
	$$ = NULL;
}
 */
				

/*****************************************************************************
 *
 *	Names and constants
 *
 *****************************************************************************/

relation_name:
			SpecialRuleRelation						
			| ColId									
		;

		;

name_list:	name
					
			| name_list ',' name
					
		;


name:		ColId									;


attr_name:	ColId									;

func_name:	function_name
					
			| dotted_name							
		;


/*
 * Constants
 */
AexprConst: Iconst
				
			| FCONST
				
			| Sconst
				
			| BCONST
				
			| XCONST
				
			| ConstTypename Sconst
				
			| ConstInterval Sconst opt_interval
				
			| ConstInterval '(' Iconst ')' Sconst opt_interval
				
			| PARAM opt_indirection
				
			| TRUE_P
				
			| FALSE_P
				
			| NULL_P
				
		;

Iconst:		ICONST									;
Sconst:		SCONST									;

/*
 * Name classification hierarchy.
 *
 * IDENT is the lexeme returned by the lexer for identifiers that match
 * no known keyword.  In most cases, we can accept certain keywords as
 * names, not only IDENTs.	We prefer to accept as many such keywords
 * as possible to minimize the impact of "reserved words" on programmers.
 * So, we divide names into several possible classes.  The classification
 * is chosen in part to make keywords acceptable as names wherever possible.
 */

/* Column identifier --- names that can be column, table, etc names.
 */
ColId:		IDENT									
			| unreserved_keyword					
			| col_name_keyword						
		;

/* Type identifier --- names that can be type names.
 */
type_name:	IDENT									
			| unreserved_keyword					
		;

/* Function identifier --- names that can be function names.
 */
function_name:
			IDENT									
			| unreserved_keyword					
			| func_name_keyword						
		;

/* Column label --- allowed labels in "AS" clauses.
 * This presently includes *all* Postgres keywords.
 */
ColLabel:	IDENT									
			| unreserved_keyword					
			| col_name_keyword						
			| func_name_keyword						
			| reserved_keyword						
		;


/*
 * Keyword classification lists.  Generally, every keyword present in
 * the Postgres grammar should appear in exactly one of these lists.
 *
 * Put a new keyword into the first list that it can go into without causing
 * shift or reduce conflicts.  The earlier lists define "less reserved"
 * categories of keywords.
 */

/* "Unreserved" keywords --- available for use as any kind of name.
 */
unreserved_keyword:
			  ABORT_P
			| ABSOLUTE_P
			| ACCESS
			| ACTION
			| ADD
			| AFTER
			| AGGREGATE
			| ALTER
			| ASSERTION
			| ASSIGNMENT
			| AT
			| BACKWARD
			| BEFORE
			| BEGIN_P
			| BY
			| CACHE
			| CALLED
			| CASCADE
			| CHAIN
			| CHARACTERISTICS
			| CHECKPOINT
			| CLASS
			| CLOSE
			| CLUSTER
			| COMMENT
			| COMMIT
			| COMMITTED
			| CONSTRAINTS
			| CONVERSION_P
			| COPY
			| CREATEDB
			| CREATEUSER
			| CURSOR
			| CYCLE
			| DATABASE
			| DAY_P
			| DEALLOCATE
			| DECLARE
			| DEFAULTS
			| DEFERRED
			| DEFINER
			| DELETE_P
			| DELIMITER
			| DELIMITERS
			| DOMAIN_P
			| DOUBLE_P
			| DROP
			| EACH
			| ENCODING
			| ENCRYPTED
			| ESCAPE
			| EXCLUDING
			| EXCLUSIVE
			| EXECUTE
			| EXPLAIN
			| EXTERNAL
			| FETCH
			| FIRST_P
			| FORCE
			| FORWARD
			| FUNCTION
			| GLOBAL
			| HANDLER
			| HOLD
			| HOUR_P
			| IMMEDIATE
			| IMMUTABLE
			| IMPLICIT_P
			| INCLUDING
			| INCREMENT
			| INDEX
			| INHERITS
			| INOUT
			| INPUT_P
			| INSENSITIVE
			| INSERT
			| INSTEAD
			| INVOKER
			| ISOLATION
			| KEY
			| LANCOMPILER
			| LANGUAGE
			| LAST_P
			| LEVEL
			| LISTEN
			| LOAD
			| LOCAL
			| LOCATION
			| LOCK_P
			| MATCH
			| MAXVALUE
			| MINUTE_P
			| MINVALUE
			| MODE
			| MONTH_P
			| MOVE
			| NAMES
			| NATIONAL
			| NEXT
			| NO
			| NOCREATEDB
			| NOCREATEUSER
			| NOTHING
			| NOTIFY
			| OF
			| OIDS
			| OPERATOR
			| OPTION
			| OUT_P
			| OWNER
			| PARTIAL
			| PASSWORD
			| PATH_P
			| PENDANT
			| PRECISION
			| PREPARE
			| PRESERVE
			| PRIOR
			| PRIVILEGES
			| PROCEDURAL
			| PROCEDURE
			| READ
			| RECHECK
			| REINDEX
			| RELATIVE_P
			| RENAME
			| REPLACE
			| RESET
			| RESTART
			| RESTRICT
			| RETURNS
			| REVOKE
			| ROLLBACK
			| ROWS
			| RULE
			| SCHEMA
			| SCROLL
			| SECOND_P
			| SECURITY
			| SEQUENCE
			| SERIALIZABLE
			| SESSION
			| SET
			| SHARE
			| SHOW
			| SIMPLE
			| STABLE
			| START
			| STATEMENT
			| STATISTICS
			| STDIN
			| STDOUT
			| STORAGE
			| SYSID
			| STRICT_P
			| TEMP
			| TEMPLATE
			| TEMPORARY
			| TOAST
			| TRANSACTION
			| TRIGGER
			| TRUNCATE
			| TRUSTED
			| TYPE_P
			| UNENCRYPTED
			| UNKNOWN
			| UNLISTEN
			| UNTIL
			| UPDATE
			| USAGE
			| VACUUM
			| VALID
			| VALIDATOR
			| VALUES
			| VARYING
			| VERSION
			| VIEW
			| VOLATILE
			| WITH
			| WITHOUT
			| WORK
			| WRITE
			| YEAR_P
			| ZONE
		;

/* Column identifier --- keywords that can be column, table, etc names.
 *
 * Many of these keywords will in fact be recognized as type or function
 * names too; but they have special productions for the purpose, and so
 * can't be treated as "generic" type or function names.
 *
 * The type names appearing here are not usable as function names
 * because they can be followed by '(' in typename productions, which
 * looks too much like a function call for an LR(1) parser.
 */
col_name_keyword:
			  BIGINT
			| BIT
			| BOOLEAN_P
			| CHAR_P
			| CHARACTER
			| COALESCE
			| CONVERT
			| DEC
			| DECIMAL_P
			| EXISTS
			| EXTRACT
			| FLOAT_P
			| INT_P
			| INTEGER
			| INTERVAL
			| NCHAR
			| NONE
			| NULLIF
			| NUMERIC
			| OVERLAY
			| POSITION
			| REAL
			| ROW
			| SETOF
			| SMALLINT
			| SUBSTRING
			| TIME
			| TIMESTAMP
			| TREAT
			| TRIM
			| VARCHAR
		;

/* Function identifier --- keywords that can be function names.
 *
 * Most of these are keywords that are used as operators in expressions;
 * in general such keywords can't be column names because they would be
 * ambiguous with variables, but they are unambiguous as function identifiers.
 *
 * Do not include POSITION, SUBSTRING, etc here since they have explicit
 * productions in a_expr to support the goofy SQL9x argument syntax.
 * - thomas 2000-11-28
 */
func_name_keyword:
			  AUTHORIZATION
			| BETWEEN
			| BINARY
			| CROSS
			| FREEZE
			| FULL
			| ILIKE
			| IN_P
			| INNER_P
			| IS
			| ISNULL
			| JOIN
			| LEFT
			| LIKE
			| NATURAL
			| NOTNULL
			| OUTER_P
			| OVERLAPS
			| RIGHT
			| SIMILAR
			| VERBOSE
		;

/* Reserved keyword --- these keywords are usable only as a ColLabel.
 *
 * Keywords appear here if they could not be distinguished from variable,
 * type, or function names in some contexts.  Don't put things here unless
 * forced to.
 */
reserved_keyword:
			  ALL
			| ANALYSE
			| ANALYZE
			| AND
			| ANY
			| ARRAY
			| AS
			| ASC
			| BOTH
			| CASE
			| CAST
			| CHECK
			| COLLATE
			| COLUMN
			| CONSTRAINT
			| CREATE
			| CURRENT_DATE
			| CURRENT_TIME
			| CURRENT_TIMESTAMP
			| CURRENT_USER
			| DEFAULT
			| DEFERRABLE
			| DESC
			| DISTINCT
			| DO
			| ELSE
			| END_P
			| EXCEPT
			| FALSE_P
			| FOR
			| FOREIGN
			| FROM
			| GRANT
			| GROUP_P
			| HAVING
			| INITIALLY
			| INTERSECT
			| INTO
			| LEADING
			| LIMIT
			| LOCALTIME
			| LOCALTIMESTAMP
			| NEW
			| NOT
			| NULL_P
			| OFF
			| OFFSET
			| OLD
			| ON
			| ONLY
			| OR
			| ORDER
			| PLACING
			| PRIMARY
			| REFERENCES
			| SELECT
			| SESSION_USER
			| SOME
			| TABLE
			| THEN
			| TO
			| TRAILING
			| TRUE_P
			| UNION
			| UNIQUE
			| USER
			| USING
			| WHEN
			| WHERE
		;


SpecialRuleRelation:
			OLD
				
			| NEW
				
		;

%%
