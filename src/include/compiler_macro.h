/***************************************************************************
 *
 *  compiler_macro.h: Various macro declarations for compiler
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#ifndef COMPILER_MACROS

#define COMPILER_MACROS

void *check_alloc( int ) ;

/* defining names of programs */
#define JDCMP "jedacmp"
#define JDPP  "jedapp"

/* defining file extentions */
#define JDOEXT ".jo"
#define JDHEXT ".jh"

#define CMPVERSION "0"
#define CMPREVISION "0"

/* defining release home environment variable name */
#define JEDA_HOME "JEDA_HOME"

#define ALLOC(x) check_alloc(sizeof(x))

#define DEALOC(x) check_free(x)

#define ERROR_LOCATION(x,y) fprintf(stderr, "%s:%d: ", x, y )

/* number of 32-bit word for bit length x */
#define NWORD(x) ( (x-1)/32 + 1 )
#define LASTWORD(x) ( (x-1)/32  )

#define NBYTE(x) ( sizeof(int) * ( (x-1)/32 + 1 ) )

#define GETSHORT(x,l)  ( (l & 1) ? ((x[l>>1] >> 16) & 0xffff) : x[l>>1] & 0xffff )

#define SETSHORT(x,y,l)  x[l>>1] = (l & 1) ? (x[l>>1] & 0xffff)|(y<<16) : (x[l>>1] & 0xffff0000) | (y & 0xffff)

#define BIT_RIGHT_SHIFT(x,n,l) \
  for( i = 0 ; i <= n ; i++ ) \
    x[i] =  \
      ( ( i+(l/32) > n )    ? 0 : ((unsigned)x[i+(l/32)] >> (l % 32) ) ) | \
      ( ( (i+(l/32)+1) > n) ? 0 : (x[i+(l/32)+1]) << (32-(l % 32)) )


#define BIT_LEFT_SHIFT(x,n,l) \
  for( i = n ; i >= 0 ; i-- ) \
    x[i] =  \
      ( ( i-(l/32) < 0 )    ? 0 : ((unsigned)x[i-(l/32)] << (l % 32) ) ) | \
      ( ( (i-(l/32)-1) < 0 ) ? 0 : ((unsigned)x[i-(l/32)-1]) >> (32-(l % 32)) )

#define GET_BIT(x,l)  ( ( x[l/32] >> (l%32) ) & 1 )

#define SKIP_ON_ERROR  if( error_flag ) break

#define ERROR_CLR_BLOCK if( error_flag ) { error_flag = 0 ; } else
#define PASS0 if ( compile_pass==0 )

#define PASS1 if ( compile_pass==1 )

#define PASS2 if ( compile_pass==2 )

#define PASS0_1 if ( compile_pass==0 || compile_pass==1 )

#define PASS1_2 if ( compile_pass==1 || compile_pass==2 )

#define CLEAR_VAR_FUNC_ATTR_FLAGS \
  is_export_flag = \
  is_static_flag = is_extern_flag = is_local_flag = is_protected_flag =  \
  is_abstract_flag = is_final_flag = is_global_flag = is_clone_flag = \
  is_overwrite_flag = 0 
  
  
#endif

