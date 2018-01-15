/* 
  Runtime Error Messages
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
#define DBG_PRINT1(a) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print1 %x\n", a ) ; 
#define DBG_PRINT2(a,b) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print2 %x %x\n", a,b ) ; 
#define DBG_PRINT3(a,b,c) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print3 %x %x %x\n", a,b,c ) ; 
#define DBG_PRINT4(a,b,c,d) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print4 %x %x %x %x\n", a,b,c,d ) ; 
#define DBG_PRINT5(a,b,c,d,e) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print5 %x %x %x %x %x\n", a,b,c,d,e ) ; 
#define DBG_PRINT6(a,b,c,d,e,f) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print6 %x %x %x %x %x %x\n", a,b,c,d,e,f ) ; 
#define DBG_PRINT7(a,b,c,d,e,f,g) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print7 %x %x %x %x %x %x %x\n", a,b,c,d,e,f,g ) ; 
#define DBG_PRINT8(a,b,c,d,e,f,g,h) \
  DBG_CODE_LOCATION ; \
  PRINTF ( "Debug print5 %x %x %x %x %x %x %x %x\n", a,b,c,d,e,f,g,h ) ; 
#define ERROR_NULL_SIGNAL_ACCESS  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Empty signal access\n" ) ; \
  FATAL_ERROR

#define ERROR_EMPTY_OBJECT_ACCESS  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Empty object access\n" ) ; \
  FATAL_ERROR

#define ERROR_RUNTIME_FATAL_NO_MEMORY  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: No more memory\n" ) ; \
  FATAL_ERROR

#define ERROR_INTERNAL_TREAD_QUEUE_INCONSISTENT \
  CODE_LOCATION ; \
  PRINTF ( "JEDA INTERNAL ERROR: thread queue inconsistent\n" ) ; \
  FATAL_ERROR

#define ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal subtype for array access\n" ) ; \
  FATAL_ERROR 
#define ERROR_ARRAY_ACCESS_SIZE_OVERFLOW \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Array access range overflow\n" ) ; \
  FATAL_ERROR 
#define ERROR_ARRAY_INDEX_ACCESS_OVERFLOW \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Array access range overflow\n" ) ; \
  FATAL_ERROR 
#define ERROR_MULTI_DIMENSION_ARRAY_ACCESS_INDEX_OVERFLOW(x,y) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Multi-dimentional array access range overflow, actual:%d limit:%d\n", x, y ) ; \
  FATAL_ERROR 
#define ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal subtype for associative array access\n" ) ; \
  FATAL_ERROR 
#define ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal subtype for string indexed array access\n" ) ; \
  FATAL_ERROR 
#define ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: null string for string indexed array access\n" ) ; \

#define ERROR_ILLEGAL_DATA_TYPE_FOR_INTERGER_CONVERSION \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal data type for integer conversion\n" ) ; \
  FATAL_ERROR 
#define ERROR_ILLEGAL_DATA_TYPE_FOR_FLOAT_CONVERSION \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal data type for float conversion\n" ) ; \
  FATAL_ERROR 
#define ERROR_ILLEGAL_DATA_TYPE_FOR_BIT_ASSIGNMENT \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal data type for assignment to bit\n" ) ; \
  FATAL_ERROR 
#define ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Upper bit is smaller than lower bit for big endian bit data\n" ) ; \
  FATAL_ERROR 
#define ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Upper bit is larger than lower bit for small endian bit data\n" ) ; \
  FATAL_ERROR 
#define ERROR_BIT_OFFSET_OUT_OF_RANGE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: bit offset out of range\n" ) ; \
  FATAL_ERROR 
#define WARNING_ACCESS_TO_EMPTY_ASSOC_ARRAY \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: Access to an empty associative array entry\n" ) ; \
  WARNING_HANDLER ;
#define WARNING_DATA_OVERFLOW_DETECTED_ON_BIT_TO_INT_CONVERSION \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: Data overflow detected on bit to int conversion\n" ) ; 
#define WARNING_ACCESS_TO_EMPTY_LIST_LOCATION \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: Access to an empty list location\n" ) ; \
  WARNING_HANDLER ;


#define ERROR_ILLEGAL_ARGUMENT_FOR(x) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal argument for %s\n", x ) ; 

#define ERROR_ILLEGAL_DATA_TYPE_FOR_STORE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: illegal data type for store\n" ) ; \
  FATAL_ERROR 

#define ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: illegal data type for comparison\n" ) ; \
  FATAL_ERROR 

#define ERROR_LIST_RECEIVE_IS_CALLED_ON_LIST_IN_NON_MAILBOX_MODE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: list.receive() called on non-mailbox mode list\n" ) ; \
  FATAL_ERROR 

#define ERROR_LIST_ENTER_IS_CALLED_ON_LIST_IN_MAILBOX_MODE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: list.enter() called on mailbox mode list\n" ) ; \
  FATAL_ERROR 

#define ERROR_LIST_EXIT_DATA_NTH_NOT_FOUND_IN_THE_LIST(x) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: list.exit() %d-th data not found in the list\n",x ) ; \
  FATAL_ERROR 

#define ERROR_DIMENSION_TOO_LARGE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: too large dimention to array\n" ) ; \
  FATAL_ERROR 

#define WARNING_X_OR_Z_DATA_ASSIGNED_TO_FLOAT \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: data with X or Z is assigned to floating point data\n" ) ; \
  WARNING_HANDLER ;

#define WARNING_X_OR_Z_DATA_ASSIGNED_TO_INTEGER \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: data with X or Z is assigned to integer data\n" ) ; \
  WARNING_HANDLER ;

#define WARNING_POP_DATA_FROM_EMPTY_LIST \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: pop data from empty list\n" ) ; \
  WARNING_HANDLER ;

#define VERIF_ERROR JEDA_verif_error()

#define ERROR_WRONG_ENDIAN_PARTIAL_VECTOR_FOR_PORT(u,l,s,p) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: wrong endian partial vector [%d:%d] for %s%s%s\n",\
    u, l, (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  VERIF_ERROR
  
#define ERROR_OUT_OF_RANGE_PARTIAL_VECTOR_FOR_PORT(u,l,s,p) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: out of range partial vector [%d:%d] for %s%s%s\n",\
    u, l, (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  VERIF_ERROR

#define ERROR_NORMAL_DRIVE_CONFLICT(s,p,l,f) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA VERIFICATION ERROR: Normal drive conflict for %s%s%s\n",\
    (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  PRINTF ( "   Previously driven " ) ; \
  JD_at_location(l,f) ; \
  PRINTF ( "\n" ) ; \
  VERIF_ERROR

#define ERROR_STRONG_DRIVE_CONFLICT(s,p,l,f) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA VERIFICATION ERROR: Strong drive conflict for %s%s%s\n",\
    (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  PRINTF ( "   Previously driven " ) ; \
  JD_at_location(l,f) ; \
  PRINTF ( "\n" ) ; \
  VERIF_ERROR

#define ERROR_DUPLICATE_DRIVE_CLOCK_CALL_ON_PORT(s,p) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Duplicate drive clock call on %s%s%s\n",\
    (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  VERIF_ERROR

#define ERROR_STOP_CLOCK_CALLED_ON_UNDRIVEN_PORT(s,p) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: stop_clock() called on undriven port %s%s%s\n", \
    (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  VERIF_ERROR

#define ERROR_INTERNAL_EMPTY_DELAY_QUEUE \
  CODE_LOCATION ; \
  PRINTF ( "Error, internal: empty delay queue\n" ) ; \
  FATAL_ERROR 

#define ERROR_DUPLICATED_DRIVE_BACK_REQUEST(s,p) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: duplicated drive_back() call on port %s%s%s\n", \
    (s==NULL)?"":s, (s==NULL)?"":".", p ) ; \
  VERIF_ERROR

#define ERROR_INTERNAL_UNKNOWN_PLI_REQUEST_TYPE_FOR_PORT(x) \
  CODE_LOCATION ; \
  PRINTF ( "Error, internal: unknown pli request type %d\n", x ) ; \
  FATAL_ERROR 

#define ERROR_WEAK_DRIVE_CONFLICT(x,y,l,f) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Weak Drive Conflict detected on %s%s%s\n", (x)?x:"", (x)?".":"", y ) ; \
  PRINTF ( "   Previously driven " ) ; \
  JD_at_location(l,f) ; \
  PRINTF ( "\n" ) ; \
  VERIF_ERROR

#define ERROR_DEPTH_TOO_DEEP(x, y)  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: depth %d exceeded the port depth %d\n", x, y ) ; \
  FATAL_ERROR

#define ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(x)  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: %d-th multi-array index overflow\n", x ) ; \
  FATAL_ERROR

#define ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: illegal data type for random seed\n" ) ; \
  FATAL_ERROR 

#define ERROR_EXECUTION_REACHES_ILLEGAL_POINT \
  CODE_LOCATION ; \
  PRINTF( "JEDA RUNTIME ERROR: program control reaches to impossible location\n" ) ; \
  PRINTF( "  Possiblely calling undefined abstract function.\n" ) ; \
  FATAL_ERROR 

#define ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: illegal class type for cast assignment\n" ) ; \
  FATAL_ERROR 

#define WARNING_ILLEGAL_DATA_TYPE_FOR_SORT \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: Illegal data type for sort\n" ) ; \
  WARNING_HANDLER ;
  
#define WARNING_X_VALUE_DETECTED_IN_SORT \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: X value detected in sort, result unknown\n" ) ; \
  WARNING_HANDLER ;

#define WARNING_INDEX_NOT_FOUND_IN_DELETE_INDEX_FUNC \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: index not found in delete_index() function call\n" ) ;   \
  WARNING_HANDLER ;

#define WARNING_RECURSIVE_ADVICE_CALL_DETECTED \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: Recursive advice call detected. It's not supported\n" ) ; \
  WARNING_HANDLER ;


#define ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC(x,y) \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal %d-th argument for system function \'%s\' \n", \
    x, y ) ; \
  FATAL_ERROR 

#define ERROR_FUNCTION_RETURNING_WITHOUT_VALUE(x)  \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: function %s returning without value\n", x ) ; \
  FATAL_ERROR

#define WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED \
  CODE_LOCATION ; \
  PRINTF ( "JEDA WARNING: empty event object access (use \'+jeda_ignore_empty_event\' to avoid this message)\n" ) ; 

#define ERROR_NO_RETURN_VALUE_IN_CURRENT_ADVICE \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: No return value can be accessed in current advice\n" ) ; \
  FATAL_ERROR 

/* RND/CYC object error message */

#define ERROR_NUMBER_OF_ELEMENT_TOO_LARGE_FOR_CYC \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Number of element too large for CYC()\n" ) ; \
  FATAL_ERROR 

#define ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal data type for RND()\n" ) ; \
  FATAL_ERROR 

#define ERROR_ILLEGAL_NUM_ARGS \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal number of argument\n" ) ; \
  FATAL_ERROR 

#define ERROR_RND_LIMIT_OVERFLOW \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: RND/CYC random generation limit overflow, the constraints may be too strict\n" ) ; \
  FATAL_ERROR 

#define ERROR_WEIGHT_TIMES_NUM_ELEMENT_OVERFLOW \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Random waits times num_element overflow. Wait and/or range may be too large\n" ) ; \
  FATAL_ERROR 

#define ERROR_ILLEGAL_DUP_CALL_FOR_SYS_RND \
  CODE_LOCATION ; \
  PRINTF ( "JEDA RUNTIME ERROR: Illegal duplication called for CYC/RND object\n" ) ; \
  FATAL_ERROR 
