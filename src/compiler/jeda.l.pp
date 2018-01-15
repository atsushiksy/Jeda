%{
/***************************************************************************
 *
 *  Jeda Lexical Analizer prep file
 *
 *  Author: Atsushi Kasuya, Teshager Tesfaye
 *
 *
 ***************************************************************************/
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

#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/error_msg.h"

#include "y_tab.h"

static lex_error() ;

typedef struct s_keyword_entry {
  char *pattern ;
  int  token ;
} keyword_entry ;

/* automaticall generated keyword table */
#include "jeda_keyword_table.h"

#define INITIAL_LINE_SIZE 256 

unsigned lex_line_num, lex_debug_line_num;
char *input_file_name ;
int  in_file_used = 0 ;

char *last_text = NULL ;

static unsigned search_keyword(char *pattern) ;

static void set_last( char *type, char *token ) ;

static char *last_string = NULL ;

%}

white		[ \t\f]+
newline		\n
identifier	[a-zA-Z_][a-zA-Z0-9_]*
numeric		[0-9]
decimal		[0-9_]
integerval	[1-9][0-9]*
hex		[a-fA-F0-9xXzZ_]
octal		[0-7_xXzZ]
binary		[01_xXzZ]

%%

\#.*$  {
  /* preprocessor comment line must have the following format:
    #  <line_num>  "<file_name>"
  */
  
  char *p, *q, *r, *lnum ;
  
  p = q = (char *)strdup( yytext ) ;
  q+= 2 ;
  while( isspace(*q) ) q++ ;
  r = q ;
  while( isdigit(*r) ) r++ ;
  if( q != r ) {
    *r = '\0' ;
    lnum = q ;
    q = r+1 ;
    while( isspace(*q) )q++ ;
    if( *q == '"' ) {
      q++ ;
      r = q ;
      while( *r != '"' ) r++ ;
      *r = '\0' ;
      if( strlen(q) ) {
        lex_line_num = atoi(lnum)-1 ;
        lex_debug_line_num = lex_line_num ;
        /* Don't release if we share it for debug info */
        /* if( input_file_name && !in_file_used ) free( input_file_name ) ;
        */
        input_file_name = (char *)strdup(q) ;
        add_file_to_list(q) ; /* func in parse_tree.c */
        in_file_used = 0 ;
      }
      else if( input_file_name == NULL ) input_file_name = "???" ;
    }
  }
  free( p ) ;
}
{white}   {         
  }
{newline}  {
      lex_line_num++ ;
  }
<<EOF>>  { lex_line_num-- ;  return 0 ; }
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
          strcat( last_string, yytext ) ;
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
        return String_literal ; 
      }
    }
    else {
      /* detect " */
      if( yytext[yyleng-1] == '\\' ) {
        /* got excaped ", so need to continue scanning string */
        /* avoid yymore due to a flex problem?? */
        /* yymore() ;  * set flag to append input to the end of yytext */
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
        }
        unput( c ) ;  /* unput ", so next search goes to string again */
        /* CONTINUE */
      }
      else {
        if( last_string ) {
          char *p ;
          int len ;
          char t ;
          t = yytext[yyleng-1] ; /* this is " */
          yytext[yyleng-1] = '\0' ;
          len = strlen(last_string) ;
          p = (char *)calloc(len+yyleng+3,sizeof(char)) ;
          strcat(p, last_string) ;
          strcat(p, yytext) ;
          len = strlen(p) ;
          p[len] = t ;
          p[len+1] = c ;
          yylval.string_value.name = p ;
          free(last_string) ;
          last_string = NULL ;
        }
        else {
          char *p ;
          int len ;
          char t ;
          t = yytext[yyleng-1] ; /* this is " */
          yytext[yyleng-1] = '\0' ;
          p = (char *)calloc(yyleng+3,sizeof(char)) ;
          strcat(p, yytext) ;
          len = strlen(p) ;
          p[len] = t ;
          p[len+1] = c ;
          yylval.string_value.name = p ;
        }
        yylval.string_value.linenum = lex_line_num ;
        yylval.string_value.filename = input_file_name ;
        return String_literal ;
      }
    }
  }
#INCLUDE "jeda_operator_lex.h"
{identifier} {
    int token ;
    if( token = search_keyword( yytext ) ) {
      yylval.keyword.linenum = lex_line_num ;
      yylval.keyword.filename = input_file_name ;
      set_last( "keyword", yytext ) ;
      return token ;
    }
    else {
      int c ;
      /* yylval.string_value.name = (char *)strdup(yytext) ; */
      yylval.string_value.name = 
        (char *)calloc( strlen(yytext)+100, sizeof(char) ) ;
      strcat( yylval.string_value.name, yytext ) ;
      yylval.string_value.linenum = lex_line_num ;
      yylval.string_value.filename = input_file_name ;
      set_last( "identifier", yytext ) ;
      /*
      if( !strcmp( "scan_distribution", yytext ) ) {
        lex_catch_it() ;
      }
      */
      return Identifier ;
    }
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
  return Real_or_Depth;
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
    yylval.keyword.linenum = lex_line_num ;
    yylval.keyword.filename = input_file_name ;
    set_last( "token", yytext ) ;
    return( yytext[0] ) ;
  }



%%

static void set_last( char *type, char *token ){
  /* if( last_text ) free(last_text) ; */
  
  last_text = (char *)calloc( strlen(type)+strlen(token)+10, sizeof(char) ) ;
  
  strcat( last_text, type ) ;
  strcat( last_text, " \"" ) ;
  strcat( last_text, token ) ;
  strcat( last_text, "\"" ) ;
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

int yywrap(void) { return 1; }

static lex_error() {
  printf( "Lex error detected!!\n" ) ;
}

int lex_catch_it() {
  printf( "Lex catch %s!\n", yytext ) ;
}
