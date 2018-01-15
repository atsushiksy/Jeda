/*
 *  Auto Header file generator
 *
 *  Author: Atsushi Kasuya
 *
 */
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
   Copyright (C) 2002 Jeda Technologies, Inc.

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

extern char *in_fname ;
extern FILE *hout ;
extern block_node *root_block ;

static void output_header_top() {
  HOUT0( "/***********************************************************\n" ) ;
  HOUT1( " * Jeda Header for %s \n", in_fname ) ;
  HOUT3( " *   created by %s (%s %s)\n", JDCMP, CMPVERSION, CMPREVISION ) ;
  HOUT0( " ***********************************************************/\n" ) ;
}

static void output_data_type_hdr( named_node *type ) {
  int size0, size1 ;
  
  switch( type->type ) {
    case JD_void_type:
      HOUT0( "void " ) ;
      break ;
    case JD_int_type:
      HOUT0( "int " ) ;
      break ;
    case JD_long_type:
      HOUT0( "long " ) ;
      break ;
    case JD_float_type:
      HOUT0( "float " ) ;
      break ;
    case JD_double_type:
      HOUT0( "double " ) ;
      break ;
    case JD_bit_type:
      size1 = type->ub ;
      size0 = type->lb ;
      if( size0 == size1 && size0 == 0 ) 
        HOUT0( "bit " ) ;
      else
        HOUT2( "bit [%d:%d] ", size1, size0 ) ;
      break ;
    case JD_bool_type:
      HOUT0( "bool " ) ;
      break ;
    case JD_class_name:
      HOUT1( "%s ", type->name ) ;
      break ;
    case JD_sys_class_name:
      HOUT1( "%s ", type->name ) ;
      break ;
    case JD_string_type:
      HOUT0( "string " ) ;
      break ;
    case JD_signal_type:
      HOUT0( "signal " ) ;
      break ;
    case JD_enum_type:
      HOUT1( "%s ", type->name ) ;
      break ;
    case JD_class_type:
      break ;
    default:
      fprintf( stderr, "Illegal type code detected\n" ) ;
      exit(1) ;
      break ;
  
  }
}

static void output_array_range_hdr( expression_node *range ) {
  if( range != NULL ) {
    switch( range->type ) {
      case JD_single_range_exp:
        if( range->is.range.num_index == 1 ) {
          HOUT1( "[%d]", range->is.range.int_ub ) ;
        }
        else {
          while( range ) {
            HOUT1( "[%d]", range->is.range.int_ub ) ;
            range = range->next ;
          }
        }
        break ;
      case JD_stridx_array_exp:
        HOUT0( "[*]" ) ;
        break ;
      case JD_assoc_array_exp:
        HOUT0( "[]" ) ;
        break ;
      case JD_list_array_exp:
        HOUT0( "<>" ) ;
        break ;
    }
  }
}

static void output_global_var_hdr( named_node *variable ) {
  if( variable->is_extern ) return ;
  HOUT0( "extern " ) ;
  output_data_type_hdr( variable->data_type ) ;
  HOUT1( "%s ", variable->name ) ;
  output_array_range_hdr( variable->info.var.range ) ;
  HOUT0( " ;\n" ) ; 
}

static void globalvar_block_header( block_node *blk ) {
  named_node *variable ;
  
  variable = blk->is.local_global_var.dcl->is.variable.declare->var_list ;
  while( variable ) {
    output_global_var_hdr( variable ) ;
    variable = variable->next ;
  }
}

static void output_enum_hdr( named_node *enm, char *indent ) {
  named_node *member ;
  HOUT0( indent ) ;
  HOUT1( "enum %s = ", enm->name ) ;
  member = enm->info.enum_members ;
  while( member ) {
    HOUT1( "%s", member->name ) ;
    if( member->next ) HOUT0( ", " ) ;
    member = member->next ;
  }
  HOUT0( " ;\n" ) ; 
}

static void typedef_block_header( block_node *blk ) {
  named_node *enm ;
  if( blk->is.local_global_var.dcl ) {
    enm = blk->is.local_global_var.dcl->is.enum_def.edf ;
    output_enum_hdr( enm, "" ) ;
  }
}

static void print_expression( expression_node *exp ) ;

static void print_string( expression_node *exp ) {
  char *p = exp->is.string ;
  
  /* HOUT0( "\"" ) ; */
  while( *p != '\0' ) {
    HOUT1( "%c", *p++ ) ;
  }
  
  /* HOUT0( "\"" ) ; */
}

static void print_concatenation( expression_node *exp ) {
  expression_node *nxt ;
  nxt = exp->is.expression->next ;
  HOUT0( "{" ) ;
  while( nxt ) {
    print_expression( nxt ) ;
    nxt = nxt->next ;
    if( nxt ) HOUT0( "," ) ;
  }  
  HOUT0( "}" ) ;
}

static void print_duplication( expression_node *exp ) {
  HOUT0( "{" ) ;
  print_expression( exp->is.binary.left ) ;
  HOUT0( "{" ) ;
  print_expression( exp->is.binary.right ) ;
  HOUT0( "}" ) ;
  HOUT0( "}" ) ;
}

static unsigned get_bit( unsigned *data, int location ) {
  unsigned bit ;
  bit = ( data[location/32] >> (location % 32) ) & 1 ;
  
  return bit ;
  
}

static void print_bit( expression_node *exp ) {
  int i, supress ;
  unsigned bit_a, bit_b ;
  unsigned v ;
  supress = 1 ;
  HOUT1( "%d'b", exp->is.bit_const.size ) ;
  for( i = exp->is.bit_const.size-1 ; i >= 0 ; i-- ) {
    bit_a = get_bit( (unsigned *)exp->is.bit_const.a, i ) ;
    bit_b = get_bit( (unsigned *)exp->is.bit_const.b, i ) ;
    v = bit_a | ( bit_b << 1 ) ;
    switch( v ) {
      case 0:
        if( !supress ) HOUT0( "0" ) ; 
        break ;
      case 1: HOUT0( "1" ) ; supress = 0 ; break ;
      case 2: HOUT0( "z" ) ; supress = 0 ; break ;
      case 3: HOUT0( "x" ) ; supress = 0 ; break ;
    }
  }
  if( supress ) HOUT0( "0" ) ;
}

static void print_expression( expression_node *exp ) {
  switch ( exp->type ) {
    case JD_string_exp:
      print_string( exp ) ;
      break ;
    case JD_varport_exp:
      HOUT1( "%s", exp->is.var_.name->name ) ;
      break ;
    case JD_null_exp:
      HOUT0( "null" ) ;
      break ;
    case JD_true_exp:
      HOUT0( "true" ) ;
      break ;
    case JD_false_exp:
      HOUT0( "false" ) ;
      break ;
    case JD_function_call_exp:
      break ;
    case JD_verilog_task_call_exp:
      break ;
    case JD_preincrement_exp:
      break ;
    case JD_predecrement_exp:
      break ;
    case JD_postincrement_exp:
      break ;
    case JD_postdecrement_exp:
      break ;
    case JD_concatenation_exp:
      print_concatenation( exp ) ;
      break ;
    case JD_duplication_exp:
      print_duplication( exp ) ;
      break ;
    case JD_list_exp:
      break ;
    case JD_real_num:
      HOUT1( "%s", exp->string ) ;
      break ;
    case JD_int_num:
      HOUT1( "%d", exp->is.int_value ) ;
      break ;
    case JD_bit_num:
      print_bit( exp ) ;
      break ;
    case JD_exp_minus_exp:     /* expression '-' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " - " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_plus_exp:      /* expression '+' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " + " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_times_exp:     /* expression '*' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " * " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_div_exp:       /* expression '/' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " / " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_mod_exp:       /* expression '%' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " % " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_and_exp:     /* expression '&' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " & " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_or_exp:      /* expression '|' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " | " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_eor_exp:     /* expression '^' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ^ " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_nand_exp:      /* expression '&~' expression  */
                               /* expression '~&' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ~& " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_nor_exp:     /* expression '|~' expression  */
                             /* expression '~|' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ~| " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_neor_exp:      /* expression '^~' expression  */
                               /* expression '~^' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ~^ " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_land_exp:      /* expression '&&' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " && " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_lor_exp:     /* expression '||' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " || " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_rshift_exp:    /* expression '>>' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " >> " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_urshift_exp:   /* expression '>>>' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " >>> " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_lshift_exp:    /* expression '<<' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " << " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_lt_exp: /* expression '<' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " < " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_gt_exp: /* expression '>' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " > " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_eqeq_exp:      /* expression '==' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " == " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_le_exp: /* expression '<=' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " <= " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_ge_exp: /* expression '>=' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " >= " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_ne_exp: /* expression '!=' expression  */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " != " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_eqeqeq_exp:    /* expression '===' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " === " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_neqeq_exp:     /* pression '!==' expression   */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " !== " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_eqQeq_exp:     /* expression '=?=' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " =?= " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_eqeqQ_exp:     /* expression '==?' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ==? " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_Qeqeq_exp:     /* expression '?==' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " ?== " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_exp_Qne_exp:     /* expression '!?=' expression */
      print_expression( exp->is.binary.left ) ;
      HOUT0( " !?= " ) ;
      print_expression( exp->is.binary.right ) ;
      break ;
    case JD_u_minus_exp:     /* '-' expression              */
      HOUT0( "-" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_tilde_exp:     /* '~' expression              */
      HOUT0( "~" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_not_exp:  /* '!' expression              */
      HOUT0( "!" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_and_exp:  /* '&' expression              */
      HOUT0( "&" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_or_exp:   /* '|' expression              */
      HOUT0( "|" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_eor_exp:  /* '^' expression              */
      HOUT0( "^" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_nand_exp: /* '~&' expression             */
                        /* '&~' expression             */
      HOUT0( "~&" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_nor_exp:  /* '~|' expression             */
                        /* '|~' expression             */
      HOUT0( "~|" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_u_neor_exp: /* '^~' expression             */
                        /* '~^' expression             */
      HOUT0( "~^" ) ;
      print_expression( exp->is.unary.this ) ;
      break ;
    case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
      print_expression( exp->is.trinary.first ) ;
      HOUT0( " =:= " ) ;
      print_expression( exp->is.trinary.second ) ;
      HOUT0( " : " ) ;
      print_expression( exp->is.trinary.third ) ;
      break ;
    case JD_conditional_exp:   /*  '?' exp ':' exp            */
      print_expression( exp->is.trinary.first ) ;
      HOUT0( " ? " ) ;
      print_expression( exp->is.trinary.second ) ;
      HOUT0( " : " ) ;
      print_expression( exp->is.trinary.third ) ;
      break ;
      
    case JD_super_exp:
      break ;
    case JD_this_exp:
      break ;

    case JD_timed_till_exp:
      break ;
    case JD_timed_while_exp:
      break ;
    case JD_parallel_or_exp:
      break ;
    case JD_parallel_and_exp:
      break ;

    case JD_assign_exp:
      break ;
    case JD_assign_list_exp:
      break ;
    case JD_rnd_exp:
    case JD_cyc_exp:
      break ;
    /*
    case JD_new_exp:    * new                         *
    case JD_new_block_exp:     * new  { <block }             *
      * should be handled by assign exp *
    case JD_port_exp:
    case JD_X_num: 
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
  
}

static void output_func_farg_var_hdr( statement_node *arg ) {
  
  if( arg->is.variable.declare->is_var ) HOUT0( "var " ) ;
  output_data_type_hdr( arg->is.variable.declare->data_type ) ;
  HOUT1( "%s", arg->is.variable.declare->var_list->name ) ;
  output_array_range_hdr( arg->is.variable.declare->var_list->info.var.range ) ;
  
  if( arg->is.variable.declare->var_list->info.var.init ) {
    HOUT0( " = " ) ;
    print_expression( 
      arg->is.variable.declare->var_list->info.var.init 
    ) ;
  }
  
}

static void output_func_farg_hdr( statement_node *arg ) {
  
  output_data_type( arg->is.formal_arg.data_type ) ;
  HOUT0( "\n" ) ;
}

static void output_a_farg_hdr( statement_node *farg ) {
  switch( farg->type ) {
    case JD_vardeclare_statement:
      output_func_farg_var_hdr( farg ) ;
      break ;
    case JD_formal_arg_statement:
      output_func_farg_hdr( farg ) ;
      break ;
    case JD_one_or_more_arg_statement:
      HOUT0( "  (" ) ;
      output_a_farg_hdr( farg->is.formal_arg.child ) ;
      HOUT0( "  )+" ) ;
      break ;
    case JD_zero_or_more_arg_statement:
      HOUT0( "  (" ) ;
      output_a_farg_hdr( farg->is.formal_arg.child ) ;
      HOUT0( "  )*" ) ;
      break ;
    case JD_wildcard_arg_statement:
      HOUT0( "  ?" ) ;
      break ;
    case JD_opt_args_statement:
      HOUT0( "  [" ) ;
      output_a_farg_hdr( farg->is.formal_arg.child ) ;
      HOUT0( "  ]" ) ;
  }
}

static void output_fargs_hdr( statement_node *fargs ) {

  while( fargs ) {
    output_a_farg_hdr( fargs ) ;
    if ( fargs->next ) {
      HOUT0( ", " ) ;
    }
    fargs = fargs->next ;
  }
}

static void def_func_header( block_node *blk, char *indent ) {
  named_node *func ;
  statement_node *fargs;
  func = blk->name ;
  fargs = blk->is.func.args ;
  if( func->is_local ) return ;
  HOUT0( indent ) ;
  if( func->is_abstract ) HOUT0( "abstract " ) ;
  if( func->is_protected ) HOUT0( "protected " ) ;
  if( func->is_final ) HOUT0( "final " ) ;
  if( func->is_export ) HOUT0( "export " ) ;
  if( func->is_overwrite ) HOUT0( "overwrite " ) ;
  if( strcmp( func->name, "new" ) ) 
    output_data_type_hdr( func->data_type ) ;
  HOUT1( "%s ( ", func->name ) ;
  /* output formal args */
  output_fargs_hdr( fargs ) ;
  
  HOUT0( " ) ;\n" ) ;
  
}

static void def_variable_hdr( 
  statement_node *variable, named_node *var, char *indent 
) {
  if( variable->is.variable.declare->is_local ) return ;
  if( variable->is.variable.declare->is_extern ) return ;
  HOUT0( indent ) ;
  if( variable->is.variable.declare->is_static ) HOUT0( "static " ) ;
  if( variable->is.variable.declare->is_final ) HOUT0( "final " ) ;
  if( variable->is.variable.declare->is_global ) HOUT0( "global " ) ;
  if( variable->is.variable.declare->is_clone ) HOUT0( "clone " ) ;
  
  output_data_type_hdr( variable->is.variable.declare->data_type ) ;
  HOUT1( " %s ", var->name ) ;
  output_array_range_hdr( 
    variable->is.variable.declare->var_list->info.var.range 
  ) ;
  HOUT0( " ;\n" ) ;
}

static void def_class_header( block_node *blk ) {
  block_node *mbk ;
  statement_node *member ;
  named_node *variable ;

  if( blk->name->name[0] == '.' ) return ; /* system class, don't output */
  if( blk->name->is_local ) return ;
  if( blk->name->is_extern ) return ;
  HOUT0( "extern " ) ;
  if( blk->name->is_abstract ) HOUT0( "abstract " ) ;
  if( blk->name->is_final ) HOUT0( "final " ) ;
  HOUT1( "class %s ", blk->name->name ) ;
  if( blk->name->info.class_name.parent ) {
    HOUT1( "extends %s ", blk->name->info.class_name.parent->name ) ;
  }
  HOUT0( "{\n" ) ;
  member = blk->is.class.edf ;
  while( member ) {
    HOUT0( "  " ) ; /* indent */
    output_enum_hdr( member->is.enum_def.edf, "  " ) ;
    member = member->next ;
  }
  
  member = blk->is.class.members ;
  while( member ) {
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      def_variable_hdr( member, variable, "  " ) ;
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  mbk = blk->is.class.member_funcs ;
  while( mbk ) {
    def_func_header( mbk, "  " ) ;
    mbk = mbk->next ;
  }  
  HOUT1( "} // end of class %s\n", blk->name->name ) ;
}

/*************** aspect header ********************/

static void def_pointcut_header( statement_node *pointcut ) {
  expression_node *p ;
  switch( pointcut->type ) {
    case JD_call_pointcut_statement: 
      HOUT0( " call(" ) ; 
      break ;
    case JD_return_pointcut_statement: 
      HOUT0( " return(" ) ;
      break ;
    case JD_overwrite_pointcut_statement: 
      HOUT0( " overwrite(" ) ; 
      break ;
  }
  p = pointcut->prefix ;
  while( p ) {
    HOUT1( "%s", p->is.string ) ;
    p = p->next ;
    if( p ) HOUT0( "." ) ;
  }
  HOUT0( ")" ) ;
}

static void def_advice_header( block_node *blk, char *indent ) {
  named_node *func ;
  statement_node *fargs, *pc;
  
  func = blk->name ;
  fargs = blk->is.func.args ;
  HOUT0( indent ) ;
  HOUT0( "advice " ) ;
  output_data_type_hdr( func->data_type ) ;
  HOUT1( "%s ( ", func->name ) ;
  /* output formal args */
  output_fargs_hdr( fargs ) ;
  
  HOUT0( " )" ) ;
  pc = blk->is.func.pointcut ;
  while( pc ) {
    def_pointcut_header( pc ) ;
    pc = pc->next ;
  }
  HOUT0( " ; \n" ) ;
}


static void def_aspect_header( block_node *blk ) {
  block_node *mbk ;
  statement_node *member ;
  named_node *variable ;

  if( blk->name->is_extern ) return ;
  HOUT0( "extern " ) ;
  HOUT1( "aspect %s ", blk->name->name ) ;
  HOUT0( "{\n" ) ;
  member = blk->is.class.edf ;
  while( member ) {
    HOUT0( "  " ) ; /* indent */
    output_enum_hdr( member->is.enum_def.edf, "  " ) ;
    member = member->next ;
  }
  
  member = blk->is.class.members ;
  while( member ) {
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      def_variable_hdr( member, variable, "  " ) ;
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  mbk = blk->is.class.member_funcs ;
  while( mbk ) {
    if( mbk->type == JD_advice_definition )
      def_advice_header( mbk, "  " ) ;
    else 
      def_func_header( mbk, "  " ) ;
    mbk = mbk->next ;
  }
  HOUT1( "} // end of class %s\n", blk->name->name ) ;
}

/* header gen top */
void headergen() {
  block_node *blk ;

  output_header_top() ;

  blk = root_block ;
  
  while( blk ) {
    switch( blk->type ) {
      case JD_port_block:
        /* port_block_header( blk ) ; */
        break ;
      case JD_portset_block:
        /* portset_block_header( blk ) ; */
        break ;
      case JD_connectionset_block:
        break ;
      case JD_local_var_dcl:
        break ;
      case JD_global_var_dcl:
        HOUT0( "\n" ) ;
        globalvar_block_header( blk ) ;
        break ;
      case JD_initial_statement_block:
        break ;
      case JD_type_definition:
        HOUT0( "\n" ) ;
        typedef_block_header( blk ) ;
        break ;
      case JD_function_definition:
        HOUT0( "\n" ) ;
        HOUT0( "extern " ) ;
        def_func_header( blk, "" ) ;
        break ;
      case JD_class_definition:
        HOUT0( "\n" ) ;
        def_class_header( blk ) ;
        break ;
      case JD_verilog_class:
        /* verilogclass_block_header( blk ) ; */
        break ;
      case JD_aspect_definition:
        HOUT0( "\n" ) ;
        def_aspect_header( blk ) ;
        break ;
        
      case JD_statement_block:
      case JD_local_variable_definition:
      case JD_memberfunction_definition:
      case JD_verilog_function:
      case JD_block_st:
      default:
        fprintf( stderr, "Illegal block type detected in headergen()\n" ) ;
        break ;
    }
    blk = blk->next ;
  }
  
}
