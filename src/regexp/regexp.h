/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
/* #define NSUBEXP  10 */
typedef struct regexp {
        /* chenged to pointer to be dynamically allocated.
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	*/
	char **startp;
	char **endp;
	int  nparens;           /* number of parenthesis */
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *JD_regcomp(char *);
extern int JD_regexec();
extern void JD_regdealloc();

extern void regsub();
extern void JD_regerror();
