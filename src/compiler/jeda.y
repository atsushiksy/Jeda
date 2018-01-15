%{
/***************************************************************************
 *
 *  Jeda Compiler (prep-file for btyacc)
 *
 *  Author: Atsushi Kasuya
 *
 *
 *   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
 *   Copyright (C) 2002, 2003 Jeda Technologies, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2, or (at your option) any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.  
 *
 *  Coding note:
 *    This source is parsed by several perl script along with other .data 
 *      files to conform the compiler code
 *    
 *    #INCLUDE is handled by gen_parser.pl. But this must be a non-nested file
 *    #BEGIN <name>  to #END is extracted as syntax file under syntax 
 *      directory, and eventually used in spec/language.spec
 *    <<name>> represent action block coded in jeda_parser_action.data
 *      and expanded by gen_parser.pl. This is hidden on syntax file.
 *    keywords are coded as 'a_keyword' directly. Those keywords must be
 *      defined in jeda_keyword.data. Multi-caractor operator such as
 *      == must be declared in jeda_operator.data with associated logical
 *      name. All of those words are converted to logical token, and code and
 *      table for lex are automatically created.
 *    Types of nonterminal node must be declared in 
 *      jeda_nonterminal_node_type.data. The %type declaration will
 *      be automatically generated from the file.
 *
 *
 *
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/compiler_macro.h"
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/symtab.h"
#include "../include/error_msg.h"
#include "../include/constant.h"

   
expression_node *unary_constant_folding( 
  expression_node *exp 
) ;
expression_node *binary_constant_folding( 
  expression_node *exp 
) ;

extern int lex_port_parsing ;
extern int compile_pass ;
extern block_node *root_block ;
extern int scope_id ;
extern int first_verilog_block ;

extern int jeda_util_parsing ;

extern char *input_file_name ;
extern int   in_file_used ;

extern named_node *current_scope ;
extern named_node *top_scope ;
extern named_node *local_scope ;

extern int error_count ;
extern int error_flag ;

extern unsigned lex_line_num ;

block_node *current_block = NULL ;
int current_block_level = 0 ;
int is_static_flag = 0 ;
int is_extern_flag = 0 ;
int is_local_flag = 0 ;
int is_global_flag = 0 ;
int is_protected_flag = 0 ;
int is_abstract_flag = 0 ;
int is_final_flag = 0 ;
int is_clone_flag = 0 ;
int is_export_flag = 0 ;
int is_overwrite_flag = 0 ;
int label_num = 0 ;
int no_static_flag = 0 ;
int no_local_flag = 0 ;
int no_final_flag = 0 ;
int no_clone_flag = 0 ;
int no_global_flag = 0 ;
int no_extern_flag = 0 ;
int class_parsing = 0 ;
int class_initial_code = 0 ; /* set during class initial statment to detect
                                illegal member name access */

named_node *current_func_name ;

int ex_error_flag = 0 ;

int in_function = 0 ;
int in_local_function = 0 ;
int in_block = 0 ;

int aspect_parsing = 0 ;
int in_advice = 0 ;

int repeat_level = 0 ;

static char *filename ;
static int linenum ;

int func_line_num ;
int class_line_num ;

int yyerrflag ;

static named_node *this_class_name = NULL ;

expression_node *top_static_rnd = NULL ;

%}

%union {
  int  integer_value ;
  struct {
    char *name ;
    int  linenum ;
    char *filename ;
  } string_value ;
  struct {
    int  linenum ;
    char *filename ;
  } keyword ;
  struct {
    int is_static_flag ;
    int is_extern_flag ;
    int is_local_flag ;
    int is_protected_flag ;
    int is_abstract_flag ;
    int is_overwrite_flag ;
    int is_final_flag ;
    int is_export_flag ;
  } flags ;
  block_node *block ;
  JD_assign_operator assign_opr ;
  expression_node *expression ;
  statement_node  *statement ;
  JD_join_type    join ;
  JD_edge_type    edge ;
  JD_vf_attrib    *vfattr ; 
  JD_port_type     portdir ;
  named_node      *name ;
  JD_expression_type exp_type ;
  JD_strength     strength ;
  connection_node  *connection ;
}

%token <string_value> Identifier 
%token <string_value> Real_num         /* real */
%token <string_value> Real_or_Depth    /* real or depth */
%token <string_value> Decimal_num      /* number */
%token <string_value> V_decimal_num    /* verilog format decimal */
%token <string_value> C_decimal_num    /* C format decimal */
%token <string_value> V_hex_num        /* verilog format hex */
%token <string_value> C_hex_num        /* C format hex */
%token <string_value> V_octal_num      /* verilog format octal */
%token <string_value> C_octal_num      /* C format octal */
%token <string_value> V_bin_num        /* verilog format binary */
%token <string_value> C_bin_num        /* C format binary */
%token <string_value> String_literal   /* string */

%token <keyword> ';' 
%type <keyword> semicolon

/* #include is not activated while yaccing.
   #INCLUDE is handled by gen_parser.pl. But this must be a non-nested file
 */
%token <keyword> JDT_abstract
%token <keyword> JDT_advice
%token <keyword> JDT_array
%token <keyword> JDT_aspect
%token <keyword> JDT_assoc
%token <keyword> JDT_bit
%token <keyword> JDT_bothedge
%token <keyword> JDT_break
%token <keyword> JDT_breakpoint
%token <keyword> JDT_call
%token <keyword> JDT_case
%token <keyword> JDT_casex
%token <keyword> JDT_casez
%token <keyword> JDT_class
%token <keyword> JDT_class_type
%token <keyword> JDT_clone
%token <keyword> JDT_connectionset
%token <keyword> JDT_continue
%token <keyword> JDT_default
%token <keyword> JDT_depth
%token <keyword> JDT_do
%token <keyword> JDT_double
%token <keyword> JDT_drive
%token <keyword> JDT_else
%token <keyword> JDT_enum
%token <keyword> JDT_export
%token <keyword> JDT_extends
%token <keyword> JDT_extern
%token <keyword> JDT_false
%token <keyword> JDT_final
%token <keyword> JDT_float
%token <keyword> JDT_for
%token <keyword> JDT_forever
%token <keyword> JDT_fork
%token <keyword> JDT_global
%token <keyword> JDT_if
%token <keyword> JDT_initial
%token <keyword> JDT_inout
%token <keyword> JDT_input
%token <keyword> JDT_int
%token <keyword> JDT_join
%token <keyword> JDT_knife
%token <keyword> JDT_list
%token <keyword> JDT_local
%token <keyword> JDT_long
%token <keyword> JDT_marray
%token <keyword> JDT_negedge
%token <keyword> JDT_new
%token <keyword> JDT_node
%token <keyword> JDT_null
%token <keyword> JDT_output
%token <keyword> JDT_overwrite
%token <keyword> JDT_p_and
%token <keyword> JDT_p_or
%token <keyword> JDT_port
%token <keyword> JDT_portset
%token <keyword> JDT_posedge
%token <keyword> JDT_protected
%token <keyword> JDT_randcase
%token <keyword> JDT_repeat
%token <keyword> JDT_return
%token <keyword> JDT_sample
%token <keyword> JDT_signal
%token <keyword> JDT_skew
%token <keyword> JDT_spoon
%token <keyword> JDT_static
%token <keyword> JDT_string
%token <keyword> JDT_stridx
%token <keyword> JDT_strong
%token <keyword> JDT_super
%token <keyword> JDT_teminate
%token <keyword> JDT_terminate
%token <keyword> JDT_this
%token <keyword> JDT_true
%token <keyword> JDT_typedef
%token <keyword> JDT_var
%token <keyword> JDT_verilog
%token <keyword> JDT_void
%token <keyword> JDT_weak
%token <keyword> JDT_while
%token <keyword> JDT_RND
%token <keyword> JDT_CYC
%token <keyword> JD_ANDTILDA
%token <keyword> JD_TILDAAND
%token <keyword> JD_ORTILDA
%token <keyword> JD_TILDAOR
%token <keyword> JD_EORTILDA
%token <keyword> JD_TILDAEOR
%token <keyword> JD_LAND
%token <keyword> JD_LOR
%token <keyword> JD_RSHIFT
%token <keyword> JD_URSHIFT
%token <keyword> JD_LSHIFT
%token <keyword> JD_EQEQ
%token <keyword> JD_LE
%token <keyword> JD_GE
%token <keyword> JD_NE
%token <keyword> JD_EQEQEQ
%token <keyword> JD_NEQEQ
%token <keyword> JD_Q_EQ
%token <keyword> JD_Q_NEQ
%token <keyword> JD_QEQEQ
%token <keyword> JD_EQEQQ
%token <keyword> JD_C_EQEQ
%token <keyword> JD_D_PLUS
%token <keyword> JD_D_MINUS
%token <keyword> JD_D_AT
%token <keyword> JD_C_EQ
%token <keyword> JD_PLUS_EQ
%token <keyword> JD_MINUS_EQ
%token <keyword> JD_STAR_EQ
%token <keyword> JD_SLSH_EQ
%token <keyword> JD_MOD_EQ
%token <keyword> JD_LSHIFT_EQ
%token <keyword> JD_RSHIFT_EQ
%token <keyword> JD_URSHIFT_EQ
%token <keyword> JD_AND_EQ
%token <keyword> JD_OR_EQ
%token <keyword> JD_EOR_EQ
%token <keyword> JD_NAND_EQ
%token <keyword> JD_ANDN_EQ
%token <keyword> JD_NOR_EQ
%token <keyword> JD_ORN_EQ
%token <keyword> JD_NEOR_EQ
%token <keyword> JD_EORN_EQ
%token <keyword> '?'
%token <keyword> ':'
%token <keyword> '!'
%token <keyword> '%'
%token <keyword> '^'
%token <keyword> '&'
%token <keyword> '|'
%token <keyword> '+'
%token <keyword> '-'
%token <keyword> '<'
%token <keyword> '>'
%token <keyword> '='
%token <keyword> '*'
%token <keyword> '/'
%token <keyword> '.'
%token <keyword> '~'
%token <keyword> '@'
%token <keyword> '#'
%token <keyword> '$'
%token <keyword> '{'
%token <keyword> '}'
%token <keyword> '('
%token <keyword> ')'
%token <keyword> '['
%token <keyword> ']'
%token <keyword> '\''


%type <block>	translation_units translation_unit 
%type <block>	portset_definition connectionset_definition 
%type <block>	local_global_variable_declaration top_initial_statement 
%type <block>	function_definition class_definition 
%type <block>	verilog_class_definition verilog_task_definition 
%type <block>	verilog_task_definitions new_function_definition 
%type <block>	port_definition aspect_definition 
%type <block>	advice_definition 
%type <expression>	actual_arguments actual_argument 
%type <expression>	new_expression opt_new_args 
%type <expression>	opt_new_object assign_expression 
%type <expression>	left_expression opt_bit_range 
%type <expression>	bit_range opt_array_range 
%type <expression>	array_ranges array_range 
%type <expression>	opt_var_init var_init 
%type <expression>	opt_arg_init arg_init 
%type <expression>	opt_statement_prefix cycle_element 
%type <expression>	opt_clock_definition opt_multi_expressions 
%type <expression>	multi_expressions for_expression 
%type <expression>	constant number 
%type <expression>	expression opt_expression 
%type <expression>	timed_expression time_range 
%type <expression>	parallel_eval_expression timed_expression_list 
%type <expression>	a_ver_expression opt_access_range 
%type <expression>	access_range bit_access_range 
%type <expression>	array_access_ranges array_access_range 
%type <expression>	function_call_expression opt_actual_arguments 
%type <expression>	bool_null_expression state_expression 
%type <expression>	concatenation_expression list_expression 
%type <expression>	varport_expression port_declarations 
%type <expression>	port_declaration opt_port_attribs 
%type <expression>	port_attribs port_attrib 
%type <expression>	port_expression constant_concatenation 
%type <expression>	constant_list expression_list 
%type <expression>	clock_attrib drive_attrib 
%type <expression>	sample_attrib depth_attrib 
%type <expression>	node_attrib opt_depth 
%type <expression>	opt_skew string_constant 
%type <expression>	string_list opt_super_new_call 
%type <expression>	enum_expression pointcut_pattern 
%type <expression>	pointcut_element rand_val 
%type <expression>	rand_value rand_values 
%type <expression>	rand_range rand_arg 
%type <expression>	rand_parm rand_args 
%type <expression>	opt_rand_args static_random 
%type <expression>	rand_cond rnd_constant 
%type <expression>	rnd_string 
%type <statement>	initial_statement opt_class_items 
%type <statement>	class_items class_item 
%type <statement>	verilog_task_call_statement opt_aspect_items 
%type <statement>	aspect_items aspect_item 
%type <statement>	statement block 
%type <statement>	opt_statements statements 
%type <statement>	opt_var_enum_definitions var_enum_definitions 
%type <statement>	var_enum_definition var_definition 
%type <statement>	opt_formal_args formal_args 
%type <statement>	formal_arg opt_outer_block 
%type <statement>	null_statement if_else_statement 
%type <statement>	opt_else while_statement 
%type <statement>	for_statement repeat_statement 
%type <statement>	do_while_statement forever_statement 
%type <statement>	case_statement opt_case_items 
%type <statement>	case_items case_item 
%type <statement>	casex_statement casez_statement 
%type <statement>	randcase_statement opt_randcase_items 
%type <statement>	randcase_items randcase_item 
%type <statement>	break_statement continue_statement 
%type <statement>	fork_join_statement terminate_statement 
%type <statement>	breakpoint_statement expression_statement 
%type <statement>	assign_statement return_statement 
%type <statement>	function_call_statement enum_type_definition 
%type <statement>	type_definition opt_new_outer_block 
%type <statement>	new_block pointcut_spec 
%type <statement>	pointcut_specifications 
%type <integer_value>	opt_var 
%type <assign_opr>	assign_opr 
%type <string_value>	opt_exit_label label 
%type <string_value>	opt_string 
%type <join>	joins 
%type <edge>	edge_specification 
%type <vfattr>	opt_var_func_attrib var_func_attribs 
%type <vfattr>	var_func_attrib opt_extern 
%type <name>	opt_func_type var_func_type 
%type <name>	data_type data_type_name 
%type <name>	user_data_type enum_members_list 
%type <name>	enum_member class_type_definition 
%type <name>	class_name_list var_name_list 
%type <name>	var_name func_name 
%type <name>	arg_name class_name 
%type <name>	class_name_ref portset_name 
%type <name>	port_name portset_port 
%type <name>	opt_extention opt_void 
%type <name>	clock_port_name aspect_name 
%type <name>	advice_name 
%type <exp_type>	parallel_primitive 
%type <strength>	opt_strength 
%type <portdir>	port_direction 
%type <connection>	port_connections port_connection 


/* precedence table */
%right JD_RSHIFT_EQ JD_URSHIFT_EQ JD_LSHIFT_EQ JD_AND_EQ JD_OR_EQ JD_EOR_EQ JD_NAND_EQ JD_ANDN_EQ JD_NEOR_EQ JD_EORN_EQ JD_NOR_EQ JD_ORN_EQ
%right '=' JD_PLUS_EQ JD_MINUS_EQ JD_STAR_EQ JD_SLSH_EQ JD_MOD_EQ
%right '?' ':' 
%left JD_LOR 
%left JD_LAND 
%left '|' 
%left '^'  
%left '&'  
%left JD_C_EQEQ 
%left JD_Q_EQ JD_Q_NEQ JD_EQEQ JD_NE JD_EQEQEQ JD_NEQEQ JD_EQEQQ JD_QEQEQ
%left '>' JD_LE '<' JD_GE 
%left JD_LSHIFT JD_RSHIFT JD_URSHIFT
%left '-' '+'
%left  '*' '/' '%'
%left  '!' '~' 
%left  JD_TILDAAND JD_ANDTILDA JD_TILDAOR JD_ORTILDA JD_TILDAEOR JD_EORTILDA  
%right JD_D_PLUS JD_D_MINUS 
%left  '.'



%start jeda

%%



jeda:  translation_units
  ;
  
translation_units:
  translation_unit
  {
    /* trns_unit */
  
    SKIP_ON_ERROR ;
    PASS2 {
      root_block = $1 ;
    }
    $$ = $1 ;
  }
  |
  translation_units translation_unit
  {
    SKIP_ON_ERROR ;
    PASS2 {
      block_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error 
  {
    $$ = NULL ;
  }
  |
  translation_units error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;
  
translation_unit:
  port_definition
  {
    $$ = $1 ; 
  }
  |
  portset_definition
  {
    $$ = $1 ; 
  }
  |
  connectionset_definition
  {
    $$ = $1 ; 
  }
  |
  local_global_variable_declaration
  {
    $$ = $1 ; 
  }
  |
  top_initial_statement
  {
    $$ = $1 ; 
  }
  |
  type_definition
  {
    /* type_block */
    
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(block_node) ;
      $$->type = JD_type_definition ;
      $$->is.local_global_var.dcl = $1 ; /* temporaly store here */
      /* in the case of class variable definition, this node becomes
         NULL, and no further action is required. (true?)
       */
    }
  }
  |
  function_definition
  {
    $$ = $1 ; 
  }
  |
  class_definition
  {
    $$ = $1 ; 
  }
  |
  verilog_class_definition
  {
    $$ = $1 ; 
  }
  |
  aspect_definition
  {
    $$ = $1 ; 
  }
  ;



/***********************************
 *  port 
 ***********************************/


port_definition:
  port_declaration
  {
    /* port_definition */
    $$ = NULL ;
    ERROR_CLR_BLOCK {
      PASS2 {
        $$ = ALLOC(block_node) ;
        $$->type = JD_port_block ;
        $$->is.port = $1 ;
      }
    }
  }
  ;
  
port_declaration:
  port_direction 
  opt_bit_range 
  port_name 
  opt_port_attribs 
  semicolon
  {
    /* port_dcl */
    SKIP_ON_ERROR ;
    PASS1 {
      $3->info.port.portset = NULL ;
      $3->info.port.type = $1 ;
      if( $2 ) {
        $3->ub = $2->is.range.int_ub ;
        $3->lb = $2->is.range.int_lb ;
        $3->i_endian = $2->is.range.i_endian ;
      }
    }
    PASS2 {
      named_node *port ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_portdef_exp ;
      $$->filename = $5.filename ;
      $$->linenum = $5.linenum ;
      $$->is.port = $3 ;
      scan_port_attrib( $3, $4, $5.filename, $5.linenum ) ; 
      /* name, list_atrib */
    }
  }
  ;


port_direction:
  JDT_input  
  {
    /* input */
    $$ = JD_port_input ;
  }
  |
  JDT_output  
  {
    /* output */
    $$ = JD_port_output ;
  }
  |
  JDT_inout   
  {
    /* inout */
    $$ = JD_port_inout ;
  }
  ;

opt_bit_range:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  bit_range
  {
    $$ = $1 ; 
  }
  ;
  
bit_range:
  '[' constant ':' constant ']'
  {
    /* bit_range */
    SKIP_ON_ERROR ;
    PASS1_2 {
      int size0, size1  ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_bit_range_exp ;
      $$->is.range.ub = $2 ;
      $$->is.range.lb = $4 ;
      if( !eval_to_int( $2, &size1 ) || !eval_to_int( $4, &size0 ) ) {
        ERROR_LOCATION( $5.filename, $5.linenum ) ;
        ERROR_ILLEGAL_BIT_RANGE_INDEX ;
      }
      else {
        $$->is.range.int_ub = size1 ; 
        $$->is.range.int_lb = size0 ;
        if( $$->is.range.int_ub < 0 ) {
          ERROR_LOCATION( $5.filename, $5.linenum ) ;
          ERROR_ILLEGAL_BIT_RANGE_INDEX ;
        }
        else {
          if( $$->is.range.int_lb < 0 ) {
            ERROR_LOCATION( $5.filename, $5.linenum ) ;
            ERROR_ILLEGAL_BIT_RANGE_INDEX ;
          }
          else {
            if( size1 < size0 ) {
              $$->is.range.i_endian = 1 ;
            }
            else {
              $$->is.range.i_endian = 0 ;
            }
          }
        }
      }
      $$->filename = $5.filename ;
      $$->linenum = $5.linenum ;
    }
  }
  ;
  
port_name: 
  Identifier 
  {
    /* port_name */
    SKIP_ON_ERROR ;
    PASS1 {
      if( findname_in_scope( current_scope, $1.name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_DUPLICATED_NAME($1.name) ;
        $$ = NULL ;
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->type = JD_port_name ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        addname( $$ ) ;
      }
    }
    PASS2{
      $$ = findname_in_scope( current_scope, $1.name ) ;
      if( !$$ || $$->type != JD_port_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
        $$ = NULL ;
      }
    }
  }
  ;

opt_port_attribs:
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
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
  


/***********************************
 *  port attribute
 ***********************************/



port_attrib:
  clock_attrib
  {
    $$ = $1 ; 
  }
  |
  drive_attrib
  {
    $$ = $1 ; 
  }
  |
  sample_attrib
  {
    $$ = $1 ; 
  }
  |
  depth_attrib
  {
    $$ = $1 ; 
  }
  |
  node_attrib
  {
    $$ = $1 ; 
  }
  ;
  
clock_attrib:
  '@' clock_port_name
  {
    /* clock_attrib */
    $$ = NULL ;
    SKIP_ON_ERROR ;
    PASS2 {
      if( $2 != NULL ) {
        $$ = ALLOC(expression_node) ;
        $$->type = JD_port_attr_exp ;
        $$->is.port_attr.attr = JD_port_clock ;
        $$->is.port_attr.port_name = $2 ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
      }
    }
  }
  ;
  
clock_port_name: 
  Identifier 
  {
    /* clock_port_name */
    $$ = NULL ;
    SKIP_ON_ERROR ;
    PASS2{
      $$ = findname_in_scope( top_scope, $1.name ) ;
      if( !$$ || $$->type != JD_port_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNKNOWN_PORT_NAME($1.name) ;
        $$ = NULL ;
      }
    }
  }
  |
  Identifier '.' Identifier
  {
    /* portset_port */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *port_set ;
      named_node *name ;
      
      $$ = NULL ;
      
      port_set = findname( $1.name ) ;
      if( port_set == NULL || port_set->type != JD_portset_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNKNOWN_PORT_SET_NAME($1.name) ;
      }
      else {
        name = findname_in_scope(port_set->child_scope, $3.name ) ;
        if( name == NULL || name->type != JD_port_name ) {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_UNKNOWN_PORT_NAME($3.name) ;
        }
        else $$ = name ;
      }
    }
  }
  ;

drive_attrib:
  JDT_drive edge_specification opt_skew
  {
    /* sample_attrib */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_port_attr_exp ;
      $$->is.port_attr.attr = JD_port_drive ;
      $$->is.port_attr.edge = $2 ;
      $$->is.port_attr.skew = $3 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

sample_attrib:
  JDT_sample edge_specification opt_skew 
  {
    /* sample_attrib */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_port_attr_exp ;
      $$->is.port_attr.attr = JD_port_sample ;
      $$->is.port_attr.edge = $2 ;
      $$->is.port_attr.skew = $3 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
    
opt_skew:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  JDT_skew constant
  {
    /* skew_attrib */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ =  $2 ;
    }
  }
  ;

depth_attrib:
  JDT_depth constant
  {
    /* depth_attrib */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_port_attr_exp ;
      $$->is.port_attr.attr = JD_port_depth ;
      $$->is.port_attr.depth = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

node_attrib:
  JDT_node String_literal
  {
    /* node_attrib */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_port_attr_exp ;
      $$->is.port_attr.attr = JD_port_node ;
      $$->is.port_attr.node = $2.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
  
    }
  }
  ;



/***********************************
 *  port set
 ***********************************/


portset_definition:
  JDT_portset 
  portset_name opt_port_attribs
  '{'
  {
    /* enter_port_scope */
  
    SKIP_ON_ERROR ;
    PASS1_2 {
      enter_scope( $2 ) ;
    }
  }
    port_declarations
  {
    SKIP_ON_ERROR ;
    PASS1_2 {
      exit_scope() ;
    }
  }
  '}' 
  {
    /* portset_def */
    $$ = NULL ;
    ERROR_CLR_BLOCK {
      PASS2 {
        scan_port_attrib ( $2, $3, $1.filename, $1.linenum ) ;
          /* port_node *port, expression_node *list_attrib, 
             char *filename, int linenum                    */
        $$ = ALLOC(block_node) ;
        $$->type = JD_portset_block ;
        $$->name = $2 ;
        $$->is.portset.port_list = $6 ;
        {
          expression_node *port ;
          port = $6 ;
          while( port ) {
            port->is.port->info.port.portset = $2 ;
            port = port->next ;
          }
        }
      }
    }
  }
  ;

portset_name: 
  Identifier
  {
    /* portset_name */
    SKIP_ON_ERROR ;
    PASS1 {
      if( findname_in_scope( current_scope, $1.name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_DUPLICATED_NAME($1.name) ;
        $$ = NULL ;
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->type = JD_portset_name ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        addname( $$ ) ;
      }
    }
    PASS2{
      $$ = findname_in_scope( current_scope, $1.name ) ;
      if( !$$ || $$->type != JD_portset_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
    }
  }
  ;

port_declarations:
  port_declaration
  {
    $$ = $1 ; 
  }
  |
  port_declarations 
  port_declaration
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  port_declarations error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;
  



/***********************************
 *  connection set
 ***********************************/


connectionset_definition:
  JDT_connectionset 
  '{' 
    port_connections
  '}' 
  {
    /* connectionset_definition */
    $$ = NULL ;
    ERROR_CLR_BLOCK {
      PASS2 {
        $$ = ALLOC(block_node) ;
        $$->type = JD_connectionset_block ;
        $$->is.connection.connect_list = $3 ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
      } 
    } 
  }
  ;
  
port_connections:
  port_connection
  {
    $$ = $1 ; 
  }
  |
  port_connections port_connection
  {
    SKIP_ON_ERROR ;
    PASS2 {
      connection_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  port_connections error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;
  
port_connection:
  portset_port  String_literal semicolon 
  {
    /* port_connection */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(connection_node) ;
      $$->port = $1 ;
      $$->node_string = $2.name ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
    }
  }
  ;
  
portset_port:
   Identifier '.' Identifier 
  {
    /* portset_port */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *port_set ;
      named_node *name ;
      
      $$ = NULL ;
      
      port_set = findname( $1.name ) ;
      if( port_set == NULL || port_set->type != JD_portset_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNKNOWN_PORT_SET_NAME($1.name) ;
      }
      else {
        name = findname_in_scope(port_set->child_scope, $3.name ) ;
        if( name == NULL || name->type != JD_port_name ) {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_UNKNOWN_PORT_NAME($3.name) ;
        }
        else $$ = name ;
      }
    }
  }
   |
   Identifier
  {
    /* clock_port_name */
    $$ = NULL ;
    SKIP_ON_ERROR ;
    PASS2{
      $$ = findname_in_scope( top_scope, $1.name ) ;
      if( !$$ || $$->type != JD_port_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNKNOWN_PORT_NAME($1.name) ;
        $$ = NULL ;
      }
    }
  }
   ;



/***********************************
 *  local and global variable
 ***********************************/



local_global_variable_declaration:
  var_definition
  {
    /* local_global_var_dcl */
    $$ = NULL ;
    ERROR_CLR_BLOCK {
      PASS2 {
      
        $$ = ALLOC(block_node) ;
        $$->is.local_global_var.dcl = $1 ;
        $$->scope = current_scope ;
        if( $1 ) {
          $$->type =
            $1->is.variable.declare->is_local ? 
              JD_local_var_dcl : JD_global_var_dcl ;
          if( $1->is.variable.declare->is_static ) {
            ERROR_LOCATION( $1->filename, $1->linenum ) ;
            ERROR_ILLEGAL_STATIC_ATTRIBUTE_FOR_LOCAL_GLOBAL_VARIABLE ;
          }
          if( $1->is.variable.declare->is_final ) {
            ERROR_LOCATION( $1->filename, $1->linenum ) ;
            ERROR_ILLEGAL_FINAL_ATTRIBUTE_FOR_LOCAL_GLOBAL_VARIABLE ;
          }
          if( $1->is.variable.declare->is_clone ) {
            ERROR_LOCATION( $1->filename, $1->linenum ) ;
            ERROR_ILLEGAL_CLONE_ATTRIB_FOR_VARIABLE_DECLARATION ; 
          }
        }
      }
    }
  }
  ;


/***********************************
 *  initial statement
 ***********************************/


top_initial_statement:
  JDT_initial 
  {
    /* top_initial_statement_begin */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(block_node) ;
      $$->type = JD_initial_statement_block ;
      $$->parent = current_block ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      current_block = $$ ; /* set new block since statement will be parsed */
      in_block++ ;  /* prohibit static usage */
    }
  }
  statement
  {
    /* initial_statement */
    $$ = NULL ;
    ERROR_CLR_BLOCK {
      PASS2 {
        $$ = $2 ;
        $$->is.initial.statement = $3 ;
        current_block = $$->parent ;
        in_block-- ;  /* control static usage */
      }
    }
  }
  ;
  
initial_statement:
  JDT_initial 
  {
    /* initial_statement_begin */
    SKIP_ON_ERROR ;
    PASS2 {
      in_block++ ;  /* prohibit static usage */
      class_initial_code = 1 ;
    }
  }
  statement 
  {
    /* initial_statement_end */
    SKIP_ON_ERROR ;
    PASS2 {
      in_block-- ;  /* control static usage */
      class_initial_code = 0 ;
    }
    $$ = $3 ;
  }
  ;



/***********************************
 *  variable and enum definition
 ***********************************/


opt_var_enum_definitions:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  var_enum_definitions
  {
    $$ = $1 ; 
  }
  ;

var_enum_definitions:
  var_enum_definition
  {
    $$ = $1 ; 
  }
  |
  var_enum_definitions var_enum_definition
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  var_enum_definitions error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;
  
var_enum_definition:
  var_definition
  {
    $$ = $1 ; 
  }
  |
  enum_type_definition 
  {
    $$ = $1 ; 
  }
  ;



/***********************************
 *  variable definition
 ***********************************/


var_definition:
  opt_var_func_attrib var_func_type var_name_list semicolon
  {
    /* var_defs */
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *variable = $3 ;
      while( !error_count && variable ) {
        variable->data_type = $2 ;
        if( findname_in_scope( current_scope, variable->name ) ) {
          ERROR_LOCATION( variable->filename, variable->linenum ) ;
          ERROR_DUPLICATED_NAME( variable->name ) ;
        }
        else {
          addname( variable ) ;
          set_var_def_flags( variable, class_parsing, aspect_parsing ) ;
        }
        variable = variable->next ; 
      }
    }
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_vardeclare_statement ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      $$->is.variable.declare = ALLOC(var_declaration) ;
      if($2) $$->is.variable.declare->data_type = $2 ;
      $$->is.variable.declare->var_list = $3 ;
      {
        named_node *variable = $3 ;
        while( variable ) {
          variable->data_type = $2 ;
          if( variable->info.var.init ) {
            if( !check_init_data_type( 
                   variable->data_type, variable->info.var.init, 1 ) 
              ) 
            {
              ERROR_LOCATION( $$->filename, $$->linenum ) ;
              ERROR_ILLEGAL_DATA_TYPE_FOR_INITIALIZATION( variable->name ) ; 
            }
            if( variable->info.var.init->type == JD_rnd_exp || 
                variable->info.var.init->type == JD_cyc_exp    )
            {
              expression_node *rnd = variable->info.var.init ;
              switch( variable->data_type->type ) {
                case JD_int_type:
                  rnd->is.rnd.dst_width = -1 ;
                  break ;
                case JD_bit_type:
                  if( variable->data_type->i_endian )
                    rnd->is.rnd.dst_width = 
                      variable->data_type->lb - variable->data_type->ub + 1 ;
                  else
                    rnd->is.rnd.dst_width = 
                      variable->data_type->ub - variable->data_type->lb + 1 ;
                  break ;
              }
            }
          }
          variable->is_static = is_static_flag ;
          variable->is_local = 
            is_local_flag | ( is_static_flag & in_local_function ) ;
          variable->is_clone = is_clone_flag ;
          variable = variable->next ; 
        }
      }
      $$->is.variable.declare->is_var = 0 ;
      $$->is.variable.declare->is_arg = 0 ;
      $$->is.variable.declare->is_static = is_static_flag ;
      $$->is.variable.declare->is_local = 
        is_local_flag | ( is_static_flag & in_local_function ) ;
      $$->is.variable.declare->is_final = is_final_flag ;
      $$->is.variable.declare->is_extern = is_extern_flag ;
      $$->is.variable.declare->is_global = is_global_flag ;
      $$->is.variable.declare->is_clone = is_clone_flag ;
      
      if( is_static_flag && (in_block > 1) ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_STATIC_VARIABLE_DECLARATION_WITHIN_LOCAL_BLOCK ;
      }
      if( is_abstract_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_ABSTRUCT_ATTRIB_FOR_VARIABLE_DECLARATION ;
      }
      if( is_export_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_EXPORT_ATTRIB_FOR_VARIABLE_DECLARATION ;
      }
      if( is_static_flag && no_static_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_STATIC_ATTRIB_FOR_VARIABLE_DECLARATION ;
      }
      if( is_local_flag && no_local_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_LOCAL_ATTRIB_FOR_VARIABLE_DECLARATION ;
      }
      if( is_final_flag && no_final_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_FINAL_ATTRIB_FOR_VARIABLE_DECLARATION ;        
      }
      if( is_extern_flag && no_extern_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_EXTERN_ATTRIB_FOR_VARIABLE_DECLARATION ;        
      }
      if( is_clone_flag && no_clone_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_CLONE_ATTRIB_FOR_VARIABLE_DECLARATION ; 
      }
      if( is_global_flag && no_global_flag ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_GLOBAL_ATTRIB_FOR_VARIABLE_DECLARATION ; 
      }
      if( ( is_global_flag && is_local_flag )  ||
          ( is_extern_flag && is_global_flag ) ||
          ( is_extern_flag && is_clone_flag ) ||
          ( is_local_flag  && is_clone_flag ) ||
          ( is_global_flag && is_clone_flag ) ||
          ( is_local_flag && is_extern_flag )     ) {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_COMBINATION_OF_VARIABLE_ATTRIBUTE ;
      }
    }  
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
  ;

var_name_list:
  var_name 
  {
    $$ = $1 ; 
  }
  |
  var_name_list ',' var_name
  {
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
var_name:
  Identifier opt_array_range opt_var_init
  {
    /* var_name */
    SKIP_ON_ERROR ;
    PASS1 {
      $$ = ALLOC(named_node) ;
      if( class_parsing || aspect_parsing )
        $$->type = JD_mvar_name ;
      else 
        $$->type = JD_var_name ;
      $$->name = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      $$->info.var.range = $2 ;
      if( $3 ) { /* initialize variable */
        /* Can't check the type here, since we don't know the type yet */
        if( $2 != NULL ) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_INITIALIZATION_ON_ARRAY_VARIABLE ;
        }
      }
      if( $2 == NULL ) $$->sub_type = JD_single_sub_type ;
      else if( $2->type == JD_single_range_exp ) {
        /* [n]([n]..) */
        if( $2->is.range.num_index > 1 ) 
          $$->sub_type = JD_multi_array_sub_type ;
        else
          $$->sub_type = JD_array_sub_type ;
      }
      else if( $2->type == JD_assoc_array_exp ) /* [] */
        $$->sub_type = JD_assoc_sub_type ;
      else if( $2->type == JD_stridx_array_exp ) /* [*] */
        $$->sub_type = JD_stridx_sub_type ;
      else if( $2->type == JD_list_array_exp )   /* <> */
        $$->sub_type = JD_list_sub_type ;
    }
    PASS2{
      $$ = findname_in_scope( current_scope, $1.name ) ;
      if( !$$ || ( $$->type != JD_mvar_name && $$->type != JD_var_name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
      $$->info.var.init = $3 ;
    }
  }
  ;

/*
  An array can be statically allocated (e.g. ary[5]), string indexed
  associative array (e.g. sary[*]), associative array (e.g. asary[]),
  and list (e.g. list <> ).
*/
opt_array_range:
  /* emptry */
  {
    $$ = NULL ;
  }
  |
  array_ranges  
  {
    /* array_ranges */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = $1 ;
      if( $$ ) {
        expression_node *index = $$ ;
        int i = 0 ;
      
        /* setting how many index are used (nulti-dimention) */
        while( index ) {
          i++ ;
          index = index->next ;  
        }
        $$->is.range.num_index = i ;
      }
    }
  }
  |
  '['  '*'  ']' 
  {
    /* string_indexed_array_range */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_stridx_array_exp ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
    }
  }
  |
  '['  ']'
  {
    /* assoc_array_range */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_assoc_array_exp ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
    }
  }
  |
  '<'  '>' 
  {
    /* list_range */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_list_array_exp ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
    }
  }
  ;
  
array_ranges:
  array_range
  {
    $$ = $1 ; 
  }
  |
  array_ranges array_range
  {
    SKIP_ON_ERROR ;
    PASS1 {
      expression_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
array_range:
  '[' constant ']'
  {
    /* array_range */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_single_range_exp ;
      $$->is.range.ub = $2 ;
      if( !eval_to_int( $2, &$$->is.range.int_ub ) || $$->is.range.int_ub < 0 ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_ARRAY_INDEX ;
      }
      $$->is.range.num_index = 1 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
    }
  }
  ;

opt_var_init:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  var_init 
  {
    $$ = $1 ; 
  }
  ;
  
var_init:
  '=' expression 
  {
    $$ = $2 ; 
  }
  |
  '=' new_expression
  {
    $$ = $2 ; 
  }
  ;



/***********************************
 *  data types
 ***********************************/


opt_func_type:
  /* empty */
  {
    /* void */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
    }
  }
  |
  var_func_type opt_array_range
  {
    /* func_type */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = $1 ;
      if( $$ != NULL ) {
        $$->info.var.range = $2 ;
        if( $2 ) {
          switch( $2->type ) {
            case JD_single_range_exp:
              if( $2->is.range.num_index > 1 ) 
                $$->sub_type = JD_multi_array_sub_type ;
              else
                $$->sub_type = JD_array_sub_type ;
              break ;
            case JD_stridx_array_exp:
              $$->sub_type = JD_stridx_sub_type ;
              break ;
            case JD_assoc_array_exp:
              $$->sub_type = JD_assoc_sub_type ;
              break ;
            case JD_list_array_exp:
              $$->sub_type = JD_list_sub_type ;
              break ;
          }
        }
        else {
          $$->sub_type = JD_single_sub_type ;
        }
      }
    }
  }
  ;
  
var_func_type:  
  data_type  
  {
    $$ = $1 ; 
  }
  |
  user_data_type 
  {
    $$ = $1 ; 
  }
  ;
  
data_type:
  JDT_void
  {
    /* void */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "void" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_int 
  {
    /* int */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_int_type ;
      $$->name = "int" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_float  
  {
    /* float */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_float_type ;
      $$->name = "float" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_double  
  {
    /* double */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_double_type ;
      $$->name = "double" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_bit opt_bit_range
  {
    /* bit */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_bit_type ;
      $$->name = "bit" ;
      if( $2 ) {
        $$->ub = $2->is.range.int_ub ;
        $$->lb = $2->is.range.int_lb ;
        $$->i_endian = $2->is.range.i_endian ;
        DEALOC($2) ;
      }
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_string
  {
    /* string */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_string_type ;
      $$->name = "string" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_signal
  {
    /* signal */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_signal_type ;
      $$->name = "signal" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_class_type
  {
    /* class_type */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->name = "class_type" ;
      $$->type = JD_class_type ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
  
user_data_type: 
  Identifier
  {
    /* user_type */
    SKIP_ON_ERROR ;
    PASS1 {
      $$ = find_user_type( $1.name ) ;
    }
    PASS2 {
      $$ = find_user_type( $1.name ) ;
      if($$ == NULL ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_FOR_USER_DATA_TYPE($1.name) ; 
      }
    }  
  }
  ;



/***********************************
 *  type definition
 ***********************************/


type_definition:
  enum_type_definition 
  {
    $$ = $1 ; 
  }
  |
  class_type_definition
  {
    $$ = NULL ;
  }
  ;
  
enum_type_definition:
  JDT_enum Identifier '=' enum_members_list semicolon
  {
    /* new_enum_type_dec */
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *enm ;
      named_node *member ;
      int n ;
      if( findname_in_scope( current_scope, $2.name ) ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_DUPLICATED_NAME( $2.name ) ;
        break ;
      }
      else {
        enm = ALLOC(named_node) ;
        enm->type = JD_enum_type ;
        enm->name = $2.name ;
        enm->filename = $2.filename ;
        enm->linenum = $2.linenum ;
        addname( enm ) ;
        enm->info.enum_members = $4 ;
        member = $4 ;
        n = 0 ;
        while( member ) {
          if( findname_in_scope( current_scope, member->name ) ) {
            ERROR_LOCATION( member->filename, member->linenum ) ;
            ERROR_DUPLICATED_NAME( member->name ) ;
          }
          addname( member ) ;
          member->data_type = enm ;
          member->info.enum_body = enm ;
          member->index = n++ ;
          member = member->next ;
        }
      }
    }
    PASS2{
      named_node *enm ;
      enm = findname_in_scope( current_scope, $2.name ) ;
      if( !enm || enm->type != JD_enum_type ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($2.name) ;
      }
      $$ = ALLOC(statement_node) ;
      $$->type = JD_enum_def_statement ;
      $$->is.enum_def.edf = enm ;
      $$->filename = $5.filename ;
      $$->linenum = $5.linenum ;
    }  
  }
  ;

enum_members_list:
  enum_member
  {
    $$ = $1 ; 
  }
  |
  enum_members_list ',' enum_member
  {
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
enum_member: 
  Identifier 
  {
    /* enum_member */
    SKIP_ON_ERROR ;
    PASS1 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_enum_member_name ;
      $$->name = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
  

class_type_definition:
  JDT_typedef JDT_class class_name_list semicolon 
  {
    $$ = NULL ;
  }
  ;
  
class_name_list:
  class_name
  {
    $$ = $1 ; 
  }
  |
  class_name_list ',' class_name
  {
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;


data_type_name:
  JDT_array 
  {
    /* array */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "array" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_marray 
  {
    /* array */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "marray" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_assoc 
  {
    /* assoc */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "assoc" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_stridx  
  {
    /* stridx */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "stridx" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_list  
  {
    /* list */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_void_type ;
      $$->name = "list" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_string
  {
    /* string */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_string_type ;
      $$->name = "string" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_signal
  {
    /* signal */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_signal_type ;
      $$->name = "signal" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_port
  {
    /* signal */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(named_node) ;
      $$->type = JD_dport_type ;
      $$->name = "port" ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

class_name:
  Identifier
  {
    /* class_name */
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *cls ;
      if( cls = findname_in_scope( top_scope, $1.name ) ) {
        if(cls->type != JD_class_name) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( $1.name ) ;
        }
        $$ = cls ;
      }
      else {
        $$ = ALLOC(named_node) ;
        if( jeda_util_parsing ) $$->type = JD_sys_class_name ;
        else $$->type = JD_class_name ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        addname( $$ ) ;
      }
    }
    PASS2{
      named_node *cls ;
      if( cls = findname_in_scope( top_scope, $1.name ) ) {
        if(cls->type != JD_class_name && cls->type != JD_sys_class_name) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( $1.name ) ;
        }
        $$ = cls ;
      }
    }
  }
  |
  '.' data_type_name
  {
    /* dot_class_name */
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *cls ;
      char *dot_name ;
      dot_name = (char *)calloc( strlen($2->name)+2, sizeof(char) ) ;
      dot_name[0] = '.' ;
      strcat( &dot_name[1], $2->name ) ;
      if( cls = findname_in_scope( top_scope, dot_name ) ) {
        if(cls->type != JD_sys_class_name) {
          ERROR_LOCATION( $2->filename, $2->linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( dot_name ) ;
        }
        $$ = cls ;
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->type = JD_sys_class_name ;
        $$->name = dot_name ;
        $$->filename = $2->filename ;
        $$->linenum = $2->linenum ;
        addname( $$ ) ;
      }
    }
    PASS2{
      named_node *cls ;
      char *dot_name ;
      dot_name = (char *)calloc( strlen($2->name)+2, sizeof(char) ) ;
      dot_name[0] = '.' ;
      strcat( &dot_name[1], $2->name ) ;
      if( cls = findname_in_scope( top_scope, dot_name ) ) {
        if(cls->type != JD_sys_class_name) {
          ERROR_LOCATION( $2->filename, $2->linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( dot_name ) ;
        }
        $$ = cls ;
      }
    }
  }
  ;

class_name_ref:
  Identifier
  {
    /* class_name_ref */
    SKIP_ON_ERROR ;
    PASS1_2 {
      named_node *cls ;
      if( cls = findname_in_scope( top_scope, $1.name ) ) {
        if(cls->type != JD_class_name) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( $1.name ) ;
        }
        $$ = cls ;
      }
      else {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS( $1.name ) ;
        $$ = NULL ;
      }
    }
  }
  ;



/***********************************
 *  variable, function & class attributes
 ***********************************/
 
/* the followings are all the possible attributes combinations for variable,
   function, and class. 
   Here's the keyword and valid type:
   
   static   :  variable
   extern   :  variable, function, class. body of function should be empty.
   local    :  variable, function, class
   private  :  variable in class, function in class
   abstract  :  class, function in abstract class 
   overwrite : function in class
   
*/



/* This definition is globally shared by all the types
   for persing reason, and semantics must be handled at action level per 
   each type.
*/

opt_var_func_attrib:
  /* empty */
  {
    $$ = NULL ;
  }
  | 
  var_func_attribs 
  {
    $$ = NULL ;
  }
  ;
  
var_func_attribs: 
  var_func_attrib
  {
    $$ = NULL ;
  }
  |
  var_func_attribs var_func_attrib
  {
    $$ = NULL ;
  }
  ;
  
var_func_attrib:
  JDT_static
  {
    /* static */
    PASS1_2 {
      is_static_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_extern
  {
    /* extern */
    PASS1_2 {
      is_extern_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_local
  {
    /* local */
    PASS1_2 {
      is_local_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_global
  {
    /* global */
    PASS1_2 {
      is_global_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_protected
  {
    /* protected */
    PASS1_2 {
      is_protected_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_abstract
  {
    /* abstract */
    PASS1_2 {
      is_abstract_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_final
  {
    /* final */
    PASS1_2 {
      is_final_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_export
  {
    /* final */
    PASS1_2 {
      is_export_flag = 1 ;
    }
    $$ = NULL ;
  }
  | 
  JDT_clone
  {
    /* clone */
    PASS1_2 {
      is_clone_flag = 1 ;
    }
    $$ = NULL ;
  }
  |
  JDT_overwrite
  {
    /* overwrite */
    PASS1_2 {
      is_overwrite_flag = 1 ;
    }
    $$ = NULL ;
  }
  ;
  


/***********************************
 *  function definition
 ***********************************/


function_definition:
  opt_var_func_attrib 
  opt_func_type 
  func_name
  '(' 
  {
    /* func_name_found */
    SKIP_ON_ERROR ;
    in_function = 1 ;
    in_local_function = is_local_flag ;
    current_func_name = $3 ;
    PASS1 {
      if( $3 ) {
        if( $3->type == JD_unknown_name ) {
          if( class_parsing || aspect_parsing ) {
            $3->type = JD_mfunc_name ;
          }
          else {
            $3->type = JD_func_name ;
          }
        }
      }
      enter_scope( $3 ) ;
      $3->data_type = $2 ;
    }
    PASS2 {
      $3->data_type = $2 ;
      enter_scope( $3 ) ;
      /* save flag value to yacc stack */
      $<flags>$.is_static_flag = is_static_flag ;
      $<flags>$.is_extern_flag = is_extern_flag ;
      $<flags>$.is_local_flag = is_local_flag ;
      $<flags>$.is_protected_flag = is_protected_flag ;
      $<flags>$.is_overwrite_flag = is_overwrite_flag ;
      $<flags>$.is_abstract_flag = is_abstract_flag ;
      $<flags>$.is_final_flag = is_final_flag ;
      $<flags>$.is_export_flag = is_export_flag ;
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
    opt_formal_args 
  ')' 
  {
    PASS2 {
      if( !abstract_arg_check( $6, 0 ) ) {
        ERROR_LOCATION( $7.filename, $7.linenum ) ;
        ERROR_ILLEGAL_MIXTURE_OF_ABSTRUCT_ARGUMENT ;
      }
    }
  }
  opt_outer_block
  {
    /* func_define */
    if( !in_function ) break ;
    current_func_name = NULL ;
    PASS1_2 {
      exit_scope() ;
      in_function = 0 ;
      in_local_function = 0 ;
    }
    ERROR_CLR_BLOCK {
      PASS2 {
        /* restore flag value from yacc stack */
        is_static_flag = $<flags>5.is_static_flag  ;
        is_extern_flag = $<flags>5.is_extern_flag  ;
        is_local_flag = $<flags>5.is_local_flag  ;
        is_protected_flag = $<flags>5.is_protected_flag  ;
        is_overwrite_flag = $<flags>5.is_overwrite_flag  ;
        is_abstract_flag = $<flags>5.is_abstract_flag  ;
        is_final_flag = $<flags>5.is_final_flag  ;
        is_export_flag = $<flags>5.is_export_flag  ;
        if( $9 == NULL ) { /* $9 = opt_outer_block */
          if( is_export_flag ) {
            ERROR_LOCATION( $7.filename, $7.linenum ) ;
            ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE ;
          }
          if( is_abstract_flag && aspect_parsing ) {
            ERROR_LOCATION( $7.filename, $7.linenum ) ;
            ERROR_ILLEGAL_ABSTRACT_ATTRIB_USAGE ;
          }
          /*
          if( is_overwrite_flag ) {
            ERROR_LOCATION( $7.filename, $7.linenum ) ;
            ERROR_ILLEGAL_OVERWRITE_ATTRIB_USAGE ;
          }
          */
          if( class_parsing || aspect_parsing ) {
            /* member function declaration */
            if( $3->info.func.class_block == NULL )
            {
              $$ = ALLOC(block_node) ;
              $$->type = JD_memberfunction_definition ;
              $$->name = $3 ;
              $$->name->data_type = $2 ;
              $$->is.func.args = $6 ;
              $3->info.func.args = $6 ;
              $$->linenum = $3->linenum ;
              $$->filename = $3->filename ;
              $3->info.func.class_block = current_block ;
              $3->info.func.func_block = $$ ;
              set_mfunc_def_flags( $3 ) ;
            }
            else check_member_func( $2, $3, $6 ) ;
          }
          else { 
            /* function declaration */
            if( $3->info.func.func_block == NULL ) {
              $$ = ALLOC(block_node) ;
              $$->type = JD_function_definition ;
              $$->name = $3 ;
              $$->name->data_type = $2 ;
              $$->is.func.args = $6 ;
              $3->info.func.args = $6 ;
              $$->linenum = $4.linenum ;
              $$->filename = $4.filename ;
              $3->info.func.func_block = $$ ;
              set_func_def_flags( $3 ) ;
              if( !strcmp( $$->name->name, "main" ) && $6 != NULL ) {
                ERROR_LOCATION( $7.filename, $7.linenum ) ;
                ERROR_ILLEGAL_MAIN_DEFINITION_WITH_ARGUMENT ;
              }
            }
            else check_func( $2, $3, $6 ) ;
          }
        }
        else {  /* body block exist */
          if( is_abstract_arg( $6 ) ) {
            ERROR_LOCATION( $7.filename, $7.linenum ) ;
            ERROR_ILLEGAL_USAGE_OF_ABSTRUCT_ARGUMENT ;
          }
          if( class_parsing || aspect_parsing ) {
            /* member function declaration */
            if( is_abstract_flag ) {
              ERROR_LOCATION( $7.filename, $7.linenum ) ;
              ERROR_ILLEGAL_ABSTRUCT_FUNCTION_WITH_ACTUAL_BLOCK ;
            }
            if( is_export_flag ) {
              ERROR_LOCATION( $7.filename, $7.linenum ) ;
              ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE ;
            }
            if( $3->info.func.class_block == NULL )
            {
              $$ = ALLOC(block_node) ;
              $$->type = JD_memberfunction_definition ;
              $$->name = $3 ;
              $$->is.func.block = $9 ;
              $$->is.func.args = $6 ;
              $3->info.func.args = $6 ;
              $$->linenum = $4.linenum ;
              $$->filename = $4.filename ;
              $3->info.func.class_block = current_block ;
              $3->info.func.func_block = $$ ;
              set_mfunc_def_flags( $3 ) ;
            }
            else if( $3->info.func.func_block->is.func.block ) {
              ERROR_LOCATION( $4.filename, $4.linenum ) ;
              ERROR_MULTIPLE_DECLARATION_OF_FUNCTION_FOUND ;
            }
            else {
              check_member_func( $2, $3, $6 ) ;
              $$->is.func.block = $9 ;
              $$->linenum = $4.linenum ;
              $$->filename = $4.filename ;
            }
          }
          else { 
            /* function declaration */
            if( is_export_flag && $2->type != JD_void_type ) {
              ERROR_LOCATION( $7.filename, $7.linenum ) ;
              ERROR_EXPORT_FUNCTION_MUST_BE_VOID_TYPE ;
            }
            if( $3->info.func.func_block == NULL ) {
              $$ = ALLOC(block_node) ;
              $$->type = JD_function_definition ;
              $$->name = $3 ;
              $$->is.func.block = $9 ;
              $$->is.func.args = $6 ;
              $3->info.func.args = $6 ;
              $$->linenum = $4.linenum ;
              $$->filename = $4.filename ;
              $3->info.func.func_block = $$ ;
              set_func_def_flags( $3 ) ;
              if( !strcmp( $$->name->name, "main" ) && $6 != NULL ) {
                ERROR_LOCATION( $7.filename, $7.linenum ) ;
                ERROR_ILLEGAL_MAIN_DEFINITION_WITH_ARGUMENT ;
              }
              if( is_export_flag ) {
                int n = 1 ;
                statement_node *farg =  $6 ;
                while( farg ) {
                  if( !is_bit_arg(farg) ) {
                    ERROR_LOCATION( $7.filename, $7.linenum ) ;
                    ERROR_ILLEGAL_NTH_ARG_TYPE_FOR_EXPORT_FUNCTION(n) ;
                  }
                  n++ ;
                  farg = farg->next ;
                }
              }
            }
            else if ($3->type != JD_func_name) {
              ERROR_LOCATION( $4.filename, $4.linenum ) ;
              ERROR_ILLEGAL_NAME_FOR_FUNCTION( $3->name ) ;
            }
            else if( $9 && $3->info.func.func_block->is.func.block ) {
                ERROR_LOCATION( $4.filename, $4.linenum ) ;
                ERROR_MULTIPLE_DECLARATION_OF_FUNCTION_FOUND ;
            }
            else {
              check_func( $2, $3, $6 ) ;
              $3->info.func.func_block->is.func.block = $9 ;
              $3->info.func.func_block->linenum = $4.linenum ;
              $3->info.func.func_block->filename = $4.filename ;
              $$ = NULL ;
            }
          }      
        }
      }
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
  ;

func_name:
  Identifier 
  {
    /* func_name */
    named_node *nm ;
    named_node *scope ;
    SKIP_ON_ERROR ;
    PASS1 {
      if( nm = findname_in_scope( current_scope, $1.name ) ) {
        $$ = nm ;
        if( 
          ((class_parsing || aspect_parsing) && nm->type != JD_mfunc_name) ||
          (!class_parsing && !aspect_parsing && nm->type != JD_func_name )   
          ) 
        {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_DUPLICATED_NAME($1.name) ;
        }
      }
      else {
        $$ = ALLOC(named_node) ;
        /* type can't be defined yet */
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        /* type can't be defined yet */
        $$->type = JD_unknown_name ;
        /* printf( "adding func_name %s\n", $$->name ) ; */
        addname( $$ ) ;
      }
    }
    PASS2 {
      nm = findname_in_scope( current_scope, $1.name ) ;
      $$ = nm ;
      if (nm == NULL) {
        printname_in_scope( current_scope ) ;
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
      else {
        if( 
          ((class_parsing || aspect_parsing) && nm->type != JD_mfunc_name) ||
          (!class_parsing && !aspect_parsing && nm->type != JD_func_name )   
          ) 
        {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
        }
      }
    }
  }
  ;


opt_outer_block:
  semicolon
  {
    $$ = NULL ;
  }
  |
  block
  {
    $$ = $1 ; 
  }
  ;

opt_formal_args:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  formal_args
  {
    $$ = $1 ; 
  }
  ;
  
formal_args:
  formal_arg
  {
    $$ = $1 ; 
  }
  |
  formal_args ',' formal_arg 
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;

formal_arg:
  opt_var var_func_type arg_name opt_array_range opt_arg_init 
  {
    /* formal_arg */
    SKIP_ON_ERROR ;
    PASS1 {
      if( $3 ) {
        $3->info.var.is_var = $1 ;
        $3->info.var.range = $4 ;
        $3->data_type = $2 ;
        if( $4 ) {
          switch( $4->type ) {
            case JD_single_range_exp:
              if( $4->is.range.num_index > 1 ) 
                $3->sub_type = JD_multi_array_sub_type ;
              else
                $3->sub_type = JD_array_sub_type ;
              break ;
            case JD_stridx_array_exp:
              $3->sub_type = JD_stridx_sub_type ;
              break ;
            case JD_assoc_array_exp:
              $3->sub_type = JD_assoc_sub_type ;
              break ;
            case JD_list_array_exp:
              $3->sub_type = JD_list_sub_type ;
              break ;
          }
        }
        else {
          $3->sub_type = JD_single_sub_type ;
        }
      }
    }
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_vardeclare_statement ;
      $$->is.variable.declare = ALLOC(var_declaration) ;
      $$->is.variable.declare->data_type = $2 ;
      $$->is.variable.declare->var_list = $3 ;
      $$->is.variable.declare->is_var = $1 ;
      $$->is.variable.declare->is_arg = 1 ;
      if( $3 ) {
        $3->data_type = $2 ;
        $3->info.var.init = $5 ;
        if( $3->info.var.init ) {
          if( !check_init_data_type( 
                 $3->data_type, $3->info.var.init, 0 ) 
            ) 
          {
            ERROR_LOCATION( $3->filename, $3->linenum ) ;
            ERROR_ILLEGAL_DATA_TYPE_FOR_INITIALIZATION( $3->name ) ; 
          }
        }
        $3->info.var.is_var = $1 ;
      }
    }
  }
  |
  var_func_type
  {
    /* formal_arg_type */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_formal_arg_statement ;
      $$->is.formal_arg.data_type = $1 ;
    }
  }
  |
  '?'
  {
    /* wild_card_arg */
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_wildcard_arg_statement ;
    }
  }
  |
  '(' formal_args ')' '+'
  {
    /* one_or_more_args */
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_one_or_more_arg_statement ;
      $$->is.formal_arg.child = $2 ;
    }
  }
  |
  '(' formal_args ')' '*'
  {
    /* zero_or_more_args */
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_zero_or_more_arg_statement ;
      $$->is.formal_arg.child = $2 ;
    }
  }
  |
  '(' formal_args ')' '|'
  {
    /* or_of_formal_args */
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_or_of_args_statement ;
      $$->is.formal_arg.child = $2 ;
    }
  }
  |
  '[' formal_args ']' 
  {
    /* opt_formal_args */
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_opt_args_statement ;
      $$->is.formal_arg.child = $2 ;
    }
  }
  ;

opt_var:
  /* empty */
  {
    $$ = 0 ;
  }
  |
  JDT_var 
  {
    $$ = 1 ;
  }
  ;
  
arg_name:
  Identifier
  {
    /* arg_name */
    SKIP_ON_ERROR ;
    PASS1 {
      if( findname_in_scope( current_scope, $1.name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_DUPLICATED_NAME($1.name) ;
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->type = JD_arg_name ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        addname( $$ ) ;
      }
    }
    PASS2 {
      $$ = findname_in_scope( current_scope, $1.name ) ;
      if( !$$ || $$->type != JD_arg_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
    }
  }
  ;

opt_arg_init:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  arg_init 
  {
    $$ = $1 ; 
  }
  ;
  
arg_init:
  '=' constant 
  {
    $$ = $2 ; 
  }
  |
  '=' string_constant
  {
    $$ = $2 ; 
  }
  |
  '=' bool_null_expression
  {
    $$ = $2 ; 
  }
  |
  '=' enum_expression
  {
    $$ = $2 ; 
  }
  ;

enum_expression:
  Identifier
  {
    /* enum_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *name ;
      
      name = findname( $1.name ) ;
      if( name == NULL ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNDEFINED_NAME($1.name) ;
      }
      else if( name->type != JD_enum_member_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_USAGE_FOR_ENUM($1.name) ;
      }
      else {
        $$ = ALLOC(expression_node) ;
        $$->type = JD_varport_exp ;
        $$->is.var.name = name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        set_var_type( $$, NULL, NULL ) ; /* exp, range, depth */
      }
    }
  }
  ;



/***********************************
 *  class definition
 ***********************************/


class_definition:
  opt_var_func_attrib JDT_class class_name 
  opt_extention
  {
    /* class_def_begin */
    SKIP_ON_ERROR ;
    PASS1_2{
      enter_class_scope( $3 ) ;
      class_parsing = 1 ;
      this_class_name = $3 ;
    }
    PASS1{
      $3->info.class_name.parent = $4 ;
      $3->is_abstract = is_abstract_flag ;
      $3->is_extern = is_extern_flag ;
      $3->is_local = is_local_flag ;
      $3->is_final = is_final_flag ;
    }
    PASS2 {
      /* at this point, name registration is already done by class_name */
      $<block>$ = ALLOC(block_node) ;
      $<block>$->name = $3 ;
      $3->info.class_name.block = $<block>$ ;
      $<block>$->filename = $2.filename ;
      $<block>$->linenum = $2.linenum ;
      $<block>$->type = JD_class_definition ;
      $<block>$->parent = current_block ;
      $<block>$->scope = current_scope ;
      current_block = $<block>$ ;
      $<block>$->is.class.is_abstract = is_abstract_flag ;
      $<block>$->is.class.is_extern = is_extern_flag ;
      $<block>$->is.class.is_local = is_local_flag ;
      $<block>$->is.class.is_final = is_final_flag ;
      $<block>$->linenum = $2.linenum ;
      $<block>$->filename = $2.filename ;
      in_file_used = 1 ;
      if( !check_extention($3, $4) ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS_EXTENTION( $4->name ) ;
      }
      if( is_static_flag ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_STATIC_ATTRIB_FOR_CLASS_DECLARATION ;
      }
      if( is_extern_flag && is_local_flag ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_COMBINATION_OF_EXTERN_AND_LOCAL ;
      }
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
  '{'
    opt_class_items
  '}' 
  {
    /* class_def_end */
    if( !class_parsing ) break ; /* error before entering class */
    PASS1_2{
      exit_scope( ) ;
      class_parsing = 0 ;
      this_class_name = NULL ;
    }
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = $<block>5 ;
      /* XXX may be this is not a right place to check consistency, as
        it is possible that its parent is not yet fully parsed in PASS_2
      check_abstract_members( $$->name, $8.filename, $8.linenum ) ;
      check_overwrite_members( $$->name, $8.filename, $8.linenum ) ;
       */
      current_block = $$->parent ;
    }
  }
  ;

opt_extention:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  JDT_extends class_name_ref
  {
    $$ = $2 ; 
  }
  ;
  
opt_class_items:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  class_items
  {
    $$ = NULL ;
  }
  ;
  
class_items:
  class_item
  {
    $$ = NULL ;
  }
  |
  class_items class_item
  {
    $$ = NULL ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  class_items error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;

class_item:
  var_definition
  {
    /* class_var_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.members == NULL ) {
          current_block->is.class.members = $1 ;
        }
        else {
          statement_node *member = current_block->is.class.members ;
          while( member->next ) member = member->next ;
          member->next = $1 ;
        }
      }
      if( $1 )
      {
        named_node *pmem ;
        named_node *variable = $1->is.variable.declare->var_list ;
        while( variable ) {
          if( this_class_name ) {
            pmem = 
              findname_in_classname( 
                this_class_name->info.class_name.parent, 
                variable->name 
              ) ;
            if( pmem && pmem->is_final ) {
              ERROR_LOCATION( variable->filename, variable->linenum ) ;
              ERROR_ILLEGAL_REDEFINITION_OF_FINAL_MEMBER_NAME(variable->name) ;
            }
          }
          variable->is_member = 1 ;
          variable = variable->next ; 
        }
      }
    }
  }
  |
  enum_type_definition 
  {
    /* class_enum_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.edf == NULL ) {
          current_block->is.class.edf = $1 ;
        }
        else {
          statement_node *edf = current_block->is.class.edf ;
          while( edf->next ) edf = edf->next ;
          edf->next = $1 ;
        }
      }
    }
  }
  |
  initial_statement
  {
    /* class_initial_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.initial == NULL ) {
          current_block->is.class.initial = $1 ;
        }
        else {
          statement_node *initial = current_block->is.class.initial ;
          while( initial->next ) initial = initial->next ;
          initial->next = $1 ;
        }
      }
    }
  }
  |
  function_definition
  {
    /* class_function_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.member_funcs == NULL ) {
          current_block->is.class.member_funcs = $1 ;
        }
        else {
          block_node *member_funcs = current_block->is.class.member_funcs ;
          while( member_funcs->next ) member_funcs = member_funcs->next ;
          member_funcs->next = $1 ;
        }
      }
      if( $1 ) {
        named_node *pmem ;
        if( this_class_name ) {
          pmem = 
            findname_in_classname( 
              this_class_name->info.class_name.parent,
              $1->name->name 
            ) ;
          if( pmem && pmem->is_final ) {
            ERROR_LOCATION( $1->name->filename, $1->name->linenum ) ;
            ERROR_ILLEGAL_REDEFINITION_OF_FINAL_MEMBER_NAME($1->name->name) ;
          }
        }
        $1->name->is_member = 1 ;
      }
    }
  }
  |
  new_function_definition
  {
    /* class_function_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.member_funcs == NULL ) {
          current_block->is.class.member_funcs = $1 ;
        }
        else {
          block_node *member_funcs = current_block->is.class.member_funcs ;
          while( member_funcs->next ) member_funcs = member_funcs->next ;
          member_funcs->next = $1 ;
        }
      }
      if( $1 ) {
        named_node *pmem ;
        if( this_class_name ) {
          pmem = 
            findname_in_classname( 
              this_class_name->info.class_name.parent,
              $1->name->name 
            ) ;
          if( pmem && pmem->is_final ) {
            ERROR_LOCATION( $1->name->filename, $1->name->linenum ) ;
            ERROR_ILLEGAL_REDEFINITION_OF_FINAL_MEMBER_NAME($1->name->name) ;
          }
        }
        $1->name->is_member = 1 ;
      }
    }
  }
  ;
  
new_function_definition:
  opt_void JDT_new
  '(' 
  {
    /* new_func_found */
    named_node *nm ;
    SKIP_ON_ERROR ;
    PASS1 {
      if( !(nm = findname_in_scope( current_scope, "new" ) ) ) {
        nm = ALLOC(named_node) ;
        nm->name = (char *)strdup("new") ;
        nm->filename = $2.filename ;
        nm->linenum = $2.linenum ;
        nm->type = JD_unknown_name ;
        addname( nm ) ;
        set_new_def_flags( nm ) ;
      }
      enter_scope( nm ) ;
    }
    PASS2 {
      nm = findname_in_scope( current_scope, "new" ) ;
      if (nm == NULL) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2("new") ;
      }
      enter_scope( nm ) ;
    }
  }
    opt_formal_args 
  ')' 
  {
    PASS2 {
      if( !abstract_arg_check( $5, 0 ) ) {
        ERROR_LOCATION( $6.filename, $6.linenum ) ;
        ERROR_ILLEGAL_MIXTURE_OF_ABSTRUCT_ARGUMENT ;
      }
    }
  }
  opt_new_outer_block
  {
    /* new_func_define */
    named_node *nm ;
    PASS1_2 {
      exit_scope() ;
    }
    ERROR_CLR_BLOCK {
      PASS2 {
        nm = findname_in_scope( current_scope, "new" ) ;
        if (nm == NULL) {
          ERROR_LOCATION( $2.filename, $2.linenum ) ;
          ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2( "new" ) ;
        }
        if( $8 == NULL ) { /* $8 = opt_outer_block */
          /* member function declaration */
          if( nm->type == JD_unknown_name ) {
            $$ = ALLOC(block_node) ;
            $$->type = JD_memberfunction_definition ;
            $$->name = nm ;
            $$->is.func.args = $5 ;
            $$->filename = $2.filename ;
            $$->linenum = $2.linenum ;
            nm->data_type = current_block->name ;
            nm->type = JD_mfunc_name ;
            nm->info.func.class_block = current_block ;
            nm->info.func.func_block = $$ ;
            nm->info.func.args = $5 ;
          }
          else check_member_func( nm->data_type, nm, $5 ) ;
        }
        else {  /* body block exist */
          /* member function declaration */
          if( is_abstract_arg( $5 ) ) {
            ERROR_LOCATION( $6.filename, $6.linenum ) ;
            ERROR_ILLEGAL_USAGE_OF_ABSTRUCT_ARGUMENT ;
          }
          if( nm->type == JD_unknown_name ) {
            $$ = ALLOC(block_node) ;
            $$->type = JD_memberfunction_definition ;
            $$->name = nm ;
            $$->is.func.block = $8 ;
            $$->is.func.args = $5 ;
            $$->filename = $2.filename ;
            $$->linenum = $2.linenum ;
            nm->data_type = current_block->name ;
            nm->type = JD_mfunc_name ;
            nm->info.func.class_block = current_block ;
            nm->info.func.func_block = $$ ;
            nm->info.func.args = $5 ;
          }
          else if (nm->type != JD_mfunc_name) {
            ERROR_LOCATION( $2.filename, $2.linenum ) ;
            ERROR_ILLEGAL_NAME_FOR_MEMBER_FUNCTION( nm->name ) ;
          }
          else if( nm->info.func.func_block->is.func.block ) {
            ERROR_LOCATION( $2.filename, $2.linenum ) ;
            ERROR_MULTIPLE_DECLARATION_OF_FUNCTION_FOUND ;
          }
          else {
            check_member_func( nm->data_type, nm, $5 ) ;
            $$->is.func.block = $8 ;
            $$->filename = $2.filename ;
            $$->linenum = $2.linenum ;
          }
          {
            expression_node *data ;
            named_node *super ;
            named_node *mfunc ;
            int tmp ;
            data = $8->is.block.super_new ;
            super = this_class_name->info.class_name.parent ;
            if( data != NULL ) {
              /* explisit super_new call */
              
              if( !super ) {
                ERROR_LOCATION( data->filename, data->linenum ) ;
                ERROR_ILLEGAL_SUPER_NEW_CALL ;
              }
              else {
                mfunc = findmember_in_class( super, "new", &tmp ) ;
                if( mfunc ) {
                  /* can't check here, do it expression.c 
                  actual_arg_check(
                    mfunc, data->is.new.args, NULL, data->filename, data->linenum
                  ) ;
                  */
                }
                else {
                  if( data->is.new.args != NULL ) {
                    ERROR_LOCATION(data->filename, data->linenum) ;
                    ERROR_NUMBER_OF_ARGUMENT_MISMATCH_ON_NEW_FUNCTION ;
                  }
                }
              }
            }
            else {
              /* implicit call, super.new should have default args, or no arg */
              if( super ) {
                mfunc = findmember_in_class( super, "new", &tmp ) ;
                if( mfunc ) {
                  if( !arg_end_check(mfunc->info.func.args) ) {
                    ERROR_LOCATION( $2.filename, $2.linenum ) ;
                    ERROR_SUPER_NEW_CALL_REQUIRES_ARGMENTS ;
                  }
                }
              }
            }
          }
        }
      }
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
  ;

opt_new_outer_block:
  semicolon
  {
    $$ = NULL ;
  }
  |
  new_block
  {
    $$ = $1 ; 
  }
  ;

new_block:
  '{'
  {
    /* block_begin */
    SKIP_ON_ERROR ;
    PASS1_2 {
      enter_scope( current_scope ) ;
      in_block++ ;
      /* repeat_level = 0 ; */
    }
    PASS2 {
      current_block_level++ ;
    }
  }
    opt_var_enum_definitions
    opt_super_new_call
    opt_statements
  '}'
  {
    /* new_block_statement */
    if( !in_block ) break ;
    PASS1_2 {
      exit_scope() ;
      in_block-- ;
    }
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *st ;
      
      current_block_level-- ;
      $$ = ALLOC(statement_node) ;
      $$->type = JD_block ;
      $$->is.block.level = current_block_level ;
      $$->is.block.vars = $3 ;
      $$->is.block.super_new = $4 ;
      $$->is.block.statements = $5 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

opt_super_new_call:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  JDT_super '.' JDT_new opt_new_args semicolon
  {
    /* super_new_call */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_new_exp ;
      $$->data_type = JD_exp_new_type ;
      $$->is.new.args = $4 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

opt_void:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  JDT_void
  {
    $$ = NULL ;
  }
  ;
  


/***********************************
 *  verilog class definition
 ***********************************/


verilog_class_definition:
  opt_var_func_attrib JDT_class JDT_verilog '{'
  {
    /* verilog_class_begin */
    SKIP_ON_ERROR ;
    PASS1_2 {
      named_node *vlog ;
      class_parsing = 1 ;
      vlog = findname_in_scope( current_scope, "verilog" ) ;
      if( vlog == NULL ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_VERILOG_CLASS_NOT_FOUND ;
      }
      enter_class_scope( vlog ) ;
    }
    PASS2 {
      $$ = ALLOC(block_node) ;
      $$->name = findname_in_scope( current_scope, "verilog" ) ;
      $$->type = JD_verilog_class ;
      $$->parent = current_block ;
      current_block = $$ ;
      if( is_abstract_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_ABSTRUCT_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
      if( is_extern_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        WARNING_ILLEGAL_EXTERN_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
      if( is_local_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_LOCAL_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
      if( is_final_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_FINAL_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
      if( is_export_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_EXPORT_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
      if( is_static_flag ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_STATIC_ATTRIB_FOR_VERILOG_CLASS_DECLARATION ;
      }
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
     verilog_task_definitions  
  '}'
  {
    /* verilog_class_end */
    class_parsing = 0 ;
    SKIP_ON_ERROR ;
    PASS1_2 {
      exit_scope() ;
    }
    PASS2 {
      current_block = $<block>5->parent ;
      $$ = $5 ;
    }
  }
  ;
  
verilog_task_definitions:
  verilog_task_definition
  {
    $$ = NULL ;
  }
  |
  verilog_task_definitions verilog_task_definition
  {
    $$ = NULL ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  verilog_task_definitions error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;

verilog_task_definition:
  opt_func_type 
  func_name
  '(' 
  {
    /* veri_func_name_found */
    SKIP_ON_ERROR ;
    PASS1_2 {
      enter_scope( $2 ) ;
    }
  }
    opt_formal_args 
  ')' 
  String_literal semicolon
  {
    /* veri_func_define */
    /* note that named_node for verilog function does not have arg names
       as its child. Assuming that no futher code will be extended so
       that there's no reason to have name structure
     */
    SKIP_ON_ERROR ;
    PASS1_2 {
      exit_scope() ;
    }
    PASS1 {
      $2->data_type = $1 ;
      $2->type = JD_mfunc_name ;
    }
    PASS2 {
      if( is_abstract_arg( $5 ) ) {
        ERROR_LOCATION( $8.filename, $8.linenum ) ;
        ERROR_ILLEGAL_USAGE_OF_ABSTRUCT_ARGUMENT ;
      }
      if( $1->type != JD_void_type ) {
        ERROR_LOCATION( $8.filename, $8.linenum ) ;
        ERROR_VERILOG_CLASS_FUNCTION_MUST_BE_VOID_TYPE ;
      }
      if( $2->info.func.func_block == NULL ) {
        $$ = ALLOC(block_node) ;
        $$->type = JD_verilog_function ;
        $$->name = $2 ;
        $$->filename = $8.filename ;
        $$->linenum = $8.linenum ;
        $$->is.func.args = $5 ;
        $$->is.func.path = $7.name ;
        $2->info.func.class_block = current_block ;
        $2->info.func.func_block = $$ ;
        $2->info.func.args = $5 ;
        {
          int n = 1 ;
          statement_node *farg =  $5 ;
          while( farg ) {
            if( !is_simple_arg(farg) ) {
              ERROR_LOCATION( $8.filename, $8.linenum ) ;
              ERROR_ILLEGAL_NTH_ARG_TYPE_FOR_VERILOG_TASK(n) ;
            }
            n++ ;
            farg = farg->next ;
          }
        }
        if( current_block ) {
          if( current_block->is.user_veri.funcs == NULL )
            current_block->is.user_veri.funcs = $$ ;
          else {
            block_node *last = current_block->is.user_veri.funcs ;
            while( last->next ) last = last->next ;
            last->next = $$ ;
          }
        }
      }
      else {
        if ($2->type != JD_verilog_function) { 
          /* can't happen?? */
          ERROR_LOCATION( $8.filename, $8.linenum ) ;
          ERROR_ILLEGAL_NAME_FOR_MEMBER_FUNCTION( $2->name ) ;
        }
        else check_member_func( $1, $2, $5 ) ; /* may need special check */
        if( strcmp( $2->info.func.func_block->is.func.path, $7.name ) ) {
          ERROR_LOCATION( $8.filename, $8.linenum ) ;
          ERROR_INCONSISTENT_VERILOG_TASK_PATH( $7.name ) ;
        }
        $$ = NULL ;
      }
    }
  }
  ;
  


/***********************************
 *  aspect definition
 ***********************************/


aspect_name:
  Identifier
  {
    /* aspect_name */
    SKIP_ON_ERROR ;
    PASS1 {
      named_node *asp ;
      if( asp = findname_in_scope( top_scope, $1.name ) ) {
        if(asp->type != JD_aspect_name) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_ASPECT( $1.name ) ;
        }
        $$ = asp ;
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->type = JD_aspect_name ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        addname( $$ ) ;
      }
    }
    PASS2{
      named_node *asp ;
      if( asp = findname_in_scope( top_scope, $1.name ) ) {
        if(asp->type != JD_aspect_name) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_USAGE_FOR_ASPECT( $1.name ) ;
        }
        $$ = asp ;
      }
    }
  }
  ;

aspect_definition:
  opt_extern JDT_aspect aspect_name 
  '{'
  {
    /* aspect_begin */
    SKIP_ON_ERROR ;
    PASS1_2{
      enter_class_scope( $3 ) ;
      aspect_parsing = 1 ;
      this_class_name = $3 ;
    }
    PASS1{
      $3->is_extern = is_extern_flag ;
    }
    PASS2 {
      /* at this point, name registration is already done by aspect_name */
      $<block>$ = ALLOC(block_node) ;
      $<block>$->name = $3 ;
      $3->info.class_name.block = $<block>$ ;
      $<block>$->filename = $2.filename ;
      $<block>$->linenum = $2.linenum ;
      $<block>$->type = JD_aspect_definition ;
      $<block>$->parent = current_block ;
      $<block>$->scope = current_scope ;
      current_block = $<block>$ ;
      $<block>$->is.class.is_extern = is_extern_flag ;
      in_file_used = 1 ;
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
    opt_aspect_items
  '}'
  {
    /* aspect_end */
    if( !aspect_parsing ) break ; /* error before entering class */
    PASS1_2{
      exit_scope( ) ;
      aspect_parsing = 0 ;
      this_class_name = NULL ;
    }
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = $<block>5 ;
      current_block = $$->parent ;
    }
  }
  ;
  
opt_extern:
  /* emptry */
  {
    $$ = NULL ;
  }
  | 
  JDT_extern
  {
    /* extern */
    PASS1_2 {
      is_extern_flag = 1 ;
    }
    $$ = NULL ;
  }
  ;

opt_aspect_items:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  aspect_items
  {
    $$ = $1 ; 
  }
  ;
     
aspect_items:
  aspect_item
  {
    $$ = NULL ;
  }
  |
  aspect_items aspect_item
  {
    $$ = NULL ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  aspect_items error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;
    
aspect_item:
  var_definition
  {
    /* class_var_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.members == NULL ) {
          current_block->is.class.members = $1 ;
        }
        else {
          statement_node *member = current_block->is.class.members ;
          while( member->next ) member = member->next ;
          member->next = $1 ;
        }
      }
      if( $1 )
      {
        named_node *pmem ;
        named_node *variable = $1->is.variable.declare->var_list ;
        while( variable ) {
          if( this_class_name ) {
            pmem = 
              findname_in_classname( 
                this_class_name->info.class_name.parent, 
                variable->name 
              ) ;
            if( pmem && pmem->is_final ) {
              ERROR_LOCATION( variable->filename, variable->linenum ) ;
              ERROR_ILLEGAL_REDEFINITION_OF_FINAL_MEMBER_NAME(variable->name) ;
            }
          }
          variable->is_member = 1 ;
          variable = variable->next ; 
        }
      }
    }
  }
  |
  enum_type_definition 
  {
    /* class_enum_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.edf == NULL ) {
          current_block->is.class.edf = $1 ;
        }
        else {
          statement_node *edf = current_block->is.class.edf ;
          while( edf->next ) edf = edf->next ;
          edf->next = $1 ;
        }
      }
    }
  }
  |
  initial_statement
  {
    /* class_initial_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.initial == NULL ) {
          current_block->is.class.initial = $1 ;
        }
        else {
          statement_node *initial = current_block->is.class.initial ;
          while( initial->next ) initial = initial->next ;
          initial->next = $1 ;
        }
      }
    }
  }
  |
  function_definition
  {
    /* aspect_function_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.member_funcs == NULL ) {
          current_block->is.class.member_funcs = $1 ;
        }
        else {
          block_node *member_funcs = current_block->is.class.member_funcs ;
          while( member_funcs->next ) member_funcs = member_funcs->next ;
          member_funcs->next = $1 ;
        }
      }
      if( $1 && $1->name ) {
        $1->name->is_member = 1 ;
      }
    }
  }
  |
  advice_definition
  {
    /* aspect_advice_definition */
    SKIP_ON_ERROR ;
    PASS2 {
      if( current_block ) {
        if( current_block->is.class.member_funcs == NULL ) {
          current_block->is.class.member_funcs = $1 ;
        }
        else {
          block_node *member_funcs = current_block->is.class.member_funcs ;
          while( member_funcs->next ) member_funcs = member_funcs->next ;
          member_funcs->next = $1 ;
        }
      }
      if( $1 && $1->name ) {
        $1->name->is_member = 1 ;
      }
    }
  }
  ;
  
advice_name:
  Identifier
  {
    /* advice_name */
    named_node *nm ;
    named_node *scope ;
    SKIP_ON_ERROR ;
    PASS1 {
      if( nm = findname_in_scope( current_scope, $1.name ) ) {
        $$ = nm ;
        if( nm->type != JD_advice_name )
        {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_DUPLICATED_NAME($1.name) ;
        }
      }
      else {
        $$ = ALLOC(named_node) ;
        $$->name = $1.name ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        /* type can't be defined yet */
        $$->type = JD_advice_name ;
        addname( $$ ) ;
      }
    }
    PASS2 {
      nm = findname_in_scope( current_scope, $1.name ) ;
      $$ = nm ;
      if (nm == NULL) {
        printname_in_scope( current_scope ) ;
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
      else {
        if( nm->type != JD_advice_name )
        {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
        }
      }
    }
  }
  ;


advice_definition:
  JDT_advice opt_func_type advice_name 
  '(' 
  {
    /* func_name_found */
    SKIP_ON_ERROR ;
    in_advice = 1 ;
    current_func_name = $3 ;
    PASS1 {
      enter_scope( $3 ) ;
      $3->data_type = $2 ;
    }
    PASS2 {
      $3->data_type = $2 ;
      enter_scope( $3 ) ;
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
    opt_formal_args 
  ')' 
  pointcut_specifications
  {
    /* advice_pointcut */
    if( !in_advice ) break ;
    PASS2 {
      $3->info.func.pointcut = $8 ;
    }
  }
  opt_outer_block
  {
    /* advice_define */
    $$ = NULL ;
    if( !in_advice ) break ;
    current_func_name = NULL ;
    in_advice = 0 ;
    PASS1_2 {
      exit_scope() ;
    }
    ERROR_CLR_BLOCK {
      PASS2 {
        if( is_abstract_arg( $6 ) ) {
          ERROR_LOCATION( $7.filename, $7.linenum ) ;
          ERROR_ILLEGAL_USAGE_OF_ABSTRUCT_ARGUMENT ;
        }
        if( $10 == NULL ) { /* $10 = opt_outer_block */
          /* member function declaration */
          if( $3->info.func.class_block == NULL )
          {
            $$ = ALLOC(block_node) ;
            $$->type = JD_advice_definition ;
            $$->name = $3 ;
            $$->name->data_type = $2 ;
            $$->is.func.args = $6 ;
            $$->is.func.pointcut = $8 ;
            $3->info.func.args = $6 ;
            $$->linenum = $3->linenum ;
            $$->filename = $3->filename ;
            $3->info.func.class_block = current_block ;
            $3->info.func.func_block = $$ ;
          }
          else check_advice( $2, $3, $6, $8 ) ;
        }
        else {  /* body block exist */
          /* advice block declaration */
          if( $3->info.func.class_block == NULL )
          {
            $$ = ALLOC(block_node) ;
            $$->type = JD_advice_definition ;
            $$->name = $3 ;
            $$->is.func.block = $10 ;
            $$->is.func.args = $6 ;
            $$->is.func.pointcut = $8 ;
            $3->info.func.args = $6 ;
            $$->linenum = $4.linenum ;
            $$->filename = $4.filename ;
            $3->info.func.class_block = current_block ;
            $3->info.func.func_block = $$ ;
          }
          else if( $3->info.func.func_block->is.func.block ) {
            ERROR_LOCATION( $4.filename, $4.linenum ) ;
            ERROR_MULTIPLE_DECLARATION_OF_ADVICE_FOUND ;
          }
          else {
            check_advice( $2, $3, $6, $8 ) ;
            $$->is.func.block = $10 ;
            $$->linenum = $4.linenum ;
            $$->filename = $4.filename ;
          }
          if( $$->is.func.pointcut == NULL ) $$->is.func.pointcut = $8 ;
        }
      }
    }
    CLEAR_VAR_FUNC_ATTR_FLAGS ;
  }
  ;
      
pointcut_specifications:
  pointcut_spec
  {
    $$ = $1 ; 
  }
  |
  pointcut_specifications pointcut_spec
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;

pointcut_spec:
  JDT_call '(' pointcut_pattern ')'  
  {
    /* call_pointcut */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_call_pointcut_statement ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      $$->prefix = $3 ;
    }
  }
  |
  JDT_return '(' pointcut_pattern ')'
  {
    /* return_pointcut */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_return_pointcut_statement ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      $$->prefix = $3 ;
    }
  }
  |
  JDT_overwrite '(' pointcut_pattern ')'
  {
    /* overwrite_pointcut */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_overwrite_pointcut_statement ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      $$->prefix = $3 ;
    }
  }
  ;

pointcut_pattern:
  pointcut_element
  {
    $$ = $1 ; 
  }
  |
  pointcut_pattern '.' pointcut_element
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
pointcut_element:
  Identifier
  {
    /* pointcut_name */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_pointcut_name ;
      $$->is.string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  String_literal
  {
    /* string_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_string_exp ;
      $$->data_type = JD_exp_string_type ;
      $$->is.string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
  


/***********************************
 *  statement
 ***********************************/


statement:
  null_statement
  {
    $$ = $1 ; 
  }
  |
  if_else_statement 
  {
    $$ = $1 ; 
  }
  |
  while_statement 
  {
    $$ = $1 ; 
  }
  |
  for_statement 
  {
    $$ = $1 ; 
  }
  |
  repeat_statement 
  {
    $$ = $1 ; 
  }
  |
  do_while_statement 
  {
    $$ = $1 ; 
  }
  |
  forever_statement 
  {
    $$ = $1 ; 
  }
  |
  case_statement 
  {
    $$ = $1 ; 
  }
  |
  casex_statement 
  {
    $$ = $1 ; 
  }
  |
  casez_statement 
  {
    $$ = $1 ; 
  }
  |
  randcase_statement 
  {
    $$ = $1 ; 
  }
  |
  break_statement 
  {
    $$ = $1 ; 
  }
  |
  continue_statement
  {
    $$ = $1 ; 
  }
  |
  fork_join_statement
  {
    $$ = $1 ; 
  }
  |
  terminate_statement
  {
    $$ = $1 ; 
  }
  |
  breakpoint_statement
  {
    $$ = $1 ; 
  }
  |
  expression_statement
  {
    $$ = $1 ; 
  }
  |
  assign_statement
  {
    $$ = $1 ; 
  }
  |
  function_call_statement
  {
    $$ = $1 ; 
  }
  |
  return_statement
  {
    $$ = $1 ; 
  }
  |
  verilog_task_call_statement
  {
    $$ = $1 ; 
  }
  |
  block
  {
    $$ = $1 ; 
  }
  ;



/***********************************
 *  block
 ***********************************/


block:
  '{'
  {
    /* block_begin */
    SKIP_ON_ERROR ;
    PASS1_2 {
      enter_scope( current_scope ) ;
      in_block++ ;
      /* repeat_level = 0 ; */
    }
    PASS2 {
      current_block_level++ ;
    }
  }
    opt_var_enum_definitions
    opt_statements
  '}'
  {
    /* block_statement */
    if( !in_block ) break ;
    PASS1_2 {
      exit_scope() ;
      in_block-- ;
    }
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *st ;
      
      current_block_level-- ;
      $$ = ALLOC(statement_node) ;
      $$->type = JD_block ;
      $$->is.block.level = current_block_level ;
      $$->is.block.vars = $3 ;
      $$->is.block.statements = $4 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

opt_statements:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  statements
  {
    $$ = $1 ; 
  }
  ;

statements:
  statement
  {
    $$ = $1 ; 
  }
  |
  statements statement
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
    yyerrok ;
    error_flag = 0 ;
  }
  /* BEGIN Error Recovery section */
  |
  error
  {
    $$ = NULL ;
  }
  |
  statements error  
  {
    $$ = NULL ;
  }
  /* End Error Recovery section */
  ;



/***********************************
 *  statement prefix
 ***********************************/


opt_statement_prefix:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  cycle_element
  {
    $$ = $1 ; 
  }
  |
  label
  {
    /* label */
    SKIP_ON_ERROR ;
    PASS1 {
      if( findname_in_scope( current_scope, $1.name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_DUPLICATED_NAME($1.name) ;
      }
      else {
        named_node *lb ;
        lb = ALLOC(named_node) ;
        lb->type = JD_label_name ;
        lb->name = $1.name ;
        lb->filename = $1.filename ;
        lb->linenum = $1.linenum ;
        addname( lb ) ;
      }
    }
    PASS2 {
      named_node *lb ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_statement_prefix_exp ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      $$->is.smt_prefix.label = $1.name ;
      $$->is.smt_prefix.continue_num = label_num++ ;
      $$->is.smt_prefix.break_num = label_num++ ;
      lb = findname_in_scope( current_scope, $1.name ) ;
      if( !lb || lb->type != JD_label_name ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
      }
      else lb->info.label = $$ ;
    }
  }
  |
  label cycle_element
  {
    /* label_cycle */
    SKIP_ON_ERROR ;
    PASS1 {
      if( findname_in_scope( current_scope, $1.name ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_DUPLICATED_NAME($1.name) ;
      }
      else {
        named_node *lb ;
        lb = ALLOC(named_node) ;
        lb->type = JD_label_name ;
        lb->name = $1.name ;
        lb->filename = $1.filename ;
        lb->linenum = $1.linenum ;
        addname( lb ) ;
      }
    }
    PASS2 {
      named_node *lb ;
      $$ = $2 ;
      if($$) {
        $$->is.smt_prefix.label = $1.name ;
        $$->is.smt_prefix.continue_num = label_num++ ;
        $$->is.smt_prefix.break_num = label_num++ ;
        lb = findname_in_scope( current_scope, $1.name ) ;
        if( !lb || lb->type != JD_label_name ) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2($1.name) ;
        }
        else lb->info.label = $$ ;
      }
    }
  }
  ;

label:
  Identifier ':'
  {
    $$ = $1 ; 
  }
  ;
  
cycle_element:
  '@' opt_expression opt_clock_definition 
  {
    /* cycle_element */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $3 == NULL ) {
        $$ = ALLOC(expression_node) ;
        $$->type = JD_statement_prefix_exp ;
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        if( $2 == NULL ) {
          $$->is.smt_prefix.cycle = ALLOC(expression_node) ;
          $$->is.smt_prefix.cycle->type = JD_int_num ;
          $$->is.smt_prefix.cycle->data_type = JD_exp_num_type ;
          $$->is.smt_prefix.cycle->is.int_value = 1 ;
          $$->is.smt_prefix.cycle->string = (char *)strdup("1") ;
          $$->is.smt_prefix.cycle->filename = $1.filename ;
          $$->is.smt_prefix.cycle->linenum = $1.linenum ;
        }
        else $$->is.smt_prefix.cycle = $2 ;
        $$->is.smt_prefix.clock = NULL ;
        $$->is.smt_prefix.edge = JD_posedge ;  /* default is posedge */
      }
      else {
        $$ = $3 ;
        if($$) {
          $$->filename = $1.filename ;
          $$->linenum = $1.linenum ;
          if( $2 == NULL ) {
            $$->is.smt_prefix.cycle = ALLOC(expression_node) ;
            $$->is.smt_prefix.cycle->type = JD_int_num ;
            $$->is.smt_prefix.cycle->data_type = JD_exp_num_type ;
            $$->is.smt_prefix.cycle->is.int_value = 1 ;
            $$->is.smt_prefix.cycle->string = (char *)strdup("1") ;
            $$->is.smt_prefix.cycle->filename = $1.filename ;
            $$->is.smt_prefix.cycle->linenum = $1.linenum ;
          }
          else $$->is.smt_prefix.cycle = $2 ;
        }
      }
    }
  }
  ;
  
opt_clock_definition:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  '(' edge_specification  port_expression ')'
  {
    /* clock_def */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_statement_prefix_exp ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      /* clock == NULL and edge != noedge means CLOCK is specified */
      $$->is.smt_prefix.clock = $3 ;
      $$->is.smt_prefix.edge = $2 ;
      if( $3 ) {
        if( !is_signal_exp($3) ) {
          ERROR_LOCATION( $4.filename, $4.linenum ) ;
          ERROR_ILLEGAL_EXPRESSION_FOR_CLOCK_DEFINITION ;
        }
      }
    }
  }
  ;
  
edge_specification:
  JDT_posedge 
  {
    /* posedge */
    $$ = JD_posedge ;
  }
  |
  JDT_negedge 
  {
    /* negedge */
    $$ = JD_negedge ;
  }
  |
  JDT_bothedge 
  {
    /* bothedge */
    $$ = JD_bothedge ;
  }
  |
  /* empty */
  {
    /* noedge */
    $$ = JD_noedge ;
  }
  ;

port_expression:
  varport_expression 
  {
    $$ = $1 ; 
  }
  ;




/***********************************
 *  null statement
 ***********************************/


null_statement:
  opt_statement_prefix semicolon
  {
    /* null_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_null_statement ;
      $$->prefix = $1 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;



/***********************************
 *  if statement
 ***********************************/


if_else_statement:
  opt_statement_prefix JDT_if '(' expression ')' statement
  opt_else
  {
    /* if_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_if_statement ;
      $$->prefix = $1 ;
      $$->is.if_.cond = $4 ;
      $$->is.if_.statement = $6 ;
      $$->is.if_.elsif_statements = NULL ; /* elsif is ommitted */
      $$->is.if_.else_statement = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $4->data_type == JD_exp_new_type ||
           $4->data_type == JD_exp_void_type ||
           !check_cond_data_type( $4 )
      ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_IF_CONDITION ;
      }
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;

opt_else:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  JDT_else statement
  {
    /* else */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_else_statement ;
      $$->is.else_.statement = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;


/***********************************
 *  while statement
 ***********************************/


while_statement:
  opt_statement_prefix JDT_while '(' expression ')' 
  {
    /* loop_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 && $1->is.smt_prefix.label ) {
        /* current_block_level is used on break/continue
         * to pull out stack flame to release the local variables
         */
        push_label( 
          $1->is.smt_prefix.label, 
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
      else {
        $1 = ALLOC(expression_node) ;
        $1->type = JD_statement_prefix_exp ;
        $1->filename = "" ;
        $1->linenum = 0 ;
        $1->is.smt_prefix.label = "" ;
        $1->is.smt_prefix.continue_num = label_num++ ;
        $1->is.smt_prefix.break_num = label_num++ ;
  
        push_label( 
          NULL,  
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
    }
  }
  statement 
  {
    /* while_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_while_statement ;
      $$->prefix = $1 ;
      $$->is.while_.cond = $4 ;
      $$->is.while_.statement = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_loop_expression($4) ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_WHILE_CONDITION ;
      }
      pop_label() ;
    }
  }
  ;



/***********************************
 *  for statement
 ***********************************/


for_statement:
  opt_statement_prefix JDT_for '(' 
    opt_multi_expressions ';' opt_expression ';' opt_multi_expressions
  ')' 
  {
    /* loop_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 && $1->is.smt_prefix.label ) {
        /* current_block_level is used on break/continue
         * to pull out stack flame to release the local variables
         */
        push_label( 
          $1->is.smt_prefix.label, 
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
      else {
        $1 = ALLOC(expression_node) ;
        $1->type = JD_statement_prefix_exp ;
        $1->filename = "" ;
        $1->linenum = 0 ;
        $1->is.smt_prefix.label = "" ;
        $1->is.smt_prefix.continue_num = label_num++ ;
        $1->is.smt_prefix.break_num = label_num++ ;
  
        push_label( 
          NULL,  
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
    }
  }
  statement 
  {
    /* for_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_for_statement ;
      $$->prefix = $1 ;
      $$->is.for_.initial = $4 ;
      $$->is.for_.cond = $6 ;
      $$->is.for_.last = $8 ;
      $$->is.for_.statement = $11 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_loop_expression($6) ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_FOR_CONDITION ;
      }
      pop_label() ;
    }
  }
  ;
  
opt_multi_expressions:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  multi_expressions
  {
    $$ = $1 ; 
  }
  ;

multi_expressions:
  for_expression
  {
    $$ = $1 ; 
  }
  |
  multi_expressions ',' for_expression
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
for_expression:
  state_expression 
  {
    $$ = $1 ; 
  }
  |
  assign_expression
  {
    $$ = $1 ; 
  }
  ;
  


/***********************************
 *  repeat statement
 ***********************************/


repeat_statement:
  opt_statement_prefix JDT_repeat '(' expression ')' 
  {
    /* loop_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      repeat_level++ ;
      if( $1 && $1->is.smt_prefix.label ) {
        /* current_block_level is used on break/continue
         * to pull out stack flame to release the local variables
         */
        push_label( 
          $1->is.smt_prefix.label, 
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          repeat_level
        ) ;
      }
      else {
        $1 = ALLOC(expression_node) ;
        $1->type = JD_statement_prefix_exp ;
        $1->filename = "" ;
        $1->linenum = 0 ;
        $1->is.smt_prefix.label = "" ;
        $1->is.smt_prefix.continue_num = label_num++ ;
        $1->is.smt_prefix.break_num = label_num++ ;
  
        push_label( 
          NULL,  
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          repeat_level
        ) ;
      }
    }
  }
  statement 
  {
    /* repeat_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_repeat_statement ;
      $$->prefix = $1 ;
      $$->is.repeat.number = $4 ;
      $$->is.repeat.statement = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $4->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_REPEAT ;
      }
      pop_label() ;
      repeat_level-- ;
    }
  }
  ;



/***********************************
 *  do while statement
 ***********************************/


do_while_statement:
  opt_statement_prefix JDT_do 
  {
    /* loop_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 && $1->is.smt_prefix.label ) {
        /* current_block_level is used on break/continue
         * to pull out stack flame to release the local variables
         */
        push_label( 
          $1->is.smt_prefix.label, 
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
      else {
        $1 = ALLOC(expression_node) ;
        $1->type = JD_statement_prefix_exp ;
        $1->filename = "" ;
        $1->linenum = 0 ;
        $1->is.smt_prefix.label = "" ;
        $1->is.smt_prefix.continue_num = label_num++ ;
        $1->is.smt_prefix.break_num = label_num++ ;
  
        push_label( 
          NULL,  
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
    }
  }
  statement JDT_while '(' expression ')' semicolon 
  {
    /* do_while_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_do_while_statement ;
      $$->prefix = $1 ;
      $$->is.dowhile.statement = $4 ;
      $$->is.dowhile.cond = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_loop_expression($7) ) {
        ERROR_LOCATION( $7->filename, $7->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_WHILE_CONDITION ;
      }
      pop_label() ;
    }
  }
  ;



/***********************************
 *  forever statement
 ***********************************/


forever_statement:
  opt_statement_prefix JDT_forever 
  {
    /* loop_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 && $1->is.smt_prefix.label ) {
        /* current_block_level is used on break/continue
         * to pull out stack flame to release the local variables
         */
        push_label( 
          $1->is.smt_prefix.label, 
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
      else {
        $1 = ALLOC(expression_node) ;
        $1->type = JD_statement_prefix_exp ;
        $1->filename = "" ;
        $1->linenum = 0 ;
        $1->is.smt_prefix.label = "" ;
        $1->is.smt_prefix.continue_num = label_num++ ;
        $1->is.smt_prefix.break_num = label_num++ ;
  
        push_label( 
          NULL,  
          $1->is.smt_prefix.continue_num, 
          $1->is.smt_prefix.break_num, 
          current_block_level,
          0
        ) ;
      }
    }
  }
  statement 
  {
    /* forever_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_forever_statement ;
      $$->prefix = $1 ;
      $$->is.forever.statement = $4 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      pop_label() ;
    }
  }
  ;



/***********************************
 *  case statement
 ***********************************/


case_statement:
  opt_statement_prefix JDT_case '(' expression ')' '{'
    opt_case_items
  '}' 
  {
    /* case_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_case_statement ;
      $$->prefix = $1 ;
      $$->is.case_statement.cond = $4 ;
      $$->is.case_statement.items = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
      case_items_type_check( $4, $7, $2.filename, $2.linenum  ) ;
    }
  }
  ;

opt_case_items:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  case_items
  {
    $$ = $1 ; 
  }
  ;

case_items:
  case_item
  {
    $$ = $1 ; 
  }
  |
  case_items case_item
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
    
case_item:
  expression_list ':' statement
  {
    /* case_item */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *cond ;
      cond = $1 ;
      $$ = ALLOC(statement_node) ;
      $$->type = JD_case_item ;
      $$->is.case_item.cond = $1 ;
      $$->is.case_item.statement = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
    }
  }
  |
  JDT_default ':' statement 
  {
    /* case_default */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_case_default ;
      $$->is.case_item.statement = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
    }
  }
  ;

expression_list:
  expression
  {
    $$ = $1 ; 
  }
  |
  expression_list ',' expression
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;



/***********************************
 *  casex statement
 ***********************************/


casex_statement:
  opt_statement_prefix JDT_casex '(' expression ')' '{'
    opt_case_items
  '}'
  {
    /* casex_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_casex_statement ;
      $$->prefix = $1 ;
      $$->is.case_statement.cond = $4 ;
      $$->is.case_statement.items = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
      case_items_type_check( $4, $7 ) ;
    }
  }
  ;



/***********************************
 *  casez statement
 ***********************************/


casez_statement:
  opt_statement_prefix JDT_casez '(' expression ')' '{'
    opt_case_items
  '}' 
  {
    /* casez_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_casez_statement ;
      $$->prefix = $1 ;
      $$->is.case_statement.cond = $4 ;
      $$->is.case_statement.items = $7 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
      case_items_type_check( $4, $7 ) ;
    }
  }
  ;



/***********************************
 *  randcase statement
 ***********************************/


randcase_statement:
  opt_statement_prefix JDT_randcase '{'
    opt_randcase_items
  '}' 
  {
    expression_node *rnd, *tmp, **pr ;
    statement_node *item ;
    int num_element = 0 ;
    int index = 0 ;
    /* randcase_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_randcase_statement ;
      $$->prefix = $1 ;
      $$->is.case_statement.items = $4 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
      rnd = ALLOC(expression_node) ;
      rnd->type = JD_rnd_exp ;
      rnd->data_type = JD_exp_num_type ;
      rnd->is.rnd.dst_width == -1 ; /* int type */
      $$->is.case_statement.cond = rnd ;
      item = $$->is.case_statement.items ;
      pr = &rnd->is.rnd.range ;
      while( item ) {
        tmp = ALLOC(expression_node) ;
        tmp->type = JD_rand_val_exp ;
        tmp->is.rnd_range.ub = ALLOC(expression_node) ;
        tmp->is.rnd_range.ub->type = JD_int_num ;
        tmp->is.rnd_range.ub->is.int_value = index++ ;
        item->is.case_item.cond = tmp->is.rnd_range.ub ;
        tmp->is.rnd_range.weight = item->is.case_item.weight ;
        tmp->is.rnd_range.weight_flag = 1 ;
        *pr = tmp ;
        pr = &tmp->is.rnd_range.nxt ;
        num_element++ ;
        item = item->next ;
      }
      rnd->is.rnd.num_element = num_element ;
      if( top_static_rnd ) check_static_rnd( top_static_rnd, rnd ) ;
      else top_static_rnd = rnd ;
    }
  }
  ;

opt_randcase_items:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  randcase_items
  {
    $$ = $1 ; 
  }
  ;

randcase_items:
  randcase_item
  {
    $$ = $1 ; 
  }
  |
  randcase_items randcase_item
  {
    SKIP_ON_ERROR ;
    PASS2 {
      statement_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
    
randcase_item:
  Decimal_num ':' statement
  {
    /* randcase_item */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_case_item ;
      $$->is.case_item.weight = atol($1.name) ;
      $$->is.case_item.statement = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
    }
  }
  ;



/***********************************
 *  break statement
 ***********************************/


break_statement:
  opt_statement_prefix JDT_break opt_exit_label semicolon
  {
    /* break_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      label_data *label ;
      $$ = ALLOC(statement_node) ;
      $$->type = JD_break_statement ;
      $$->prefix = $1 ;
      $$->is.break_continue.label = $3.name ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      label = find_label($3.name) ;
      if( label ) {
        $$->is.break_continue.label_num = label->break_num ;
        $$->is.break_continue.level = label->level ;
        $$->is.break_continue.repeat = label->repeat ;
      }
      else {
        if( $3.name ) {
          ERROR_BREAK_LABEL_CANT_BE_FOUND( $3.name ) ;
        }
        else {
          ERROR_BREAK_STATEMENT_ON_OUT_OF_LOOP ;
        }
      }
      if( $1 && $1->is.smt_prefix.label ) {
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;

opt_exit_label:
  /* empty */
  {
    /* NullLabel */
    $$.name = NULL ;
  
  }
  |
  Identifier
  {
    $$ = $1 ; 
  }
  ;
  


/***********************************
 *  continue statement
 ***********************************/


continue_statement:
  opt_statement_prefix JDT_continue opt_exit_label semicolon 
  {
    /* continue_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      label_data *label ;
      $$ = ALLOC(statement_node) ;
      $$->type = JD_continue_statement ;
      $$->prefix = $1 ;
      $$->is.break_continue.label = $3.name ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      label = find_label($3.name) ;
      if( label ) {
        $$->is.break_continue.label_num = label->continue_num ;
        $$->is.break_continue.level = label->level ;
      }
      else {
        if( $3.name ) {
          ERROR_LOCATION( $2.filename, $2.linenum ) ;
          ERROR_CONTINUE_LABEL_CANT_BE_FOUND( $3.name ) ;
        }
        else {
          ERROR_LOCATION( $2.filename, $2.linenum ) ;
          ERROR_CONTINUE_STATEMENT_ON_OUT_OF_LOOP ;
        }
      }
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;



/***********************************
 *  fork join statement
 ***********************************/


fork_join_statement:
  opt_statement_prefix JDT_fork
    statements
  joins  
  {
    /* fork_join_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_fork_join_statement ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      $$->prefix = $1 ;
      $$->is.fork_join.statement_list = $3 ;
      $$->is.fork_join.join_flag = $4 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;

joins:
  /* empty */
  JDT_join
  {
    $$ = JD_join ;
  }
  |
  JDT_spoon 
  {
    $$ = JD_spoon ;
  }
  |
  JDT_knife 
  {
    $$ = JD_knife ;
  }
  ;



/***********************************
 *  terminate statement
 ***********************************/


terminate_statement:
  opt_statement_prefix JDT_terminate semicolon 
  {
    /* teminate_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_terminate_statement ;
      $$->prefix = $1 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;



/***********************************
 *  breakpoint statement
 ***********************************/

/* working here */
breakpoint_statement:
  opt_statement_prefix JDT_breakpoint opt_string semicolon 
  {
    /* breakpoint_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_breakpoint_statement ;
      $$->prefix = $1 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      $$->is.breakpoint.filename = $3.name ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;

opt_string:
  /* nothing */
  {
    /* DDnameeqNULL */
    SKIP_ON_ERROR ;
    PASS2 {
      $$.name = NULL ;
    }
  }
  |
  String_literal
  {
    $$ = $1 ; 
  }
  ;
  


/***********************************
 *  expression statement
 ***********************************/


expression_statement:
  opt_statement_prefix state_expression semicolon 
  {
    /* expression_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_expression_statement ;
      $$->prefix = $1 ;
      $$->is.exp.expression = $2 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
      if( $1 && $1->is.smt_prefix.label ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED ;
      }
    }
  }
  ;



/***********************************
 *  assign statement
 ***********************************/


assign_statement:
  opt_statement_prefix assign_expression opt_strength semicolon
  {
    /* assign_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_assign_statement ;
      $$->prefix = $1 ;
      $$->is.assign.expression = $2 ;
      $2->is.assign.strength = $3 ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      if( $3 != JD_normal ) {
        expression_node *lvalue = $2->is.assign.lvalue ;
        if( !is_port_expression( lvalue ) ) {
          ERROR_LOCATION( $4.filename, $4.linenum ) ;
          ERROR_ILLEGAL_STRENGTH_USAGE_ON_NON_PORT_ASSIGNMENT ;
        }
      }
    }
  }
  ;
  
opt_strength:
  /* nothing */
  {
    $$ = JD_normal ;
  }
  |
  JDT_weak
  {
    $$ = JD_weak ;
  }
  |
  JDT_strong
  {
    $$ = JD_strong ;
  }
  ;



/***********************************
 *  function call statement
 ***********************************/


function_call_statement:
  opt_statement_prefix function_call_expression semicolon 
  {
    /* func_call_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_function_call_statement ;
      $$->prefix = $1 ;
      $$->is.funccall.expression = $2 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
    }
  }
  ;

return_statement:
  JDT_return opt_expression semicolon
  {
    /* return_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(statement_node) ;
      $$->type = JD_return_statement ;
      $$->is.ret.expression = $2 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
      $$->is.ret.dtype = current_func_name->data_type ;
      $$->is.ret.ptype = this_class_name ;
      if( $2 ) {
        if( !check_name_data_type( 
               current_func_name->data_type, this_class_name, $2, 1 ) 
          ) 
        {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_RETURN_DATA_TYPE_MISMATCH ;
        }
      }
    }
  }
  ;



/***********************************
 *  verilog task call statement
 ***********************************/


verilog_task_call_statement:
  JDT_verilog '.' Identifier 
  '(' 
    opt_actual_arguments 
  ')' semicolon
  {
    /* verilog_task_call_statement */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *verilog = findname( "verilog" ) ;
      if( verilog == NULL ) {
        ERROR_VERILOG_CLASS_NOT_DEFINED ;
      }
      else {
        named_node *mfunc = findname_in_scope( verilog->child_scope, $3.name ) ;
        if( mfunc == NULL ) {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_MEMBER_FUNCTION_NOT_DEFINED( $3.name ) ;
        }
        else {
          expression_node *vcall ;
          vcall = ALLOC(expression_node) ;
          vcall->type = JD_verilog_task_call_exp ;
          vcall->data_type == JD_void_type ;
          vcall->is.var.name = mfunc ;
          vcall->is.var.args = $5 ;
          vcall->filename = $7.filename ;
          vcall->linenum = $7.linenum ;
          /* TBI
          arg_check( mfunc, $5 ) ;
          */
          $$ = ALLOC(statement_node) ;
          $$->type = JD_verilog_task_call_statement ;
          $$->is.funccall.expression = vcall ;
          $$->filename = $7.filename ;
          $$->linenum = $7.linenum ;
        }
      }
    }
  }
  ;




/***********************************
 *  numbers
 ***********************************/


number:
  Real_num
  {
    /* real_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_real_num ;
      $$->data_type = JD_exp_num_type ;
      $$->is.real_value = atof($1.name) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  Real_or_Depth
  {
    /* real_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_real_num ;
      $$->data_type = JD_exp_num_type ;
      $$->is.real_value = atof($1.name) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  Decimal_num
  {
    /* decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_int_num ;
      $$->data_type = JD_exp_num_type ;
      $$->is.int_value = atol($1.name) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  V_decimal_num
  {
    /* v_decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_decimal( $1.name, $$ ) ;
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_decimal_num
  {
    /* c_decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_decimal( $1.name, $$ ) ;
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_hex_num
  {
    /* v_hex_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_hex( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_hex_num
  {
    /* c_hex_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_hex( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_octal_num
  {
    /* v_octal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_octal( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_octal_num
  {
    /* c_octal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_octal( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_bin_num
  {
    /* v_bin_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_binary( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_bin_num
  {
    /* c_bin_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_binary( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  ;




/***********************************
 *  constant
 ***********************************/


constant:
  number
  {
    $$ = $1 ; 
  }
  |
  constant '-' constant
  {
    /* const_minus_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_minus_exp, $1, $3 ) ;
    }
  }
  |
  constant '+' constant
  {
    /* const_plus_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_plus_exp, $1, $3 ) ;
    }
  }
  |
  constant '*' constant   
  {
    /* const_times_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_times_exp, $1, $3 ) ;
    }
  }
  |
  constant '/' constant   
  {
    /* const_div_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_div_exp, $1, $3 ) ;
    }
  }
  |
  constant '%' constant   
  {
    /* const_mod_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_mod_exp, $1, $3 ) ;
    }
  }
  |
  constant '&' constant   
  {
    /* const_and_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_and_exp, $1, $3 ) ;
    }
  }
  |
  constant '|' constant   
  {
    /* const_or_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_or_exp, $1, $3 ) ;
    }
  }
  |
  constant '^' constant   
  {
    /* const_eor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_eor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_ANDTILDA constant  
  {
    /* const_nand_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_nand_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_TILDAAND constant  
  {
    /* const_nand_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_nand_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_ORTILDA constant  
  {
    /* const_nor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_nor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_TILDAOR constant  
  {
    /* const_nor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_nor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_EORTILDA constant  
  {
    /* const_neor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_neor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_TILDAEOR constant  
  {
    /* const_neor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_aop_const( JD_exp_neor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_LAND constant  
  {
    /* const_land_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_lop_const( JD_exp_land_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_LOR constant  
  {
    /* const_lor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_lop_const( JD_exp_lor_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_RSHIFT constant  
  {
    /* const_rshift_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_sop_const( JD_exp_rshift_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_URSHIFT constant 
  {
    /* const_urshift_const */
    SKIP_ON_ERROR ;
    PASS1 {
      $$ = const_sop_const( JD_exp_urshift_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_LSHIFT constant  
  {
    /* const_lshift_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_sop_const( JD_exp_lshift_exp, $1, $3 ) ;
    }
  }
  |
  constant '<' constant   
  {
    /* const_lt_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_lt_exp, $1, $3 ) ;
    }
  }
  |
  constant '>' constant   
  {
    /* const_gt_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_gt_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_EQEQ constant  
  {
    /* const_eqeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_eqeq_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_LE constant  
  {
    /* const_le_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_le_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_GE constant  
  {
    /* const_ge_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_ge_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_NE constant  
  {
    /* const_ne_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_ne_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_EQEQEQ constant 
  {
    /* const_eqeqeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_eqeqeq_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_NEQEQ constant 
  {
    /* const_neqeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_neqeq_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_Q_EQ constant 
  {
    /* const_eqQeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_eqQeq_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_Q_NEQ constant 
  {
    /* const_eqQeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_Qne_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_EQEQQ constant 
  {
    /* const_eqeqQ_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_eqeqQ_exp, $1, $3 ) ;
    }
  }
  |
  constant JD_QEQEQ constant 
  {
    /* const_Qeqeq_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = const_cop_const( JD_exp_Qeqeq_exp, $1, $3 ) ;
    }
  }
  |
  '-' constant
  {
    /* u_minus_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_minus_exp, $2 ) ;
    }
  }
  |
  '~' constant
  {
    /* u_tilde_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_tilde_exp, $2 ) ;
    }
  }
  |
  '!' constant
  {
    /* u_not_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_not_exp, $2 ) ;
    }
  }
  |
  '&' constant
  {
    /* u_and_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_and_exp, $2 ) ;
    }
  }
  |
  '|' constant
  {
    /* u_or_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_or_exp, $2 ) ;
    }
  }
  |
  '^' constant
  {
    /* u_eor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_eor_exp, $2 ) ;
    }
  }
  |
  JD_TILDAAND constant
  {
    /* u_nand_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_nand_exp, $2 ) ;
    }
  }
  |
  JD_ANDTILDA constant
  {
    /* u_nand_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_nand_exp, $2 ) ;
    }
  }
  |
  JD_TILDAOR constant
  {
    /* u_nor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_nor_exp, $2 ) ;
    }
  }
  |
  JD_ORTILDA constant
  {
    /* u_nor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_nor_exp, $2 ) ;
    }
  }
  |
  JD_EORTILDA constant 
  {
    /* u_neor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_neor_exp, $2 ) ;
    }
  }
  |
  JD_TILDAEOR constant 
  {
    /* u_neor_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = aop_const( JD_u_neor_exp, $2 ) ;
    }
  }
  |
  constant JD_C_EQEQ constant ':' constant 
  {
    /* mask_comp_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = mask_comp_const( $1, $3, $5 ) ;
    }
  }
  |
  constant '?' constant ':' constant 
  {
    /* conditional_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = conditional_const( $1, $3, $5 ) ;
    }
  }
  |
  '(' constant ')' 
  {
    $$ = $2 ; 
  }
  |
  constant_concatenation
  {
    $$ = $1 ; 
  }
  ;

constant_concatenation:
  '{' constant_list '}' 
  {
    /* concatenate_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = concatenate_const( $2 ) ;
    }
  }
  |
  '{' constant '{' constant '}' '}'
  {
    /* duplicate_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = duplicate_const( $2, $4 ) ;
    }
  }
  ;
  
constant_list:
  constant
  {
    $$ = $1 ; 
  }
  |
  constant_list ',' constant
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;

/***********************************
 *  string constant
 ***********************************/
  
string_constant:
  String_literal
  {
    /* string_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_string_exp ;
      $$->data_type = JD_exp_string_type ;
      $$->is.string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  '{' string_list '}' 
  {
    /* concatenate_string */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = concatenate_string( $2 ) ;
    }
  }
  |
  '{' constant '{' string_constant '}' '}'
  {
    /* duplicate_string */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = duplicate_string( $2, $4 ) ;
    }
  }
  |
  constant '?' string_constant ':' string_constant 
  {
    /* conditional_const */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = conditional_const( $1, $3, $5 ) ;
    }
  }
  |
  '(' string_constant ')' 
  {
    $$ = $2 ; 
  }
  ;

string_list:
  string_constant
  {
    $$ = $1 ; 
  }
  |
  string_list ',' string_constant
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
    


/***********************************
 *  expression
 ***********************************/


expression:
  number
  {
    $$ = $1 ; 
  }
  |
  JDT_return
  {
    /* return_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *name ;
      /* check if this is variable, as it could be function name */
      if(!in_advice) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_RETURN_EXPRESSION_ON_OUT_OF_ADVICE ;
      }
      $$ = ALLOC(expression_node) ;
      $$->type = JD_return_exp ;
      set_func_type_of_func($$, current_func_name) ;
      $$->user_data_type = current_func_name ;
    }
  }
  |
  string_constant
  {
    $$ = $1 ; 
  }
  |
  timed_expression 
  {
    $$ = $1 ; 
  }
  |
  parallel_eval_expression
  {
    $$ = $1 ; 
  }
  |
  varport_expression
  {
    /* varport_expression */
    $$ = $1 ;
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *name ;
      /* check if this is variable, as it could be function name */
      if( $1->type == JD_this_exp ) {
        if( aspect_parsing && !in_advice ) {
          ERROR_LOCATION( $1->filename, $1->linenum ) ;
          ERROR_ILLEGAL_USAGE_OF_THIS_IN_ASPECT_FUNCTION ;
        }
      }
      else if(  $1->type == JD_super_exp ) {
        /* This is OK */
      }
      else if( !is_var_name( $1->is.var.name ) ) {
        ERROR_LOCATION( $1->filename, $1->linenum ) ;
        ERROR_ILLEGAL_NAME_TYPE_FOR_VARIABLE($1->is.var.name->name) ;
      }
    }
  }
  |
  bool_null_expression
  {
    $$ = $1 ; 
  }
  |
  function_call_expression
  {
    $$ = $1 ; 
  }
  |
  state_expression
  {
    $$ = $1 ; 
  }
  |
  concatenation_expression
  {
    $$ = $1 ; 
  }
  |
  list_expression
  {
    $$ = $1 ; 
  }
  |
  expression '-' expression   
  {
    /* exp_minus_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_minus_exp ;
      $$->is.binary.left = $1 ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "-" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "-" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '+' expression   
  {
    /* exp_plus_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_plus_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "+" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "+" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '*' expression   
  {
    /* exp_times_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_times_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "*" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "*" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '/' expression   
  {
    /* exp_div_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_div_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "/" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "/" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '%' expression   
  {
    /* exp_mod_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_mod_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "%" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "%" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '&' expression   
  {
    /* exp_and_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_and_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "&" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "&" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '|' expression   
  {
    /* exp_or_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_or_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "|" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "|" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression '^' expression   
  {
    /* exp_eor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_eor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "^" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "^" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_ANDTILDA expression  
  {
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_nand_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~&" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~&" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_TILDAAND expression  
  {
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_nand_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~&" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~&" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_ORTILDA expression  
  {
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_nor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~|" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~|" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_TILDAOR expression  
  {
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_nor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~|" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~|" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_EORTILDA expression  
  {
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_neor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~^" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~^" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_TILDAEOR expression  
  {
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_neor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "~^" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "~^" ) ;
      }
      $$ = binary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_LAND expression  
  {
    /* exp_land_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_land_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type == JD_exp_new_type || !check_cond_data_type( $1 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_ON_1ST_ELEMENT_OF_( "&&" ) ;
      }
      if( $3->data_type == JD_exp_new_type || !check_cond_data_type( $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_ON_2ND_ELEMENT_OF_( "&&" ) ;
      }
    }
  }
  |
  expression JD_LOR expression  
  {
    /* exp_lor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_lor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type == JD_exp_new_type || !check_cond_data_type( $1 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_ON_1ST_ELEMENT_OF_( "||" ) ;
      }
      if( $3->data_type == JD_exp_new_type || !check_cond_data_type( $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_ON_2ND_ELEMENT_OF_( "||" ) ;
      }
    }
  }
  |
  expression JD_RSHIFT expression  
  {
    /* exp_rshift_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_rshift_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( ">>" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( ">>" ) ;
      }
    }
  }
  |
  expression JD_URSHIFT expression 
  {
    /* exp_urshift_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_urshift_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( ">>>" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( ">>>" ) ;
      }
    }
  }
  |
  expression JD_LSHIFT expression  
  {
    /* exp_lshift_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_lshift_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "<<" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "<<" ) ;
      }
    }
  }
  |
  expression '<' expression   
  {
    /* exp_lt_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_lt_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "<" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "<" ) ;
      }
    }
  }
  |
  expression '>' expression   
  {
    /* exp_gt_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_gt_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( ">" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( ">" ) ;
      }
    }
  }
  |
  expression JD_EQEQ expression  
  {
    /* exp_eqeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_eqeq_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_compare_ok( $1, $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_COMBINATION_FOR_( "==" ) ;
      }
    }
  }
  |
  expression JD_LE expression  
  {
    /* exp_le_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_le_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "<=" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "<=" ) ;
      }
    }
  }
  |
  expression JD_GE expression  
  {
    /* exp_ge_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_ge_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( ">=" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( ">=" ) ;
      }
    }
  }
  |
  expression JD_NE expression  
  {
    /* exp_ne_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_ne_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_compare_ok( $1, $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_COMBINATION_FOR_( "!=" ) ;
      }
    }
  }
  |
  expression JD_EQEQEQ expression 
  {
    /* exp_eqeqeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_eqeqeq_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_compare_ok( $1, $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_COMBINATION_FOR_( "===" ) ;
      }
    }
  }
  |
  expression JD_NEQEQ expression 
  {
    /* exp_neqeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_neqeq_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( !is_compare_ok( $1, $3 ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_COMBINATION_FOR_( "!==" ) ;
      }
    }
  }
  |
  expression JD_Q_EQ expression 
  {
    /* exp_eqQeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_eqQeq_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "=?=" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "=?=" ) ;
      }
    }
  }
  |
  expression JD_Q_NEQ expression 
  {
    /* exp_Qne_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_Qne_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "!?=" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "!?=" ) ;
      }
    }
  }
  |
  expression JD_EQEQQ expression 
  {
    /* exp_eqeqQ_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_eqeqQ_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "==?" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "==?" ) ;
      }
    }
  }
  |
  expression JD_QEQEQ expression 
  {
    /* exp_Qeqeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_exp_Qeqeq_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.binary.left = $1 ;
      $$->is.binary.right = $3 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "?==" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "?==" ) ;
      }
    }
  }
  |
  '-' expression
  {
    /* u_minus_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_minus_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "-" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  '~' expression
  {
    /* u_tilde_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_tilde_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  '!' expression
  {
    /* u_not_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_not_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type == JD_exp_new_type ||
           $2->data_type == JD_exp_void_type
      ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "!" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  '&' expression
  {
    /* u_and_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_and_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "&" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  '|' expression
  {
    /* u_or_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_or_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "|" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  '^' expression
  {
    /* u_eor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_eor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "^" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_TILDAAND expression
  {
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_nand_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~&" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_ANDTILDA expression
  {
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_nand_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~&" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_TILDAOR expression
  {
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_nor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~|" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_ORTILDA expression
  {
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_nor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~|" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_EORTILDA expression 
  {
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_neor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~^" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  JD_TILDAEOR expression 
  {
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_u_neor_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.unary.this = $2 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY( "~^" ) ;
      }
      $$ = unary_constant_folding( $$ ) ;
    }
  }
  |
  expression JD_C_EQEQ expression ':' expression 
  {
    /* eqCeq_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_eqCeq_exp ;
      $$->data_type = JD_exp_num_type   ;
      $$->is.trinary.first = $1 ;
      $$->is.trinary.second = $3 ;
      $$->is.trinary.third = $5 ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      if( $1->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $1->filename, $1->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_( "= :" ) ;
      }
      if( $3->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "= :" ) ;
      }
      if( $5->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $5->filename, $5->linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_( "= :" ) ;
      }
    }
  }
  |
  expression '?' expression ':' expression 
  {
    /* conditional_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_conditional_exp ;
      $$->data_type = $3->data_type ;
      $$->is.trinary.first = $1 ;
      $$->is.trinary.second = $3 ;
      $$->is.trinary.third = $5 ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      if( $1->data_type == JD_exp_new_type ) {
        ERROR_LOCATION( $1->filename, $1->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_FOR_1ST_ELEMENT_OF_( "? : " ) ;
      }
      if( $3->data_type != $5->data_type ) {
        ERROR_LOCATION( $5->filename, $5->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_COMBINATION_FOR_( "? : " ) ;
      }
    }
  }
  |
  '(' expression ')'
  {
    $$ = $2 ; 
  }
  |
  static_random
  {
    $$ = $1 ; 
  }
  ;


opt_expression:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  expression
  {
    $$ = $1 ; 
  }
  ;



/***********************************
 *  static random
 ***********************************/
  
static_random:
  JDT_RND '('  opt_rand_args ')'
  {
    /* static_rnd */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *arg, *tmp ;
      int width = 0 ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rnd_exp ;
      $$->data_type = JD_exp_num_type ;
      arg = $3 ;
      while( arg ) {
        switch( arg->type ) {
          case JD_rand_val_exp:
          case JD_rand_range_exp:
            if( $$->is.rnd.range ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_RANGE_DECLARED ;
            }
            $$->is.rnd.range = arg ;
            tmp = arg ;
            while( tmp ) {
              $$->is.rnd.num_element += tmp->is.rnd_range.num_element ;
              if( tmp->is.rnd_range.width > width ) 
                width = tmp->is.rnd_range.width ;
              tmp = tmp->is.rnd_range.nxt ;
            }
            $$->is.rnd.width = width ;
            break ;
          case JD_rand_size_exp:
            if( $$->is.rnd.size ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_SIZE_DECLARED ;
            }
            $$->is.rnd.size = arg->is.int_value ;
            break ;
          case JD_rand_limit_exp:
            if( $$->is.rnd.limit ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_LIMIT_DECLARED ;
            }
            $$->is.rnd.limit = arg->is.int_value ;
            break ;
          case JD_rand_name_exp:
            if( $$->is.rnd.name ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_NAME_DECLARED ;
            }
            $$->is.rnd.name = arg->is.string ;
            break ;
          case JD_rand_cond_lt:
          case JD_rand_cond_gt:
          case JD_rand_cond_le:
          case JD_rand_cond_ge:
          case JD_rand_cond_ne:
          case JD_rand_cond_Qeq:
          case JD_rand_cond_Qne:
            if( !$$->is.rnd.cond ) $$->is.rnd.cond = arg ;
            else {
              tmp = $$->is.rnd.cond ;
              while( tmp->is.rnd_cond.nxt ) tmp = tmp->is.rnd_cond.nxt ;
              tmp->is.rnd_cond.nxt = arg ;
            }
            break ;
        }
        arg = arg->next ;
      }
      if( top_static_rnd ) {
        if( !check_static_rnd( top_static_rnd, $$ ) ) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_NAMED_RND_WITH_DIFFERENT_RANGE_DETECTED( $$->is.rnd.name ) ;
        }
      }
      else top_static_rnd = $$ ;
    }  
  }
  |
  JDT_CYC '('  opt_rand_args ')'
  {
    /* static_cyc */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *arg, *tmp ;
      int width = 0 ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_cyc_exp ;
      $$->data_type = JD_exp_num_type ;
      arg = $3 ;
      while( arg ) {
        switch( arg->type ) {
          case JD_rand_val_exp:
          case JD_rand_range_exp:
            if( $$->is.rnd.range ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_RANGE_DECLARED ;
            }
            $$->is.rnd.range = arg ;
            tmp = arg ;
            while( tmp ) {
              $$->is.rnd.num_element += tmp->is.rnd_range.num_element ;
              if( tmp->is.rnd_range.width > width ) 
                width = tmp->is.rnd_range.width ;
              if( tmp->is.rnd_range.weight || tmp->is.rnd_range.weight_flag )
              {
                ERROR_LOCATION( tmp->filename, tmp->linenum ) ;
                ERROR_ILLEGAL_USAGE_OF_WEIGHT_IN_CYC ;
              }
              tmp = tmp->is.rnd_range.nxt ;
            }
            $$->is.rnd.width = width ;
            break ;
          case JD_rand_size_exp:
            if( $$->is.rnd.size ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_SIZE_DECLARED ;
            }
            $$->is.rnd.size = arg->is.int_value ;
            if( arg->is.int_value > 96 ) {
              ERROR_LOCATION( $3->filename, $3->linenum ) ;
              ERROR_CYC_SIZE_TOO_LARGE_MAX_96 ;
            }
            break ;
          case JD_rand_limit_exp:
            if( $$->is.rnd.limit ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_RAND_LIMIT_DECLARED ;
            }
            $$->is.rnd.limit = arg->is.int_value ;
            break ;
          case JD_rand_name_exp:
            if( $$->is.rnd.name ) {
              ERROR_LOCATION( arg->filename, arg->linenum ) ;
              ERROR_MORE_THAN_ONE_NAME_DECLARED ;
            }
            $$->is.rnd.name = arg->is.string ;
            break ;
          case JD_rand_cond_lt:
          case JD_rand_cond_gt:
          case JD_rand_cond_le:
          case JD_rand_cond_ge:
          case JD_rand_cond_ne:
          case JD_rand_cond_Qeq:
          case JD_rand_cond_Qne:
            if( !$$->is.rnd.cond ) $$->is.rnd.cond = arg ;
            else {
              tmp = $$->is.rnd.cond ;
              while( tmp->is.rnd_cond.nxt ) tmp = tmp->is.rnd_cond.nxt ;
              tmp->is.rnd_cond.nxt = arg ;
            }
            break ;
        }
        arg = arg->next ;
      }
      if( top_static_rnd ) {
        if( !check_static_rnd( top_static_rnd, $$ ) ) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_NAMED_RND_WITH_DIFFERENT_RANGE_DETECTED( $$->is.rnd.name ) ;
        }
      }
      else top_static_rnd = $$ ;
    }  
  }
  ;

opt_rand_args:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  rand_args
  {
    $$ = $1 ; 
  }
  ;
  
rand_args:
  rand_arg
  {
    $$ = $1 ; 
  }
  |
  rand_args ',' rand_arg
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
rand_arg:
  rand_range
  {
    $$ = $1 ; 
  }
  |
  rand_parm
  {
    $$ = $1 ; 
  }
  |
  rand_cond 
  {
    $$ = $1 ; 
  }
  ;
  
rand_range:
  '{' 
    rand_values
  '}'
  {
    $$ = $2 ; 
  }
  ;
  
rand_values:
  rand_value
  {
    $$ = $1 ; 
  }
  |
  rand_values ',' rand_value
  {
    /* rand_values */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        if( (d1->is.rnd_range.weight && !d3->is.rnd_range.weight) ||
          (!d1->is.rnd_range.weight && d3->is.rnd_range.weight)
        ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_INCONSISTENT_WEIGHT_DECLARATION ;
        }
        while(d1->is.rnd_range.nxt) d1 = d1->is.rnd_range.nxt ;
        d1->is.rnd_range.nxt = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
rand_value:
  rand_val
  {
    $$ = $1 ; 
  }
  |
  rand_val '@' constant
  {
    /* rand_value_single */
    SKIP_ON_ERROR ;
    PASS2 {
      int weight  ;
      $$ = $1 ;
      if( !eval_to_int( $3, &weight ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_WEIGHT ;
      }
      else {
        $$->is.rnd_range.weight = weight ;
        $$->is.rnd_range.weight_flag = 1 ;
      }
    }
  }
  |
  rand_val JD_D_AT constant
  {
    /* rand_value_double */
    SKIP_ON_ERROR ;
    PASS2 {
      int weight  ;
      $$ = $1 ;
      if( !eval_to_int( $3, &weight ) ) {
        ERROR_LOCATION( $3->filename, $3->linenum ) ;
        ERROR_ILLEGAL_CONSTANT_TYPE ;
      }
      else {
        $$->is.rnd_range.weight = weight ;
        $$->is.rnd_range.weight_flag = 2 ;
      }
      $$->filename = $1->filename ;
      $$->linenum = $1->linenum ;
    }
  }
  ;

rand_val:
  rnd_constant
  {
    /* rand_range */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_val_exp ;
      $$->is.rnd_range.ub = $1 ;
      $$->is.rnd_range.num_element = 1 ;
      $$->filename = $1->filename ;
      $$->linenum = $1->linenum ;
    }
  }
  |
  rnd_constant ':' rnd_constant
  {
    /* rand_range */
    SKIP_ON_ERROR ;
    PASS2 {
      int size, flag, width  ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_range_exp ;
      
      if( const_range_const( $1, $3, &size, &flag, &width) ) {
        $$->is.rnd_range.num_element = size ;
        $$->is.rnd_range.width = width ;
        if( flag ) {
          $$->is.rnd_range.ub = $3 ;
          $$->is.rnd_range.lb = $1 ;
        }
        else {
          $$->is.rnd_range.ub = $1 ;
          $$->is.rnd_range.lb = $3 ;
        }
      }
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
    }
  }
  ;

rnd_constant:
  Decimal_num
  {
    /* decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_int_num ;
      $$->data_type = JD_exp_num_type ;
      $$->is.int_value = atol($1.name) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  V_decimal_num
  {
    /* v_decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_decimal( $1.name, $$ ) ;
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_decimal_num
  {
    /* c_decimal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_decimal( $1.name, $$ ) ;
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_hex_num
  {
    /* v_hex_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_hex( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_hex_num
  {
    /* c_hex_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_hex( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_octal_num
  {
    /* v_octal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_octal( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_octal_num
  {
    /* c_octal_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_octal( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  V_bin_num
  {
    /* v_bin_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_v_binary( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  |
  C_bin_num
  {
    /* c_bin_num */
    SKIP_ON_ERROR ;
    PASS1_2 {
      $$ = ALLOC(expression_node) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
      scan_c_binary( $1.name, $$ ) ;   
      $$->data_type = JD_exp_num_type ;
    }
  }
  ;

rnd_string:
  String_literal
  {
    /* string_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_string_exp ;
      $$->data_type = JD_exp_string_type ;
      $$->is.string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
    
rand_parm:
  Identifier '=' rnd_constant
  {
    /* rand_parm */
    SKIP_ON_ERROR ;
    PASS2 {
      int tmp ;
      $$ = ALLOC(expression_node) ;
      if( !strcmp( $1.name, "size" ) ) {
        $$->type = JD_rand_size_exp ;
        if( !eval_to_int( $3, &tmp ) ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        else if( tmp == 0 ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_RND_SIZE ;
        }
        $$->is.int_value = tmp ;
      }
      else if( !strcmp( $1.name, "limit" ) ) {
        int tmp ;
        $$->type = JD_rand_limit_exp ;
        if( !eval_to_int( $3, &tmp ) ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        $$->is.int_value = tmp ;
      }
      else if( !strcmp( $1.name, "name" ) ) {
        $$->type = JD_rand_name_exp ;
        if( $3->type != JD_string_exp ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        $$->is.string = $3->is.string ;
      }
      else {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_RND_PARAMETER( $1.name ) ;
      }
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
    }
  }
  |
  Identifier '=' rnd_string
  {
    /* rand_parm */
    SKIP_ON_ERROR ;
    PASS2 {
      int tmp ;
      $$ = ALLOC(expression_node) ;
      if( !strcmp( $1.name, "size" ) ) {
        $$->type = JD_rand_size_exp ;
        if( !eval_to_int( $3, &tmp ) ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        else if( tmp == 0 ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_RND_SIZE ;
        }
        $$->is.int_value = tmp ;
      }
      else if( !strcmp( $1.name, "limit" ) ) {
        int tmp ;
        $$->type = JD_rand_limit_exp ;
        if( !eval_to_int( $3, &tmp ) ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        $$->is.int_value = tmp ;
      }
      else if( !strcmp( $1.name, "name" ) ) {
        $$->type = JD_rand_name_exp ;
        if( $3->type != JD_string_exp ) {
          ERROR_LOCATION( $3->filename, $3->linenum ) ;
          ERROR_ILLEGAL_CONSTANT_TYPE ;
        }
        $$->is.string = $3->is.string ;
      }
      else {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_RND_PARAMETER( $1.name ) ;
      }
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
    }
  }
  ;

rand_cond:
  '<' expression 
  {
    /* rand_cond_lt */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_lt ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  '>' expression 
  {
    /* rand_cond_gt */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_gt ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  JD_LE expression 
  {
    /* rand_cond_le */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_le ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  JD_GE expression 
  {
    /* rand_cond_ge */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_ge ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  JD_NE expression 
  {
    /* rand_cond_ne */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_ne ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  JD_Q_EQ expression 
  {
    /* rand_cond_Qeq */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_Qeq ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  |
  JD_Q_NEQ expression 
  {
    /* rand_cond_Qne */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_rand_cond_Qne ;
      $$->is.rnd_cond.exp = $2 ;
      if( $2->data_type != JD_exp_num_type ) {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND ;
      }
    }
  }
  ;

/***********************************
 *  timed expression
 ***********************************/

 
timed_expression:
  time_range '(' expression_list ')'
  {
    /* timed_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 ) {
        $1->is.time.exp_list = $3 ;
        $1->data_type = JD_exp_num_type ;
      }
      $$ = $1 ;
    } 
  }
  ;

time_range:
  '@' expression opt_clock_definition 
  {
    /* time_range_a */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_timed_till_exp ;
      $$->data_type = JD_exp_unknown_type ;
      $$->is.time.delay = $2 ;
      $$->is.time.window = NULL ;
      $$->is.time.clock = $3 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  '@' opt_expression ',' expression opt_clock_definition
  {
    /* time_range_b */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_timed_till_exp ;
      $$->data_type = JD_exp_unknown_type ;
      $$->is.time.delay = $2 ;
      $$->is.time.window = $4 ;
      $$->is.time.clock = $5 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JD_D_AT expression opt_clock_definition 
  {
    /* time_range_c */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_timed_while_exp ;
      $$->data_type = JD_exp_unknown_type ;
      $$->is.time.delay = $2 ;
      $$->is.time.window = NULL ;
      $$->is.time.clock = $3 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JD_D_AT opt_expression ',' expression opt_clock_definition 
  {
    /* time_range_d */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_timed_while_exp ;
      $$->data_type = JD_exp_unknown_type ;
      $$->is.time.delay = $2 ;
      $$->is.time.window = $4 ;
      $$->is.time.clock = $5 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;


    


parallel_eval_expression:
  parallel_primitive '(' timed_expression_list ')' 
  {
    /* parallel_eval_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *exp ;
      int n ;
      $$ = ALLOC(expression_node) ;
      $$->type = $1 ;
      $$->is.time.exp_list = $3 ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      $$->data_type = JD_exp_num_type ;
      exp = $3 ;
      n = 0 ;
      while( exp ) {
        n++ ;
        if( exp->data_type == JD_exp_new_type ) {
          ERROR_LOCATION( exp->filename, exp->linenum ) ;
          ERROR_ILLEGAL_DATA_TYPE_ON_NTH_ARG_FOR_PARALLEL_COND(n) ;
        }
        exp = exp->next ;
      }
    }
  }
  ;
  
parallel_primitive:
  JDT_p_and 
  {
    /* p_and */
    $$ = JD_parallel_and_exp ;
  }
  |
  JDT_p_or 
  {
    /* p_or */
    $$ = JD_parallel_or_exp ;
  }
  ;

timed_expression_list:
  timed_expression
  {
    $$ = $1 ; 
  }
  |
  timed_expression_list ',' timed_expression
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;



/***********************************
 *  boolean, null
 ***********************************/


bool_null_expression:
  JDT_null 
  {
    /* null_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_null_exp ;
      $$->data_type = JD_exp_void_type ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_true
  {
    /* true_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_true_exp ;
      $$->data_type = JD_exp_bool_type ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_false
  {
    /* false_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_false_exp ;
      $$->data_type = JD_exp_bool_type ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;



/***********************************
 *  state expression ++ --
 ***********************************/

  
state_expression:
  varport_expression JD_D_PLUS 
  {
    /* post_increment */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_postincrement_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.expression = $1 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type || 
          $1->type != JD_varport_exp || 
          !var_port_type( $1->is.var.name->type )   
        ) 
      {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_FOR_NUMERICAL_OPERATION ;
      }
    }
  }
  |
  varport_expression JD_D_MINUS 
  {
    /* post_decrement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_postdecrement_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.expression = $1 ;
      $$->filename = $2.filename ;
      $$->linenum = $2.linenum ;
      if( $1->data_type != JD_exp_num_type || 
          $1->type != JD_varport_exp || 
          !var_port_type( $1->is.var.name->type )   
        ) 
      {
        ERROR_LOCATION( $2.filename, $2.linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_FOR_NUMERICAL_OPERATION ;
      }
    }
  }
  |
  JD_D_PLUS varport_expression 
  {
    /* pre_increment */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_preincrement_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.expression = $2 ;
      $$->filename = $2->filename ;
      $$->linenum = $2->linenum ;
      if( $2->data_type != JD_exp_num_type || 
          $2->type != JD_varport_exp || 
          !var_port_type( $2->is.var.name->type )   
        ) 
      {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_FOR_NUMERICAL_OPERATION ;
      }
    }
  }
  |
  JD_D_MINUS varport_expression  
  {
    /* pre_decrement */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_predecrement_exp ;
      $$->data_type = JD_exp_num_type ;
      $$->is.expression = $2 ;
      $$->filename = $2->filename ;
      $$->linenum = $2->linenum ;
      if( $2->data_type != JD_exp_num_type || 
          $2->type != JD_varport_exp || 
          !var_port_type( $2->is.var.name->type )   
        ) 
      {
        ERROR_LOCATION( $2->filename, $2->linenum ) ;
        ERROR_ILLEGAL_EXPRESSION_FOR_NUMERICAL_OPERATION ;
      }
    }
  }
  ;



/***********************************
 *  concatenation
 ***********************************/

concatenation_expression:
  '{' expression_list '}' 
  {
    /* concatenation_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_concatenation_exp ;
      $$->is.expression = $2 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
      {
        expression_node *exp = $2 ;
        int n = 0 ;
        while( exp ) {
          n++ ;
          if( exp->data_type != JD_exp_num_type && 
              exp->data_type != JD_exp_string_type )
          {
            ERROR_LOCATION( $2->filename, $2->linenum ) ;
            ERROR_ILLEGAL_NTH_EXPRESSION_FOR_CONCATENATION_OPERATION( n ) ;
            exp = NULL ; 
          }
          else if ( exp->next && exp->data_type != exp->next->data_type ) {
            ERROR_LOCATION( $2->filename, $2->linenum ) ;
            ERROR_ILLEGAL_NTH_EXPRESSION_FOR_CONCATENATION_OPERATION( n+1 ) ;
            exp = NULL ; 
          }
          else exp = exp->next ;
        }
        $$->data_type = $2->data_type ;
      }
    }
  }
  |
  '{' expression '{' expression '}' '}'
  {
    /* duplicate_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_duplication_exp ;
      $$->is.binary.left = $2 ;
      $$->is.binary.right = $4 ;
      $$->filename = $6.filename ;
      $$->linenum = $6.linenum ;
      if( $2->data_type != JD_exp_num_type || !is_int_expression($2)) {
        ERROR_LOCATION( $6.filename, $6.linenum ) ;
        ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT ;
      }
      else if( $4->data_type != JD_exp_num_type &&  
               $4->data_type != JD_exp_string_type ) 
      {
        ERROR_LOCATION( $6.filename, $6.linenum ) ;
        ERROR_NUMERICAL_OR_STRING_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT ;
      }
      $$->data_type = $4->data_type ;
    }
  }
  ;



/************************************
 * list expression
 ************************************/

list_expression:
  '\'' '{' expression_list '}' 
  {
    /* list_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_list_exp ;
      $$->is.expression = $3 ;
      $$->filename = $4.filename ;
      $$->linenum = $4.linenum ;
      $$->data_type = JD_exp_list_type ;
      {
        expression_node *exp = $3 ;
        int n = 0 ;
        while( exp ) {
          n++ ;
          if ( is_array_exp_type(exp->data_type) ) {
            ERROR_LOCATION( $4.filename, $4.linenum ) ;
            ERROR_ARRAY_OR_LIST_CANT_BE_AN_ELEMENT_OF_LIST(n) ;
            exp->next = NULL ; 
          }
          if ( exp->next && exp->data_type != exp->next->data_type ) {
            ERROR_LOCATION( $4.filename, $4.linenum ) ;
            ERROR_ILLEGAL_TYPE_DETECTED_ON_NTH_ELEMENT_FOR_LIST_OPERATION(n+1) ;
            exp = NULL ; 
          }
          else exp = exp->next ;
        }
      }
    }
  }
  ;



/***********************************
 *  variable or port expression
 ***********************************/


varport_expression:
  a_ver_expression
  {
    $$ = $1 ; 
  }
  |
  varport_expression '.' Identifier opt_depth opt_access_range
  {
    /* var_dot_var */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *name ;
      int offset ;
      named_node *class ;
      
      name = find_member( $1, $3.name, &offset, &class ) ;
      if( name == NULL ) {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_UNDEFINED_MEMBER_NAME($3.name) ;
      }
      else if ( (name->is_local || name->is_protected ) &&
                 $1->type != JD_this_exp && $1->type != JD_super_exp ) 
      {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_ILLEGAL_ACCESS_TO_PROTECTED_MEMBER_NAME($3.name) ;
      }
      else {
        $$ = ALLOC(expression_node) ;
        $$->type = JD_varport_exp ;
        $$->is.var.name = name ;
        $$->is.var.depth = $4 ;
        $$->is.var.member_offset = offset ;
        $$->is.var.class = class ;
        $$->filename = $3.filename ;
        $$->linenum = $3.linenum ;
        $$->is.var.pre = $1 ;
        if( $4 ) { 
          /* depth specified */
          if( !port_type( name ) || 
              ( name->sub_type!=JD_single_sub_type && $5 == NULL) ) 
          {
            ERROR_LOCATION( $4->filename, $4->linenum ) ;
            ERROR_ILLEGAL_USAGE_OF_DEPTH ;
          }
        }
        $$->is.var.range = $5 ;
        if( !check_var_range( name, $4, $5, $3.filename, $3.linenum ) ) {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_ILLEGAL_ACCESS_RANGE ;
        }
        set_var_type( $$, $5, $4 ) ; /* exp, range, depth */
      }
    }
  }
  |
  varport_expression '.' JDT_super
  {
    /* var_dot_super */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1->type != JD_varport_exp || 
          $1->is.var.name->type != JD_var_name ||
          $1->is.var.name->data_type->type != JD_class_name ||
          $1->is.var.name->data_type->info.class_name.parent == NULL ) 
      {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_SUPER_USAGE_ON_CLASS_WITHOUT_PARENT ;
      }
      if( is_array_exp_type($1->data_type) ) 
      {
        ERROR_LOCATION( $3.filename, $3.linenum ) ;
        ERROR_ILLEGAL_SUPER_USAGE_ON_ARRAY_OR_LIST ;
      }
      $$ = ALLOC(expression_node) ;
      $$->type = JD_super_exp ;
      $$->data_type = JD_exp_object_type ;
      $$->user_data_type = $1->is.var.name->data_type->info.class_name.parent ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
      $$->is.var.pre = $1 ;
    }
  }
  ;
  
a_ver_expression:
  Identifier opt_depth opt_access_range
  {
    /* var_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      named_node *name ;
      
      name = findname( $1.name ) ;
      if( name == NULL ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNDEFINED_NAME($1.name) ;
      }
      else if( name->type != JD_port_name && name->type != JD_portset_name && 
               name->type != JD_aspect_name &&
               name->data_type == NULL ) 
      {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_UNKNOWN_DATA_TYPE_OF_VARIABLE($1.name) ;
      }
      else if( !var_port_func_type( name->type ) ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_NAME_TYPE_FOR_VARIABLE_FUNCION($1.name) ;
      }
      else {
        $$ = ALLOC(expression_node) ;
        if( name->type == JD_aspect_name )
          $$->type = JD_aspect_exp ;
        else
          $$->type = JD_varport_exp ;
        $$->is.var.name = name ;
        $$->is.var.depth = $2 ;
        if( name->is_member ) {
          int offset ;
          named_node *ret ;
          ret = findmember_in_class( this_class_name, $1.name, &offset ) ;
          if( name != ret ) {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_NAME_TYPE_FOR_VARIABLE_FUNCION($1.name) ;
          }
          $$->is.var.member_offset = offset ;
          $$->is.var.class = this_class_name ;
          if( name->is_local && offset ) {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_ACCESS_TO_PROTECTED_MEMBER_NAME($1.name) ;
          }
          if( class_initial_code && this_class_name->type != JD_aspect_name &&
              (offset || !name->is_static) ) 
          {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_ACCESS_NON_STATIC_MEMBER_NAME_IN_CLASS_INIT($1.name) ;
          }
        }
        $$->filename = $1.filename ;
        $$->linenum = $1.linenum ;
        if( $2 ) { /* depth is defined */
          if( !port_type( name ) || 
              ( name->sub_type!=JD_single_sub_type && $3 == NULL) ) 
          {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_USAGE_OF_DEPTH ;
          }
        }
        $$->is.var.range = $3 ;
        if( !check_var_range( name, $2, $3, $1.filename, $1.linenum ) ) {
          ERROR_LOCATION( $1.filename, $1.linenum ) ;
          ERROR_ILLEGAL_ACCESS_RANGE ;
        }
        set_var_type( $$, $3, $2 ) ; /* exp, range, depth */
      }
    }
  }
  |
  JDT_super
  {
    /* super */
    SKIP_ON_ERROR ;
    PASS2 {
      if( !class_parsing ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_SUPER_USAGE_ON_OUT_OF_CLASS ;
      }
      if( this_class_name->info.class_name.parent == NULL ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_SUPER_USAGE_ON_CLASS_WITHOUT_PARENT ;
      }
      $$ = ALLOC(expression_node) ;
      $$->type = JD_super_exp ;
      $$->data_type = JD_exp_object_type ;
      $$->user_data_type = this_class_name->info.class_name.parent ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  |
  JDT_this
  {
    /* this */
    SKIP_ON_ERROR ; 
    PASS2 {
      if( !class_parsing && !aspect_parsing) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_THIS_USAGE_ON_OUT_OF_CLASS ;
      }
      $$ = ALLOC(expression_node) ;
      $$->type = JD_this_exp ;
      $$->data_type = JD_exp_object_type ;
      if( in_advice ) {
        statement_node *a, *b ;
        expression_node *xa, *xb ;
        int check_ok = 1 ;
        /* check all the pointcut has a single class name */
        a = current_func_name->info.func.pointcut ;
        while( a ) {
          xa = a->prefix ;
          if( xa->type != JD_pointcut_name || xa->next == NULL ) {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_THIS_USAGE_IN_ADVACE_WITH_UNKNOWN_CLASS ;
            a = NULL ;
            check_ok = 0 ;
          }
          else {
            b = a->next ;
            if( b ) {
              xb = b->prefix ;
              if( strcmp( xa->is.string, xb->is.string ) ) {
                ERROR_LOCATION( $1.filename, $1.linenum ) ;
                ERROR_ILLEGAL_THIS_USAGE_IN_ADVACE_WITH_UNKNOWN_CLASS ;
                b = NULL ;
                check_ok = 0 ;
              }
            }
            a = b ;
          }
        }
        if( check_ok ) {
          named_node *cls ;
          xa = current_func_name->info.func.pointcut->prefix ;
          cls = findname_in_scope( top_scope, xa->is.string ) ;
          if( cls ) {
            if(cls->type != JD_class_name) {
              ERROR_LOCATION( $1.filename, $1.linenum ) ;
              ERROR_ILLEGAL_THIS_USAGE_IN_ADVACE_WITH_UNKNOWN_CLASS ;
            }
            else {
              $$->user_data_type = cls ;
            }
          }
          else {
            ERROR_LOCATION( $1.filename, $1.linenum ) ;
            ERROR_ILLEGAL_THIS_USAGE_IN_ADVACE_WITH_UNKNOWN_CLASS ;
          }
        }
      }
      else if( aspect_parsing ) {
        ERROR_LOCATION( $1.filename, $1.linenum ) ;
        ERROR_ILLEGAL_USAGE_OF_THIS_IN_ASPECT_FUNCTION ;
      }
      else 
        $$->user_data_type = this_class_name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

  
opt_depth:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  Real_or_Depth
  {
    /* dot_depth */
    SKIP_ON_ERROR ;
    PASS2 {
      char *depth = &$1.name[1] ;
      $$ = ALLOC(expression_node) ;
      $$->type = JD_int_num ;
      $$->is.int_value = atol(depth) ;
      $$->string = $1.name ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;
  
opt_access_range:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  access_range
  {
    $$ = $1 ; 
  }
  ;

access_range:
  array_access_ranges
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *exp, *exp1 ;
      int i = 0 ;
      $$ = $1 ;
      if( $1 ) {
        exp = $1 ;
        while(exp) {
          i++ ;
          exp = exp->next ;
        }
        $$->is.range.num_index = i ;
        if( i == 2 ) {
          /* [n][n] */
          exp = $1 ;
          exp1 = $$->next ;
          if( exp->type != JD_single_range_exp  && 
              exp1->type != JD_single_range_exp    )
          {
            /* [string][string] is not acceptable */
            ERROR_LOCATION( exp->filename, exp->linenum ) ;
            ERROR_ILLEGAL_DATA_TYPE_FOR_MULTIDIMENTION_ARRAY_ACCESS_RANGE(i) ;
          }
          if( exp->type != JD_single_range_exp  && 
              exp1->type == JD_single_range_exp    )
          {
            /* [string][n], so the last one becomes bit access */
            $$->is.range.num_index = 1 ;
            $$->is.range.bit_index = 1 ;
            exp1->type = JD_single_bit_range_exp ;
          }
        }
        else if( i > 1 ) {
          i = 0 ;
          exp = $1 ;
          while( exp ) {
            /* more than two, all must be [n] */
            i++ ; /* just for error message */
            if( exp->type != JD_single_range_exp ) {
              ERROR_LOCATION( exp->filename, exp->linenum ) ;
              ERROR_ILLEGAL_DATA_TYPE_FOR_MULTIDIMENTION_ARRAY_ACCESS_RANGE(i) ;
            }
            exp = exp->next ;
          }
        }
      }
    }
  }
  |
  array_access_ranges bit_access_range
  {
    /* array_access_range_bit_access_range */
    /* [n]..[u:l] */
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *exp, *tail ;
      int i = 0 ;
      $$ = $1 ;
      if( $1 ) {
        exp = $1 ;
        tail = exp ;
        while(exp) {
          i++ ;
          exp = exp->next ;
          if( exp ) tail = exp ;
        }
        $$->is.range.num_index = i ;
        $$->is.range.bit_index = 1 ; /* set flag to indicate bit access exists */
        if( i > 1 ) {
          i = 0 ;
          exp = $1 ;
          while( exp ) {
            i++ ;
            if( exp->type != JD_single_range_exp ) {
              ERROR_LOCATION( exp->filename, exp->linenum ) ;
              ERROR_ILLEGAL_DATA_TYPE_FOR_MULTIDIMENTION_ARRAY_ACCESS_RANGE(i) ;
            }
            exp = exp->next ;
          }
        }
        tail->next = $2 ; /* connect the bit access range at the end */
      }
    }
  }
  |
  bit_access_range
  {
    $$ = $1 ; 
  }
  ;
  
bit_access_range:
  '[' expression ':' expression ']'
  {
    /* bit_access_range */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_bit_range_exp ;
      $$->is.range.ub = $2 ;
      $$->is.range.lb = $4 ;
      $$->filename = $5.filename ;
      $$->linenum = $5.linenum ;
      if( !is_int_expression($2) || !is_int_expression($4) ) {
        ERROR_LOCATION( $5.filename, $5.linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ARRAY_OR_BIT_ACCESS_RANGE ;
      }
    }
  }
  ;
  
array_access_ranges:
  array_access_range
  {
    $$ = $1 ; 
  }
  |
  array_access_ranges array_access_range
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d2 ;
      d1 = $1 ;
      d2 = $2 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
array_access_range:
  '[' expression ']'  
  {
    /* array_access_range */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->is.range.ub = $2 ;
      $$->filename = $3.filename ;
      $$->linenum = $3.linenum ;
      if( is_string_expression($2) )
        $$->type = JD_string_range_exp ;
      else {
        $$->type = JD_single_range_exp ;
        if( !is_int_expression($2)  ) {
          ERROR_LOCATION( $3.filename, $3.linenum ) ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ARRAY_OR_BIT_ACCESS_RANGE ;
        }
      }
    }
  }
  ; 
 


/***********************************
 *  function call
 ***********************************/

  
function_call_expression:
  varport_expression '(' opt_actual_arguments ')'  
  {
    /* func_call_exp */
    SKIP_ON_ERROR ;
    PASS2 {
      if( $1 ) {
        $1->type = JD_function_call_exp ;
        $1->is.var.args = $3 ;
        if( !is_func_name( $1->is.var.name ) ) {
          ERROR_LOCATION( $1->filename, $1->linenum ) ;
          ERROR_ILLEGAL_NAME_FOR_FUNCTION_CALL( $1->is.var.name->name ) ;
        }
        else {
          set_func_type( $1 ) ;
        }
        /* can't check here, do it expression.c 
        actual_arg_check(  
          $1->is.var.name, $3, $1->is.var.pre, $1->filename, $1->linenum 
        ) ;
        */
        $1->filename = $4.filename ;
        $1->linenum = $4.linenum ;
      }
      $$ = $1 ;
    }
  }
  ;

opt_actual_arguments:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  actual_arguments
  {
    $$ = $1 ; 
  }
  ;
  
actual_arguments:
  actual_argument 
  {
    $$ = $1 ; 
  }
  |
  actual_arguments ',' actual_argument
  {
    SKIP_ON_ERROR ;
    PASS2 {
      expression_node *d1, *d3 ;
      d1 = $1 ;
      d3 = $3 ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    $$ = $1 ;
  }
  ;
  
actual_argument:
  expression 
  {
    $$ = $1 ; 
  }
  |
  '*'
  {
    /* default_arg */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_default_arg ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

/***********************************
 *  new
 ***********************************/

new_expression:
  JDT_new opt_new_args opt_new_object 
  {
    /* new_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_new_exp ;
      if( $3 && $3->type == JD_new_block_exp )
        $$->data_type = JD_exp_new_block_type ;
      else $$->data_type = JD_exp_new_type ;
      $$->is.new.args = $2 ;
      $$->is.new.obj = $3 ;
      $$->filename = $1.filename ;
      $$->linenum = $1.linenum ;
    }
  }
  ;

opt_new_args:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  '(' ')'
  {
    $$ = NULL ;
  }
  |
  '(' actual_arguments ')'
  {
    $$ = $2 ; 
  }
  ;
  
opt_new_object:
  /* empty */
  {
    $$ = NULL ;
  }
  |
  left_expression 
  {
    $$ = $1 ; 
  }
  |
  block  /* this is for thread new */
  {
    /* new_block */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_new_block_exp ;
      $$->is.new_bl.ock = $1 ;
    }
  }
  ;


/***********************************
 *  assign expression ( not a part of expression)
 ***********************************/


assign_expression:
  left_expression assign_opr expression  
  {
    /* assign_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_assign_exp ;
      $$->is.assign.lvalue = $1 ;
      $$->is.assign.ops = $2 ;
      $$->is.assign.expression = $3 ;
      $$->is.assign.strength = JD_normal ;
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
      if( $$->is.assign.lvalue->type == JD_varport_exp &&
          $$->is.assign.lvalue->is.var.range != NULL &&
          $$->is.assign.lvalue->is.var.name->sub_type == JD_list_sub_type
        ) 
      {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_LIST_ACCESS_FOR_ASSIGNMENT ; 
      }
      else if( !check_assign_data_type( 
           $$->is.assign.lvalue, $$->is.assign.expression, 1 ) 
        ) 
      {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGNMENT ; 
      }
      else if( $2 != JD_assign_EQ && $2 != JD_assign_NB_EQ && 
          ($1->type != JD_varport_exp || $1->data_type != JD_exp_num_type
           || $3->data_type != JD_exp_num_type )
        )
      {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ARITHMETIC_ASSIGNMENT ; 
      }
      else 
        set_assign_data_type_offset( 
         $$->is.assign.lvalue, $$->is.assign.expression
        );
      
      if( $3->type == JD_rnd_exp || $3->type == JD_cyc_exp ) 
      {
        expression_node *lvalue = $$->is.assign.lvalue ;
        switch( lvalue->is.var.name->type ) {
          case JD_port_name:
          
            break ;
          case JD_var_name:
          case JD_mvar_name:
          case JD_arg_name:
            switch( lvalue->is.var.name->data_type->type ) {
              case JD_int_type:
                $3->is.rnd.dst_width = -1 ;
                break ;
              case JD_bit_type:
                if( lvalue->is.var.name->data_type->i_endian )
                  $3->is.rnd.dst_width = 
                    lvalue->is.var.name->data_type->lb - 
                    lvalue->is.var.name->data_type->ub + 1 ;
                else
                  $3->is.rnd.dst_width = 
                    lvalue->is.var.name->data_type->ub - 
                    lvalue->is.var.name->data_type->lb + 1 ;
                break ;
            }
            break ;
        }
      }
    }
  }
  |
  left_expression '=' new_expression
  {
    /* assign_new_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_assign_exp ;
      $$->is.assign.lvalue = $1 ;
      $$->is.assign.ops = JD_assign_EQ ;
      $$->is.assign.expression = $3 ;
      $$->is.assign.strength = JD_normal ;
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
      if( !check_assign_data_type( 
           $$->is.assign.lvalue, $$->is.assign.expression, 1 ) 
        ) 
      {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGNMENT ; 
      }
    }
  }
  |
  list_expression '=' expression
  {
    /* assign_list_expression */
    SKIP_ON_ERROR ;
    PASS2 {
      $$ = ALLOC(expression_node) ;
      $$->type = JD_assign_list_exp ;
      $$->is.assign.lvalue = $1 ;
      $$->is.assign.ops = JD_assign_EQ ;
      $$->is.assign.expression = $3 ;
      $$->is.assign.strength = JD_normal ;
      $$->filename = $3->filename ;
      $$->linenum = $3->linenum ;
      if( !check_assign_list_data_type( 
           $$->is.assign.lvalue, $$->is.assign.expression ) 
        ) 
      {
        ERROR_LOCATION( $$->filename, $$->linenum ) ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGNMENT ; 
      }
    }
  }
  ;
  
left_expression:
  varport_expression 
  {
    $$ = $1 ; 
  }
  ;
  
assign_opr:
  '=' 
  {
    $$ = JD_assign_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_LE 
  {
    $$ = JD_assign_NB_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_PLUS_EQ 
  {
    $$ = JD_assign_Pls_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_MINUS_EQ 
  {
    $$ = JD_assign_Minus_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_STAR_EQ 
  {
    $$ = JD_assign_Time_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_SLSH_EQ 
  {
    $$ = JD_assign_Div_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_MOD_EQ  
  {
    $$ = JD_assign_Mod_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_RSHIFT_EQ 
  {
    $$ = JD_assign_Rsft_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_URSHIFT_EQ 
  {
    $$ = JD_assign_Ursft_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_LSHIFT_EQ 
  {
    $$ = JD_assign_Lsft_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_AND_EQ 
  {
    $$ = JD_assign_And_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_OR_EQ 
  {
    $$ = JD_assign_Or_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_EOR_EQ 
  {
    $$ = JD_assign_Eor_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_NAND_EQ 
  {
    $$ = JD_assign_Nand_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_ANDN_EQ 
  {
    $$ = JD_assign_Nand_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_NOR_EQ 
  {
    $$ = JD_assign_Nor_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_ORN_EQ
  {
    $$ = JD_assign_Nor_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_NEOR_EQ 
  {
    $$ = JD_assign_Neor_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  |
  JD_EORN_EQ 
  {
    $$ = JD_assign_Neor_EQ ;
    filename = $1.filename ;
    linenum = $1.linenum ;
  }
  ;



semicolon:
  ';' 
  {
    yyerrok ;
    /*
    ex_error_flag |= error_flag ;
    error_flag = 0 ;
    */
    $$ = $1 ;
  }
  ;

    
%%
