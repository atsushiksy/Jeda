/* 
    Top level of code generation
    
    Author: Atsushi Kasuya
    
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

void output_statement( statement_node *stmt, int level ) ;
void output_enum( named_node *enm ) ;
void output_data_type( named_node *type ) ;
void output_array_range( expression_node *range ) ;
void output_class_alloc( block_node *cls ) ;
void output_func_top( named_node *funk ) ;
void output_mfunc_top( named_node *cls, named_node *funk ) ;
void output_super_new( block_node *cls ) ;
void output_abstract_mfunc( named_node *cls, named_node *funk ) ;
char *eval_expression( expression_node *, int id ) ;
char *eval_new_expression( 
  expression_node *exp, expression_node *obj, int id 
) ;
void output_rnd_var( expression_node *rnd ) ;

extern int debug_flag ;

extern FILE *out ;

extern int leaf_function ;
extern int in_new_function ;
extern int in_advice ;
extern int in_aspect ;
int in_class = 0 ;

extern block_node *root_block ;

extern int class_initial_code ;

extern int jeda_util_comp_flag ;

/* for static variable allocation */
char *current_class_name = "null" ;
char *current_func_name = "null" ;

static void out_port_dir( named_node *port ) {
  switch( port->info.port.type ) {
    case JD_port_input:
      OUT2( "input [%d:%d] ", port->ub, port->lb );
      break ;
    case JD_port_output:
      OUT2( "output [%d:%d] ", port->ub, port->lb );
      break ;
    case JD_port_inout:
      OUT2( "inout [%d:%d] ", port->ub, port->lb );
      break ;
  }
}

static void out_port_edge( JD_edge_type edge ) {
  switch( edge ) {
    case JD_posedge:
      OUT0( "posedge " );
      break ;
    case JD_negedge:
      OUT0( "negedge " );
      break ;
    case JD_bothedge:
      OUT0( "bothedge " );
      break ;
    case JD_noedge:
    default:
      OUT0( "noedge " );
      break ;
  }
}

static void out_port_attrib( named_node *port ) {
  named_node *clock ;
  
  if( port->info.port.sample_edge != JD_noedge ) {
    OUT0( "sample " ) ;
    out_port_edge( port->info.port.sample_edge ) ;
    OUT1( "skew %d ", port->info.port.inputskew ) ;
  }
  else if( port->info.port.type != JD_port_output && 
           port->info.port.portset &&
           port->info.port.portset->info.port.sample_edge != JD_noedge ) 
  {
    OUT0( "sample " ) ;
    out_port_edge( port->info.port.portset->info.port.sample_edge ) ;
    OUT1( "skew %d ", port->info.port.portset->info.port.inputskew ) ;
  }
  if( port->info.port.drive_edge != JD_noedge ) {
    OUT0( "drive " ) ;
    out_port_edge( port->info.port.drive_edge ) ;
    OUT1( "skew %d ", port->info.port.outputskew ) ;
  }
  else if( port->info.port.type != JD_port_input &&
           port->info.port.portset &&
           port->info.port.portset->info.port.drive_edge != JD_noedge ) 
  {
    OUT0( "drive " ) ;
    out_port_edge( port->info.port.portset->info.port.drive_edge ) ;
    OUT1( "skew %d ", port->info.port.portset->info.port.outputskew ) ;
  }
  if( port->info.port.depth ) {
    OUT1( "depth %d ", port->info.port.depth ) ;
  }
  else if( port->info.port.type != JD_port_output &&
           port->info.port.portset &&
           port->info.port.portset->info.port.depth ) 
  {
    OUT1( "depth %d ", port->info.port.portset->info.port.depth ) ;
  }
  if( port->info.port.node_path ) {
    OUT1( "path %s ", port->info.port.node_path ) ;
  }
  if( clock = port->info.port.clock ) {
    if( clock->info.port.portset ) 
      OUT2( "@ $%s.$%s ",  clock->info.port.portset->name, clock->name ) ;
    else
      OUT1( "@ $%s ", clock->name ) ; 
  }
  else if( port->info.port.portset &&
           port->info.port.portset->info.port.clock ) 
  {
    clock = port->info.port.portset->info.port.clock ;
    if( clock->info.port.portset ) 
      OUT2( "@ $%s.$%s ",  clock->info.port.portset->name, clock->name ) ;
    else
      OUT1( "@ $%s ", clock->name ) ; 
  }
  else OUT0( "@ $CLOCK " ) ;
  
}

static void out_port( expression_node *port_exp, char *indent ) {
  named_node *port ;

  port = port_exp->is.port ;
  
  OUT1( "%sport ", indent ) ;
  out_port_dir( port ) ;
  OUT1( "$%s ", port->name ) ;
  out_port_attrib( port ) ;
  OUT0( "\n" ) ;

}

static void port_block( block_node *blk ) {

  out_port( blk->is.port, "" ) ;
  
}

static void portset_block( block_node *blk ) {
  expression_node *port_exp ;
  
  OUT1( "portset $%s {\n", blk->name->name ) ;
  port_exp = blk->is.portset.port_list ;
  while( port_exp ) {
    out_port( port_exp, "  " ) ;
    port_exp = port_exp->next ;
  }
  OUT1( "} ; end of portset %s \n", blk->name->name ) ;
}

static void connectionset_block( block_node *blk ) {
  connection_node *connect = blk->is.connection.connect_list ;
  OUT0( "\n" ) ;
  while( connect ) {
    OUT0( "connection " ) ;
    if( connect->port->info.port.portset ) 
      OUT1( "portset $%s ", connect->port->info.port.portset->name ) ;
    else OUT0( "port " ) ;
    OUT2( "$%s = %s\n", connect->port->name, connect->node_string ) ;
    connect = connect->next ;
  }
}

static void localvar_block( block_node *blk ) {
  named_node *variable ;
  
  variable = blk->is.local_global_var.dcl->is.variable.declare->var_list ;
  while( variable ) {
    output_global_local_var( variable, 1 ) ;
    variable = variable->next ;
  }
}

static void globalvar_block( block_node *blk ) {
  named_node *variable ;
  
  variable = blk->is.local_global_var.dcl->is.variable.declare->var_list ;
  while( variable ) {
    output_global_local_var( variable, 0 ) ;
    variable = variable->next ;
  }
}

static void output_global_local_var_init( named_node *variable ) {
  char *ptype ;
  expression_node obj ;
  
  if( !variable->info.var.init ) return ;
  
  if( variable->info.var.init->type == JD_new_exp || 
      variable->info.var.init->type == JD_new_block_exp
  ) {
    obj.is.var.name = variable ;
    ptype = eval_new_expression( variable->info.var.init, &obj, 0 ) ;
  }
  else 
    ptype = eval_expression( variable->info.var.init, 0 ) ;

  OUT0( "  storeg " ) ;
  OUT2( "(%s,%s) ", get_var_type(variable), ptype ) ;
  OUT1( "$%s ", variable->name ) ;
  OUT0( " \n" ) ; 
}


static void localvar_block_init( block_node *blk ) {
  named_node *variable ;
  
  variable = blk->is.local_global_var.dcl->is.variable.declare->var_list ;
  while( variable ) {
    output_global_local_var_init( variable ) ;
    variable = variable->next ;
  }
}

static void globalvar_block_init( block_node *blk ) {
  named_node *variable ;
  
  variable = blk->is.local_global_var.dcl->is.variable.declare->var_list ;
  while( variable ) {
    output_global_local_var_init( variable ) ;
    variable = variable->next ;
  }
}


static void initialstatement_block( block_node *blk ) {
  if( blk->is.initial.statement ) {
    OUT0( "initial_begin\n" ) ;
    output_statement( blk->is.initial.statement, 0 ) ;
    OUT0( "initial_end  ; end of initial\n" ) ;
  }
}

static void class_initialstatement_block( block_node *blk ) {
  statement_node *init = blk->is.class.initial ;
  
  class_initial_code = 1 ;
  while( init ) {
    OUT1( "class_initial_begin $%s\n", blk->name->name ) ;
    output_statement( init, 0 ) ;
    OUT1( "class_initial_end  ; end of initial class %s\n", blk->name->name ) ;
    init = init->next ;
  }
  class_initial_code = 0 ;
}

static void typedef_block( block_node *blk ) {
  named_node *enm ;
  if( blk->is.local_global_var.dcl ) {
    enm = blk->is.local_global_var.dcl->is.enum_def.edf ;
    output_enum( enm ) ;
  }
}

static void output_func_farg_var( statement_node *arg ) {
  
  if( arg->is.variable.declare->is_var ) OUT0( "  var " ) ;
  else OUT0( "  " ) ;
  output_data_type( arg->is.variable.declare->data_type ) ;
  OUT1( " $%s ", arg->is.variable.declare->var_list->name ) ;
  output_array_range( arg->is.variable.declare->var_list->info.var.range ) ;
  OUT0( "\n" ) ;
}

static void output_func_farg( statement_node *arg ) {
  
  OUT0( "  formal " ) ;
  output_data_type( arg->is.formal_arg.data_type ) ;
  OUT0( "\n" ) ;
}

static void output_a_farg( statement_node *farg ) {
  switch( farg->type ) {
    case JD_vardeclare_statement:
      output_func_farg_var( farg ) ;
      break ;
    case JD_formal_arg_statement:
      output_func_farg( farg ) ;
      break ;
    case JD_one_or_more_arg_statement:
      OUT0( "  (\n" ) ;
      output_a_farg( farg->is.formal_arg.child ) ;
      OUT0( "  )+\n" ) ;
      break ;
    case JD_zero_or_more_arg_statement:
      OUT0( "  (\n" ) ;
      output_a_farg( farg->is.formal_arg.child ) ;
      OUT0( "  )*\n" ) ;
      break ;
    case JD_wildcard_arg_statement:
      OUT0( "  ?\n" ) ;
      break ;
    case JD_opt_args_statement:
      OUT0( "  [\n" ) ;
      output_a_farg( farg->is.formal_arg.child ) ;
      OUT0( "  ]\n" ) ;
      break ;
  }
}

static void output_fargs( statement_node *fargs, char *indent ) {

  while( fargs ) {
    OUT0( indent ) ;
    output_a_farg( fargs ) ;
    fargs = fargs->next ;
  }
}

static void def_func( block_node *blk, int index, char *indent ) {
  named_node *func ;
  statement_node *fargs;
  char id[100] ;
  
  func = blk->name ;
  fargs = blk->is.func.args ;
  /* detect if this function is leaf or not */
  if( blk->is.func.block ) {  /* func block exist, real definition */
    leaf_function = 1 ;
    check_block_statement( blk->is.func.block ) ;
    func->is_leaf = leaf_function ;
  }
  OUT2( "%sfunction %d ", indent, index ) ;
  if( func->is_abstract ) OUT0( "abstract " ) ;
  if( func->is_local ) OUT0( "local " ) ;
  if( func->is_protected ) OUT0( "protected " ) ;
  if( func->is_final ) OUT0( "final " ) ;
  if( func->is_export ) OUT0( "export " ) ;
  if( func->is_leaf || jeda_util_comp_flag ) OUT0( "leaf " ) ;
  if( func->is_overwrite ) OUT0( "overwrite " ) ;
  if( func->is_extern ) OUT0( "extern " ) ;
  output_data_type( func->data_type ) ;
  OUT3( "$%s %d \"%s\" (\n", func->name, func->linenum, func->filename ) ;
  /* output formal args */
  sprintf( id, "%s  ", indent ) ;
  output_fargs( fargs, id ) ;
  
  OUT1( "%s)\n", indent ) ;
  
}

static void def_variable( 
  statement_node *variable, named_node *var, int num, char *indent 
) {
  OUT2( "%svariable %d ", indent, num ) ;
  
  output_data_type( variable->is.variable.declare->data_type ) ;
  OUT1( " $%s ", var->name ) ;
  output_array_range( 
    variable->is.variable.declare->var_list->info.var.range 
  ) ;
  if( variable->is.variable.declare->is_static ) OUT0( "static " ) ;
  if( variable->is.variable.declare->is_local ) OUT0( "local " ) ;
  if( variable->is.variable.declare->is_final ) OUT0( "final " ) ;
  if( variable->is.variable.declare->is_extern ) OUT0( "extern " ) ;
  if( variable->is.variable.declare->is_global ) OUT0( "global " ) ;
  if( variable->is.variable.declare->is_clone ) OUT0( "clone " ) ;
  OUT0( "\n" ) ;
}

static void def_class( block_node *blk ) {
  block_node *mbk ;
  statement_node *member ;
  named_node *variable ;
  int num = 0 ;
  /* checking consistency with parent */
  check_abstract_members( blk->name ) ;
  check_overwrite_members( blk->name ) ;
  if( blk->name->name[0] == '.' ) /* avoiding dot name */
    OUT1( "class $%s ", &blk->name->name[1] ) ;
  else 
    OUT1( "class $%s ", blk->name->name ) ;
  if( blk->name->info.class_name.parent ) {
    OUT1( "extends $%s ", blk->name->info.class_name.parent->name ) ;
  }
  if( blk->name->is_abstract ) OUT0( "abstract " ) ;
  if( blk->name->is_extern ) OUT0( "extern " ) ;
  if( blk->name->is_local ) OUT0( "local " ) ;
  if( blk->name->is_final ) OUT0( "final " ) ;
  OUT2( " %d \"%s\" {\n", blk->name->linenum, blk->name->filename ) ;
  member = blk->is.class.edf ;
  while( member ) {
    OUT0( "  " ) ; /* indent */
    output_enum( member->is.enum_def.edf ) ;
    member = member->next ;
  }
  
  member = blk->is.class.members ;
  while( member ) {
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      def_variable( member, variable, num++, "  " ) ;
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  mbk = blk->is.class.member_funcs ;
  while( mbk ) {
    if( mbk->name->is_final )
      def_func( mbk, -1, "  " ) ;
    else
      def_func( mbk, num++, "  " ) ;
    mbk = mbk->next ;
  }  
  OUT1( "}  ; end of class %s\n", blk->name->name ) ;
}

/***************************************************************
 *  Static Variable allocation
 ***************************************************************/
static void output_static_var( 
  char *class, char *func, named_node *variable 
) {
  OUT2( "  stat_var $%s $%s ", class, func ) ;
  output_data_type( variable->data_type ) ;
  OUT1( " $%s ", variable->name ) ;
  output_array_range( variable->info.var.range ) ;
  if( variable->is_static ) OUT0( " static" ) ;
  if( variable->is_clone ) OUT0( " clone" ) ;
  if( variable->is_extern ) OUT0( " extern" ) ;
  if( variable->is_local ) OUT0( " local" ) ;
  OUT0( " \n" ) ; 
}

static void output_static_vars( 
  char *class, char *func, statement_node *vars 
) {
  named_node *var ;
  
  if( vars->type != JD_enum_def_statement ) {
    var = vars->is.variable.declare->var_list ;
    while( var ) {
      if( var->is_static ) output_static_var( class, func, var ) ;
      var = var->next ;
    }
  }
}

static void output_aspect_static_vars( 
  char *class, char *func, statement_node *vars 
) {
  named_node *var ;
  
  if( vars->type != JD_enum_def_statement ) {
    var = vars->is.variable.declare->var_list ;
    while( var ) {
      var->is_static = 1 ;
      output_static_var( class, func, var ) ;
      var = var->next ;
    }
  }
}

static void function_static_vars( block_node *blk ) {
  statement_node *vars ;
  
  if( blk && blk->is.func.block && !blk->name->is_extern ) {
    vars = blk->is.func.block->is.block.vars ;
    while( vars ) {
      output_static_vars( "null", blk->name->name, vars ) ;
      vars = vars->next ;
    }
  }
}

static void member_function_static_vars( char *class, block_node *blk ) {
  statement_node *vars ;
  if( blk && blk->is.func.block ) {
    vars = blk->is.func.block->is.block.vars ;
    while( vars ) {
      output_static_vars( class, blk->name->name, vars ) ;
      vars = vars->next ;
    }
  }
}

static void class_static_vars( block_node *cls ) {
  statement_node *vars ;
  block_node *mbk ;
  if( cls->name->is_extern ) return ;
  vars = cls->is.class.members ;
  while( vars ) {
    output_static_vars( cls->name->name, "null", vars ) ;
    vars = vars->next ;
  }
  
  mbk = cls->is.class.member_funcs ;
  while( mbk ) {
    member_function_static_vars( cls->name->name, mbk ) ;
    mbk = mbk->next ;
  }  

}


/***************************************************************
 *  Static Variable initialization
 ***************************************************************/
static void output_static_var_init( 
  char *class, char *func, named_node *variable 
) {
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

  OUT0( "  store_static " ) ;
  OUT2( "(%s,%s) ", get_var_type(variable), ptype ) ;
  OUT3( "$%s $%s $%s", class, func, variable->name ) ;
  OUT0( " \n" ) ; 
}

static void output_static_vars_init( 
  char *class, char *func, statement_node *vars 
) {
  named_node *var ;
  
  if( vars->type != JD_enum_def_statement ) {
    var = vars->is.variable.declare->var_list ;
    while( var ) {
      if( var->is_static && var->info.var.init ) 
        output_static_var_init( class, func, var ) ;
      var = var->next ;
    }
  }
}

static void function_static_vars_init( block_node *blk ) {
  statement_node *vars ;
  
  if( blk && blk->is.func.block ) {
    vars = blk->is.func.block->is.block.vars ;
    while( vars ) {
      output_static_vars_init( "null", blk->name->name, vars ) ;
      vars = vars->next ;
    }
  }
}

static void member_function_static_vars_init( char *class, block_node *blk ) {
  statement_node *vars ;
  if( blk && blk->is.func.block ) {
    vars = blk->is.func.block->is.block.vars ;
    while( vars ) {
      output_static_vars_init( class, blk->name->name, vars ) ;
      vars = vars->next ;
    }
  }
}

static void class_static_vars_init( block_node *cls ) {
  statement_node *vars ;
  block_node *mbk ;
  vars = cls->is.class.members ;
  while( vars ) {
    output_static_vars_init( cls->name->name, "null", vars ) ;
    vars = vars->next ;
  }
  
  mbk = cls->is.class.member_funcs ;
  while( mbk ) {
    member_function_static_vars_init( cls->name->name, mbk ) ;
    mbk = mbk->next ;
  }  

}

/***************************************************************
 *  RND/CYC static vars alloc and init
 ***************************************************************/
extern expression_node *top_static_rnd ;

static void rnd_static_vars() {
  expression_node *rnd ;
  rnd = top_static_rnd ;
  while( rnd ) {
    output_rnd_var(rnd) ; 
    rnd = rnd->is.rnd.nxt ;
  }
}

static void init_rnd_var( expression_node *rnd ) 
{
  expression_node *tmp ;
  tmp = rnd->is.rnd.range ;
  while( tmp ) {
    if( tmp->type == JD_rand_range_exp ) {
      (void)eval_expression( tmp->is.rnd_range.lb, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      (void)eval_expression( tmp->is.rnd_range.ub, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.num_element ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.weight ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.weight_flag ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  call_m $sys_RND $set_range_d 5 ; \n" ) ;
      OUT0( "  pop 5 ; \n" ) ;
    }
    else {
      (void)eval_expression( tmp->is.rnd_range.ub, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.weight ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.weight_flag ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  call_m $sys_RND $set_range_s 3 ; \n" ) ;
      OUT0( "  pop 3 ; \n" ) ;
    }
    tmp = tmp->is.rnd_range.nxt ;
  }
  if( rnd->is.rnd.limit ) {
    OUT1( "  load_const (int) %d ;\n", rnd->is.rnd.limit ) ;
    OUT0( "  push  alu  ; \n" ) ;
    OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
    OUT0( "  call_m $sys_RND $set_limit 1 ; \n" ) ;
    OUT0( "  pop 1 ; \n" ) ;
  }
}

static void init_cyc_var( expression_node *rnd ) 
{
  expression_node *tmp ;
  tmp = rnd->is.rnd.range ;
  while( tmp ) {
    if( tmp->type == JD_rand_range_exp ) {
      (void)eval_expression( tmp->is.rnd_range.lb, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      (void)eval_expression( tmp->is.rnd_range.ub, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  load_const (int) %d ;\n", tmp->is.rnd_range.num_element ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  call_m $sys_CYC $set_range_d 3 ; \n" ) ;
      OUT0( "  pop 3 ; \n" ) ;
    }
    else {
      (void)eval_expression( tmp->is.rnd_range.ub, 0 ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  call_m $sys_CYC $set_range_s 1 ; \n" ) ;
      OUT0( "  pop 1 ; \n" ) ;
    }
    tmp = tmp->is.rnd_range.nxt ;
  }
  if( rnd->is.rnd.limit ) {
    OUT1( "  load_const (int) %d ;\n", rnd->is.rnd.limit ) ;
    OUT0( "  push  alu  ; \n" ) ;
    OUT1( "  loadg $RND_%d ; \n", rnd->is.rnd.index ) ;
    OUT0( "  call_m $sys_CYC $set_limit 1 ; \n" ) ;
    OUT0( "  pop 1 ; \n" ) ;
  }
}

static void rnd_static_vars_init() {
  expression_node *rnd ;
  int width ;
  
  rnd = top_static_rnd ;
  while( rnd ) {
    if( rnd->is.rnd.size && (rnd->is.rnd.size > rnd->is.rnd.width) ) 
      width = rnd->is.rnd.size ;
    else if( rnd->is.rnd.dst_width &&
             (rnd->is.rnd.dst_width > rnd->is.rnd.width) ) 
      width = rnd->is.rnd.dst_width ;
    else if( rnd->is.rnd.dst_width &&
             rnd->is.rnd.dst_width == -1 &&
             rnd->is.rnd.width == 0 ) 
      width = rnd->is.rnd.dst_width ;
    else width = rnd->is.rnd.width ;
    if( rnd->type == JD_rnd_exp ) {
      OUT1( "  load_const (int) %d ;\n", width ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT0( "  new_obj $sys_RND ; \n" ) ;
      OUT0( "  call_m $sys_RND $new 1 ; \n" ) ;
      OUT1( "  storeg (obj,obj) $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  pop 2 ; \n" ) ;
      init_rnd_var( rnd ) ;
    }
    else {
      OUT1( "  load_const (int) %d ;\n", width ) ;
      OUT0( "  push  alu  ; \n" ) ;
      OUT0( "  new_obj $sys_CYC ; \n" ) ;
      OUT0( "  call_m $sys_CYC $new 1 ; \n" ) ;
      OUT1( "  storeg (obj,obj) $RND_%d ; \n", rnd->is.rnd.index ) ;
      OUT0( "  pop 2 ; \n" ) ;
      init_cyc_var( rnd ) ;
    }
    rnd = rnd->is.rnd.nxt ;
  }
}

/***************************************************************
 *  Program block output
 ***************************************************************/

static void verilogclass_block( block_node *blk ) {
  block_node *mbk ;
  named_node *func ;
  statement_node *fargs;
  
  mbk = blk->is.user_veri.funcs ;
  while( mbk ) {
    func = mbk->name ;
    fargs = mbk->is.func.args ;
    OUT1( "verilog_task $%s (\n", func->name ) ;
    output_fargs( fargs, "  " ) ;
    OUT1( ") %s ", mbk->is.func.path ) ;
    OUT1( "; end of verilog task %s\n", func->name ) ;
    mbk = mbk->next ;
  }
}

static void function_block( block_node *blk ) {
  int level ;
  if( blk->is.func.block ) {  /* func block exist, real definition */
    current_func_name = blk->name->name ;
    leaf_function = 1 ;
    if( !strcmp(blk->name->name,"main") ) leaf_function = 0 ;
    check_block_statement( blk->is.func.block ) ;
    output_func_top( blk->name ) ;
    level = output_func_args( blk->is.func.args, 0 ) ; /* offset = 0 */
    output_block( blk->is.func.block, level ) ;
    output_return() ;
    current_func_name = "null" ;
  }
}

static void out_member_dbg_info( block_node *clsblk, int level ) {
  statement_node *member ;
  int index ;
  if( debug_flag ) {
    if( clsblk->name->info.class_name.parent ) {
      out_member_dbg_info( 
        clsblk->name->info.class_name.parent->info.class_name.block,
        level+1
      ) ;
    }
    member = clsblk->is.class.edf ;
    while( member ) {
      named_node *mem ;
      OUT1( "#enum %s ", member->is.enum_def.edf->name ) ;
      mem = member->is.enum_def.edf->info.enum_members ;
      while( mem ) {
        OUT1( "%s ", mem->name ) ;
        mem = mem->next ;
      }
      OUT0( " \n" ) ; 
      member = member->next ;
    }
    member = clsblk->is.class.members ;
    index = 0 ;
    while( member ) {
      named_node *mem ;
      mem = member->is.variable.declare->var_list ;
      while( mem ) {
        OUT4( 
          "#member_variable %s %s %d %d\n", 
          clsblk->name->name, mem->name, index++, level
        ) ;
        mem = mem->next ;
      }
      member = member->next ;
    }
  }
}

static void member_function_block( 
  named_node *cls, block_node *blk, block_node *clsblk 
) 
{
  int level ;
  if( blk->is.func.block ) {  /* func block exist, real definition */
    current_func_name = blk->name->name ;
    if( !strcmp(current_func_name, "new") ) in_new_function = 1 ;
    leaf_function = 1 ;
    check_block_statement( blk->is.func.block ) ;
    output_mfunc_top( cls, blk->name ) ;
    level = output_func_args( blk->is.func.args, 1 ) ; /* offset = 1 */
    out_member_dbg_info( clsblk, 0 ) ;
    if( !strcmp( blk->name->name, "new") ) {
      output_super_new( clsblk ) ;
    }
    output_block( blk->is.func.block, level ) ;
    output_pop_this() ;
    output_return() ;
    current_func_name = "null" ;
    in_new_function = 0 ;
  }
  else if( blk->name->is_abstract && !cls->is_extern ) {
    output_abstract_mfunc( cls, blk->name ) ;
  }
}

static void out_aspect_dbg_info( block_node *clsblk, int level ) {
  statement_node *member ;
  int index ;
  if( debug_flag ) {
    member = clsblk->is.class.edf ;
    while( member ) {
      named_node *mem ;
      OUT1( "#enum %s ", member->is.enum_def.edf->name ) ;
      mem = member->is.enum_def.edf->info.enum_members ;
      while( mem ) {
        OUT1( "%s ", mem->name ) ;
        mem = mem->next ;
      }
      OUT0( " \n" ) ; 
      member = member->next ;
    }
    member = clsblk->is.class.members ;
    index = 0 ;
    while( member ) {
      named_node *mem ;
      mem = member->is.variable.declare->var_list ;
      while( mem ) {
        OUT4( 
          "#aspect_variable %s %s %d %d\n", 
          clsblk->name->name, mem->name, index++, level
        ) ;
        mem = mem->next ;
      }
      member = member->next ;
    }
  }
}

static void aspect_function_block( 
  named_node *cls, block_node *blk, block_node *clsblk 
) 
{
  int level ;
  if( blk->is.func.block ) {  /* func block exist, real definition */
    current_func_name = blk->name->name ;
    leaf_function = 1 ;
    check_block_statement( blk->is.func.block ) ;
    output_mfunc_top( cls, blk->name ) ;
    level = output_func_args( blk->is.func.args, 0 ) ; /* offset = 0 */
    out_aspect_dbg_info( clsblk, 0 ) ;
    output_block( blk->is.func.block, level ) ;
    output_return() ;
    current_func_name = "null" ;
    in_new_function = 0 ;
  }
}

static void class_block( block_node *blk ) {
  block_node *mbk ;
  current_class_name = blk->name->name ;
  current_func_name = "null" ;
  output_class_alloc( blk ) ;
  in_class = 1 ;
  mbk = blk->is.class.member_funcs ;
  while( mbk ) {
    member_function_block( blk->name, mbk, blk ) ;
    mbk = mbk->next ;
  }  
  current_class_name = "null" ;
  in_class = 0 ;

}

/*************** aspect *******************/
static void out_pointcut( expression_node *p ) {
  if( p->next ) {
    out_pointcut( p->next ) ;
    OUT0( " " ) ;
  }
  if( p->type == JD_pointcut_name )
    OUT1( "$%s", p->is.string ) ;
  else
    OUT1( "%s", p->is.string ) ;
}

static void def_pointcut( statement_node *pointcut, char *indent ) {
  switch( pointcut->type ) {
    case JD_call_pointcut_statement: 
      OUT1( "%scall(", indent ) ; 
      break ;
    case JD_return_pointcut_statement: 
      OUT1( "%sreturn(", indent ) ;
      break ;
    case JD_overwrite_pointcut_statement: 
      OUT1( "%soverwrite(",indent ) ; 
      break ;
  }
  out_pointcut(  pointcut->prefix ) ;
  OUT0( ")\n") ;
}

static void def_advice( block_node *blk, int index, char *indent ) {
  named_node *func ;
  statement_node *fargs, *pointcut;
  char id[100] ;
  
  func = blk->name ;
  fargs = blk->is.func.args ;
  if( blk->is.func.block ) {  /* func block exist, real definition */
    leaf_function = 1 ;
    check_block_statement( blk->is.func.block ) ;
    func->is_leaf = leaf_function ;
  }
  pointcut = blk->is.func.pointcut ;
  OUT2( "%sadvice %d ", indent, index ) ;
  if( func->is_leaf || jeda_util_comp_flag ) OUT0( "leaf " ) ;
  output_data_type( func->data_type ) ;
  OUT3( "$%s %d \"%s\" \n", func->name, func->linenum, func->filename ) ;
  while( pointcut ) {
    def_pointcut(pointcut, indent) ;
    pointcut = pointcut->next ;
  }
  OUT1( "%s(\n", indent ) ;
  /* output formal args */
  sprintf( id, "%s  ", indent ) ;
  output_fargs( fargs, id ) ;
  OUT1( "%s)\n", indent ) ;
  
}

static void def_aspect( block_node *blk ) {
  block_node *mbk ;
  statement_node *member ;
  named_node *variable ;
  int num = 0 ;
  OUT1( "aspect $%s ", blk->name->name ) ;
  if( blk->name->is_extern ) OUT0( "extern " ) ;
  OUT2( " %d \"%s\" {\n", blk->name->linenum, blk->name->filename ) ;
  member = blk->is.class.edf ;
  while( member ) {
    OUT0( "  " ) ; /* indent */
    output_enum( member->is.enum_def.edf ) ;
    member = member->next ;
  }
  
  member = blk->is.class.members ;
  while( member ) {
    member->is.variable.declare->is_static = 1 ;
    variable = member->is.variable.declare->var_list ;
    while( variable ) {
      def_variable( member, variable, num++, "  " ) ;
      variable = variable->next ;
    }
    member = member->next ;
  }
  
  mbk = blk->is.class.member_funcs ;
  while( mbk ) {
    if( mbk->type == JD_advice_definition )
      def_advice( mbk, -1, "  " ) ;
    else
      def_func( mbk, -1, "  " ) ;
    mbk = mbk->next ;
  }  
  OUT1( "}  ; end of aspect %s\n", blk->name->name ) ;
}

static void aspect_static_vars( block_node *asp ) {
  statement_node *vars ;
  block_node *mbk ;
  if( asp->name->is_extern ) return ;
  vars = asp->is.class.members ;
  while( vars ) {
    output_aspect_static_vars( asp->name->name, "null", vars ) ;
    vars = vars->next ;
  }
  
  mbk = asp->is.class.member_funcs ;
  while( mbk ) {
    member_function_static_vars( asp->name->name, mbk ) ;
    mbk = mbk->next ;
  }  

}

static void advice_block( 
  named_node *cls, block_node *blk, block_node *clsblk 
) 
{
  int level ;
  if( blk->is.func.block ) {  /* func block exist, real definition */
    current_func_name = blk->name->name ;
    in_advice = 1 ;
    output_advice_top( cls, blk->name ) ;
    level = output_func_args( blk->is.func.args, 1 ) ; /* offset = 0 */
    out_aspect_dbg_info( clsblk, 0 ) ;
    output_block( blk->is.func.block, level ) ;
    output_pop_this() ;
    output_return() ;
    current_func_name = "null" ;
    in_advice = 0 ;
  }
}


static void aspect_block( block_node *blk ) {
  block_node *mbk ;

  current_class_name = blk->name->name ;
  current_func_name = "null" ;
  mbk = blk->is.class.member_funcs ;
  in_aspect = 1 ;
  while( mbk ) {
    if( mbk->type == JD_advice_definition )
      advice_block( blk->name, mbk, blk ) ;    
    else
      aspect_function_block( blk->name, mbk, blk ) ;
    mbk = mbk->next ;
  }  
  current_class_name = "null" ;
  in_aspect = 0 ;

}

/*************** codegen top *******************/
void codegen() {
  block_node *blk ;
  
  blk = root_block ;
  while( blk && !blk->filename ) blk = blk->next ;
  output_initial(blk) ;
  
  blk = root_block ;
  
  while( blk ) {
    OUT0( "\n" ) ;
    switch( blk->type ) {
      case JD_port_block:
        port_block( blk ) ;
        break ;
      case JD_portset_block:
        portset_block( blk ) ;
        break ;
      case JD_connectionset_block:
        connectionset_block( blk ) ;
        break ;
      case JD_local_var_dcl:
        localvar_block( blk ) ;
        break ;
      case JD_global_var_dcl:
        globalvar_block( blk ) ;
        break ;
      case JD_initial_statement_block:
        break ;
      case JD_type_definition:
        typedef_block( blk ) ;
        break ;
      case JD_function_definition:
        def_func( blk, -1, "" ) ;
        break ;
      case JD_class_definition:
        def_class( blk ) ;
        break ;
      case JD_verilog_class:
        verilogclass_block( blk ) ;
        break ;
      case JD_aspect_definition:
        def_aspect( blk ) ;
        break ;
        
      case JD_statement_block:
      case JD_local_variable_definition:
      case JD_memberfunction_definition:
      case JD_verilog_function:
      case JD_block_st:
      default:
        fprintf( stderr, "Illegal block type detected in codegen()\n" ) ;
        break ;
    }
    blk = blk->next ;
  }

  if( jeda_util_comp_flag ) return ;
  
  blk = root_block ;
  
  while( blk ) {
    switch( blk->type ) {
      case JD_function_definition:
        function_static_vars( blk ) ;
        break ;
      case JD_class_definition:
        class_static_vars( blk ) ;
        break ;
      case JD_aspect_definition:
        aspect_static_vars( blk ) ;
        break ;
      default:
        break ;
    }
    blk = blk->next ;
  }
  
  /* RND/CYC static vars */
  rnd_static_vars() ;
  
  blk = root_block ;
  
  OUT0( "static_init  \n" ) ;
  while( blk ) {
    switch( blk->type ) {
      case JD_local_var_dcl:
        localvar_block_init( blk ) ;
        break ;
      case JD_global_var_dcl:
        globalvar_block_init( blk ) ;
        break ;
      case JD_function_definition:
        function_static_vars_init( blk ) ;
        break ;
      case JD_class_definition:
        class_static_vars_init( blk ) ;
        break ;
      case JD_aspect_definition:
        class_static_vars_init( blk ) ;
        break ;
      default:
        break ;
    }
    blk = blk->next ;
  }

  /* RND/CYC static vars */
  rnd_static_vars_init() ;
  OUT0( "static_init_end \n" ) ;

  blk = root_block ;
  
  while( blk ) {
    switch( blk->type ) {
      case JD_initial_statement_block:
        initialstatement_block( blk ) ;
        break ;
      case JD_class_definition:
        class_initialstatement_block( blk ) ;
        break ;
      case JD_aspect_definition:
        class_initialstatement_block( blk ) ;
        break ;
      default:
        break ;
    }
    blk = blk->next ;
  }

  blk = root_block ;
  
  while( blk ) {
    switch( blk->type ) {
      case JD_function_definition:
        function_block( blk ) ;
        break ;
      case JD_class_definition:
        class_block( blk ) ;
        break ;
      case JD_aspect_definition:
        aspect_block( blk ) ;
        break ;
        
      default:
        break ;
    }
    blk = blk->next ;
  }
  
  
}

