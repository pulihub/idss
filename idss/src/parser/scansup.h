/*-------------------------------------------------------------------------
 *
 * scansup.h
 *	  scanner support routines.  used by both the bootstrap lexer
 * as well as the normal lexer
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
 *-------------------------------------------------------------------------
 */

#ifndef SCANSUP_H
#define SCANSUP_H

extern char *scanstr(const char *s);

extern char *downcase_truncate_identifier(const char *ident, int len, int warn);

extern void truncate_identifier(char *ident, int len, int warn);

#endif   /* SCANSUP_H */
