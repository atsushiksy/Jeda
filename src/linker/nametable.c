/***************************************************************************
 *
 *  nametable.c: name handling functions for Jeda linker
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#include <stddef.h>
/* #include <errno.h>
#include <stdlib.h>
#include <time.h> */

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"

#define NAME_HASH_SIZE 256 

extern unsigned lex_line_num, lex_debug_line_num ;
extern char *input_file_name  ;
extern char *last_text ;

#ifdef PRODUCT_CODE
extern int main_defined ;
extern int module_linked ;
extern link_file_name *top_module_file ;
extern char *out_fname ;
#endif

JD_name **Global_name_top = NULL ;
JD_name **Local_name_top = NULL ;
JD_name **Verilog_name_top = NULL ;
JD_name **Static_name_top = NULL ;
JD_name **Local_static_name_top = NULL ;

static JD_name **init_hash() {
  return (JD_name **)calloc( sizeof(JD_name *), NAME_HASH_SIZE ) ;
}

static int hash_of_name( char *name ) {
  char *np ;
  int ret = 0 ;
  
  np = name ;
  while( *np != '\0' ) {
    ret += *np++ ;  
  }
  return ret % NAME_HASH_SIZE ;
}

JD_name *find_name_in_scope( char *name, JD_name **scope ) {
  int hash ;
  JD_name *nm ;
  
  if( scope == NULL ) return NULL ;
  
  hash = hash_of_name( name ) ;
  nm = scope[hash] ;
  
  while(nm) {
    if( !strcmp(nm->name, name ) ) break ;
    nm = nm->next ;
  }
  
  return nm ;
}

int num_member_in_scope( JD_name **scope ) {
  int i ;
  JD_name *nm ;
  int num = 0 ;
  if( scope == NULL ) return 0 ;
  
  for( i = 0 ; i < NAME_HASH_SIZE ; i++ ) {
    nm = scope[i] ;
    while( nm ) {
      if( nm->type == JD_variable_name || 
          nm->type == JD_enum_member_name ||
          nm->type == JD_mfunc_name ||
          nm->type == JD_port_name ) 
      {
        num++ ;
      }
      nm = nm->next ;
    }
  }
  return num ;
}

JD_name *find_name( char *name ) {
  JD_name *ret ;
  
  ret = find_name_in_scope( name, Local_name_top ) ;
  if( !ret ) ret = find_name_in_scope( name, Global_name_top ) ;
  return ret ;
}

JD_name *find_global_name( char *name ) {
  return find_name_in_scope( name, Global_name_top ) ;
}

JD_name *find_local_name( char *name ) {
  return find_name_in_scope( name, Local_name_top ) ;
}

JD_name *add_name_to_scope( char *name, JD_name ***scopep ) {
  int hash ;
  JD_name *nm, *new, **scp ;

  if( *scopep == NULL ) *scopep = init_hash() ;

  scp = *scopep ;
    
  new = ALLOC( JD_name ) ;
  new->name = name ;
  hash = hash_of_name( name ) ;
  nm = scp[hash] ;
  
  if( nm ) {
    /* fprintf( stderr, "name %s hash %d hits\n", name, hash ) ; */
    while(nm->next) {
      if( nm == nm->next ) yyerror( "name table looping" ) ;
      nm = nm->next ;
    }
    nm->next = new ;
  }
  else scp[hash] = new ;
  
  return new ;
}

JD_name *add_global_name( char *name ){
  return add_name_to_scope( name, &Global_name_top ) ;
}

JD_name *add_local_name( char *name ){
  return add_name_to_scope( name, &Local_name_top ) ;
}

JD_name *add_static_name( char *cls, char *fnc, char *name ){
  int len ;
  char *full_name ;
  
  len = strlen(cls)+strlen(fnc)+strlen(name)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, name ) ;
  return add_name_to_scope( full_name, &Static_name_top ) ;
}

JD_name *add_local_static_name( char *cls, char *fnc, char *name ){
  int len ;
  char *full_name ;
  
  len = strlen(cls)+strlen(fnc)+strlen(name)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, name ) ;
  return add_name_to_scope( full_name, &Local_static_name_top ) ;
}

JD_name *find_static_name( char *cls, char *fnc, char *name ) {
  int len ;
  char *full_name ;
  JD_name *ret ;
  
  len = strlen(cls)+strlen(fnc)+strlen(name)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, name ) ;
  ret =  find_name_in_scope( full_name, Static_name_top ) ;
  free( full_name ) ;
  return ret ;
}

JD_name *find_local_static_name( char *cls, char *fnc, char *name ) {
  int len ;
  char *full_name ;
  JD_name *ret ;
  
  len = strlen(cls)+strlen(fnc)+strlen(name)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, name ) ;
  ret =  find_name_in_scope( full_name, Local_static_name_top ) ;
  free( full_name ) ;
  return ret ;
}

JD_name *add_child_name( char *name, JD_name *parent ){
  return add_name_to_scope( name, &parent->child ) ;
}

JD_name *find_verilog_name( char *name ) {
  return find_name_in_scope( name, Verilog_name_top ) ;
}

JD_name *add_verilog_name( char *name ){
  return add_name_to_scope( name, &Verilog_name_top ) ;
}

JD_name **Debug_name_top = NULL ;

static int debug_name_index = 0 ;

JD_name *Debug_name_head = NULL ;
JD_name *Debug_name_tail = NULL ;

int debug_name_lookup( char *name ) {
  JD_name *ret ;
  ret =  find_name_in_scope( name, Debug_name_top ) ;
  if( ret ) return ret->index ;
  else ret = add_name_to_scope( name, &Debug_name_top ) ;
  ret->index = debug_name_index++ ;
  
  if( Debug_name_tail ) {
    Debug_name_tail->link = ret ;
    Debug_name_tail = ret ;
  }
  else {
    Debug_name_head = Debug_name_tail = ret ;
  }
  return ret->index ;
}

void check_name_definition( JD_name **scope ) {
  int i, j ;
  JD_name *nm, *mnm ;
  
  if( !scope ) return ;
  
  if( !main_defined || module_linked ) return ;
  for( i = 0 ; i < NAME_HASH_SIZE ; i++ ) {
    nm = scope[i] ;
    while( nm ) {
      switch( nm->type ) {
        case JD_func_name:
          if( !nm->defined ) {
            WARNING_FUNCTION_BODY_NOT_FOUND( nm->name ) ;
          }
          break ;
        case JD_class_name:
          if( !nm->defined ) {
            WARNING_CLASS_BODY_NOT_FOUND( nm->name ) ;
          }
          else if( nm->child ) {
            for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
              mnm = nm->child[j] ;
              while(mnm) {
                /* working here, not warn if abstract function */
                if( !mnm->defined ){
                  if( !is_info_abstract( mnm->info ) ) {
                    WARNING_CLASS_MEMBER_BODY_NOT_FOUND( nm->name, mnm->name ) ;
                  }
                }
                mnm = mnm->next ;
              }
            } 
          }
          break ;
      }
      nm = nm->next ;
    }
  }

}

void check_global_name() {
  check_name_definition( Global_name_top ) ;
}

void check_local_name( vcode_info *vc ) {
  
  check_name_definition( Local_name_top ) ;
  
  if( vc ) vc->local_name_top = Local_name_top ;
  
  Local_name_top = NULL ;
  Local_static_name_top = NULL ;
}
