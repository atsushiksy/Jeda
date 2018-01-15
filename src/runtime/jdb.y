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

%token <keyword> JD_ANDTILDA
%token <keyword> JD_TILDAAND
%token <keyword> JD_ORTILDA
%token <keyword> JD_TILDAOR
%token <keyword> JD_EORTILDA
%token <keyword> JD_TILDAEOR
%token <keyword> JD_LAND
%token <keyword> JD_LOR
%token <keyword> JD_RSHIFT
%token <keyword> JD_URSHIFT
%token <keyword> JD_LSHIFT
%token <keyword> JD_EQEQ
%token <keyword> JD_LE
%token <keyword> JD_GE
%token <keyword> JD_NE
%token <keyword> JD_EQEQEQ
%token <keyword> JD_NEQEQ
%token <keyword> JD_Q_EQ
%token <keyword> JD_Q_NEQ
%token <keyword> JD_QEQEQ
%token <keyword> JD_EQEQQ
%token <keyword> JD_C_EQEQ
%token <keyword> JD_D_PLUS
%token <keyword> JD_D_MINUS
%token <keyword> JD_D_AT
%token <keyword> JD_C_EQ
%token <keyword> JD_PLUS_EQ
%token <keyword> JD_MINUS_EQ
%token <keyword> JD_STAR_EQ
%token <keyword> JD_SLSH_EQ
%token <keyword> JD_MOD_EQ
%token <keyword> JD_LSHIFT_EQ
%token <keyword> JD_RSHIFT_EQ
%token <keyword> JD_URSHIFT_EQ
%token <keyword> JD_AND_EQ
%token <keyword> JD_OR_EQ
%token <keyword> JD_EOR_EQ
%token <keyword> JD_NAND_EQ
%token <keyword> JD_ANDN_EQ
%token <keyword> JD_NOR_EQ
%token <keyword> JD_ORN_EQ
%token <keyword> JD_NEOR_EQ
%token <keyword> JD_EORN_EQ
%token <keyword> '?'
%token <keyword> ':'
%token <keyword> '!'
%token <keyword> '%'
%token <keyword> '^'
%token <keyword> '&'
%token <keyword> '|'
%token <keyword> '+'
%token <keyword> '-'
%token <keyword> '<'
%token <keyword> '>'
%token <keyword> '='
%token <keyword> '*'
%token <keyword> '/'
%token <keyword> '.'
%token <keyword> '~'
%token <keyword> '@'
%token <keyword> '#'
%token <keyword> '$'
%token <keyword> '{'
%token <keyword> '}'
%token <keyword> '('
%token <keyword> ')'
%token <keyword> '['
%token <keyword> ']'
%token <keyword> '\''

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
%right JD_RSHIFT_EQ JD_URSHIFT_EQ JD_LSHIFT_EQ JD_AND_EQ JD_OR_EQ JD_EOR_EQ JD_NAND_EQ JD_ANDN_EQ JD_NEOR_EQ JD_EORN_EQ JD_NOR_EQ JD_ORN_EQ
%right '=' JD_PLUS_EQ JD_MINUS_EQ JD_STAR_EQ JD_SLSH_EQ JD_MOD_EQ
%right '?' ':' 
%left JD_LOR 
%left JD_LAND 
%left '|' 
%left '^'  
%left '&'  
%left JD_C_EQEQ 
%left JD_Q_EQ JD_Q_NEQ JD_EQEQ JD_NE JD_EQEQEQ JD_NEQEQ JD_EQEQQ JD_QEQEQ
%left '>' JD_LE '<' JD_GE 
%left JD_LSHIFT JD_RSHIFT JD_URSHIFT
%left '-' '+'
%left  '*' '/' '%'
%left  '!' '~' 
%left  JD_TILDAAND JD_ANDTILDA JD_TILDAOR JD_ORTILDA JD_TILDAEOR JD_EORTILDA  
%right JD_D_PLUS JD_D_MINUS 
%left  '.'


%start debug_expression

%%

debug_expression:
  assign_expression
  {
    /* top_assign_expression */
    $$ = NULL ;
  }
  |
  expression
  {
    /* top_expression */
    SKIP_ON_ERROR ;
    if( JD_debugger_assign_command ) {
      GBG_ERROR_ILLEGAL_EXPRESSION_FOR_ASSIGN_COMMAND ;
    }
    else {
      JD_dbg_print_expression( $1, 0 ) ; /* (exp, indent) */
    }
    $$ = NULL ;
  }
  |
  error 
  {
    /* error */
    JD_debugger_clear_word() ;
    $$ = NULL ;
  }
  ;
  
assign_expression:
  varport_expression '=' expression  
  {
    /* assign_expression */
    SKIP_ON_ERROR ;
    if( JD_debugger_assign_command ) {
      if( $1->type != JD_D_dt ) {
        GBG_ERROR_ILLEGAL_LEFT_EXPRESSION_FOR_ASSIGN_COMMAND ;
      }
      else {
        JD_dbg_assign_expression( $1, $3 ) ;
      }
    }
    else {
      GBG_ERROR_ILLEGAL_ASSIGN_EXPRESSION_FOR_PRINT_COMMAND ;
    }
    $$ = NULL ;
  }
  ;

/***********************************
 *  expression
 ***********************************/
expression:
  number
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  String_literal
  {
    /* string_expression */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_string ;
    $$->string = $1 ;
  }
  |
  varport_expression
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  JD_WORD_NULL 
  {
    /* null_expression */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_null ;
    $$->dt = NULL ;
  }
  |
  expression '-' expression   
  {
    /* exp_minus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_minus_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '+' expression   
  {
    /* exp_plus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_plus_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '*' expression   
  {
    /* exp_times_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_times_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '/' expression   
  {
    /* exp_div_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_div_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '%' expression   
  {
    /* exp_mod_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_mod_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '&' expression   
  {
    /* exp_and_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_and_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '|' expression   
  {
    /* exp_or_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_or_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '^' expression   
  {
    /* exp_eor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_ANDTILDA expression  
  {
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nand_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_TILDAAND expression  
  {
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nand_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_ORTILDA expression  
  {
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_TILDAOR expression  
  {
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_EORTILDA expression  
  {
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_TILDAEOR expression  
  {
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_LAND expression  
  {
    /* exp_land_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_land_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_LOR expression  
  {
    /* exp_lor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lor_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_RSHIFT expression  
  {
    /* exp_rshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_rshift_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_URSHIFT expression 
  {
    /* exp_urshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_urshift_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_LSHIFT expression  
  {
    /* exp_lshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lshift_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '<' expression   
  {
    /* exp_lt_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lt_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression '>' expression   
  {
    /* exp_gt_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_gt_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_EQEQ expression  
  {
    /* exp_eqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeq_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_LE expression  
  {
    /* exp_le_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_le_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_GE expression  
  {
    /* exp_ge_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_ge_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_NE expression  
  {
    /* exp_ne_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_ne_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_EQEQEQ expression 
  {
    /* exp_eqeqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeqeq_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_NEQEQ expression 
  {
    /* exp_neqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neqeq_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_Q_EQ expression 
  {
    /* exp_eqQeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqQeq_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_Q_NEQ expression 
  {
    /* exp_Qne_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_Qne_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_EQEQQ expression 
  {
    /* exp_eqeqQ_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeqQ_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  expression JD_QEQEQ expression 
  {
    /* exp_Qeqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_Qeqeq_op, $1, $3, $$ ) ;
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
  }
  |
  '-' expression
  {
    /* u_minus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_minus( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  '~' expression
  {
    /* u_tilde_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_tilde( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  '!' expression
  {
    /* u_not_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_not( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  '&' expression
  {
    /* u_and_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_and( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  '|' expression
  {
    /* u_or_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_or( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  '^' expression
  {
    /* u_eor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_eor( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_TILDAAND expression
  {
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nand( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_ANDTILDA expression
  {
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nand( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_TILDAOR expression
  {
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nor( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_ORTILDA expression
  {
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nor( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_EORTILDA expression 
  {
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_neor( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  JD_TILDAEOR expression 
  {
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_neor( $2, $$ ) ;
    JD_dbg_free_exp($2) ;
  }
  |
  expression JD_C_EQEQ expression ':' expression 
  {
    /* u_eqCeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    JD_dbg_eval_exp($5) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_eqCeq_exp( $1, $3, $5, $$ ) ; 
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
    JD_dbg_free_exp($5) ;
  }
  |
  expression '?' expression ':' expression 
  {
    /* conditional_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($1) ;
    JD_dbg_eval_exp($3) ;
    JD_dbg_eval_exp($5) ;
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_eval_cond_exp( $1, $3, $5, $$ ) ; 
    JD_dbg_free_exp($1) ;
    JD_dbg_free_exp($3) ;
    JD_dbg_free_exp($5) ;
  }
  |
  '(' expression ')'
  {
    /* DDeqD2 */
    $$ = $2 ; 
  }
  |
  concat_expression
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  '{' number '{' expression '}' '}'
  {
    /* duplicate_string */
    int len = 0 ;
    int i, n ;
    
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($4) ;
    if( $4->type != JD_D_string ) {
      DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_DUPLICATION ;
      JD_dbg_free_exp($4) ;
      break ;
    }
    if( $4 && $4->string ) {
      len = strlen($4->string) ;
      if( $2->type == JD_D_int ) {
        n = $2->int_value ;
        len *= n ;
      }
      else n = 0 ; 
    }
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_string ;
    $$->string = calloc(len+1,sizeof(char)) ;
    for( i = 0 ; i < n ; i++ ) {
      strcat($$->string, $4->string) ;
    }
    JD_dbg_free_exp($4) ;
  }
  ;

varport_expression:
  a_ver_expression
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  varport_expression '.' Identifier opt_depth opt_access_range
  {
    /* var_dot_var */
    SKIP_ON_ERROR ;
    /* working here */
    JD_dbg_eval_exp($1) ;
    $$ = (void *)JD_debugger_get_member( $1, $3 ) ;
    SKIP_ON_ERROR ;
    $$->depth = $4 ;
    if( $5 ) {
      JD_debugger_resolve_array_access( $$, $5 ) ;
    }
  }
  |
  varport_expression '.' JD_WORD_SUPER
  {
    /* var_dot_super */
    SKIP_ON_ERROR ;
    $$ =  (void *)JD_dbg_get_super_of( $1 ) ;
    
  }
  ;
  
a_ver_expression:
  Identifier opt_depth opt_access_range
  {
    /* var_expression */
    JD_dbg_expression_node *exp ;
    SKIP_ON_ERROR ;
    
    exp = (void *)JD_debugger_get_data( $1 ) ;
    if( exp && $3 ) {
      if( exp->type == JD_D_dt ) {
        exp =  (void *)JD_debugger_resolve_array_access( exp, $3 ) ;
      }
      else {
        DBG_ERROR_ILLEGAL_NAME_TYPE_FOR_ARRAY_ACCESS ;
      }
    }
    $$ = exp ;
    SKIP_ON_ERROR ;
    if( exp ) {
      if( $2 ) {
        if( exp->dt == NULL || exp->dt->type != JD_D_signal ||
            exp->dt->sub_type != JD_SD_single
        ) {
          DBG_ERROR_ILLEGAL_DEPTH_USAGE_ON_NON_SIGNAL_EXP ;
          break ;
        }
        exp->depth = $2 ;
        if( $2->int_value > 0 ) {
          JD_pli_object *obj ;
          obj = (JD_pli_object *)exp->dt->data.signal.port ;
          if( obj == NULL ) {
            DBG_ERROR_EMPTY_SIGNAL_ACCESS ;
            break ;
          }
          if( obj->is.port.depth < $2->int_value ) {
            DBG_ERROR_SIGNAL_DEPTH_TOO_DEEP ;
            break ;
          }
        }
      }
    }
    else {
      DBG_ERROR_NAME_NOT_FOUND( $1 ) ;
    }
  }
  |
  JD_WORD_SUPER
  {
    /* super */
    SKIP_ON_ERROR ;
    $$ =  (void *)JD_dbg_get_super_exp() ;
  }
  |
  JD_WORD_THIS
  {
    /* this */
    SKIP_ON_ERROR ;
    $$ =  (void *)JD_dbg_get_this_exp() ;
  }
  ;

  
opt_depth:
  /* empty */
  {
    /* DDeqNULL */
    $$ = NULL ;
  }
  |
  Real_or_Depth
  {
    /* dot_depth */
    SKIP_ON_ERROR ;
    $$ =  (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_int ;
    $$->int_value = atoi(&$1[1]) ;
    free($1) ;
  }
  ;
  
opt_access_range:
  /* empty */
  {
    /* DDeqNULL */
    $$ = NULL ;
  }
  |
  access_range
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  ;

access_range:
  array_access_ranges
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  array_access_ranges bit_access_range
  {
    /* array_access_range_bit_access_range */
    SKIP_ON_ERROR ;
    /* array_access_range_bit_access_range */
    $$ = $1 ;
    if( $$ ) $$->next_level = $2 ;
  }
  |
  bit_access_range
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  ;
  
bit_access_range:
  '[' expression ':' expression ']'
  {
    /* bit_access_range */
    SKIP_ON_ERROR ;
    /* bit_access_range */
    JD_dbg_eval_exp_to_int($2) ;
    JD_dbg_eval_exp_to_int($4) ;
    SKIP_ON_ERROR ;
    $$ = $2 ;
    if( $$ ) { 
      $$->next = $4 ;
      $$->etype = JD_dbg_exp_bit_range ;
    }
  }
  ;
  
array_access_ranges:
  array_access_range
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  array_access_ranges array_access_range
  {
    /* Expression_D1_NEXTeqD2 */
    SKIP_ON_ERROR ;
    {
      JD_dbg_expression_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
array_access_range:
  '[' expression ']'  
  {
    /* array_access_range */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp($2) ;
    SKIP_ON_ERROR ;
    $$ = $2 ;
    if( $$ ) {
      if( $$->type == JD_D_string )
        $$->etype = JD_dbg_exp_string_array_range ;
      else {
        JD_dbg_eval_exp_to_int($$) ;
        SKIP_ON_ERROR ;
        $$->etype = JD_dbg_exp_array_range ;
      }
    }
  }
  ;
  
number:
  Real_num
  {
    /* real_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_double ;
    $$->real_value = atof($1) ;
  }
  |
  Real_or_Depth
  {
    /* real_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_double ;
    $$->real_value = atof($1) ;
  }
  |
  Decimal_num
  {
    /* decimal_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    $$->type = JD_D_int ;
    $$->int_value = atoi($1) ;
  }
  |
  V_decimal_num
  {
    /* v_decimal_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_decimal( $1, $$ ) ;
  }
  |
  C_decimal_num
  {
    /* c_decimal_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_decimal( $1, $$ ) ;
  }
  |
  V_hex_num
  {
    /* v_hex_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_hex( $1, $$ ) ;
  }
  |
  C_hex_num
  {
    /* c_hex_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_hex( $1, $$ ) ;
  }
  |
  V_octal_num
  {
    /* v_octal_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_octal( $1, $$ ) ;
  }
  |
  C_octal_num
  {
    /* c_octal_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_octal( $1, $$ ) ;
  }
  |
  V_bin_num
  {
    /* v_bin_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_binary( $1, $$ ) ;
  }
  |
  C_bin_num
  {
    /* c_bin_num */
    SKIP_ON_ERROR ;
    $$ = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_binary( $1, $$ ) ;
  }
  ;


/***********************************
 *  string constant
 ***********************************/
  
concat_expression:
  '{' expression_list '}'
  {
    /* concat_expression */
    char *str ;
    int len ;
    JD_dbg_expression_node *d ;
    
    SKIP_ON_ERROR ;
    d = $2 ;
    JD_dbg_eval_exp(d) ;
    if( d->type == JD_D_string ) {
      len = 0 ;
      while( d ) {
        if( d->string )
          len += strlen(d->string) ;
        d = d->next ;
        if( d ) {
          JD_dbg_eval_exp(d) ;
          if( d->type != JD_D_string ) {
            DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_CONCATENATION ;
            JD_dbg_free_exp($2) ;
            break ;
          }
        }
      }
      SKIP_ON_ERROR ;
      $$ = (void *)JD_dbg_alloc_exp() ;
      $$->type = JD_D_string ;
      $$->string = calloc(len+1,sizeof(char)) ;
      d = $2 ;
      while( d ) {
        if( d->string )
          strcat($$->string, d->string) ;
        d = d->next ;
      }
    }
    else {
      len = 0 ;
      while( d ) {
        if( d->type == JD_D_bit )
          len += d->size ;
        else if( d->type == JD_D_int ) 
          len += 32 ;
        else {
          DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_CONCATENATION ;
          JD_dbg_free_exp($2) ;
          break ;
        }  
        d = d->next ;
        if( d ) JD_dbg_eval_exp(d) ;
      }
      SKIP_ON_ERROR ;
      $$ = (void *)JD_dbg_alloc_exp() ;
      $$->type = JD_D_bit ;
      $$->size = len ;
      $$->bit_a = calloc(NWORD(len),sizeof(int)) ;
      $$->bit_b = calloc(NWORD(len),sizeof(int)) ;
      d = $2 ;
      while( d ) {
        if( d->type == JD_D_bit ) {
          JD_dbg_concat_bit( 
            $$->bit_a, $$->bit_b, $$->size, 
            d->bit_a, d->bit_b, d->size
          ) ;
        }
        else if( d->type == JD_D_int ) {
          len = 0 ;
          JD_dbg_concat_bit( 
            $$->bit_a, $$->bit_b, $$->size, 
            &d->int_value, &len, 32
          ) ;
        }
        d = d->next ;
      }
    }
    JD_dbg_free_exp($2) ;
  }
  ;
  
expression_list:
  expression
  {
    /* DDeqD1 */
    $$ = $1 ; 
  }
  |
  expression_list ',' expression
  {
    /* Expression_D1_NEXTeqD3 */
    SKIP_ON_ERROR ;
    {
      JD_dbg_expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  

%%

int JDB_yyerror() {
  PRINTF( "Syntax error detected in expression\n" ) ;
}


