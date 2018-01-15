/***************************************************************************
 *
 *  bit_operations.h: header for bit_operations.c
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
void JD_bit_minus_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_bit_sub_bit( int *A0, int S0, int *A1, int S1 ) ;
void JD_bit_plus_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_bit_add_bit( int *A0, int S0, int *A1, int S1 ) ;
void JD_bit_times_bit(
  int *AR, int *BR, int SR, int *A0, int *B0, int S0, int *A1, int *B1, int S1 
);
void JD_bit_div_bit(
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR                   /* error will be set on zero-divide */
);
void JD_bit_mod_bit(
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR 
);
void JD_bit_and_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ; 
void JD_bit_or_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ; 
void JD_bit_eor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_bit_nand_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_bit_nor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_bit_neor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) ;
void JD_u_minus_bit( int *A0, int *B0, int S0 ) ;
void JD_u_tilde_bit( int *A0, int *B0, int S0 ) ;
int JD_u_not_bit( int *A0, int *B0, int S0 ) ;
int JD_u_and_bit( int *A0, int *B0, int S0 ) ;
int JD_u_or_bit( int *A0, int *B0, int S0 ) ;
int JD_u_eor_bit( int *A0, int *B0, int S0 ) ;
int JD_u_nand_bit( int *A0, int *B0, int S0 ) ;
int JD_u_nor_bit( int *A0, int *B0, int S0 ) ;
int JD_u_neor_bit( int *A0, int *B0, int S0 ) ;
