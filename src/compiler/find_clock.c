/*
   Finding implicit clock port
   
     Author: Atsushi Kasuya
*/

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

#include "../include/compiler_macro.h"
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/symtab.h"
#include "../include/error_msg.h"
#include "../include/constant.h"
#include "../include/output.h"
#include "../include/output_macro.h"

expression_node *find_clock_on_expression( expression_node * ) ;

expression_node *find_clock_on_verport( expression_node *exp ) {
  expression_node *ret = NULL ;

  /* variable or port with depth means the access to the value
     to the signal.
   */
  if( exp->is.var.depth ) ret = exp ; /* must be a signal */
    
  return( ret ) ;
}
 

expression_node *find_clock_on_func_call( expression_node *exp ) {
  expression_node *ret = NULL ;
  
  if( exp->is.var.name->is_clock )  ret = exp->is.var.pre ;
  
  return( ret ) ;
}

expression_node *find_clock_on_assign( expression_node *exp ) {
  expression_node *ret = NULL ;
  ret = find_clock_on_expression( exp->is.assign.lvalue ) ;
  if( !ret ) ret = find_clock_on_expression( exp->is.assign.expression ) ;
  return( ret ) ;
}

expression_node *find_clock_on_list_assign( expression_node *exp ) {
  expression_node *ret = NULL ;
  expression_node *e ;
  e = exp->is.assign.lvalue ;
  while( e && ret == NULL ) {
    ret = find_clock_on_expression( e ) ;
    e = e->next ;
  }
  if( !ret ) ret = find_clock_on_expression( exp->is.assign.expression ) ;
  return( ret ) ;
}

expression_node *find_clock_on_expression( expression_node *exp ) {
  expression_node *ret = NULL ;
  expression_node *nxt ;
  
  if( !exp ) return NULL ;
  
  switch ( exp->type ) {
    case JD_string_exp:
    case JD_null_exp:
    case JD_true_exp:
    case JD_false_exp:
    case JD_verilog_task_call_exp:
    case JD_real_num:
    case JD_int_num:
    case JD_bit_num:
    case JD_super_exp:
    case JD_this_exp:
      ret =  NULL ;
      break ;
    case JD_varport_exp:
      ret = find_clock_on_verport( exp ) ;
      break ;
    case JD_function_call_exp:
      ret = find_clock_on_func_call( exp ) ;
      break ;
    case JD_preincrement_exp:
    case JD_predecrement_exp:
    case JD_postincrement_exp:
    case JD_postdecrement_exp:
      ret = find_clock_on_verport( exp->is.expression ) ;
      break ;
    case JD_concatenation_exp:
      nxt = exp->is.expression ;
      while( nxt && (ret == NULL) ) {
        ret = find_clock_on_expression( nxt ) ;
        nxt = nxt->next ;
      }
      break ;
    case JD_list_exp:
      while( exp && (ret == NULL) ) {
        ret = find_clock_on_expression( exp->is.expression ) ;
        exp = exp->next ;
      }  
      break ;
    case JD_duplication_exp:
    case JD_exp_minus_exp:     /* expression '-' expression   */
    case JD_exp_plus_exp:      /* expression '+' expression   */
    case JD_exp_times_exp:     /* expression '*' expression   */
    case JD_exp_div_exp:       /* expression '/' expression   */
    case JD_exp_mod_exp:       /* expression '%' expression   */
    case JD_exp_and_exp:     /* expression '&' expression   */
    case JD_exp_or_exp:      /* expression '|' expression   */
    case JD_exp_eor_exp:     /* expression '^' expression   */
    case JD_exp_nand_exp:      /* expression '&~' expression  */
    case JD_exp_nor_exp:     /* expression '|~' expression  */
    case JD_exp_neor_exp:      /* expression '^~' expression  */
    case JD_exp_land_exp:      /* expression '&&' expression  */
    case JD_exp_lor_exp:     /* expression '||' expression  */
    case JD_exp_rshift_exp:    /* expression '>>' expression  */
    case JD_exp_urshift_exp:   /* expression '>>>' expression */
    case JD_exp_lshift_exp:    /* expression '<<' expression  */
    case JD_exp_lt_exp: /* expression '<' expression   */
    case JD_exp_gt_exp: /* expression '>' expression   */
    case JD_exp_eqeq_exp:      /* expression '==' expression  */
    case JD_exp_le_exp: /* expression '<=' expression  */
    case JD_exp_ge_exp: /* expression '>=' expression  */
    case JD_exp_ne_exp: /* expression '!=' expression  */
    case JD_exp_eqeqeq_exp:    /* expression '===' expression */
    case JD_exp_neqeq_exp:     /* pression '!==' expression   */
    case JD_exp_eqQeq_exp:     /* expression '=?=' expression */
    case JD_exp_eqeqQ_exp:     /* expression '==?' expression */
    case JD_exp_Qeqeq_exp:     /* expression '?==' expression */
    case JD_exp_Qne_exp:     /* expression '!?=' expression */
      ret = find_clock_on_expression( exp->is.binary.left ) ;
      if( !ret ) ret = find_clock_on_expression( exp->is.binary.right ) ;
      break ;
    case JD_u_minus_exp:     /* '-' expression              */
    case JD_u_tilde_exp:     /* '~' expression              */
    case JD_u_not_exp:  /* '!' expression              */
    case JD_u_and_exp:  /* '&' expression              */
    case JD_u_or_exp:   /* '|' expression              */
    case JD_u_eor_exp:  /* '^' expression              */
    case JD_u_nand_exp: /* '~&' expression             */
    case JD_u_nor_exp:  /* '~|' expression             */
    case JD_u_neor_exp: /* '^~' expression             */
      ret = find_clock_on_expression( exp->is.unary.this ) ;
      break ;
    case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
    case JD_conditional_exp:   /*  '?' exp ':' exp            */
      ret = find_clock_on_expression( exp->is.trinary.first ) ;
      if( !ret ) ret = find_clock_on_expression( exp->is.trinary.second ) ;
      if( !ret ) ret = find_clock_on_expression( exp->is.trinary.third ) ;
      break ;
    case JD_timed_till_exp:
    case JD_timed_while_exp:
      ret = exp->is.time.clock ;
      nxt = exp->is.time.exp_list ;
      while( !ret && nxt ) {
        ret = find_clock_on_expression( nxt ) ;
        nxt = nxt->next ;
      }
    case JD_parallel_or_exp:
    case JD_parallel_and_exp:
      nxt = exp->is.time.exp_list ;
      while( !ret && nxt ) {
        ret = find_clock_on_expression( nxt ) ;
        nxt = nxt->next ;
      }
      break ;
    case JD_assign_exp:
      ret = find_clock_on_assign( exp ) ;
      break ;
    case JD_assign_list_exp:
      ret = find_clock_on_list_assign( exp ) ;
      break ;
    case JD_rnd_exp:
    case JD_cyc_exp:
      {
        expression_node *cnd = exp->is.rnd.cond ;
        while( !ret && cnd ) {
          ret = find_clock_on_expression( cnd->is.rnd_cond.exp )  ;
          cnd = cnd->is.rnd_cond.nxt ;
        }
      }
      break ;
      
    /*
    case JD_new_exp:    * new                         *
    case JD_new_block_exp:     * new  { <block }             *
      * should be handled by assign exp *
    case JD_X_num: 
    case JD_port_exp:
    case JD_portdef_exp:
    case JD_port_attr_exp:
    case JD_bit_range_exp:
    case JD_single_range_exp:
    case JD_assoc_array_exp:
    case JD_stridx_array_exp:
    case JD_list_array_exp:
    case JD_statement_prefix_exp:
    case JD_CLOCK_exp:
    case JD_default_arg:
      * should not be used at this level *
    */
    default:
      fprintf( stderr, "Compiler Internal Error!! \n" ) ; 
      fprintf( stderr, "Unknown/Illegal expression %d\n", exp->type ) ; 
      exit(1) ;
      break ;
    
    
  }
  
  return( ret ) ;
}

expression_node *find_clock_on_expression_list( expression_node *exp ) {
  expression_node *ret ;
  
  ret = NULL ;
  
  while( !ret && exp ) {
    ret = find_clock_on_expression( exp ) ;
    exp = exp->next ;
  }
  
  return ret ;
  
}

expression_node *find_clock_on_statement( statement_node *stmt ) {
  expression_node *ret = NULL ;
  expression_node *exp ;
  
  switch( stmt->type ) {
    case JD_null_statement:
      break ;
    case JD_if_statement:
      ret = find_clock_on_expression( stmt->is.if_.cond ) ;
      break ;
    case JD_while_statement:
      ret = find_clock_on_expression( stmt->is.while_.cond ) ;
      break ;
    case JD_for_statement: 
      exp = stmt->is.for_.initial ;
      while( exp && !ret ) {
        ret = find_clock_on_expression( exp ) ;
        exp = exp->next ;
      }
      if( !ret ) ret = find_clock_on_expression( stmt->is.for_.cond ) ;
      exp = stmt->is.for_.last ;
      while( exp && !ret ) {
        ret = find_clock_on_expression( exp ) ;
        exp = exp->next ;
      }
      break ;
    case JD_repeat_statement: 
      ret = find_clock_on_expression( stmt->is.repeat.number ) ;
      break ;
    case JD_do_while_statement: 
      ret = find_clock_on_expression( stmt->is.dowhile.cond ) ;
      break ;
    case JD_forever_statement: 
      break ;
    case JD_case_statement: 
    case JD_casex_statement: 
    case JD_casez_statement: 
      ret = find_clock_on_expression( stmt->is.case_statement.cond ) ;
      break ;
    case JD_expression_statement:
      ret = find_clock_on_expression( stmt->is.exp.expression ) ;
      break ;
    case JD_assign_statement:
      ret = find_clock_on_expression( stmt->is.assign.expression ) ;
      break ;
    case JD_function_call_statement:
      ret = find_clock_on_expression( stmt->is.funccall.expression ) ;
      break ;
    case JD_return_statement:
      ret = find_clock_on_expression( stmt->is.ret.expression ) ;
      break ;
    case JD_break_statement: 
    case JD_continue_statement:
    case JD_fork_join_statement:
    case JD_terminate_statement:
    case JD_breakpoint_statement:
    case JD_block:
    case JD_verilog_task_call_statement:
      break ;
    
    /*  
    case JD_enum_def_statement:
    case JD_vardeclare_statement:
    case JD_portconnect_statement:
    case JD_initial_statement:
    case JD_elsif_statement:
    case JD_else_statement:
    case JD_case_item: 
    case JD_case_default: 
    case JD_statement_list:
    */
      /* can't happen */
    default:
      fprintf( stderr, "Compiler Internal error, illegal statemen type\n" ) ;
      exit(1) ;
      break ;
  }  
  return( ret ) ;
}

