/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.

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
/* binary int ALU ops              */
/***********************************/
void JD_exec_i_lt_i() {
  /**MACRO EXEC_INT_LT_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value < ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_LT_INT**/  
}

void JD_exec_i_gt_i() {
  /**MACRO EXEC_INT_GT_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value > ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_GT_INT**/  
}

void JD_exec_i_eqeq_i() {
  /**MACRO EXEC_INT_EQEQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value == ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_EQEQ_INT**/  
}

void JD_exec_i_le_i() {
  /**MACRO EXEC_INT_LE_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value <= ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_LE_INT**/  
}

void JD_exec_i_ge_i() {
  /**MACRO EXEC_INT_GE_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value >= ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_GE_INT**/  
}

void JD_exec_i_ne_i() {
  /**MACRO EXEC_INT_NE_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value != ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_NE_INT**/  
}

void JD_exec_i_eqeqeq_i() {
  /**MACRO EXEC_INT_EQEQEQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value == ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_EQEQEQ_INT**/  
}

void JD_exec_i_neqeq_i() {
  /**MACRO EXEC_INT_NEQEQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value != ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_NEQEQ_INT**/  
}

void JD_exec_i_eqQeq_i() {
  /**MACRO EXEC_INT_EQQEQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value == ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_BIT_EQQEQ_BIT**/  
}

void JD_exec_i_eqeqQ_i() {
  /**MACRO EXEC_INT_EQEQQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value == ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_EQEQQ_INT**/  
}

void JD_exec_i_Qeqeq_i() {
  /**MACRO EXEC_INT_QEQEQ_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value == ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_QEQEQ_INT**/  
}

void JD_exec_i_Qne_i() {
  /**MACRO EXEC_INT_QNE_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( int_value != ACC_INT ) ACC_INT = 1 ;
    else ACC_INT = 0 ;
  }
  /**END MACRO EXEC_INT_QNE_INT**/  
}

void JD_exec_i_minus_i() {
  /**MACRO EXEC_INT_MINUS_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value - ACC_INT ;
  }
  /**END MACRO EXEC_BIT_MINUS_BIT**/  
}

void JD_exec_i_plus_i() {
  /**MACRO EXEC_INT_PLUS_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value + ACC_INT ;
  }
  /**END MACRO EXEC_INT_PLUS_INT**/  
}

void JD_exec_i_times_i() {
  /**MACRO EXEC_INT_TIMES_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value * ACC_INT ;
  }
  /**END MACRO EXEC_INT_TIMES_INT**/  
}

void JD_exec_i_div_i() {
  /**MACRO EXEC_INT_DIV_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( ACC_INT == 0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_INT = int_value / ACC_INT ;
  }
  /**END MACRO EXEC_INT_DIV_INT**/  
}

void JD_exec_i_mod_i() {
  /**MACRO EXEC_INT_MOD_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    if( ACC_INT == 0 ) { ERROR_ZERO_DIVIDE ; }
    else ACC_INT = int_value % ACC_INT ;
  }
  /**END MACRO EXEC_INT_MOD_INT**/  
}

void JD_exec_i_and_i() {
  /**MACRO EXEC_INT_AND_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value & ACC_INT ;
  }
  /**END MACRO EXEC_INT_AND_INT**/  
}

void JD_exec_i_or_i() {
  /**MACRO EXEC_INT_OR_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value | ACC_INT ;
  }
  /**END MACRO EXEC_INT_OR_INT**/  
}

void JD_exec_i_eor_i() {
  /**MACRO EXEC_INT_EOR_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = int_value ^ ACC_INT ;
  }
  /**END MACRO EXEC_INT_EOR_INT**/  
}

void JD_exec_i_nand_i() {
  /**MACRO EXEC_INT_NAND_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = ~(int_value & ACC_INT) ;
  }
  /**END MACRO EXEC_INT_NAND_INT**/  
}

void JD_exec_i_NOR_i() {
  /**MACRO EXEC_INT_NOR_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = ~(int_value | ACC_INT) ;
  }
  /**END MACRO EXEC_INT_NOR_INT**/  
}

void JD_exec_i_neor_i() {
  /**MACRO EXEC_INT_NEOR_INT**/
  {
    int int_value ;
    int_value = POP_STACK_INT ;
    ACC_INT = ~(int_value ^ ACC_INT) ;
  }
  /**END MACRO EXEC_INT_NEOR_INT**/  
}


/***********************************/
/* unary bit ALU ops               */
/***********************************/

void JD_exec_u_minus_i() {
  /**MACRO EXEC_U_MINUS_INT**/
  {
    ACC_INT = -ACC_INT ;
  }
  /**END MACRO EXEC_U_MINUS_INT**/  
}

void JD_exec_u_tilde_i() {
  /**MACRO EXEC_U_TILDE_INT**/
  {
    ACC_INT = ~ACC_INT ;
  }
  /**END MACRO EXEC_U_TILDE_INT**/  
}

void JD_exec_u_not_i() {
  /**MACRO EXEC_U_NOT_INT**/
  {
    ACC_INT = ACC_INT ? 0 : 1 ;
  }
  /**END MACRO EXEC_U_NOT_INT**/  
}

void JD_exec_u_and_i() {
  /**MACRO EXEC_U_AND_INT**/
  {
    ACC_A[0] = (ACC_INT==0xffffffff) ? 0 : 1 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_AND_INT**/  
}

void JD_exec_u_or_i() {
  /**MACRO EXEC_U_OR_INT**/
  {
    ACC_A[0] = ACC_INT ? 1 : 0 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_OR_INT**/  
}

void JD_exec_u_eor_i() {
  /**MACRO EXEC_U_EOR_INT**/
  {
    int i;
    unsigned int a ;
    ACC_A[0] = 0 ;
    for( i = 0 ; i < 32 ; i++ )
      ACC_A[0] ^= ((ACC_INT>>i)&1) ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_EOR_INT**/  
}

void JD_exec_u_nand_i() {
  /**MACRO EXEC_U_NAND_INT**/
  {
    ACC_A[0] = (ACC_INT==0xffffffff) ? 1 : 0 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_NAND_INT**/  
}

void JD_exec_u_nor_i() {
  /**MACRO EXEC_U_NOR_INT**/
  {
    ACC_A[0] = ACC_INT ? 0 : 1 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_NOR_INT**/  
}

void JD_exec_u_neor_i() {
  /**MACRO EXEC_U_NEOR_INT**/
  {
    int i;
    ACC_A[0] = 0 ;
    for( i = 0 ; i < 32 ; i++ )
      ACC_A[0] ^= ((ACC_INT>>i)&1) ;
    ACC_A[0] = (~ACC_A[0])&1 ;
    ACC_B[0] = 0 ;
    ACC_SIZE = 1 ;
    ACC_TYPE = JD_D_sbit ;
  }
  /**END MACRO EXEC_U_NEOR_INT**/  
}


