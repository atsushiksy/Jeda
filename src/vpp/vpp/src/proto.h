/* Copyright (C) 1996 Himanshu M. Thaker

This file is part of vpp.

Vpp is distributed in the hope that it will be useful,
but without any warranty.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.

Everyone is granted permission to copy, modify and redistribute
vpp, but only under the conditions described in the
document "vpp copying permission notice".   An exact copy
of the document is supposed to have been given to you along with
vpp so that you can know how you may redistribute it all.
It should be in a file named COPYING.  Among other things, the
copyright notice and this notice must be preserved on all copies.  */

expr *create_identifier();
expr *create_expression();
expr *create_primary();
void traverse_expression();
void print_operand();
expr_list *create_expression_list();
t_ident_ref *create_ident_ref();
stmt *create_statement();

int set_to_vpp_mode();
int set_to_initial_mode();
varp *create_variable_ref();
var_values *create_variable();
void insert_variable_into_table();
var_values *get_variable_ref();
void free_expr();

gen_ll *create_generic_ll();
gen_ll *append_generic_ll();
gen_ll *push();
gen_ll *pop();

gen_ll *create_if_structure();

char *double2ascii();

double evaluate_expression();
double perform_operand();
double get_define_value();
