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
void init_scope() ;
void flush_scope( named_node *scope ) ;
void rewind_scope() ;
void enter_scope( named_node *parent ) ;
void enter_class_scope( named_node *parent ) ;
void exit_scope () ;
void push_scope( named_node *parent, named_node *child ) ;
void pop_scope() ;
void addname_to_scope( named_node *scope, named_node *new );
void addname( named_node *new ) ;
named_node *findmember_in_class( named_node *scope, char *name, int *offset ) ;
named_node *findname_in_scope( named_node *scope, char *name ) ;
named_node *findname_in_class( named_node *scope, char *name ) ;
named_node *findname( char *name ) ;
named_node *find_user_type( char *name ) ;

named_node *findname_in_classname( named_node *class, char *name ) ;


