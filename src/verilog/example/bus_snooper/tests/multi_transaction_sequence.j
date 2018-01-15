/* 
   
   Copyright (C) 2001 Juniper Networks Inc.

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
#include "../include/cpu_bus.jh"
#include "../include/system_bus.jh"
#include "../include/sys_defines.jh"
#include "../models/bus_snooper_model.jh"
#include "../utils/bus_class_0.jh"
#include "../utils/cpu_bus_driver_1.jh"
#include "../utils/sys_bus_driver_1.jh"
#include "../utils/bus_transactions_2.jh"

#include "../include/global_var.jh"

void snooper_test ()
{
  bit [39:0] address;
  bit [2:0] cid ;
  bit [511:0] data ;
  bit [511:0] data1 ;
  
  
  address = {urandom(),urandom(),6'b0} ;
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;
   
  
  bus_drive.cpu_rto_foreign_rts(
    0,		// integer dly,
    address,	// bit [39:0] address,
    10,		// integer rto_addr2data_dly,
    data,	// bit [511:0] data,
    30,		// integer rto2rts_dly,
    5,		// bit [3:0] rts_did,
    4,		// bit [3:0] rts_cid,
    10		// integer rts_addr2data_dly
  ) ;

  printf( " cpu_rto_foreign_rts completed\n" ) ;
  
  
  
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  bus_drive.cpu_rto_foreign_rto(
    10,		// integer dly,
    address,	// bit [39:0] address,
    10,		// integer rto_addr2data_dly,
    6'b0,	// bit [6:0] drv_shared_in,
    data,	// bit [511:0] data,
    30,		// integer rto2rto_dly,
    7,		// bit [3:0] rto_did,
    5,		// bit [3:0] rto_cid,
    20		// integer frto_addr2data_dly
  ) ;
  

  printf( " cpu_rto_foreign_rto completed\n" ) ;
  
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  bus_drive.cpu_rts_foreign_rto(
    10,		// integer dly,
    address,	// bit [39:0] address,
    20,		// integer rts_addr2data_dly,
    6'b0,	// bit [6:0] drv_shared_in,
    data,	// bit [511:0] data,
    15,		// integer rts2rto_dly,
    3,		// bit [3:0] rto_did,
    1,		// bit [3:0] rto_cid,
    20		// integer frto_addr2data_dly
  ) ;
  
  printf( " cpu_rto_foreign_rto 2 completed\n" ) ;
  
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  data1 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  bus_drive.cpu_wb_cancel_on_foreign_rto (
    10,		// integer dly,
    address,	// bit [39:0] address,
    data,	// bit [511:0] rd_data,
    data1,	// bit [511:0] wr_data,
    6,		// bit [3:0] rto_did,
    4,		// bit [3:0] rto_cid,
    10,		// integer cpu_wb_start_dly,
    30,		// integer forgin_rto_a2d_dly,
    5		// integer cpu_wb_a2d_dly
  ) ;

  printf( " cpu_wb_cancel_on_foreign_rto completed\n" ) ;
  
  
}

    
  
  
  
