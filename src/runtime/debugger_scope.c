/*
 * Jeda Debugger name scope support 
 */
/* 
   
   Copyright (C) 2001 Juniper Networks Inc.
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
/* #include <math.h> */
#include "runtime.h"
#include "mem_macro.h"
#include "runtime_error_msg.h"
#include "data.h"
#include "thread.h"
#include "bit_operations.h"
#include "bit_macro.h"
#include "exec_macro.h"
#include "data_macro.h"
#include "code_exec_macro.h"

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#include "debugger.h"
#include "debugger_error_msg.h"

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif

extern JD_debug_scope JD_global_scope[] ;
extern JD_debug_scope JD_local_scopes[] ;
extern JD_debug_scope JD_scope_table[] ;

extern int JD_global_scope_num ;
extern int JD_local_scope_num ;
extern int JD_scope_table_num ;

extern JD_debug_stack *JD_debugged_stack ;

extern int JD_dbg_eval_error ;

JD_dbg_name_node **JD_dbg_active_scope_table ;
JD_dbg_name_node **JD_dbg_global_scope_table ;
JD_dbg_name_node ***JD_dbg_local_scope_table ;
int *JD_dbg_local_scope_table_maxid ;

JD_dbg_name_node *JD_debugger_lookup_name( 
  char *name, int flag
) ;

/***********************************************************************
 * Construction of the name scope tables are done at the begining of the
 *  simulation. 
 ***********************************************************************/

/* 
  construct_scope_table():
  Read scope table and construct the scope structure. 
  This reader can be used on global, local and active scope table.
 */

static int construct_scope_table( 
  JD_dbg_name_node **scope, JD_debug_scope *table, 
  JD_dbg_name_node_scope_type scope_type, int auto_flag, int max
)
{
  int i, j, k ;
  int sid = 0 ;
  int tid = 0 ;
  int pid = 0 ;
  
  int n = 0 ;
  
  int blk_flag ;
  
  JD_dbg_name_node *nws, *ps ;
  
  while( table[tid].type >= 0 ) {
    n++ ;
    nws = (JD_dbg_name_node *)calloc( 1, sizeof(JD_dbg_name_node) ) ;
    nws->index = table[tid].index ;
    nws->name = table[tid].name ;
    nws->dtype = table[tid].dtype ;
    nws->level = table[tid].level ;
    nws->ub = table[tid].ub ;
    nws->lb = table[tid].lb ;
    nws->scope = scope_type ;
    
    switch( table[tid].type ) {
      case 0:
        nws->type = JD_dbg_empty_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_FUNC:
        nws->type = JD_dbg_func_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        break ;
      case DBG_TYPE_ADVICE:
        nws->type = JD_dbg_advice_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        break ;
      case DBG_TYPE_ASPECT:
        nws->type = JD_dbg_aspect_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_CLASS:
        nws->type = JD_dbg_class_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_BLOCK:
        nws->type = JD_dbg_block_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        break ;
      case DBG_TYPE_PORTSET:
        nws->type = JD_dbg_portset_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_PORT:
        nws->type = JD_dbg_port_type ;
        blk_flag = 0 ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        break ;
      case DBG_TYPE_VAR:
        nws->type = JD_dbg_var_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_ARG:
        nws->type = JD_dbg_arg_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_ENUM:
        nws->type = JD_dbg_enum_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_MVAR:
        nws->type = JD_dbg_mvar_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_AVAR:
        nws->type = JD_dbg_avar_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
    }
    if( table[tid].parent ) {
      ps = scope[table[tid].parent] ;
      if( ps == NULL ) {
        fprintf( 
          stderr, "JEDA LINKER INTERNAL ERROR: null scope on %d\n",
          table->parent
        ) ;
        exit(1) ;
      }
      if( blk_flag ) {
        nws->parent = ps ;
      }
      else {
        /* connect this to the end */
        while( ps->next ) ps = ps->next ;
        ps->next = nws ;
      }
    }
    if( sid > max+1 ) {
      fprintf( 
        stderr, "JEDA LINKER INTERNAL ERROR: scope table overflow %d\n",
        sid
      ) ;
      exit(1) ;
    }
    tid++ ;
  }
  
  return n ;
  
}

#ifdef PRODUCT_CODE

static int build_scope_table( 
  JD_dbg_name_node **scope, JD_debug_scope *table, 
  JD_dbg_name_node_scope_type scope_type, int auto_flag, int max,
  int offset, int name_offset
)
{
  int i, j, k ;
  int sid ;
  int tid = 0 ;
  int pid = 0 ;
  
  int n = 0 ;
  
  int blk_flag ;
  
  JD_dbg_name_node *nws, *ps ;
  
  sid = offset ;
  
  while( table[tid].type >= 0 ) {
    n++ ;
    nws = (JD_dbg_name_node *)calloc( 1, sizeof(JD_dbg_name_node) ) ;
    nws->index = table[tid].index ;
    nws->name = table[tid].name + name_offset ;
    nws->dtype = table[tid].dtype + name_offset ;
    nws->level = table[tid].level ;
    nws->ub = table[tid].ub ;
    nws->lb = table[tid].lb ;
    nws->scope = scope_type ;
    
    switch( table[tid].type ) {
      case 0:
        nws->type = JD_dbg_empty_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_FUNC:
        nws->type = JD_dbg_func_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        nws->lb += name_offset ; /* hold srcfilename */
        break ;
      case DBG_TYPE_ADVICE:
        nws->type = JD_dbg_advice_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        nws->lb += name_offset ; /* hold srcfilename */
        break ;
      case DBG_TYPE_ASPECT:
        nws->type = JD_dbg_aspect_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        nws->lb += name_offset ; /* hold srcfilename */
        break ;
      case DBG_TYPE_CLASS:
        nws->type = JD_dbg_class_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        nws->lb += name_offset ; /* hold srcfilename */
        break ;
      case DBG_TYPE_BLOCK:
        nws->type = JD_dbg_block_type ;
        if( auto_flag || !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        if( auto_flag ) blk_flag = 1 ;
        else blk_flag = 0 ;
        break ;
      case DBG_TYPE_PORTSET:
        nws->type = JD_dbg_portset_type ;
        scope[sid++] = nws ;
        blk_flag = 1 ;
        break ;
      case DBG_TYPE_PORT:
        nws->type = JD_dbg_port_type ;
        blk_flag = 0 ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        break ;
      case DBG_TYPE_VAR:
        nws->type = JD_dbg_var_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_ARG:
        nws->type = JD_dbg_arg_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_ENUM:
        nws->type = JD_dbg_enum_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_MVAR:
        nws->type = JD_dbg_mvar_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
      case DBG_TYPE_AVAR:
        nws->type = JD_dbg_avar_type ;
        if( !table[tid].parent ) {
          scope[sid++] = nws ;
        }
        blk_flag = 0 ;
        break ;
    }
    if( table[tid].parent ) {
      ps = scope[table[tid].parent+offset] ;
      if( ps == NULL ) {
        fprintf( 
          stderr, "JEDA LINKER INTERNAL ERROR: null scope on %d + %d\n",
          table->parent, offset
        ) ;
        exit(1) ;
      }
      if( blk_flag ) {
        nws->parent = ps ;
      }
      else {
        /* connect this to the end */
        while( ps->next ) ps = ps->next ;
        ps->next = nws ;
      }
    }
    if( sid > max+1 ) {
      fprintf( 
        stderr, "JEDA LINKER INTERNAL ERROR: scope table overflow %d\n",
        sid
      ) ;
      exit(1) ;
    }
    tid++ ;
  }
  
  return sid ;
  
}
extern JD_module_table JD_modules_entries[] ;

JD_debugger_construct_scope_structure() {
  int i, j, k, m, n, p ;
  int head ;
  int tail ;
  int size ;
  int maxid ;
  JD_dbg_name_node **scope ;
  
  size = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    for( j = 0 ; strcmp(JD_modules_entries[i].name_table[j],"") ; j++ ) {
      size ++ ;
    }
  }
  JD_Debug_name_table = 
    (char **)calloc( size+1, sizeof(char *) );
  n = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    if( JD_modules_entries[i].name_offset ) {
      *(JD_modules_entries[i].name_offset) = n ;
    }
    for( j = 0 ; strcmp(JD_modules_entries[i].name_table[j],"") ; j++ ) {
      JD_Debug_name_table[n++] =  JD_modules_entries[i].name_table[j] ;
    }
  }
    
  size = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    size += *(JD_modules_entries[i].s_size) ;
  }
  JD_dbg_active_scope_table = 
    (JD_dbg_name_node **)
    calloc( size+1, sizeof(JD_dbg_name_node *) );
  n = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    if(JD_modules_entries[i].s_offset) {
      *(JD_modules_entries[i].s_offset) = n ;
    }
    if( JD_modules_entries[i].name_offset ) {
      m = *(JD_modules_entries[i].name_offset) ;
    }
    else m = 0 ;
    n = build_scope_table( 
      JD_dbg_active_scope_table, JD_modules_entries[i].scope, 
      JD_dbg_active_scope_type, 1, size+1,
      n, m
    ) ;
  }
  
  size = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    size += *(JD_modules_entries[i].g_size) ;
  }
  JD_dbg_global_scope_table =
    (JD_dbg_name_node **)
    calloc( size+1, sizeof(JD_dbg_name_node *) );
  n = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    if( JD_modules_entries[i].name_offset ) {
      m = *(JD_modules_entries[i].name_offset) ;
    }
    else m = 0 ;
    n = build_scope_table( 
      JD_dbg_global_scope_table, JD_modules_entries[i].g_scope, 
      JD_dbg_global_scope_type, 0, size+1,
      n, m
    ) ;
  }
  /* for local scope, construct multiple (array) of scope */
  size = 0 ;
  for( i = 0 ; JD_modules_entries[i].g_scope ; i++ ) {
    size++ ;
  }
  JD_dbg_local_scope_table =
    (JD_dbg_name_node ***)
    calloc( size+1, sizeof(JD_dbg_name_node **) ) ;
  JD_dbg_local_scope_table_maxid = 
    (int *) calloc( size+1, sizeof(int) ) ;
  p = 0 ;
  for( k = 0 ; JD_modules_entries[k].g_scope ; k++ ) {
    head = 0 ;
    if( JD_modules_entries[k].name_offset ) {
      m = *(JD_modules_entries[k].name_offset) ;
    }
    else m = 0 ;
    if( JD_modules_entries[k].l_offset ) {
      *(JD_modules_entries[k].l_offset) = p ; /* set local table offset */
    }
    for( i = 0 ; JD_modules_entries[k].l_scope[head].index >= 0 ; i++ ) {
      n = 0 ;
      tail = head ;
      while( JD_modules_entries[k].l_scope[tail].type >= 0 ) tail++ ;
      size = tail - head ;
      if( size ) {
        JD_dbg_local_scope_table[p] =
          (JD_dbg_name_node **)
          calloc( size+1, sizeof(JD_dbg_name_node *) ) ;
        n = build_scope_table( 
          JD_dbg_local_scope_table[p], &JD_modules_entries[k].l_scope[head], 
          JD_dbg_local_scope_type, 0, size,
          n, m
        ) ;
        scope = JD_dbg_local_scope_table[p] ;
        j = 0 ;
        maxid = 0 ;
        while( scope[j] ) {
          if( scope[j]->index > maxid ) maxid = scope[j]->index ;
          j++ ;
        }
        JD_dbg_local_scope_table_maxid[p++] = maxid ;
      }
      head = tail + 1 ;
    }
  }
}

#else

JD_debugger_construct_scope_structure() {
  int i, j ;
  int head ;
  int tail ;
  int size ;
  int maxid ;
  JD_dbg_name_node **scope ;
  
  JD_dbg_active_scope_table = 
    (JD_dbg_name_node **)
    calloc( JD_scope_table_num+1, sizeof(JD_dbg_name_node *) );
  construct_scope_table( 
    JD_dbg_active_scope_table, JD_scope_table, 
    JD_dbg_active_scope_type, 1, JD_scope_table_num
  ) ;
  
  JD_dbg_global_scope_table =
    (JD_dbg_name_node **)
    calloc( JD_global_scope_num+1, sizeof(JD_dbg_name_node *) );
  construct_scope_table( 
    JD_dbg_global_scope_table, JD_global_scope, 
    JD_dbg_global_scope_type, 0, JD_global_scope_num
  ) ;
  
  /* for local scope, construct multiple (array) of scope */
  head = 0 ;
  JD_dbg_local_scope_table =
    (JD_dbg_name_node ***)
    calloc( JD_local_scope_num, sizeof(JD_dbg_name_node **) ) ;
  JD_dbg_local_scope_table_maxid = 
    (int *) calloc( JD_local_scope_num, sizeof(int) ) ;
  for( i = 0 ; i < JD_local_scope_num ; i++ ) {
    tail = head ;
    while( JD_local_scopes[tail].type >= 0 ) tail++ ;
    size = tail - head ;
    if( size ) {
      JD_dbg_local_scope_table[i] =
        (JD_dbg_name_node **)
        calloc( size+1, sizeof(JD_dbg_name_node *) ) ;
      construct_scope_table( 
        JD_dbg_local_scope_table[i], &JD_local_scopes[head], 
        JD_dbg_local_scope_type, 0, size
      ) ;
      scope = JD_dbg_local_scope_table[i] ;
      j = 0 ;
      maxid = 0 ;
      while( scope[j] ) {
        if( scope[j]->index > maxid ) maxid = scope[j]->index ;
        j++ ;
      }
      JD_dbg_local_scope_table_maxid[i] = maxid ;
    }
    head = tail+1 ;
  }
 
}

#endif

/***********************************************************************
 * Name look up functions for debugger expression evaluation
 ***********************************************************************/

JD_dbg_name_node *JD_debugger_lookup_member( 
  char *name, JD_dbg_name_node *scope, int *level, int up_flag 
) 
{
  JD_dbg_name_node *child, *parent ;
  
  child = scope->next ;
  while( child ) {
    if( !strcmp( name, JD_Debug_name_table[child->name] ) ) return child ;
    child = child->next ;
  }
  if( !up_flag ) return NULL ;
  if( !strcmp( JD_Debug_name_table[scope->dtype], "this") ) return NULL ;
  *level = *level + 1 ;
  parent = JD_debugger_lookup_name( JD_Debug_name_table[scope->dtype], 0 ) ;
  if( (parent == NULL) || 
      ( (parent->type != JD_dbg_class_type) && 
        (parent->type != JD_dbg_aspect_type)   ) ) return NULL ;
  return JD_debugger_lookup_member( name, parent, level, up_flag ) ;
}

int JD_debugger_lookup_class() 
{
  JD_dbg_name_node *scope ;
  
  if( !JD_debugged_stack->active ) {
    DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
    return -1 ;
  }
  
  scope =
    JD_dbg_active_scope_table[JD_debugged_stack->scope] ;
  while( scope ) {
    if( scope->type == DBG_TYPE_FUNC || scope->type == DBG_TYPE_ADVICE ) 
      return scope->dtype ;
    scope = scope->parent ;
  }
  return -1 ;
}

JD_dbg_name_node *JD_debugger_lookup_member_name_on_active_scope( 
  char *name, JD_dbg_name_node *scope
) 
{
  JD_dbg_name_node *child ;
  char *nm ;
  
  while( scope ) {
    child = scope ;
    while( child ) {
      if( child->type != JD_dbg_empty_type && 
          child->type != JD_dbg_block_type    ) 
      {
        nm = JD_Debug_name_table[child->name] ;
        if( !strcmp(name, nm) ) return child ; 
      }
      child = child->next ;
    }
    scope = scope->parent ;
  }
   
   return NULL ;
}

JD_dbg_name_node *JD_debugger_lookup_member_name_on_static_scope( 
  char *name, JD_dbg_name_node **scope
) 
{
  JD_dbg_name_node *child ;
  char *nm ;
  int i ;
  
  i = 0 ;
  while( scope[i] ) {
    if( scope[i]->type != JD_dbg_empty_type && 
        scope[i]->type != JD_dbg_block_type    ) 
    {
      nm = JD_Debug_name_table[scope[i]->name] ;
      if( !strcmp(name, nm) ) return scope[i] ; 
    }
    i++ ;
  }
   
  return NULL ;
}

JD_dbg_name_node *JD_debugger_lookup_name( 
  char *name, int flag
) 
{
  JD_dbg_name_node *ret ;
  
  ret = NULL ;
  
  if( flag ) {
    if( !JD_debugged_stack->active ) {
      DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
      return NULL ;
    }
    ret = 
      JD_debugger_lookup_member_name_on_active_scope( 
        name, JD_dbg_active_scope_table[JD_debugged_stack->scope]
      ) ;  
  }
  if( ret == NULL ) {
    ret = 
      JD_debugger_lookup_member_name_on_static_scope( 
        name, JD_dbg_local_scope_table[JD_debugged_stack->local_scope] 
      ) ;
  }
  if( ret == NULL ) {
    ret = 
      JD_debugger_lookup_member_name_on_static_scope( 
        name, JD_dbg_global_scope_table
      ) ;
  }
    
  return ret ;
}

JD_dbg_name_node *JD_debugger_lookup_member_name_on_static_scope_w_f( 
  char *name, char **filename, JD_dbg_name_node **scope
) 
{
  JD_dbg_name_node *child ;
  char *nm ;
  int filenum ;
  int i ;
  
  i = 0 ;
  while( scope[i] ) {
    if( scope[i]->type == JD_dbg_func_type || 
        scope[i]->type == JD_dbg_class_type ||
        scope[i]->type == JD_dbg_advice_type ||
        scope[i]->type == JD_dbg_aspect_type   ) 
    {
      filenum = scope[i]->lb ;
      nm = JD_Debug_name_table[scope[i]->name] ;
      if( !strcmp(name, nm) ) {
        if( *filename == NULL ) {
          *filename = JD_Debug_name_table[filenum] ;
          return scope[i] ;
        }
        else { 
          if( !strcmp( *filename, JD_Debug_name_table[filenum] ) )
            return scope[i] ; 
        }
      }
    }
    i++ ;
  }
   
  return NULL ;
}

void JD_dbg_lookup_funcname(
  char *name, char **filename, int *linenum 
)
{
  JD_dbg_name_node *ret, *fnc ;
  char *cp ;
  cp = name ;
  while( *cp != '\0' && *cp != '.' && !isspace(*cp) ) cp++ ;
  if( *cp == '\0' || isspace(*cp) ) {
    if( isspace(*cp) ) *cp = '\0' ;
    ret = 
      JD_debugger_lookup_member_name_on_static_scope_w_f( 
        name, filename, 
        JD_dbg_local_scope_table[JD_debugged_stack->local_scope] 
      ) ;
    if( ret == NULL ) {
      ret = 
        JD_debugger_lookup_member_name_on_static_scope_w_f( 
          name, filename, JD_dbg_global_scope_table
        ) ;
    }
    if( ret && ret->type == JD_dbg_func_type ) *linenum = ret->ub ;
    else *linenum = -1 ;
  }
  else {
    /* class.member form */
    *cp = '\0' ;
    cp++ ;
    ret = 
      JD_debugger_lookup_member_name_on_static_scope_w_f( 
        name, filename, 
        JD_dbg_local_scope_table[JD_debugged_stack->local_scope] 
      ) ;
    if( ret == NULL ) {
      ret = 
        JD_debugger_lookup_member_name_on_static_scope_w_f( 
          name, filename, JD_dbg_global_scope_table
        ) ;
    }
    if( ret ) {
      int tmp ;
      fnc = JD_debugger_lookup_member( cp, ret, &tmp, 0 ) ;
      *linenum = ret->dtype ;
      if( fnc && fnc->type == JD_dbg_func_type ) *linenum = fnc->ub ;
      else *linenum = -1 ;
    }
    else *linenum = -1 ;
    
  }
}

JD_dbg_name_node *JD_debugger_lookup_class_var( 
  JD_dbg_name_node *scope, int index
) 
{
  JD_dbg_name_node *child ;
  char *nm ;
  
  child = scope ;
  while( child ) {
    if( child->type == JD_dbg_var_type &&
        child->index == index 
      ) 
    {
      return child ; 
    }
    child = child->next ;
  }
   
  return NULL ;
}

JD_dbg_name_node *JD_debugger_lookup_local_var( 
  int index
) 
{
  JD_dbg_name_node *scope, *child, *keep ;
  
  if( !JD_debugged_stack->active ) {
    DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
    return NULL ;
  }
  
  keep = NULL ;
  
  scope = JD_dbg_active_scope_table[JD_debugged_stack->scope] ;
  
  while( scope ) {
    child = scope ;
    while( child ) {
      if( ( child->type == JD_dbg_var_type || 
            child->type == JD_dbg_arg_type    )  
        ) 
      {
        if( child->index == index ) return child ; 
        if( child->index > index ) {
          if( keep == NULL ) keep = child ;
          else if( keep->index > child->index ) keep = child ;
        }
      }
      child = child->next ;
    }
    scope = scope->parent ;
  }
   
  return keep ;
}

JD_dbg_name_node *JD_debugger_lookup_local_scope_var( 
  int index
) 
{
  JD_dbg_name_node **scope ;
  char *nm ;
  int i, maxid ;
  
  scope = JD_dbg_local_scope_table[JD_debugged_stack->local_scope] ;
  maxid = JD_dbg_local_scope_table_maxid[JD_debugged_stack->local_scope] ;
  if( index > maxid ) return NULL ;
  if( !scope ) return NULL ;
  i = 0 ;
  while( scope[i] ) {
    if( scope[i]->index == index && scope[i]->type == JD_dbg_var_type ) 
    {
      return scope[i] ; 
    }
    i++ ;
  }
   
  return JD_dbg_global_scope_table[0] ;
}

JD_dbg_name_node *JD_debugger_lookup_global_scope_var( 
  int index
) 
{
  JD_dbg_name_node **scope ;
  char *nm ;
  int i ;
  int imax = 0 ;
  
  scope = JD_dbg_global_scope_table ;
  i = 0 ;
  while( scope[i] ) {
    if( scope[i]->index == index && scope[i]->type == JD_dbg_var_type ) 
    {
      return scope[i] ; 
    }
    if( scope[i]->index > imax ) imax = scope[i]->index ;
    i++ ;
  }
   
  if( index > imax ) return NULL ;
  else return scope[0] ;
}

JD_dbg_expression_node *JD_debugger_get_data(
  char *name
) 
{
  JD_dbg_expression_node *ret ;
  JD_dbg_name_node *name_node, *clss, *mem ;
  int index, tmp ;
  JD_data *dt ;
  int base ;
  JD_thread *thrd ;
  
  name_node = JD_debugger_lookup_name( name, 1 ) ;
  if( !name_node ) {
    return NULL ;
  }
  ret = (JD_dbg_expression_node *)JD_dbg_alloc_exp() ;
  switch( name_node->scope ) {
    case JD_dbg_active_scope_type: /* local stack variable */
      thrd = JD_debugged_stack->thrd ;
      switch( name_node->type ) {
        case JD_dbg_var_type:  /* variable */
        case JD_dbg_arg_type:  /* argument */
          ret->dt = 
            thrd->stack[JD_debugged_stack->stk_frame + name_node->index + 1] ;
          while( ret->dt->type == JD_D_pointer ) {
            ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
          }
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_dt ;
          ret->etype = JD_dbg_exp_variable ;
          ret->scope = name_node ;
          break ;
        case JD_dbg_mvar_type:  /* member variable */
          if( !JD_debugged_stack->this || 
              !JD_debugged_stack->this->data.obj.ect ) 
          {
            DBG_ERROR_EMPTY_OBJECT_ACCESS ;
            free(ret) ;
            return NULL ;
          }
          dt = JD_debugged_stack->this ;
          clss = 
            JD_debugger_lookup_name( 
              JD_Debug_name_table[name_node->dtype], 0 
            ) ;
          if( !clss ) {
            DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
              JD_Debug_name_table[name_node->dtype]
            ) ;
            free(ret) ;
            return NULL ;
          }
          if( name_node->level ) {
            int i ;
            for( i = 0 ; i < name_node->level ; i++ ) {
              dt = dt->data.obj.ect->super ;
              if( !dt ) {
                DBG_ERROR_EMPTY_OBJECT_ACCESS ;
                free(ret) ;
                return NULL ;
              }
            }
          }
          ret->dt = 
            dt->data.obj.ect->table[name_node->index] ;
          while( ret->dt->type == JD_D_pointer ) {
            ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
          }
          mem = clss->next ;
          while( mem ) {
            if( mem->type == JD_dbg_var_type &&
                mem->index == name_node->index  ) break ;
            mem = mem->next ;
          }
          if( !mem ) {
            DBG_ERROR_MEMBER_NAME_NOT_FOUND_IN(
              JD_Debug_name_table[name_node->name],
              JD_Debug_name_table[name_node->dtype]
            ) ;
            free(ret) ;
            return NULL ;
          }
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_dt ;
          ret->etype = JD_dbg_exp_variable ;
          ret->scope = mem ;
          break ;
        case JD_dbg_avar_type:  /* aspect member variable */
          clss = 
            JD_debugger_lookup_name( 
              JD_Debug_name_table[name_node->dtype], 0 
            ) ;
          if( !clss ) {
            DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
              JD_Debug_name_table[name_node->dtype]
            ) ;
            free(ret) ;
            return NULL ;
          }
          ret->dt = GLOBAL_DATA[name_node->index] ;
          while( ret->dt->type == JD_D_pointer ) {
            ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
          }
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_dt ;
          ret->etype = JD_dbg_exp_variable ;
          ret->scope = mem ;
          break ;
        case JD_dbg_class_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_parent ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_class ;
          break ;
        case JD_dbg_aspect_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_parent ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_aspect ;
          break ;
        case JD_dbg_enum_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->pname = JD_Debug_name_table[name_node->index] ;
          ret->int_value = name_node->level ;
          ret->type = JD_D_enum ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_enum ;
          break ;
        case JD_dbg_func_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->etype = JD_dbg_exp_func ;
          ret->scope = name_node ;
          /* may need more info to get the filename, current dir name
             to make this info usefull
          */
          break ;
        case JD_dbg_advice_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->etype = JD_dbg_exp_advice ;
          ret->scope = name_node ;
          /* may need more info to get the filename, current dir name
             to make this info usefull
          */
          break ;
      }
      break ;
    case JD_dbg_local_scope_type: /* local scope variable */
    case JD_dbg_global_scope_type: /* global scope variable */
      switch( name_node->type ) {
        case JD_dbg_var_type:  /* variable */
        case JD_dbg_arg_type:  /* argument */
        case JD_dbg_avar_type: /* aspect variable */ 
          ret->dt = GLOBAL_DATA[name_node->index] ;
          while( ret->dt->type == JD_D_pointer ) {
            ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
          }
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_dt ;
          ret->etype = JD_dbg_exp_variable ;
          ret->scope = name_node ;
          break ;
        case JD_dbg_port_type:  /* port */
          ret->dt = GLOBAL_DATA[name_node->index] ;
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_dt ;
          ret->etype = JD_dbg_exp_port ;
          ret->scope = name_node ;
          break ;
        case JD_dbg_portset_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_parent ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_portset ;
          break ;
       case JD_dbg_class_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_parent ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_class ;
          break ;
        case JD_dbg_enum_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->pname = JD_Debug_name_table[name_node->index] ;
          ret->int_value = name_node->level ;
          ret->type = JD_D_enum ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_enum ;
          break ;
        case JD_dbg_func_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_func ;
          /* may need more info to get the filename, current dir name
              to make this info usefull
          */
          break ;
        case JD_dbg_aspect_type:
          ret->name = JD_Debug_name_table[name_node->name] ;
          ret->type = JD_D_parent ;
          ret->scope = name_node ;
          ret->etype = JD_dbg_exp_aspect ;
          break ;
      }
      break ;
  }
  
  return ret ;
  
}
 
JD_dbg_expression_node *JD_debugger_get_member(
  JD_dbg_expression_node *exp, char *name
) 
{
  JD_dbg_name_node *name_node, *parent ;
  JD_dbg_expression_node *ret ;
  int level ;
  
  if( exp->etype == JD_dbg_exp_variable ) {
    if( exp->dt->type != JD_D_object ) {
      DBG_ERROR_VAR_NOT_HAVE_MEMBER( name ) ;
      JD_dbg_free_exp(exp) ;
      return NULL ;
    }
    
    if( exp->dt->sub_type != JD_SD_single ) {
      DBG_ERROR_ILEEGAL_MEMBER_ACCESS_ON_NON_SINGLE_VARIABLE( name ) ;
      JD_dbg_free_exp(exp) ;
      return NULL ;
    }
    
  }
  else if( exp->etype != JD_dbg_exp_portset && exp->etype != JD_dbg_exp_aspect)
  {
    DBG_ERROR_ILEEGAL_OBJECT_TYPE_FOR_MEMBER_ACCESS ;
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  
  if( exp->etype == JD_dbg_exp_portset ) {
    name_node = exp->scope->next ;
    
    while( name_node ) {
      if( !strcmp(name, JD_Debug_name_table[name_node->name]) ) {
        ret = JD_dbg_alloc_exp() ;
        ret->etype = JD_dbg_exp_port ;
        ret->dt = GLOBAL_DATA[name_node->index] ;
        ret->name = JD_Debug_name_table[name_node->name] ;
        ret->type = JD_D_dt ;
        ret->scope = name_node ;
        JD_dbg_free_exp(exp) ;
        return ret ;
      }
      name_node = name_node->next ;
    }
    DBG_ERROR_MEMBER_NAME_NOT_FOUND_IN( 
      name, exp->name
    );
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  
  if( exp->etype != JD_dbg_exp_aspect ) {
    parent = 
      JD_debugger_lookup_name( 
        JD_Debug_name_table[exp->scope->dtype], 0 
      ) ;
  }
  else {
    parent = 
      JD_debugger_lookup_name( exp->name, 0 ) ;
  }
  if( parent == NULL ) {
    DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
      JD_Debug_name_table[exp->scope->dtype] 
    );
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  
  level = 0 ;
  name_node = JD_debugger_lookup_member( name, parent, &level, 1 ) ;
  if( !name_node ) {
    DBG_ERROR_MEMBER_NAME_NOT_FOUND_IN( 
      name, JD_Debug_name_table[exp->scope->dtype]
    );
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  if( level ) {
    while(level) {
      if( exp->dt == NULL || exp->dt->data.obj.ect == NULL ) {
        DBG_ERROR_EMPTY_OBJECT_ACCESS ;
        return NULL ;
      }
      exp->dt = exp->dt->data.obj.ect->super ;
      level-- ;
    }
  }
  ret = JD_dbg_alloc_exp() ;
  switch( name_node->type ) {
    case JD_dbg_var_type:  /* variable */
      if( exp->dt->sub_type == JD_SD_single ) {
        if( exp->dt->data.obj.ect == NULL ) {
          free(ret) ;
          DBG_ERROR_EMPTY_OBJECT_ACCESS ;
          return NULL ;
        }
        ret->dt = exp->dt->data.obj.ect->table[name_node->index] ;
      }
      else ret->dt = exp->dt ;
      while( ret->dt->type == JD_D_pointer ) {
        ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
      }
      ret->name = JD_Debug_name_table[name_node->name] ;
      ret->type = JD_D_dt ;
      ret->etype = JD_dbg_exp_variable ;
      ret->scope = name_node ;
      break ;
    case JD_dbg_avar_type:  /* variable */
      ret->dt = GLOBAL_DATA[name_node->index] ;
      while( ret->dt->type == JD_D_pointer ) {
        ret->dt = (JD_data *)JD_get_ptr_element(ret->dt) ;
      }
      ret->name = JD_Debug_name_table[name_node->name] ;
      ret->type = JD_D_dt ;
      ret->etype = JD_dbg_exp_variable ;
      ret->scope = name_node ;
      break ;
    case JD_dbg_class_type:
    case JD_dbg_aspect_type:
      free(ret) ;
      DBG_ERROR_NAME_TYPE_INCONSISTANCE ;
      return NULL ;
      break ;
    case JD_dbg_enum_type:
      ret->name = JD_Debug_name_table[name_node->name] ;
      ret->pname = JD_Debug_name_table[name_node->index] ;
      ret->int_value = name_node->level ;
      ret->type = JD_D_enum ;
      ret->scope = name_node ;
      ret->etype = JD_dbg_exp_enum ;
      break ;
    case JD_dbg_func_type:
      ret->name = JD_Debug_name_table[name_node->name] ;
      ret->etype = JD_dbg_exp_func ;
      break ;
    case JD_dbg_advice_type:
      ret->name = JD_Debug_name_table[name_node->name] ;
      ret->etype = JD_dbg_exp_advice ;
      break ;
  }
  
  JD_dbg_free_exp(exp) ;
  
  return ret ;
  
}

JD_dbg_expression_node *JD_dbg_get_this_exp(){
  JD_thread *thrd ;
  JD_dbg_expression_node *ret ;
  int cls ;
  JD_dbg_name_node *name_node ;
  
  if( !JD_debugged_stack->active ) {
    DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
    return NULL ;
  }
  
  thrd = JD_debugged_stack->thrd ;
  
  if( JD_debugged_stack->this == NULL ) {
    DBG_ERROR_THIS_IS_ACCESSED_IN_A_NON_CLASS_CONTEXT;
    return NULL ;
  }
  cls = JD_debugger_lookup_class() ;
  if( cls < 0 ) {
    DBG_ERROR_CLASS_SCOPE_CANT_FOUND_IN_CURRENT_CONTEXT;
    return NULL ;
  }
  name_node = 
    JD_debugger_lookup_name( 
      "this", 0 
    ) ;
    
  /* use "this" node as temp scope */
  if( name_node == NULL ) {
    DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
      "this" 
    );
    return NULL ;
  }
  name_node->dtype = cls ; /* keep class name as the data type */
  ret = JD_dbg_alloc_exp() ;
  ret->dt = JD_debugged_stack->this ;
  ret->name = "this" ;
  ret->type = JD_D_dt ;
  ret->etype = JD_dbg_exp_variable ;
  ret->scope = name_node ;  
    
  return ret ;
}

JD_dbg_expression_node *JD_dbg_get_super_exp(){
  JD_thread *thrd ;
  JD_dbg_expression_node *ret ;
  int cls ;
  JD_dbg_name_node *name_node, *super_node ;
  
  if( !JD_debugged_stack->active ) {
    DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
    return NULL ;
  }
  
  thrd = JD_debugged_stack->thrd ;
  
  if( JD_debugged_stack->this == NULL ) {
    DBG_ERROR_SUPER_IS_ACCESSED_IN_A_NON_CLASS_CONTEXT;
    return NULL ;
  }
  if( JD_debugged_stack->this->data.obj.ect == NULL || 
      JD_debugged_stack->this->data.obj.ect->super == NULL ) 
  {
    DBG_ERROR_SUPER_IS_NOT_FOUND_IN_THIS_CLASS_CONTEXT;
    return NULL ;
  }
  cls = JD_debugger_lookup_class() ;
  if( cls < 0 ) {
    DBG_ERROR_CLASS_SCOPE_CANT_FOUND_IN_CURRENT_CONTEXT;
    return NULL ;
  }
  name_node = 
    JD_debugger_lookup_name( 
      JD_Debug_name_table[cls], 0 
    ) ;
  if( name_node == NULL ) {
    DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
      JD_Debug_name_table[cls] 
    );
    return NULL ;
  }
  /* Use super_node as temp scope for super */
  /* May cause problem upon recursive super access?? */
  super_node = 
    JD_debugger_lookup_name( 
      "super", 0 
    ) ;
  if( super_node == NULL ) {
    DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
      "super" 
    );
    return NULL ;
  }
  super_node->dtype = name_node->dtype ; /* class->dtype carries super name */
  ret = JD_dbg_alloc_exp() ;
  ret->dt = JD_debugged_stack->this->data.obj.ect->super ;
  ret->name = "super" ;
  ret->type = JD_D_dt ;
  ret->etype = JD_dbg_exp_variable ;
  ret->scope = name_node ;
    
  return ret ;
  
}

JD_dbg_expression_node *JD_dbg_get_super_of( JD_dbg_expression_node *exp )
{
  JD_dbg_expression_node *ret ;
  int cls ;
  JD_dbg_name_node *name_node, *super_node ;
  
  if( !JD_debugged_stack->active ) {
    DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE ;
    return NULL ;
  }
    
  name_node = exp->scope ;
  if( name_node == NULL ) {
    DBG_ERROR_SCOPE_NOT_FOUND ;
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  super_node =
    JD_debugger_lookup_name( 
      JD_Debug_name_table[name_node->dtype], 0 
    ) ;
  if( super_node == NULL ) {
    DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
      JD_Debug_name_table[name_node->dtype] 
    );
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  if( exp->dt && 
      ( exp->dt->type != JD_D_object || exp->dt->sub_type != JD_SD_single ) 
  ) {
    DBG_ERROR_SUPER_ACCESS_ON_NON_SINGLE_OBJECT ;
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  if( exp->dt == NULL || exp->dt->data.obj.ect == NULL ||
      exp->dt->data.obj.ect->super == NULL
   ) {
    DBG_ERROR_EMPTY_OBJECT_ACCESS ;
    JD_dbg_free_exp(exp) ;
    return NULL ;
  }
  ret = JD_dbg_alloc_exp() ;
  ret->dt = exp->dt->data.obj.ect->super ;
  ret->name = "super" ;
  ret->type = JD_D_dt ;
  ret->etype = JD_dbg_exp_variable ;
  ret->scope = super_node ;
  
  JD_dbg_free_exp(exp) ;
  
  return ret ;
  
}

static int check_bit_range( int ub, int lb, int xub, int xlb ) {
  if( ub >= lb ) {
    if( xub < xlb || xub > ub || xlb < lb ) {
      DBG_ERROR_ILLEGAL_BIT_ACCESS_RANGE ;
      return 0 ;
    }
    else return 1 ;
  }
  else {  
    if( xub > xlb || xub < ub || xlb > lb ) {
      DBG_ERROR_ILLEGAL_BIT_ACCESS_RANGE ;
      return 0 ;
    }
    else return 2 ;
  }
}

static void attatch_bit_index (
  JD_dbg_expression_node *exp, JD_dbg_expression_node *index,
  int ub, int lb, int flag
) {
  if( flag == 1 ) {
    exp->bit_index = index ;
    exp->bit_index->int_value -= lb ;
    exp->bit_index->next->int_value -= lb ;
  }
  else { 
    exp->bit_index = index->next ;
    exp->bit_index->next = index ;
    index->next = NULL ;
    exp->bit_index->int_value -= ub ;
    exp->bit_index->next->int_value -= ub ;
  }
  
}


static JD_dbg_expression_node *resolve_bit_array_in_array_access( 
  JD_dbg_expression_node *exp, JD_dbg_expression_node *index
){
  int tmp ;
  if( index->etype == JD_dbg_exp_bit_range ) {
    if( exp->dt->type == JD_D_sbit || exp->dt->type == JD_D_bit ) {
      /* at this point, scope info should have correct ub/lb of the data */
      tmp = 
        check_bit_range( 
          exp->scope->ub, exp->scope->lb, 
          index->int_value, index->next->int_value
        ) ;
      if( !tmp ) {
        return NULL ;
      }
      attatch_bit_index( exp, index, exp->scope->ub, exp->scope->lb, tmp ) ;
      return exp ;
    }
    else if( exp->dt->type == JD_D_signal ) {
      JD_pli_object *obj ;
      obj = (JD_pli_object *)exp->dt->data.signal.port ;
      if( obj == NULL ) {
        DBG_ERROR_EMPTY_SIGNAL_ACCESS ;
        return NULL ;
      }
      tmp = 
        check_bit_range( 
          obj->is.port.ub, obj->is.port.lb, 
          index->int_value, index->next->int_value
        ) ;
      if( !tmp ) {
        return NULL ;
      }
      attatch_bit_index( exp, index, obj->is.port.ub, obj->is.port.lb, tmp ) ;
      return exp ;
    }
    else {
      DBG_ERROR_ILLEGAL_BIT_ACCESS_TO_NON_SINGLE_DATA ;
      return NULL ;
    }
  }
  else if( index->etype == JD_dbg_exp_array_range ) 
  {
    if( exp->dt->type != JD_D_sbit && exp->dt->type != JD_D_bit &&
        exp->dt->type != JD_D_signal 
    ) {
      DBG_ERROR_ARRAY_ACCESS ;
      return NULL ;
    }
    if( exp->dt->type == JD_D_sbit || exp->dt->type == JD_D_bit ) {
      tmp = 
        check_bit_range( 
          exp->scope->ub, exp->scope->lb, 
          index->int_value, index->int_value
        ) ;
      if( !tmp ) {
        return NULL ;
      }
      exp->bit_index = index ;
      index->etype = JD_dbg_exp_single_bit_range ;
      if( tmp == 1 ) index->int_value -= exp->scope->lb ;
      else index->int_value -= exp->scope->ub ;
      return exp ;
    }
    if( exp->dt->type == JD_D_signal ) {
      JD_pli_object *obj ;
      obj = (JD_pli_object *)exp->dt->data.signal.port ;
      if( obj == NULL ) {
        DBG_ERROR_EMPTY_SIGNAL_ACCESS ;
        return NULL ;
      }
      tmp = 
        check_bit_range( 
          obj->is.port.ub, obj->is.port.lb, 
          index->int_value, index->int_value
        ) ;
      if( !tmp ) {
        return NULL ;
      }
      index->etype = JD_dbg_exp_single_bit_range ;
      if( tmp == 1 ) index->int_value -= obj->is.port.lb ;
      else index->int_value -= obj->is.port.ub ;
      exp->bit_index = index ;
      return exp ;
    }
  }
  else {
    DBG_ERROR_ARRAY_ACCESS ;
    return NULL ;
  }
}

/*
  JD_debugger_resolve_array_access: based on the data type, 
  analize the array index structure and add array_index, bit_index
  to the expression. It does not resolve it to the actual value,
  as this function can be used to the left hand of the assignment.
*/
JD_dbg_expression_node *JD_debugger_resolve_array_access( 
  JD_dbg_expression_node *exp, JD_dbg_expression_node *index
){
  JD_dbg_expression_node *tindex, *pindex ;
  JD_dbg_expression_node *texp ;
  int nary ;
  int i ;
  
  if( exp->dt == NULL ) {
    DBG_ERROR_EMPTY_DATA_ACCESS ;
    JD_dbg_free_exp(exp) ;
    JD_dbg_free_exp(index) ;
    return NULL ;
  }
  if( index->etype == JD_dbg_exp_bit_range ) {
    if( exp->dt->sub_type != JD_SD_single ) {
      DBG_ERROR_ILLEGAL_BIT_ACCESS_TO_NON_SINGLE_DATA ;
      JD_dbg_free_exp(exp) ;
      JD_dbg_free_exp(index) ;
      return NULL ;
    }
    texp = resolve_bit_array_in_array_access( exp, index ) ;
    if( !texp ) { 
      JD_dbg_free_exp(exp) ;
      JD_dbg_free_exp(index) ;
      return NULL ;
    }
    else return texp ;
  }
  else { 
    switch( exp->dt->sub_type ) {
      case JD_SD_single:
        if( index->etype == JD_dbg_exp_array_range && index->next == NULL &&
            index->next_level == NULL 
        ) {
          texp = resolve_bit_array_in_array_access( exp, index ) ;
          if( !texp ) { 
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
          else return texp ;
        }
        else {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        break ;
      case JD_SD_array:
      case JD_SD_assoc:
        if( ( index->next && index->next_level ) || 
            ( index->next && index->next->next )   
          ) 
        {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        if( index->etype == JD_dbg_exp_array_range ) {
          if( exp->dt->sub_type == JD_SD_array &&
              index->int_value >= exp->dt->data.array.size ) 
          {
            DBG_ERROR_ARRAY_INDEX_X_TOO_LARGE_MAX_IS_Y( 
              index->int_value, exp->dt->data.array.size-1
            ) ;
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
          if( index->next ) {
            tindex = index->next ;
            texp = resolve_bit_array_in_array_access( exp, tindex ) ;
            if( !texp ) { 
              JD_dbg_free_exp(exp) ;
              JD_dbg_free_exp(index) ;
              return NULL ;
            }
          }
          if( index->next_level ) {
            tindex = index->next_level ;
            texp = resolve_bit_array_in_array_access( exp, tindex ) ;
            if( !texp ) { 
              JD_dbg_free_exp(exp) ;
              JD_dbg_free_exp(index) ;
              return NULL ;
            }
          }
          exp->array_index = index ;
          index->next = NULL ;
          index->next_level = NULL ;
          return exp ;
        }
        else {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        break ;
      case JD_SD_marray:
        nary = 0 ;
        tindex = index ;
        while( tindex ) {
          tindex = tindex->next ;
          nary++ ;
        }
        if( ( nary != exp->dt->data.array.dim_size[0] &&
              nary != exp->dt->data.array.dim_size[0] + 1     ) ||
            ( nary == exp->dt->data.array.dim_size[0] + 1 && 
              index->next_level                               )    ) 
        {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        tindex = index ;
        for( i = 0 ; i < exp->dt->data.array.dim_size[0] ; i++ ) {
          if( tindex->int_value > exp->dt->data.array.dim_size[i+1] ) {
            DBG_ERROR_NTH_ARRAY_INDEX_OVERFLOW( i ) ;
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
        }
        if( nary == exp->dt->data.array.dim_size[0] + 1 ) {
          pindex = tindex = index ;
          while( tindex->next ) {
            pindex = tindex ;
            tindex = tindex->next ;
          }
          texp = resolve_bit_array_in_array_access( exp, tindex ) ;
          if( !texp ) { 
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
          else pindex->next = NULL ;
        }
        if( index->next_level ) {
          tindex = index->next_level ;
          texp = resolve_bit_array_in_array_access( exp, tindex ) ;
          if( !texp ) { 
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
          else index->next_level = NULL ;
        }
        exp->array_index = index ;
        return exp ;
        break ;
      case JD_SD_strided:
        if( ( index->next && index->next_level ) || 
            ( index->next && index->next->next ) ||
            index->etype != JD_dbg_exp_string_array_range
          ) 
        {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        if( index->next ) {
          tindex = index->next ;
          texp = resolve_bit_array_in_array_access( exp, tindex ) ;
          if( !texp ) { 
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
        }
        if( index->next_level ) {
          tindex = index->next_level ;
          texp = resolve_bit_array_in_array_access( exp, tindex ) ;
          if( !texp ) { 
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
        }
        exp->array_index = index ;
        index->next = NULL ;
        index->next_level = NULL ;
        return exp ;
        break ;
      case JD_SD_list:
        if( ( index->next && index->next_level ) || 
            ( index->next && index->next->next )   
          ) 
        {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        if( index->etype == JD_dbg_exp_array_range ) {
          JD_data_unit *ut, *at ;
          int n ;
          n = 0 ;
          at = NULL ;
          ut =  exp->dt->data.list.top ;
          while( ut ) {
            if( n == index->int_value ) at = ut ;
            n++ ;
            ut = ut->next ;
          }
          if( !at ) 
          {
            DBG_ERROR_ARRAY_INDEX_X_TOO_LARGE_MAX_IS_Y( 
              index->int_value, n-1
            ) ;
            JD_dbg_free_exp(exp) ;
            JD_dbg_free_exp(index) ;
            return NULL ;
          }
          if( index->next ) {
            tindex = index->next ;
            texp = resolve_bit_array_in_array_access( exp, tindex ) ;
            if( !texp ) { 
              JD_dbg_free_exp(exp) ;
              JD_dbg_free_exp(index) ;
              return NULL ;
            }
          }
          if( index->next_level ) {
            tindex = index->next_level ;
            texp = resolve_bit_array_in_array_access( exp, tindex ) ;
            if( !texp ) { 
              JD_dbg_free_exp(exp) ;
              JD_dbg_free_exp(index) ;
              return NULL ;
            }
          }
          exp->array_index = index ;
          index->next = NULL ;
          index->next_level = NULL ;
          return exp ;
        }
        else {
          DBG_ERROR_ARRAY_ACCESS ;
          JD_dbg_free_exp(exp) ;
          JD_dbg_free_exp(index) ;
          return NULL ;
        }
        break ;
      default:
        DBG_ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
        JD_dbg_free_exp(exp) ;
        JD_dbg_free_exp(index) ;
        return NULL ;
        break ;
    }
  }
}



