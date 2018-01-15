/*
 * Jeda Debugger expression evaluation
 */
/* 
   
   Copyright (C) 2001, 2002 Juniper Networks Inc.

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

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#include "debugger.h"
#include "debugger_error_msg.h"

static void  copy_exp( JD_dbg_expression_node *dd, JD_dbg_expression_node *d );

int JD_dbg_eval_error = 0 ;
#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif

/********************************************************************
 *  Debugger Eval Stack operation
 ********************************************************************/

JD_dbg_expression_node *JD_dbg_eval_stack_top = NULL ;

void JD_dbg_push( JD_dbg_expression_node *exp ) {
  if( JD_dbg_eval_stack_top )
    exp->next = JD_dbg_eval_stack_top ;
  JD_dbg_eval_stack_top = exp ;
}

JD_dbg_expression_node *JD_dbg_pop() {
  JD_dbg_expression_node *ret ;
  
  ret = JD_dbg_eval_stack_top ;
  if( JD_dbg_eval_stack_top )
    JD_dbg_eval_stack_top = ret->next ;
  else {
    DBG_ERROR_DEBUGGER_EVAL_STACK_UNDERFLOW ;
  }
  return ret ;
}


JD_dbg_expression_node *JD_dbg_alloc_exp() {
  JD_dbg_expression_node *ret ;
  
  ret = (JD_dbg_expression_node *)calloc( 1, sizeof(JD_dbg_expression_node) ) ;
  return ret ;
  
}

void JD_dbg_free_exp( JD_dbg_expression_node *exp ) {
  if( exp ) {
    if( exp->depth ) JD_dbg_free_exp( exp->depth ) ;
    if( exp->array_index ) JD_dbg_free_exp( exp->array_index ) ;
    if( exp->bit_index ) JD_dbg_free_exp( exp->bit_index ) ;
    if( exp->next ) JD_dbg_free_exp( exp->next ) ;
    if( exp->next_level ) JD_dbg_free_exp( exp->next_level ) ;
    
    if( exp->bit_a ) free( exp->bit_a ) ;
    if( exp->bit_b ) free( exp->bit_b ) ;
    if( exp->string ) free( exp->string ) ;
    free(exp) ;
  }
}

/***************************************************************
 *  Constant scanning functions
 ***************************************************************/
/* left-shifting array of bit data by num */
static void lshift_bit( unsigned *data, int glen, int num )
{
  int i, j ;
  for( i = 0 ; i < num ; i ++ )
    for( j = glen-1 ; j >= 0 ; j-- )
      data[j] = (data[j] << 1) | ( (j == 0) ? 0 : (data[j-1]>>31) & 1 ) ;
}


/* right-shifting array of bit data by num */
static void rshift_bit( unsigned *data, int glen, int num )
{
  int i, j ;
  for( i = 0 ; i < num ; i ++ )
    for( j = 0 ; j < glen ; j-- )
      data[j] = 
        ( (data[j] >> 1) & 0xefffffff ) | 
        ( (j == glen-1) ? 0 : (data[j+1]&1) << 31 ) ;
}

static unsigned get_bit( unsigned *data, int location, int len )
{
  unsigned ret, mask ;
  int i ;
  
  ret = data[location/32] ;
  
  ret >>= (location % 32) ;
  
  mask = 0 ;
  for( i = 0 ; i < len ; i++ ) {
    mask <<= 1 ;
    mask |= 1 ;
  }
  
  return ret & mask ;
}


/* calculate a = a + b */
static void add_bit( unsigned *a, unsigned *b, int glen )
{
  int i ;
  unsigned tmp, carry ;
  carry = 0 ;
  for( i = 0 ; i < glen ; i++ ) {
    tmp = a[i] + b[i] + carry ;
    if( tmp < a[i] ) carry = 1 ; /* detecting overflow */
    a[i] = tmp ;
  }
}

static void scan_decimal( int size, char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  unsigned *bit_c ;
  int ovflow = 0 ;
  int glen ;
  int i ;
  
  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_c = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    switch( *cp ) {
      case '_' :
        break ;
      default  :  /* 0 - 9 */
        /* do bit_a = bit_a * 10  */
        for( i = 0 ; i < glen ; i++ ) bit_b[i] = bit_a[i] ;
        if( get_bit(bit_a, size-3, 3) ) {
          if( !ovflow  ) {
            DBG_WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
            ovflow = 1 ;
          }
          size += 4 ;
          if( NWORD(size) > glen ) {
            int pglen = glen ;
            glen = NWORD(size) ;
            bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
            bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
            bit_c = (unsigned *)realloc(bit_c, glen*sizeof(unsigned) ) ;
            for( i = pglen-1 ; i < glen ; i++ ) {
              bit_a[i] = bit_b[i] = bit_c[i] = 0 ;
            }
          }
        }
        lshift_bit(bit_a, glen, 3 ) ; /* a = a * 8 */
        lshift_bit(bit_b, glen, 1 ) ; /* b = a * 8 */
        add_bit(bit_a, bit_b, glen ) ; /* a = (a*8) + (a*2) */
        bit_c[0] = *cp-'0' ;
        add_bit(bit_a, bit_c, glen ) ;
        break ;
    }
    cp++ ;
  }
  
  exp->type = JD_D_bit ;
  exp->size = size ;
  exp->bit_a = bit_a ;
  for( i = 0 ; i < glen ; i++ ) bit_b[i] = 0 ;
  exp->bit_b = bit_b ;
  free( bit_c ) ;


}
void JD_scan_v_decimal(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      DBG_ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;
  
}


void JD_scan_c_decimal( char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  int  size ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  unsigned *bit_c ;
  int glen ;
  int ovflow = 1 ; /* no warning since size is not specified */
  
  size = 32 ;
  glen =  1 ;
    
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;

}

static void scan_hex( int size, char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  
  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-4, 4) || get_bit(bit_b, size-4, 4) ) {
        if( !ovflow  ) {
          DBG_WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 4 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 4 ) ; 
      lshift_bit(bit_b, glen, 4 ) ; 
      switch( *cp ) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
          bit_a[0] |= (*cp-'a'+10) ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          bit_a[0] |= (*cp-'A'+10) ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0xf ;
          bit_b[0] |= 0xf ;
          if( ext_flag==0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0xf ;
          if( ext_flag==0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }
  
  if( ext_flag > 1 ) {
    int a, b, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }
  
  exp->type = JD_D_bit ;
  exp->size = size ;
  exp->bit_a = bit_a ;
  exp->bit_b = bit_b ;
  
}

void JD_scan_v_hex(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      DBG_ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip h/H */
  
  scan_hex( size, cp, exp ) ;

}

void JD_scan_c_hex(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip x/X */
  
  scan_hex( size, cp, exp ) ;

}

static void scan_octal( int size, char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-3, 3) || get_bit(bit_b, size-3, 3) ) {
        if( !ovflow  ) {
          DBG_WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 3 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 3 ) ; 
      lshift_bit(bit_b, glen, 3 ) ; 
      switch( *cp ) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag==0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x7 ;
          bit_b[0] |= 0x7 ;
          if( ext_flag==0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x7 ;
          if( ext_flag==0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }

  if( ext_flag > 1 ) {
    int a, b, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }

  exp->type = JD_D_bit ;
  exp->size = size ;
  exp->bit_a = bit_a ;
  exp->bit_b = bit_b ;
  
}


void JD_scan_v_octal(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      DBG_ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

void JD_scan_c_octal(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

static void scan_binary( int size, char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-1, 1) || get_bit(bit_b, size-1, 1) ) {
        if( !ovflow  ) {
          DBG_WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
          ovflow = 1 ;
        }
        size += 1 ;
        if( NWORD(size) > glen ) {
          int pglen = glen ;
          glen = NWORD(size) ;
          bit_a = (unsigned *)realloc(bit_a, glen*sizeof(unsigned) ) ;
          bit_b = (unsigned *)realloc(bit_b, glen*sizeof(unsigned) ) ;
          for( i = pglen-1 ; i < glen ; i++ ) {
            bit_a[i] = bit_b[i] = 0 ;
          }
        }
      }
      lshift_bit(bit_a, glen, 1 ) ; 
      lshift_bit(bit_b, glen, 1 ) ; 
      switch( *cp ) {
        case '0': case '1':
          /* do bit_a = bit_a * 10  */
          bit_a[0] |= (*cp-'0') ;
          if( ext_flag == 0 ) ext_flag = 1 ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x1 ;
          bit_b[0] |= 0x1 ;
          if( ext_flag == 0 ) ext_flag = 3 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x1 ;
          if( ext_flag == 0 ) ext_flag = 2 ;
          break ;
      }
    }
    cp++ ;
  }

  if( ext_flag > 1 ) {
    int a, b, m, j ;
    int n = (size-1)/32 + 1 ;
    for( i = n-1 ; i >= 0 ; i-- ) {
      if ( bit_a[i] ) {
        a = bit_a[i] ;
        j = 0 ;
        while( (a & 0x80000000) == 0 ) {
          a <<= 1 ;
          j++ ;
        }
        if( j ) {
          m = 0xffffffff << (32-j) ;
          bit_b[i] |= m ;
          if( ext_flag == 3 ) bit_a[i] |= m ;
        }
        break ;
      }
      else {
        bit_b[i] = 0xffffffff ;
        if( ext_flag == 3 ) bit_a[i] = 0xffffffff ;
      } 
    }
  }
  
  if( size % 32 ) {
    int m = (1<<(size%32))-1 ;
    bit_a[glen-1] &= m ;
    bit_b[glen-1] &= m ;
  }

  exp->type = JD_D_bit ;
  exp->size = size ;
  exp->bit_a = bit_a ;
  exp->bit_b = bit_b ;
  
}


void JD_scan_v_binary(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  char save ;
  int  size ;
  
  if( *cp == '\'' ) {
    size = 32 ;
    cp++ ; /* skip ' */
  }
  else {
    while( isdigit(*cp) ) cp++ ;
    save = *cp ;
    *cp = '\0' ;  /* this location must be ' */
    size = atoi(number) ;
    *cp++ = save ;
    if( size == 0 ) {
      DBG_ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

void JD_scan_c_binary(  char *number, JD_dbg_expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

/***************************************************************
 *  binary calculation functions
 ***************************************************************/
 
static void copy_bit( 
  JD_dbg_expression_node *d1, JD_dbg_expression_node *dd 
) {
  int nw ;
  int i ;
  if( d1->type == JD_D_dt ) {
    if( d1->dt->type == JD_D_sbit ) {
      nw = NWORD(d1->dt->data.sbit.size) ;
      dd->bit_a = (void *)calloc( nw , sizeof(int) ) ;
      dd->bit_b = (void *)calloc( nw , sizeof(int) ) ;
      dd->bit_a[0] = d1->dt->data.sbit.a ;
      dd->bit_b[0] = d1->dt->data.sbit.b ;
      dd->size = d1->dt->data.sbit.size ;
    }
    else {
      nw = NWORD(d1->dt->data.bit.size) ;
      dd->bit_a = (void *)calloc( nw, sizeof(int) ) ;
      dd->bit_b = (void *)calloc( nw, sizeof(int) ) ;
      for( i = 0 ; i < nw ; i++) {
        dd->bit_a[i] = d1->dt->data.bit.a[i] ;
        dd->bit_b[i] = d1->dt->data.bit.b[i] ;
      }
    }
  }
  else {
    nw = NWORD(d1->size) ;
    dd->size = d1->size ;
    dd->bit_a = (void *)calloc( nw, sizeof(int) ) ;
    dd->bit_b = (void *)calloc( nw, sizeof(int) ) ;
    for( i = 0 ; i < nw ; i++) {
      dd->bit_a[i] = d1->bit_a[i] ;
      dd->bit_b[i] = d1->bit_b[i] ;
    }
  }
  dd->type = JD_D_bit ; 
}


#define INT_INT_CODE 0
#define INT_BIT_CODE 1
#define INT_FLT_CODE 2
#define BIT_INT_CODE 3
#define BIT_BIT_CODE 4
#define BIT_FLT_CODE 5
#define FLT_INT_CODE 6
#define FLT_BIT_CODE 7
#define FLT_FLT_CODE 8
#define OBJ_OBJ_CODE 9
#define STR_STR_CODE 10
#define SIG_SIG_CODE 11

static int data_type_code( JD_dbg_expression_node *d1, JD_dbg_expression_node *d2 )
{
  int ret = -1 ;
  JD_data_type type1, type2 ;
  
  if( d1->type == JD_D_dt ) {
    if( d1->dt->sub_type != JD_SD_single ) return ret ;
    type1 = d1->dt->type ;
  }
  else type1 = d1->type ;
  
  if( d2->type == JD_D_dt ) {
    if( d2->dt->sub_type != JD_SD_single ) return ret ;
    type2 = d2->dt->type ;
  }
  else type2 = d2->type ;
  
  switch( type1 ) {
    case JD_D_int:
      switch( type2 ) {
        case JD_D_int:
          ret = INT_INT_CODE ;
          break ;
        case JD_D_bit:
          ret = INT_BIT_CODE ;
          break ;
        case JD_D_double:
        case JD_D_float:
          ret = INT_FLT_CODE ;
          break ;
      }
      break ;
    case JD_D_bit:
      switch( type2 ) {
        case JD_D_int:
          ret = BIT_INT_CODE ;
          break ;
        case JD_D_bit:
          ret = BIT_BIT_CODE ;
          break ;
        case JD_D_double:
        case JD_D_float:
          ret = BIT_FLT_CODE ;
          break ;
      }
      break ;
    case JD_D_double:
    case JD_D_float:
      switch( type2 ) {
        case JD_D_int:
          ret = FLT_INT_CODE ;
          break ;
        case JD_D_bit:
          ret = FLT_BIT_CODE ;
          break ;
        case JD_D_double:
        case JD_D_float:
          ret = FLT_FLT_CODE ;
          break ;
      }
      break ;
    case JD_D_object:
      if( type1 == type2 || type2 == JD_D_null ) ret = OBJ_OBJ_CODE ;
      break ;
    case JD_D_null:
      switch( type2 ) {
        case JD_D_object:
          ret = OBJ_OBJ_CODE ;
          break ;
        case JD_D_string:
          ret = STR_STR_CODE ;
          break ;
        case JD_D_signal:
          ret = SIG_SIG_CODE ;
          break ;
      }
      break ;
    case JD_D_string:
      if( type1 == type2 || type2 == JD_D_null ) ret = STR_STR_CODE ;
    case JD_D_signal:
      if( type1 == type2 || type2 == JD_D_null ) ret = SIG_SIG_CODE ;
      break ;
  }
  return ret ;
}


/********************************************************************
 *  Bit size of the binary alu result 
 *  According to the rules of expression bit length on IEEE Std1364
 ********************************************************************/ 
static int get_bit_size( JD_dbg_op ops, int size1, int size2 ) 
{
  int size ;
  
  switch( ops ) {
    case JD_dbg_b_times_op:     /* expression '*' expression   */
    case JD_dbg_b_div_op:       /* expression '/' expression   */
    case JD_dbg_b_mod_op:       /* expression '%' expression   */
    case JD_dbg_b_land_op:      /* expression '&&' expression  */
    case JD_dbg_b_lor_op:       /* expression '||' expression  */
    case JD_dbg_b_rshift_op:    /* expression '>>' expression  */
    case JD_dbg_b_urshift_op:   /* expression '>>>' expression */
    case JD_dbg_b_lshift_op:    /* expression '<<' expression  */
    case JD_dbg_b_lt_op:        /* expression '<' expression   */
    case JD_dbg_b_gt_op:        /* expression '>' expression   */
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
    case JD_dbg_b_le_op:        /* expression '<=' expression  */
    case JD_dbg_b_ge_op:        /* expression '>=' expression  */
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
    case JD_dbg_b_Qne_op:        /* expression '!?=' expression  */
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
    case JD_dbg_b_eqQeq_op:     /* expression '=?=' expression */
    case JD_dbg_b_eqeqQ_op:     /* expression '==?' expression */
    case JD_dbg_b_Qeqeq_op:     /* expression '?==' expression */
      return size1 ;
    default:
      if( size1 > size2 ) return size1 ;
      else return size2 ;
  }
  
}

static void eval_bit_bin_ops (
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
) 
{
  int *a1, *b1, size1 ;
  int *a2, *b2, size2 ;
  int *ar, *br, sizer ;
  int err = 0 ;
  int is_safe ;
  int size, nw, nw1 ;
  int i ;
  
  /* first we solve the size so that we know how many bit needed for result */
  if( d1->type == JD_D_dt ) {
    if( d1->dt->type == JD_D_sbit ) {
      size1 = d1->dt->data.sbit.size ;
    }
    else {
      size1 = d1->dt->data.bit.size ;
    }
  }
  else {
    size1 = d1->size ;
  }
  
  if( d2->type == JD_D_dt ) {
    if( d2->dt->type == JD_D_sbit ) {
      size2 = d2->dt->data.sbit.size ;
      a2 = &d2->dt->data.sbit.a ;
      b2 = &d2->dt->data.sbit.b ;
    }
    else {
      size2 = d2->dt->data.bit.size ;
      a2 = d2->dt->data.bit.a ;
      b2 = d2->dt->data.bit.b ;
    }
  }
  else {
    size2 = d2->size ;
    a2 = d2->bit_a ;
    b2 = d2->bit_b ;
  }
  
  
  nw1 = NWORD(size1) ;
  size = get_bit_size( op, size1, size2 ) ;
  nw = NWORD(size) ;
  
  if( d1->type == JD_D_dt ) {
    if( d1->dt->type == JD_D_sbit ) {
      a1 = (void *)calloc( nw, sizeof(int) ) ;
      b1 = (void *)calloc( nw, sizeof(int) ) ;
      a1[0] = d1->dt->data.sbit.a ;
      b1[0] = d1->dt->data.sbit.b ;
    }
    else {
      a1 = (void *)calloc( nw, sizeof(int) ) ;
      b1 = (void *)calloc( nw, sizeof(int) ) ;
      for( i = 0 ; i < nw1 ; i++) {
        a1[i] = d1->dt->data.bit.a[i] ;
        b1[i] = d1->dt->data.bit.b[i] ;
      }
    }
  }
  else {
    a1 = (void *)calloc( nw, sizeof(int) ) ;
    b1 = (void *)calloc( nw, sizeof(int) ) ;
    for( i = 0 ; i < nw1 ; i++) {
      a1[i] = d1->bit_a[i] ;
      b1[i] = d1->bit_b[i] ;
    }
  }
  
  /* default result, may be overwritten */
  dd->type = JD_D_bit ; 
  dd->bit_a = a1 ;
  dd->bit_b = b1 ;
  dd->size = size ;
    
  switch( op ) {
    case JD_dbg_b_minus_op:     /* exp '-' exp   */
      JD_bit_minus_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_plus_op:      /* exp '+' exp   */
      JD_bit_plus_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_times_op:     /* exp '*' exp   */
      sizer = size1 + size2 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_times_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2) ;
      dd->bit_a = ar ;
      dd->bit_b = br ;
      dd->size = sizer ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_div_op:       /* exp '/' exp   */
      sizer = size1 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_div_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2, &err) ;
      if( err )
        DBG_ERROR_ZERO_DIVIDE_ON_EXPRESSION_EVALUATION ;
      dd->bit_a = ar ;
      dd->bit_b = br ;
      dd->size = sizer ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_mod_op:       /* exp '%' exp   */
      size = size1 ;
      ar = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      br = (int *)calloc(NWORD(sizer), sizeof(int)) ;
      JD_bit_mod_bit( ar, br, sizer, a1, b1, size1, a2, b2, size2, &err) ;
      if( err )
        DBG_ERROR_ZERO_DIVIDE_ON_EXPRESSION_EVALUATION ;
      dd->bit_a = ar ;
      dd->bit_b = br ;
      dd->size = sizer ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_and_op:       /* exp '&' exp   */
      JD_bit_and_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_or_op:        /* exp '|' exp   */
      JD_bit_or_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_eor_op:       /* exp '^' exp   */
      JD_bit_eor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_nand_op:      /* exp '&~' exp or exp '~&' exp  */
      JD_bit_nand_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_nor_op:       /* exp '|~' exp or exp '~|' exp  */
      JD_bit_nor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_neor_op:      /* exp '^~' exp or exp '~^' exp  */
      JD_bit_neor_bit( a1, b1, size1, a2, b2, size2) ;
      break ;
    case JD_dbg_b_lt_op:        /* expression '<' expression   */
      dd->int_value = JD_bit_lt_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_gt_op:        /* expression '>' expression   */
      dd->int_value = JD_bit_gt_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
      dd->int_value = JD_bit_eqeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_le_op:        /* expression '<=' expression  */
      dd->int_value = JD_bit_le_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_ge_op:        /* expression '>=' expression  */
      dd->int_value = JD_bit_ge_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
      dd->int_value = JD_bit_ne_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_Qne_op:        /* expression '!?=' expression  */
      dd->int_value = JD_bit_Qne_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      dd->int_value = JD_bit_eqeqeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      dd->int_value = JD_bit_neqeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_eqQeq_op:     /* expression '=?=' expression */
      dd->int_value = JD_bit_eqQeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_eqeqQ_op:     /* expression '==?' expression */
      dd->int_value = JD_bit_eqeqQ_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    case JD_dbg_b_Qeqeq_op:     /* expression '?==' expression */
      dd->int_value = JD_bit_Qeqeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      dd->type = JD_D_int ;
      free(a1) ; free(b1) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION ;
      break ;
    
  }
  
}

static void eval_int_bin_ops (
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
) 
{
  int op1, op2 ;
  
  if( d1->type == JD_D_dt ) {
    op1 = d1->dt->data.int_value ;
  }
  else op1 = d1->int_value ;
  if( d2->type == JD_D_dt ) {
    op2 = d2->dt->data.int_value ;
  }
  else op2 = d2->int_value ;
  
  dd->type = JD_D_int ;
  
  switch( op ) {
    case JD_dbg_b_minus_op:     /* exp '-' exp   */
      dd->int_value = op1 - op2 ;
      break ;
    case JD_dbg_b_plus_op:      /* exp '+' exp   */
      dd->int_value = op1 + op2 ;
      break ;
    case JD_dbg_b_times_op:     /* exp '*' exp   */
      dd->int_value = op1 * op2 ;
      break ;
    case JD_dbg_b_div_op:       /* exp '/' exp   */
      dd->int_value = op1 / op2 ;
      break ;
    case JD_dbg_b_mod_op:       /* exp '%' exp   */
      dd->int_value = op1 % op2 ;
      break ;
    case JD_dbg_b_and_op:       /* expression '&' expression   */
      dd->int_value = op1 & op2 ;
      break ;
    case JD_dbg_b_or_op:        /* expression '|' expression   */
      dd->int_value = op1 | op2 ;
      break ;
    case JD_dbg_b_eor_op:       /* expression '^' expression   */
      dd->int_value = op1 ^ op2 ;
      break ;
    case JD_dbg_b_nand_op:      /* expression '&~' expression  */
      dd->int_value = ~(op1 & op2) ;
      break ;
    case JD_dbg_b_nor_op:       /* expression '|~' expression  */
      dd->int_value = ~(op1 | op2) ;
      break ;
    case JD_dbg_b_neor_op:      /* expression '^~' expression  */
      dd->int_value = ~(op1 ^ op2) ;
      break ;
    case JD_dbg_b_rshift_op:    /* expression '>>' expression  */
      dd->int_value = op1 >> op2 ;
      break ;
    case JD_dbg_b_urshift_op:   /* expression '>>>' expression */
      dd->int_value = (unsigned)op1 >> op2 ;
      break ;
    case JD_dbg_b_lshift_op:    /* expression '<<' expression  */
      dd->int_value = op1 << op2 ;
      break ;
    case JD_dbg_b_lt_op:        /* expression '<' expression   */
      dd->int_value = (op1 < op2)?1:0 ;
      break ;
    case JD_dbg_b_gt_op:        /* expression '>' expression   */
      dd->int_value = (op1 > op2)?1:0 ;
      break ;
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
      dd->int_value = (op1 == op2)?1:0 ;
      break ;
    case JD_dbg_b_le_op:        /* expression '<=' expression  */
      dd->int_value = (op1 <= op2)?1:0 ;
      break ;
    case JD_dbg_b_ge_op:        /* expression '>=' expression  */
      dd->int_value = (op1 >= op2)?1:0 ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
      dd->int_value = (op1 != op2)?1:0 ;
      break ;
    case JD_dbg_b_Qne_op:        /* expression '!?=' expression  */
      dd->int_value = (op1 != op2)?1:0 ;
      break ;
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      break ;
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      dd->int_value = (op1 != op2)?1:0 ;
      break ;
    case JD_dbg_b_eqQeq_op:     /* expression '=?=' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      break ;
    case JD_dbg_b_eqeqQ_op:     /* expression '==?' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      break ;
    case JD_dbg_b_Qeqeq_op:     /* expression '?==' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION ;
      break ;
  }

}

static void eval_flt_bin_ops (
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{
  double op1, op2 ;
  
  if( d1->type == JD_D_dt ) {
    op1 = 
      (d1->dt->type==JD_D_float) ? 
        d1->dt->data.float_value : d1->dt->data.double_value ;
  }
  else op1 = d1->real_value ;
  if( d2->type == JD_D_dt ) {
    op2 = 
      (d2->dt->type==JD_D_float) ? 
        d2->dt->data.float_value : d2->dt->data.double_value ;
  }
  else op2 = d2->real_value ;
  
  dd->type = JD_D_double ;
  
  switch( op ) {
    case JD_dbg_b_minus_op:     /* exp '-' exp   */
      dd->real_value = op1 - op2 ;
      break ;
    case JD_dbg_b_plus_op:      /* exp '+' exp   */
      dd->real_value = op1 + op2 ;
      break ;
    case JD_dbg_b_times_op:     /* exp '*' exp   */
      dd->real_value = op1 * op2 ;
      break ;
    case JD_dbg_b_div_op:       /* exp '/' exp   */
      dd->real_value = op1 / op2 ;
      break ;
    case JD_dbg_b_mod_op:       /* exp '%' exp   */
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_MODULO_OPERATION ;
      break ;
    case JD_dbg_b_lt_op:        /* expression '<' expression   */
      dd->int_value = (op1 < op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_gt_op:        /* expression '>' expression   */
      dd->int_value = (op1 > op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
      dd->int_value = (op1 == op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_le_op:        /* expression '<=' expression  */
      dd->int_value = (op1 <= op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_ge_op:        /* expression '>=' expression  */
      dd->int_value = (op1 >= op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
      dd->int_value = (op1 != op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_Qne_op:        /* expression '!?=' expression  */
      dd->int_value = (op1 != op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      dd->int_value = (op1 != op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_eqQeq_op:     /* expression '=?=' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_eqeqQ_op:     /* expression '==?' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    case JD_dbg_b_Qeqeq_op:     /* expression '?==' expression */
      dd->int_value = (op1 == op2)?1:0 ;
      dd->type = JD_D_int ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION ;
      break ;
  }
  
}

static void eval_obj_bin_ops( 
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, 
  JD_dbg_expression_node *dd 
)
{
  dd->type = JD_D_int ;
  
  switch( op ) {
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
      dd->int_value = JD_obj_eqeq_obj( d1->dt, d2->dt ) ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
      dd->int_value = !JD_obj_eqeq_obj( d1->dt, d2->dt ) ;
      break ;
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      if( d1->dt->data.obj.ect == d2->dt->data.obj.ect &&
          d1->dt->data.obj.alloc == d2->dt->data.obj.alloc ) 
        dd->int_value = 1 ;
      else
        dd->int_value = 0 ;
      break ;
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      if( d1->dt->data.obj.ect != d2->dt->data.obj.ect ||
          d1->dt->data.obj.alloc != d2->dt->data.obj.alloc ) 
        dd->int_value = 0 ;
      else
        dd->int_value = 1 ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_BITWIDTH_LOGIC_OPERATION ;
      break ;
  }
  
}

static void eval_sig_bin_ops( 
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{
  dd->type = JD_D_int ;
  
  switch( op ) {
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      if( d1->dt->data.signal.port == d2->dt->data.signal.port &&
          d1->dt->data.signal.data0 == d2->dt->data.signal.data0 &&
          d1->dt->data.signal.data1 == d2->dt->data.signal.data1    ) 
        dd->int_value = 1 ;
      else
        dd->int_value = 0 ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      if( d1->dt->data.signal.port == d2->dt->data.signal.port &&
          d1->dt->data.signal.data0 == d2->dt->data.signal.data0 &&
          d1->dt->data.signal.data1 == d2->dt->data.signal.data1    ) 
        dd->int_value = 0 ;
      else
        dd->int_value = 1 ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_SIGNAL_BINARY_OPERATION ;
      break ;
  }
}

static void eval_str_bin_ops( 
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{

  dd->type = JD_D_int ;
  
  switch( op ) {
    case JD_dbg_b_eqeq_op:      /* expression '==' expression  */
    case JD_dbg_b_eqeqeq_op:    /* expression '===' expression */
      if( !strcmp(d1->string, d2->string) ) 
        dd->int_value = 1 ;
      else
        dd->int_value = 0 ;
      break ;
    case JD_dbg_b_ne_op:        /* expression '!=' expression  */
    case JD_dbg_b_neqeq_op:     /* pression '!==' expression   */
      if( !strcmp(d1->string, d2->string) ) 
        dd->int_value = 0 ;
      else
        dd->int_value = 1 ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_OPERAND_TYPE_FOR_STRING_BINARY_OPERATION ;
      break ;
  }

}

static int is_logic_op( JD_dbg_op op ) {
  switch( op ) {
    case JD_dbg_b_land_op:      /* expression '&&' expression  */
    case JD_dbg_b_lor_op:       /* expression '||' expression  */
      return 1 ;
    default:
      return 0 ;
  }
}

static int is_shift_op( JD_dbg_op op ) {
  switch( op ) {
    case JD_dbg_b_rshift_op:    /* expression '>>' expression  */
    case JD_dbg_b_urshift_op:   /* expression '>>>' expression */
    case JD_dbg_b_lshift_op:    /* expression '<<' expression  */
      return 1 ;
    default:
      return 0 ;
  }
}

eval_bit_shift_ops(
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{
  int shift, sft, rsft, idx ;
  int i, j, n, a, b, aa, bb, mm ;
  
  copy_bit( d1, dd ) ;
  
  dd->type = JD_D_bit ;
  
  shift = d2->int_value ;
  n = LASTWORD(dd->size) ;
  if( !shift || shift < 0 ) return ;
  if( shift >= dd->size ) {
    for( i = 0 ; i < NWORD(dd->size) ; i++ ) {
      dd->bit_a[i] = 0 ;
      dd->bit_b[i] = 0 ;
    }
    return ;
  }
  switch( op ) {
    case JD_dbg_b_rshift_op:    /* expression '>>' expression  */
    case JD_dbg_b_urshift_op:   /* expression '>>>' expression */
      sft = shift % 32 ;
      rsft = sft?(32-sft):0 ;
      idx = shift / 32 ;
      aa = bb = 0 ;
      j = idx ;
      mm = (unsigned)0xffffffff>>((32-(ACC_SIZE%32))%32) ;
      for( i = 0 ; i <= n ; i++ ) {
        if( j+1 <= n && sft ) {
          a = (unsigned int)dd->bit_a[j+1] << rsft ;
          b = (unsigned int)dd->bit_b[j+1] << rsft ;
        }
        else a = b = 0 ;
        if( j <= n ) {
          a |= (unsigned int)dd->bit_a[j] >> sft ;
          b |= (unsigned int)dd->bit_b[j] >> sft ;
        }
        dd->bit_a[i] = a ;
        dd->bit_b[i] = b ;
        j++ ;
      }
      dd->bit_a[n] &= mm ;
      dd->bit_b[n] &= mm ;
      break ;
    case JD_dbg_b_lshift_op:    /* expression '<<' expression  */
      sft = shift % 32 ;
      rsft = sft?(32-sft):0 ;
      idx = shift / 32 ;
      aa = bb = 0 ;
      j = n - idx ;
      mm = (unsigned)0xffffffff>>((32-(ACC_SIZE%32))%32) ;
      for( i = n ; i >= 0 ; i-- ) {
        if( j >= 0 ) {
          a = dd->bit_a[j] << sft ;
          b = dd->bit_b[j] << sft ;
          if( j-1 >= 0 && sft ) {
            a |= (unsigned int)dd->bit_a[j-1] >> rsft ;
            b |= (unsigned int)dd->bit_b[j-1] >> rsft ;
          }
        }
        else {
          a = b = 0 ;
        }
        dd->bit_a[i] = a ;
        dd->bit_b[i] = b ;
        j-- ;
      }
      dd->bit_a[n] &= mm ;
      dd->bit_b[n] &= mm ;
      break ;
  }
}

static void bit_to_int( JD_dbg_expression_node *d ) 
{
  int i ;
  int err = 0 ;
  
  d->type = JD_D_int ;
  for( i = 0 ; i < NWORD(d->size) ; i++ ) 
    if( d->bit_b[i] ) {
      err = 1 ;
    }
  for( i = 1 ; i < NWORD(d->size) ; i++ ) 
    if( d->bit_a[i] ) {
      err = 1 ;
    }
  if( err ) {
    d->int_value = -1 ;
  }
  else {
    d->int_value = d->bit_a[0] ;
  }
  free( d->bit_a ) ;
  free( d->bit_b ) ;
}

static void int_to_bit( JD_dbg_expression_node *d ) 
{
  
  d->type = JD_D_bit ;
  d->size = 32 ;
  d->bit_a = (int *)calloc(1, sizeof(int) ) ;
  d->bit_b = (int *)calloc(1, sizeof(int) ) ;
  d->bit_a[0] = d->int_value ;
}

static void flt_to_bit( JD_dbg_expression_node *d ) 
{
  
  d->type = JD_D_bit ;
  d->size = 32 ;
  d->bit_a = (int *)calloc(1, sizeof(int) ) ;
  d->bit_b = (int *)calloc(1, sizeof(int) ) ;
  d->bit_a[0] = (int)d->real_value ;
}

static void bit_to_flt( JD_dbg_expression_node *d ) 
{
  int i, n ;
  double sft ;
  n = LASTWORD(d->size) ;
  for( i = 0 ; i <= n ; i++ ) {
    if( d->bit_b[i] ) {
      DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_FLOAT_CONVERSION ;
      n = 0 ;
    }
  }
  sft = 2.0 ;
  for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
  d->real_value = 0.0 ;
  n = NWORD(ACC_SIZE) ;
  for( i = 0 ; i < n ; i++ ) {
    d->real_value *= sft ;
    d->real_value += d->bit_a[i] ;
  }
  d->type = JD_D_float ;
  free(d->bit_a) ;
  free(d->bit_b) ;
}

static void shift_ops(
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{
  int code ;
    
  code = data_type_code( d1, d2 ) ;
  
  switch( code ) {
    case INT_INT_CODE:
      eval_int_bin_ops( op, d1, d2, dd ) ;
      break ;
    case INT_BIT_CODE:
      bit_to_int( d2 ) ;
      eval_int_bin_ops( op, d1, d2, dd ) ;
      break ;
    case INT_FLT_CODE:
      d2->type = JD_D_int ;
      d2->int_value = d2->real_value ;
      eval_int_bin_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_INT_CODE:
      eval_bit_shift_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_BIT_CODE:
      bit_to_int( d2 ) ;
      eval_bit_shift_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_FLT_CODE:
      d2->type = JD_D_int ;
      d2->int_value = d2->real_value ;
      eval_bit_shift_ops( op, d1, d2, dd ) ;
      break ;
    case FLT_INT_CODE:
    case FLT_BIT_CODE:
    case FLT_FLT_CODE:
    case OBJ_OBJ_CODE:
    case STR_STR_CODE:
    case SIG_SIG_CODE:
    default:
      DBG_ERROR_ILLEGAL_DATA_TYPE_COMBINATION_FOR_BINARY_OPS ;
      break ;
  }

}

int JD_dbg_is_true( JD_dbg_expression_node *d ) {
  int i ;
  switch( d->type ) {
    case JD_D_int:
      if( d->int_value ) return 1 ;
      else return 0 ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      for( i = 0 ; i < NWORD(d->size) ; i++ ) if( d->bit_b[i] ) return 0 ;
      for( i = 0 ; i < NWORD(d->size) ; i++ ) if( d->bit_a[i] ) return 1 ;
      return 0 ;
      break ;
    case JD_D_float:
    case JD_D_double:
      if( d->real_value != 0. ) return 1 ;
      else return 0 ;
    case JD_D_object:
      if( d->dt && d->dt->data.obj.ect ) return 1 ;
      else return 0 ;
    case JD_D_string:
      if( d->string && d->string[0] != '\0' ) return 1 ;
      else return 0 ;
    case JD_D_signal:
      if( d->dt && d->dt->data.signal.port ) return 1 ;
      else return 0 ;
    default:
      return 0 ;
  
  }
}

static void logical_ops(
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
)
{
  dd->type = JD_D_int ;
  switch( op ) {
    case JD_dbg_b_land_op:      /* expression '&&' expression  */
      if( JD_dbg_is_true( d1 ) && JD_dbg_is_true( d2 ) ) dd->int_value = 1 ;
      else dd->int_value = 0 ;
      break ;
    case JD_dbg_b_lor_op:       /* expression '||' expression  */
      if( JD_dbg_is_true( d1 ) || JD_dbg_is_true( d2 ) ) dd->int_value = 1 ;
      else dd->int_value = 0 ;
      break ;
  }
  
}

void JD_eval_bin_ops( 
  JD_dbg_op op,
  JD_dbg_expression_node *d1, JD_dbg_expression_node *d2, JD_dbg_expression_node *dd 
) {
  int code ;
  
  if( is_logic_op( op ) ) { /* &&, || */
    logical_ops( op, d1, d2, dd ) ;
    return ;
  }
  
  if( is_shift_op( op ) ) {
    shift_ops( op, d1, d2, dd ) ;
    return ;
  }
  
  code = data_type_code( d1, d2 ) ;
  
  switch( code ) {
    case INT_INT_CODE:
      eval_int_bin_ops( op, d1, d2, dd ) ;
      break ;
    case INT_BIT_CODE:
      int_to_bit( d1 ) ;
      eval_bit_bin_ops( op, d1, d2, dd ) ;
      break ;
    case INT_FLT_CODE:
      d1->real_value = d1->int_value ;
      eval_flt_bin_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_INT_CODE:
      int_to_bit( d2 ) ;
      eval_bit_bin_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_BIT_CODE:
      eval_bit_bin_ops( op, d1, d2, dd ) ;
      break ;
    case BIT_FLT_CODE:
      flt_to_bit( d2 ) ;
      eval_bit_bin_ops( op, d1, d2, dd ) ;
      break ;
    case FLT_INT_CODE:
      d2->type = JD_D_double ;
      d2->real_value = d2->int_value ;
      eval_flt_bin_ops( op, d1, d2, dd ) ;
      break ;
    case FLT_BIT_CODE:
      flt_to_bit( d1 ) ;
      eval_bit_bin_ops( op, d1, d2, dd ) ;
      break ;
    case FLT_FLT_CODE:
      eval_flt_bin_ops( op, d1, d2, dd ) ;
      break ;
    case OBJ_OBJ_CODE:
      eval_obj_bin_ops( op, d1, d2, dd ) ;
      break ;
    case STR_STR_CODE:
      eval_str_bin_ops( op, d1, d2, dd ) ;
      break ;
    case SIG_SIG_CODE:
      eval_sig_bin_ops( op, d1, d2, dd ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_DATA_TYPE_COMBINATION_FOR_BINARY_OPS ;
      break ;
  }
}

JD_eval_u_minus( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  copy_exp( dd, d1 ) ;
  
  switch( dd->type ) {
    case JD_D_int:
      dd->int_value = -dd->int_value ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      JD_u_minus_bit( dd->bit_a, dd->bit_b, dd->size ) ;
      break ;
    case JD_D_float:
    case JD_D_double:
      dd->real_value = -dd->real_value ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
}

JD_eval_u_tilde( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  copy_exp( dd, d1 ) ;
  
  switch( dd->type ) {
    case JD_D_int:
      dd->int_value = ~dd->int_value ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      JD_u_tilde_bit( dd->bit_a, dd->bit_b, dd->size ) ;
      break ;
    case JD_D_float:
    case JD_D_double:
      dd->real_value = -dd->real_value ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
}

JD_eval_u_not( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  switch( d1->type ) {
    case JD_D_int:
      dd->int_value = d1->int_value?0:1 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      dd->int_value = JD_u_not_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    case JD_D_float:
    case JD_D_double:
      if( d1->real_value == 0.0 )
        dd->int_value = 1 ;
      else
        dd->int_value = 0 ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->type = JD_D_int ;
}

JD_eval_u_and( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r ;
  switch( d1->type ) {
    case JD_D_int:
      r = (d1->int_value == -1)?1:0 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_and_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

JD_eval_u_or( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r ;
  switch( d1->type ) {
    case JD_D_int:
      r = (d1->int_value)?1:0 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_or_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

JD_eval_u_eor( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r, i ;
  switch( d1->type ) {
    case JD_D_int:
      r = 0 ;
      for( i = 0 ; i < 32 ; i++ )
        r = (r ^ ((d1->int_value>>i)&1)) & 1 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_eor_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

JD_eval_u_nand( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r ;
  switch( d1->type ) {
    case JD_D_int:
      r = (d1->int_value == -1)?0:1 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_nand_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

JD_eval_u_nor( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r ;
  switch( d1->type ) {
    case JD_D_int:
      r = (d1->int_value)?0:1 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_nor_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

JD_eval_u_neor( JD_dbg_expression_node *d1, JD_dbg_expression_node *dd )
{
  int r, i ;
  switch( d1->type ) {
    case JD_D_int:
      r = 0 ;
      for( i = 0 ; i < 32 ; i++ )
        r = (r ^ ((d1->int_value>>i)&1)) & 1 ;
      r = (~r)&1 ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      r = JD_u_neor_bit( d1->bit_a, d1->bit_b, d1->size ) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      break ;
  
  }
  dd->size = 1 ;
  dd->bit_a = (int *)calloc(1,sizeof(int)) ;
  dd->bit_b = (int *)calloc(1,sizeof(int)) ;
  dd->bit_a[0] = r & 1 ;
  dd->bit_b[0] = (r>>1) & 1 ;
  dd->type = JD_D_bit ;
}

void JD_eval_eqCeq_exp(
  JD_dbg_expression_node *d0,
  JD_dbg_expression_node *d1,
  JD_dbg_expression_node *d2,
  JD_dbg_expression_node *dd
)
{
  int err = 0 ;
  switch( d0->type ) {
    case JD_D_int:
      int_to_bit( d0 ) ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      break ;
    case JD_D_float:
    case JD_D_double:
      flt_to_bit(d0) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      err = 1 ;
      break ;
  }
  switch( d1->type ) {
    case JD_D_int:
      int_to_bit( d1 ) ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      break ;
    case JD_D_float:
    case JD_D_double:
      flt_to_bit(d1) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      err = 1 ;
      break ;
  }
  switch( d2->type ) {
    case JD_D_int:
      int_to_bit( d2 ) ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      break ;
    case JD_D_float:
    case JD_D_double:
      flt_to_bit(d2) ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_TYPE ;
      err = 1 ;
      break ;
  }
  if( !err ) {
    JD_bit_and_bit( 
      d0->bit_a, d0->bit_b, d0->size, d2->bit_a, d2->bit_b, d2->size
    ) ;
    JD_bit_and_bit( 
      d1->bit_a, d1->bit_b, d1->size, d2->bit_a, d2->bit_b, d2->size
    ) ;
    dd->int_value = 
      JD_bit_eqeq_bit( 
        d0->bit_a, d0->bit_b, d0->size, d1->bit_a, d1->bit_b, d1->size
      ) ;
  }
  else dd->int_value = 0 ;
  dd->type = JD_D_int ;
}

static void  copy_exp( JD_dbg_expression_node *dd, JD_dbg_expression_node *d )
{

  dd->type = d->type ;
  
  switch( d->type ) {
    case JD_D_int:
      dd->int_value = d->int_value ;
      dd->string = d->string ;
      break ;
    case JD_D_sbit:
    case JD_D_vbit:  /* bit in vstack */
    case JD_D_bit:
      copy_bit( d, dd ) ;
      break ;
    case JD_D_float:
    case JD_D_double:
      dd->real_value = d->real_value ;
      break ;
    case JD_D_signal:
    case JD_D_object:
      dd->dt = d->dt ;
      break ;
    case JD_D_string:
      dd->string = (char *)strdup(d->string) ;
      break ;
    default:
      break ;
  
  }
}

void JD_eval_cond_exp(
  JD_dbg_expression_node *d0,
  JD_dbg_expression_node *d1,
  JD_dbg_expression_node *d2,
  JD_dbg_expression_node *dd
)
{
  if( JD_dbg_is_true( d0 ) ) {
    copy_exp( dd, d1 ) ;
  }
  else {
    copy_exp( dd, d2 ) ;
  }
  
}

static void eval_single_port( JD_dbg_expression_node *exp ) 
{
  JD_pli_object *obj ;
  int i ;
  obj = (JD_pli_object *)exp->dt->data.signal.port ;
  if( exp->depth ) {
    if( exp->bit_index ) {
      if( exp->bit_index->etype == JD_dbg_exp_single_bit_range ) {
        JD_sample_port( 
          obj, exp->depth->int_value, 
          1, exp->bit_index->int_value, exp->bit_index->int_value
        ) ;
      }
      else {
        JD_sample_port( 
          obj, exp->depth->int_value, 
          1, exp->bit_index->int_value, exp->bit_index->next->int_value
        ) ;
      }
      JD_dbg_free_exp(exp->bit_index) ;
      exp->bit_index = NULL ;
    }
    else {
      JD_sample_port( obj, exp->depth->int_value, 0, 0, 0 ) ;
    }
    exp->type = JD_D_bit ;
    exp->bit_a = (int *)calloc(NWORD(ACC_SIZE), sizeof(int)) ;
    exp->bit_b = (int *)calloc(NWORD(ACC_SIZE), sizeof(int)) ;
    for( i = 0 ; i < NWORD(ACC_SIZE) ; i++ ) {
      exp->bit_a[i] = ACC_A[i] ;
      exp->bit_b[i] = ACC_B[i] ;
    }
    exp->size = ACC_SIZE ;
  }
  /*
  else {
    DBG_ERROR_ILLEGAL_SIGNAL_POINTER_ACCESS_FOR_EXPRESSION ;
    JD_dbg_free_exp(exp) ;
  }
  */
}

static void eval_array_index( JD_dbg_expression_node *exp ) {
  JD_data_unit *ut ;
  int hs, offset ;
  JD_hash_entry *hsh ;
  int i, j, ds ;
  JD_dbg_expression_node *index ;
  
  switch( exp->dt->sub_type ) {
    case JD_SD_array:
      offset = exp->array_index->int_value ;
      ut = &exp->dt->data.array.body[offset] ;
      exp->dt = ut->data ;
      break ;
    case JD_SD_assoc:
      offset = exp->array_index->int_value ;
      hs = CALC_DATA_HASH(offset) ;
      hsh = exp->dt->data.hash.table[hs] ;
      while(hsh) {
        if( hsh->idx == offset ) break ;
        if( hsh->idx > offset ) hsh = hsh->right ;
        else hsh = hsh->left ;
      }
      if( hsh ) {
        exp->dt = hsh->data ;
      }
      else {
        DBG_ERROR_EMPTY_ARRAY_ELEMENT( offset ) ;
        JD_dbg_free_exp(exp) ;
        return ;
      }
      break ;
    case JD_SD_marray:
      index = exp->array_index ;
      offset = 0 ;
      ds = exp->dt->data.array.dim_size[0] ;
      for( i = 0 ; i < ds ; i++ ) {
        hs = 1 ;
        for( j = ds ; j > 0 ; j-- ) {
          hs *= exp->dt->data.array.dim_size[j] ;
        }
        offset += (index->int_value * hs ) ;
        index = index->next ;
      }
      ut = &exp->dt->data.array.body[offset] ;
      exp->dt = ut->data ;
      break ;
    case JD_SD_strided:
      CALC_STRING_HASH(hs,exp->array_index->string) ;
      hsh = exp->dt->data.hash.table[hs] ;
      while( hsh ) {
        int t ;
        if( !(t = strcmp(exp->array_index->string, hsh->string)) ) break ;
        if( t > 0 ) hsh = hsh->right ;
        else hsh = hsh->left ;
      }
      if( hsh )  exp->dt = hsh->data ;
      else {
        DBG_ERROR_EMPTY_STRING_ARRAY_ELEMENT( exp->array_index->string ) ;
        JD_dbg_free_exp(exp) ;
        return ;
      }
      break ;
    case JD_SD_list:
      {
        JD_data_unit *ut, *at ;
        int n ;
        n = 0 ;
        at = NULL ;
        ut =  exp->dt->data.list.top ;
        offset = exp->array_index->int_value ;
        while( ut ) {
          if( n == offset ) at = ut ;
          n++ ;
          ut = ut->next ;
        }
        if( !at ) 
        {
          DBG_ERROR_EMPTY_LIST_ELEMENT( offset, n-1 ) ;
          JD_dbg_free_exp(exp) ;
          return ;
        }
        exp->dt = at->data ;
      }
      break ;
    default:
      DBG_ERROR_ILLEGAL_SUBTYPE_FOR_ARRAY_ACCESS ;
      JD_dbg_free_exp(exp) ;
      exp = NULL ;
      break ;
  }
  JD_dbg_free_exp(exp->array_index) ;
  exp->array_index = NULL ;
}

void JD_dbg_eval_exp( JD_dbg_expression_node *exp ) {
  int i, j, ub, lb, size ;
  int ls, us ;
  unsigned int mask, a, b ;
  unsigned int lmsk, umsk, uumsk ;
  
  switch( exp->type ) {
    case JD_D_signal:
      eval_single_port( exp ) ;
      break ;
    case JD_D_dt:
      if( exp->array_index ) {
        eval_array_index( exp ) ;
        if( JD_dbg_eval_error ) return ;
      }
      if( exp->dt->sub_type == JD_SD_single ) switch( exp->dt->type ) {
        case JD_D_sbit:
          exp->bit_a = (int *)calloc(1,sizeof(int) ) ;
          exp->bit_b = (int *)calloc(1,sizeof(int) ) ;
          exp->type = JD_D_bit ;
          exp->size = exp->dt->data.sbit.size ;
          if( exp->bit_index ) {
            if( exp->bit_index->etype == JD_dbg_exp_single_bit_range ) {
              ub = lb = exp->bit_index->int_value ;
            }
            else {
              ub = exp->bit_index->int_value ;
              lb = exp->bit_index->next->int_value ;
            }
            a = exp->dt->data.sbit.a ;
            b = exp->dt->data.sbit.b ;
            a >>= lb ;
            b >>= lb ;
            mask = (1 << (ub-lb+1))-1 ;
            a &= mask ;
            b &= mask ;
            exp->bit_a[0] = a ;
            exp->bit_b[0] = b ;
          }
          else {
            exp->bit_a[0] = exp->dt->data.sbit.a ;
            exp->bit_b[0] = exp->dt->data.sbit.b ;
          }
          exp->dt = NULL ;
          break ;
        case JD_D_bit:
          if( exp->bit_index ) {
            if( exp->bit_index->etype == JD_dbg_exp_single_bit_range ) {
              ub = lb = exp->bit_index->int_value ;
            }
            else {
              ub = exp->bit_index->int_value ;
              lb = exp->bit_index->next->int_value ;
            }
            size = ub-lb+1 ;
            exp->bit_a = 
              (int *)calloc(NWORD(size),sizeof(int) ) ;
            exp->bit_b = 
              (int *)calloc(NWORD(size),sizeof(int) ) ;
            i = LASTWORD(lb+1) ;
            ls = (lb%32) ;
            us = 32-ls ;
            lmsk = (unsigned int)0xffffffff >> ls ;
            uumsk = (1 << us )-1 ;
            if( uumsk ) uumsk = ~uumsk ;
            umsk = (size%32)?(1 << (size%32))-1:0xffffffff ;
            for( j = 0 ; j < LASTWORD(size) ; i++, j++ ) {
              exp->bit_a[j] = 
                ((exp->dt->data.bit.a[i]>>ls)&lmsk) |
                ((exp->dt->data.bit.a[i+1]<<us)&uumsk) ;
              exp->bit_b[j] = 
                ((exp->dt->data.bit.b[i]>>ls)&lmsk) |
                ((exp->dt->data.bit.b[i+1]<<us)&uumsk) ;
            }
            if( i == LASTWORD(exp->dt->data.bit.size) ) {
              a = b = 0 ;
            }
            else {
              a = exp->dt->data.bit.a[i+1] ;
              b = exp->dt->data.bit.b[i+1] ;
            }
            exp->bit_a[j] = 
              ( ((exp->dt->data.bit.a[i]>>ls)&lmsk) | ((a<<us)&uumsk) ) & umsk ;
            exp->bit_b[j] = 
              ( ((exp->dt->data.bit.b[i]>>ls)&lmsk) | ((b<<us)&uumsk) ) & umsk ;
            exp->type = JD_D_bit ;
            exp->size = size ;
            exp->dt = NULL ;
          }
          else {
            exp->bit_a = 
              (int *)calloc(NWORD(exp->dt->data.bit.size),sizeof(int) ) ;
            exp->bit_b = 
              (int *)calloc(NWORD(exp->dt->data.bit.size),sizeof(int) ) ;
            for( i = 0 ; i < NWORD(exp->dt->data.bit.size); i++ ) {
              exp->bit_a[i] = exp->dt->data.bit.a[i] ;
              exp->bit_b[i] = exp->dt->data.bit.b[i] ;
            }
            exp->type = JD_D_bit ;
            exp->size = exp->dt->data.sbit.size ;
            exp->dt = NULL ;
          }
          break ;
        case JD_D_int:
          exp->int_value = exp->dt->data.int_value ;
          exp->type = JD_D_int ;
          break ;
        case JD_D_float:
          exp->real_value = exp->dt->data.float_value ;
          exp->type = JD_D_float ;
          break ;
        case JD_D_double:
          exp->real_value = exp->dt->data.double_value ;
          exp->type = JD_D_float ;
          break ;
        case JD_D_string:
          if( exp->dt->data.string )
            exp->string = (char *)strdup(exp->dt->data.string) ;
          else
            exp->string = NULL ;
          exp->type = JD_D_string ;
        case JD_D_signal:
          eval_single_port( exp ) ;
          break ;
      }
      break ;
    case JD_D_parent:
      /*
      DBG_ERROR_ILLEGAL_NAME_FOR_EXPRESSION( exp->name ) ;
      JD_dbg_free_exp(exp) ;
      */
      break ;
    case JD_D_enum:
      DBG_ERROR_ILLEGAL_NAME_FOR_EXPRESSION( exp->name ) ;
      JD_dbg_free_exp(exp) ;
      break ;
  }
  
}

void JD_dbg_eval_exp_to_int( JD_dbg_expression_node *exp ) {
  int i ;
  
  JD_dbg_eval_exp( exp ) ;
  
  if( JD_dbg_eval_error ) return ;
  
  switch( exp->type ) {
    case JD_D_bit:
      for( i = 0 ; i < NWORD(exp->size) ; i++ ) {
        if( exp->bit_b[i] ) {
          DBG_ERROR_X_OR_Z_DETECTED_IN_BIT_WHILE_CONVERTING_TO_INT ;
          JD_dbg_free_exp(exp) ;
          exp = NULL ;
          return ;
        }
        if( i && exp->bit_a[i] ) {
          DBG_ERROR_OVERFLOW_DETECTED_IN_BIT_WHILE_CONVERTING_TO_INT ;
          JD_dbg_free_exp(exp) ;
          exp = NULL ;
          return ;
        }
      }
      exp->int_value = exp->bit_a[0] ;
      exp->type = JD_D_int ;
      free( exp->bit_a ) ;
      free( exp->bit_b ) ;
      exp->bit_a = NULL ;
      exp->bit_b = NULL ;
      break ;
    case JD_D_int:
      break ;
    case JD_D_float:
      exp->int_value = exp->real_value ;
      exp->type = JD_D_int ;
      break ;
    default:
      DBG_ERROR_ILLEGAL_DATA_TYPE_DETECTED_WHILE_CONVERTING_TO_INT ;
      JD_dbg_free_exp(exp) ;
      exp = NULL ;
      break ;
  }
}

static void set_bit(
  JD_data *dt, int *a, int *b, int bsize
) {
  int i, n ;
  int msk ;
  
  if (dt->data.bit.size > bsize) {
    n = LASTWORD(bsize) ;
    for( i = 0 ; i <=n ; i++ ) {
      dt->data.bit.a[i] = a[i] ;
      dt->data.bit.b[i] = b[i] ;
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
      dt->data.bit.a[i] = a[i] ;
      dt->data.bit.b[i] = b[i] ;
    }
    dt->data.bit.a[n] = a[n] & msk ;
    dt->data.bit.b[n] = b[n] & msk ;
  }

}

static void set_subbit(
  JD_data *dt, int ub, int lb, int *bit_a, int *bit_b, int bsize
) {
  unsigned int lmsk, umsk ;
  int size ;
  
  size = ub - lb + 1 ;
  if( dt->type == JD_D_sbit ) {
    lmsk = ((unsigned int)0xffffffff << lb ) & 
          ((unsigned int)0xffffffff >> (31-ub) );
    dt->data.sbit.a = (dt->data.sbit.a & ~lmsk) | ((bit_a[0]<<lb) & lmsk) ;
    dt->data.sbit.b = (dt->data.sbit.b & ~lmsk) | ((bit_b[0]<<lb) & lmsk) ;
  }
  else {
    int i, j, n, an, a, b, aa, ab, ls, us ;
    i = LASTWORD(lb+1) ;
    ls = (lb%32) ;
    us = 32-ls ;
    lmsk = (unsigned int)0xffffffff << ls ;
    umsk = (unsigned int)0xffffffff >> (31-(ub%32)) ;
    a = dt->data.bit.a[i] & ~lmsk ;
    b = dt->data.bit.b[i] & ~lmsk ;
    an = LASTWORD(bsize) ;
    j = 0 ;
    n = LASTWORD(ub+1) ;
    for( ; i < n ; i++ ) {
      aa = (j<=an)?bit_a[j]:0 ;
      ab = (j<=an)?bit_b[j]:0 ;
      dt->data.bit.a[i] = ((aa << ls) & lmsk) | a ;
      dt->data.bit.b[i] = ((ab << ls) & lmsk) | b ;
      a = (aa >> us) & ~lmsk ;
      b = (ab >> us) & ~lmsk ;
      j++ ;
    }
    aa = (j<=an)?bit_a[j]:0 ;
    ab = (j<=an)?bit_b[j]:0 ;
    dt->data.bit.a[i] = 
      (dt->data.bit.a[i] & ~umsk) | ((aa << ls) & lmsk & umsk) | a ;
    dt->data.bit.b[i] = 
      (dt->data.bit.b[i] & ~umsk)  | ((ab << ls) & lmsk & umsk) | b ;
  }
  
}

void JD_dbg_concat_bit( 
  int *bit_a, int *bit_b, int size, 
  int *bit_a1, int *bit_b1, int size1
) {
  JD_data d ;
  
  lshift_bit( (unsigned *)bit_a, NWORD(size), size1 ) ;
  lshift_bit( (unsigned *)bit_b, NWORD(size), size1 ) ;
  d.type = JD_D_bit ;
  d.data.bit.a = bit_a ;
  d.data.bit.b = bit_b ;
  d.data.bit.size = size ;
  
  set_subbit(
    &d, size1-1, 0, bit_a1, bit_b1, size1
  ) ;

}


void JD_dbg_assign_expression( 
  JD_dbg_expression_node *left, JD_dbg_expression_node *exp 
) {
  int i, err ;
  double sft, flt ;
  int msk, n ;
  
  JD_dbg_eval_exp(exp) ;
  
  if( left->array_index )
    eval_array_index( left ) ;
    
  if( JD_dbg_eval_error ) return ;
  
  while( left->dt->type == JD_D_pointer ) {
    left->dt = (JD_data *)JD_get_ptr_element(left->dt) ;
  }
  
  if( left->dt->sub_type != JD_SD_single ) {
    DGB_ERROR_ARRAY_ASSIGNMENT_IS_NOT_SUPPORTED ;
  } else {
    switch( left->dt->type ) {
      case JD_D_int:
        switch( exp->type ) {
          case JD_D_int:
            left->dt->data.int_value = exp->int_value ;
            break ;
          case JD_D_bit:
            err = 0 ;
            for( i = 0 ; i < NWORD(exp->size) ; i++ ) {
              if ( exp->bit_b[i] ) {
                DGB_ERROR_X_DETECTED_IN_INT_ASSIGNMENT ;
                i = exp->size ; 
              }
              else if ( i && exp->bit_a[i] ) {
                DGB_ERROR_OVERFLOW_DETECTED_IN_INT_ASSIGNMENT ;
                i = exp->size ; 
              }
            }
            if( !JD_dbg_eval_error ) 
              left->dt->data.int_value = exp->bit_a[0] ;
            break ;
          case JD_D_float:
            left->dt->data.int_value = exp->real_value ;
            break ;
          default:
            DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
            break ;
        }
        break ;
      case JD_D_bit:
        if( left->bit_index == NULL ) {
          switch( exp->type ) {
            case JD_D_int:
              msk = (left->dt->data.bit.size > 32)?
                      0xffffffff :
                      BITMASK(left->dt->data.bit.size) ;
              left->dt->data.bit.a[0] = exp->int_value & msk ;
              left->dt->data.bit.b[0] = 0 ;
              for( i = 1 ; i < NWORD(left->dt->data.bit.size) ; i++ ) {
                left->dt->data.bit.a[i] = 0 ;
                left->dt->data.bit.b[i] = 0 ;
              }
              break ;
            case JD_D_bit:
              set_bit( left->dt, exp->bit_a, exp->bit_b, exp->size ) ;
              break ;
            default:
              DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
              break ;
          }
        }
        else { /* left->bit_index  */
          int bit0 = 0 ;
          int ub, lb ;
          if( left->bit_index->etype == JD_dbg_exp_bit_range ) {
            ub = left->bit_index->int_value ;
            lb = left->bit_index->next->int_value ;
            
          }
          else {
            ub = lb = left->bit_index->int_value ;
          }
          switch( exp->type ) {
            case JD_D_int:
              set_subbit(
                left->dt, ub, lb, &exp->int_value, &bit0, 32
              ) ;
              break ;
            case JD_D_bit:
              set_subbit(
                left->dt, ub, lb, exp->bit_a, exp->bit_b, exp->size
              ) ;
              break ;
            default:
              DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
              break ;
          }
        }
        break ;
      case JD_D_sbit:
        if( left->bit_index == NULL ) {
          switch( exp->type ) {
            case JD_D_int:
              msk = BITMASK(left->dt->data.sbit.size) ;
              left->dt->data.sbit.a = exp->int_value & msk ;
              left->dt->data.sbit.b = 0 ;
              break ;
            case JD_D_bit:
              msk = BITMASK(left->dt->data.sbit.size) ;
              left->dt->data.sbit.a = exp->bit_a[0] & msk ;
              left->dt->data.sbit.b = exp->bit_b[0] & msk ;
              break ;
            default:
              DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
              break ;
          }
        }
        else {
          int bit0 = 0 ;
          int ub, lb ;
          if( left->bit_index->etype == JD_dbg_exp_bit_range ) {
            ub = left->bit_index->int_value ;
            lb = left->bit_index->next->int_value ;
            
          }
          else {
            ub = lb = left->bit_index->int_value ;
          }
          switch( exp->type ) {
            case JD_D_int:
              set_subbit(
                left->dt, ub, lb, &exp->int_value, &bit0, 32
              ) ;
              break ;
            case JD_D_bit:
              set_subbit(
                left->dt, ub, lb, exp->bit_a, exp->bit_b, exp->size
              ) ;
              break ;
            default:
              DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
              break ;
          }
        }
        break ;
      case JD_D_float:
        switch( exp->type ) {
          case JD_D_int:
            left->dt->data.float_value = exp->int_value ;
            break ;
          case JD_D_bit:
            bit_to_flt( exp ) ;
            left->dt->data.float_value = exp->real_value ;
            break ;
          case JD_D_float:
            left->dt->data.float_value = exp->real_value ;
            break ;
          default:
            DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
            break ;
        }
        break ;
      case JD_D_double:
        switch( exp->type ) {
          case JD_D_int:
            left->dt->data.double_value = exp->int_value ;
            break ;
          case JD_D_bit:
            bit_to_flt( exp ) ;
            left->dt->data.double_value = exp->real_value ;
            break ;
          case JD_D_float:
            left->dt->data.double_value = exp->real_value ;
            break ;
          default:
            DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
            break ;
            break ;
        }
        break ;
      case JD_D_object:
        if( exp->type == JD_D_null ) {
          if( left->dt->data.obj.ect && 
              (--left->dt->data.obj.ect->ref_count) == 0 ) 
          {
            (left->dt->data.obj.ect->dealloc)( 
              (void *)left->dt->data.obj.ect->table,
              left->dt->data.obj.ect->num_entry,
              left->dt->data.obj.ect->tbl_size
            ) ;
            DEALLOC_OBJ_TABLE(left->dt->data.obj.ect) ;
          }
          left->dt->data.obj.ect = NULL ;
        }
        else if( exp->type == JD_D_dt ) {
          if( exp->dt->type != JD_D_object || 
              left->dt->data.obj.alloc != exp->dt->data.obj.alloc ) 
          {
            DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
          }
          else {
            if( left->dt->data.obj.ect && 
                (--left->dt->data.obj.ect->ref_count) == 0 ) 
            {
              (left->dt->data.obj.ect->dealloc)( 
                (void *)left->dt->data.obj.ect->table,
                left->dt->data.obj.ect->num_entry,
                left->dt->data.obj.ect->tbl_size
              ) ;
              DEALLOC_OBJ_TABLE(left->dt->data.obj.ect) ;
            }
            left->dt->data.obj.ect = exp->dt->data.obj.ect ;
            if( left->dt->data.obj.ect ) left->dt->data.obj.ect->ref_count++ ;
          }
        }
        else {
          DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
        }
        break ;
      case JD_D_string:
        if( exp->type != JD_D_string ) {
          DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
        }
        else {
          if( left->dt->data.string ) DEALLOC_STRING( left->dt->data.string ) ;
          if( exp->string ) 
            left->dt->data.string = (char *)strdup( exp->string ) ;
          else
            left->dt->data.string = NULL ;
        }
        break ;
      case JD_D_signal:
        if( exp->type != JD_D_dt || exp->dt->type != JD_D_signal ||
            exp->depth
        ) {
          DBG_ERROR_DATA_TYPE_MISMATCH_ON_ASSIGNMENT ;
        }
        else if (left->scope && left->scope->type == JD_dbg_port_type )
        {
          DBG_ERROR_PORT_CAN_NOT_BE_REASSIGENED ;
        }
        else {
          left->dt->data.signal.port = exp->dt->data.signal.port ;
          left->dt->data.signal.data0 = exp->dt->data.signal.data0 ;
          left->dt->data.signal.data1 = exp->dt->data.signal.data1 ;
        }
        break ;
    }
  }
}

