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
#include <stdlib.h>
#include <string.h>

#include "../include/runtime.h"
#include "../include/exec_macro.h"
#include "../include/runtime_error_msg.h"

int JD_check_bit_set( int *b, int size ) ;
double JD_bit_2_real( int *a, int size ) ;
JD_data *JD_get_ptr_element( JD_data *dt ) ;

/* value from floatio.h in gnu/libstdc++-2.8.0/libio */

/* 11-bit exponent (VAX G floating point) is 308 decimal digits */
#define	MAXEXP		308
/* 128 bit fraction takes up 39 decimal digits; max reasonable precision */

#define	MAXFRACT	39
#define	BUF		(MAXEXP+MAXFRACT+1)	/* + decimal point */
#define	DEFPREC		6

#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
 * Flags used during conversion.
 */
#define	LONGINT		0x01		/* long integer */
#define	LONGDBL		0x02		/* long double; unimplemented */
#define	SHORTINT	0x04		/* short integer */
#define	ALT		0x08		/* alternate form */
#define	LADJUST		0x10		/* left adjustment */
#define	ZEROPAD		0x20		/* zero (as opposed to blank) pad */
#define	HEXPREFIX	0x40		/* add 0x or 0X prefix */

#define MIN_STR_BUFF 80

static int str_size ;
static int str_ptr ;
static int arg_base ;
static int arg_ptr ;

#define MIN_TMP_BUFF 1024
static char *tbuf = NULL ;
static int  tbuf_size = 0 ;
static char *tbuf_ptr ;

static char *float_buf = NULL ;
static char *fmt_buf = NULL ;

static void clear_tbuf() {
  if( tbuf == NULL ) {
    tbuf = (char *)calloc( MIN_TMP_BUFF, sizeof(char) ) ;
    tbuf_size = MIN_TMP_BUFF ;
  }
  tbuf[tbuf_size-1] = '\0' ;
  tbuf_ptr = &tbuf[tbuf_size-2] ;
}

static void put_tbuf( char ch ) {
  if( tbuf == tbuf_ptr ) {
    int i ;
    char *tmp, *dp, *sp ;
    tmp = (char *)calloc( tbuf_size + MIN_TMP_BUFF, sizeof(char) ) ;
    sp = &tbuf[tbuf_size-1] ;
    dp = &tmp[tbuf_size + MIN_TMP_BUFF - 1] ;
    for( i = 0 ; i < tbuf_size ; i++ ) *dp-- = *sp-- ;
    free(tbuf) ;
    tbuf = tmp ;
    tbuf_ptr = dp ;
    tbuf_size += MIN_TMP_BUFF ;
  }
  *tbuf_ptr = ch ;
  tbuf_ptr-- ;
}

static void zero_suppress_tbuf() {
  while( tbuf_ptr != &tbuf[tbuf_size-3] && tbuf_ptr[1] == '0' ) tbuf_ptr++ ;
}

static void PRINTCHAR( int iotype, char **str, FILE *fp, int ch ) {
  char *buff, *src ;
  int i ;
  switch( iotype ) {
    case 0: /* printout to stdout */
      PRINTF( "%c", ch ) ;
      break ;
    case 1: /* print out to string */
      if( str_ptr >= str_size ) {
        str_size += MIN_STR_BUFF ;
        buff = (char *)calloc( str_size, sizeof(char) ) ;
        src = *str ;
        for( i = 0 ; i < str_ptr ; i++ ) buff[i] = src[i] ;
        *str = buff ;
        if( str_ptr ) free(src) ;
      }
      src = *str ;
      src[str_ptr++] = ch ;
      break ;
    case 2: /* print out to a file */
      putc( ch, fp ) ;
      break ;
    case 3: /* print out to stderr */
      fprintf( stderr, "%c", ch ) ;
      break ;
  }
}

static void PAD_SP( int iotype, char **str, FILE *fp, int howmany ) {
  while(howmany) {
    PRINTCHAR( iotype,str,fp, ' ' );
    howmany-- ;
  }
}

static void PAD_0( int iotype, char **str, FILE *fp, int howmany ) {
  while(howmany) {
    PRINTCHAR( iotype,str,fp, '0' );
    howmany-- ;
  }
}

static void PRINTOUT( int iotype, char **str, FILE *fp, char *txt, int num )
{
  int i, size ;
  char *buff, *src, cc ;
  switch( iotype ) {
    case 0: /* printout to stdout */
      if( num < 0 ) PRINTF( "%s", txt ) ;
      else {
        for( i = 0 ; i < num ; i++ ) {
          cc = *txt++ ;
          if( cc == '\\' ) {
            cc = *txt++ ;
            i++ ;
            switch(cc) {
              case 'n': PRINTF( "\n" ) ; break ;
              case 't': PRINTF( "\t" ) ; break ;
              case 'b': PRINTF( "\b" ) ; break ;
              case 'v': PRINTF( "\v" ) ; break ;
              case 'f': PRINTF( "\f" ) ; break ;
              case 'r': PRINTF( "\r" ) ; break ;
              default : PRINTF( "%c", cc ) ; break ;
            }
          }
          else PRINTF( "%c", cc ) ;
        }
      }
      break ;
    case 1: /* print out to string */
      if( num < 0 ) size = strlen(txt) ;
      else size = num ;
      if( str_ptr + size + 1 >= str_size ) {
        while( str_ptr + size + 1 >= str_size ) str_size += MIN_STR_BUFF ;
        buff = (char *)calloc( str_size, sizeof(char) ) ;
        src = *str ;
        for( i = 0 ; i < str_ptr ; i++ ) buff[i] = src[i] ;
        *str = buff ;
        if( str_ptr ) free(src) ;
      }
      src = *str ;
      if( num < 0 ) sprintf( src, "%s", txt ) ;
      else {
        for( i = 0 ; i < num ; i++ ) {
          cc = txt[i] ;
          if( cc == '\\' ) {
            i++ ;
            cc = txt[i] ;
            switch(cc) {
              case 'n': src[str_ptr++] = '\n' ; break ;
              case 't': src[str_ptr++] = '\t' ; break ;
              case 'b': src[str_ptr++] = '\b' ; break ;
              case 'v': src[str_ptr++] = '\v' ; break ;
              case 'f': src[str_ptr++] = '\f' ; break ;
              case 'r': src[str_ptr++] = '\r' ; break ;
              default : src[str_ptr++] = cc ; break ;
            }
          }
          else src[str_ptr++] = cc ;
        }
      }
      break ;
    case 2: /* print out to a file */
      if( num < 0 ) fprintf( fp, "%s", txt ) ;
      else {
        for( i = 0 ; i < num ; i++ ) {
          cc = txt[i] ;
          if( cc == '\\' ) {
            i++ ;
            cc = txt[i] ;
            switch(cc) {
              case 'n': fprintf( fp, "\n" ) ; break ;
              case 't': fprintf( fp, "\t" ) ; break ;
              case 'b': fprintf( fp, "\b" ) ; break ;
              case 'v': fprintf( fp, "\v" ) ; break ;
              case 'f': fprintf( fp, "\f" ) ; break ;
              case 'r': fprintf( fp, "\r" ) ; break ;
              default :  fprintf( fp, "%c", cc  ) ; break ;
            }
          }
          else fprintf( fp, "%c", cc ) ;
        }
      }
      break ;
    case 3: /* print out to stderr */
      if( num < 0 ) fprintf( stderr, "%s", txt ) ;
      else {
        for( i = 0 ; i < num ; i++ ) {
          cc = txt[i] ;
          if( cc == '\\' ) {
            i++ ;
            cc = txt[i] ;
            switch(cc) {
              case 'n': fprintf( stderr, "%c", '\n' ) ; break ;
              case 't': fprintf( stderr, "%c", '\t' ) ; break ;
              case 'b': fprintf( stderr, "%c", '\b' ) ; break ;
              case 'v': fprintf( stderr, "%c", '\v' ) ; break ;
              case 'f': fprintf( stderr, "%c", '\f' ) ; break ;
              case 'r': fprintf( stderr, "%c", '\r' ) ; break ;
              default : fprintf( stderr, "%c",  cc  ) ; break ;
            }
          }
          else fprintf( stderr, "%c", cc ) ;
        }
      }
      break ;
  }

}

/* reading JD_data value from arg list */
static JD_data *va_arg_jd() {
  JD_data *ret ;
  
  if( arg_ptr == arg_base ) return NULL ;
  ret = STACK[SP - arg_base + arg_ptr] ;
  arg_ptr++ ;
  return ret ;
}

/* reading integer value from arg list, corresponds to va_arg(ap,int) */
static int va_arg_int() {
  JD_data *dt ;
  int i, n ;
  dt = va_arg_jd() ;
  if( dt ) {
    if( IS_SMLINT(dt) ) {
      return SMLINT2INT(dt) ;
    }
    else if( dt->sub_type == JD_SD_single ) {
      switch( dt->type ) {
        case JD_D_int: 
          return dt->data.int_value ;
          break ;
        case JD_D_sbit: 
          if( dt->data.sbit.b ) return 0 ;
          else return dt->data.sbit.a ;
          break ;
        case JD_D_bit: 
        case JD_D_vbit: 
          n = LASTWORD(dt->data.bit.size) ;
          for( i = 0 ; i <= n ; i++ ) {
            if( dt->data.bit.b[i] ) return 0 ;
            if( i && dt->data.bit.a[i] ) return 0 ;
          }
          return dt->data.bit.a[0] ;
          break ;
        case JD_D_float:
          i = dt->data.float_value ;
          return i ;
          break ;
        case JD_D_double:
          i = dt->data.double_value ;
          return i ;
          break ;
        case JD_D_string:
          i = dt->data.string[0] ;
          return i ;
          break ;
        default:
          return 0 ;
      }
    }
    else return 0 ;
  }
  else return 0 ;
}

/* reading double value from arg list, corresponds to va_arg(ap,double) */
static double va_arg_double() {
  double ret ;
  JD_data *dt ;
  int i, n ;
  dt = va_arg_jd() ;
  if( dt ) {
    if( IS_SMLINT(dt) ) {
      ret = SMLINT2INT(dt) ;
    }
    else if( dt->sub_type == JD_SD_single ) {
      switch( dt->type ) {
        case JD_D_int: 
          ret = dt->data.int_value ;
          break ;
        case JD_D_sbit: 
          if( dt->data.sbit.b ) ret = 0 ;
          else ret = dt->data.sbit.a ;
          break ;
        case JD_D_bit: 
        case JD_D_vbit: 
          if( JD_check_bit_set(dt->data.bit.b, dt->data.bit.size) ) ret = 0.0 ;
          else ret = JD_bit_2_real(dt->data.bit.a, dt->data.bit.size) ;
          break ;
        case JD_D_float:
          ret = dt->data.float_value ;
          break ;
        case JD_D_double:
          ret = dt->data.double_value ;
          break ;
        case JD_D_string:
          ret = dt->data.string[0] ;
          break ;
        default:
          ret = 0 ;
      }
    }
    else ret = 0 ;
  }
  else ret = 0 ;
  return ret ;
}

/* reading pointer value from arg list, corresponds to va_arg(ap, void *) */
static JD_data *va_arg_ptr() {

  JD_data *dt = va_arg_jd() ;
  if( IS_SMLINT(dt) ) return NULL ;
  while( dt && dt->type == JD_D_pointer ) {
    dt = JD_get_ptr_element(dt) ;
  }
  return dt ;
}

/* reading string value from arg list, corresponds to va_arg(ap, char *) */
static char *va_arg_string() {
  char *ret ;
  JD_data *dt ;
  int i, n ;
  dt = va_arg_jd() ;
  if( dt ) {
    if( IS_SMLINT(dt) ) return "<int_value>" ;
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
    if( dt->sub_type == JD_SD_single ) {
      switch( dt->type ) {
        case JD_D_int: 
          ret = "<int_value>" ;
          break ;
        case JD_D_sbit: 
          ret = "<bit_value>" ;
          break ;
        case JD_D_bit: 
        case JD_D_vbit: 
          ret = "<bit_value>" ;
          break ;
        case JD_D_float:
          ret = "<float_value>" ;
          break ;
        case JD_D_double:
          ret = "<double_value>" ;
          break ;
        case JD_D_object:
          ret = "<object_value>" ;
          break ;
        case JD_D_thread:
          ret = "<thread_value>" ;
          break ;
        case JD_D_null:
          ret = "<null_value>" ;
          break ;
        case JD_D_void:
          ret = "<void_value>" ;
          break ;
        case JD_D_signal:
          ret = "<signal_value>" ;
          break ;
        case JD_D_string:
          ret = dt->data.string ;
          break ;
        default:
          ret = "<unknown>" ;
      }
    }
    else ret = "<array_value>" ;
  }
  else ret = "<void_value>" ;
  return ret ;
}

static void put_oct_to_tbuf( JD_data *dt, char ch ) {
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  char X, Z ;
  
  X = (ch=='o')?'x':'X' ;
  Z = (ch=='o')?'z':'Z' ;
  if( IS_SMLINT(dt) ) {
    _ulong = SMLINT2INT(dt) ;
    do {
      put_tbuf( to_char(_ulong & 7) ) ;
      _ulong >>= 3;
    } while (_ulong);
  }
  else switch( dt->type ) {
    case JD_D_int:
      _ulong = dt->data.int_value ;
      do {
        put_tbuf( to_char(_ulong & 7) ) ;
        _ulong >>= 3;
      } while (_ulong);
      break ;
    case JD_D_sbit:
      {
        unsigned long a, b, c, a7, b7, c7 ;
        a = dt->data.sbit.a ;
        b = dt->data.sbit.b ;
        c = (unsigned)0xffffffff >> (32-dt->data.sbit.size) ;
        do {
          a7 = a & 7 ; b7 = b & 7 ; c7 = c & 7 ;
          if( b7 ) {
            if( b7 == c7 ) {
              if( a7 = 0 ) put_tbuf( Z ) ;
              else if( a7 == c7 ) put_tbuf( X ) ;
              else put_tbuf( '?' ) ;
            }
            else put_tbuf( '?' ) ;
          }
          else put_tbuf( to_char(a7) ) ;
          a >>= 3 ; b >>= 3 ; c >>= 3 ;
        } while ( a || b ) ;
      }
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        unsigned long a, b, c, a7, b7, c7, mm ;
        int i, j, k, n ;
        i = dt->data.bit.size ;
        n = LASTWORD(i) ;
        k = (i%3)?(i/3)+1:(i/3) ;
        mm = (unsigned)0xffffffff >> (32- (i%32))%32 ;
        i = 0 ; j = 0 ;
        while(k) {
          if( (j<3) && (i<=n) ) {
            if( i ) {
              a = (unsigned)dt->data.bit.a[i-1] >> (32-j);
              b = (unsigned)dt->data.bit.b[i-1] >> (32-j);
              c = (unsigned)0xffffffff >> (32-j) ;
            }
            else {
              a = b = c = 0 ;
            }
            a |= (unsigned)dt->data.bit.a[i] << j;
            b |= (unsigned)dt->data.bit.b[i] << j;
            c |= ((i==n)?(mm<<j):(unsigned)0xffffffff) ;
            i++ ;
            j = 32 ;
          }
          a7 = a & 7 ;
          b7 = b & 7 ;
          c7 = c & 7 ;
          if( b7 ) {
            if( b7 == c7 ) {
              if( a7 == c7 ) put_tbuf( X ) ;
              else if( a7 == 0 ) put_tbuf( Z ) ;
              else put_tbuf( '?' ) ;
            }
            else put_tbuf( '?' ) ;
          }
          else put_tbuf( to_char( a7 ) ) ;
          a >>= 3 ; b >>= 3 ; c >>= 3 ;
          j-=3 ;                         
          k-- ;
        }
      }
      zero_suppress_tbuf() ;
      break ;
  }  /* end of switch( dt->type ) { */

}

/* static version of bit_ge_bit, which does not check B0/B1 */
static int bit_ge_bit( int *A0, int S0, int *A1, int S1 ) {
  register int RET ;
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        return 0 ;
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          return 1 ;
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          return 1 ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          return 0 ;  /* A0 < A1 */
        }
      }
    }
    return 1 ; /* A0 == A1 */
  }
}

static void put_dec_to_tbuf( JD_data *dt, char ch ) {
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  char X, Z ;
  
  X = (ch=='d')?'x':'X' ;
  Z = (ch=='d')?'z':'Z' ;
  if( IS_SMLINT(dt) ) {
    _ulong = SMLINT2INT(dt) ;
    while (_ulong >= 10) {
      put_tbuf( to_char(_ulong % 10) ) ;
      _ulong /= 10;
    } 
    put_tbuf( to_char(_ulong) ) ;
  }
  else switch( dt->type ) {
    case JD_D_int:
      _ulong = dt->data.int_value ;
      while (_ulong >= 10) {
        put_tbuf( to_char(_ulong % 10) ) ;
        _ulong /= 10;
      } 
      put_tbuf( to_char(_ulong) ) ;
      break ;
    case JD_D_sbit:
      {
        unsigned long a, b ;
        a = dt->data.sbit.a ;
        b = dt->data.sbit.b ;
        if( b ) {
          put_tbuf( '?' ) ;
        }
        else {
          while (a >= 10) {
            put_tbuf( to_char(a % 10) ) ;
            a /= 10;
          } 
          put_tbuf( to_char(a) ) ;
        } 
      }
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        int s, ten, i, n ;
        if( JD_check_bit_set( dt->data.bit.b, dt->data.bit.size ) ) {
          put_tbuf( '?' ) ;
        }
        else {
          s = dt->data.bit.size ;
          n = LASTWORD(s) ;
          ADJUST_TEMP(s) ;
          ten = 10 ;
          for( i = 0 ; i <= n ; i++ ) TMP_A[i] = dt->data.bit.a[i] ;
          while( bit_ge_bit( TMP_A, s, &ten, 32 ) ) {
            JD_bit_div_bit_ts( TMP_B, s, TMP_A, s, &ten, 32, &i ) ;
            put_tbuf( to_char(i) ) ;
            for( i = 0 ; i <= n ; i++ ) TMP_A[i] = TMP_B[i] ;
          }
          put_tbuf( to_char(TMP_A[0] % 10) ) ;
        }
      }
      zero_suppress_tbuf() ;
      break ;
  }  /* end of switch( dt->type ) { */

}

static void put_hex_to_tbuf( JD_data *dt, char ch ) {
  char *xdigs; /* digits for [xX] conversion */
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  char X, Z ;
  
  if( ch=='X' || ch=='H' ) {
    X = 'X' ;
    Z = 'Z' ;
    xdigs = "0123456789ABCDEF";
  }
  else {
    X = 'x' ;
    Z = 'z' ;
    xdigs = "0123456789abcdef";
  }
  
  if( IS_SMLINT(dt) ) {
    _ulong = SMLINT2INT(dt) ;
    do {
      put_tbuf( xdigs[_ulong & 15] ) ;
      _ulong >>= 4;
    } while (_ulong);
  }
  else switch( dt->type ) {
    case JD_D_int:
      _ulong = dt->data.int_value ;
      do {
        put_tbuf( xdigs[_ulong & 15] ) ;
        _ulong >>= 4;
      } while (_ulong);
      break ;
    case JD_D_sbit:
      {
        unsigned long a, b, c, a15, b15, c15 ;
        int i, j, k, n ;
        i = dt->data.sbit.size ;
        n = LASTWORD(i) ;
        k = (i%4)?(i/4)+1:(i/4) ;
        a = dt->data.sbit.a ;
        b = dt->data.sbit.b ;
        c = (unsigned)0xffffffff >> (32-dt->data.sbit.size) ;
        while(k) {
          a15 = a & 15 ; b15 = b & 15 ; c15 = c & 15 ;
          if( b15 ) {
            if( b15 == c15 ) {
              if( a15 = 0 ) put_tbuf( Z ) ;
              else if( a15 == c15 ) put_tbuf( X ) ;
              else put_tbuf( '?' ) ;
            }
            else put_tbuf( '?' ) ;
          }
          else put_tbuf( xdigs[a15] ) ;
          a >>= 4 ; b >>= 4 ; c >>= 4 ;
          k-- ;
        } 
      }
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        unsigned long a, b, c, a15, b15, c15, mm ;
        int i, j, k, n ;
        i = dt->data.bit.size ;
        n = LASTWORD(i) ;
        k = (i%4)?(i/4)+1:(i/4) ;
        mm = (unsigned)0xffffffff >> (32- (i%32))%32 ;
        i = 0 ; j = 0 ;
        while(k) {
          if( (j==0) && (i<=n) ) {
            a = (unsigned)dt->data.bit.a[i] ;
            b = (unsigned)dt->data.bit.b[i] ;
            c = ((i==n)?mm:(unsigned)0xffffffff) ;
            i++ ;
            j = 32 ;
          }
          a15 = a & 15 ;
          b15 = b & 15 ;
          c15 = c & 15 ;
          if( b15 ) {
            if( b15 == c15 ) {
              if( a15 == c15 ) put_tbuf( X ) ;
              else if( a15 == 0 ) put_tbuf( Z ) ;
              else put_tbuf( '?' ) ;
            }
            else put_tbuf( '?' ) ;
          }
          else put_tbuf( xdigs[a15] ) ;
          a >>= 4 ; b >>= 4 ; c >>= 4 ;
          j-=4 ;                         
          k-- ;
        }
      }
      break ;
  }  /* end of switch( dt->type ) { */

}


static void put_bin_to_tbuf( JD_data *dt, char ch ) {
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  char X, Z ;
  
  if( ch=='B' ) {
    X = 'X' ;
    Z = 'Z' ;
  }
  else {
    X = 'x' ;
    Z = 'z' ;
  }
  
  if( IS_SMLINT(dt) ) {
    _ulong = SMLINT2INT(dt) ;
    do {
      put_tbuf( to_char(_ulong & 1) ) ;
      _ulong >>= 1;
    } while (_ulong);
  }
  else switch( dt->type ) {
    case JD_D_int:
      _ulong = dt->data.int_value ;
      do {
        put_tbuf( to_char(_ulong & 1) ) ;
        _ulong >>= 1;
      } while (_ulong);
      break ;
    case JD_D_sbit:
      {
        unsigned long a, b, c, a1, b1 ;
        int k ;
        a = dt->data.sbit.a ;
        b = dt->data.sbit.b ;
        k = dt->data.sbit.size ;
        while( k ) {
          a1 = a & 1 ; b1 = b & 1 ;
          if( b1 ) {
            if( a1 == 0 ) put_tbuf( Z ) ;
            else if( a1 == 1 ) put_tbuf( X ) ;
          }
          else put_tbuf( to_char(a1) ) ;
          a >>= 1 ; b >>= 1 ; k-- ;
        } ;
      }
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        unsigned long a, b, a1, b1 ;
        int i, j, k, n, mm ;
        k = dt->data.bit.size ;
        mm = (unsigned)0xffffffff >> (32- (k%32))%32 ;
        i = 0 ; j = 0 ; 
        a = 0 ; b = 0 ;
        while(k) {
          if( j==0 ) {
            a = (unsigned)dt->data.bit.a[i] ;
            b = (unsigned)dt->data.bit.b[i] ;
            j = 32 ;
            i++ ;
          }
          a1 = a & 1 ;
          b1 = b & 1 ;
          if( b1 ) {
            if( a1 == 1 ) put_tbuf( X ) ;
            else put_tbuf( Z ) ;
          }
          else put_tbuf( to_char(a1) ) ;
          a >>= 1 ; b >>= 1 ;
          j-- ;                         
          k-- ;
        }
      }
      break ;
  }  /* end of switch( dt->type ) { */

}

/* we don't care much about short, long stuff (at least at this point) */

int JD_sys_fprintf (
  int iotype, char **str, FILE *fp, char *fmt0, int num_args
)
{
  register char *fmt; /* format string */
  char *fmt_start ;
  register int ch;	/* character from fmt */
  register int n;		/* handy integer (short term usage) */
  register char *cp;	/* handy char pointer (short term usage) */
  char *fmark;	/* for remembering a place in fmt */
  register int flags;	/* flags as above */
  int ret;		/* return value accumulator */
  int width;		/* width from format (%8d), or 0 */
  int prec;		/* precision from format (%.3d), or -1 */
  char sign;		/* sign prefix (' ', '+', '-', or \0) */
  int softsign;		/* temporary negative sign for floats */
  double _double;		/* double precision arguments %[eEfgG] */
  int fpprec;		/* `extra' floating precision in [eEfgG] */
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  JD_data *JD_dt;	/* integer arguments %[diouxX] */
  enum { OCT, DEC, HEX, BIN } base;/* base for [diouxX] conversion */
  int dprec;		/* a copy of prec if [diouxX], 0 otherwise */
  int dpad;		/* extra 0 padding needed for integers */
  int fieldsz;		/* field size expanded by sign, dpad etc */
	/* The initialization of 'size' is to suppress a warning that
	   'size' might be used unitialized.  It seems gcc can't
	   quite grok this spaghetti code ... */
  int size = 0;		/* size of converted field or string */
  char ox[2];		/* space for 0x hex-prefix */
  
  str_size = 0 ;  /* size of the allocated string */
  str_ptr = 0 ;  /* write pointer to the string */
  arg_ptr = 1 ;
  arg_base = num_args ;
  
  if( fmt0 == NULL ) {
    return ;
  }
  fmt = fmt0;
  ret = 0;

  /*
   * Scan the format for conversions (`%' character).
   */
  for (;;) {
    clear_tbuf() ; /* clear temp buffer, and allocate if NULL */
    for (fmark = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)  put_tbuf(ch) ;
      if ((n = fmt - fmark) != 0) {
        PRINTOUT(iotype,str,fp, fmark, n);
        ret += n;
      }
      if (ch == '\0') goto done;
      fmt_start = fmt ;
      fmt++;		/* skip over '%' */

      flags = 0;
      dprec = 0;
      fpprec = 0;
      width = 0;
      prec = -1;
      sign = '\0';
      
      clear_tbuf() ; /* clear temp buffer */

rflag:  
      ch = *fmt++;
reswitch:	
      switch (ch) {
        case ' ':
          /*
           * ``If the space and + flags both appear, the space
           * flag will be ignored.''
           *	-- ANSI X3J11
           */
          if (!sign) sign = ' ';
          goto rflag;
        case '#':
          flags |= ALT;
          goto rflag;
        case '*':
          /*
          * ``A negative field width argument is taken as a
          * - flag followed by a positive field width.'
          *	-- ANSI X3J11
          * They don't exclude field widths read from args.
          */
          if ((width = va_arg_int()) >= 0) goto rflag;
          width = -width;
          /* FALLTHROUGH */
        case '-': 
          flags |= LADJUST;
          flags &= ~ZEROPAD; /* '-' disables '0' */
          goto rflag;
        case '+':
          sign = '+';
          goto rflag;
        case '.':
          if ((ch = *fmt++) == '*') {
            n = va_arg_int();
            prec = n < 0 ? -1 : n ;
            goto rflag;
          }
          n = 0;
          while (is_digit(ch)) {
            n = 10 * n + to_digit(ch);
            ch = *fmt++;
          }
          prec = n < 0 ? -1 : n;
          goto reswitch;
        case '0':
          /*
           * ``Note that 0 is taken as a flag, not as the
           * beginning of a field width.''
           *	-- ANSI X3J11
           */
          if (!(flags & LADJUST))
            flags |= ZEROPAD; /* '-' disables '0' */
          goto rflag;
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          n = 0;
          do {
            n = 10 * n + to_digit(ch);
            ch = *fmt++;
          } while (is_digit(ch));
          width = n;
          goto reswitch;
        case 'L':
          flags |= LONGDBL;
          goto rflag;
        /* 'h' is eaten by verilog format 
        case 'h':
          flags |= SHORTINT;
          goto rflag;
        */
        case 'l':
           flags |= LONGINT;
           goto rflag;
        case 'c':
          PRINTCHAR( iotype,str,fp, va_arg_int() );
          continue;
        case 'D': 
          flags |= LONGINT;
          /*FALLTHROUGH*/
        case 'd':
        case 'i':
          JD_dt = va_arg_jd();
          if( IS_SMLINT(JD_dt) ) {
            int x = SMLINT2INT(JD_dt) ;
            if( x < 0 ) {
              x = -x ;
              JD_dt = INT2SMLINT(x) ;
              sign = '-' ;
            }
          }
          else if ( JD_dt && 
               JD_dt->type == JD_D_int && 
               JD_dt->sub_type == JD_SD_single &&
               JD_dt->data.int_value < 0 ) 
          {
            JD_dt->data.int_value = -JD_dt->data.int_value ;
            sign = '-';
          }
          base = DEC;
          goto number ;
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
          _double = va_arg_double();
          {
            int fmt_len = fmt - fmt_start;
            int i;
            
            if( fmt_buf == NULL )
              fmt_buf = (char *)calloc(100,sizeof(char));
            for(i = 0 ; i < fmt_len ; i++ ) fmt_buf[i] = fmt_start[i] ;
            /* allocate more than MAXEXP+MAXFACT+1 = 308+39+1 */
            if( float_buf == NULL ) 
              float_buf = (char *)calloc(400,sizeof(char)) ;
            sprintf( float_buf, fmt_buf, _double ) ;
            n = strlen(float_buf) ;
            PRINTOUT( iotype,str,fp, float_buf, -1 ) ;
            ret += n;
          }
          /* CHECK ERROR! */
          continue;
/* Don't know what to do with 'n', just skip it           
        case 'n':
          if (flags & LONGINT)
            *va_arg(ap, long *) = ret;
          else if (flags & SHORTINT)
            *va_arg(ap, short *) = ret;
          else
            *va_arg(ap, int *) = ret;
          continue;	/ * no output * /
*/
        case 'O':
          flags |= LONGINT;
          /*FALLTHROUGH*/
        case 'o':
          JD_dt = va_arg_jd();
          base = OCT;
          goto nosign;
        case 'p':
          /*
           * ``The argument shall be a pointer to void.  The
           * value of the pointer is converted to a sequence
           * of printable characters, in an implementation-
           * defined manner.''
           *	-- ANSI X3J11
           */
          /* NOSTRICT */
          JD_dt = va_arg_ptr();
          base = HEX;
          flags |= HEXPREFIX;
          ch = 'x';
          goto nosign;
        case 's':
          if ((cp = va_arg_string()) == NULL)
            cp = "(null)";
          if (prec >= 0) {
            /*
             * can't use strlen; can only look for the
             * NUL in the first `prec' characters, and
             * strlen() will go further.
             */
            char *p = (char*)memchr(cp, 0, prec);
            if (p != NULL) {
              size = p - cp;
              if (size > prec)
                size = prec;
            } else
              size = prec;
          } else
            size = strlen(cp);
          sign = '\0';
          break;
        case 'U':
          flags |= LONGINT ;
          /*FALLTHROUGH*/
        case 'u':
          JD_dt = va_arg_jd();
          base = DEC;
          goto nosign;
        case 'X':
        case 'x':
        case 'H':
        case 'h':
          JD_dt = va_arg_jd();
          base = HEX;
          /* leading 0x/X only if non-zero */
          if( IS_SMLINT(JD_dt) ) {
            if ( flags & ALT && ( SMLINT2INT(JD_dt) != 0 ) )
              flags |= HEXPREFIX;
          }
          if (flags & ALT && 
              JD_dt && 
              JD_dt->type == JD_D_int && 
              JD_dt->sub_type == JD_SD_single && 
              JD_dt->data.int_value != 0 
             )
            flags |= HEXPREFIX;
          /* unsigned conversions */
          goto nosign;
        case 'B':
        case 'b':
          JD_dt = va_arg_jd();
          base = BIN;
nosign:
          sign = '\0';
          /*
           * ``... diouXx conversions ... if a precision is
           * specified, the 0 flag will be ignored.''
           *	-- ANSI X3J11
           */
number:
          if ((dprec = prec) >= 0) flags &= ~ZEROPAD;
          /*
           * ``The result of converting a zero value with an
           * explicit precision of zero is no characters.''
           *	-- ANSI X3J11
           */
          if ( ( JD_dt != NULL && 
                 ( IS_SMLINT(JD_dt) ||
                   JD_dt->sub_type==JD_SD_single ) && 
                 prec != 0 ) )
          {
            switch (base) {
              case OCT:
                put_oct_to_tbuf( JD_dt, ch ) ;
                break;
              case DEC:
                put_dec_to_tbuf( JD_dt, ch ) ;
                break;
              case HEX:
                put_hex_to_tbuf( JD_dt, ch ) ;
                break;
              case BIN:
                put_bin_to_tbuf( JD_dt, ch ) ;
                break;
              default:
                cp = "bug in vform: bad base";
                goto skipsize;
            }
            size = &tbuf[tbuf_size-2] - tbuf_ptr;
            cp = tbuf_ptr+1 ;
          }
skipsize:
          break;
        default:	/* "%?" prints ?, unless ? is NUL */
          if (ch == '\0') goto done;
          /* pretend it was %c with argument ch */
          put_tbuf( ch ) ;
          size = 1;
          cp = tbuf_ptr ;
          sign = '\0';
          break;
      } /* end of switch */
      /*
       * All reasonable formats wind up here.  At this point,
       * `cp' points to a string which (if not flags&LADJUST)
       * should be padded out to `width' places.  If
       * flags&ZEROPAD, it should first be prefixed by any
       * sign or other prefix; otherwise, it should be blank
       * padded before the prefix is emitted.  After any
       * left-hand padding and prefixing, emit zeroes
       * required by a decimal [diouxX] precision, then print
       * the string proper, then emit zeroes required by any
       * leftover floating precision; finally, if LADJUST,
       * pad with blanks.
       */
          
          /*
           * compute actual size, so we know how much to pad.
           */
          fieldsz = size;
          dpad = dprec - size;
          if (dpad < 0)
            dpad = 0;

          if (sign) fieldsz++;
          else if (flags & HEXPREFIX)
            fieldsz += 2;
          fieldsz += dpad;

          /* right-adjusting blank padding */
          if (width > fieldsz && (flags & (LADJUST|ZEROPAD)) == 0)
            PAD_SP(iotype,str,fp, width - fieldsz);
            
          /* prefix */
          if (sign) {
            PRINTCHAR( iotype,str,fp,sign);
          } else if (flags & HEXPREFIX) {
            PRINTCHAR( iotype,str,fp,'0');
            PRINTCHAR( iotype,str,fp,ch);
          }
          
          /* right-adjusting zero padding */
          if (width > fieldsz && (flags & (LADJUST|ZEROPAD)) == ZEROPAD)
            PAD_0(iotype,str,fp, width - fieldsz);
            
            /* leading zeroes from decimal precision */
            PAD_0(iotype,str,fp, dpad);
            
            /* the string or number proper */
            PRINTOUT(iotype,str,fp, cp, size);
            
            /* left-adjusting padding (always blank) */
            if (width > fieldsz && flags & LADJUST)
              PAD_SP(iotype,str,fp, width - fieldsz);
              
            /* finally, adjust ret */
            ret += width > fieldsz ? width : fieldsz;

	}
done:
	return ret;
error:
	return EOF;
	/* NOTREACHED */
}


JD_sf_printf() {
  JD_data *fmt_dt ;
  
  fmt_dt = STACK[SP - NUM_ARG] ;

  JD_sys_fprintf (
    0, NULL, NULL, fmt_dt->data.string, NUM_ARG
  ) ;

}

JD_sf_error() {
  JD_data *fmt_dt ;
  int cycle ;

  JEDA_pli_get_cycle( &cycle ) ;

  JD_code_location() ;
  PRINTF( "JEDA VERIFICATION ERROR @ %d :\n  ", cycle ) ;  
  fmt_dt = STACK[SP - NUM_ARG] ;

  JD_sys_fprintf (
    0, NULL, NULL, fmt_dt->data.string, NUM_ARG
  ) ;

  VERIF_ERROR ;
  
}

JD_sf_warning() {
  JD_data *fmt_dt ;
  int cycle ;

  JEDA_pli_get_cycle( &cycle ) ;
  
  JD_code_location() ;
  PRINTF( "Jeda VERIFICATION WARNING @ %d :\n  ", cycle ) ;  
  fmt_dt = STACK[SP - NUM_ARG] ;

  JD_sys_fprintf (
    0, NULL, NULL, fmt_dt->data.string, NUM_ARG
  ) ;

}
