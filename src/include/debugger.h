/*
  Data Structures for debugger
 */

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

#define SKIP_ON_ERROR if( JD_dbg_eval_error ) break 
#define DEFAULT_VAL_SIZE 32

#define MIN_STR_BUFF 80

typedef struct JD_dbg_expression_node_s JD_dbg_expression_node ;
typedef struct JD_dbg_name_node_s JD_dbg_name_node ;
typedef struct JD_dbg_break_point_s JD_dbg_break_point ;

typedef enum {
  JD_dbg_b_minus_op,     /* expression '-' expression   */
  JD_dbg_b_plus_op,      /* expression '+' expression   */
  JD_dbg_b_times_op,     /* expression '*' expression   */
  JD_dbg_b_div_op,       /* expression '/' expression   */
  JD_dbg_b_mod_op,       /* expression '%' expression   */
  JD_dbg_b_and_op,       /* expression '&' expression   */
  JD_dbg_b_or_op,        /* expression '|' expression   */
  JD_dbg_b_eor_op,       /* expression '^' expression   */
  JD_dbg_b_nand_op,      /* expression '&~' expression  */
                        /* expression '~&' expression  */
  JD_dbg_b_nor_op,       /* expression '|~' expression  */
                        /* expression '~|' expression  */
  JD_dbg_b_neor_op,      /* expression '^~' expression  */
                        /* expression '~^' expression  */
  JD_dbg_b_land_op,      /* expression '&&' expression  */
  JD_dbg_b_lor_op,       /* expression '||' expression  */
  JD_dbg_b_rshift_op,    /* expression '>>' expression  */
  JD_dbg_b_urshift_op,   /* expression '>>>' expression */
  JD_dbg_b_lshift_op,    /* expression '<<' expression  */
  JD_dbg_b_lt_op,        /* expression '<' expression   */
  JD_dbg_b_gt_op,        /* expression '>' expression   */
  JD_dbg_b_eqeq_op,      /* expression '==' expression  */
  JD_dbg_b_le_op,        /* expression '<=' expression  */
  JD_dbg_b_ge_op,        /* expression '>=' expression  */
  JD_dbg_b_ne_op,        /* expression '!=' expression  */
  JD_dbg_b_Qne_op,        /* expression '!?=' expression  */
  JD_dbg_b_eqeqeq_op,    /* expression '===' expression */
  JD_dbg_b_neqeq_op,     /* pression '!==' expression   */
  JD_dbg_b_eqQeq_op,     /* expression '=?=' expression */
  JD_dbg_b_eqeqQ_op,     /* expression '==?' expression */
  JD_dbg_b_Qeqeq_op,     /* expression '?==' expression */
  JD_dbg_u_minus_op,       /* '-' expression              */
  JD_dbg_u_tilde_op,       /* '~' expression              */
  JD_dbg_u_not_op,         /* '!' expression              */
  JD_dbg_u_and_op,         /* '&' expression              */
  JD_dbg_u_or_op,          /* '|' expression              */
  JD_dbg_u_eor_op,         /* '^' expression              */
  JD_dbg_u_nand_op,        /* '~&' expression             */
                        /* '&~' expression             */
  JD_dbg_u_nor_op,         /* '~|' expression             */
                        /* '|~' expression             */
  JD_dbg_u_neor_op,        /* '^~' expression             */
                        /* '~^' expression             */
  JD_dbg_eqCeq_op,         /* exp '=:=' exp ':' exp       */
  JD_dbg_conditional_op   /*  '?' exp ':' exp            */
} JD_dbg_op ;

typedef enum {
  JD_dbg_exp_value,
  JD_dbg_exp_variable,
  JD_dbg_exp_port,
  JD_dbg_exp_portset,
  JD_dbg_exp_class,
  JD_dbg_exp_enum,
  JD_dbg_exp_func,
  JD_dbg_exp_aspect,
  JD_dbg_exp_advice,
  JD_dbg_exp_array_range,
  JD_dbg_exp_string_array_range,
  JD_dbg_exp_bit_range,
  JD_dbg_exp_single_bit_range
} JD_dbg_exp_type ;

struct JD_dbg_expression_node_s {
  JD_dbg_exp_type etype ;
  JD_data_type type ;
  int int_value ;
  double real_value ;
  int size ;
  int *bit_a ;
  int *bit_b ;
  char *string ;  /* always store the exclusive string data, should be freed */
  char *name ;    /* just store the pointer, should not be freed */
  char *pname ;   /* just store the pointer, should not be freed */
  JD_data *dt ;
  JD_data *left ;
  JD_dbg_expression_node *depth ;
  JD_dbg_expression_node *array_index ;
  JD_dbg_expression_node *bit_index ;
  JD_dbg_expression_node *next ;
  JD_dbg_expression_node *next_level ;
  JD_dbg_name_node *scope ;
  int index ;
  int dt_enum_flag ;  /* set if value is enum member. string holds the name */
} ;

typedef enum {
  JD_dbg_empty_type,
  JD_dbg_func_type,
  JD_dbg_class_type,
  JD_dbg_block_type,
  JD_dbg_portset_type,
  JD_dbg_port_type,
  JD_dbg_var_type,
  JD_dbg_arg_type,
  JD_dbg_enum_type,
  JD_dbg_mvar_type,  
  JD_dbg_avar_type,  
  JD_dbg_advice_type,
  JD_dbg_aspect_type  
}  JD_dbg_name_node_type ;

typedef enum {
  JD_dbg_global_scope_type,
  JD_dbg_local_scope_type,
  JD_dbg_active_scope_type  
}  JD_dbg_name_node_scope_type ;

struct JD_dbg_name_node_s {
  JD_dbg_name_node_type type ;
  JD_dbg_name_node_scope_type scope ;
  int index ;
  int name ;
  int dtype ;
  int level ;
  int ub ;
  int lb ;
  JD_dbg_name_node *parent ;
  JD_dbg_name_node *next ;
} ;

#define BREAK_POINT_HASH_SIZE 256 

#define BREAK_POINT_HASH(x,y,z) (( x + y + z ) % BREAK_POINT_HASH_SIZE) 

struct JD_dbg_break_point_s {
  int  cwdnum ;
  int  filenum ;
  int  linenum ;
  /* link in the static table */
  JD_dbg_break_point *next ;
  /* link in the same file */
  JD_dbg_break_point *nextpoint ;
  /* link in the breakpoint */
  JD_dbg_break_point *fw ;
  JD_dbg_break_point *bw ;
  /* link in the breakpoint chain */
  JD_dbg_break_point *next_list ;
  int set ;        /* indicate this entry is already set */
  int temp_flag ;  /* indicate this is temp break, cleard on the break */
} ;

void JD_dbg_push( JD_dbg_expression_node *exp ) ;
JD_dbg_expression_node *JD_dbg_pop() ;
JD_dbg_expression_node *JD_dbg_alloc_exp() ;
void JD_dbg_free_exp( JD_dbg_expression_node *exp ) ;
