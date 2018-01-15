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
#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"

static int compile_pass = 2 ;

/* Cut and paste from parse_tree.c */

/***************************************************************
 *  Constant handling functions
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

static void scan_decimal( int size, char *number, expression_node *exp )
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
          if( !ovflow && compile_pass==2 ) {
            ERROR_LOCATION(exp->filename, exp->linenum) ;
            WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
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
  
  if( size == 32 ) {
    exp->type = JD_int_num ;
    exp->is.int_value = bit_a[0] ;
    free( bit_a ) ;
    free( bit_b ) ;
    free( bit_c ) ;
  }
  else {
    exp->type = JD_bit_num ;
    exp->is.bit_const.size = size ;
    exp->is.bit_const.a = (int *)bit_a ;
    for( i = 0 ; i < glen ; i++ ) bit_b[i] = 0 ;
    exp->is.bit_const.b = (int *)bit_b ;
    free( bit_c ) ;
  }


}
void scan_v_decimal(  char *number, expression_node *exp )
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
      ERROR_LOCATION(exp->filename, exp->linenum) ;
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;
  
}


void scan_c_decimal( char *number, expression_node *exp )
{
  char *cp = number ;
  int  size ;
  
  size = 32 ;
    
  cp++ ; /* skip d/D */
  
  scan_decimal( size, cp, exp ) ;

}

static void scan_hex( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-4, 4) || get_bit(bit_b, size-4, 4) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
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
          break ;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
          bit_a[0] |= (*cp-'a'+10) ;
          break ;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          bit_a[0] |= (*cp-'A'+10) ;
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0xf ;
          bit_b[0] |= 0xf ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0xf ;
          break ;
      }
    }
    cp++ ;
  }

  exp->type = JD_bit_num ;
  exp->is.bit_const.size = size ;
  exp->is.bit_const.a = (int *)bit_a ;
  exp->is.bit_const.b = (int *)bit_b ;
  
}

void scan_v_hex(  char *number, expression_node *exp )
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
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip h/H */
  
  scan_hex( size, cp, exp ) ;

}

void scan_c_hex(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip x/X */
  
  scan_hex( size, cp, exp ) ;

}

static void scan_octal( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-3, 3) || get_bit(bit_b, size-3, 3) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
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
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x7 ;
          bit_b[0] |= 0x7 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x7 ;
          break ;
      }
    }
    cp++ ;
  }

  exp->type = JD_bit_num ;
  exp->is.bit_const.size = size ;
  exp->is.bit_const.a = (int *)bit_a ;
  exp->is.bit_const.b = (int *)bit_b ;
  
}


void scan_v_octal(  char *number, expression_node *exp )
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
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

void scan_c_octal(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_octal( size, cp, exp ) ;

}

static void scan_binary( int size, char *number, expression_node *exp )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-1, 1) || get_bit(bit_b, size-1, 1) ) {
        if( !ovflow && compile_pass==2 ) {
          ERROR_LOCATION(exp->filename, exp->linenum) ;
          WARNING_DECIMAL_CONSTANT_OVERFLOW_EXTENDING_SIZE ;
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
          break ;
        case 'x': case 'X':
          bit_a[0] |= 0x1 ;
          bit_b[0] |= 0x1 ;
          break ;
        case 'z': case 'Z':
          bit_b[0] |= 0x1 ;
          break ;
      }
    }
    cp++ ;
  }

  exp->type = JD_bit_num ;
  exp->is.bit_const.size = size ;
  exp->is.bit_const.a = (int *)bit_a ;
  exp->is.bit_const.b = (int *)bit_b ;
  
}


void scan_v_binary(  char *number, expression_node *exp )
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
      ERROR_ILLEGAL_NUMBER_REPRESENTATION ;
      return ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

void scan_c_binary(  char *number, expression_node *exp )
{
  char *cp = number ;
  int size ;
  
  size = 32 ;
  
  cp++ ; /* skip 0 */
  cp++ ; /* skip o/O */
  
  scan_binary( size, cp, exp ) ;

}

