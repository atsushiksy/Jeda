/***************************************************************************
 *
 *  constant.c: Constant Calculation Functions
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#include "../include/compiler_macro.h"
#include <stdio.h>
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/error_msg.h"
#include "../include/bit_operations.h"

int eval_to_int ( expression_node *exp, int *ret ) 
{
  int i ;
  
  switch( exp->type ) {
    case JD_int_num:
      *ret = exp->is.int_value ;
      return( 1 ) ;
    case JD_bit_num:
      for( i = 1 ; i <= LASTWORD(exp->is.bit_const.size) ; i++ ) {
        if( exp->is.bit_const.b[i] || exp->is.bit_const.a[i] ) return (0) ;
      }
      if( exp->is.bit_const.b[0] ) return (0) ;
      *ret = exp->is.bit_const.a[0] ;
      return(1) ;
    default:
      return(0) ;
  }
}

char *eval_to_bitstr( expression_node *exp ) 
{
  int i ;
  char *ret ;
  int a, b ;
  switch( exp->type ) {
    case JD_int_num:
      ret = (char *)calloc( 33, sizeof(char) ) ;
      for( i = 0 ; i < 32 ; i++ ) {
        if( (exp->is.int_value >> (31-i) ) & 1 ) ret[i] = '1' ;
        else ret[i] = '0' ;
      }
      return( ret ) ;
    case JD_bit_num:
      ret = (char *)calloc( exp->is.bit_const.size+1, sizeof(char) ) ;
      for( i = 0 ; i < exp->is.bit_const.size ; i++ ) {
        a = ( ( exp->is.bit_const.a[(i/32)] >> (i%32) ) & 1 ) ;
        b = ( ( exp->is.bit_const.b[(i/32)] >> (i%32) ) & 1 ) ;
      }
      if( b ) {
        if( a ) ret[i] = 'x' ;
        else ret[i] = 'z' ;
      }
      else {
        if( a ) ret[i] = '1' ;
        else ret[i] = '0' ;
      }
      return( ret ) ;
    default:
      return( NULL ) ;
  }
}

 /*
   binary arithmetic operation conversion rules:
   
     float op float -> float
     float op int   -> float
     float op bit   -> float
     int   op float -> float
     int   op int   -> int
     int   op bit   -> bit
     bit   op float -> float
     bit   op int   -> bit
     bit   op bit   -> bit
*/

/********************************************************************
 *  Bit size of the binary alu result 
 *  According to the rules of expression bit length on IEEE Std1364
 ********************************************************************/ 
static int get_bit_size( JD_expression_type ops, int size1, int size2 ) 
{
  int size ;
  
  if( (ops == JD_exp_rshift_exp) || 
      (ops == JD_exp_urshift_exp) ||
      (ops == JD_exp_lshift_exp)  ||
      (ops == JD_exp_times_exp)  ||
      (ops == JD_exp_div_exp)  ||
      (ops == JD_exp_mod_exp)
    )
  {
    /*  Multiply and divide allocate different area for the result */
    size = size1 ;
  }
  else {
    if( size1 > size2 ) size = size1 ;
    else size = size2 ;
  }
  
  return size ;
  
}

static void extend_bit(
  JD_expression_type ops,
  expression_node  *op,
  int size1
)
{
  int size = get_bit_size( ops, op->is.bit_const.size, size1 ) ;
  

  /* extending op size if needed */
  if( size > op->is.bit_const.size ) {
    int i ;
    op->is.bit_const.a = 
      (int *)realloc(
        op->is.bit_const.a, 
        NBYTE(size)
      );
    op->is.bit_const.b = 
      (int *)realloc(
        op->is.bit_const.b, 
        NBYTE(size)
      );
    for( i = LASTWORD(op->is.bit_const.size) ; i < size ; i++ ) {
      op->is.bit_const.a[i] = 0 ;
      op->is.bit_const.b[i] = 0 ;
    }
    op->is.bit_const.size = size ;
  }
}   
          
static void real_aop_real (
  JD_expression_type ops,
  double *op1,
  double op2
)
{
  switch( ops ) {
    case JD_exp_minus_exp:     /* exp '-' exp   */
      *op1 = *op1 - op2 ;
      break ;
    case JD_exp_plus_exp:      /* exp '+' exp   */
      *op1 = *op1 + op2 ;
      break ;
    case JD_exp_times_exp:     /* exp '*' exp   */
      *op1 = *op1 * op2 ;
      break ;
    case JD_exp_div_exp:       /* exp '/' exp   */
      *op1 = *op1 / op2 ;
      break ;
    case JD_exp_mod_exp:       /* exp '%' exp   */
      ERROR_ILLEGAL_OPERAND_TYPE_FOR_MODULO_OPERATION ;
      break ;
    case JD_exp_and_exp:       /* exp '&' exp   */
    case JD_exp_or_exp:        /* exp '|' exp   */
    case JD_exp_eor_exp:       /* exp '^' exp   */
    case JD_exp_nand_exp:      /* exp '&~' exp or exp '~&' exp  */
    case JD_exp_nor_exp:       /* exp '|~' exp or exp '~|' exp  */
    case JD_exp_neor_exp:      /* exp '^~' exp or exp '~^' exp  */
      ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION ;
      break ;
  }

}

static void int_aop_int (
  JD_expression_type ops,
  int *op1,
  int op2
)
{
  switch( ops ) {
    case JD_exp_minus_exp:     /* exp '-' exp   */
      *op1 = *op1 - op2 ;
      break ;
    case JD_exp_plus_exp:      /* exp '+' exp   */
      *op1 = *op1 + op2 ;
      break ;
    case JD_exp_times_exp:     /* exp '*' exp   */
      *op1 = *op1 * op2 ;
      break ;
    case JD_exp_div_exp:       /* exp '/' exp   */
      *op1 = *op1 / op2 ;
      break ;
    case JD_exp_mod_exp:       /* exp '%' exp   */
      *op1 = *op1 % op2 ;
      break ;
    case JD_exp_and_exp:       /* exp '&' exp   */
      *op1 = *op1 & op2 ;
      break ;
    case JD_exp_or_exp:        /* exp '|' exp   */
      *op1 = *op1 | op2 ;
      break ;
    case JD_exp_eor_exp:       /* exp '^' exp   */
      *op1 = *op1 ^ op2 ;
      break ;
    case JD_exp_nand_exp:      /* exp '&~' exp or exp '~&' exp  */
      *op1 = ~(*op1 & op2) ;
      break ;
    case JD_exp_nor_exp:       /* exp '|~' exp or exp '~|' exp  */
      *op1 = ~(*op1 | op2) ;
      break ;
    case JD_exp_neor_exp:      /* exp '^~' exp or exp '~^' exp  */
      *op1 = ~(*op1 ^ op2) ;
      break ;
  }

}

static void bit_op_bit (
  JD_expression_type ops,
  expression_node *op1, 
  int *a2, int *b2, int size2
)
{
  int *a1, *b1, size1 ;
  int *ar, *br, sizer ;
  int err = 0 ;
  
  a1 = op1->is.bit_const.a ;
  b1 = op1->is.bit_const.b ;
  size1 = op1->is.bit_const.size ;

  switch( ops ) {
    case JD_exp_minus_exp:     /* exp '-' exp   */
      JD_bit_minus_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_plus_exp:      /* exp '+' exp   */
      JD_bit_plus_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_times_exp:     /* exp '*' exp   */
      sizer = size1 + size2 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_times_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2) ;
      free(a1) ;
      free(b1) ;
      op1->is.bit_const.a = ar ;
      op1->is.bit_const.b = br ;
      op1->is.bit_const.size = sizer ;
      break ;
    case JD_exp_div_exp:       /* exp '/' exp   */
      sizer = size1 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_div_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2, &err) ;
      if( err )
        ERROR_ZERO_DIVIDE_ON_CONSTANT_CALCULATION ;
      free(a1) ;
      free(b1) ;
      op1->is.bit_const.a = ar ;
      op1->is.bit_const.b = br ;
      op1->is.bit_const.size = sizer ;
      break ;
    case JD_exp_mod_exp:       /* exp '%' exp   */
      sizer = size1 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_mod_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2, &err) ;
      if( err )
        ERROR_ZERO_DIVIDE_ON_CONSTANT_CALCULATION ;
      free(a1) ;
      free(b1) ;
      op1->is.bit_const.a = ar ;
      op1->is.bit_const.b = br ;
      op1->is.bit_const.size = sizer ;
      break ;
    case JD_exp_and_exp:       /* exp '&' exp   */
      JD_bit_and_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_or_exp:        /* exp '|' exp   */
      JD_bit_or_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_eor_exp:       /* exp '^' exp   */
      JD_bit_eor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_nand_exp:      /* exp '&~' exp or exp '~&' exp  */
      JD_bit_nand_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_nor_exp:       /* exp '|~' exp or exp '~|' exp  */
      JD_bit_nor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_exp_neor_exp:      /* exp '^~' exp or exp '~^' exp  */
      JD_bit_neor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
  }

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
  return ret ;
}

/************************************************************
 *  const aops const (Arithmetic Operation only )
 ************************************************************/
static int check_bit_set( int *b, int size )
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

static void free_bit( expression_node *op )
{
  free( op->is.bit_const.a ) ;
  free( op->is.bit_const.b ) ;
  free( op ) ;
}

static void free_string( expression_node *op )
{
  free( op->is.string ) ;
  free( op ) ;
}

expression_node  *const_aop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
)
{
  
  double real_value ;
  
  if( op1->type == JD_X_num || op2->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return NULL ;
  }
  switch( op1->type ) {
    case JD_real_num:
      switch( op2->type ) {
        case JD_real_num:
          real_aop_real( ops, &op1->is.real_value, op2->is.real_value) ;
          free(op2) ;
          break ;
        case JD_int_num:
          op2->is.real_value = op2->is.int_value ;
          real_aop_real( ops, &op1->is.real_value, op2->is.real_value ) ;
          break ;
        case JD_bit_num:
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) {
             ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          real_value = 
            JD_bit_2_real( 
              op2->is.bit_const.a, op2->is.bit_const.size 
            ) ;
          real_aop_real( ops, &op1->is.real_value, real_value ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
    case JD_int_num:
      switch( op2->type ) {
        case JD_real_num:
          op1->is.real_value = op1->is.int_value ;
          op1->type = JD_real_num ;
          real_aop_real( ops, &op1->is.real_value, op2->is.real_value ) ;
          break ;
        case JD_int_num:
          int_aop_int( ops, &op1->is.int_value, op2->is.int_value ) ;
          free(op2) ;
          break ;
        case JD_bit_num:
          {  
            /* converting op1 to be bit_num */
            int tmp = op1->is.int_value ;
            op1->type = JD_bit_num ;
            op1->is.bit_const.size = 32 ;
            op1->is.bit_const.a = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.b = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.a[0] = tmp ;
            extend_bit( ops, op1, op2->is.bit_const.size ) ;
          }
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          bit_op_bit( 
            ops,
            op1,
            op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
          ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
    case JD_bit_num:
      switch( op2->type ) {
        case JD_real_num:
          if( check_bit_set( op1->is.bit_const.b, op1->is.bit_const.size ) ) {
             ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          op1->is.real_value = 
            JD_bit_2_real( 
              op1->is.bit_const.a, op1->is.bit_const.size 
            ) ;
          op1->type = JD_real_num ;
          real_aop_real( ops, &op1->is.real_value, op2->is.real_value ) ;
          free(op2) ;
          break ;
        case JD_int_num:       
          {
            int *a, b, size ;
            if( check_bit_set( op1->is.bit_const.b, op1->is.bit_const.size ) ) 
            {
               ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            }
            extend_bit( ops, op1, 32 ) ;
            a = &op2->is.int_value ;
            b = 0 ;
            size = 32 ;
            bit_op_bit( 
              ops,
              op1,
              a, &b, size
            ) ;
          }
          free(op2) ;
          break ;
        case JD_bit_num:
          extend_bit( ops, op1, op2->is.bit_const.size ) ;
          bit_op_bit(
            ops, 
            op1,
            op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
          ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
  }
  
  op1->data_type = JD_exp_num_type ;
  return op1 ;
    
}

/* constant logical binary operation */
expression_node  *const_lop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
)
{
  
  int b1, b2 ;
  int i;
  
  switch( op1->type ) {
    case JD_X_num:
      b1 = -1 ;
      break ;
    case JD_real_num:
      if( op1->is.real_value == 0.0 ) b1 = 0 ;
      else b1 = 1 ;
      break ;
    case JD_int_num:
      if( op1->is.int_value == 0 ) b1 = 0 ;
      else b1 = 1 ;
      break ;
    case JD_bit_num:
      b1 = 0 ;
      for( i = 0 ; i < NWORD(op1->is.bit_const.size) ; i++ ) {
        if( op1->is.bit_const.b[i] ) break ;
      }
      if( i == NWORD(op1->is.bit_const.size) ) { /* bit b is all clear */
        for( i = 0 ; i < NWORD(op1->is.bit_const.size) ; i++ ) {
          if( op1->is.bit_const.a[i] ) {
            b1 = 1 ;
            break ;
          }
        }
      }
      free(op1->is.bit_const.a) ;
      free(op1->is.bit_const.b) ;
      break ;
  }
  switch( op2->type ) {
    case JD_X_num:
      b2 = -1 ;
      break ;
    case JD_real_num:
      if( op2->is.real_value == 0.0 ) b2 = 0 ;
      else b2 = 1 ;
      free(op2) ;
      break ;
    case JD_int_num:
      if( op2->is.int_value == 0 ) b2 = 0 ;
      else b2 = 1 ;
      free(op2) ;
      break ;
    case JD_bit_num:
      b2 = 0 ;
      for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
        /* As in Verilog, x and z is handled as false */
        if( op2->is.bit_const.b[i] ) break ;
      }
      if( i == NWORD(op2->is.bit_const.size) ) {
        for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
          if( op2->is.bit_const.a[i] ) {
            b2 = 1 ;
            break ;
          }
        }
      }
      free_bit(op2) ;
      break ;
  }
  
  if( b1 >= 0 && b2 >= 0 ) {
    op1->type = JD_int_num ;
    switch( ops ) {
      case JD_exp_land_exp:
        op1->is.int_value = b1 & b2 ;
        break ;
      case JD_exp_lor_exp:
        op1->is.int_value = b1 | b2 ;
        break ;
    }
  }
  else {
    switch( ops ) {
      case JD_exp_land_exp:
        if( b1 == 0 || b2 == 0 ) {
          op1->is.int_value = 0 ;
          op1->type = JD_int_num ;
        }
        else {
          op1->type = JD_X_num ;
        }
        break ;
      case JD_exp_lor_exp:
        if( b1 == 1 || b2 == 1 ) {
          op1->is.int_value = 1 ;
          op1->type = JD_int_num ;
        }
        else {
          op1->type = JD_X_num ;
        }
        break ;
    }
  }
  op1->data_type = JD_exp_num_type ;
  return op1 ;
    
}


/* constant shift operation */
expression_node  *const_sop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
)
{
  
  int num_shift , i ;
  
  switch( op2->type ) {
    case JD_real_num:
      num_shift = op2->is.real_value ;
      if( op2->is.real_value > (num_shift + 1.0) ) {
        ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_TOO_LARGE_VALUE ;
      }
      free(op2) ;
      break ;
    case JD_int_num:
      num_shift = op2->is.int_value ;
      free(op2) ;
      break ;
    case JD_bit_num:
      num_shift = op2->is.bit_const.a[0] ;
      for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
        /* As in Verilog, x and z is handled as false */
        if( op2->is.bit_const.b[i] ) {
          ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          break ;
        }
      }
      if( i == NWORD(op2->is.bit_const.size) ) {
        for( i = 1 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
          if( op2->is.bit_const.a[i] ) {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_TOO_LARGE_VALUE ;
            break ;
          }
        }
      }
      free_bit(op2) ;
      break ;
  }
  
  switch( op1->type ) {
    case JD_real_num:
      ERROR_ILLEGAL_OPERAND_TYPE_FOR_SHIFT_OPERATION ;
      /*
      switch( ops ) {
        case JD_exp_rshift_exp:
        case JD_exp_urshift_exp:
          op1->is.real_value >>= num_shift ;
          break ;
        case JD_exp_lshift_exp:
          op1->is.real_value <<= num_shift ;
          break ;
      }
      */
      break ;
    case JD_int_num:
      switch( ops ) {
        case JD_exp_rshift_exp:
          op1->is.int_value >>= num_shift ;
          break ;
        case JD_exp_urshift_exp:
          op1->is.int_value =  (unsigned)op1->is.int_value >> num_shift ;
          break ;
        case JD_exp_lshift_exp:
          op1->is.int_value <<= num_shift ;
          break ;
      }
      break ;
    case JD_bit_num:
      switch( ops ) {
        case JD_exp_rshift_exp:
        case JD_exp_urshift_exp:
          BIT_RIGHT_SHIFT( 
            op1->is.bit_const.a, NWORD(op1->is.bit_const.size), num_shift
          ) ;
          BIT_RIGHT_SHIFT( 
            op1->is.bit_const.b, NWORD(op1->is.bit_const.size), num_shift
          ) ;
          break ;
        case JD_exp_lshift_exp:
          BIT_LEFT_SHIFT( 
            op1->is.bit_const.a, NWORD(op1->is.bit_const.size), num_shift
          ) ;
          BIT_LEFT_SHIFT( 
            op1->is.bit_const.b, NWORD(op1->is.bit_const.size), num_shift
          ) ;
          break ;
      }
      break ;
  }
  
  op1->data_type = JD_exp_num_type ;
  return op1 ;
    
}

/************************************************************
 *  const cops const (Compare Operation only )
 ************************************************************/
static void real_cop_real( 
  JD_expression_type ops, expression_node *op1, double op2
) 
{
  int ret ;
  switch( ops ) {
    case JD_exp_lt_exp:
      if( op1->is.real_value < op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_gt_exp:
      if( op1->is.real_value > op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeq_exp:
      if( op1->is.real_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_le_exp:
      if( op1->is.real_value <= op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_ge_exp:
      if( op1->is.real_value >= op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_ne_exp:
      if( op1->is.real_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeqeq_exp:
      if( op1->is.real_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_neqeq_exp:
      if( op1->is.real_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqQeq_exp:
      if( op1->is.real_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeqQ_exp:
      if( op1->is.real_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_Qeqeq_exp:
      if( op1->is.real_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_Qne_exp:
      if( op1->is.real_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
  }
  
  op1->type = JD_int_num ;
  op1->is.int_value = ret ;
  
}

static void int_cop_int( 
  JD_expression_type ops, expression_node *op1, int op2
) 
{
  int ret ;
  switch( ops ) {
    case JD_exp_lt_exp:
      if( op1->is.int_value < op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_gt_exp:
      if( op1->is.int_value > op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeq_exp:
      if( op1->is.int_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_le_exp:
      if( op1->is.int_value <= op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_ge_exp:
      if( op1->is.int_value >= op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_ne_exp:
      if( op1->is.int_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeqeq_exp:
      if( op1->is.int_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_neqeq_exp:
      if( op1->is.int_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqQeq_exp:
      if( op1->is.int_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_eqeqQ_exp:
      if( op1->is.int_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_Qeqeq_exp:
      if( op1->is.int_value == op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
    case JD_exp_Qne_exp:
      if( op1->is.int_value != op2 ) ret = 1 ;
      else ret = 0 ;
      break ;
  }
  
  op1->is.int_value = ret ;
  
}

static void bit_cop_bit( 
  JD_expression_type ops, expression_node *op1, 
  int *a2, int *b2, int s2
) 
{
  int ret ;
  int *a1, *b1, s1 ;
  
  a1 = op1->is.bit_const.a ;
  b1 = op1->is.bit_const.a ;
  s1 = op1->is.bit_const.size ;
  
  switch( ops ) {
    case JD_exp_lt_exp:
      ret = JD_bit_lt_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_gt_exp:
      ret = JD_bit_gt_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_eqeq_exp:
      ret = JD_bit_eqeq_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_le_exp:
      ret = JD_bit_le_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_ge_exp:
      ret = JD_bit_ge_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_ne_exp:
      ret = JD_bit_ne_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_eqeqeq_exp:
      ret = JD_bit_eqeqeq_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_neqeq_exp:
      ret = JD_bit_neqeq_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_eqQeq_exp:
      ret = JD_bit_eqQeq_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_eqeqQ_exp:
      ret = JD_bit_eqeqQ_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_Qeqeq_exp:
      ret = JD_bit_Qeqeq_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
    case JD_exp_Qne_exp:
      ret = JD_bit_Qne_bit( a1, b1, s1, a2, b2, s2 ) ;
      break ;
  }
  
  if( ret >= 0 ) {
    op1->is.int_value = ret ;
    op1->type = JD_int_num ;
  }
  else {
    op1->type = JD_X_num ;
  }
  free(a1) ;
  free(b1) ;
  
}

expression_node  *const_cop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
)
{
  
  double real_val ;
  int *ta, *tb ;
  
  if( op1->type == JD_X_num || op2->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return NULL ;
  }
  switch( op1->type ) {
    case JD_real_num:
      switch( op2->type ) {
        case JD_real_num:
          real_cop_real( ops, op1, op2->is.real_value) ;
          free(op2) ;
          break ;
        case JD_int_num:
          op2->is.real_value = op2->is.int_value ;
          real_cop_real( ops, op1, op2->is.real_value ) ;
          break ;
        case JD_bit_num:
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) {
             ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          real_val = 
            JD_bit_2_real( 
              op2->is.bit_const.a, op2->is.bit_const.size 
            ) ;
          real_cop_real( ops, op1, real_val ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
    case JD_int_num:
      switch( op2->type ) {
        case JD_real_num:
          op1->is.real_value = op1->is.int_value ;
          op1->type = JD_real_num ;
          real_cop_real( ops, op1, op2->is.real_value ) ;
          break ;
        case JD_int_num:
          int_cop_int( ops, op1, op2->is.int_value ) ;
          free(op2) ;
          break ;
        case JD_bit_num:
          {  
            /* converting op1 to be bit_num */
            int tmp = op1->is.int_value ;
            op1->type = JD_bit_num ;
            op1->is.bit_const.size = 32 ;
            op1->is.bit_const.a = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.b = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.a[0] = tmp ;
          }
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          bit_cop_bit( 
            ops,
            op1,
            op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
          ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
    case JD_bit_num:
      switch( op2->type ) {
        case JD_real_num:
          if( check_bit_set( op1->is.bit_const.b, op1->is.bit_const.size ) ) {
             ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
          }
          ta = op1->is.bit_const.a ;
          tb = op1->is.bit_const.b ;
          op1->is.real_value = 
            JD_bit_2_real( 
              op1->is.bit_const.a, op1->is.bit_const.size 
            ) ;
          op1->type = JD_real_num ;
          real_cop_real( ops, op1, op2->is.real_value ) ;
          free(op2) ;
          free(ta) ;
          free(tb) ;
          break ;
        case JD_int_num:       
          {
            int *a, b, size ;
            if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) 
            {
               ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            }
            a = &op1->is.int_value ;
            b = 0 ;
            size = 32 ;
            bit_cop_bit( 
              ops,
              op1,
              a, &b, size
            ) ;
          }
          free(op2) ;
          break ;
        case JD_bit_num:
          bit_cop_bit(
            ops, 
            op1,
            op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
          ) ;
          free_bit(op2) ;
          break ;
      }
      break ;
  }
  
  op1->data_type = JD_exp_num_type ;
  return op1 ;
    
}


/* unary operations */
expression_node  *aop_real( 
  JD_expression_type ops, expression_node *op1
)
{
  switch( ops ) {
    case JD_u_minus_exp:
      op1->is.real_value = -op1->is.real_value ;
      break ;
    case JD_u_not_exp:
      if( op1->is.real_value==0.0 ) op1->is.int_value = 1 ;
      else op1->is.int_value = 0 ;
      op1->type = JD_int_num ;
      break ;
    case JD_u_tilde_exp:
    case JD_u_and_exp:
    case JD_u_or_exp:
    case JD_u_eor_exp:
    case JD_u_nand_exp:
    case JD_u_nor_exp:
    case JD_u_neor_exp:
      ERROR_ILLEGAL_UNARY_BIT_OPERATION_ON_REAL_VALUE ;
      break ;
  }
  return op1 ;
}

expression_node  *aop_int( 
  JD_expression_type ops, expression_node *op1
)
{
  switch( ops ) {
    case JD_u_minus_exp:
      op1->is.int_value = -op1->is.int_value ;
      break ;
    case JD_u_tilde_exp:
      op1->is.int_value = ~op1->is.int_value ;
      break ;
    case JD_u_not_exp:
      if( op1->is.int_value==0 ) op1->is.int_value = 1 ;
      else op1->is.int_value = 0 ;
      break ;
    case JD_u_and_exp:
      if( op1->is.int_value == 0xffffffff ) op1->is.int_value = 1 ;
      else op1->is.int_value = 0 ;
      break ;
    case JD_u_or_exp:
      if( op1->is.int_value ) op1->is.int_value = 1 ;
      else op1->is.int_value = 0 ;
      break ;
    case JD_u_eor_exp:
      {
        int i , r;
        r = 0 ;
        for( i = 0 ; i < 32 ; i++ ) {
          r ^= op1->is.int_value & 1 ;
          op1->is.int_value >>= 1 ;
        }
        op1->is.int_value = r ;
      }
      break ;
    case JD_u_nand_exp:
      if( op1->is.int_value == 0xffffffff ) op1->is.int_value = 0 ;
      else op1->is.int_value = 1 ;
      break ;
    case JD_u_nor_exp:
      if( op1->is.int_value ) op1->is.int_value = 0 ;
      else op1->is.int_value = 1 ;
      break ;
    case JD_u_neor_exp:
      {
        int i , r;
        r = 1 ;
        for( i = 0 ; i < 32 ; i++ ) {
          r ^= op1->is.int_value & 1 ;
          op1->is.int_value >>= 1 ;
        }
        op1->is.int_value = r ;
      }
      break ;
  }
  return op1 ;
}

expression_node  *aop_bit( 
  JD_expression_type ops, expression_node *op1
)
{
  switch( ops ) {
    case JD_u_minus_exp:
      JD_u_minus_bit( 
        op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
      ) ;
      break ;
    case JD_u_tilde_exp:
      JD_u_tilde_bit( 
        op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
      ) ;
      break ;
    case JD_u_not_exp:
      {  int r ;
        r = JD_u_not_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        free( op1->is.bit_const.a ) ;
        free( op1->is.bit_const.b ) ;
        op1->type = JD_int_num ;
        op1->is.int_value = r ;
      }
      break ;
    case JD_u_and_exp:
      { int r ;
        r = JD_u_and_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
    case JD_u_or_exp:
      { int r ;
        r = JD_u_or_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
    case JD_u_eor_exp:
      { int r ;
        r = JD_u_eor_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
    case JD_u_nand_exp:
      { int r ;
        r = JD_u_nand_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
    case JD_u_nor_exp:
      { int r ;
        r = JD_u_nor_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
    case JD_u_neor_exp:
      { int r ;
        r = JD_u_neor_bit( 
          op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
        ) ;
        if( op1->is.bit_const.size > 32 ) {
          free( op1->is.bit_const.a ) ;
          free( op1->is.bit_const.b ) ;
          op1->is.bit_const.a = (int *)calloc( 1, sizeof(int) ) ;
          op1->is.bit_const.b = (int *)calloc( 1, sizeof(int) ) ;
        }
        op1->is.bit_const.size = 1 ;
        op1->is.bit_const.a[0] = r & 1 ;
        op1->is.bit_const.b[0] = r >> 1  ;
      }
      break ;
  }
  return op1 ;
}

expression_node  *aop_const( 
  JD_expression_type ops, expression_node *op1
)
{
  
  if( op1->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return NULL ;
  }
  switch( op1->type ) {
    case JD_real_num:
      aop_real( ops, op1 ) ;
      break ;
    case JD_int_num:
      aop_int( ops, op1 ) ;
      break ;
    case JD_bit_num:
      aop_bit( ops, op1 ) ;
      break ;
  }
  
  op1->data_type = JD_exp_num_type ;
  return op1 ;
    
}


expression_node  *mask_comp_const( 
  expression_node *op0, expression_node *op1, expression_node *op2
)
{
  
  double real_val ;
  int ta0, tb0, ta1, tb1, ta2, tb2 ;
  int *a0, *b0, *a1, *b1, *a2, *b2 ;
  int w0, w1, w2 ;
  int s0, s1, s2 ;
  
  int i, ret ;
  
  if( op0->type == JD_X_num || op1->type == JD_X_num || op2->type == JD_X_num) 
  {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return op0 ;
  }
  else if( op0->type == JD_real_num || op1->type == JD_real_num || 
      op2->type == JD_real_num ) 
  {
    ERROR_WRONG_DATA_TYPE_FOR_MASK_COMPARE_CONSTANT ;
    return op0 ;
  }
  else {
    switch( op0->type ) {
      case JD_int_num:
        ta0 = op0->is.int_value ;
        tb0 = 0 ;
        a0 = &ta0 ;
        b0 = &tb0 ;
        s0 = 32 ;
        break ;
      case JD_bit_num:
        a0 = op0->is.bit_const.a ;
        b0 = op0->is.bit_const.b ;
        s0 = op0->is.bit_const.size ;
        break ;
    }
    switch( op1->type ) {
      case JD_int_num:
        ta1 = op1->is.int_value ;
        tb1 = 0 ;
        a1 = &ta1 ;
        b1 = &tb1 ;
        s1 = 32 ;
        break ;
      case JD_bit_num:
        a1 = op1->is.bit_const.a ;
        b1 = op1->is.bit_const.b ;
        s1 = op1->is.bit_const.size ;
        break ;
    }
    switch( op2->type ) {
      case JD_int_num:
        ta2 = op2->is.int_value ;
        tb2 = 0 ;
        a2 = &ta2 ;
        b2 = &tb2 ;
        s0 = 32 ;
        break ;
      case JD_bit_num:
        a2 = op2->is.bit_const.a ;
        b2 = op2->is.bit_const.b ;
        s2 = op2->is.bit_const.size ;
        break ;
    }
    w0 = LASTWORD(s0) ;
    w1 = LASTWORD(s1) ;
    w2 = LASTWORD(s2) ;
    for( i = 0 ; i <= w2 ; i++ ) {
      if( b2[i] ) {
        ret = 2 ;
        goto eval_end ;
      }
    }
    ret = 1 ;
    for( i = w2 ; i >= 0 ; i-- ) {
      if( i > w0 && i <= w1 ) {
        if( a1[i] & a2[i] ) {
          ret = 0 ;
          goto eval_end ;
        }
      }
      else {
        if( (a0[i] & a2[i] != a1[i] & a2[i]) || 
            (b0[i] & a2[i] != b1[i] & a2[i])    ) {
          ret = 0 ;
          goto eval_end ;
        }
        
      }
    
    }
    eval_end:
  }
  
  if( op0->type == JD_bit_num ) {
    free(op0->is.bit_const.a) ;
    free(op0->is.bit_const.b) ;
    op0->type = JD_int_num ;
  }
  if( op1->type == JD_int_num )
    free(op1) ;
  else if( op1->type == JD_bit_num ) 
    free_bit(op1) ;
  if( op2->type == JD_int_num )
    free(op2) ;
  else if( op2->type == JD_bit_num ) 
    free_bit(op2) ;
  
  switch( ret ) {
    case 0: 
      op0->is.int_value = 0 ;
      break ;
    case 1:
      op1->is.int_value = 1 ;
      break ;
    case 2:
      op1->type = JD_X_num ;
      break ;
  }
  
  op0->data_type = JD_exp_num_type ;
  return op0 ;
    
}


expression_node  *conditional_const( 
  expression_node *op0, expression_node *op1, expression_node *op2
)
{
    
  int sel ;
  int w0 ;
  int i ;
  int *a0, *b0 ;
  expression_node  *ret ;
    
  if( op0->type == JD_X_num ) 
  {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return op1 ;
  }
  else {
    switch( op0->type ) {
      case JD_real_num:
        if( op0->is.real_value ) sel = 1 ;
        else sel = 0 ;
        free(op0) ;
        break ;
      case JD_int_num:
        if( op0->is.int_value ) sel = 1 ;
        else sel = 0 ;
        free(op0) ;
        break ;
      case JD_bit_num:
        a0 = op0->is.bit_const.a ;
        b0 = op0->is.bit_const.b ;
        w0 = LASTWORD(op0->is.bit_const.size) ;
        for( i = 0 ; i <= w0 ; i++ ) {
          if( b0[i] ) {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return op1 ;
          }
          if( a0[i] ) {
            sel = 1 ;
          }
        }
        free_bit(op0) ;
        break ;
    }
    
  }  
  switch( sel ) {
    case 0: 
      ret = op2 ;
      switch( op1->type ) {
        case JD_real_num:
        case JD_int_num:
          free(op1) ;
          ret->data_type = JD_exp_num_type ;
          break ;
        case JD_bit_num:
          free_bit(op1) ;
          ret->data_type = JD_exp_num_type ;
          break ;
        case JD_string_exp:
          free_string(op1) ;
          ret->data_type = JD_exp_string_type ;
          break ;
      }
      break ;
    case 1:
      ret = op1 ;
      switch( op2->type ) {
        case JD_real_num:
        case JD_int_num:
          free(op2) ;
          ret->data_type = JD_exp_num_type ;
          break ;
        case JD_bit_num:
          free_bit(op2) ;
          ret->data_type = JD_exp_num_type ;
          break ;
        case JD_string_exp:
          free_string(op2) ;
          ret->data_type = JD_exp_string_type ;
          break ;
      }
      break ;
  }
  
  return ret ;
    
}

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

expression_node  *concatenate_const( 
  expression_node *op0
)
{
    
  int len, l, nexp ;
  expression_node  *exp , *next ;
  int *a ;
  int *b ;
  int w0 ;
  int i ;
  
  exp = op0 ;
  len = 0 ;
  nexp = 0 ;
  while( exp ) {
    nexp++ ;
    if( exp->type == JD_int_num ) {
      len += 32 ;
    }
    else if( exp->type = JD_bit_num ) {
      len += exp->is.bit_const.size ;
    }
    else {
      if( exp->filename ) ERROR_LOCATION( exp->filename, exp->linenum ) ;
      ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
      return op0 ;
    }
    exp=exp->next ;
  } 
  a = (int *)calloc( NWORD(len), sizeof(int) ) ;
  b = (int *)calloc( NWORD(len), sizeof(int) ) ;
  
  exp = op0 ;
  l = 0 ;
  while( nexp ) {
    int x ;
    exp = op0 ;
    for( x = 1 ; x < nexp ; x++ ) exp = exp->next ; /* do from bottom */
    if( exp->type == JD_int_num ) {
      setbit(a, l, exp->is.int_value, 32 ) ;
      l += 32 ;
    }
    else if( exp->type = JD_bit_num ) {
      int s = exp->is.bit_const.size ;
      w0 = LASTWORD(exp->is.bit_const.size) ;
      for( i = 0 ; i <= w0 ; i++ ) {
        setbit( a, l, exp->is.bit_const.a[i], (i==w0)? s : 32 ) ;
        setbit( b, l, exp->is.bit_const.b[i], (i==w0)? s : 32 ) ;
        l += (i==w0)? s : 32 ;
        s -= 32 ;
      }
    }
    else {
      if( exp->filename ) ERROR_LOCATION( exp->filename, exp->linenum ) ;
      ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
      return op0 ;
    }
    nexp-- ;
  } 
  exp = op0->next ;
  
  while( exp ) {
    next = exp->next ;
    if( exp->type == JD_bit_num ) {
      free(exp->is.bit_const.a) ;
      free(exp->is.bit_const.b) ;
    }
    free(exp) ;
    exp = next ;
  }
  if( op0->type = JD_bit_num ) {
    free(op0->is.bit_const.a) ;
    free(op0->is.bit_const.b) ;
  }
  op0->type = JD_bit_num ;
  op0->is.bit_const.size = len ;
  op0->is.bit_const.a = a ;
  op0->is.bit_const.b = b ;
  
  op0->data_type = JD_exp_num_type ;
  return op0 ;
  
}


expression_node  *duplicate_const( 
  expression_node *op0, expression_node *op1
)
{
    
  int i, len, l, n ;
  int *a ;
  int *b ;
  int w0 ;
  
  if( op0->type != JD_int_num ) {
    ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
    return op0 ;
  }
  n = op0->is.int_value ;
  if( op1->type == JD_int_num ) {
    len = 32 ;
  }
  else if( op1->type = JD_bit_num ) {
    len = op1->is.bit_const.size ;
  }
  else {
    ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
    return op0 ;
  }
  len *= n ;
  a = (int *)calloc( NWORD(len), sizeof(int) ) ;
  b = (int *)calloc( NWORD(len), sizeof(int) ) ;
  
  l = 0 ;
  for( i = 0 ; i < n ; i++ ) {
    if( op1->type == JD_int_num ) {
      setbit(a, l, op1->is.int_value, 32 ) ;
      l += 32 ;
    }
    else if( op1->type = JD_bit_num ) {
      int s = op1->is.bit_const.size ;
      w0 = LASTWORD(s) ;
      for( i = 0 ; i <= w0 ; i++ ) {
        setbit( a, l, op1->is.bit_const.a[i], (i==w0)? s : 32 ) ;
        setbit( b, l, op1->is.bit_const.b[i], (i==w0)? s : 32 ) ;
        l += (i==w0)? s : 32 ;
        s -= 32 ;
      }
    }
    else {
      ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
      return op0 ;
    }
  } 
  
  if( op1->type = JD_bit_num ) {
    free(op1->is.bit_const.a) ;
    free(op1->is.bit_const.b) ;
  }
  free(op1) ;
  op0->type = JD_bit_num ;
  op0->is.bit_const.size = len ;
  op0->is.bit_const.a = a ;
  op0->is.bit_const.b = b ;
  
  op0->data_type = JD_exp_num_type ;
  return op0 ;
  
}


expression_node  *concatenate_string( 
  expression_node *op0
)
{
    
  expression_node  *exp , *next, *p ;
  int len ;
  
  exp = op0 ;
  next = op0->next ;
  
  while( next ) {
    len = strlen( op0->is.string ) ;
    op0->is.string = 
      (char *)realloc(
        op0->is.string, (len+strlen(next->is.string))*sizeof(char)
      ) ;
    op0->is.string[len-1] = 0 ;
    strcat(op0->is.string, &next->is.string[1]) ;
    p = next ;
    next = next->next ;
    free_string(p) ;
  }
  
  return op0 ;
}

expression_node  *duplicate_string( 
  expression_node *op0, expression_node *op1
)
{
    
  int i, len, n, l ;
  char *dup ;
  
  if( op0->type != JD_int_num ) {
    ERROR_ILLEGAL_DATA_TYPE_FOR_CONSTANT_CONCATENATION ;
    return op0 ;
  }
  n = op0->is.int_value ;
  len = strlen( op1->is.string ) ;
  dup = (char *)calloc( (len-1)*n + 1, sizeof(char) ) ;
  strcat( dup,op1->is.string ) ;
  l = len ;
  for( i = 1 ; i < n ; i++ ) {
    dup[l-1] = 0 ;
    l += (len-2) ;
    strcat( dup, &op1->is.string[1] ) ;
    /*
    fprintf( stderr, dup ) ;
    fprintf( stderr, "\n" ) ;
    */
  }
  free(op1->is.string) ;
  op1->is.string = dup ;
  
  return op1 ;

}

/* constand calculation for static random */
int const_range_const( 
  expression_node *op1, expression_node *op2, unsigned int *size,
  int *flag, int *width
)
{
  int ret, i ;
  
  if( op1->type == JD_X_num || op2->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return 0 ;
  }
  switch( op1->type ) {
    case JD_int_num:
      switch( op2->type ) {
        case JD_int_num:
          if( op1->is.int_value > op2->is.int_value ) {
            *size = op1->is.int_value - op2->is.int_value + 1 ;
            *flag = 0 ;
          }
          else {
            *size = op2->is.int_value - op1->is.int_value + 1 ;
            *flag = 1 ;
          }
          *width = 32 ;
          return 1 ;
          break ;
        case JD_bit_num:
          {  
            /* converting op1 to be bit_num */
            int tmp = op1->is.int_value ;
            op1->type = JD_bit_num ;
            op1->is.bit_const.size = 32 ;
            op1->is.bit_const.a = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.b = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op1->is.bit_const.a[0] = tmp ;
          }
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          ret = 
            JD_bit_gt_bit( 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size,
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size 
            ) ;
          if( ret < 0 ) {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          if( ret > 0 ) {
            /* op1 > op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op1->is.bit_const.size) ; i++ ) {
              a[i] = op1->is.bit_const.a[i] ;
              b[i] = op1->is.bit_const.b[i] ;
            }
            JD_bit_plus_bit( 
              a, b, s, 
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 0 ;
            if( op2->is.bit_const.size > 32 )
              *width = op2->is.bit_const.size ;
            else
              *width = 32 ;
            free( a ) ;
            free( b ) ;
            return 1 ;
          }
          else {
            /* op1 <= op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
              a[i] = op2->is.bit_const.a[i] ;
              b[i] = op2->is.bit_const.b[i] ;
            }
            JD_bit_plus_bit( 
              a, b, s, 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 1 ;
            if( op2->is.bit_const.size > op1->is.bit_const.size )
              *width = op2->is.bit_const.size ;
            else
              *width = op1->is.bit_const.size ;
            free( a ) ;
            free( b ) ;
            return 1 ;
          }
          break ;
        default:
          ERROR_ILLEGAL_CONSTANT_TYPE_FOR_RANDOM_RANGE ;
          break ;
      }
      break ;
    case JD_bit_num:
      switch( op2->type ) {
        case JD_int_num:       
          {  
            /* converting op1 to be bit_num */
            int tmp = op2->is.int_value ;
            op2->type = JD_bit_num ;
            op2->is.bit_const.size = 32 ;
            op2->is.bit_const.a = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op2->is.bit_const.b = (int *)calloc(NBYTE(op1->is.bit_const.size));
            op2->is.bit_const.a[0] = tmp ;
          }
          if( check_bit_set( op1->is.bit_const.b, op1->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          ret = 
            JD_bit_gt_bit( 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size,
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size 
            ) ;
          if( ret < 0 ) {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          if( ret > 0 ) {
            /* op1 > op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op1->is.bit_const.size) ; i++ ) {
              a[i] = op1->is.bit_const.a[i] ;
              b[i] = op1->is.bit_const.b[i] ;
            }
            JD_bit_plus_bit( 
              a, b, s, 
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 0 ;
            free( a ) ;
            free( b ) ;
            if( op2->is.bit_const.size > op1->is.bit_const.size )
              *width = op2->is.bit_const.size ;
            else
              *width = op1->is.bit_const.size ;
            return 1 ;
          }
          else {
            /* op1 <= op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
              a[i] = op2->is.bit_const.a[i] ;
              b[i] = op2->is.bit_const.b[i] ;
            }
            JD_bit_minus_bit( 
              a, b, s, 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 1 ;
            free( a ) ;
            free( b ) ;
            if( op2->is.bit_const.size > op1->is.bit_const.size )
              *width = op2->is.bit_const.size ;
            else
              *width = op1->is.bit_const.size ;
            return 1 ;
          }
          break ;
        case JD_bit_num:
          if( check_bit_set( op1->is.bit_const.b, op1->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          if( check_bit_set( op2->is.bit_const.b, op2->is.bit_const.size ) ) 
          {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          ret = 
            JD_bit_gt_bit( 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size,
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size 
            ) ;
          if( ret < 0 ) {
            ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
            return 0 ;
          }
          if( ret > 0 ) {
            /* op1 > op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op1->is.bit_const.size) ; i++ ) {
              a[i] = op1->is.bit_const.a[i] ;
              b[i] = op1->is.bit_const.b[i] ;
            }
            JD_bit_minus_bit( 
              a, b, s, 
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 0 ;
            free( a ) ;
            free( b ) ;
            if( op2->is.bit_const.size > op1->is.bit_const.size )
              *width = op2->is.bit_const.size ;
            else
              *width = op1->is.bit_const.size ;
            return 1 ;
          }
          else {
            /* op1 <= op2 */
            unsigned int *a, *b ;
            int s ;
            if( op1->is.bit_const.size > op2->is.bit_const.size )
              s = op1->is.bit_const.size ;
            else
              s = op2->is.bit_const.size ;
            a = (int *)calloc(NBYTE(s));
            b = (int *)calloc(NBYTE(s));
            for( i = 0 ; i < NWORD(op2->is.bit_const.size) ; i++ ) {
              a[i] = op2->is.bit_const.a[i] ;
              b[i] = op2->is.bit_const.b[i] ;
            }
            JD_bit_minus_bit( 
              a, b, s, 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size
            ) ;
            for( i = 1 ; i < NWORD(s) ; i++ ) {
              if( a[i] ) {
                ERROR_RANDOM_RANGE_TOO_LARGE_MAX_2_XX_32 ;
                return 0 ;
              }
            }
            *size = a[0] + 1 ;
            *flag = 1 ;
            free( a ) ;
            free( b ) ;
            if( op2->is.bit_const.size > op1->is.bit_const.size )
              *width = op2->is.bit_const.size ;
            else
              *width = op1->is.bit_const.size ;
            return 1 ;
          }
          break ;
        default:
          ERROR_ILLEGAL_CONSTANT_TYPE_FOR_RANDOM_RANGE ;
          return 0 ;
          break ;
      }
      break ;
    default:
      ERROR_ILLEGAL_CONSTANT_TYPE_FOR_RANDOM_RANGE ;
      return 0 ;
      break ;
  }
  
}

int range_const_comp( 
  expression_node *op1, expression_node *op2
)
{
  
  switch( op1->type ) {
    case JD_int_num:
      switch( op2->type ) {
        case JD_int_num:
          if( op1->is.int_value != op2->is.int_value )  return 0 ;
          else return 1 ;
          break ;
        default:
          return 0 ;
          break ;
      }
      break ;
    case JD_bit_num:
      switch( op2->type ) {
        case JD_int_num:       
          return 0 ;
          break ;
        case JD_bit_num:
          return 
            JD_bit_eqeq_bit( 
              op1->is.bit_const.a, op1->is.bit_const.b, op1->is.bit_const.size,
              op2->is.bit_const.a, op2->is.bit_const.b, op2->is.bit_const.size 
            ) ;
        default:
          return 0 ;
          break ;
      }
      break ;
    default:
      ERROR_ILLEGAL_CONSTANT_TYPE_FOR_RANDOM_RANGE ;
      return 0 ;
      break ;
  }
  
}

static int is_const_type( expression_node *op ) {
  if( op->data_type != JD_exp_num_type ) return 0 ;
  switch( op->type ) {
    case JD_int_num:
      return 1 ;
      break ;
    case JD_real_num:
      return 1 ;
      break ;
    case JD_bit_num:
      return 1 ;
      break ;
    default:
      return 0 ;
  }
}

expression_node *binary_constant_folding( 
  expression_node *exp 
)
{
  JD_expression_type ops;
  expression_node *op1, *op2, *new ;
  
  op1 = exp->is.binary.left ;
  op2 = exp->is.binary.right ;
  ops = exp->type ;
  
  if( !is_const_type( op1 ) || !is_const_type( op2 ) ) {
    return exp ;
  }
  
  new = const_aop_const( ops, op1, op2 ) ;
  
  
  if( new->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return exp ;
  }

  return new ;
  
}

expression_node *unary_constant_folding( 
  expression_node *exp 
)
{
  JD_expression_type ops;
  expression_node *op, *new ;
  
  op = exp->is.unary.this ;
  ops = exp->type ;
  
  if( !is_const_type( op ) ) {
    return exp ;
  }
  
  new = aop_const( ops, op ) ;
  
  
  if( new->type == JD_X_num ) {
    ERROR_CONSTANT_CALCLATION_FAILED_DUE_TO_X_OR_Z_IN_VALUE ;
    return exp ;
  }

  return new ;
  
}
