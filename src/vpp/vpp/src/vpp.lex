%p 6000
%e 2000
%{

#include "common.h"
#include <math.h>

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
 * $Log:$
 * Revision 1.8  1995/04/23  17:18:12  hemi
 * All features except string handling.
 */


typedef struct names
   {
        char *macro_name;
        char *macro;
	int is_number;
	double value;
        struct names *next;
   } names;

gen_ll_mgr *code_pointer = NULL;
char line[MAXNAME];
int last_mode = INITIAL;
names *Defines_Table[BASE];
int nl_count = 1;

names *lookup_define();
char *full_lookup() ;
#define MAX_INCLUDE_DEPTH 8
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_nl_count[MAX_INCLUDE_DEPTH];
char *include_fname[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

#define YY_USER_ACTION store_lines();
char old_line[MAXNAME];
char current_line[MAXNAME];
#define COMMENT_EVERY 10

char *current_file;
int dont_add_comments = TRUE;

#define PP 0
#define MM 1
#define PE 2
#define ME 3

%}

%x MPP
%x COPY
%x INCL
%x SKIP_TO_EOL

WHITE           [ \t]
NEWLINE		[\n]
DIGIT10         [0-9_]
NMBR10          {DIGIT10}+
NAME            [a-zA-Z_][a-zA-Z0-9_\$]*
MEM             \[[^;\n\]]*\]

%%

<INITIAL,MPP>"/*"  {eat_up_comment(TRUE);}
<COPY>"/*"         {eat_up_comment(TRUE);}
		    
<INITIAL,MPP>"//".*$         { /* printf("%s",yytext);*/ }
<COPY>"//".*$                {/*store_it(yytext,FALSE,FALSE);*/}

<COPY>`accelerate		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`noaccelerate		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`autoexpand_vectomets	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`celldefine		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`endcelldefine		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`default_decay_time	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`default_nettype		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`default_rswitch_strength	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`default_switch_strength	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`default_trireg_strength	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`delay_mode_distributed	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`delay_mode_path		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`delay_mode_unit		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`delay_mode_zero		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`expand_vetornets		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`noexpand_vectornets	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`protect			{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`endprotect		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`pre_16a_paths		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`end_pre_16a_paths	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`protected		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`unprotected		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`remove_gatenames		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`noremove_gatenames	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`resetall			{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`rs_technology		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>"`switch default"		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>"`switch resisitive"	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>"`switch XL"		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`timescale		{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`unconnected_drive	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`nounconnected_drive	{/*store_it(yytext,FALSE,FALSE);*/}
<COPY>`uselib			{/*store_it(yytext,FALSE,FALSE);*/}

<INITIAL>`accelerate[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`noaccelerate[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`autoexpand_vectomets[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`celldefine[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`endcelldefine[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`default_decay_time[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`default_nettype[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`default_rswitch_strength[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`default_switch_strength[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`default_trireg_strength[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`delay_mode_distributed[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`delay_mode_path[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`delay_mode_unit[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`delay_mode_zero[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`expand_vetornets[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`noexpand_vectornets[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`protect[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`endprotect[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`pre_16a_paths[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`end_pre_16a_paths[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`unprotected[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`remove_gatenames[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`noremove_gatenames[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`resetall[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`rs_technology[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>"`switch default"[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>"`switch resisitive"[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>"`switch XL"[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`timescale[^\n]*		{/*printf("%s",yytext);*/}
<INITIAL>`unconnected_drive[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`nounconnected_drive[^\n]*	{/*printf("%s",yytext);*/}
<INITIAL>`uselib[^\n]*		{/*printf("%s",yytext);*/}

<*>^{WHITE}*"`ifdef"	{return TTIFDEF;}
<*>^{WHITE}*"`ifndef"	{return TTIFNDEF;}
<*>^{WHITE}*"`if"	{return TTIF;}
<*>^{WHITE}*"`let"	{return TTLET;}
<*>^{WHITE}*"`for"	{return TTFOR;}
<*>^{WHITE}*"`while"	{return TTWHILE;}
<*>^{WHITE}*"`switch"	{return TTSWITCH;}
<*>^{WHITE}*"`case"	{return TTCASE;}
<*>^{WHITE}*"`default"	{return TTDEFAULT;}
<*>^{WHITE}*`breaksw[^\n]* {return TTBREAKSW;}
<*>^{WHITE}*`else[^\n]* {return TTELSE;}
<*>^{WHITE}*`endif[^\n]* {return TTENDIF;}
<*>^{WHITE}*`endfor[^\n]* {return TTENDFOR;}
<*>^{WHITE}*`endwhile[^\n]* {return TTENDWHILE;}
<*>^{WHITE}*`endswitch[^\n]* {return TTENDSWITCH;}

<INITIAL>`define[^\n]* {/*printf("%s",yytext);*/store_define(yytext);}
<COPY>`define[^\n]* {/*store_it(yytext,FALSE, FALSE);*/store_define(yytext);}

<INITIAL>`undef[^\n]* {undef_macro(yytext);/*printf("%s",yytext);*/}
<COPY>`undef[^\n]* {undef_macro(yytext);/*store_it(yytext,FALSE, FALSE);*/}

<INITIAL,COPY>`include	   {BEGIN(INCL);}

<INITIAL,COPY,MPP>{NAME}{WHITE}*{MEM}?{WHITE}*"++" {do_post_command(yytext,PP);}
<INITIAL,COPY,MPP>{NAME}{WHITE}*{MEM}?{WHITE}*"--" {do_post_command(yytext,MM);}
<INITIAL,COPY,MPP>{NAME}{WHITE}*{MEM}?{WHITE}*"+=" {do_post_command(yytext,PE);}
<INITIAL,COPY,MPP>{NAME}{WHITE}*{MEM}?{WHITE}*"-=" {do_post_command(yytext,ME);}

<INCL>[ \t"]*	   {}
<INCL>[^ \"\t\n]+  {
		    FILE *fp;

		    if (include_stack_ptr >= MAX_INCLUDE_DEPTH) {
		      fprintf(stderr, "ERROR : lex : includes nested too deep\n");
		      exit (1);
		    }
		    fp = fopen(yytext, "r");
		    if (!fp) {
		      fprintf(stderr, "ERROR : lex : could not open file '%s'\n",yytext);
		      BEGIN(SKIP_TO_EOL);
		    }
		    else {
		      include_nl_count[include_stack_ptr] = nl_count;
		      include_fname[include_stack_ptr] = NMALLOC(strlen(current_file)+1, char);
                      strcpy(include_fname[include_stack_ptr], current_file);
                      free(current_file);
                      current_file = NMALLOC(strlen(yytext)+1,char);
                      strcpy(current_file, yytext);

		      include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;

		      nl_count = 1;
		      yyin = fp;
		      yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                      do_comment_count(TRUE,nl_count);
		      BEGIN(INITIAL);
		    }
		}
<SKIP_TO_EOL>.*$	{BEGIN(INITIAL);}

<<EOF>>		{
		  if (--include_stack_ptr < 0) yyterminate();
		  else {
		    yy_delete_buffer(YY_CURRENT_BUFFER);
		    yy_switch_to_buffer(include_stack[include_stack_ptr]);
		    nl_count = include_nl_count[include_stack_ptr];
                    free(current_file);
                    current_file = include_fname[include_stack_ptr];
                    do_comment_count(TRUE, nl_count);
		    BEGIN(SKIP_TO_EOL);
		  }
		}

<MPP>"LOG2"	{return TTLOG2;}
<MPP>"ROUND"	{return TTROUND;}
<MPP>"CEIL"	{return TTCEIL;}
<MPP>"FLOOR"	{return TTFLOOR;}
<MPP>"MAX"	{return TTMAX;}
<MPP>"MIN"	{return TTMIN;}
<MPP>"ABS"	{return TTABS;}
<MPP>"STOI"	{return TTSTOI;}
<MPP>"ITOS"	{return TTITOS;}
<MPP>"SUBSTR"	{return TTSUBSTR;}
<MPP>"SYSTEM"	{return TTSYSTEM;}
<MPP>"EVEN"	{return TTEVEN;}
<MPP>"ODD"	{return TTODD;}

<INITIAL>`{NAME}  {printf("%s", full_lookup(yytext+1));}
<COPY>`{NAME}  {store_it(full_lookup(yytext+1),TRUE,TRUE);}

<INITIAL>{WHITE}+ {strcpy(ttid, yytext); return TTWHITE;}
<INITIAL,MPP>{NAME}	{strcpy(ttid, yytext); return TTNAME;}

<INITIAL>\\::  {printf("::");}
<COPY>\\::  {store_it("::",FALSE,FALSE);}

<*>::    {}

<INITIAL,MPP>{NMBR10}	{strcpy(ttid, yytext); return TTNUM;}
<INITIAL,MPP>{DIGIT10}*\.{DIGIT10}+ {strcpy(ttid, yytext);return TTREALNUM;}
<INITIAL,MPP>{DIGIT10}+\.{DIGIT10}* {strcpy(ttid, yytext);return TTREALNUM;}

<MPP>\"[^\"\n]*[\"]?  {strcpy(ttid,yytext); return TTQS;}
<MPP>{WHITE}+ 	{}
<MPP>{NEWLINE} 	{nl_count++; return '\n';}
<MPP>"`"	{}
<MPP>":"	{return ':';}
<MPP>"("	{return '(';}
<MPP>")"	{return ')';}
<MPP>"+"	{return '+';}
<MPP>"-"	{return '-';}
<MPP>"*"	{return '*';}
<MPP>"/"	{return '/';}
<MPP>"%"	{return '%';}
<MPP>"**"	{return TT_POWER;}
<MPP>"=="	{return TTL_EQ;}
<MPP>"!="	{return TTL_NEQ;}
<MPP>"&&"	{return TTL_AND;}
<MPP>"||"	{return TTL_OR;}
<MPP>"<"	{return '<';}
<MPP>"<="	{return TTL_LEQ;}
<MPP>">"	{return '>';}
<MPP>">="	{return TTL_GEQ;}
<MPP>"&"	{return '&';}
<MPP>"|"	{return '|';}
<MPP>"^"	{return '^';}
<MPP>"<<"	{return TTL_SHIFTL;}
<MPP>">>"	{return TTL_SHIFTR;}
<MPP>"!"	{return '!';}
<MPP>"="	{return '=';}
<MPP>";"	{return ';';}
<MPP>"~"	{return '~';}
<MPP>","	{return ',';}
<MPP>.		{return TT_UNKNOWN;}

<COPY>{NEWLINE}	{nl_count++; store_it(yytext,FALSE,FALSE);}
<COPY>.		{store_it(yytext,FALSE,FALSE);}
<INITIAL>{NEWLINE} {nl_count++; printf("\n"); do_comment_count(FALSE,nl_count);}
<INITIAL>.	{printf("%c",yytext[0]);}

%%

int set_to_vpp_mode()
{
    last_mode = YY_START;
    BEGIN(MPP);
}

int set_to_initial_mode()
{
    last_mode = YY_START;
    BEGIN(INITIAL);
}

int set_to_copy()
{
/*printf("about to begin COPY\n");*/
    last_mode = YY_START;
    BEGIN(COPY);
}

int set_to_last_mode()
{

    BEGIN(last_mode);
}

int store_it(str,subst,expand_define)
char *str;
int subst;
int expand_define;
{
    char nn[MAXSTR];
    int len;
    int i;
    names *np;
    var_values *vvp;

    if ((strcmp(str,"\n") != 0) && (!subst)) {
	sprintf(line, "%s%s", line,str);
    }
    else {
        if ((is_keyword(str+1) && subst)) printf("%s",str);
	else if (!subst) sprintf(line, "%s\n",line);
	if (code_pointer == NULL) {
	    code_pointer = MALLOC(gen_ll_mgr);
	    code_pointer->head = NULL;
	    code_pointer->tail = NULL;
	}
	if (code_pointer->head == NULL) {
	    code_pointer->head = MALLOC(gen_ll);
	    code_pointer->head->next = NULL;
	    code_pointer->head->prev = NULL;
	    code_pointer->tail = code_pointer->head;
	}
	else {
	    code_pointer->tail->next = MALLOC(gen_ll);
	    code_pointer->tail->next->prev = code_pointer->tail;
	    code_pointer->tail = code_pointer->tail->next;
	}
	
	code_pointer->tail->ll_type = CODE_POINTER;
	code_pointer->tail->item = (ll_generic *)NMALLOC(strlen(line)+1,char);
	strcpy(((char *)code_pointer->tail->item), line);
	code_pointer->tail->line_count = nl_count;
	
	code_pointer->tail->next = NULL;
	sprintf(line,"");

	if (subst) {
	    code_pointer->tail->next = MALLOC(gen_ll);
	    code_pointer->tail->next->prev = code_pointer->tail;
	    code_pointer->tail = code_pointer->tail->next;

	    len = strlen(str);
	    /*
	     * First check to see if it a `define.  If it is,
	     * then we DON'T convert it.  Otherwise, we do convert it.
	     */
	    if (np = lookup_define(Defines_Table, str+1,hash(str+1))) {
		if (expand_define) {
		    code_pointer->tail->ll_type = CODE_POINTER;
		    code_pointer->tail->item =
		      (ll_generic *)NMALLOC(strlen(np->macro)+1,char);
		    strcpy(((char *)code_pointer->tail->item),np->macro);
		}
		else {
		    code_pointer->tail->ll_type = CODE_POINTER;
		    code_pointer->tail->item =
		      (ll_generic *)NMALLOC(len+1,char);
		    strcpy(((char*)code_pointer->tail->item), str);
		}
	    }
	    else {
	        if ((vvp = get_variable_ref(STRING,str+1)) == NULL) {
		  sprintf(nn,"ERROR : variable `%s has not been assigned prior to use",str+1);
		  yyerror(nn);
		}
		code_pointer->tail->ll_type = CODE_POINTER2;
		code_pointer->tail->item =
		  (ll_generic *)NMALLOC(len,char); /* note no +1 */
		strcpy(((char *)code_pointer->tail->item), str+1);
	    }
	    code_pointer->tail->line_count = nl_count;
	    code_pointer->tail->next = NULL;
	}
/*
{
gen_ll *tmp;
printf("\n-----------vvvv code so far is \n");
tmp = code_pointer->head;
while (tmp != NULL) {
printf("%s",tmp->item);
tmp = tmp->next;
}
}
printf("\n-----------^^^^\n");
*/
    }
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *    This function assumes that yytext contains %name%, where name is
 * what is to be looked up and substituted with the appropriate integer
 * value, that has either been defined before with "`let", or inside
 * a looping construct.
 *
 **************************************************************************/
int lookup()
{
    char name[MAXSTR];
    int len;
    var_values *vvp;
    int i;

    len = strlen(yytext);


    strcpy(name,yytext+3);
    print_variable(name, TRUE);
}


/**************************************************************************
 *
 * Function Name : int store_define()
 * Parameters :
 *      char *def_text - text of the define statement
 * Purpose :
 *    To add a "`define" into the table.  Note that if a previous entry
 * already exists, it is first removed, and then the new entry superceeds
 * the old one.
 *
 **************************************************************************/
store_define(def_text)
char *def_text;
{
   char *txt_ptr;
   char macro_name[MAXSTR];
   int hashval;

   txt_ptr = (char *)(((int)strstr(def_text, "define")) + ((int )strlen("define")));
   sscanf(txt_ptr, "%s", macro_name);
   txt_ptr = (char *)(((int)strstr(txt_ptr, macro_name)) + ((int)strlen(macro_name)));

   hashval = hash(macro_name);

   /*
    * First, see if the name has previously been stored.
    */
   if (lookup_define(Defines_Table, macro_name, hashval) != NULL) {
/*
       printf("WARNING : lex : redefining '%s', using '%s'\n",macro_name,txt_ptr);
*/
       delete_entry(Defines_Table, macro_name, hashval);
   }
   /*
    * Now, store the name and macro into the table.
    */
   add_macro(Defines_Table, macro_name, hashval, txt_ptr);
}


/**************************************************************************
 *
 * Function Name : int hash()
 * Parameters :
 *      char *text - text you wish to get hash value of
 * Purpose :
 *      This routine sums up the ascii value of the text, mod a BASE value,
 * and returns this value as the hash value.
 *
 **************************************************************************/
int hash(text)
char *text;
{
  int sum;
  int i;

  sum = 0;
  for(i=0; i<strlen(text); i++) sum = sum + text[i];
  return (sum % BASE);
}

char *full_lookup( name )
  char *name ;
{
  char *cp, *p, t, *tp, *nn ;
  char *ret ;
  names *np ;
  int i,j,k ;
  np = lookup_define( Defines_Table, name, hash(name) ) ;
  if( !np ) {
    ret =  (char *)calloc(strlen(name)+5,sizeof(char)) ;
    strcat( ret, "`") ;
    strcat( ret, name) ;
    return ret ;
  }
  cp = (char *)strdup(np->macro) ;
  ret = (char *)calloc(strlen(cp)+5,sizeof(char)) ;
  j = 0 ;
  p = cp ;
  while( *p ) {
    if( *p == '`' && isalpha(p[1]) ) {
      p++ ;
      tp = p ;
      while( *tp && (isalnum(*tp) || *tp=='_') ) tp++ ;
      t = *tp ;
      *tp = '\0' ;
      nn = full_lookup(p) ;
      *tp = t ;
      p = tp ;
      tp = ret ;
      ret = (char *)calloc(strlen(cp)+strlen(nn)+5,sizeof(char)) ;
      strcat(ret, tp) ;
      strcat(ret, nn) ;
      j = strlen(ret) ;
      free(tp) ;
    }
    else {
      ret[j++] = *p ;
      p++ ;
    }
  }
  return ret ;
}

/**************************************************************************
 *
 * Function Name : names *lookup_define()
 * Parameters :
 *      names *table[] - table where names are to be stored in
 *      char *macro_name - name to lookup
 *      int hashval - computed hash value;
 * Return Value :
 *      names * -  NULL if not found, address of a 'names' if found
 * Purpose :
 *    See if the name given is defined in the table.  To make for quick
 * searches, the entries are hashed into a linked list table.
 *
 **************************************************************************/
names *lookup_define(table, macro_name, hashval)
names *table[];
char *macro_name;
int hashval;
{
  names *np;
  for (np = table[hashval]; np != NULL; np=np->next) {
      if (strcmp(macro_name,np->macro_name) == 0) {
        if( np->macro_name[0] == '`' ) {
          np = lookup_define(table, np->macro_name, hash(np->macro_name)) ;
        }
        return(np);
      }
  }
  return(NULL);
}

/**************************************************************************
 *
 * Function Name : int delete_entry()
 * Parameters :
 *      names *table[] - table where names are to be stored in
 *      char *macro_name - name to delete
 *      int hashval - computed hash value;
 * Return Value :
 *      none
 * Purpose :
 *    To delete an entry in a hash'd database.  We may wish to delete
 * a macro for example if we come across a duplicate identical macro def,
 * then the newest one replaces the current one.  Note that this routine
 * simply deletes the entry from the table.  It is up to the user to add
 * a new name, if desired.
 *
 **************************************************************************/
int delete_entry(table, macro_name, hashval)
names *table[];
char *macro_name;
int hashval;
{
  names *np;
  names *last_np;

  for (last_np = np = table[hashval]; np != NULL; np=np->next) {
      if (strcmp(macro_name,np->macro_name) == 0) {
        if (last_np == table[hashval]) table[hashval]->next = np->next;
        else last_np->next = np->next;
        free(np->macro_name);
/*
        free(np->macro);
        free(np);
*/
        return(1);
      }
      last_np = np;
  }
  fprintf(stderr,"WARNING : lex : delete_entry : unable to find entry to delete!\n");
}

/**************************************************************************
 *
 * Function Name : int add_macro()
 * Parameters :
 *      names *table[] - table where names are to be stored in
 *      char *macro_name - name to store macro as
 *      int hashval - computed hash value;
 *      char *text - pointer to an actual copy of macro corresponding to
 *                   macro_name
 * Return Value :
 *      none
 * Purpose :
 *    To add a macro definition into a hash'd database.
 *
 **************************************************************************/
int add_macro(table, macro_name, hashval, text)
names *table[];
char *macro_name;
int hashval;
char *text;
{
    names *table_ptr;
    int i;

    table_ptr = table[hashval];
    /*
     * First, check for "//" and "/*" because comments are legal in a define
     * statement, but should not be included into the expansion
     */
    for (i=0; i<strlen(text); i++) {
	if (text[i] == '/') {
	    if (i < (strlen(text)-1)) {
		if ((text[i+1] == '/') || (text[i+1] == '*')) {
		  text[i] = '\0';
		  break;
		}
	    }
	}
    }
    /*
     * If we are at the head of the index, insert there.
     */
    if (table_ptr == NULL) {
        table[hashval] = table_ptr = MALLOC(names);
    }
    /*
     * search to the end of the linked list and insert there
     */
    else {
        while (table_ptr->next != NULL) table_ptr = table_ptr->next;
        table_ptr->next = MALLOC(names);
        table_ptr = table_ptr->next;
    }
    table_ptr->next = NULL;
    table_ptr->next = NULL;
    table_ptr->macro_name = NMALLOC(strlen(macro_name)+1,char);
    strcpy(table_ptr->macro_name, macro_name);
    table_ptr->macro = NMALLOC(strlen(text)+1,char);
    strcpy(table_ptr->macro, text);
    table_ptr->is_number = ascii2double(table_ptr->macro,&(table_ptr->value));
/*
printf("is_number=%d, value=%7.3f\n",table_ptr->is_number,table_ptr->value);
*/
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
print_variable(name, expand_define)
char *name;
int expand_define;
{
    double v;
    var_values *vvp;
    char strg[MAXSTR];
    double dval;
    int i;

    if (lookup_define(Defines_Table, name, hash(name))) {
	if (expand_define) {
	    printf("%s",lookup_define(Defines_Table, name, hash(name))->macro);
	    /*
	    print_double(lookup_define(Defines_Table, name, hash(name))->value);
	    */
	}
	else printf("`%s",name);
    }
    else if (is_keyword(name)) printf("`%s",name);
    else {
	vvp = get_variable_ref(STRING,name);
	if (vvp == NULL) {
	    sprintf(strg,"ERROR : '%s' not defined",name);
	    yyerror(strg);
	}
	else {
	    if ((vvp->strg)[0] == '"') {
		for (i=1;i<strlen(vvp->strg);i++) {
		    if ((vvp->strg)[i] == '"') break;
		    else printf("%c",(vvp->strg)[i]);
		}
	    }
	    else {
		ascii2double(vvp->strg,&dval);
		print_double(dval);
	    }
	}
    }
}

print_double(value)
double value;
{
    int tmp;
    double tmp_val;

    if (is_an_integer(value)) {
	tmp = (int) rint(value);
	printf("%d",tmp);
    }
    else {
	printf("%f",value);
    }
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 *    none
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
store_lines()
{
    if (strcmp(yytext,"\n") == 0) {
	strcpy(old_line,current_line);
	strcpy(current_line, "");
    }
    else strcat(current_line, yytext);
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
do_comment_count(force_print, count)
int force_print;
int count;
{

    if ((((nl_count % COMMENT_EVERY) == 0) || (force_print)) &&
	!dont_add_comments) {
	printf("// vpp_source %s %d\n",current_file, count);
    }
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *    Given a text followed by either ++ or --, we expand it to 
 * "name = name+1" or "name = name-1".  Note that we unput into the
 * parse stream, so that parsing occurs on the expanded text.
 *
 **************************************************************************/
do_post_command(txt, post_type)
char *txt;
int post_type;
{
    int i;
    int len;
    char tmp[MAXSTR];

    strcpy(tmp, txt);

    len = strlen(tmp);

    if ((post_type == PP) || (post_type == MM)) {
	unput('1');
    }
    if ((post_type == MM) || (post_type == ME)) {
	unput('-');
    }
    else {
	unput('+');
    }
    for (i=(len-3);i>=0;i--){
	unput(tmp[i]);
    }
    unput('=');
    for (i=(len-3);i>=0;i--) {
	unput(tmp[i]);
    }
}


/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
double get_define_value(name)
char *name;
{
    double dval;
    names *np;

    if (np=lookup_define(Defines_Table,name,hash(name))) {
	dval = np->value;
    }
    else dval = 0.0;

    return (dval);
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
int is_keyword(name)
char *name;
{
  if (strcmp(name,"ifdef") == 0) return (TRUE);
  else if (strcmp(name,"ifndef") == 0) return (TRUE);
  else if (strcmp(name,"if") == 0) return (TRUE);
  else if (strcmp(name,"let") == 0) return (TRUE);
  else if (strcmp(name,"for") == 0) return (TRUE);
  else if (strcmp(name,"while") == 0) return (TRUE);
  else if (strcmp(name,"switch") == 0) return (TRUE);
  else if (strcmp(name,"case") == 0) return (TRUE);
  else if (strcmp(name,"default") == 0) return (TRUE);
  else if (strcmp(name,"breaksw") == 0) return (TRUE);
  else if (strcmp(name,"else") == 0) return (TRUE);
  else if (strcmp(name,"endif") == 0) return (TRUE);
  else if (strcmp(name,"endwhile") == 0) return (TRUE);
  else if (strcmp(name,"endswitch") == 0) return (TRUE);
  else if (strcmp(name,"define") == 0) return (TRUE);

  return (FALSE);
}

/**************************************************************************
 *
 * Function Name : int eat_up_comment(fp)
 * Parameters :
 *      FILE *fp;
 * Purpose :
 *      To parse out comments.
 *
 **************************************************************************/
eat_up_comment(to_stack)
int to_stack;
{
    register int c;
    char tmp[MAXNAME];
    register int i;
    
    if (to_stack) store_it("/*",FALSE,FALSE);
    else printf("/*");
    for (;;) {
      while ((c = input()) != '*' && c != EOF) {
	tmp[0] = c; tmp[1]='\0';
	if (to_stack) store_it(tmp,FALSE,FALSE);
	else printf("%c",c);
	if (c == '\n') nl_count++; /* eat up text */
      }
      if (c == '*') {
	tmp[0] = c; tmp[1]='\0';
	if (to_stack) store_it(tmp,FALSE,FALSE);
	else printf("%c",c);
	while ((c = input()) == '*') {
	  tmp[0] = c; tmp[1]='\0';
	  if (to_stack) store_it(tmp,FALSE,FALSE);
	  else printf("%c",c);
	}
	tmp[0] = c; tmp[1]='\0';
	if (to_stack) store_it(tmp,FALSE,FALSE);
	else printf("%c",c);
	if (c == '/') {
	  break; /* found the end of comment */
	}
	if (c == EOF) {
	  printf("EOF in comment\n");
	  break;
	} /* of if we got EOF */
      } /* of if we got a '*' */
    } /* end of for loop */
 } /* end of eat_up_comment() */

/**************************************************************************
 *
 * Function Name : int undef_macro(def_text)
 * Parameters :
 *     char *def_text - text of the macro to undefine
 * Return Value :
 * Purpose :
 *     This function exists to undefine a macro, if it so exists.  Note
 * that no warning message is given if the macro does not exist.
 *
 **************************************************************************/
int undef_macro(def_text)
char *def_text;
{
   char *txt_ptr;
   char macro_name[MAXSTR];
   int hashval;

   txt_ptr = (char *) (((int)strstr(def_text, "undef")) + ((int)(char *)strlen("undef")));
   sscanf(txt_ptr, "%s", macro_name);

   hashval = hash(macro_name);

   /*
    * First, see if the name has previously been stored.
    */
   if (lookup_define(Defines_Table, macro_name, hashval) != NULL) {
     delete_entry(Defines_Table, macro_name, hashval); 
  }
}

/**************************************************************************
 *
 * Function Name :
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/

