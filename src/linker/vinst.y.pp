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
#INCLUDE "vinst_keyword_token.h" 
#INCLUDE "vinst_nonterminal_node_type.h" 


%start object_files

%%

object_files:
  virtual_code
  <<object_file>>
  |
  object_files virtual_code
  <<ObjectChain>>
  ;
  
virtual_code:  
  inital_header 
  vinst_units
  <<DDeqD2>>
  ;

inital_header:
  'jedacmp' Decimal_num Decimal_num EOL
  <<inital_header>>
  ;
  
vinst_units:
  vinst_unit
  <<DDeqD1>>
  |
  vinst_units vinst_unit
  <<VcodeChain>>
  ;
  
vinst_unit:
  empty_line
  <<DDeqNULL>>
  |
  debug_information
  <<DDeqD1>>
  |
  port_info
  <<top_port_info>>
  |
  portset_info
  <<top_portset_info>>
  |
  connection_info
  <<DDeqD1>>
  |
  variable_dcl
  <<top_variable_dcl>>
  |
  enum_dcl
  <<top_enum_dcl>>
  |
  function_info
  <<top_function_info>>
  |
  class_info
  <<top_class_info>>
  |
  aspect_info
  <<top_aspect_info>>
  |
  verilog_task_info
  <<top_verilog_task_info>>
  |
  static_var_dcl
  <<DDeqD1>>
  |
  static_init_code
  <<DDeqD1>>
  |
  init_code
  <<DDeqD1>>
  |
  class_init_code
  <<DDeqD1>>
  |
  class_alloc_code
  <<DDeqD1>>
  |
  afunc_code
  <<DDeqD1>>
  |
  advice_code
  <<DDeqD1>>
  |
  mfunc_code
  <<DDeqD1>>
  |
  func_code
  <<DDeqD1>>
  ;
  
empty_line:
  EOL
  <<DDeqNULL>>
  ;

debug_information:
  DebugInfo EOL
  <<debug_information>>
  ;
  
port_info:
  'port' direction '[' Decimal_num ':' Decimal_num ']' name opt_port_attrib EOL
  <<port_info>>
  ;

direction:
  'input'
  <<DDeqD1>>
  |
  'output'
  <<DDeqD1>>
  |
  'inout'
  <<DDeqD1>>
  ;
  
opt_port_attrib:
  /* empty */
  <<DDeqNULL>>
  |
  port_attribs
  <<DDeqD1>>
  ;
  
port_attribs:
  port_attrib
  <<DDeqD1>>
  |
  port_attribs port_attrib
  <<VcodeChain>>
  ;
 
port_attrib:
  'sample' edge 'skew' int_value
  <<port_access_attrib>>
  |
  'drive' edge 'skew' int_value
  <<port_access_attrib>>
  |
  'depth' Decimal_num
  <<depth>>
  |
  'path' String_lit
  <<path>>
  |
  '@' name
  <<port_clock_attirb>>
  |
  '@' name '.' name
  <<portset_clock_attirb>>
  ;
  
int_value:
  Decimal_num
  <<int_value>>
  |
  '-' Decimal_num
  <<minus_int_value>>
  ;
  
portset_info:
  'portset' name '{' EOL
  port_infos
  '}' EOL
  <<portset_info>>
  ;
  
port_infos:
  port_info
  <<DDeqD1>>
  |
  port_infos port_info
  <<VcodeChain>>
  ;


connection_info:
  'connection' 'port' name '=' String_lit EOL
  <<port_connection_info>>
  |
  'connection' 'portset' name name '=' String_lit EOL
  <<portset_connection_info>>
  ;
  
variable_dcl:
  'variable' Decimal_num data_type name array_range opt_var_attribs EOL
  <<variable_dcl>>
  ;

data_type:
  'void'
  <<data_type>>
  |
  'int'
  <<data_type>>
  |
  'long'
  <<data_type>>
  |
  'float'
  <<data_type>>
  |
  'double'
  <<data_type>>
  |
  'bit' '[' Decimal_num ':' Decimal_num ']'
  <<bit_data_type>>
  |
  'class' name
  <<named_data_type>>
  |
  'sys_class' name
  <<named_data_type>>
  |
  'string'
  <<data_type>>
  |
  'signal'
  <<data_type>>
  |
  'class_type'
  <<data_type>>
  |
  'enum' name
  <<named_data_type>>
  ;  

array_range:
  'single'
  <<array_range>>
  |
  'array' Decimal_num
  <<array_array_range>>
  |
  'marray' Decimal_num '(' range_list ')'
  <<marray_array_range>>
  |
  'stridx'
  <<array_range>>
  |
  'assoc'
  <<array_range>>
  |
  'list'
  <<array_range>>
  ;
  
range_list:
  Decimal_num
  <<a_range>>
  |
  range_list Decimal_num
  <<range_list>>
  ;
  
opt_var_attribs:
  /* empty */
  <<DDeqNULL>>
  |
  var_attribs
  <<DDeqD1>>
  ;

var_attribs:
  var_attrib
  <<attrib>>
  | 
  var_attribs var_attrib
  <<attribs>>
  ;

var_attrib:
  'static'
  <<DDeqD1>>
  |
  'clone'
  <<DDeqD1>>
  |
  'extern'
  <<DDeqD1>>
  |
  'local'
  <<DDeqD1>>
  |
  'global'
  <<DDeqD1>>
  |
  'protected'
  <<DDeqD1>>
  |
  'final'
  <<DDeqD1>>
  ;

enum_dcl:
  'enum' name enum_members EOL
  <<enum_dcl>>
  ;
  
enum_members:
  name
  <<a_string>>
  |
  enum_members name
  <<string_list>>
  ;
  
function_info:
  'function' Decimal_num opt_func_options data_type name Decimal_num String_lit 
  '(' EOL
  opt_arguments
  ')' EOL
  <<function_info>>
  ;

opt_func_options:
  /* empty */
  <<null_attrib>>
  |
  func_options
  <<DDeqD1>>
  ;

func_options:
  func_option
  <<attrib>>
  |
  func_options func_option
  <<attribs>>
  ;

func_option:
  'abstract'
  <<DDeqD1>>
  |
  'local'
  <<DDeqD1>>
  |
  'protected'
  <<DDeqD1>>
  |
  'final'
  <<DDeqD1>>
  |
  'export'
  <<DDeqD1>>
  |
  'leaf'
  <<DDeqD1>>
  |
  'overwrite'
  <<DDeqD1>>
  |
  'extern'
  <<DDeqD1>>
  ;

opt_arguments:
  /* empty */
  <<DDeqNULL>>
  |
  arguments
  <<DDeqD1>>
  ;

arguments:
  argument
  <<DDeqD1>>
  |
  arguments argument
  <<VcodeChain>>
  ;

argument:
  opt_var data_type name array_range EOL
  <<argument>>
  |
  'formal' data_type EOL
  <<formal_argument>>
  |
  '(' EOL 
    argument 
  ')' '*' EOL
  <<formal_argument>>
  |
  '?' EOL
  <<formal_argument>>
  |
  '(' EOL
     argument 
   ')' '+' EOL
  <<formal_argument>>
  | 
  '[' EOL
    argument 
  ']' EOL
  <<formal_argument>>
  ;
  
opt_var:
  /* empty */
  <<DDeq0>>
  |
  'var'
  <<DDeq1>>
  ;

pointcuts:
  pointcut
  <<DDeqD1>>
  |
  pointcuts pointcut
  <<VcodeChain>>
  ;

pointcut:
  'call' '(' pointcut_names ')' EOL
  <<call_pointcut>>
  |
  'return' '(' pointcut_names ')' EOL
  <<return_pointcut>>
  |
  'overwrite' '(' pointcut_names ')' EOL
  <<overwrite_pointcut>>
  ;  
  
  
pointcut_names:
  pointcut_name
  <<DDeqD1>>
  |
  pointcut_names pointcut_name
  <<pointcut_chain>>
  ;
  
pointcut_name:
  name
  <<pointcut_name>>
  |
  String_lit
  <<pointcut_string>>
  ;
  
advice_info:
  'advice' Decimal_num opt_func_options data_type name 
  Decimal_num String_lit EOL
  pointcuts
  '(' EOL
  opt_arguments
  ')' EOL
  <<advice_info>>
  ;
  
class_info:
  'class' name opt_extention opt_class_attribs Decimal_num String_lit '{' EOL
  <<class_info_begin>>
    opt_class_members
  '}' EOL
  <<class_info_end>>
  ;

aspect_info:
  'aspect' name Decimal_num String_lit '{' EOL
  <<aspect_info_begin>>
    opt_aspect_members
  '}' EOL
  <<aspect_info_end>>
  ;

verilog_task_info:
  'verilog_task' Name '(' EOL
    opt_arguments
  ')' String_lit EOL
  <<verilog_task_info>>
  ;
  
opt_extention:
  /* empty */
  <<DDeqNULL>>
  |
  'extends' name
  <<extention>>
  ;

opt_class_attribs:
  /* empty */
  <<DDeqNULL>>
 |
  class_attribs
  <<DDeqD1>>
  ;

class_attribs:
  class_attrib
  <<attrib>>
  |
  class_attribs class_attrib
  <<attribs>>
  ;

class_attrib:
  'abstract'
  <<DDeqD1>>
  |
  'local'
  <<DDeqD1>>
  |
  'final'
  <<DDeqD1>>
  |
  'extern'
  <<DDeqD1>>
  ;

opt_class_members:
  /* empty */
  <<DDeqNULL>>
  |
  class_members
  <<DDeqD1>>
  ;

class_members:
  class_member
  <<DDeqD1>>
  |
  class_members class_member
  <<VcodeChain>>
  ;
    
class_member:
  variable_dcl
  <<DDeqD1>>
  |
  function_info
  <<DDeqD1>>
  ;
 
opt_aspect_members:
  /* empty */
  <<DDeqNULL>>
  |
  aspect_members
  <<DDeqD1>>
  ;

aspect_members:
  aspect_member
  <<DDeqD1>>
  |
  aspect_members aspect_member
  <<VcodeChain>>
  ;
    
aspect_member:
  variable_dcl
  <<DDeqD1>>
  |
  function_info
  <<DDeqD1>>
  |
  advice_info
  <<DDeqD1>>
  ;
  
static_var_dcl:
  'stat_var' name name data_type name array_range opt_var_attribs EOL
  <<static_var_dcl>>
  ;
  
static_init_code:
  'static_init' EOL
  opt_vinsts
  'static_init_end' EOL
  <<static_init_code>>
  ;
  
init_code:
  'initial_begin' EOL
  opt_vinsts
  'initial_end' EOL  
  <<init_code>>
  ;
  
class_init_code:
  'class_initial_begin' name EOL
  opt_vinsts
  'class_initial_end' EOL
  <<class_init_code>>
  ;
  
class_alloc_code:
  'class_alloc' name EOL
  opt_vinsts
  'class_alloc_end' EOL
  <<class_alloc_code>>
  ;
  
mfunc_code:
  'mfunc' name name EOL
  opt_vinsts
  'func_end' EOL
  <<mfunc_code>>
  ;
  
func_code:
  'func' name EOL
  opt_vinsts
  'func_end' EOL
  <<func_code>>
  ;

afunc_code:
  'afunc' name name EOL
  opt_vinsts
  'func_end' EOL
  <<afunc_code>>
  ;

advice_code:
  'advice' name name EOL
  opt_vinsts
  'advice_end' EOL
  <<advice_code>>
  ;

/***************************************************************/
/*     VINSTS                                                  */
/***************************************************************/
opt_vinsts:
  /* empty */
  <<DDeqNULL>>
  |
  vinsts
  <<DDeqD1>>
  ;
  
vinsts:
  vinst
  <<Vinst>>
  |
  vinsts vinst
  <<VinstChain>>
  ;
  
vinst:
  enum_dcl
  <<vinst_enum_dcl>>
  |
  empty_line
  <<DDeqNULL>>
  |
  debug_information
  <<debug_info_vinst>>
  |
  Identifier ':' EOL
  <<label>>
  |
  'null' EOL
  <<simple>>
  |
  'lvar' Decimal_num data_type name array_range opt_flags EOL
  <<var_aloc>>
  |
  'lvar' 'static' Decimal_num data_type name name name array_range opt_flags EOL
  <<static_var_aloc>>
  |
  'new' EOL
  <<simple>>
  |
  'new_obj' name EOL
  <<new_obj>>
  |
  'copy_obj' EOL
  <<simple>>
  |
  'gen_child' EOL
  <<simple>>
  |
  'alloc' Decimal_num data_type name array_range opt_flags EOL
  <<var_aloc>>
  |
  'alloc_static' name name Decimal_num data_type name array_range opt_flags EOL
  <<alloc_static>>
  |
  opt_var 'arg' Decimal_num Decimal_num data_type name array_range opt_flags EOL
  <<arg_alloc>>
  |
  'void' EOL
  <<simple>>
  |
  'to_int' EOL
  <<simple>>
  |
  'convert' 'bit' Decimal_num Decimal_num stype EOL
  <<convert_bit>>
  |
  'convert' 'string' stype EOL
  <<convert>>
  |
  'convert' 'float' stype EOL
  <<convert>>
  |
  'inc'  stype EOL
  <<aluops>>
  |
  'dec'  stype EOL
  <<aluops>>
  |
  'minus' dtype EOL  
  <<aluops>>
  |
  'plus'  dtype EOL
  <<aluops>>
  |
  'times' dtype EOL 
  <<aluops>>
  |
  'div'   dtype EOL 
  <<aluops>>
  |
  'mod'   dtype EOL
  <<aluops>>
  |
  'and'   dtype EOL 
  <<aluops>>
  |
  'or'  dtype EOL  
  <<aluops>>
  |
  'eor'   dtype EOL 
  <<aluops>>
  |
  'nand'  dtype EOL 
  <<aluops>>
  |
  'nor'   dtype EOL 
  <<aluops>>
  |
  'neor'  dtype EOL  
  <<aluops>>
  |
  'rshift' stype EOL    
  <<aluops>>
  |
  'urshift' stype EOL  
  <<aluops>>
  |
  'lshift' stype EOL   
  <<aluops>>
  |
  'lt'  dtype EOL 
  <<aluops>>
  |
  'gt'  dtype EOL  
  <<aluops>>
  |
  'eqeq'  dtype EOL  
  <<aluops>>
  |
  'le'  dtype EOL  
  <<aluops>>
  |
  'ge'  dtype EOL 
  <<aluops>>
  |
  'ne'  dtype EOL  
  <<aluops>>
  |
  'eqeqeq' dtype EOL
  <<aluops>>
  |
  'neqeq' dtype EOL 
  <<aluops>>
  |
  'eqQeq' dtype EOL 
  <<aluops>>
  |
  'eqeqQ' dtype EOL 
  <<aluops>>
  |
  'Qeqeq' dtype EOL 
  <<aluops>>
  |
  'Qne' dtype EOL 
  <<aluops>>
  |
  'eqCeq' ttype EOL
  <<aluops>>
  |
  'cmp' dtype EOL 
  <<aluops>>
  |
  'cmpz' dtype EOL 
  <<aluops>>
  |
  'cmpx' dtype EOL 
  <<aluops>>
  |
  'u_minus' stype EOL
  <<aluops>>
  |
  'u_tilde' stype EOL
  <<aluops>>
  |
  'u_not' stype EOL
  <<aluops>>
  |
  'u_and' stype EOL
  <<aluops>>
  |
  'u_or' stype EOL
  <<aluops>>
  |
  'u_eor' stype EOL
  <<aluops>>
  |
  'u_nand' stype EOL
  <<aluops>>
  |
  'u_nor' stype EOL
  <<aluops>>
  |
  'u_neor' stype EOL
  <<aluops>>
  |
  'load_zero' EOL
  <<simple>>
  |
  'load_one' EOL
  <<simple>>
  |
  'load_const' stype const_data EOL
  <<load_const>>
  |
  'load_this' opt_var EOL
  <<op_optvar>>
  |
  'load_super' opt_var EOL
  <<op_optvar>>
  |
  'load_thread' label EOL
  <<op_label>>
  |
  'get_this' EOL
  <<simple>>
  |
  'get_super' opt_var Decimal_num EOL
  <<op_optvar_index>>
  |
  'get_child' opt_var Decimal_num EOL
  <<op_optvar_index>>
  |
  'concat' dtype EOL
  <<aluops>>
  |
  'duplicate' stype EOL
  <<aluops>>
  |
  'list' stype Decimal_num EOL
  <<aluops_index>>
  |
  'get_subbit' opt_bit_type EOL
  <<op_subbit>>
  |
  'pop' Decimal_num EOL
  <<op_index>>
  |
  'dup' Decimal_num EOL
  <<op_index>>
  |
  'pop' 'alu' EOL
  <<op_subop>>
  |
  'push' 'alu' EOL
  <<op_subop>>
  |
  'copy' 'alu' Decimal_num EOL
  <<op_subop_index>>
  |
  'gen_frame' EOL
  <<simple>>
  |
  'gen_m_frame' EOL
  <<simple>>
  |
  'gen_a_frame' EOL
  <<simple>>
  |
  'pop_this' EOL
  <<simple>>
  |
  'jmp' label EOL
  <<op_label>>
  |
  'jz' stype label EOL
  <<op_type_label>>
  |
  'jnz' stype label EOL
  <<op_type_label>>
  |
  'return_l' EOL
  <<simple>>
  |
  'return' EOL
  <<simple>>
  |
  'return_n' EOL
  <<simple>>
  |
  'call' name Decimal_num EOL
  <<call>>
  |
  'call_m' name name Decimal_num EOL
  <<call_m>>
  |
  'call_a' name name Decimal_num EOL
  <<call_a>>
  |
  'veri_call' name Decimal_num EOL
  <<veri_call>>
  |
  'fork' EOL
  <<simple>>
  |
  'exec' label EOL
  <<op_label>>
  |
  'join' EOL
  <<simple>>
  |
  'spoon' EOL
  <<simple>>
  |
  'spoon' 'cancel' EOL
  <<op_subop>>
  |
  'knife' EOL
  <<simple>>
  |
  'exit' EOL
  <<simple>>
  |
  'exit' 'p_and' EOL
  <<op_subop>>
  |
  'exit' 'p_or' EOL
  <<op_subop>>
  |
  'terminate' EOL
  <<simple>>
  |
  'breakpoint' String_lit EOL
  <<breakpoint>>
  |
  'sync' 'clk_of_pset_port' edge name name EOL
  <<sync_pset>>
  |
  'sync' 'clk_of_port' edge name EOL
  <<sync_port>>
  |
  'sync' 'clk_of_signal' edge EOL
  <<sync_sig>>
  |
  'sync' 'pset_port' edge name name EOL
  <<sync_pset>>
  |
  'sync' 'port' edge name EOL
  <<sync_port>>
  |
  'sync' 'signal' edge EOL
  <<sync_sig>>
  |
  'loadl' opt_array_access opt_static opt_var Decimal_num EOL
  <<loadl>>
  |
  'loadg' opt_array_access opt_var name EOL
  <<loadg>>
  |
  'loadm' opt_array_access opt_static opt_var name name EOL
  <<loadm>>
  |
  'load_port' name EOL
  <<load_port>>
  |
  'load_pset_port' name name EOL
  <<load_pset>>
  |
  'load_port_ptr' name EOL
  <<load_port>>
  |
  'load_pset_port_ptr' name name EOL
  <<load_pset>>
  |
  'load_port' 'subbit' name EOL
  <<load_port_sub>>
  |
  'load_pset_port' 'subbit' name name EOL
  <<load_pset_sub>>
  |
  'load_port_ptr' 'subbit' name EOL
  <<load_port_sub>>
  |
  'load_pset_port_ptr' 'subbit' name name EOL
  <<load_pset_sub>>
  |
  'load_sig_pt' EOL
  <<simple>>
  |
  'load_sig_pt' 'subbit' EOL
  <<op_subop>>
  |
  'get_sig_subbit' EOL
  <<simple>>
  |
  'load_static' opt_array_access name name name EOL
  <<load_static>>
  |
  'storel' opt_array_access opt_static dtype Decimal_num EOL
  <<storel>>
  |
  'storel' opt_array_access opt_static 'subbit' opt_bit_type 
    dtype Decimal_num EOL
  <<storel_sub>>
  |
  'storeg' opt_array_access opt_static dtype name EOL
  <<storeg>>
  |
  'storeg' opt_array_access opt_static 'subbit' opt_bit_type dtype name EOL
  <<storeg_sub>>
  |
  'storem' opt_array_access opt_static dtype name name EOL
  <<storem>>
  |
  'storem' opt_array_access opt_static 'subbit' opt_bit_type dtype name name EOL
  <<storem_sub>>
  |
  'store_port' dtype strength name EOL
  <<store_port>>
  |
  'store_port' 'subbit' opt_bit_type dtype strength name EOL
  <<store_port_sub>>
  |
  'store_pset_port' dtype strength name name EOL
  <<store_pset>>
  |
  'store_pset_port' 'subbit' opt_bit_type dtype strength name name EOL
  <<store_pset_sub>>
  |
  'store_sig_l' opt_array_access opt_static dtype strength Decimal_num EOL
  <<store_sig_l>>
  |
  'store_sig_g' opt_array_access opt_static dtype strength name EOL
  <<store_sig_g>>
  |
  'store_sig_m' opt_array_access opt_static dtype strength name name EOL
  <<store_sig_m>>
  |
  'store_sig_l' opt_array_access opt_static 'subbit' opt_bit_type 
    dtype strength Decimal_num EOL
  <<store_sig_l_sub>>
  |
  'store_sig_g' opt_array_access opt_static 'subbit' opt_bit_type 
    dtype strength name EOL
  <<store_sig_g_sub>>
  |
  'store_sig_m' opt_array_access opt_static 'subbit' opt_bit_type 
    dtype strength name name EOL
  <<store_sig_m_sub>>
  |
  'store_static' opt_array_access dtype name name name EOL
  <<store_static>>
  |
  'store_sig_static' opt_array_access dtype strength name name name EOL
  <<store_sig_static>>
  |
  'store_static' opt_array_access 'subbit' opt_bit_type 
    dtype name name name EOL
  <<store_static_sub>>
  |
  'store_sig_static' opt_array_access 'subbit' opt_bit_type
    dtype strength name name name EOL
  <<store_sig_static_sub>>
  |
  'store_obj' dtype name name EOL
  <<store_obj>>
  |
  'store_adjust' stype EOL
  <<aluops>>
  |
  'alloc_func' Decimal_num name
  <<alloc_func>>
  |
  'overwrite' name name name
  <<overwrite>>
  |
  'chk_overwrite' Decimal_num name
  <<chk_overwrite>>
  |
  'call_advice' pointcut_specs EOL
  <<advice_op>>
  |
  'return_advice' pointcut_specs EOL
  <<advice_op>>
  |
  'save_acc' EOL
  <<simple>>
  |
  'restore_acc' EOL
  <<simple>>
  |
  'enter_advice' EOL
  <<simple>>
  |
  'get_return' EOL
  <<simple>>
  |
  'error'
  <<simple>>
  ;

pointcut_specs:
  pointcut_spec
  <<DDeqD1>>
  |
  pointcut_specs pointcut_spec
  <<NameChain>>
  ;

pointcut_spec:
  name opt_class_pointcut
  <<pointcut_spec>>
  ;

opt_class_pointcut:
  /* empty */
  <<DDeqNULL>>
  |
  '(' name ')'
  <<class_pointcut>>
  ;
  
stype:
  '(' type_code ')'
  <<stype>>
  ;
  
dtype:
  '(' type_code ',' type_code ')'
  <<dtype>>
  ;

ttype:
  '(' type_code ',' type_code ',' type_code ')'
  <<ttype>>
  ;

opt_flags:
  /* empty */
  <<DDeqNULL>>
  |
  'static'
  <<static_flag>>
  |
  'clone'
  <<clone_flag>>
  |
  'static' 'clone'
  <<static_clone_flag>>
  |
  'clone' 'static'
  <<static_clone_flag>>
  ;
  
type_code:
  'string'
  <<DDeqD1>>
  |
  'void'
  <<DDeqD1>>
  |
  'null'
  <<DDeqD1>>
  |
  'int'
  <<DDeqD1>>
  |
  'bit'
  <<DDeqD1>>
  |
  'float'
  <<DDeqD1>>
  |
  'ptset'
  <<DDeqD1>>
  |
  'signal'
  <<DDeqD1>>
  |
  'enum'
  <<DDeqD1>>
  |
  'obj'
  <<DDeqD1>>
  |
  'array'
  <<DDeqD1>>
  |
  'marray'
  <<DDeqD1>>
  |
  'assoc'
  <<DDeqD1>>
  |
  'stridx'
  <<DDeqD1>>
  |
  'list'
  <<DDeqD1>>
  ;
  
const_data:
  number
  <<DDeqD1>>
  |
  String_lit
  <<string_const>>
  |
  name name Decimal_num
  <<enum_const>>
  ;
  
label:
  Identifier
  <<DDeqD1>>
  ;
  
edge:
  'posedge'
  <<DDeqD1>>
  |
  'negedge'
  <<DDeqD1>>
  |
  'bothedge'
  <<DDeqD1>>
  |
  'noedge'
  <<DDeqD1>>
  ;


opt_array_access:
  /* empty */
  <<DDeqNULL>>
  |
  'array' Decimal_num
  <<array_array_range>>
  |
  'marray' Decimal_num '(' range_list ')'
  <<marray_array_range>>
  |
  'stridx'
  <<simple_array_range>>
  |
  'assoc'
  <<simple_array_range>>
  |
  'list'
  <<simple_array_range>>
  ;

opt_static:
  /* empty */
  <<DDeq0>>
  |
  'static'
  <<DDeq1>>
  ;

opt_bit_type:
  /* empty */
  <<DDeqNULL>>
  |
  'i_endian' '[' Decimal_num ':' Decimal_num ']'
  <<bit_type>>
  |
  'nz_lsb' '[' Decimal_num ':' Decimal_num ']'
  <<bit_type>>
  ;

number:
  Real_num         /* real */
  <<real_num>>
  |
  Decimal_num      /* number */
  <<decimal_num>>
  |
  V_decimal_num    /* verilog format decimal */
  <<v_decimal_num>>
  |
  C_decimal_num    /* C format decimal */
  <<c_decimal_num>>
  |
  V_hex_num        /* verilog format hex */
  <<v_hex_num>>
  |
  C_hex_num        /* C format hex */
  <<c_hex_num>>
  |
  V_octal_num      /* verilog format octal */
  <<v_octal_num>>
  |
  C_octal_num      /* C format octal */
  <<c_octal_num>>
  |
  V_bin_num        /* verilog format binary */
  <<v_bin_num>>
  |
  C_bin_num        /* C format binary */
  <<c_bin_num>>
  ;

strength:
  'normal'
  <<DDeqD1>>
  |
  'weak'
  <<DDeqD1>>
  |
  'strong'
  <<DDeqD1>>
  ;
    
name:
  Name
  <<DDeqD1>>
  ;
  
%%
