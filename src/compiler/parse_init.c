/***************************************************************************
 *
 *  parse_init.c: functions to initialize the compiler parse tree
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

extern int scope_id ;

static void add_clock_port() {
  named_node *clock ;
  
  clock = ALLOC(named_node) ;
  clock->type = JD_port_name ;
  clock->name = (char *)strdup( "CLOCK" ) ;
  clock->filename = (char *)strdup( "<system init>" ) ;
  clock->linenum = 0 ;
  addname( clock ) ;

}

named_node *pinit_add_class( char *name ) {
  named_node *pt ;
  named_node *tmp ;

  pt = ALLOC(named_node) ;
  pt->type = JD_sys_class_name ;
  pt->name = (char *)strdup( name ) ;
  pt->filename = (char *)strdup( "<system init>" ) ; ;
  pt->linenum = 0 ;
  addname( pt ) ;
  tmp = ALLOC(named_node) ;
  tmp->name = (char *)strdup("") ;
  tmp->type = JD_name_dummy ;
  tmp->parent_scope = pt ;
  tmp->prev_scope = current_scope ;  /* this is where to exit */
  tmp->scope_id = scope_id++ ;
  pt->child_scope = tmp ;
  return pt ;
}

void add_verilog_class()
{
  (void)pinit_add_class( "verilog" ) ;  
}


void parse_init() {

  add_clock_port() ;
  add_verilog_class() ; 
  
  /* Other sytem funcs/tasks are set by parsing jeda_utils.jdh. */
  
}
