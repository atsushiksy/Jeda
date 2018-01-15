/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

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
#ifndef NO_INCLUDE

#include <stdio.h>
/* #include <math.h> */
#include "runtime.h"
#include "mem_macro.h"
#include "runtime_error_msg.h"
#include "data.h"
#include "thread.h"
#include "bit_operations.h"
#include "bit_macro.h"
#include "exec_macro.h"
#include "data_macro.h"
#include "code_exec_macro.h"
#include "exec_object_macro.h"

#endif
JD_data *JD_get_ptr_element(JD_data *dt) ;
/************************************************************
 * 'new' set a new object table entry to ACC
 ************************************************************/
void JD_exec_new() {
  /**MACRO EXEC_NEW**/
  {
    JD_data *dt ;
    JD_object_table *tb ;
    dt = JD_alloc_data_entry();
    dt->type = JD_D_object ;
    dt->data.obj.alloc = ALLOC_OBJ ;
    tb = dt->data.obj.ect = ALLOC_OBJ_TABLE ;
    tb->dealloc = (void *)DEALLOC_OBJ ;
    tb->tbl_size = INITAL_OBJ_TABLE_SIZE ;
    tb->num_entry = 0 ;
    tb->table = ALLOC_OBJ_TABLE_TABLE( INITAL_OBJ_TABLE_SIZE ) ;
    tb->ref_count = 1 ;
    PUSH_TO_STACK(dt) ; /* VSP must be empty when allocating variable */
  }
  /**END MACRO EXEC_NEW**/  
}

/************************************************************
 * 'gen_child' do new and set ACC as super
 ************************************************************/
void JD_exec_gen_child() {
  /**MACRO EXEC_GEN_CHILD**/
  {
    JD_data *dt ;
    JD_object_table *tb ;
    dt = JD_alloc_data_entry();
    dt->type = JD_D_object ;
    dt->data.obj.alloc = ALLOC_OBJ ;
    tb = dt->data.obj.ect = ALLOC_OBJ_TABLE ;
    tb->dealloc = DEALLOC_OBJ ;
    tb->tbl_size = INITAL_OBJ_TABLE_SIZE ;
    tb->num_entry = 0 ;
    tb->table = ALLOC_OBJ_TABLE_TABLE( INITAL_OBJ_TABLE_SIZE ) ;
    tb->super = ACC_DT ;
    ACC_DT->data.obj.ect->child = dt ;
    tb->ref_count = 1 ;
    PUSH_TO_STACK(dt) ; /* VSP must be empty when allocating variable */
  }
  /**END MACRO EXEC_GEN_CHILD**/  
}


/************************************************************
 * 'get_this' set 'this' object to ACC
 ************************************************************/
void JD_exec_get_this() {
  /**MACRO EXEC_GET_THIS**/
  {
    ACC_DT = THREAD->this ;
    ACC_TYPE = JD_D_dt ;
  }
  /**END MACRO EXEC_GET_THIS**/  
}

/************************************************************
 * 'get_super' get super class instance of ACC to ACC
 ************************************************************/
void JD_exec_get_super() {
  /**MACRO EXEC_GET_SUPER**/
  {
    while( TMP_INT-- ) {
      if( ACC_DT->data.obj.ect == NULL ) {
        ERROR_EMPTY_OBJECT_ACCESS ;
      }
      if( ACC_DT->data.obj.ect->super == NULL ) {
        ERROR_EMPTY_OBJECT_ACCESS ;
      }
      ACC_DT = ACC_DT->data.obj.ect->super ;
    }
  }
  /**END MACRO EXEC_GET_SUPER**/  
}

/************************************************************
 * 'get_child' get child class instance of ACC to ACC
 ************************************************************/
void JD_exec_get_child() {
  /**MACRO EXEC_GET_CHILD**/
  {
    while( TMP_INT-- ) {
      if( ACC_DT->data.obj.ect == NULL ) {
        ERROR_EMPTY_OBJECT_ACCESS ;
      }
      ACC_DT = ACC_DT->data.obj.ect->child ;
      if( ACC_DT == NULL ) {
        ERROR_EMPTY_OBJECT_ACCESS ;
      }
    }
  }
  /**END MACRO EXEC_GET_CHILD**/  
}

/************************************************************
 * 'copy_obj' get super class instance of ACC to ACC
 ************************************************************/
void JD_exec_copy_obj() {
  /**MACRO EXEC_COPY_OBJ**/
  {
    JD_data *dt, *odt ;
    int n, i ;
    if( VSP ) {
      if( VS_TOP ) dt = VSTACK[VS_TOP-1] ;
      else dt = VSTACK[VSP_LIMIT-1] ;
    }
    else dt = STACK[SP-1] ; /* top of stack should hold the data */
    if( ACC_DT->data.obj.ect == NULL ) {
        ERROR_EMPTY_OBJECT_ACCESS ;
    }
    if( ACC_DT->data.obj.ect->super ) {
      dt->data.obj.ect->super = 
        JD_duplicate_data( ACC_DT->data.obj.ect->super ) ;
    }
    n = ACC_DT->data.obj.ect->num_entry ;
    dt->data.obj.ect->num_entry = 0 ;
    for( i = 0 ; i < n ; i++ ) {
      odt = ACC_DT->data.obj.ect->table[i] ;
      dt->data.obj.ect->table[i] = JD_duplicate_data( odt )  ;
      dt->data.obj.ect->num_entry++ ;
    }
    CALL_EXEC_POP_ACC ;
    
  }
  /**END MACRO EXEC_COPY_OBJ**/  
}

/*****************************************************************************/
/* alloc instruction is used to allocate class variable */
/* 'alloc' Decimal_num data_type name array_range opt_flags EOL */
/*****************************************************************************/

void JD_set_object() {
  /**MACRO EXEC_SET_OBJECT**/
  {
    JD_data *top ;
    JD_object_table *tb ;
    top = STACK[SP-1] ;
    tb = top->data.obj.ect ;
    if( tb->num_entry == tb->tbl_size ) {
      REALLOC_OBJ_TABLE_TABLE(tb) ;
    }
    tb->table[tb->num_entry++] = TMP_DT ;
  }
  /**END MACRO EXEC_SET_OBJECT**/  
}


void JD_exec_alloc_static() {
  /**MACRO EXEC_ALLOC_STATIC**/
  {
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->data.ptr.pdata = GLOBAL_DATA[TMP_INT] ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STATIC**/  
}

/*************************************************/
/* member function overwrite                     */
/*************************************************/
void JD_exec_alloc_func() {
  /**MACRO EXEC_ALLOC_FUNC**/
  {
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_func ;
    TMP_DT->data.func.func = NULL ;
    TMP_DT->data.func.data = NULL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_FUNC**/  
}

void JD_exec_overwrite() {
  /**MACRO EXEC_OVERWRITE**/
  {
    JD_data *dt ;
    JD_data *top ;
    top = STACK[SP-1] ;
    dt = top->data.obj.ect->super ;
    TMP_DT = dt->data.obj.ect->table[TMP_INT] ;
    TMP_DT->data.func.func = ALLOC_OBJ ;
    TMP_DT->data.func.data = top ;  /* this */
  }
  /**END MACRO EXEC_OVERWRITE**/  
}


void JD_exec_error() {
  /**MACRO EXEC_ERROR**/
  {
    ERROR_EXECUTION_REACHES_ILLEGAL_POINT ;
    exit(1) ;
  }
  /**END MACRO EXEC_ERROR**/  
}
/*************************************************/
/* single data                                   */
/*************************************************/
void JD_exec_alloc_single_int() {
  /**MACRO EXEC_ALLOC_SINGLE_INT**/
  {
    CALL_ALLOC_DT_SINGLE_INT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_INT**/  
}

void JD_exec_alloc_single_bit() {
  /**MACRO EXEC_ALLOC_SINGLE_BIT**/
  {
    CALL_ALLOC_DT_SINGLE_BIT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_BIT**/  
}

void JD_exec_alloc_single_float() {
  /**MACRO EXEC_ALLOC_SINGLE_FLOAT**/
  {
    CALL_ALLOC_DT_SINGLE_FLOAT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_FLOAT**/  
}

void JD_exec_alloc_single_double() {
  /**MACRO EXEC_ALLOC_SINGLE_DOUBLE**/
  {
    CALL_ALLOC_DT_SINGLE_DOUBLE ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_DOUBLE**/  
}

void JD_exec_alloc_single_string() {
  /**MACRO EXEC_ALLOC_SINGLE_STRING**/
  {
    CALL_ALLOC_DT_SINGLE_STRING ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_STRING**/  
}

void JD_exec_alloc_single_signal() {
  /**MACRO EXEC_ALLOC_SINGLE_SIGNAL**/
  {
    CALL_ALLOC_DT_SINGLE_SIGNAL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_alloc_single_obj() {
  /**MACRO EXEC_ALLOC_SINGLE_OBJ**/
  {  
    CALL_ALLOC_DT_SINGLE_OBJ ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_OBJ**/  
}

void JD_exec_alloc_single_thread() {
  /**MACRO EXEC_ALLOC_SINGLE_THREAD**/
  {  
    CALL_ALLOC_DT_SINGLE_THREAD ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_SINGLE_THREAD**/  
}


/*************************************************/
/* array data ARRAY_SIZE should hold the size    */
/*************************************************/
void JD_exec_alloc_array_int() {
  /**MACRO EXEC_ALLOC_ARRAY_INT**/
  {
    CALL_ALLOC_DT_ARRAY_INT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_INT**/  
}

void JD_exec_alloc_array_bit() {
  /**MACRO EXEC_ALLOC_ARRAY_BIT**/
  {
    CALL_ALLOC_DT_ARRAY_BIT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_BIT**/  
}

void JD_exec_alloc_array_float() {
  /**MACRO EXEC_ALLOC_ARRAY_FLOAT**/
  {
    CALL_ALLOC_DT_ARRAY_FLOAT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_FLOAT**/  
}

void JD_exec_alloc_array_double() {
  /**MACRO EXEC_ALLOC_ARRAY_DOUBLE**/
  {
    CALL_ALLOC_DT_ARRAY_DOUBLE ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_DOUBLE**/  
}

void JD_exec_alloc_array_string() {
  /**MACRO EXEC_ALLOC_ARRAY_STRING**/
  {
    CALL_ALLOC_DT_ARRAY_STRING ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_STRING**/  
}

void JD_exec_alloc_array_signal() {
  /**MACRO EXEC_ALLOC_ARRAY_SIGNAL**/
  {
    CALL_ALLOC_DT_ARRAY_SIGNAL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_SIGNAL**/  
}

void JD_exec_alloc_array_obj() {
  /**MACRO EXEC_ALLOC_ARRAY_OBJ**/
  {  
    CALL_ALLOC_DT_ARRAY_OBJ ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_OBJ**/  
}

void JD_exec_alloc_array_thread() {
  /**MACRO EXEC_ALLOC_ARRAY_THREAD**/
  {  
    CALL_ALLOC_DT_ARRAY_THREAD ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ARRAY_THREAD**/  
}

/*************************************************/
/* assoc array data                              */
/*************************************************/
void JD_exec_alloc_assoc_int() {
  /**MACRO EXEC_ALLOC_ASSOC_INT**/
  {
    CALL_ALLOC_DT_ASSOC_INT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_INT**/  
}

void JD_exec_alloc_assoc_bit() {
  /**MACRO EXEC_ALLOC_ASSOC_BIT**/
  {
    CALL_ALLOC_DT_ASSOC_BIT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_BIT**/  
}

void JD_exec_alloc_assoc_float() {
  /**MACRO EXEC_ALLOC_ASSOC_FLOAT**/
  {
    CALL_ALLOC_DT_ASSOC_FLOAT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_FLOAT**/  
}

void JD_exec_alloc_assoc_double() {
  /**MACRO EXEC_ALLOC_ASSOC_DOUBLE**/
  {
    CALL_ALLOC_DT_ASSOC_DOUBLE ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_DOUBLE**/  
}

void JD_exec_alloc_assoc_string() {
  /**MACRO EXEC_ALLOC_ASSOC_STRING**/
  {
    CALL_ALLOC_DT_ASSOC_STRING ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_STRING**/  
}

void JD_exec_alloc_assoc_signal() {
  /**MACRO EXEC_ALLOC_ASSOC_SIGNAL**/
  {
    CALL_ALLOC_DT_ASSOC_SIGNAL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_alloc_assoc_obj() {
  /**MACRO EXEC_ALLOC_ASSOC_OBJ**/
  {  
    CALL_ALLOC_DT_ASSOC_OBJ ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_OBJ**/  
}

void JD_exec_alloc_assoc_thread() {
  /**MACRO EXEC_ALLOC_ASSOC_THREAD**/
  {  
    CALL_ALLOC_DT_ASSOC_THREAD ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_ASSOC_THREAD**/  
}


/*************************************************/
/* string indexed array data                     */
/*************************************************/
void JD_exec_alloc_strided_int() {
  /**MACRO EXEC_ALLOC_STRIDED_INT**/
  {
    CALL_ALLOC_DT_STRIDED_INT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_INT**/  
}

void JD_exec_alloc_strided_bit() {
  /**MACRO EXEC_ALLOC_STRIDED_BIT**/
  {
    CALL_ALLOC_DT_STRIDED_BIT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_BIT**/  
}

void JD_exec_alloc_strided_float() {
  /**MACRO EXEC_ALLOC_STRIDED_FLOAT**/
  {
    CALL_ALLOC_DT_STRIDED_FLOAT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_FLOAT**/  
}

void JD_exec_alloc_strided_double() {
  /**MACRO EXEC_ALLOC_STRIDED_DOUBLE**/
  {
    CALL_ALLOC_DT_STRIDED_DOUBLE ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_DOUBLE**/  
}

void JD_exec_alloc_strided_string() {
  /**MACRO EXEC_ALLOC_STRIDED_STRING**/
  {
    CALL_ALLOC_DT_STRIDED_STRING ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_STRING**/  
}

void JD_exec_alloc_strided_signal() {
  /**MACRO EXEC_ALLOC_STRIDED_SIGNAL**/
  {
    CALL_ALLOC_DT_STRIDED_SIGNAL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_alloc_strided_obj() {
  /**MACRO EXEC_ALLOC_STRIDED_OBJ**/
  {  
    CALL_ALLOC_DT_STRIDED_OBJ ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_OBJ**/  
}

void JD_exec_alloc_strided_thread() {
  /**MACRO EXEC_ALLOC_STRIDED_THREAD**/
  {  
    CALL_ALLOC_DT_STRIDED_THREAD ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_STRIDED_THREAD**/  
}


/*************************************************/
/* list data                                     */
/*************************************************/
void JD_exec_alloc_list_int() {
  /**MACRO EXEC_ALLOC_LIST_INT**/
  {
    CALL_ALLOC_DT_LIST_INT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_INT**/  
}

void JD_exec_alloc_list_bit() {
  /**MACRO EXEC_ALLOC_LIST_BIT**/
  {
    CALL_ALLOC_DT_LIST_BIT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_BIT**/  
}

void JD_exec_alloc_list_float() {
  /**MACRO EXEC_ALLOC_LIST_FLOAT**/
  {
    CALL_ALLOC_DT_LIST_FLOAT ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_FLOAT**/  
}

void JD_exec_alloc_list_double() {
  /**MACRO EXEC_ALLOC_LIST_DOUBLE**/
  {
    CALL_ALLOC_DT_LIST_DOUBLE ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_DOUBLE**/  
}

void JD_exec_alloc_list_string() {
  /**MACRO EXEC_ALLOC_LIST_STRING**/
  {
    CALL_ALLOC_DT_LIST_STRING ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_STRING**/  
}

void JD_exec_alloc_list_signal() {
  /**MACRO EXEC_ALLOC_LIST_SIGNAL**/
  {
    CALL_ALLOC_DT_LIST_SIGNAL ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_alloc_list_obj() {
  /**MACRO EXEC_ALLOC_LIST_OBJ**/
  {  
    CALL_ALLOC_DT_LIST_OBJ ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_OBJ**/  
}

void JD_exec_alloc_list_thread() {
  /**MACRO EXEC_ALLOC_LIST_THREAD**/
  {  
    CALL_ALLOC_DT_LIST_THREAD ;
    CALL_EXEC_SET_OBJECT ;
  }
  /**END MACRO EXEC_ALLOC_LIST_THREAD**/  
}

/*****************************************************************/
/*  Load Object Member                                           */
/*****************************************************************/
void JD_exec_loadm_single() {
  /**MACRO EXEC_LOADM_SINGLE**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_COPY_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADM_SINGLE**/  
}

void JD_exec_loadm_single_var() {
  /**MACRO EXEC_LOADM_SINGLE_VAR**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->sub_type = JD_SD_single ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    TMP_DT->data.ptr.pdata = dt ;
    CALL_EXEC_COPY_ACC(TMP_DT) ;

  }
  /**END MACRO EXEC_LOADM_SINGLE_VAR**/  
}


void JD_exec_loadm_array() {
  /**MACRO EXEC_LOADM_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    int index = ACC_INT ;
    CALL_EXEC_POP_ACC ; /* Inst */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= index || index < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[index] ;
    dt = ut->data ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_SET_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADM_ARRAY**/  
}

void JD_exec_loadm_marray() {
  /**MACRO EXEC_LOADM_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    int index = ACC_INT ;
    CALL_EXEC_POP_ACC ; /* Inst */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[index] ;
    dt = ut->data ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_SET_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADM_MARRAY**/  
}

void JD_exec_loadm_assoc() {
  /**MACRO EXEC_LOADM_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs ;
    int index = ACC_INT ;
    CALL_EXEC_POP_ACC ; /* Inst */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_assoc ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    hs = CALC_DATA_HASH(index) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( hsh->idx == index ) break ;
      if( hsh->idx > index ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      rt = hsh->data ;
      CALL_EXEC_COPY_ACC(rt) ;
    }
    else {
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
       WARNING_ACCESS_TO_EMPTY_ASSOC_ARRAY ;
#endif
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      CALL_EXEC_NULL_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_LOADM_ASSOC**/  
}

void JD_exec_loadm_stridx() {
  /**MACRO EXEC_LOADM_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs, t ;
    TMP_STRING = ACC_STRING ;
    ACC_STRING = NULL ;
    CALL_EXEC_POP_ACC ; /* Inst */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
    if( TMP_STRING == NULL ) {
      ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS ;
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      CALL_EXEC_NULL_ACC(dt) ;
      return ;
    }
    CALC_STRING_HASH(hs,TMP_STRING) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( !(t = strcmp(hsh->string,TMP_STRING)) ) break ;
      if( t > 0 ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      rt = hsh->data ;
      CALL_EXEC_COPY_ACC(rt) ;
    }
    else {
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
       WARNING_ACCESS_TO_EMPTY_ASSOC_ARRAY ;
#endif
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      CALL_EXEC_NULL_ACC(dt) ;
    }
    DEALLOC_STRING(TMP_STRING) ;
  }
  /**END MACRO EXEC_LOADM_STRIDX**/  
}

void JD_exec_loadm_list() {
  /**MACRO EXEC_LOADM_LIST**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut, *at ;
    int n ;
    int index = ACC_INT ;
    CALL_EXEC_POP_ACC ; /* Inst */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    
    if( dt->sub_type != JD_SD_list ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else {
      n = 0 ;
      ut =  dt->data.list.top ;
      while( ut ) {
        if( n == ACC_INT ) at = ut ;
        n++ ;
        ut = ut->next ;
      }
      if( n < ACC_INT || ACC_INT < 0 ) {
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
         WARNING_ACCESS_TO_EMPTY_LIST_LOCATION ;
#endif
      }
    }
    if( at ) CALL_EXEC_SET_ACC(at->data) ;
    else {
      CALL_EXEC_NULL_ACC(dt) ;
    }

  }
  /**END MACRO EXEC_LOADM_LIST**/  
}

/*****************************************************************/
/*  Store Class Member                                           */
/*****************************************************************/
/*
 *  Store single <type>
 */ 
void JD_exec_storem_single_int() {
  /**MACRO EXEC_STOREM_SINGLE_INT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.int_value = ACC_INT ;
#ifdef JEDA_VERILOG_VAR_UPDATE
    if( IS_STATICDATA(dt) ) 
      JD_update_static_var(dt) ;
#endif
  }
  /**END MACRO EXEC_STOREM_SINGLE_INT**/  
}

void JD_exec_storem_single_flt_dbl() {
  /**MACRO EXEC_STOREM_SINGLE_FLT_DBL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    if( dt->type == JD_D_float )
      dt->data.float_value = ACC_FLOAT ;
    else 
      dt->data.double_value = ACC_FLOAT ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_FLT_DBL**/  
}

void JD_exec_storem_single_bit() {
  /**MACRO EXEC_STOREM_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int msk ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    if( dt->type == JD_D_sbit ) {
      msk = BITMASK(dt->data.sbit.size) ;
      dt->data.sbit.a = ACC_A[0] & msk ;
      dt->data.sbit.b = ACC_B[0] & msk ;
    }
    else {
      int i, n ;
      if (dt->data.bit.size > ACC_SIZE) {
        n = LASTWORD(ACC_SIZE) ;
        for( i = 0 ; i <=n ; i++ ) {
          dt->data.bit.a[i] = ACC_A[i] ;
          dt->data.bit.b[i] = ACC_B[i] ;
        }
        n = LASTWORD(dt->data.bit.size) ;
        for( ; i <= n ; i++ ) {
          dt->data.bit.a[i] = 0 ;
          dt->data.bit.b[i] = 0 ;
        }
      }
      else {
        n = LASTWORD(dt->data.bit.size) ;
        msk = BITMASK(dt->data.bit.size) ;
        for( i = 0 ; i < n ; i++ ) {
          dt->data.bit.a[i] = ACC_A[i] ;
          dt->data.bit.b[i] = ACC_B[i] ;
        }
        dt->data.bit.a[n] = ACC_A[n] & msk ;
        dt->data.bit.b[n] = ACC_B[n] & msk ;
      }
    }
#ifdef JEDA_VERILOG_VAR_UPDATE
    if( IS_STATICDATA(dt) ) 
      JD_update_static_var(dt) ;
#endif
  }
  /**END MACRO EXEC_STOREM_SINGLE_BIT**/  
}

void JD_exec_storem_string_to_single_bit() {
  /**MACRO EXEC_STOREM_STRING_TO_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREM_STRING_TO_SINGLE_BIT**/  
}

void JD_exec_storem_single_string() {
  /**MACRO EXEC_STOREM_SINGLE_STRING**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.string = ACC_STRING ;
    ACC_STRING = NULL ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_STRING**/  
}

void JD_exec_storem_single_string_null() {
  /**MACRO EXEC_STOREM_SINGLE_STRING_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.string = NULL ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_STRING_NULL**/  
}

void JD_exec_storem_single_obj() {
  /**MACRO EXEC_STOREM_SINGLE_OBJ**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.obj.ect = ACC_DT->data.obj.ect ;
    if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_OBJ**/  
}

void JD_exec_storem_single_obj_null() {
  /**MACRO EXEC_STOREM_SINGLE_OBJ_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.obj.ect = NULL ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_OBJ_NULL**/  
}

void JD_exec_storem_single_thread() {
  /**MACRO EXEC_STOREM_SINGLE_THREAD**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.thrd = ACC_DT->data.thrd ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_THREAD**/  
}

void JD_exec_storem_single_thread_null() {
  /**MACRO EXEC_STOREM_SINGLE_THREAD_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.thrd = NULL ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_THREAD_NULL**/  
}


void JD_exec_storem_single_signal() {
  /**MACRO EXEC_STOREM_SINGLE_SIGNAL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.signal.port = ACC_DT->data.signal.port ;
    dt->data.signal.data0 = ACC_DT->data.signal.data0 ;
    dt->data.signal.data1 = ACC_DT->data.signal.data1 ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_SIGNAL**/  
}

void JD_exec_storem_single_signal_null() {
  /**MACRO EXEC_STOREM_SINGLE_SIGNAL_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    dt->data.signal.port = NULL ;
    dt->data.signal.data0 = 0 ;
    dt->data.signal.data1 = 0 ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_SIGNAL_NULL**/  
}

void JD_exec_storem_single_dt() {
  /**MACRO EXEC_STOREM_SINGLE_DT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    JD_duplicate_data_to( dt, ACC_DT ) ;
  }
  /**END MACRO EXEC_STOREM_SINGLE_DT**/  
}


/*
 * Store subbit
 */
void JD_exec_storem_subbit() {
  /**MACRO EXEC_STOREM_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    lb = ACC_INT ;  /* lb is in ACC */
    CALL_EXEC_POP_ACC ;
    ub = ACC_INT ;  /* ub is pushed to stack */
    CALL_EXEC_POP_ACC ; /* object is pushed to stack */
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_POP_ACC ; /* data to be stored */
    switch(ACC_TYPE) {
      case JD_D_int:
        CALL_EXEC_INT_TO_BIT ;
        break ;
      case JD_D_bit:
        break ;
      case JD_D_float:
      case JD_D_double:
        CALL_EXEC_FLOAT_TO_BIT ;
        break ;
      default:
        ERROR_ILLEGAL_DATA_TYPE_FOR_BIT_ASSIGNMENT ;
    }        
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    size = (dt->type == JD_D_sbit)?(dt->data.sbit.size):(dt->data.bit.size);
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= size || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == -1 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub > U_BIT ) || (lb < L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == 1 ) {
      if( ub > lb ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub < U_BIT ) || (lb > L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif
    if( I_ENDIAN == 0 ) {
      aub = ub ;
      alb = lb ;
    }
    else if( I_ENDIAN == -1 ) {
      aub = ub-L_BIT ;
      alb = lb-L_BIT ;
    }
    else {
      aub = L_BIT - ub ;
      alb = L_BIT - lb ;
    }
    if( dt->type == JD_D_sbit ) {
      lmsk = ((unsigned int)0xffffffff << alb ) & 
            ((unsigned int)0xffffffff >> (31-aub) );
      dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((ACC_A[0]<<alb) & lmsk) ;
      dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((ACC_B[0]<<alb) & lmsk) ;
    }
    else {
      int i, j, n, an, a, b, aa, ab, ls, us ;
      i = LASTWORD(alb+1) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff << ls ;
      umsk = (unsigned int)0xffffffff >> (31-(aub%32)) ;
      a = dt->data.bit.a[i] & ~lmsk ;
      b = dt->data.bit.b[i] & ~lmsk ;
      an = LASTWORD(ACC_SIZE) ;
      j = 0 ;
      n = LASTWORD(aub+1) ;
      for( ; i < n ; i++ ) {
        aa = (j<=an)?ACC_A[j]:0 ;
        ab = (j<=an)?ACC_B[j]:0 ;
        dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
        dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
        a = (aa >> us) & ~lmsk ;
        b = (ab >> us) & ~lmsk ;
        j++ ;
      }
      aa = (j<=an)?ACC_A[j]:0 ;
      ab = (j<=an)?ACC_B[j]:0 ;
      dt->data.bit.a[i] = 
        (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
      dt->data.bit.b[i] = 
        (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
    }
  }
  /**END MACRO EXEC_STOREM_SUBBIT**/  
}


/*
 *  Store array
 */
void JD_exec_storem_array() {
  /**MACRO EXEC_STOREM_ARRAY**/
  {  /* TMP_INT holds member variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= TMP_INT2 || TMP_INT2 < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREM_ARRAY**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to ACC_INT
 */ 
void JD_exec_storem_marray() {
  /**MACRO EXEC_STOREM_MARRAY**/
  {  /* TMP_INT holds member variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREM_MARRAY**/  
}

void JD_exec_storem_assoc() {
  /**MACRO EXEC_STOREM_ASSOC**/
  {  /* TMP_INT holds member variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_assoc ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    hs = CALC_DATA_HASH(TMP_INT2) ;
    hsh = dt->data.hash.table[hs] ;
    phsh = NULL ;
    while( hsh ) {
      if( hsh->idx == TMP_INT2 ) break ;
      phsh = hsh ;
      if( hsh->idx > TMP_INT2 ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      dt = hsh->data ;
      CALL_EXEC_ACC_TO_DT(dt) ;
    }
    else {
      /* Allocate the new entry */
      hsh = ALLOC_HASH_ENTRY ;
      hsh->type = JD_assoc_hash ;
      hsh->idx = TMP_INT2 ;
      if( phsh ) {
        if( phsh->idx > TMP_INT2 ) phsh->right = hsh ;
        else phsh->left = hsh ;
      }
      else dt->data.hash.table[hs] = hsh ;
      hsh->data = JD_alloc_single_data( dt ) ;
      dt = hsh->data ;
      CALL_EXEC_ACC_TO_DT(dt) ;
    }
  }
  /**END MACRO EXEC_STOREM_ASSOC**/  
}

void JD_exec_storem_stridx() {
  /**MACRO EXEC_STOREM_STRIDX**/
  {  /* TMP_INT holds member variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    if( TMP_STRING == NULL ) {
      ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS ;
      return ;
    }
    CALC_STRING_HASH(hs,TMP_STRING) ;
    phsh = NULL ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( !(t = strcmp(hsh->string,TMP_STRING) )) break ;
      phsh = hsh ;
      if( t > 0 ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      rt = hsh->data ;
      CALL_EXEC_ACC_TO_DT(rt) ;
      DEALLOC_STRING(TMP_STRING) ;
    }
    else {
      /* Allocate the new entry */
      hsh = ALLOC_HASH_ENTRY ;
      hsh->type = JD_assoc_hash ;
      hsh->string = TMP_STRING ;
      TMP_STRING = NULL ;
      if( phsh ) {
        if( t > 0 ) phsh->right = hsh ;
        else phsh->left = hsh ;
      }
      else dt->data.hash.table[hs] = hsh ;
      
      hsh->data = JD_alloc_single_data( dt ) ;
      dt = hsh->data ;
      CALL_EXEC_ACC_TO_DT(dt) ;
    }
  }
  /**END MACRO EXEC_STOREM_STRIDX**/  
}

/*
 *  Store object member array subbit
 */
void JD_exec_storem_array_subbit() {
  /**MACRO EXEC_STOREM_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= TMP_INT2 || TMP_INT2 < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    ub = U_INDEX ;
    lb = L_INDEX ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    size = (dt->type == JD_D_sbit)?(dt->data.sbit.size):(dt->data.bit.size);
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= size || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == -1 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub > U_BIT ) || (lb < L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == 1 ) {
      if( ub > lb ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub < U_BIT ) || (lb > L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif
    if( I_ENDIAN == 0 ) {
      aub = ub ;
      alb = lb ;
    }
    else if( I_ENDIAN == -1 ) {
      aub = ub-L_BIT ;
      alb = lb-L_BIT ;
    }
    else {
      aub = L_BIT - ub ;
      alb = L_BIT - lb ;
    }
    STORE_SIZE = aub - alb + 1 ;
    if( dt->type == JD_D_sbit ) {
      lmsk = ((unsigned int)0xffffffff << alb ) & 
            ((unsigned int)0xffffffff >> (31-aub) );
      dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((ACC_A[0]<<alb) & lmsk) ;
      dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((ACC_B[0]<<alb) & lmsk) ;
    }
    else {
      int i, j, n, an, a, b, aa, ab, ls, us ;
      i = LASTWORD(alb+1) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff << ls ;
      umsk = (unsigned int)0xffffffff >> (31-(aub%32)) ;
      a = dt->data.bit.a[i] & ~lmsk ;
      b = dt->data.bit.b[i] & ~lmsk ;
      an = LASTWORD(ACC_SIZE) ;
      j = 0 ;
      n = LASTWORD(aub+1) ;
      for( ; i < n ; i++ ) {
        aa = (j<=an)?ACC_A[j]:0 ;
        ab = (j<=an)?ACC_B[j]:0 ;
        dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
        dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
        a = (aa >> us) & ~lmsk ;
        b = (ab >> us) & ~lmsk ;
        j++ ;
      }
      aa = (j<=an)?ACC_A[j]:0 ;
      ab = (j<=an)?ACC_B[j]:0 ;
      dt->data.bit.a[i] = 
        (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
      dt->data.bit.b[i] = 
        (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
    }
  }
  /**END MACRO EXEC_STOREM_ARRAY_SUBBIT**/  
}

/*
 * store object memboer marray subbit
 */ 
void JD_exec_storem_marray_subbit() {
  /**MACRO EXEC_STOREM_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= TMP_INT2 || TMP_INT2 < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    ub = U_INDEX ;
    lb = L_INDEX ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    size = (dt->type == JD_D_sbit)?(dt->data.sbit.size):(dt->data.bit.size);
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= size || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == -1 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub > U_BIT ) || (lb < L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == 1 ) {
      if( ub > lb ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub < U_BIT ) || (lb > L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif
    if( I_ENDIAN == 0 ) {
      aub = ub ;
      alb = lb ;
    }
    else if( I_ENDIAN == -1 ) {
      aub = ub-L_BIT ;
      alb = lb-L_BIT ;
    }
    else {
      aub = L_BIT - ub ;
      alb = L_BIT - lb ;
    }
    STORE_SIZE = aub - alb + 1 ;
    if( dt->type == JD_D_sbit ) {
      lmsk = ((unsigned int)0xffffffff << alb ) & 
            ((unsigned int)0xffffffff >> (31-aub) );
      dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((ACC_A[0]<<alb) & lmsk) ;
      dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((ACC_B[0]<<alb) & lmsk) ;
    }
    else {
      int i, j, n, an, a, b, aa, ab, ls, us ;
      i = LASTWORD(alb+1) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff << ls ;
      umsk = (unsigned int)0xffffffff >> (31-(aub%32)) ;
      a = dt->data.bit.a[i] & ~lmsk ;
      b = dt->data.bit.b[i] & ~lmsk ;
      an = LASTWORD(ACC_SIZE) ;
      j = 0 ;
      n = LASTWORD(aub+1) ;
      for( ; i < n ; i++ ) {
        aa = (j<=an)?ACC_A[j]:0 ;
        ab = (j<=an)?ACC_B[j]:0 ;
        dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
        dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
        a = (aa >> us) & ~lmsk ;
        b = (ab >> us) & ~lmsk ;
        j++ ;
      }
      aa = (j<=an)?ACC_A[j]:0 ;
      ab = (j<=an)?ACC_B[j]:0 ;
      dt->data.bit.a[i] = 
        (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
      dt->data.bit.b[i] = 
        (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
    }
  }
  /**END MACRO EXEC_STOREM_MARRAY_SUBBIT**/  
}

void JD_exec_storem_assoc_subbit() {
  /**MACRO EXEC_STOREM_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_assoc ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    hs = CALC_DATA_HASH(TMP_INT2) ;
    hsh = dt->data.hash.table[hs] ;
    phsh = NULL ;
    while( hsh ) {
      if( hsh->idx == TMP_INT2 ) break ;
      phsh = hsh ;
      if( hsh->idx > TMP_INT2 ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      dt = hsh->data ;
    }
    else {
      /* Allocate the new entry */
      hsh = ALLOC_HASH_ENTRY ;
      hsh->type = JD_assoc_hash ;
      hsh->idx = TMP_INT2 ;
      if( phsh ) {
        if( phsh->idx > TMP_INT2 ) phsh->right = hsh ;
        else phsh->left = hsh ;
      }
      else dt->data.hash.table[hs] = hsh ;
      dt = hsh->data = JD_alloc_single_data( dt ) ;
    }
    ub = U_INDEX ;
    lb = L_INDEX ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    size = (dt->type == JD_D_sbit)?(dt->data.sbit.size):(dt->data.bit.size);
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= size || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == -1 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub > U_BIT ) || (lb < L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == 1 ) {
      if( ub > lb ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub < U_BIT ) || (lb > L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif
    if( I_ENDIAN == 0 ) {
      aub = ub ;
      alb = lb ;
    }
    else if( I_ENDIAN == -1 ) {
      aub = ub-L_BIT ;
      alb = lb-L_BIT ;
    }
    else {
      aub = L_BIT - ub ;
      alb = L_BIT - lb ;
    }
    STORE_SIZE = aub - alb + 1 ;
    if( dt->type == JD_D_sbit ) {
      lmsk = ((unsigned int)0xffffffff << alb ) & 
            ((unsigned int)0xffffffff >> (31-aub) );
      dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((ACC_A[0]<<alb) & lmsk) ;
      dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((ACC_B[0]<<alb) & lmsk) ;
    }
    else {
      int i, j, n, an, a, b, aa, ab, ls, us ;
      i = LASTWORD(alb+1) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff << ls ;
      umsk = (unsigned int)0xffffffff >> (31-(aub%32)) ;
      a = dt->data.bit.a[i] & ~lmsk ;
      b = dt->data.bit.b[i] & ~lmsk ;
      an = LASTWORD(ACC_SIZE) ;
      j = 0 ;
      n = LASTWORD(aub+1) ;
      for( ; i < n ; i++ ) {
        aa = (j<=an)?ACC_A[j]:0 ;
        ab = (j<=an)?ACC_B[j]:0 ;
        dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
        dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
        a = (aa >> us) & ~lmsk ;
        b = (ab >> us) & ~lmsk ;
        j++ ;
      }
      aa = (j<=an)?ACC_A[j]:0 ;
      ab = (j<=an)?ACC_B[j]:0 ;
      dt->data.bit.a[i] = 
        (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
      dt->data.bit.b[i] = 
        (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
    }
  }
  /**END MACRO EXEC_STOREM_ASSOC_SUBBIT**/  
}

void JD_exec_storem_stridx_subbit() {
  /**MACRO EXEC_STOREM_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
    if( TMP_STRING == NULL ) {
      ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS ;
      return ;
    }
    CALC_STRING_HASH(hs,TMP_STRING) ;
    phsh = NULL ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( !(t = strcmp(hsh->string,TMP_STRING) )) break ;
      phsh = hsh ;
      if( t > 0 ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) {
      dt = hsh->data ;
      DEALLOC_STRING(TMP_STRING) ;
    }
    else {
      /* Allocate the new entry */
      hsh = ALLOC_HASH_ENTRY ;
      hsh->type = JD_assoc_hash ;
      hsh->string = TMP_STRING ;
      ACC_STRING = NULL ;
      if( phsh ) {
        if( t > 0 ) phsh->right = hsh ;
        else phsh->left = hsh ;
      }
      else dt->data.hash.table[hs] = hsh ;
      
      hsh->data = JD_alloc_single_data( dt ) ;
      dt = hsh->data ;
    }
    ub = U_INDEX ;
    lb = L_INDEX ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    size = (dt->type == JD_D_sbit)?(dt->data.sbit.size):(dt->data.bit.size);
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= size || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == -1 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub > U_BIT ) || (lb < L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else if( I_ENDIAN == 1 ) {
      if( ub > lb ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( ( ub < U_BIT ) || (lb > L_BIT) || ub < 0 || lb < 0 ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif
    if( I_ENDIAN == 0 ) {
      aub = ub ;
      alb = lb ;
    }
    else if( I_ENDIAN == -1 ) {
      aub = ub-L_BIT ;
      alb = lb-L_BIT ;
    }
    else {
      aub = L_BIT - ub ;
      alb = L_BIT - lb ;
    }
    STORE_SIZE = aub - alb + 1 ;
    if( dt->type == JD_D_sbit ) {
      lmsk = ((unsigned int)0xffffffff << alb ) & 
            ((unsigned int)0xffffffff >> (31-aub) );
      dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((ACC_A[0]<<alb) & lmsk) ;
      dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((ACC_B[0]<<alb) & lmsk) ;
    }
    else {
      int i, j, n, an, a, b, aa, ab, ls, us ;
      i = LASTWORD(alb+1) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff << ls ;
      umsk = (unsigned int)0xffffffff >> (31-(aub%32)) ;
      a = dt->data.bit.a[i] & ~lmsk ;
      b = dt->data.bit.b[i] & ~lmsk ;
      an = LASTWORD(ACC_SIZE) ;
      j = 0 ;
      n = LASTWORD(aub+1) ;
      for( ; i < n ; i++ ) {
        aa = (j<=an)?ACC_A[j]:0 ;
        ab = (j<=an)?ACC_B[j]:0 ;
        dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
        dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
        a = (aa >> us) & ~lmsk ;
        b = (ab >> us) & ~lmsk ;
        j++ ;
      }
      aa = (j<=an)?ACC_A[j]:0 ;
      ab = (j<=an)?ACC_B[j]:0 ;
      dt->data.bit.a[i] = 
        (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
      dt->data.bit.b[i] = 
        (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
    }
  }
  /**END MACRO EXEC_STOREM_STRIDX_SUBBIT**/  
}

