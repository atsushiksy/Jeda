/*
 * bit and int ALU operations
 */
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
#include "compiler_macro.h"

/***********************************/
/* binary bit ALU ops              */
/***********************************/
void JD_exec_b_lt_b() {
  /**MACRO EXEC_BIT_LT_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_LT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_LT_BIT**/  
}

void JD_exec_b_gt_b() {
  /**MACRO EXEC_BIT_GT_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_GT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_GT_BIT**/  
}

void JD_exec_b_eqeq_b() {
  /**MACRO EXEC_BIT_EQEQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQ_BIT**/  
}

void JD_exec_b_le_b() {
  /**MACRO EXEC_BIT_LE_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_LE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_LE_BIT**/  
}

void JD_exec_b_ge_b() {
  /**MACRO EXEC_BIT_GE_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_GE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_GE_BIT**/  
}

void JD_exec_b_ne_b() {
  /**MACRO EXEC_BIT_NE_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_NE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_NE_BIT**/  
}

void JD_exec_b_eqeqeq_b() {
  /**MACRO EXEC_BIT_EQEQEQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQEQ_BIT**/  
}

void JD_exec_b_neqeq_b() {
  /**MACRO EXEC_BIT_NEQEQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_NEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_NEQEQ_BIT**/  
}

void JD_exec_b_eqQeq_b() {
  /**MACRO EXEC_BIT_EQQEQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQQEQ_BIT**/  
}

void JD_exec_b_eqeqQ_b() {
  /**MACRO EXEC_BIT_EQEQQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQQ_BIT**/  
}

void JD_exec_b_Qeqeq_b() {
  /**MACRO EXEC_BIT_QEQEQ_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_QEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_QEQEQ_BIT**/  
}

void JD_exec_b_Qne_b() {
  /**MACRO EXEC_BIT_QNE_BIT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_QNE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_QNE_BIT**/  
}

void JD_exec_b_minus_b() {
  /**MACRO EXEC_BIT_MINUS_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    S0 = s ;
    CALL_BIT_MINUS_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_MINUS_BIT**/  
}

void JD_exec_b_plus_b() {
  /**MACRO EXEC_BIT_PLUS_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0+1:S1+1 ;
    ADJUST_ACCB(s) ;
    S0 = s ;
    CALL_BIT_PLUS_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_PLUS_BIT**/  
}

void JD_exec_b_times_b() {
  /**MACRO EXEC_BIT_TIMES_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = S0 + S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
    CALL_BIT_TIMES_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_BIT_TIMES_BIT**/  
}

void JD_exec_b_div_b() {
  /**MACRO EXEC_BIT_DIV_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_ODIV_BIT ;
#else
    CALL_BIT_DIV_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_BIT_DIV_BIT**/  
}

void JD_exec_b_mod_b() {
  /**MACRO EXEC_BIT_MOD_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_OMOD_BIT ;
#else
    CALL_BIT_MOD_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_BIT_MOD_BIT**/  
}

void JD_exec_b_and_b() {
  /**MACRO EXEC_BIT_AND_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_AND_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_AND_BIT**/  
}

void JD_exec_b_or_b() {
  /**MACRO EXEC_BIT_OR_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_OR_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_OR_BIT**/  
}

void JD_exec_b_eor_b() {
  /**MACRO EXEC_BIT_EOR_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_EOR_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_EOR_BIT**/  
}

void JD_exec_b_nand_b() {
  /**MACRO EXEC_BIT_NAND_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NAND_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_NAND_BIT**/  
}

void JD_exec_b_NOR_b() {
  /**MACRO EXEC_BIT_NOR_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NOR_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_NOR_BIT**/  
}

void JD_exec_b_neor_b() {
  /**MACRO EXEC_BIT_NEOR_BIT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NEOR_BIT ;
    SWAP_ACC ;
  }
  /**END MACRO EXEC_BIT_NEOR_BIT**/  
}


/***********************************/
/* unary bit ALU ops               */
/***********************************/

void JD_exec_u_minus_b() {
  /**MACRO EXEC_U_MINUS_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_MINUS_BIT ;
  }
  /**END MACRO EXEC_U_MINUS_BIT**/  
}

void JD_exec_u_tilde_b() {
  /**MACRO EXEC_U_TILDE_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_TILDE_BIT ;
  }
  /**END MACRO EXEC_U_TILDE_BIT**/  
}

void JD_exec_u_not_b() {
  /**MACRO EXEC_U_NOT_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_NOT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_U_NOT_BIT**/  
}

void JD_exec_u_and_b() {
  /**MACRO EXEC_U_AND_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_AND_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_AND_BIT**/  
}

void JD_exec_u_or_b() {
  /**MACRO EXEC_U_OR_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_OR_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_OR_BIT**/  
}

void JD_exec_u_eor_b() {
  /**MACRO EXEC_U_EOR_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_EOR_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_EOR_BIT**/  
}

void JD_exec_u_nand_b() {
  /**MACRO EXEC_U_NAND_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_NAND_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_NAND_BIT**/  
}

void JD_exec_u_nor_b() {
  /**MACRO EXEC_U_NOR_BIT**/
  {
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_NOR_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_NOR_BIT**/  
}

void JD_exec_u_neor_b() {
  /**MACRO EXEC_U_NEOR_BIT**/
  {
    JD_data *dt ;
    int s ;
    A0 = ACC_A ;
    B0 = ACC_B ;
    S0 = ACC_SIZE ;
    CALL_U_NEOR_BIT ;
    ACC_A[0] = RET & 1 ;
    ACC_B[0] = (RET>>1) & 1 ;
    ACC_SIZE = 1 ;
  }
  /**END MACRO EXEC_U_NEOR_BIT**/  
}


/***********************************/
/* binary int op bit ALU ops       */
/***********************************/
void JD_exec_i_lt_b() {
  /**MACRO EXEC_INT_LT_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_LT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_LT_BIT**/  
}

void JD_exec_i_gt_b() {
  /**MACRO EXEC_INT_GT_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_GT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_GT_BIT**/  
}

void JD_exec_i_eqeq_b() {
  /**MACRO EXEC_INT_EQEQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_EQEQ_BIT**/  
}

void JD_exec_i_le_b() {
  /**MACRO EXEC_INT_LE_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_LE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_LE_BIT**/  
}

void JD_exec_i_ge_b() {
  /**MACRO EXEC_INT_GE_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_GE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_GE_BIT**/  
}

void JD_exec_i_ne_b() {
  /**MACRO EXEC_INT_NE_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_NE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_NE_BIT**/  
}

void JD_exec_i_eqeqeq_b() {
  /**MACRO EXEC_INT_EQEQEQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_EQEQEQ_BIT**/  
}

void JD_exec_i_neqeq_b() {
  /**MACRO EXEC_INT_NEQEQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_NEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_NEQEQ_BIT**/  
}

void JD_exec_i_eqQeq_b() {
  /**MACRO EXEC_INT_EQQEQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_EQQEQ_BIT**/  
}

void JD_exec_i_eqeqQ_b() {
  /**MACRO EXEC_INT_EQEQQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_EQEQQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_EQEQQ_BIT**/  
}

void JD_exec_i_Qeqeq_b() {
  /**MACRO EXEC_INT_QEQEQ_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_QEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_QEQEQ_BIT**/  
}

void JD_exec_i_Qne_b() {
  /**MACRO EXEC_INT_QNE_BIT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    CALL_BIT_QNE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_INT_QNE_BIT**/  
}

void JD_exec_i_minus_b() {
  /**MACRO EXEC_INT_MINUS_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_MINUS_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_MINUS_BIT**/  
}

void JD_exec_i_plus_b() {
  /**MACRO EXEC_INT_PLUS_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0+1:S1+1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_PLUS_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_PLUS_BIT**/  
}

void JD_exec_i_times_b() {
  /**MACRO EXEC_INT_TIMES_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = S0 + S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
    CALL_BIT_TIMES_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_TIMES_BIT**/  
}

void JD_exec_i_div_b() {
  /**MACRO EXEC_INT_DIV_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_ODIV_BIT ;
#else
    CALL_BIT_DIV_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_DIV_BIT**/  
}

void JD_exec_i_mod_b() {
  /**MACRO EXEC_INT_MOD_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    A0 = &int_value ;
    B0 = &INT_ZERO ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_OMOD_BIT ;
#else
    CALL_BIT_MOD_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_MOD_BIT**/  
}

void JD_exec_i_and_b() {
  /**MACRO EXEC_INT_AND_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_AND_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_AND_BIT**/  
}

void JD_exec_i_or_b() {
  /**MACRO EXEC_INT_OR_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_OR_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_OR_BIT**/  
}

void JD_exec_i_eor_b() {
  /**MACRO EXEC_INT_EOR_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_EOR_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_EOR_BIT**/  
}

void JD_exec_i_nand_b() {
  /**MACRO EXEC_INT_NAND_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_NAND_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_NAND_BIT**/  
}

void JD_exec_i_NOR_b() {
  /**MACRO EXEC_INT_NOR_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_NOR_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_NOR_BIT**/  
}

void JD_exec_i_neor_b() {
  /**MACRO EXEC_INT_NEOR_BIT**/
  {
    int s, int_value ;
    int_value = POP_STACK_INT ;
    S0 = 32 ;
    A1 = ACC_A ;
    B1 = ACC_B ;
    S1 = ACC_SIZE ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    SET_INT_2_TMP(int_value,s) ;
    A0 = TMP_A ;
    B0 = TMP_B ;
    CALL_BIT_NEOR_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
  }
  /**END MACRO EXEC_INT_NEOR_BIT**/  
}


/***********************************/
/* binary bit op int ALU ops       */
/***********************************/
void JD_exec_b_lt_i() {
  /**MACRO EXEC_BIT_LT_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_LT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_LT_INT**/  
}

void JD_exec_b_gt_i() {
  /**MACRO EXEC_BIT_GT_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_GT_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_GT_INT**/  
}

void JD_exec_b_eqeq_i() {
  /**MACRO EXEC_BIT_EQEQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQ_INT**/  
}

void JD_exec_b_le_i() {
  /**MACRO EXEC_BIT_LE_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_LE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_LE_INT**/  
}

void JD_exec_b_ge_i() {
  /**MACRO EXEC_BIT_GE_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_GE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_GE_INT**/  
}

void JD_exec_b_ne_i() {
  /**MACRO EXEC_BIT_NE_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_NE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_NE_INT**/  
}

void JD_exec_b_eqeqeq_i() {
  /**MACRO EXEC_BIT_EQEQEQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQEQ_INT**/  
}

void JD_exec_b_neqeq_i() {
  /**MACRO EXEC_BIT_NEQEQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_NEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_NEQEQ_INT**/  
}

void JD_exec_b_eqQeq_i() {
  /**MACRO EXEC_BIT_EQQEQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQQEQ_INT**/  
}

void JD_exec_b_eqeqQ_i() {
  /**MACRO EXEC_BIT_EQEQQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_EQEQQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_EQEQQ_INT**/  
}

void JD_exec_b_Qeqeq_i() {
  /**MACRO EXEC_BIT_QEQEQ_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_QEQEQ_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_QEQEQ_INT**/  
}

void JD_exec_b_Qne_i() {
  /**MACRO EXEC_BIT_QNE_INT**/
  {
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    CALL_BIT_QNE_BIT ;
    SET_ACC_BOOL(RET) ;
  }
  /**END MACRO EXEC_BIT_QNE_INT**/  
}

void JD_exec_b_minus_i() {
  /**MACRO EXEC_BIT_MINUS_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_MINUS_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_MINUS_INT**/  
}

void JD_exec_b_plus_i() {
  /**MACRO EXEC_BIT_PLUS_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0+1:S1+1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_PLUS_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_PLUS_INT**/  
}

void JD_exec_b_times_i() {
  /**MACRO EXEC_BIT_TIMES_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = S0 + S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
    CALL_BIT_TIMES_BIT ;
    SET_TMP_BIT_2_ACC(s) ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_TIMES_INT**/  
}

void JD_exec_b_div_i() {
  /**MACRO EXEC_BIT_DIV_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_ODIV_BIT ;
#else
    CALL_BIT_DIV_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_DIV_INT**/  
}

void JD_exec_b_mod_i() {
  /**MACRO EXEC_BIT_MOD_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_TEMP(s) ;
    AR = TMP_A ;
    BR = TMP_B ;
    SR = s ;
#ifdef OPTIMIZED_DIVIDE
    CALL_BIT_OMOD_BIT ;
#else
    CALL_BIT_MOD_BIT ;
#endif
    if( ERR ) { ERROR_ZERO_DIVIDE ; }
    SET_TMP_BIT_2_ACC(s) ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_MOD_INT**/  
}

void JD_exec_b_and_i() {
  /**MACRO EXEC_BIT_AND_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_AND_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_AND_INT**/  
}

void JD_exec_b_or_i() {
  /**MACRO EXEC_BIT_OR_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_OR_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_OR_INT**/  
}

void JD_exec_b_eor_i() {
  /**MACRO EXEC_BIT_EOR_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_EOR_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_EOR_INT**/  
}

void JD_exec_b_nand_i() {
  /**MACRO EXEC_BIT_NAND_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NAND_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_NAND_INT**/  
}

void JD_exec_b_NOR_i() {
  /**MACRO EXEC_BIT_NOR_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NOR_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_NOR_INT**/  
}

void JD_exec_b_neor_i() {
  /**MACRO EXEC_BIT_NEOR_INT**/
  {
    int s ;
    POP_BIT_TO_ACCB ;
    A0 = ACCB_A ;
    B0 = ACCB_B ;
    S0 = ACCB_SIZE ;
    A1 = &ACC_INT ;
    B1 = &INT_ZERO ;
    S1 = 32 ;
    s = (S0>S1)?S0:S1 ;
    ADJUST_ACCB(s) ;
    CALL_BIT_NEOR_BIT ;
    SWAP_ACC ;
    ACC_TYPE = JD_D_bit ;
  }
  /**END MACRO EXEC_BIT_NEOR_INT**/  
}


