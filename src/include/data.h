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

void JD_set_data_info(
  JD_data  *n_dt,
  JD_data  *n_ft,
  JD_data  *n_g0,
  JD_data  *n_g1
) ;

void JD_get_data_info(
  JD_data  **n_dt,
  JD_data  **n_ft,
  JD_data  **n_g0,
  JD_data  **n_g1
) ;

void JD_inital_data_allocation() ;

void JD_data_deallocation() ;

JD_data *JD_alloc_data_entry() ;
void JD_copy_single_data( JD_data *ret, JD_data *dt ) ;
JD_data *JD_duplicate_single_data( JD_data *dt ) ;
JD_hash_entry *JD_duplicate_hash_entry( JD_hash_entry *ent, JD_data *base ) ;

void move_data_to_gen1( JD_data *this ) ;

JD_data *JD_alloc_int() ;

JD_data *JD_alloc_bit( int BIT_SIZE ) ;

JD_data *JD_dup_bit( JD_data *dt ) ;

JD_data *JD_alloc_single_data( JD_data *dt ) ;

JD_data *JD_duplicate_data( JD_data *dt ) ;
JD_data *JD_copy_data( JD_data *ret, JD_data *dt ) ;
void JD_duplicate_data_to( JD_data *ret, JD_data *dt ) ;
void JD_push_tmp_dt() ;
void JD_alloc_single_int() ;
void JD_alloc_single_bit() ;
void JD_alloc_single_float() ;
void JD_alloc_single_double() ;
void JD_alloc_single_string() ;
void JD_alloc_single_signal() ;
void JD_alloc_single_obj() ;
void JD_alloc_single_thread() ;

JD_data *JD_alloc_single( JD_data *dt, char *info ) ;
JD_data *JD_alloc_int() ;
void JD_alloc_array_int() ;
void JD_alloc_array_bit() ;
void JD_alloc_array_float() ;
void JD_alloc_array_double() ;
void JD_alloc_array_string() ;
void JD_alloc_array_signal() ;
void JD_alloc_array_obj() ;
void JD_alloc_array_thread() ;
void JD_alloc_marray_dim() ;
void JD_set_marray_dim() ;
void JD_alloc_assoc_int() ;
void JD_alloc_assoc_bit() ;
void JD_alloc_assoc_float() ;
void JD_alloc_assoc_double() ;
void JD_alloc_assoc_string() ;
void JD_alloc_assoc_signal() ;
void JD_alloc_assoc_obj() ;
void JD_alloc_assoc_thread() ;
void JD_alloc_strided_int() ;
void JD_alloc_strided_bit() ;
void JD_alloc_strided_float() ;
void JD_alloc_strided_double() ;
void JD_alloc_strided_string() ;
void JD_alloc_strided_signal() ;
void JD_alloc_strided_obj() ;
void JD_alloc_strided_thread() ;
void JD_alloc_list_int() ;
void JD_alloc_list_bit() ;
void JD_alloc_list_float() ;
void JD_alloc_list_double() ;
void JD_alloc_list_string() ;
void JD_alloc_list_signal() ;
void JD_alloc_list_obj() ;
void JD_alloc_list_thread() ;
void JD_dealloc_obj_table_table() ;
