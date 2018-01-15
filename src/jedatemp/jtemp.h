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
#define NWORD(x) ( (x-1)/32 + 1 )
#define OUT0(a) fprintf( out, a ) 
#define OUT1(a,b) fprintf( out, a, b ) 
#define OUT2(a,b,c) fprintf( out, a, b, c ) 
#define OUT3(a,b,c,d) fprintf( out, a, b, c, d ) 
#define OUT4(a,b,c,d,e) fprintf( out, a, b, c, d, e ) 
#define OUT5(a,b,c,d,e,f) fprintf( out, a, b, c, d, e, f ) 

typedef struct io_port_s io_port ;
typedef struct module_s module ;
typedef struct name_list_s name_list ;

struct io_port_s {
  int  dir ; /* 0:input 1:output 2: inout */
  int  ub ;
  int  lb ;
  char *name ;
  io_port *next ;
} ;

struct module_s {
  char *name ;
  io_port  *port ;
  module *next ;
} ;

struct name_list_s {
  char *name ;
  name_list *next ;
} ;
