/*
 * Code generation for statement level
 */
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
   Copyright (C) 2002, 2003 Jeda Technologies, Inc.

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

char *eval_expression( expression_node *, int id ) ;
char *eval_new_expression( 
  expression_node *exp, expression_node *obj, int id 
) ;
int check_expression( expression_node *exp ) ;
int check_expression_list( expression_node *exp ) ;

void check_statement( statement_node *stmt ) ;
void check_block_statement( statement_node *blk ) ;
expression_node *find_clock_on_statement( statement_node *stmt ) ;

extern FILE *out ;

extern int label_num ;

extern char *current_class_name ;
extern char *current_func_name ;

extern int JD_sync_on_signal_flag ;

char *current_filename ;
int  current_linenum ;

int in_new_function ;

extern int debug_flag ;

extern file_data *compiled_files ;

int statement_id = 0 ;

int leaf_function = 0 ;

int in_aspect = 0 ;
extern int in_advice ;

label_data *block_level = NULL ;

int gl_level = 0 ;

void output_statement( statement_node *stmt, int level ) ;

/***************************************************************
 *  Basic output 
 ***************************************************************/
void output_initial(block_node *blk) {
  char *h ;
  file_data *f ;
  
  OUT3( "%s %s %s \n", JDCMP, CMPVERSION, CMPREVISION ) ;
  if( debug_flag ) {
    h = (char *)getcwd( NULL, 1024 ) ;
    OUT1( "#cwd %s\n", h ) ;
    f = compiled_files ;
    while( f ) {
      OUT2( "#file %s %s\n", f->filename, h ) ;
      f = f->next ;
    }
    free(h) ;
  }
}

char *get_data_type_name( named_node *type ) {
  switch( type->type ) {
    case JD_void_type:
      return( "void" ) ;
      break ;
    case JD_int_type:
      return( "int" ) ;
      break ;
    case JD_long_type:
      return( "long" ) ;
      break ;
    case JD_float_type:
      return( "float" ) ;
      break ;
    case JD_double_type:
      return( "double" ) ;
      break ;
    case JD_bit_type:
      return( "bit" ) ;
      break ;
    case JD_bool_type:
      return( "bool" ) ;
      break ;
    case JD_class_name:
      return( type->name ) ;
      break ;
    case JD_sys_class_name:
      return( type->name ) ;
      break ;
    case JD_string_type:
      return( "string" ) ;
      break ;
    case JD_signal_type:
      return( "signal" ) ;
      break ;
    case JD_enum_type:
      return( type->name ) ;
      break ;
    case JD_class_type:
      return( "class_type" ) ;
      break ;
    default:
      break ;
  
  }
}

int get_data_type_ub( named_node *type ) {
  if( type->type == JD_bit_type )
      return type->ub ;
  else return 0 ;
}

int get_data_type_lb( named_node *type ) {
  if( type->type == JD_bit_type )
      return type->lb ;
  else return 0 ;
}

void output_data_type( named_node *type ) {
  int size0, size1 ;
  
  switch( type->type ) {
    case JD_void_type:
      OUT0( "void " ) ;
      break ;
    case JD_int_type:
      OUT0( "int " ) ;
      break ;
    case JD_long_type:
      OUT0( "long " ) ;
      break ;
    case JD_float_type:
      OUT0( "float " ) ;
      break ;
    case JD_double_type:
      OUT0( "double " ) ;
      break ;
    case JD_bit_type:
      size1 = type->ub ;
      size0 = type->lb ;
      OUT2( "bit [%d:%d] ", size1, size0 ) ;
      break ;
    case JD_bool_type:
      OUT0( "bool " ) ;
      break ;
    case JD_class_name:
      OUT1( "class $%s ", type->name ) ;
      break ;
    case JD_sys_class_name:
      OUT1( "sys_class $%s ", type->name ) ;
      break ;
    case JD_string_type:
      OUT0( "string " ) ;
      break ;
    case JD_signal_type:
      OUT0( "signal " ) ;
      break ;
    case JD_enum_type:
      OUT1( "enum $%s ", type->name ) ;
      break ;
    case JD_class_type:
      OUT0( "class_type " ) ;
      break ;
    default:
      fprintf( stderr, "Illegal type code detected\n" ) ;
      exit(1) ;
      break ;
  
  }
}

/*
  single
  array N
  marray <dimention> <range0> <range1> .. 
  str_indexed
  assoc_array
  list
*/

void output_array_range( expression_node *range ) {
  if( range == NULL ) {
    OUT0( " single " ) ;
  }
  else {
    switch( range->type ) {
      case JD_single_range_exp:
        if( range->is.range.num_index == 1 ) {
          OUT1( " array %d ", range->is.range.int_ub ) ;
        }
        else {
          OUT1( " marray %d ( ", range->is.range.num_index ) ;
          while( range ) {
            OUT1( "%d ", range->is.range.int_ub ) ;
            range = range->next ;
          }
          OUT0( ") " ) ;
        }
        break ;
      case JD_stridx_array_exp:
        OUT0( " stridx " ) ;
        break ;
      case JD_assoc_array_exp:
        OUT0( " assoc " ) ;
        break ;
      case JD_list_array_exp:
        OUT0( " list " ) ;
        break ;
    }
  }
}

/***************************************************************
 *  Function Entry Output 
 ***************************************************************/

/* func <func_name> */
void output_func_top( named_node *func ) {
  OUT0( "\nfunc " ) ;
  OUT1( " $%s \n", func->name ) ;
  OUT1( "  gen_frame ; frame for func %s\n", func->name ) ;
  if( debug_flag ) {
    OUT1( "#gen_func_frame %s\n", func->name ) ;
  }
}

/* mfunc <class_name> <func_name> */
void output_mfunc_top( named_node *cls, named_node *func ) {
  if( in_aspect ) {
    OUT0( "\nafunc " ) ;
    OUT2( " $%s $%s \n", cls->name, func->name ) ;
    OUT1( "  gen_frame ; frame for func %s\n", func->name ) ;
    if( debug_flag ) {
      OUT2( "#gen_afunc_frame %s.%s\n", cls->name, func->name ) ;
    }
  }
  else {
    OUT0( "\nmfunc " ) ;
    OUT2( " $%s $%s \n", cls->name, func->name ) ;
    OUT2( "  chk_overwrite %d $%s\n", func->info.func.index, func->name ) ;
    OUT2( 
      "  gen_m_frame ; frame for class %s mfunc %s\n", cls->name, func->name 
    ) ;
    if( debug_flag ) {
      OUT2( "#gen_mfunc_frame %s.%s\n", cls->name, func->name ) ;
    }
  }
}

void output_abstract_mfunc( named_node *cls, named_node *func ) {
  OUT0( "\nmfunc " ) ;
  OUT2( " $%s $%s \n", cls->name, func->name ) ;
  OUT2( "  chk_overwrite %d $%s\n", func->info.func.index, func->name ) ;
  OUT2( 
    "  error ; abstract %s.%s undefined\n", cls->name, func->name 
  ) ;
  OUT0( "func_end\n" ) ;
}

/* advice <class_name> <advice_name> */
void output_advice_top( named_node *cls, named_node *func ) {
  OUT0( "\nadvice " ) ;
  OUT2( " $%s $%s \n", cls->name, func->name ) ;
  OUT1( "  gen_a_frame ; frame for func %s\n", func->name ) ;
  if( debug_flag ) {
    OUT2( "#gen_advice_frame %s.%s\n", cls->name, func->name ) ;
  }
}

/* arg <index> <data_type> <arg_name> */
/* note: init value is only usefull when called, or header generation */
void output_func_arg( statement_node *arg, int level, int index ) {
  char *var ;
  
  var = arg->is.variable.declare->is_var ? "var " : "" ;
  OUT3( "  %sarg %d %d ", var, level, index ) ;
  output_data_type( arg->is.variable.declare->data_type ) ;
  OUT1( " $%s ", arg->is.variable.declare->var_list->name ) ;
  output_array_range( arg->is.variable.declare->var_list->info.var.range ) ;
  OUT0( " \n" ) ;
  /* index is used when this arg is referenced */
  arg->is.variable.declare->var_list->info.var.index = level ;
  arg->is.variable.declare->var_list->local_var = 1 ; 
  if( debug_flag ) {
    OUT5( 
      "#argument %s %d %s %d %d\n", 
      arg->is.variable.declare->var_list->name, level,
      get_data_type_name( arg->is.variable.declare->data_type ),
      get_data_type_ub(arg->is.variable.declare->data_type),
      get_data_type_lb(arg->is.variable.declare->data_type)
    );
  }
}

int output_func_args( statement_node *args, int offset ) {
  int level = 0 ;
  statement_node *a = args;
  int l = 0 ;
  while( a ) {
    l++ ;
    a = a->next ;
  }
  while( args ) {
    output_func_arg( args, level+offset, l ) ;
    l-- ;
    level++ ;
    args = args->next ;
  }
  return level+offset ;
}

/* var <index> <data_type> <var_name> <flags> */
void output_var( named_node *variable, int level ) {
  if( variable->is_static ) {
    OUT1( "  lvar static %d ", level ) ;
    output_data_type( variable->data_type ) ;
    OUT1( " $%s ", current_class_name ) ;
    OUT1( " $%s ", current_func_name ) ;
    OUT1( " $%s ", variable->name ) ;
    output_array_range( variable->info.var.range ) ;
    /* if( variable->is_clone ) OUT0( " clone" ) ; */
    if( variable->is_extern ) OUT0( " extern" ) ;
    /* if( variable->is_local ) OUT0( " local" ) ; */
    OUT0( " \n" ) ;
  }
  else {
    OUT1( "  lvar %d ", level ) ;
    output_data_type( variable->data_type ) ;
    OUT1( " $%s ", variable->name ) ;
    output_array_range( variable->info.var.range ) ;
    if( variable->is_clone ) OUT0( " clone" ) ;
    if( variable->is_extern ) OUT0( " extern" ) ;
    /* if( variable->is_local ) OUT0( " local" ) ; */
    OUT0( " \n" ) ;
  } 
  /* the index is used when a variable is referenced */
  variable->info.var.index = level ;
  variable->local_var = 1 ; /* this func is for local var only */
  if( debug_flag ) {
    OUT5( 
      "#variable %s %d %s %d %d\n", variable->name, level, 
      get_data_type_name( variable->data_type ),
      get_data_type_ub(variable->data_type),
      get_data_type_lb(variable->data_type)
    );
  }
}

/* enum <name> <member>.. */
void output_enum( named_node *enm ) {
  named_node *member ;
  OUT1( "enum $%s ", enm->name ) ;
  member = enm->info.enum_members ;
  while( member ) {
    OUT1( "$%s ", member->name ) ;
    member = member->next ;
  }
  OUT0( " \n" ) ; 
  if( debug_flag ) {
    OUT1( "#enum %s ", enm->name ) ;
    member = enm->info.enum_members ;
    while( member ) {
      OUT1( "%s ", member->name ) ;
      member = member->next ;
    }
    OUT0( " \n" ) ; 
  }
}

int output_vars( statement_node *vars, int level ) {
  named_node *var ;
  
  if( vars->type == JD_enum_def_statement ) {
    output_enum( vars->is.enum_def.edf ) ;
  }
  else {
    var = vars->is.variable.declare->var_list ;
    while( var ) {
      output_var( var, level ) ;
      var = var->next ;
      level++ ;
    }
  }
  return level ;
}

/* clear_enum <name>, unused */
output_clear_enum( statement_node *vars ) {
  if( vars->type == JD_enum_def_statement ) {
    OUT1( "clear_enum $%s ", vars->is.enum_def.edf->name ) ;
    OUT0( " \n" ) ; 
  }
}

/***************************************************************
 *  Variable Initialization (in function) 
 ***************************************************************/

/* var initalization code */
/* store ( static | local ) level name data_type exp_type */
void output_var_init( 
  statement_node *vars, named_node *variable, int id, int level 
) {
  if( variable->info.var.init != NULL && !variable->is_static ) {
    expression_node exp, lvalue ;
    output_statement_debug_info( vars, id ) ;
    exp.type = JD_assign_exp ;
    exp.is.assign.ops = JD_assign_EQ ;
    exp.is.assign.expression = variable->info.var.init ;
    exp.is.assign.lvalue = &lvalue ;
    lvalue.type = JD_varport_exp ;
    lvalue.is.var.pre = NULL ;
    lvalue.is.var.range = NULL ;
    lvalue.is.var.depth = NULL ;
    lvalue.is.var.name = variable ;
    (void)eval_expression( &exp, id ) ;
  }
}

void output_var_inits( statement_node *vars, int id, int level ) {
  named_node *var ;
  if( vars->type != JD_enum_def_statement ) {
    var = vars->is.variable.declare->var_list ;
    while( var ) {
      output_var_init( vars, var, id, level ) ;
      var = var->next ;
    }
  }
}

/***************************************************************
 *  Block Output 
 ***************************************************************/

void output_block_statement( statement_node *blk, int id, int level ) {
  statement_node *stmt ;
  int this_level = level ;
  label_data *lb ;
  
  lb = ALLOC(label_data) ;
  
  if( block_level == NULL ) {
    block_level = lb ;
    lb->var_level = this_level ;
    lb->level = blk->is.block.level ;
  }
  else {
    lb->prev = block_level ;
    block_level = lb ;
  }
  if( debug_flag ) {
    OUT1( "#block %d \n", level ) ;
  }
  
  stmt = blk->is.block.vars ;
  while( stmt ) {
    this_level = output_vars( stmt, this_level ) ;
    block_level->this_level = this_level ;
    stmt = stmt->next ;
  }
  
  /* OUT3( "#statement %d %s %d\n", id, blk->filename, blk->linenum ) ; */
  /* output var initialize code */
  stmt = blk->is.block.vars ;
  while( stmt ) {
    output_var_inits( stmt, id, this_level ) ;
    stmt = stmt->next ;
  }
  
  stmt = blk->is.block.statements ;
  
  while( stmt ) {
    output_statement( stmt, this_level ) ;
    stmt = stmt->next ;
  }

  /* this can't support full name level
  stmt = blk->is.block.vars ;
  while( stmt ) {
    output_clear_enum( stmt ) ;
    stmt = stmt->next ;
  }
  */
  
  if( /* blk->next && */ (this_level - level) ) {
    OUT2( "  pop %0d ; block statement %d\n", this_level - level, id ) ;
  }
  block_level = lb->prev ;
  free(lb) ;
  if( debug_flag ) {
    OUT1( "#endblock %d \n", level ) ;
  }
    
}

void output_block( statement_node *blk, int level ) {
  int id =   statement_id++ ;
  output_block_statement( blk, id, level ) ;
}


/***************************************************************
 *  Leaf Function Check 
 ***************************************************************/
void check_var_init( 
  statement_node *vars, named_node *variable 
) {
  if( variable->info.var.init != NULL ) {
    leaf_function &= check_expression( variable->info.var.init ) ;
  }
}

void check_var_inits( statement_node *vars ) {
  named_node *var ;
  while( leaf_function && vars ) {
    if( vars->type != JD_enum_def_statement ) {
      var = vars->is.variable.declare->var_list ;
      while( leaf_function && var ) {
        check_var_init( vars, var ) ;
        var = var->next ;
      }
    }
    vars = vars->next ;
  }
}

void check_if_statement( statement_node *stmt ) {
  
  leaf_function &= check_expression( stmt->is.if_.cond ) ;
  /* if body */
  if( leaf_function ) check_statement( stmt->is.if_.statement ) ;
  /* No elsif */
  /* else */
  if( leaf_function && stmt->is.if_.else_statement )
    check_statement( stmt->is.if_.else_statement->is.else_.statement ) ;
  
}

void check_while_statement( statement_node *stmt ) {
  leaf_function &= check_expression( stmt->is.while_.cond ) ;
  check_statement( stmt->is.while_.statement ) ;
}

void check_for_statement( statement_node *stmt ) {
    
  leaf_function &= check_expression_list( stmt->is.for_.initial ) ;
  
  if( leaf_function && stmt->is.for_.cond ) {
    leaf_function &= check_expression( stmt->is.for_.cond ) ;
  }
  
  if( leaf_function )
    check_statement( stmt->is.for_.statement ) ;
  
  if( leaf_function )
    leaf_function &= check_expression( stmt->is.for_.initial ) ;
   
}

void check_repeat_statement( statement_node *stmt ) {

  leaf_function &= check_expression( stmt->is.repeat.number ) ;
  if( leaf_function )
    check_statement( stmt->is.repeat.statement ) ;
    
}

void check_do_while_statement( statement_node *stmt ) {

  check_statement( stmt->is.dowhile.statement ) ;

  if( leaf_function )
    leaf_function &= check_expression( stmt->is.dowhile.cond ) ;
  
}

void check_forever_statement( statement_node *stmt ) {
  check_statement( stmt->is.forever.statement ) ;
    
}

void check_case_statement( statement_node *stmt ) {
  statement_node *item ;
  expression_node *itm ;
  leaf_function &= check_expression( stmt->is.case_statement.cond ) ;
  item = stmt->is.case_statement.items ;
  
  while( leaf_function && item ) {
    if( item->type != JD_case_default ) {
      itm = item->is.case_item.cond ;
      while( leaf_function && itm ) {
        leaf_function &= check_expression( itm ) ;
        itm = itm->next ;
      }
    }
    
    if( leaf_function )
      check_statement( item->is.case_item.statement ) ;
    item = item->next ;
  }
}

void check_expression_statement( statement_node *stmt ) {
  leaf_function &= check_expression( stmt->is.exp.expression ) ;
}

void check_assign_statement( statement_node *stmt ) {
  leaf_function &= 
    check_expression( stmt->is.assign.expression ) ;
}


void check_cycle_element( statement_node *stmt ) {
  if( stmt->prefix && stmt->prefix->is.smt_prefix.cycle ) leaf_function = 0 ;
}

void check_statement( statement_node *stmt ) {

  /*  Cycle element should be OK, since it won't proceed 
    check_cycle_element( stmt ) ;
  */
  
  switch( stmt->type ) {
    case JD_null_statement:
      leaf_function &= 1 ;
      break ;
    case JD_if_statement:
      check_if_statement( stmt ) ;
      break ;
    case JD_while_statement:
      check_while_statement( stmt ) ;
      break ;
    case JD_for_statement: 
      check_for_statement( stmt ) ;
      break ;
    case JD_repeat_statement: 
      check_repeat_statement( stmt ) ;
      break ;
    case JD_do_while_statement: 
      check_do_while_statement( stmt ) ;
      break ;
    case JD_forever_statement: 
      check_forever_statement( stmt ) ;
      break ;
    case JD_case_statement: 
    case JD_casex_statement: 
    case JD_casez_statement: 
      check_case_statement( stmt ) ;
      break ;
    case JD_randcase_statement: 
    case JD_break_statement: 
    case JD_continue_statement:
      leaf_function &= 1 ;
      break ;
    case JD_fork_join_statement:
      leaf_function = 0 ;
      break ;
    case JD_terminate_statement:
    case JD_breakpoint_statement:
      leaf_function &= 1 ;
      break ;
    case JD_expression_statement:
      check_expression_statement( stmt ) ;
      break ;
    case JD_assign_statement:
      check_assign_statement( stmt ) ;
      break ;
    case JD_function_call_statement:
      leaf_function &= 1 ;
      break ;
    case JD_return_statement:
      leaf_function &= 1 ;
      break ;
    case JD_block:
      check_block_statement( stmt ) ;
      break ;
    case JD_verilog_task_call_statement:
      leaf_function &= 1 ;
      break ;
      
    case JD_enum_def_statement:
    case JD_vardeclare_statement:
    case JD_portconnect_statement:
    case JD_initial_statement:
    case JD_elsif_statement:
    case JD_else_statement:
    case JD_case_item: 
    case JD_case_default: 
    case JD_statement_list:
      /* can't happen */
      break ;
  }  
}

void check_block_statement( statement_node *blk ) {
  statement_node *stmt ;
  
  /* check var initialize code */
  stmt = blk->is.block.vars ;
  while( leaf_function && stmt ) {
    check_var_inits( stmt ) ;
    stmt = stmt->next ;
  }
  
  stmt = blk->is.block.statements ;
  
  while( leaf_function && stmt ) {
    check_statement( stmt ) ;
    stmt = stmt->next ;
  }
    
}

/***************************************************************
 *  Debug Info Output 
 ***************************************************************/

void output_statement_debug_info( statement_node *stmt, int id ) {
  current_filename = stmt->filename ;
  current_linenum = stmt->linenum ;
  OUT3( "#statement %d %s %d\n", id, stmt->filename, stmt->linenum ) ;
}

/***************************************************************
 *  Statement Output 
 ***************************************************************/

void output_null_statement( statement_node *stmt, int id, int level ) {
  OUT1( "  null  ; null statement %d\n", id ) ;
}

void output_elsif_statement( 
  statement_node *stmt, int e_label, int id, int level 
) {
  int label = label_num++ ;
  char *type ;
  OUT1( "  ; elsif statement %d\n", id ) ;  
  type = eval_expression( stmt->is.if_.cond, id ) ;
  OUT3( "  jz (%s) l%d  ; if statement %d\n", type, label, id ) ;
  output_statement( stmt->is.if_.statement, level ) ;
  OUT2( "  jmp l%d  ; elsif statement %d\n", e_label, id ) ;
  OUT2( "l%d:  ; elsif statement %d\n", label, id ) ;
}

void output_if_statement( statement_node *stmt, int id, int level ) {
  statement_node *elsif ;
  int label = label_num++ ;
  int e_label = label_num++ ;
  char *type ;
  
  OUT1( "  ; if statement %d\n", id ) ;  
  type = eval_expression( stmt->is.if_.cond, id ) ;
  OUT3( "  jz (%s) l%d  ; if statement %d\n", type, label, id ) ;
  /* if body */
  output_statement( stmt->is.if_.statement, level ) ;
  /* jump to the end */
  OUT2( "  jmp l%d  ; if statement %d\n", e_label, id ) ;
  /* if fail, eval elsif */
  OUT2( "l%d:  ; if statement %0d\n", label, id ) ;
  elsif = stmt->is.if_.elsif_statements ;
  while( elsif ) {
    output_elsif_statement( elsif, e_label, id, level ) ;
    elsif = elsif->next ;
  }
  /* else */
  if( stmt->is.if_.else_statement )
    output_statement( 
      stmt->is.if_.else_statement->is.else_.statement, level 
    ) ;
  OUT2( "l%d:  ; if statement %0d\n", e_label, id ) ;
  
}

/* pushing label info for break/continue */
void loop_statement_label( statement_node *stmt, int level, int rpt ) {
  
  push_label( 
    stmt->prefix->is.smt_prefix.label, 
    stmt->prefix->is.smt_prefix.continue_num, 
    stmt->prefix->is.smt_prefix.break_num, 
    level,
    rpt
  ) ;
  
}

void output_while_statement( statement_node *stmt, int id, int level ) {
  int label = stmt->prefix->is.smt_prefix.continue_num ;
  int e_label = stmt->prefix->is.smt_prefix.break_num ;
  char *type ;
  
  loop_statement_label( stmt, level, 0 ) ;
  OUT1( "  ; while statement %d\n", id ) ;  
  OUT2( "l%d:  ; while statement %d\n", label, id ) ;
  output_statement_debug_info( stmt, id ) ;
  type = eval_expression( stmt->is.while_.cond, id ) ;
  OUT3( "  jz (%s) l%d  ; while statement %d\n", type, e_label, id ) ;
  output_statement( stmt->is.while_.statement, level ) ;
  OUT2( "  jmp l%d  ; while statement %d\n", label, id ) ;
  OUT2( "l%d:  ; while statement %d\n", e_label, id ) ;
  pop_label() ;
}

void output_for_statement( statement_node *stmt, int id, int level ) {
  int label0, label1, label2 ;
  char *type ;
  expression_node *exp ;
  
  label0 = label_num++ ; 
  label1 = stmt->prefix->is.smt_prefix.continue_num ;
  label2 = stmt->prefix->is.smt_prefix.break_num ;
  loop_statement_label( stmt, level, 0 ) ;
  
  OUT1( "  ; for statement %d\n", id ) ; 
  exp = stmt->is.for_.initial ;
  while(exp) {
    (void)eval_expression( exp, id ) ;
    exp = exp->next ;
  }
  
  OUT2( "l%d:  ; for statement %d\n", label0, id ) ;
  output_statement_debug_info( stmt, id ) ;
  if( stmt->is.for_.cond ) {
    type = eval_expression( stmt->is.for_.cond, id ) ;
    OUT3( "  jz (%s) l%d  ; for statement %d\n", type, label2, id ) ;
  }
  
  output_statement( stmt->is.for_.statement, level ) ;
  
  OUT2( "l%d:  ; for statement %d\n", label1, id ) ;
  exp = stmt->is.for_.last ;
  while( exp ) {
    (void)eval_expression( exp, id ) ;
    exp = exp->next ;
  }
  
  /* need to put continue label, continue/break mechanism. */
    
  OUT2( "  jmp l%d  ; for statement %d\n", label0, id ) ;
  OUT2( "l%d:  ; for statement %d\n", label2, id ) ;
  pop_label() ;
  
}

void output_repeat_statement( statement_node *stmt, int id, int level ) {
  int label0, label1, label2, label3 ;
  char *type ;

  label0 = label_num++ ; 
  label1 = label_num++ ; 
  label2 = stmt->prefix->is.smt_prefix.continue_num ;
  label3 = stmt->prefix->is.smt_prefix.break_num ;

  loop_statement_label( stmt, level, 1 ) ;
  
  OUT1( "  ; repeat statement %d\n", id ) ; 
  type = eval_expression( stmt->is.repeat.number, id ) ;
  OUT1( "  push  alu  ; repeat statement %d\n", id ) ;
  OUT2( "l%d:  ; repeat statement %d\n", label0, id ) ;
  output_statement_debug_info( stmt, id ) ;
  OUT3( "  jz (%s) l%d  ; repeat statement %d\n", type, label1, id ) ;

  /* increment level since top of stack contains the temp value */
  output_statement( stmt->is.repeat.statement, level+1 ) ; 
  
  OUT2( "l%d:  ; repeat statement %d\n", label2, id ) ;
  OUT1( "  pop  alu  ; repeat statement %d\n", id ) ;
  OUT2( "  dec  (%s)  ; repeat statement %d\n", type, id ) ;
  OUT1( "  push  alu  ; repeat statement %d\n", id ) ;
  OUT2( "  jmp l%d  ; repeat statement %d\n", label0, id ) ;
  
  OUT2( "l%d:  ; repeat statement %d\n", label1, id ) ;
  OUT1( "  pop 1 ; repeat statement %d\n", id ) ;
  OUT2( "l%d:  ; repeat statement %d\n", label3, id ) ;
  
  pop_label() ;
  
}

void output_do_while_statement( statement_node *stmt, int id, int level ) {
  int label0, label1, label2 ;
  char *type ;
  
  label0 = label_num++ ; 
  label1 = stmt->prefix->is.smt_prefix.continue_num ;
  label2 = stmt->prefix->is.smt_prefix.break_num ;
  
  loop_statement_label( stmt, level, 0 ) ;
  
  OUT1( "  ; do while statement %d\n", id ) ; 
  OUT2( "l%d:  ; do while statement %d\n", label0, id ) ;

  output_statement( stmt->is.dowhile.statement, level ) ;

  OUT2( "l%d:  ; do while statement %d\n", label1, id ) ;
  output_statement_debug_info( stmt, id ) ;
  type = eval_expression( stmt->is.dowhile.cond, id ) ;
  OUT3( "  jnz (%s) l%d  ; do while statement %d\n", type, label0, id ) ;
  OUT2( "l%d:  ; do while statement %d\n", label2, id ) ;
  
  pop_label() ;
  
}

void output_forever_statement( statement_node *stmt, int id, int level ) {
  int label0, label1 ;

  label0 = stmt->prefix->is.smt_prefix.continue_num ;
  label1 = stmt->prefix->is.smt_prefix.break_num ;

  loop_statement_label( stmt, level, 0 ) ;
  
  OUT1( "  ; forever statement %d\n", id ) ; 
  OUT2( "l%d:  ; forever statement %d\n", label0, id ) ;
  
  output_statement( stmt->is.forever.statement, level ) ;
  
  output_statement_debug_info( stmt, id ) ;
  OUT2( "  jmp l%d  ; forever statement %d\n", label0, id ) ;
  OUT2( "l%d:  ; forever statement %d\n", label1, id ) ;
  
  pop_label() ;
  
}

void output_case_statement( statement_node *stmt, int id, int level, int xz ) {
  int label0, label1, label2 ;
  char *type, *type1 ;
  statement_node *item ;
  expression_node *itm ;
  int default_def = 0 ;
  
  label0 = label_num++ ;  /* exit label */
  type = eval_expression( stmt->is.case_statement.cond, id ) ;
  OUT1( "  push  alu  ; case statement %d\n", id ) ;
  item = stmt->is.case_statement.items ;
  while( item ) {
    if( item->type != JD_case_default ) {
      itm = item->is.case_item.cond ;
      label1 = label_num++ ;
      label2 = label_num++ ;
      while( itm ) {
        type1 = eval_expression( itm , id ) ;
        /* OUT1( "  dup 1   ; case statement %d\n", id ) ; */
        if( !xz ) /* case */
          OUT3( "  cmp (%s,%s)  ; case statement %d\n", type, type1, id ) ;
        else if( xz == 1 )
          OUT3( "  cmpx (%s,%s)  ; casex statement %d\n", type, type1, id ) ;
        else /* if( xz == 2 ) */
          OUT3( "  cmpz (%s,%s)  ; casez statement %d\n", type, type1, id ) ;
        OUT3( 
          "  jnz (%s) l%d  ; case statement %d\n", "int", label1, id 
        ) ;
        itm = itm->next ;
      }
      OUT2( 
        "  jmp l%d  ; case statement %d\n", label2, id 
      ) ;
    }
    
    if( item->type != JD_case_default ) {
      OUT2( "l%d:  ; case statement %d\n", label1, id ) ;
    }
    /* remove case eval value since no more evaluation */
    OUT1( "  pop  1 ; case statement %d\n", id ) ;
    output_statement( item->is.case_item.statement, level ) ;
    OUT2( "  jmp l%d  ; case statement %d\n", label0, id ) ;
    
    if( item->type != JD_case_default ) {
      OUT2( "l%d:  ; case statement %d\n", label2, id ) ;
      item = item->next ;
    }
    else {
      if( item->next != NULL ) {
        /* error */
      }
      default_def = 1 ;
      item = NULL ;
    }
  }
  if( !default_def )
    OUT1( "  pop  1 ; case statement %d\n", id ) ;
  OUT2( "l%d:  ; case statement %d\n", label0, id ) ;
}                

void output_break_statement( statement_node *stmt, int id, int level ) {
  label_data *lb ;
  
  lb = find_label(stmt->is.break_continue.label) ;
  
  if( lb ) {
    if( level - lb->level ) {
      OUT2( "  pop %0d ; break statement %d\n", level - lb->level, id ) ;
    }
  }
  else {
    /* error */
    ERROR_BREAK_LABEL_CANT_BE_FOUND( stmt->is.break_continue.label ) ;
  }

  OUT2(
    "  jmp l%d  ; break statement %d\n", 
    stmt->is.break_continue.label_num, id 
  ) ;
  
}

void output_continue_statement( statement_node *stmt, int id, int level ) {
  label_data *lb ;
  
  lb = find_label(stmt->is.break_continue.label) ;
  
  if( lb ) {
    /* if continuing on repeat statement, there must be one temp value at
       the top of stack. Thus pop just before the level. The value of
       break_continue.repeat contains number of continuous repeat
       statement to handle correct stack release. e.g.
       
         repeat( foo )
           repeat( bar ) {
             .
             .
             
             continue ;
           }
       
       This continue must release stack at the level of first repeat statement
       plus 1 since it should hold the value for repeat.
       
       This information is saved in 'repeat' flag of the target label.
       
    */
    if( level - lb->level - lb->repeat ) {
      OUT2(
        "  pop %0d ; continue statement %d\n", 
        level - lb->level - lb->repeat, id 
      ) ;
    }
  }
  else {
    /* error */
    ERROR_CONTINUE_LABEL_CANT_BE_FOUND( stmt->is.break_continue.label ) ;
  }

  OUT2(
    "  jmp l%d  ; continue statement %d\n", 
    stmt->is.break_continue.label_num, id 
  ) ;
  
}

void output_fork_join_statement( statement_node *stmt, int id, int level ) {
  int label  ;
  int i_label ;
  int c_num = 0 ;
  statement_node *child ;
  i_label = label_num++ ;  /* exit label */
  OUT1( "  ; fork/join statement %d\n", id ) ;  
  OUT1( "  fork  ; fork %0d \n", id ) ;
  child = stmt->is.fork_join.statement_list ;
  while( child ) {
    label = label_num++ ;  /* exit label */
    OUT3(
      "  exec f%0d  ; fork %0d child(%0d) \n", label, id, c_num++ 
    ) ;
    child = child->next ;
  }
  if( stmt->is.fork_join.join_flag == JD_join )
    OUT1( "  join  ; fork %0d \n", id ) ;
  else if( stmt->is.fork_join.join_flag == JD_spoon )
    OUT1( "  spoon  ; fork %0d \n", id ) ;
  else if( stmt->is.fork_join.join_flag == JD_knife )
    OUT1( "  knife  ; fork %0d \n", id ) ;
  OUT2( "  jmp l%d  ; fork statement %d\n", i_label, id ) ;
  
  /* spit out,child code */
  label = i_label ;
  child = stmt->is.fork_join.statement_list ;
  c_num=0 ;
  while( child ) {
    label++ ;  /* exit label */
    OUT3( "f%d:  ; fork %0d child(%0d) \n", label, id, c_num ) ;
    output_statement( child, level ) ;
    OUT2( "  exit  ; fork %0d child(%0d) \n", id, c_num++ ) ;
    child = child->next ;
  }
  OUT2( "l%d:  ; fork statement %d\n", i_label, id ) ;
}

void output_terminate_statement( statement_node *stmt, int id, int level ) {
  OUT1( "  terminate  ; terminate %0d \n", id ) ;
}

void output_breakpoint_statement( statement_node *stmt, int id, int level ) {
  if( stmt->is.breakpoint.filename )
    OUT2( 
      "  breakpoint %s ; breakpoint %0d \n", 
      stmt->is.breakpoint.filename, id 
    ) ;
  else 
    OUT1( "  breakpoint \"\" ; breakpoint %0d \n", id ) ;
}

void output_expression_statement( statement_node *stmt, int id, int level ) {
  (void) eval_expression( stmt->is.exp.expression, id ) ;
}

void output_assign_statement( statement_node *stmt, int id, int level ) {
  (void) eval_expression( stmt->is.assign.expression, id ) ;
}

void output_function_call_statement( statement_node *stmt, int id, int level ) {
  (void) eval_expression( stmt->is.funccall.expression, id ) ;
}

void output_type_conversion( statement_node *stmt, char *type, int id ) {
  named_node *dtype = stmt->is.ret.dtype ;
  expression_node *data = stmt->is.ret.expression ;
  
  if( dtype->sub_type == JD_single_sub_type ) {
    switch( dtype->type ) {
      case JD_int_type:
        if( strcmp( type, "int" ) ) {
          OUT1( "  to_int ; return %d\n", id ) ;
        }
        break ;
      case JD_long_type:
        if( strcmp( type, "long" ) ) {
          OUT2( "  convert long (%s) ; return %d\n", type, id ) ;
        }
        break ;
      case JD_bit_type:
        OUT4( 
          "  convert bit %d %d (%s) ; return %d\n", 
          dtype->ub, dtype->lb, type, id 
        ) ;
        break ;
      case JD_string_type:
        if( strcmp( type, "string" ) ) {
          OUT2( "  convert string (%s) ; return %d\n", type, id ) ;
        }
        break ;
      case JD_float_type:
      case JD_double_type:
        if( strcmp( type, "float" ) && strcmp( type, "double" ) ) {
          OUT2( "  convert float (%s) ; return %d\n", type, id ) ;
        }
        break ;
    }
  }
}

void output_return_statement( statement_node *stmt, int id, int level ) {
  char *type ;

  if( in_advice ) {
    if( stmt->is.ret.expression ) {
      OUT1( "  ; advice return (expression) %0d \n", id ) ;
      type = eval_expression( stmt->is.ret.expression, id ) ;
      output_type_conversion( stmt, type, id ) ;
    }
    else {
      OUT1( "  restore_acc  ; advice return (void) %0d \n", id ) ;
    }
    OUT1( "  pop_this  ; recover previous 'this'\n", id ) ;
    if( debug_flag ) {
      OUT0( "#exit_function\n" ) ;
    }
    OUT1( "  return_l  ; return (void) %0d \n", id ) ;
    return ;
  }
  
  if( stmt->is.ret.expression ) {
    OUT1( "  ; return (expression) %0d \n", id ) ;
    type = eval_expression( stmt->is.ret.expression, id ) ;
    output_type_conversion( stmt, type, id ) ;
  }
  else if( !in_new_function ) {
    OUT1( "  void  ; return (void) %0d \n", id ) ;
  }
  if( strcmp( current_class_name, "null" ) ) {
    /* returning from class member function */
    OUT1( "  pop_this  ; recover previous 'this'\n", id ) ;
  }
  if( debug_flag ) {
    OUT0( "#exit_function\n" ) ;
  }
  if( leaf_function )
    OUT1( "  return_l  ; return (x) %0d \n", id ) ;
  else
    OUT1( "  return  ; return (x) %0d \n", id ) ;
}

void output_pop_this() {
  int id = statement_id++ ;
  
  OUT1( "  pop_this  ; recover previous 'this'\n", id ) ;
}

void output_return() {
  int id = statement_id++ ;
  
  if( in_advice ) {
    if( debug_flag ) {
      OUT0( "#exit_function\n" ) ;
    }
    OUT1( "  restore_acc  ; advice return (void) %0d \n", id ) ;
    OUT1( "  pop_this  ; recover previous 'this'\n", id ) ;
    OUT1( "  return_l  ; return (void) %0d \n", id ) ;
    OUT0( "advice_end \n" ) ;
    return ;
  }
  
  if( !in_new_function )
    OUT1( "  void  ; return (void) %0d \n", id ) ;
  
  if( debug_flag ) {
    OUT0( "#exit_function\n" ) ;
  }
  if( leaf_function )
    OUT1( "  return_l  ; return (void) %0d \n", id ) ;
  else
    OUT1( "  return  ; return (void) %0d \n", id ) ;
  OUT0( "func_end \n" ) ;
}

void output_verilog_call_statement( statement_node *stmt, int id, int level ) {
  eval_expression( stmt->is.funccall.expression, id ) ;
}

void output_edge( JD_edge_type edge ) {
  switch( edge ) {
    case JD_posedge:
      OUT0( " posedge " ) ;
      break ;
    case JD_negedge:
      OUT0( " negedge " ) ;
      break ;
    case JD_bothedge:
      OUT0( " bothedge " ) ;
      break ;
    default:
      OUT0( " noedge " ) ;
      break ;
    
  }
}

void output_sync_on_signal( expression_node *exp, JD_edge_type edge, int id ) 
{
  named_node *variable = exp->is.var.name ;
  if ( variable->type == JD_port_name ) {
    /* sync on a CLOCK OF A PORT */
    if( exp->is.var.pre ) {
      OUT0( "  sync clk_of_pset_port " ) ;
      output_edge( edge ) ;
      OUT3( 
        " $%s $%s ; @n (edge clock) statement %d\n",
        exp->is.var.pre->is.var.name->name, exp->is.var.name->name, id
      ) ;
    }
    else {
      OUT0( "  sync clk_of_port " ) ;
      output_edge( edge ) ;
      OUT2( 
        " $%s ; @n (edge clock) statement %d\n",
        exp->is.var.name->name, id
      ) ;
    }
  }
  else {    
    PUSH_ALU(id) ;
    JD_sync_on_signal_flag = 1 ; /* set ghis bit to get signal */
    (void)eval_expression( exp, id ) ;
    JD_sync_on_signal_flag = 0 ;
    OUT0( "  sync clk_of_signal " ) ;
    output_edge( edge ) ;
    OUT2( 
      " ; @n on clock of signal %s %d\n",
      exp->is.var.name->name, id
    ) ;
  }
    
}

void output_cycle_element( statement_node *stmt, int id ) {
  expression_node *exp ;
  char *ptype, *dtype ;
  int nb_flag = 0 ;
  int label0, label1 ;
  
  if( stmt->prefix && stmt->prefix->is.smt_prefix.cycle ) { 
    if( stmt->type == JD_assign_statement && 
        stmt->is.assign.expression->is.assign.ops == JD_assign_NB_EQ )
    {
      nb_flag = 1 ; /* non blocking assign */
      label0 = label_num++ ;  /* exit label */
      label1 = label_num++ ;  /* assign label */
      dtype = 
        eval_expression( stmt->is.assign.expression->is.assign.expression, id );
      PUSH_ALU(id) ;
      OUT2( "  jmp l%d  ; non-block assign %d\n", label1, id ) ;
      OUT2( "f%d:   ; %d\n", label0, id ) ;
    }
    
    ptype = eval_expression( stmt->prefix->is.smt_prefix.cycle, id ) ;
    if( NOT_INT(ptype) ) TO_INT(id) ;
    exp = stmt->prefix->is.smt_prefix.clock ;
    if( exp ) {
      if( exp->is.var.name->type == JD_port_name ) {
        if( exp->is.var.pre ) {
          OUT0( "  sync pset_port " ) ;
          output_edge( stmt->prefix->is.smt_prefix.edge ) ;
          OUT3( 
            " $%s $%s ; @n (edge clock) statement %d\n",
            exp->is.var.pre->is.var.name->name, exp->is.var.name->name, id
          ) ;
        }
        else {
          OUT0( "  sync port " ) ;
          output_edge( stmt->prefix->is.smt_prefix.edge ) ;
          OUT2( 
            " $%s ; @n (edge clock) statement %d\n",
            exp->is.var.name->name, id
          ) ;
        }
      }
      else {
        /* name->data_type->type == JD_signal_type && exp->is.var.depth */
        PUSH_ALU(id) ;
        (void)eval_expression( exp, id ) ;
        OUT0( "  sync signal " ) ;
        output_edge( stmt->prefix->is.smt_prefix.edge ) ;
        OUT1( " ; @n (edge clock) statement %d\n", id ) ;
      } 
    }
    else {
      exp = find_clock_on_statement( stmt ) ;
      if( exp ) {
        output_sync_on_signal( exp, stmt->prefix->is.smt_prefix.edge, id ) ;
      }
      else {
        /* using 'CLOCK' as the clock */
        OUT0( "  sync port " ) ;
        output_edge( stmt->prefix->is.smt_prefix.edge ) ;
        OUT2( 
          " $%s ; @n statement %d\n",
          "CLOCK", id
        ) ;
      }
    }
    
    OUT3( "#statement %d %s %d\n", id, current_filename, current_linenum ) ;
    
    if( nb_flag ) {
      /* last half ob non-blocking assign */
      POP_ALU(id) ; /* data to be stored */
      store_varport( 
        stmt->is.assign.expression->is.assign.lvalue, dtype, id,
        stmt->is.assign.expression->is.assign.strength
      ) ;
      OUT1( "  exit  ; end of non-blocking assign %d \n", id ) ;
      
      OUT_LABEL( label1, id ) ;
      OUT1( "  ; fork for non-blocking assign statement %d\n", id ) ;  
      OUT1( "  fork  ; nb %0d \n", id ) ;
      OUT2( "  exec f%0d ; exec non-blocking assignment %d\n", label0, id ) ;
      OUT1( "  knife ; leaving non-blocking assignment %d\n", id ) ;
      POP(1,id) ; /* remove data to be stored */
    }
  }
}

void output_statement( statement_node *stmt, int level ) {
  statement_id++ ;
  
  /* block will generate debug statement after data allocation */
  if(stmt->type != JD_block)
    output_statement_debug_info( stmt, statement_id ) ;
    
  output_cycle_element( stmt, statement_id ) ;
  switch( stmt->type ) {
    case JD_null_statement:
      output_null_statement( stmt, statement_id, level ) ;
      break ;
    case JD_if_statement:
      output_if_statement( stmt, statement_id, level ) ;
      break ;
    case JD_while_statement:
      output_while_statement( stmt, statement_id, level ) ;
      break ;
    case JD_for_statement: 
      output_for_statement( stmt, statement_id, level ) ;
      break ;
    case JD_repeat_statement: 
      output_repeat_statement( stmt, statement_id, level ) ;
      break ;
    case JD_do_while_statement: 
      output_do_while_statement( stmt, statement_id, level ) ;
      break ;
    case JD_forever_statement: 
      output_forever_statement( stmt, statement_id, level ) ;
      break ;
    case JD_case_statement: 
    case JD_randcase_statement: 
      output_case_statement( stmt, statement_id, level, 0 ) ;
      break ;
    case JD_casex_statement: 
      output_case_statement( stmt, statement_id, level, 1 ) ;
      break ;
    case JD_casez_statement: 
      output_case_statement( stmt, statement_id, level, 2 ) ;
      break ;
    case JD_break_statement: 
      output_break_statement( stmt, statement_id, level ) ;
      break ;
    case JD_continue_statement:
      output_continue_statement( stmt, statement_id, level ) ;
      break ;
    case JD_fork_join_statement:
      output_fork_join_statement( stmt, statement_id, level ) ;
      break ;
    case JD_terminate_statement:
      output_terminate_statement( stmt, statement_id, level ) ;
      break ;
    case JD_breakpoint_statement:
      output_breakpoint_statement( stmt, statement_id, level ) ;
      break ;
    case JD_expression_statement:
      output_expression_statement( stmt, statement_id, level ) ;
      break ;
    case JD_assign_statement:
     if( stmt->prefix && stmt->prefix->is.smt_prefix.cycle && 
         stmt->is.assign.expression->is.assign.ops == JD_assign_NB_EQ ) 
      {
        /* do nothing, already done under output_cycle_element */
      }
      else output_assign_statement( stmt, statement_id, level ) ;
      break ;
    case JD_function_call_statement:
      output_function_call_statement( stmt, statement_id, level ) ;
      break ;
    case JD_return_statement:
      output_return_statement( stmt, statement_id, level ) ;
      break ;
    case JD_block:
      output_block_statement( stmt, statement_id, level ) ;
      break ;
    case JD_verilog_task_call_statement:
      output_verilog_call_statement( stmt, statement_id, level ) ;
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
}


/* var <index> <data_type> <var_name> <flags> */
void output_global_local_var( named_node *variable, int local ) {
  int level = gl_level++ ;
  OUT1( "variable %d ", level ) ;
  output_data_type( variable->data_type ) ;
  OUT1( " $%s ", variable->name ) ;
  output_array_range( variable->info.var.range ) ;
  OUT1( " %s ", local?"local":"global" ) ;
  if( variable->is_static ) OUT0( " static" ) ;
  if( variable->is_clone ) OUT0( " clone" ) ;
  if( variable->is_extern ) OUT0( " extern" ) ;
  OUT0( " \n" ) ; 
  /* the index is used when a variable is referenced */
  variable->info.var.index = level ;
  variable->info.var.local = 0 ; /*  */
}

void output_rnd_var( expression_node *rnd ) {
  int level = gl_level++ ;
  OUT1( "variable %d ", level ) ;
  if( rnd->type == JD_rnd_exp ) {
    OUT0( "  sys_class $sys_RND " ) ;
  }
  else {
    OUT0( "  sys_class $sys_CYC " ) ;
  }
  OUT1( " $RND_%d ", rnd->is.rnd.index ) ;
  OUT0( " single local" ) ;
  OUT0( " \n" ) ; 
}

#ifndef EVALEXP

char *eval_expression( expression_node *exp, int id ) {
  /* TBI */
  OUT0( "  eval_expression()\n" ) ;
  return( "exp" ) ;
}

int check_expression( expression_node *exp ) {
  return( 0 ) ;
}

#endif

void output_member_variable( char *cls, named_node *variable, int level ) {
  if( variable->is_static ) {
    OUT2( "  alloc_static $%s $null %d ", cls, level ) ;
    output_data_type( variable->data_type ) ;
    OUT1( " $%s ", variable->name ) ;
    output_array_range( variable->info.var.range ) ;
    if( variable->is_clone ) OUT0( " clone" ) ;
    OUT0( " \n" ) ;
  }
  else {
    OUT1( "  alloc %d ", level ) ;
    output_data_type( variable->data_type ) ;
    OUT1( " $%s ", variable->name ) ;
    output_array_range( variable->info.var.range ) ;
    if( variable->is_clone ) OUT0( " clone" ) ;
    OUT0( " \n" ) ;
  }
  /* the index is used when a variable is referenced */
  variable->info.var.index = level ;
}

void output_member_func( named_node *func, int level ) {
  OUT1( "  alloc_func %d ", level ) ;
  OUT1( " $%s ", func->name ) ;
  OUT0( " \n" ) ;
  /* the index is used when a variable is referenced */
  func->info.func.index = level ;
}

void output_overwrite_func( 
  named_node *parent, named_node *class, named_node *func 
) {
  OUT0( "  overwrite " ) ;
  OUT3( " $%s $%s $%s", parent->name, class->name, func->name ) ;
  OUT0( " \n" ) ;
}

void output_member_init( named_node *cls, named_node *variable ) {
  char *ptype ;
  expression_node obj ;
  if( variable->info.var.init->type == JD_new_exp || 
      variable->info.var.init->type == JD_new_block_exp
  ) {
    obj.is.var.name = variable ;
    ptype = eval_new_expression( variable->info.var.init, &obj, 0 ) ;
  }
  else 
    ptype = eval_expression( variable->info.var.init, 0 ) ;
  
  OUT5( 
    "  store_obj (%s,%s) $%s $%s ; initialize class member(%d)\n", 
    get_var_type(variable), ptype, 
    cls->name, variable->name, variable->info.var.index
  ) ;
}

void output_class_alloc( block_node *cls ) {
  statement_node *member ;
  named_node *variable ;
  block_node *member_funcs ;
  int level = 0 ;
  named_node *mfunc, *obj_type ;
  named_node *ctype, *cfunc ;
  expression_node *cnew, *args ;
  int n, tmp ;
  
  if( cls->name->is_extern ) return ;
  OUT1( "\nclass_alloc $%s\n", cls->name->name ) ;
  if( cls->name->info.class_name.parent ) {
    /* if new() is not defined and super.new() is defined, then call it
      here with default args
    */
    if( !findmember_in_class( cls->name, "new", &tmp ) ) {
      obj_type = cls->name->info.class_name.parent ;
      mfunc = findmember_in_class( obj_type, "new", &tmp ) ;
      if( mfunc ) {
        if( !arg_end_check(mfunc->info.func.args) ) {
          ERROR_LOCATION( cls->filename, cls->linenum ) ;
          ERROR_TOO_FEW_ARGUMENTS_ON_IMPLISIT_SUPER_NEW_CALL ; 
        }
        n = eval_default_args( mfunc, 0, 0 ) ;
        OUT1( 
          "  new_obj $%s ; STACK[SP++] <- super\n",
           cls->name->info.class_name.parent->name
        ) ;
        OUT3( 
          "  call_m $%s $%s %d  ; class alloc\n", 
          obj_type->name, "new", n 
        ) ;
        if(n) OUT1 ( "  pop %d ; clear args, class alloc\n", n ) ;
      }
      else {
        OUT1( 
          "  new_obj $%s ; STACK[SP++] <- super\n",
          cls->name->info.class_name.parent->name
        ) ;
      }
    }
    else {
      OUT1( 
        "  new_obj $%s ; STACK[SP++] <- super\n",
        cls->name->info.class_name.parent->name
      ) ;
    }
    OUT0( "  gen_child ; STACK[SP++] <- child of STACK[--SP]\n" ) ;
  }
  else 
    OUT0( "  new ; STACK[SP++] <- empty obj\n" ) ;
  member = cls->is.class.edf ;
  while( member ) {
    output_enum( member->is.enum_def.edf ) ;
    member = member->next ;
  }

  member = cls->is.class.members ;
  while( member ) {
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      output_member_variable( cls->name->name, variable, level++) ;
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  member_funcs = cls->is.class.member_funcs ;
  while( member_funcs ) {
    mfunc = member_funcs->name ;
    if( !mfunc->is_local && !mfunc->is_final ) {
      output_member_func( mfunc, level++ ) ;
    }
    if( mfunc->is_overwrite || mfunc->is_unabstracted ) {
      output_overwrite_func( 
        cls->name->info.class_name.parent, cls->name, mfunc  
      ) ;
    }
    member_funcs = member_funcs->next ;
  }
  
  member = cls->is.class.members ;
  while( member ) {
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      if( variable->info.var.init && !variable->is_static ) {
        output_member_init( cls->name, variable ) ;
      }
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  OUT1( "  return_n ; end of alloc %s\n", cls->name->name ) ;
  OUT0( "class_alloc_end \n" ) ;
  
}


void output_super_new( block_node *cls ) {
  statement_node *member ;
  named_node *variable ;
  block_node *member_funcs ;
  int level = 0 ;
  named_node *mfunc, *obj_type ;
  named_node *ctype, *cfunc ;
  expression_node *cnew, *args ;
  int n, tmp ;
  
  if( cls->name->is_extern ) return ;
  if( cls->name->info.class_name.parent ) {
    /* call super.new if needed */
    obj_type = cls->name->info.class_name.parent ;
    /* calling obj.new */
    mfunc = findmember_in_class( obj_type, "new", &tmp ) ;
    if( mfunc ) {
      ctype = cls->name ;
      cfunc = findmember_in_class( ctype, "new", &tmp ) ;
      if( cfunc && cfunc->info.func.func_block->is.func.block ) 
        cnew = cfunc->info.func.func_block->is.func.block->is.block.super_new ;
      else cnew = NULL ;
      n = 0 ;
      if( cnew ) {
        OUT3( "#statement %d %s %d\n", 0, cnew->filename, cnew->linenum ) ;
        args = cnew->is.new.args ;
        /* check arguments here as we can't do it on PASS_2, arg info may not be
           ready when func is defined after calling
         */
        actual_arg_check(  
          mfunc, args, NULL, cnew->filename, cnew->linenum 
        ) ;
        while( args ) {
          (void)eval_expression( args, 0 ) ;
          PUSH_ALU(0) ;
          n++ ;
          args = args->next ;
        }
      }
      else {
        int tmp, n ;
        tmp = actual_arg_checking(  
          mfunc, NULL, NULL, &n 
        ) ;
        if( tmp != 1 ) {
          ERROR_LOCATION( cls->filename, cls->linenum ) ;
          switch ( tmp ) {
            case 0:
              ERROR_TOO_FEW_ARGUMENTS_ON_IMPLISIT_SUPER_NEW_CALL ; 
              break ;
            case -1:
              ERROR_DATA_TYPE_ERROR_DETECTED_ON_NTH_ARG(n) ;
              break ;
            case -2:
              ERROR_TOO_MANY_ARGUMENTS ;        
              break ;
          }
        }
      }
      /* output missing argument from default */
      n = eval_default_args( mfunc, n, 0 ) ;
      OUT0( "  load_this  ; super.new()\n" ) ;
      OUT0( "  get_super 1 ; super.new()\n" ) ;
      OUT3( 
        "  call_m $%s $%s %d  ; super.new()\n", 
        obj_type->name, "new", n 
      ) ;
      if(n) OUT1 ( "  pop %d ; clear args, class alloc\n", n ) ;
    }
  }
  
}
