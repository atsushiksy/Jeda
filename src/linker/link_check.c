/***************************************************************************
 *
 *  link_check.c: name info check functions for Jeda linker
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
#include <stdio.h>
#include <stddef.h>
/* #include <errno.h>
#include <stdlib.h>
#include <time.h> */

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"
#include "vinst.tab.h"

extern int parse_sys_utils ;

int check_port_attribs( vcode_info *a, vcode_info *b ) {
  vcode_info *bb ;
  int flag ;
  if( !a && !b ) return 1 ;
  
  while( a ) {
    bb = b ;
    flag = 1 ;
    while( bb && flag ) {
      if( a->type == bb->type ) {
        switch( a->type ) {
          case JD_port_access_attrib:
            if( a->is.port_access_attrib.drive_sample == 
                bb->is.port_access_attrib.drive_sample    )
            {
              if( a->is.port_access_attrib.edge != 
                  bb->is.port_access_attrib.edge    ) {
                ERROR_PORT_EDGE_ATTRIB_MISMATCH ;
                return 0 ;
              }
              if( a->is.port_access_attrib.skew != 
                  bb->is.port_access_attrib.skew    ) {
                ERROR_PORT_SKEW_ATTRIB_MISMATCH ;
                return 0 ;
              }
              else flag = 0 ;
            }
            break ;
          case JD_port_depth_attrib:
            if( a->is.int_value != bb->is.int_value ) {
              ERROR_PORT_DEPTH_ATTRIB_MISMATCH ;
              return 0 ;
            }
            else flag = 0 ;
            break ;
          case JD_port_path_attrib:
            if( strcmp( a->is.string, bb->is.string ) ) {
              ERROR_PORT_PATH_ATTRIB_MISMATCH ;
              return 0 ;
            }
            else flag = 0 ;
            break ;
          case JD_port_clock_attrib:
            if( 
              ( a->is.port_clock_attrib.pset && !bb->is.port_clock_attrib.pset )
              ||
              ( bb->is.port_clock_attrib.pset && !a->is.port_clock_attrib.pset )
              ||
              ( a->is.port_clock_attrib.pset && bb->is.port_clock_attrib.pset && 
                strcmp(
                  a->is.port_clock_attrib.pset, bb->is.port_clock_attrib.pset 
                ) 
              )
              ||
              strcmp( 
                a->is.port_clock_attrib.port, bb->is.port_clock_attrib.port 
              ) 
              )
            {
              ERROR_PORT_CLOCK_ATTRIB_MISMATCH ;
              return 0 ;
            }
            else flag = 0 ;
            break ;
        }  
      }
      if( flag ) bb = bb->next ;
    } 
    if( !bb ) return 0 ; 
    a = a->next ;
  } 
  return 1 ;
}

int comp_data_type( vcode_info *a, vcode_info *b ) {

  if( a->is.data_type.type != b->is.data_type.type ) return 0 ;
  
  switch( a->is.data_type.type ) {
    case VI_bit:
      if( a->is.data_type.ub != b->is.data_type.ub ) return 0 ;
      if( a->is.data_type.lb != b->is.data_type.lb ) return 0 ;
      break ;
    case VI_class:
    case VI_enum:
      if( strcmp( a->is.data_type.name, b->is.data_type.name ) ) return 0 ;
      break ;
  }
  return 1 ;
}

int comp_array_range( vcode_info *a, vcode_info *b ) {
  vcode_info *ad, *bd ;
  if( !a && !b ) return 1 ;
  if( !a || !b ) return 0 ;
  if( a->is.array_range.type != b->is.array_range.type ) return 0 ;
  switch( a->is.array_range.type ) {
    case VI_array:
      if( a->is.array_range.size != b->is.array_range.size ) return 0 ;
      break ;
    case VI_marray:
      if( a->is.array_range.size != b->is.array_range.size ) return 0 ;
      ad = a->is.array_range.range_list ;
      bd = b->is.array_range.range_list ;
      while( ad && bd ) {
        if( ad->is.int_value != bd->is.int_value ) return 0 ;
        ad = ad->next ;
        bd = bd->next ;
      }
      if( ad || bd ) return 0 ;
      break ;
  }
  return 1 ;
}

int comp_attrib( vcode_info *a, vcode_info *b ) {
  if( a == NULL ) {
    if( b == NULL ) return 1 ;
    else {
      if( b->is.attrib.is_static ) return 0 ;
      if( b->is.attrib.is_clone ) return 0 ;
      if( b->is.attrib.is_local ) return 0 ;
      if( b->is.attrib.is_global ) return 0 ;
      if( b->is.attrib.is_protected ) return 0 ;
      if( b->is.attrib.is_final ) return 0 ;
      if( b->is.attrib.is_abstract ) return 0 ;
      return 1 ;
    }
  }
  if( b == NULL ) {
    if( a->is.attrib.is_static ) return 0 ;
    if( a->is.attrib.is_clone ) return 0 ;
    if( a->is.attrib.is_local ) return 0 ;
    if( a->is.attrib.is_global ) return 0 ;
    if( a->is.attrib.is_protected ) return 0 ;
    if( a->is.attrib.is_final ) return 0 ;
    if( a->is.attrib.is_abstract ) return 0 ;
    return 1 ;
  }
  if( a->is.attrib.is_static != b->is.attrib.is_static ) return 0 ;
  if( a->is.attrib.is_clone != b->is.attrib.is_clone ) return 0 ;
  if( a->is.attrib.is_local != b->is.attrib.is_local ) return 0 ;
  if( a->is.attrib.is_global != b->is.attrib.is_global ) return 0 ;
  if( a->is.attrib.is_protected != b->is.attrib.is_protected ) return 0 ;
  if( a->is.attrib.is_final != b->is.attrib.is_final ) return 0 ;
  if( a->is.attrib.is_abstract != b->is.attrib.is_abstract ) return 0 ;
  /* 
  if( a->is.attrib.is_extern != b->is.attrib.is_extern ) return 0 ;
  */
  if( a->is.attrib.is_export != b->is.attrib.is_export ) {
    a->is.attrib.is_export |= b->is.attrib.is_export ;
    b->is.attrib.is_export |= a->is.attrib.is_export ;
  }
  if( a->is.attrib.is_leaf != b->is.attrib.is_leaf ) {
    a->is.attrib.is_leaf |= b->is.attrib.is_leaf ;
    b->is.attrib.is_leaf |= a->is.attrib.is_leaf ;
  }
  return 1 ;
}

int comp_info( vcode_info *a, vcode_info *b ) {
  if( !a && !b ) return 1 ;
  if( a->type != b->type ) return 0 ;
  
  switch( a->type ) {
    case JD_port_info:
      if( a->is.port.direction != b->is.port.direction ) return 0 ;
      if( a->is.port.ub != b->is.port.ub ) return 0 ;
      if( a->is.port.lb != b->is.port.lb ) return 0 ;
      if( !check_port_attribs( a->is.port.attribs, b->is.port.attribs ) )
        return 0 ;
      return check_port_attribs( b->is.port.attribs, a->is.port.attribs ) ;
      break ;
    case JD_portset_info:
      return 1 ;
      break ;
    case JD_connection_info:
      return 0 ; /* should not be used */
      break ;
    case JD_variable_dcl:
      if( !comp_data_type( 
            a->is.variable_dcl.data_type, a->is.variable_dcl.data_type 
          ) 
        )  return 0 ;
      if( !comp_array_range( 
            a->is.variable_dcl.array_range, a->is.variable_dcl.array_range 
          ) 
        )  return 0 ;
      if( !comp_attrib( 
            a->is.variable_dcl.var_attrib, a->is.variable_dcl.var_attrib 
          ) 
        )  return 0 ;
      /* ?? I guess I don't need this. ??
      if( a->is.variable_dcl.class != b->is.variable_dcl.class ) return 0 ;
      */
      return 1 ;
      break ;
    case JD_enum_dcl:
      
      break ;
    case JD_function_info:
      break ;
    case JD_class_info:
      break ;
    case JD_veri_task_info:
      break ;
    case JD_static_var_info:
      break ;
    case JD_stat_init_code:
      break ;
    case JD_init_code:
      break ;
    case JD_class_init_code:
      break ;
    case JD_class_alloc_code:
      break ;
    case JD_mfunc_code:
      break ;
    case JD_func_code:
      break ;
    case JD_port_access_attrib:
      break ;
    case JD_port_depth_attrib:
      break ;
    case JD_port_path_attrib:
      break ;
    case JD_port_clock_attrib:
      break ;
    case JD_string_const:
      break ;
    case JD_bit_const:
      break ;
    case JD_int_const:
      break ;
    case JD_float_const:
      break ;    
  }
  return 0 ;
}

void comp_portset_info( vcode_info *a, JD_name *nmb  ) {
  vcode_info *pa ;
  JD_name *nmpb ;
  
  pa = a->is.portset.port_infos ;
  while( pa ) {
    nmpb = find_name_in_scope( pa->is.port.name, nmb->child ) ;
    if( !nmpb ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PREV_PORTSET_DCL( 
        pa->is.port.name, nmb->name
      ) ;
    }
    if( !comp_info( pa, nmpb->info ) ) {
      ERROR_PORTSET_PORT_NAME_ATTRIB_MISMATCH_IN_PREV_PORTSET_DCL( 
        pa->is.port.name, nmb->name
      ) ;    
    }
    pa = pa->next ;
  }
}

void comp_enum_info( vcode_info *a, JD_name *nmb  ) {
  vcode_info *pa, *pb ;
  
  pa = a->is.enum_dcl.members ;
  pb = nmb->info->is.enum_dcl.members ;
  
  while( pa ) {
    if( !pb ) {
      ERROR_NUMBER_OF_ENUM_MEMBER_MISMATCH( nmb->name ) ;
      return ;
    }
    if( strcmp( pa->is.string, pb->is.string ) ) {
      ERROR_ENUM_MEMBER_MISMATCH( nmb->name, pa->is.string, pb->is.string ) ;
      return ;
    }
    pa = pa->next ;
    pb = pb->next ;
  }
  if( pb ) {
    ERROR_NUMBER_OF_ENUM_MEMBER_MISMATCH( nmb->name ) ;
    return ;
  }
}

int check_enum_info( vcode_info *a, vcode_info *b  ) {
  vcode_info *pa, *pb ;
  
  pa = a->is.enum_dcl.members ;
  pb = b->is.enum_dcl.members ;
  
  while( pa ) {
    if( !pb ) {
      return 0 ;
    }
    if( strcmp( pa->is.string, pb->is.string ) ) {
      return 0 ;
    }
    pa = pa->next ;
    pb = pb->next ;
  }
  if( pb ) {
    return 0;
  }
  return 1 ;
}


void comp_func_info( char *class, vcode_info *a, vcode_info *b  ) {
  vcode_info *pa, *pb ;
  int n ;
  
  pa = a->is.function_info.func_options ;
  pb = b->is.function_info.func_options ;
  if( pa || pb ) {
    /*
    if( !pa || !pb ) {
      ERROR_FUNCTION_ATTRIBUTE_MISMATCH(
        class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
    */
    if( !comp_attrib(pa, pb) ) {
      ERROR_FUNCTION_ATTRIBUTE_MISMATCH(
        class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
  }
  pa = a->is.function_info.data_type ;
  pb = b->is.function_info.data_type ;
  if( !comp_data_type( pa, pb ) ) {
    ERROR_DATA_TYPE_OF_FUNCTION_MISMATCH(
      class?class:"", class?".":"", a->is.function_info.name
    ) ;
    return ;
  }
  pa = a->is.function_info.arguments ;
  pb = b->is.function_info.arguments ;
  n = 1 ;
  
  while( pa ) {
    if( !pb ) {
      ERROR_NUMBER_OF_ARGUMENT_IN_FUNCTION_MISMATCH(
        class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
    if( pa->is.argument.var != pb->is.argument.var ) {
      ERROR_NTH_ARGUMENT_VAR_TYPE_MISMATCH_IN_FUNCTION(
        n, class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_data_type( pa->is.argument.data_type, pb->is.argument.data_type) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_FUNCTION(
        n, class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_array_range( 
          pa->is.argument.array_range, pb->is.argument.array_range
        ) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_FUNCTION(
        n, class?class:"", class?".":"", a->is.function_info.name
      ) ;
      return ;
    }
    n++ ;
    pa = pa->next ;
    pb = pb->next ;
  }
  if( pb ) {
    ERROR_NUMBER_OF_ARGUMENT_IN_FUNCTION_MISMATCH(
      class?class:"", class?".":"", a->is.function_info.name
    ) ;
  }
}

int pointcut_match( vcode_info *a, vcode_info *b ) 
{
  if( !b ) return 0 ;
  if( a->type != b->type ) return 0 ;
  a = a->is.pointcut_info.next ;
  b = b->is.pointcut_info.next ;
  while( a ) {
    if( !b ) return 0 ;
    if( a->type != b->type ) return 0 ;
    if( strcmp( a->is.pointcut_info.name, b->is.pointcut_info.name ) ) 
      return 0 ;
    a = a->is.pointcut_info.next ;
    b = b->is.pointcut_info.next ;
  }
  if( b ) return 0 ;
  
  return 1 ;
}

void comp_advice_info( char *class, vcode_info *a, vcode_info *b  ) {
  vcode_info *pa, *pb ;
  int n ;
  
  pa = a->is.function_info.data_type ;
  pb = b->is.function_info.data_type ;
  if( !comp_data_type( pa, pb ) ) {
    ERROR_DATA_TYPE_OF_ADVICE_MISMATCH(
      class, a->is.function_info.name
    ) ;
    return ;
  }
  pa = a->is.function_info.arguments ;
  pb = b->is.function_info.arguments ;
  n = 1 ;
  
  while( pa ) {
    if( !pb ) {
      ERROR_NUMBER_OF_ARGUMENT_IN_ADVICE_MISMATCH(
        class, a->is.function_info.name
      ) ;
      return ;
    }
    if( pa->is.argument.var != pb->is.argument.var ) {
      ERROR_NTH_ARGUMENT_VAR_TYPE_MISMATCH_IN_ADVICE(
        n, class, a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_data_type( pa->is.argument.data_type, pb->is.argument.data_type) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_ADVICE(
        n, class, a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_array_range( 
          pa->is.argument.array_range, pb->is.argument.array_range
        ) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_ADVICE(
        n, class, a->is.function_info.name
      ) ;
      return ;
    }
    n++ ;
    pa = pa->next ;
    pb = pb->next ;
  }
  if( pb ) {
    ERROR_NUMBER_OF_ARGUMENT_IN_ADVICE_MISMATCH(
      class, a->is.function_info.name
    ) ;
  }

  pa = a->is.function_info.pointcut ;
  
  while( pa ) {
    pb = b->is.function_info.pointcut ;
    while( pb ) {
      if( pointcut_match( pa, pb ) ) break ;
      pb = pb->next ;
    }
    if( !pb ) {
      ERROR_POINTCUT_IN_ADVICE_MISMATCH(
        class, a->is.function_info.name
      ) ;
      return ;
    }
    pa = pa->next ;
  }
}



void comp_veri_task_info( vcode_info *a, vcode_info *b  ) {
  vcode_info *pa, *pb ;
  int n ;
  
  pa = a->is.function_info.arguments ;
  pb = b->is.function_info.arguments ;
  n = 1 ;
  
  while( pa ) {
    if( !pb ) {
      ERROR_NUMBER_OF_ARGUMENT_IN_FUNCTION_MISMATCH(
        "verilog", ".", a->is.function_info.name
      ) ;
      return ;
    }
    if( pa->is.argument.var != pb->is.argument.var ) {
      ERROR_NTH_ARGUMENT_VAR_TYPE_MISMATCH_IN_FUNCTION(
        n, "verilog", ".", a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_data_type( pa->is.argument.data_type, pb->is.argument.data_type) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_FUNCTION(
        n, "verilog", ".", a->is.function_info.name
      ) ;
      return ;
    }
    if( 
        !comp_array_range( 
          pa->is.argument.array_range, pb->is.argument.array_range
        ) 
      ) 
    {
      ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_FUNCTION(
        n, "verilog", ".", a->is.function_info.name
      ) ;
      return ;
    }
    n++ ;
    pa = pa->next ;
    pb = pb->next ;
  }
  if( pb ) {
    ERROR_NUMBER_OF_ARGUMENT_IN_FUNCTION_MISMATCH(
      "verilog", ".", a->is.function_info.name
    ) ;
  }
  
  if( strcmp( a->is.function_info.path, b->is.function_info.path ) ) {
    ERROR_VERILOG_TASK_PATH_MISMATCH(
      a->is.function_info.name, 
      a->is.function_info.path, b->is.function_info.path
    ) ;
  }
}

char *get_info_name( vcode_info *info ) {
  char *ret ;
  switch( info->type ) {
    case JD_port_info:
      ret = info->is.port.name ;
      break ;
    case JD_portset_info:
      ret = info->is.portset.name ;
      break ;
    case JD_variable_dcl:
      ret = info->is.variable_dcl.name ;
      break ;
    case JD_enum_dcl:
      ret = info->is.enum_dcl.name ;
      break ;
    case JD_function_info:
    case JD_advice_info:
      ret = info->is.function_info.name ;
      break ;
    case JD_class_info:
    case JD_aspect_info:
      ret = info->is.class.name ;
      break ;
    case JD_veri_task_info:
      ret = info->is.veri_task.name ;
      break ;
    default:
      ret = "" ;
      break ;
  
  }
  return ret ;
}
 
int is_info_abstract( vcode_info *info ) {
  int ret = 0 ;
  switch( info->type ) {
    case JD_function_info:
      if( info->is.function_info.func_options )
        ret = info->is.function_info.func_options->is.attrib.is_abstract ;
      break ;
    case JD_class_info:
      if( info->is.class.attribs )
        ret = info->is.class.attribs->is.attrib.is_abstract ;
      break ;
  }
  return ret ;
}

JD_name_type get_info_type( vcode_info *info ) {
  switch( info->type ) {
    case JD_port_info:
      return JD_port_name ;
      break ;
    case JD_portset_info:
      return JD_portset_name ;
      break ;
    case JD_variable_dcl:
      return JD_variable_name ;
      break ;
    case JD_enum_dcl:
      return JD_enum_name ;
      break ;
    case JD_advice_info:
      return JD_advice_name ;
      break ;
    case JD_aspect_info:
      return JD_aspect_name ;
      break ;
    case JD_function_info:
      if( info->is.function_info.class ) {
        if( info->is.function_info.sys_flag ) return JD_sys_mfunc_name ;
        else return JD_mfunc_name ;
      }
      else
        return JD_func_name ;
      break ;
    case JD_class_info:
        if( parse_sys_utils ) return JD_sys_class_name ;
        else return JD_class_name ;
      break ;
    case JD_veri_task_info:
      return JD_func_name ;
      break ;
  }
  return JD_unknown_name ;
}

void comp_member( char *class, vcode_info *a, vcode_info *b ) {
  if( a->type != b->type ) {
    ERROR_MEMBER_TYPE_MISMATCH_IN_CLASS( class, get_info_name(a) ) ;
    return ;
  }
  switch( a->type ) {
    case JD_enum_dcl:
      if( !check_enum_info( a, b ) ) {
        ERROR_ENUM_DEFINITION_MISMATCH_IN_CLASS( class, get_info_name(a) ) ;
      }
      break ;
    case JD_variable_dcl:
      if( !comp_info( a, b ) ) {
        ERROR_VARIABLE_DEFINITION_MISMATCH_IN_CLASS( class, get_info_name(a) ) ;
      }
      break ;
    case JD_function_info:
      comp_func_info( class, a, b ) ;
      break ;
    case JD_advice_info:
      comp_advice_info( class, a, b ) ;
      break ;
  }

}

void append_member( vcode_info *a, JD_name *bnm ) {
  vcode_info *b, *c ;
  JD_name *anm ;
  int *pp = NULL ;
  int x, y ;
  
  b = bnm->info ;
  x = 0 ;
  while( b->next ) {
    c = bnm->info ;
    for( y = 0 ; y < x ; y++ ) {
      c = c->next ;
      if( c == b ) {
        printf( "Error!! append_member found loop\n" ) ;
        *pp = 0 ; 
      }
    }
    if( b == a ) {
      printf( "Error!! append_member found duplicated member\n" ) ;
      *pp = 0 ; 
    }
    b = b->next ;
  }
  b = bnm->info ;
  while( b->next ) b = b->next ;
  b->next = a ;
  anm = add_name_to_scope( get_info_name( a ), &bnm->child ) ;
  anm->type = get_info_type( a ) ;
  anm->info = a ;
}

int comp_class( vcode_info *a, JD_name *bnm ) {
  vcode_info *b ;
  vcode_info *pa, *pb, *nxt ;
  char *ppa, *ppb ;
  JD_name *pnm ;
  
  b = bnm->info ;
  
  ppa = a->is.class.parent ;
  ppb = b->is.class.parent ;
  
  if( ppa || ppb ) {
    if( !ppa || !ppb ) {
      ERROR_PARENT_CLASS_MISMATCH( a->is.class.name ) ;
      return 0 ;
    }
    if( strcmp(ppa, ppb) ) {
      ERROR_CLASS_EXTENTION_NAME_MISMATCH( a->is.class.name, ppa, ppb ) ;
      return 0 ;
    }
  }
  if( !comp_attrib( a->is.class.attribs, b->is.class.attribs ) ) {
    ERROR_CLASS_ATTRIB_MISMATCH( a->is.class.name ) ;
    return 0 ;
  }
  
  pa = a->is.class.members ;
  while( pa ) {
    pnm = find_name_in_scope( get_info_name(pa), bnm->child ) ;
    if( pnm ) {
      /* check consistency */
      pb = pnm->info ;
      comp_member( bnm->name, pa, pb ) ;
      pa = pa->next ;
    }
    else {
      /* append the new member to class structure (reasonable??) */
      nxt = pa->next ;
      pa->next = NULL ;
      append_member( pa, bnm ) ;
      pa = nxt ;
    }
  }
  return 1 ;
}

int comp_aspect( vcode_info *a, JD_name *bnm ) {
  vcode_info *b ;
  vcode_info *pa, *pb, *nxt ;
  char *ppa, *ppb ;
  JD_name *pnm ;
  
  b = bnm->info ;
    
  pa = a->is.class.members ;
  while( pa ) {
    pnm = find_name_in_scope( get_info_name(pa), bnm->child ) ;
    if( pnm ) {
      /* check consistency */
      pb = pnm->info ;
      comp_member( bnm->name, pa, pb ) ;
      pa = pa->next ;
    }
    else {
      /* append the new member to class structure (reasonable??) */
      nxt = pa->next ;
      pa->next = NULL ;
      append_member( pa, bnm ) ;
      pa = nxt ;
    }
  }
  return 1 ;
}
