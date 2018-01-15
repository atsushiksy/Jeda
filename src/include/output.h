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
void output_initial() ;
void output_data_type( named_node *type ) ;
void output_array_range( expression_node *range ) ;
void output_func_top( named_node *funk ) ;
void output_func_arg( statement_node *arg, int level, int index ) ;
int output_func_args( statement_node *args, int offset ) ;
void output_block_statement( statement_node *blk, int id, int level ) ;
void output_statement( statement_node *stmt, int level ) ;
void output_block( statement_node *blk, int level ) ;
void output_statement_debug_info( statement_node *stmt, int id ) ;
char *eval_expression( expression_node *exp, int id ) ;
void eval_args( expression_node *args, int id ) ;
void output_pop_this() ;
void output_return() ;
void output_global_local_var( named_node *variable, int local ) ;
