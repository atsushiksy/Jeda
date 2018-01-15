/*
  Data structures for static random
 */
/* 
   
   Copyright (C) 2003 Jeda Technologies, Inc.

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
typedef struct JD_static_random_s JD_static_random ;
typedef struct JD_static_rnd_range_s JD_static_rnd_range ;

typedef enum {
  JD_static_rnd_int_type,
  JD_static_rnd_rng_type,
  JD_static_cyc_int_type,
  JD_static_cyc_rng_type
} JD_static_rnd_type ;

struct JD_static_rnd_range_s {
  int weight ;
  int flag ;   /* 0: single 1: single weight 2: double weight */
  int weight_s ;
  int weight_e ;
  int u_size ;
  unsigned int *u_data ;
  int l_size ;
  unsigned int *l_data ;
  int num_element ;
  JD_static_rnd_range *next ;
} ;

struct JD_static_random_s {
  JD_static_rnd_type type ;
  int seed ;
  JD_mt_random  *rnd ;
  int size ;
  int num_element ;
  int limit ;
  int width ;
  int act_width ;
  JD_static_rnd_range *range ;
  unsigned int rnd_data[3] ;
  unsigned int data[3] ;
  unsigned int sdata[3] ;
  unsigned int zdata[3] ;
  unsigned int pdata[3] ;
  unsigned int mdata[3] ;
  unsigned int ppdata[3] ;
  unsigned int udata[3] ;
  int next_zero ;
  int swap_point ;
  
} ;

