/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
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

#ifndef NO_INCLUDE

#include <stdio.h>
/* #include <math.h> */
#include "runtime.h"
#include "runtime_error_msg.h"
#include "data.h"
#include "thread.h"
#include "bit_operations.h"
#include "bit_macro.h"
#include "exec_macro.h"
#include "data_macro.h"
#include "code_exec_macro.h"
#ifdef PRODUCT_CODE
#include "veriuser.h"
#include "acc_user.h"
#include "pli.h"
#endif
#include "mem_macro.h"

#endif

#ifdef PRODUCT_CODE
#include "../include/license.h"
  static int exec_check_ok = 0 ;
  void JD_exec_check( int r, int k ) {
    if( check_dsa_key( r, k ) ) {
      fprintf( stderr, "Jeda license code error (%d %d) detected\n", r, k ) ;
      exit(1) ;
    }
    exec_check_ok = 1 ;
  }
#endif

#ifdef NOTIFY_GC_STATUS
  int JD_pop_stack_int_num = 0 ;
  int JD_vstack_ovflw_int_num = 0 ;
  int JD_vstack_ovflw_smlint_num = 0 ;
  int JD_vstack_ovflw_nonint_num = 0 ;
  int JD_push_acc_vstack_ovflw_int_num = 0 ;
  int JD_flush_vstack_int_num = 0 ;
  int JD_flush_vstack_smlint_num = 0 ;
  int JD_flush_vstack_nonint_num = 0 ;
  int JD_pop_acc_dt_num = 0 ;
  int JD_pop_acc_non_single_num = 0 ;
#endif

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif

extern int JD_pli_current_cycle ;
int JD_code_file_index = -1 ;
int JD_code_line_num = -1 ;
int JD_pointcut_file_index = -1 ;
int JD_pointcut_line_num = -1 ;
static int code_statement_num = -1 ;


void JD_code_location() {
  if( JD_code_file_index >= 0 ) {
    PRINTF( 
      "At line %d in \"%s\" : ", 
      JD_code_line_num,
      JD_Debug_name_table[JD_code_file_index] 
    ) ;
  }
}

void JD_dbg_code_location() {
  if( JD_code_file_index >= 0 ) {
    PRINTF( 
      "At line %d in \"%s\" @ %d : ", 
      JD_code_line_num,
      JD_Debug_name_table[JD_code_file_index],
      JD_pli_current_cycle
    ) ;
  }
}

void JD_at_location( int line_num, int file_index ) {
  if( file_index >= 0 ) {
    PRINTF( 
      "at line %d in \"%s\"", 
      line_num,
      JD_Debug_name_table[file_index] 
    ) ;
  }
}

void JD_get_current_location( int *line_num, int *file_index ) {
  *line_num = JD_code_line_num ;
  *file_index = JD_code_file_index ;
}

void JD_set_location( int n, int s, int l ) {
  JD_code_file_index = n ;
  JD_code_line_num = l ;
  code_statement_num = s ;
  /*
  PRINTF( 
    "At line %d in \"%s\" \n", 
     JD_code_line_num,
     JD_Debug_name_table[JD_code_file_index] 
  ) ;
  */
}

void JD_set_pointcut_location() {
  JD_pointcut_file_index = JD_code_file_index ;
  JD_pointcut_line_num = JD_code_line_num ;
}

void JD_get_pointcut_location( int *line_num, int *file_index ) {
  *line_num = JD_pointcut_line_num ;
  *file_index = JD_pointcut_file_index ;
}


/* called from FATAL_ERROR */
void jeda_exit( int n ) {
  exit(n) ;
}

/*****************************************************************************/
/* procedures to handle vstack (something similar to register ring in SPARC) */
/*   VSTACK is an array of JD_data pointer, plus pre-allocated bit space     */
/*   that works as a temp buffer before it is really committed to the stack. */
/*   Since stack area is mostly uses as the temporary store, it is desirable */
/*   to avoid allocation/deallocation of bit field which is not covered on   */
/*   generation GC. This temp buffer is VSP_LIMIT entries. When an bit data  */
/*   with size more than 32, which require to allocate *a, *b, is temporary  */
/*   using ACC_A, ACC_B as is, placed into the ring buffer structure.        */
/*   This case, the data type is marked as JD_D_vbit instead of JD_D_bit to  */
/*   indicate data must be allocated when it's spilled to stack.             */
/*   The ring buffer size is currently set to 8, as following the SPARC      */
/*   architecture. I recall Patterson mentioned that 8 would cover the major */
/*   situation of the temp data.                                             */
/*****************************************************************************/
/* This JD_copy_dt is used to copy a element on the Vstack to a regular
   JD_data structure. As VSTACK contents remains, its content can be 
   cleared, and ref_count for the object (if any) does not needed to be
   incremented.
*/
void JD_copy_dt( JD_data *dt, JD_data *x ) {
  /**MACRO EXEC_COPY_DT(dt,x)**/
  {
    dt->type = x->type ;
    dt->sub_type = x->sub_type ;
    dt->base = x->base ;
    if( x->sub_type != JD_SD_single ) {
      /* blindly copy all 3 entries */
      dt->data.module.func0 = x->data.module.func0 ;
      dt->data.module.func1 = x->data.module.func1 ;
      dt->data.module.data0 = x->data.module.data0 ;
      x->data.module.func0 = NULL ;
      x->data.module.func1 = NULL ;
      x->data.module.data0 = NULL ;
    }
    else {
      switch(x->type) {
      case JD_D_sbit:
        dt->data.sbit.size = x->data.sbit.size ;
        dt->data.sbit.a = x->data.sbit.a ;
        dt->data.sbit.b = x->data.sbit.b ;
        break ;
      case JD_D_vbit:
        {
          int i, n ;
          unsigned int *a, *b ;
          n = ((x->data.bit.size-1)/32)+1 ;
          dt->data.bit.a = ALLOC_BIT( n ) ;
          dt->data.bit.b = ALLOC_BIT( n ) ;
          a = x->data.bit.a ;
          b = x->data.bit.b ; 
          for( i = 0 ; i < n ; i++ ) { 
            dt->data.bit.a[i] = a[i] ; 
            dt->data.bit.b[i] = b[i] ; 
          }
          dt->type = JD_D_bit ;
          dt->data.bit.size = x->data.bit.size ;
        }
        break ;
      case JD_D_bit:
        dt->data.bit.a = x->data.bit.a ;
        dt->data.bit.b = x->data.bit.b ;
        dt->data.bit.size = x->data.bit.size ;
        break ;
      case JD_D_int:
        dt->data.int_value = x->data.int_value ;
        break ;
      case JD_D_float:
        dt->data.float_value = x->data.float_value ;
        break ;
      case JD_D_double:
        dt->data.double_value = x->data.double_value ;
        break ;
      case JD_D_string:
        dt->data.string = x->data.string ;
        break ;
      case JD_D_null:
        break ;
      case JD_D_object:
        dt->data.obj.ect = x->data.obj.ect ;
        dt->data.obj.alloc = x->data.obj.alloc ;
        /* if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ; */
        break ;
      default:
        /* blindly copy all 3 entries */
        dt->data.module.func0 = x->data.module.func0 ;
        dt->data.module.func1 = x->data.module.func1 ;
        dt->data.module.data0 = x->data.module.data0 ;
        break ;
      }
      x->data.module.func0 = NULL ;
      x->data.module.func1 = NULL ;
      x->data.module.data0 = NULL ;
    } 
  }
  /**END MACRO EXEC_COPY_DT**/  
}

/* this one is used to copy ACC_DT to the regular JD_data */
/* Note that ACC_DT can contain the object in the system, so it can't 
   be cleared after the copy.
*/
void JD_set_dt( JD_data *dt, JD_data *x ) {
  /**MACRO EXEC_SET_DT(dt,x)**/
  {
    dt->type = x->type ;
    dt->sub_type = x->sub_type ;
    dt->base = x->base ;
    if( x->sub_type != JD_SD_single ) {
      JD_duplicate_data_to( dt, x ) ;
      /* blindly copy all 3 entries 
      vdt->data.module.func0 = x->data.module.func0 ;
      dt->data.module.func1 = x->data.module.func1 ;
      dt->data.module.data0 = x->data.module.data0 ;
      */
    }
    else {
      switch(x->type) {
      case JD_D_sbit:
        dt->data.sbit.size = x->data.sbit.size ;
        dt->data.sbit.a = x->data.sbit.a ;
        dt->data.sbit.b = x->data.sbit.b ;
        break ;
      case JD_D_vbit:
        {
          int i, n ;
          unsigned int *a, *b ;
          n = ((x->data.bit.size-1)/32)+1 ;
          dt->data.bit.a = ALLOC_BIT( n ) ;
          dt->data.bit.b = ALLOC_BIT( n ) ;
          a = x->data.bit.a ;
          b = x->data.bit.b ; 
          for( i = 0 ; i < n ; i++ ) { 
            dt->data.bit.a[i] = a[i] ; 
            dt->data.bit.b[i] = b[i] ; 
          }
          dt->type = JD_D_bit ;
          dt->data.bit.size = x->data.bit.size ;
        }
        break ;
      case JD_D_bit:
        dt->data.bit.a = x->data.bit.a ;
        dt->data.bit.b = x->data.bit.b ;
        dt->data.bit.size = x->data.bit.size ;
        break ;
      case JD_D_int:
        dt->data.int_value = x->data.int_value ;
        break ;
      case JD_D_float:
        dt->data.float_value = x->data.float_value ;
        break ;
      case JD_D_double:
        dt->data.double_value = x->data.double_value ;
        break ;
      case JD_D_string:
        dt->data.string = x->data.string ;
        break ;
      case JD_D_null:
        break ;
      case JD_D_object:
        dt->data.obj.ect = x->data.obj.ect ;
        dt->data.obj.alloc = x->data.obj.alloc ;
        if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
        break ;
      default:
        /* blindly copy all 3 entries */
        dt->data.module.func0 = x->data.module.func0 ;
        dt->data.module.func1 = x->data.module.func1 ;
        dt->data.module.data0 = x->data.module.data0 ;
        break ;
      }
    } 
  }
  /**END MACRO EXEC_SET_DT**/  
}

JD_data *JD_pop_stack() {
  JD_data *ret ;
  
  if( VSP ) {
    if( VS_TOP ) VS_TOP-- ;
    else VS_TOP = VSP_LIMIT-1 ;
    VSP-- ;
    ret = JD_alloc_data_entry() ;
    JD_copy_dt( ret, VSTACK[VS_TOP] ) ;
    VSTACK[VS_TOP]->sub_type = JD_SD_single ;
#ifdef NOTIFY_GC_STATUS
    if( ret->type == JD_D_int )
      JD_pop_stack_int_num++ ;
#endif
#ifdef VSTACK_PROFILE
    VSTACK_HIT++ ;
#endif
  }
  else {
    ret = STACK[--SP] ;
    if( IS_SMLINT(ret) ) {
      int i ;
      i = SMLINT2INT(ret) ;
      ret = JD_alloc_data_entry() ;
      ret->type = JD_D_int ;
      ret->data.int_value = i ;
      ret->sub_type = JD_SD_single ;
    }
#ifdef VSTACK_PROFILE
    VSTACK_MISS++ ;
#endif
  }
  return ret ;
}

void JD_up_stack() {
  
  if( VSP ) {
    if( VS_TOP ) VS_TOP-- ;
    else VS_TOP = VSP_LIMIT-1 ;
    VSP-- ;
    VSTACK[VS_TOP]->sub_type = JD_SD_single ;
#ifdef VSTACK_PROFILE
    VSTACK_HIT++ ;
#endif
  }
  else {
    SP-- ;
#ifdef VSTACK_PROFILE
    VSTACK_MISS++ ;
#endif
  }
}

int JD_pop_stack_int() {
  int ret ;
  JD_data *dt ;
  
  if( VSP ) {
#ifdef VSTACK_PROFILE
    VSTACK_HIT++ ;
#endif
    if( VS_TOP ) VS_TOP-- ;
    else VS_TOP = VSP_LIMIT-1 ;
    VSP-- ;
    return VSTACK[VS_TOP]->data.int_value ;
  }
  else {
    dt = STACK[--SP] ;
    if( IS_SMLINT(dt) ) 
      ret = SMLINT2INT(dt) ;
    else
      ret = dt->data.int_value ;
#ifdef ENABLE_JEDA_DATA_CACHE
    JD_dealloc_data_on_cache(dt, STACK[SP-1]) ;
#endif
#ifdef VSTACK_PROFILE
    VSTACK_MISS++ ;
#endif
  }
  return ret ;
}

void JD_pop_bit_to_accb() {
  int size, i, n ;
  JD_data *dt ;
  
  if( VSP ) {
#ifdef VSTACK_PROFILE
    VSTACK_HIT++ ;
#endif
    if( VS_TOP ) VS_TOP-- ;
    else VS_TOP = VSP_LIMIT-1 ;
    VSP-- ;
    if( VSTACK[VS_TOP]->type == JD_D_sbit ) {
      ACCB_A[0] =  VSTACK[VS_TOP]->data.sbit.a ;
      ACCB_B[0] =  VSTACK[VS_TOP]->data.sbit.b ;
      ACCB_SIZE = VSTACK[VS_TOP]->data.sbit.size ;
    }
    else {
      ACCB_SIZE =  VSTACK[VS_TOP]->data.bit.size ; ;
      n = LASTWORD(ACCB_SIZE) ;
      if( ACCB_WD < n ) { 
        ACCB_A = (unsigned int *)realloc( ACCB_A, NBYTE(ACCB_SIZE) ) ; 
        ACCB_B = (unsigned int *)realloc( ACCB_B, NBYTE(ACCB_SIZE) ) ; 
        ACCB_WD = n ;
      } 
      for( i = 0 ; i <= n ; i++ ) {
        ACCB_A[i] = VSTACK[VS_TOP]->data.bit.a[i] ;
        ACCB_B[i] = VSTACK[VS_TOP]->data.bit.b[i] ;
      }
    }
  }
  else {
    dt = STACK[--SP] ;
    if( dt->type == JD_D_sbit ) {
      ACCB_A[0] =  dt->data.sbit.a ;
      ACCB_B[0] =  dt->data.sbit.b ;
      ACCB_SIZE = dt->data.sbit.size ;
#ifdef ENABLE_JEDA_DATA_CACHE
      JD_dealloc_data_on_cache(dt, STACK[SP-1]) ;
#endif
    }
    else {
      ACCB_SIZE =  dt->data.bit.size ; ;
      n = LASTWORD(ACCB_SIZE) ;
      if( ACCB_WD < n ) { 
        ACCB_A = (unsigned int *)realloc( ACCB_A, NBYTE(ACCB_SIZE) ) ; 
        ACCB_B = (unsigned int *)realloc( ACCB_B, NBYTE(ACCB_SIZE) ) ; 
        ACCB_WD = n ;
      } 
      for( i = 0 ; i <= n ; i++ ) {
        ACCB_A[i] = dt->data.bit.a[i] ;
        ACCB_B[i] = dt->data.bit.b[i] ;
      }
    }
#ifdef VSTACK_PROFILE
    VSTACK_MISS++ ;
#endif
  }
}


JD_data *JD_get_stack(int x) {
  JD_data *ret ;
  
  if( VSP >= x ) {
    if( VS_TOP >= x ) ret = VSTACK[VS_TOP - x] ; 
    else ret = VSTACK[VSP_LIMIT + VS_TOP - x] ;
  }
  else {
    ret = STACK[SP-(x-VSP)] ;
  }
  return ret ;
}

void JD_push_vstack( JD_data *data ) {
  /**MACRO EXEC_PUSH_VSTACK**/
  {
    if(VSP==VSP_LIMIT) {  /* vstack full, flush one entry */
      JD_data *dt ;
      if( FITSMLINT(VSTACK[VS_TOP]) ) {
        dt = GENSMLINT(VSTACK[VS_TOP]) ;
#ifdef NOTIFY_GC_STATUS
          JD_vstack_ovflw_smlint_num++ ;
#endif
      }
      else {
        dt = JD_alloc_data_entry() ;
        JD_copy_dt( dt, VSTACK[VS_TOP] ) ;
#ifdef NOTIFY_GC_STATUS
        if( dt->type == JD_D_int )
          JD_vstack_ovflw_int_num++ ;
        else 
          JD_vstack_ovflw_nonint_num++ ;
#endif
#ifdef JEDA_DATA_TRACE
        dt->file_name = VSTACK[VS_TOP]->file_name  ;
        dt->location = VSTACK[VS_TOP]->location  ;
#endif
      }
      PUSH_TO_STACK(dt) ;
      VSP-- ;
#ifdef VSTACK_PROFILE
      VSTACK_FULL++ ;
#endif
    }
#ifdef JEDA_DATA_TRACE
    VSTACK[VS_TOP]->file_name = JD_code_file_index ;
    VSTACK[VS_TOP]->location = JD_code_line_num ;
#endif
    VSTACK[VS_TOP] = data ;
    VS_TOP = (VS_TOP+1) % VSP_LIMIT ;
    VSP++ ;
  }
  /**END MACRO EXEC_PUSH_VSTACK**/  
}

void JD_push_acc() {
  /**MACRO EXEC_PUSH_ACC**/
  {
    JD_data *dt ;
    if(VSP==VSP_LIMIT) {  /* vstack full, flush one entry */
      if( FITSMLINT(VSTACK[VS_TOP]) ) {
        dt = GENSMLINT(VSTACK[VS_TOP]) ;
#ifdef NOTIFY_GC_STATUS
          JD_vstack_ovflw_smlint_num++ ;
#endif
      }
      else {
        dt = JD_alloc_data_entry() ;
        JD_copy_dt( dt, VSTACK[VS_TOP] ) ;
#ifdef NOTIFY_GC_STATUS
        if( dt->type == JD_D_int )
          JD_vstack_ovflw_int_num++ ;
        else 
          JD_vstack_ovflw_nonint_num++ ;
#endif
#ifdef JEDA_DATA_TRACE
        dt->file_name = VSTACK[VS_TOP]->file_name  ;
        dt->location = VSTACK[VS_TOP]->location  ;
#endif
      }
      PUSH_TO_STACK(dt) ;
      VSP-- ;
    }
    dt = VSTACK[VS_TOP] ;
    dt->data.list.top = NULL ;
    dt->data.list.bottom = NULL ;
    dt->data.list.sem = NULL ;
    dt->base = NULL ;
#ifdef JEDA_DATA_TRACE
    dt->file_name = JD_code_file_index ;
    dt->location = JD_code_line_num ;
#endif
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        dt->sub_type = JD_SD_single ;
        if( ACC_SIZE <= 32 ) {
          dt->type = JD_D_sbit ;
          dt->data.sbit.a = ACC_A[0] ;
          dt->data.sbit.b = ACC_B[0] ;
          dt->data.sbit.size = ACC_SIZE ;
        }
        else {
          int i, n ;
          unsigned int *taba, *tabb ;
          dt->type = JD_D_vbit ;
          n = NWORD(ACC_SIZE) ;
          if( n > VS_WD[VS_TOP] ) {
            VS_A[VS_TOP] = 
              (unsigned int *)realloc( VS_A[VS_TOP], NBYTE(ACC_SIZE) ) ;
            VS_B[VS_TOP] = 
              (unsigned int *)realloc( VS_B[VS_TOP], NBYTE(ACC_SIZE) ) ;
            VS_WD[VS_TOP] = n ;
          }
          taba = VS_A[VS_TOP] ;
          tabb = VS_B[VS_TOP] ;
          for( i = 0 ; i < n ; i++ ) {
            taba[i] = ACC_A[i] ;
            tabb[i] = ACC_B[i] ;
          }
          dt->data.bit.a = (int *)VS_A[VS_TOP] ;
          dt->data.bit.b = (int *)VS_B[VS_TOP] ;
          dt->data.bit.size = ACC_SIZE ;
        }
        break ;
      case JD_D_int:
        dt->sub_type = JD_SD_single ;
        dt->type = JD_D_int ;
        dt->data.int_value = ACC_INT ;
        break ;
      case JD_D_float:
        dt->sub_type = JD_SD_single ;
        dt->type = JD_D_float ;
        dt->data.float_value = ACC_FLOAT ;
        break ;
      case JD_D_double:
        dt->sub_type = JD_SD_single ;
        dt->type = JD_D_double ;
        dt->data.double_value = ACC_FLOAT ;
        break ;
      case JD_D_string:
        dt->sub_type = JD_SD_single ;
        dt->type = JD_D_string ;
        dt->data.string = ACC_STRING ;
        ACC_STRING = NULL ;
        ACC_TYPE = JD_D_void ;
        break ;
      case JD_D_dt:
        if( ACC_DT ) {
          JD_set_dt( dt, ACC_DT ) ;
          ACC_DT = NULL ;
          ACC_TYPE = JD_D_null ;
        }
        else {
          dt->type = JD_D_null ;
          dt->sub_type = JD_SD_single ;
          ACC_TYPE = JD_D_null ;
        }
        break ;
      case JD_D_null:
        dt->type = JD_D_null ;
        dt->sub_type = JD_SD_single ;
        break ;
      default:
        dt->type = JD_D_null ;
        dt->sub_type = JD_SD_single ;
        break ;
    }
    
    VS_TOP = (VS_TOP+1) % VSP_LIMIT ;
    VSP++ ;
  }
  /**END MACRO EXEC_PUSH_ACC**/  
}


void JD_flush_vstack() {
  /**MACRO EXEC_FLUSH_VSTACK**/
  {
    JD_data *dt, *st ;
    while(VSP) {
      if( VS_TOP < VSP ) {
        st = VSTACK[VSP_LIMIT+VS_TOP-VSP] ;
      }
      else {
        st = VSTACK[VS_TOP-VSP] ;
      }
      if( FITSMLINT(st) ) {
        dt = GENSMLINT(st) ;
#ifdef NOTIFY_GC_STATUS
        JD_flush_vstack_smlint_num++ ;
#endif
      }
      else {
        dt = JD_alloc_data_entry() ;
        JD_copy_dt( dt, st ) ;
        st->sub_type = JD_SD_single ;
#ifdef NOTIFY_GC_STATUS
        if( dt->type == JD_D_int )
          JD_flush_vstack_int_num++ ;
        else
          JD_flush_vstack_nonint_num++ ;
#endif
      }
      PUSH_TO_STACK(dt) ;
      VSP-- ;
    }
  }
  /**END MACRO EXEC_FLUSH_VSTACK**/  
}

void JD_set_acc( JD_data *x ) {
  /**MACRO EXEC_SET_ACC(x)**/
  {
    if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    if( x->sub_type != JD_SD_single ) {
      ACC_DT = x ;
      ACC_TYPE = JD_D_dt ;
    }
    else {
      ACC_TYPE = x->type ;
      switch(ACC_TYPE) {
      case JD_D_sbit:
        ACC_SIZE = x->data.sbit.size ;
        ACC_A[0] = x->data.sbit.a ;
        ACC_B[0] = x->data.sbit.b ;
        break ;
      case JD_D_vbit:
      case JD_D_bit:
        {
          int i, n ;
          n = LASTWORD(x->data.bit.size) ;
          if( ACC_WD <= n ) {  /* extending ACC bit space */
            ACC_A = 
              (unsigned int *)realloc( ACC_A, NBYTE(x->data.bit.size) ) ;
            ACC_B = 
              (unsigned int *)realloc( ACC_B, NBYTE(x->data.bit.size) ) ;
            ACC_WD = n+1 ;
          }
          for( i = 0 ; i <= n ; i++ ) {
            ACC_A[i] = x->data.bit.a[i] ;
            ACC_B[i] = x->data.bit.b[i] ;
          }
          ACC_SIZE = x->data.bit.size ;
        }
        break ;
      case JD_D_int:
        ACC_INT = x->data.int_value ;
        break ;
      case JD_D_float:
        ACC_FLOAT = x->data.float_value ;
        break ;
      case JD_D_double:
        ACC_FLOAT = x->data.double_value ;
        break ;
      case JD_D_string:
        if( x->data.string ) 
          ACC_STRING = (char *)strdup(x->data.string) ;
        break ;
      case JD_D_null:
        break ;
      default:
        ACC_DT = x ;
        ACC_TYPE = JD_D_dt ;
        break ;
      }
    } 
  }
  /**END MACRO EXEC_SET_ACC**/  
}

void JD_copy_acc( JD_data *x ) {
  /**MACRO EXEC_COPY_ACC(x)**/
  {
    if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    if( x->sub_type != JD_SD_single ) {
      ACC_DT = x ;
      ACC_TYPE = JD_D_dt ;
    }
    else {
      ACC_TYPE = x->type ;
      switch(ACC_TYPE) {
      case JD_D_sbit:
        ACC_SIZE = x->data.sbit.size ;
        ACC_A[0] = x->data.sbit.a ;
        ACC_B[0] = x->data.sbit.b ;
        break ;
      case JD_D_vbit:
      case JD_D_bit:
        {
          int i, n ;
          n = LASTWORD(x->data.bit.size) ;
          if( ACC_WD <= n ) {  /* extending ACC bit space */
            ACC_A = 
              (unsigned int *)realloc( ACC_A, NBYTE(x->data.bit.size) ) ;
            ACC_B = 
              (unsigned int *)realloc( ACC_B, NBYTE(x->data.bit.size) ) ;
            ACC_WD = n+1 ;
          }
          for( i = 0 ; i <= n ; i++ ) {
            ACC_A[i] = x->data.bit.a[i] ;
            ACC_B[i] = x->data.bit.b[i] ;
          }
          ACC_SIZE = x->data.bit.size ;
        }
        break ;
      case JD_D_int:
        ACC_INT = x->data.int_value ;
        break ;
      case JD_D_float:
        ACC_FLOAT = x->data.float_value ;
        break ;
      case JD_D_double:
        ACC_FLOAT = x->data.double_value ;
        break ;
      case JD_D_string:
        if( x->data.string )
          ACC_STRING = (char *)strdup(x->data.string) ;
        else
          ACC_STRING = NULL ;
        break ;
      case JD_D_null:
        break ;
      default:
        ACC_DT = x ;
        ACC_TYPE = JD_D_dt ;
        break ;
      }
    }  
  }
  /**END MACRO EXEC_COPY_ACC**/  
}

void JD_acc_to_dt(JD_data *x) {
  /**MACRO EXEC_ACC_TO_DT(x)**/
  {
    int mask ;
    /* at this moment, basic type of data (x) and ACC is the same,
       but bit size is not the same
    */
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        switch(x->type) {
          case JD_D_sbit:
            mask = BITMASK(x->data.sbit.size) ;
            STORE_SIZE = x->data.sbit.size ;
            x->data.sbit.a = mask & ACC_A[0] ;
            x->data.sbit.b = mask & ACC_B[0] ;
#ifdef JEDA_VERILOG_VAR_UPDATE
            if( IS_STATICDATA(x) ) 
              JD_update_static_var(x) ;
#endif
            break ;
          case JD_D_bit:
            {
              int i, n, nn ;
              mask = BITMASK(x->data.bit.size) ;
              STORE_SIZE = x->data.bit.size ;
              n = NWORD(ACC_SIZE) ;
              nn = NWORD(STORE_SIZE) ;
              for( i = 0 ; i < nn ; i++ ) {
                if( i == n-1 ) {
                  x->data.bit.a[i] = mask & ACC_A[i] ;
                  x->data.bit.b[i] = mask & ACC_B[i] ;
                }
                else if( i < n-1 ) {
                  x->data.bit.a[i] = ACC_A[i] ;
                  x->data.bit.b[i] = ACC_B[i] ;
                }
                else {
                  x->data.bit.a[i] = 0 ;
                  x->data.bit.b[i] = 0 ;
                }
              }
#ifdef JEDA_VERILOG_VAR_UPDATE
              if( IS_STATICDATA(x) ) 
                JD_update_static_var(x) ;
#endif
            }
            break ;
        }
        break ;
      case JD_D_int:
        STORE_SIZE = 32 ;
        x->data.int_value = ACC_INT;
        break ;
      case JD_D_float:
      case JD_D_double:
        if( x->type == JD_D_float )
          x->data.float_value = ACC_FLOAT ;
        else
          x->data.double_value = ACC_FLOAT ;
        break ;
      case JD_D_string:
        switch( x->type ) {
          case JD_D_string:
            if( x->data.string ) 
              DEALLOC_STRING(x->data.string) ;
            x->data.string = ACC_STRING ;
            ACC_STRING = NULL ;
            break ;
          case JD_D_sbit:
            {
              int nb, j ;
              nb = (x->data.sbit.size)/8 ;
              j = 0 ;
              x->data.sbit.a = x->data.sbit.b = 0 ;
              for(  ; nb > 0 ; nb-- ) {
                x->data.sbit.a = 
                  (x->data.sbit.a << 8) | 
                  ( ( ACC_STRING[j] != NULL_CHAR)?ACC_STRING[j++]:0 ) ;
              }
            }
            break ;
          case JD_D_bit:
            {
              int nw, nb, size, j ;
              size = x->data.bit.size/8 ;
              size *= 8 ; /* round to 8 */
              nw = LASTWORD(size) ;
              j = 0 ;
              nb = (size%32)/8 ;
              if( nb == 0 ) nb = 4 ;
              for(  ; nw >= 0 ; nw-- ) {
                x->data.bit.a[nw] = x->data.bit.b[nw] = 0 ;
                for(  ; nb > 0 ; nb-- ) {
                  x->data.bit.a[nw] = 
                    (x->data.bit.a[nw] << 8) | 
                    ( ( ACC_STRING[j] != NULL_CHAR)?ACC_STRING[j++]:0 ) ;
                }
                nb = 4 ;
              }
            }
            break ;
        }
        break ;
      case JD_D_null:
        switch( x->type ) {
          case JD_D_string:
            x->data.string = NULL ;
            break ;
          case JD_D_signal:
            x->data.signal.port = NULL ;
            break ;
          case JD_D_object:
            if( x->data.obj.ect && (-- x->data.obj.ect->ref_count) == 0 ) {
              (x->data.obj.ect->dealloc)( 
                (void *)x->data.obj.ect->table,
                x->data.obj.ect->num_entry,
                x->data.obj.ect->tbl_size
              ) ;
              DEALLOC_OBJ_TABLE(x->data.obj.ect) ;
            }
            x->data.obj.ect = NULL ;
        }
        break ;
      default:
        if( ACC_DT->type == JD_D_object ) {
          if( ACC_DT->data.obj.ect ) ACC_DT->data.obj.ect->ref_count++ ;
          if( x->data.obj.ect && (-- x->data.obj.ect->ref_count) == 0 ) {
            (x->data.obj.ect->dealloc)( 
              (void *)x->data.obj.ect->table,
              x->data.obj.ect->num_entry,
              x->data.obj.ect->tbl_size
            ) ;
            DEALLOC_OBJ_TABLE(x->data.obj.ect) ;
          }
#ifndef  NO_OBJ_TYPE_CHECK
          if( x->data.obj.alloc && 
              x->data.obj.alloc != ACC_DT->data.obj.alloc ) 
          {
            ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
          }
#endif
          x->data.obj.ect = ACC_DT->data.obj.ect ;
          x->data.obj.alloc = ACC_DT->data.obj.alloc ;
        }
        else JD_copy_data(x, ACC_DT) ;
        break ;
    }
    
  }
  /**END MACRO EXEC_ACC_TO_DT**/  
}

void JD_set_acc_to_dt(JD_data *x) {
  /**MACRO EXEC_SET_ACC_TO_DT(x)**/
  {
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        x->type = ACC_TYPE ;
        if( ACC_SIZE <= 32 ) {
          x->data.sbit.size = ACC_SIZE ;
          x->data.sbit.a = ACC_A[0] ;
          x->data.sbit.b = ACC_B[0] ;
          break ;
        }
        else {
          int i, n ;
          x->type = JD_D_bit ;
          x->data.bit.size = ACC_SIZE ;
          n = ((ACC_SIZE-1)/32)+1 ;
          x->data.bit.a = ALLOC_BIT( n ) ;
          x->data.bit.b = ALLOC_BIT( n ) ;
          for( i = 0 ; i < n ; i++ ) { 
            x->data.bit.a[i] = ACC_A[i] ;
            x->data.bit.a[i] = ACC_B[i] ;
          }
          x->data.bit.size = ACC_SIZE;
        }
#ifdef JEDA_VERILOG_VAR_UPDATE
        if( IS_STATICDATA(x) ) 
          JD_update_static_var(x) ;
#endif
        break ;
      case JD_D_int:
        x->type = ACC_TYPE ;
        x->data.int_value = ACC_INT;
#ifdef JEDA_VERILOG_VAR_UPDATE
        if( IS_STATICDATA(x) ) 
          JD_update_static_var(x) ;
#endif
        break ;
      case JD_D_float:
        x->type = ACC_TYPE ;
        x->data.float_value = ACC_FLOAT ;
        break ;
      case JD_D_double:
        x->type = ACC_TYPE ;
        x->data.double_value = ACC_FLOAT ;
        break ;
      case JD_D_string:
        x->type = ACC_TYPE ;
        x->data.string = ACC_STRING ;
        ACC_STRING = NULL ;
        break ;
      case JD_D_null:
        switch( x->type ) {
          case JD_D_string:
            x->data.string = NULL ;
            break ;
          case JD_D_signal:
            x->data.signal.port = NULL ;
            break ;
          case JD_D_object:
            if( x->data.obj.ect && (-- x->data.obj.ect->ref_count) == 0 ) {
              (x->data.obj.ect->dealloc)( 
                (void *)x->data.obj.ect->table,
                x->data.obj.ect->num_entry,
                x->data.obj.ect->tbl_size
              ) ;
              DEALLOC_OBJ_TABLE(x->data.obj.ect) ;
            }
            x->data.obj.ect = NULL ;
        }
        break ;
      default:
        if( ACC_DT->type == JD_D_object ) {
          if( x->data.obj.ect && (-- x->data.obj.ect->ref_count) == 0 ) {
            (x->data.obj.ect->dealloc)( 
              (void *)x->data.obj.ect->table,
              x->data.obj.ect->num_entry,
              x->data.obj.ect->tbl_size
            ) ;
            DEALLOC_OBJ_TABLE(x->data.obj.ect) ;
          }
#ifndef  NO_OBJ_TYPE_CHECK
          if( x->data.obj.alloc && 
              x->data.obj.alloc != ACC_DT->data.obj.alloc ) 
          {
            ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
          }
#endif
          x->data.obj.ect = ACC_DT->data.obj.ect ;
          x->data.obj.alloc = ACC_DT->data.obj.alloc ;
          if( x->data.obj.ect ) x->data.obj.ect->ref_count++ ;
        }
        else JD_copy_data(x, ACC_DT) ;
        break ;
    }
    
  }
  /**END MACRO EXEC_SET_ACC_TO_DT**/  
}

void JD_null_acc( JD_data *dt ) {
  /**MACRO EXEC_NULL_ACC(dt)**/
  {
    int i, n ;
    if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    ACC_TYPE = dt->type ;
    switch(ACC_TYPE) {
      case JD_D_sbit:
        ACC_TYPE = JD_D_bit ;
        ACC_SIZE = (int)dt->base ;
        ACC_A[0] = ACC_B[0] = (ACC_SIZE==32)?0xffffffff:(1<<ACC_SIZE)-1 ;
        break ;
      case JD_D_vbit:
      case JD_D_bit:
        ACC_SIZE = (int)dt->base ;
        n = LASTWORD(ACC_SIZE) ;
        if( ACC_WD <= n ) {  /* extending ACC bit space */
          ACC_A = 
            (unsigned int *)realloc( ACC_A, NBYTE(dt->data.bit.size) ) ;
          ACC_B = 
            (unsigned int *)realloc( ACC_B, NBYTE(dt->data.bit.size) ) ;
          ACC_WD = n+1 ;
        }
        for( i = 0 ; i < n ; i++ ) {
          ACC_A[i] = ACC_B[i] = 0xffffffff ;
        }
        ACC_A[n] = ACC_B[n] = (ACC_SIZE%32)?((1<<(ACC_SIZE%32))-1):0xffffffff ;
        break ;
      case JD_D_int:
        ACC_INT = 0 ;
        break ;
      case JD_D_float:
        ACC_FLOAT = 0.0 ;
        break ;
      case JD_D_double:
        ACC_FLOAT = 0.0 ;
        break ;
      case JD_D_string:
        ACC_STRING = NULL ;
        break ;
      default:
        ACC_TYPE = JD_D_null ;
        ACC_DT = NULL ;
        break ;
    }
    
  }
  /**END MACRO EXEC_NULL_ACC**/  
}

void JD_pop_acc() {
  /**MACRO EXEC_POP_ACC**/
  {  
    JD_data *dt ;
    if( VSP ) {
      if( VS_TOP ) VS_TOP-- ;
      else VS_TOP = VSP_LIMIT-1 ;
      VSP-- ;
      dt = VSTACK[VS_TOP] ;
#ifdef VSTACK_PROFILE
      VSTACK_HIT++ ;
#endif
      if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
        DEALLOC_STRING(ACC_STRING) ;
        ACC_STRING = NULL ;
      }
      if( dt->sub_type == JD_SD_single ) {
        ACC_TYPE = dt->type ;
        switch(ACC_TYPE) {
          case JD_D_sbit:
            ACC_SIZE = dt->data.sbit.size ;
            ACC_A[0] = dt->data.sbit.a ;
            ACC_B[0] = dt->data.sbit.b ;
            ACC_TYPE = JD_D_bit ;
            break ;
          case JD_D_vbit:
          case JD_D_bit:
            {
              int tmp ;
              unsigned int *tab ;
              tab = VS_A[VS_TOP] ;
              VS_A[VS_TOP] = ACC_A ;
              ACC_A = tab ;
              tab = VS_B[VS_TOP] ;
              VS_B[VS_TOP] = ACC_B ;
              ACC_B = tab ;
              tmp = VS_WD[VS_TOP] ;
              VS_WD[VS_TOP] = ACC_WD ;
              ACC_WD = tmp ;
              ACC_TYPE = JD_D_bit ;
            }
            dt->type = JD_D_int ;
            dt->data.list.top = NULL ;
            dt->data.list.bottom = NULL ;
            dt->data.list.sem = NULL ;
            dt->base = NULL ;
            break ;
          case JD_D_int:
            ACC_INT = dt->data.int_value ;
            break ;
          case JD_D_float:
            ACC_FLOAT = dt->data.float_value ;
            break ;
          case JD_D_double:
            ACC_FLOAT = dt->data.double_value ;
            break ;
          case JD_D_string:
            ACC_STRING = dt->data.string ;
            dt->type = JD_D_int ;
            dt->data.list.top = NULL ;
            dt->data.list.bottom = NULL ;
            dt->data.list.sem = NULL ;
            dt->base = NULL ;
            break ;
          case JD_D_null:
            break ;
          default:
            ACC_TYPE = JD_D_dt ;
            ACC_DT = dt ;
#ifdef NOTIFY_GC_STATUS
            JD_pop_acc_dt_num++ ;
#endif
            VSTACK[VS_TOP] = JD_alloc_data_entry() ;
            break ;
        }
      }
      else {
        ACC_TYPE = JD_D_dt ;
        ACC_DT = dt ;
        VSTACK[VS_TOP] = JD_alloc_data_entry() ;
#ifdef NOTIFY_GC_STATUS
        JD_pop_acc_non_single_num++ ;
#endif
      }
    }
    else {
      dt = STACK[--SP] ;
#ifdef VSTACK_PROFILE
      VSTACK_MISS++ ;
#endif
      if( IS_SMLINT(dt) ) {
        ACC_INT = SMLINT2INT(dt) ;
        ACC_TYPE = JD_D_int ;
      }
      else {
        if( dt->sub_type == JD_SD_single ) {
          ACC_TYPE = dt->type ;
          switch(ACC_TYPE) {
            case JD_D_sbit:
              ACC_SIZE = dt->data.sbit.size ;
              ACC_A[0] = dt->data.sbit.a ;
              ACC_B[0] = dt->data.sbit.b ;
#ifdef ENABLE_JEDA_DATA_CACHE
              JD_dealloc_data_on_cache(dt, STACK[SP-1]) ;
#endif
              break ;
            case JD_D_vbit:
            case JD_D_bit:
              {
                int i, n ;
                n = LASTWORD(dt->data.bit.size) ;
                if( ACC_WD <= n ) {  /* extending ACC bit space */
                  ACC_A = 
                    (unsigned int *)realloc( ACC_A, NBYTE(dt->data.bit.size) ) ;
                  ACC_B = 
                    (unsigned int *)realloc( ACC_B, NBYTE(dt->data.bit.size) ) ;
                  ACC_WD = n+1 ;
                }
                for( i = 0 ; i <= n ; i++ ) {
                  ACC_A[i] = dt->data.bit.a[i] ;
                  ACC_B[i] = dt->data.bit.b[i] ;
                }
                ACC_SIZE = dt->data.bit.size ;
              }
              break ;
            case JD_D_int:
              ACC_INT = dt->data.int_value ;
#ifdef ENABLE_JEDA_DATA_CACHE
              if( SP ) JD_dealloc_data_on_cache(dt, STACK[SP-1]) ;
#endif
              break ;
            case JD_D_float:
              ACC_FLOAT = dt->data.float_value ;
              break ;
            case JD_D_double:
              ACC_FLOAT = dt->data.double_value ;
              break ;
            case JD_D_string:
              ACC_STRING = (char *)strdup(dt->data.string) ;
              break ;
            case JD_D_null:
              break ;
            default:
              ACC_TYPE = JD_D_dt ;
              ACC_DT = dt ;
              break ;
          }
        }
        else {
          ACC_TYPE = JD_D_dt ;
          ACC_DT = dt ;
        }
      } /* else of IS_SMLINT(dt) */
    }
  }
  /**END MACRO EXEC_POP_ACC**/  
}

/*****************************************************************************/
/* stack handling operations                                                 */
/*****************************************************************************/
/*
 4. Stack Operation
  pop <n>               : pop n-enties from stack and discard
  push alu              :  stac[sp++] <- ACC
  pop alu               :  ACC <- stac[--sp]
  copy alu <n>          :  ACC <- stack[sp-n-1] 
*/

/*
  'copy' 'alu' Decimal_num EOL
  <<op_subop_index>>
*/
void JD_exec_copy_alu() { /* ACC_INT holds the index value */
  /**MACRO EXEC_COPY_ALU**/
  {
    JD_data *dt ;
    if( VSP >= ACC_INT ) {
      if( VS_TOP < ACC_INT ) dt = VSTACK[VSP_LIMIT+VS_TOP-ACC_INT] ;
      else dt = VSTACK[VS_TOP-ACC_INT] ;
      CALL_EXEC_COPY_ACC(dt) ;
    }
    else {
      dt = STACK[SP-VSP-ACC_INT] ;
      if( IS_SMLINT(dt) ) {
        ACC_INT = SMLINT2INT(dt) ;
        ACC_TYPE = JD_D_int ;
      }
      else CALL_EXEC_COPY_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_COPY_ALU**/  
}

void JD_exec_pop_alu() { /* ACC_INT holds the index value */
  /**MACRO EXEC_POP_ALU**/
  {
    JD_data *dt ;
  
    if( VSP ) {
      if( VS_TOP ) VS_TOP-- ;
      else VS_TOP = VSP_LIMIT-1 ;
      VSP-- ;
      CALL_EXEC_SET_ACC( VSTACK[VS_TOP] ) ;
#ifdef VSTACK_PROFILE
      VSTACK_HIT++ ;
#endif
    }
    else {
      dt = STACK[--SP] ;
      if( IS_SMLINT(dt) ) {
        ACC_INT = SMLINT2INT(dt) ;
        ACC_TYPE = JD_D_int ;
      }
      else CALL_EXEC_SET_ACC(dt) ;
#ifdef ENABLE_JEDA_DATA_CACHE
      if( SP ) JD_dealloc_data_on_cache(dt, STACK[SP-1]) ;
#endif
#ifdef VSTACK_PROFILE
      VSTACK_MISS++ ;
#endif
    }
  }
  /**END MACRO EXEC_POP_ALU**/  
}

void JD_exec_pop_n() { /* TMP_INT holds the index value */
  /**MACRO EXEC_POP_N**/
  {
    JD_data *dt ;
    while( VSP && TMP_INT ) {
      if( VS_TOP ) VS_TOP-- ;
      else VS_TOP = VSP_LIMIT-1 ;
      VSP-- ;
      TMP_INT-- ;
    }
    
    while( TMP_INT ) {
#ifdef ENABLE_JEDA_DATA_CACHE
      dt = STACK[--SP] ;
      if( SP ) JD_dealloc_data_on_cache( dt, STACK[SP-1] ) ;
#else
      SP-- ;
#endif
      TMP_INT-- ;
    }
  }
  /**END MACRO EXEC_POP_N**/  
}

/*****************************************************************************/
/* lvar: allocating local variable on stack                                  */
/*  'lvar' Decimal_num data_type name array_range opt_flags EOL              */
/* Following routine must be called after flush_vstack()                     */
/*****************************************************************************/

/*************************************************/
/* single data                                   */
/*************************************************/
void JD_exec_lvar_single_int() {
  /**MACRO EXEC_LVAR_SINGLE_INT**/
  {
    CALL_ALLOC_DT_SINGLE_INT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_INT**/  
}

void JD_exec_lvar_single_bit() {
  /**MACRO EXEC_LVAR_SINGLE_BIT**/
  {
    CALL_ALLOC_DT_SINGLE_BIT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_BIT**/  
}

void JD_exec_lvar_single_float() {
  /**MACRO EXEC_LVAR_SINGLE_FLOAT**/
  {
    CALL_ALLOC_DT_SINGLE_FLOAT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_FLOAT**/  
}

void JD_exec_lvar_single_double() {
  /**MACRO EXEC_LVAR_SINGLE_DOUBLE**/
  {
    CALL_ALLOC_DT_SINGLE_DOUBLE ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_DOUBLE**/  
}

void JD_exec_lvar_single_string() {
  /**MACRO EXEC_LVAR_SINGLE_STRING**/
  {
    CALL_ALLOC_DT_SINGLE_STRING ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_STRING**/  
}

void JD_exec_lvar_single_signal() {
  /**MACRO EXEC_LVAR_SINGLE_SIGNAL**/
  {
    CALL_ALLOC_DT_SINGLE_SIGNAL ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_lvar_single_obj() {
  /**MACRO EXEC_LVAR_SINGLE_OBJ**/
  {  
    CALL_ALLOC_DT_SINGLE_OBJ ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_OBJ**/  
}

void JD_exec_lvar_single_thread() {
  /**MACRO EXEC_LVAR_SINGLE_THREAD**/
  {  
    CALL_ALLOC_DT_SINGLE_THREAD ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_SINGLE_THREAD**/  
}

/*************************************************/
/* array data ARRAY_SIZE should hold the size    */
/*************************************************/
void JD_exec_lvar_array_int() {
  /**MACRO EXEC_LVAR_ARRAY_INT**/
  {
    CALL_ALLOC_DT_ARRAY_INT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_INT**/  
}

void JD_exec_lvar_array_bit() {
  /**MACRO EXEC_LVAR_ARRAY_BIT**/
  {
    CALL_ALLOC_DT_ARRAY_BIT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_BIT**/  
}

void JD_exec_lvar_array_float() {
  /**MACRO EXEC_LVAR_ARRAY_FLOAT**/
  {
    CALL_ALLOC_DT_ARRAY_FLOAT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_FLOAT**/  
}

void JD_exec_lvar_array_double() {
  /**MACRO EXEC_LVAR_ARRAY_DOUBLE**/
  {
    CALL_ALLOC_DT_ARRAY_DOUBLE ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_DOUBLE**/  
}

void JD_exec_lvar_array_string() {
  /**MACRO EXEC_LVAR_ARRAY_STRING**/
  {
    CALL_ALLOC_DT_ARRAY_STRING ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_STRING**/  
}

void JD_exec_lvar_array_signal() {
  /**MACRO EXEC_LVAR_ARRAY_SIGNAL**/
  {
    CALL_ALLOC_DT_ARRAY_SIGNAL ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_SIGNAL**/  
}

void JD_exec_lvar_array_obj() {
  /**MACRO EXEC_LVAR_ARRAY_OBJ**/
  {  
    CALL_ALLOC_DT_ARRAY_OBJ ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_OBJ**/  
}

void JD_exec_lvar_array_thread() {
  /**MACRO EXEC_LVAR_ARRAY_THREAD**/
  {  
    CALL_ALLOC_DT_ARRAY_THREAD ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ARRAY_THREAD**/  
}

/*************************************************/
/* assoc array data                              */
/*************************************************/
void JD_exec_lvar_assoc_int() {
  /**MACRO EXEC_LVAR_ASSOC_INT**/
  {
    CALL_ALLOC_DT_ASSOC_INT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_INT**/  
}

void JD_exec_lvar_assoc_bit() {
  /**MACRO EXEC_LVAR_ASSOC_BIT**/
  {
    CALL_ALLOC_DT_ASSOC_BIT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_BIT**/  
}

void JD_exec_lvar_assoc_float() {
  /**MACRO EXEC_LVAR_ASSOC_FLOAT**/
  {
    CALL_ALLOC_DT_ASSOC_FLOAT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_FLOAT**/  
}

void JD_exec_lvar_assoc_double() {
  /**MACRO EXEC_LVAR_ASSOC_DOUBLE**/
  {
    CALL_ALLOC_DT_ASSOC_DOUBLE ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_DOUBLE**/  
}

void JD_exec_lvar_assoc_string() {
  /**MACRO EXEC_LVAR_ASSOC_STRING**/
  {
    CALL_ALLOC_DT_ASSOC_STRING ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_STRING**/  
}

void JD_exec_lvar_assoc_signal() {
  /**MACRO EXEC_LVAR_ASSOC_SIGNAL**/
  {
    CALL_ALLOC_DT_ASSOC_SIGNAL ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_lvar_assoc_obj() {
  /**MACRO EXEC_LVAR_ASSOC_OBJ**/
  {  
    CALL_ALLOC_DT_ASSOC_OBJ ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_OBJ**/  
}

void JD_exec_lvar_assoc_thread() {
  /**MACRO EXEC_LVAR_ASSOC_THREAD**/
  {  
    CALL_ALLOC_DT_ASSOC_THREAD ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_ASSOC_THREAD**/  
}

/*************************************************/
/* string indexed array data                     */
/*************************************************/
void JD_exec_lvar_strided_int() {
  /**MACRO EXEC_LVAR_STRIDED_INT**/
  {
    CALL_ALLOC_DT_STRIDED_INT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_INT**/  
}

void JD_exec_lvar_strided_bit() {
  /**MACRO EXEC_LVAR_STRIDED_BIT**/
  {
    CALL_ALLOC_DT_STRIDED_BIT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_BIT**/  
}

void JD_exec_lvar_strided_float() {
  /**MACRO EXEC_LVAR_STRIDED_FLOAT**/
  {
    CALL_ALLOC_DT_STRIDED_FLOAT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_FLOAT**/  
}

void JD_exec_lvar_strided_double() {
  /**MACRO EXEC_LVAR_STRIDED_DOUBLE**/
  {
    CALL_ALLOC_DT_STRIDED_DOUBLE ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_DOUBLE**/  
}

void JD_exec_lvar_strided_string() {
  /**MACRO EXEC_LVAR_STRIDED_STRING**/
  {
    CALL_ALLOC_DT_STRIDED_STRING ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_STRING**/  
}

void JD_exec_lvar_strided_signal() {
  /**MACRO EXEC_LVAR_STRIDED_SIGNAL**/
  {
    CALL_ALLOC_DT_STRIDED_SIGNAL ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_lvar_strided_obj() {
  /**MACRO EXEC_LVAR_STRIDED_OBJ**/
  {  
    CALL_ALLOC_DT_STRIDED_OBJ ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_OBJ**/  
}

void JD_exec_lvar_strided_thread() {
  /**MACRO EXEC_LVAR_STRIDED_THREAD**/
  {  
    CALL_ALLOC_DT_STRIDED_THREAD ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STRIDED_THREAD**/  
}


/*************************************************/
/* list data                                     */
/*************************************************/
void JD_exec_lvar_list_int() {
  /**MACRO EXEC_LVAR_LIST_INT**/
  {
    CALL_ALLOC_DT_LIST_INT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_INT**/  
}

void JD_exec_lvar_list_bit() {
  /**MACRO EXEC_LVAR_LIST_BIT**/
  {
    CALL_ALLOC_DT_LIST_BIT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_BIT**/  
}

void JD_exec_lvar_list_float() {
  /**MACRO EXEC_LVAR_LIST_FLOAT**/
  {
    CALL_ALLOC_DT_LIST_FLOAT ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_FLOAT**/  
}

void JD_exec_lvar_list_double() {
  /**MACRO EXEC_LVAR_LIST_DOUBLE**/
  {
    CALL_ALLOC_DT_LIST_DOUBLE ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_DOUBLE**/  
}

void JD_exec_lvar_list_string() {
  /**MACRO EXEC_LVAR_LIST_STRING**/
  {
    CALL_ALLOC_DT_LIST_STRING ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_STRING**/  
}

void JD_exec_lvar_list_signal() {
  /**MACRO EXEC_LVAR_LIST_SIGNAL**/
  {
    CALL_ALLOC_DT_LIST_SIGNAL ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_SIGNAL**/  
}

/* OBJ_ALLOC should hold the pointer to the alloc function */
void JD_exec_lvar_list_obj() {
  /**MACRO EXEC_LVAR_LIST_OBJ**/
  {  
    CALL_ALLOC_DT_LIST_OBJ ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_OBJ**/  
}

void JD_exec_lvar_list_thread() {
  /**MACRO EXEC_LVAR_LIST_THREAD**/
  {  
    CALL_ALLOC_DT_LIST_THREAD ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_LIST_THREAD**/  
}

/*************************************************/
/* static data                                   */
/*************************************************/
void JD_exec_lvar_static() {
  /**MACRO EXEC_LVAR_STATIC**/
  {
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->data.ptr.pdata = GLOBAL_DATA[TMP_INT] ;
    PUSH_TO_STACK(TMP_DT) ;
  }
  /**END MACRO EXEC_LVAR_STATIC**/  
}


/*************************************************/
/* duplicate bit & string                        */
/*************************************************/
static void setbit( 
  int *a, int l, int b, int s 
){
  int d ;
  
  d = (b & ((unsigned)0xffffffff>>(32-s)) ) << (l%32) ;
  a[l/32] = ( a[l/32] & ~((unsigned)0xffffffff>>(32-s)) ) | d ;
  if( (s + (l%32)) > 32 ) {
    d = ( b >> ((s+(l%32))-32) ) & 
          ((unsigned)0xffffffff>>(32-( (s+(l%32))-32 ) ) )  ;
    a[l/32+1] =
      ( a[l/32+1] & 
         ~( ((unsigned)0xffffffff>>(32-( (s+(l%32))-32 ) ) ) ) ) | d ;
  }
}

void JD_exec_duplicate_bit() {
  /**MACRO EXEC_DUPLICATE_BIT**/
  {  
    JD_data *dt ;
    int i, j, l, m, n, s ;
    if( ACC_TYPE == JD_D_int ) {
      ACC_TYPE = JD_D_bit ;
      ACC_A[0] = ACC_INT ;
      ACC_B[0] = 0 ;
      ACC_SIZE = 32 ;
    }
    dt = POP_STACK ;
    S0 = n = dt->data.int_value ; /* {n{bit}} where n is guaranteed to be int */
    s = LASTWORD(ACC_SIZE) ;
    S0 *= ACC_SIZE ; 
    m = LASTWORD(S0) ;
    if( ACC_WD <= m ){
      ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(S0) ) ; 
      ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(S0) ) ; 
      ACC_WD = m+1 ;
    }
    l = ACC_SIZE ;
    for( i = 0 ; i < n ; i++ ) { /* num times */
      s = ACC_SIZE ;
      for( j = 0 ; j <= s ; j++ ) { /* per iteration */
        setbit( ACC_A, l, ACC_A[j], (j==s)?(n%32):32 ) ;
        setbit( ACC_B, l, ACC_B[j], (j==s)?(n%32):32 ) ;
        l += (j==s)? (n%32):32 ;
        s -= 32 ;
      }
    }
    ACC_SIZE = S0 ;
  }
  /**END MACRO EXEC_DUPLICATE_BIT**/  
}

void JD_exec_duplicate_string() {
  /**MACRO EXEC_DUPLICATE_STRING**/
  {  
    JD_data *dt ;
    char *str ;
    int len, i ;
    dt = POP_STACK ;
    S0 = dt->data.int_value ; /* {n{bit}} where n is guaranteed to be int */
    len = strlen( ACC_STRING ) ;
    str = ALLOC_STRING( len*S0 ) ;
    strcat( str, ACC_STRING ) ;
    for( i = 1 ; i < S0 ; i++ ) {
      strcat( str, ACC_STRING ) ;
    }
    DEALLOC_STRING( ACC_STRING ) ;
    ACC_STRING = str ;
  }
  /**END MACRO EXEC_DUPLICATE_STRING**/  
}

/*****************************************************************/
/*  Load Thread                                                  */
/*****************************************************************/
void JD_exec_load_thread() {
  /**MACRO EXEC_LOAD_THREAD**/
  {  /* TMP_INT holds label index */
    JD_thread *thrd = JD_copy_thread( THREAD ) ;
    ACC_DT = JD_alloc_data_entry();
    ACC_DT->data.thrd = thrd ;
    thrd->run_index = TMP_INT ; /* set start index */
    ACC_TYPE = ACC_DT->type = JD_D_thread ;
  }
  /**END MACRO EXEC_LOAD_THREAD**/  
}


/*****************************************************************/
/*  Load Local Memory                                            */
/*****************************************************************/
JD_data *JD_get_ptr_element( JD_data *dt ) {
  JD_data *nt ;
  JD_data_unit *ut ;
  int hs, t ;
  JD_hash_entry *hsh ;
  nt = dt->data.ptr.pdata ;
  if( dt->sub_type == JD_SD_ptr_offset ) {
    int offset = dt->data.ptr.offset ;
    switch(nt->sub_type) {
      case JD_SD_array:
        ut = &nt->data.array.body[offset] ;
        nt = ut->data ;
        break ;
      case JD_SD_assoc:
        hs = CALC_DATA_HASH(offset) ;
        hsh = nt->data.hash.table[hs] ;
        while(hsh) {
          if( hsh->idx == offset ) break ;
          if( hsh->idx > offset ) hsh = hsh->right ;
          else hsh = hsh->left ;
        }
        if( hsh ) nt = hsh->data ;
        else nt = JD_alloc_single(nt, (char *)nt->base) ;
        break ;
      case JD_SD_strided:
        CALC_STRING_HASH(hs,dt->data.ptr.string) ;
        hsh = dt->data.hash.table[hs] ;
        while( hsh ) {
          if( !(t = strcmp(dt->data.ptr.string, hsh->string)) ) break ;
          if( t > 0 ) hsh = hsh->right ;
          else hsh = hsh->left ;
        }
        if( hsh )  nt = hsh->data ;
        else nt = JD_alloc_single(nt, (char *)nt->base) ;
        break ;
    }
  }
  return nt ;
}

void JD_exec_loadl_single() {
  /**MACRO EXEC_LOADL_SINGLE**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    CALL_EXEC_COPY_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADL_SINGLE**/  
}

void JD_exec_loadl_single_var() {
  /**MACRO EXEC_LOADL_SINGLE_VAR**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->sub_type = JD_SD_single ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    TMP_DT->data.ptr.pdata = dt ;
    CALL_EXEC_COPY_ACC(TMP_DT) ;

  }
  /**END MACRO EXEC_LOADL_SINGLE_VAR**/  
}

void JD_exec_loadl_single_static() {
  /**MACRO EXEC_LOADL_SINGLE_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_COPY_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADL_SINGLE_STATIC**/  
}

/* ARGSUSED */
void get_assoc_unit( JD_data *dt, JD_data *rt ) {
  /**MACRO EXEC_GET_ASSOC_UNIT**/
  {
    JD_hash_entry *hsh ;
    int hs = CALC_DATA_HASH(ACC_INT) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( hsh->idx == ACC_INT ) break ;
      if( hsh->idx > ACC_INT ) hsh = hsh->right ;
      else hsh = hsh->left ;
    }
    if( hsh ) rt = hsh->data ;
  }
  /**END MACRO EXEC_GET_ASSOC_UNIT**/  
}

void JD_exec_loadl_array() {
  /**MACRO EXEC_LOADL_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;

    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= ACC_INT || ACC_INT < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[ACC_INT] ;
    CALL_EXEC_SET_ACC(ut->data) ;
  }
  /**END MACRO EXEC_LOADL_ARRAY**/  
}

static void JD_check_range() {
  /**MACRO CHECK_RANGE**/
  {  /* TMP_INT holds the index, ACC_INT holds array size per dimension */
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
     if( TMP_INT >= ACC_INT || TMP_INT < 0 ) {
       ERROR_MULTI_DIMENSION_ARRAY_ACCESS_INDEX_OVERFLOW(TMP_INT, ACC_INT) ; 
     }
#endif
  }
  /**END MACRO CHECK_RANGE**/  
}

void JD_exec_loadl_marray() {
  /**MACRO EXEC_LOADL_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[ACC_INT] ;
    CALL_EXEC_SET_ACC(ut->data) ;
  }
  /**END MACRO EXEC_LOADL_MARRAY**/  
}

void JD_exec_loadl_assoc() {
  /**MACRO EXEC_LOADL_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_assoc ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    hs = CALC_DATA_HASH(ACC_INT) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( hsh->idx == ACC_INT ) break ;
      if( hsh->idx > ACC_INT ) hsh = hsh->right ;
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
      CALL_EXEC_NULL_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_LOADL_ASSOC**/  
}

void JD_exec_loadl_stridx() {
  /**MACRO EXEC_LOADL_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs, t ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
    if( ACC_STRING == NULL ) {
      ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS ;
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      CALL_EXEC_NULL_ACC(dt) ;
      return ;
    }
    CALC_STRING_HASH(hs,ACC_STRING) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( !(t = strcmp(hsh->string,ACC_STRING)) ) break ;
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
      CALL_EXEC_NULL_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_LOADL_STRIDX**/  
}

void JD_exec_loadl_list() {
  /**MACRO EXEC_LOADL_LIST**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut, *at ;
    int n ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    at = NULL ;
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
  /**END MACRO EXEC_LOADL_LIST**/  
}

/*****************************************************************/
/*  Load Global Memory                                           */
/*****************************************************************/
void JD_exec_loadg_single() {
  /**MACRO EXEC_LOADG_SINGLE**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    CALL_EXEC_COPY_ACC(dt) ;
  }
  /**END MACRO EXEC_LOADG_SINGLE**/  
}

void JD_exec_loadg_single_var() {
  /**MACRO EXEC_LOADG_SINGLE_VAR**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->sub_type = JD_SD_single ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    TMP_DT->data.ptr.pdata = dt ;
    CALL_EXEC_COPY_ACC(TMP_DT) ;
  }
  /**END MACRO EXEC_LOADG_SINGLE**/  
}


void JD_exec_loadg_array() {
  /**MACRO EXEC_LOADG_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = GLOBAL_DATA[TMP_INT] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= ACC_INT || ACC_INT < 0 ) {
      ERROR_ARRAY_ACCESS_SIZE_OVERFLOW ;
    }
#endif
    ut = &dt->data.array.body[ACC_INT] ;
    CALL_EXEC_SET_ACC(ut->data) ;
  }
  /**END MACRO EXEC_LOADG_ARRAY**/  
}

void JD_exec_loadg_marray() {
  /**MACRO EXEC_LOADG_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = GLOBAL_DATA[TMP_INT] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[ACC_INT] ;
    CALL_EXEC_SET_ACC(ut->data) ;
  }
  /**END MACRO EXEC_LOADG_MARRAY**/  
}

void JD_exec_loadg_assoc() {
  /**MACRO EXEC_LOADG_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs ;
    
    dt = GLOBAL_DATA[TMP_INT] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_assoc ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ASSOC_ARRAY_ACCESS ;
    }
#endif
    hs = CALC_DATA_HASH(ACC_INT) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( hsh->idx == ACC_INT ) break ;
      if( hsh->idx > ACC_INT ) hsh = hsh->right ;
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
      CALL_EXEC_NULL_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_LOADG_ASSOC**/  
}

void JD_exec_loadg_stridx() {
  /**MACRO EXEC_LOADG_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh ;
    int hs, t ;
    
    dt = GLOBAL_DATA[TMP_INT] ;

#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
    if( ACC_STRING == NULL ) {
      ERROR_NULL_STRING_FOR_STRING_ARRAY_ACCESS ;
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      CALL_EXEC_NULL_ACC(dt) ;
      return ;
    }
    CALC_STRING_HASH(hs,ACC_STRING) ;
    hsh = dt->data.hash.table[hs] ;
    while( hsh ) {
      if( !(t = strcmp(hsh->string,ACC_STRING)) ) break ;
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
      CALL_EXEC_NULL_ACC(dt) ;
    }
  }
  /**END MACRO EXEC_LOADG_STRIDX**/  
}

void JD_exec_loadg_list() {
  /**MACRO EXEC_LOADG_LIST**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut, *at ;
    int n ;
    dt = GLOBAL_DATA[TMP_INT] ;
    at = NULL ;
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
  /**END MACRO EXEC_LOADG_LIST**/  
}

/*****************************************************************/
/*  Convert Data                                                 */
/*****************************************************************/
void JD_exec_to_int() {
  /**MACRO EXEC_TO_INT**/
  {
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
    int i, n ;
#endif
    switch( ACC_TYPE ) {
      case JD_D_sbit:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
        if( ACC_B[0] ) {
          ERROR_ILLEGAL_DATA_TYPE_FOR_INTERGER_CONVERSION ;
        }
#endif
        ACC_INT = ACC_A[0] ;
        break ;
      case JD_D_bit:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
        n = LASTWORD(ACC_SIZE) ;
        for( i = 0 ; i <= n ; i++ ) {
          if( ACC_B[i] ) {
            ERROR_ILLEGAL_DATA_TYPE_FOR_INTERGER_CONVERSION ;
            n = 0 ;
          }
          else if( i & ACC_A[i] ) {
            WARNING_DATA_OVERFLOW_DETECTED_ON_BIT_TO_INT_CONVERSION ;
            n = 0 ;
          }
        }
#endif
        ACC_INT = ACC_A[0] ;
        break ;
      case JD_D_float:
      case JD_D_double:
        ACC_INT = ACC_FLOAT ;
        break ;
      default:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
        ERROR_ILLEGAL_DATA_TYPE_FOR_INTERGER_CONVERSION ;
#endif
    }
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_TO_INT**/
}

void JD_exec_int_to_bit_size() {
  /**MACRO EXEC_INT_TO_BIT_SIZE**/
  {
    unsigned int msk ;
    ACC_A[0] = ACC_INT ;
    ACC_B[0] = 0 ;
    ACC_SIZE = TMP_INT ;
    
    if( TMP_INT <= 32 ) {
      if( TMP_INT != 32 ) {
        msk = (1 << TMP_INT) - 1 ;
        ACC_A[0] &= msk ;
      }
      ACC_TYPE = JD_D_sbit ;
    }
    else {
      int i, j ;
      ACC_TYPE = JD_D_bit ;
      j = LASTWORD(ACC_SIZE) ;
      if( ACC_WD < j ) { 
        ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(TMP_INT) ) ; 
        ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(TMP_INT) ) ; 
        ACC_WD = j ; 
      } 
      TMP_INT2 = NWORD(TMP_INT) ;
      for( i = 1 ; i < TMP_INT2 ; i++ ) {
        ACC_A[i] = 0 ;
        ACC_B[i] = 0 ;
      }
    }
  }
  /**END MACRO EXEC_INT_TO_BIT_SIZE**/
}

void JD_exec_bit_to_bit_size() {
  /**MACRO EXEC_BIT_TO_BIT_SIZE**/
  {    
    if( TMP_INT > ACC_SIZE ) {
      int i, j ;
      ACC_TYPE = JD_D_bit ;
      j = LASTWORD(TMP_INT) ;
      if( ACC_WD < j ) { 
        ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(TMP_INT) ) ; 
        ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(TMP_INT) ) ; 
        ACC_WD = j ;
        for( i = ACC_WD ; i < j ; i++ ) {
          ACC_A[i] = 0 ;
          ACC_B[i] = 0 ;
        }
      } 
    }
    ACC_SIZE = TMP_INT ;
  }
  /**END MACRO EXEC_BIT_TO_BIT_SIZE**/
}

void JD_exec_float_to_bit() ;

void JD_exec_float_to_bit_size() {
  /**MACRO EXEC_FLOAT_TO_BIT_SIZE**/
  {    
    if( TMP_INT > ACC_SIZE ) {
      int i, j ;
      ACC_TYPE = JD_D_bit ;
      j = LASTWORD(TMP_INT) ;
      if( ACC_WD < j ) { 
        ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(TMP_INT) ) ; 
        ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(TMP_INT) ) ; 
        ACC_WD = j ;
        for( i = 0 ; i < j ; i++ ) {
          ACC_A[i] = 0 ;
          ACC_B[i] = 0 ;
        }
      } 
    }
    ACC_SIZE = TMP_INT ;
    JD_exec_float_to_bit() ;
  }
  /**END MACRO EXEC_FLOAT_TO_BIT_SIZE**/
}

void JD_exec_string_to_bit_size() {
  /**MACRO EXEC_STRING_TO_BIT_SIZE**/
  {
    if( TMP_INT > ACC_SIZE ) {
      int i, j ;
      ACC_TYPE = JD_D_bit ;
      j = LASTWORD(TMP_INT) ;
      if( ACC_WD < j ) { 
        ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(TMP_INT) ) ; 
        ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(TMP_INT) ) ; 
        ACC_WD = j ;
        for( i = 0 ; i < j ; i++ ) {
          ACC_A[i] = 0 ;
          ACC_B[i] = 0 ;
        }
      } 
    }
    ACC_SIZE = TMP_INT ;
    if( ACC_SIZE <= 32 ) {
      ACC_TYPE = JD_D_sbit ;
      ACC_A[0] = ACC_B[0] = 0 ;
      if( ACC_STRING )
      {
        int nb, j ;
        nb = (ACC_SIZE)/8 ;
        j = 0 ;
        ACC_A[0] = ACC_B[0] = 0 ;
        for(  ; nb > 0 ; nb-- ) {
          ACC_A[0] = 
            (ACC_A[0] << 8) | 
            ( ( ACC_STRING[j] != NULL_CHAR)?ACC_STRING[j++]:0 ) ;
        }
        DEALLOC_STRING(ACC_STRING) ;
        ACC_STRING = NULL ;
      }
    }
    else {
      ACC_TYPE = JD_D_bit ;
      if( ACC_STRING )
      {
        int nw, nb, size, j ;
        size = ACC_SIZE/8 ;
        size *= 8 ; /* round to 8 */
        nw = LASTWORD(ACC_SIZE) ;
        j = 0 ;
        nb = (size%32)/8 ;
        if( nb == 0 ) nb = 4 ;
        for(  ; nw >= 0 ; nw-- ) {
          ACC_A[nw] = 0 ;
          for(  ; nb > 0 ; nb-- ) {
            ACC_A[nw] = 
              (ACC_A[nw] << 8) | 
              ( ( ACC_STRING[j] != NULL_CHAR)?ACC_STRING[j++]:0 ) ;
          }
          nb = 4 ;
        }
        DEALLOC_STRING(ACC_STRING) ;
        ACC_STRING = NULL ;
      }
      else {
        int nw, i ;
        nw = LASTWORD(ACC_SIZE) ;
        for( i = 0 ; i <= nw ; i++ )
          ACC_A[i] = ACC_B[i] = 0 ;
      }
    }
    
  }
  /**END MACRO EXEC_STRING_TO_BIT_SIZE**/
}

void JD_exec_int_to_bit() {
  /**MACRO EXEC_INT_TO_BIT**/
  {
    ACC_A[0] = ACC_INT ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 32 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_INT_TO_BIT**/
}

void JD_exec_bit_to_int() {
  /**MACRO EXEC_BIT_TO_INT**/
  {
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
    int i, n ;
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      if( ACC_B[i] ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_INTERGER_CONVERSION ;
        n = 0 ;
      }
      else if( i & ACC_A[i] ) { 
        WARNING_DATA_OVERFLOW_DETECTED_ON_BIT_TO_INT_CONVERSION ;
        n = 0 ;
      }
    }
#endif
    ACC_INT = ACC_A[0] ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_BIT_TO_INT**/
}

void JD_exec_int_to_float() {
  /**MACRO EXEC_INT_TO_FLOAT**/
  {
    ACC_FLOAT = ACC_INT ;
    ACC_TYPE = JD_D_float ;
  }
  /**END MACRO EXEC_INT_TO_FLOAT**/
}

void JD_exec_float_to_int() {
  /**MACRO EXEC_FLOAT_TO_INT**/
  {
    ACC_INT = ACC_FLOAT ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_FLOAT_TO_INT**/
}

void JD_exec_bit_to_float() {
  /**MACRO EXEC_BIT_TO_FLOAT**/
  {
    int i, n ;
    double sft ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      if( ACC_B[i] ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_FLOAT_CONVERSION ;
        n = 0 ;
      }
    }
#endif
    sft = 2.0 ;
    for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
    ACC_FLOAT = 0.0 ;
    n = NWORD(ACC_SIZE) ;
    for( i = 0 ; i < n ; i++ ) {
      ACC_FLOAT *= sft ;
      ACC_FLOAT += ACC_A[i] ;
    }
    ACC_TYPE = JD_D_float ;
  }
  /**END MACRO EXEC_BIT_TO_FLOAT**/

}

void JD_exec_float_to_bit() {
  /**MACRO EXEC_FLOAT_TO_BIT**/
  {
    double sft ;
    int i ;
    ACC_A[0] = 0 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 32 ;
    sft = 2.0 ;
    for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
    i = 0 ;
    if( ACC_FLOAT > 1.0 ) {
      while( ACC_FLOAT > 1.0 ) {
        ACC_A[i++] = fmod(ACC_FLOAT,sft) ;
        ACC_FLOAT /= sft ;
        if( i == ACC_WD ) break ;
      }
      ACC_SIZE = i * 32 ;
    }
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_FLOAT_TO_BIT**/
}

/*****************************************************************/
/*  Store Local Memory                                           */
/*****************************************************************/
/*
 *  Store single <type>
 */ 
void JD_exec_storel_single_int() {
  /**MACRO EXEC_STOREL_SINGLE_INT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    dt->data.int_value = ACC_INT ;
    STORE_SIZE = 32 ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_INT**/  
}

void JD_exec_storel_single_flt_dbl() {
  /**MACRO EXEC_STOREL_SINGLE_FLT_DBL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->type == JD_D_float )
      dt->data.float_value = ACC_FLOAT ;
    else 
      dt->data.double_value = ACC_FLOAT ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_FLT_DBL**/  
}

void JD_exec_storel_single_bit() {
  /**MACRO EXEC_STOREL_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int msk ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->type == JD_D_sbit ) {
      STORE_SIZE = dt->data.sbit.size ;
      msk = BITMASK(dt->data.sbit.size) ;
      dt->data.sbit.a = ACC_A[0] & msk ;
      dt->data.sbit.b = ACC_B[0] & msk ;
    }
    else {
      int i, n ;
      STORE_SIZE = dt->data.bit.size ;
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
  }
  /**END MACRO EXEC_STOREL_SINGLE_BIT**/  
}

void JD_exec_storel_string_to_single_bit() {
  /**MACRO EXEC_STOREL_STRING_TO_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREL_STRING_TO_SINGLE_BIT**/  
}

void JD_exec_storel_single_string() {
  /**MACRO EXEC_STOREL_SINGLE_STRING**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->data.string ) DEALLOC_STRING(dt->data.string) ;
    if( ACC_TYPE == JD_D_null ) dt->data.string = NULL ;
    else {
      dt->data.string = ACC_STRING ;
      ACC_STRING = NULL ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_STRING**/  
}

void JD_exec_storel_single_string_null() {
  /**MACRO EXEC_STOREL_SINGLE_STRING_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->data.string ) DEALLOC_STRING(dt->data.string) ;
    dt->data.string = NULL ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_STRING_NULL**/  
}

void JD_exec_storel_single_obj() {
  /**MACRO EXEC_STOREL_SINGLE_OBJ**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    if( ACC_TYPE == JD_D_null ) dt->data.obj.ect = NULL ;
    else {
#ifndef  NO_OBJ_TYPE_CHECK
      if( dt->data.obj.alloc && 
          dt->data.obj.alloc != ACC_DT->data.obj.alloc ) 
      {
        ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
      }
#endif
      dt->data.obj.ect = ACC_DT->data.obj.ect ;
      if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_OBJ**/  
}

void JD_exec_storel_single_obj_null() {
  /**MACRO EXEC_STOREL_SINGLE_OBJ_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    dt->data.obj.ect = NULL ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_OBJ_NULL**/  
}

void JD_exec_storel_single_thread() {
  /**MACRO EXEC_STOREL_SINGLE_THREAD**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( ACC_TYPE == JD_D_null ) dt->data.thrd = NULL ;
    else {
      dt->data.thrd = ACC_DT->data.thrd ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_THREAD**/  
}


void JD_exec_storel_single_thread_null() {
  /**MACRO EXEC_STOREL_SINGLE_THREAD_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    dt->data.thrd = NULL ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_THREAD_NULL**/  
}


void JD_exec_storel_single_signal() {
  /**MACRO EXEC_STOREL_SINGLE_SIGNAL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    if( ACC_TYPE == JD_D_null ) dt->data.signal.port = NULL ;
    else {
      dt->data.signal.port = ACC_DT->data.signal.port ;
      dt->data.signal.data0 = ACC_DT->data.signal.data0 ;
      dt->data.signal.data1 = ACC_DT->data.signal.data1 ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_SIGNAL**/  
}

void JD_exec_storel_single_signal_null() {
  /**MACRO EXEC_STOREL_SINGLE_SIGNAL_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    dt->data.signal.port = ACC_DT->data.signal.port ;
    dt->data.signal.data0 = ACC_DT->data.signal.data0 ;
    dt->data.signal.data1 = ACC_DT->data.signal.data1 ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_SIGNAL_NULL**/  
}


void JD_exec_storel_single_dt() {
  /**MACRO EXEC_STOREL_SINGLE_DT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    JD_duplicate_data_to( dt, ACC_DT ) ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_DT**/  
}



/*
 *  Store single static <type>
 */ 

void JD_exec_storel_single_int_static() {
  /**MACRO EXEC_STOREL_SINGLE_INT_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    dt->data.int_value = ACC_INT ;
    STORE_SIZE = 32 ;
#ifdef JEDA_VERILOG_VAR_UPDATE
    if( IS_STATICDATA(dt) ) 
      JD_update_static_var(dt) ;
#endif
  }
  /**END MACRO EXEC_STOREL_SINGLE_INT_STATIC**/  
}

void JD_exec_storel_single_flt_dbl_static() {
  /**MACRO EXEC_STOREL_SINGLE_FLT_DBL_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->type == JD_D_float )
      dt->data.float_value = ACC_FLOAT ;
    else 
      dt->data.double_value = ACC_FLOAT ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_FLT_DBL_STATIC**/  
}

void JD_exec_storel_single_bit_static() {
  /**MACRO EXEC_STOREL_SINGLE_BIT_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int msk ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->type == JD_D_sbit ) {
      STORE_SIZE = dt->data.sbit.size ;
      msk = BITMASK(dt->data.sbit.size) ;
      dt->data.sbit.a = ACC_A[0] & msk ;
      dt->data.sbit.b = ACC_B[0] & msk ;
    }
    else {
      int i, n ;
      STORE_SIZE = dt->data.bit.size ;
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
  /**END MACRO EXEC_STOREL_SINGLE_BIT_STATIC**/  
}

void JD_exec_storel_single_string_static() {
  /**MACRO EXEC_STOREL_SINGLE_STRING_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->data.string ) DEALLOC_STRING(dt->data.string) ;
    if( ACC_TYPE == JD_D_null ) dt->data.string = NULL ;
    else {
      dt->data.string = ACC_STRING ;
      ACC_STRING = NULL ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_STRING_STATIC**/  
}

void JD_exec_storel_single_string_static_null() {
  /**MACRO EXEC_STOREL_SINGLE_STRING_STATIC_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->data.string ) DEALLOC_STRING(dt->data.string) ;
    dt->data.string = NULL ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_STRING_STATIC_NULL**/  
}

void JD_exec_storel_single_obj_static() {
  /**MACRO EXEC_STOREL_SINGLE_OBJ_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
    if( ACC_TYPE == JD_D_null ) dt->data.obj.ect = NULL ;
    else {
#ifndef  NO_OBJ_TYPE_CHECK
      if( dt->data.obj.alloc && 
          dt->data.obj.alloc != ACC_DT->data.obj.alloc ) 
      {
        ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
      }
#endif
      dt->data.obj.ect = ACC_DT->data.obj.ect ;
      if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_OBJ_STATIC**/  
}

void JD_exec_storel_single_obj_static_null() {
  /**MACRO EXEC_STOREL_SINGLE_OBJ_STATIC_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
    dt->data.obj.ect = NULL ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_OBJ_STATIC_NULL**/  
}

void JD_exec_storel_single_thread_static() {
  /**MACRO EXEC_STOREL_SINGLE_THREAD_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( ACC_TYPE == JD_D_null ) dt->data.thrd = NULL ;
    else dt->data.thrd = ACC_DT->data.thrd ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_THREAD_STATIC**/  
}

void JD_exec_storel_single_signal_static() {
  /**MACRO EXEC_STOREL_SINGLE_SIGNAL_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( ACC_TYPE == JD_D_null ) dt->data.signal.port = NULL ;
    else {
      dt->data.signal.port = ACC_DT->data.signal.port ;
      dt->data.signal.data0 = ACC_DT->data.signal.data0 ;
      dt->data.signal.data1 = ACC_DT->data.signal.data1 ;
    }
  }
  /**END MACRO EXEC_STOREL_SINGLE_SIGNAL_STATIC**/  
}

void JD_exec_storel_single_signal_static_null() {
  /**MACRO EXEC_STOREL_SINGLE_SIGNAL_STATIC_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    dt->data.signal.port = NULL ;
    dt->data.signal.data0 = 0 ;
    dt->data.signal.data1 = 0 ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_SIGNAL_STATIC_NULL**/  
}

void JD_exec_storel_single_dt_static() {
  /**MACRO EXEC_STOREL_SINGLE_DT_STATIC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    JD_duplicate_data_to( dt, ACC_DT ) ;
  }
  /**END MACRO EXEC_STOREL_SINGLE_DT_STATIC**/  
}

/*
 * Store subbit
 */
void JD_exec_storel_subbit() {
  /**MACRO EXEC_STOREL_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    lb = ACC_INT ;  /* ACC should hold lb */
    CALL_EXEC_POP_ACC ;
    ub = ACC_INT ;  /* ub is pushed to stack */

    CALL_EXEC_POP_ACC ;
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
  /**END MACRO EXEC_STOREL_SUBBIT**/  
}


/*
 *  Store array
 */
void JD_exec_storel_array() {
  /**MACRO EXEC_STOREL_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_ARRAY**/  
}

/*
 *  Store local array subbit
 */
void JD_exec_storel_array_subbit() {
  /**MACRO EXEC_STOREL_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_ARRAY_SUBBIT**/  
}

/*
 * store local marray subbit
 */ 
void JD_exec_storel_marray_subbit() {
  /**MACRO EXEC_STOREL_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_MARRAY_SUBBIT**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to ACC_INT
 */ 
void JD_exec_storel_marray() {
  /**MACRO EXEC_STOREL_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREL_MARRAY**/  
}

void JD_exec_storel_assoc_subbit() {
  /**MACRO EXEC_STOREL_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_ASSOC_SUBBIT**/  
}

void JD_exec_storel_assoc() {
  /**MACRO EXEC_STOREL_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_ASSOC**/  
}

void JD_exec_storel_stridx() {
  /**MACRO EXEC_STOREL_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_STRIDX**/  
}

void JD_exec_storel_stridx_subbit() {
  /**MACRO EXEC_STOREL_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
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
  /**END MACRO EXEC_STOREL_STRIDX_SUBBIT**/  
}

/*****************************************************************/
/*  Store Global Memory                                          */
/*****************************************************************/
/*
 *  Store single <type>
 */ 
void JD_exec_storeg_single_int() {
  /**MACRO EXEC_STOREG_SINGLE_INT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    dt->data.int_value = ACC_INT ;
    STORE_SIZE = 32 ;
    
#ifdef JEDA_VERILOG_VAR_UPDATE
    JD_update_global_var(TMP_INT) ;
#endif

  }
  /**END MACRO EXEC_STOREG_SINGLE_INT**/  
}

void JD_exec_storeg_single_flt_dbl() {
  /**MACRO EXEC_STOREG_SINGLE_FLT_DBL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt->type == JD_D_float )
      dt->data.float_value = ACC_FLOAT ;
    else 
      dt->data.double_value = ACC_FLOAT ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_FLT_DBL**/  
}

void JD_exec_storeg_single_bit() {
  /**MACRO EXEC_STOREG_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int msk ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt->type == JD_D_sbit ) {
      STORE_SIZE = dt->data.sbit.size ;
      msk = BITMASK(dt->data.sbit.size) ;
      dt->data.sbit.a = ACC_A[0] & msk ;
      dt->data.sbit.b = ACC_B[0] & msk ;
    }
    else {
      int i, n ;
      STORE_SIZE = dt->data.bit.size ;
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
    JD_update_global_var(TMP_INT) ;
#endif
  }
  /**END MACRO EXEC_STOREG_SINGLE_BIT**/  
}

void JD_exec_storeg_string_to_single_bit() {
  /**MACRO EXEC_STOREG_STRING_TO_SINGLE_BIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREG_STRING_TO_SINGLE_BIT**/  
}

void JD_exec_storeg_single_string() {
  /**MACRO EXEC_STOREG_SINGLE_STRING**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( ACC_TYPE == JD_D_null ) dt->data.string = NULL ;
    else {
      dt->data.string = ACC_STRING ;
      ACC_STRING = NULL ;
    }
  }
  /**END MACRO EXEC_STOREG_SINGLE_STRING**/  
}

void JD_exec_storeg_single_string_null() {
  /**MACRO EXEC_STOREG_SINGLE_STRING_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    dt->data.string = NULL ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_STRING_NULL**/  
}

void JD_exec_storeg_single_obj() {
  /**MACRO EXEC_STOREG_SINGLE_OBJ**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    if( ACC_TYPE == JD_D_null ) dt->data.obj.ect = NULL ;
    else {
#ifndef  NO_OBJ_TYPE_CHECK
      if( dt->data.obj.alloc && 
          dt->data.obj.alloc != ACC_DT->data.obj.alloc ) 
      {
        ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
      }
#endif
      dt->data.obj.ect = ACC_DT->data.obj.ect ;
      if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
    }
  }
  /**END MACRO EXEC_STOREG_SINGLE_OBJ**/  
}

void JD_exec_storeg_single_obj_null() {
  /**MACRO EXEC_STOREG_SINGLE_OBJ_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count) == 0 ) {
      (dt->data.obj.ect->dealloc)( 
        (void *)dt->data.obj.ect->table,
        dt->data.obj.ect->num_entry,
        dt->data.obj.ect->tbl_size
      ) ;
      DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
    }
    dt->data.obj.ect = NULL ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_OBJ_NULL**/  
}


void JD_exec_storeg_single_thread() {
  /**MACRO EXEC_STOREG_SINGLE_THREAD**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( ACC_TYPE == JD_D_null ) dt->data.thrd = NULL ;
    else
      dt->data.thrd = ACC_DT->data.thrd ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_THREAD**/  
}


void JD_exec_storeg_single_signal() {
  /**MACRO EXEC_STOREG_SINGLE_SIGNAL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( ACC_TYPE == JD_D_null ) dt->data.signal.port = NULL ;
    else {
      dt->data.signal.port = ACC_DT->data.signal.port ;
      dt->data.signal.data0 = ACC_DT->data.signal.data0 ;
      dt->data.signal.data1 = ACC_DT->data.signal.data1 ;
    }
  }
  /**END MACRO EXEC_STOREG_SINGLE_SIGNAL**/  
}

void JD_exec_storeg_single_signal_null() {
  /**MACRO EXEC_STOREG_SINGLE_SIGNAL_NULL**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    dt->data.signal.port = NULL ;
    dt->data.signal.data0 = 0 ;
    dt->data.signal.data1 = 0 ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_SIGNAL_NULL**/  
}

void JD_exec_storeg_single_dt() {
  /**MACRO EXEC_STOREG_SINGLE_DT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    dt = GLOBAL_DATA[TMP_INT] ;
    JD_duplicate_data_to( dt, ACC_DT ) ;
  }
  /**END MACRO EXEC_STOREG_SINGLE_DT**/  
}


/*
 * Store subbit
 */
void JD_exec_storeg_subbit() {
  /**MACRO EXEC_STOREG_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    lb = ACC_INT ;  /* ACC should hold lb */
    CALL_EXEC_POP_ACC ;
    ub = ACC_INT ;  /* ub is pushed to stack */

    CALL_EXEC_POP_ACC ;
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
#ifdef JEDA_VERILOG_VAR_UPDATE
    JD_update_global_var(TMP_INT) ;
#endif
  }
  /**END MACRO EXEC_STOREG_SUBBIT**/  
}


/*
 *  Store array
 */
void JD_exec_storeg_array() {
  /**MACRO EXEC_STOREG_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = GLOBAL_DATA[TMP_INT] ;
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
#ifdef JEDA_VERILOG_VAR_UPDATE
    if( dt->type == JD_D_int || dt->type == JD_D_sbit || dt->type == JD_D_bit )
      JD_update_global_var_array(TMP_INT, TMP_INT2) ;
#endif
  }
  /**END MACRO EXEC_STOREG_ARRAY**/  
}

/*
 *  Store array subbit
 */
void JD_exec_storeg_array_subbit() {
  /**MACRO EXEC_STOREG_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
    else if( dt->data.array.size <= ACC_INT || ACC_INT < 0 ) {
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
      else if( ub >= size  || ub < 0 || lb < 0 ) {
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
#ifdef JEDA_VERILOG_VAR_UPDATE
    JD_update_global_var_array(TMP_INT, TMP_INT2) ;
#endif
  }
  /**END MACRO EXEC_STOREG_ARRAY_SUBBIT**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to ACC_INT
 */ 
void JD_exec_storeg_marray() {
  /**MACRO EXEC_STOREG_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    dt = GLOBAL_DATA[TMP_INT] ;
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
    CALL_EXEC_ACC_TO_DT(dt) ;
  }
  /**END MACRO EXEC_STOREG_MARRAY**/  
}

/*
 * store marray subbit
 */ 
void JD_exec_storeg_marray_subbit() {
  /**MACRO EXEC_STOREG_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    dt = GLOBAL_DATA[TMP_INT] ;
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
  /**END MACRO EXEC_STOREG_MARRAY_SUBBIT**/  
}

void JD_exec_storeg_assoc() {
  /**MACRO EXEC_STOREG_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
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
  /**END MACRO EXEC_STOREG_ASSOC**/  
}

void JD_exec_storeg_assoc_subbit() {
  /**MACRO EXEC_STOREG_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
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
  /**END MACRO EXEC_STOREG_ASSOC_SUBBIT**/  
}

void JD_exec_storeg_stridx() {
  /**MACRO EXEC_STOREG_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt, *rt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
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
  /**END MACRO EXEC_STOREG_STRIDX**/  
}

void JD_exec_storeg_stridx_subbit() {
  /**MACRO EXEC_STOREG_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    unsigned int lmsk, umsk ;
    int ub, lb, size ;
    int aub, alb ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( dt->sub_type != JD_SD_strided ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_STRIDX_ARRAY_ACCESS ;
    }
#endif
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
  /**END MACRO EXEC_STOREG_STRIDX_SUBBIT**/  
}

/* construct one list element from stack */
/* The data is pushed in the reverse order, i.e. '{0, 1, 2, 3} */
/*   push 0, push 1, push 2, push 3 and stack top is 3 */
void JD_exec_list() {
  /**MACRO EXEC_LIST**/
  { 
    JD_data *dt ;
    JD_data_unit *ut, *pt ;
    CALL_EXEC_POP_ACC ;
    dt = JD_alloc_data_entry() ;
    CALL_EXEC_SET_ACC_TO_DT(dt) ;
    ut = ALLOC_DATA_UNIT(1) ;
    ut->data = dt ;
    dt->base = TMP_DT ;
    pt = TMP_DT->data.list.top ;
    if( pt != NULL ) pt->pre = ut ;
    else TMP_DT->data.list.bottom = ut ;
    ut->next = pt ;
    ut->pre = NULL ;
    TMP_DT->data.list.top = ut ;
    TMP_DT->type = dt->type ;
  }
  /**END MACRO EXEC_LIST**/  
}

void JD_exec_get_subbit() {
  /**MACRO EXEC_GET_SUBBIT**/
  {  
    unsigned int lmsk, umsk, uumsk ;
    int ub, lb ;
    int aub, alb ;
    lb = ACC_INT ;  /* ACC should hold lb */
    CALL_EXEC_POP_ACC ;
    ub = ACC_INT ;  /* ub is pushed to stack */

    CALL_EXEC_POP_ACC ; /* target data for get_subbit */
#ifndef SKIP_ARRAY_BOUNDARY_CHECK
    switch(ACC_TYPE) {
      case JD_D_bit:
      case JD_D_sbit:
      case JD_D_vbit:
        break ;
      default:
        ERROR_ILLEGAL_DATA_TYPE_FOR_BIT_ASSIGNMENT ;
    }        
    if( I_ENDIAN == 0 ) {
      if( ub < lb ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( ub >= ACC_SIZE || ub < 0 || lb < 0 ) {
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
    if( ACC_TYPE == JD_D_sbit ) {
      umsk = (unsigned int)0xffffffff >> (32-aub) ;
      ACC_A[0] = (ACC_A[0] & umsk) >> alb  ;
      ACC_B[0] = (ACC_B[0] & umsk) >> alb  ;
    }
    else {
      int i, j, an, a, b, ls, us, ns, nwn ;
      
      /* implement (ACC>>alb)&umsk */
      
      i = LASTWORD(alb+1) ;
      an = LASTWORD(ACC_SIZE) ;
      ls = (alb%32) ;
      us = 32-ls ;
      lmsk = (unsigned int)0xffffffff >> ls ;
      uumsk = (1 << us )-1 ;
      if( uumsk ) uumsk = ~uumsk ;
      ns = aub - alb +1 ;
      umsk = (ns%32)?(1 << (ns%32))-1:0xffffffff ;
      nwn = LASTWORD(ns) ;
      for( j = 0 ; j < nwn ; i++, j++ ) {
        ACC_A[j] = ( ((ACC_A[i]>>ls)&lmsk) | ((ACC_A[i+1]<<us)&uumsk) ) ;
        ACC_B[j] = ( ((ACC_B[i]>>ls)&lmsk) | ((ACC_B[i+1]<<us)&uumsk) ) ;
      }
      if( i == an ) {
        a = b = 0 ;
      }
      else {
        a = ACC_A[i+1] ;
        b = ACC_B[i+1] ;
      }
      ACC_A[j] = ( ((ACC_A[i]>>ls)&lmsk) | ((a<<us)&uumsk) ) & umsk ;
      ACC_B[j] = ( ((ACC_B[i]>>ls)&lmsk) | ((b<<us)&uumsk) ) & umsk ;
      ACC_SIZE = ns ;
    }
  }
  /**END MACRO EXEC_GET_SUBBIT**/  
}


/* misc ALU ops */

void JD_exec_inc_bit() {
  /**MACRO EXEC_INC_BIT**/  
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    A1 = &INT_ONE ;
    B1 = &INT_ZERO ;
    S1 = 1 ;
    CALL_BIT_PLUS_BIT ;
  }
  /**END MACRO EXEC_INC_BIT**/  
}

void JD_exec_dec_bit() {
  /**MACRO EXEC_DEC_BIT**/  
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    A1 = &INT_ONE ;
    B1 = &INT_ZERO ;
    S1 = 1 ;
    CALL_BIT_MINUS_BIT ;
  }
  /**END MACRO EXEC_DEC_BIT**/  
}

void JD_top_of_stack() {
  /**MACRO EXEC_TOP_OF_STACK**/
  {  
    if( VSP ) {
      if( VS_TOP ) TMP_DT = VSTACK[VS_TOP-1] ;
      else TMP_DT = VSTACK[VSP_LIMIT-1] ;
    }
    else 
      TMP_DT = STACK[SP-1] ;
  }
  /**END MACRO EXEC_TOP_OF_STACK**/  
}

void JD_exec_cmp_bit_int() {
  /**MACRO EXEC_CMP_BIT_INT**/
  {  
    A0 = BIT_A(TMP_DT) ;
    B0 = BIT_B(TMP_DT) ;
    S0 = DT_BIT_SIZE(TMP_DT) ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
  }
  /**END MACRO EXEC_CMP_BIT_INT**/  
}

void JD_exec_cmp_float_int() {
  /**MACRO EXEC_CMP_FLOAT_INT**/
  {  
    ACC_FLOAT = ACC_INT ;
    if( TMP_DT->type == JD_D_float ) {
      ACC_INT = (ACC_FLOAT==TMP_DT->data.float_value)?1:0 ;
    }
    else {
      ACC_INT = (ACC_FLOAT==TMP_DT->data.double_value)?1:0 ;
    }
  }
  /**END MACRO EXEC_CMP_FLOAT_INT**/  
}

void JD_exec_cmp_int_bit() {
  /**MACRO EXEC_CMP_INT_BIT**/
  {  
    A0 = &TMP_DT->data.int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_INT_BIT**/  
}

void JD_exec_cmp_bit_bit() {
  /**MACRO EXEC_CMP_BIT_BIT**/
  {  
    A0 = BIT_A(TMP_DT) ;
    B0 = BIT_B(TMP_DT) ;
    S0 = DT_BIT_SIZE(TMP_DT) ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_BIT_BIT**/  
}

void JD_exec_cmp_bit_float() {
  /**MACRO EXEC_CMP_BIT_FLOAT**/
  { 
    double flt ;
    if( TMP_DT->type == JD_D_sbit ) {
      if( TMP_DT->data.sbit.b ) {
        ACC_INT = 0 ;
        ACC_TYPE = JD_D_int ;
        return ;
      }
      flt = TMP_DT->data.sbit.a ;
    }
    else {
      if( JD_check_bit_set( TMP_DT->data.bit.b, TMP_DT->data.bit.size ) ) {
        ACC_INT = 0 ;
        ACC_TYPE = JD_D_int ;
        return ;
      }
      flt = JD_bit_2_real(TMP_DT->data.bit.a, TMP_DT->data.bit.size) ;
    }
    ACC_INT = (ACC_FLOAT==flt)?1:0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_BIT_FLOAT**/  
}


void JD_exec_cmp_int_float() {
  /**MACRO EXEC_CMP_INT_FLOAT**/
  {  
    double flt = TMP_DT->data.int_value ;
    ACC_INT = (ACC_FLOAT==flt)?1:0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_INT_FLOAT**/  
}

void JD_exec_cmp_float_bit() {
  /**MACRO EXEC_CMP_FLOAT_BIT**/
  { 
    double flt ;
    if( TMP_DT->type == JD_D_float ) flt = TMP_DT->data.float_value ;
    else flt = TMP_DT->data.double_value ;
    if( JD_check_bit_set( ACC_B, ACC_SIZE ) ) {
      ACC_INT = 0 ;
      ACC_TYPE = JD_D_int ;
      return ;
    }
    ACC_FLOAT = JD_bit_2_real(ACC_A, ACC_SIZE) ;
    ACC_INT = (ACC_FLOAT==flt)?1:0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_FLOAT_BIT**/  
}

void JD_exec_cmp_float_float() {
  /**MACRO EXEC_CMP_FLOAT_FLOAT**/
  { 
    double flt ;
    if( TMP_DT->type == JD_D_float ) flt = TMP_DT->data.float_value ;
    else flt = TMP_DT->data.double_value ;
    ACC_INT = (ACC_FLOAT==flt)?1:0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_FLOAT_FLOAT**/  
}

void JD_exec_cmp_string_string() {
  /**MACRO EXEC_CMP_STRING_STRING**/
  { 
    if( ACC_STRING ) {
      if( TMP_DT->data.string && !strcmp(ACC_STRING,TMP_DT->data.string) )
        ACC_INT = 1 ;
      else {
        if( TMP_DT->data.string == NULL && ACC_STRING[0] == NULL_CHAR ) 
          ACC_INT = 1 ; 
        else 
          ACC_INT = 0 ;
      }
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    else {
      if( TMP_DT->data.string == NULL || TMP_DT->data.string[0] == NULL_CHAR )
        ACC_INT = 1 ;
      else 
        ACC_INT = 0 ;
    }
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_STRING_STRING**/  
}

void JD_exec_cmp_signal_signal() {
  /**MACRO EXEC_CMP_SIGNAL_SIGNAL**/
  { 
    if( ACC_DT->data.signal.port == TMP_DT->data.signal.port &&
        ACC_DT->data.signal.data0 == TMP_DT->data.signal.data0 &&
        ACC_DT->data.signal.data1 == TMP_DT->data.signal.data1    )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_SIGNAL_SIGNAL**/  
}


void JD_exec_cmp_obj_obj() {
  /**MACRO EXEC_CMP_OBJ_OBJ**/
  { 
    if( ACC_DT->data.obj.ect == TMP_DT->data.obj.ect &&
        ACC_DT->data.obj.alloc == TMP_DT->data.obj.alloc )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_OBJ_OBJ**/  
}


void JD_exec_cmp_thread_thread() {
  /**MACRO EXEC_CMP_THREAD_THREAD**/
  { 
    if( ACC_DT->data.thrd == TMP_DT->data.thrd )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMP_THREAD_THREAD**/  
}

void JD_exec_cmpx_int_bit() {
  /**MACRO EXEC_CMPX_INT_BIT**/
  {  
    int i, n, m, a, b ;
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      a = ACC_A[i] ;
      b = ACC_B[i] ;
      m = ~(a & b) ; /* mask x */
      ACC_A[i] = a & m ;
      ACC_B[i] = b & m ;
    }
    A0 = &TMP_DT->data.int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMPX_INT_BIT**/  
}


void JD_exec_cmpx_bit_int() {
  /**MACRO EXEC_CMPX_BIT_INT**/
  {  
    int i, n, m, a, b, size ;
    if( TMP_DT->type == JD_D_sbit ) {
      size = TMP_DT->data.sbit.size ;
      m = ~(TMP_DT->data.sbit.a & TMP_DT->data.sbit.b) ;
      TMP_A[0] = TMP_DT->data.sbit.a & m ;
      TMP_B[0] = TMP_DT->data.sbit.b & m ;
    }
    else {  
      size = TMP_DT->data.bit.size ;
      ADJUST_TEMP(size) ;
      n = LASTWORD(size) ;
      for( i = 0 ; i <= n ; i++ ) {
        a = TMP_DT->data.bit.a[i] ;
        b = TMP_DT->data.bit.b[i] ;
        m = ~(a & b) ;
        TMP_A[i] = a & m ;
        TMP_B[i] = b & m ;
      }
    }
    A0 = TMP_A ;
    B0 = TMP_B ;
    S0 = size ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
  }
  /**END MACRO EXEC_CMPX_BIT_INT**/  
}

void JD_exec_cmpx_bit_bit() {
  /**MACRO EXEC_CMPX_BIT_BIT**/
  {  
    int i, n, m, a, b, size ;
    if( TMP_DT->type == JD_D_sbit ) {
      size = TMP_DT->data.sbit.size ;
      m = ~(TMP_DT->data.sbit.a & TMP_DT->data.sbit.b) ;
      TMP_A[0] = TMP_DT->data.sbit.a & m ;
      TMP_B[0] = TMP_DT->data.sbit.b & m ;
    }
    else {  
      size = TMP_DT->data.bit.size ;
      ADJUST_TEMP(size) ;
      n = LASTWORD(size) ;
      for( i = 0 ; i <= n ; i++ ) {
        a = TMP_DT->data.bit.a[i] ;
        b = TMP_DT->data.bit.b[i] ;
        m = ~(a & b) ;
        TMP_A[i] = a & m ;
        TMP_B[i] = b & m ;
      }
    }
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      a = ACC_A[i] ;
      b = ACC_B[i] ;
      m = ~(a & b) ; /* mask x */
      ACC_A[i] = a & m ;
      ACC_B[i] = b & m ;
    }
    A0 = TMP_A ;
    B0 = TMP_B ;
    S0 = size ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMPX_BIT_BIT**/  
}


void JD_exec_cmpz_int_bit() {
  /**MACRO EXEC_CMPZ_INT_BIT**/
  {  
    int i, n, m, a, b ;
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      a = ACC_A[i] ;
      b = ACC_B[i] ;
      m = ~(~a & b) ; /* mask z */
      ACC_A[i] = a & m ;
      ACC_B[i] = b & m ;
    }
    A0 = &TMP_DT->data.int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMPZ_INT_BIT**/  
}


void JD_exec_cmpz_bit_int() {
  /**MACRO EXEC_CMPZ_BIT_INT**/
  {  
    int i, n, m, a, b, size ;
    if( TMP_DT->type == JD_D_sbit ) {
      size = TMP_DT->data.sbit.size ;
      m = ~(~TMP_DT->data.sbit.a & TMP_DT->data.sbit.b) ;
      TMP_A[0] = TMP_DT->data.sbit.a & m ;
      TMP_B[0] = TMP_DT->data.sbit.b & m ;
    }
    else {  
      size = TMP_DT->data.bit.size ;
      ADJUST_TEMP(size) ;
      n = LASTWORD(size) ;
      for( i = 0 ; i <= n ; i++ ) {
        a = TMP_DT->data.bit.a[i] ;
        b = TMP_DT->data.bit.b[i] ;
        m = ~(a & b) ;
        TMP_A[i] = a & m ;
        TMP_B[i] = b & m ;
      }
    }
    A0 = TMP_A ;
    B0 = TMP_B ;
    S0 = size ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
  }
  /**END MACRO EXEC_CMPZ_BIT_INT**/  
}

void JD_exec_cmpz_bit_bit() {
  /**MACRO EXEC_CMPZ_BIT_BIT**/
  {  
    int i, n, m, a, b, size ;
    if( TMP_DT->type == JD_D_sbit ) {
      size = TMP_DT->data.sbit.size ;
      m = ~(~TMP_DT->data.sbit.a & TMP_DT->data.sbit.b) ;
      TMP_A[0] = TMP_DT->data.sbit.a & m ;
      TMP_B[0] = TMP_DT->data.sbit.b & m ;
    }
    else {  
      size = TMP_DT->data.bit.size ;
      ADJUST_TEMP(size) ;
      n = LASTWORD(size) ;
      for( i = 0 ; i <= n ; i++ ) {
        a = TMP_DT->data.bit.a[i] ;
        b = TMP_DT->data.bit.b[i] ;
        m = ~(~a & b) ;
        TMP_A[i] = a & m ;
        TMP_B[i] = b & m ;
      }
    }
    n = LASTWORD(ACC_SIZE) ;
    for( i = 0 ; i <= n ; i++ ) {
      a = ACC_A[i] ;
      b = ACC_B[i] ;
      m = ~(~a & b) ; /* mask z */
      ACC_A[i] = a & m ;
      ACC_B[i] = b & m ;
    }
    A0 = TMP_A ;
    B0 = TMP_B ;
    S0 = size ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    ACC_INT = RET ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_CMPX_BIT_BIT**/  
}

void JD_exec_concat_int_int() {
  /**MACRO EXEC_CONCAT_INT_INT**/
  {  
    int nsize ;
    JD_data *dt ;
    dt = POP_STACK ;
    nsize = 64 ;
    if( ACC_WD < 2 ) {  /* extending ACC bit space */
      ACC_A = 
        (unsigned int *)realloc( ACC_A, NBYTE(nsize) ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, NBYTE(nsize) ) ;
      ACC_WD = 2 ;
    }
    ACC_A[0] = ACC_INT ;
    ACC_B[0] = 0 ;
    ACC_A[1] = dt->data.int_value ;
    ACC_B[1] = 0 ;
    ACC_SIZE = 64 ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_CONCAT_INT_INT**/  
}

void JD_exec_concat_bit_int() {
  /**MACRO EXEC_CONCAT_BIT_INT**/
  {  
    int i, n, j, size, nsize ;
    JD_data *dt ;
    dt = POP_STACK ;
    size = (dt->type == JD_D_sbit)?dt->data.sbit.size:dt->data.bit.size ;
    nsize = size+32 ;
    n = LASTWORD(nsize) ;
    if( ACC_WD <= n ) {  /* extending ACC bit space */
      ACC_A = 
        (unsigned int *)realloc( ACC_A, NBYTE(nsize) ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, NBYTE(nsize) ) ;
      ACC_WD = n+1 ;
    }
    ACC_A[0] = ACC_INT ;
    ACC_B[0] = 0 ;
    n = LASTWORD(size) ;
    if( dt->type == JD_D_sbit ) {
      ACC_A[1] = dt->data.sbit.a ;
      ACC_B[1] = dt->data.sbit.b ;
    }
    else {
      for( i = 0, j = 1 ; i <= n ; i++, j++ ) {
        ACC_A[j] = dt->data.bit.a[i] ;
        ACC_B[j] = dt->data.bit.b[i] ;
      }
    }
    ACC_SIZE = nsize ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_CONCAT_BIT_INT**/  
}
  
void JD_exec_concat_int_bit() {
  /**MACRO EXEC_CONCAT_INT_BIT**/
  {  
    int n, j, size, nsize, ls ;
    JD_data *dt ;
    dt = POP_STACK ;
    ls = (ACC_SIZE % 32) ;
    size = 32 ;
    nsize = size+ACC_SIZE ;
    n = LASTWORD(nsize) ;
    if( ACC_WD <= n ) {  /* extending ACC bit space */
      ACC_A = 
        (unsigned int *)realloc( ACC_A, NBYTE(nsize) ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, NBYTE(nsize) ) ;
      ACC_WD = n+1 ;
    }
    n = LASTWORD(size) ;
    j = LASTWORD(ACC_SIZE) ;

    if( ls == 0 ) {
      j++ ;
      ACC_A[j] = dt->data.int_value ;
      ACC_B[j] = 0 ;
    }
    else {
      ACC_A[j] = ACC_A[j] | (dt->data.int_value << ls) ;
      ACC_B[j] &= (1<<ls)-1 ;
      ACC_A[++j] = (unsigned int)dt->data.int_value >> (32-ls) ;
      ACC_B[j] = 0 ;
    }
    ACC_SIZE = nsize ;
  }
  /**END MACRO EXEC_CONCAT_INT_BIT**/  
}

void JD_exec_concat_bit_bit() {
  /**MACRO EXEC_CONCAT_BIT_BIT**/
  {  
    int i, n, j, size, nsize, ls, a, b ;
    JD_data *dt ;
    dt = POP_STACK ;
    ls = (ACC_SIZE % 32) ;
    size = (dt->type == JD_D_sbit)?dt->data.sbit.size:dt->data.bit.size ;
    nsize = size+ACC_SIZE ;
    n = LASTWORD(nsize) ;
    if( ACC_WD <= n ) {  /* extending ACC bit space */
      ACC_A = 
        (unsigned int *)realloc( ACC_A, NBYTE(nsize) ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, NBYTE(nsize) ) ;
      ACC_WD = n+1 ;
    }
    n = LASTWORD(size) ;
    j = LASTWORD(ACC_SIZE) ;
    if( ls == 0 ) {
      j++ ;
      a = b = 0 ;
    }
    else {
      a = ACC_A[j] & ( (unsigned int)0xffffffff >> (32-ls) ) ;
      b = ACC_B[j] & ( (unsigned int)0xffffffff >> (32-ls) ) ;
    }
    if( dt->type == JD_D_sbit ) {
      ACC_A[j] = a | (dt->data.sbit.a << ls) ;
      ACC_B[j] = b | (dt->data.sbit.b << ls) ;
      if( size + ls > 32 ) {
        ACC_A[++j] = (unsigned int)dt->data.sbit.a >> (32-ls)  ;
        ACC_B[j] = (unsigned int)dt->data.sbit.b >> (32-ls)  ;
      }
    }
    else {
      for( i = 0 ; i <= n ; i++ ) {
        ACC_A[j] = a | (dt->data.bit.a[i] << ls) ;
        ACC_B[j++] = b | (dt->data.bit.b[i] << ls) ;
        a = ls?((unsigned int)dt->data.bit.a[i] >> (32-ls)):0 ;
        b = ls?((unsigned int)dt->data.bit.b[i] >> (32-ls)):0 ;
      }
      if( (size+ls)%32 ) {
        ACC_A[j] = a ;
        ACC_B[j] = b ;
      }
    }
    ACC_SIZE = nsize ;
  }
  /**END MACRO EXEC_CONCAT_BIT_BIT**/  
}

void JD_exec_concat_string_string() {
  /**MACRO EXEC_CONCAT_STRING_STRING**/
  {  
    char *str = NULL ;
    int len = 0 ;
    JD_data *dt ;
    dt = POP_STACK ;
    if( ACC_STRING )
      len += strlen(ACC_STRING) ;
    if( dt->data.string )
     len += strlen(dt->data.string) ;
    if( len ) {
      str = ALLOC_STRING(len) ;
      if( ACC_STRING ) {
        strcat(str,ACC_STRING) ;
        DEALLOC_STRING(ACC_STRING) ;
      }
      if( dt->data.string )
        strcat(str, dt->data.string ) ;
    } 
    ACC_STRING = str ;
    ACC_TYPE = JD_D_string ;
  }
  /**END MACRO EXEC_CONCAT_STRING_STRING**/  
}


void JD_exec_lshift_bit() {
  /**MACRO EXEC_LSHIFT_BIT**/
  {  
    int shift, sft, rsft, idx ;
    int i, j, n, a, b, mm ;
    shift = ACC_INT ;
    CALL_EXEC_POP_ACC ;
    n = LASTWORD(ACC_SIZE) ;
    if( ACC_SIZE < shift ) {
      for( i = 0 ; i <= n ; i++ ){
        ACC_A[i] = 0 ;
        ACC_B[i] = 0 ;
      }
    }
    else if( shift ) {
      sft = shift % 32 ;
      rsft = sft?(32-sft):0 ;
      idx = shift / 32 ;
      j = n - idx ;
      mm = (unsigned)0xffffffff>>((32-(ACC_SIZE%32))%32) ;
      for( i = n ; i >= 0 ; i-- ) {
        if( j >= 0 ) {
          a = ACC_A[j] << sft ;
          b = ACC_B[j] << sft ;
          if( j-1 >= 0 && sft ) {
            a |= (unsigned int)ACC_A[j-1] >> rsft ;
            b |= (unsigned int)ACC_B[j-1] >> rsft ;
          }
        }
        else {
          a = b = 0 ;
        }
        ACC_A[i] = a ;
        ACC_B[i] = b ;
        j-- ;
      }
      ACC_A[n] &= mm ;
      ACC_B[n] &= mm ;
    }
  }
  /**END MACRO EXEC_LSHIFT_BIT**/
}

void JD_exec_rshift_bit() {
  /**MACRO EXEC_RSHIFT_BIT**/
  {  
    int shift, sft, rsft, idx ;
    int i, j, n, a, b, mm ;
    shift = ACC_INT ;
    CALL_EXEC_POP_ACC ;
    n = LASTWORD(ACC_SIZE) ;
    if( ACC_SIZE < shift ) {
      for( i = 0 ; i <= n ; i++ ){
        ACC_A[i] = 0 ;
        ACC_B[i] = 0 ;
      }
    }
    else if( shift ) {
      sft = shift % 32 ;
      rsft = sft?(32-sft):0 ;
      idx = shift / 32 ;
      j = idx ;
      mm = (unsigned)0xffffffff>>((32-(ACC_SIZE%32))%32) ;
      for( i = 0 ; i <= n ; i++ ) {
        if( j+1 <= n && sft ) {
          a = (unsigned int)ACC_A[j+1] << rsft ;
          b = (unsigned int)ACC_B[j+1] << rsft ;
        }
        else a = b = 0 ;
        if( j <= n ) {
          a |= (unsigned int)ACC_A[j] >> sft ;
          b |= (unsigned int)ACC_B[j] >> sft ;
        }
        ACC_A[i] = a ;
        ACC_B[i] = b ;
        j++ ;
      }
      ACC_A[n] &= mm ;
      ACC_B[n] &= mm ;
    }
  }
  /**END MACRO EXEC_RSHIFT_BIT**/
}

void JD_exec_lshift_int() {
  /**MACRO EXEC_LSHIFT_INT**/
  {  
    int shift ;
    shift = ACC_INT ;
    CALL_EXEC_POP_ACC ;
    ACC_INT = ACC_INT << shift ;
  }
  /**END MACRO EXEC_LSHIFT_INT**/
}


void JD_exec_rshift_int() {
  /**MACRO EXEC_RSHIFT_INT**/
  {  
    int shift ;
    shift = ACC_INT ;
    CALL_EXEC_POP_ACC ;
    ACC_INT = (int)ACC_INT >> shift ;
  }
  /**END MACRO EXEC_RSHIFT_INT**/
}


void JD_exec_urshift_int() {
  /**MACRO EXEC_URSHIFT_INT**/
  {  
    int shift ;
    shift = ACC_INT ;
    CALL_EXEC_POP_ACC ;
    ACC_INT = (unsigned int)ACC_INT >> shift ;
  }
  /**END MACRO EXEC_URSHIFT_INT**/
}


/*****************************************/
/* EQEQ for non-numeric types            */
/*****************************************/
void JD_exec_string_eqeq_string() {
  /**MACRO EXEC_STRING_EQEQ_STRING**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_STRING ) {
      if( dt->data.string && !strcmp(ACC_STRING,dt->data.string) )
        ACC_INT = 1 ;
      else {
        /* null and "" not match anymore 
        if( dt->data.string == NULL && ACC_STRING[0] == NULL_CHAR ) 
          ACC_INT = 1 ; 
        else 
        */
          ACC_INT = 0 ;
      }
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    else {
      if( dt->data.string == NULL ) /* || dt->data.string[0] == NULL_CHAR ) */
        ACC_INT = 1 ;
      else 
        ACC_INT = 0 ;
    }
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_STRING_EQEQ_STRING**/  
}

void JD_exec_signal_eqeq_signal() {
  /**MACRO EXEC_SIGNAL_EQEQ_SIGNAL**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.signal.port == dt->data.signal.port &&
        ACC_DT->data.signal.data0 == dt->data.signal.data0 &&
        ACC_DT->data.signal.data1 == dt->data.signal.data1    )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_SIGNAL_EQEQ_SIGNAL**/  
}


void JD_exec_string_ne_string() {
  /**MACRO EXEC_STRING_NE_STRING**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_STRING ) {
      if( dt->data.string && !strcmp(ACC_STRING,dt->data.string) )
        ACC_INT = 0 ;
      else {
        if( dt->data.string == NULL && ACC_STRING[0] == NULL_CHAR ) ACC_INT = 0 ; 
        else ACC_INT = 1 ;
      }
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    else {
      if( dt->data.string == NULL || dt->data.string[0] == NULL_CHAR )
        ACC_INT = 0 ;
      else 
        ACC_INT = 1 ;
    }
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_STRING_NE_STRING**/  
}

void JD_exec_signal_ne_signal() {
  /**MACRO EXEC_SIGNAL_NE_SIGNAL**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.signal.port == dt->data.signal.port &&
        ACC_DT->data.signal.data0 == dt->data.signal.data0 &&
        ACC_DT->data.signal.data1 == dt->data.signal.data1    )
      ACC_INT = 0 ;
    else
      ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_SIGNAL_NE_SIGNAL**/  
}


int JD_obj_eqeq_obj( JD_data *dt1, JD_data *dt2 ) {
  int i ;
  JD_object_table *tbl1, *tbl2 ;
  JD_data *mdt1, *mdt2 ;
  
  if( dt1->data.obj.alloc != dt2->data.obj.alloc ) return 0 ;
  tbl1 = dt1->data.obj.ect ;
  tbl2 = dt2->data.obj.ect ;
  if( tbl1 == NULL && tbl2 == NULL ) return 1 ;
  if( tbl1 == NULL || tbl2 == NULL ) return 0 ;
  if( (tbl1->super && tbl2->super == NULL) || 
      (tbl2->super && tbl1->super == NULL)    ) return 0 ;
  if( (tbl1->child && tbl2->child == NULL) || 
      (tbl2->child && tbl1->child == NULL)    ) return 0 ;
  if( tbl1->super ) {
    if( !JD_obj_eqeq_obj( tbl1->super, tbl2->super ) ) return 0 ;
  }
  if( tbl1->child ) {
    if( !JD_obj_eqeq_obj( tbl1->child, tbl2->child ) ) return 0 ;
  }
  for( i = 0 ; i < tbl1->num_entry ; i++ ) {
    mdt1 = tbl1->table[i] ;
    mdt2 = tbl2->table[i] ;
    if( mdt1 == NULL || mdt2 == NULL ) return 0 ; /* error?, avoid code dump */
    if( mdt1->type != mdt2->type ) return 0 ; /* another error? */
    if( mdt1->sub_type != mdt2->sub_type ) return 0 ;
    if( mdt1->sub_type != JD_SD_single ) {
      if( mdt1 != mdt2 ) return 0 ;
    }
    else {
      switch( mdt1->type ) {
        case JD_D_int:
          if( mdt1->data.int_value != mdt2->data.int_value ) return 0 ;
          break ;
        case JD_D_sbit:
          if( mdt1->data.sbit.a != mdt2->data.sbit.a ) return 0 ;
          if( mdt1->data.sbit.b != mdt2->data.sbit.b ) return 0 ;
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          {
            int i, n ;
            n = NWORD(mdt1->data.bit.size) ;
            for( i = 0 ; i < n ; i++ ) {
              if( mdt1->data.bit.a[i] != mdt2->data.bit.a[i] ) return 0 ;
              if( mdt1->data.bit.b[i] != mdt2->data.bit.b[i] ) return 0 ;
            }
          }
          break ;
        case JD_D_float:
          if( mdt1->data.float_value != mdt2->data.float_value ) return 0 ;
          break ;
        case JD_D_double:
          if( mdt1->data.double_value != mdt2->data.double_value ) return 0 ;
          break ;
        case JD_D_object:  /* only one level of compare */
          if( mdt1->data.obj.ect != mdt2->data.obj.ect ||
              mdt1->data.obj.alloc != mdt2->data.obj.alloc ) return 0 ;
          break ;
        case JD_D_thread:
          if( mdt1->data.thrd != mdt2->data.thrd ) return 0 ; 
          break ;
        case JD_D_string:
          if( (mdt1->data.string || mdt2->data.string ) ) 
          {
            if( mdt1->data.string == NULL || mdt2->data.string == NULL ) 
              return 0 ;
            if( strcmp( mdt1->data.string, mdt2->data.string ) ) return 0 ;
          }
          break ;
        case JD_D_signal:
          if( mdt1->data.signal.port != mdt2->data.signal.port ||
              mdt1->data.signal.data0 != mdt2->data.signal.data0 || 
              mdt1->data.signal.data1 != mdt2->data.signal.data1 
            ) return 0 ; 
          break ;
      } /* end of switch */
    } /* end of else */
  } /* end of for */
  return 1 ;
}

/* obj == obj must compare the contents in object table */
/*   must be implemented */
void JD_exec_obj_eqeq_obj() {
  /**MACRO EXEC_OBJ_EQEQ_OBJ**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    ACC_INT = JD_obj_eqeq_obj( ACC_DT, dt ) ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_OBJ_EQEQ_OBJ**/  
}

void JD_exec_obj_ne_obj() {
  /**MACRO EXEC_OBJ_NE_OBJ**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.obj.ect == dt->data.obj.ect &&
        ACC_DT->data.obj.alloc == dt->data.obj.alloc )
      ACC_INT = 0 ;
    else
      ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_OBJ_NE_OBJ**/  
}

void JD_exec_obj_eqeqeq_obj() {
  /**MACRO EXEC_OBJ_EQEQEQ_OBJ**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.obj.ect == dt->data.obj.ect &&
        ACC_DT->data.obj.alloc == dt->data.obj.alloc )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_OBJ_EQEQ_OBJ**/  
}

void JD_exec_obj_neqeq_obj() {
  /**MACRO EXEC_OBJ_NEQEQ_OBJ**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.obj.ect == dt->data.obj.ect &&
        ACC_DT->data.obj.alloc == dt->data.obj.alloc )
      ACC_INT = 0 ;
    else
      ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_OBJ_NE_OBJ**/  
}

void JD_exec_thread_eqeq_thread() {
  /**MACRO EXEC_THREAD_EQEQ_THREAD**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.thrd == dt->data.thrd )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_THREAD_EQEQ_THREAD**/  
}

void JD_exec_thread_eqeqeq_thread() {
  /**MACRO EXEC_THREAD_EQEQEQ_THREAD**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.thrd == dt->data.thrd )
      ACC_INT = 1 ;
    else
      ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_THREAD_EQEQEQ_THREAD**/  
}

void JD_exec_thread_ne_thread() {
  /**MACRO EXEC_THREAD_NE_THREAD**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.thrd == dt->data.thrd )
      ACC_INT = 0 ;
    else
      ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_THREAD_NE_THREAD**/  
}

void JD_exec_thread_neqeq_thread() {
  /**MACRO EXEC_THREAD_NEQEQ_THREAD**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    if( ACC_DT->data.thrd == dt->data.thrd )
      ACC_INT = 0 ;
    else
      ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_THREAD_NEQEQ_THREAD**/  
}


void JD_exec_pop_eqeq_null() {
  /**MACRO EXEC_POP_EQEQ_NULL**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    switch( dt->type ) {
      case JD_D_null:
        ACC_INT = 1 ;
        break ;
      case JD_D_object:
        if( dt->data.obj.ect == NULL ) ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_string:
        if( dt->data.string == NULL ) /* || dt->data.string[0] == NULL_CHAR ) */
          ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_signal:
        if( dt->data.signal.port == NULL ) ACC_INT = 1 ;
        else ACC_INT = 0 ;
    }
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_POP_EQEQ_NULL**/  
}

void JD_exec_pop_ne_null() {
  /**MACRO EXEC_POP_NE_NULL**/
  { 
    JD_data *dt ;
    dt = GET_STACK(1) ;
    switch( dt->type ) {
      case JD_D_null:
        ACC_INT = 0 ;
        break ;
      case JD_D_object:
        if( dt->data.obj.ect ) ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_string:
        if( dt->data.string ) /* && dt->data.string[0] != NULL_CHAR ) */
          ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_signal:
        if( dt->data.signal.port ) ACC_INT = 1 ;
        else ACC_INT = 0 ;
    }
    ACC_TYPE = JD_D_int ;
    UP_STACK ;
  }
  /**END MACRO EXEC_POP_NE_NULL**/  
}

void JD_exec_null_eqeq_acc() {
  /**MACRO EXEC_NULL_EQEQ_ACC**/
  { 
    UP_STACK ;
    switch( ACC_TYPE ) {
      case JD_D_null:
        ACC_INT = 1 ;
        break ;
      case JD_D_string:
        if( ACC_STRING == NULL ) /* || ACC_STRING[0] == NULL_CHAR ) */
          ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_dt:
        switch( ACC_DT->type ) {
          case JD_D_object:
            if( ACC_DT->data.obj.ect == NULL ) ACC_INT = 1 ;
            else ACC_INT = 0 ;
            break ;
          case JD_D_signal:
            if( ACC_DT->data.signal.port == NULL ) ACC_INT = 1 ;
            else ACC_INT = 0 ;
        }
        break ;
    }
    CLEAR_ACC ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_NULL_EQEQ_ACC**/  
}

void JD_exec_null_ne_acc() {
  /**MACRO EXEC_NULL_NE_ACC**/
  { 
    UP_STACK ;
    switch( ACC_TYPE ) {
      case JD_D_null:
        ACC_INT = 0 ;
        break ;
      case JD_D_string:
        if( ACC_STRING != NULL ) /* && ACC_STRING[0] != NULL_CHAR ) */
          ACC_INT = 1 ;
        else ACC_INT = 0 ;
        break ;
      case JD_D_dt:
        switch( ACC_DT->type ) {
          case JD_D_object:
            if( ACC_DT->data.obj.ect ) ACC_INT = 1 ;
            else ACC_INT = 0 ;
            break ;
          case JD_D_signal:
            if( ACC_DT->data.signal.port ) ACC_INT = 1 ;
            else ACC_INT = 0 ;
        }
        break ;
    }
    CLEAR_ACC ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_NULL_NE_ACC**/  
}

void JD_exec_gen_frame() {
  /**MACRO EXEC_GEN_FRAME**/
  { 
    JD_data *dt ;
    /* if( THREAD->stk_frame ) { */
      STACK[SP] = dt = JD_alloc_int() ;
      dt->data.int_value = THREAD->stk_frame ;
    /* } */
    THREAD->stk_frame = SP++ ;
  }
  /**END MACRO EXEC_GEN_FRAME**/  
}

void JD_exec_gen_m_frame() {
  /**MACRO EXEC_GEN_M_FRAME**/
  { 
    JD_data *dt ;
    dt = JD_alloc_int() ;
    PUSH_TO_STACK(dt) ;
    dt->data.int_value = THREAD->stk_frame ;
    THREAD->stk_frame = SP-1 ;
    if( THREAD->this ) {
      PUSH_TO_STACK(THREAD->this) ;  /* same previous object */
    }
    else {
      PUSH_TO_STACK( JD_alloc_int() ) ; /* dummy object */
    }
    THREAD->this = ACC_DT ; /* ACC_DT should hold new 'this' */
  }
  /**END MACRO EXEC_GEN_M_FRAME**/  
}

void JD_exec_gen_a_frame() {
  /**MACRO EXEC_GEN_A_FRAME**/
  { 
    JD_data *dt ;
    dt = JD_alloc_int() ;
    PUSH_TO_STACK(dt) ;
    dt->data.int_value = THREAD->stk_frame ;
    THREAD->stk_frame = SP-1 ;
    if( THREAD->this ) {
      PUSH_TO_STACK(THREAD->this) ;  /* same previous object */
    }
    else {
      PUSH_TO_STACK( JD_alloc_int() ) ; /* dummy object */
    }
    if( ACC_TYPE == JD_D_dt )
      THREAD->this = ACC_DT ; /* ACC_DT should hold new 'this' */
    else
      THREAD->this = NULL ;
  }
  /**END MACRO EXEC_GEN_A_FRAME**/  
}

void JD_exec_pop_this() {
  /**MACRO EXEC_POP_THIS**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame+1] ;
    if( dt->type == JD_D_object )
      THREAD->this = dt ;  /* pop previous object */
    else THREAD->this = NULL ;
  }
  /**END MACRO EXEC_POP_THIS**/  
}

/* pop_this for new function, returning 'this' in ACC */
void JD_exec_pop_this_new() {
  /**MACRO EXEC_POP_THIS_NEW**/
  { 
    JD_data *dt ;
    ACC_DT = THREAD->this ;
    ACC_TYPE = JD_D_dt ;
    dt = STACK[THREAD->stk_frame+1] ;
    if( dt->type == JD_D_object )
      THREAD->this = dt ;  /* pop previous object */
    else THREAD->this = NULL ;
  }
  /**END MACRO EXEC_POP_THIS_NEW**/  
}

void JD_exec_var_arg() {
  /**MACRO EXEC_VAR_ARG**/
  { 
    JD_data *dt ;
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->type = JD_D_pointer ;
    TMP_DT->sub_type = JD_SD_single ;
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    TMP_DT->data.ptr.pdata = dt ;
    PUSH_TO_STACK(TMP_DT) ;
    
  }
  /**END MACRO EXEC_VAR_ARG**/  
}


/*********************************************************/  
/* Copy argument single                                  */
/*   TMP_DT points to the local var just allocated       */
/*********************************************************/  

void JD_exec_copy_arg_single_int(){
  /**MACRO EXEC_COPY_ARG_SINGLE_INT**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( IS_SMLINT(dt) ) TMP_DT->data.int_value = SMLINT2INT(dt) ;
    else switch(dt->type) {
      case JD_D_int:
        TMP_DT->data.int_value = dt->data.int_value ;
        break ;
      case JD_D_bit:
        TMP_DT->data.int_value = dt->data.bit.a[0] ;
        /* may check if b is zero or not */
        break ;
      case JD_D_sbit:
        TMP_DT->data.int_value = dt->data.sbit.a ;
        /* may check if b is zero or not */
        break ;
      case JD_D_float:
        TMP_DT->data.int_value = dt->data.float_value ;
        break ;
      case JD_D_double:
        TMP_DT->data.int_value = dt->data.double_value ;
        break ;
    }
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_INT**/  
}

void JD_exec_copy_arg_single_float(){
  /**MACRO EXEC_COPY_ARG_SINGLE_FLOAT**/
  { 
    JD_data *dt ;
    float sft, flt ;
    int i, n ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( IS_SMLINT(dt) ) TMP_DT->data.float_value = SMLINT2INT(dt) ;
    else switch(dt->type) {
      case JD_D_int:
        TMP_DT->data.float_value = dt->data.int_value ;
        break ;
      case JD_D_bit:
        sft = 2.0 ;
        for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
        flt = 0.0 ;
        n = NWORD(ACC_SIZE) ;
        for( i = 0 ; i < n ; i++ ) {
          flt *= sft ;
          flt += dt->data.bit.a[i] ;
        }
        TMP_DT->data.double_value = flt ;
        /* may check if b is zero or not */
        break ;
      case JD_D_sbit:
        TMP_DT->data.float_value = dt->data.sbit.a ;
        /* may check if b is zero or not */
        break ;
      case JD_D_float:
        TMP_DT->data.float_value = dt->data.float_value ;
        break ;
      case JD_D_double:
        TMP_DT->data.float_value = dt->data.double_value ;
        break ;
    }
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_FLOAT**/  
}

void JD_exec_copy_arg_single_double(){
  /**MACRO EXEC_COPY_ARG_SINGLE_DOUBLE**/
  { 
    JD_data *dt ;
    double sft, flt ;
    int i, n ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( IS_SMLINT(dt) ) TMP_DT->data.double_value = SMLINT2INT(dt) ;
    else switch(dt->type) {
      case JD_D_int:
        TMP_DT->data.double_value = dt->data.int_value ;
        break ;
      case JD_D_bit:
        sft = 2.0 ;
        for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
        flt = 0.0 ;
        n = NWORD(ACC_SIZE) ;
        for( i = 0 ; i < n ; i++ ) {
          flt *= sft ;
          flt += dt->data.bit.a[i] ;
        }
        TMP_DT->data.double_value = flt ;
        /* may check if b is zero or not */
        break ;
      case JD_D_sbit:
        TMP_DT->data.double_value = dt->data.sbit.a ;
        /* may check if b is zero or not */
        break ;
      case JD_D_float:
        TMP_DT->data.double_value = dt->data.float_value ;
        break ;
      case JD_D_double:
        TMP_DT->data.double_value = dt->data.double_value ;
        break ;
    }
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_DOUBLE**/  
}

void JD_exec_copy_arg_single_bit(){
  /**MACRO EXEC_COPY_ARG_SINGLE_BIT**/
  { 
    JD_data *dt ;
    double sft, flt ;
    int msk, i, n ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( IS_SMLINT(dt) ) {
      int x = SMLINT2INT(dt) ;
      if( TMP_DT->type == JD_D_sbit ) {
        msk = BITMASK(TMP_DT->data.sbit.size) ;
        TMP_DT->data.sbit.a = x & msk ;
        TMP_DT->data.sbit.b = 0 ;
      }
      else {
        n = LASTWORD(TMP_DT->data.bit.size) ;
        if( n ) {
          TMP_DT->data.bit.a[0] = x ;
          TMP_DT->data.bit.b[0] = 0 ;
          for( i = 1 ; i <= n ; i++ ) {
            TMP_DT->data.bit.a[i] = 0 ;
            TMP_DT->data.bit.b[i] = 0 ;
          }
        }
        else {
          msk = BITMASK(TMP_DT->data.sbit.size) ;
          TMP_DT->data.bit.a[0] = x & msk ;
          TMP_DT->data.bit.b[0] = 0 ;
        }
      }
    }
    else switch(dt->type) {
      case JD_D_int:
        if( TMP_DT->type == JD_D_sbit ) {
          msk = BITMASK(TMP_DT->data.sbit.size) ;
          TMP_DT->data.sbit.a = dt->data.int_value & msk ;
          TMP_DT->data.sbit.b = 0 ;
        }
        else {
          n = LASTWORD(TMP_DT->data.bit.size) ;
          if( n ) {
            TMP_DT->data.bit.a[0] = dt->data.int_value ;
            TMP_DT->data.bit.b[0] = 0 ;
            for( i = 1 ; i <= n ; i++ ) {
              TMP_DT->data.bit.a[i] = 0 ;
              TMP_DT->data.bit.b[i] = 0 ;
            }
          }
          else {
            msk = BITMASK(TMP_DT->data.sbit.size) ;
            TMP_DT->data.bit.a[0] = dt->data.int_value & msk ;
            TMP_DT->data.bit.b[0] = 0 ;
          }
        }
        break ;
      case JD_D_bit:
        if( TMP_DT->type == JD_D_sbit ) {
          msk = BITMASK(TMP_DT->data.sbit.size) ;
          TMP_DT->data.sbit.a = dt->data.bit.a[0] & msk ;
          TMP_DT->data.sbit.b = dt->data.bit.b[0] & msk ;
        }
        else {
          int i, n ;
          if (dt->data.bit.size < TMP_DT->data.bit.size) {
            n = LASTWORD(dt->data.bit.size) ;
            for( i = 0 ; i <=n ; i++ ) {
              TMP_DT->data.bit.a[i] = dt->data.bit.a[i] ;
              TMP_DT->data.bit.b[i] = dt->data.bit.b[i] ;
            }
            n = LASTWORD(TMP_DT->data.bit.size) ;
            for( ; i <= n ; i++ ) {
              TMP_DT->data.bit.a[i] = 0 ;
              TMP_DT->data.bit.b[i] = 0 ;
            }
          }
          else {
            n = LASTWORD(TMP_DT->data.bit.size) ;
            msk = BITMASK(TMP_DT->data.bit.size) ;
            for( i = 0 ; i < n ; i++ ) {
              TMP_DT->data.bit.a[i] = dt->data.bit.a[i] ;
              TMP_DT->data.bit.b[i] = dt->data.bit.b[i] ;
            }
            TMP_DT->data.bit.a[n] = dt->data.bit.a[n] & msk ;
            TMP_DT->data.bit.b[n] = dt->data.bit.b[n] & msk ;
          }
        }
        break ;
      case JD_D_sbit:
        if( TMP_DT->type == JD_D_sbit ) {
          msk = BITMASK(TMP_DT->data.sbit.size) ;
          TMP_DT->data.sbit.a = dt->data.sbit.a & msk ;
          TMP_DT->data.sbit.b = dt->data.sbit.b & msk ;
        }
        else {
          TMP_DT->data.bit.a[0] = dt->data.sbit.a ;
          TMP_DT->data.bit.b[0] = dt->data.sbit.b ;
          n = LASTWORD(TMP_DT->data.bit.size) ;
          for( i = 1 ; i <= n ; i++ ) {
            TMP_DT->data.bit.a[i] = 0 ;
            TMP_DT->data.bit.b[i] = 0 ;
          }
        }
        break ;
      case JD_D_float:
        if( TMP_DT->type == JD_D_sbit ) {
          TMP_DT->data.sbit.a = dt->data.float_value ;
          TMP_DT->data.sbit.b = 0 ;
        }
        else {
          sft = 2.0 ;
          n = LASTWORD(TMP_DT->data.bit.size) ;
          for( i = 0 ; i <= n ; i++ ) {
            TMP_DT->data.bit.a[i] = 0 ;
            TMP_DT->data.bit.b[i] = 0 ;
          }
          for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
          i = 0 ;
          flt = dt->data.float_value ;
          while( flt > 1.0 ) {
            ACC_A[i++] = fmod(flt,sft) ;
            flt /= sft ;
            if( i == n ) break ;
          }
        }
        break ;
      case JD_D_double:
        if( TMP_DT->type == JD_D_sbit ) {
          TMP_DT->data.sbit.a = dt->data.double_value ;
          TMP_DT->data.sbit.b = 0 ;
        }
        else {
          sft = 2.0 ;
          n = LASTWORD(TMP_DT->data.bit.size) ;
          for( i = 0 ; i <= n ; i++ ) {
            TMP_DT->data.bit.a[i] = 0 ;
            TMP_DT->data.bit.b[i] = 0 ;
          }
          for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
          i = 0 ;
          flt = dt->data.double_value ;
          while( flt > 1.0 ) {
            ACC_A[i++] = fmod(flt,sft) ;
            flt /= sft ;
            if( i == n ) break ;
          }
        }
        break ;
      case JD_D_string:
        if( TMP_DT->type == JD_D_sbit ) 
        {
          int nb, j ;
          nb = (TMP_DT->data.sbit.size)/8 ;
          j = 0 ;
          TMP_DT->data.sbit.a = TMP_DT->data.sbit.b = 0 ;
          for(  ; nb > 0 ; nb-- ) {
            TMP_DT->data.sbit.a = 
              (TMP_DT->data.sbit.a << 8) | 
              ( ( dt->data.string[j] != NULL_CHAR)?dt->data.string[j++]:0 ) ;
          }
        }
        else {
          int nw, nb, size, j ;
          size = TMP_DT->data.bit.size/8 ;
          size *= 8 ; /* round to 8 */
          nw = LASTWORD(size) ;
          j = 0 ;
          nb = (size%32)/8 ;
          if( nb == 0 ) nb = 4 ;
          for(  ; nw >= 0 ; nw-- ) {
            TMP_DT->data.bit.a[nw] = TMP_DT->data.bit.b[nw] = 0 ;
            for(  ; nb > 0 ; nb-- ) {
              TMP_DT->data.bit.a[nw] = 
                (TMP_DT->data.bit.a[nw] << 8) | 
                ( ( dt->data.string[j] != NULL_CHAR)?dt->data.string[j++]:0 ) ;
            }
            nb = 4 ;
          }
        }
        break ;
    }
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_BIT**/  
}


void JD_exec_copy_arg_single_obj(){
  /**MACRO EXEC_COPY_ARG_SINGLE_OBJ**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( dt->type == JD_D_null ) TMP_DT->data.obj.ect = NULL ;
    else {
      TMP_DT->data.obj.ect = dt->data.obj.ect ;
      if( TMP_DT->data.obj.ect ) TMP_DT->data.obj.ect->ref_count++ ;
#ifndef  NO_OBJ_TYPE_CHECK
      if( dt->data.obj.alloc && 
          dt->data.obj.alloc != TMP_DT->data.obj.alloc ) 
      {
        ERROR_ILLEGAL_CAST_ASSIGNMENT_DETECTED ;
      }
#endif
      TMP_DT->data.obj.alloc = dt->data.obj.alloc ;
    }
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_OBJ**/  
}

void JD_exec_copy_arg_single_string(){
  /**MACRO EXEC_COPY_ARG_SINGLE_STRING**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    if( dt->data.string )
      TMP_DT->data.string = (char *)strdup(dt->data.string) ;
    else TMP_DT->data.string = NULL ;

  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_STRING**/  
}

void JD_exec_copy_arg_single_signal(){
  /**MACRO EXEC_COPY_ARG_SINGLE_SIGNAL**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    TMP_DT->data.signal.port = dt->data.signal.port ;
    TMP_DT->data.signal.data0 = dt->data.signal.data0 ;
    TMP_DT->data.signal.data1 = dt->data.signal.data1 ;

  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_SIGNAL**/  
}

void JD_exec_copy_arg_single_thread(){
  /**MACRO EXEC_COPY_ARG_SINGLE_THREAD**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    TMP_DT->data.thrd = dt->data.thrd ;
  }
  /**END MACRO EXEC_COPY_ARG_SINGLE_THREAD**/  
}


/*********************************************************/  
/* Copy argument array                                   */
/*   TMP_DT points to the local var just allocated       */
/*********************************************************/  
void JD_exec_arg_non_single(){
  /**MACRO EXEC_ARG_NON_SINGLE**/
  { 
    JD_data *dt ;
    
    dt = STACK[THREAD->stk_frame-TMP_INT] ;
    TMP_DT = JD_duplicate_data( dt ) ;
    PUSH_TO_STACK(TMP_DT) ;

  }
  /**END MACRO EXEC_ARG_NON_SINGLE**/  
}

/*********************************************************/  
/* Adjust top of stack data with last store size         */
/*   Used as a part of list assigment                    */
/*********************************************************/  
void JD_exec_store_adjust_int(){
  /**MACRO EXEC_STORE_ADJUST_INT**/
  {     
    CALL_EXEC_POP_ACC ;
    if( STORE_SIZE >= 32 ) ACC_INT = 0 ;
    else ACC_INT = (unsigned int)ACC_INT >> STORE_SIZE ;
    CALL_EXEC_PUSH_ACC ;

  }
  /**END MACRO EXEC_STORE_ADJUST_INT**/  
}

void JD_exec_store_adjust_bit(){
  /**MACRO EXEC_STORE_ADJUST_BIT**/
  {     
    int n ;
    ACC_INT = STORE_SIZE ;
    CALL_EXEC_RSHIFT_BIT ;
    CALL_EXEC_PUSH_ACC ;

  }
  /**END MACRO EXEC_STORE_ADJUST_BIT**/  
}


/*********************************************************/  
/* Save and restore ACC to save area                     */
/*   Used at the top and end of advice                   */
/*********************************************************/  

void JD_save_acc() {
  /**MACRO EXEC_SAVE_ACC**/
  {
    S_ACC_TYPE = ACC_TYPE ;
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        S_ACC_SIZE = ACC_SIZE ;
        if( ACC_SIZE <= 32 ) {
          S_ACC_A[0] = ACC_A[0] ;
          S_ACC_B[0] = ACC_B[0] ;
        }
        else {
          int i, n ;
          n = NWORD(ACC_SIZE) ;
          if( n > S_ACC_WD ) {
            S_ACC_A = 
              (unsigned int *)realloc( S_ACC_A, NBYTE(ACC_SIZE) ) ;
            S_ACC_B = 
              (unsigned int *)realloc( S_ACC_B, NBYTE(ACC_SIZE) ) ;
            S_ACC_WD = n ;
          }
          for( i = 0 ; i < n ; i++ ) {
            S_ACC_A[i] = ACC_A[i] ;
            S_ACC_B[i] = ACC_B[i] ;
          }
        }
        break ;
      case JD_D_int:
        S_ACC_INT = ACC_INT ;
        break ;
      case JD_D_float:
      case JD_D_double:
        S_ACC_FLOAT = ACC_FLOAT ;
        break ;
      case JD_D_string:
        S_ACC_STRING = ACC_STRING ;
        ACC_STRING = NULL ;
        ACC_TYPE = JD_D_void ;
        break ;
      case JD_D_dt:
        S_ACC_DT = ACC_DT ;
        ACC_DT = NULL ;
        ACC_TYPE = JD_D_null ;
        break ;
      case JD_D_null:
      default:
        break ;
    }
    
  }
  /**END MACRO EXEC_SAVE_ACC**/  
}


void JD_restore_acc() {
  /**MACRO EXEC_RESTORE_ACC**/
  {  
    if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    ACC_TYPE = S_ACC_TYPE ;
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        ACC_SIZE = S_ACC_SIZE ;
        if( ACC_SIZE <= 32 ) {
          S_ACC_A[0] = ACC_A[0] ;
          S_ACC_B[0] = ACC_B[0] ;
        }
        else {
          int i, n ;
          n = NWORD(ACC_SIZE) ;
          if( n > ACC_WD ) {
            ACC_A = 
              (unsigned int *)realloc( ACC_A, NBYTE(ACC_SIZE) ) ;
            ACC_B = 
              (unsigned int *)realloc( ACC_B, NBYTE(ACC_SIZE) ) ;
            ACC_WD = n ;
          }
          for( i = 0 ; i < n ; i++ ) {
            ACC_A[i] = S_ACC_A[i] ;
            ACC_B[i] = S_ACC_B[i] ;
          }
        }
        break ;
      case JD_D_int:
        ACC_INT = S_ACC_INT ;
        break ;
      case JD_D_double:
      case JD_D_float:
        ACC_FLOAT = S_ACC_FLOAT ;
        break ;
      case JD_D_string:
        ACC_STRING = S_ACC_STRING ;
        S_ACC_STRING = NULL ;
        break ;
      case JD_D_dt:
        ACC_TYPE = JD_D_dt ;
        ACC_DT = S_ACC_DT ;
        S_ACC_DT = NULL ;
        S_ACC_TYPE = JD_D_null ;
        break ;
      case JD_D_null:
      default:
        break ;
    }
  }
  /**END MACRO EXEC_RESTORE_ACC**/  
}

void JD_get_return() {
  /**MACRO EXEC_GET_RETURN**/
  {  
    if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) {
      DEALLOC_STRING(ACC_STRING) ;
      ACC_STRING = NULL ;
    }
    ACC_TYPE = S_ACC_TYPE ;
    switch(ACC_TYPE) {
      case JD_D_sbit:
      case JD_D_vbit:
      case JD_D_bit:
        ACC_SIZE = S_ACC_SIZE ;
        if( ACC_SIZE <= 32 ) {
          S_ACC_A[0] = ACC_A[0] ;
          S_ACC_B[0] = ACC_B[0] ;
        }
        else {
          int i, n ;
          n = NWORD(ACC_SIZE) ;
          if( n > ACC_WD ) {
            ACC_A = 
              (unsigned int *)realloc( ACC_A, NBYTE(ACC_SIZE) ) ;
            ACC_B = 
              (unsigned int *)realloc( ACC_B, NBYTE(ACC_SIZE) ) ;
            ACC_WD = n ;
          }
          for( i = 0 ; i < n ; i++ ) {
            ACC_A[i] = S_ACC_A[i] ;
            ACC_B[i] = S_ACC_B[i] ;
          }
        }
        break ;
      case JD_D_int:
        ACC_INT = S_ACC_INT ;
        break ;
      case JD_D_double:
      case JD_D_float:
        ACC_FLOAT = S_ACC_FLOAT ;
        break ;
      case JD_D_string:
        ACC_STRING = (char *)strdup(S_ACC_STRING) ;
        break ;
      case JD_D_dt:
        ACC_TYPE = JD_D_dt ;
        ACC_DT = S_ACC_DT ;
        break ;
      case JD_D_null:
      default:
        ERROR_NO_RETURN_VALUE_IN_CURRENT_ADVICE ;
        break ;
    }
  }
  /**END MACRO EXEC_GET_RETURN**/  
}
