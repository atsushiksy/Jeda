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
#include "runtime_error_msg.h"
#include "mem_macro.h"
#include "data.h"
#include "exec_macro.h"
#include "code_exec_macro.h"


/* string springf( fmt, vlaues.. ) */
void JD_sf_sprintf()
{
  JD_data *fmt_dt ;
  
  fmt_dt = STACK[SP - NUM_ARG] ;
  if( ACC_STRING ) {
    DEALLOC_STRING(ACC_STRING) ;
  }
  JD_sys_fprintf (
    1, &ACC_STRING, NULL, fmt_dt->data.string, NUM_ARG
  ) ;

  ACC_TYPE = JD_D_string ;
  
}

/* strlen( string ) */
void JD_sf_strlen()
{
  JD_data *dt ;

  dt = STACK[SP - NUM_ARG] ;
  if( dt->data.string ) {
    ACC_INT = strlen(dt->data.string) ;
  }
  else ACC_INT = 0 ;
  ACC_TYPE = JD_D_int ;
}

/* substring( string, int start, [int end] ) */
void JD_sf_substring()
{
  JD_data *dt ;
  char *org ;
  int i, strt, endd ;
  
  if( NUM_ARG == 3 ) {
    dt = STACK[SP - 1] ;
    if( IS_SMLINT(dt) ) endd = SMLINT2INT(dt) ;
    else endd = dt->data.int_value ;
    dt = STACK[SP - 2] ;
    if( IS_SMLINT(dt) ) strt = SMLINT2INT(dt) ;
    else strt = dt->data.int_value ;
    dt = STACK[SP - 3] ;
    org = dt->data.string ;
  }
  else {
    dt = STACK[SP - 1] ;
    if( IS_SMLINT(dt) ) strt = SMLINT2INT(dt) ;
    else strt = dt->data.int_value ;
    dt = STACK[SP - 2] ;
    org = dt->data.string ;
    if( org ) endd = strlen(org) ;
    else endd = 0 ;
  }
  if( org && (endd - strt + 1 > 0) ) {
    ACC_STRING = ALLOC_STRING(endd-strt+1) ;
    i = 0 ;
    while( strt <= endd ) {
      ACC_STRING[i++] = org[strt++] ;
    }
    ACC_STRING[i] = 0 ;
  }
  ACC_TYPE = JD_D_string ;
}

/* int atoi( string ) */
void JD_sf_atoi()
{
  JD_data *dt ;
  char *sp, *p ;
  int minus = 0 ;
  dt = STACK[SP - 1] ;
  sp = dt->data.string ;
  ACC_INT = 0 ;
  if( sp ) {
    if( *sp == '-' ){ minus = 1 ; sp++ ; }
    if( p = (char *)strstr(sp, "\'d")  ){ sp = p+2 ;}
    else if( p = (char *)strstr(sp, "\'D") ){ sp = p+2 ;}
    while( *sp ) {
      if( *sp == '_' ) sp++ ;
      else if( *sp < '0' || *sp > '9' ) break ;
      else {
        ACC_INT = (ACC_INT*10) + (*sp - '0') ;
        sp++ ;
      }
    }
  }
  if( minus ) ACC_INT = -ACC_INT ;
  ACC_TYPE = JD_D_int ;
}

static int is_bin_char( char x ) {
  if( x == 'X' ) return 1 ;
  if( x == 'x' ) return 1 ;
  if( x == 'z' ) return 1 ;
  if( x == 'Z' ) return 1 ;
  if( x == '0' ) return 1 ;
  if( x == '1' ) return 1 ;
  return 0 ;
}

static int is_oct_char( char x ) {
  switch( x ) {
    case 'X':
    case 'x':
    case 'Z':
    case 'z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      return 1 ;
    default:
      return 0 ;
  }
}

static int is_hex_char( char x ) {
  switch( x ) {
    case 'X':
    case 'x':
    case 'Z':
    case 'z':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
      return 1 ;
    default:
      return 0 ;
  }
}


static int get_bita( char x ) {
  switch( x ) {
    case 'X':
    case 'x':
      return 0xf ; 
    case 'Z':
    case 'z':
      return 0x0 ; 
    case '0':
      return 0x0 ; 
    case '1':
      return 0x1 ; 
    case '2':
      return 0x2 ; 
    case '3':
      return 0x3 ; 
    case '4':
      return 0x4 ; 
    case '5':
      return 0x5 ; 
    case '6':
      return 0x6 ; 
    case '7':
      return 0x7 ; 
    case '8':
      return 0x8 ; 
    case '9':
      return 0x9 ; 
    case 'a':
    case 'A':
      return 0xa ; 
    case 'b':
    case 'B':
      return 0xb ; 
    case 'c':
    case 'C':
      return 0xc ; 
    case 'd':
    case 'D':
      return 0xd ; 
    case 'e':
    case 'E':
      return 0xe ; 
    case 'f':
    case 'F':
      return 0xf ; 
    default:
      return 0 ;
  }
}

static int get_bitb( char x ) {
  switch( x ) {
    case 'X':
    case 'x':
      return 0xf ; 
    case 'Z':
    case 'z':
      return 0xf ; 
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    default:
      return 0 ;
  }
}

static void lshift_acc( int shift ) {
  int sft, rsft, idx ;
  int i, j, n, a, b, aa, bb, mm ;

  n = LASTWORD(ACC_SIZE) ;
  sft = shift % 32 ;
  rsft = sft?(32-sft):0 ;
  idx = shift / 32 ;
  aa = bb = 0 ;
  j = n - idx ;
  mm = (unsigned)0xffffffff>>((32-(ACC_SIZE%32))%32) ;
  for( i = n ; i >= 0 ; i-- ) {
    if( j >= 0 ) {
      a = ACC_A[j] << sft ;
      b = ACC_B[j] << sft ;
      if( j-1 >= 0 ) {
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

/* int abintobit( string ) */
void JD_sf_abintobit()
{
  JD_data *dt ;
  char *sp, *p ;
  int i, n ;
  dt = STACK[SP - 1] ;
  sp = dt->data.string ;
  ACC_INT = 0 ;
  if( sp ) {
    if( p = (char *)strstr(sp, "\'b")  ){ sp = p+2 ;}
    else if( p = (char *)strstr(sp, "\'B") ){ sp = p+2 ;}
    n = 0 ;
    p = sp ;
    /* counting how many bit we need */
    while( *p ) {
      if( *p == '_' ) p++ ;
      else if( !is_bin_char(*p) ) break ;
      else {
        n++ ;
        p++ ;
      }
    }
    if( ACC_WD <= LASTWORD(n) ) {
      /* extending ACC bit space */
      ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(n) ) ;
      ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(n) ) ;
      ACC_WD = LASTWORD(n)+1 ;
    }
    ACC_SIZE = n ;
    n = NWORD(n) ;
    for( i = 0 ; i < n ; i++ ) {
      ACC_A[i] = ACC_B[i] = 0 ;
    }
    while( *sp ) {
      if( *sp == '_' ) sp++ ;
      else if( !is_bin_char( *sp) ) break ;
      else {
        lshift_acc(1) ;
        ACC_A[0] |= (get_bita(*sp) & 1) ;
        ACC_B[0] |= (get_bitb(*sp) & 1) ;
        sp++ ;
      }
    }
    DEALLOC_STRING(ACC_STRING) ;
  }
  if( ACC_SIZE <= 32 ) ACC_TYPE = JD_D_sbit ;
  else ACC_TYPE = JD_D_bit ;
}

/* int aocttobit( string ) */
void JD_sf_aocttobit()
{
  JD_data *dt ;
  char *sp, *p ;
  int i, n ;
  dt = STACK[SP - 1] ;
  sp = dt->data.string ;
  if( sp ) {
    if( p = (char *)strstr(sp, "\'o")  ){ sp = p+2 ;}
    else if( p = (char *)strstr(sp, "\'O") ){ sp = p+2 ;}
    n = 0 ;
    p = sp ;
    /* counting how many bit we need */
    while( *p ) {
      if( *p == '_' ) p++ ;
      else if( !is_oct_char(*p) ) break ;
      else {
        n+=3 ;
        p++ ;
      }
    }
    if( ACC_WD <= LASTWORD(n) ) {
      /* extending ACC bit space */
      ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(n) ) ;
      ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(n) ) ;
      ACC_WD = LASTWORD(n)+1 ;
    }
    ACC_SIZE = n ;
    n = NWORD(n) ;
    for( i = 0 ; i < n ; i++ ) {
      ACC_A[i] = ACC_B[i] = 0 ;
    }
    while( *sp ) {
      if( *sp == '_' ) sp++ ;
      else if( !is_oct_char( *sp) ) break ;
      else {
        lshift_acc(3) ;
        ACC_A[0] |= (get_bita(*sp) & 7) ;
        ACC_B[0] |= (get_bitb(*sp) & 7) ;
        sp++ ;
      }
    }
    DEALLOC_STRING(ACC_STRING) ;
  }
  if( ACC_SIZE <= 32 ) ACC_TYPE = JD_D_sbit ;
  else ACC_TYPE = JD_D_bit ;
}

/* int ahextobit( string ) */
void JD_sf_ahextobit()
{
  JD_data *dt ;
  char *sp, *p ;
  int i, n ;
  dt = STACK[SP - 1] ;
  sp = dt->data.string ;
  ACC_INT = 0 ;
  if( sp ) {
    if( p = (char *)strstr(sp, "\'h")  ){ sp = p+2 ;}
    else if( p = (char *)strstr(sp, "\'H") ){ sp = p+2 ;}
    n = 0 ;
    p = sp ;
    /* counting how many bit we need */
    while( *p ) {
      if( *p == '_' ) p++ ;
      else if( !is_hex_char(*p) ) break ;
      else {
        n+=4 ;
        p++ ;
      }
    }
    if( ACC_WD <= LASTWORD(n) ) {
      /* extending ACC bit space */
      ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(n) ) ;
      ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(n) ) ;
      ACC_WD = LASTWORD(n)+1 ;
    }
    ACC_SIZE = n ;
    n = NWORD(n) ;
    for( i = 0 ; i < n ; i++ ) {
      ACC_A[i] = ACC_B[i] = 0 ;
    }
    while( *sp ) {
      if( *sp == '_' ) sp++ ;
      else if( !is_hex_char( *sp) ) break ;
      else {
        lshift_acc(4) ;
        ACC_A[0] |= get_bita(*sp) ;
        ACC_B[0] |= get_bitb(*sp) ;
        sp++ ;
      }
    }
    DEALLOC_STRING(ACC_STRING) ;
  }
  if( ACC_SIZE <= 32 ) ACC_TYPE = JD_D_sbit ;
  else ACC_TYPE = JD_D_bit ;
}

