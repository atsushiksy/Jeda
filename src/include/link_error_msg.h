/* 
    Jeda Linker Error Messages
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
#define ERROR_PORT_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Port \'%s\' type mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_PORT_NAME_ATTRIB_MISMATCH(x) \
  sprintf( \
    error_message, "Port \'%s\' attribute mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_PORTSET_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Portset \'%s\' type mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_VARIABLE_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Variable \'%s\' type mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_VARIABLE_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Variable \'%s\' not found in name table", x \
  ) ; yyerror( error_message )
#define ERROR_VARIABLE_NAME_ATTRIB_MISMATCH(x) \
  sprintf( \
    error_message, "Variable \'%s\' attribute mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_ENUM_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Enum \'%s\' type mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_FUNCTION_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Function \'%s\' type mismatch to previous definition", x \
  ) ; yyerror( error_message )
#define ERROR_CLASS_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Class \'%s\' type mismatch to previous definition", x \
  ) ;  yyerror( error_message )
#define ERROR_ASPECT_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Aspect \'%s\' type mismatch to previous definition", x \
  ) ;  yyerror( error_message )
#define ERROR_ADVICE_NAME_TYPE_MISMATCH(x) \
  sprintf( \
    error_message, "Advice \'%s\' type mismatch to previous definition", x \
  ) ;  yyerror( error_message )

#define ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_VAR(x,y) \
  sprintf( \
    error_message, "Class \'%s\' variable \'%s\' body defined more than once", x, y \
  ) ;  yyerror( error_message )
#define ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_FUNC(x,y) \
  sprintf( \
    error_message, "Class \'%s\' function \'%s\' body defined more than once", x, y \
  ) ;  yyerror( error_message )
#define ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_FUNC(x,y) \
  sprintf( \
    error_message, "Aspect \'%s\' function \'%s\' body defined more than once", x, y \
  ) ;  yyerror( error_message )
#define ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_ADVICE(x,y) \
  sprintf( \
    error_message, "Aspect \'%s\' advice \'%s\' body defined more than once", x, y \
  ) ;  yyerror( error_message )
#define ERROR_DUPLICATED_DEFINITION_OF_FUNC(x) \
  sprintf( \
    error_message, "Function \'%s\' body defined more than once", x \
  ) ;  yyerror( error_message )

/* messages used in code generation */

#define ERROR_FATAL_NO_MEMORY \
  yyerror( "NO More Memory!!" ) ; exit(1) 

#define ERROR_ILLEGAL_NUMBER_REPRESENTATION \
  yyerror( "illegal number representation" )

#define WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE \
  yywarning( "WARNING: decimal constant overflow" )

#undef ERROR_LOCATION
#define ERROR_LOCATION(x,y)  


/* messaged uses in nametable.c */
#define WARNING_FUNCTION_BODY_NOT_FOUND(x) \
  sprintf( \
    error_message, "Function \'%s\' body not defined", x \
  ) ;  yywarning( error_message )
#define WARNING_CLASS_BODY_NOT_FOUND(x) \
  sprintf( \
    error_message, "Class \'%s\' body not defined", x \
  ) ;  yywarning( error_message )
#define WARNING_CLASS_MEMBER_BODY_NOT_FOUND(x,y) \
  sprintf( \
    error_message, "Class member \'%s.%s\' body not defined", x, y \
  ) ;  yywarning( error_message )


/* messaged uses in link_check.c */
#define ERROR_PORT_EDGE_ATTRIB_MISMATCH \
  yyerror( "Port edge attribute mismatch to previous definition" )
#define ERROR_PORT_SKEW_ATTRIB_MISMATCH \
  yyerror( "Port skew attribute mismatch to previous definition" )
#define ERROR_PORT_DEPTH_ATTRIB_MISMATCH \
  yyerror( "Port depth attribute mismatch to previous definition" )
#define ERROR_PORT_PATH_ATTRIB_MISMATCH \
  yyerror( "Port path attribute mismatch to previous definition" )
#define ERROR_PORT_CLOCK_ATTRIB_MISMATCH \
  yyerror( "Port clock attribute mismatch to previous definition" )
#define ERROR_PORT_NAME_NOT_FOUND_IN_PREV_PORTSET_DCL(x,y) \
  sprintf( \
    error_message, "Port %s not found in previous portset %s definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_PORTSET_PORT_NAME_ATTRIB_MISMATCH_IN_PREV_PORTSET_DCL(x,y) \
  sprintf( \
    error_message, "Port %s attribute mismatch to previous portset %s definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_NUMBER_OF_ENUM_MEMBER_MISMATCH( x ) \
  sprintf( \
    error_message, "Enum \'%s\' member mismatch", x \
  ) ;  yyerror( error_message )
#define ERROR_ENUM_MEMBER_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "enum %s member mismatch, this:\'%s\' previous: \'%s\'", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_FUNCTION_ATTRIBUTE_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "Function \'%s%s%s\' attribute mismatch to previous definition", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_DATA_TYPE_OF_FUNCTION_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "Function \'%s%s%s\' data type mismatch to previous definition", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_DATA_TYPE_OF_ADVICE_MISMATCH(x,y) \
  sprintf( \
    error_message, "Advice \'%s.%s\' data type mismatch to previous definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_NUMBER_OF_ARGUMENT_IN_FUNCTION_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "Number of argument for function \'%s%s%s\' mismatch to previous definition", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_NUMBER_OF_ARGUMENT_IN_ADVICE_MISMATCH(x,y) \
  sprintf( \
    error_message, "Number of argument for advice \'%s.%s\' mismatch to previous definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_NTH_ARGUMENT_VAR_TYPE_MISMATCH_IN_FUNCTION(n,x,y,z) \
  sprintf( \
    error_message, "Var attribute on %d-th argument for function \'%s%s%s\' mismatch to previous definition", n, x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_FUNCTION(n,x,y,z) \
  sprintf( \
    error_message, "Data type of %d-th argument for function \'%s%s%s\' mismatch to previous definition", n, x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_NTH_ARGUMENT_VAR_TYPE_MISMATCH_IN_ADVICE(n,x,y) \
  sprintf( \
    error_message, "Var attribute on %d-th argument for advice \'%s.%s\' mismatch to previous definition", n, x, y \
  ) ;  yyerror( error_message )
#define ERROR_NTH_ARGUMENT_DATA_TYPE_MISMATCH_IN_ADVICE(n,x,y) \
  sprintf( \
    error_message, "Data type of %d-th argument for advice \'%s.%s\' mismatch to previous definition", n, x, y \
  ) ;  yyerror( error_message )
#define ERROR_POINTCUT_IN_ADVICE_MISMATCH(x,y) \
  sprintf( \
    error_message, "Pointcut for advice \'%s.%s\' mismatch to previous definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_VERILOG_TASK_PATH_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "Verilog task %s path mismatch, this:%s previous: %s", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_MEMBER_TYPE_MISMATCH_IN_CLASS(x,y) \
  sprintf( \
    error_message, "Class \'%s\' member \'%s\' type mismatch to previous definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_ENUM_DEFINITION_MISMATCH_IN_CLASS(x,y) \
  sprintf( \
    error_message, "Enum \'%s\' definition in class \'%s\' mismatch to previous definition", y, x \
  ) ;  yyerror( error_message )
#define ERROR_VARIABLE_DEFINITION_MISMATCH_IN_CLASS(x,y) \
  sprintf( \
    error_message, "Class \'%s\' member variable \'%s\' type mismatch to previous definition", x, y \
  ) ;  yyerror( error_message )
#define ERROR_PARENT_CLASS_MISMATCH(x) \
  sprintf( \
    error_message, "Class \'%s\' extention mismatch to previous definition", x \
  ) ;  yyerror( error_message )
#define ERROR_CLASS_EXTENTION_NAME_MISMATCH(x,y,z) \
  sprintf( \
    error_message, "Class \'%s\' extention name mismatch to previous definition, this: \'%s\' previous:\'%s\'", x, y, z \
  ) ;  yyerror( error_message )
#define ERROR_CLASS_ATTRIB_MISMATCH(x) \
  sprintf( \
    error_message, "Class \'%s\' attribute mismatch to previous definition", x \
  ) ;  yyerror( error_message )



#define ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN \
  yyerror( "illegal data type for assign" )


#define ERROR_FUNC_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Function \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_CLASS_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Class \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_ASPECT_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Aspect \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_MEMBER_NAME_NOT_FOUND(x,y) \
  sprintf( \
    error_message, "Fatal, Member name \'%s.%s\' not found in the class", x, y \
  ) ; yyerror( error_message )

#define ERROR_MEMBER_FUNCTION_NOT_FOUND(x,y) \
  sprintf( \
    error_message, "Fatal, Member function \'%s.%s\' not found in the class", x, y \
  ) ; yyerror( error_message )

#define ERROR_PORT_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Port \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_PORT_SET_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Port_set \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET(x,y) \
  sprintf( \
    error_message, "Fatal, Port \'%s.%s\' not found in the port_set", x, y \
  ) ; yyerror( error_message )


#define ERROR_VERILOG_TASK_NAME_NOT_FOUND(x) \
  sprintf( \
    error_message, "Fatal, Verilog task \'%s\' not found in name table", x \
  ) ; yyerror( error_message )

#define ERROR_ILLEGAL_OVERWRITE_INDEX_FOR_MEMBER_FUNCTION(x,y) \
  sprintf( \
    error_message, "Fatal, Illegal index for overwriting \'%s.%s\' (may be protexted)", x, y \
  ) ; yyerror( error_message )

#define ERROR_ILLEGAL_OVERWRITE_INDEX_FOR_MEMBER_FUNCTION(x,y) \
  sprintf( \
    error_message, "Fatal, Illegal index for overwriting \'%s.%s\' (may be protexted)", x, y \
  ) ; yyerror( error_message )

#ifdef PRODUCT_CODE

#define ERROR_ILLEGAL_ACCESS_TO_GLOBAL_VARIABLE_IN_MODULE(x) \
  sprintf( \
    error_message, "Fatal, Illegal access to global variable \'%s\' in module compilation", x \
  ) ; yyerror( error_message )

#define ERROR_ILLEGAL_ACCESS_TO_GLOBAL_PORT_IN_MODULE(x) \
  sprintf( \
    error_message, "Fatal, Illegal access to global port \'%s\' in module compilation", x \
  ) ; yyerror( error_message )

#endif
