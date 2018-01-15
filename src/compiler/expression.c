/*
 *  Compile expressions
 *
 *  Author: Atsushi Kasuya
 *
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
expression_node *find_clock_on_expression_list( expression_node *exp ) ;
void output_edge( JD_edge_type edge ) ;
void output_sync_on_signal( expression_node *exp, JD_edge_type edge, int id ) ;

void output_bit_index( expression_node *range, int id ) ;

extern FILE *out ;

extern int label_num ;

extern label_data *block_level ;

extern int class_initial_code ;
extern int in_aspect ;
extern int in_advice ;
extern int in_class ;

extern char *current_filename ;
extern int  current_linenum ;
extern int debug_flag ;

extern char *current_class_name ;
extern char *current_func_name ;

int var_flag = 0 ;

/* JD_sync_on_signal_flag is set when sync clk_of_signal is being 
   constructed as the result of implisit clock search. 
   When this bit is set variable evaluation will ignore the depth and 
   sub-bit access to get the signal as the result.
 */
int JD_sync_on_signal_flag = 0 ;


char *eval_super_exp( int id ) ;
char *eval_this_exp( expression_node *exp, int id ) ;

static void output_expression_debug_info( int id ) {
  if( debug_flag ) {
    OUT3( "#statement %d %s %d\n", id, current_filename, current_linenum ) ;
  }
}

static int get_type( char *tstr ) {
  if( !strcmp( tstr, STR_TXT ) ) return STR_TYP ;
  if( !strcmp( tstr, VOID_TXT ) ) return VOID_TYP ;
  if( !strcmp( tstr, NULL_TXT ) ) return NULL_TYP ;
  if( !strcmp( tstr, INT_TXT ) ) return INT_TYP ;
  if( !strcmp( tstr, BIT_TXT ) ) return BIT_TYP ;
  if( !strcmp( tstr, FLOAT_TXT ) ) return FLOAT_TYP ;
  if( !strcmp( tstr, PTSET_TXT ) ) return PTSET_TYP ;
  if( !strcmp( tstr, SIGNAL_TXT ) ) return SIGNAL_TYP ;
  if( !strcmp( tstr, ENUM_TXT ) ) return ENUM_TYP ;
  if( !strcmp( tstr, FLOAT_TXT ) ) return FLOAT_TYP ;
  if( !strcmp( tstr, OBJ_TXT ) ) return OBJ_TYP ;
  if( !strcmp( tstr, ARRY_TXT ) ) return ARRY_TYP ;
  if( !strcmp( tstr, MARRY_TXT ) ) return MARRY_TYP ;
  if( !strcmp( tstr, ASOC_TXT ) ) return ASOC_TYP ;
  if( !strcmp( tstr, STIX_TXT ) ) return STIX_TYP ;
  if( !strcmp( tstr, LIST_TXT ) ) return LIST_TYP ;
  return NO_TYP ; 
}

char *type_string_of_variable( named_node *name ) {
  if( name->type == JD_port_name ) {
    return( SIGNAL_TXT ) ;
  }
  else if( name->type == JD_portset_name ) {
    return( PTSET_TXT ) ;
  }
  else {
    switch( name->data_type->type ) {
      case JD_void_type:
        return( VOID_TXT ) ;
        break ;
      case JD_int_type:
      case JD_long_type:
      case JD_bool_type:
        return( INT_TXT ) ;
        break ;
      case JD_float_type:
      case JD_double_type:
        return( FLOAT_TXT ) ;
        break ;
      case JD_bit_type:
        return( BIT_TXT ) ;
        break ;
      case JD_class_name:
      case JD_sys_class_name:
        return( OBJ_TXT ) ;
        break ;
      case JD_string_type:
        return( STR_TXT ) ;
        break ;
      case JD_signal_type:
        return( SIGNAL_TXT ) ;
        break ;
      case JD_enum_type:
        return( ENUM_TXT ) ;
        break ;
      default:
        fprintf( stderr, "Error unknown type in type_string_of_variable\n" ) ;
        break ;
    }
    return( NULL ) ;
  }
}

char *type_string_of( named_node *data_type ) {
  switch( data_type->type ) {
    case JD_void_type:
      return( VOID_TXT ) ;
      break ;
    case JD_int_type:
    case JD_long_type:
    case JD_bool_type:
      return( INT_TXT ) ;
      break ;
    case JD_float_type:
    case JD_double_type:
      return( FLOAT_TXT ) ;
      break ;
    case JD_bit_type:
      return( BIT_TXT ) ;
      break ;
    case JD_class_name:
    case JD_sys_class_name:
      return( OBJ_TXT ) ;
      break ;
    case JD_string_type:
     return( STR_TXT ) ;
      break ;
    case JD_signal_type:
      return( SIGNAL_TXT ) ;
      break ;
    case JD_enum_type:
      return( ENUM_TXT ) ;
      break ;
    default:
      fprintf( stderr, "Error unknown type in type_string_of\n" ) ;
      break ;
  }
  return( NULL ) ;
}


char *eval_string( expression_node *exp, int id ) {
  char *p = exp->is.string ;
  
  OUT1( "  ; string exp %d\n", id ) ;
  OUT0( "  load_const (string) " ) ;
  while( *p != '\0' ) {
    OUT1( "%c", *p++ ) ;
  }
  OUT1( "  ; string exp %d\n", id ) ;
  return( STR_TXT ) ;
}

char *output_load_single( 
  named_node *variable, expression_node *range, expression_node *depth, int id 
) {
  char *type ;
  if( variable->type == JD_port_name ) {
    if( range == NULL || JD_sync_on_signal_flag ) {
      if( depth && !JD_sync_on_signal_flag ) {
        eval_expression( depth, id ) ;
        OUT0( "  load_port " ) ;
        OUTPORT(variable, id) ;
        return( BIT_TXT ) ;
      }
      else {
        OUT0( "  load_port_ptr " ) ;
        OUTPORT(variable, id) ;
        return( SIGNAL_TXT ) ;
      }
    }
    else { /* range != NULL */
      if( range->type == JD_bit_range_exp ) {
        /* [ub:lb] */ 
        type = eval_expression( range->is.range.ub, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        PUSH_ALU(id) ;
        type = eval_expression( range->is.range.lb, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
      }
      else {
        /* one bit access to a bit variable */
        type = eval_expression( range->is.range.ub, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        PUSH_ALU(id) ;
      }
      if( depth ) {
        PUSH_ALU(id) ;
        eval_expression( depth, id ) ;
        OUT0( "  load_port subbit " ) ;
        OUTPORT(variable, id) ;
        return( BIT_TXT ) ;
      }
      else {
        OUT0( "  load_port_ptr subbit " ) ;
        OUTPORT(variable, id) ;
        return( SIGNAL_TXT ) ;
      }
    }
  }
  else {  /* variable is not a port */
    if( variable->local_var ) {
      OUT0( "  loadl " ) ;
      if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      OUT0( "  loadg " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTGLOBAL( variable, id ) ;
    }
    if( range == NULL || JD_sync_on_signal_flag ) {
      if( depth && !JD_sync_on_signal_flag ) {
        /* loading signal port with given depth */
        /* this should only happens for signal variable */
        PUSH_ALU(id) ;
        eval_expression( depth, id ) ;
        OUT3( 
          "  load_sig_pt  ; var %s.%d %d\n", 
          variable->name, depth->is.int_value , id
        ) ;
        return( BIT_TXT ) ;
      }
      switch ( variable->sub_type ) {
        case JD_single_sub_type: 
          return( type_string_of_variable( variable ) ) ;
          break ;
        case JD_array_sub_type:
          return( ARRY_TXT ) ;
          break ;
        case JD_multi_array_sub_type:
          return( MARRY_TXT ) ;
          break ;
        case JD_assoc_sub_type:
          return( ASOC_TXT ) ;
          break ;
        case JD_stridx_sub_type:
          return( STIX_TXT ) ;
          break ;
        case JD_list_sub_type:
          return( LIST_TXT ) ;
          break ;
      }
    }
    else {  /* range is specified */
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
      if( depth ) {
        /* loading signal port with given depth */
        /* this should only happens for signal variable */
        PUSH_ALU(id) ;
        eval_expression( depth, id ) ;
        OUT3( 
          "  load_sig_pt subbit ; var %s.%d %d\n", 
          variable->name,depth->is.int_value , id
        ) ;
        return( BIT_TXT ) ;
      }
      else {
        if( port_type( variable ) ) {
          OUT2( 
            "  get_sig_subbit ; signal %s[u:l] %d\n", 
            variable->name , id
          ) ;
          return( SIGNAL_TXT ) ;
        }
        else {
          OUT0(" get_subbit ") ;
          if( variable->data_type->i_endian ) 
            OUT2( 
              " i_endian[%d:%d] ", 
              variable->data_type->ub, variable->data_type->lb
            ) ;
          else if( variable->data_type->lb > 0 )
            OUT2( 
              " nz_lsb[%d:%d] ", 
              variable->data_type->ub, variable->data_type->lb
            ) ;
          OUT2(" ; %s[x] %d\n", variable->name, id) ;
          return( BIT_TXT ) ;
        }
      }
    }
  }
  return( NULL ) ;
}

void output_bit_access (
  named_node *variable, expression_node *range, int id
) {
  char *type ;
  /* case that bit-access index is at the end */
  while( range->next ) range = range->next ; /* get the last one */
  /* [ub:lb] */ 
  if( range->type == JD_bit_range_exp ) {
    /* [ub:lb] */ 
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.lb, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    OUT0(" get_subbit ") ;
    if( variable->data_type->i_endian ) 
      OUT2( 
        " i_endian[%d:%d] ", variable->data_type->ub, variable->data_type->lb
      ) ;
    else if( variable->data_type->lb > 0 )
      OUT2( 
        " nz_lsb[%d:%d] ", variable->data_type->ub, variable->data_type->lb
      ) ;
    OUT2(" ; %s[x] %d\n", variable->name, id) ;
  }
  else {
    /* one bit access to a bit variable */
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
    OUT0(" get_subbit ") ;
    if( variable->data_type->i_endian ) 
      OUT2( 
        " i_endian[%d:%d] ", variable->data_type->ub, variable->data_type->lb
      ) ;
    else if( variable->data_type->lb > 0 )
      OUT2( 
        " nz_lsb[%d:%d] ", variable->data_type->ub, variable->data_type->lb 
      ) ;
    OUT2(" ; %s[x] %d\n", variable->name, id) ;
  }

}

char *output_load_array( 
  named_node *variable, expression_node *range, expression_node *depth, int id 
) {
  char *type ;
  /* note: by the syntax limitation, array index access can't have
     the depth except the sub-bit access
     
     sigset.pt.0[5] -- possible
     
     sigarray[5].0  -- not allowed
     
  */
   
  if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    (void)eval_expression( range->is.range.ub, id ) ;
    if( variable->local_var ) {
      OUT0( "  loadl stridx " ) ;
      if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      OUT0( "  loadg stridx " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTGLOBAL(  variable, id ) ;
    }
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {
    /* single dimension array */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) )
      TO_INT(id) ;
    if( variable->local_var ) {
      OUT1( "  loadl %s ", ARRAY_TYPE ) ;
      DIM_OUT ;
      if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      OUT1( "  loadg %s ", ARRAY_TYPE ) ;
      DIM_OUT ;
      if( var_flag ) OUT0( "var " ) ;
      OUTGLOBAL(  variable, id ) ;
    }
  }
  else if( variable->sub_type == JD_list_sub_type ) {
    /* trial: direct list access */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    if( variable->local_var ) {
      OUT0( "  loadl list " ) ;
      if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      OUT0( "  loadg list " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTGLOBAL(  variable, id ) ;
    }
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if( dim ) {
        PUSH_ALU(id) ;
      }
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT( type ) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    a_range = variable->info.var.range ;
    if( variable->local_var ) {
      OUT1( "  loadl marray %d ( ", a_range->is.range.num_index ) ;
      while( a_range ) {
        OUT1( "%d ", a_range->is.range.int_ub ) ;
        a_range = a_range->next ;
      }
      OUT0( ") " ) ;
      if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      OUT1( "  loadg marray %d ( ", range->is.range.num_index ) ;
      while( a_range ) {
        OUT1( "%d ", a_range->is.range.int_ub ) ;
        a_range = a_range->next ;
      }
      OUT0( ") " ) ;
      if( var_flag ) OUT0( "var " ) ;
      OUTGLOBAL(  variable, id ) ;
    }
  }
  if( depth && !JD_sync_on_signal_flag) {
    /* loading signal port with given depth */
    /* this should only happens for signal variable */
    PUSH_ALU(id) ;
    if( range->is.range.bit_index == 0 ) {
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt  ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    else {
      output_bit_index( range, id ) ;
      PUSH_ALU(id) ;
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt subbit ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    return( BIT_TXT ) ;
  }
  else {
    if( port_type( variable ) ) {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag ) {
        while( range->next ) range = range->next ;
        PUSH_ALU(id) ;
        type = eval_expression( range->is.range.ub, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        PUSH_ALU(id) ;
        type = eval_expression( range->is.range.lb, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        OUT2( 
          "  get_sig_subbit ; var %s[u:l] %d\n", 
          variable->name , id
        ) ;
      }
      return( type_string_of_variable( variable ) ) ;
    }
    else {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag ) {
        output_bit_access( variable, range, id ) ;
        return( BIT_TXT ) ;
      }
      else return( type_string_of_variable( variable ) ) ;
    }
  }
}

char *output_load_static_member_single( 
  char *pre_name, named_node *variable, expression_node *range, expression_node *depth, int id 
) {
  
  OUT3( 
    "  load_static $%s $null $%s ; %d\n", pre_name, variable->name, id 
  ) ;
  if( range == NULL || JD_sync_on_signal_flag ) {
    if( depth && !JD_sync_on_signal_flag ) {
      /* loading signal port with given depth */
      /* this should only happens for signal variable */
      PUSH_ALU(id) ;
      (void)eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt  ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
      return( BIT_TXT ) ;
    }
    
    switch ( variable->sub_type ) {
      case JD_single_sub_type: 
        return( type_string_of_variable( variable ) ) ;
        break ;
      case JD_array_sub_type:
        return( ARRY_TXT ) ;
        break ;
      case JD_multi_array_sub_type:
        return( MARRY_TXT ) ;
        break ;
      case JD_assoc_sub_type:
        return( ASOC_TXT ) ;
        break ;
      case JD_stridx_sub_type:
        return( STIX_TXT ) ;
        break ;
      case JD_list_sub_type:
        return( LIST_TXT ) ;
        break ;
    }
  }
  else { /* range specified */
    PUSH_ALU(id) ;
    output_bit_index( range, id ) ;
    if( depth ) {
      /* loading signal port with given depth */
      /* this should only happens for signal variable */
      PUSH_ALU(id) ;
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt subbit ; var %s.%d %d\n", 
        variable->name,depth->is.int_value , id
      ) ;
      return( BIT_TXT ) ;
    }
    else {
      if( port_type( variable ) ) {
        OUT2( 
          "  get_sig_subbit ; signal %s[u:l] %d\n", 
          variable->name , id
        ) ;
        return( SIGNAL_TXT ) ;
      }
      else {
        OUT0(" get_subbit ") ;
        if( variable->data_type->i_endian ) 
          OUT2( 
            " i_endian[%d:%d] ", 
            variable->data_type->ub, variable->data_type->lb
          ) ;
        else if( variable->data_type->lb > 0 )
          OUT2( 
            " nz_lsb[%d:%d] ", 
            variable->data_type->ub, variable->data_type->lb
          ) ;
        OUT2(" ; %s[x] %d\n", variable->name, id) ;
        return( BIT_TXT ) ;
      }
    }
  }
  return( NULL ) ;
}

char *output_load_member_single( 
  char *pre_name, named_node *variable, 
  expression_node *range, expression_node *depth, int id 
) {
  char *type ;
  
  if( variable->type == JD_port_name ) {
    if( range == NULL || JD_sync_on_signal_flag) {
      if( depth && !JD_sync_on_signal_flag ) {
        (void)eval_expression( depth, id ) ;
        OUT0( "  load_pset_port " ) ;
        OUTPSETPORT( pre_name, variable, id) ;
        return( BIT_TXT ) ;
      }
      else {
        OUT0( "  load_pset_port_ptr " ) ;
        OUTPSETPORT( pre_name, variable, id) ;
        return( SIGNAL_TXT ) ;
      }
    }
    else { /* range != NULL */
      if( range->type == JD_bit_range_exp ) {
        /* [ub:lb] */ 
        type = eval_expression( range->is.range.ub, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        PUSH_ALU(id) ;
        type = eval_expression( range->is.range.lb, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
      }
      else {
        /* one bit access to a bit variable */
        type = eval_expression( range->is.range.ub, id ) ;
        if( NOT_INT( type ) ) TO_INT(id) ;
        PUSH_ALU(id) ;
      }
      if( depth ) {
        PUSH_ALU(id) ;
        eval_expression( depth, id ) ;
        OUT0( "  load_pset_port subbit " ) ;
        OUTPSETPORT( pre_name, variable, id) ;
        return( BIT_TXT ) ;
      }
      else {
        OUT0( "  load_pset_port_ptr subbit " ) ;
        OUTPSETPORT( pre_name, variable, id) ;
        return( SIGNAL_TXT ) ;
      }
    }
  }
  else {
    OUT0( "  loadm " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUTMEMBER( pre_name, variable, id) ;
    if( range == NULL || JD_sync_on_signal_flag ) {
      if( depth && !JD_sync_on_signal_flag ) {
        /* loading signal port with given depth */
        /* this should only happens for signal variable */
        PUSH_ALU(id) ;
        (void)eval_expression( depth, id ) ;
        OUT3( 
          "  load_sig_pt  ; var %s.%d %d\n", 
          variable->name, depth->is.int_value , id
        ) ;
        return( BIT_TXT ) ;
      }
    
      switch ( variable->sub_type ) {
        case JD_single_sub_type: 
          return( type_string_of_variable( variable ) ) ;
          break ;
        case JD_array_sub_type:
          return( ARRY_TXT ) ;
          break ;
        case JD_multi_array_sub_type:
          return( MARRY_TXT ) ;
          break ;
        case JD_assoc_sub_type:
          return( ASOC_TXT ) ;
          break ;
        case JD_stridx_sub_type:
          return( STIX_TXT ) ;
          break ;
        case JD_list_sub_type:
          return( LIST_TXT ) ;
          break ;
      }
    }
    else {  /* range != NULL */
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
      if( depth ) {
        /* loading signal port with given depth */
        /* this should only happens for signal variable */
        PUSH_ALU(id) ;
        eval_expression( depth, id ) ;
        OUT3( 
          "  load_sig_pt subbit ; var %s.%d %d\n", 
          variable->name,depth->is.int_value , id
        ) ;
        return( BIT_TXT ) ;
      }
      else {
        if( port_type( variable ) ) {
          OUT2( 
            "  get_sig_subbit ; signal %s[u:l] %d\n", 
            variable->name , id
          ) ;
          return( SIGNAL_TXT ) ;
        }
        else {
          OUT0(" get_subbit ") ;
          if( variable->data_type->i_endian ) 
            OUT2( 
              " i_endian[%d:%d] ", 
              variable->data_type->ub, variable->data_type->lb
            ) ;
          else if( variable->data_type->lb > 0 )
            OUT2( 
              " nz_lsb[%d:%d] ", 
              variable->data_type->ub, variable->data_type->lb
            ) ;
          OUT2(" ; %s[x] %d\n", variable->name, id) ;
          return( BIT_TXT ) ;
        }
      }
    }
  }
  return( NULL ) ;
}

char *output_load_static_member_array( 
  char *pre_name, named_node *variable, 
  expression_node *range, expression_node *depth, int id 
) {
  char *type ;
  if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    (void)eval_expression( range->is.range.ub, id ) ;
    OUT3( 
      "  load_static stridx $%s $null $%s ; %d\n", 
      pre_name, variable->name, id 
    ) ;
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {
    /* single dimension array */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT(type) ) TO_INT(id) ;
    if( variable->sub_type == JD_array_sub_type ) {
      OUT0( "  load_static array " ) ;
    }
    else {
      OUT0( "  load_static assoc " ) ;
    }
    OUT3( 
      "$%s $null $%s ; %d\n", pre_name, variable->name, id 
    ) ;
  }
  else if( variable->sub_type == JD_list_sub_type ) {
    /* trial: direct list access */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    OUT0( "  load_static list " ) ;
    OUT3( 
      "$%s $null $%s ; %d\n", pre_name, variable->name, id 
    ) ;
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if( dim ) {
        PUSH_ALU(id) ;
      }
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT( type ) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    a_range = variable->info.var.range ;
    OUT1( "  load_static marray %d (", a_range->is.range.num_index ) ;
    while( a_range ) {
      OUT1( "%d ", a_range->is.range.int_ub ) ;
      a_range = a_range->next ;
    }
    OUT0( ") " ) ;
    OUT3( 
      "$%s $null $%s ; %d\n", pre_name, variable->name, id 
    ) ;
  }

  if( depth && !JD_sync_on_signal_flag ) {
    /* loading signal port with given depth */
    /* this should only happens for signal variable */
    if( range->is.range.bit_index == 0 ) {
      OUT1( "  push  alu  ; signal.depth %d\n", id ) ;
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt  ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    else {
      output_bit_index( range, id ) ;
      PUSH_ALU(id) ;
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt subbit ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    return( BIT_TXT ) ;
  }
  else {
    if( port_type( variable ) ) {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag) {
        PUSH_ALU(id) ;
        output_bit_index( range, id ) ;
        OUT2( 
          "  get_sig_subbit ; var %s[u:l] %d\n", 
          variable->name , id
        ) ;
      }
      return( type_string_of_variable( variable ) ) ;
    }
    else {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag ) {
        output_bit_access( variable, range, id ) ;
        return( BIT_TXT ) ;
      }
      else return( type_string_of_variable( variable ) ) ;
    }
  }
  
}
  
char *output_load_member_array( 
  char *pre_name, named_node *variable, 
  expression_node *range, expression_node *depth, int id 
) {
  char *type ;
  /* note: by the syntax limitation, array index access can't have
     the depth except the sub-bit access
     
     sigset.pt.0[5] -- possible
     
     sigarray[5].0  -- not allowed
     
  */
  if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    PUSH_ALU(id) ;
    (void)eval_expression( range->is.range.ub, id ) ;
    OUT0( "  loadm stridx " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUTMEMBER(pre_name, variable, id) ;
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {
    /* single dimension array */
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.ub, id ) ;
    if( variable->sub_type == JD_array_sub_type ) {
      if( NOT_INT(type) ) TO_INT(id) ;
      OUT0( "  loadm array " ) ;
      DIM_OUT ;
    }
    else {
      if( NOT_INT(type) ) TO_INT(id) ;
      OUT0( "  loadm assoc " ) ;
    }
    if( variable->is_static ) OUT0( "static " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUTMEMBER(pre_name, variable, id) ;
  }
  else if( variable->sub_type == JD_list_sub_type ) {
    /* trial: direct list access */
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    OUT0( "  loadm list " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUTMEMBER(pre_name, variable, id) ;
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    PUSH_ALU(id) ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if( dim ) {
        PUSH_ALU(id) ;
      }
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT(type) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    a_range = variable->info.var.range ;
    OUT1( "  loadm marray %d ( ", a_range->is.range.num_index ) ;
    while( a_range ) {
      OUT1( "%d ", a_range->is.range.int_ub ) ;
      a_range = a_range->next ;
    }
    OUT0( ") " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUTMEMBER(pre_name, variable, id) ;
  }
  if( depth && !JD_sync_on_signal_flag) {
    /* loading signal port with given depth */
    /* this should only happens for signal variable */
    PUSH_ALU(id) ;
    if( range->is.range.bit_index == 0 ) {
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt  ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    else {
      output_bit_index( range, id ) ;
      PUSH_ALU(id) ;
      eval_expression( depth, id ) ;
      OUT3( 
        "  load_sig_pt subbit ; var %s.%d %d\n", 
        variable->name, depth->is.int_value , id
      ) ;
    }
    return( BIT_TXT ) ;
  }
  else {
    if( port_type( variable ) ) {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag ) {
        output_bit_index( range, id ) ;
        OUT2( 
          "  get_sig_subbit ; var %s[u:l] %d\n", 
          variable->name , id
        ) ;
      }
      return( type_string_of_variable( variable ) ) ;
    }
    else {
      if( range->is.range.bit_index && !JD_sync_on_signal_flag ) {
        output_bit_access( variable, range, id ) ;
        return( BIT_TXT ) ;
      }
      else return( type_string_of_variable( variable ) ) ;
    }
  }
}

/* eval var or port */
char *eval_varport( expression_node *exp, int id ) {
  char *ret = NULL ;
  named_node *variable = exp->is.var.name ;
  named_node *parent ;
  char *pre_name ;
  int i ;
  expression_node *range ;
    
  range = exp->is.var.range ;

  if( exp->is.var.name->type == JD_enum_member_name ) {
    /* this is the enum member */
    OUT3( 
      "  load_const (enum) $%s $%s %d ; \n",
      exp->is.var.name->data_type->name,
      exp->is.var.name->name,
      exp->is.var.name->index
    ) ;
    return( ENUM_TXT ) ;
  }
  
  
  if(exp->is.var.pre) {
    if( variable->type == JD_port_name) {
      pre_name =exp->is.var.pre->is.var.name->name ;
    }
    /* the case of this.variable, super.variable */
    else if( exp->is.var.pre->type == JD_this_exp && in_aspect && !in_advice ) {
      parent = exp->is.var.class ;
      pre_name = parent->name ;
      if( range == NULL || 
          range->type == JD_bit_range_exp || 
          range->type == JD_single_bit_range_exp )
      {
        ret = 
          output_load_static_member_single( 
            pre_name, variable, range, exp->is.var.depth, id  
          ) ;
      }
      else {
        ret = 
          output_load_static_member_array( 
            pre_name, variable, exp->is.var.range, exp->is.var.depth, id  
          ) ;
      }
    }
    else if( exp->is.var.pre->type == JD_this_exp ) {
      OUT1( "  load_this  ; %d\n", id ) ;
      parent = exp->is.var.class ;
      pre_name = parent->name ;
    }
    else if( exp->is.var.pre->type == JD_super_exp ) {
      OUT1( "  load_this  ; %d\n", id ) ;
      OUT1( "  get_super 1 ; %d\n", id ) ;
      parent = exp->is.var.class ;
      parent = parent->info.class_name.parent ;
      pre_name = parent->name ;
    }
    else if( exp->is.var.pre->type == JD_aspect_exp ) {
      pre_name = exp->is.var.pre->is.var.name->name ;
      if( range == NULL || 
          range->type == JD_bit_range_exp || 
          range->type == JD_single_bit_range_exp )
      {
        ret = 
          output_load_static_member_single( 
            pre_name, variable, range, exp->is.var.depth, id  
          ) ;
      }
      else {
        ret = 
          output_load_static_member_array( 
            pre_name, variable, exp->is.var.range, exp->is.var.depth, id  
          ) ;
      }
    }
    else {
      (void)eval_varport( exp->is.var.pre, id ) ;
      var_flag = exp->is.var.var_arg ;
      parent = exp->is.var.pre->is.var.name->data_type ;
      if( exp->is.var.member_offset > 0 ) {
        OUT0( "  get_super " ) ;
        if( var_flag ) OUT0( "var " ) ;
        OUT2( "%d ; %d\n", exp->is.var.member_offset, id ) ;
        for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
          parent = parent->info.class_name.parent ;
        }
      }
      pre_name = parent->name ;
    }
    /* get member of the object */
    if( ret == NULL ) {
      if( range == NULL || 
          range->type == JD_bit_range_exp || 
          range->type == JD_single_bit_range_exp )
      {
        ret = 
          output_load_member_single( 
            pre_name, variable, range, exp->is.var.depth, id  
          ) ;
      }
      else {
        ret = 
          output_load_member_array( 
            pre_name, variable, exp->is.var.range, exp->is.var.depth, id  
          ) ;
      }
    }
  }
  else if( variable->is_member ) {
    /* member used within class */
    parent = exp->is.var.class ;
    if( class_initial_code || in_aspect ) {
      pre_name = parent->name ;
      if( range == NULL || 
          range->type == JD_bit_range_exp || 
          range->type == JD_single_bit_range_exp )
      {
        ret = 
          output_load_static_member_single( 
            pre_name, variable, range, exp->is.var.depth, id  
          ) ;
      }
      else {
        ret = 
          output_load_static_member_array( 
            pre_name, variable, exp->is.var.range, exp->is.var.depth, id  
          ) ;
      }
    }
    else {
      OUT0( "  load_this " ) ;
      var_flag = exp->is.var.var_arg ;
      /* if( var_flag ) OUT0( "var " ) ; */
      OUT1( " ; %d\n", id ) ;
      if( exp->is.var.member_offset > 0 ) {
        OUT0( "  get_super " ) ;
        if( var_flag ) OUT0( "var " ) ;
        OUT2( "%d ; %d\n", exp->is.var.member_offset, id ) ;
        for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
          parent = parent->info.class_name.parent ;
        }
      }
      pre_name = parent->name ;
      if( range == NULL || 
          range->type == JD_bit_range_exp || 
          range->type == JD_single_bit_range_exp )
      {
        ret = 
          output_load_member_single( 
            pre_name, variable, range, exp->is.var.depth, id  
          ) ;
      }
      else {
        ret = 
          output_load_member_array( 
            pre_name, variable, exp->is.var.range, exp->is.var.depth, id  
          ) ;
      }
    }
  }
  else {
    /* just a var */
    if( !exp->is.var.member_offset ) var_flag = exp->is.var.var_arg ;
    if( range == NULL || 
        range->type == JD_bit_range_exp || 
        range->type == JD_single_bit_range_exp )
    {
      ret = output_load_single( variable, range, exp->is.var.depth, id  ) ;
    }
    else {
      ret = 
        output_load_array( 
          variable, exp->is.var.range, exp->is.var.depth, id 
        ) ;
    }
  }
  if( exp->is.var.offset > 0 ) {
    var_flag = exp->is.var.var_arg ;
    OUT0( "  get_super " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUT2( "%d ; %d\n", exp->is.var.offset, id ) ;
  }
  if( exp->is.var.offset < 0 ) {
    var_flag = exp->is.var.var_arg ;
    OUT0( "  get_child " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUT2( "%d ; %d\n", (-exp->is.var.offset), id ) ;
  }
  var_flag = 0 ;
  return( ret ) ;
}

char *eval_null( int id ) {
  
  OUT1( "  null  ; ACC = null %d\n", id ) ;
  return( NULL_TXT ) ;
}

char *eval_true( int id ) {
  
  LOAD_ONE(id) ;
  return( INT_TXT ) ;
}

char *eval_false( int id ) {
  
  LOAD_ZERO(id) ;
  return( INT_TXT ) ;
}

char *get_func_type_str( 
  expression_node *func_exp
) 
{
  named_node *func, *data_type ;
  
  func = func_exp->is.var.name ;
  if( func->data_type->sub_type == JD_single_sub_type ) 
  {
    data_type = func->data_type ;
    switch( data_type->type ) {
      case JD_void_type:
        return( VOID_TXT ) ;
        break ;
      case JD_int_type:
      case JD_long_type:
        return( INT_TXT ) ;
        break ;
      case JD_float_type:
      case JD_double_type:
        return( FLOAT_TXT ) ;
        break ;
      case JD_bit_type:
        return( BIT_TXT ) ;
        break ;
      case JD_bool_type:
        return( INT_TXT ) ;
        break ;
      case JD_class_name:
      case JD_sys_class_name:
        return( OBJ_TXT ) ;
        break ;
      case JD_string_type:
        return( STR_TXT ) ;
        break ;
      case JD_signal_type:
        return( SIGNAL_TXT ) ;
        break ;
      case JD_enum_type:
        return( ENUM_TXT ) ;
        break ;
      case JD_same_type:  /* old scheme */
      case JD_class_type:
        return get_func_type_str( func_exp->is.var.pre ) ;
        break ;
      default:
        fprintf( stderr, "Unknown func type for %s detected\n", func->name ) ;
        return( VOID_TXT ) ;
        break ;
    }
  }
  else {
    data_type = func->data_type ;
    switch( data_type->sub_type ) {
      case JD_array_sub_type:
        return( ARRY_TXT ) ;
        break ;
      case JD_multi_array_sub_type:
        return( MARRY_TXT ) ;
        break ;
      case JD_assoc_sub_type:
        return( ASOC_TXT ) ;
        break ;
      case JD_stridx_sub_type:
        return( STIX_TXT ) ;
        break ;
      case JD_list_sub_type:
        return( LIST_TXT ) ;
        break ;
      default:
        fprintf( stderr, "Unknown func type for %s detected\n", func->name ) ;
        return( VOID_TXT ) ;
        break ;
    }
  }
  
}

int eval_default_args( named_node *func, int act_num, int id ) {
  statement_node *farg = func->info.func.args ;
  int i , n ;
  
  for( i = 0 ; (i < act_num) && farg ; i++ ) {
    if( farg->type != JD_formal_arg_statement &&
        farg->type != JD_wildcard_arg_statement &&
        farg->type != JD_one_or_more_arg_statement &&
        farg->type != JD_zero_or_more_arg_statement &&
        farg->type != JD_or_of_args_statement &&
        farg->type != JD_opt_args_statement 
      ) farg = farg->next ;
  }
  n = act_num ;
  while( farg ) {
    switch(farg->type) {
      case JD_vardeclare_statement:
        if( farg->is.variable.declare->var_list->info.var.init ) {
          eval_expression( 
            farg->is.variable.declare->var_list->info.var.init, id
          );
          PUSH_ALU(id) ;
          n++ ;
        }
        break ;
      default:
        return n ;
        break ;  
    }
    farg = farg->next ;
  }
  return n ;
}

void put_pointcut_name( expression_node *exp ) {
  if( exp->type == JD_super_exp ) {
    OUT0( " $super" ) ; 
  }
  else if( exp->type == JD_this_exp ) {
    OUT0( " $this" ) ; 
  }
  else {
    OUT1( " $%s", exp->is.var.name->name ) ; 
  }
  if( exp->is.var.pre ) {
    put_pointcut_name( exp->is.var.pre ) ;
  }
}

void put_advice_pattern( expression_node *exp ) {
  named_node *cls ;
  int i ;
  OUT1( " $%s", exp->is.var.name->name ) ; 
  if( exp->is.var.pre ) {
    cls = exp->is.var.class ;
    if( exp->is.var.member_offset ) {
      for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
        cls = cls->info.class_name.parent ;
      }
    }
    OUT1( "($%s)",cls->name ) ;
    put_pointcut_name( exp->is.var.pre ) ;
  }
  else {
    cls = exp->is.var.class ;
    if( cls ) {
      if( exp->is.var.member_offset ) {
        for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
          cls = cls->info.class_name.parent ;
        }
      }
      OUT1( "($%s)",cls->name ) ;
    }
  }
  OUT1( " $%s", current_func_name ) ;
  if( in_class ) 
    OUT1( " $%s", current_class_name ) ;
  OUT0( " ; \n" ) ;
  /* create instructions to load the instance if this is a class instance
     match. The instrucitons will be removed if the hit is not instance
     class level
  */
  if( exp->is.var.pre ) {
    (void)eval_expression( exp->is.var.pre, 0 ) ;
    if( exp->is.var.member_offset ) {
      OUT1( "  get_super %d ;\n", exp->is.var.member_offset ) ;
    }
  }
  else if( exp->is.var.name->is_member ) {
    OUT0( "  load_this ; \n" ) ;
    if( exp->is.var.member_offset ) {
      OUT1( "  get_super %d ;\n", exp->is.var.member_offset ) ;
    }
  }
  OUT0( "  enter_advice \n" ) ;
}

void put_call_advice( expression_node *exp ) {
  if( !in_aspect ) {
    OUT0( "  call_advice " ) ;
    put_advice_pattern( exp ) ;
  }
}

void put_return_advice( expression_node *exp ) {
  if( !in_aspect ) {
    OUT0( "  return_advice " ) ;
    put_advice_pattern( exp ) ;
  }
}

char *eval_return_exp( expression_node *exp, int id ) {
  OUT1( "  get_return ;  %d\n", id ) ;
  return type_string_of( exp->user_data_type->data_type ) ;
}

char *eval_func_call( expression_node *exp, int id ) {
  expression_node *args ;
  named_node *cls ;
  named_node *func ;
  statement_node *farg ;
  int i ;
  int n = 0 ;
  
  args = exp->is.var.args ;
  func = exp->is.var.name ;
  farg = func->info.func.args ;
  /* check arguments here as we can't do it on PASS_2, arg info may not be
     ready when func is defined after calling
   */
  actual_arg_check(  
    exp->is.var.name, args, exp->is.var.pre, exp->filename, exp->linenum 
  ) ;
  
  while( args ) {
    if( args->type == JD_default_arg ) {
      (void)eval_expression( 
        farg->is.variable.declare->var_list->info.var.init, id
      );
    }
    else (void)eval_expression( args, id ) ;
    PUSH_ALU(id) ;
    n++ ;
    args = args->next ;
    if( farg->type != JD_formal_arg_statement &&
        farg->type != JD_wildcard_arg_statement &&
        farg->type != JD_one_or_more_arg_statement &&
        farg->type != JD_zero_or_more_arg_statement &&
        farg->type != JD_or_of_args_statement &&
        farg->type != JD_opt_args_statement 
      ) farg = farg->next ;
  }
  
  /* output missing argument from default */
  
  n = eval_default_args( func, n, id ) ;
  
  if( exp->is.var.pre ) {
    if( exp->is.var.pre->type == JD_aspect_exp ) {
      OUT4( 
        "  call_a $%s $%s %d  ; %d\n", 
        exp->is.var.pre->is.var.name->name, exp->is.var.name->name, n, id 
      ) ;
    }
    else {
      put_call_advice( exp ) ;
      (void)eval_expression( exp->is.var.pre, id ) ;
      cls = exp->is.var.class ;
      if( exp->is.var.member_offset ) {
        OUT2( "  get_super %d ; %d\n", exp->is.var.member_offset, id ) ;
        for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
          cls = cls->info.class_name.parent ;
        }
      }
      OUT4( 
        "  call_m $%s $%s %d  ; %d\n", 
        cls->name, exp->is.var.name->name, n, id 
      ) ;
      put_return_advice( exp ) ;
    }
    if( n ) OUT2( "  pop %d ; clear args %d\n", n, id ) ;
  } 
  else if( func->is_member ) {
    put_call_advice( exp ) ;
    OUT0( "  load_this ; \n" ) ;
    cls = exp->is.var.name->info.func.class_block->name ;
    if( exp->is.var.member_offset ) {
      OUT1( "  get_super %d ;\n", exp->is.var.member_offset ) ;
      /*
      for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
        cls = cls->info.class_name.parent ;
      }
      */
    }
    OUT4( 
      "  call_m $%s $%s %d  ; %d\n", 
      cls->name, exp->is.var.name->name, n, id 
    ) ;
    put_return_advice( exp ) ;
    if( n ) OUT2( "  pop %d ; clear args %d\n", n, id ) ;
  }
  else {
    put_call_advice( exp ) ;
    OUT3( 
      "  call $%s %d  ; %d\n", 
      exp->is.var.name->name, n, id 
    ) ;
    put_return_advice( exp ) ;
    if( n ) OUT2 ( "  pop %d ; clear args %d\n", n, id ) ;
  }
  return get_func_type_str( exp ) ;
  
}

char *eval_verilog_func_call( expression_node *exp, int id ) {
  expression_node *args ;
  int n = 0 ;
  
  args = exp->is.var.args ;

  actual_arg_check(  
    exp->is.var.name, args, exp->is.var.pre, exp->filename, exp->linenum 
  ) ;
  
  while( args ) {
    (void)eval_expression( args, id ) ;
    PUSH_ALU(id) ;
    n++ ;
    args = args->next ;
  }
  
  OUT3( "  veri_call $%s %d  ; %d\n", exp->is.var.name->name, n, id ) ;
  OUT2 ( "  pop %d ; clear args %d\n", n, id ) ;
  
  return VOID_TXT ;
  
}

char *get_var_type(
  named_node *variable, int flag
) {
  if( flag ) return( type_string_of_variable( variable ) ) ;
  switch ( variable->sub_type ) {
    case JD_single_sub_type: 
      return( type_string_of_variable( variable ) ) ;
      break ;
    case JD_array_sub_type:
      return( ARRY_TXT ) ;
      break ;
    case JD_multi_array_sub_type:
      return( MARRY_TXT ) ;
      break ;
    case JD_assoc_sub_type:
      return( ASOC_TXT ) ;
      break ;
    case JD_stridx_sub_type:
      return( STIX_TXT ) ;
      break ;
    case JD_list_sub_type:
      return( LIST_TXT ) ;
      break ;
  }
  return( NULL ) ;
}

static void out_strength( JD_strength strength ) {
  switch( strength ) {
    case JD_normal: OUT0( " normal " ) ; break ;
    case JD_weak:  OUT0( " weak " ) ; break ;
    case JD_strong:  OUT0( " strong " ) ; break ;
  }
}

/* ACC holds the value to be stored */
void output_store_single( 
  named_node *variable, expression_node *depth, char *ptype, int id, 
  JD_strength strength 
) {
  
  if( variable->type == JD_port_name ) {
    /* depth should be zero, and ignored */
    OUT2( " store_port (%s,%s) ", BIT_TXT, ptype ) ;
    out_strength( strength ) ;
    OUTPORT(variable, id) ;
  }
  else {
    if( depth ) {
      /* storing signal port with given depth */
      /* this should only happens for signal variable */
      if( variable->local_var ) {
        OUT0( " store_sig_l " ) ;
        if( variable->is_static || variable->info.var.is_var ) 
          OUT0( "static " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        out_strength( strength ) ;
        OUTLOCAL(  variable, id ) ;
      }
      else {
        OUT0( " store_sig_g " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        out_strength( strength ) ;
        OUTGLOBAL( variable, id ) ;
      }
    }
    else {
      if( variable->local_var ) {
        OUT0( "  storel " ) ;
        if( variable->is_static || variable->info.var.is_var ) 
          OUT0( "static " ) ;
        OUT2( "(%s,%s) ", get_var_type(variable,0), ptype ) ;
        OUTLOCAL(  variable, id ) ;
      }
      else {
        OUT0( "  storeg " ) ;
        OUT2( "(%s,%s) ", get_var_type(variable,0), ptype ) ;
        OUTGLOBAL( variable, id ) ;
      }
    }
  }
}

/* ACC holds the value to be stored */
void output_store_subbit( 
  named_node *variable, expression_node *depth, char *ptype, int id, 
  JD_strength strength 
) {
  
  if( variable->type == JD_port_name ) {
    /* depth should be zero, and ignored */
    OUT2( " store_port subbit (%s,%s) ", BIT_TXT, ptype  ) ;
    if( variable->i_endian ) 
      OUT2( 
        " i_endian[%d:%d] ", variable->ub, variable->lb
      ) ;
    else if( variable->lb > 0 )
      OUT2( 
        " nz_lsb[%d:%d] ", variable->ub, variable->lb 
      ) ;
    out_strength( strength ) ;
    OUTPORT(variable, id) ;
  }
  else {
    if( depth ) {
      /* storing signal port with given depth = 0 */
      /* this should only happens for signal variable */
      if( variable->local_var ) {
        OUT0( " store_sig_l subbit " ) ;
        if( variable->is_static || variable->info.var.is_var ) 
          OUT0( "static " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        out_strength( strength ) ;
        OUTLOCAL(  variable, id ) ;
      }
      else {
        OUT0( " store_sig_g subbit " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        out_strength( strength ) ;
        OUTGLOBAL( variable, id ) ;
      }
    }
    else {
      if( variable->local_var ) {
        OUT0( "  storel " ) ;
        if( variable->is_static || variable->info.var.is_var ) 
          OUT0( "static " ) ;
        OUT0( "subbit " ) ;
        if( variable->data_type->i_endian ) 
          OUT2( 
            " i_endian[%d:%d] ", 
            variable->data_type->ub, variable->data_type->lb 
          ) ;
        else if( variable->data_type->lb > 0 )
          OUT2( 
            " nz_lsb[%d:%d] ", variable->data_type->ub, variable->data_type->lb
          ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        OUTLOCAL(  variable, id ) ;
      }
      else {
        OUT0( " storeg subbit " ) ;
        if( variable->data_type->i_endian ) 
          OUT2( 
            " i_endian[%d:%d] ", 
            variable->data_type->ub, variable->data_type->lb 
          ) ;
        else if( variable->data_type->lb > 0 )
          OUT2(
            " nz_lsb[%d:%d] ", 
            variable->data_type->ub, variable->data_type->lb 
          ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        OUTGLOBAL( variable, id ) ;
      }
    }
  }
}

void output_bit_index( expression_node *range, int id ) {
  char *type ;
  
  while( range->next ) range = range->next ; /* get the last one */
  if( range->type == JD_bit_range_exp ) {
    /* [ub:lb] */ 
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
    type = eval_expression( range->is.range.lb, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
  }
  else {
    /* one bit access to a bit variable */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
  }
  
}

void out_endian( named_node *variable ) {
  OUT0( " subbit " ) ;
  if( variable->data_type->i_endian ) 
    OUT2( 
      " i_endian[%d:%d] ", variable->data_type->ub, variable->data_type->lb
    ) ;
  else if( variable->data_type->lb > 0 )
    OUT2( 
      " nz_lsb[%d:%d] ", variable->data_type->ub, variable->data_type->lb 
    ) ;
}

/* top of stack holds the value to be stored */
void output_store_array( 
  named_node *variable, expression_node *range, expression_node *depth, 
  char *ptype, int id, JD_strength strength 
) {
  char *type ;
  
  /* note: by the syntax limitation, array index access can't have
     the depth except the sub-bit access
     
     sigset.pt.0[5] -- possible
     
     sigarray[5].0  -- not allowed
     
  */
   
  if( variable->sub_type == JD_single_sub_type ) {
    output_bit_index( range, id ) ; 
    output_store_subbit( variable, depth, ptype, id, strength ) ;
  }
  else if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    (void)eval_expression( range->is.range.ub, id ) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
    } 
    if( variable->local_var ) {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( "  store_sig_l stridx " ) ;
      else
        OUT0( "  storel stridx " ) ;
      if( variable->is_static || variable->info.var.is_var ) 
        OUT0( "static " ) ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( " store_sig_g stridx " ) ;
      else
        OUT0( " storeg stridx " ) ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTGLOBAL(  variable, id ) ;
    }
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {  
    /* single dimension array */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT(type) )
      TO_INT(id) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ; 
    }
    if( variable->local_var ) {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( "  store_sig_l " ) ;
      else
        OUT0( "  storel " ) ;
      OUT1( "%s ", ARRAY_TYPE ) ;
      DIM_OUT ;
      if( variable->is_static || variable->info.var.is_var ) 
        OUT0( "static " ) ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTLOCAL(  variable, id ) ;
    }
    else {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( " store_sig_g " ) ;
      else
        OUT0( " storeg " ) ;
      OUT1( " %s ", ARRAY_TYPE ) ;
      DIM_OUT ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTGLOBAL( variable, id ) ;
    }
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if( dim ) PUSH_ALU(id) ;
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT(type) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ; 
    }
    a_range = variable->info.var.range ;
    if( variable->local_var ) {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( " store_sig_l " ) ;
      else
        OUT0( " storel " ) ;
      OUT1( "marray %d (", a_range->is.range.num_index ) ;
      while( a_range ) {
        OUT1( "%d ", a_range->is.range.int_ub ) ;
        a_range = a_range->next ;
      }
      OUT0( ") " ) ;
      if( variable->is_static || variable->info.var.is_var ) 
        OUT0( "static " ) ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTLOCAL( variable, id ) ;
    }
    else {
      if( depth ) /* for store, depth should be 0 (checked by parser) */
        OUT0( "  store_sig_g " ) ;
      else
        OUT0( "  storeg " ) ;
      OUT1( " marray %d ( ", a_range->is.range.num_index ) ;
      while( a_range ) {
        OUT1( "%d ", a_range->is.range.int_ub ) ;
        a_range = a_range->next ;
      }
      OUT0( ") " ) ;
      if( range->is.range.bit_index ) 
        out_endian( variable ) ;
      OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
      if( depth ) out_strength( strength ) ;
      OUTGLOBAL( variable, id ) ;
    }
  }
}

output_store_static_member_single( 
  char *pre_name, named_node *variable, expression_node *depth, 
  char *ptype, int id, JD_strength strength
) {
  if( depth ) {
    /* loading signal port with given depth */
    /* this should only happens for signal variable */
    OUT0( "  store_sig_static  " ) ;
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    out_strength( strength ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
  else {
    OUT0( "  store_static  " ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,0), ptype ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
}


/* stack top: object, second: data to be stored */
void output_store_member_single( 
  char *pre_name, named_node *variable, expression_node *depth, 
  char *ptype, int id, JD_strength strength
) {

  if( depth ) {
    /* loading signal port with given depth */
    /* this should only happens for signal variable */
    OUT0( "  store_sig_m  " ) ;
    if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    out_strength( strength ) ;
    OUTMEMBER(pre_name,variable,id) ;
  }
  else {
    OUT0( "  storem  " ) ;
    if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,0), ptype ) ;
    OUTMEMBER(pre_name,variable,id) ;
  }

}

/* stack: object, data */
void output_store_member_subbit( 
  char *pre_name, named_node *variable, 
  expression_node *ub, expression_node *lb, 
  expression_node *depth, char *ptype, int id, JD_strength strength 
) {
  char *type ;
  
  type = eval_expression( ub, id ) ;
  if( NOT_INT( type ) ) TO_INT(id) ;
  PUSH_ALU(id) ;
  if( lb != NULL ) { /* [ub:lb] */
    type = eval_expression( lb, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
  }
  /* ACC: lb stack: ub, object, data */
  if( depth ) {
    /* storing signal port with given depth */
    /* this should only happens for signal variable */
    OUT0( " store_sig_m " ) ;
    if( variable->is_static || variable->info.var.is_var ) OUT0( "static " ) ;
    /* as we don't know which port in the signal, we can't spill endian here 
       endian check must be done at runtime 
     */
    OUT0( "subbit " ) ; 
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    out_strength( strength ) ;
    OUTMEMBER(pre_name,variable,id) ;
  }
  else {
    OUT0( " storem " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    /* this must be a bit vector variable, so we can spill endian info. */
    out_endian( variable ) ;
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    OUTMEMBER(pre_name,variable,id) ;
  }
}

/* stack: object, data */
void output_store_static_member_subbit( 
  char *pre_name, named_node *variable, 
  expression_node *ub, expression_node *lb, 
  expression_node *depth, char *ptype, int id, JD_strength strength 
) {
  char *type ;
  
  type = eval_expression( ub, id ) ;
  if( NOT_INT( type ) ) TO_INT(id) ;
  PUSH_ALU(id) ;
  if( lb != NULL ) {
    type = eval_expression( lb, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
  }
  /* ACC:lb stack: ub, object, data */
  if( depth ) {
    /* storing signal port with given depth */
    /* this should only happens for signal variable */
    OUT0( " store_sig_static  subbit " ) ;
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    out_strength( strength ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
  else {
    OUT0( " store_static " ) ;
    out_endian( variable ) ;
    OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
}

void output_store_static_member_array( 
  char *pre_name, named_node *variable, expression_node *range,
  expression_node *depth, char *ptype, int id, JD_strength strength 
) {
  char *type ;
  
  if( variable->sub_type == JD_single_sub_type ) {
    if( range->type == JD_bit_range_exp ) {
      /* [ub:lb] */ 
      output_store_static_member_subbit( 
        pre_name, variable, range->is.range.ub, range->is.range.lb, 
        depth, ptype, id, strength 
      ) ;
    }
    else {  /* [ub] */
      /* one bit access to a bit variable */
      output_store_static_member_subbit( 
        pre_name, variable, range->is.range.ub, NULL, 
        depth, ptype, id, strength 
      ) ;
    }
  }
  else if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    (void)eval_expression( range->is.range.ub, id ) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
    }
    if( depth ) 
      OUT0( "  store_sig_static stridx " ) ;
    else
      OUT0( "  store_static stridx " ) ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
    if( depth ) out_strength( strength ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {
    /* single dimension array */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT(type) )
      TO_INT(id) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
    } 
    if( depth ) 
      OUT0( "  store_sig_static " ) ;
    else
      OUT0( "  store_static " ) ;
    OUT1( " %s ", ARRAY_TYPE ) ;
    DIM_OUT ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
    if( depth ) out_strength( strength ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if( dim ) PUSH_ALU(id) ;
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT(type) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    if( range->is.range.bit_index ) { 
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
    }
    a_range = variable->info.var.range ;
    if( depth ) 
      OUT0( "  store_sig_static " ) ;
    else
      OUT0( "  store_static " ) ;
    OUT1( " marray %d ( ", a_range->is.range.num_index ) ;
    while( a_range ) {
      OUT1( "%d ", a_range->is.range.int_ub ) ;
      a_range = a_range->next ;
    }
    OUT0( ") " ) ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
    if( depth ) out_strength( strength ) ;
    OUT3( " $%s $null $%s ; %d\n", pre_name, variable->name, id ) ;
  }
}
  


/* stack top: object, second: data to be stored */
void output_store_member_array( 
  char *pre_name, named_node *variable, expression_node *range,
  expression_node *depth, char *ptype, int id, JD_strength strength 
) {
  char *type ;
  
  /* note: by the syntax limitation, array index access can't have
     the depth except the sub-bit access
     
     sigset.pt.0[5] -- possible
     
     sigarray[5].0  -- not allowed
     
  */
   
  if( variable->sub_type == JD_single_sub_type ) {
    if( range->type == JD_bit_range_exp ) {
      /* [ub:lb] */ 
      output_store_member_subbit( 
        pre_name, variable, range->is.range.ub, range->is.range.lb, 
        depth, ptype, id, strength 
      ) ;
    }
    else {
      /* one bit access to a bit variable */
      output_store_member_subbit( 
        pre_name, variable, range->is.range.ub, NULL, 
        depth, ptype, id, strength 
      ) ;
    }
  }
  else if( variable->sub_type == JD_stridx_sub_type ) {
    /* string indexed array */
    (void)eval_expression( range->is.range.ub, id ) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ; 
    }
    if( depth ) 
      OUT0( "  store_sig_m " ) ;
    else
      OUT0( "  storem " ) ;
    OUT0( " stridx " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
    if( depth ) out_strength( strength ) ;
    OUTMEMBER( pre_name, variable, id) ;
  }
  else if( variable->sub_type == JD_array_sub_type || 
           variable->sub_type == JD_assoc_sub_type   ) 
  {
    /* single dimension array */
    type = eval_expression( range->is.range.ub, id ) ;
    if( NOT_INT(type) )
      TO_INT(id) ;
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ;
    } 
    if( depth ) 
      OUT0( "  store_sig_m " ) ;
    else
      OUT0( "  storem " ) ;
    OUT1( " %s ", ARRAY_TYPE ) ;
    DIM_OUT ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( "(%s,%s) ", get_var_type(variable,1), ptype ) ;
    if( depth ) out_strength( strength ) ;
    OUTMEMBER( pre_name, variable, id) ;
  }
  else {
    /* multi-dimension array */
    expression_node *a_range = range ;
    int dim ;
    for( dim = 0 ; dim < range->is.range.num_index ; dim++ ) {
      if(dim) PUSH_ALU(id) ;
      type = eval_expression( a_range->is.range.ub, id ) ;
      if( NOT_INT(type) ) TO_INT(id) ;
      a_range = a_range->next ;
    }
    if( range->is.range.bit_index ) {
      PUSH_ALU(id) ;
      output_bit_index( range, id ) ; 
    }
    a_range = variable->info.var.range ;
    if( depth ) 
      OUT0( "  store_sig_m " ) ;
    else
      OUT0( "  storem " ) ;
    OUT1( " marray %d (", a_range->is.range.num_index ) ;
    while( a_range ) {
      OUT1( "%d ", a_range->is.range.int_ub ) ;
      a_range = a_range->next ;
    }
    OUT0( ") " ) ;
    if( variable->is_static ) OUT0( "static " ) ;
    if( range->is.range.bit_index ) 
      out_endian( variable ) ;
    OUT2( 
      "(%s,%s) ", 
      get_var_type(variable,1), ptype
    ) ;
    if( depth ) out_strength( strength ) ;
    OUTMEMBER( pre_name, variable, id) ;
  }
}

/* stack: object, data */
static void output_subbit( 
  expression_node *range, int id
) {
  char *type ;
  expression_node *ub, *lb ;
  if( range->type == JD_bit_range_exp ) {
    /* [ub:lb] => ACC:lb stack: ub */ 
    ub = range->is.range.ub ;
    lb = range->is.range.lb ; 
    type = eval_expression( ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
    type = eval_expression( lb, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
  }
  else {
    /* [ub] =>  ACC:ub stack: ub */ 
    ub = range->is.range.ub ;
    type = eval_expression( ub, id ) ;
    if( NOT_INT( type ) ) TO_INT(id) ;
    PUSH_ALU(id) ;
  }
  
}
  

/* ACC holds the data to be stored */
void store_varport(
  expression_node *exp, char *ptype, int id, JD_strength strength
) {
  named_node *variable = exp->is.var.name ;
  named_node *parent ;
  char *pre_name ;
  int i ;
  
  if(exp->is.var.pre) {
    if( variable->type == JD_port_name) {
      pre_name = exp->is.var.pre->is.var.name->name ;
      if( exp->is.var.range == NULL ) {
        OUT0( "  store_pset_port " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        out_strength( strength ) ;
        OUTPSETPORT(pre_name,variable,id) ;
      }
      else {
        PUSH_ALU(id) ;
        output_subbit( exp->is.var.range, id ) ;
        OUT0( "  store_pset_port subbit " ) ;
        OUT2( "(%s,%s) ", BIT_TXT, ptype ) ;
        if( variable->i_endian ) 
          OUT2( 
            " i_endian[%d:%d] ", 
            variable->ub, variable->lb
          ) ;
        else if( variable->lb > 0 )
          OUT2( 
            " nz_lsb[%d:%d] ", 
            variable->ub, variable->lb 
          ) ;
        out_strength( strength ) ;
        OUTPSETPORT(pre_name,variable,id) ;
      }
    }
    else if( exp->is.var.pre->type == JD_aspect_exp ) {
      /* get member of the object */
      if( exp->is.var.range == NULL ) {
        output_store_static_member_single( 
          exp->is.var.pre->is.var.name->name, variable, 
          exp->is.var.depth, ptype, id, strength 
        ) ;
      }
      else {
        PUSH_ALU(id) ; /* stack: data */
        output_store_static_member_array( 
          exp->is.var.pre->is.var.name->name, variable, exp->is.var.range, 
          exp->is.var.depth, ptype, id, strength  
        ) ;
      }
    }
    else if ( exp->is.var.pre->type == JD_this_exp && in_aspect && !in_advice )
    {
      /* here we are in the aspect-function */
      /* get member of the object */
      if( exp->is.var.range == NULL ) {
        output_store_static_member_single( 
          exp->is.var.pre->user_data_type->name, variable, 
          exp->is.var.depth, ptype, id, strength 
        ) ;
      }
      else {
        PUSH_ALU(id) ; /* stack: data */
        output_store_static_member_array( 
          exp->is.var.pre->user_data_type->name, variable, exp->is.var.range, 
          exp->is.var.depth, ptype, id, strength  
        ) ;
      }
        
    }
    else {
      PUSH_ALU(id) ; /* stack: data */
      /* the case of this.variable, super.variable */
      if( exp->is.var.pre->type == JD_this_exp ) {
        OUT1( "  load_this  ; %d\n", id ) ;
        parent = exp->is.var.class ;
        pre_name = parent->name ;
      }
      else if( exp->is.var.pre->type == JD_super_exp ) {
        OUT1( "  load_this  ; %d\n", id ) ;
        OUT1( "  get_super 1 ; %d\n", id ) ;
        parent = exp->is.var.class ;
        parent = parent->info.class_name.parent ;
        pre_name = parent->name ;
      }
      else {
        (void)eval_varport( exp->is.var.pre, id ) ;
        parent = exp->is.var.pre->is.var.name->data_type ;
        if( exp->is.var.member_offset > 0 ) {
          OUT2( "  get_super %d ; %d\n", exp->is.var.member_offset, id ) ;
          for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
            parent = parent->info.class_name.parent ;
          }
        }
        pre_name = parent->name ;
      }
      /* get member of the object */
      if( exp->is.var.range == NULL ) {
        output_store_member_single( 
          pre_name, variable, exp->is.var.depth, ptype, id, strength 
        ) ;
      }
      else {
        PUSH_ALU(id) ; /* stack: object, data */
        output_store_member_array( 
          pre_name, variable, exp->is.var.range, 
          exp->is.var.depth, ptype, id, strength  
        ) ;
      }
    }
  }
  else if( variable->is_member ) {
    if( class_initial_code || in_aspect ) {
      /* this must be a static access */
      parent = exp->is.var.class ;
      pre_name = parent->name ;
      if( exp->is.var.range == NULL ) {
        output_store_static_member_single( 
          pre_name, variable, exp->is.var.depth, ptype, id, strength
        ) ;
      }
      else {
        PUSH_ALU(id) ;
        output_store_static_member_array( 
          pre_name, variable, exp->is.var.range, 
          exp->is.var.depth, ptype, id, strength  
        ) ;
      }
    }
    else {
      PUSH_ALU(id) ; /* stack: data */
      /* member used within class */
      parent = exp->is.var.class ;
      OUT0( "  load_this\n" ) ;
      if( exp->is.var.member_offset > 0 ) {
        OUT2( "  get_super %d ; %d\n", exp->is.var.member_offset, id ) ;
        for( i = 0 ; i < exp->is.var.member_offset ; i++ ) {
          parent = parent->info.class_name.parent ;
        }
      }
      pre_name = parent->name ;
      /* get member of the object */
      if( exp->is.var.range == NULL ) {
        output_store_member_single( 
          pre_name, variable, exp->is.var.depth, ptype, id, strength 
        ) ;
      }
      else {
        PUSH_ALU(id) ; /* stack: object, data */
        output_store_member_array( 
          pre_name, variable, exp->is.var.range, 
          exp->is.var.depth, ptype, id, strength  
        ) ;
      }
    }
  }
  else {
    /* just a var */
    if( exp->is.var.range == NULL ) {
      output_store_single( /* ACC carries the data to be stored */
        variable, exp->is.var.depth, ptype, id, strength
      ) ;
    }
    else {
      PUSH_ALU(id) ;  /* push the data to be stored */
      output_store_array( 
        variable, exp->is.var.range, exp->is.var.depth, ptype, id, strength 
      ) ;
    }
  }
  
}

char *eval_preincrement( expression_node *exp, int id ) {
  char *ptype ;
  
  ptype = eval_expression( exp->is.expression, id ) ;
  OUT2( "  inc  (%s)  ; ++foo %d\n", ptype, id ) ;
  store_varport( exp->is.expression, ptype, id, JD_normal ) ;
  
  return ptype ;
}

char *eval_predecrement( expression_node *exp, int id ) {
  char *ptype ;
  
  ptype = eval_expression( exp->is.expression, id ) ;
  OUT2( "  dec  (%s)  ; --foo %d\n", ptype, id ) ;
  store_varport( exp->is.expression, ptype, id, JD_normal ) ;
  
  return ptype ;
}


char *eval_postincrement( expression_node *exp, int id ) {
  char *ptype ;
  
  ptype = eval_expression( exp->is.expression, id ) ;
  PUSH_ALU(id) ;
  if( strcmp(ptype,"int") )
    OUT1( "  copy alu 1 ; %d\n", id ) ;
  OUT2( "  inc  (%s)  ; foo++ %d\n", ptype, id ) ;
  store_varport( exp->is.expression, ptype, id, JD_normal ) ;
  POP_ALU(id) ;
  
  return ptype ;
}

char *eval_postdecrement( expression_node *exp, int id ) {
  char *ptype ;
  
  ptype = eval_expression( exp->is.expression, id ) ;
  PUSH_ALU(id) ;
  if( strcmp(ptype,"int") )
    OUT1( "  copy alu 1 ; %d\n", id ) ;
  OUT2( "  dec  (%s)  ; foo-- %d\n", ptype, id ) ;
  store_varport( exp->is.expression, ptype, id, JD_normal ) ;
  POP_ALU(id) ;
  
  return ptype ;
}

char *eval_concatenation( expression_node *exp, int id ) {
  char *ptype, *dtype ;
  expression_node *nxt ;
  ptype = eval_expression( exp->is.expression, id ) ;
  nxt = exp->is.expression->next ;
  while( nxt ) {
    PUSH_ALU(id) ;
    dtype = eval_expression( nxt, id ) ;
    OUT3( "  concat (%s,%s) ; %d\n", ptype, dtype, id ) ;
    if( strcmp( ptype, STR_TXT ) ) {
      /* not string */
      ptype = BIT_TXT ;
    }
    nxt = nxt->next ;
  }  
  return ptype ;
}

char *eval_duplication( expression_node *exp, int id ) {
  char *ptype ;
  (void)eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  ptype = eval_expression( exp->is.binary.right, id ) ;
  OUT2( "  duplicate (%s) ; %d\n", ptype, id ) ;

  if( strcmp( ptype, STR_TXT ) ) {
    /* not string */
    ptype = BIT_TXT ;
  }
  
  return ptype ;
}


char *eval_list( expression_node *exp, int id ) {
  char *ptype ;
  int n = 0 ;
  
  exp = exp->is.expression ;
  while( exp ) {
    ptype = eval_expression( exp, id ) ;
    PUSH_ALU(id) ;
    n++ ;
    exp = exp->next ;
  }  
  OUT3( "  list (%s) %d ; %d\n", ptype, n, id ) ;
  return LIST_TXT ;
}


char *eval_real( expression_node *exp, int id ) {
  OUT3( "  load_const (%s) %s ; %d\n", FLOAT_TXT, exp->string, id ) ;
  return FLOAT_TXT ;
}

char *eval_int( expression_node *exp, int id ) {
  OUT3( "  load_const (%s) %d ; %d\n", INT_TXT, exp->is.int_value, id ) ;
  return INT_TXT ;
}

static unsigned get_bit( unsigned *data, int location ) {
  unsigned bit ;
  bit = ( data[location/32] >> (location % 32) ) & 1 ;
  
  return bit ;
  
}

void output_bit( expression_node *exp ) {
  int i, supress ;
  unsigned bit_a, bit_b ;
  unsigned v ;
  supress = 1 ;
  OUT1( "%d'b", exp->is.bit_const.size ) ;
  for( i = exp->is.bit_const.size-1 ; i >= 0 ; i-- ) {
    bit_a = get_bit( (unsigned *)exp->is.bit_const.a, i ) ;
    bit_b = get_bit( (unsigned *)exp->is.bit_const.b, i ) ;
    v = bit_a | ( bit_b << 1 ) ;
    switch( v ) {
      case 0:
        if( !supress ) OUT0( "0" ) ; 
        break ;
      case 1: OUT0( "1" ) ; supress = 0 ; break ;
      case 2: OUT0( "z" ) ; supress = 0 ; break ;
      case 3: OUT0( "x" ) ; supress = 0 ; break ;
    }
  }
  if( supress ) OUT0( "0" ) ;
}

char *eval_bit( expression_node *exp, int id ) {
  OUT1( "  load_const (%s) ", BIT_TXT ) ;
  output_bit( exp ) ;
  OUT1( " ; %d\n", id ) ;
  return BIT_TXT ;
}

static char *get_bin_type( int ltype, int rtype, int op ) {
  char *ret = NO_TYP ;
  
  switch(op) {
    case MINUS_OP: 
    case PLUS_OP: 
    case TIMES_OP: 
    case DIV_OP: 
    case MOD_OP: 
      if( ltype == FLOAT_TYP || rtype == FLOAT_TYP ) ret = FLOAT_TXT ;
      else if( ltype == BIT_TYP || rtype == BIT_TYP ) ret = BIT_TXT ;
      else if( ltype != INT_TYP || rtype != INT_TYP ) {
        fprintf( stderr, "Unkown type combination for alu ops\n" ) ;
      }
      else ret = INT_TXT ;
      break ;
    case AND_OP: 
    case NAND_OP: 
    case OR_OP: 
    case NOR_OP: 
    case EOR_OP: 
    case NEOR_OP: 
      if( ltype == BIT_TYP || rtype == BIT_TYP ) ret = BIT_TXT ;
      else if( ltype != INT_TYP || rtype != INT_TYP ) {
        fprintf( stderr, "Unkown type combination for alu ops\n" ) ;
      }
      else ret = INT_TXT ;
      break ;
    default:
      fprintf( stderr, "Unkown op type for alu ops\n" ) ;
      break ;
  }
  return ret ;
  
}

char *eval_exp_minus_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  minus (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), MINUS_OP ) ;
  
  return ptype ;
}

char *eval_exp_plus_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  plus (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), PLUS_OP ) ;
  
  return ptype ;
}

char *eval_exp_times_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  times (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), TIMES_OP ) ;
  
  return ptype ;
}

char *eval_exp_div_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  div (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), DIV_OP ) ;
  
  return ptype ;
}

char *eval_exp_mod_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  mod (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), MOD_OP ) ;
  
  return ptype ;
}

char *eval_exp_and_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  and (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), AND_OP ) ;
  
  return ptype ;
}

char *eval_exp_or_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  or (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), OR_OP ) ;
  
  return ptype ;
}

char *eval_exp_eor_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  eor (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), EOR_OP ) ;
  
  return ptype ;
}

char *eval_exp_nand_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  nand (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), NAND_OP ) ;
  
  return ptype ;
}

char *eval_exp_nor_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  nor (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), NOR_OP ) ;
  
  return ptype ;
}

char *eval_exp_neor_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  neor (%s,%s) ; %d\n", ltype, rtype, id ) ;

  ptype = get_bin_type( get_type(ltype), get_type(rtype), NEOR_OP ) ;
  
  return ptype ;
}

char *eval_exp_land_exp( expression_node *exp, int id ) {
  int label0, label1 ;
  char *ptype ;
  
  label0 = label_num++ ;
  label1 = label_num++ ;
  ptype = eval_expression( exp->is.binary.left, id ) ;
  OUT3( "  jz (%s) l%d  ; && %d\n", ptype, label0, id ) ;
  ptype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  jz (%s) l%d  ; && %d\n", ptype, label0, id ) ;
  LOAD_ONE(id) ;
  OUT2( "  jmp l%d  ; && %d\n", label1, id ) ;
  OUT_LABEL(label0,id) ;
  LOAD_ZERO(id) ;
  OUT_LABEL(label1,id) ;
  
  ptype = INT_TXT ;
  
  return ptype ;
}

char *eval_exp_lor_exp( expression_node *exp, int id ) {
  int label0, label1, label2 ;
  char *ptype ;
  
  label0 = label_num++ ;
  label1 = label_num++ ;
  label2 = label_num++ ;
  ptype = eval_expression( exp->is.binary.left, id ) ;
  OUT3( "  jnz (%s) l%d  ; || %d\n", ptype, label0, id ) ;
  ptype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  jz (%s) l%d  ; || %d\n", ptype, label1, id ) ;
  OUT_LABEL(label0,id) ;
  LOAD_ONE(id) ;
  OUT2( "  jmp l%d  ; || %d\n", label2, id ) ;
  OUT_LABEL(label1,id) ;
  LOAD_ZERO(id) ;
  OUT_LABEL(label2,id) ;
  
  ptype = INT_TXT ;
  
  return ptype ;
}


char *eval_exp_rshift_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  if( NOT_INT( rtype ) ) TO_INT(id) ;
  OUT2( "  rshift (%s) ; %d\n", ltype, id ) ;
  
  return ltype ;
}

char *eval_exp_urshift_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  if( NOT_INT( rtype ) ) TO_INT(id) ;
  OUT2( "  urshift (%s) ; %d\n", ltype, id ) ;
  
  return ltype ;
}

char *eval_exp_lshift_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  if( NOT_INT( rtype ) ) TO_INT(id) ;
  OUT2( "  lshift (%s) ; %d\n", ltype, id ) ;
  
  return ltype ;
}


char *eval_exp_lt_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  lt (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_gt_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  gt (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_eqeq_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  eqeq (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_le_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  le (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_ge_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  ge (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_ne_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  ne (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_eqeqeq_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  eqeqeq (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_neqeq_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  neqeq (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_eqQeq_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  eqQeq (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_eqeqQ_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  eqeqQ (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_exp_Qeqeq_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  Qeqeq (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}


char *eval_exp_Qne_exp( expression_node *exp, int id ) {
  char *ltype, *rtype ;
  
  ltype = eval_expression( exp->is.binary.left, id ) ;
  PUSH_ALU(id) ;
  rtype = eval_expression( exp->is.binary.right, id ) ;
  OUT3( "  Qne (%s,%s) ; %d\n", ltype, rtype, id ) ;
  
  return INT_TXT ;
}

char *eval_u_minus_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_minus (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}

char *eval_u_tilde_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_tilde (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}

char *eval_u_not_exp( expression_node *exp, int id ) {
  char *ptype ;
  
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_not (%s) ; %d\n", ptype, id ) ;
  
  ptype = INT_TXT ;
  
  return ptype ;
}
char *eval_u_and_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_and (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}
char *eval_u_or_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_or (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}
char *eval_u_eor_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_eor (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}
char *eval_u_nand_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_nand (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}

char *eval_u_nor_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_nor (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}
char *eval_u_neor_exp( expression_node *exp, int id ) {
  char *ptype ;
  ptype = eval_expression( exp->is.unary.this, id ) ;
  OUT2( "  u_neor (%s) ; %d\n", ptype, id ) ;
  
  return ptype ;
}


char *eval_eqCeq_exp( expression_node *exp, int id ) {
  char *type0, *type1, *type2 ;
  
  type0 = eval_expression( exp->is.trinary.first, id ) ;
  PUSH_ALU(id) ;
  type1 = eval_expression( exp->is.trinary.second, id ) ;
  PUSH_ALU(id) ;
  type2 = eval_expression( exp->is.trinary.third, id ) ;
  OUT4( "  eqCeq (%s,%s,%s) ; %d\n", type0, type1, type2, id ) ;
  
  return INT_TXT ;
}

char *eval_conditional_exp( expression_node *exp, int id ) {
  char *type0, *type1 ;
  int label0, label1 ;
  
  label0 = label_num++ ;
  label1 = label_num++ ;
  
  type0 = eval_expression( exp->is.trinary.first, id ) ;
  OUT3( "  jz (%s) l%d  ; && %d\n", type0, label0, id ) ;
  type1 = eval_expression( exp->is.trinary.second, id ) ;
  OUT2( "  jmp l%d  ; && %d\n", label1, id ) ;
  OUT_LABEL(label0,id) ;
  (void)eval_expression( exp->is.trinary.third, id ) ;
  OUT_LABEL(label1,id) ;
  
  return type1 ;
}

char *eval_super_exp( int id ) {
  OUT0( "  load_super " );
  if( var_flag ) OUT0( "var " ) ;
  OUT1( "  ; %d\n", id ) ;
  
  return OBJ_TXT ;
}

char *eval_this_exp( expression_node *exp, int id ) {
  OUT0( "  load_this " ) ;
  /* if( var_flag ) OUT0( "var " ) ; */
  OUT1( " ; %d\n", id ) ;

  if( exp->is.var.offset > 0 ) {
    var_flag = exp->is.var.var_arg ;
    OUT0( "  get_super " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUT2( "%d ; %d\n", exp->is.var.offset, id ) ;
  }
  if( exp->is.var.offset < 0 ) {
    var_flag = exp->is.var.var_arg ;
    OUT0( "  get_child " ) ;
    if( var_flag ) OUT0( "var " ) ;
    OUT2( "%d ; %d\n", (-exp->is.var.offset), id ) ;
  }
  
  return OBJ_TXT ;
}

char *eval_new_expression( 
  expression_node *exp, expression_node *obj, int id 
) {
  named_node *mfunc, *obj_type ;
  int n = 0 ;
  int tmp ;
  expression_node *args ;
  int label0, label1 ;
  
  if( exp->data_type == JD_exp_new_type ) {
    obj_type = obj->is.var.name->data_type ;
    /* calling obj.new */
    mfunc = findmember_in_class( obj_type, "new", &tmp ) ;
    if( mfunc ) {
      /* stack: args, ACC <- object */
      args = exp->is.new.args ;
      while( args ) {
        (void)eval_expression( args, id ) ;
        PUSH_ALU(id) ;
        n++ ;
        args = args->next ;
      }
      /* output missing argument from default */
      n = eval_default_args( mfunc, n, id ) ;
      
      OUT2( "  new_obj $%s ; new  %d\n", obj_type->name, id ) ;
      OUT4( 
        "  call_m $%s $%s %d  ; %d\n", 
        obj_type->name, "new", n, id 
      ) ;
      if(n) OUT2 ( "  pop %d ; clear args %d\n", n, id ) ;
    }
    else {
      /* ACC <- object */
      OUT2( "  new_obj $%s ; new  %d\n", obj_type->name, id ) ;
    }
    if( exp->is.new.obj ) {
      /* new <object> ; duplicate <object */
      PUSH_ALU(id) ;
      (void) eval_expression( exp->is.new.obj, id ) ;
      OUT1( "  copy_obj  ; new <obj> stack[sp-1],ACC) %d\n", id ) ;
    }
  }
  else {
    /* exp->data_type == JD_exp_new_block_type */
    /* this is storing block to thread */
    label0 = label_num++ ;
    label1 = label_num++ ;
    OUT2( "  jmp l%d ; new <block> %d\n", label1, id ) ;
    OUT_LABEL(label0,id) ;
    output_statement( 
      exp->is.new.obj->is.new_bl.ock, block_level->this_level 
    ) ;
    OUT1( "  exit  ; new <block> %d \n", id ) ;
    OUT_LABEL(label1,id) ;
    OUT2( "  load_thread l%d ; new <block> %d\n", label0, id ) ;
  }
  return OBJ_TXT ;
}



char *eval_assign_exp( expression_node *exp, int id ) {
  char *ptype, *ltype, *rtype ;
  
  if( exp->is.assign.ops != JD_assign_EQ && 
      exp->is.assign.ops != JD_assign_NB_EQ    ) {
    ltype = eval_expression( exp->is.assign.lvalue, id ) ;
    PUSH_ALU(id) ;
  }
  
  if( exp->is.assign.expression->type == JD_new_exp ) {
    rtype =
      eval_new_expression( 
        exp->is.assign.expression, exp->is.assign.lvalue, id 
      ) ;
  }
  else rtype = eval_expression( exp->is.assign.expression, id ) ;
  
  switch( exp->is.assign.ops ) {
    case JD_assign_Pls_EQ:
      OUT3( "  plus (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), PLUS_OP ) ;
      break ;
    case JD_assign_Minus_EQ:
      OUT3( "  minus (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), MINUS_OP ) ;
      break ;
    case JD_assign_Time_EQ:
      OUT3( "  times (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), TIMES_OP ) ;
      break ;
    case JD_assign_Div_EQ:
      OUT3( "  div (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), DIV_OP ) ;
      break ;
    case JD_assign_Mod_EQ:
      OUT3( "  mod (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), MOD_OP ) ;
      break ;
    case JD_assign_Rsft_EQ:
      if( NOT_INT( rtype ) ) TO_INT(id) ;
      OUT2( "  rshift (%s) ; %d\n", ltype, id ) ;
      ptype = ltype ;
      break ;
    case JD_assign_Ursft_EQ:
      if( NOT_INT( rtype ) ) TO_INT(id) ;
      OUT2( "  urshift (%s) ; %d\n", ltype, id ) ;
      ptype = ltype ;
      break ;
    case JD_assign_Lsft_EQ:
      if( NOT_INT( rtype ) ) TO_INT(id) ;
      OUT2( "  lshift (%s) ; %d\n", ltype, id ) ;
      ptype = ltype ;
      break ;
    case JD_assign_And_EQ:
      OUT3( "  and (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), AND_OP ) ;
      break ;
    case JD_assign_Or_EQ:
      OUT3( "  or (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), OR_OP ) ;
      break ;
    case JD_assign_Eor_EQ:
      OUT3( "  eor (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), EOR_OP ) ;
      break ;
    case JD_assign_Nand_EQ:
      OUT3( "  nand (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), NAND_OP ) ;
      break ;
    case JD_assign_Nor_EQ:
      OUT3( "  nor (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), NOR_OP ) ;
      break ;
    case JD_assign_Neor_EQ:
      OUT3( "  neor (%s,%s) ; %d\n", ltype, rtype, id ) ;
      ptype = get_bin_type( get_type(ltype), get_type(rtype), NEOR_OP ) ;
      break ;
    case JD_assign_EQ: 
      ptype = rtype ;
      break ;
    case JD_assign_NB_EQ:
      ptype = rtype ;
      break ;
    default:
      fprintf( stderr, "Compiler Internal error, unknown assign type\n" ) ;
      exit(1) ;
      break ;
  }

  store_varport( exp->is.assign.lvalue, ptype, id, exp->is.assign.strength ) ;
  
  return WHAT_TXT ;
  
}

char *eval_assign_list_exp( expression_node *exp, int id ) {
  char *rtype ;
  expression_node *e ;
  int i, j, n ;
  rtype = eval_expression( exp->is.assign.expression, id ) ;
  PUSH_ALU(id) ;
  
  n = 0 ;
  e = exp->is.assign.lvalue->is.expression ;
  while(e) {
    n++ ;
    e = e->next ;
  }
  
  for( i = 0 ; i < n ; i++ ) {
    /* need to store from tail to adjust data */
    e = exp->is.assign.lvalue->is.expression ;
    for( j = n-1-i ; j > 0 ; j-- ) e = e->next ;
    store_varport( e, rtype, id, exp->is.assign.strength ) ;
    if( i < n-1 ) {
      OUT1( "  store_adjust (%s) ; list assign adjust \n", rtype ) ;
    }
  }
  POP_ALU(id) ;
  return WHAT_TXT ;
  
}

void eval_clock_dly_exp( expression_node *texp, int id ) {
  expression_node *clk ;
  expression_node *exp = NULL ;

  clk = texp->is.time.clock ;
  if( clk ) exp = clk->is.smt_prefix.clock ;
  if( exp ) {
    if( exp->is.var.name->type == JD_port_name ) {
      if( exp->is.var.pre ) {
        OUT0( "  sync pset_port " ) ;
        output_edge( clk->is.smt_prefix.edge ) ;
        OUT3( 
          " $%s $%s ; @n (edge clock) statement %d\n",
          exp->is.var.pre->is.var.name->name, exp->is.var.name->name, id
        ) ;
      }
      else {
        OUT0( "  sync port " ) ;
        output_edge( clk->is.smt_prefix.edge ) ;
        OUT2( 
          " $%s ; @n (edge clock) statement %d\n",
          exp->is.var.name->name, id
        ) ;
      }
    }
    else {
      (void)eval_expression( exp, id ) ;
      OUT0( "  sync signal " ) ;
      output_edge( clk->is.smt_prefix.edge ) ;
        OUTMEMBER( 
          exp->is.var.pre->is.var.name, 
          exp->is.var.name, id
        ) ;
      OUT1( " ; @ exp %d\n", id ) ;
    }
  }
  else {
    exp = find_clock_on_expression_list( texp->is.time.exp_list ) ;
    if( exp ) {
      if( clk )
        output_sync_on_signal( exp, clk->is.smt_prefix.edge, id ) ;
      else
        output_sync_on_signal( exp, JD_posedge, id ) ;
    }
    else {
      /* using 'CLOCK' as the clock */
      OUT0( "  sync port " ) ;
      if( clk )
        output_edge( clk->is.smt_prefix.edge ) ;
      else
        output_edge( JD_posedge ) ;
      OUT2( 
        " $%s ; @n statement %d\n",
        "CLOCK", id
      ) ;
    }
  }
}

char *eval_timed_till_exp( expression_node *texp, int id ) {
  expression_node *exp ;
  char *ptype ;
  int label0, label1, label2, label3 ;
  
  /* construct evaluation loop */
  label0 = label_num++ ;  /* loop label */
  label1 = label_num++ ;  /* check end label */
  label2 = label_num++ ;  /* true label */
  label3 = label_num++ ;  /* exit label */

  ptype = eval_expression( texp->is.time.delay, id ) ;
  if( NOT_INT(ptype) ) TO_INT(id) ;
  
  eval_clock_dly_exp( texp, id ) ; 
  
  /* push window value to stack */
  if( texp->is.time.window) {
    ptype = eval_expression( texp->is.time.window, id ) ;
    if( NOT_INT(ptype) ) TO_INT(id) ;
  }
  else {
    LOAD_ZERO(id) ;
  }
  PUSH_ALU(id) ;  /* window value to the top of stack */
  
  OUT_LABEL(label0, id) ;
  output_expression_debug_info(id) ;
   exp = texp->is.time.exp_list ;
   while( exp ) {
     ptype = eval_expression( exp, id ) ;
     OUT3( "  jz (%s) l%d  ; @a,b( exp ) %d\n", ptype, label1, id ) ;
     exp = exp->next ;
   }
   /* eval become true */
   OUT2( "  jmp  l%d ; @a,b( exp ) %d\n", label2, id ) ;
  OUT_LABEL(label1, id) ;
   /* false, end check */
   POP_ALU(id) ;
   OUT3( "  jz (%s) l%d  ; @a,b( exp ) %d\n", ptype, label3, id ) ;
   OUT2( "  dec  (%s)  ; window-- %d\n", INT_TXT, id ) ;
   PUSH_ALU(id) ;
   LOAD_ONE(id) ;
   eval_clock_dly_exp( texp, id ) ; 
   OUT2( "  jmp  l%d ; @a,b( exp ) %d\n", label0, id ) ;
  OUT_LABEL(label2, id) ;
   POP(1,id) ;  /* remove window value from the stack */
   LOAD_ONE(id) ;
  OUT_LABEL(label3, id) ;
    
  
  return INT_TXT ;
}

char *eval_timed_while_exp( expression_node *texp, int id ) {
  expression_node *exp ;
  char *ptype ;
  int label0, label1, label2, label3 ;

  /* construct evaluation loop */
  label0 = label_num++ ;  /* loop label */
  label1 = label_num++ ;  /* false label */
  label2 = label_num++ ;  /* true label */
  label3 = label_num++ ;  /* exit label */
  
  ptype = eval_expression( texp->is.time.delay, id ) ;
  if( NOT_INT(ptype) ) TO_INT(id) ;
  
  eval_clock_dly_exp( texp, id ) ; 
  
  /* push window value to stack */
  if( texp->is.time.window) {
    ptype = eval_expression( texp->is.time.window, id ) ;
    if( NOT_INT(ptype) ) TO_INT(id) ;
  }
  else {
    LOAD_ONE(id) ;
  }
  PUSH_ALU(id) ;  /* window value to the top of stack */
  
  
  OUT_LABEL(label0, id) ;
  output_expression_debug_info(id) ;
   exp = texp->is.time.exp_list ;
   while( exp ) {
     ptype = eval_expression( exp, id ) ;
     OUT3( "  jz (%s) l%d  ; @a,b( exp ) %d\n", ptype, label1, id ) ;
     exp = exp->next ;
   }
   /* check the window */
   POP_ALU(id) ;
   OUT3( "  jz (%s) l%d  ; @a,b( exp ) %d\n", ptype, label2, id ) ;
   OUT2( "  dec  (%s)  ; --window %d\n", INT_TXT, id ) ;
   PUSH_ALU(id) ;
   LOAD_ONE(id) ;
   eval_clock_dly_exp( texp, id ) ; 
   OUT2( "  jmp  l%d ; @a,b( exp ) %d\n", label0, id ) ;
  OUT_LABEL(label1, id) ;
   POP(1,id) ;
   LOAD_ZERO(id) ;        /* false */
   OUT2( "  jmp  l%d ; @a,b( exp ) %d\n", label3, id ) ;
  OUT_LABEL(label2, id) ;
   LOAD_ONE(id) ;
  OUT_LABEL(label3, id) ;
    
  
  return INT_TXT ;
}

char *eval_parallel_or_exp( expression_node *texp, int id ) {
  int label  ;
  int i_label ;
  int c_num = 0 ;
  char *ptype ;
  expression_node *child ;
  
  i_label = label_num++ ;  /* exit label */
  LOAD_ZERO(id) ;
  OUT1( "  fork  ; p_or(..) %0d \n", id ) ;
  child = texp->is.time.exp_list ;
  while( child ) {
    label = label_num++ ;  /* exit label */
    OUT3(
      "  exec f%0d  ; fork %0d child(%0d) \n", label, id, c_num++ 
    ) ;
    child = child->next ;
  }
  OUT1( "  join  ; p_or %0d \n", id ) ;
  OUT2( "  jmp l%d  ; fork statement %d\n", i_label, id ) ;

  /* spit out,child code */
  label = i_label + 1 ;
  child = texp->is.time.exp_list ;
  c_num=0 ;
  while( child ) {
    OUT2( "  ; p_or condition (%d) %d \n", c_num, id ) ;
    OUT1( "f%d:\n", label ) ;
    label++ ;
    output_expression_debug_info(id) ;
    ptype = eval_expression( child, id ) ;
    if( NOT_INT(ptype) ) TO_INT(id) ;
    OUT2( "  exit p_or ; %0d child(%0d) \n", id, c_num++ ) ;
    child = child->next ;
  }  
  OUT1( "  ; end of p_or %d\n", id ) ;
  OUT_LABEL(i_label, id) ;
  return INT_TXT ;
}

char *eval_parallel_and_exp( expression_node *texp, int id ) {
  int label  ;
  int i_label ;
  int c_num = 0 ;
  char *ptype ;
  expression_node *child ;
  
  i_label = label_num++ ;  /* exit label */
  LOAD_ONE(id) ;
  OUT1( "  fork  ; p_and(..) %0d \n", id ) ;
  child = texp->is.time.exp_list ;
  while( child ) {
    label = label_num++ ;  /* exit label */
    OUT3(
      "  exec f%0d  ; fork %0d child(%0d) \n", label, id, c_num++ 
    ) ;
    child = child->next ;
  }
  OUT1( "  join  ; p_and %0d \n", id ) ;
  OUT2( "  jmp l%d  ; fork statement %d\n", i_label, id ) ;

  /* spit out,child code */
  label = i_label + 1 ;
  child = texp->is.time.exp_list ;
  c_num=0 ;
  while( child ) {
    OUT2( "  ; p_and condition (%d) %d \n", c_num, id ) ;
    OUT1( "f%d:\n", label ) ;
    label++ ;
    output_expression_debug_info(id) ;
    ptype = eval_expression( child, id ) ;
    if( NOT_INT(ptype) ) TO_INT(id) ;
    OUT2( "  exit p_and ; %0d child(%0d) \n", id, c_num++ ) ;
    child = child->next ;
  }  
  OUT1( "  ; end of p_and %d\n", id ) ;
  OUT_LABEL(i_label, id) ;
  
  return INT_TXT ;
}

/**********************************
 *  RND / CYC expression
 **********************************/
 
char *eval_rnd_exp( expression_node *exp, int id ) {
  expression_node *cnd ;
  int n ;
  n = 0 ;
  cnd = exp->is.rnd.cond ;
  while(cnd) {
    switch(cnd->type) {
      case JD_rand_cond_lt:
        OUT0( "  load_const (int) 1 ;\n" ) ;
        break ;
      case JD_rand_cond_gt:
        OUT0( "  load_const (int) 2 ;\n" ) ;
        break ;
      case JD_rand_cond_le:
        OUT0( "  load_const (int) 3 ;\n" ) ;
        break ;
      case JD_rand_cond_ge:
        OUT0( "  load_const (int) 4 ;\n" ) ;
        break ;
      case JD_rand_cond_ne:
        OUT0( "  load_const (int) 5 ;\n" ) ;
        break ;
      case JD_rand_cond_Qeq:
        OUT0( "  load_const (int) 6 ;\n" ) ;
        break ;
      case JD_rand_cond_Qne:
        OUT0( "  load_const (int) 7 ;\n" ) ;
        break ;
    }
    PUSH_ALU(id) ;  /* window value to the top of stack */
    eval_expression( cnd->is.rnd_cond.exp, id ) ;
    PUSH_ALU(id) ;  /* window value to the top of stack */
    n++ ;
    cnd = cnd->is.rnd_cond.nxt ;
  }
  OUT1( "  loadg $RND_%d ; \n", exp->is.rnd.index ) ;
  if( exp->type == JD_rnd_exp ) {
    OUT1( "  call_m $sys_RND $gen_random %d ; \n", n * 2 ) ;
  }
  else {
    OUT1( "  call_m $sys_CYC $gen_random %d ; \n", n * 2 ) ;
  }
  if( n ) OUT2( "  pop %d ; clear args %d\n", n * 2, id ) ;
  if( exp->is.rnd.size && (exp->is.rnd.size > exp->is.rnd.width) ) 
    return( BIT_TXT ) ;
  else if( exp->is.rnd.dst_width &&
           (exp->is.rnd.dst_width > exp->is.rnd.width) ) 
    return( BIT_TXT ) ;
  else if( exp->is.rnd.dst_width &&
           exp->is.rnd.dst_width == -1 &&
           exp->is.rnd.width == 0 ) 
    return( INT_TXT ) ;
  else 
    return( BIT_TXT ) ;
}

char *eval_expression( expression_node *exp, int id ) {
  char *ret ;
  
  if( !exp ) return WHAT_TXT ;
  
  switch ( exp->type ) {
    case JD_string_exp:
      ret = eval_string( exp, id ) ;
      break ;
    case JD_varport_exp:
      ret = eval_varport( exp, id ) ;
      break ;
    case JD_null_exp:
      ret = eval_null( id ) ;
      break ;
    case JD_true_exp:
      ret = eval_true( id ) ;
      break ;
    case JD_false_exp:
      ret = eval_false( id ) ;
      break ;
    case JD_function_call_exp:
      ret = eval_func_call( exp, id ) ;
      break ;
    case JD_verilog_task_call_exp:
      ret = eval_verilog_func_call( exp, id ) ;
      break ;
    case JD_preincrement_exp:
      ret = eval_preincrement( exp, id ) ;
      break ;
    case JD_predecrement_exp:
      ret = eval_predecrement( exp, id ) ;
      break ;
    case JD_postincrement_exp:
      ret = eval_postincrement( exp, id ) ;
      break ;
    case JD_postdecrement_exp:
      ret = eval_postdecrement( exp, id ) ;
      break ;
    case JD_concatenation_exp:
      ret = eval_concatenation( exp, id ) ;
      break ;
    case JD_duplication_exp:
      ret = eval_duplication( exp, id ) ;
      break ;
    case JD_list_exp:
      ret = eval_list( exp, id ) ;
      break ;
    case JD_real_num:
      ret = eval_real( exp, id ) ;
      break ;
    case JD_int_num:
      ret = eval_int( exp, id ) ;
      break ;
    case JD_bit_num:
      ret = eval_bit( exp, id ) ;
      break ;
    case JD_exp_minus_exp:     /* expression '-' expression   */
      ret = eval_exp_minus_exp( exp, id ) ;
      break ;
    case JD_exp_plus_exp:      /* expression '+' expression   */
      ret = eval_exp_plus_exp( exp, id ) ;
      break ;
    case JD_exp_times_exp:     /* expression '*' expression   */
      ret = eval_exp_times_exp( exp, id ) ;
      break ;
    case JD_exp_div_exp:       /* expression '/' expression   */
      ret = eval_exp_div_exp( exp, id ) ;
      break ;
    case JD_exp_mod_exp:       /* expression '%' expression   */
      ret = eval_exp_mod_exp( exp, id ) ;
      break ;
    case JD_exp_and_exp:     /* expression '&' expression   */
      ret = eval_exp_and_exp( exp, id ) ;
      break ;
    case JD_exp_or_exp:      /* expression '|' expression   */
      ret = eval_exp_or_exp( exp, id ) ;
      break ;
    case JD_exp_eor_exp:     /* expression '^' expression   */
      ret = eval_exp_eor_exp( exp, id ) ;
      break ;
    case JD_exp_nand_exp:      /* expression '&~' expression  */
                               /* expression '~&' expression  */
      ret = eval_exp_nand_exp( exp, id ) ;
      break ;
    case JD_exp_nor_exp:     /* expression '|~' expression  */
                             /* expression '~|' expression  */
      ret = eval_exp_nor_exp( exp, id ) ;
      break ;
    case JD_exp_neor_exp:      /* expression '^~' expression  */
                               /* expression '~^' expression  */
      ret = eval_exp_neor_exp( exp, id ) ;
      break ;
    case JD_exp_land_exp:      /* expression '&&' expression  */
      ret = eval_exp_land_exp( exp, id ) ;
      break ;
    case JD_exp_lor_exp:     /* expression '||' expression  */
      ret = eval_exp_lor_exp( exp, id ) ;
      break ;
    case JD_exp_rshift_exp:    /* expression '>>' expression  */
      ret = eval_exp_rshift_exp( exp, id ) ;
      break ;
    case JD_exp_urshift_exp:   /* expression '>>>' expression */
      ret = eval_exp_urshift_exp( exp, id ) ;
      break ;
    case JD_exp_lshift_exp:    /* expression '<<' expression  */
      ret = eval_exp_lshift_exp( exp, id ) ;
      break ;
    case JD_exp_lt_exp: /* expression '<' expression   */
      ret = eval_exp_lt_exp( exp, id ) ;
      break ;
    case JD_exp_gt_exp: /* expression '>' expression   */
      ret = eval_exp_gt_exp( exp, id ) ;
      break ;
    case JD_exp_eqeq_exp:      /* expression '==' expression  */
      ret = eval_exp_eqeq_exp( exp, id ) ;
      break ;
    case JD_exp_le_exp: /* expression '<=' expression  */
      ret = eval_exp_le_exp( exp, id ) ;
      break ;
    case JD_exp_ge_exp: /* expression '>=' expression  */
      ret = eval_exp_ge_exp( exp, id ) ;
      break ;
    case JD_exp_ne_exp: /* expression '!=' expression  */
      ret = eval_exp_ne_exp( exp, id ) ;
      break ;
    case JD_exp_eqeqeq_exp:    /* expression '===' expression */
      ret = eval_exp_eqeqeq_exp( exp, id ) ;
      break ;
    case JD_exp_neqeq_exp:     /* pression '!==' expression   */
      ret = eval_exp_neqeq_exp( exp, id ) ;
      break ;
    case JD_exp_eqQeq_exp:     /* expression '=?=' expression */
      ret = eval_exp_eqQeq_exp( exp, id ) ;
      break ;
    case JD_exp_eqeqQ_exp:     /* expression '==?' expression */
      ret = eval_exp_eqeqQ_exp( exp, id ) ;
      break ;
    case JD_exp_Qeqeq_exp:     /* expression '?==' expression */
      ret = eval_exp_Qeqeq_exp( exp, id ) ;
      break ;
    case JD_exp_Qne_exp:     /* expression '!?=' expression */
      ret = eval_exp_Qne_exp( exp, id ) ;
      break ;
    case JD_u_minus_exp:     /* '-' expression              */
      ret = eval_u_minus_exp( exp, id ) ;
      break ;
    case JD_u_tilde_exp:     /* '~' expression              */
      ret = eval_u_tilde_exp( exp, id ) ;
      break ;
    case JD_u_not_exp:  /* '!' expression              */
      ret = eval_u_not_exp( exp, id ) ;
      break ;
    case JD_u_and_exp:  /* '&' expression              */
      ret = eval_u_and_exp( exp, id ) ;
      break ;
    case JD_u_or_exp:   /* '|' expression              */
      ret = eval_u_or_exp( exp, id ) ;
      break ;
    case JD_u_eor_exp:  /* '^' expression              */
      ret = eval_u_eor_exp( exp, id ) ;
      break ;
    case JD_u_nand_exp: /* '~&' expression             */
                        /* '&~' expression             */
      ret = eval_u_nand_exp( exp, id ) ;
      break ;
    case JD_u_nor_exp:  /* '~|' expression             */
                        /* '|~' expression             */
      ret = eval_u_nor_exp( exp, id ) ;
      break ;
    case JD_u_neor_exp: /* '^~' expression             */
                        /* '~^' expression             */
      ret = eval_u_neor_exp( exp, id ) ;
      break ;
    case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
      ret = eval_eqCeq_exp( exp, id ) ;
      break ;
    case JD_conditional_exp:   /*  '?' exp ':' exp            */
      ret = eval_conditional_exp( exp, id ) ;
      break ;
      
    case JD_super_exp:
      ret = eval_super_exp( id ) ;
      break ;
    case JD_this_exp:
      ret = eval_this_exp( exp, id ) ;
      break ;

    case JD_timed_till_exp:
      ret = eval_timed_till_exp( exp, id ) ;
      break ;
    case JD_timed_while_exp:
      ret = eval_timed_while_exp( exp, id ) ;
      break ;
    case JD_parallel_or_exp:
      ret = eval_parallel_or_exp( exp, id ) ;
      break ;
    case JD_parallel_and_exp:
      ret = eval_parallel_and_exp( exp, id ) ;
      break ;

    case JD_assign_exp:
      ret = eval_assign_exp( exp, id ) ;
      break ;
    case JD_assign_list_exp:
      ret = eval_assign_list_exp( exp, id ) ;
      break ;
    case JD_return_exp:
      ret = eval_return_exp( exp, id ) ;
      break ;
    case JD_rnd_exp:
    case JD_cyc_exp:
      ret = eval_rnd_exp( exp, id ) ;
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
  
  return( ret ) ;
}


int check_expression( expression_node *exp ) {
  
  if( !exp ) return 0 ;
  
  switch ( exp->type ) {
    case JD_string_exp:
      return 1 ;
      break ;
    case JD_varport_exp:
    case JD_null_exp:
    case JD_true_exp:
    case JD_false_exp:
      return 1 ;
      break ;
    case JD_function_call_exp:
    case JD_verilog_task_call_exp:
      return 1 ;
      break ;
    case JD_preincrement_exp:
    case JD_predecrement_exp:
    case JD_postincrement_exp:
    case JD_postdecrement_exp:
    case JD_concatenation_exp:
    case JD_duplication_exp:
    case JD_list_exp:
    case JD_real_num:
    case JD_int_num:
    case JD_bit_num:
      return 1 ;
      break ;
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
      if( !check_expression( exp->is.binary.left ) ) return 0 ;
      else return ( check_expression( exp->is.binary.right ) ) ;
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
      return check_expression( exp->is.unary.this ) ;
      break ;
    case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
    case JD_conditional_exp:   /*  '?' exp ':' exp            */
      if( !check_expression( exp->is.trinary.first ) ) return 0 ;
      else if( !check_expression( exp->is.trinary.second ) ) return 0 ;
      else return ( check_expression( exp->is.trinary.third ) ) ;
      break ;
    case JD_super_exp:
    case JD_this_exp:
    case JD_return_exp:
      return 1 ;
      break ;

    case JD_timed_till_exp:
    case JD_timed_while_exp:
      return 1 ; /* they are OK as far as done in a single expresson */
    case JD_parallel_or_exp:
    case JD_parallel_and_exp:
      return 0 ;  /* they are the one that fork */
      break ;
    case JD_assign_exp:
      if( !check_expression( exp->is.assign.expression ) ) return 0 ;
      else {
        if( exp->is.assign.ops == JD_assign_NB_EQ ) return 0 ;
        if( exp->is.assign.ops != JD_assign_EQ && 
           exp->is.assign.ops != JD_assign_NB_EQ    ) {
           return check_expression( exp->is.assign.lvalue ) ;
        }
        else return 1 ;
      }
      break ;
    case JD_assign_list_exp:
      if( !check_expression( exp->is.assign.expression ) ) return 0 ;
      else {
        expression_node *e = exp->is.assign.lvalue ;
        while( e ) {
          if( !check_expression( e ) ) return 0 ;
          e = e->next ;
        }
        return 1 ;
      }
      break ;
    case JD_new_exp:    
      /* TDI: new function must be parsed */
      return 1 ;
      break ;
    case JD_new_block_exp:     
      return 1 ;
      break ;
    case JD_rnd_exp:
    case JD_cyc_exp:
      {
        expression_node *cnd = exp->is.rnd.cond ;
        while( cnd ) {
          if( !check_expression( cnd->is.rnd_cond.exp ) ) return 0 ;
          cnd = cnd->is.rnd_cond.nxt ;
        }
      }
      return 1 ;
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
  
  return( 0 ) ;
}


int check_expression_list( expression_node *exp ) {
  int ret = 1 ;
  
  while( ret && exp ) {
    ret &= check_expression( exp ) ;
    exp = exp->next ;
  }
  
  return ret ;
}
