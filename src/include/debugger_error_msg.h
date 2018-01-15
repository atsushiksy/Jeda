/* 
  Debugger Error Messages
 */
/* 
   
   Copyright (C) 2001 Juniper Networks Inc.
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
#define DBG_ERROR JD_dbg_eval_error = 1 


#define DBG_ERROR_CANNOT_GO_UP_FUTHER \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not go up further\n" ) ; 
#define DBG_ERROR_CANNOT_GO_DOWN_FUTHER \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not go down further\n" ) ;

#define DBG_ERROR_UNKNOWN_FILE(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Unknown file \"%s\" \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_UNKNOWN_DIR(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Unknown dir \"%s\" \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_NAME_NOT_FOUND(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Unknown name \"%s\" \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_BREAKPOINT_CAN_NOT_BE_LOCATED_IN_FILE( x, y ) \
  PRINTF ( "JEDA DEBUGGER ERROR: Line number %d can not be located in \"%s\" \n", x, y ) ; \
  DBG_ERROR 

#define DGB_ERROR_ARRAY_ASSIGNMENT_IS_NOT_SUPPORTED \
  PRINTF ( "JEDA DEBUGGER ERROR: Direct array assignment is not supported \n" ) ; \
  DBG_ERROR 

#define GBG_ERROR_ILLEGAL_EXPRESSION_FOR_ASSIGN_COMMAND \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal expression for assign command \n" ) ; \
  DBG_ERROR 

#define GBG_ERROR_ILLEGAL_LEFT_EXPRESSION_FOR_ASSIGN_COMMAND \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal left expression for assign command \n" ) ; \
  DBG_ERROR 

#define GBG_ERROR_ILLEGAL_ASSIGN_EXPRESSION_FOR_PRINT_COMMAND \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal assign expression for print command \n" ) ; \
  DBG_ERROR 

#define DBG_ERROR_ILLEGAL_NAME_TYPE_FOR_ARRAY_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal name type for array access \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_DEPTH_USAGE_ON_NON_SIGNAL_EXP \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal depth usage on non-signal variable \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_SIGNAL_DEPTH_TOO_DEEP \
  PRINTF ( "JEDA DEBUGGER ERROR: Signal depth too deep \n" ) ; \
  DBG_ERROR 
  
#define DGB_ERROR_X_DETECTED_IN_INT_ASSIGNMENT \
  PRINTF ( "JEDA DEBUGGER ERROR: X detected in integer assignment \n" ) ; \
  DBG_ERROR 
#define DGB_ERROR_OVERFLOW_DETECTED_IN_INT_ASSIGNMENT \
  PRINTF ( "JEDA DEBUGGER ERROR: Overflow detected in integer assignment \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT \
  PRINTF ( "JEDA DEBUGGER ERROR: Data type mismatch on assignment \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_PORT_CAN_NOT_BE_REASSIGENED \
  PRINTF ( "JEDA DEBUGGER ERROR: Port cannot be reassigned \n" ) ; \
  DBG_ERROR 

#define DBG_ERROR_ACCESS_TO_NON_ACTIVE_SCOPE \
  PRINTF ( "JEDA DEBUGGER ERROR: Accessing name on a non active scope\n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ARRAY_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal array access\n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ARRAY_INDEX_X_TOO_LARGE_MAX_IS_Y(x,y) \
  PRINTF ( "JEDA DEBUGGER ERROR: Array index %d too large, must be less than %d \n", x, y ) ; \
  DBG_ERROR 
#define DBG_ERROR_BREAKPOINT_IS_NOT_SET(x,y) \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not set breakepoint at %s:%d \n", x, y ) ; \
  DBG_ERROR 
#define DBG_ERROR_CANNOT_FIND_THREAD_ID(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not find thread id %d \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_CANNOT_OPEN_FILE(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not open file \"%s\" \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_CANT_FIND_DIR_INFO_FOR_FILE(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Can not find directory information for file \"%s\" \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_CLASS_SCOPE_CANT_FOUND_IN_CURRENT_CONTEXT \
  PRINTF ( "JEDA DEBUGGER ERROR: Class scope can not be found in current conrext \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Data type name \"%s\" can not be found in the debugger information \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_DEBUGGER_EVAL_STACK_UNDERFLOW \
  PRINTF ( "JEDA DEBUGGER ERROR: Evaluation stack underflow \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_ARRAY_ELEMENT(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Array element [%d] is empty \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_DATA_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Empty data access \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_OBJECT_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Empty object access \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_SIGNAL_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Empty signal access \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_STRING_ARRAY_ELEMENT(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: String indexed array element [\"%s\"] is empty \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_EMPTY_LIST_ELEMENT(x,y) \
  PRINTF ( "JEDA DEBUGGER ERROR: index [%d] to list is too large, max [%d]\n", x, y ) ; \
  DBG_ERROR 
#define DBG_ERROR_EXPECTING_FILE_COLUMN_LINENUM(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal argument \"%s\" \n",x ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILEEGAL_MEMBER_ACCESS_ON_NON_SINGLE_VARIABLE(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal member \"%s\" access on non-single variable \n", x ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILEEGAL_OBJECT_TYPE_FOR_MEMBER_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal object type for member access \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_ARG_FOR_FILE_LOCATION(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal argument \"%s\" \n",x ) ; \
  DBG_ERROR 

#define DBG_ERROR_ILLEGAL_ARG_FOR_THREAD_COMMAND \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal argument for thread command \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_BIT_ACCESS_RANGE \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal bit access range \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_BIT_ACCESS_TO_NON_SINGLE_DATA \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal bit access to non-single data \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_DATA_TYPE_COMBINATION_FOR_BINARY_OPS \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type combination for binary ops \n" ) ; \
  DBG_ERROR 
#define DBG_ERROR_ILLEGAL_DATA_TYPE_DETECTED_WHILE_CONVERTING_TO_INT \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type detected while converting to int \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_FLOAT_CONVERSION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type for float conversion \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_NAME_FOR_EXPRESSION(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal name \"%s\" for expression \n", x ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_NUMBER_REPRESENTATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal number representation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal operand type for bitwidth logic operation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_MODULO_OPERATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal operand type for % operation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_SIGNAL_BINARY_OPERATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal operand type for signal binary operation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_STRING_BINARY_OPERATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal operand type for signal binary operation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_SIGNAL_POINTER_ACCESS_FOR_EXPRESSION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal signal pointer access for expression \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal subtype for array access \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_TYPE \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal type \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_MEMBER_INDEX_OUT_OF_BOUND(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Member \"%s\" index out of bound \n",x ) ; \
  DBG_ERROR
#define DBG_ERROR_MEMBER_NAME_NOT_FOUND_IN(x,y)  \
  PRINTF ( "JEDA DEBUGGER ERROR: Member \"%s\" not found in class \"%s\" \n",x,y ) ; \
  DBG_ERROR
#define DBG_ERROR_NAME_TYPE_INCONSISTANCE \
  PRINTF ( "JEDA DEBUGGER ERROR: Name type inconsistance detected \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_NTH_ARRAY_INDEX_OVERFLOW(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Array index [%d] overflow \n", x ) ; \
  DBG_ERROR
#define DBG_ERROR_OVERFLOW_DETECTED_IN_BIT_WHILE_CONVERTING_TO_INT \
  PRINTF ( "JEDA DEBUGGER ERROR: Overflow detected while converting bit to int \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_SCOPE_NOT_FOUND \
  PRINTF ( "JEDA DEBUGGER ERROR: Scope not found \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_SUPER_ACCESS_ON_NON_SINGLE_OBJECT \
  PRINTF ( "JEDA DEBUGGER ERROR: Super access to non-single object \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_SUPER_IS_ACCESSED_IN_A_NON_CLASS_CONTEXT \
  PRINTF ( "JEDA DEBUGGER ERROR: Super access in non-class context \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_SUPER_IS_NOT_FOUND_IN_THIS_CLASS_CONTEXT \
  PRINTF ( "JEDA DEBUGGER ERROR: Super is not found in this class context \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_THIS_IS_ACCESSED_IN_A_NON_CLASS_CONTEXT \
  PRINTF ( "JEDA DEBUGGER ERROR: This is accessed in non-class context \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_UNKNOWN_FORMAT_CHAR(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Unknown format charactor \'%c\' \n", x ) ; \
  DBG_ERROR
#define DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Unknown type %d for print \n", x ) ; \
  DBG_ERROR
#define DBG_ERROR_USAGE_SHOW_VAR_LVAR_THREAD_LTHREAD \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal argument for show command, must be vars, lvars, threads, lthreads \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_VAR_NOT_HAVE_MEMBER(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Variable \"%s\" has no member\n",x ) ; \
  DBG_ERROR
#define DBG_ERROR_X_OR_Z_DETECTED_IN_BIT_WHILE_CONVERTING_TO_INT \
  PRINTF ( "JEDA DEBUGGER ERROR: X or Z detected while converting bit to int \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ZERO_DIVIDE_ON_EXPRESSION_EVALUATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Zero divide \n" ) ; \
  DBG_ERROR
#define DBG_FUNC_NAME_NOT_FOUND(x) \
  PRINTF ( "JEDA DEBUGGER ERROR: Function name \"%s\" not found \n", x ) ; \
  DBG_ERROR

#define DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_DUPLICATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type for string duplication \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_CONCATENATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type for string concatenation \n" ) ; \
  DBG_ERROR
#define DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_CONCATENATION \
  PRINTF ( "JEDA DEBUGGER ERROR: Illegal data type for concatenation \n" ) ; \
  DBG_ERROR

#define DBG_MESSAGE_BREAKPOINT_IS_CLEARED(x,y) \
  PRINTF ( "Breakpoint %s:%d is cleared\n",x,y ) ; 
#define DBG_WARNING_BREAKPOINT_IS_ALREADY_SET(x,y) \
  PRINTF ( "JEDA Debugger WARNING: Breakpoint %s:%d is already set \n", x, y ) ; 
#define DBG_WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE \
  PRINTF ( "JEDA Debugger WARNING: Decimal constant overflow detected \n" ) ; 
