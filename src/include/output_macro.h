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
#define STR_TXT    "string"
#define VOID_TXT   "void"
#define NULL_TXT   "null"
#define INT_TXT    "int"
#define BIT_TXT    "bit"
#define FLOAT_TXT  "float"
#define PTSET_TXT  "ptset"
#define SIGNAL_TXT "signal"
#define ENUM_TXT   "enum"
#define FLOAT_TXT  "float"
#define OBJ_TXT    "obj"
#define ARRY_TXT   "array"
#define MARRY_TXT  "marray"
#define ASOC_TXT   "assoc"
#define STIX_TXT   "stridx"
#define LIST_TXT   "list"
#define WHAT_TXT   "what!!"

#define NO_TYP    0
#define STR_TYP    1
#define VOID_TYP   2
#define NULL_TYP   3
#define INT_TYP    4
#define BIT_TYP    5
#define FLOAT_TYP  6
#define PTSET_TYP  7
#define SIGNAL_TYP 8
#define ENUM_TYP   9
#define OBJ_TYP    10
#define ARRY_TYP   11
#define MARRY_TYP  12
#define ASOC_TYP   13
#define STIX_TYP   14
#define LIST_TYP   15

#define NO_OP      0
#define MINUS_OP   1
#define PLUS_OP    2
#define TIMES_OP   3
#define DIV_OP     4
#define MOD_OP     5
#define AND_OP     6
#define OR_OP      7
#define EOR_OP     8
#define NAND_OP    9
#define NOR_OP     10
#define NEOR_OP    11

#define OUT0(a) fprintf( out, a ) 
#define OUT1(a,b) fprintf( out, a, b ) 
#define OUT2(a,b,c) fprintf( out, a, b, c ) 
#define OUT3(a,b,c,d) fprintf( out, a, b, c, d ) 
#define OUT4(a,b,c,d,e) fprintf( out, a, b, c, d, e ) 
#define OUT5(a,b,c,d,e,f) fprintf( out, a, b, c, d, e, f ) 

#define HOUT0(a) fprintf( hout, a ) 
#define HOUT1(a,b) fprintf( hout, a, b ) 
#define HOUT2(a,b,c) fprintf( hout, a, b, c ) 
#define HOUT3(a,b,c,d) fprintf( hout, a, b, c, d ) 
#define HOUT4(a,b,c,d,e) fprintf( hout, a, b, c, d, e ) 
#define HOUT5(a,b,c,d,e,f) fprintf( hout, a, b, c, d, e, f ) 

#define OUTPORT(x,y) \
  fprintf( out, "$%s  ; port %d %d\n", x->name, x->info.var.index, y ) 

#define OUTLOCAL(x,y) \
  fprintf( out, "%d  ; local var %s %d\n", x->info.var.index, x->name, y ) 

#define OUTGLOBAL(x,y) \
  fprintf( out, "$%s  ; global var %d %d\n", x->name, x->info.var.index, y ) 

#define OUTPSETPORT(x,y,z) \
  fprintf( out, "$%s $%s  ; pset.port(%d) %d\n", x, y->name, y->info.var.index, z )

#define OUTMEMBER(x,y,z) \
  fprintf( out, "$%s $%s  ; member(%d) %d\n", x, y->name, y->info.var.index, z )

#define PUSH_ALU(x) fprintf( out, "  push  alu  ; %d\n", x ) 
#define POP_ALU(x) fprintf( out, "  pop  alu  ; %d\n", x ) 
#define TO_INT(x) fprintf( out, "  to_int  ; ACC -> integer %d\n", x ) 
#define LOAD_ZERO(x) fprintf( out, "  load_zero ; ACC = 0 %d\n", x ) 
#define LOAD_ONE(x) fprintf( out, "  load_one ; ACC = 1 %d\n", x ) 

#define ARRAY_TYPE (variable->sub_type==JD_array_sub_type)?"array":"assoc"

#define DIM_OUT \
  if(variable->sub_type==JD_array_sub_type) \
    OUT1( "%d ", variable->info.var.range->is.range.int_ub )
#define NOT_INT(x) strcmp( x, INT_TXT ) 

#define POP(x, y) fprintf( out, "  pop %d  ; discard %d from stack %d\n", x, x, y ) 
#define COPY_ALU(x,y) fprintf( out, "  copy alu %0d  ; ACC <- st[%d] %d\n", x, x, y ) 


#define OUT_LABEL(x,y) fprintf( out, "l%d:  ; %d\n", x, y ) 
