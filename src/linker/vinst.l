%{
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/link_error_msg.h"
#include "../include/linker.h"

#include "vinst.tab.h"

extern int parse_sys_utils ;

typedef struct s_keyword_entry {
  char *pattern ;
  int  token ;
} keyword_entry ;

/* automaticall generated keyword table */
#include "vinst_keyword_table.h"

unsigned lex_line_num, lex_debug_line_num;
char *input_file_name ;

char *last_text = NULL ;

static char *last_string = NULL ;

static unsigned search_keyword(char *pattern) ;
static void set_last( char *type, char *token ) ;

%}

white		[ \t\f]+
newline		\n
identifier	[a-zA-Z_][a-zA-Z0-9_]*
name		\$[a-zA-Z0-9_]*
sname           \$\.[a-zA-Z0-9_]*
numeric		[0-9]
decimal		[0-9_]
integerval	[1-9][0-9]*
hex		[a-fA-F0-9xXzZ_]
octal		[0-7_xXzZ]
binary		[01_xXzZ]

%%

\#[^\n]*  {
  int len ;
  /* Debug Info format is TBD
     Once determined, it may be better to parse as the part of syntax
  */
      len = strlen(yytext) ;
      yylval.string_value.name = (char *)calloc(len+10, sizeof(char)) ;
      strncat(yylval.string_value.name, yytext, len) ;
      yylval.string_value.linenum = lex_line_num ;
      yylval.string_value.filename = input_file_name ;
      set_last( "debug_info", yytext ) ;
      return DebugInfo ;
  
}
{white}   {         
  }
{newline}  {
    /*  Looks like this causes the flex internal error. This may be a bug
        in flex code that handles the unput sequence.
    int c  = input() ;
    if( c != 0 ) {
    */
      lex_line_num++ ;
    /*
      unput(c) ;
    }
    */
    return EOL ;
  }
\;[^"\n]* {  
   /* comment line */
}  
\"[^"\n]* {    
    /* begining of string */
    int c ;
    
    c = input() ;
    if( c == '\n' ) {
      lex_line_num++ ;
      if( yytext[yyleng-1] == '\\' ) {
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, &yytext[1] ) ; /* remove " at the top */
        }
        /* yymore() ;  * set flag to append input to the end of yytext */
        unput( '\"' ) ;  /* next search goes to string again */
        /* CONTINUE */
      }
      else {
        fprintf(
          stderr,
          LEX_ERROR_RETURN_FOUND_IN_STRING,
          lex_line_num, input_file_name
        ) ;
        /*
        c = input() ;
        while( c != '\"' ) {
          if( c == '\n' ) {
            lex_line_num++ ;
          }
          c = input() ;
        }
        unput(c) ;
        */
        return String_lit ; 
      }
    }
    else {
      /* detect " */
      if( yytext[yyleng-1] == '\\' ) {
        /* got excaped ", so need to continue scanning string */
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, &yytext[1] ) ; /* remove " at the top */
        }
        unput( c ) ;  /* unput ", so next search goes to string again */
        /* CONTINUE */
      }
      else { 
        /* yytext[yyleng] = c ; */ /* this is " */
        yytext[yyleng] = '\0' ;
        if( last_string ) {
          char *p ;
          int len ;
          char t ;
          len = strlen(last_string) ;
          p = (char *)calloc(len+yyleng+5,sizeof(char)) ;
          t = yytext[yyleng-1] ; 
          yytext[yyleng-1] = '\0' ;
          strcat(p, last_string) ;
          strcat(p, yytext) ;
          yytext[yyleng-1] = t ;
          len = strlen(p) ;
          p[len] = t ;
          yylval.string_value.name = p ;
          free(last_string) ;
          last_string = NULL ;
        }
        else {
          char *p ;
          int len ;
          char t ;
          if( yyleng > 1 ) {
            t = yytext[yyleng-1] ; 
            yytext[yyleng-1] = '\0' ;
            p = (char *)calloc(yyleng+5,sizeof(char)) ;
            strcat(p, &yytext[1]) ; /* remove " at the top */
            yytext[yyleng-1] = t ;
            len = strlen(p) ;
            p[len] = t ;
          }
          else p = (char *)calloc(4,sizeof(char)) ;
          yylval.string_value.name = p ;
        }
        yylval.string_value.linenum = lex_line_num ;
        yylval.string_value.filename = input_file_name ;
        return String_lit ;
      }
    }
  }
{identifier} {
    int token ;
    if( token = search_keyword( yytext ) ) {
      yylval.keyword = token ;
      set_last( "keyword", yytext ) ;
      return token ;
    }
    else {
      int c ;
      yylval.string_value.name = (char *)strdup(yytext) ;
      yylval.string_value.linenum = lex_line_num ;
      yylval.string_value.filename = input_file_name ;
      yylval.string_value.sys_flag = parse_sys_utils ;
      set_last( "identifier", yytext ) ;
      return Identifier ;
    }
  }
{name} {
    yylval.string_value.name = (char *)strdup(&yytext[1]) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    yylval.string_value.sys_flag = parse_sys_utils ;
    set_last( "name", yytext ) ;
    return Name ;
  }
{sname} {
    yylval.string_value.name = (char *)strdup(&yytext[2]) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    yylval.string_value.sys_flag = parse_sys_utils ;
    set_last( "sname", yytext ) ;
    return Name ;
  }
0 {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "decimal number", yytext ) ;
    return Decimal_num ;
  }
{integerval} {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "integer number", yytext ) ;
    return Decimal_num ;
  }
-{integerval} {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "integer number", yytext ) ;
    return Decimal_num ;
  }
{integerval}\'[dD]{decimal}+   {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "decimal number", yytext ) ;
    return V_decimal_num ;
  }
\'[dD]{decimal}+   {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "decimal number", yytext ) ;
    return V_decimal_num ;
  }
{integerval}\'[oO]{octal}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "octal number", yytext ) ;
    return V_octal_num ;
  }
\'[oO]{octal}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "octal number", yytext ) ;
    return V_octal_num ;
  }
{integerval}\'[hHxX]{hex}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "hex number", yytext ) ;
    return V_hex_num ;
  }
\'[hHxX]{hex}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "hex number", yytext ) ;
    return V_hex_num ;
  }
{integerval}\'[bB]{binary}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "binary number", yytext ) ;
    return V_bin_num ;
  }
\'[bB]{binary}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "binary number", yytext ) ;
    return V_bin_num ;
  }
0[dD]{decimal}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "decimal number", yytext ) ;
    return C_decimal_num ;
  }
0[xXhH]{hex}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "hex number", yytext ) ;
    return C_hex_num ;
  }
0[oO]{octal}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "octal number", yytext ) ;
    return C_octal_num ;
  }
0[bB]{binary}+  {
    yylval.string_value.name = (char *)strdup(yytext) ;
    yylval.string_value.linenum = lex_line_num ;
    yylval.string_value.filename = input_file_name ;
    set_last( "binary number", yytext ) ;
    return C_bin_num ;
  }
{integerval}\.([0-9]+)?([eE][+-]?[0-9]+)?  {
  /*
   * tesh
   * floating number detection
   */

  yylval.string_value.name = (char *)strdup(yytext) ;
  yylval.string_value.linenum = lex_line_num ;
  yylval.string_value.filename = input_file_name ;
  set_last( "real number", yytext ) ;
  return Real_num;
}
0\.([0-9]+)?([eE][+-]?[0-9]+)?  {
  /*
   * tesh
   * floating number detection
   */

  yylval.string_value.name = (char *)strdup(yytext) ;
  yylval.string_value.linenum = lex_line_num ;
  yylval.string_value.filename = input_file_name ;
  set_last( "real number", yytext ) ;
  return Real_num;
}
\.[0-9]+ {
  /* special case, both used for depth and floar */
  yylval.string_value.name = (char *)strdup(yytext) ;
  yylval.string_value.linenum = lex_line_num ;
  yylval.string_value.filename = input_file_name ;
  set_last( "real number", yytext ) ;
  return Real_num;
}
\.[0-9]+[eE][+-]?[0-9]+  {
  /*
   * tesh
   * floating number detection
   */

  yylval.string_value.name = (char *)strdup(yytext) ;
  yylval.string_value.linenum = lex_line_num ;
  yylval.string_value.filename = input_file_name ;
  set_last( "real number", yytext ) ;
  return Real_num;
}
{integerval}[eE][+-]?[0-9]+  {
  /*
   * tesh
   * floating number detection
   */

  yylval.string_value.name = (char *)strdup(yytext) ;
  yylval.string_value.linenum = lex_line_num ;
  yylval.string_value.filename = input_file_name ;
  set_last( "real number", yytext ) ;
  return Real_num;
}
.  {
    yylval.keyword = yytext[0] ;
    set_last( "token", yytext ) ;
    return( yytext[0] ) ;
  }



%%

static void set_last( char *type, char *token ){
  if( last_text ) free(last_text) ;
  
  last_text = (char *)calloc( strlen(type)+strlen(token)+10, sizeof(char) ) ;
  
  sprintf( last_text, "%s \"%s\"", type, token ) ;
  //fprintf( stderr, "lex: %s \"%s\" \n", type, token ) ;
  
}

static unsigned search_keyword(char *pattern)
{
  int i ;
  
  i = 0 ;
  while(keyword_table[i].pattern != NULL ) {
    if( !strcmp( pattern, keyword_table[i].pattern ) ) {
      return keyword_table[i].token ;
    }
    i++ ;
  }
  return( 0 ) ;
}


/* 
int yywrap(void) { return 1; }
*/
