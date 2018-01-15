/*
     Jeda Compiler Error Messages
 */

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
#define MAX_ERROR_MESSAGE  1024 

extern char *error_message ;

/* message for lex */
#define LEX_ERROR_RETURN_FOUND_IN_STRING \
  "Lex error: <CR> found in string at line %d of %s\n"


/* messages used in parse actions */
#define ERROR_ILLEGAL_EXPRESSION_FOR_CLOCK_DEFINITION \
  yyerror( "Illegal expression for clock source" )
#define ERROR_ILLEGAL_NAME_DETECTED_ON_PASS1(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' on compile pass 1", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_DETECTED_ON_PASS2(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' on compile pass 2", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_FOR_USER_DATA_TYPE(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for user data type", x \
  ) ; \
  yyerror( error_message )
#define ERROR_BREAK_LABEL_CANT_BE_FOUND(x) \
  sprintf( \
    error_message, "break label \'%s\' not defined", x \
  ) ; \
  yyerror( error_message )
#define ERROR_BREAK_STATEMENT_ON_OUT_OF_LOOP \
  yyerror( "break statement out of loop" )
#define ERROR_BROKEN_NAME_TREE_ON_PASS1 \
  yyerror( "broken name tree structure on pass1" )
#define ERROR_CONTINUE_LABEL_CANT_BE_FOUND(x) \
  sprintf( \
    error_message, "continue label %s not defined", x \
  ) ; \
  yyerror( error_message )
#define ERROR_CONTINUE_STATEMENT_ON_OUT_OF_LOOP \
  yyerror( "continue statement out of loop" )
#define ERROR_DUPLICATED_NAME(x) \
  sprintf( \
    error_message, "duplicated name \'%s\' used", x \
  ) ; \
  yyerror( error_message )
#define ERROR_GOT_NEW_CLASS_TYPE_IN_PASS1 \
  yyerror( "got new class type find in pass1, broken name tree" )
#define ERROR_GOT_NEW_ENUM_TYPE_IN_PASS1 \
  yyerror( "got new enum type in pass1, broken name tree" )
#define ERROR_ILLEGAL_RETURN_EXPRESSION_ON_OUT_OF_ADVICE \
  yyerror( "Illegal 'return' expression used at outside of advice" )
#define ERROR_ILLEGAL_ACCESS_RANGE \
  yyerror( "Illegal access range" )
#define ERROR_ILLEGAL_ARRAY_INDEX \
  yyerror( "Illegal array index" )
#define ERROR_ILLEGAL_BIT_RANGE_INDEX \
  yyerror( "Illegal bit range index" )
#define ERROR_ILLEGAL_COMBINATION_OF_EXTERN_AND_LOCAL \
  yyerror( "Illegal combitation of extern and local attribute" )
#define ERROR_ILLEGAL_EXTERN_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal extern attribute usage" )
#define ERROR_ILLEGAL_FINAL_ATTRIBUTE_FOR_LOCAL_GLOBAL_VARIABLE \
  yyerror( "Illegal final attribute usage" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_FOR_CLASS_DECLARATION \
  yyerror( "Illegal final attribute usage" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal final attribute usage" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yyerror( "Illegal final attribute usage" )
#define ERROR_ILLEGAL_EXPORT_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yyerror( "Illegal export attribute usage" )
#define ERROR_ILLEGAL_INITIALIZATION_ON_ARRAY_VARIABLE \
  yyerror( "Illegal initialization on array-variable" )
#define ERROR_ILLEGAL_LOCAL_ATTRIBUTE_FOR_LOCAL_VARIABLE \
  yyerror( "Illegal local attribute usage" )
#define ERROR_ILLEGAL_LOCAL_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal local attribute usage" )
#define ERROR_ILLEGAL_LOCAL_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yyerror( "Illegal local attribute usage" )
#define ERROR_ILLEGAL_NAME_FOR_FUNCTION(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for function", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_FOR_MEMBER_FUNCTION(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for member function", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_FOR_USER_FUNCTION(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for user function", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_TYPE_FOR_VARIABLE_FUNCION(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for variable or function", x \
  ) ;  \
  yyerror( error_message )  
#define ERROR_ILLEGAL_NAME_TYPE_FOR_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for variable", x \
  ) ;  \
  yyerror( error_message )  
#define ERROR_UNKNOWN_DATA_TYPE_OF_VARIABLE(x) \
  sprintf( \
    error_message, "Unkown data type of variable name \'%s\', possible forward reference", x \
  ) ;  \
  yyerror( error_message )  
#define ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for class", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_USAGE_FOR_CLASS_EXTENTION(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for class extention", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NAME_USAGE_FOR_ENUM(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for enum", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_STATIC_ATTRIBUTE_FOR_LOCAL_GLOBAL_VARIABLE \
  yyerror( "Illegal static attribute usage" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_FOR_CLASS_DECLARATION \
  yyerror( "Illegal static attribute for class declaration" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal static attribute for variable declaration" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yyerror( "Illegal static attribute for verilog class declaration" )
#define ERROR_ILLEGAL_USAGE_OF_DEPTH \
  yyerror( "Illegal usage of depth" )
#define ERROR_ILLEGAL_STATIC_VARIABLE_DECLARATION_WITHIN_LOCAL_BLOCK \
  yyerror( "Illegal static variable declaration within a local block" )
#define ERROR_ILLEGAL_ABSTRUCT_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal abstract attribute for variable declaration" )
#define ERROR_ILLEGAL_EXPORT_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal export attribute for variable declaration" )
#define ERROR_ILLEGAL_ABSTRUCT_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yyerror( "Illegal abstract attribute for verilog class declaration" )
#define ERROR_INCONSISTENT_VERILOG_TASK_PATH(x) \
  sprintf( \
    error_message, "Inconsistent Verilog task path \"%s\" ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_MEMBER_FUNCTION_NOT_DEFINED(x) \
  sprintf( \
    error_message, "Member function \'%s\' not defined", x \
  ) ; \
  yyerror( error_message )
#define ERROR_MULTIPLE_DECLARATION_OF_FUNCTION_FOUND \
  yyerror( "More than one declaration of function" )
#define ERROR_MULTIPLE_DECLARATION_OF_ADVICE_FOUND \
  yyerror( "More than one declaration of advice" )
#define ERROR_NO_SUCH_SIGNAL_FOUND_IN_SIGSET(x,y) \
  sprintf( \
    error_message, "Port \'%s\' is not found in portset \'%s\'", x, y \
  ) ; \
  yyerror( error_message )
#define ERROR_NO_SUCH_SIGSET_DECLARED(x) \
  sprintf( \
    error_message, "Portset \'%s\' is not found", x \
  ) ; \
  yyerror( error_message )
#define ERROR_UNDEFINED_NAME(x) \
  sprintf( \
    error_message, "Undefined name \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_UNDEFINED_MEMBER_NAME(x) \
  sprintf( \
    error_message, "Undefined member name \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_ACCESS_TO_PROTECTED_MEMBER_NAME(x) \
  sprintf( \
    error_message, "Illegal access to local/protected member name \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_ACCESS_NON_STATIC_MEMBER_NAME_IN_CLASS_INIT(x) \
  sprintf( \
    error_message, "Illegal access to non-static member name in class initialization statement \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_REDEFINITION_OF_FINAL_MEMBER_NAME(x) \
  sprintf( \
    error_message, "Illegal redefinition to a final member name \'%s\' ", x \
  ) ; \
  yyerror( error_message )

#define ERROR_MISSING_ABSTRUCT_FUNCTION_DEFINITION(x) \
  sprintf( \
    error_message, "Missing abstract function \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_ABSTRUCT_FUNCTION_REDEFINITION(x) \
  sprintf( \
    error_message, "Illegal abstract function redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_ABSTRUCT_FUNCTION_TYPE_REDEFINITION(x) \
  sprintf( \
    error_message, "Illegal abstract function type redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ARG_MISMATCH_ON_ABSTRUCT_FUNCTION_REDEFINITION(x) \
  sprintf( \
    error_message, "Argument type mismatch on abstract function redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )

#define ERROR_ILLEGAL_OVERWRITE_FUNCTION_IN_NON_EXTENTED_CLASS \
  yyerror( "Illegal overwrite function in non-extended class" )

#define ERROR_MISSING_OVERWRITE_FUNCTION_IN_PARENT_CLASS(x) \
  sprintf( \
    error_message, "Missing overwrite function \'%s\' in parent class", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_OVERWRITE_FUNCTION_REDEFINITION(x) \
  sprintf( \
    error_message, "Illegal overwrite function redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_OVERWRITE_FUNCTION_TYPE_REDEFINITION(x) \
  sprintf( \
    error_message, "Illegal overwrite function type redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ARG_MISMATCH_ON_OVERWRITE_FUNCTION_REDEFINITION(x) \
  sprintf( \
    error_message, "Argument type mismatch on overwrite function redefinition \'%s\' ", x \
  ) ; \
  yyerror( error_message )


#define ERROR_EXPORT_FUNCTION_MUST_BE_VOID_TYPE \
  yyerror( "Export function must be void type" )

#define ERROR_VERILOG_CLASS_FUNCTION_MUST_BE_VOID_TYPE \
  yyerror( "Verilog class function must be void type" )
#define ERROR_VERILOG_CLASS_NOT_DEFINED \
  yyerror( "Verilog class not defined" )
#define ERROR_VERILOG_CLASS_NOT_FOUND \
  yyerror( "Verilog class not found" )
  
#define ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_REPEAT \
  yyerror( "Illegal expression type for repeat number" )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_WHILE_CONDITION \
  yyerror( "Illegal expression type for while condition" )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_FOR_CONDITION \
  yyerror( "Illegal expression type for for statement condition" )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_IF_CONDITION \
  yyerror( "Illegal expression type for if condition" )
#define ERROR_ILLEGAL_STRENGTH_USAGE_ON_NON_PORT_ASSIGNMENT \
  yyerror( "Illegal assign strength usage on non port type variable" )
#define ERROR_RETURN_DATA_TYPE_MISMATCH \
  yyerror( "Returning data type inconsistent with function type" )
#define ERROR_ILLEGAL_NAME_FOR_FUNCTION_CALL(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for function call", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_INITIALIZATION(x) \
  sprintf( \
    error_message, "Illegal data type for variable \'%s\' initialization", x \
  ) ; \
  yyerror( error_message )

#define WARNING_ILLEGAL_EXTERN_ATTRIB_FOR_VERILOG_CLASS_DECLARATION \
  yywarning( "Illegal extern attrib for verilog class declaration, ignored" ) 
#define WARNING_INVALID_LABEL_FOR_NON_LOOP_STATEMENT_IGNORED \
  yywarning( "Illegal label on non-loop statement, ignored" ) 

/* messages used in symtab.c */
#define ERROR_SCOPE_STACK_LINK_BROKEN \
  yyerror( "name scope stack link is broken" )
#define ERROR_VARIABLE_SCOPE_STACK_UNDERFLOW \
  yyerror( "variable scope stack underflow" )
#define ERROR_VARIABLE_SCOPE_STRUCTURE_IS_BROKEN \
  yyerror( "variable scope structure is broken" )

/* messages used in parse_tree.c */
#define ERROR_CLOCK_PORT_MUST_BE_INPUT_OR_INOUT \
  yyerror( "CLOCK port must be input or inout" )
#define ERROR_CLOCK_PORT_SHOULD_NOT_BE_VECTORED \
  yyerror( "CLOCK port should not be vectored" )
#define ERROR_DATA_TYPE_OF_PRE_DEFINED_FUNCTION_MISMATCH(x) \
  sprintf( \
    error_message, "data type mismatch on pre-defined function \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_DATA_TYPE_OF_PRE_DEFINED_ADVICE_MISMATCH(x) \
  sprintf( \
    error_message, "data type mismatch on pre-defined advice \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_NTH_POINTCUT_MISMATCH(x) \
  sprintf( \
    error_message, "pointcut (%d) mismatch from pre-defined advice", x \
  ) ; \
  yyerror( error_message )
#define ERROR_DEPTH_DEFINED_ON_CLOCK_PORT \
  yyerror( "depth defined on clock port" )
#define ERROR_DRIVECLOCK_DEFINED_ON_NON_CLOCK_PORT \
  yyerror( "driveclock on non-clock port" )
#define ERROR_DRIVE_EDGE_DEFINED_ON_CLOCK_PORT \
  yyerror( "drive edge defined on non-clock port" )
#define ERROR_FATAL_NO_MEMORY \
  yyerror( "NO More Memory!!" )
#define ERROR_FINAL_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH \
  yyerror( "pre-defined function attribute mismatch" )
#define ERROR_ILLEGAL_BIT_RANGE \
  yyerror( "illegal bit range" )
#define ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_EXTERN \
  yyerror( "illegal combination of local and extern attribute" )
#define ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_EXPORT \
  yyerror( "illegal combination of local and export attribute" )
#define ERROR_ILLEGAL_COMBINATION_OF_EXPORT_AND_EXTERN \
  yyerror( "illegal combination of export and extern attribute" )
#define ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_FINAL \
  yyerror( "illegal combination of local and final attribute" )
#define ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_OVERWRITE \
  yyerror( "illegal combination of local and overwrite attribute" )
#define ERROR_ILLEGAL_COMBINATION_OF_LOCAL_AND_PROTECTED \
  yyerror( "illegal combination of local and protected attribute" )
#define ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE_FOR_MEMBER_FUNCTION \
  yyerror( "illegal extern attribute usage" )
#define ERROR_ILLEGAL_EXTERN_ATTRIB_USAGE \
  yyerror( "illegal extern attribute usage" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_USAGE_FOR_FUNCTION \
  yyerror( "illegal final attribute usage" )
#define ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE_ON_ABSTRUCT_METHOD \
  yyerror( "illegal local attribute usage on abstract function" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_USAGE_ON_ABSTRUCT_METHOD \
  yyerror( "illegal final attribute usage on abstract function" )
#define ERROR_ILLEGAL_FINAL_ATTRIB_USAGE \
  yyerror( "illegal final attribute usage" )
#define ERROR_ILLEGAL_NUMBER_REPRESENTATION \
  yyerror( "illegal number representation" )
#define ERROR_ILLEGAL_PATH_FOR_NODE \
  yyerror( "illegal path for node" )
#define ERROR_ILLEGAL_PERIOD_USAGE_ON_INPUT \
  yyerror( "illegal period usage on input" )
#define ERROR_ILLEGAL_PROTECTED_ATTRIB_USAGE_FOR_FUNCTION \
  yyerror( "illegal protected attribute usage" )
#define ERROR_ILLEGAL_PROTECTED_ATTRIB_USAGE \
  yyerror( "illegal protected attribute usage" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_FUNCTION \
  yyerror( "illegal static attribute usage" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_USAGE \
  yyerror( "illegal static attribute usage" )
#define ERROR_ILLEGAL_STATIC_ATTRIB_USAGE_FOR_MEMBER_FUNCTION \
  yyerror( "illegal static attribute usage" )
#define ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE_FOR_MEMBER_FUNCTION \
  yyerror( "illegal export attribute usage" )
#define ERROR_ILLEGAL_USAGE_OF_BOTHDRIVE_ON_INPUT \
  yyerror( "illegal bothdrive attribute usage on input port" )
#define ERROR_ILLEGAL_USAGE_OF_DEPHT_ON_OUTPUT \
  yyerror( "illegal depth usage on output port" )
#define ERROR_ILLEGAL_USAGE_OF_INPUTSKEW_ON_OUTPUT \
  yyerror( "illegal inputskew usage on output port" )
#define ERROR_ILLEGAL_USAGE_OF_NEGDRIVE_ON_INPUT \
  yyerror( "illegal negdrive usage on input port" )
#define ERROR_ILLEGAL_USAGE_OF_OUTPUTSKEW_ON_INPUT \
  yyerror( "illegal output skew usage on input port" )
#define ERROR_ILLEGAL_USAGE_OF_POSDRIVE_ON_INPUT \
  yyerror( "illegal posdrive usage on input port" )
#define ERROR_ILLEGAL_USAGE_OF_POSSAMPLE_ON_OUTPUT \
  yyerror( "illegal possample usage on output port" )
#define ERROR_ILLEGAL_VALUE_FOR_DEPTH \
  yyerror( "illegal value for depth" )
#define ERROR_ILLEGAL_VALUE_FOR_INITIAL_VALUE_FOR_DRIVECLOCK \
  yyerror( "illegal initial value for drive clock" )
#define ERROR_ILLEGAL_VALUE_FOR_PERIOD \
  yyerror( "illegal value for period" )
#define ERROR_ILLEGAL_VALUE_FOR_INPUT_SKEW \
  yyerror( "illegal value for input skew" )
#define ERROR_INPUT_SKEW_MUST_BE_NEGATIVE_VALUE \
  yyerror( "input skew must be a negative value" )
#define ERROR_ILLEGAL_VALUE_FOR_OUTPUT_SKEW \
  yyerror( "illegal value for output skew" )
#define ERROR_ILLEGAL_VALUE_FOR_RET \
  yyerror( "illegal value for ret" )
#define ERROR_ILLEGAL_VALUE_FOR_START_DELAY_VALUE_FOR_DRIVECLOCK \
  yyerror( "illegal start delay value for driveclock" )
#define ERROR_ILLEGAL_VALUE_FOR_VCA \
  yyerror( "illegal value for vca" )
#define ERROR_ILLEGAL_VCA_ON_OUTPUT \
  yyerror( "illegal vca usage on output port" )
#define ERROR_ILLEGAL_ABSTRUCT_ATTRIB_USAGE \
  yyerror( "illegal abstract attribute usage" )
#define ERROR_ILLEGAL_OVERWRITE_ATTRIB_USAGE \
  yyerror( "illegal overwrite attribute usage" )
#define ERROR_ILLEGAL_ABSTRUCT_ATTRIB_USAGE_FOR_FUNCTION \
  yyerror( "illegal abstract attribute usage" )
#define ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE_FOR_FUNCTION \
  yyerror( "illegal local attribute usage" )
#define ERROR_ILLEGAL_EXPORT_ATTRIB_USAGE \
  yyerror( "illegal export attribute usage" )
#define ERROR_ILLEGAL_LOCAL_ATTRIB_USAGE \
  yyerror( "illegal local attribute usage" )
#define ERROR_ILLEGAL_ABSTRACT_ATTRIB_USAGE \
  yyerror( "illegal abstract attribute usage" )
#define ERROR_ILLEGAL_ABSTRUCT_METHOD_IN_NON_ABSTRUCT_CLASS \
  yyerror( "illegal abstract function declaration in non-abstract class" )
#define ERROR_INPUT_SKEW_DEFINED_ON_CLOCK_PORT \
  yyerror( "input skew defined on clock port" )
#define ERROR_INTERNAL_LABEL_STACK_OUT_OF_ORDER \
  yyerror( "internal label stack out of order" )
#define ERROR_LOCAL_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH \
  yyerror( "local attrib mismatch with pre-defined function" )
#define ERROR_MORE_THAN_ONE_DEPTH_ATTRIB_DETECTED \
  yyerror( "more than one depth attrib detected" )
#define ERROR_MORE_THAN_ONE_DRIVE_EDGE_DETECTED \
  yyerror( "more than one drive-edge attrib detected" )
#define ERROR_MORE_THAN_ONE_INPUTSKEW_ATTRIB_DETECTED \
  yyerror( "more than one inputskew attrib detected" )
#define ERROR_MORE_THAN_ONE_NODE_ATTRIB_DETECTED \
  yyerror( "more than one node attrib detected" )
#define ERROR_MORE_THAN_ONE_OUTPUTSKEW_ATTRIB_DETECTED \
  yyerror( "more than one outputskew attrib detected" )
#define ERROR_MORE_THAN_ONE_PERIOD_ATTRIB_DETECTED \
  yyerror( "more than one period attrib detected" )
#define ERROR_MORE_THAN_ONE_SAMPLE_EDGE_DETECTED \
  yyerror( "more than one sample-edge attrib detected" )
#define ERROR_MORE_THAN_ONE_VCA_ATTRIB_DETECTED \
  yyerror( "more than one vca-attrib attrib detected" )
#define ERROR_OUTPUT_SKEW_DEFINED_ON_CLOCK_PORT \
  yyerror( "output skew defined on clock port" )
#define ERROR_PROTECTED_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH \
  yyerror( "protected attrib mismatch with pre-defined function" )
#define ERROR_RETURN_VALUE_DEFINED_ON_CLOCK_PORT \
  yyerror( "ret attrib defined on clock port" )
#define ERROR_SAMPLE_EDGE_DEFINED_ON_CLOCK_PORT \
  yyerror( "sample-edge attribute defined on clock port" )
#define ERROR_UNKNOWN_RANGE \
  yyerror( "unknown range" )
#define ERROR_UNKNOWN_TYPE_FOR_SIG_ATTR \
  yyerror( "unknown type for port attribute" )
#define ERROR_VCA_DEFINED_ON_CLOCK_PORT \
  yyerror( "vca attribute defined on clock port" )
#define ERROR_ABSTRUCT_ATTRIB_OF_PRE_DEFINED_FUNCTION_MISMATCH \
  yyerror( "abstract attribute mismatch with pre-defined function" )

#define ERROR_PRE_DEFINED_FUNCTION_TYPE_MISMATCH \
  yyerror( "type mismatch with pre-defined function" )
#define ERROR_NUMBER_OF_ARG_MISMATCH_ON_PRE_DEFINED_FUNCTION \
  yyerror( "number of arguments mismatch with pre-defined function" )
#define ERROR_N_TH_ARGMENT_TYPE_MISMATCH_ON_PRE_DEFINED_FUNCTION(x) \
  sprintf( \
    error_message, "%d-th Argument type mismatch with pre-defined function", x \
  ) ; \
  yyerror( error_message )

#define ERROR_STRING_TYPE_ARRAY_RANGE_DETECTED_FOR_PORT_ACCESS(x) \
  sprintf( \
    error_message, "Illegal string access range on port \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_MULTI_DIMENTION_ACCESS_DETECTED_FOR_PORT_ACCESS(x) \
  sprintf( \
    error_message, "Illegal multi-dimension range on port \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_STRING_RANGE_ON_NON_STRING_INDEXED_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal string access range on non-string-indexed variable \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_ARRAY_RANGE_ON_STRING_INDEXED_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal access range on string-indexed variable \'%s\'", x \
  ) ; \
  yyerror( error_message )


#define ERROR_ILLEGAL_ARRAY_RANGE_ON_NON_ARRAY_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal array range on non-array variable \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_BIT_RANGE_ON_NON_BIT_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal bit range on non-bit variable \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_BIT_RANGE_ON_PORT_WITHOUT_DEPTH(x) \
  sprintf( \
    error_message, "Illegal bit range on port without depth \'%s\', accessing data requires .N expression", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_BIT_RANGE_ON_ARRAY_VARIABLE(x) \
  sprintf( \
    error_message, "Illegal bit range on array variable \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_MISSING_ACCESS_RANGE_ON_ARRAY_VARIABLE(x) \
  sprintf( \
    error_message, "Missing array range for array variable \'%s\'", x \
  ) ; \
  yyerror( error_message )
#define ERROR_ARRAY_RANGE_DIMENTION_MISMATCH_ON_VARIABLE(x) \
  sprintf( \
    error_message, "Array range dimention mismatch for variable \'%s\'", x \
  ) ; \
  yyerror( error_message )

#define ERROR_ILLEGAL_DATA_TYPE_ON_NTH_CONDITION_FOR_CASE_STATEMENT(x) \
  sprintf( \
    error_message, "%d-th condition type mismatch for case statement", x \
  ) ; \
  yyerror( error_message )
#define ERROR_NEW_OBJECT_TYPE_ERROR \
  yyerror( "Data type error for new object" )
#define ERROR_ILLEGAL_NEW_FUNCTION_CALL_TO_ABSTRUCT_CLASS \
  yyerror( "Illegal new function call on abstract class" )
#define ERROR_NUMBER_OF_ARGUMENT_MISMATCH_ON_NEW_FUNCTION \
  yyerror( "Number of argument mismatch on new() function" )

#define ERROR_ILLEGAL_SUPER_NEW_CALL \
  yyerror( "Illegal super.new call in non-extended class" )

#define ERROR_SUPER_NEW_CALL_REQUIRES_ARGMENTS \
  yyerror( "Implicit Super.new() call failed, new() requires argument" )

#define ERROR_ILLEGAL_EXTERN_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal extern attribute usage" )
#define ERROR_ILLEGAL_GLOBAL_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal global attibute usage" )
#define ERROR_ILLEGAL_CLONE_ATTRIB_FOR_VARIABLE_DECLARATION \
  yyerror( "Illegal clone attibute usage" )
#define ERROR_ILLEGAL_COMBINATION_OF_VARIABLE_ATTRIBUTE \
  yyerror( "Illegal combination of variable attribute" )


#define WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE \
  yywarning( "WARNING: decimal constant overflow" ) 
#define WARNING_NO_DRIVE_EDGE_ASSUME_ASYNC \
  yywarning( "WARNING: no drive edge specified on output port, set as async drive" ) 
#define WARNING_NO_SAMPLING_EDGE_ASSUME_ASYNC \
  yywarning( "WARNING: no sample edge on input port, set as async sample" ) 

#define ERROR_PORT_WITHOUT_DEPTH_USED_IN_LEFT_SIDE_OF_ASSIGN(x) \
  sprintf( \
    error_message, "port %s without depth is used on left side of assignment", \
    x \
  ) ; \
  yyerror( error_message ) 


/* messages in expression checking */
#define ERROR_ILLEGAL_EXPRESSION_FOR_NUMERICAL_OPERATION \
  yyerror( "Illegal expression for numerical operation" )
#define ERROR_ILLEGAL_NTH_EXPRESSION_FOR_CONCATENATION_OPERATION(x) \
  sprintf( \
    error_message, "%d-th expression is illegal for concatenation operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT \
  yyerror( "Numerical expression is expected for 1st element of the concatenation" )
#define ERROR_NUMERICAL_OR_STRING_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT \
  yyerror( "Numerical or string expression is expected for 2nd element of the concatenation" )
#define ERROR_ARRAY_OR_LIST_CANT_BE_AN_ELEMENT_OF_LIST(x) \
  sprintf( \
    error_message, \
    "Array or list expression detected on %d-th element, can't be used for list construction", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_TYPE_DETECTED_ON_NTH_ELEMENT_FOR_LIST_OPERATION(x) \
  sprintf( \
    error_message, \
    "Illegal data type is detected on %d-th expression for list operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_1ST_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Numerical expression is expected for 1st element of %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_2ND_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Numerical expression is expected for 2nd element of %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_3RD_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Numerical expression is expected for 2rd element of %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_DATA_TYPE_ON_1ST_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Illegal 1st element for binary %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_DATA_TYPE_ON_2ND_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Illegal 2nd element for binary %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_EXPRESSION_TYPE_COMBINATION_FOR_(x) \
  sprintf( \
    error_message, \
    "Illegal expression type combination for %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_NUMERICAL_EXPRESSION_IS_EXPECTED_FOR_UNARY(x) \
  sprintf( \
    error_message, \
    "Illegal expression type for unary %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_EXPRESSION_TYPE_FOR_UNARY(x) \
  sprintf( \
    error_message, \
    "Illegal expression type for unary %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_EXPRESSION_FOR_1ST_ELEMENT_OF_(x) \
  sprintf( \
    error_message, \
    "Illegal expression type for 1st element of %s operation", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_EXPRESSION_COMBINATION_FOR_(x) \
  sprintf( \
    error_message, \
    "Illegal expression type combination for %s operation", \
    x \
  ) ; \
  yyerror( error_message )

#define ERROR_ILLEGAL_SUPER_USAGE_ON_OUT_OF_CLASS \
  yyerror( "Illegal \'super\' usage at outside of class" ) 
#define ERROR_ILLEGAL_THIS_USAGE_ON_OUT_OF_CLASS \
  yyerror( "Illegal \'this\' usage at outside of class" ) 
#define ERROR_ILLEGAL_USAGE_OF_THIS_IN_ASPECT_FUNCTION \
  yyerror( "Illegal \'this\' usage in function of aspect" ) 
#define ERROR_ILLEGAL_THIS_USAGE_IN_ADVACE_WITH_UNKNOWN_CLASS \
  yyerror( "Illegal \'this\' usage in advice with undetermined class" ) 
#define ERROR_ILLEGAL_SUPER_USAGE_ON_CLASS_WITHOUT_PARENT \
  yyerror( "Illegal \'super\' usage, no super class defined" ) 
#define ERROR_ILLEGAL_SUPER_USAGE_ON_ARRAY_OR_LIST \
  yyerror( "Illegal \'super\' usag on array/list data" ) 

#define ERROR_ILLEGAL_DATA_TYPE_FOR_MULTIDIMENTION_ARRAY_ACCESS_RANGE(x) \
  sprintf( \
    error_message, \
    "Data type error detected on %d-th index fot multi-dimention array", \
    x \
  ) ; \
  yyerror( error_message )

#define ERROR_ILLEGAL_DATA_TYPE_FOR_ARRAY_OR_BIT_ACCESS_RANGE \
  yyerror( "Illegal data type for array or bit access index" )

#define ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGNMENT \
  yyerror( "Illegal data type combination for assignment" )

#define ERROR_ILLEGAL_LIST_ACCESS_FOR_ASSIGNMENT \
  yyerror( "Illegal direct list accesss for left hand of assignment" )

#define ERROR_ILLEGAL_DATA_TYPE_FOR_ARITHMETIC_ASSIGNMENT \
  yyerror( "Illegal data type for arithmetic assignment" )

#define ERROR_ILLEGAL_MIXTURE_OF_ABSTRUCT_ARGUMENT \
  yyerror( "Illegal mixture of abstract argument and regular argument" )
#define ERROR_ILLEGAL_USAGE_OF_ABSTRUCT_ARGUMENT \
  yyerror( "Illegal usage of abstract argument" )
#define ERROR_ILLEGAL_ABSTRUCT_FUNCTION_WITH_ACTUAL_BLOCK \
  yyerror( "Illegal abstract function with actual block" )
  
#define ERROR_DATA_TYPE_ERROR_DETECTED_ON_NTH_ARG(x) \
  sprintf( \
    error_message, \
    "Data type error detected on %d-th argument", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NTH_ARG_TYPE_FOR_EXPORT_FUNCTION(x) \
  sprintf( \
    error_message, \
    "Illegal data type for export function on %d-th argument", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_NTH_ARG_TYPE_FOR_VERILOG_TASK(x) \
  sprintf( \
    error_message, \
    "Illegal data type for Verilog task on %d-th argument", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_TOO_MANY_ARGUMENTS \
  yyerror( "Too many arguments are specified" )
#define ERROR_TOO_FEW_ARGUMENTS \
  yyerror( "Too few argument(s)" )

#define ERROR_TOO_FEW_ARGUMENTS_ON_IMPLISIT_SUPER_NEW_CALL \
  yyerror( "Too few argument(s) for implisit super.new() call" )

#define ERROR_VECTORED_SIGNAL_IS_USED_FOR_CLOCK(x) \
  sprintf( \
    error_message, \
    "Vectored signal %s is used for the clock", \
    x \
  ) ; \
  yyerror( error_message )

#define ERROR_UNKNOWN_PORT_NAME(x) \
  sprintf( \
    error_message, \
    "Unknown port name %s", \
    x \
  ) ; \
  yyerror( error_message )

#define ERROR_UNKNOWN_PORT_SET_NAME(x) \
  sprintf( \
    error_message, \
    "Unknown port_set name %s", \
    x \
  ) ; \
  yyerror( error_message )


#define ERROR_ILLEGAL_NAME_USAGE_FOR_ASPECT(x) \
  sprintf( \
    error_message, "Illegal name \'%s\' for aspect", x \
  ) ; \
  yyerror( error_message )

/* messages used in constant.c */
#define ERROR_ILLEGAL_OPERAND_TYPE_FOR_SHIFT_OPERATION \
  yyerror( "illegal operand type for shift operation" )
#define ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_TOO_LARGE_VALUE \
  yyerror( "constant calculation failed due to too large value" )
#define ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE \
  yyerror( "constant calculation failed due to X or Z in operand" )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION \
  yyerror( "Illegal data type for constant concatenation" )
#define ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION \
  yyerror( "Illegal operand type for bit-width logic operation" )
#define ERROR_ILLEGAL_OPERAND_TYPE_FOR_MODULO_OPERATION \
  yyerror( "Illegal operand type for modulo operation" )
#define ERROR_ILLEGAL_UNARY_BIT_OPERATION_ON_REAL_VALUE \
  yyerror( "Illegal unary bit operation on real value" )
#define ERROR_WRONG_DATA_TYPE_FOR_MASK_COMPARE_CONSTANT \
  yyerror( "wrong data type for masked compare constant" )
#define ERROR_ZERO_DIVIDE_ON_CONSTANT_CALCULATION \
  yyerror( "divide by zero on constant calculation" )

#define ERROR_ILLEGAL_DATA_TYPE_ON_NTH_ARG_FOR_PARALLEL_COND(x) \
  sprintf( \
    error_message, \
    "Illegal data type detected on %d-th element of parallel condition", \
    x \
  ) ; \
  yyerror( error_message )

#define ERROR_ILLEGAL_MAIN_DEFINITION_WITH_ARGUMENT \
  yyerror( "Illegal main() definition with argument" )


#define ERROR_MORE_THAN_ONE_RAND_RANGE_DECLARED\
  yyerror( "more than one RND range declared" )
#define ERROR_MORE_THAN_ONE_RAND_SIZE_DECLARED\
  yyerror( "more than one RND size declared" )
#define ERROR_MORE_THAN_ONE_RAND_LIMIT_DECLARED\
  yyerror( "more than one RND limit declared" )
#define ERROR_MORE_THAN_ONE_NAME_DECLARED\
  yyerror( "more than one RND name declared" )
#define ERROR_ILLEGAL_USAGE_OF_WEIGHT_IN_CYC\
  yyerror( "Illegal usage of weight in CYC()" )
#define ERROR_INCONSISTENT_WEIGHT_DECLARATION\
  yyerror( "Inconsistent weight declaration" )
#define ERROR_ILLEGAL_WEIGHT\
  yyerror( "Illegal weight" )
#define ERROR_ILLEGAL_CONSTANT_TYPE\
  yyerror( "Illegal constant type" )
#define ERROR_ILLEGAL_RND_SIZE\
  yyerror( "Illegal RND/CYC size" )
#define ERROR_CYC_SIZE_TOO_LARGE_MAX_96\
  yyerror( "CYC size too large, max 96 bit" )
#define ERROR_ILLEGAL_DATA_TYPE_FOR_RND_COND\
  yyerror( "Illegal data type for RND/CYC constraint" )
#define ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32\
  yyerror( "RND/CYC range too large, max 2^^32" )
#define ERROR_ILLEGAL_CONSTANT_TYPE_FOR_RANDOM_RANGE\
  yyerror( "Illegal constant type for RND range" )
#define ERROR_NAMED_RND_WITH_DIFFERENT_RANGE_DETECTED(x) \
  sprintf( \
    error_message, \
    "Named RND/CYC %s with different range detected", \
    x \
  ) ; \
  yyerror( error_message )
#define ERROR_ILLEGAL_RND_PARAMETER(x) \
  sprintf( \
    error_message, \
    "Illegal RND/CYC parameter %s detected", \
    x \
  ) ; \
  yyerror( error_message )

/* messages used in code generation */

