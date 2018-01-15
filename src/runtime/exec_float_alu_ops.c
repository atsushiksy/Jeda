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
#include <stdio.h>
#include "runtime.h"
#include "mem_macro.h"
#include "runtime_error_msg.h"
#include "bit_operations.h"
#include "bit_macro.h"
#include "exec_macro.h"

/***********************************/
/* binary float ALU ops            */
/***********************************/
void JD_exec_f_lt_f() {
  /**MACRO EXEC_F_LT_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value < ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_LT_F**/  
}

void JD_exec_f_gt_f() {
  /**MACRO EXEC_F_GT_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value > ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_GT_F**/  
}

void JD_exec_f_eqeq_f() {
  /**MACRO EXEC_F_EQEQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_EQEQ_F**/  
}

void JD_exec_f_le_f() {
  /**MACRO EXEC_F_LE_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value <= ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_LE_F**/  
}

void JD_exec_f_ge_f() {
  /**MACRO EXEC_F_GE_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value >= ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_GE_F**/  
}

void JD_exec_f_ne_f() {
  /**MACRO EXEC_F_NE_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value != ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_NE_F**/  
}

void JD_exec_f_eqeqeq_f() {
  /**MACRO EXEC_F_EQEQEQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_EQEQEQ_F**/  
}

void JD_exec_f_neqeq_f() {
  /**MACRO EXEC_F_NEQEQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value != ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_NEQEQ_F**/  
}

void JD_exec_f_eqQeq_f() {
  /**MACRO EXEC_F_EQQEQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_EQQEQ_F**/  
}

void JD_exec_f_eqeqQ_f() {
  /**MACRO EXEC_F_EQEQQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_EQEQQ_F**/  
}

void JD_exec_f_Qeqeq_f() {
  /**MACRO EXEC_F_QEQEQ_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( dt->data.float_value == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_F_QEQEQ_F**/  
}

void JD_exec_f_minus_f() {
  /**MACRO EXEC_F_MINUS_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    ACC_FLOAT = dt->data.float_value - ACC_FLOAT ;
  }
  /**END MACRO EXEC_F_MINUS_F**/  
}

void JD_exec_f_plus_f() {
  /**MACRO EXEC_F_PLUS_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    ACC_FLOAT = dt->data.float_value + ACC_FLOAT ;
  }
  /**END MACRO EXEC_F_PLUS_F**/  
}

void JD_exec_f_times_f() {
  /**MACRO EXEC_F_TIMES_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    ACC_FLOAT = dt->data.float_value * ACC_FLOAT ;
  }
  /**END MACRO EXEC_F_TIMES_F**/  
}

void JD_exec_f_div_f() {
  /**MACRO EXEC_F_DIV_F**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    if( ACC_FLOAT == 0.0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_FLOAT = dt->data.float_value / ACC_FLOAT ;
  }
  /**END MACRO EXEC_F_DIV_F**/  
}

void JD_exec_f_mod_f() {
  /**MACRO EXEC_F_MOD_F**/
  {
    JD_data *dt ;
    double p ;
    dt = POP_STACK ;
    p = dt->data.int_value ;
    if( ACC_FLOAT == 0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_FLOAT = fmod(p, ACC_FLOAT) ;
  }
  /**END MACRO EXEC_F_MOD_F**/  
}

/***********************************/
/* unary bit ALU ops               */
/***********************************/

void JD_exec_u_minus_f() {
  /**MACRO EXEC_U_MINUS_F**/
  {
    ACC_FLOAT = -ACC_FLOAT ;
  }
  /**END MACRO EXEC_U_MINUS_F**/  
}

void JD_exec_u_tilde_f() {
  /**MACRO EXEC_U_TILDE_F**/
  {
    ACC_FLOAT = -ACC_FLOAT ;
  }
  /**END MACRO EXEC_U_TILDE_F**/  
}

void JD_exec_u_not_f() {
  /**MACRO EXEC_U_NOT_F**/
  {
    ACC_INT = (ACC_FLOAT==0.0) ? 0 : 1 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_U_NOT_F**/  
}


/***********************************/
/* mixed type binary float ALU ops */
/***********************************/

int JD_check_bit_set( int *b, int size )
{
  int i ;
  int mask ;
  
  for( i = 0 ; i < NWORD(size) ; i++ ) {
    mask = 0xffffffff ;
    if( i == LASTWORD(size) ) mask >>= ((32-(size%32))%32) ;
    if( b[i] & mask ) return(1) ;
  }
  return(0) ;
}

double JD_bit_2_real( int *a, int size )
{
  double ret, sft ;
  int i ;
  int mask ;
  
  sft = 2.0 ;
  for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
  ret = 0.0 ;
  for( i = 0 ; i < NWORD(size) ; i++ ) {
    ret *= sft ;
    mask = 0xffffffff ;
    if( i == LASTWORD(size) ) mask >>= ((32-(size%32))%32) ;
    ret += (double) (a[i] & mask) ;
  }
}

static void convert_to_float( JD_data *dt ) {
  if( dt->type == JD_D_int ) TMP_FLOAT = dt->data.int_value ;
  else if( dt->type == JD_D_float ) TMP_FLOAT = dt->data.float_value ;
  else if( dt->type == JD_D_sbit ) {
#ifdef JEDA_RUNTIME_ERROR_CHECK
    if( dt->data.sbit.b ) 
      ERROR_ILLEGAL_DATA_FOR_FLOAT_CONVERSION ;
#endif
    TMP_FLOAT = dt->data.sbit.a ;
  }
  else if( dt->type == JD_D_bit ) {
#ifdef JEDA_RUNTIME_ERROR_CHECK
    if( JD_check_bit_set(dt->data.bit.b, dt->data.bit.size) ) 
      ERROR_ILLEGAL_DATA_FOR_FLOAT_CONVERSION ;
#endif
    TMP_FLOAT = JD_bit_2_real(dt->data.bit.a, dt->data.bit.size) ;
  }

}

static void convert_ACC_to_float() {
  if( ACC_TYPE == JD_D_int ) ACC_FLOAT = ACC_INT ;
  else if( ACC_TYPE == JD_D_float ) ;
  else if( ACC_TYPE == JD_D_bit ) {
#ifdef JEDA_RUNTIME_ERROR_CHECK
    if( JD_check_bit_set(ACC_B, ACC_SIZE) ) 
      ERROR_ILLEGAL_DATA_FOR_FLOAT_CONVERSION ;
#endif
    ACC_FLOAT = JD_bit_2_real(ACC_A, ACC_SIZE) ;
    ACC_TYPE = JD_D_float ;
  }

}

void JD_exec_m_lt_m() {
  /**MACRO EXEC_M_LT_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT < ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_LT_M**/  
}

void JD_exec_m_gt_m() {
  /**MACRO EXEC_M_GT_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT > ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_GT_M**/  
}

void JD_exec_m_eqeq_m() {
  /**MACRO EXEC_M_EQEQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_EQEQ_M**/  
}

void JD_exec_m_le_m() {
  /**MACRO EXEC_M_LE_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT <= ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_LE_M**/  
}

void JD_exec_m_ge_m() {
  /**MACRO EXEC_M_GE_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT >= ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_GE_M**/  
}

void JD_exec_m_ne_m() {
  /**MACRO EXEC_M_NE_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT != ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_NE_M**/  
}

void JD_exec_m_eqeqeq_m() {
  /**MACRO EXEC_M_EQEQEQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_EQEQEQ_M**/  
}

void JD_exec_m_neqeq_m() {
  /**MACRO EXEC_M_NEQEQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT != ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_NEQEQ_M**/  
}

void JD_exec_m_eqQeq_m() {
  /**MACRO EXEC_M_EQQEQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_EQQEQ_M**/  
}

void JD_exec_m_eqeqQ_m() {
  /**MACRO EXEC_M_EQEQQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_EQEQQ_M**/  
}

void JD_exec_m_Qeqeq_m() {
  /**MACRO EXEC_M_QEQEQ_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( TMP_FLOAT == ACC_FLOAT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }
  /**END MACRO EXEC_M_QEQEQ_M**/  
}

void JD_exec_m_minus_m() {
  /**MACRO EXEC_M_MINUS_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    ACC_FLOAT = TMP_FLOAT - ACC_FLOAT ;
    ACC_TYPE = JD_D_float ;
  }
  /**END MACRO EXEC_M_MINUS_M**/  
}

void JD_exec_m_plus_m() {
  /**MACRO EXEC_M_PLUS_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    ACC_FLOAT = TMP_FLOAT + ACC_FLOAT ;
    ACC_TYPE = JD_D_float ;
  }
  /**END MACRO EXEC_M_PLUS_M**/  
}

void JD_exec_m_times_m() {
  /**MACRO EXEC_M_TIMES_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    ACC_FLOAT = TMP_FLOAT * ACC_FLOAT ;
    ACC_TYPE = JD_D_float ;
  }
  /**END MACRO EXEC_M_TIMES_M**/  
}

void JD_exec_m_div_m() {
  /**MACRO EXEC_M_DIV_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( ACC_FLOAT == 0.0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_FLOAT = TMP_FLOAT / ACC_FLOAT ;
  }
  /**END MACRO EXEC_M_DIV_M**/  
}

void JD_exec_m_mod_m() {
  /**MACRO EXEC_M_MOD_M**/
  {
    JD_data *dt ;
    dt = POP_STACK ;
    convert_to_float(dt) ;
    convert_ACC_to_float() ;
    if( ACC_FLOAT == 0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_FLOAT = fmod(TMP_FLOAT, ACC_FLOAT) ;
  }
  /**END MACRO EXEC_M_MOD_M**/  
}
