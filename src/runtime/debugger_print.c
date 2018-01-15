/*
 * Jeda Debugger print out data
 */
/* 
   
   Copyright (C) 2001, 2002 Juniper Networks Inc.
   Copyright (C) 2002 Jeda Technologies, Inc.

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

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#include "code_exec_macro.h"
#include "debugger.h"
#include "debugger_error_msg.h"

int JD_dbg_prompt_user_reply() ;

#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif
extern JD_debug_stack *JD_debugged_stack ;

extern int JD_dbg_eval_error ;

int JD_dbg_print_out_select = 0 ;
int JD_dbg_print_interactive = 1 ;
char *JD_dbg_print_prefix = NULL ;
char *JD_dbg_print_end_char = "\n" ;

int JD_dbg_print_format = 0 ;
char **JD_dbg_print_str ; 
FILE *JD_dbg_print_fp ; 
int JD_dbg_print_fnum ; 

int JD_dbg_print_zero_supress = 0 ;

static int str_ptr ;
static int str_size ;

static char *xdigs = "0123456789ABCDEF";

void JD_debugger_show_threads() ;

extern JD_dbg_break_point *JD_debug_top_break_point ;

#define MIN_TMP_BUFF 1024
static char *tbuf = NULL ;
static int  tbuf_size = 0 ;
static char *tbuf_ptr ;

void DBGPRINT( char *txt, int num )
{
  int i, size ;
  char *buff, *src, cc ;
  switch( JD_dbg_print_out_select ) {
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
        src = *JD_dbg_print_str ;
        for( i = 0 ; i < str_ptr ; i++ ) buff[i] = src[i] ;
        *JD_dbg_print_str = buff ;
        if( str_ptr ) free(src) ;
      }
      src = *JD_dbg_print_str ;
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
      if( num < 0 ) {
        write(JD_dbg_print_fnum, txt, strlen(txt)) ;
      }
      else {
        char tbuf[3] ;
        for( i = 0 ; i < num ; i++ ) {
          cc = txt[i] ;
          if( cc == '\\' ) {
            i++ ;
            cc = txt[i] ;
            switch(cc) {
              case 'n': write(JD_dbg_print_fnum, "\n" ) ; break ;
              case 't': write(JD_dbg_print_fnum, "\t" ) ; break ;
              case 'b': write(JD_dbg_print_fnum, "\b" ) ; break ;
              case 'v': write(JD_dbg_print_fnum, "\v" ) ; break ;
              case 'f': write(JD_dbg_print_fnum, "\f" ) ; break ;
              case 'r': write(JD_dbg_print_fnum, "\r" ) ; break ;
              default :  
                tbuf[0] = cc ; write(JD_dbg_print_fnum, tbuf, 1 ) ; break ;
            }
          }
          else {
            tbuf[0] = cc ;
            write(JD_dbg_print_fnum, tbuf, 1 ) ;
          }
        }
      }
      break ;
    case 3: /* print out to a file  */
      if( num < 0 ) fprintf( JD_dbg_print_fp, "%s", txt ) ;
      else {
        for( i = 0 ; i < num ; i++ ) {
          cc = txt[i] ;
          if( cc == '\\' ) {
            i++ ;
            cc = txt[i] ;
            switch(cc) {
              case 'n': fprintf( JD_dbg_print_fp, "%c", '\n' ) ; break ;
              case 't': fprintf( JD_dbg_print_fp, "%c", '\t' ) ; break ;
              case 'b': fprintf( JD_dbg_print_fp, "%c", '\b' ) ; break ;
              case 'v': fprintf( JD_dbg_print_fp, "%c", '\v' ) ; break ;
              case 'f': fprintf( JD_dbg_print_fp, "%c", '\f' ) ; break ;
              case 'r': fprintf( JD_dbg_print_fp, "%c", '\r' ) ; break ;
              default : fprintf( JD_dbg_print_fp, "%c",  cc  ) ; break ;
            }
          }
          else fprintf( JD_dbg_print_fp, "%c", cc ) ;
        }
      }
      break ;
    case 4: /* print out to stderr */
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

static void clear_tbuf() {
  if( tbuf == NULL ) {
    tbuf = (char *)calloc( MIN_TMP_BUFF, sizeof(char) ) ;
    tbuf_size = MIN_TMP_BUFF ;
  }
  tbuf[tbuf_size-1] = '\0' ;
  tbuf_ptr = &tbuf[tbuf_size-1] ;
}

static void flush_tbuf() {
  if( tbuf == NULL ) {
    tbuf = (char *)calloc( MIN_TMP_BUFF, sizeof(char) ) ;
    tbuf_size = MIN_TMP_BUFF ;
  }
  else {
    DBGPRINT( tbuf_ptr, -1 ) ;
  }
  tbuf[tbuf_size-1] = '\0' ;
  tbuf_ptr = &tbuf[tbuf_size-2] ;
}

static void put_tbuf( char ch ) {
  tbuf_ptr-- ;
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
}

static void zero_suppress_tbuf() {
  while( tbuf_ptr != &tbuf[tbuf_size-2] && tbuf_ptr[0] == '0' ) tbuf_ptr++ ;
}

/*
  - Macro printout (can switch output to socket)
  
  JD_dbg_print_interactive = 1 
    More-printout (stops at each 80 line, prompt, terminate with 'q' input
  
  JD_dbg_print_format
    format: 0 -- default. bit -> h, int -> d
            1 -> bit
            2 -> hex
            3 -> dec
            4 -> oct
            
   JD_dbg_print_out_select
    Printout is done with the macro DBGOUTn
    outselect:
            0 -- PRINTF
            1 -- SOCKETOUT
            
*/

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

void DBGPRINTINDENT( int indent ) {
  int i ;
  if( JD_dbg_print_end_char != NULL ) {
    for( i = 0 ; i < indent ; i++ ) DBGPRINT( "  ", -1 ) ;
  }
  if( JD_dbg_print_prefix ) {
    DBGPRINT( JD_dbg_print_prefix, -1 ) ;
    JD_dbg_print_prefix = NULL ;
  }
}

static void print_bit (
  int *bit_a, int *bit_b, int size, int indent
)
{
  clear_tbuf() ;
  
  DBGPRINTINDENT( indent ) ;
  
  switch( JD_dbg_print_format ) {
    case 0:
    case 2:
      /* hex */
      {
        unsigned long a, b, c, a15, b15, c15, mm ;
        int i, j, k, n ;
        i = size ;
        n = LASTWORD(i) ;
        k = (i%4)?(i/4)+1:(i/4) ;
        mm = (unsigned)0xffffffff >> (32- (i%32))%32 ;
        i = 0 ; j = 0 ;
        while(k) {
          if( (j==0) && (i<=n) ) {
            a = (unsigned)bit_a[i] ;
            b = (unsigned)bit_b[i] ;
            c = ((i==n)?mm:(unsigned)0xffffffff) ;
            i++ ;
            j = 32 ;
          }
          a15 = a & 15 ;
          b15 = b & 15 ;
          c15 = c & 15 ;
          if( b15 ) {
            if( b15 == c15 ) {
              if( a15 == c15 ) put_tbuf( 'X' ) ;
              else if( a15 == 0 ) put_tbuf( 'Z' ) ;
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
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'h' ) ;
      put_tbuf( '\'' ) ;
      break ;
    case 1:
      /* bin */
      {
        unsigned long a, b, a1, b1 ;
        int i, j, k ;
        
        k = size ;
        i = 0 ; j = 0 ; 
        a = 0 ; b = 0 ;
        while(k) {
          if( j==0 ) {
            a = (unsigned)bit_a[i] ;
            b = (unsigned)bit_b[i] ;
            j = 32 ;
            i++ ;
          }
          a1 = a & 1 ;
          b1 = b & 1 ;
          if( b1 ) {
            if( a1 == 1 ) put_tbuf( 'x' ) ;
            else put_tbuf( 'z' ) ;
          }
          else put_tbuf( to_char(a1) ) ;
          a >>= 1 ; b >>= 1 ;
          j-- ;                         
          k-- ;
        }
      }
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'b' ) ;
      put_tbuf( '\'' ) ;
      break ;
    case 3:
      /* dec */
      {
        int s, ten, i, n ;
        if( JD_check_bit_set( bit_b, size ) ) {
          put_tbuf( '?' ) ;
        }
        else {
          s = size ;
          n = LASTWORD(s) ;
          ADJUST_TEMP(s) ;
          ten = 10 ;
          for( i = 0 ; i <= n ; i++ ) bit_b[i] = bit_a[i] ;
          while( bit_ge_bit( bit_a, s, &ten, 32 ) ) {
            JD_bit_div_bit_ts( bit_b, s, bit_a, s, &ten, 32, &i ) ;
            put_tbuf( to_char(i) ) ;
            for( i = 0 ; i <= n ; i++ ) bit_a[i] = bit_b[i] ;
          }
          put_tbuf( to_char(bit_a[0] % 10) ) ;
        }
      }
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'd' ) ;
      put_tbuf( '\'' ) ;
      break ;
    case 4:
      /* oct */
      {
        unsigned long a, b, c, a7, b7, c7, mm ;
        int i, j, k, n ;
        i = size ;
        n = LASTWORD(i) ;
        k = (i%3)?(i/3)+1:(i/3) ;
        mm = (unsigned)0xffffffff >> (32- (i%32))%32 ;
        i = 0 ; j = 0 ;
        while(k) {
          if( (j<3) && (i<=n) ) {
            if( i ) {
              a = (unsigned)bit_a[i-1] >> (32-j);
              b = (unsigned)bit_b[i-1] >> (32-j);
              c = (unsigned)0xffffffff >> (32-j) ;
            }
            else {
              a = b = c = 0 ;
            }
            a |= (unsigned)bit_a[i] << j;
            b |= (unsigned)bit_b[i] << j;
            c |= ((i==n)?(mm<<j):(unsigned)0xffffffff) ;
            i++ ;
            j = 32 ;
          }
          a7 = a & 7 ;
          b7 = b & 7 ;
          c7 = c & 7 ;
          if( b7 ) {
            if( b7 == c7 ) {
              if( a7 == c7 ) put_tbuf( 'X' ) ;
              else if( a7 == 0 ) put_tbuf( 'Z' ) ;
              else put_tbuf( '?' ) ;
            }
            else put_tbuf( '?' ) ;
          }
          else {
            put_tbuf( to_char( a7 ) ) ;
          }
          a >>= 3 ; b >>= 3 ; c >>= 3 ;
          j-=3 ;                         
          k-- ;
        }
      }
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'o' ) ;
      put_tbuf( '\'' ) ;
      break ;
     
  }
  
  { /* printing out size info */
    unsigned long _ulong;	/* integer arguments %[diouxX] */
    _ulong = size ;
    
    while (_ulong >= 10) {
      put_tbuf( to_char(_ulong % 10) ) ;
      _ulong /= 10;
    } 
    if( _ulong ) put_tbuf( to_char(_ulong) ) ;
  }
  
  flush_tbuf() ;
  
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;

}

static void print_int (
  int int_value, int indent
)
{
  unsigned long _ulong;	/* integer arguments %[diouxX] */
  clear_tbuf() ;
  
  DBGPRINTINDENT( indent ) ;
  
  switch( JD_dbg_print_format ) {
    case 2:
      /* hex */
      _ulong = int_value ;
      do {
        put_tbuf( xdigs[_ulong & 15] ) ;
        _ulong >>= 4;
      } while (_ulong);
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'h' ) ;
      put_tbuf( '\'' ) ;
      break ;
    case 1:
      /* bin */
      _ulong = int_value ;
      do {
        put_tbuf( to_char(_ulong & 1) ) ;
        _ulong >>= 1;
      } while (_ulong);
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'b' ) ;
      put_tbuf( '\'' ) ;
      break ;
    case 0:
    case 3:
      /* dec */
      if( int_value < 0 ) {
        DBGPRINT( "-", -1 ) ;
        _ulong = -int_value ;
      }
      else _ulong = int_value ;
      while (_ulong >= 10) {
        put_tbuf( to_char(_ulong % 10) ) ;
        _ulong /= 10;
      } 
      put_tbuf( to_char(_ulong) ) ;
      zero_suppress_tbuf() ; 
      /* put_tbuf( 'd' ) ;
         put_tbuf( '\'' ) ; */
      break ;
    case 4:
      /* oct */
      _ulong = int_value ;
      do {
        put_tbuf( to_char(_ulong & 7) ) ;
        _ulong >>= 3;
      } while (_ulong);
      break ;
      if( JD_dbg_print_zero_supress )
        zero_suppress_tbuf() ; 
      put_tbuf( 'o' ) ;
      put_tbuf( '\'' ) ;
      break ;
     
  }
  
  flush_tbuf() ;
  
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;

}

static void print_real(
  double real_value, int indent
) {
  char *float_buf = (char *)calloc(400,sizeof(char)) ;
  
  DBGPRINTINDENT( indent ) ;

  sprintf( float_buf, "%F", real_value ) ;
  
  DBGPRINT( float_buf, -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  free(float_buf) ;
}

static void print_string(
  char *str, int indent
) {
  
  DBGPRINTINDENT( indent ) ;
  if( str ) {
    DBGPRINT( "\"", -1 ) ;
    DBGPRINT( str, -1 ) ;
    DBGPRINT( "\"", -1 ) ;
  }
  else
    DBGPRINT( "(null string)", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
}

static void print_data( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{

  while( dt->type == JD_D_pointer ) {
    dt = (JD_data *)JD_get_ptr_element(dt) ;
  }

  switch( dt->sub_type ) {
    case JD_SD_single:
      switch( dt->type ) {
        case JD_D_sbit:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_bit( 
              &dt->data.sbit.a, &dt->data.sbit.b, dt->data.sbit.size, 
              0
            ) ;
          }
          else {
            print_bit( 
              &dt->data.sbit.a, &dt->data.sbit.b, dt->data.sbit.size, 
              indent
            ) ;
          }
          break ;
        case JD_D_bit:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_bit( 
              dt->data.bit.a, dt->data.bit.b, dt->data.bit.size, 
              0
            ) ;
          }
          else {
            print_bit( 
              dt->data.bit.a, dt->data.bit.b, dt->data.bit.size, 
              indent
            ) ;
          }
          break ;
        case JD_D_int:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_int(
              dt->data.int_value, 0
            ) ;
          }
          else {
            print_int(
              dt->data.int_value, indent
            ) ;
          }
          break ;
        case JD_D_float:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_real(
              dt->data.float_value, 0
            ) ;
          }
          else {
            print_real(
              dt->data.float_value, indent
            ) ;
          }
          break ;
        case JD_D_double:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_real(
              dt->data.double_value, 0
            ) ;
          }
          else {
            print_real(
              dt->data.double_value, indent
            ) ;
          }
          break ;
        case JD_D_string:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_string( dt->data.string, 0 ) ;
          }
          else {
            print_string( dt->data.string, indent ) ;
          }
          break ;
        case JD_D_null:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
            print_string( "(null)", 0 ) ;
          }
          else {
            print_string( "(null)", indent ) ;
          }
          break ;
        case JD_D_object:
          DBGPRINTINDENT( indent ) ;
          if( scope ) {
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = instance of class \"", -1 ) ;
            DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
            DBGPRINT( "\"", -1 ) ;
          }
          else DBGPRINT( "(unknown object)", -1 ) ;
          DBGPRINT( JD_dbg_print_end_char, -1 ) ;
          break ;
        case JD_D_thread:
          if( scope ) {
            DBGPRINTINDENT( indent ) ;
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = thread", -1 ) ;
          }
          else DBGPRINT( "(unknown object)", -1 ) ;
         DBGPRINT( JD_dbg_print_end_char, -1 ) ;
          break ;
        case JD_D_signal:
          DBGPRINTINDENT( indent ) ;
          if( scope ) {
            DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
            DBGPRINT( " = ", -1 ) ;
          }
          DBGPRINT( "signal (", -1 ) ;
          if( dt->data.signal.port ) {
            JD_pli_object *obj ;
            obj = (JD_pli_object *)dt->data.signal.port ;
            if( obj->is.port.pset_name ) {
              DBGPRINT( obj->is.port.pset_name, -1 ) ;
              DBGPRINT( ".", -1 ) ;
            }
            DBGPRINT( obj->is.port.name, -1 ) ;
          }
          else {
            DBGPRINT( "(null)", -1 ) ;
          }
          DBGPRINT( ")", -1 ) ;
          DBGPRINT( JD_dbg_print_end_char, -1 ) ;
          break ;
        default:
          DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
          break ;
      }
      break ;
    case JD_SD_array:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " = array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_SD_assoc:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " = associative array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_SD_marray:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " = multi-dimentional array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_SD_strided:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " = string indexed array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_SD_list:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " = list of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
  }
}

static void print_obj(
  JD_data *dt, JD_dbg_name_node *scope, int indent
) {
  JD_dbg_name_node *name_node, *mem ;
  JD_dbg_name_node **members ;
  JD_data *mt ;
  int i, n ;
  int cls ;
  
  if( !dt->data.obj.ect ) {
    DBGPRINTINDENT( indent ) ;
    DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
    DBGPRINT( " (instance of class \"", -1 ) ;
    DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
    DBGPRINT( "\") = { (null) }", -1 ) ;
    DBGPRINT( JD_dbg_print_end_char, -1 ) ;
    return ;
  }
  name_node = 
    (JD_dbg_name_node *)
    JD_debugger_lookup_name( 
      JD_Debug_name_table[scope->dtype], 0 
    ) ;
  if( name_node == NULL ) {
    /* system class */
    if( dt->data.obj.ect->display ) {
      char *info, *indc ;
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " (instance of system class \"", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( "\") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      indc = (char *)calloc( (indent*2)+10, sizeof(char) ) ;
      for( i = 0 ; i < indent+1 ; i++ ) {
        strcat( indc, "  " ) ;
      }
      info = 
        (void *)(dt->data.obj.ect->display)(
          indc, dt, JD_dbg_print_end_char
        ) ;
      DBGPRINT( info, -1 ) ;
      free(info) ;
      free(indc) ;
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
    }
    else {
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
      DBGPRINT( " (instance of system class \"", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( "\")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
    }
    return ;
  }
  
  
  n = 0 ;
  mem = name_node->next ;
  while( mem ) {
    if( mem->type == JD_dbg_var_type ) n++ ;
    mem = mem->next ;
  }
  members = (JD_dbg_name_node **)calloc(n, sizeof(JD_dbg_name_node *) ) ;
  mem = name_node->next ;
  while( mem ) {
    if( mem->type == JD_dbg_var_type ) {
      if( mem->index > n ) {
        DBG_ERROR_MEMBER_INDEX_OUT_OF_BOUND( JD_Debug_name_table[mem->name] ) ;
        return ;
      }
      members[mem->index] = mem ;
    }
    mem = mem->next ;
  }
  
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
  DBGPRINT( " (instance of class \"", -1 ) ;
  DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
  DBGPRINT( "\") = {", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  if( dt->data.obj.ect->super ) {
    JD_dbg_name_node *super ;
    int keep ;
    
    super = 
      (JD_dbg_name_node *)
      JD_debugger_lookup_name( "super", 0 ) ;
      
    if( !super ) {
      DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( "super" ) ;
      return ;
    }
    keep = super->dtype ; /* if it's used */
    super->dtype = name_node->dtype ;
    print_obj( dt->data.obj.ect->super, super, indent+1 ) ;
    super->dtype = keep ;
  }
  for( i = 0 ; i < n ; i++ ) {
    mem = members[i] ;
    mt = dt->data.obj.ect->table[i] ;
    print_data( mt, mem, indent+1 ) ;
  }
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( "}", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  free( members ) ;
}

#define PRINT_LIMIT 80

static void print_array( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{
  int i ;
  int count = 0 ;
  int loop = 1 ;
  char buf[100] ;
  JD_data *st ;
  JD_data_unit *ut ;
  
  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_bit:
    case JD_D_int:
    case JD_D_float:
    case JD_D_double:
    case JD_D_string:
    case JD_D_signal:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      for( i = 0 ; i < dt->data.array.size && loop ; i++ ) {
        sprintf( buf, "[%d]=", i ) ;
        JD_dbg_print_prefix = buf ;
        ut = &dt->data.array.body[i] ;
        st = ut->data ;
        print_data( st, NULL, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_object:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of class (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_thread:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of thread", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
      break ;
  }
  

}

int set_hash_entry( 
  JD_hash_entry *hash, JD_data **dt_array, 
  int *index_array, char **str_array, int n
) {
  if( hash->left )
    n = set_hash_entry( hash->left, dt_array, index_array, str_array, n ) ;
  if( hash->right )
    n = set_hash_entry( hash->right, dt_array, index_array, str_array, n ) ;
  dt_array[n] = hash->data ;
  if( index_array )
    index_array[n] = hash->idx ;
  if( str_array )
    str_array[n] = hash->string ;
  n++ ;
  return n ;
}

static int set_hush_element( 
  JD_data *dt, JD_data **dt_array, int *index_array, char **str_array 
) {
  int i, n ;
  n = 0 ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( dt->data.hash.table[i] ) {
      n = 
        set_hash_entry( 
          dt->data.hash.table[i], dt_array, index_array, str_array, n
        ) ;
    }
  }
  return n ;
}

/*********************************************************************
 * those following two functions are the famous quick sort functions
 * you learned at the Algorithm class.
 *********************************************************************/
static int partition_int( 
  JD_data **dt_array, int *index_array, 
  int left, int right 
) {
  int lo, high ;
  int pivot, tmp ;
  JD_data *tmpd ;
  int done = 0 ;
  
  lo = left+1 ;
  high = right ;
  pivot = index_array[left] ;
  while( !done ) {
    while( index_array[lo] < pivot ) {
      lo++ ;
      if ( lo > right ) break ;
    }
    while( index_array[high] > pivot ) {
      high-- ;
      if( high < 0 ) break ;
    }
    if( lo < high ) {
      /* swap lo & high */
      tmp = index_array[lo] ;
      tmpd = dt_array[lo] ;
      index_array[lo] = index_array[high] ;
      dt_array[lo] = dt_array[high] ;
      index_array[high] = tmp ;
      dt_array[high] = tmpd ;
    }
    else done = 1 ;
  }
  if( high >= 0 ) {
    /* swap left & high */
    tmp = index_array[left] ;
    tmpd = dt_array[left] ;
    index_array[left] = index_array[high] ;
    dt_array[left] = dt_array[high] ;
    index_array[high] = tmp ;
    dt_array[high] = tmpd ;
  }
  return high ;
}

static int partition_str( 
  JD_data **dt_array, char **str_array, 
  int left, int right 
) {
  int lo, high ;
  char *pivot, *tmp ;
  JD_data *tmpd ;
  int done = 0 ;
  
  lo = left+1 ;
  high = right ;
  pivot = str_array[left] ;
  while( !done ) {
    while( strcmp(str_array[lo], pivot) < 0 ) {
      lo++ ;
      if ( lo > right ) break ;
    }
    while( strcmp(str_array[high], pivot) > 0 ) {
      high-- ;
      if( high < 0 ) break ;
    }
    if( lo < high ) {
      /* swap lo & high */
      tmp = str_array[lo] ;
      tmpd = dt_array[lo] ;
      str_array[lo] = str_array[high] ;
      dt_array[lo] = dt_array[high] ;
      str_array[high] = tmp ;
      dt_array[high] = tmpd ;
    }
    else done = 1 ;
  }
  if( high >= 0 ) {
    /* swap left & high */
    tmp = str_array[left] ;
    tmpd = dt_array[left] ;
    str_array[left] = str_array[high] ;
    dt_array[left] = dt_array[high] ;
    str_array[high] = tmp ;
    dt_array[high] = tmpd ;
  }
  return high ;
}


static void quick_sort( 
  JD_data **dt_array, int *index_array, char **str_array,
  int left, int right, int level 
) {
  int pivot_pos ;
  int i ;
  
  /*
  for( i = 0 ; i < level ; i++ ) printf( " " ) ;
  printf( "quicksort level: %d left:%d right:%d \n", level, left, right ) ;
  for( i = 0 ; i < level ; i++ ) printf( " " ) ;
  for( i = 0 ; i < 8 ; i++ ) 
    printf( "%d ", index_array[i] ) ;
  printf( "\n" ) ;
  */
  
  if( left < right ) {
    if( index_array )
      pivot_pos = partition_int( dt_array, index_array, left, right ) ;
    else 
      pivot_pos = partition_str( dt_array, str_array, left, right ) ;
    /*
    for( i = 0 ; i < level ; i++ ) printf( " " ) ;
    for( i = 0 ; i < 8 ; i++ ) 
      printf( "%d ", ua[i]->data->data.int_value ) ;
    printf( "\n" ) ;
    */
    if( pivot_pos >= 0 ) {
      quick_sort( 
        dt_array, index_array, str_array, left, pivot_pos-1, level+1 
      ) ;
      quick_sort( 
        dt_array, index_array, str_array, pivot_pos+1, right, level+1 
      ) ;
    }
  }

}

static void print_assoc( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{
  int count = 0 ;
  int n ;
  JD_data **dt_array ;
  int *index_array ;
  int loop = 1 ;
  char buf[100] ;
  JD_data *st ;
  int i ;
  
  n = JD_count_hush_element( dt ) ;
  dt_array = (JD_data **)calloc(n, sizeof(JD_data *) ) ;
  index_array = (int *)calloc(n, sizeof(int) ) ;
  set_hush_element( dt, dt_array, index_array, NULL ) ;
  
  quick_sort( dt_array, index_array, NULL, 0, n-1, 0 ) ;

  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_bit:
    case JD_D_int:
    case JD_D_float:
    case JD_D_double:
    case JD_D_string:
    case JD_D_signal:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Associative array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      for( i = 0 ; i < n && loop ; i++ ) {
        sprintf( buf, "[%d]=", index_array[i] ) ;
        JD_dbg_print_prefix = buf ;
        st = dt_array[i] ;
        print_data( st, NULL, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_object:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Associative array of class (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      for( i = 0 ; i < n && loop ; i++ ) {
        sprintf( buf, "[%d]=", index_array[i] ) ;
        JD_dbg_print_prefix = buf ;
        st = dt_array[i] ;
        print_obj( st, scope, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_thread:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Associative array of thread", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
      break ;
  }
  
  free(dt_array) ;
  free(index_array) ;
  
}

static void print_marray( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{
  int i, j, n ;
  int count = 0 ;
  int loop = 1 ;
  char *buf ;
  char ibuf[100] ;
  int *indexs ;
  int index ;
  JD_data *st ;
  JD_data_unit *ut ;
  
  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_bit:
    case JD_D_int:
    case JD_D_float:
    case JD_D_double:
    case JD_D_string:
    case JD_D_signal:
      indexs = (int *)calloc( dt->data.array.dim_size[0], sizeof(int) ) ;
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Multi-dimentional array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      n = dt->data.array.dim_size[0] ;
      buf = (char *)calloc( n*100, sizeof(char) ) ;
      while( loop ) {
        for( i = 0 ; i < n*100 ; i++ ) buf[i] = '\0' ;
        for( i = 0 ; i < n ; i++ ) {
          sprintf( tbuf, "[%d]", indexs[i] ) ;
          strcat(buf, tbuf) ;
        }
        strcat( buf, "=" ) ;
        JD_dbg_print_prefix = buf ;
        index = 0 ;
        for( i = 0 ; i < n ; i++ ) {
          index *= dt->data.array.dim_size[i+1] ;
          index += indexs[i] ;
        }
        ut = &dt->data.array.body[index] ;
        st = ut->data ;
        print_data( st, NULL, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
        if( loop ) {
          loop = 0 ;
          for( i = n ; i >= 0 ; i-- ) {
            indexs[i]++ ;
            if( indexs[i] == dt->data.array.dim_size[i+1] ) {
              indexs[i] = 0 ;
            }
            else {
              loop = 1 ;
              i = -1 ;
            }
          }
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      free( buf ) ;
      break ;
    case JD_D_object:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Multi_dimentinal-array", -1 ) ;
      for( i = 1 ; i < dt->data.array.dim_size[0]+1 ; i++ ) {
        sprintf( buf, "[%d]", dt->data.array.dim_size[i] ) ;
        DBGPRINT( buf, -1 ) ;
      }
      DBGPRINT( " of class (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ")", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_thread:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Multi_dimentinal-array", -1 ) ;
      for( i = 1 ; i < dt->data.array.dim_size[0]+1 ; i++ ) {
        sprintf( buf, "[%d]", dt->data.array.dim_size[i] ) ;
        DBGPRINT( buf, -1 ) ;
      }
      DBGPRINT( " of thread", -1 ) ;
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
      break ;
  }
    
}

static void print_strided( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{
  int count = 0 ;
  int n ;
  JD_data **dt_array ;
  char **str_array ;
  int loop = 1 ;
  char *buf ;
  JD_data *st ;
  int i ;
  
  n = JD_count_hush_element( dt ) ;
  dt_array = (JD_data **)calloc(n, sizeof(JD_data *) ) ;
  str_array = (char **)calloc(n, sizeof(char *) ) ;
  set_hush_element( dt, dt_array, NULL, str_array ) ;
  
  quick_sort( dt_array, NULL, str_array, 0, n-1, 0 ) ;

  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_bit:
    case JD_D_int:
    case JD_D_float:
    case JD_D_double:
    case JD_D_string:
    case JD_D_signal:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "String indexed array of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      for( i = 0 ; i < n && loop ; i++ ) {
        buf = (char *)calloc( strlen(str_array[i])+5, sizeof(char) ) ;
        sprintf( buf, "[\"%s\"]=", str_array[i] ) ;
        JD_dbg_print_prefix = buf ;
        st = dt_array[i] ;
        print_data( st, NULL, indent+1 ) ; 
        free(buf) ;
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_object:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of class (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      for( i = 0 ; i < n && loop ; i++ ) {
        buf = (char *)calloc( strlen(str_array[i])+5, sizeof(char) ) ;
        sprintf( buf, "[\"%s\"]=", str_array[i] ) ;
        JD_dbg_print_prefix = buf ;
        st = dt_array[i] ;
        print_obj( st, scope, indent+1 ) ; 
        free(buf) ;
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_thread:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of thread", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
      break ;
  }
  
  free(dt_array) ;
  free(str_array) ;
  
}

static void print_list( JD_data *dt, JD_dbg_name_node *scope, int indent ) 
{
  int count = 0 ;
  JD_data *st ;
  JD_data_unit *ut ;
  char buf[100] ;
  int n ;
  int loop ;
  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_bit:
    case JD_D_int:
    case JD_D_float:
    case JD_D_double:
    case JD_D_string:
    case JD_D_signal:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "List of (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      ut = dt->data.list.top ;
      n = 0 ;
      loop = 1 ;
      while( loop && ut ) {
        sprintf( buf, "(%d)=", n++ ) ;
        JD_dbg_print_prefix = buf ;
        st = ut->data ;
        print_data( st, NULL, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
        ut = ut->next ;
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_object:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "List of class (", -1 ) ;
      DBGPRINT( JD_Debug_name_table[scope->dtype], -1 ) ;
      DBGPRINT( ") = {", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      ut = dt->data.list.top ;
      n = 0 ;
      loop = 1 ;
      while( loop && ut ) {
        sprintf( buf, "(%d)=", n++ ) ;
        JD_dbg_print_prefix = buf ;
        st = ut->data ;
        print_obj( st, scope, indent+1 ) ; 
        if( JD_dbg_print_interactive && (++count == PRINT_LIMIT) ) {
          loop = JD_dbg_prompt_user_reply() ;
          count = 0 ;
        }
        ut = ut->next ;
      }
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "}", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    case JD_D_thread:
      DBGPRINTINDENT( indent ) ;
      DBGPRINT( "Array of thread", -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( dt->type ) ;
      break ;
  }
  if( dt->data.list.sem ) {
    JD_thread *thrd ;
    thrd = dt->data.list.sem->queue->thrd_top ;
    DBGPRINTINDENT( indent ) ;
    DBGPRINT( "Queue in the list = {", -1 ) ;
    if( thrd ) {
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
      while(thrd) {
        DBGPRINTINDENT( indent+1 ) ;
        sprintf( buf, "Thread id(%d)", thrd->id ) ;
        DBGPRINT( buf, -1 ) ;
        if( thrd->dbg_stack ) {
          int line_num, file_index ;
          JD_debugger_get_location_of_stack( 
            thrd->dbg_stack, &line_num, &file_index
          ) ;
          sprintf( buf, "Line %d in", line_num ) ;
          DBGPRINT( buf, -1 ) ;
          DBGPRINT( JD_Debug_name_table[file_index], -1 ) ;
        }
        DBGPRINT( JD_dbg_print_end_char, -1 ) ;
        thrd = thrd->thrd_bw ;
      }
      DBGPRINTINDENT( indent ) ;
    }
    else DBGPRINT( "(null)", -1 ) ;
    DBGPRINT( "}", -1 ) ;
    DBGPRINT( JD_dbg_print_end_char, -1 ) ;
    
  }

}

static void print_dt( JD_dbg_expression_node *exp, int indent ) 
{
  JD_data *dt = exp->dt ;
  
  DBGPRINTINDENT( indent ) ;
  while( dt->type == JD_D_pointer ) {
    dt = (JD_data *)JD_get_ptr_element(dt) ;
  }
  
  switch( dt->sub_type ) {
    case JD_SD_single:
      switch( dt->type ) {
        case JD_D_object:
          print_obj( dt, exp->scope, indent ) ;
          break ;
        case JD_D_thread:
          print_data( dt, exp->scope, indent ) ;
          break ;
        case JD_D_signal:
          print_data( dt, exp->scope, indent ) ;
          break ;
      }
      break ;
    case JD_SD_array:
      print_array( dt, exp->scope, indent ) ;
      break ;
    case JD_SD_assoc:
      print_assoc( dt, exp->scope, indent ) ;
      break ;
    case JD_SD_marray:
      print_marray( dt, exp->scope, indent ) ;
      break ;
    case JD_SD_strided:
      print_strided( dt, exp->scope, indent ) ;
      break ;
    case JD_SD_list:
      print_list( dt, exp->scope, indent ) ;
      break ;
  }

  DBGPRINT( JD_dbg_print_end_char, -1 ) ;

}

static void print_aspect(
  JD_dbg_name_node *scope, int indent
) {
  JD_dbg_name_node *mem ;
  JD_dbg_name_node **members ;
  JD_data *mt ;
  int i, j, n ;
  int cls ;
  
  if( scope == NULL ) {
    return ;
  }
  
  n = 0 ;
  mem = scope->next ;
  while( mem ) {
    if( mem->type == JD_dbg_avar_type ) n++ ;
    mem = mem->next ;
  }
  members = (JD_dbg_name_node **)calloc(n, sizeof(JD_dbg_name_node *) ) ;
  mem = scope->next ;
  i = 0 ;
  while( mem ) {
    if( mem->type == JD_dbg_avar_type ) {
      members[i++] = mem ;
    }
    mem = mem->next ;
  }
  
  /* here we do a stupid bubble sort */
  for( i = 0 ; i < n ; i++ ) {
    for( j = i+1 ; j < n ; j++ ) {
      if( members[i]->index > members[j]->index ) {
        mem = members[i] ;
        members[i] = members[j] ;
        members[j] = mem ;
      }
    }
  }
  
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( JD_Debug_name_table[scope->name], -1 ) ;
  DBGPRINT( " (aspect) = {", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  for( i = 0 ; i < n ; i++ ) {
    mem = members[i] ;
    mt = GLOBAL_DATA[mem->index] ;
    print_data( mt, mem, indent+1 ) ;
  }
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( "}", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  free( members ) ;
}

static void print_portset( JD_dbg_expression_node *exp, int indent ) 
{
  int i, n ;
  JD_dbg_name_node *port ;
  JD_data *dt ;
  
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( "portset (", -1 ) ;
  DBGPRINT( exp->name, -1 ) ;
  DBGPRINT( ") {", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
  
  n = 0 ;
  port = exp->scope->next ;
  while( port ) {
    dt = GLOBAL_DATA[port->index] ;
    print_data( dt, port, indent+1 ) ;
    port = port->next ;
  }
  DBGPRINTINDENT( indent ) ;
  DBGPRINT( "}", -1 ) ;
  DBGPRINT( JD_dbg_print_end_char, -1 ) ;
}

void JD_dbg_print_expression( 
  JD_dbg_expression_node *exp,
  int indent
) {
  str_size = 0 ;  /* size of the allocated string */
  str_ptr = 0 ;  /* write pointer to the string */

  JD_dbg_eval_exp(exp) ;
  
  switch( exp->type ) {
    case JD_D_bit:
      print_bit( 
        exp->bit_a, exp->bit_b, exp->size, 
        indent
      ) ;
      break ;
    case JD_D_int:
      print_int(
        exp->int_value, indent
      ) ;
      break ;
    case JD_D_float:
      print_real(
        exp->real_value, indent
      ) ;
      break ;
    case JD_D_string:
      print_string( exp->string, indent ) ;
      break ;
    case JD_D_null:
      print_string( "(null)", indent ) ;
      break ;
    case JD_D_dt:
      print_dt( exp, indent ) ;
      break ;
    case JD_D_parent:
      if( exp->etype == JD_dbg_exp_aspect ) {
        print_aspect( exp->scope, indent ) ;
      }
      else {
        print_portset( exp, indent ) ;
      }
      break ;
    default:
      DBG_ERROR_UNKNOWN_TYPE_FOR_PRINT( exp->type ) ;
      break ;
  }
  
}

static int is_active( char *fname, int lnum ) {
  if( !strcmp(fname, JD_Debug_name_table[JD_debugged_stack->file_index]) &&
      JD_debugged_stack->line_num == lnum
    ) return 1 ;
  else return 0 ;
}

static char dbgbuf[1024] ;

static int list_file(
  FILE *fl, char *fname, int startp, int endp
) {
  int i ;
  int c ;
  
  i = 1 ;
  
  if( i >= startp ) {
    if( is_active(fname, i) ) 
      sprintf( dbgbuf, "%4d   >", i ) ;
    else
      sprintf( dbgbuf, "%4d    ", i ) ;
      
    DBGPRINT( dbgbuf, -1 ) ;
  }

  while( (c = getc(fl)) != EOF ) {
    if( i >= startp ) { 
      sprintf( dbgbuf, "%c", c ) ; 
      DBGPRINT( dbgbuf, -1 ) ;
    }
    if( c == '\n' ) {
      if( ++i > endp ) {
        break ;
      }
      if( i >= startp ) {
        if( is_active(fname, i) ) 
          sprintf( dbgbuf, "%4d   >", i ) ;
        else
          sprintf( dbgbuf, "%4d    ", i ) ;
        DBGPRINT( dbgbuf, -1 ) ;
      }
    }
  }
  DBGPRINT( "\n", -1 ) ;
  return i ;
}

void JD_dbg_list_file( 
  int type, char *cwd, char *filename, int linenum, int num 
) {
  FILE *fl ;
  char *fullname ;
  int startp, endp, lastp ;
  
  switch( type ) {
    case 0:  /* <num> */
      fullname = 
        (char *)calloc( 
          strlen( JD_Debug_name_table[JD_debugged_stack->cwd_index] ) +
          strlen( JD_Debug_name_table[JD_debugged_stack->file_index] ) + 5,
          sizeof(char)
        ) ;
      filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
      if( filename[0] == '/' ) {
        strcat( fullname, filename ) ;
      }
      else {
        strcat( fullname, JD_Debug_name_table[JD_debugged_stack->cwd_index] ) ;
        strcat( fullname, "/" ) ;
        strcat( fullname, filename ) ;
      }
      startp = JD_debugged_stack->lnum - 1 ;
      if( startp < 0 ) startp = 0 ;
      endp = startp + 10 ;
      break ;
    case 1:  /*  -  */
      fullname = 
        (char *)calloc( 
          strlen( JD_Debug_name_table[JD_debugged_stack->cwd_index] ) +
          strlen( JD_Debug_name_table[JD_debugged_stack->file_index] ) + 5,
          sizeof(char)
        ) ;
      filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
      if( filename[0] == '/' ) {
        strcat( fullname, filename ) ;
      }
      else {
        strcat( fullname, JD_Debug_name_table[JD_debugged_stack->cwd_index] ) ;
        strcat( fullname, "/" ) ;
        strcat( fullname, filename ) ;
      }
      startp = JD_debugged_stack->lnum + linenum ;
      if( startp < 0 ) startp = 0 ;
      if( num > 0 && num > startp ) endp = num ;
      else endp = startp + 10 ;
      break ;
    case 2: /* file:lnum [[,]endnum]*/
    case 3: /* file:func [[,]endnum] */
      fullname = 
        (char *)calloc( strlen(cwd)+strlen(filename)+5, sizeof(char) );
      if( filename[0] == '/' ) {
        strcat( fullname, filename ) ;
      }
      else {
        strcat( fullname, cwd ) ;
        strcat( fullname, "/" ) ;
        strcat( fullname, filename ) ;
      }
      startp = linenum ;
      if( startp < 0 ) startp = 0 ;
      if( num > 0 && num > startp ) endp = num ;
      else endp = startp + 10 ;
      break ;
    case 4:  /* show current */
      fullname = 
        (char *)calloc( 
          strlen( JD_Debug_name_table[JD_debugged_stack->cwd_index] ) +
          strlen( JD_Debug_name_table[JD_debugged_stack->file_index] ) + 5,
          sizeof(char)
        ) ;
      filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
      if( filename[0] == '/' ) {
        strcat( fullname, filename ) ;
      }
      else {
        strcat( fullname, JD_Debug_name_table[JD_debugged_stack->cwd_index] ) ;
        strcat( fullname, "/" ) ;
        strcat( fullname, filename ) ;
      }
      startp = JD_debugged_stack->line_num - num ;
      if( startp < 0 ) startp = 0 ;
      endp = JD_debugged_stack->line_num + num ;
      break ;
    default:
      return ;
  }
  fl = fopen( fullname, "r") ;
  if( !fl ) {
    DBG_ERROR_CANNOT_OPEN_FILE( fullname ) ;
  }
  else {
    lastp = list_file( fl, filename, startp, endp ) ;
    JD_debugged_stack->lnum = lastp ;
    fclose( fl ) ;
  }
  free( fullname ) ;
}

void JD_debugger_show_breaks() {
  JD_dbg_break_point *bp ;
  char buf[100] ;
  
  bp = JD_debug_top_break_point ;
  
  while( bp ) {
    DBGPRINT( JD_Debug_name_table[bp->filenum], -1 ) ;
    DBGPRINT( ":", -1 ) ;
    sprintf( buf, "%d", bp->linenum ) ;
    DBGPRINT( buf, -1 ) ;
    DBGPRINT( "\n", -1 ) ;
    bp = bp->next_list ;
  }

}



void JD_debugger_window_show_current() {
  char *fullname ;
  char *filename ;
  JD_dbg_break_point *bp ;
  char buf[100] ;
  fullname = 
    (char *)calloc( 
      strlen( JD_Debug_name_table[JD_debugged_stack->cwd_index] ) +
      strlen( JD_Debug_name_table[JD_debugged_stack->file_index] ) + 5,
      sizeof(char)
    ) ;
  filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
  if( filename[0] == '/' ) {
    strcat( fullname, filename ) ;
  }
  else {
    strcat( fullname, JD_Debug_name_table[JD_debugged_stack->cwd_index] ) ;
    strcat( fullname, "/" ) ;
    strcat( fullname, filename ) ;
  }
  strcat( fullname, "\n" ) ;
  JD_dbg_write_window( "#file\n" ) ;
  JD_dbg_write_window( fullname ) ;
  JD_dbg_write_window( "#location\n" ) ;
  sprintf( fullname, "%d\n", JD_debugged_stack->line_num ) ;
  JD_dbg_write_window( fullname ) ;
  
  JD_dbg_write_window( "#threadlist\n" ) ;
  JD_dbg_print_out_select = 2 ;
  JD_debugger_show_threads() ;
  JD_dbg_write_window( "#end\n" ) ;
  JD_dbg_write_window( "#breaklist\n" ) ;
  JD_debugger_show_breaks() ;
  JD_dbg_write_window( "#end\n" ) ;
  JD_dbg_print_out_select = 0 ;

  bp = JD_debug_top_break_point ;
  
  while( bp ) {
    if( bp->filenum == JD_debugged_stack->file_index ) {
      if( bp->temp_flag) JD_dbg_write_window( "#tbreakpoint\n" ) ;
      else JD_dbg_write_window( "#breakpoint\n" ) ;
      sprintf( buf, "%d\n", bp->linenum ) ;
      JD_dbg_write_window( buf ) ;
    }
    bp = bp->next_list ;
  }
  
}

void JD_debugger_show_local_vars(){
  int n ;
  JD_dbg_name_node *var ;
  JD_thread *thrd ;
  JD_data *dt ;
  
  thrd = JD_debugged_stack->thrd ;
  
  n = 0 ;
  while( var = (JD_dbg_name_node *)JD_debugger_lookup_local_var( n ) ) {
    dt = thrd->stack[JD_debugged_stack->stk_frame + var->index + 1] ;
    print_data( dt, var, 0 ) ;
    n = var->index ;
    n++ ;
  }
  
}

void JD_debugger_show_vars(){
  int n ;
  JD_dbg_name_node *clss, *var ;
  JD_thread *thrd ;
  JD_data *dt ;
  
  
  DBGPRINT( "----< Global Scope Variables >----\n", -1 ) ;
  n = 0 ;
  while( var = (JD_dbg_name_node *)JD_debugger_lookup_global_scope_var( n ) ) {
    if( var->type == JD_dbg_var_type ) {
      dt = GLOBAL_DATA[var->index] ;
      print_data( dt, var, 1 ) ;
    }
    n++ ;
  }
  
  DBGPRINT( "----< Local Scope Variables >----\n", -1 ) ;
  n = 0 ;
  while( var = (JD_dbg_name_node *)JD_debugger_lookup_local_scope_var( n ) ) {
    if( var->type == JD_dbg_var_type ) {
      dt = GLOBAL_DATA[var->index] ;
      print_data( dt, var, 1 ) ;
    }
    n++ ;
  }
  if( JD_debugged_stack->inclass && JD_debugged_stack->this && JD_debugged_stack->this->data.obj.ect ) {
    int cls ;
  
    cls = JD_debugger_lookup_class() ;
    if( cls < 0 ) {
      DBG_ERROR_CLASS_SCOPE_CANT_FOUND_IN_CURRENT_CONTEXT;
      return ;
    }
    clss = 
      (JD_dbg_name_node *)JD_debugger_lookup_name( 
        JD_Debug_name_table[cls], 0 
      ) ;
    if( clss == NULL ) {
      DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
        JD_Debug_name_table[cls] 
      );
      return ;
    }
    
    var = (JD_dbg_name_node *)JD_debugger_lookup_name( "this", 0 ) ;
    if( var == NULL ) {
      DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( "this" ) ;
      return ;
    }

    DBGPRINT( "----< Class (", -1 ) ;
    DBGPRINT( JD_Debug_name_table[cls], -1 ) ;
    DBGPRINT( ") Member Variables >----\n", -1 ) ;
    var->dtype = cls ;
    print_obj( JD_debugged_stack->this, var, 2 ) ;
  }  
  if( JD_debugged_stack->inaspect ) {
    int cls ;
  
    cls = JD_debugger_lookup_class() ;
    if( cls < 0 ) {
      DBG_ERROR_CLASS_SCOPE_CANT_FOUND_IN_CURRENT_CONTEXT;
      return ;
    }
    clss = 
      (JD_dbg_name_node *)JD_debugger_lookup_name( 
        JD_Debug_name_table[cls], 0 
      ) ;
    if( clss == NULL ) {
      DBG_ERROR_DATA_TYPE_NAME_NOT_FOUND( 
        JD_Debug_name_table[cls] 
      );
      return ;
    }
    
    DBGPRINT( "----< Aspect (", -1 ) ;
    DBGPRINT( JD_Debug_name_table[cls], -1 ) ;
    DBGPRINT( ") Member Variables >----\n", -1 ) ;
    print_aspect( clss, 2 ) ;
  }  
  
  DBGPRINT( "----< Auto Variables >----\n", -1 ) ;
  thrd = JD_debugged_stack->thrd ;
  n = 0 ;
  while( var = (JD_dbg_name_node *)JD_debugger_lookup_local_var( n ) ) {
    dt = thrd->stack[JD_debugged_stack->stk_frame + var->index + 1] ;
    print_data( dt, var, 2 ) ;
    n = var->index ;
    n++ ;
  }
}

static void print_thread_info( char *pref, JD_debug_stack *stack, int indent ) {
  char *buf ;
  JD_debug_stack *tmp ;
  buf =
    (char *)calloc( 
      strlen(JD_Debug_name_table[stack->file_index]) + 
      strlen(pref) + 100, 
      sizeof(char)
    ) ;
  if( stack->active ) {
    if( stack->callee ) {
      sprintf( 
        buf, "%sline %d in \"%s\" call-> ", 
        pref, stack->line_num, JD_Debug_name_table[stack->file_index]
      ) ;
      print_thread_info( buf, stack->callee, indent ) ;
    }
    else {
      DBGPRINTINDENT( indent ) ;
      if( stack->active == 1 ) {
        sprintf( 
          buf, "%sThreadID(%d) line %d in \"%s\" ", 
          pref, stack->thrd->id,
          stack->line_num, JD_Debug_name_table[stack->file_index]
        ) ;
      }
      else {
        /* this thread is not yet started */
        sprintf( 
          buf, "%sThreadID(%d) (Not started) ", 
          pref, stack->thrd->id
        ) ;
      }
      DBGPRINT( buf, -1 ) ;
      DBGPRINT( JD_dbg_print_end_char, -1 ) ;
    }
    if( stack->hb ) {
      print_thread_info( pref, stack->hb, indent ) ;
    }
    tmp = stack->forkee ;
    while( tmp ) {
      sprintf( 
        buf, "%sThreadID(%d) line %d in \"%s\" fork-> ", 
        pref, stack->thrd->id,
        stack->line_num, JD_Debug_name_table[stack->file_index]
      ) ;
      print_thread_info( buf, tmp, indent+1 ) ;
      tmp = tmp->sb ;
    }
  }
  else {
    if( stack->callee ) {
      sprintf( 
        buf, "%s(NA ID(%d) @ %d) line %d in \"%s\" call-> ", 
        pref, stack->thrd_id, stack->cycle, 
        stack->line_num, JD_Debug_name_table[stack->file_index]
      ) ;
      print_thread_info( buf, stack->callee, indent ) ;
    }
    if( stack->forkee ) {
      tmp = stack->forkee ;
      while( tmp ) {
        sprintf( 
          buf, "%s(NA ID(%d) @ %d) line %d in \"%s\" fork-> ", 
          pref, stack->thrd_id, stack->cycle,
          stack->line_num, JD_Debug_name_table[stack->file_index]
        ) ;
        print_thread_info( buf, tmp, indent ) ;
        tmp = tmp->sb ;
      }
    }
    if( stack->hb ) {
      print_thread_info( pref, stack->hb, indent ) ;
    }
  }
  
  free( buf ) ;
}

void JD_debugger_show_local_threads(){
  char *pref ;
  JD_debug_stack *stack ;
  stack = JD_debugged_stack ;
  
  pref = (char *)JD_debugger_gen_pref_stack( stack ) ;
  print_thread_info( pref, stack, 0 ) ;
  free(pref) ;
  
}

extern JD_toplevel *JD_current_toplevel ;

void JD_debugger_show_threads() {
  JD_debug_stack *stack ;
  stack = JD_current_toplevel->top_dbg_stack ;
  
  print_thread_info( "", stack, 0 ) ;
  
}
