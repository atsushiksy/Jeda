%{
/* 
   
   Copyright (C) 2001 Juniper Networks Inc.

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
/***************************************************************************
 *
 *  Jeda Debugger Expression Lexical Analizer prep file
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "runtime.h"
#include "debugger.h"

#include "y_tab.h"

static char *Lex_wd = NULL ;
static int Lex_wd_ptr = 0 ;

/* first, the expression word is set to Lex_wd, then let YY_INPUT macro
   scans the buffer during the parse.
 */
 
#define YY_INPUT(buf,result,max_size) \
  { \
    result = JD_dbg_yyinput( buf ) ;\
  }

#define INITIAL_LINE_SIZE 256 

unsigned lex_line_num, lex_debug_line_num;
char *input_file_name ;
int  in_file_used = 0 ;

char *last_text = NULL ;

static unsigned search_keyword(char *pattern) ;

static void set_last( char *type, char *token ) ;

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

{white}   {         
  }
{newline}  {
  }
<<EOF>>  { return 0 ; }
\"[^"]* {    
    int c ;
    /* string */
    c = input() ;
    if( yytext[yyleng-1] == '\\' ) {
      yyleng-- ;
      yymore() ;  /* set flag to append input to the end of yytext */
      unput( '\"' ) ;  /* next search goes to string again */
      /* CONTINUE */
    }
    else {
      JDB_yytext[yyleng++] = '\0' ;
      JDB_yylval.string_value = (char *)strdup(&JDB_yytext[1]) ;
      return String_literal ;
    }
  }
"&~"		{
  return JD_ANDTILDA ;
}
"~&"		{
  return JD_TILDAAND ;
}
"|~"		{
  return JD_ORTILDA ;
}
"~|"		{
  return JD_TILDAOR ;
}
"^~"		{
  return JD_EORTILDA ;
}
"~^"		{
  return JD_TILDAEOR ;
}
"&&"		{
  return JD_LAND ;
}
"||"		{
  return JD_LOR ;
}
">>"		{
  return JD_RSHIFT ;
}
">>>"		{
  return JD_URSHIFT ;
}
"<<"		{
  return JD_LSHIFT ;
}
"=="		{
  return JD_EQEQ ;
}
"<="		{
  return JD_LE ;
}
">="		{
  return JD_GE ;
}
"!="		{
  return JD_NE ;
}
"==="		{
  return JD_EQEQEQ ;
}
"!=="		{
  return JD_NEQEQ ;
}
"=?="		{
  return JD_Q_EQ ;
}
"!?="		{
  return JD_Q_NEQ ;
}
"?=="		{
  return JD_QEQEQ ;
}
"==?"		{
  return JD_EQEQQ ;
}
"=:="		{
  return JD_C_EQEQ ;
}
"++"		{
  return JD_D_PLUS ;
}
"--"		{
  return JD_D_MINUS ;
}
"@@"		{
  return JD_D_AT ;
}
":="		{
  return JD_C_EQ ;
}
"+="		{
  return JD_PLUS_EQ ;
}
"-="		{
  return JD_MINUS_EQ ;
}
"*="		{
  return JD_STAR_EQ ;
}
"/="		{
  return JD_SLSH_EQ ;
}
"%="		{
  return JD_MOD_EQ ;
}
"<<="		{
  return JD_LSHIFT_EQ ;
}
">>="		{
  return JD_RSHIFT_EQ ;
}
">>>="		{
  return JD_URSHIFT_EQ ;
}
"&="		{
  return JD_AND_EQ ;
}
"|="		{
  return JD_OR_EQ ;
}
"^="		{
  return JD_EOR_EQ ;
}
"~&="		{
  return JD_NAND_EQ ;
}
"&~="		{
  return JD_ANDN_EQ ;
}
"~|="		{
  return JD_NOR_EQ ;
}
"|~="		{
  return JD_ORN_EQ ;
}
"~^="		{
  return JD_NEOR_EQ ;
}
"^~="		{
  return JD_EORN_EQ ;
}
null {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return JD_WORD_NULL ;
  }
super {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return JD_WORD_SUPER ;
  }
this {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return JD_WORD_THIS ;
  }
{identifier} {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return Identifier ;
  }
0 {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return Decimal_num ;
  }
{integerval} {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return Decimal_num ;
  }
{integerval}\'[dD]{decimal}+   {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_decimal_num ;
  }
\'[dD]{decimal}+   {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_decimal_num ;
  }
{integerval}\'[oO]{octal}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_octal_num ;
  }
\'[oO]{octal}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_octal_num ;
  }
{integerval}\'[hHxX]{hex}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_hex_num ;
  }
\'[hHxX]{hex}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_hex_num ;
  }
{integerval}\'[bB]{binary}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_bin_num ;
  }
\'[bB]{binary}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return V_bin_num ;
  }
0[dD]{decimal}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return C_decimal_num ;
  }
0[xXhH]{hex}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return C_hex_num ;
  }
0[oO]{octal}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return C_octal_num ;
  }
0[bB]{binary}+  {
    JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
    return C_bin_num ;
  }
{integerval}\.([0-9]+)?([eE][+-]?[0-9]+)?  {
  JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
  return Real_num;
}
0\.([0-9]+)?([eE][+-]?[0-9]+)?  {
  JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
  return Real_num;
}
\.[0-9]+ {
  /* special case, both used for depth and floar */
  JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
  return Real_or_Depth;
}
\.[0-9]+[eE][+-]?[0-9]+  {
  JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
  return Real_num;
}
{integerval}[eE][+-]?[0-9]+  {
  JDB_yylval.string_value = (char *)strdup(JDB_yytext) ;
  return Real_num;
}
.  {
    /* token */
    return( JDB_yytext[0] ) ;
  }



%%

int JDB_yywrap(void) { return 1; }

void JD_debugger_set_word( char *word ) {
  Lex_wd = word ;
  Lex_wd_ptr = 0 ;
}

void JD_debugger_clear_word() {
  Lex_wd = NULL ;
}

int JD_dbg_yyinput( char *buf ) {
  char c ;
  int result ;
  if( Lex_wd ) { 
    if( c = Lex_wd[Lex_wd_ptr++] ) { buf[0] = c ; result = 1 ; } 
    else result = YY_NULL ; 
  } 
  else result = YY_NULL ; 
  return result ;
}

int JD_dbg_input() {
  char c ;
  
  if( Lex_wd ) { 
    c = Lex_wd[Lex_wd_ptr++] ;
    return c ; 
  } 
  return 0 ;
}

int JD_dbg_uninput( char c ) {
  
  if( Lex_wd ) { 
    Lex_wd[--Lex_wd_ptr] = c ;
    return c ; 
  } 
  return Lex_wd_ptr ;
}
