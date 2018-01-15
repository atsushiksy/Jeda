%{
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
#include <stdlib.h>
#include <string.h>

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"

extern char *input_file_name ;

extern int error_count ;
extern int error_flag ;

extern int parse_sys_utils ;

extern unsigned lex_line_num ;

static char *filename ;
static int linenum ;

/* counting variables to allocate global space
   index 0 is prealocated for CLOCK port
 */
int num_global_var = 1 ;

int yyerrflag ;

int inst_num = 0 ;

vcode_info *Current_Class = NULL ;


vcode_info  *JD_top_inst ;
vcode_info  *JD_first_inst = NULL ;

int main_defined = 0 ;

%}

%union {
  int  integer_value ;
  char *string ;
  struct {
    char *name ;
    int  linenum ;
    char *filename ;
    int  sys_flag ;
  } string_value ;
  int keyword ;
  vcode_info *inst ;
  vinst_info *code ;
  expression_node *exp ;
  JD_name *name ;
  /* instruction_node  *inst ; */
}

%token <string_value> Identifier 
%token <string_value> Name 
%token <string_value> Real_num         /* real */
%token <string_value> Decimal_num      /* number */
%token <string_value> V_decimal_num    /* verilog format decimal */
%token <string_value> C_decimal_num    /* C format decimal */
%token <string_value> V_hex_num        /* verilog format hex */
%token <string_value> C_hex_num        /* C format hex */
%token <string_value> V_octal_num      /* verilog format octal */
%token <string_value> C_octal_num      /* C format octal */
%token <string_value> V_bin_num        /* verilog format binary */
%token <string_value> C_bin_num        /* C format binary */
%token <string_value> String_lit           /* string */
%token <string_value> DebugInfo        /* string */
%token <keyword> EOL   


/* #include is not activated while yaccing.
   #INCLUDE is handled by gen_parser.pl. But this must be a non-nested file
 */
%token <keyword> VI_Qeqeq
%token <keyword> VI_abstract
%token <keyword> VI_aspect
%token <keyword> VI_advice
%token <keyword> VI_advice_end
%token <keyword> VI_afunc
%token <keyword> VI_alloc
%token <keyword> VI_alloc_func
%token <keyword> VI_alloc_static
%token <keyword> VI_alu
%token <keyword> VI_and
%token <keyword> VI_arg
%token <keyword> VI_array
%token <keyword> VI_assoc
%token <keyword> VI_bit
%token <keyword> VI_bothedge
%token <keyword> VI_breakpoint
%token <keyword> VI_call
%token <keyword> VI_call_m
%token <keyword> VI_call_a
%token <keyword> VI_call_advice
%token <keyword> VI_cancel
%token <keyword> VI_chk_overwrite
%token <keyword> VI_class
%token <keyword> VI_class_alloc
%token <keyword> VI_class_alloc_end
%token <keyword> VI_class_initial_begin
%token <keyword> VI_class_initial_end
%token <keyword> VI_class_type
%token <keyword> VI_clk_of_port
%token <keyword> VI_clk_of_pset_port
%token <keyword> VI_clk_of_signal
%token <keyword> VI_clone
%token <keyword> VI_cmp
%token <keyword> VI_cmpx
%token <keyword> VI_cmpz
%token <keyword> VI_concat
%token <keyword> VI_connection
%token <keyword> VI_copy
%token <keyword> VI_copy_obj
%token <keyword> VI_convert
%token <keyword> VI_dec
%token <keyword> VI_depth
%token <keyword> VI_div
%token <keyword> VI_double
%token <keyword> VI_drive
%token <keyword> VI_dup
%token <keyword> VI_duplicate
%token <keyword> VI_enter_advice
%token <keyword> VI_enum
%token <keyword> VI_eor
%token <keyword> VI_eqQeq
%token <keyword> VI_eqeq
%token <keyword> VI_eqeqQ
%token <keyword> VI_eqeqeq
%token <keyword> VI_eqCeq
%token <keyword> VI_Qne
%token <keyword> VI_error
%token <keyword> VI_exec
%token <keyword> VI_exit
%token <keyword> VI_export
%token <keyword> VI_extends
%token <keyword> VI_extern
%token <keyword> VI_final
%token <keyword> VI_float
%token <keyword> VI_fork
%token <keyword> VI_formal
%token <keyword> VI_func
%token <keyword> VI_function
%token <keyword> VI_func_end
%token <keyword> VI_ge
%token <keyword> VI_gen_child
%token <keyword> VI_gen_frame
%token <keyword> VI_gen_m_frame
%token <keyword> VI_gen_a_frame
%token <keyword> VI_get_this
%token <keyword> VI_get_sig_subbit
%token <keyword> VI_get_subbit
%token <keyword> VI_get_super
%token <keyword> VI_get_child
%token <keyword> VI_get_return
%token <keyword> VI_global
%token <keyword> VI_gt
%token <keyword> VI_i_endian
%token <keyword> VI_inc
%token <keyword> VI_input
%token <keyword> VI_inout
%token <keyword> VI_initial_begin
%token <keyword> VI_initial_end
%token <keyword> VI_initial_init
%token <keyword> VI_int
%token <keyword> VI_jedacmp
%token <keyword> VI_jmp
%token <keyword> VI_jnz
%token <keyword> VI_join
%token <keyword> VI_jz
%token <keyword> VI_knife
%token <keyword> VI_le
%token <keyword> VI_leaf
%token <keyword> VI_list
%token <keyword> VI_load_const
%token <keyword> VI_load_one
%token <keyword> VI_load_port
%token <keyword> VI_load_port_ptr
%token <keyword> VI_load_pset_port
%token <keyword> VI_load_pset_port_ptr
%token <keyword> VI_load_sig_pt
%token <keyword> VI_load_static
%token <keyword> VI_load_super
%token <keyword> VI_load_this
%token <keyword> VI_load_thread
%token <keyword> VI_load_zero
%token <keyword> VI_loadg
%token <keyword> VI_loadl
%token <keyword> VI_loadm
%token <keyword> VI_local
%token <keyword> VI_long
%token <keyword> VI_lshift
%token <keyword> VI_lt
%token <keyword> VI_lvar
%token <keyword> VI_marray
%token <keyword> VI_member_signal
%token <keyword> VI_mfunc
%token <keyword> VI_minus
%token <keyword> VI_mod
%token <keyword> VI_nand
%token <keyword> VI_ne
%token <keyword> VI_negedge
%token <keyword> VI_neor
%token <keyword> VI_neqeq
%token <keyword> VI_new
%token <keyword> VI_new_obj
%token <keyword> VI_noedge
%token <keyword> VI_nor
%token <keyword> VI_normal
%token <keyword> VI_null
%token <keyword> VI_nz_lsb
%token <keyword> VI_obj
%token <keyword> VI_or
%token <keyword> VI_output
%token <keyword> VI_overwrite
%token <keyword> VI_p_and
%token <keyword> VI_p_or
%token <keyword> VI_path
%token <keyword> VI_plus
%token <keyword> VI_pop
%token <keyword> VI_pop_this
%token <keyword> VI_port
%token <keyword> VI_portset
%token <keyword> VI_posedge
%token <keyword> VI_protected
%token <keyword> VI_pset_port
%token <keyword> VI_ptset
%token <keyword> VI_push
%token <keyword> VI_restore_acc
%token <keyword> VI_return
%token <keyword> VI_return_advice
%token <keyword> VI_return_l
%token <keyword> VI_return_n
%token <keyword> VI_rshift
%token <keyword> VI_sample
%token <keyword> VI_save_acc
%token <keyword> VI_signal
%token <keyword> VI_single
%token <keyword> VI_skew
%token <keyword> VI_spoon
%token <keyword> VI_stat_var
%token <keyword> VI_static
%token <keyword> VI_static_init
%token <keyword> VI_static_init_end
%token <keyword> VI_store_adjust
%token <keyword> VI_store_obj
%token <keyword> VI_store_port
%token <keyword> VI_store_pset_port
%token <keyword> VI_store_sig_g
%token <keyword> VI_store_sig_l
%token <keyword> VI_store_sig_m
%token <keyword> VI_store_sig_static
%token <keyword> VI_store_static
%token <keyword> VI_storeg
%token <keyword> VI_storel
%token <keyword> VI_storem
%token <keyword> VI_stridx
%token <keyword> VI_string
%token <keyword> VI_strong
%token <keyword> VI_subbit
%token <keyword> VI_sync
%token <keyword> VI_sys_class
%token <keyword> VI_terminate
%token <keyword> VI_times
%token <keyword> VI_to_int
%token <keyword> VI_u_and
%token <keyword> VI_u_eor
%token <keyword> VI_u_minus
%token <keyword> VI_u_nand
%token <keyword> VI_u_neor
%token <keyword> VI_u_nor
%token <keyword> VI_u_not
%token <keyword> VI_u_or
%token <keyword> VI_u_tilde
%token <keyword> VI_urshift
%token <keyword> VI_var
%token <keyword> VI_variable
%token <keyword> VI_veri_call
%token <keyword> VI_verilog_task
%token <keyword> VI_void
%token <keyword> VI_weak

%type <integer_value>	int_value var_attrib 
%type <integer_value>	func_option opt_var 
%type <integer_value>	opt_static edge 
%type <integer_value>	type_code direction 
%type <integer_value>	class_attrib strength 
%type <string_value>	name label 
%type <string>	opt_extention 
%type <exp>	const_data number 
%type <inst>	object_files verilog_task_info 
%type <inst>	class_attribs opt_class_attribs 
%type <inst>	empty_line debug_information 
%type <inst>	virtual_code inital_header 
%type <inst>	vinst_units vinst_unit 
%type <inst>	port_info opt_port_attrib 
%type <inst>	port_attribs port_attrib 
%type <inst>	portset_info port_infos 
%type <inst>	connection_info variable_dcl 
%type <inst>	data_type array_range 
%type <inst>	range_list opt_var_attribs 
%type <inst>	var_attribs enum_dcl 
%type <inst>	enum_members function_info 
%type <inst>	opt_func_options func_options 
%type <inst>	opt_arguments arguments 
%type <inst>	argument class_info 
%type <inst>	opt_class_members class_members 
%type <inst>	class_member aspect_info 
%type <inst>	advice_info pointcut_name 
%type <inst>	pointcuts pointcut 
%type <inst>	pointcut_names opt_aspect_members 
%type <inst>	aspect_members aspect_member 
%type <inst>	static_var_dcl static_init_code 
%type <inst>	init_code class_init_code 
%type <inst>	class_alloc_code mfunc_code 
%type <inst>	func_code afunc_code 
%type <inst>	advice_code stype 
%type <inst>	dtype ttype 
%type <inst>	opt_flags opt_array_access 
%type <inst>	opt_bit_type 
%type <code>	opt_vinsts vinsts 
%type <code>	vinst 
%type <name>	pointcut_specs pointcut_spec 
%type <name>	opt_class_pointcut 


%start object_files

%%

object_files:
  virtual_code
  {
    /* object_file */
    JD_top_inst = $$ = $1 ;
  }
  |
  object_files virtual_code
  {
    /* ObjectChain */
    vcode_info *last = $1 ;
    if( $1 ) {
      while(last->next) last = last->next ;
      last->next = $2 ;
      $$ = $1 ;
    }
    else JD_top_inst = $$ = $2 ;
  }
  ;
  
virtual_code:  
  inital_header 
  vinst_units
  {
    $$ = $2 ; 
  }
  ;

inital_header:
  VI_jedacmp Decimal_num Decimal_num EOL
  {
    /* inital_header */
    int version, revision ;
    
    version = atoi($2.name) ;
    revision = atoi($3.name) ;
    
    if( version > 0 && revision > 0 ) {
      /* do the version & revision consistency check */
      /* TBI */ 
    
    }
    
  }
  ;
  
vinst_units:
  vinst_unit
  {
    $$ = $1 ; 
  }
  |
  vinst_units vinst_unit
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;
  
vinst_unit:
  empty_line
  {
    $$ = NULL ;
  }
  |
  debug_information
  {
    $$ = $1 ; 
  }
  |
  port_info
  {
    /* top_port_info */
    JD_name *name ;
    $$ = $1 ; 
    name = find_global_name( $$->is.port.name ) ;
    if( !name ) {
      name = add_global_name( $$->is.port.name ) ;
      name->info = $$ ;
      name->type = JD_port_name ;
      $$->code_name = name ;
    }
    else {
      if( name->type != JD_port_name ) {
        ERROR_PORT_NAME_TYPE_MISMATCH( $$->is.port.name ) ;
      }
      else if( !comp_info( name->info, $$ ) ) {
        ERROR_PORT_NAME_ATTRIB_MISMATCH( $$->is.port.name ) ;
      }
      $$ = NULL ;
    }
  }
  |
  portset_info
  {
    /* top_portset_info */
    JD_name *name, *ptname ;
    vcode_info *pt ;
    int lflag = 1 ;
    $$ = $1 ; 
    name = find_global_name( $$->is.portset.name ) ;
    if( !name ) {
      name = add_global_name( $$->is.portset.name ) ;
      name->info = $$ ;
      name->type = JD_portset_name ;
      pt = $$->is.portset.port_infos ;
      while( pt ) {
        ptname = add_name_to_scope( pt->is.port.name, &name->child ) ;
        ptname->type = JD_port_name ;
        ptname->info = pt ;
        pt->is.port.portset = $$ ;
        pt->code_name = ptname ;
        pt = pt->next ;
      }
    }
    else {
      if( name->type != JD_portset_name ) {
        ERROR_PORTSET_NAME_TYPE_MISMATCH( $$->is.portset.name ) ;
      }
      comp_portset_info( $$, name ) ;
      $$ = NULL ;
    }
  }
  |
  connection_info
  {
    $$ = $1 ; 
  }
  |
  variable_dcl
  {
    /* top_variable_dcl */
    JD_name *name ;
    vcode_info *attrib ;
    
    $$ = $1 ; 
    attrib = $1->is.variable_dcl.var_attrib ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( $$->is.variable_dcl.name ) ;
    else
      name = find_global_name( $$->is.variable_dcl.name ) ;
    
    if( name ) {
      if( name->type != JD_variable_name ) {
        ERROR_VARIABLE_NAME_TYPE_MISMATCH( $$->is.variable_dcl.name ) ;
      }
      else if( !comp_info( name->info, $$ ) ) {
        ERROR_VARIABLE_NAME_ATTRIB_MISMATCH( $$->is.variable_dcl.name ) ;
      }
      $$ = NULL ;
    }
    else {
      if( attrib && attrib->is.attrib.is_local ) {
        name = add_local_name( $$->is.variable_dcl.name ) ;
      }
      else {
        name = add_global_name( $$->is.variable_dcl.name ) ;
      }
      name->type = JD_variable_name ;
      name->info = $$ ;
      $$->code_name = name ;
    }
  }
  |
  enum_dcl
  {
    /* top_enum_dcl */
    JD_name *name, *mnname ;
    vcode_info *mn ;
    int  index ;
    $$ = $1 ;
    name = find_global_name( $1->is.enum_dcl.name ) ;
    if( !name ) {
      name = add_global_name( $1->is.enum_dcl.name ) ;
      name->info = $1 ;
      name->type = JD_enum_name ;
      mn = $1->is.enum_dcl.members ;
      index = 0 ;
      while( mn ) {
        mnname = add_name_to_scope( mn->is.string, &name->child ) ;
        mnname->info = mn ;
        mnname->type = JD_enum_member_name ;
        mnname->index = index++ ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_enum_name ) {
        ERROR_ENUM_NAME_TYPE_MISMATCH( $1->is.enum_dcl.name ) ;
      }
      comp_enum_info( $1, name ) ;
      $$ = NULL ;
    }
  
  }
  |
  function_info
  {
    /* top_function_info */
    JD_name *name ;
    vcode_info *mn, *attrib ;
    
    $$ = $1 ;
    attrib = $1->is.function_info.func_options ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( $$->is.function_info.name ) ;
    else
      name = find_global_name( $$->is.function_info.name ) ;
    if( !name ) {
      if( attrib && attrib->is.attrib.is_local )
        name = add_local_name( $1->is.function_info.name ) ;
      else
        name = add_global_name( $1->is.function_info.name ) ;
      name->info = $1 ;
      if( $1->is.function_info.sys_flag ) name->type = JD_sys_func_name ;
      else name->type = JD_func_name ;
    }
    else {
      if( name->type != JD_func_name ) {
        ERROR_FUNCTION_NAME_TYPE_MISMATCH( $1->is.function_info.name ) ;
      }
      comp_func_info( NULL, $1, name->info ) ;
      $$ = NULL ;
    }
  
  }
  |
  class_info
  {
    /* top_class_info */
    JD_name *name, *mname ;
    vcode_info *mn, *attrib ;
  
    $$ = $1 ;
    attrib = $1->is.class.attribs ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( $1->is.class.name ) ;
    else
      name = find_global_name( $1->is.class.name ) ;
    if( !name ) {
      if( attrib && attrib->is.attrib.is_local ) 
        name = add_local_name( $1->is.class.name ) ;
      else 
        name = add_global_name( $1->is.class.name ) ;
      name->info = $1 ;
      if( $1->is.class.sys_flag ) name->type = JD_sys_class_name ;
      else name->type = JD_class_name ;
      /* add member name to the class scope */
      mn = $1->is.class.members ;
      while( mn ) {
        mname = add_name_to_scope( get_info_name( mn ), &name->child ) ;
        mname->type = get_info_type( mn ) ;
        mname->info = mn ;
        mname->index = mn->index ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_class_name ) {
        ERROR_CLASS_NAME_TYPE_MISMATCH( $1->is.class.name ) ;
      }
      (void)comp_class( $1, name ) ;
      $$ = NULL ;
    }
  
  }
  |
  aspect_info
  {
    /* top_aspect_info */
    JD_name *name, *mname ;
    vcode_info *mn, *attrib ;
  
    $$ = $1 ;
    name = find_global_name( $1->is.class.name ) ;
    if( !name ) {
      name = add_global_name( $1->is.class.name ) ;
      name->info = $1 ;
      name->type = JD_aspect_name ;
      /* add member name to the class scope */
      mn = $1->is.class.members ;
      while( mn ) {
        mname = add_name_to_scope( get_info_name( mn ), &name->child ) ;
        mname->type = get_info_type( mn ) ;
        mname->info = mn ;
        mname->index = mn->index ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_aspect_name ) {
        ERROR_ASPECT_NAME_TYPE_MISMATCH( $1->is.function_info.name ) ;
      }
      (void)comp_aspect( $1, name ) ;
      $$ = NULL ;
    }
  
  }
  |
  verilog_task_info
  {
    /* top_verilog_task_info */
    JD_name *name ;
    vcode_info *mn ;
    
    $$ = $1 ;
    /* verilog task needs its own name scope */
    name = find_verilog_name( $1->is.function_info.name ) ;
    if( !name )
    {
      name = add_verilog_name( $1->is.function_info.name ) ;
      name->info = $1 ;
      name->type = JD_verilog_task_name ;
      $$->code_name = name ;
    }
    else {
      if( name->type != JD_verilog_task_name ) {
        ERROR_FUNCTION_NAME_TYPE_MISMATCH( $1->is.function_info.name ) ;
      }
      comp_veri_task_info( $1, name->info ) ;
      $$ = NULL ;
    }
  
  }
  |
  static_var_dcl
  {
    $$ = $1 ; 
  }
  |
  static_init_code
  {
    $$ = $1 ; 
  }
  |
  init_code
  {
    $$ = $1 ; 
  }
  |
  class_init_code
  {
    $$ = $1 ; 
  }
  |
  class_alloc_code
  {
    $$ = $1 ; 
  }
  |
  afunc_code
  {
    $$ = $1 ; 
  }
  |
  advice_code
  {
    $$ = $1 ; 
  }
  |
  mfunc_code
  {
    $$ = $1 ; 
  }
  |
  func_code
  {
    $$ = $1 ; 
  }
  ;
  
empty_line:
  EOL
  {
    $$ = NULL ;
  }
  ;

debug_information:
  DebugInfo EOL
  {
    /* debug_information */
    char *cp, *c ;
    $$ = ALLOC(vinst_info) ;
    $$->type = JD_debug_info ;
    $$->is.debug_info.string = c = cp = $1.name ;
    if( !strncmp( cp, "#statement", 10 ) ) {
      $$->type = JD_debug_statement_info ;
      $$->is.debug_info.string = c ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_info.statementnum = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_info.filename = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_info.linenum = atoi(c) ;
    }
    else if( !strncmp( cp, "#cwd", 4 ) ) {
      $$->type = JD_debug_cwd_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_cwd_info.dir = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#file", 4 ) ) {
      $$->type = JD_debug_file_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_file_info.file = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) {
        *c = '\0' ;
        c++ ;
        while( isspace(*c) && *c != '\0' ) c++ ;
        if( *c != '\0' ) {
          $$->is.debug_file_info.dir = c ;
          while( !isspace(*c) && ( *c != '\0') ) c++ ;
          if( *c != '\0' ) *c = '\0' ;
        }
        else $$->is.debug_file_info.dir = "" ;
      }
      else {
        $$->is.debug_file_info.dir = "" ;
      }
    }
    else if( !strncmp( cp, "#gen_func_frame", 15 ) ) {
      $$->type = JD_debug_gen_func_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_gen_func_frame_info.func = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_mfunc_frame", 16 ) ) {
      $$->type = JD_debug_gen_mfunc_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_afunc_frame", 16 ) ) {
      $$->type = JD_debug_gen_afunc_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_advice_frame", 17 ) ) {
      $$->type = JD_debug_gen_advice_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#argument", 9 ) ) {
      $$->type = JD_debug_argument_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_var_argument_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_var_argument_info.level = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
      $$->is.debug_var_argument_info.type = cp ;
      c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_var_argument_info.ub = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_var_argument_info.lb = atoi(c) ;
    }
    else if( !strncmp( cp, "#variable", 9 ) ) {
      $$->type = JD_debug_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      *c = '\0' ;
      $$->is.debug_var_argument_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_var_argument_info.level = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
      $$->is.debug_var_argument_info.type = cp ;
      c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_var_argument_info.ub = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_var_argument_info.lb = atoi(c) ;
    }
    else if( !strncmp( cp, "#enum", 5 ) ) {
      int n ;
      $$->type = JD_debug_enum_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_enum_info.name = cp ;
      c++ ;
      while( isspace(*c) && (*c != '\n') ) c++ ;
      cp = c ;
      n = 0 ;
      while( (*c != '\n') && (*c != '\0') ) {
        n++ ;
        while( !isspace(*c) ) c++ ;
        while( isspace(*c) && (*c != '\n') && (*c != '\0') ) c++ ;
      }
      $$->is.debug_enum_info.num_member = n ;
      if( n ) {
        int i ;
        $$->is.debug_enum_info.member = 
          (char **)calloc( n, sizeof(char *) ) ;
        c = cp ;
        for( i = 0 ; i < n ; i++ ) {
          cp = c ;
          while( !isspace(*c) && ( *c != '\0') ) c++ ;
          *c = '\0' ; c++ ;
          $$->is.debug_enum_info.member[i] = cp ;
          if( i < n-1 ) while( isspace(*c) ) c++ ;
          cp = c ;
        }
      }
    }
    else if( !strncmp( cp, "#block", 6 ) ) {
      $$->type = JD_debug_block_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_block_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#endblock", 9 ) ) {
      $$->type = JD_debug_endblock_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_endblock_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#member_variable", 16 ) ) {
      $$->type = JD_debug_member_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_member_variable_info.class = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_member_variable_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      $$->is.debug_member_variable_info.index = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_member_variable_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#aspect_variable", 16 ) ) {
      JD_name *cls, *var ;
      $$->type = JD_debug_aspect_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_member_variable_info.class = cp ;
      cls = find_local_name( cp ) ;
      if( !cls ) {
        cls = find_global_name( cp ) ;
      }
      c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      $$->is.debug_member_variable_info.name = cp ;
      if( cls ) {
        var = find_name_in_scope( cp, cls->child ) ;
        if( var && var->static_name )
          $$->is.debug_member_variable_info.static_name = var->static_name ;
      }
      c++ ;
      while( isspace(*c) ) c++ ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      $$->is.debug_member_variable_info.level = atoi(c) ;
    }
  }
  ;
  
port_info:
  VI_port direction '[' Decimal_num ':' Decimal_num ']' name opt_port_attrib EOL
  {
    /* port_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_info ;
    $$->is.port.direction = $2 ;
    $$->is.port.ub = atoi($4.name) ;
    $$->is.port.lb = atoi($6.name) ;
    $$->is.port.name = $8.name ;
    $$->is.port.attribs = $9 ;
  }
  ;

direction:
  VI_input
  {
    $$ = $1 ; 
  }
  |
  VI_output
  {
    $$ = $1 ; 
  }
  |
  VI_inout
  {
    $$ = $1 ; 
  }
  ;
  
opt_port_attrib:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  port_attribs
  {
    $$ = $1 ; 
  }
  ;
  
port_attribs:
  port_attrib
  {
    $$ = $1 ; 
  }
  |
  port_attribs port_attrib
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;
 
port_attrib:
  VI_sample edge VI_skew int_value
  {
    /* port_access_attrib */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_access_attrib ;
    $$->is.port_access_attrib.drive_sample = $1 ;
    $$->is.port_access_attrib.edge = $2 ;
    $$->is.port_access_attrib.skew = $4 ;
  }
  |
  VI_drive edge VI_skew int_value
  {
    /* port_access_attrib */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_access_attrib ;
    $$->is.port_access_attrib.drive_sample = $1 ;
    $$->is.port_access_attrib.edge = $2 ;
    $$->is.port_access_attrib.skew = $4 ;
  }
  |
  VI_depth Decimal_num
  {
    /* depth */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_depth_attrib ;
    $$->is.int_value = atoi($2.name) ;
  }
  |
  VI_path String_lit
  {
    /* path */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_path_attrib ;
    $$->is.string = $2.name ;
  }
  |
  '@' name
  {
    /* port_clock_attirb */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_clock_attrib ;
    $$->is.port_clock_attrib.port = $2.name ;
  }
  |
  '@' name '.' name
  {
    /* portset_clock_attirb */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_port_clock_attrib ;
    $$->is.port_clock_attrib.pset = $2.name ;
    $$->is.port_clock_attrib.port = $4.name ;
    /* link to symbol table: TBI */
  }
  ;
  
int_value:
  Decimal_num
  {
    /* int_value */
    $$ = atoi($1.name) ;
  }
  |
  '-' Decimal_num
  {
    /* minus_int_value */
    $$ = -atoi($2.name) ;
  }
  ;
  
portset_info:
  VI_portset name '{' EOL
  port_infos
  '}' EOL
  {
    /* portset_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_portset_info ;
    $$->is.portset.name = $2.name ;
    $$->is.portset.port_infos = $5 ;
  }
  ;
  
port_infos:
  port_info
  {
    $$ = $1 ; 
  }
  |
  port_infos port_info
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;


connection_info:
  VI_connection VI_port name '=' String_lit EOL
  {
    /* port_connection_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_connection_info ;
    $$->is.connection_info.port = $3.name ;
    $$->is.connection_info.path = $5.name ;
  }
  |
  VI_connection VI_portset name name '=' String_lit EOL
  {
    /* portset_connection_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_connection_info ;
    $$->is.connection_info.portset = $3.name ;
    $$->is.connection_info.port = $4.name ;
    $$->is.connection_info.path = $6.name ;
  }
  ;
  
variable_dcl:
  VI_variable Decimal_num data_type name array_range opt_var_attribs EOL
  {
    /* variable_dcl */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_variable_dcl ;
    $$->index = $$->is.variable_dcl.index = atoi($2.name) ;
    $$->is.variable_dcl.data_type = $3 ;
    $$->is.variable_dcl.name = $4.name ;
    $$->is.variable_dcl.array_range = $5 ;
    $$->is.variable_dcl.var_attrib = $6 ;
    $$->is.variable_dcl.class = Current_Class ;
  }
  ;

data_type:
  VI_void
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_int
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_long
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_float
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_double
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_bit '[' Decimal_num ':' Decimal_num ']'
  {
    /* bit_data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
    $$->is.data_type.ub = atoi($3.name) ;
    $$->is.data_type.lb = atoi($5.name) ;
  }
  |
  VI_class name
  {
    /* named_data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
    $$->is.data_type.name = $2.name ;
    /* link to synbol table: TBI */
  }
  |
  VI_sys_class name
  {
    /* named_data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
    $$->is.data_type.name = $2.name ;
    /* link to synbol table: TBI */
  }
  |
  VI_string
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_signal
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_class_type
  {
    /* data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
  }
  |
  VI_enum name
  {
    /* named_data_type */
    $$ = ALLOC(vcode_info) ;
    $$->is.data_type.type = $1 ;
    $$->is.data_type.name = $2.name ;
    /* link to synbol table: TBI */
  }
  ;  

array_range:
  VI_single
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  |
  VI_array Decimal_num
  {
    /* array_array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
    $$->is.array_range.size = atoi($2.name) ;
  }
  |
  VI_marray Decimal_num '(' range_list ')'
  {
    /* marray_array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
    $$->is.array_range.size = atoi($2.name) ;
    $$->is.array_range.range_list = $4 ;
  }
  |
  VI_stridx
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  |
  VI_assoc
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  |
  VI_list
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  ;
  
range_list:
  Decimal_num
  {
    /* a_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.int_value = atoi($1.name) ;
  
  }
  |
  range_list Decimal_num
  {
    /* range_list */
    vcode_info *new, *pre ;
    $$ = $1 ;
    new = ALLOC(vcode_info) ;
    new->is.int_value = atoi($2.name) ;
    pre = $$ ;
    while( pre->next ) pre = pre->next ;
    pre->next = new ;
  }
  ;
  
opt_var_attribs:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  var_attribs
  {
    $$ = $1 ; 
  }
  ;

var_attribs:
  var_attrib
  {
    /* attrib */
    $$ = ALLOC(vcode_info) ;
    switch( $1 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  | 
  var_attribs var_attrib
  {
    /* var_attribs */
    $$ = $1 ;
    switch( $2 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  ;

var_attrib:
  VI_static
  {
    $$ = $1 ; 
  }
  |
  VI_clone
  {
    $$ = $1 ; 
  }
  |
  VI_extern
  {
    $$ = $1 ; 
  }
  |
  VI_local
  {
    $$ = $1 ; 
  }
  |
  VI_global
  {
    $$ = $1 ; 
  }
  |
  VI_protected
  {
    $$ = $1 ; 
  }
  |
  VI_final
  {
    $$ = $1 ; 
  }
  ;

enum_dcl:
  VI_enum name enum_members EOL
  {
    /* enum_dcl */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_enum_dcl ;
    $$->is.enum_dcl.name = $2.name ;
    $$->is.enum_dcl.members = $3 ;
  }
  ;
  
enum_members:
  name
  {
    /* a_string */
    $$ = ALLOC(vcode_info) ;
    $$->is.string = $1.name ;
  
  }
  |
  enum_members name
  {
    /* string_list */
    vcode_info *new, *pre ;
    $$ = $1 ;
    new = ALLOC(vcode_info) ;
    new->is.string = $2.name ;
    pre = $$ ;
    while( pre->next ) pre = pre->next ;
    pre->next = new ;
  }
  ;
  
function_info:
  VI_function Decimal_num opt_func_options data_type name Decimal_num String_lit 
  '(' EOL
  opt_arguments
  ')' EOL
  {
    /* function_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_function_info ;
    $$->index = atoi($2.name) ;
    $$->is.function_info.func_options = $3 ;
    $$->is.function_info.data_type = $4 ;
    $$->is.function_info.name = $5.name ;
    $$->is.function_info.sys_flag = $5.sys_flag ;
    $$->srclinenum = atoi($6.name) ;
    $$->srcfilename = $7.name ;
    $$->is.function_info.arguments = $10 ;
    $$->is.function_info.class = Current_Class ;
    
  }
  ;

opt_func_options:
  /* empty */
  {
    /* null_attrib */
    $$ = ALLOC(vcode_info) ;
  }
  |
  func_options
  {
    $$ = $1 ; 
  }
  ;

func_options:
  func_option
  {
    /* attrib */
    $$ = ALLOC(vcode_info) ;
    switch( $1 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  |
  func_options func_option
  {
    /* var_attribs */
    $$ = $1 ;
    switch( $2 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  ;

func_option:
  VI_abstract
  {
    $$ = $1 ; 
  }
  |
  VI_local
  {
    $$ = $1 ; 
  }
  |
  VI_protected
  {
    $$ = $1 ; 
  }
  |
  VI_final
  {
    $$ = $1 ; 
  }
  |
  VI_export
  {
    $$ = $1 ; 
  }
  |
  VI_leaf
  {
    $$ = $1 ; 
  }
  |
  VI_overwrite
  {
    $$ = $1 ; 
  }
  |
  VI_extern
  {
    $$ = $1 ; 
  }
  ;

opt_arguments:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  arguments
  {
    $$ = $1 ; 
  }
  ;

arguments:
  argument
  {
    $$ = $1 ; 
  }
  |
  arguments argument
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;

argument:
  opt_var data_type name array_range EOL
  {
    /* argument */
    $$ = ALLOC(vcode_info) ;
    $$->is.argument.var = $1 ;
    $$->is.argument.data_type = $2 ;
    $$->is.argument.name = $3.name ;
    $$->is.argument.array_range = $4 ;
  }
  |
  VI_formal data_type EOL
  {
    /* formal_argument */
    $$ = NULL ;
  }
  |
  '(' EOL 
    argument 
  ')' '*' EOL
  {
    /* formal_argument */
    $$ = NULL ;
  }
  |
  '?' EOL
  {
    /* formal_argument */
    $$ = NULL ;
  }
  |
  '(' EOL
     argument 
   ')' '+' EOL
  {
    /* formal_argument */
    $$ = NULL ;
  }
  | 
  '[' EOL
    argument 
  ']' EOL
  {
    /* formal_argument */
    $$ = NULL ;
  }
  ;
  
opt_var:
  /* empty */
  {
    $$ = 0 ;
  }
  |
  VI_var
  {
    $$ = 1 ;
  }
  ;

pointcuts:
  pointcut
  {
    $$ = $1 ; 
  }
  |
  pointcuts pointcut
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;

pointcut:
  VI_call '(' pointcut_names ')' EOL
  {
    /* call_pointcut */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_call_pointcut ;
    $$->is.pointcut_info.next = $3 ;
  }
  |
  VI_return '(' pointcut_names ')' EOL
  {
    /* return_pointcut */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_return_pointcut ;
    $$->is.pointcut_info.next = $3 ;
  }
  |
  VI_overwrite '(' pointcut_names ')' EOL
  {
    /* call_pointcut */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_overwrite_pointcut ;
    $$->is.pointcut_info.next = $3 ;
  }
  ;  
  
  
pointcut_names:
  pointcut_name
  {
    $$ = $1 ; 
  }
  |
  pointcut_names pointcut_name
  {
    /* pointcut_chain */
    if( $1 ) {
      vcode_info *a ;
      a = $1 ;
      while( a->is.pointcut_info.next ) 
        a = a->is.pointcut_info.next ;
      a->is.pointcut_info.next = $2 ;
    }
    $$ = $1 ;
  }
  ;
  
pointcut_name:
  name
  {
    /* pointcut_name */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_pointcut_name ;
    $$->is.pointcut_info.name = $1.name ;
  
  }
  |
  String_lit
  {
    /* pointcut_name */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_pointcut_string ;
    $$->is.pointcut_info.name = $1.name ;
  
  }
  ;
  
advice_info:
  VI_advice Decimal_num opt_func_options data_type name 
  Decimal_num String_lit EOL
  pointcuts
  '(' EOL
  opt_arguments
  ')' EOL
  {
    /* advice_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_advice_info ;
    $$->index = atoi($2.name) ;
    $$->is.function_info.func_options = $3 ;
    $$->is.function_info.data_type = $4 ;
    $$->is.function_info.name = $5.name ;
    $$->srclinenum = atoi($6.name) ;
    $$->srcfilename = $7.name ;
    $$->is.function_info.pointcut = $9 ;
    $$->is.function_info.arguments = $12 ;
    $$->is.function_info.class = Current_Class ;
    
  }
  ;
  
class_info:
  VI_class name opt_extention opt_class_attribs Decimal_num String_lit '{' EOL
  {
    /* class_info_begin */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_class_info ;
    $$->is.class.name = $2.name ;
    $$->is.class.sys_flag = $2.sys_flag ;
    $$->is.class.parent = $3 ;
    $$->is.class.attribs = $4 ;
    $$->srclinenum = atoi($5.name) ;
    $$->srcfilename = $6.name ;
    Current_Class = $$ ;
  }
    opt_class_members
  '}' EOL
  {
    /* class_info_end */
    $$ = Current_Class ;
    Current_Class = NULL ;
    $$->is.class.members = $10 ;
  }
  ;

aspect_info:
  VI_aspect name Decimal_num String_lit '{' EOL
  {
    /* aspect_info_begin */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_aspect_info ;
    $$->is.class.name = $2.name ;
    $$->srclinenum = atoi($3.name) ;
    $$->srcfilename = $4.name ;
    Current_Class = $$ ;
  }
    opt_aspect_members
  '}' EOL
  {
    /* aspect_info_end */
    $$ = Current_Class ;
    Current_Class = NULL ;
    $$->is.class.members = $8 ;
  }
  ;

verilog_task_info:
  VI_verilog_task Name '(' EOL
    opt_arguments
  ')' String_lit EOL
  {
    /* verilog_task_info */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_veri_task_info ;
    $$->is.function_info.name = $2.name ;
    $$->is.function_info.arguments = $5 ;
    $$->is.function_info.path = $7.name ;
  }
  ;
  
opt_extention:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  VI_extends name
  {
    /* extention */
    $$ = $2.name ;
  }
  ;

opt_class_attribs:
  /* empty */
  {
    $$ = NULL ;
  }
 |
  class_attribs
  {
    $$ = $1 ; 
  }
  ;

class_attribs:
  class_attrib
  {
    /* attrib */
    $$ = ALLOC(vcode_info) ;
    switch( $1 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  |
  class_attribs class_attrib
  {
    /* var_attribs */
    $$ = $1 ;
    switch( $2 ) {
      case VI_static:
        $$->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        $$->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        $$->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        $$->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        $$->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        $$->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        $$->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        $$->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        $$->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        $$->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        $$->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  }
  ;

class_attrib:
  VI_abstract
  {
    $$ = $1 ; 
  }
  |
  VI_local
  {
    $$ = $1 ; 
  }
  |
  VI_final
  {
    $$ = $1 ; 
  }
  |
  VI_extern
  {
    $$ = $1 ; 
  }
  ;

opt_class_members:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  class_members
  {
    $$ = $1 ; 
  }
  ;

class_members:
  class_member
  {
    $$ = $1 ; 
  }
  |
  class_members class_member
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;
    
class_member:
  variable_dcl
  {
    $$ = $1 ; 
  }
  |
  function_info
  {
    $$ = $1 ; 
  }
  ;
 
opt_aspect_members:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  aspect_members
  {
    $$ = $1 ; 
  }
  ;

aspect_members:
  aspect_member
  {
    $$ = $1 ; 
  }
  |
  aspect_members aspect_member
  {
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = $1 ;
    if( JD_first_inst == NULL ) JD_first_inst = $1 ;
    if( $1 ) {
      while(last->next) {
        if( last == $2 ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;
    
aspect_member:
  variable_dcl
  {
    $$ = $1 ; 
  }
  |
  function_info
  {
    $$ = $1 ; 
  }
  |
  advice_info
  {
    $$ = $1 ; 
  }
  ;
  
static_var_dcl:
  VI_stat_var name name data_type name array_range opt_var_attribs EOL
  {
    /* static_var_dcl */
    JD_name *name, *cls, *var ;
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_static_var_info ;
    $$->is.static_var_dcl.class = $2.name ;
    $$->is.static_var_dcl.func = $3.name ;
    $$->is.static_var_dcl.data_type = $4 ;
    $$->is.static_var_dcl.name = $5.name ;
    $$->is.static_var_dcl.array_range = $6 ;
    $$->is.static_var_dcl.attribs = $7 ;
    
    if( $7 && $7->is.attrib.is_local ) {
      name = find_local_static_name( $2.name, $3.name, $5.name ) ;
      cls = find_local_name( $2.name ) ;
    }
    else {
      name = find_static_name( $2.name, $3.name, $5.name ) ;
      cls = find_global_name( $2.name ) ;
    }
    if( cls ) {
      var = find_name_in_scope( $5.name, cls->child ) ;
    }
    else var = NULL ;
    if( name ) {
      if( name->type != JD_variable_name ) {
        ERROR_VARIABLE_NAME_TYPE_MISMATCH( $$->is.variable_dcl.name ) ;
      }
      else if( !comp_info( name->info, $$ ) ) {
        ERROR_VARIABLE_NAME_ATTRIB_MISMATCH( $$->is.variable_dcl.name ) ;
      }
      $$ = NULL ; 
    }
    else {
      if( $7 && $7->is.attrib.is_local )
        name = add_local_static_name( $2.name, $3.name, $5.name ) ;
      else
        name = add_static_name( $2.name, $3.name, $5.name ) ;
      name->type = JD_variable_name ;
      name->info = $$ ;
      $$->code_name = name ;
      if( var ) var->static_name = name ;
    }
  }
  ;
  
static_init_code:
  VI_static_init EOL
  opt_vinsts
  VI_static_init_end EOL
  {
    /* static_init_code */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_stat_init_code ;
    $$->is.init_code.code = $3 ;
  
  }
  ;
  
init_code:
  VI_initial_begin EOL
  opt_vinsts
  VI_initial_end EOL  
  {
    /* init_code */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_init_code ;
    $$->is.init_code.code = $3 ;
    
  }
  ;
  
class_init_code:
  VI_class_initial_begin name EOL
  opt_vinsts
  VI_class_initial_end EOL
  {
    /* class_init_code */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_class_init_code ;
    $$->is.init_code.name = $2.name ;
    $$->is.init_code.code = $4 ;
  
  }
  ;
  
class_alloc_code:
  VI_class_alloc name EOL
  opt_vinsts
  VI_class_alloc_end EOL
  {
    JD_name *cls, *var ;
    vinst_info *code ;
    /* class_alloc_code */
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_class_alloc_code ;
    $$->is.named_code.class = $2.name ;
    $$->is.named_code.code = $4 ;
    
    cls = find_name( $2.name ) ;
    cls->defined = 1 ;
    code = $4 ;
    while( code ) {
      if( code->type == VVI_inst ) {
        if( code->itype == VI_alloc || code->itype == VI_alloc_static ) {
          var = find_name_in_scope( code->name, cls->child ) ;
          if( var->defined ) {
            ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_VAR( 
              cls->name, var->name 
            ) ;
          }
          var->defined = 1 ;
        }
      }
      code = code->next ;
    }
  }
  ;
  
mfunc_code:
  VI_mfunc name name EOL
  opt_vinsts
  VI_func_end EOL
  {
    /* mfunc_code */
    JD_name *cls, *func ;
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_mfunc_code ;
    $$->is.named_code.class = $2.name ;
    $$->is.named_code.func = $3.name ;
    $$->is.named_code.code = $5 ;
  
    cls = find_name( $2.name ) ;
    func = find_name_in_scope( $3.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_FUNC( cls->name, func->name ) ;
    }
    $$->fname = func ;
    func->defined = 1 ;  
  }
  ;
  
func_code:
  VI_func name EOL
  opt_vinsts
  VI_func_end EOL
  {
    /* func_code */
    JD_name *func ;
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_func_code ;
    $$->is.named_code.func = $2.name ;
    $$->is.named_code.code = $4 ;
    func = find_name( $2.name ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_FUNC( func->name ) ;
    }
    $$->fname = func ;
    func->defined = 1 ;  
    if( !strcmp($2.name, "main") ) main_defined = 1 ;
  }
  ;

afunc_code:
  VI_afunc name name EOL
  opt_vinsts
  VI_func_end EOL
  {
    /* afunc_code */
    JD_name *cls, *func ;
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_afunc_code ;
    $$->is.named_code.class = $2.name ;
    $$->is.named_code.func = $3.name ;
    $$->is.named_code.code = $5 ;
  
    cls = find_name( $2.name ) ;
    func = find_name_in_scope( $3.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_FUNC( cls->name, func->name ) ;
    }
    $$->fname = func ;
    func->defined = 1 ;  
  }
  ;

advice_code:
  VI_advice name name EOL
  opt_vinsts
  VI_advice_end EOL
  {
    /* advice_code */
    JD_name *cls, *func ;
    $$ = ALLOC(vcode_info) ;
    $$->type = JD_advice_code ;
    $$->is.named_code.class = $2.name ;
    $$->is.named_code.func = $3.name ;
    $$->is.named_code.code = $5 ;
  
    cls = find_name( $2.name ) ;
    func = find_name_in_scope( $3.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_ADVICE(
        cls->name, func->name 
      ) ;
    }
    $$->fname = func ;
    func->defined = 1 ;  
  }
  ;

/***************************************************************/
/*     VINSTS                                                  */
/***************************************************************/
opt_vinsts:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  vinsts
  {
    $$ = $1 ; 
  }
  ;
  
vinsts:
  vinst
  {
    $$ = $1 ;
    if( $$ ) {
      $$->filename = (char *)input_file_name ;
      $$->linenum = lex_line_num-1 ;
    }
  }
  |
  vinsts vinst
  {
    /* VinstChain */
    vinst_info *last = $1 ;
    if( $1 ) {
      while(last->next) {
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
    if( $2 ) {
      $2->filename = (char *)input_file_name ;
      $2->linenum = lex_line_num-1 ;
    }
  }
  ;
  
vinst:
  enum_dcl
  {
    /* vinst_enum_dcl */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_enum_dcl ;
    $$->vinfo = $1 ;
  }
  |
  empty_line
  {
    $$ = NULL ;
  }
  |
  debug_information
  {
    /* debug_info_vinst */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_debug_info ;
    $$->vinfo = $1 ;
  }
  |
  Identifier ':' EOL
  {
    /* label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_label ;
    $$->name = $1.name ;
    if( $1.name[0] == 'f' ) $$->itype = 1 ;
    else $$->itype = 0 ;
  }
  |
  VI_null EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_lvar Decimal_num data_type name array_range opt_flags EOL
  {
    /* var_aloc */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->index = atoi($2.name) ;
    $$->data_type = $3 ;
    $$->name = $4.name ;
    $$->array_range = $5 ;
    $$->opt_flags = $6 ; 
  }
  |
  VI_lvar VI_static Decimal_num data_type name name name array_range opt_flags EOL
  {
    /* var_aloc */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $2 ; /* use 'static' as itype */
    $$->data_type = $4 ;
    $$->class = $5.name ;
    $$->func = $6.name ;
    $$->name = $7.name ;
    $$->array_range = $8 ;
    $$->opt_flags = $9 ; 
    name = find_local_static_name( $5.name, $6.name, $7.name ) ;
    if( !name ) name = find_static_name( $5.name, $6.name, $7.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($5.name)+strlen($6.name)+strlen($7.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $5.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $6.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $7.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_new EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_new_obj name EOL
  {
    /* new_obj */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->name = $2.name ;
  }
  {
    /* new_obj */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ; 
    $$->name = $2.name ;
  }
  |
  VI_copy_obj EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_gen_child EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_alloc Decimal_num data_type name array_range opt_flags EOL
  {
    /* var_aloc */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->index = atoi($2.name) ;
    $$->data_type = $3 ;
    $$->name = $4.name ;
    $$->array_range = $5 ;
    $$->opt_flags = $6 ; 
  }
  |
  VI_alloc_static name name Decimal_num data_type name array_range opt_flags EOL
  {
    /* alloc_static */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->class = $2.name ;
    $$->func = $3.name ;
    $$->data_type = $5 ;
    $$->name = $6.name ;
    $$->array_range = $7 ;
    $$->opt_flags = $8 ; 
    name = find_local_static_name( $2.name, $3.name, $6.name ) ;
    if( !name ) name = find_static_name( $2.name, $3.name, $6.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($2.name)+strlen($3.name)+strlen($6.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $2.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $3.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $6.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  opt_var VI_arg Decimal_num Decimal_num data_type name array_range opt_flags EOL
  {
    /*  */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->is_var = $1 ;
    $$->itype = $2 ;  
    $$->index = atoi($3.name) ;
    $$->offset = atoi($4.name) ;
    $$->data_type = $5 ;
    $$->name = $6.name ;
    $$->array_range = $7 ;
    $$->opt_flags = $8 ; 
    
  }
  |
  VI_void EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_to_int EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_convert VI_bit Decimal_num Decimal_num stype EOL
  {
    /* convert_bit */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->ub = atoi($3.name) ;
    $$->lb = atoi($4.name) ;
    $$->sd_type = $5 ;
  }
  |
  VI_convert VI_string stype EOL
  {
    /* convert */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->sd_type = $3 ;
    
  }
  |
  VI_convert VI_float stype EOL
  {
    /* convert */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->sd_type = $3 ;
    
  }
  |
  VI_inc  stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_dec  stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_minus dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_plus  dtype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_times dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_div   dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_mod   dtype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_and   dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_or  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eor   dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_nand  dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_nor   dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_neor  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_rshift stype EOL    
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_urshift stype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_lshift stype EOL   
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_lt  dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_gt  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eqeq  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_le  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_ge  dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_ne  dtype EOL  
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eqeqeq dtype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_neqeq dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eqQeq dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eqeqQ dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_Qeqeq dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_Qne dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_eqCeq ttype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_cmp dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_cmpz dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_cmpx dtype EOL 
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_minus stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_tilde stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_not stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_and stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_or stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_eor stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_nand stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_nor stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_u_neor stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_load_zero EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_load_one EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_load_const stype const_data EOL
  {
    /* load_const */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->const_data = $3 ;
  }
  |
  VI_load_this opt_var EOL
  {
    /* op_optvar */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->is_var = $2 ;
  }
  |
  VI_load_super opt_var EOL
  {
    /* op_optvar */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->is_var = $2 ;
  }
  |
  VI_load_thread label EOL
  {
    /* op_label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->name = $2.name ;
  }
  |
  VI_get_this EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_get_super opt_var Decimal_num EOL
  {
    /* op_optvar_index */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->is_var = $2 ;
    $$->index = atoi($3.name) ;
  }
  |
  VI_get_child opt_var Decimal_num EOL
  {
    /* op_optvar_index */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->is_var = $2 ;
    $$->index = atoi($3.name) ;
  }
  |
  VI_concat dtype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_duplicate stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_list stype Decimal_num EOL
  {
    /*  */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->index = atoi($3.name) ;
    
  }
  |
  VI_get_subbit opt_bit_type EOL
  {
    /*  */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->bit_type = $2 ;
  }
  |
  VI_pop Decimal_num EOL
  {
    /* op_index */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ; 
    $$->index = atoi($2.name) ; 
    
  }
  |
  VI_dup Decimal_num EOL
  {
    /* op_index */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ; 
    $$->index = atoi($2.name) ; 
    
  }
  |
  VI_pop VI_alu EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_push VI_alu EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_copy VI_alu Decimal_num EOL
  {
    /* op_subop_index */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->index = atoi($3.name) ; 
  }
  |
  VI_gen_frame EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_gen_m_frame EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_gen_a_frame EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_pop_this EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_jmp label EOL
  {
    /* op_label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->name = $2.name ;
  }
  |
  VI_jz stype label EOL
  {
    /* op_label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->name = $3.name ;
  }
  |
  VI_jnz stype label EOL
  {
    /* op_label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->name = $3.name ;
  }
  |
  VI_return_l EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_return EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_return_n EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_call name Decimal_num EOL
  {
    /* call */
    JD_name *func ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->func = $2.name ;
    $$->index = atoi($3.name) ;
    func = find_name( $2.name ) ;
    if( func == NULL || 
        ( func->type != JD_func_name && func->type != JD_mfunc_name && 
          func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
      ) 
    {
      ERROR_FUNC_NAME_NOT_FOUND( $2.name ) ;
    }
    $$->fname = func ;
    $$->func_type = func->type ;  
  }
  |
  VI_call_m name name Decimal_num EOL
  {
    /* call_m */
    JD_name *cls, *func ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->class = $2.name ;
    $$->func = $3.name ;
    $$->index = atoi($4.name) ;
    cls = find_name( $2.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $2.name ) ;
    }
    else {
      func = find_name_in_scope( $3.name, cls->child ) ;
      if( func == NULL && !strcmp( $3.name, "new" ) ) {
        $$->type = VVI_dummy_new ;
      }
      else {
        if( func == NULL || 
            ( func->type != JD_func_name && func->type != JD_mfunc_name && 
            func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
          ) 
        {
          ERROR_MEMBER_FUNCTION_NOT_FOUND( $2.name, $3.name ) ;
        }
        $$->fname = func ;
        $$->func_type = func->type ;
      } 
    } 
  }
  |
  VI_call_a name name Decimal_num EOL
  {
    /* call_a */
    JD_name *cls, *func ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->class = $2.name ;
    $$->func = $3.name ;
    $$->index = atoi($4.name) ;
    cls = find_name( $2.name ) ;
    if( cls == NULL )
    {
      ERROR_ASPECT_NAME_NOT_FOUND( $2.name ) ;
    }
    else {
      func = find_name_in_scope( $3.name, cls->child ) ;
      if( func == NULL || 
          ( func->type != JD_func_name && func->type != JD_mfunc_name && 
          func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
        ) 
      {
        ERROR_MEMBER_FUNCTION_NOT_FOUND( $2.name, $3.name ) ;
      }
      $$->fname = func ;
      $$->func_type = func->type ;
    }
  }
  |
  VI_veri_call name Decimal_num EOL
  {
    /* call */
    JD_name *func ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->func = $2.name ;
    $$->index = atoi($3.name) ;
    func = find_verilog_name( $2.name ) ;
    if( func == NULL || func->type != JD_verilog_task_name  ) 
    {
      ERROR_VERILOG_TASK_NAME_NOT_FOUND( $2.name ) ;
    }
    $$->is_leaf = 1 ;
    $$->func_type = func->type ;  
  }
  |
  VI_fork EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_exec label EOL
  {
    /* op_label */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->name = $2.name ;
  }
  |
  VI_join EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_spoon EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_spoon VI_cancel EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_knife EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_exit EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_exit VI_p_and EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_exit VI_p_or EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_terminate EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_breakpoint String_lit EOL
  {
    /* breakpoint */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->name = $2.name ;
    
  }
  |
  VI_sync VI_clk_of_pset_port edge name name EOL
  {
    /* sync_pset */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
    $$->portset = $4.name ;
    $$->port = $5.name ;
    pset = find_global_name( $4.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $4.name ) ;
    }
    name = find_name_in_scope( $5.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $4.name, $5.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_sync VI_clk_of_port edge name EOL
  {
    /* sync_port */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
    $$->port = $4.name ;
    name = find_global_name( $4.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_sync VI_clk_of_signal edge EOL
  {
    /* sync_sig */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
  }
  |
  VI_sync VI_pset_port edge name name EOL
  {
    /* sync_pset */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
    $$->portset = $4.name ;
    $$->port = $5.name ;
    pset = find_global_name( $4.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $4.name ) ;
    }
    name = find_name_in_scope( $5.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $4.name, $5.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_sync VI_port edge name EOL
  {
    /* sync_port */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
    $$->port = $4.name ;
    name = find_global_name( $4.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_sync VI_signal edge EOL
  {
    /* sync_sig */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->edge = $3 ;
  }
  |
  VI_loadl opt_array_access opt_static opt_var Decimal_num EOL
  {
    /* loadl */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->is_var = $4 ;
    $$->index = atoi($5.name) ;
  }
  |
  VI_loadg opt_array_access opt_var name EOL
  {
    /* loadg */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_var = $3 ;
    $$->name = $4.name ;
    name = find_local_name( $4.name ) ;
    if( !name ) {
      name = find_global_name( $4.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_loadm opt_array_access opt_static opt_var name name EOL
  {
    /* loadm */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->is_var = $4 ;
    $$->class = $5.name ;
    $$->name = $6.name ;
    cls = find_name( $5.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $5.name ) ;
    }
    vr = find_name_in_scope( $6.name, cls->child ) ; 
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $5.name, $6.name ) ;
      exit(1) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_load_port name EOL
  {
    /* op_port */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->port = $2.name ;
    name = find_global_name( $2.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $2.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_pset_port name name EOL
  {
    /* op_pset */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->portset = $2.name ;
    $$->port = $3.name ;
    pset = find_global_name( $2.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $2.name ) ;
    }
    name = find_name_in_scope( $3.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $2.name, $3.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_port_ptr name EOL
  {
    /* op_port */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->port = $2.name ;
    name = find_global_name( $2.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $2.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_pset_port_ptr name name EOL
  {
    /* op_pset */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->portset = $2.name ;
    $$->port = $3.name ;
    pset = find_global_name( $2.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $2.name ) ;
    }
    name = find_name_in_scope( $3.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $2.name, $3.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_port VI_subbit name EOL
  {
    /* load_port_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->port = $3.name ;
    name = find_global_name( $3.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $3.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_pset_port VI_subbit name name EOL
  {
    /* op_pset_sub */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->portset = $3.name ;
    $$->port = $4.name ;
    pset = find_global_name( $3.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $3.name ) ;
    }
    name = find_name_in_scope( $4.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $3.name, $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_port_ptr VI_subbit name EOL
  {
    /* load_port_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->port = $3.name ;
    name = find_global_name( $3.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $3.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_pset_port_ptr VI_subbit name name EOL
  {
    /* op_pset_sub */
    JD_name *pset, *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
    $$->portset = $3.name ;
    $$->port = $4.name ;
    pset = find_global_name( $3.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $3.name ) ;
    }
    name = find_name_in_scope( $4.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $3.name, $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_load_sig_pt EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_load_sig_pt VI_subbit EOL
  {
    /* op_subop */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;
  }
  |
  VI_get_sig_subbit EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_load_static opt_array_access name name name EOL
  {
    /* load_static */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->class = $3.name ;
    $$->func = $4.name ;
    $$->name = $5.name ;
    name = find_local_static_name( $3.name, $4.name, $5.name ) ;
    if( !name ) name = find_static_name( $3.name, $4.name, $5.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($3.name)+strlen($4.name)+strlen($5.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $3.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $4.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $5.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_storel opt_array_access opt_static dtype Decimal_num EOL
  {
    /* storel */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->index = atoi($5.name) ;
  }
  |
  VI_storel opt_array_access opt_static VI_subbit opt_bit_type 
    dtype Decimal_num EOL
  {
    /* storel_sub */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->index = atoi($7.name) ;
  }
  |
  VI_storeg opt_array_access opt_static dtype name EOL
  {
    /* storeg */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->name = $5.name ;
    name = find_local_name( $5.name ) ;
    if( !name ) {
      name = find_global_name( $5.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( $5.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_storeg opt_array_access opt_static VI_subbit opt_bit_type dtype name EOL
  {
    /* storeg_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->name = $7.name ;
    name = find_local_name( $7.name ) ;
    if( !name ) {
      name = find_global_name( $7.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( $7.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_storem opt_array_access opt_static dtype name name EOL
  {
    /* storem */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->class = $5.name ;
    $$->name = $6.name ;
    cls = find_name( $5.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $5.name ) ;
    }
    vr = find_name_in_scope( $6.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $5.name, $6.name ) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_storem opt_array_access opt_static VI_subbit opt_bit_type dtype name name EOL
  {
    /* storem_sub */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->class = $7.name ;
    $$->name = $8.name ;
    cls = find_name( $7.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $7.name ) ;
    }
    vr = find_name_in_scope( $8.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $7.name, $8.name ) ;
      exit(1) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_store_port dtype strength name EOL
  {
    /* store_port */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->strength = $3 ;
    $$->port = $4.name ;
    name = find_global_name( $4.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $4.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_port VI_subbit opt_bit_type dtype strength name EOL
  {
    /* store_port_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;  
    $$->bit_type = $3 ;
    $$->sd_type = $4 ;
    $$->strength = $5 ;
    $$->port = $6.name ;
    name = find_global_name( $6.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( $6.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_pset_port dtype strength name name EOL
  {
    /* store_pset */
    JD_name *pset, *port ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->strength = $3 ;
    $$->portset = $4.name ;
    $$->port = $5.name ;
    pset = find_global_name( $4.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $4.name ) ;
    }
    port = find_name_in_scope( $5.name, pset->child ) ; 
    if( !port ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $4.name, $5.name ) ;
    }
    $$->code_name = port ;
  }
  |
  VI_store_pset_port VI_subbit opt_bit_type dtype strength name name EOL
  {
    /* store_pset_sub */
    JD_name *pset, *port ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->isubtype = $2 ;  
    $$->bit_type = $3 ;
    $$->sd_type = $4 ;
    $$->strength = $5 ;
    $$->portset = $6.name ;
    $$->port = $7.name ;
    pset = find_global_name( $6.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( $6.name ) ;
    }
    port = find_name_in_scope( $7.name, pset->child ) ; 
    if( !port ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( $7.name, $6.name ) ;
    }
    $$->code_name = port ;
  }
  |
  VI_store_sig_l opt_array_access opt_static dtype strength Decimal_num EOL
  {
    /* store_sig_l */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->strength = $5 ;
    $$->index = atoi($6.name) ;  
  }
  |
  VI_store_sig_g opt_array_access opt_static dtype strength name EOL
  {
    /* store_sig_g */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->strength = $5 ;
    $$->name = $6.name ;
    name = find_local_name( $6.name ) ;
    if( !name ) {
      name = find_global_name( $6.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( $6.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_sig_m opt_array_access opt_static dtype strength name name EOL
  {
    /* store_sig_m */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->sd_type = $4 ;
    $$->strength = $5 ;
    $$->class = $6.name ;
    $$->name = $7.name ;
    cls = find_name( $6.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $6.name ) ;
    }
    vr = find_name_in_scope( $7.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $6.name, $7.name ) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_store_sig_l opt_array_access opt_static VI_subbit opt_bit_type 
    dtype strength Decimal_num EOL
  {
    /* store_sig_l_sub */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->strength = $7 ;
    $$->index = atoi($8.name) ;  
  }
  |
  VI_store_sig_g opt_array_access opt_static VI_subbit opt_bit_type 
    dtype strength name EOL
  {
    /* store_sig_g_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->strength = $7 ;
    $$->name = $8.name ;  
    name = find_local_name( $8.name ) ;
    if( !name ) {
      name = find_global_name( $8.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( $8.name ) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_sig_m opt_array_access opt_static VI_subbit opt_bit_type 
    dtype strength name name EOL
  {
    /* store_sig_m_sub */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->is_static = $3 ;
    $$->isubtype = $4 ;
    $$->bit_type = $5 ;
    $$->sd_type = $6 ;
    $$->strength = $7 ;
    $$->class = $8.name ;  
    $$->name = $9.name ;  
    cls = find_name( $8.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $8.name ) ;
    }
    vr = find_name_in_scope( $9.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $8.name, $9.name ) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_store_static opt_array_access dtype name name name EOL
  {
    /* store_static */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->sd_type = $3 ;
    $$->class = $4.name ;
    $$->func = $5.name ;
    $$->name = $6.name ;
    name = find_local_static_name( $4.name, $5.name, $6.name ) ;
    if( !name ) name = find_static_name( $4.name, $5.name, $6.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($4.name)+strlen($5.name)+strlen($6.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $4.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $5.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $6.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_sig_static opt_array_access dtype strength name name name EOL
  {
    /* store_sig_static */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->sd_type = $3 ;
    $$->strength = $4 ;
    $$->class = $5.name ;
    $$->func = $6.name ;
    $$->name = $7.name ;
    name = find_local_static_name( $5.name, $6.name, $7.name ) ;
    if( !name ) name = find_static_name( $5.name, $6.name, $7.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($5.name)+strlen($6.name)+strlen($7.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $5.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $6.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $7.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_static opt_array_access VI_subbit opt_bit_type 
    dtype name name name EOL
  {
    /* store_static_sub */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->isubtype = $3 ;
    $$->bit_type = $4 ;
    $$->sd_type = $5 ;
    $$->class = $6.name ;
    $$->func = $7.name ;
    $$->name = $8.name ;
    name = find_local_static_name( $6.name, $7.name, $8.name ) ;
    if( !name ) name = find_static_name( $6.name, $7.name, $8.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($6.name)+strlen($7.name)+strlen($8.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $6.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $7.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $8.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_sig_static opt_array_access VI_subbit opt_bit_type
    dtype strength name name name EOL
  {
    /* store_sig_static */
    JD_name *name ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->array_access = $2 ;
    $$->isubtype = $3 ;
    $$->bit_type = $4 ;
    $$->sd_type = $5 ;
    $$->strength = $6 ;
    $$->class = $7.name ;
    $$->func = $8.name ;
    $$->name = $9.name ;
    name = find_local_static_name( $7.name, $8.name, $9.name ) ;
    if( !name ) name = find_static_name( $7.name, $8.name, $9.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen($7.name)+strlen($8.name)+strlen($9.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, $7.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $8.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, $9.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    $$->code_name = name ;
  }
  |
  VI_store_obj dtype name name EOL
  {
    /* store_obj */
    JD_name *cls, *vr ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
    $$->class = $3.name ;
    $$->name = $4.name ;
    cls = find_name( $3.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $3.name ) ;
    }
    vr = find_name_in_scope( $4.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( $3.name, $4.name ) ;
    }
    $$->index = vr->info->is.variable_dcl.index ;
  }
  |
  VI_store_adjust stype EOL
  {
    /* aluops */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->sd_type = $2 ;
  }
  |
  VI_alloc_func Decimal_num name
  {
    /* alloc_func */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;
    $$->index = atoi($2.name) ;
    $$->name = $3.name ;
  }
  |
  VI_overwrite name name name
  {
    /* overwrite */
    JD_name *cls, *func ;
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->parent = $2.name ;
    $$->class = $3.name ;
    $$->func = $$->name = $4.name ;
    cls = find_name( $2.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $2.name ) ;
    }
    func = find_name_in_scope( $4.name, cls->child ) ; ;
    if( func == NULL || func->type != JD_mfunc_name ) {
      ERROR_MEMBER_FUNCTION_NOT_FOUND( $2.name, $4.name ) ;
    }
    $$->index = func->info->index ;
    if( func->info->index < 0 ) {
      ERROR_ILLEGAL_OVERWRITE_INDEX_FOR_MEMBER_FUNCTION( $2.name, $4.name ) ;
    }
    cls = find_name( $3.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( $3.name ) ;
    }
    func = find_name_in_scope( $4.name, cls->child ) ; ;
    if( func == NULL || func->type != JD_mfunc_name ) {
      ERROR_MEMBER_FUNCTION_NOT_FOUND( $3.name, $4.name ) ;
    }
  }
  |
  VI_chk_overwrite Decimal_num name
  {
    /* overwrite */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->index = atoi($2.name) ;
    $$->func = $$->name = $3.name ;
  }
  |
  VI_call_advice pointcut_specs EOL
  {
    /* advice_op */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->fname = $2 ;
  }
  |
  VI_return_advice pointcut_specs EOL
  {
    /* advice_op */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
    $$->fname = $2 ;
  }
  |
  VI_save_acc EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_restore_acc EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_enter_advice EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_get_return EOL
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  |
  VI_error
  {
    /* simple */
    $$ = ALLOC(vinst_info) ;
    $$->type = VVI_inst ;
    $$->inum = inst_num++ ;
    $$->itype = $1 ;  
  }
  ;

pointcut_specs:
  pointcut_spec
  {
    $$ = $1 ; 
  }
  |
  pointcut_specs pointcut_spec
  {
    /* NameChain */
    JD_name *last = $1 ;
    if( $1 ) {
      while(last->next) {
        last = last->next ;
      }
      last->next = $2 ;
      $$ = $1 ;
    }
    else $$ = $2 ;
  }
  ;

pointcut_spec:
  name opt_class_pointcut
  {
    /* pointcut_spec */
    $$ = ALLOC(JD_name) ;
    $$->name = $1.name ;
    $$->class_name = $2 ;
  }
  ;

opt_class_pointcut:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  '(' name ')'
  {
    /* class_pointcut */
    $$ = ALLOC(JD_name) ;
    $$->name = $2.name ;
  }
  ;
  
stype:
  '(' type_code ')'
  {
    /* stype */
    $$ = ALLOC(vcode_info) ;
    $$->is.sd_type.a = $2 ;
  }
  ;
  
dtype:
  '(' type_code ',' type_code ')'
  {
    /* dtype */
    $$ = ALLOC(vcode_info) ;
    $$->is.sd_type.a = $2 ;
    $$->is.sd_type.b = $4 ;
  }
  ;

ttype:
  '(' type_code ',' type_code ',' type_code ')'
  {
    /* dtype */
    $$ = ALLOC(vcode_info) ;
    $$->is.sd_type.a = $2 ;
    $$->is.sd_type.b = $4 ;
    $$->is.sd_type.c = $6 ;
  }
  ;

opt_flags:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  VI_static
  {
    /* static_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.attrib.is_static = 1 ;
  }
  |
  VI_clone
  {
    /* clone_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.attrib.is_clone = 1 ;
  }
  |
  VI_static VI_clone
  {
    /* static_clone_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.attrib.is_static = 1 ;
    $$->is.attrib.is_clone = 1 ;
  }
  |
  VI_clone VI_static
  {
    /* static_clone_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.attrib.is_static = 1 ;
    $$->is.attrib.is_clone = 1 ;
  }
  ;
  
type_code:
  VI_string
  {
    $$ = $1 ; 
  }
  |
  VI_void
  {
    $$ = $1 ; 
  }
  |
  VI_null
  {
    $$ = $1 ; 
  }
  |
  VI_int
  {
    $$ = $1 ; 
  }
  |
  VI_bit
  {
    $$ = $1 ; 
  }
  |
  VI_float
  {
    $$ = $1 ; 
  }
  |
  VI_ptset
  {
    $$ = $1 ; 
  }
  |
  VI_signal
  {
    $$ = $1 ; 
  }
  |
  VI_enum
  {
    $$ = $1 ; 
  }
  |
  VI_obj
  {
    $$ = $1 ; 
  }
  |
  VI_array
  {
    $$ = $1 ; 
  }
  |
  VI_marray
  {
    $$ = $1 ; 
  }
  |
  VI_assoc
  {
    $$ = $1 ; 
  }
  |
  VI_stridx
  {
    $$ = $1 ; 
  }
  |
  VI_list
  {
    $$ = $1 ; 
  }
  ;
  
const_data:
  number
  {
    $$ = $1 ; 
  }
  |
  String_lit
  {
    /* string_const */
    $$ = ALLOC(expression_node) ;
    $$->type = JD_string_lit ;
    $$->is.string = $1.name ;  
  }
  |
  name name Decimal_num
  {
    /* string_const */
    $$ = ALLOC(expression_node) ;
    $$->type = JD_enum_member ;
    $$->is.enum_const.type = $1.name ;  
    $$->is.enum_const.member = $2.name ;
    $$->is.enum_const.index = atoi($3.name) ;
  }
  ;
  
label:
  Identifier
  {
    $$ = $1 ; 
  }
  ;
  
edge:
  VI_posedge
  {
    $$ = $1 ; 
  }
  |
  VI_negedge
  {
    $$ = $1 ; 
  }
  |
  VI_bothedge
  {
    $$ = $1 ; 
  }
  |
  VI_noedge
  {
    $$ = $1 ; 
  }
  ;


opt_array_access:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  VI_array Decimal_num
  {
    /* array_array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
    $$->is.array_range.size = atoi($2.name) ;
  }
  |
  VI_marray Decimal_num '(' range_list ')'
  {
    /* marray_array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
    $$->is.array_range.size = atoi($2.name) ;
    $$->is.array_range.range_list = $4 ;
  }
  |
  VI_stridx
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  |
  VI_assoc
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  |
  VI_list
  {
    /* array_range */
    $$ = ALLOC(vcode_info) ;
    $$->is.array_range.type = $1 ;
  }
  ;

opt_static:
  /* empty */
  {
    $$ = 0 ;
  }
  |
  VI_static
  {
    $$ = 1 ;
  }
  ;

opt_bit_type:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  VI_i_endian '[' Decimal_num ':' Decimal_num ']'
  {
    /* static_clone_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.bit_type.type = $1 ;
    $$->is.bit_type.ub = atoi($3.name) ;
    $$->is.bit_type.lb = atoi($5.name) ;
  }
  |
  VI_nz_lsb '[' Decimal_num ':' Decimal_num ']'
  {
    /* static_clone_flag */
    $$ = ALLOC(vcode_info) ;
    $$->is.bit_type.type = $1 ;
    $$->is.bit_type.ub = atoi($3.name) ;
    $$->is.bit_type.lb = atoi($5.name) ;
  }
  ;

number:
  Real_num         /* real */
  {
    /* real_num */
    $$ = ALLOC(expression_node) ;
    $$->type = JD_real_num ;
    $$->is.real_value = atof($1.name) ;  
    $$->string = $1.name ;
  }
  |
  Decimal_num      /* number */
  {
    /* decimal_num */
    $$ = ALLOC(expression_node) ;
    $$->type = JD_int_const ;
    $$->is.int_value = atoi($1.name) ;  
    $$->string = $1.name ;
  }
  |
  V_decimal_num    /* verilog format decimal */
  {
    /* v_decimal_num */
    $$ = ALLOC(expression_node) ;
    scan_v_decimal( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  C_decimal_num    /* C format decimal */
  {
    /* c_decimal_num */
    $$ = ALLOC(expression_node) ;
    scan_c_decimal( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  V_hex_num        /* verilog format hex */
  {
    /* v_hex_num */
    $$ = ALLOC(expression_node) ;
    scan_v_hex( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  C_hex_num        /* C format hex */
  {
    /* c_hex_num */
    $$ = ALLOC(expression_node) ;
    scan_c_hex( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  V_octal_num      /* verilog format octal */
  {
    /* v_octal_num */
    $$ = ALLOC(expression_node) ;
    scan_v_octal( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  C_octal_num      /* C format octal */
  {
    /* c_octal_num */
    $$ = ALLOC(expression_node) ;
    scan_c_octal( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  V_bin_num        /* verilog format binary */
  {
    /* v_bin_num */
    $$ = ALLOC(expression_node) ;
    scan_v_binary( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  |
  C_bin_num        /* C format binary */
  {
    /* c_bin_num */
    $$ = ALLOC(expression_node) ;
    scan_c_binary( $1.name, $$ ) ;
    $$->string = $1.name ;
  }
  ;

strength:
  VI_normal
  {
    $$ = $1 ; 
  }
  |
  VI_weak
  {
    $$ = $1 ; 
  }
  |
  VI_strong
  {
    $$ = $1 ; 
  }
  ;
    
name:
  Name
  {
    $$ = $1 ; 
  }
  ;
  
%%
