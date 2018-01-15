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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#include "debugger.h"
#include "debugger_error_msg.h"
#include "exec_macro.h"

int JD_debugger_assign_command = 0 ;

extern int JD_dbg_eval_error ;

%}


%union {
  char *string_value ;
  JD_dbg_expression_node *expression ;
  int keyword ;
}

%token <string_value> Identifier 
%token <string_value> Real_num         /* real */
%token <string_value> Real_or_Depth    /* real or depth */
%token <string_value> Decimal_num      /* number */
%token <string_value> V_decimal_num    /* verilog format decimal */
%token <string_value> C_decimal_num    /* C format decimal */
%token <string_value> V_hex_num        /* verilog format hex */
%token <string_value> C_hex_num        /* C format hex */
%token <string_value> V_octal_num      /* verilog format octal */
%token <string_value> C_octal_num      /* C format octal */
%token <string_value> V_bin_num        /* verilog format binary */
%token <string_value> C_bin_num        /* C format binary */
%token <string_value> String_literal   /* string */

/*
%token <keyword> ';' 
%type <keyword> semicolon
*/

%token <expression> JD_WORD_NULL
%token <expression> JD_WORD_SUPER
%token <expression> JD_WORD_THIS

#INCLUDE "jdb_operator_token.h" 

%type <expression>  debug_expression
%type <expression>  assign_expression
%type <expression>  number
%type <expression>  expression
%type <expression>  a_ver_expression
%type <expression>  opt_access_range
%type <expression>  access_range
%type <expression>  bit_access_range
%type <expression>  array_access_ranges
%type <expression>  array_access_range
%type <expression>  varport_expression
%type <expression>  opt_depth
%type <expression>  concat_expression
%type <expression>  expression_list


/* precedence table */
%right '>>=' '>>>=' '<<=' '&=' '|=' '^=' '~&=' '&~=' '~^=' '^~=' '~|=' '|~='
%right '=' '+=' '-=' '*=' '/=' '%='
%right '?' ':' 
%left '||' 
%left '&&' 
%left '|' 
%left '^'  
%left '&'  
%left '=:=' 
%left '=?=' '!?=' '==' '!=' '===' '!==' '==?' '?=='
%left '>' '<=' '<' '>=' 
%left '<<' '>>' '>>>'
%left '-' '+'
%left  '*' '/' '%'
%left  '!' '~' 
%left  '~&' '&~' '~|' '|~' '~^' '^~'  
%right '++' '--' 
%left  '.'


%start debug_expression

%%

debug_expression:
  assign_expression
  <<top_assign_expression>>
  |
  expression
  <<top_expression>>
  |
  error 
  <<error>>
  ;
  
assign_expression:
  varport_expression '=' expression  
  <<assign_expression>>
  ;

/***********************************
 *  expression
 ***********************************/
expression:
  number
  <<DDeqD1>>
  |
  String_literal
  <<string_expression>>
  |
  varport_expression
  <<DDeqD1>>
  |
  JD_WORD_NULL 
  <<null_expression>>
  |
  expression '-' expression   
  <<exp_minus_exp>>
  |
  expression '+' expression   
  <<exp_plus_exp>>
  |
  expression '*' expression   
  <<exp_times_exp>>
  |
  expression '/' expression   
  <<exp_div_exp>>
  |
  expression '%' expression   
  <<exp_mod_exp>>
  |
  expression '&' expression   
  <<exp_and_exp>>
  |
  expression '|' expression   
  <<exp_or_exp>>
  |
  expression '^' expression   
  <<exp_eor_exp>>
  |
  expression '&~' expression  
  <<exp_nand_exp>>
  |
  expression '~&' expression  
  <<exp_nand_exp>>
  |
  expression '|~' expression  
  <<exp_nor_exp>>
  |
  expression '~|' expression  
  <<exp_nor_exp>>
  |
  expression '^~' expression  
  <<exp_neor_exp>>
  |
  expression '~^' expression  
  <<exp_neor_exp>>
  |
  expression '&&' expression  
  <<exp_land_exp>>
  |
  expression '||' expression  
  <<exp_lor_exp>>
  |
  expression '>>' expression  
  <<exp_rshift_exp>>
  |
  expression '>>>' expression 
  <<exp_urshift_exp>>
  |
  expression '<<' expression  
  <<exp_lshift_exp>>
  |
  expression '<' expression   
  <<exp_lt_exp>>
  |
  expression '>' expression   
  <<exp_gt_exp>>
  |
  expression '==' expression  
  <<exp_eqeq_exp>>
  |
  expression '<=' expression  
  <<exp_le_exp>>
  |
  expression '>=' expression  
  <<exp_ge_exp>>
  |
  expression '!=' expression  
  <<exp_ne_exp>>
  |
  expression '===' expression 
  <<exp_eqeqeq_exp>>
  |
  expression '!==' expression 
  <<exp_neqeq_exp>>
  |
  expression '=?=' expression 
  <<exp_eqQeq_exp>>
  |
  expression '!?=' expression 
  <<exp_Qne_exp>>
  |
  expression '==?' expression 
  <<exp_eqeqQ_exp>>
  |
  expression '?==' expression 
  <<exp_Qeqeq_exp>>
  |
  '-' expression
  <<u_minus_exp>>
  |
  '~' expression
  <<u_tilde_exp>>
  |
  '!' expression
  <<u_not_exp>>
  |
  '&' expression
  <<u_and_exp>>
  |
  '|' expression
  <<u_or_exp>>
  |
  '^' expression
  <<u_eor_exp>>
  |
  '~&' expression
  <<u_nand_exp>>
  |
  '&~' expression
  <<u_nand_exp>>
  |
  '~|' expression
  <<u_nor_exp>>
  |
  '|~' expression
  <<u_nor_exp>>
  |
  '^~' expression 
  <<u_neor_exp>>
  |
  '~^' expression 
  <<u_neor_exp>>
  |
  expression '=:=' expression ':' expression 
  <<eqCeq_exp>>
  |
  expression '?' expression ':' expression 
  <<conditional_exp>>
  |
  '(' expression ')'
  <<DDeqD2>>
  |
  concat_expression
  <<DDeqD1>>
  |
  '{' number '{' expression '}' '}'
  <<duplicate_string>>
  ;

varport_expression:
  a_ver_expression
  <<DDeqD1>>
  |
  varport_expression '.' Identifier opt_depth opt_access_range
  <<var_dot_var>>
  |
  varport_expression '.' JD_WORD_SUPER
  <<var_dot_super>>
  ;
  
a_ver_expression:
  Identifier opt_depth opt_access_range
  <<var_expression>>
  |
  JD_WORD_SUPER
  <<super>>
  |
  JD_WORD_THIS
  <<this>>
  ;

  
opt_depth:
  /* empty */
  <<DDeqNULL>>
  |
  Real_or_Depth
  <<dot_depth>>
  ;
  
opt_access_range:
  /* empty */
  <<DDeqNULL>>
  |
  access_range
  <<DDeqD1>>
  ;

access_range:
  array_access_ranges
  <<DDeqD1>>
  |
  array_access_ranges bit_access_range
  <<array_access_range_bit_access_range>>
  |
  bit_access_range
  <<DDeqD1>>
  ;
  
bit_access_range:
  '[' expression ':' expression ']'
  <<bit_access_range>>
  ;
  
array_access_ranges:
  array_access_range
  <<DDeqD1>>
  |
  array_access_ranges array_access_range
  <<Expression_D1_NEXTeqD2>>
  ;
  
array_access_range:
  '[' expression ']'  
  <<array_access_range>>
  ;
  
number:
  Real_num
  <<real_num>>
  |
  Real_or_Depth
  <<real_num>>
  |
  Decimal_num
  <<decimal_num>>
  |
  V_decimal_num
  <<v_decimal_num>>
  |
  C_decimal_num
  <<c_decimal_num>>
  |
  V_hex_num
  <<v_hex_num>>
  |
  C_hex_num
  <<c_hex_num>>
  |
  V_octal_num
  <<v_octal_num>>
  |
  C_octal_num
  <<c_octal_num>>
  |
  V_bin_num
  <<v_bin_num>>
  |
  C_bin_num
  <<c_bin_num>>
  ;


/***********************************
 *  string constant
 ***********************************/
  
concat_expression:
  '{' expression_list '}'
  <<concat_expression>>
  ;
  
expression_list:
  expression
  <<DDeqD1>>
  |
  expression_list ',' expression
  <<Expression_D1_NEXTeqD3>>
  ;
  

%%

int JDB_yyerror() {
  PRINTF( "Syntax error detected in expression\n" ) ;
}


