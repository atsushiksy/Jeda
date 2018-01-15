/***************************************************************************
 *
 *  parse_tree.c: functions to construct the compiler parse tree
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#include "../include/compiler_macro.h"
#include <stdio.h>
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/error_msg.h"
#include "../include/constant.h"
#include "../include/symtab.h"

extern block_node *current_block ;
extern int current_block_level ;
extern named_node *current_scope ;
extern int compile_pass ;

extern int is_static_flag ;
extern int is_extern_flag ;
extern int is_local_flag ;
extern int is_protected_flag ;
extern int is_abstract_flag ;
extern int is_overwrite_flag ;
extern int is_export_flag ;
extern int is_final_flag ;
extern int is_clone_flag ;

extern int ex_error_flag ;

file_data *compiled_files = NULL ;

int check_an_arg(
  statement_node *farg0,
  statement_node *farg1
) ;

int check_type (
  named_node *type0,
  named_node *type1
) ;

void scan_port_attrib (
  named_node *port, expression_node *list_attrib,
  char *filename, int linenum
)
{
  /* expression_node *tmp ; */
  
  /* scanning chained attrib and construct the port node */
  while( list_attrib ) {
    if( list_attrib->type != JD_port_attr_exp ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_UNKNOWN_TYPE_FOR_SIG_ATTR ;  
      /* fatal, this should be detected as compile error */
      return ;
    }
    switch( list_attrib->is.port_attr.attr ) {
      case JD_port_clock:
        port->info.port.clock = list_attrib->is.port_attr.port_name ;
        list_attrib->is.port_attr.port_name->is_clock = 1 ;
        /* check the clock is one-bit signal */
        if( list_attrib->is.port_attr.port_name->ub != 
            list_attrib->is.port_attr.port_name->lb ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_VECTORED_SIGNAL_IS_USED_FOR_CLOCK( 
            list_attrib->is.port_attr.port_name->name
          ) ;
        }
        break ;
      case JD_port_sample:
        if( port->type == JD_port_name && 
            port->info.port.sample_edge != JD_noedge ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_MORE_THAN_ONE_SAMPLE_EDGE_DETECTED ;
        }
        port->info.port.sample_edge = list_attrib->is.port_attr.edge ;
        if( list_attrib->is.port_attr.skew ) {
          if( !eval_to_int(
                list_attrib->is.port_attr.skew, &port->info.port.inputskew
              ) 
          ) 
          {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_VALUE_FOR_INPUT_SKEW ;
          }
          else if( port->info.port.inputskew > 0 ) {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_INPUT_SKEW_MUST_BE_NEGATIVE_VALUE ;
          }
        }
        break ;
      case JD_port_drive:
        if( port->type == JD_port_name && 
            port->info.port.type == JD_port_input ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_USAGE_OF_POSDRIVE_ON_INPUT ;
        }
        if( port->info.port.drive_edge != JD_noedge ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_MORE_THAN_ONE_DRIVE_EDGE_DETECTED ;
        }
        port->info.port.drive_edge = list_attrib->is.port_attr.edge ;
        if( list_attrib->is.port_attr.skew ) {
          if( !eval_to_int(
                list_attrib->is.port_attr.skew, &port->info.port.outputskew
              ) 
            || (port->info.port.outputskew < 0) 
          ) {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_VALUE_FOR_OUTPUT_SKEW ;
          }
        }
        break ;
      case JD_port_depth:
        if( port->type == JD_port_name && 
            port->info.port.type == JD_port_output ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_USAGE_OF_DEPHT_ON_OUTPUT ;
        }
        if( port->info.port.depth ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_MORE_THAN_ONE_DEPTH_ATTRIB_DETECTED ;
        }
        if( !eval_to_int(
              list_attrib->is.port_attr.depth, &port->info.port.depth
            ) ||
            (port->info.port.depth < 0)
        ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_VALUE_FOR_DEPTH ;
        }
        break ;
      case JD_port_node:
        if( port->info.port.node_path ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_MORE_THAN_ONE_NODE_ATTRIB_DETECTED ;
        }
        port->info.port.node_path = list_attrib->is.port_attr.node ;
        break ;
    }
    /* tmp = list_attrib ; */
    list_attrib = list_attrib->next ;
    /* DEALOC_EXPRESSION(tmp) ; */
  }
}

void set_var_def_flags( named_node *var, int in_class, int in_aspect ) {

  if( is_abstract_flag ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_ABSTRUCT_ATTRIB_USAGE ;
  }
  else if( is_export_flag ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE ;
  }
  else if( is_overwrite_flag ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_OVERWRITE_ATTRIB_USAGE ;
  }
  else if( is_protected_flag && !in_class ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_PROTECTED_ATTRIB_USAGE ;
  }
  else if( is_extern_flag && (in_class || in_aspect) ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE ;
  }
  else if( is_final_flag && !in_class ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_FINAL_ATTRIB_USAGE ;
  }
  else if( is_local_flag && is_protected_flag ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_PROTECTED ;
  }
  else if( is_local_flag && is_final_flag ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_FINAL ;
  }
  else if( is_static_flag && in_aspect ) {
    ERROR_LOCATION( var->filename, var->linenum ) ;
    ERROR_ILLEGAL_STATIC_ATTRIB_USAGE ;
  }
  var->is_local = is_local_flag ;
  var->is_protected = is_protected_flag ;
  var->is_final = is_final_flag ;
  var->is_extern = is_extern_flag ;
  var->is_static = is_static_flag ;
  var->is_clone = is_clone_flag ;
  
}

void set_new_def_flags( named_node *mfunc ) {

  if( is_abstract_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_ABSTRUCT_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  else if( is_overwrite_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_OVERWRITE_ATTRIB_USAGE ;
  }
  else if( is_export_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE ;
  }
  else if( is_local_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  else if( is_extern_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  else if( is_static_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  mfunc->is_protected = is_protected_flag ;
  mfunc->is_final = is_final_flag ;
  
}

void set_mfunc_def_flags( named_node *mfunc ) {

  if( is_abstract_flag ) {
    if( !current_block->is.class.is_abstract ) {
      ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
      ERROR_ILLEGAL_ABSTRUCT_METHOD_IN_NON_ABSTRUCT_CLASS ;
    }
    else if( is_local_flag ) {
      ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
      ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE_ON_ABSTRUCT_METHOD ;
    }
    else if( is_final_flag ) {
      ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
      ERROR_ILLEGAL_FINAL_ATTRIB_USAGE_ON_ABSTRUCT_METHOD ;
    }
  }
  else if( is_local_flag && is_protected_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_PROTECTED ;
  }
  else if( is_local_flag && is_final_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_FINAL ;
  }
  else if( is_overwrite_flag && is_local_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_OVERWRITE ;
  }
  if( is_extern_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  if( is_static_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  if( is_export_flag ) {
    ERROR_LOCATION( mfunc->filename, mfunc->linenum ) ;
    ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  mfunc->is_abstract = is_abstract_flag ;
  mfunc->is_local = is_local_flag ;
  mfunc->is_protected = is_protected_flag ;
  mfunc->is_final = is_final_flag ;
  mfunc->is_overwrite = is_overwrite_flag ;
  
}

void check_member_func( 
  named_node *data_type,
  named_node *name,
  statement_node *arg_list
) {
  
  if( data_type->type != name->data_type->type ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_DATA_TYPE_OF_PRE_DEFINED_FUNCTION_MISMATCH( name->name ) ;
  }
  else if( data_type->type == JD_object_type && 
           data_type->data_type != name->data_type->data_type )
  {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_DATA_TYPE_OF_PRE_DEFINED_FUNCTION_MISMATCH( name->name ) ;
  }
  
  if( is_abstract_flag ) {
    if( !current_block->is.class.is_abstract ) {
      ERROR_LOCATION( name->filename, name->linenum ) ;
      ERROR_ILLEGAL_ABSTRUCT_METHOD_IN_NON_ABSTRUCT_CLASS ;
    }
    else if( is_local_flag ) {
      ERROR_LOCATION( name->filename, name->linenum ) ;
      ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE_ON_ABSTRUCT_METHOD ;
    }
  }
  else if( is_local_flag && is_protected_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_PROTECTED ;
  }
  else if( is_local_flag && is_final_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_FINAL ;
  }
  if( is_extern_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  if( is_static_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_MEMBER_FUNCTION ;
  }
  if( name->is_abstract != is_abstract_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_ABSTRUCT_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH ;
  }
  if( name->is_local != is_local_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_LOCAL_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH ;
  }
  if( name->is_protected != is_protected_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_PROTECTED_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH ;
  }
  if( name->is_final != is_final_flag ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_FINAL_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH ;
  }

  formal_arg_check( 
    arg_list, name->info.func.func_block->is.func.args, 
    name->filename, name->linenum
  ) ;
  
}

int pointcut_match(
  statement_node *a,
  statement_node *b
) {
  expression_node *xa, *xb ;
  if( a->type != b->type ) return 0 ;
  xa = a->prefix ;
  xb = b->prefix ;
  while( xa ) {
    if( xa->type != xb->type ) return 0 ;
    if( strcmp(xa->is.string, xb->is.string) ) return 0 ;
    xa = xa->next ;
    xb = xb->next ;
  }
  return 1 ;
}

void pointcut_check(
  statement_node *pointcut_new,
  statement_node *pointcut_old,
  char *filename, int linenum
) {
  statement_node *a, *b ;
  int i ;
  a = pointcut_new ;
  i = 0 ;
  while( a ) {
    b = pointcut_old ;
    while(b) {
      if( pointcut_match(a,b) ) break ;
      else b = b->next ;
    }
    if( b == NULL ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_NTH_POINTCUT_MISMATCH( i ) ;
    }
    i++ ;
    a = a->next ;
  }
}

void check_advice( 
  named_node *data_type,
  named_node *name,
  statement_node *arg_list,
  statement_node *pointcut_list
) {
  
  if( data_type->type != name->data_type->type ) {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_DATA_TYPE_OF_PRE_DEFINED_ADVICE_MISMATCH( name->name ) ;
  }
  else if( data_type->type == JD_object_type && 
           data_type->data_type != name->data_type->data_type )
  {
    ERROR_LOCATION( name->filename, name->linenum ) ;
    ERROR_DATA_TYPE_OF_PRE_DEFINED_ADVICE_MISMATCH( name->name ) ;
  }

  formal_arg_check( 
    arg_list, name->info.func.func_block->is.func.args, 
    name->filename, name->linenum
  ) ;
  
  pointcut_check(
    pointcut_list, name->info.func.func_block->is.func.pointcut, 
    name->filename, name->linenum
  ) ;

}

/* search the member function for primitive data types */
named_node *find_member_of_sysclass(
  named_node *syscls, char *name, int *offset, named_node **this
)
{
  named_node *ret = NULL;
  char *sysclass = NULL ;
  
  switch( syscls->type ) {
    /* member function construction code may find in parse_init.c */
    case JD_void_type:      sysclass = ".void" ; break ;
    case JD_int_type:       sysclass = ".int" ; break ;
    case JD_long_type:      sysclass = ".long" ; break ;
    case JD_float_type:     sysclass = ".float" ; break ;
    case JD_double_type:    sysclass = ".double" ; break ;
    case JD_bit_type:       sysclass = ".bit" ; break ;
    case JD_bool_type:      sysclass = ".bool" ; break ;
    case JD_string_type:    sysclass = ".string" ; break ;
    case JD_signal_type:    sysclass = ".signal" ; break ;
    case JD_enum_type:      sysclass = ".enum" ; break ;
    case JD_object_type:    sysclass = ".object" ; break ;
    default: sysclass = syscls->name ; break ;
  }
  if( sysclass ) {
    *this = findname( sysclass ) ;
    if( *this ) ret = findmember_in_class( *this, name, offset ) ;
    return ret ;
  }
  else {
    return NULL ; 
  }
}

/* search the member function of array/list data types */
named_node *find_member_of_arrayclass(
  JD_sub_type sub_type, char *name, int *offset, named_node **this
)
{
  named_node *ret = NULL;
  char *sysclass = NULL ;
  
  switch( sub_type ) {
    /* member function construction code may find in parse_init.c */
    /* dot '.' is used to avoid conflict with user defined name */
    case JD_array_sub_type:             sysclass = ".array" ; break ;
    case JD_multi_array_sub_type:       sysclass = ".marray" ; break ;
    case JD_assoc_sub_type:             sysclass = ".assoc" ; break ;
    case JD_stridx_sub_type:            sysclass = ".stridx" ; break ;
    case JD_list_sub_type:              sysclass = ".list" ; break ;
  }
  if( sysclass ) {
    *this = findname( sysclass ) ;
    if( *this ) ret = findmember_in_class( *this, name, offset ) ;
    return ret ;
  }
  return NULL ;
}

named_node *find_member( 
  expression_node *data,
  char *name,
  int  *offset, 
  named_node **this
) {
  named_node *ret ;
  if( data->type == JD_aspect_exp ) {
    *this = data->is.var.name ;
    ret = findmember_in_class( *this, name, offset ) ;
    if( ret == NULL ) return NULL ;
    if( ret->is_local || ret->is_protected ) return NULL ;
    else return ret ;
  }
  if( data->type == JD_varport_exp ) {
    if( data->is.var.name->sub_type != JD_single_sub_type && 
        data->is.var.range == NULL ) 
    {
      /* array data */
      ret = 
        find_member_of_arrayclass(
          data->is.var.name->sub_type, name, offset, this
        ) ;
      return ret ;
    }
    if( data->is.var.name->type == JD_portset_name ) {
      *offset = 0 ;
      ret = findname_in_scope( data->is.var.name->child_scope, name ) ;
      if( ret ) return ret ;
      *this = findname( ".portset" ) ;
      if( *this ) ret = findmember_in_class( *this, name, offset ) ;
      return ret ;
    }
    if( data->is.var.name->type == JD_port_name ) {
      *this = findname( ".port" ) ;
      if( *this ) ret = findmember_in_class( *this, name, offset ) ;
      return ret ;
    }
    else if( data->is.var.name->type == JD_var_name || 
             data->is.var.name->type == JD_mvar_name ||
             data->is.var.name->type == JD_arg_name   )
    {
      
      /* this is possible forward reference, that the user class is
        not set yet, and reference to the member is also the forward
        reference to the variable declaration
      */
      if( data->is.var.name->data_type == NULL ) return NULL ;
      
      if( data->is.var.name->data_type->type == JD_class_name ) 
      {
        *this = data->is.var.name->data_type ;
        ret = findmember_in_class( *this, name, offset ) ;
        if( ret == NULL ) return NULL ;
        if( ret->is_local || ret->is_protected ) return NULL ;
        else return ret ;
      }
      else {
        ret = 
          find_member_of_sysclass( 
            data->is.var.name->data_type, 
            name, offset, this
          ) ;
        return ret ;
      }
    }
    else return NULL ;
  }
  else {  /* this or super */
    *this = data->user_data_type ;
    ret = findmember_in_class( *this, name, offset ) ;
    return ret ;
  }
}

int check_extention( named_node *this, named_node *super ) {
  if( !super ) return 1 ;
  if( this == super ) return 0 ;
  if( super->is_final ) return 0 ;
  return check_extention( this, super->info.class_name.parent ) ;
}

void set_func_def_flags( named_node *func ) {

  if( is_abstract_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_ABSTRUCT_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  else if( is_protected_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_PROTECTED_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  else if( is_final_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_FINAL_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  if( is_extern_flag && is_local_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_EXTERN ;
  }
  if( is_export_flag && is_local_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_EXPORT ;
  }
  if( is_extern_flag && is_export_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_COMBINATION_OF_EXPORT_AND_EXTERN ;
  }
  if( is_static_flag ) {
    ERROR_LOCATION( func->filename, func->linenum ) ;
    ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_FUNCTION ;
  }
  func->is_local = is_local_flag ;
  func->is_extern = is_extern_flag ;
  func->is_export = is_export_flag ;
  
}

int abstract_formal_arg_check ( 
  statement_node *fargs,
  statement_node *fargs1
) 
{
  
  while( fargs ) {
    if( fargs1 == NULL ) {
      return 0 ;
    }
    if( !check_an_arg( fargs, fargs1 ) ) {
      return 0 ;
    }
    fargs = fargs->next ;
    fargs1 = fargs1->next ;
  }
  if( fargs1 != NULL ) return 0 ;
  else return 1 ;

}

void check_abstract_members( named_node *class ) {
  named_node *parent, *abst, *func ;
  
  parent = class->info.class_name.parent ;
  if( parent && parent->is_abstract ) {
    abst = parent->child_scope ;
    while( abst ) {
      if( abst->is_abstract ) { 
        if( !( func = findname_in_classname(class, abst->name) ) )
        {
          ERROR_LOCATION( class->filename, class->linenum ) ;
          ERROR_MISSING_ABSTRUCT_FUNCTION_DEFINITION(abst->name) ;
        }
        else if( func->type != JD_mfunc_name ) {
          ERROR_LOCATION( class->filename, class->linenum ) ;
          ERROR_ILLEGAL_ABSTRUCT_FUNCTION_REDEFINITION(abst->name) ;
        }
        else if( !check_type( abst->data_type, func->data_type ) ) {
          ERROR_LOCATION( class->filename, class->linenum ) ;
          ERROR_ILLEGAL_ABSTRUCT_FUNCTION_TYPE_REDEFINITION(abst->name) ;
        }
        else if( !abstract_formal_arg_check( 
                    abst->info.func.args, func->info.func.args
                  )
               )
        {
          ERROR_LOCATION( class->filename, class->linenum ) ;
          ERROR_ARG_MISMATCH_ON_ABSTRUCT_FUNCTION_REDEFINITION(func->name) ;
        }
        else {
          func->is_unabstracted = 1 ;
        }
      }
      abst = abst->next_in_scope ;
    }
  }
}

void check_overwrite_members( named_node *class ) {
  named_node *parent, *ovrwt, *func ;
  
  parent = class->info.class_name.parent ;
  ovrwt = class->child_scope ;
  while( ovrwt ) {
    if( ovrwt->is_overwrite ) {
      if( !parent ) {
        ERROR_LOCATION( ovrwt->filename, ovrwt->linenum ) ;
        ERROR_ILLEGAL_OVERWRITE_FUNCTION_IN_NON_EXTENTED_CLASS ;
      }
      else if( !( func = findname_in_classname(parent, ovrwt->name) ) )
      {
        ERROR_LOCATION( ovrwt->filename, ovrwt->linenum ) ;
        ERROR_MISSING_OVERWRITE_FUNCTION_IN_PARENT_CLASS(ovrwt->name) ;
      }
      else if( func->type != JD_mfunc_name ) {
        ERROR_LOCATION( ovrwt->filename, ovrwt->linenum ) ;
        ERROR_ILLEGAL_OVERWRITE_FUNCTION_REDEFINITION(ovrwt->name) ;
      }
      else if( !check_type( ovrwt->data_type, func->data_type ) ) {
        ERROR_LOCATION( ovrwt->filename, ovrwt->linenum ) ;
        ERROR_ILLEGAL_OVERWRITE_FUNCTION_TYPE_REDEFINITION(ovrwt->name) ;
      }
      else if( !abstract_formal_arg_check( 
                  ovrwt->info.func.args, func->info.func.args
                )
             )
      {
        ERROR_LOCATION( ovrwt->filename, ovrwt->linenum ) ;
        ERROR_ARG_MISMATCH_ON_OVERWRITE_FUNCTION_REDEFINITION(func->name) ;
      }
    }
    ovrwt = ovrwt->next_in_scope ;
  }
}

/*
 * functions to handle label stack for break and continue
 * statement
 */
static label_data *last_label = NULL ;

void push_label( 
  char *label, int continue_num, int break_num, int level, int repeat 
) {
  label_data *lb = ALLOC(label_data) ;
  lb->label = label ;
  lb->continue_num = continue_num ;
  lb->break_num = break_num ;
  lb->level = level ;
  lb->repeat = repeat ;
  lb->prev = last_label ;
  last_label = lb ;
}

/* push label is called when label is detected at
 * the beginning of a loop statement.
 */
void pop_label() 
{
  label_data *p ;
  
  if( last_label ) {
    p = last_label ;
    last_label = last_label->prev ;
    free(p) ;
  }
  else {
    ERROR_INTERNAL_LABEL_STACK_OUT_OF_ORDER ;
  }
}

/* 
 * find label is called when break/continue with/without label
 * is detected.
 */
label_data *find_label( char *label )
{
  label_data *lb = last_label ;
  if( label == NULL ) return lb ;
  while(lb) {
    if( lb->label != NULL && !strcmp( label, lb->label ) ) return lb ;
    lb = lb->prev ;
  }
  return NULL ;

}



/*
 *
 *  ALLOC( ) is mapped to this
 *
 */
void *check_alloc( int size ) {
  int *ret ;
  
  ret = (int *)calloc(1, size+10 ) ;
  
  if( ret == NULL ) {
    ERROR_FATAL_NO_MEMORY ;
  }
  
  return ret ;

}

void check_free( int *p ) {
  
  if( p ) free(p) ;

}

/***************************************************************
 *  Constant handling functions
 ***************************************************************/
/* left-shifting array of bit data by num */
static void lshift_bit( unsigned *data, int glen, int num )
{
  int i, j ;
  for( i = 0 ; i < num ; i ++ )
    for( j = glen-1 ; j >= 0 ; j-- )
      data[j] = (data[j] << 1) | ( (j == 0) ? 0 : (data[j-1]>>31) & 1 ) ;
}


/* right-shifting array of bit data by num */
/* never used

static void rshift_bit( unsigned *data, int glen, int num )
{
  int i, j ;
  for( i = 0 ; i < num ; i ++ )
    for( j = 0 ; j < glen ; j-- )
      data[j] = 
        ( (data[j] >> 1) & 0xefffffff ) | 
        ( (j == glen-1) ? 0 : (data[j+1]&1) << 31 ) ;
}

*/

static unsigned get_bit( unsigned *data, int location, int len )
{
  unsigned ret, mask ;
  int i ;
  
  ret = data[location/32] ;
  
  ret >>= (location % 32) ;
  
  mask = 0 ;
  for( i = 0 ; i < len ; i++ ) {
    mask <<= 1 ;
    mask |= 1 ;
  }
  
  return ret & mask ;
}


/* calculate a = a + b */
static void add_bit( unsigned *a, unsigned *b, int glen )
{
  int i ;
  unsigned tmp, carry ;
  carry = 0 ;
  for( i = 0 ; i < glen ; i++ ) {
    tmp = a[i] + b[i] + carry ;
    if( tmp < a[i] ) carry = 1 ; /* detecting overflow */
    a[i] = tmp ;
  }
}

static void scan_decimal( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  unsigned *bit_c ;
  int ovflow = 0 ;
  int glen ;
  int i ;
  
  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_c = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    switch( *cp ) {
      case '_' :
        break ;
      default  :  /* 0 - 9 */
        /* do bit_a = bit_a * 10  */
        for( i = 0 ; i < glen ; i++ ) bit_b[i] = bit_a[i] ;
        if( get_bit(bit_a, size-3, 3) ) {
          if( !ovflow && compile_pass==2 ) {
            ERROR_LOCATION(exp->filename, exp->linenum) ;
            WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
            ovflow = 1 ;
          }
          size += 4 ;
          if( NWORD(size) > glen ) {
            int pglen = glen ;
            glen = NWORD(size) ;
            bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
            bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
            bit_c = (unsigned *)realloc(bit_c, glen*sizeof(unsigned) ) ;
            for( i = pglen-1 ; i < glen ; i++ ) {
              bit_a[i] = bit_b[i] = bit_c[i] = 0 ;
            }
          }
        }
        lshift_bit(bit_a, glen, 3 ) ; /* a = a * 8 */
        lshift_bit(bit_b, glen, 1 ) ; /* b = a * 8 */
        add_bit(bit_a, bit_b, glen ) ; /* a = (a*8) + (a*2) */
        bit_c[0] = *cp-'0' ;
        add_bit(bit_a, bit_c, glen ) ;
        break ;
    }
    cp++ ;
  }
  
  /* this optimization won't work for {bit,bit} type of operation
  if( size < 32 ) {
    exp->type = JD_int_num ;
    exp->is.int_value = bit_a[0] ;
    free( bit_a ) ;
    free( bit_b ) ;
    free( bit_c ) ;
  }
  else {
  */
    exp->type = JD_bit_num ;
    exp->is.bit_const.size = size ;
    exp->is.bit_const.a = (int *)bit_a ;
    for( i = 0 ; i < glen ; i++ ) bit_b[i] = 0 ;
    exp->is.bit_const.b = (int *)bit_b ;
    free( bit_c ) ;
  /* } */


}
void scan_v_decimal(  char *number, expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      ERROR_LOCATION(exp->filename, exp->linenum) ;
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;
  
}


void scan_c_decimal( char *number, expression_node *exp )
{
  char *cp = number ;
  int  size ;

  size = 32 ;
    
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;

}

static void scan_hex( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  
  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-4, 4) || get_bit(bit_b, size-4, 4) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 4 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 4 ) ; 
      lshift_bit(bit_b, glen, 4 ) ; 
      switch( *cp ) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
          bit_a[0] |= (*cp-'a'+10) ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          bit_a[0] |= (*cp-'A'+10) ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0xf ;
          bit_b[0] |= 0xf ;
          if( ext_flag==0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0xf ;
          if( ext_flag==0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }
  
  if( ext_flag > 1 ) {
    int a, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }
  
  /* this optimization won't work for {bit,bit} type of operation
  if( size < 32 ) {
    exp->type = JD_int_num ;
    exp->is.int_value = bit_a[0] ;
    free( bit_a ) ;
    free( bit_b ) ;
  }
  else {
  */
    exp->type = JD_bit_num ;
    exp->is.bit_const.size = size ;
    exp->is.bit_const.a = (int *)bit_a ;
    exp->is.bit_const.b = (int *)bit_b ;
  /* } */
  
}

void scan_v_hex(  char *number, expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip h/H */
  
  scan_hex( size, cp, exp ) ;

}

void scan_c_hex(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip x/X */
  
  scan_hex( size, cp, exp ) ;

}

static void scan_octal( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-3, 3) || get_bit(bit_b, size-3, 3) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 3 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 3 ) ; 
      lshift_bit(bit_b, glen, 3 ) ; 
      switch( *cp ) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x7 ;
          bit_b[0] |= 0x7 ;
          if( ext_flag==0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x7 ;
          if( ext_flag==0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }

  if( ext_flag > 1 ) {
    int a, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }

  /* this optimization won't work for {bit,bit} type of operation
  if( size < 32 ) {
    exp->type = JD_int_num ;
    exp->is.int_value = bit_a[0] ;
    free( bit_a ) ;
    free( bit_b ) ;
  }
  else {
  */
    exp->type = JD_bit_num ;
    exp->is.bit_const.size = size ;
    exp->is.bit_const.a = (int *)bit_a ;
    exp->is.bit_const.b = (int *)bit_b ;
  /* } */
  
}


void scan_v_octal(  char *number, expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

void scan_c_octal(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

static void scan_binary( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-1, 1) || get_bit(bit_b, size-1, 1) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 1 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 1 ) ; 
      lshift_bit(bit_b, glen, 1 ) ; 
      switch( *cp ) {
        case '0': case '1':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag == 0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x1 ;
          bit_b[0] |= 0x1 ;
          if( ext_flag == 0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x1 ;
          if( ext_flag == 0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }

  if( ext_flag > 1 ) {
    int a, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }

  /* this optimization won't work for {bit,bit} type of operation
  if( size < 32 ) {
    exp->type = JD_int_num ;
    exp->is.int_value = bit_a[0] ;
    free( bit_a ) ;
    free( bit_b ) ;
  }
  else {
  */
    exp->type = JD_bit_num ;
    exp->is.bit_const.size = size ;
    exp->is.bit_const.a = (int *)bit_a ;
    exp->is.bit_const.b = (int *)bit_b ;
  /* } */
  
}


void scan_v_binary(  char *number, expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

void scan_c_binary(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

int check_type (
  named_node *type0,
  named_node *type1
)
{
  if( type0->type != type1->type ) return 0 ;
  if( type0->type == JD_object_type || type0->type == JD_enum_type ) {
    if( type0 != type1 ) return 0 ;
  }
  return 1 ;
}

int check_range(
  expression_node *r0, expression_node *r1 
)
{
  int rc0, rc1 ;
  if( r0 == NULL && r1 == NULL ) return 1 ;
  if( r0 == NULL || r1 == NULL ) return 0 ;
  if( r0 ->type != r1->type ) return 0 ;
  if( r0->type == JD_single_range_exp ) {
    /* checking all multi-dimentional definitions */
    while( r0 ) {
      if( r1 == NULL ) return 0 ;
      if( !eval_to_int( r0->is.range.ub, &rc0 ) ||
          !eval_to_int( r1->is.range.ub, &rc1 ) ||
          (rc0 < 0) || (rc1 < 0) || (rc0 != rc1)   ) return 0 ;
      r0 = r0->next ;
      r1 = r1->next ;
    }
    if( r1 ) return 0 ;
  }
  return 1 ;
}

int is_simple_arg( statement_node *farg ) {
  if( farg->type != JD_vardeclare_statement )  return 0 ;
  if( farg->is.variable.declare->data_type->type != JD_bit_type &&
      farg->is.variable.declare->data_type->type != JD_int_type   ) return 0 ;
  return 1 ;
}

int is_bit_arg( statement_node *farg ) {
  if( farg->type != JD_vardeclare_statement )  return 0 ;
  if( farg->is.variable.declare->data_type->type != JD_bit_type ) return 0 ;
  return 1 ;
}

int check_an_arg(
  statement_node *farg0,
  statement_node *farg1
)
{
  if( farg0->type != JD_vardeclare_statement &&
      farg0->type != JD_formal_arg_statement &&
      farg0->type != JD_one_or_more_arg_statement &&
      farg0->type != JD_zero_or_more_arg_statement &&
      farg0->type != JD_wildcard_arg_statement  )  return 0 ;
      
  if( farg1->type != JD_vardeclare_statement &&
      farg1->type != JD_formal_arg_statement &&
      farg1->type != JD_one_or_more_arg_statement &&
      farg1->type != JD_zero_or_more_arg_statement &&
      farg1->type != JD_wildcard_arg_statement  )  return 0 ;
  
  if( farg0->type != farg1->type ) return 0 ;
  
  if( farg0->type == JD_vardeclare_statement ) {
    if( farg0->is.variable.declare->data_type->type != 
        farg1->is.variable.declare->data_type->type           ) return 0 ;
  
    if( farg0->is.variable.declare->data_type->type == JD_object_type ||
        farg0->is.variable.declare->data_type->type == JD_enum_type
     ) {
      if( farg0->is.variable.declare->data_type != 
            farg1->is.variable.declare->data_type   ) return 0 ;
  
    }
    if( farg0->is.variable.declare->is_var != 
          farg0->is.variable.declare->is_var    ) return 0 ;
    if( !check_range( 
          farg0->is.variable.declare->var_list->info.var.range,
          farg1->is.variable.declare->var_list->info.var.range   ) ) return 0 ;
    return 1 ;
  }
  else if( farg0->type == JD_one_or_more_arg_statement ||
           farg0->type == JD_zero_or_more_arg_statement   )  
  {
    return 
      check_an_arg( 
        farg0->is.formal_arg.child, farg1->is.formal_arg.child
      ) ;
  }
  else if( farg0->type == JD_formal_arg_statement ) {
    if( farg0->is.formal_arg.data_type->type != 
        farg1->is.formal_arg.data_type->type           ) return 0 ;
  
    if( farg0->is.formal_arg.data_type->type == JD_object_type ||
        farg0->is.formal_arg.data_type->type == JD_enum_type
     ) {
      if( farg0->is.formal_arg.data_type != 
            farg1->is.formal_arg.data_type   ) return 0 ;
  
    }
    return 1 ;
  }
  return 0 ;
}

void formal_arg_check ( 
  statement_node *fargs,
  statement_node *fargs1,
  char *filename,
  int linenum
) 
{
  int argc = 1 ;
  
  while( fargs ) {
    if( fargs1 == NULL ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_NUMBER_OF_ARG_MISMATCH_ON_PRE_DEFINED_FUNCTION ;
      return ;
    }
    if( !check_an_arg( fargs, fargs1 ) ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_N_TH_ARGMENT_TYPE_MISMATCH_ON_PRE_DEFINED_FUNCTION( argc ) ;
    }
    fargs = fargs->next ;
    fargs1 = fargs1->next ;
    argc++ ;
  }
  if( fargs1 != NULL ) {
    ERROR_LOCATION( filename, linenum ) ;
    ERROR_NUMBER_OF_ARG_MISMATCH_ON_PRE_DEFINED_FUNCTION ;
    return ;
  }

}

/* user_function_definition: 
   $2=var_func_type, $3=func_name, $5=opt_formal_args 
*/
void check_func( 
  named_node *ftype,
  named_node *fname,
  statement_node *fargs
){
  statement_node *fargs1 ;
  
  if( fname->data_type ) {
    if( !check_type( ftype, fname->data_type ) ) {
      ERROR_LOCATION( fname->filename, fname->linenum ) ;
      ERROR_PRE_DEFINED_FUNCTION_TYPE_MISMATCH ;
      return ;
    }
  }
  else fname->data_type = ftype ;
  
  fargs1 = fname->info.func.func_block->is.func.args ;
  
  formal_arg_check( fargs, fargs1, fname->filename, fname->linenum ) ;
    
}

/* user_function_definition: 
   $1=var_func_type, $2=func_name, $4=opt_formal_args 
*/
void check_user_member_func( 
  named_node *ftype,
  named_node *fname,
  statement_node *fargs
){
  statement_node *fargs1 ;
  
  if( !check_type( ftype, fname->data_type ) ) {
    ERROR_LOCATION( fname->filename, fname->linenum ) ;
    ERROR_PRE_DEFINED_FUNCTION_TYPE_MISMATCH ;
    return ;
  }
  
  fargs1 = fname->info.func.class_block->is.func.args ;
  
  formal_arg_check( fargs, fargs1, fname->filename, fname->linenum ) ;
  
}

int var_port_func_type( JD_data_name_type  type ) {

  switch( type ) {
    case JD_mvar_name:
    case JD_var_name:
    case JD_arg_name:
    case JD_port_name:
    case JD_portset_name:
    case JD_enum_member_name:
    case JD_func_name:
    case JD_mfunc_name:
    case JD_aspect_name:
    case JD_advice_name:
      return 1 ;
      break ;
    default:
      return 0 ;
      break ;
  }
  

}

int var_port_type( JD_data_name_type  type ) {

  switch( type ) {
    case JD_mvar_name:
    case JD_var_name:
    case JD_arg_name:
    case JD_port_name:
      return 1 ;
      break ;
    default:
      return 0 ;
      break ;
  }
  

}

int port_type( named_node *name ) {

  if( name->type == JD_port_name ) return 1 ;
  else {
    if( name->type == JD_var_name || name->type == JD_mvar_name ||
        name->type == JD_arg_name  ) 
    {
      if( name->data_type->type == JD_signal_type ) {
        return 1 ;   
      }
      else return 0 ;
    }
    else return 0 ;
  }
}

int is_port_var_exp( expression_node *exp ) {
  named_node *name = exp->is.var.name ;
  
  if( exp->type != JD_varport_exp ) return 0 ;
  
  if( name->type == JD_port_name ) return 1 ;
  else {
    if( name->type == JD_var_name || name->type == JD_mvar_name ||
        name->type == JD_arg_name  ) 
    {

      if( name->data_type->type == JD_signal_type && exp->is.var.depth ) {
        return 1 ;   
      }
      else return 0 ;
    }
    else return 0 ;
  }
}

int is_signal_exp( expression_node *exp ) {
  named_node *name = exp->is.var.name ;
  
  if( exp->type != JD_varport_exp ) return 0 ;
  
  if( name->type == JD_port_name ) {
    if( exp->is.var.depth ) return 0 ;
    else return 1 ;   
  }
  else {
    if( name->type == JD_var_name || name->type == JD_mvar_name ||
        name->type == JD_arg_name  ) 
    {

      if( name->data_type->type == JD_signal_type) {
        if( exp->is.var.depth ) return 0 ;
        else if( name->sub_type != JD_single_sub_type ) {
          if( exp->is.var.range == NULL ) return 0 ;
          else return 1 ;
        }
        else return 1 ;   
      }
      else return 0 ;
    }
    else return 0 ;
  }
}

int check_var_range( 
  named_node *name, expression_node *depth, expression_node *range, 
  char *filename, int linenum
) {
  int ret = 0 ;
  if( name->type == JD_port_name ) {
    if( range == NULL ) return 1 ;
    if( range->type == JD_string_range_exp ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_STRING_TYPE_ARRAY_RANGE_DETECTED_FOR_PORT_ACCESS(name->name) ;
    }
    if( range->is.range.num_index > 1 ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_MULTI_DIMENTION_ACCESS_DETECTED_FOR_PORT_ACCESS(name->name) ;
    }
    else ret = 1 ;
    if( range->type == JD_single_range_exp ) {
      range->type = JD_single_bit_range_exp ;
      /* now accessing data on the port needs depth (otherwise pointer) */
      if( depth == NULL ) {
	ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_BIT_RANGE_ON_PORT_WITHOUT_DEPTH(name->name) ;
      }
    }
  }
  else if( range != NULL ) {
    if( range->type == JD_bit_range_exp ) {
      if( name->data_type->type == JD_signal_type ) {
        /* if we allow partial vector of signal, remove the following check */
        if( depth == NULL ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_BIT_RANGE_ON_NON_BIT_VARIABLE(name->name) ;
        }
        else ret = 1 ;
      }
      else if( name->data_type->type != JD_bit_type ) 
      {
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_BIT_RANGE_ON_NON_BIT_VARIABLE(name->name) ;
      }
      else if( name->sub_type != JD_single_sub_type ) {
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_BIT_RANGE_ON_ARRAY_VARIABLE(name->name) ;
      }
      else ret = 1 ;
    }
    else if( range->type == JD_string_range_exp ) {
      if( name->sub_type != JD_stridx_sub_type ) {
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_STRING_RANGE_ON_NON_STRING_INDEXED_VARIABLE(name->name) ;
      }
      else if( range->is.range.bit_index ) {
        if( name->data_type->type == JD_signal_type ) {
          if( depth == NULL ) {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_BIT_RANGE_ON_NON_BIT_VARIABLE(name->name) ;
          }
          else ret = 1 ;
        }
        else if( name->data_type->type != JD_bit_type ) 
        {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_BIT_RANGE_ON_NON_BIT_VARIABLE(name->name) ;
        }
        else ret = 1 ;
      }
      else ret = 1 ;
    }
    else {
      /* range->type = JD_single_range_exp */
      if( name->sub_type == JD_single_sub_type ) {
        if( range->is.range.bit_index ) { /* has [u:l] at the end */
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
        }
        else if( name->data_type->type == JD_signal_type ){
          if( depth == NULL )
	  {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
          }
	  else ret = 1 ;
        }
        else if( name->data_type->type != JD_bit_type ) 
        {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
        }
        else if( range->is.range.num_index > 1 ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
        }
	else ret = 1 ;
	/* has single [n] on bit type data, mark this as single bit range */
        range->is.range.bit_index = 1 ;
        range->type = JD_single_bit_range_exp ;
      }
      else if( name->sub_type == JD_array_sub_type ||
               name->sub_type == JD_assoc_sub_type    ) 
      {
        /* expect single [n], plus possible one more [n] for bit access */
        if( range->is.range.num_index == 2 ) {
          if( range->is.range.bit_index ) { /* has [u:l] at the end */
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
          }
          else if( name->data_type->type == JD_signal_type ) {
            if( depth == NULL ) {
              ERROR_LOCATION( filename, linenum ) ;
              ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
            }
            else {
	      ret = 1 ;
              range->is.range.bit_index = 1 ;
              range->is.range.num_index = 1 ;
              range->next->type = JD_single_bit_range_exp ;
	    }
          }
          else if( name->data_type->type != JD_bit_type ) 
          {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
          }
          else {
            ret = 1 ;
            range->is.range.bit_index = 1 ;
            range->is.range.num_index = 1 ;
            range->next->type = JD_single_bit_range_exp ;
          }
        } 
        else if( range->is.range.num_index > 1 ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
        }
        else ret = 1 ;
      }
      else if( name->sub_type == JD_multi_array_sub_type ) {
        if( name->info.var.range->is.range.num_index == 
              range->is.range.num_index - 1              )
        {
          if( range->is.range.bit_index ) {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
          }
          else if( name->data_type->type == JD_signal_type ) {
            if( depth == NULL ) {
              ERROR_LOCATION( filename, linenum ) ;
              ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
            }
            else {
              range->is.range.bit_index = 1 ;
              range->is.range.num_index -= 1 ;
              while( range->next ) range = range->next ;
              range->type = JD_single_bit_range_exp ;
              ret = 1 ;
            }
          }
          else if( name->data_type->type != JD_bit_type ) 
          {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
          }
          else {
            range->is.range.bit_index = 1 ;
            range->is.range.num_index -= 1 ;
            while( range->next ) range = range->next ;
            range->type = JD_single_bit_range_exp ;
            ret = 1 ;
          }
        }
        else if( name->info.var.range->is.range.num_index != 
              range->is.range.num_index                  )
        {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
        }
        else {
	  if( range->is.range.bit_index ) {
	    /* it has [u:l] at the end */
	    if( name->data_type->type == JD_signal_type ) {
	      if( depth == NULL ) {
		ERROR_LOCATION( filename, linenum ) ;
		ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
	      }
              else {
                ret = 1 ;
              }
            }
            else if( name->data_type->type != JD_bit_type ) 
            {
              ERROR_LOCATION( filename, linenum ) ;
              ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
            }
            else {
              ret = 1 ;
            }
	  }
          else ret = 1 ;
	}
      }
      else if( name->sub_type == JD_list_sub_type ) {
        /* trial: let direct access to list */
        /* expect single [n], plus possible one more [n] for bit access */
        if( range->is.range.num_index == 2 ) {
          if( range->is.range.bit_index ) { /* has [u:l] at the end */
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
          }
          else if( name->data_type->type == JD_signal_type ) {
            if( depth == NULL ) {
              ERROR_LOCATION( filename, linenum ) ;
              ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
            }
            else {
	      ret = 1 ;
              range->is.range.bit_index = 1 ;
              range->is.range.num_index = 1 ;
              range->next->type = JD_single_bit_range_exp ;
	    }
          }
          else if( name->data_type->type != JD_bit_type ) 
          {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
          }
          else {
            ret = 1 ;
            range->is.range.bit_index = 1 ;
            range->is.range.num_index = 1 ;
            range->next->type = JD_single_bit_range_exp ;
          }
        } 
        else if( range->is.range.num_index > 1 ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
        }
        else ret = 1 ;
        /*
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
        */
      }
      else if( name->sub_type == JD_stridx_sub_type ) {
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_ARRAY_RANGE_ON_STRING_INDEXED_VARIABLE(name->name) ;
      }
      else { /* non array type */
        if( range->is.range.num_index > 1 || range->is.range.bit_index ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
        }
        else if( name->data_type->type == JD_signal_type ) {
          if( depth == NULL ) {
            ERROR_LOCATION( filename, linenum ) ;
            ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(name->name) ;
          }
          else {
            ret = 1 ;
            range->is.range.bit_index = 1 ;
            range->is.range.num_index = 1 ;
            range->next->type = JD_single_bit_range_exp ;
	  }
        }
        else if( name->data_type->type != JD_bit_type ) 
        {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(name->name) ;
        }
        else {
          ret = 1 ;
          range->is.range.bit_index = 1 ;
          range->is.range.num_index = 0 ;
          range->type = JD_single_bit_range_exp ;
        }
      }
    }
  }
  else { /* case range==NULL */
    ret = 1 ;
    /* direct array access is allowed !
    if( name->sub_type != JD_single_sub_type &&
        name->sub_type != JD_list_sub_type      ) 
    {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_MISSING_ACCESS_RANGE_ON_ARRAY_VARIABLE(name->name) ;        
        
    }
    */
  }
  
  return ret ;
  
}


/* type checking code */

int is_loop_expression( expression_node *exp ) {
  if( exp ==  NULL ) return 0 ;
  switch( exp->data_type ) {
    case JD_exp_num_type:    return 1 ;
    case JD_exp_string_type: return 1 ;
    case JD_exp_void_type:   return 0 ;
    case JD_exp_object_type: return 1 ;
    case JD_exp_enum_type:   return 1 ;
  }
  return 0 ;
}

int is_compare_ok( expression_node *c0, expression_node *c1 ) {
  if( c0 == NULL || c1 == NULL ) return 0 ;
  if( ( c0->data_type == JD_exp_object_type ||
        c0->data_type == JD_exp_string_type ||
        c0->data_type == JD_exp_signal_type    ) && 
      c1->data_type == JD_exp_void_type         ) return 1 ;
  if( ( c1->data_type == JD_exp_object_type ||
        c1->data_type == JD_exp_string_type ||
        c1->data_type == JD_exp_signal_type    ) && 
      c0->data_type == JD_exp_void_type         ) return 1 ;
  if( c0->data_type != c1->data_type ) return 0 ;
  if( c0->data_type == JD_exp_enum_type || 
      c0->data_type == JD_exp_object_type ) 
  {
    if( c0->user_data_type != c1->user_data_type ) return 0 ;
    else return 1 ;
  }
  else return 1 ;
  
}

void case_items_type_check( 
  expression_node *cond, statement_node *items, char *filename, int linenum
)
{
  int n = 1 ;
  expression_node *itm ;
  while( items ) {
    if( items->type == JD_case_default ) {
      /* no check for default */
    }
    else {
      itm = items->is.case_item.cond ;
      if( !itm ) {
        ERROR_LOCATION( filename, linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_ON_NTH_CONDITION_FOR_CASE_STATEMENT( n ) ;
        return ;
      }
      while( itm ) {
        if( !is_compare_ok( cond, itm ) ) {
          ERROR_LOCATION( filename, linenum ) ;
          ERROR_ILLEGAL_DATA_TYPE_ON_NTH_CONDITION_FOR_CASE_STATEMENT( n ) ;
          return ;
        }
        n++ ;
        itm = itm->next ;
      }
    }
    items = items->next ;
  }
  
}

int is_port_expression( expression_node *exp  ) {
  if( exp == NULL ) return 0 ;
  if( exp->type == JD_port_exp ) return 1 ;
  if( exp->type == JD_varport_exp ) {
    return( port_type( exp->is.var.name ) ) ;
  }
  return 0 ;
}

int is_parent_class( named_node *child, named_node *dtype )
{
  while( child ) {
    if( child == dtype ) return 1 ;
    child = child->info.class_name.parent ;
  } 
  return 0 ;
}

int get_parent_class_offset( named_node *child, named_node *dtype )
{
  int offset = 0 ;
  named_node *keep ;
  
  keep = child ;
  while( child ) {
    if( child == dtype ) return offset ;
    offset++ ;
    child = child->info.class_name.parent ;
  } 
  offset = 0 ;
  if( child == NULL ) {
    while( dtype ) {
      if( keep == dtype ) return offset ;
      offset-- ;
      dtype = dtype->info.class_name.parent ;
    }
  }
  /* fatal situation, no offset found in either direction */
  fprintf( 
    stderr, "Jeda compiler runtime error, class cast offset can't be found\n" 
  ) ;
  exit(1) ;
  /*NOTREACHED*/
}

/*ARGSUSED*/
int array_var_dtype_check (
  named_node *var, expression_node *data, int parent_flag
)
{
  if( data->type != JD_varport_exp ) return 0 ;
  
  switch( var->sub_type ) {
    case JD_array_sub_type:
      if( data->data_type != JD_exp_array_type ) return 0 ;
      return
        array_type_check(
          var->info.var.range, 
          data->is.var.name->info.var.range
        ) ;
      break ;
    case JD_multi_array_sub_type:
      if( data->data_type != JD_exp_marray_type ) return 0 ;
      return
        array_type_check(
          var->info.var.range, 
          data->is.var.name->info.var.range
        ) ;
      break ;
    case JD_assoc_sub_type:
      if( data->data_type != JD_exp_assoc_type ) return 0 ;
      return
        array_type_check(
          var->info.var.range, 
          data->is.var.name->info.var.range
        ) ;
      break ;
    case JD_stridx_sub_type:
      if( data->data_type != JD_exp_stridx_type ) return 0 ;
      return
        array_type_check(
          var->info.var.range, 
          data->is.var.name->info.var.range
        ) ;
      break ;
    case JD_list_sub_type:
      if( data->data_type != JD_exp_list_type ) return 0 ;
      return
        array_type_check(
          var->info.var.range, 
          data->is.var.name->info.var.range
        ) ;
      break ;
  }
  
  if( var->data_type->type != data->is.var.name->data_type->type ) return 0 ;
  if( var->data_type->type == JD_object_type ) {
    return 0 ; /* TBI */
  }
  return 1 ;
}

int check_var_arg(
  statement_node *farg,
  expression_node *arg
)
{
  
  if( farg->is.variable.declare->is_var ) {
    if( arg->type != JD_varport_exp ) return 0 ;
    if( arg->is.var.name->data_type->type != 
          farg->is.variable.declare->data_type->type )
      return 0 ;
    if( arg->is.var.name->data_type->type == JD_class_name &&
        arg->is.var.name->data_type != farg->is.variable.declare->data_type )
      return 0 ;
    if( !check_range( 
          farg->is.variable.declare->var_list->info.var.range,
          arg->is.var.name->info.var.range   ) 
      ) return 0 ;
    if( arg->is.var.name->data_type->type == JD_bit_type ) {
      if( arg->is.var.name->data_type->ub != 
          farg->is.variable.declare->data_type->ub ) return 0 ;
      if( arg->is.var.name->data_type->lb != 
          farg->is.variable.declare->data_type->lb ) return 0 ;
    }
    if( arg->is.var.depth ) return 0 ;
    arg->is.var.var_arg = 1 ;
  }
  else {
    if( arg->type == JD_default_arg ) {
      if( farg->is.variable.declare->var_list->info.var.init ) return 1 ;
      else return 0 ;
    }
    if( farg->is.variable.declare->var_list->sub_type == JD_single_sub_type ) 
    {
      return 
        var_dtype_check( farg->is.variable.declare->data_type, arg, 1, 1 ) ;
    }
    else {
      /* non-single type */
      return
        array_var_dtype_check( farg->is.variable.declare->var_list, arg, 1 ) ;
    }
  }
  return 1 ;
}

int arg_end_check ( statement_node *farg ) {
  while( farg ) {
    switch(farg->type) {
      case JD_zero_or_more_arg_statement: 
        break ;
      case JD_one_or_more_arg_statement:
        if( !arg_end_check( farg->is.formal_arg.child ) ) return 0 ;
        break ;
      case JD_opt_args_statement:
        break ;
      case JD_vardeclare_statement:
        if( farg->is.variable.declare->var_list->info.var.init == NULL ) 
          return 0 ;
        if( farg->is.variable.declare->is_var ) return 0 ;
        break ;
      default:
        return 0 ;
        break ;  
    }
    farg = farg->next ;
  }
  return 1 ;
}

int check_arg_type (
  statement_node *farg,
  expression_node *arg,
  expression_node *pre,
  int *location
)
{
  int tmp, count, i ;
  int ret = 0 ;
  statement_node *child, *next ;
  int one_detected = 0 ;
  while( arg ) {
    if( farg == NULL ) {
      return ret ;
    }
    switch(farg->type) {
      case JD_or_of_args_statement:
        child = farg->is.formal_arg.child ;
        while(child) {
          count = 0 ;
          next = child->next ;
          child->next = NULL ;
          tmp = check_arg_type( child , arg, pre, &count ) ; 
          child->next = next ;
          if( tmp <= 0 ) {
            child = child->next ;
          }
          else {
            ret = 1 ;
            for( i = 0 ; i < count ; i++ ) arg = arg->next ;
            *location = *location + count ;
            farg = farg->next ;
            child = NULL ;
          }
        }
        break ;
      case JD_opt_args_statement:
        child = farg->is.formal_arg.child ;
        count = 0 ;
        tmp = check_arg_type( child , arg, pre, &count ) ; 
        if( tmp > 0 ) {
          ret = 1 ;
          for( i = 0 ; i < count ; i++ ) arg = arg->next ;
          *location = *location + count ;
        }
        farg = farg->next ;
        break ;
      case JD_zero_or_more_arg_statement:
        child = farg->is.formal_arg.child ;
        tmp = 1 ;
        while(tmp > 0) {
          count = 0 ;
          tmp = check_arg_type( child , arg, pre, &count ) ; 
          if( tmp <= 0 ) farg = farg->next ;
          else {
            for( i = 0 ; i < count ; i++ ) arg = arg->next ;
            *location = *location + count ;
            ret = 1 ;
          }
        }
        break ;
      case JD_one_or_more_arg_statement:
        child = farg->is.formal_arg.child ;
        tmp = 1 ;
        one_detected = 0 ;
        while(tmp > 0) {
          count = 0 ;
          tmp = check_arg_type( child , arg, pre,  &count ) ; 
          if( tmp <= 0 ) {
            if( one_detected ) {
              farg = farg->next ;
            }
            else {
              return -1 ;
            }
          }
          else {
            for( i = 0 ; i < count ; i++ ) arg = arg->next ;
            *location = *location + count ;
            ret = 1 ;
            one_detected = 1 ;
          }
        }
        break ;
      case JD_wildcard_arg_statement:
        arg = arg->next ;
        farg = farg->next ;
        *location = *location + 1 ;
        ret = 1 ;
        break ;
      case JD_vardeclare_statement:
        tmp = check_var_arg( farg, arg ) ;
        if ( !tmp ) return -1 ;
        farg = farg->next ;
        arg = arg->next ;
        *location = *location + 1 ;
        ret = 1 ;
        break ;
      case JD_formal_arg_statement:
        tmp = 
          formal_dtype_check( 
            farg->is.formal_arg.data_type,
            arg, pre,
            1
          ) ;
        if ( !tmp ) return -1 ;
        farg = farg->next ;
        arg = arg->next ;
        *location = *location + 1 ;
        ret = 1 ;
        break ;
      default:
        return -1 ;
        break ;  
    }
  }
  if( !arg_end_check( farg ) ) {
    return -2 ;
  }
  return 1 ;
}

void actual_arg_check ( 
  named_node *func,
  expression_node *arg,
  expression_node *pre,
  char *filename,
  int linenum
) 
{
  int i ;
  int n = 0 ;
  int ret ;
  statement_node *farg ;
  
  farg = func->info.func.args ;
  
  ret = check_arg_type( farg, arg, pre, &n ) ;
  if( ret < 0 ) { 
    ERROR_LOCATION( filename, linenum ) ;
    if( ret == -1 ) {
      ERROR_DATA_TYPE_ERROR_DETECTED_ON_NTH_ARG(n) ;
    }
    else {
      ERROR_TOO_FEW_ARGUMENTS ; 
    }
  }
  else if ( ret == 0 && !arg_end_check(farg) ) {
    ERROR_LOCATION( filename, linenum ) ;
    ERROR_TOO_FEW_ARGUMENTS ; 
  }
  else {
    for( i = 0 ; i < n ; i++ ) arg = arg->next ;
    if( arg ) {
      ERROR_LOCATION( filename, linenum ) ;
      ERROR_TOO_MANY_ARGUMENTS ;        
    }
  }
  
}

/*
  -1 : wrong arg type at *nn
  0  : too few argment
  -2 : too many argument
  1  : OK
*/
int actual_arg_checking ( 
  named_node *func,
  expression_node *arg,
  expression_node *pre,
  int *nn
) 
{
  int i ;
  int n = 0 ;
  int ret ;
  statement_node *farg ;
  
  farg = func->info.func.args ;
  
  ret = check_arg_type( farg, arg, pre, &n ) ;
  *nn = n ;
  if( ret < 0 ) { 
    return ret ;
  }
  else if ( ret == 0 && !arg_end_check(farg) ) {
    return 0 ; 
  }
  else {
    for( i = 0 ; i < n ; i++ ) arg = arg->next ;
    if( arg ) {
      return -2 ;        
    }
  }
  return 1 ;
}

int var_dtype_check( 
  named_node *dtype, expression_node *data, int parent_flag, int str_flag 
)
{
  int tmp ;
  switch( dtype->type ) {
    case JD_void_type: return 0 ;
      break ;
    case JD_int_type:
    case JD_long_type:
      if( data->data_type == JD_exp_num_type || 
          data->data_type == JD_exp_bool_type  ) return 1 ;
      else return 0 ;
      break ;
    case JD_bit_type:
      if( data->data_type == JD_exp_num_type || 
          ( str_flag && data->data_type == JD_exp_string_type ) ||
          data->data_type == JD_exp_bool_type  ) return 1 ;
      else return 0 ;
      break ;
    case JD_signal_type:
      if( data->data_type == JD_exp_signal_type  ) return 1 ;
      else if( data->type == JD_null_exp ) return 1 ;
      else return 0 ;
      break ;
    case JD_string_type:
      if( data->data_type == JD_exp_string_type )  return 1 ;
      else if( data->type == JD_null_exp ) return 1 ;
      else return 0 ;
      break ;
    case JD_float_type:
    case JD_double_type:
      if( data->data_type == JD_exp_num_type ) return 1 ;
      else return 0 ;
      break ;
    case JD_class_name:
    case JD_sys_class_name:
      if( data->data_type == JD_exp_object_type ) {
        if( data->user_data_type == dtype ) return 1 ;
        else if( parent_flag &&
                 is_parent_class( data->user_data_type, dtype )  ) 
        {
          data->is.var.offset = 
            get_parent_class_offset( 
              data->user_data_type, dtype 
            )  ;
          return 1 ;
        }
        else if( parent_flag &&
                 is_parent_class( dtype, data->user_data_type )  ) 
        {
          /* get_parent_class offset works in both direction and
             returns negative number
           */
          data->is.var.offset = 
            get_parent_class_offset( 
              data->user_data_type, dtype
            )  ;
          return 1 ;
        }
        else return 0 ;
      }
      else if( data->data_type == JD_exp_new_type ) {
        named_node *mfunc ;
        
        if( dtype->is_abstract ) {
          ERROR_LOCATION(data->filename, data->linenum) ;
          ERROR_ILLEGAL_NEW_FUNCTION_CALL_TO_ABSTRUCT_CLASS ;
        }        
        mfunc = findmember_in_class( dtype, "new", &tmp ) ;
        if( mfunc ) {
          actual_arg_check(
            mfunc, data->is.new.args, NULL, data->filename, data->linenum
          ) ;
        }
        else {
          if( data->is.new.args != NULL ) {
            ERROR_LOCATION(data->filename, data->linenum) ;
            ERROR_NUMBER_OF_ARGUMENT_MISMATCH_ON_NEW_FUNCTION ;
            return 1 ;
          }
        }
        if( data->is.new.obj ) {
          if( !var_dtype_check( dtype, data->is.new.obj, 0, 0 ) ||
              is_array_exp_type(data->is.new.obj->data_type)
            )
          {
            ERROR_LOCATION(data->filename, data->linenum) ;
            ERROR_NEW_OBJECT_TYPE_ERROR ;
            return 1 ;
          }
          else {
            return 1 ;
          }
        }
        else return 1 ;
      }
      else if( data->data_type == JD_exp_new_block_type ) {
        /* new { <block }  is only for thread */
        if( strcmp( dtype->name, "thread" ) ) {
          ERROR_LOCATION(data->filename, data->linenum) ;
          ERROR_NEW_OBJECT_TYPE_ERROR ;
          return 1 ;
        }
        else return 1 ;
      }
      else if( data->type == JD_null_exp ) return 1 ;
      else return 0 ;
      break ;
    case JD_enum_type:
      if( data->data_type == JD_exp_enum_type && 
          data->user_data_type == dtype ) return 1 ;
      else return 0 ;
      break ;
    default: return 0 ;
  
  }

}

/* Add more strict matching rule TBI */
int formal_dtype_check( 
  named_node *dtype, expression_node *data, expression_node *pre, 
  int parent_flag 
)
{
  int tmp ;
  switch( dtype->type ) {
    case JD_void_type: return 0 ;
    case JD_int_type:
    case JD_long_type:
    case JD_bit_type:
      if( data->data_type == JD_exp_num_type ) return 1 ;
      else return 0 ;
    case JD_signal_type:
      if( data->data_type == JD_exp_num_type ) return 1 ;
      else if ( data->data_type == JD_exp_new_type ) {
        return is_port_expression( data->is.new.obj ) ;
      }
      else return 0 ;
    case JD_string_type:
      if( data->data_type == JD_exp_string_type )  return 1 ;
      else return 0 ;
      break ;
    case JD_float_type:
    case JD_double_type:
      if( data->data_type == JD_exp_num_type ) return 1 ;
      else return 0 ;
    case JD_class_name:
      if( data->data_type == JD_exp_object_type ) {
        if( data->user_data_type == dtype ) return 1 ;
        else if( parent_flag &&
                 is_parent_class( data->user_data_type, dtype )  ) return 1 ;
        else if( parent_flag &&
                 is_parent_class( dtype, data->user_data_type )  ) return 1 ;
        else return 0 ;
      }
      else if( data->data_type = JD_exp_new_type ) {
        named_node *mfunc = findmember_in_class( dtype, "new", &tmp ) ;
        if( mfunc ) {
          actual_arg_check(
            mfunc, data->is.new.args, NULL, data->filename, data->linenum
          ) ;
        }
        else {
          if( data->is.new.args != NULL ) {
            ERROR_LOCATION(data->filename, data->linenum) ;
            ERROR_NUMBER_OF_ARGUMENT_MISMATCH_ON_NEW_FUNCTION ;
            return 1 ;
          }
        }
        if( data->is.new.obj ) {
          if( !var_dtype_check( dtype, data->is.new.obj, 0, 0 ) ) {
            ERROR_LOCATION(data->filename, data->linenum) ;
            ERROR_NEW_OBJECT_TYPE_ERROR ;
            return 1 ;
          }
          else {
            return 1 ;
          }
        }
        else return 1 ;
      }
      else return 0 ;
      break ;
    case JD_enum_type:
      if( data->data_type == JD_exp_enum_type && 
          data->user_data_type == dtype ) return 1 ;
      else return 0 ;
      break ;
    case JD_same_type:  /* old scheme */
    case JD_class_type:
      { 
        named_node *ptype ;
        if( pre ) {
          ptype = pre->is.var.name->data_type ;
          return formal_dtype_check( ptype, data, NULL, 0 ) ;
        }
        else return 0 ;
      }
      break ;
    default: return 0 ;
  
  }

}

int is_array_exp_type( JD_exp_data_type type ) {
  int ret = 0 ;
  
  switch( type ) {
    case JD_exp_array_type:
    case JD_exp_marray_type:
    case JD_exp_assoc_type:
    case JD_exp_stridx_type:
    case JD_exp_list_type:
      ret = 1 ;
  }
  
  return ret ;
  
}

int var_name_dtype_check( 
  named_node *dtype1, named_node *dtype2
)
{
  if( dtype1->type != dtype2->type ) return 0 ;
  switch( dtype1->type ) {
    case JD_void_type: return 0 ;
    case JD_int_type:
    case JD_long_type:
    case JD_bit_type:
    case JD_signal_type:
    case JD_string_type:
    case JD_float_type:
    case JD_double_type:
    case JD_bool_type:
      return 1 ;
      break ;
    case JD_object_type:
    case JD_class_name:
    case JD_enum_type:
      if( dtype1 == dtype2 ) return 1 ;
      else return 0 ;
      break ;
    default: 
      return 0 ;
      break ;
  }

}

int array_type_check( 
  expression_node *range1, expression_node *range2
)
{
  if( range1->type != range2->type ) return 0 ;
  switch( range1->type ) {
    case JD_single_range_exp:
      if( range1->is.range.num_index != range2->is.range.num_index ) return 0 ;
      while( range1 ) {
        if( range2 == NULL ) return 0 ;
        if( range1->is.range.int_ub != range2->is.range.int_ub ) return 0 ;
        range1 = range1->next ;
        range2 = range2->next ;
      }
      if( range2 ) return 0 ;
      return 1 ;
      break ;
    case JD_stridx_array_exp:
    case JD_assoc_array_exp:
    case JD_list_array_exp:
    case JD_list_assoc_array_exp:
      return 1 ;
      break ;
    default: 
      return 0 ;
      break ;
  }

}

/* simple type check to expression */
int check_data_type( 
  expression_node *dtype, expression_node *data, int parent_flag 
)
{
  int ret ;
  
  if( is_array_exp_type(data->data_type) ) return 0 ;
  if( dtype->type != JD_varport_exp ) return 0 ;
  switch( dtype->is.var.name->type ) {
    case JD_port_name:
      if( data->data_type == JD_exp_num_type ) return 1 ;
      else return 0 ;  /* only value can be assign to port */
      break ;
    case JD_var_name:
    case JD_mvar_name:
    case JD_arg_name:
        ret = var_dtype_check( 
                dtype->is.var.name->data_type, data, parent_flag, 0 
              ) ;
        return ret ;
        break ;
      default:
        return 0 ;
        break ;
  }
  
}


/* Check assignment data is OK */
/* assignment can be on the array structure directly, thus data check
   must be sure if the sub_types match.
*/
int check_assign_data_type( 
  expression_node *lvalue, expression_node *data, int parent_flag 
)
{
  int ret ;
  
  if( lvalue->type == JD_varport_exp ) {
    if( !is_array_exp_type(lvalue->data_type) ) {
      if( is_array_exp_type(data->data_type) ) return 0 ;
      switch( lvalue->is.var.name->type ) {
        case JD_port_name:
          if( data->data_type == JD_exp_num_type ) {
            if( lvalue->is.var.depth == NULL ) {
              ERROR_LOCATION( lvalue->filename, lvalue->linenum ) ;
              ERROR_PORT_WITHOUT_DEPTH_USED_IN_LEFT_SIDE_OF_ASSIGN(
                lvalue->is.var.name->name
              ) ; 
            }
            else if( lvalue->is.var.depth->is.int_value > 0 ) {
              ERROR_LOCATION( lvalue->filename, lvalue->linenum ) ;
              ERROR_ILLEGAL_VALUE_FOR_DEPTH ; 
              return 0 ;
            }
            return 1 ;
          }
          else return 0 ;  /* only value can be assign to port */
          break ;
        case JD_var_name:
        case JD_mvar_name:
        case JD_arg_name:
          if( lvalue->is.var.depth ) {
            if( data->data_type == JD_exp_num_type ) return 1 ;
            else return 0 ;  /* only value can be assign to port */
          }
          else {
            named_node *variable = lvalue->is.var.name ;
            ret = var_dtype_check( 
                  lvalue->is.var.name->data_type, data, parent_flag, 1 
                ) ;
            if( ret && variable->data_type->type == JD_bit_type &&
                data->data_type == JD_exp_string_type ) 
            {
              /* assign string to bit case, we don't support subbit */
              if( lvalue->is.var.range ) {
                if( variable->sub_type == JD_single_sub_type ) return 0 ;
                else if( lvalue->is.var.range->is.range.bit_index ) return 0 ;
                else return 1 ;
              }
            }
            return ret ;
          }
          break ;
        case JD_same_type:  /* old scheme */
        case JD_class_type:
          if( lvalue->is.var.pre ) 
            ret = 
              check_assign_data_type( lvalue->is.var.pre, data, 0 ) ;
          else ret = 0 ;
          return ret ;
          break ;
        default:
          return 0 ;
          break ;
      }
    }
    else {
      /* is array */
      if( lvalue->data_type != data->data_type ) return 0 ;
      switch( data->type ) {
        case JD_varport_exp:
          ret = 
            var_name_dtype_check( 
              lvalue->is.var.name->data_type, data->is.var.name->data_type 
            ) ;
          ret &= 
            array_type_check(
              lvalue->is.var.name->info.var.range, 
              data->is.var.name->info.var.range
            ) ;
          return ret ;
          break ;
        case JD_list_exp:
          if( data->type == JD_varport_exp ) {
            /* if data is variable, check the data type */
            if( !check_data_type( lvalue, data, 0 ) ) return 0 ;
            else return 1 ;
          }
          else {
            /* if data is list, extract one element and check the type */
            data = data->is.expression ;
            while( data ) {
              if( !check_data_type( lvalue, data, 0 ) ) return 0 ;
              data = data->next ;
            }
            return 1 ;
          }
          break ;
        case JD_function_call_exp:
          ret = 
            var_name_dtype_check( 
              lvalue->is.var.name->data_type, 
              data->is.var.name->data_type 
            ) ;
          ret &= 
            array_type_check(
              lvalue->is.var.name->info.var.range, 
              data->is.var.name->data_type->info.var.range
            ) ;
          return ret ;
          break ;
        default:
          return 0 ;
          break ;
      }
    }
  }
  /* list not supported for now */
  else {  /* list TBI */
    return 0 ;
  }
  
}


void set_assign_data_type_offset( 
  expression_node *lvalue, expression_node *data 
)
{
  
  if( lvalue->type == JD_varport_exp ) {
    if( !is_array_exp_type(lvalue->data_type) &&
        !is_array_exp_type(data->data_type)      )
    {
      switch( lvalue->is.var.name->type ) {
        case JD_var_name:
        case JD_mvar_name:
        case JD_arg_name:
          if( !lvalue->is.var.depth && 
               lvalue->is.var.name->data_type->type == JD_class_name && 
               data->data_type == JD_exp_object_type &&
               data->user_data_type != lvalue->is.var.name->data_type 
            )
          {
            data->is.var.offset = 
              get_parent_class_offset( 
                data->user_data_type, lvalue->is.var.name->data_type 
              )  ;
          }
          break ;
        default:
          break ;
      }
    }
  }
}

int check_name_data_type( 
  named_node *dtype, named_node *ptype, expression_node *data, int parent_flag 
)
{
  int ret ;
  
  if( dtype->sub_type == JD_single_sub_type ) {
    if( is_array_exp_type(data->data_type) ) return 0 ;
    switch( dtype->type ) {
      case JD_same_type:  /* old scheme */
      case JD_class_type:
        if( ptype )
          ret = check_name_data_type( ptype, NULL, data, 0 );
        else ret = 0 ;
        return ret ;
        break ;
      default:
        ret = var_dtype_check( 
                dtype, data, parent_flag, 0 
              ) ;
        return ret ;
        break ;
    }
  }
  else {
    /* is array */
    if( !is_array_exp_type(data->data_type) ) return 0 ;
    switch( data->type ) {
      case JD_varport_exp:
        ret = 
          var_name_dtype_check( 
            dtype->data_type, data->is.var.name->data_type 
          ) ;
        ret &= 
          array_type_check(
            dtype->info.var.range, 
            data->is.var.name->info.var.range
          ) ;
        return ret ;
        break ;
      case JD_list_exp:
        if( data->type == JD_varport_exp ) {
          /* if data is variable, check the data type */
          ret = 
            var_name_dtype_check( 
              dtype->data_type, data->is.var.name->data_type 
            ) ;
          ret &= 
            array_type_check(
              dtype->info.var.range, 
              data->is.var.name->info.var.range
            ) ;
          return ret ;
        }
        else {
          switch( dtype->type ) {
            case JD_port_name:
              if( data->data_type == JD_exp_num_type ) return 1 ;
              else return 0 ;  /* only value can be assign to port */
              break ;
            case JD_var_name:
            case JD_mvar_name:
            case JD_arg_name:
              ret = var_dtype_check( 
                  dtype, data, parent_flag, 0 
                ) ;
              return ret ;
              break ;
            default:
              return 0 ;
              break ;
          }
          break ;
        default:
          return 0 ;
          break ;
      }
    }
  }
  
}

/* checking the lvalue type and data for list assignment
   '{a,b,c} = data ;
   This assignment is only apply to the bit, int form
 */
int check_assign_list_data_type(
  expression_node *lvalue, expression_node *data 
)
{
  expression_node *exp = lvalue->is.expression ;
  if( data->data_type != JD_exp_num_type ) return 0 ; 
  while( exp ) {
    if( exp->type == JD_varport_exp ) {
      if( is_array_exp_type(exp->data_type) ) return 0 ;
      switch( exp->is.var.name->type ) {
        case JD_port_name:
          if( exp->is.var.depth == NULL ) {
            ERROR_LOCATION( lvalue->filename, lvalue->linenum ) ;
            ERROR_PORT_WITHOUT_DEPTH_USED_IN_LEFT_SIDE_OF_ASSIGN(
              exp->is.var.name->name
            ) ; 
            return 0 ;
          }
          else if( exp->is.var.depth->is.int_value > 0 ) {
            ERROR_LOCATION( lvalue->filename, lvalue->linenum ) ;
            ERROR_ILLEGAL_VALUE_FOR_DEPTH ; 
            return 0 ;
          }
          break ;
        case JD_var_name:
        case JD_mvar_name:
        case JD_arg_name:
          if( exp->is.var.depth == NULL ) {
            switch(exp->is.var.name->data_type->type) {
              case JD_int_type:
              case JD_long_type:
              case JD_bit_type:
                break ; ;
              case JD_signal_type:
                ERROR_LOCATION( lvalue->filename, lvalue->linenum ) ;
                ERROR_PORT_WITHOUT_DEPTH_USED_IN_LEFT_SIDE_OF_ASSIGN(
                  exp->is.var.name->name
                ) ; 
                return 0 ;
              default:
                return 0 ;
            }
            break ;
          }
          else {
            if(exp->is.var.name->data_type->type!=JD_signal_type) 
              return 0 ;
          }
      } /* end of switch( exp->is.var.name->type ) */
    } /* end of if ( exp->type == JD_varport_exp ) */
    else return 0 ;
    exp = exp->next ;
  } /* end of while */
  return 1 ;
}

int is_initial_expression( expression_node *data ) {
  
  switch( data->type ) {
  case JD_this_exp:
    return 1 ;
    break ;
  case JD_new_exp:
    if( data->is.new.obj ) return 0 ;
    else return 1 ;
    break ;
  case JD_exp_new_block_type:
    return 0 ;
    break ;
  case JD_varport_exp:
    /*
    if( data->is.var.name->type == JD_port_name ) return 1 ;
    else return 0 ;
    */
    return 1 ;
    break ;
  case JD_function_call_exp:
    /* Is this OK? see if what's gonna happen */
    return 1 ;    
    break ;
  case JD_preincrement_exp:
  case JD_predecrement_exp:
  case JD_postincrement_exp:
  case JD_postdecrement_exp:
    return 1 ;
    break ;
  case JD_concatenation_exp:
    {
      expression_node *exp = data->is.expression ;
      while(exp) {
        if( !is_initial_expression(exp) ) return 0 ;
        exp = exp->next ;
      }
      return 1 ;
    }
    break ;
  case JD_duplication_exp:
    if( !is_initial_expression( data->is.binary.left ) )
      return 0 ;
    return is_initial_expression( data->is.binary.right ) ;
    break ;
  case JD_int_num:
  case JD_bit_num:
  case JD_null_exp:
  case JD_X_num: 
  case JD_real_num:
  case JD_string_exp:       
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
                        /* expression '~&' expression  */
  case JD_exp_nor_exp:     /* expression '|~' expression  */
                        /* expression '~|' expression  */
  case JD_exp_neor_exp:      /* expression '^~' expression  */
                        /* expression '~^' expression  */
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
  case JD_exp_Qne_exp:       /* expression '!?=' expression */
    if( !is_initial_expression( data->is.binary.left ) )
      return 0 ;
    return is_initial_expression( data->is.binary.right ) ;
    break ;
  case JD_u_minus_exp:     /* '-' expression              */
  case JD_u_tilde_exp:     /* '~' expression              */
  case JD_u_not_exp:  /* '!' expression              */
  case JD_u_and_exp:  /* '&' expression              */
  case JD_u_or_exp:   /* '|' expression              */
  case JD_u_eor_exp:  /* '^' expression              */
  case JD_u_nand_exp: /* '~&' expression             */
                        /* '&~' expression             */
  case JD_u_nor_exp:  /* '~|' expression             */
                        /* '|~' expression             */
  case JD_u_neor_exp: /* '^~' expression             */
                        /* '~^' expression             */
    return is_initial_expression( data->is.unary.this ) ;
    break ;
  case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
    return 0 ;
    break ;
  case JD_conditional_exp:   /*  '?' exp ':' exp            */
                        /* '(' expression ')'          */
    if( !is_initial_expression( data->is.trinary.first ) )
      return 0 ;
    if( !is_initial_expression( data->is.trinary.second ) )
      return 0 ;
    return is_initial_expression( data->is.trinary.third ) ;
    break ;
  case JD_port_exp:
    return 0 ;
    break ;
  case JD_rnd_exp:
  case JD_cyc_exp:
    {
      expression_node *exp ;
      exp = data->is.rnd.cond ;
      while( exp ) {
        if( !is_initial_expression( exp->is.rnd_cond.exp ) )
          return 0 ;
        exp = exp->is.rnd_cond.nxt ;
      }
    }
    return 1 ;
    break ;
  default:
    return 0 ;
    break ;
  }


}

int check_init_data_type( 
  named_node *dtype, expression_node *data, int parent_flag 
)
{
  int ret ;
  
  if( dtype->sub_type == JD_single_sub_type ) {
    if( is_array_exp_type(data->data_type) ) return 0 ;
    switch( dtype->type ) {
      case JD_same_type:  /* old scheme */
      case JD_class_type:
        return 0 ;
        break ;
      default:
        ret = var_dtype_check( 
                dtype, data, parent_flag, 1 
              ) ;
        if( !ret ) return 0 ;
        
        return is_initial_expression( data ) ;
        break ;
    }
  }
  else return 0 ; /* is array */
  
}

/* ?? not used */
int check_list_type( expression_node *exp1, expression_node *exp2 ) {
  int ret ;
  
  if( exp1->type != exp2->type ) return 0 ;
  switch( exp1->is.var.name->type ) {
    case JD_port_name:
      if( exp1->is.var.name->type == JD_port_name ) return 1 ;
      else return 0 ;  /* only value can be assign to port */
      break ;
    case JD_var_name:
    case JD_mvar_name:
      ret = var_name_dtype_check( 
              exp1->is.var.name->data_type, exp2->is.var.name->data_type
            ) ;
      return ret ;
      break ;
    default:
      return 0 ;
      break ;
  
  }
}

int is_func_name( named_node *name ) {
  if( name->type == JD_func_name || name->type == JD_mfunc_name ) return 1 ;
  else return 0 ;
}

int is_var_name( named_node *name ) {
  if( name->type == JD_arg_name || 
      name->type == JD_var_name || 
      name->type == JD_mvar_name || 
      name->type == JD_enum_member_name ||
      name->type == JD_port_name    ) return 1 ;
  else return 0 ;
}

void set_var_type( 
  expression_node *var, expression_node *range, expression_node *depth
) 
{
  named_node *data_type ;
  
  if(var->is.var.name->type == JD_port_name ) {
    if( depth )
      var->data_type = JD_exp_num_type ;
    else
      var->data_type = JD_exp_signal_type ;
  }
  else if(var->is.var.name->type == JD_portset_name ) {
    var->data_type = JD_exp_portset_type ;
  }
  else if(var->is.var.name->type == JD_aspect_name ) {
    var->data_type = JD_exp_aspect_type ;
  }
  else if( var->is.var.name->sub_type == JD_single_sub_type || range ) 
  {
    /* if range is specified, its data type is based on the variable type */
    data_type = var->is.var.name->data_type ;
    switch( data_type->type ) {
      case JD_void_type:
        var->data_type = JD_exp_void_type ;
        break ;
      case JD_int_type:
      case JD_long_type:
      case JD_float_type:
      case JD_double_type:
      case JD_bit_type:
        var->data_type = JD_exp_num_type ;
        break ;
      case JD_bool_type:
        var->data_type = JD_exp_bool_type ;
        break ;
      case JD_class_name:
      case JD_sys_class_name:
        var->data_type = JD_exp_object_type ;
        var->user_data_type = data_type ;
        break ;
      case JD_string_type:
        var->data_type = JD_exp_string_type ;
        break ;
      case JD_signal_type:
        if( depth )
          var->data_type = JD_exp_num_type ;
        else
          var->data_type = JD_exp_signal_type ;
        break ;
      case JD_enum_type:
        var->data_type = JD_exp_enum_type ;
        var->user_data_type = data_type ;
        break ;
      default:
        var->data_type = JD_exp_unknown_type ;
        break ;
    }
  }
  else {
    switch( var->is.var.name->sub_type ) {
      case JD_array_sub_type:
        var->data_type = JD_exp_array_type ;
        break ;
      case JD_multi_array_sub_type:
        var->data_type = JD_exp_marray_type ;
        break ;
      case JD_assoc_sub_type:
        var->data_type = JD_exp_assoc_type ;
        break ;
      case JD_stridx_sub_type:
        var->data_type = JD_exp_stridx_type ;
        break ;
      case JD_list_sub_type:
        var->data_type = JD_exp_list_type ;
        break ;
      default:
        var->data_type = JD_exp_unknown_type ;
        break ;
    }
  }

}

int check_cond_data_type( expression_node *exp ) {
  switch( exp->data_type ) {
    case JD_exp_void_type:
      return 0 ;
      break ;
    case JD_exp_num_type:
    case JD_exp_bool_type:
    case JD_exp_object_type:
    case JD_exp_signal_type:
    case JD_exp_string_type:
      return 1 ;
      break ;
    case JD_exp_enum_type:
      return 1 ;
      break ;
    case JD_exp_array_type:
    case JD_exp_marray_type:
    case JD_exp_assoc_type:
    case JD_exp_stridx_type:
    case JD_exp_list_type:
      return 0 ;
      break ;
    default:
      return 0 ;
      break ;
  }
}

JD_exp_data_type  get_data_type( named_node *data_type ) {
  switch( data_type->type ) {
    case JD_void_type:
      return JD_exp_void_type ;
      break ;
    case JD_int_type:
    case JD_long_type:
    case JD_float_type:
    case JD_double_type:
    case JD_bit_type:
      return JD_exp_num_type ;
      break ;
    case JD_bool_type:
      return JD_exp_bool_type ;
      break ;
    case JD_class_name:
    case JD_sys_class_name:
      return JD_exp_object_type ;
      break ;
    case JD_string_type:
      return JD_exp_string_type ;
    case JD_signal_type:
      return JD_exp_signal_type ;
      break ;
    case JD_enum_type:
      return JD_exp_enum_type ;
      break ;
    default:
      return JD_exp_unknown_type ;
      break ;
  }
}


void set_func_type( 
  expression_node *func_exp
) 
{
  named_node *func, *data_type ;
  
  func = func_exp->is.var.name ;
  if( func->data_type->sub_type == JD_single_sub_type ) 
  {
    data_type = func->data_type ;
    
    switch( data_type->type ) {
      case JD_class_name:
      case JD_sys_class_name:
      case JD_enum_type:
        func_exp->data_type = get_data_type( data_type ) ;
        func_exp->user_data_type = data_type ;
        break ;
      case JD_same_type:  /* old scheme */
      case JD_class_type:
        data_type = func_exp->is.var.pre->is.var.name->data_type ;
        func_exp->data_type = get_data_type( data_type ) ;
        func_exp->user_data_type = data_type ;
        break ;
      default:
        func_exp->data_type = get_data_type( data_type ) ;
        break ;
    }
  }
  else {
    data_type = func->data_type ;
    switch( data_type->sub_type ) {
      case JD_array_sub_type:
        func_exp->data_type = JD_exp_array_type ;
        break ;
      case JD_multi_array_sub_type:
        func_exp->data_type = JD_exp_marray_type ;
        break ;
      case JD_assoc_sub_type:
        func_exp->data_type = JD_exp_assoc_type ;
        break ;
      case JD_stridx_sub_type:
        func_exp->data_type = JD_exp_stridx_type ;
        break ;
      case JD_list_sub_type:
        func_exp->data_type = JD_exp_list_type ;
        break ;
      default:
        func_exp->data_type = JD_exp_unknown_type ;
        break ;
    }
  }

}

void set_func_type_of_func( 
  expression_node *func_exp, named_node *func
) 
{
  named_node *data_type ;
  
  if( func->data_type->sub_type == JD_single_sub_type ) 
  {
    data_type = func->data_type ;
    
    switch( data_type->type ) {
      case JD_class_name:
      case JD_sys_class_name:
      case JD_enum_type:
        func_exp->data_type = get_data_type( data_type ) ;
        func_exp->user_data_type = data_type ;
        break ;
      case JD_same_type:  /* old scheme */
      case JD_class_type:
        data_type = func_exp->is.var.pre->is.var.name->data_type ;
        func_exp->data_type = get_data_type( data_type ) ;
        func_exp->user_data_type = data_type ;
        break ;
      default:
        func_exp->data_type = get_data_type( data_type ) ;
        break ;
    }
  }
  else {
    data_type = func->data_type ;
    switch( data_type->sub_type ) {
      case JD_array_sub_type:
        func_exp->data_type = JD_exp_array_type ;
        break ;
      case JD_multi_array_sub_type:
        func_exp->data_type = JD_exp_marray_type ;
        break ;
      case JD_assoc_sub_type:
        func_exp->data_type = JD_exp_assoc_type ;
        break ;
      case JD_stridx_sub_type:
        func_exp->data_type = JD_exp_stridx_type ;
        break ;
      case JD_list_sub_type:
        func_exp->data_type = JD_exp_list_type ;
        break ;
      default:
        func_exp->data_type = JD_exp_unknown_type ;
        break ;
    }
  }

}


/* ARGSUSED */
int abstract_arg_check( statement_node *args, int flag ) {
  
  /* Don't check, Allow all the combination
  while( args ) {
    switch( args->type ) {
      case JD_vardeclare_statement:
        if( flag == 2 ) return 0 ;
        flag = 1 ;
        break ;
      case JD_formal_arg_statement:
        if( flag == 1 ) return 0 ;
        flag = 2 ;
        break ;
      case JD_one_or_more_arg_statement:
        if( flag == 1 ) return 0 ;
        flag = 2 ;
        if( !abstract_arg_check( args->is.formal_arg.child, 2 ) ) return 0 ;
        break ;
      case JD_zero_or_more_arg_statement:
        if( flag == 1 ) return 0 ;
        flag = 2 ;
        if( !abstract_arg_check( args->is.formal_arg.child, 2 ) ) return 0 ;
        break ;
      case JD_wildcard_arg_statement:
        if( flag == 1 ) return 0 ;
        flag = 2 ;
        break ;
      case JD_or_of_args_statement:
        if( flag == 1 ) return 0 ;
        flag = 2 ;
        break ;
      default:
        return 0 ;
        break ;
    }
    args = args->next ;
  }
  */
  
  return 1 ;
}

int is_abstract_arg( statement_node *args ) {
  
  while( args ) {
    if( args->type != JD_vardeclare_statement ) return 1 ;
    args = args->next ;
  }
  return 0 ;
}

int is_int_data_type( named_node *data_type ) {
  switch( data_type->type ) {
  case JD_void_type:
  case JD_long_type:
  case JD_float_type:
  case JD_double_type:
    return 0 ;
    break ;
  case JD_int_type:
  case JD_bit_type:
    if( data_type->sub_type == JD_single_sub_type )
      return 1 ;
    else
      return 0 ;
    break ;
  default:
    return 0 ;
    break ;

  }
}

int is_int_expression( expression_node *data ) {
  named_node *data_type ;
  
  if( data->data_type != JD_exp_num_type ) return 0 ;
  switch( data->type ) {
  case JD_varport_exp:
    /* check if varport is port or int/bit var */
    data_type = data->is.var.name->data_type ;
    return is_int_data_type( data_type ) ;
    break ;
  case JD_function_call_exp:
    /* check if function type is int/bit */
    data_type = data->is.var.name->data_type ;
    return is_int_data_type( data_type ) ;    
  case JD_preincrement_exp:
  case JD_predecrement_exp:
  case JD_postincrement_exp:
  case JD_postdecrement_exp:
    return is_int_expression( data->is.expression ) ;
    break ;
  case JD_concatenation_exp:
  case JD_duplication_exp:
    return 1 ;
    break ;
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
                        /* expression '~&' expression  */
  case JD_exp_nor_exp:     /* expression '|~' expression  */
                        /* expression '~|' expression  */
  case JD_exp_neor_exp:      /* expression '^~' expression  */
                        /* expression '~^' expression  */
    if( !is_int_expression( data->is.binary.left ) )
      return 0 ;
    return is_int_expression( data->is.binary.right ) ;
    break ;
  case JD_exp_land_exp:      /* expression '&&' expression  */
  case JD_exp_lor_exp:     /* expression '||' expression  */
    return 1 ;
    break ;
  case JD_exp_rshift_exp:    /* expression '>>' expression  */
  case JD_exp_urshift_exp:   /* expression '>>>' expression */
  case JD_exp_lshift_exp:    /* expression '<<' expression  */
    return is_int_expression( data->is.binary.left ) ;
    break ;
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
    return 1 ;
    break ;
  case JD_u_minus_exp:     /* '-' expression              */
  case JD_u_tilde_exp:     /* '~' expression              */
  case JD_u_not_exp:  /* '!' expression              */
  case JD_u_and_exp:  /* '&' expression              */
  case JD_u_or_exp:   /* '|' expression              */
  case JD_u_eor_exp:  /* '^' expression              */
  case JD_u_nand_exp: /* '~&' expression             */
                        /* '&~' expression             */
  case JD_u_nor_exp:  /* '~|' expression             */
                        /* '|~' expression             */
  case JD_u_neor_exp: /* '^~' expression             */
                        /* '~^' expression             */
    return is_int_expression( data->is.unary.this ) ;
    break ;
  case JD_eqCeq_exp:  /* exp '=:=' exp ':' exp       */
    return 1 ;
    break ;
  case JD_conditional_exp:   /*  '?' exp ':' exp            */
                        /* '(' expression ')'          */
    if( !is_int_expression( data->is.trinary.second ) )
      return 0 ;
    return is_int_expression( data->is.trinary.third ) ;
    break ;
  case JD_port_exp:
    return 1 ;
    break ;
  default:
    return 0 ;
    break ;
  }


}

int is_string_expression( expression_node *data ) {
  
  if( data->data_type == JD_exp_string_type ) return 1 ;
  else return 0 ;
  
}


void add_file_to_list( char *name ) {
  file_data *p, *s ;
  
  if( compiled_files == NULL ) {
    p = (file_data *)calloc(1,sizeof(file_data)) ;
    p->filename = (char *)strdup(name) ;
    compiled_files = p ;
    return ;
  }
  else {
    p = compiled_files ;
    while( p ) {
      if( !strcmp( p->filename, name ) ) return ;
      s = p ;
      p = p->next ;
    }
    p = (file_data *)calloc(1,sizeof(file_data)) ;
    p->filename = (char *)strdup(name) ;
    s->next = p ;
  }
}


int check_static_rnd( expression_node *top, expression_node *new ) {
  expression_node *rng0, *rng1 ;
  
  while( top ) {
    if( new->is.rnd.name && top->is.rnd.name && 
        !strcmp(new->is.rnd.name, top->is.rnd.name ) )
    {
      if( top->type != new->type ) return 0 ;
      if( new->is.rnd.range && top->is.rnd.range ) {
        rng0 = top->is.rnd.range ;
        rng1 = new->is.rnd.range ;
        while( rng0 ) {
          if( !rng1 ) {
            return 0 ;
          }
          if( rng0->type != rng1->type ) 
            return 0 ;
          if( rng0->is.rnd_range.weight != rng1->is.rnd_range.weight ) 
            return 0 ;
          if( rng0->is.rnd_range.weight_flag != 
                  rng1->is.rnd_range.weight_flag ) 
            return 0 ;
          if( !range_const_comp( rng0->is.rnd_range.ub, rng1->is.rnd_range.ub ) ) return 0 ;
          if( rng0->type == JD_rand_range_exp ) {
            if( !range_const_comp( rng0->is.rnd_range.lb, rng1->is.rnd_range.lb ) ) return 0 ;
          }
          rng0 = rng0->is.rnd_range.nxt ;
          rng1 = rng1->is.rnd_range.nxt ;
        }
      }
      else if( new->is.rnd.range && !top->is.rnd.range ) {
        top->is.rnd.range  = new->is.rnd.range ;
      }
      new->is.rnd.index = top->is.rnd.index ;
      return 1 ;
    }
    if( !top->is.rnd.nxt ) {
      top->is.rnd.nxt = new ;
      new->is.rnd.index = top->is.rnd.index + 1 ;
      top = NULL ;
    }
    else top = top->is.rnd.nxt ;
  }
  return 1 ;
}
