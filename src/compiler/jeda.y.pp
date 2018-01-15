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
#INCLUDE "jeda_keyword_token.h" 
#INCLUDE "jeda_operator_token.h" 

#INCLUDE "jeda_nonterminal_node_type.h" 

#BEGIN precedence_table
/* precedence table */
%right '>>=' '>>>=' '<<=' '&=' '|=' '^=' '~&=' '&~=' '~^=' '^~=' '~|=' '|~='
%right '=' '+=' '-=' '*=' '/=' '%='
%right '?' ':' 
%left '||' 
%left '&&' 
%left '|' 
%left '^'  
%left '&'  
%left '=:=' 
%left '=?=' '!?=' '==' '!=' '===' '!==' '==?' '?=='
%left '>' '<=' '<' '>=' 
%left '<<' '>>' '>>>'
%left '-' '+'
%left  '*' '/' '%'
%left  '!' '~' 
%left  '~&' '&~' '~|' '|~' '~^' '^~'  
%right '++' '--' 
%left  '.'

#END

%start jeda

%%

#BEGIN translation_unit

jeda:  translation_units
  ;
  
translation_units:
  translation_unit
  <<trns_unit>>
  |
  translation_units translation_unit
  <<Block_D1_NEXTeqD2_errok>>
ERROR_BEGIN
  |
  error 
  <<DDeqNULL>>
  |
  translation_units error  
  <<DDeqNULL>>
ERROR_END
  ;
  
translation_unit:
  port_definition
  <<DDeqD1>>
  |
  portset_definition
  <<DDeqD1>>
  |
  connectionset_definition
  <<DDeqD1>>
  |
  local_global_variable_declaration
  <<DDeqD1>>
  |
  top_initial_statement
  <<DDeqD1>>
  |
  type_definition
  <<type_block>>
  |
  function_definition
  <<DDeqD1>>
  |
  class_definition
  <<DDeqD1>>  
  |
  verilog_class_definition
  <<DDeqD1>>  
  |
  aspect_definition
  <<DDeqD1>>  
  ;

#END

/***********************************
 *  port 
 ***********************************/
#BEGIN port

port_definition:
  port_declaration
  <<port_definition>>
  ;
  
port_declaration:
  port_direction 
  opt_bit_range 
  port_name 
  opt_port_attribs 
  semicolon
  <<port_dcl>>
  ;


port_direction:
  'input'  
  <<input>>
  |
  'output'  
  <<output>>
  |
  'inout'   
  <<inout>>
  ;

opt_bit_range:
  /* empty */
  <<DDeqNULL>>
  |
  bit_range
  <<DDeqD1>>
  ;
  
bit_range:
  '[' constant ':' constant ']'
  <<bit_range>>
  ;
  
port_name: 
  Identifier 
  <<port_name>>
  ;

opt_port_attribs:
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
  <<Expression_D1_NEXTeqD2>>
  ;
  
#END

/***********************************
 *  port attribute
 ***********************************/

#BEGIN port_attrib

port_attrib:
  clock_attrib
  <<DDeqD1>>
  |
  drive_attrib
  <<DDeqD1>>
  |
  sample_attrib
  <<DDeqD1>>
  |
  depth_attrib
  <<DDeqD1>>
  |
  node_attrib
  <<DDeqD1>>
  ;
  
clock_attrib:
  '@' clock_port_name
  <<clock_attrib>>
  ;
  
clock_port_name: 
  Identifier 
  <<individual_port_name>>
  |
  Identifier '.' Identifier
  <<portset_port>>
  ;

drive_attrib:
  'drive' edge_specification opt_skew
  <<drive_attrib>>  
  ;

sample_attrib:
  'sample' edge_specification opt_skew 
  <<sample_attrib>>  
  ;
    
opt_skew:
  /* empty */
  <<DDeqNULL>>
  |
  'skew' constant
  <<skew_attrib>>
  ;

depth_attrib:
  'depth' constant
  <<depth_attrib>>
  ;

node_attrib:
  'node' String_literal
  <<node_attrib>>
  ;

#END

/***********************************
 *  port set
 ***********************************/
#BEGIN port_set

portset_definition:
  'portset' 
  portset_name opt_port_attribs
  '{'
    <<enter_port_scope>>
    port_declarations
    <<exit_scope>>
  '}' 
  <<portset_def>>
  ;

portset_name: 
  Identifier
  <<portset_name>> 
  ;

port_declarations:
  port_declaration
  <<DDeqD1>>
  |
  port_declarations 
  port_declaration
  <<Expression_D1_NEXTeqD2_errok>>  
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  port_declarations error  
  <<DDeqNULL>>
ERROR_END
  ;
  
#END


/***********************************
 *  connection set
 ***********************************/
#BEGIN connection_set

connectionset_definition:
  'connectionset' 
  '{' 
    port_connections
  '}' 
  <<connectionset_definition>>
  ;
  
port_connections:
  port_connection
  <<DDeqD1>>
  |
  port_connections port_connection
  <<Connection_D1_NEXTeqD2_errok>>  
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  port_connections error  
  <<DDeqNULL>>
ERROR_END
  ;
  
port_connection:
  portset_port  String_literal semicolon 
  <<port_connection>>
  ;
  
portset_port:
   Identifier '.' Identifier 
   <<portset_port>>
   |
   Identifier
   <<individual_port_name>>
   ;

#END

/***********************************
 *  local and global variable
 ***********************************/

#BEGIN local_global_variable_declaration

local_global_variable_declaration:
  var_definition
  <<local_global_var_dcl>>
  ;
#END

/***********************************
 *  initial statement
 ***********************************/
#BEGIN initial_statement

top_initial_statement:
  'initial' 
  <<top_initial_statement_begin>>
  statement
  <<initial_statement>>
  ;
  
initial_statement:
  'initial' 
  <<initial_statement_begin>>
  statement 
  <<initial_statement_end>>
  ;

#END

/***********************************
 *  variable and enum definition
 ***********************************/
#BEGIN var_enum_definition

opt_var_enum_definitions:
  /* empty */
  <<DDeqNULL>>
  |
  var_enum_definitions
  <<DDeqD1>>
  ;

var_enum_definitions:
  var_enum_definition
  <<DDeqD1>>
  |
  var_enum_definitions var_enum_definition
  <<Statement_D1_NEXTeqD2_errok>>  
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  var_enum_definitions error  
  <<DDeqNULL>>
ERROR_END
  ;
  
var_enum_definition:
  var_definition
  <<DDeqD1>>
  |
  enum_type_definition 
  <<DDeqD1>>
  ;

#END

/***********************************
 *  variable definition
 ***********************************/
#BEGIN variable_definition

var_definition:
  opt_var_func_attrib var_func_type var_name_list semicolon
  <<var_defs>>
  ;

var_name_list:
  var_name 
  <<DDeqD1>>
  |
  var_name_list ',' var_name
  <<Name_D1_NEXTeqD3>>
  ;
  
var_name:
  Identifier opt_array_range opt_var_init
  <<var_name>>
  ;

/*
  An array can be statically allocated (e.g. ary[5]), string indexed
  associative array (e.g. sary[*]), associative array (e.g. asary[]),
  and list (e.g. list <> ).
*/
opt_array_range:
  /* emptry */
  <<DDeqNULL>>
  |
  array_ranges  
  <<array_ranges>>
  |
  '['  '*'  ']' 
  <<string_indexed_array_range>>
  |
  '['  ']'
  <<assoc_array_range>>
  |
  '<'  '>' 
  <<list_range>>
  ;
  
array_ranges:
  array_range
  <<DDeqD1>>
  |
  array_ranges array_range
  <<P1_Expression_D1_NEXTeqD2>>
  ;
  
array_range:
  '[' constant ']'
  <<array_range>>
  ;

opt_var_init:
  /* empty */
  <<DDeqNULL>>
  |
  var_init 
  <<DDeqD1>>
  ;
  
var_init:
  '=' expression 
  <<DDeqD2>>
  |
  '=' new_expression
  <<DDeqD2>>
  ;

#END

/***********************************
 *  data types
 ***********************************/
#BEGIN data_type

opt_func_type:
  /* empty */
  <<evoid>>
  |
  var_func_type opt_array_range
  <<func_type>>
  ;
  
var_func_type:  
  data_type  
  <<DDeqD1>>
  |
  user_data_type 
  <<DDeqD1>>
  ;
  
data_type:
  'void'
  <<void>>
  |
  'int' 
  <<int>>
  |
  'float'  
  <<float>>
  |
  'double'  
  <<double>>
  |
  'bit' opt_bit_range
  <<bit>>
  |
  'string'
  <<string>>
  |
  'signal'
  <<signal>>
  |
  'class_type'
  <<class_type>>
  ;
  
user_data_type: 
  Identifier
  <<user_type>>
  ;

#END

/***********************************
 *  type definition
 ***********************************/
#BEGIN type_definition

type_definition:
  enum_type_definition 
  <<DDeqD1>>
  |
  class_type_definition
  <<DDeqNULL>>
  ;
  
enum_type_definition:
  'enum' Identifier '=' enum_members_list semicolon
  <<enum_type_dec>>
  ;

enum_members_list:
  enum_member
  <<DDeqD1>>
  |
  enum_members_list ',' enum_member
  <<Name_D1_NEXTeqD3>>
  ;
  
enum_member: 
  Identifier 
  <<enum_member>>
  ;
  

class_type_definition:
  'typedef' 'class' class_name_list semicolon 
  <<DDeqNULL>>
  ;
  
class_name_list:
  class_name
  <<DDeqD1>>
  |
  class_name_list ',' class_name
  <<Name_D1_NEXTeqD3>>
  ;


data_type_name:
  'array' 
  <<array>>
  |
  'marray' 
  <<marray>>
  |
  'assoc' 
  <<assoc>>
  |
  'stridx'  
  <<stridx>>
  |
  'list'  
  <<list>>
  |
  'string'
  <<string>>
  |
  'signal'
  <<signal>>
  |
  'port'
  <<port>>
  ;

class_name:
  Identifier
  <<class_name>>
  |
  '.' data_type_name
  <<dot_class_name>>
  ;

class_name_ref:
  Identifier
  <<class_name_ref>>
  ;

#END

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

#BEGIN var_func_class_attrib

/* This definition is globally shared by all the types
   for persing reason, and semantics must be handled at action level per 
   each type.
*/

opt_var_func_attrib:
  /* empty */
  <<DDeqNULL>>
  | 
  var_func_attribs 
  <<DDeqNULL>>
  ;
  
var_func_attribs: 
  var_func_attrib
  <<DDeqNULL>>
  |
  var_func_attribs var_func_attrib
  <<DDeqNULL>>
  ;
  
var_func_attrib:
  'static'
  <<static>>
  | 
  'extern'
  <<extern>>
  | 
  'local'
  <<local>>
  | 
  'global'
  <<global>>
  | 
  'protected'
  <<protected>>
  | 
  'abstract'
  <<abstract>>
  | 
  'final'
  <<final>>
  | 
  'export'
  <<export>>
  | 
  'clone'
  <<clone>>
  |
  'overwrite'
  <<overwrite>>
  ;
  
#END

/***********************************
 *  function definition
 ***********************************/
#BEGIN function_definition

function_definition:
  opt_var_func_attrib 
  opt_func_type 
  func_name
  '(' 
  <<func_name_found>>
    opt_formal_args 
  ')' 
  <<formal_arg_check>>
  opt_outer_block
  <<func_define>>
  ;

func_name:
  Identifier 
  <<func_name>>
  ;


opt_outer_block:
  semicolon
  <<DDeqNULL>>
  |
  block
  <<DDeqD1>>
  ;

opt_formal_args:
  /* empty */
  <<DDeqNULL>>
  |
  formal_args
  <<DDeqD1>>
  ;
  
formal_args:
  formal_arg
  <<DDeqD1>>
  |
  formal_args ',' formal_arg 
  <<Statement_D1_NEXTeqD3>>
  ;

formal_arg:
  opt_var var_func_type arg_name opt_array_range opt_arg_init 
  <<formal_arg>>
  |
  var_func_type
  <<formal_arg_type>>
  |
  '?'
  <<wild_card_arg>>
  |
  '(' formal_args ')' '+'
  <<one_or_more_args>>
  |
  '(' formal_args ')' '*'
  <<zero_or_more_args>>
  |
  '(' formal_args ')' '|'
  <<or_of_formal_args>>
  |
  '[' formal_args ']' 
  <<opt_formal_args>>
  ;

opt_var:
  /* empty */
  <<DDeq0>>
  |
  'var' 
  <<DDeq1>>
  ;
  
arg_name:
  Identifier
  <<arg_name>>
  ;

opt_arg_init:
  /* empty */
  <<DDeqNULL>>
  |
  arg_init 
  <<DDeqD1>>
  ;
  
arg_init:
  '=' constant 
  <<DDeqD2>>
  |
  '=' string_constant
  <<DDeqD2>>
  |
  '=' bool_null_expression
  <<DDeqD2>>
  |
  '=' enum_expression
  <<DDeqD2>>
  ;

enum_expression:
  Identifier
  <<enum_expression>>
  ;

#END

/***********************************
 *  class definition
 ***********************************/
#BEGIN class_definition

class_definition:
  opt_var_func_attrib 'class' class_name 
  opt_extention
  <<class_def_begin>>
  '{'
    opt_class_items
  '}' 
  <<class_def_end>>
  ;

opt_extention:
  /* empty */
  <<DDeqNULL>>
  |
  'extends' class_name_ref
  <<DDeqD2>>
  ;
  
opt_class_items:
  /* empty */
  <<DDeqNULL>>
  |
  class_items
  <<DDeqNULL>>
  ;
  
class_items:
  class_item
  <<DDeqNULL>>
  |
  class_items class_item
  <<DDeqNULL_errok>>
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  class_items error  
  <<DDeqNULL>>
ERROR_END
  ;

class_item:
  var_definition
  <<class_var_definition>>
  |
  enum_type_definition 
  <<class_enum_definition>>
  |
  initial_statement
  <<class_initial_statement>>
  |
  function_definition
  <<class_function_definition>>
  |
  new_function_definition
  <<class_function_definition>>
  ;
  
new_function_definition:
  opt_void 'new'
  '(' 
  <<new_func_found>>
    opt_formal_args 
  ')' 
  <<new_formal_arg_check>>
  opt_new_outer_block
  <<new_func_define>>
  ;

opt_new_outer_block:
  semicolon
  <<DDeqNULL>>
  |
  new_block
  <<DDeqD1>>
  ;

new_block:
  '{'
   <<block_begin>>
    opt_var_enum_definitions
    opt_super_new_call
    opt_statements
  '}'
  <<new_block_statement>>
  ;

opt_super_new_call:
  /* empty */
  <<DDeqNULL>>
  |
  'super' '.' 'new' opt_new_args semicolon
  <<super_new_call>>
  ;

opt_void:
  /* empty */
  <<DDeqNULL>>
  |
  'void'
  <<DDeqNULL>>
  ;
  
#END

/***********************************
 *  verilog class definition
 ***********************************/
#BEGIN verilog_class_definition

verilog_class_definition:
  opt_var_func_attrib 'class' 'verilog' '{'
  <<verilog_class_begin>>
     verilog_task_definitions  
  '}'
  <<verilog_class_end>>
  ;
  
verilog_task_definitions:
  verilog_task_definition
  <<DDeqNULL>>
  |
  verilog_task_definitions verilog_task_definition
  <<DDeqNULL_errok>>
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  verilog_task_definitions error  
  <<DDeqNULL>>
ERROR_END
  ;

verilog_task_definition:
  opt_func_type 
  func_name
  '(' 
  <<veri_func_name_found>>
    opt_formal_args 
  ')' 
  String_literal semicolon
  <<veri_func_define>>
  ;
  
#END

/***********************************
 *  aspect definition
 ***********************************/
#BEGIN aspect_definition

aspect_name:
  Identifier
  <<aspect_name>>
  ;

aspect_definition:
  opt_extern 'aspect' aspect_name 
  '{'
  <<aspect_begin>>
    opt_aspect_items
  '}'
  <<aspect_end>>
  ;
  
opt_extern:
  /* emptry */
  <<DDeqNULL>>
  | 
  'extern'
  <<extern>>
  ;

opt_aspect_items:
  /* empty */
  <<DDeqNULL>>
  |
  aspect_items
  <<DDeqD1>>
  ;
     
aspect_items:
  aspect_item
  <<DDeqNULL>>
  |
  aspect_items aspect_item
  <<DDeqNULL_errok>>
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  aspect_items error  
  <<DDeqNULL>>
ERROR_END
  ;
    
aspect_item:
  var_definition
  <<class_var_definition>>
  |
  enum_type_definition 
  <<class_enum_definition>>
  |
  initial_statement
  <<class_initial_statement>>
  |
  function_definition
  <<aspect_function_definition>>
  |
  advice_definition
  <<aspect_advice_definition>>
  ;
  
advice_name:
  Identifier
  <<advice_name>>
  ;


advice_definition:
  'advice' opt_func_type advice_name 
  '(' 
  <<advice_name_found>>
    opt_formal_args 
  ')' 
  pointcut_specifications
  <<advice_pointcut>>
  opt_outer_block
  <<advice_define>>
  ;
      
pointcut_specifications:
  pointcut_spec
  <<DDeqD1>>
  |
  pointcut_specifications pointcut_spec
  <<Statement_D1_NEXTeqD2>>
  ;

pointcut_spec:
  'call' '(' pointcut_pattern ')'  
  <<call_pointcut>>
  |
  'return' '(' pointcut_pattern ')'
  <<return_pointcut>>
  |
  'overwrite' '(' pointcut_pattern ')'
  <<overwrite_pointcut>>
  ;

pointcut_pattern:
  pointcut_element
  <<DDeqD1>>
  |
  pointcut_pattern '.' pointcut_element
  <<Expression_D1_NEXTeqD3>>
  ;
  
pointcut_element:
  Identifier
  <<pointcut_name>>
  |
  String_literal
  <<string_expression>>
  ;
  
#END

/***********************************
 *  statement
 ***********************************/
#BEGIN statement 

statement:
  null_statement
  <<DDeqD1>>
  |
  if_else_statement 
  <<DDeqD1>>
  |
  while_statement 
  <<DDeqD1>>
  |
  for_statement 
  <<DDeqD1>>
  |
  repeat_statement 
  <<DDeqD1>>
  |
  do_while_statement 
  <<DDeqD1>>
  |
  forever_statement 
  <<DDeqD1>>
  |
  case_statement 
  <<DDeqD1>>
  |
  casex_statement 
  <<DDeqD1>>
  |
  casez_statement 
  <<DDeqD1>>
  |
  randcase_statement 
  <<DDeqD1>>
  |
  break_statement 
  <<DDeqD1>>
  |
  continue_statement
  <<DDeqD1>>
  |
  fork_join_statement
  <<DDeqD1>>
  |
  terminate_statement
  <<DDeqD1>>
  |
  breakpoint_statement
  <<DDeqD1>>
  |
  expression_statement
  <<DDeqD1>>
  |
  assign_statement
  <<DDeqD1>>
  |
  function_call_statement
  <<DDeqD1>>
  |
  return_statement
  <<DDeqD1>>
  |
  verilog_task_call_statement
  <<DDeqD1>>
  |
  block
  <<DDeqD1>>
  ;

#END

/***********************************
 *  block
 ***********************************/
#BEGIN block

block:
  '{'
   <<block_begin>>
    opt_var_enum_definitions
    opt_statements
  '}'
  <<block_statement>>
  ;

opt_statements:
  /* empty */
  <<DDeqNULL>>
  |
  statements
  <<DDeqD1>>
  ;

statements:
  statement
  <<DDeqD1>>
  |
  statements statement
  <<Statement_D1_NEXTeqD2_errok>>
ERROR_BEGIN
  |
  error
  <<DDeqNULL>>
  |
  statements error  
  <<DDeqNULL>>
ERROR_END
  ;

#END

/***********************************
 *  statement prefix
 ***********************************/
#BEGIN  statement_prefix

opt_statement_prefix:
  /* empty */
  <<DDeqNULL>>
  |
  cycle_element
  <<DDeqD1>>
  |
  label
  <<label>>
  |
  label cycle_element
  <<label_cycle>>
  ;

label:
  Identifier ':'
  <<DDeqD1>>
  ;
  
cycle_element:
  '@' opt_expression opt_clock_definition 
  <<cycle_element>>
  ;
  
opt_clock_definition:
  /* empty */
  <<DDeqNULL>>
  |
  '(' edge_specification  port_expression ')'
  <<clock_def>>
  ;
  
edge_specification:
  'posedge' 
  <<posedge>>
  |
  'negedge' 
  <<negedge>>
  |
  'bothedge' 
  <<bothedge>>
  |
  /* empty */
  <<noedge>>
  ;

port_expression:
  varport_expression 
  <<DDeqD1>>
  ;

#END


/***********************************
 *  null statement
 ***********************************/
#BEGIN null_statement

null_statement:
  opt_statement_prefix semicolon
  <<null_statement>>
  ;

#END

/***********************************
 *  if statement
 ***********************************/
#BEGIN if_statement  

if_else_statement:
  opt_statement_prefix 'if' '(' expression ')' statement
  opt_else
  <<if_statement>>
  ;

opt_else:
  /* empty */
  <<DDeqNULL>>
  |
  'else' statement
  <<else>>
  ;
#END

/***********************************
 *  while statement
 ***********************************/
#BEGIN while_statement   

while_statement:
  opt_statement_prefix 'while' '(' expression ')' 
  <<loop_statement>>
  statement 
  <<while_statement>>
  ;

#END

/***********************************
 *  for statement
 ***********************************/
#BEGIN for_statement

for_statement:
  opt_statement_prefix 'for' '(' 
    opt_multi_expressions ';' opt_expression ';' opt_multi_expressions
  ')' 
  <<loop_statement>>
  statement 
  <<for_statement>>
  ;
  
opt_multi_expressions:
  /* empty */
  <<DDeqNULL>>
  |
  multi_expressions
  <<DDeqD1>>
  ;

multi_expressions:
  for_expression
  <<DDeqD1>>
  |
  multi_expressions ',' for_expression
  <<Expression_D1_NEXTeqD3>>
  ;
  
for_expression:
  state_expression 
  <<DDeqD1>>
  |
  assign_expression
  <<DDeqD1>>
  ;
  
#END

/***********************************
 *  repeat statement
 ***********************************/
#BEGIN repeat_statement

repeat_statement:
  opt_statement_prefix 'repeat' '(' expression ')' 
  <<repeat_loop_statement>>
  statement 
  <<repeat_statement>>
  ;

#END

/***********************************
 *  do while statement
 ***********************************/
#BEGIN do_while_statement

do_while_statement:
  opt_statement_prefix 'do' 
  <<loop_statement>>
  statement 'while' '(' expression ')' semicolon 
  <<do_while_statement>>
  ;

#END

/***********************************
 *  forever statement
 ***********************************/
#BEGIN forever_statement

forever_statement:
  opt_statement_prefix 'forever' 
  <<loop_statement>>
  statement 
  <<forever_statement>>
  ;

#END

/***********************************
 *  case statement
 ***********************************/
#BEGIN case_statement

case_statement:
  opt_statement_prefix 'case' '(' expression ')' '{'
    opt_case_items
  '}' 
  <<case_statement>>
  ;

opt_case_items:
  /* empty */
  <<DDeqNULL>>
  |
  case_items
  <<DDeqD1>>
  ;

case_items:
  case_item
  <<DDeqD1>>
  |
  case_items case_item
  <<Statement_D1_NEXTeqD2>>
  ;
    
case_item:
  expression_list ':' statement
  <<case_item>>
  |
  'default' ':' statement 
  <<case_default>>
  ;

expression_list:
  expression
  <<DDeqD1>>
  |
  expression_list ',' expression
  <<Expression_D1_NEXTeqD3>>
  ;

#END

/***********************************
 *  casex statement
 ***********************************/
#BEGIN casex_statement

casex_statement:
  opt_statement_prefix 'casex' '(' expression ')' '{'
    opt_case_items
  '}'
  <<casex_statement>> 
  ;

#END

/***********************************
 *  casez statement
 ***********************************/
#BEGIN casez_statement

casez_statement:
  opt_statement_prefix 'casez' '(' expression ')' '{'
    opt_case_items
  '}' 
  <<casez_statement>>
  ;

#END

/***********************************
 *  randcase statement
 ***********************************/
#BEGIN randcase_statement

randcase_statement:
  opt_statement_prefix 'randcase' '{'
    opt_randcase_items
  '}' 
  <<randcase_statement>>
  ;

opt_randcase_items:
  /* empty */
  <<DDeqNULL>>
  |
  randcase_items
  <<DDeqD1>>
  ;

randcase_items:
  randcase_item
  <<DDeqD1>>
  |
  randcase_items randcase_item
  <<Statement_D1_NEXTeqD2>>
  ;
    
randcase_item:
  Decimal_num ':' statement
  <<randcase_item>>
  ;

#END

/***********************************
 *  break statement
 ***********************************/
#BEGIN break_statement

break_statement:
  opt_statement_prefix 'break' opt_exit_label semicolon
  <<break_statement>> 
  ;

opt_exit_label:
  /* empty */
  <<NullLabel>>
  |
  Identifier
  <<DDeqD1>>
  ;
  
#END

/***********************************
 *  continue statement
 ***********************************/
#BEGIN continue_statement

continue_statement:
  opt_statement_prefix 'continue' opt_exit_label semicolon 
  <<continue_statement>>
  ;

#END

/***********************************
 *  fork join statement
 ***********************************/
#BEGIN fork_join_statement

fork_join_statement:
  opt_statement_prefix 'fork'
    statements
  joins  
  <<fork_join_statement>>
  ;

joins:
  /* empty */
  'join'
  <<join>>
  |
  'spoon' 
  <<spoon>>
  |
  'knife' 
  <<knife>>
  ;

#END

/***********************************
 *  terminate statement
 ***********************************/
#BEGIN terminate_statement

terminate_statement:
  opt_statement_prefix 'terminate' semicolon 
  <<teminate_statement>>
  ;

#END

/***********************************
 *  breakpoint statement
 ***********************************/
#BEGIN breakpoint_statement
/* working here */
breakpoint_statement:
  opt_statement_prefix 'breakpoint' opt_string semicolon 
  <<breakpoint_statement>>
  ;

opt_string:
  /* nothing */
  <<DDnameeqNULL>>
  |
  String_literal
  <<DDeqD1>>
  ;
  
#END

/***********************************
 *  expression statement
 ***********************************/
#BEGIN expression_statement

expression_statement:
  opt_statement_prefix state_expression semicolon 
  <<expression_statement>> 
  ;

#END

/***********************************
 *  assign statement
 ***********************************/
#BEGIN assign_statement

assign_statement:
  opt_statement_prefix assign_expression opt_strength semicolon
  <<assign_statement>> 
  ;
  
opt_strength:
  /* nothing */
  <<normal>>
  |
  'weak'
  <<weak>>
  |
  'strong'
  <<strong>>
  ;

#END

/***********************************
 *  function call statement
 ***********************************/
#BEGIN function_call_statement

function_call_statement:
  opt_statement_prefix function_call_expression semicolon 
  <<func_call_statement>>
  ;

return_statement:
  'return' opt_expression semicolon
  <<return_statement>>
  ;

#END

/***********************************
 *  verilog task call statement
 ***********************************/
#BEGIN verilog_task_call_statement

verilog_task_call_statement:
  'verilog' '.' Identifier 
  '(' 
    opt_actual_arguments 
  ')' semicolon
  <<verilog_task_call_statement>>
  ;

#END


/***********************************
 *  numbers
 ***********************************/
#BEGIN number_representation

number:
  Real_num
  <<real_num>>
  |
  Real_or_Depth
  <<real_num>>
  |
  Decimal_num
  <<decimal_num>>
  |
  V_decimal_num
  <<v_decimal_num>>
  |
  C_decimal_num
  <<c_decimal_num>>
  |
  V_hex_num
  <<v_hex_num>>
  |
  C_hex_num
  <<c_hex_num>>
  |
  V_octal_num
  <<v_octal_num>>
  |
  C_octal_num
  <<c_octal_num>>
  |
  V_bin_num
  <<v_bin_num>>
  |
  C_bin_num
  <<c_bin_num>>
  ;

#END


/***********************************
 *  constant
 ***********************************/
#BEGIN constant

constant:
  number
  <<DDeqD1>>
  |
  constant '-' constant
  <<const_minus_const>>
  |
  constant '+' constant
  <<const_plus_const>>
  |
  constant '*' constant   
  <<const_times_const>>
  |
  constant '/' constant   
  <<const_div_const>>
  |
  constant '%' constant   
  <<const_mod_const>>
  |
  constant '&' constant   
  <<const_and_const>>
  |
  constant '|' constant   
  <<const_or_const>>
  |
  constant '^' constant   
  <<const_eor_const>>
  |
  constant '&~' constant  
  <<const_nand_const>>
  |
  constant '~&' constant  
  <<const_nand_const>>
  |
  constant '|~' constant  
  <<const_nor_const>>
  |
  constant '~|' constant  
  <<const_nor_const>>
  |
  constant '^~' constant  
  <<const_neor_const>>
  |
  constant '~^' constant  
  <<const_neor_const>>
  |
  constant '&&' constant  
  <<const_land_const>>
  |
  constant '||' constant  
  <<const_lor_const>>
  |
  constant '>>' constant  
  <<const_rshift_const>>
  |
  constant '>>>' constant 
  <<const_urshift_const>>
  |
  constant '<<' constant  
  <<const_lshift_const>>
  |
  constant '<' constant   
  <<const_lt_const>>
  |
  constant '>' constant   
  <<const_gt_const>>
  |
  constant '==' constant  
  <<const_eqeq_const>>
  |
  constant '<=' constant  
  <<const_le_const>>
  |
  constant '>=' constant  
  <<const_ge_const>>
  |
  constant '!=' constant  
  <<const_ne_const>>
  |
  constant '===' constant 
  <<const_eqeqeq_const>>
  |
  constant '!==' constant 
  <<const_neqeq_const>>
  |
  constant '=?=' constant 
  <<const_eqQeq_const>>
  |
  constant '!?=' constant 
  <<const_Qne_const>>
  |
  constant '==?' constant 
  <<const_eqeqQ_const>>
  |
  constant '?==' constant 
  <<const_Qeqeq_const>>
  |
  '-' constant
  <<u_minus_const>>
  |
  '~' constant
  <<u_tilde_const>>
  |
  '!' constant
  <<u_not_const>>
  |
  '&' constant
  <<u_and_const>>
  |
  '|' constant
  <<u_or_const>>
  |
  '^' constant
  <<u_eor_const>>
  |
  '~&' constant
  <<u_nand_const>>
  |
  '&~' constant
  <<u_nand_const>>
  |
  '~|' constant
  <<u_nor_const>>
  |
  '|~' constant
  <<u_nor_const>>
  |
  '^~' constant 
  <<u_neor_const>>
  |
  '~^' constant 
  <<u_neor_const>>
  |
  constant '=:=' constant ':' constant 
  <<mask_comp_const>>
  |
  constant '?' constant ':' constant 
  <<conditional_const>>
  |
  '(' constant ')' 
  <<DDeqD2>>
  |
  constant_concatenation
  <<DDeqD1>>
  ;

constant_concatenation:
  '{' constant_list '}' 
  <<concatenate_const>>
  |
  '{' constant '{' constant '}' '}'
  <<duplicate_const>>
  ;
  
constant_list:
  constant
  <<DDeqD1>>
  |
  constant_list ',' constant
  <<Expression_D1_NEXTeqD3>>
  ;

/***********************************
 *  string constant
 ***********************************/
  
string_constant:
  String_literal
  <<string_expression>>
  |
  '{' string_list '}' 
  <<concatenate_string>>
  |
  '{' constant '{' string_constant '}' '}'
  <<duplicate_string>>
  |
  constant '?' string_constant ':' string_constant 
  <<conditional_const>>
  |
  '(' string_constant ')' 
  <<DDeqD2>>
  ;

string_list:
  string_constant
  <<DDeqD1>>
  |
  string_list ',' string_constant
  <<Expression_D1_NEXTeqD3>>
    
#END

/***********************************
 *  expression
 ***********************************/
#BEGIN expression

expression:
  number
  <<DDeqD1>>
  |
  'return'
  <<return_expression>>
  |
  string_constant
  <<DDeqD1>>
  |
  timed_expression 
  <<DDeqD1>>
  |
  parallel_eval_expression
  <<DDeqD1>>
  |
  varport_expression
  <<varport_expression>>
  |
  bool_null_expression
  <<DDeqD1>>
  |
  function_call_expression
  <<DDeqD1>>
  |
  state_expression
  <<DDeqD1>>
  |
  concatenation_expression
  <<DDeqD1>>
  |
  list_expression
  <<DDeqD1>>
  |
  expression '-' expression   
  <<exp_minus_exp>>
  |
  expression '+' expression   
  <<exp_plus_exp>>
  |
  expression '*' expression   
  <<exp_times_exp>>
  |
  expression '/' expression   
  <<exp_div_exp>>
  |
  expression '%' expression   
  <<exp_mod_exp>>
  |
  expression '&' expression   
  <<exp_and_exp>>
  |
  expression '|' expression   
  <<exp_or_exp>>
  |
  expression '^' expression   
  <<exp_eor_exp>>
  |
  expression '&~' expression  
  <<exp_nand_exp>>
  |
  expression '~&' expression  
  <<exp_nand_exp>>
  |
  expression '|~' expression  
  <<exp_nor_exp>>
  |
  expression '~|' expression  
  <<exp_nor_exp>>
  |
  expression '^~' expression  
  <<exp_neor_exp>>
  |
  expression '~^' expression  
  <<exp_neor_exp>>
  |
  expression '&&' expression  
  <<exp_land_exp>>
  |
  expression '||' expression  
  <<exp_lor_exp>>
  |
  expression '>>' expression  
  <<exp_rshift_exp>>
  |
  expression '>>>' expression 
  <<exp_urshift_exp>>
  |
  expression '<<' expression  
  <<exp_lshift_exp>>
  |
  expression '<' expression   
  <<exp_lt_exp>>
  |
  expression '>' expression   
  <<exp_gt_exp>>
  |
  expression '==' expression  
  <<exp_eqeq_exp>>
  |
  expression '<=' expression  
  <<exp_le_exp>>
  |
  expression '>=' expression  
  <<exp_ge_exp>>
  |
  expression '!=' expression  
  <<exp_ne_exp>>
  |
  expression '===' expression 
  <<exp_eqeqeq_exp>>
  |
  expression '!==' expression 
  <<exp_neqeq_exp>>
  |
  expression '=?=' expression 
  <<exp_eqQeq_exp>>
  |
  expression '!?=' expression 
  <<exp_Qne_exp>>
  |
  expression '==?' expression 
  <<exp_eqeqQ_exp>>
  |
  expression '?==' expression 
  <<exp_Qeqeq_exp>>
  |
  '-' expression
  <<u_minus_exp>>
  |
  '~' expression
  <<u_tilde_exp>>
  |
  '!' expression
  <<u_not_exp>>
  |
  '&' expression
  <<u_and_exp>>
  |
  '|' expression
  <<u_or_exp>>
  |
  '^' expression
  <<u_eor_exp>>
  |
  '~&' expression
  <<u_nand_exp>>
  |
  '&~' expression
  <<u_nand_exp>>
  |
  '~|' expression
  <<u_nor_exp>>
  |
  '|~' expression
  <<u_nor_exp>>
  |
  '^~' expression 
  <<u_neor_exp>>
  |
  '~^' expression 
  <<u_neor_exp>>
  |
  expression '=:=' expression ':' expression 
  <<eqCeq_exp>>
  |
  expression '?' expression ':' expression 
  <<conditional_exp>>
  |
  '(' expression ')'
  <<DDeqD2>>
  |
  static_random
  <<DDeqD1>>
  ;


opt_expression:
  /* empty */
  <<DDeqNULL>>
  |
  expression
  <<DDeqD1>>
  ;

#END

/***********************************
 *  static random
 ***********************************/
  
static_random:
  'RND' '('  opt_rand_args ')'
  <<static_rnd>>
  |
  'CYC' '('  opt_rand_args ')'
  <<static_cyc>>
  ;

opt_rand_args:
  /* empty */
  <<DDeqNULL>>
  |
  rand_args
  <<DDeqD1>>
  ;
  
rand_args:
  rand_arg
  <<DDeqD1>>
  |
  rand_args ',' rand_arg
  <<Expression_D1_NEXTeqD3>>
  ;
  
rand_arg:
  rand_range
  <<DDeqD1>>
  |
  rand_parm
  <<DDeqD1>>
  |
  rand_cond 
  <<DDeqD1>>
  ;
  
rand_range:
  '{' 
    rand_values
  '}'
  <<DDeqD2>>
  ;
  
rand_values:
  rand_value
  <<DDeqD1>>
  |
  rand_values ',' rand_value
  <<rand_values>>
  ;
  
rand_value:
  rand_val
  <<DDeqD1>>
  |
  rand_val '@' constant
  <<rand_value_single>>
  |
  rand_val '@@' constant
  <<rand_value_double>>
  ;

rand_val:
  rnd_constant
  <<rand_val>>
  |
  rnd_constant ':' rnd_constant
  <<rand_range>>
  ;

rnd_constant:
  Decimal_num
  <<decimal_num>>
  |
  V_decimal_num
  <<v_decimal_num>>
  |
  C_decimal_num
  <<c_decimal_num>>
  |
  V_hex_num
  <<v_hex_num>>
  |
  C_hex_num
  <<c_hex_num>>
  |
  V_octal_num
  <<v_octal_num>>
  |
  C_octal_num
  <<c_octal_num>>
  |
  V_bin_num
  <<v_bin_num>>
  |
  C_bin_num
  <<c_bin_num>>
  ;

rnd_string:
  String_literal
  <<string_expression>>
  ;
    
rand_parm:
  Identifier '=' rnd_constant
  <<rand_parm>>
  |
  Identifier '=' rnd_string
  <<rand_parm>>
  ;

rand_cond:
  '<' expression 
  <<rand_cond_lt>>
  |
  '>' expression 
  <<rand_cond_gt>>
  |
  '<=' expression 
  <<rand_cond_le>>
  |
  '>=' expression 
  <<rand_cond_ge>>
  |
  '!=' expression 
  <<rand_cond_ne>>
  |
  '=?=' expression 
  <<rand_cond_Qeq>>
  |
  '!?=' expression 
  <<rand_cond_Qne>>
  ;

/***********************************
 *  timed expression
 ***********************************/
#BEGIN timed_expression
 
timed_expression:
  time_range '(' expression_list ')'
  <<timed_expression>>
  ;

time_range:
  '@' expression opt_clock_definition 
  <<time_range_a>>
  |
  '@' opt_expression ',' expression opt_clock_definition
  <<time_range_b>> 
  |
  '@@' expression opt_clock_definition 
  <<time_range_c>>
  |
  '@@' opt_expression ',' expression opt_clock_definition 
  <<time_range_d>>
  ;

#END
    
#BEGIN parallel_eval_expression

parallel_eval_expression:
  parallel_primitive '(' timed_expression_list ')' 
  <<parallel_eval_expression>>
  ;
  
parallel_primitive:
  'p_and' 
  <<p_and>>
  |
  'p_or' 
  <<p_or>>
  ;

timed_expression_list:
  timed_expression
  <<DDeqD1>>
  |
  timed_expression_list ',' timed_expression
  <<Expression_D1_NEXTeqD3>>
  ;

#END

/***********************************
 *  boolean, null
 ***********************************/
#BEGIN bool_null_expression

bool_null_expression:
  'null' 
  <<null_expression>>
  |
  'true'
  <<true_expression>>
  |
  'false'
  <<false_expression>>
  ;

#END

/***********************************
 *  state expression ++ --
 ***********************************/
#BEGIN state_expression
  
state_expression:
  varport_expression '++' 
  <<post_increment>>
  |
  varport_expression '--' 
  <<post_decrement>>
  |
  '++' varport_expression 
  <<pre_increment>>
  |
  '--' varport_expression  
  <<pre_decrement>>
  ;

#END

/***********************************
 *  concatenation
 ***********************************/
#BEGIN concatenation_expression
concatenation_expression:
  '{' expression_list '}' 
  <<concatenation_expression>>
  |
  '{' expression '{' expression '}' '}'
  <<duplicate_expression>>
  ;

#END

/************************************
 * list expression
 ************************************/
#BEGIN list_expression
list_expression:
  '\'' '{' expression_list '}' 
  <<list_expression>>
  ;

#END

/***********************************
 *  variable or port expression
 ***********************************/
#BEGIN varport_expression

varport_expression:
  a_ver_expression
  <<DDeqD1>>
  |
  varport_expression '.' Identifier opt_depth opt_access_range
  <<var_dot_var>>
  |
  varport_expression '.' 'super'
  <<var_dot_super>>
  ;
  
a_ver_expression:
  Identifier opt_depth opt_access_range
  <<var_expression>>
  |
  'super'
  <<super>>
  |
  'this'
  <<this>>
  ;

  
opt_depth:
  /* empty */
  <<DDeqNULL>>
  |
  Real_or_Depth
  <<dot_depth>>
  ;
  
opt_access_range:
  /* empty */
  <<DDeqNULL>>
  |
  access_range
  <<DDeqD1>>
  ;

access_range:
  array_access_ranges
  <<array_access_ranges>>
  |
  array_access_ranges bit_access_range
  <<array_access_range_bit_access_range>>
  |
  bit_access_range
  <<DDeqD1>>
  ;
  
bit_access_range:
  '[' expression ':' expression ']'
  <<bit_access_range>>
  ;
  
array_access_ranges:
  array_access_range
  <<DDeqD1>>
  |
  array_access_ranges array_access_range
  <<Expression_D1_NEXTeqD2>>
  ;
  
array_access_range:
  '[' expression ']'  
  <<array_access_range>>
  ; 
 
#END

/***********************************
 *  function call
 ***********************************/
#BEGIN function_call
  
function_call_expression:
  varport_expression '(' opt_actual_arguments ')'  
  <<func_call_exp>>
  ;

opt_actual_arguments:
  /* empty */
  <<DDeqNULL>>
  |
  actual_arguments
  <<DDeqD1>>
  ;
  
actual_arguments:
  actual_argument 
  <<DDeqD1>>
  |
  actual_arguments ',' actual_argument
  <<Expression_D1_NEXTeqD3>>
  ;
  
actual_argument:
  expression 
  <<DDeqD1>>
  |
  '*'
  <<default_arg>>
  ;

/***********************************
 *  new
 ***********************************/
#BEGIN new_expression
new_expression:
  'new' opt_new_args opt_new_object 
  <<new_expression>>
  ;

opt_new_args:
  /* empty */
  <<DDeqNULL>>
  |
  '(' ')'
  <<DDeqNULL>>
  |
  '(' actual_arguments ')'
  <<DDeqD2>>
  ;
  
opt_new_object:
  /* empty */
  <<DDeqNULL>>
  |
  left_expression 
  <<DDeqD1>>
  |
  block  /* this is for thread new */
  <<new_block>>
  ;

#END
/***********************************
 *  assign expression ( not a part of expression)
 ***********************************/
#BEGIN assign_expression

assign_expression:
  left_expression assign_opr expression  
  <<assign_expression>>
  |
  left_expression '=' new_expression
  <<assign_new_expression>>
  |
  list_expression '=' expression
  <<assign_list_expression>>
  ;
  
left_expression:
  varport_expression 
  <<DDeqD1>>
  ;
  
assign_opr:
  '=' 
  <<assign_eq>>
  |
  '<=' 
  <<assign_nb_eq>>
  |
  '+=' 
  <<assign_plus_eq>>
  |
  '-=' 
  <<assign_minus_eq>>
  |
  '*=' 
  <<assign_times_eq>>
  |
  '/=' 
  <<assign_div_eq>>
  |
  '%='  
  <<assign_mod_eq>>
  |
  '>>=' 
  <<assign_rshif_eq>>
  |
  '>>>=' 
  <<assign_urshift_eq>>
  |
  '<<=' 
  <<assign_lshift_eq>>
  |
  '&=' 
  <<assign_and_eq>>
  |
  '|=' 
  <<assign_or_eq>>
  |
  '^=' 
  <<assign_eor_eq>>
  |
  '~&=' 
  <<assign_nand_eq>>
  |
  '&~=' 
  <<assign_nand_eq>>
  |
  '~|=' 
  <<assign_nor_eq>>
  |
  '|~='
  <<assign_nor_eq>>
  |
  '~^=' 
  <<assign_neor_eq>>
  |
  '^~=' 
  <<assign_neor_eq>>
  ;

#END

semicolon:
  ';' 
  <<semicolon>>
  ;

    
%%
