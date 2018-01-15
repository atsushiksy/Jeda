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

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#endif

JD_data *JD_get_ptr_element( JD_data *dt ) ;
void JEDA_pli_get_cycle( int *ret ) ;
void JEDA_pli_get_time( int *high, int *lo ) ;

void JD_drive_port( 
  JD_pli_object *obj, unsigned *a_vec, unsigned *b_vec, int size,
  int partial, int ub, int lb,
  int strength  /* 0:weak, 1:normal, 2:strong */
) ;

/* get cycle count */
void JD_sf_get_cycle() {
  JEDA_pli_get_cycle( &ACC_A[0] ) ;
  ACC_B[0] = 0 ;
  ACC_SIZE = 32 ;
  ACC_TYPE = JD_D_sbit ;
}

void JD_sf_get_time() {
  JEDA_pli_get_time( &ACC_A[1], &ACC_A[0] ) ;
  ACC_B[0] = 0 ;
  ACC_B[0] = 0 ;
  ACC_SIZE = 64 ;
  ACC_TYPE = JD_D_bit ;
}

/*
  Sync on the given number of given edge of a port. The sample is 
  done on asynchronously (at any change on the port immediately).
*/
void JD_sync_on_port(
  JD_pli_object *obj, JD_edge_type edge, int num_cycle
) ;

/*
  Sync on the given number of given edge of a port sampled at the
  proper edge of given ref-clock.
*/
void JD_sync_port_on_clk(
  JD_pli_object *obj, JD_edge_type edge, int num_cycle 
) ;

void JD_exec_store_port() {
  /**MACRO EXEC_STORE_PORT**/
  {  /* TMP_INT holds global variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
  }
  /**END MACRO EXEC_STORE_PORT**/  
}


void JD_exec_store_port_subbit() {
  /**MACRO EXEC_STORE_PORT_SUBBIT**/
  {  /* TMP_INT holds global variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    int ub, lb ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    JD_drive_port( 
      obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
    ) ;
  }
  /**END MACRO EXEC_STORE_PORT_SUBBIT**/  
}

void JD_exec_store_sig_g() {
  /**MACRO EXEC_STORE_SIG_G**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G**/  
}

void JD_exec_store_sig_g_subbit() {
  /**MACRO EXEC_STORE_SIG_G_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    int ub, lb ;
    dt = GLOBAL_DATA[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_SUBBIT**/  
}

/*
 *  Store array
 */
void JD_exec_store_sig_g_array() {
  /**MACRO EXEC_STORE_SIG_G_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_ARRAY**/  
}

/*
 *  Store array subbit
 */
void JD_exec_store_sig_g_array_subbit() {
  /**MACRO EXEC_STORE_SIG_G_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_ARRAY_SUBBIT**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to TMP_INT2
 */ 
void JD_exec_store_sig_g_marray() {
  /**MACRO EXEC_STORE_SIG_G_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_MARRAY**/  
}

/*
 * store marray subbit
 */ 
void JD_exec_store_sig_g_marray_subbit() {
  /**MACRO EXEC_STORE_SIG_G_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_MARRAY_SUBBIT**/  
}

void JD_exec_store_sig_g_assoc() {
  /**MACRO EXEC_STORE_SIG_G_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_ASSOC**/  
}

void JD_exec_store_sig_g_assoc_subbit() {
  /**MACRO EXEC_STORE_SIG_G_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_ASSOC_SUBBIT**/  
}

void JD_exec_store_sig_g_stridx() {
  /**MACRO EXEC_STORE_SIG_G_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      DEALLOC_STRING(TMP_STRING) ;
      dt = hsh->data ;
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_STRIDX**/  
}

void JD_exec_store_sig_g_stridx_subbit() {
  /**MACRO EXEC_STORE_SIG_G_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    dt = GLOBAL_DATA[TMP_INT] ;
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_G_STRIDX_SUBBIT**/  
}

void JD_exec_store_sig_l() {
  /**MACRO EXEC_STORE_SIG_L**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L**/  
}

void JD_exec_store_sig_l_subbit() {
  /**MACRO EXEC_STORE_SIG_L_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    int ub, lb ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_SUBBIT**/  
}

/*
 *  Store array
 */
void JD_exec_store_sig_l_array() {
  /**MACRO EXEC_STORE_SIG_L_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_ARRAY**/  
}

/*
 *  Store array subbit
 */
void JD_exec_store_sig_l_array_subbit() {
  /**MACRO EXEC_STORE_SIG_L_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_ARRAY_SUBBIT**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to TMP_INT2
 */ 
void JD_exec_store_sig_l_marray() {
  /**MACRO EXEC_STORE_SIG_L_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_MARRAY**/  
}

/*
 * store marray subbit
 */ 
void JD_exec_store_sig_l_marray_subbit() {
  /**MACRO EXEC_STORE_SIG_L_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_MARRAY_SUBBIT**/  
}

void JD_exec_store_sig_l_assoc() {
  /**MACRO EXEC_STORE_SIG_L_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_ASSOC**/  
}

void JD_exec_store_sig_l_assoc_subbit() {
  /**MACRO EXEC_STORE_SIG_L_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_ASSOC_SUBBIT**/  
}

void JD_exec_store_sig_l_stridx() {
  /**MACRO EXEC_STORE_SIG_L_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      DEALLOC_STRING(TMP_STRING) ;
      dt = hsh->data ;
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_STRIDX**/  
}

void JD_exec_store_sig_l_stridx_subbit() {
  /**MACRO EXEC_STORE_SIG_L_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    dt = STACK[THREAD->stk_frame + TMP_INT + 1 ] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_L_STRIDX_SUBBIT**/  
}

void JD_exec_store_sig_m() {
  /**MACRO EXEC_STORE_SIG_M**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M**/  
}

void JD_exec_store_sig_m_subbit() {
  /**MACRO EXEC_STORE_SIG_M_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_SUBBIT**/  
}

/*
 *  Store array
 */
void JD_exec_store_sig_m_array() {
  /**MACRO EXEC_STORE_SIG_M_ARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_ARRAY**/  
}

/*
 *  Store array subbit
 */
void JD_exec_store_sig_m_array_subbit() {
  /**MACRO EXEC_STORE_SIG_M_ARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_array ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_ARRAY_SUBBIT**/  
}

/*
 * store marray access assumes that the actual offset is
 * calculated to TMP_INT2
 */ 
void JD_exec_store_sig_m_marray() {
  /**MACRO EXEC_STORE_SIG_M_MARRAY**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      JD_drive_port(
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_MARRAY**/  
}

/*
 * store marray subbit
 */ 
void JD_exec_store_sig_m_marray_subbit() {
  /**MACRO EXEC_STORE_SIG_M_MARRAY_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_data_unit *ut ;
    JD_pli_object *obj ;
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( dt->sub_type != JD_SD_marray ) {
      ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
    }
#endif
    ut = &dt->data.array.body[TMP_INT2] ;
    dt = ut->data ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( dt->data.signal.data0 < 0 ) {
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
    else {
      U_BIT = dt->data.signal.data1 ;
      L_BIT = dt->data.signal.data0 ;
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      JD_drive_port( 
        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
      ) ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_MARRAY_SUBBIT**/  
}

void JD_exec_store_sig_m_assoc() {
  /**MACRO EXEC_STORE_SIG_M_ASSOC**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_ASSOC**/  
}

void JD_exec_store_sig_m_assoc_subbit() {
  /**MACRO EXEC_STORE_SIG_M_ASSOC_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs ;
    JD_pli_object *obj ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_ASSOC_SUBBIT**/  
}

void JD_exec_store_sig_m_stridx() {
  /**MACRO EXEC_STORE_SIG_M_STRIDX**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        JD_drive_port(
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_STRIDX**/  
}

void JD_exec_store_sig_m_stridx_subbit() {
  /**MACRO EXEC_STORE_SIG_M_STRIDX_SUBBIT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_hash_entry *hsh, *phsh ;
    int hs, t ;
    JD_pli_object *obj ;
    
    if( TMP_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
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
      obj = (JD_pli_object *)dt->data.signal.port ;
      if( obj == NULL ) {
        ERROR_NULL_SIGNAL_ACCESS ;
      }
      if( dt->data.signal.data0 < 0 ) {
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
      else {
        U_BIT = dt->data.signal.data1 ;
        L_BIT = dt->data.signal.data0 ;
        if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX > L_INDEX ) {
            ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += U_BIT ;
          L_INDEX += U_BIT ;
        }
        else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
          if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
            ERROR_BIT_OFFSET_OUT_OF_RANGE ;
          }
          if( U_INDEX < L_INDEX ) {
            ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
          }
#endif    
          U_INDEX += L_BIT ;
          L_INDEX += L_BIT ;
        }
        JD_drive_port( 
          obj, ACC_A, ACC_B, ACC_SIZE, 1, U_INDEX, L_INDEX, DRV_STRENGTH 
        ) ;
      }
    }
    else {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
  }
  /**END MACRO EXEC_STORE_SIG_M_STRIDX_SUBBIT**/  
}


/* load port data */

void JD_exec_load_port() {
  /**MACRO EXEC_LOAD_PORT**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    JD_sample_port( obj, ACC_INT, 0, 0, 0 ) ;
  }
  /**END MACRO EXEC_LOAD_PORT**/  
}

void JD_exec_load_port_sub() {
  /**MACRO EXEC_LOAD_PORT_SUB**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    TMP_INT = ACC_INT ; /* depth */ 
    CALL_EXEC_POP_ACC ; /* lb */ 
    L_INDEX = ACC_INT ; 
    CALL_EXEC_POP_ACC ; /* ub */ 
    U_INDEX = ACC_INT ; 
    JD_sample_port( obj, TMP_INT, 1, U_INDEX, L_INDEX ) ;
  }
  /**END MACRO EXEC_LOAD_PORT**/  
}

void JD_exec_load_port_ptr() {
  /**MACRO EXEC_LOAD_PORT_PTR**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    ACC_DT = JD_alloc_data_entry() ;
    ACC_DT->data.signal.port = dt->data.signal.port ;
    ACC_DT->data.signal.data0 = dt->data.signal.data0 ;
    ACC_DT->data.signal.data1 = dt->data.signal.data1 ;
    ACC_DT->type = JD_D_signal ;
    ACC_TYPE = JD_D_dt ;
  }
  /**END MACRO EXEC_LOAD_PORT_PTR**/  
}

void JD_exec_load_port_ptr_sub() {
  /**MACRO EXEC_LOAD_PORT_PTR_SUB**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    obj = (JD_pli_object *)dt->data.signal.port ;
    L_INDEX = ACC_INT ; 
    CALL_EXEC_POP_ACC ; /* ub */ 
    U_INDEX = ACC_INT ; 
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
    if( obj->is.port.iendian ) {
      if( U_INDEX > L_INDEX ) {
        ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
      }
      else if( U_INDEX < obj->is.port.ub || L_INDEX > obj->is.port.lb ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
    else {
      if( U_INDEX < L_INDEX ) {
        ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
      }
      else if( U_INDEX > obj->is.port.ub || L_INDEX < obj->is.port.lb ) {
        ERROR_BIT_OFFSET_OUT_OF_RANGE ;
      }
    }
#endif    
    ACC_DT = JD_alloc_data_entry() ;
    ACC_DT->data.signal.port = dt->data.signal.port ;
    ACC_DT->data.signal.data0 = L_INDEX ;
    ACC_DT->data.signal.data1 = U_INDEX ;
    ACC_DT->type = JD_D_signal ;
    ACC_TYPE = JD_D_dt ;
  }
  /**END MACRO EXEC_LOAD_PTR_PORT**/  
}

void JD_exec_load_sig_pt() {
  /**MACRO EXEC_LOAD_SIG_PT**/
  {  /* TMP_INT holds local variable index */
    JD_pli_object *obj ;
    TMP_INT = ACC_INT ;
    CALL_EXEC_POP_ACC ; /* signal */ 
    obj = (JD_pli_object *)ACC_DT->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    if( ACC_DT->data.signal.data0 < 0 )
      JD_sample_port( obj, TMP_INT, 0, 0, 0 ) ;
    else {
      U_BIT = ACC_DT->data.signal.data1 ;
      L_BIT = ACC_DT->data.signal.data0 ;
      JD_sample_port( obj, TMP_INT, 1, U_BIT, L_BIT ) ;
    }
    ACC_DT = NULL ;
  }
  /**END MACRO EXEC_LOAD_SIG_PT**/  
}

void JD_exec_load_sig_pt_sub() {
  /**MACRO EXEC_LOAD_SIG_PT_SUB**/
  {  /* TMP_INT holds local variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    TMP_INT = ACC_INT ; /* depth */
    CALL_EXEC_POP_ACC ; /* lb */ 
    L_INDEX = ACC_INT ; 
    CALL_EXEC_POP_ACC ; /* ub */ 
    U_INDEX = ACC_INT ; 
    CALL_EXEC_POP_ACC ; /* signal */ 
    obj = (JD_pli_object *)ACC_DT->data.signal.port ;
    if( obj == NULL ) {
      ERROR_NULL_SIGNAL_ACCESS ;
    }
    U_BIT = ACC_DT->data.signal.data1 ;
    L_BIT = ACC_DT->data.signal.data0 ;
    if( U_BIT < 0 ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
      if( obj->is.port.iendian ) {
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
        else if( U_INDEX < obj->is.port.ub || L_INDEX > obj->is.port.lb ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
      }
      else {
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
        else if( U_INDEX > obj->is.port.ub || L_INDEX < obj->is.port.lb ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
      }
#endif    
    }
    else {
      if( obj->is.port.iendian ) {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX >(L_BIT-U_BIT) || L_INDEX > (L_BIT-U_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX > L_INDEX ) {
          ERROR_UPPER_BIT_LARGER_THAN_LOWER_BIT_FOR_SMALL_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += U_BIT ;
        L_INDEX += U_BIT ;
      }
      else {
#ifndef SKIP_ARRAY_SUBTYPE_CHECK
        if( U_INDEX > (U_BIT-L_BIT) || L_INDEX > (U_BIT-L_BIT) ) {
          ERROR_BIT_OFFSET_OUT_OF_RANGE ;
        }
        if( U_INDEX < L_INDEX ) {
          ERROR_UPPER_BIT_SMALLER_THAN_LOWER_BIT_FOR_BIG_ENDIAN_BIT_ACCESS ;
        }
#endif    
        U_INDEX += L_BIT ;
        L_INDEX += L_BIT ;
      }
      
    }
  
    JD_sample_port( obj, TMP_INT, 1, U_INDEX, L_INDEX ) ;
    ACC_DT = NULL ;
  }
  /**END MACRO EXEC_LOAD_SIG_PT_SUB**/  
}

void JD_exec_sync_port() {
  /**MACRO EXEC_SYNC_PORT**/
  {  /* TMP_INT holds global variable index */
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;
    if( THREAD->terminated ) {
      JD_thread_exit( THREAD ) ;
    }
  }
  /**END MACRO EXEC_SYNC_PORT**/  
}

void JD_exec_sync_clk_of_port() {
  /**MACRO EXEC_SYNC_CLOCK_OF_PORT**/
  {  /* TMP_INT holds global variable index */
    JD_data *dt, *clk ;
    JD_pli_object *obj ;
    dt = GLOBAL_DATA[TMP_INT] ;
    if( dt == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    clk = GLOBAL_DATA[obj->is.port.clk_index] ;
    if( clk == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    obj = (JD_pli_object *)clk->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;
    if( THREAD->terminated ) {
      JD_thread_exit( THREAD ) ;
    }
  }
  /**END MACRO EXEC_SYNC_CLOCK_OF_PORT**/  
}

void JD_exec_sync_clk_of_signal() {
  /**MACRO EXEC_SYNC_CLOCK_OF_SIGNAL**/
  {  
    JD_data *dt, *clk ;
    JD_pli_object *obj ;
    dt = ACC_DT ;
    if( dt == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    clk = GLOBAL_DATA[obj->is.port.clk_index] ;
    if( clk == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    obj = (JD_pli_object *)clk->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    CALL_EXEC_POP_ACC ; /* num_cycle */
    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;
    if( THREAD->terminated ) {
      JD_thread_exit( THREAD ) ;
    }
  }
  /**END MACRO EXEC_SYNC_CLOCK_OF_SIGNAL**/  
}


/* EXEC_SYNC_SIGNAL */
void JD_exec_sync_signal() {
  /**MACRO EXEC_SYNC_SIGNAL**/
  {  
    JD_data *dt ;
    JD_pli_object *obj ;
    dt = ACC_DT ;
    if( dt == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    obj = (JD_pli_object *)dt->data.signal.port ;
    if( obj == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    CALL_EXEC_POP_ACC ; /* num_cycle */
    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;
  }
  /**END MACRO EXEC_SYNC_SIGNAL**/  
}


