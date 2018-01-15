/***************************************************************************
 *
 *  parse_types.h: Various enum declarations for parse tree
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
#ifndef INC_PARSE_TYPES

#define INC_PARSE_TYPES

typedef enum {
  JD_null_statement,
  JD_if_statement,
  JD_elsif_statement,
  JD_else_statement,
  JD_while_statement,
  JD_for_statement, 
  JD_repeat_statement, 
  JD_do_while_statement, 
  JD_forever_statement, 
  JD_case_statement, 
  JD_case_item, 
  JD_case_default, 
  JD_casex_statement, 
  JD_casez_statement, 
  JD_randcase_statement, 
  JD_break_statement, 
  JD_continue_statement,
  JD_fork_join_statement,
  JD_terminate_statement,
  JD_breakpoint_statement,
  JD_expression_statement,
  JD_assign_statement,
  JD_function_call_statement,
  JD_return_statement,
  JD_block,
  JD_statement_list,
  JD_initial_statement,
  JD_portconnect_statement,
  JD_vardeclare_statement,
  JD_formal_arg_statement,
  JD_one_or_more_arg_statement,
  JD_zero_or_more_arg_statement,
  JD_wildcard_arg_statement,
  JD_or_of_args_statement,
  JD_opt_args_statement,
  JD_enum_def_statement,
  JD_verilog_task_call_statement,
  JD_call_pointcut_statement,
  JD_return_pointcut_statement,
  JD_overwrite_pointcut_statement  
} JD_statement_type ;



typedef enum  {
  JD_port_def_exp,       
  JD_string_exp,       
  JD_timed_till_exp,       
  JD_timed_while_exp,       
  JD_parallel_or_exp,       
  JD_parallel_and_exp,       
  JD_varport_exp,       
  JD_null_exp,       
  JD_true_exp,       
  JD_false_exp,       
  JD_function_call_exp,       
  JD_verilog_task_call_exp,       
  JD_preincrement_exp,       
  JD_predecrement_exp,       
  JD_postincrement_exp,       
  JD_postdecrement_exp,       
  JD_concatenation_exp,       
  JD_duplication_exp,       
  JD_list_exp,
  JD_X_num, 
  JD_real_num,
  JD_int_num,
  JD_bit_num,
  JD_exp_minus_exp,     /* expression '-' expression   */
  JD_exp_plus_exp,      /* expression '+' expression   */
  JD_exp_times_exp,     /* expression '*' expression   */
  JD_exp_div_exp,       /* expression '/' expression   */
  JD_exp_mod_exp,       /* expression '%' expression   */
  JD_exp_and_exp,       /* expression '&' expression   */
  JD_exp_or_exp,        /* expression '|' expression   */
  JD_exp_eor_exp,       /* expression '^' expression   */
  JD_exp_nand_exp,      /* expression '&~' expression  */
                        /* expression '~&' expression  */
  JD_exp_nor_exp,       /* expression '|~' expression  */
                        /* expression '~|' expression  */
  JD_exp_neor_exp,      /* expression '^~' expression  */
                        /* expression '~^' expression  */
  JD_exp_land_exp,      /* expression '&&' expression  */
  JD_exp_lor_exp,       /* expression '||' expression  */
  JD_exp_rshift_exp,    /* expression '>>' expression  */
  JD_exp_urshift_exp,   /* expression '>>>' expression */
  JD_exp_lshift_exp,    /* expression '<<' expression  */
  JD_exp_lt_exp,        /* expression '<' expression   */
  JD_exp_gt_exp,        /* expression '>' expression   */
  JD_exp_eqeq_exp,      /* expression '==' expression  */
  JD_exp_le_exp,        /* expression '<=' expression  */
  JD_exp_ge_exp,        /* expression '>=' expression  */
  JD_exp_ne_exp,        /* expression '!=' expression  */
  JD_exp_Qne_exp,        /* expression '!?=' expression  */
  JD_exp_eqeqeq_exp,    /* expression '===' expression */
  JD_exp_neqeq_exp,     /* pression '!==' expression   */
  JD_exp_eqQeq_exp,     /* expression '=?=' expression */
  JD_exp_eqeqQ_exp,     /* expression '==?' expression */
  JD_exp_Qeqeq_exp,     /* expression '?==' expression */
  JD_u_minus_exp,       /* '-' expression              */
  JD_u_tilde_exp,       /* '~' expression              */
  JD_u_not_exp,         /* '!' expression              */
  JD_u_and_exp,         /* '&' expression              */
  JD_u_or_exp,          /* '|' expression              */
  JD_u_eor_exp,         /* '^' expression              */
  JD_u_nand_exp,        /* '~&' expression             */
                        /* '&~' expression             */
  JD_u_nor_exp,         /* '~|' expression             */
                        /* '|~' expression             */
  JD_u_neor_exp,        /* '^~' expression             */
                        /* '~^' expression             */
  JD_eqCeq_exp,         /* exp '=:=' exp ':' exp       */
  JD_conditional_exp,   /*  '?' exp ':' exp            */
                        /* '(' expression ')'          */
  JD_new_exp,           /* new                         */
  JD_new_block_exp,     /* new  { <block }             */
  JD_port_exp,
  JD_portdef_exp,
  JD_port_attr_exp,
  JD_bit_range_exp,
  JD_single_bit_range_exp,
  JD_single_range_exp,
  JD_string_range_exp,
  JD_assoc_array_exp,
  JD_stridx_array_exp,
  JD_list_array_exp,
  JD_list_assoc_array_exp,
  JD_statement_prefix_exp,
  JD_CLOCK_exp,
  JD_assign_exp,
  JD_assign_list_exp,
  JD_super_exp,
  JD_this_exp,
  JD_default_arg,
  JD_aspect_exp,
  JD_pointcut_name,
  JD_return_exp,
  JD_rnd_exp,
  JD_cyc_exp,
  JD_rand_val_exp,
  JD_rand_range_exp,
  JD_rand_size_exp,
  JD_rand_limit_exp,
  JD_rand_name_exp,
  JD_rand_cond_lt,
  JD_rand_cond_gt,
  JD_rand_cond_le,
  JD_rand_cond_ge,
  JD_rand_cond_ne,
  JD_rand_cond_Qeq,
  JD_rand_cond_Qne
} JD_expression_type ;


typedef enum {
  JD_exp_unknown_type,
  JD_exp_num_type,
  JD_exp_portset_type,
  JD_exp_string_type,
  JD_exp_bool_type,
  JD_exp_void_type,
  JD_exp_object_type,
  JD_exp_enum_type,
  JD_exp_new_type,
  JD_exp_new_block_type,
  JD_exp_array_type,
  JD_exp_marray_type,
  JD_exp_assoc_type,
  JD_exp_stridx_type,
  JD_exp_list_type,
  JD_exp_signal_type,
  JD_exp_aspect_type,
  JD_exp_return_type
} JD_exp_data_type ;

typedef enum {
  JD_join,
  JD_spoon,
  JD_knife
} JD_join_type ;

typedef enum {
  JD_assign_EQ,        /* = */ 
  JD_assign_NB_EQ,     /* <= */ 
  JD_assign_Pls_EQ,    /* += */ 
  JD_assign_Minus_EQ,  /* -= */ 
  JD_assign_Time_EQ,   /* *= */ 
  JD_assign_Div_EQ,    /* /= */ 
  JD_assign_Mod_EQ,    /* %= */  
  JD_assign_Rsft_EQ,   /* >>= */ 
  JD_assign_Ursft_EQ,  /* >>>= */ 
  JD_assign_Lsft_EQ,   /* <<= */ 
  JD_assign_And_EQ,    /* &= */ 
  JD_assign_Or_EQ,     /* |= */ 
  JD_assign_Eor_EQ,    /* ^= */ 
  JD_assign_Nand_EQ,   /* ~&= &~= */ 
  JD_assign_Nor_EQ,    /* ~|= |~= */
  JD_assign_Neor_EQ    /* ~^= ^~= */ 
} JD_assign_operator ;

typedef enum {
  JD_port_block,
  JD_portset_block,
  JD_connectionset_block,
  JD_local_var_dcl,
  JD_global_var_dcl,
  JD_initial_statement_block,
  JD_statement_block,
  JD_type_definition,
  JD_local_variable_definition,
  JD_function_definition,
  JD_memberfunction_definition,
  JD_class_definition,
  JD_verilog_class,
  JD_verilog_function,
  JD_aspect_definition,
  JD_advice_definition,
  JD_block_st
} JD_block_type ;

typedef enum {
  JD_unknown_name = 0,
  JD_void_type,
  JD_int_type,
  JD_long_type,
  JD_float_type,
  JD_double_type,
  JD_bit_type,
  JD_bool_type,
  JD_object_type,
  JD_string_type,
  JD_signal_type,
  JD_same_type,  /* special type that has same type as instance, old scheme */
  JD_class_type,  /* special type that has same type as instance */
  JD_dport_type,
  JD_enum_type,
  JD_arg_name,
  JD_var_name,
  JD_func_name,
  JD_class_name,
  JD_sys_class_name,
  JD_portset_name,
  JD_port_name,
  JD_mvar_name,
  JD_mfunc_name,
  JD_label_name,
  JD_enum_name,
  JD_enum_member_name,
  JD_aspect_name,
  JD_advice_name,
  JD_name_dummy
} JD_data_name_type ;

typedef enum {
  JD_single_sub_type,
  JD_array_sub_type,
  JD_multi_array_sub_type,
  JD_assoc_sub_type,
  JD_stridx_sub_type,
  JD_list_sub_type
} JD_sub_type ;

typedef enum {
  JD_port_input,
  JD_port_output,
  JD_port_inout,
  JD_port_set
} JD_port_type ;

typedef enum {
  JD_noedge = 0,
  JD_posedge,
  JD_negedge,
  JD_bothedge,
  JD_async,
  JD_clockedge
} JD_edge_type ;

typedef enum {
  JD_port_clock,
  JD_port_sample,
  JD_port_drive,
  JD_port_depth,
  JD_port_node
} JD_port_attr ;

typedef enum {
  JD_local_type,
  JD_global_type,
  JD_static_type,
  JD_protected_type,
  JD_virtual_type,
  JD_final_type
} JD_vf_attrib ;

typedef enum {
  JD_normal,
  JD_weak,
  JD_strong
} JD_strength ;


#endif /* INC_PARSE_TYPES */
