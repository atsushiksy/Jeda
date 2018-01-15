/* some part of Perl5 handy.h following is the original Copyright message */

/*    handy.h
 *
 *    Copyright (c) 1991-1997, Larry Wall
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */
 

#define isALNUM(c)	(isALPHA(c) || isDIGIT(c) || (c) == '_')
#define isIDFIRST(c)	(isALPHA(c) || (c) == '_')
#define isALPHA(c)	(isUPPER(c) || isLOWER(c))
#define isSPACE(c) \
	((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) =='\r' || (c) == '\f')
#define isDIGIT(c)	((c) >= '0' && (c) <= '9')
#define isUPPER(c)	((c) >= 'A' && (c) <= 'Z')
#define isLOWER(c)	((c) >= 'a' && (c) <= 'z')
#define isPRINT(c)	(((c) > 32 && (c) < 127) || isSPACE(c))
#define toUPPER(c)	(isLOWER(c) ? (c) - ('a' - 'A') : (c))
#define toLOWER(c)	(isUPPER(c) ? (c) + ('a' - 'A') : (c))

/* This conversion works both ways, strangely enough. */
#define toCTRL(c)    (toUPPER(c) ^ 64)


/* ISMULT2 is from perl5 regcomp.c, and modified */
#define	ISMULT2(s)	((*s) == '*' || (*s) == '+' || (*s) == '?')
