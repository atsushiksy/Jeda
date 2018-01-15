/***************************************************************************
 *
 *  symtab.c: symbol table functions for the compiler parse tree
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#include "../include/compiler_macro.h"
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/error_msg.h"
#include "../include/symtab.h"

extern int compile_pass ;

extern int ex_error_flag ;

 named_node *top_scope = NULL ;
 named_node *current_scope = NULL ;
 named_node *local_scope = NULL ;
 
 int scope_id = 0 ;
 int first_verilog_block ;
 
void init_scope() {
  top_scope = ALLOC(named_node) ;
  top_scope->name = "" ;
  top_scope->type = JD_name_dummy ;
  top_scope->scope_id = scope_id++ ;
  current_scope = top_scope ;
  /*  Add all the system defined functions and classes here *
  /* 
    add_system_names() ;
  */
  first_verilog_block = 1 ;
}

/* flush out all the entry under a scope , obsolete, not used */
void flush_scope( named_node *scope )
{
  named_node *tmp, *this ;
  
  /* append the new name at the end of next_in_scope link */
  this = scope->child_scope ;
  while( this ) {
    tmp = this->next_in_scope ;
    flush_scope( this ) ;
    free( this->name ) ;
    free( this ) ;
    this = tmp ;
  }

}


/* rewind the scope state, before pass 1 */
void rewind_scope() {
  current_scope = top_scope ;
  scope_id = 1 ;
  first_verilog_block = 1 ;
}

void enter_scope( named_node *parent ) {
  named_node *tmp ;
  PASS1 {
    /* during pass 0, constructing scope tree structure */
    /* JD_name_dummy type is used to construct the scope tree */
    tmp = ALLOC(named_node) ;
    tmp->name = (char *)strdup("") ;
    tmp->type = JD_name_dummy ;
    if(parent) tmp->parent_scope = parent ;
    else tmp->parent_scope = current_scope ;  /* save current as parent */
    tmp->prev_scope = current_scope ;  /* this is where to exit */
    tmp->scope_id = scope_id++ ;
    current_scope = tmp ; /* new scope as current */
    
    /* link as parent's child_scope */
    tmp = current_scope->parent_scope->child_scope ;
    if( tmp ) {
      while( tmp->next_brother ) tmp = tmp->next_brother ;
      tmp->next_brother = current_scope ;
    }
    else current_scope->parent_scope->child_scope = current_scope ;
  }
  PASS2{
    if( parent ) tmp = parent->child_scope ;
    else tmp = current_scope->child_scope ;
    while( tmp ) {
      if( tmp->scope_id == scope_id ) break ;
      tmp = tmp->next_brother ;
    }
    if( tmp == NULL ) {
      ERROR_SCOPE_STACK_LINK_BROKEN ;
      fprintf( stderr, "Can't continue, exiting..\n" ) ;
      exit(1) ;
    }
    tmp->prev_scope = current_scope ;  /* this is where to exit */
    current_scope = tmp ;
    scope_id++ ;
  }
}

void enter_class_scope( named_node *parent ) {
  named_node *tmp ;
  PASS1 {
    /* during pass 0, constructing scope tree structure */
    /* JD_name_dummy type is used to construct the scope tree */
    if( parent->child_scope == NULL ) {
      tmp = ALLOC(named_node) ;
      tmp->name = (char *)strdup("") ;
      tmp->type = JD_name_dummy ;
      tmp->parent_scope = parent ;
      parent->child_scope = tmp ;
    }
    else tmp = parent->child_scope ;
    tmp->prev_scope = current_scope ;  /* this is where to exit */
    current_scope = tmp ; /* new scope as current */
    
    /* link as parent's child_scope */
  }
  PASS2{
    tmp = parent->child_scope ;
    if( tmp == NULL ) {
      ERROR_SCOPE_STACK_LINK_BROKEN ;
      fprintf( stderr, "Can't continue, exiting..\n" ) ;
      exit(1) ;
    }
    tmp->prev_scope = current_scope ;  /* this is where to exit */
    current_scope = tmp ;
  }
}

void exit_scope () {
  if( current_scope == NULL ) {
    ERROR_VARIABLE_SCOPE_STACK_UNDERFLOW ;
    fprintf( stderr, "Can't continue, exiting..\n" ) ;
    exit(1) ;
  }
  else current_scope = current_scope->prev_scope ;
}


void push_scope( named_node *parent, named_node *child ) {
  named_node *tmp ;
  PASS0_1 {
    if( child == NULL ) {
      child = ALLOC(named_node) ;
      child->name = (char *)strdup("") ;
      child->type = JD_name_dummy ;
    }
    child->parent_scope = parent ;
    child->scope_id = scope_id++ ;
    child->prev_scope = current_scope ; /* to pop scope */
    current_scope = child ; /* new scope as current */
    
    /* link as parent's child_scope */
    tmp = parent->child_scope ;
    if( tmp ) {
      while( tmp->next_brother ) tmp = tmp->next_brother ;
      tmp->next_brother = current_scope ;
    }
    else current_scope->parent_scope->child_scope = current_scope ;
  }
}

void pop_scope() {
  PASS0_1 {
    if( current_scope->prev_scope ) current_scope = current_scope->prev_scope ;
    else {
      ERROR_SCOPE_STACK_LINK_BROKEN ;
    }
  }
}


/* may not be needed? */
void addname_to_scope( named_node *scope, named_node *new )
{
  named_node *tmp ;
  
  /* append the new name at the end of next_in_scope link */
  tmp = scope ;
  while( tmp->next_in_scope ) tmp = tmp->next_in_scope ;
  tmp->next_in_scope = new ;

}

void addname( named_node *new )
{
  named_node *tmp ;
  
  if( current_scope == NULL ) {
    ERROR_VARIABLE_SCOPE_STRUCTURE_IS_BROKEN ;
  }
  else {
    /* append the new name at the end of next_in_scope link */
    tmp = current_scope ;
    while( tmp->next_in_scope ) tmp = tmp->next_in_scope ;
    tmp->next_in_scope = new ;
  }

}

named_node *findname_in_scope( named_node *scope, char *name ) {
  named_node *tmp ;
  
  tmp = scope ;
  while( tmp ) {
    /* search the name within the next_in_scope link */
    if( tmp->name && !strcmp(tmp->name, name ) ) return tmp ;
    tmp = tmp->next_in_scope ;
  }
  return NULL ;
}

void printname_in_scope( named_node *scope ) {
  named_node *tmp ;
  
  printf( "Names in this scope: " ) ;
  tmp = scope ;
  while( tmp ) {
    /* search the name within the next_in_scope link */
    if( tmp->name ) printf( "%s ", tmp->name ) ;
    tmp = tmp->next_in_scope ;
  }
  printf( "\n" ) ;
}


named_node *findmember_in_class( named_node *scope, char *name, int *offset ) {
  named_node *tmp ;
  
  *offset = 0 ;
  
  while( scope ) {
    tmp = scope->child_scope ;
    while( tmp ) {
      if( tmp->name && !strcmp(tmp->name, name ) ) return tmp ;
      tmp = tmp->next_in_scope ;
    }
    if( !strcmp( name, "new" ) ) break ; /* new must be solved in its scope */
    scope = scope->info.class_name.parent ;
    *offset = *offset + 1 ;
  }
  return NULL ;
  
}

named_node *findname_in_class( named_node *scope, char *name ) {
  named_node *tmp ;
  
  while( scope ) {
    if( scope->info.class_name.parent ) {
      tmp = scope->info.class_name.parent->child_scope ;
      while( tmp ) {
        if( tmp->name && !strcmp(tmp->name, name ) ) return tmp ;
        tmp = tmp->next_in_scope ;
      }
    }
    scope = scope->info.class_name.parent ;
  }
  
  return NULL ;

}

named_node *findname_in_classname( named_node *class, char *name ) {
  named_node *tmp ;
  
  if( class ) {
    tmp = class->child_scope ;
    while( tmp ) {
      if( tmp->name && !strcmp(tmp->name, name ) ) return tmp ;
      tmp = tmp->next_in_scope ;
    }
  }
  return NULL ;

}

named_node *findname( char *name ) {
  named_node *scope, *tmp ;
  
  scope = current_scope ;
  while( scope ) {
    tmp = scope ;
    while( tmp ) {
      if( tmp->name && !strcmp(tmp->name, name ) ) return tmp ;
      tmp = tmp->next_in_scope ;
    }
    if( scope->parent_scope && scope->prev_scope != scope->parent_scope &&
        scope->parent_scope->type == JD_class_name  )
    {
      tmp = findname_in_class( scope->parent_scope, name ) ;
      if( tmp ) return tmp ;
    }
    scope = scope->prev_scope ;
  }
  return NULL ;
}



/* find_user_type is called from lex to identify if a name is
 * declared as user defined data type.
 */
named_node *find_user_type( char *name ) 
{
  named_node *tmp ;
  
  if( tmp = findname( name ) ) {
    if( tmp->type == JD_enum_type ) return tmp ;
    else if( tmp->type == JD_class_name ) return tmp ;
    else if( tmp->type == JD_sys_class_name ) return tmp ;
    else return NULL ;
  }
  else return NULL ;
}

