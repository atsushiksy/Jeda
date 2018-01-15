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
#ifndef CODEGEN_MACROS

#define CODEGEN_MACROS

extern int code_indent ;
void out_indent() ;

#define OUT0(a) { out_indent() ; fprintf( out, a ) ; }
#define OUT1(a,b) { out_indent() ; fprintf( out, a, b ) ; }
#define OUT2(a,b,c) { out_indent() ; fprintf( out, a, b, c ) ; }
#define OUT3(a,b,c,d) { out_indent() ; fprintf( out, a, b, c, d ) ; }
#define OUT4(a,b,c,d,e) { out_indent() ; fprintf( out, a, b, c, d, e ) ; }
#define OUT5(a,b,c,d,e,f) { out_indent() ; fprintf( out, a, b, c, d, e, f ) ; }
#define OUT6(a,b,c,d,e,f,g) \
  { out_indent() ; fprintf( out, a, b, c, d, e, f, g ) ; }
#define OUT7(a,b,c,d,e,f,g,h) \
  { out_indent() ; fprintf( out, a, b, c, d, e, f, g, h ) ; }
#define OUT8(a,b,c,d,e,f,g,h,i) \
  { out_indent() ; fprintf( out, a, b, c, d, e, f, g, h, i ) ; }
#define OUT9(a,b,c,d,e,f,g,h,i,j) \
  { out_indent() ; fprintf( out, a, b, c, d, e, f, g, h, i, j ) ; }
#define OUT10(a,b,c,d,e,f,g,h,i,j,k) \
  { out_indent() ; fprintf( out, a, b, c, d, e, f, g, h, i, j, k ) ; }


#define VOUT0(a) fprintf( vout, a ) 
#define VOUT1(a,b) fprintf( vout, a, b ) 
#define VOUT2(a,b,c) fprintf( vout, a, b, c ) 
#define VOUT3(a,b,c,d) fprintf( vout, a, b, c, d ) 
#define VOUT4(a,b,c,d,e) fprintf( vout, a, b, c, d, e ) 
#define VOUT5(a,b,c,d,e,f) fprintf( vout, a, b, c, d, e, f ) 
#define VOUT6(a,b,c,d,e,f,g) fprintf( vout, a, b, c, d, e, f, g ) 
#define VOUT7(a,b,c,d,e,f,g,h) fprintf( vout, a, b, c, d, e, f, g, h ) 
#define VOUT8(a,b,c,d,e,f,g,h,i) fprintf( vout, a, b, c, d, e, f, g, h, i ) 
#define VOUT9(a,b,c,d,e,f,g,h,i,j) \
  fprintf( vout, a, b, c, d, e, f, g, h, i, j ) 
#define VOUT10(a,b,c,d,e,f,g,h,i,j,k) \
  fprintf( vout, a, b, c, d, e, f, g, h, i, j, k ) 
#define VOUT11(a,b,c,d,e,f,g,h,i,j,k,l) \
  fprintf( vout, a, b, c, d, e, f, g, h, i, j, k, l ) 



#endif
