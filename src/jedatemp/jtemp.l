%{
/*
 *  Jeda Template Generator
 *
 *  The lex part of partial (quick & dirty) verilog parser. 
 *
 *  Author: Atsushi Kasuya
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
#include "jtemp.h"
#include "jtemp.tab.h"

char *last_text = NULL ;

static void set_last( char *type, char *token ) ;

char *last_string = NULL ;

unsigned lex_line_num ;

%}

newline     \n
Space       [\r\t\b ]
Alpha       [a-zA-Z]
AlphaU      [a-zA-Z_]
AlphaNum    [a-zA-Z0-9]
AlphaNumU   [a-zA-Z0-9_]
Digit       [0-9]
DigitU      [0-9_]
Number      {Digit}{DigitU}*
Decimal     ({Number})?'[dD]{Number}
Octal       ({Number})?'[oO][0-7xXzZ?][0-7xXzZ?_]*
Hexdecimal  ({Number})?'[hH][0-9a-fA-FxXzZ?][0-9a-fA-FxXzZ?_]*
Binary      ({Number})?'[bB][01xXzZ?][01xXzZ?_]*
Operators   [\[\]\{\}\:\,\.\;\+\-\*\/\(\)]

%%

{Space}+  { continue ; }
{newline}  {
  lex_line_num++ ;
}
<<EOF>>  { lex_line_num-- ;  return 0 ; }

"/*"         { skipcommentblock() ; continue ; }
"//"[^\n]*\n { continue ; }
module             { return YYMODULE; }
endmodule          { return YYENDMODULE; }
inout              { return YYINOUT; }
input              { return YYINPUT; }
output             { return YYOUTPUT; }
primitive          { return YYPRIMITIVE; }
endprimitive       { return YYENDPRIMITIVE; }
`[^\n]*\n          { continue ; /* may be time scale? */ }
{AlphaU}{AlphaNumU}* { 
				yylval.charp = (char *)strdup(&yytext[0]) ;
                                return YYID; 
                              }
\\[^\n\t\b\r ]*      { 
				yylval.charp = (char *)strdup(&yytext[0]) ;
                                return YYID; 
                              }
\"[^"\n]* {    
    /* begining of string */
    int c ;
    
    c = input() ;
    if( c == '\n' ) {
      if( yytext[yyleng-1] == '\\' ) {
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng-1] = '\0' ;
          strcat( last_string, yytext ) ;
        }
        yymore() ;  /* set flag to append input to the end of yytext */
        unput( '\"' ) ;  /* next search goes to string again */
        /* CONTINUE */
      }
      else {
        fprintf( stderr, "jtemp lex error: <cr> found in string\n" ) ;
        c = input() ;
        while( c != '\"' ) {
          if( c == '\n' ) {
            lex_line_num++ ;
          }
          c = input() ;
        }
        unput(c) ;
        return YYSTRING ; 
      }
    }
    else {
      /* detect " */
      if( yytext[yyleng-1] == '\\' ) {
        /* got excaped ", so need to continue scanning string */
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
        }
        /* yymore() ;  * set flag to append input to the end of yytext */
        unput( c ) ;  /* unput ", so next search goes to string again */
        /* CONTINUE */
      }
      else {
        yytext[yyleng++] = c ; /* this is " */
        yytext[yyleng++] = '\0' ;
        if( last_string ) {
          char *p ;
          int len ;
          p = last_string ;
          len = strlen(p) ;
          last_string = (char *)calloc(len+yyleng+1, sizeof(char) ) ;
          strcat( last_string, p ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
          free(p) ;
        }
        else {
          last_string = (char *)calloc(yyleng+1, sizeof(char) ) ;
          yytext[yyleng] = '\0' ;
          strcat( last_string, yytext ) ;
        }
        set_last( "string", yytext ) ;
        return YYSTRING ;
      }
    }
  }

{Number}*\.{Number}+ { 
  yylval.charp = &yytext[0] ;
  set_last( "number", yytext ) ;
  return YYRNUMBER; 
}
  
{Number}+\.{Number}* { 
  yylval.charp = &yytext[0] ;
  set_last( "number", yytext ) ;
  return YYRNUMBER; 
}

{Number}             { 
  yylval.ival = scan_decimal( 32, &yytext[0] ) ;
  set_last( "number", yytext ) ;
  return YYINUMBER; 
}

{Binary}             {
  yylval.ival = scan_v_binary( &yytext[0] ) ;
  set_last( "number", yytext ) ;
  return YYINUMBER ; 
}

{Octal}              {
  yylval.ival = scan_v_octal( &yytext[0] ) ;
  set_last( "number", yytext ) ;
 return YYINUMBER ; 
}

{Decimal}            {
  yylval.ival = scan_v_decimal( &yytext[0] ) ;
  set_last( "number", yytext ) ;
  return YYINUMBER ; 
}

{Hexdecimal}         {
  yylval.ival = scan_v_hex( &yytext[0] ) ;
  set_last( "number", yytext ) ;
  return YYINUMBER ; 
}
{Operators}          { 
  set_last( "operator", yytext ) ;
  return yytext[0]; }
.                    { 
  set_last( "other", yytext ) ;
  return YYOTHER ; }

%%    

int yywrap()
{
return 1;
}

skipcommentblock()
{
    int done, level = 0;
    char c;

    for (done=0; !done; ) {
        if ((c = input()) == '*') {
            if ((c = input()) == '/') {
                done = (level-- == 0); 
            } else {
                unput(c);
            }
	} else if (c == '/') {
	    if ((c = input()) == '*') {
		level++;
	    } else {
		unput(c);
	    }
        } else if (c == 0) {
            fprintf(stderr, "incomplete comment detected\n");
            exit(1) ;
        }
        
    }
}
 /**************************************************************
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

int scan_decimal( int size, char *number )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  unsigned *bit_c ;
  int ovflow = 0 ;
  int glen ;
  int i ;
  int ret ;
  
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
          if( !ovflow ) {
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
  
  ret = bit_a[0] ;
  for( i = 1 ; i < glen ; i++ ) {
    if( bit_a[i] ) ret = -1 ;
  }
  free( bit_a ) ;
  free( bit_b ) ;
  free( bit_c ) ;

  return ret ;

}


int scan_v_decimal( char *number )
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
      return -1 ;
    }
  }
  
  
  cp++ ; /* skip d/D */
  
  return scan_decimal( size, cp ) ;
  
}

int scan_hex( int size, char *number )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;
  int ret ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  
  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-4, 4) || get_bit(bit_b, size-4, 4) ) {
        if( !ovflow ) {
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

  if( bit_b[0] ) ret = -1 ;
  else ret = bit_a[0] ;
  
  for( i = 1 ; i < glen ; i++ ) {
    if( bit_a[i] || bit_b[i] ) ret = -1 ;
  }
  free( bit_a ) ;
  free( bit_b ) ;
  
  return ret ;
  
}

int scan_v_hex(  char *number )
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
      return -1 ;
    }
  }
  
  cp++ ; /* skip h/H */
  
  return scan_hex( size, cp ) ;

}


int scan_octal( int size, char *number )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;
  int ret ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-3, 3) || get_bit(bit_b, size-3, 3) ) {
        if( !ovflow ) {
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

  if( bit_b[0] ) ret = -1 ;
  else ret = bit_a[0] ;
  
  for( i = 1 ; i < glen ; i++ ) {
    if( bit_a[i] || bit_b[i] ) ret = -1 ;
  }
  free( bit_a ) ;
  free( bit_b ) ;
  
  return ret ;
  
}


int scan_v_octal( char *number )
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
      return -1 ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  return scan_octal( size, cp ) ;

}

int scan_binary( int size, char *number )
{
  char *cp = number ;
  unsigned *bit_a ;
  unsigned *bit_b ;
  int glen ;
  int i ;
  int ovflow = 0 ;
  int ext_flag = 0 ;
  int ret ;

  glen = (size-1)/32 + 1 ;
  
  bit_a = (unsigned *)calloc(glen, sizeof(unsigned) ) ;
  bit_b = (unsigned *)calloc(glen, sizeof(unsigned) ) ;

  while( *cp != '\0' ) {
    if( *cp != '_' ) { 
      if( get_bit(bit_a, size-1, 1) || get_bit(bit_b, size-1, 1) ) {
        if( !ovflow ) {
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

  if( bit_b[0] ) ret = -1 ;
  else ret = bit_a[0] ;
  
  for( i = 1 ; i < glen ; i++ ) {
    if( bit_a[i] || bit_b[i] ) ret = -1 ;
  }
  
  free( bit_a ) ;
  free( bit_b ) ;
  
  return ret ;
  
}


int scan_v_binary( char *number )
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
      return -1 ;
    }
  }
  
  cp++ ; /* skip o/O */
  
  return scan_binary( size, cp ) ;

}

static void set_last( char *type, char *token ){
  /* if( last_text ) free(last_text) ; */
  
  last_text = (char *)calloc( strlen(type)+strlen(token)+10, sizeof(char) ) ;
  
  strcat( last_text, type ) ;
  strcat( last_text, " \"" ) ;
  strcat( last_text, token ) ;
  strcat( last_text, "\"" ) ;
}
