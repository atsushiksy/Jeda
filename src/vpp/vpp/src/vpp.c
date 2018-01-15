/* Copyright (C) 1996 Himanshu M. Thaker

This file is part of vpp.

Vpp is distributed in the hope that it will be useful,
but without any warranty.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.

Everyone is granted permission to copy, modify and redistribute
vpp, but only under the conditions described in the
document "vpp copying permission notice".   An exact copy
of the document is supposed to have been given to you along with
vpp so that you can know how you may redistribute it all.
It should be in a file named COPYING.  Among other things, the
copyright notice and this notice must be preserved on all copies.  */

/*
 * $Header:$
 * Program :
 * Author : Himanshu M. Thaker
 * Date : Apr. 16, 1995
 * Description :
 * $Log:$
 * Revision 1.3  1995/04/21  18:11:58  hemi
 * *** empty log message ***
 *
 * Revision 1.3  1995/04/21  18:11:58  hemi
 * *** empty log message ***
 *
 * Revision 1.2  1995/04/18  00:23:43  hemi
 * *** empty log message ***
 *
 * Revision 1.1  1995/04/16  19:38:15  hemi
 * Initial revision
 */


/*
 * if you want debugging inforamation, uncomment the #define's below
 */
/*#define DEBUG1*/
/*#define DEBUG2*/

/*
 * some function definitions, that are used in this file. 
 */


/*
 * include files...
 */
#include "common.h"
#include "y.tab.c"
#include <stdio.h>

/*
 * my defn's start here
 */
extern FILE *yyin;
extern char *current_file;
extern int dont_add_comments;
/*
extern char *yytext;
*/

/*
 * my defn's end here
 */

main(argc, argv)
int argc;
char *argv[];
{
  FILE *fopen();
  char fname[80];
  int i;

/*
  yydebug = TRUE;
*/

/*
  dont_add_comments = TRUE;
*/

  /*
   * check to see if there are enough arguments...
   */
  if (argc < 2)
    {
/*
      printf("ERROR : invalid usage\n");
      printf("Usage : vip <verilog_file>\n");
      exit (-1);
*/
	yyin = stdin;
	while (1) yyparse();
    }
  /*
   * check to see if the file can be opened for reading...
   */
  for(i=1; i<argc; i++) {
    nl_count = 1;
    fprintf(stderr,"Parsing '%s'\n",argv[i]);
    if ((yyin = fopen(argv[i], "r")) == NULL) {
      fprintf(stderr,"ERROR : could not open file %s\n", argv[i]);
    }
    else {
      current_file = NMALLOC(strlen(argv[i])+1,char);
      strcpy(current_file, argv[i]);
      yyparse();
      fclose(yyin);
      free(current_file);
    }
  }
}

/*
 *--------------------------------------------------------------------------
 *       the routines below are required by lex and yacc, an explanation of
 * which can be found in the manuals.
 *--------------------------------------------------------------------------
 */

/*
yyerror(s)
char *s;
{
    fprintf(stderr,"vip : %s\n",s);
    fprintf(stderr,"vip : last yytext='%s'\n",yytext);
}
*/

yyerror(s)
char *s;
{
    int len_tok; /* length of yytext */
    int len_line; /* length of current line */
    int i;

    fprintf(stderr,"%5d:%s\n",nl_count-1,old_line);
    fprintf(stderr,"%5d:%s\n",nl_count,current_line);
    len_tok = strlen(yytext);
    len_line = strlen(current_line);
    if ((len_line-len_tok) > 0) {
	for (i=0; i<(len_line-len_tok-1+7); i++) fprintf(stderr," ");
	
	for (i=len_line-len_tok; i<len_line; i++) fprintf(stderr,"^");
	fprintf(stderr,"\n");
    }
    fprintf(stderr,"%s\n\tfile '%s'",s, current_file);
    fprintf(stderr," near line %d\n",nl_count);
}

yywrap ()
{
    return (1);
}

