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
  bit [39:0] address1;
  bit [2:0] cid ;
  bit [511:0] data ;
  
  
  address = {urandom(),urandom(),6'b0} ;
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;
          
  printf( "data = %h\n", data ) ;
  
  bus_drive.cpu_read_access(
    RTS,		//  bit [3:0]  command,
    address,		//  bit [39:0] address,
    7'b0,		//  bit [6:0] drv_shared_in,
    1'b0,		//  bit       exp_shared_out,
    7'b0,		//  bit [6:0] drv_owned_in,
    1'b0,		//  bit       exp_owned_out,
    10,			//  integer   addr2data_dly,
    data,		//  bit [511:0] data,
    ACKD,		// bit [1:0] ack,
    cid		// var bit [2:0] cid,
  ) ;

  // RTO to the existing entry
  bus_drive.cpu_read_access(
    RTO,		//  bit [3:0]  command,
    address,		//  bit [39:0] address,
    7'b0,		//  bit [6:0] drv_shared_in,
    1'b1,		//  bit       exp_shared_out,
    7'b0,		//  bit [6:0] drv_owned_in,
    1'b0,		//  bit       exp_owned_out,
    10,			//  integer   addr2data_dly,
    data,		//  bit [511:0] data,
    ACKD,		// bit [1:0] ack,
    cid		// var bit [2:0] cid,
  ) ;

  
  address1 = {urandom(),urandom(),6'b0} ;
  while( address1[OFFSET_BITS] == address[OFFSET_BITS] )
    address1 = {urandom(),urandom(),6'b0} ;
  data = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;


  // RTO to not-shared entry
  bus_drive.cpu_read_access(
    RTO,		//  bit [3:0]  command,
    address1,		//  bit [39:0] address,
    7'b0,		//  bit [6:0] drv_shared_in,
    1'b0,		//  bit       exp_shared_out,
    7'b0,		//  bit [6:0] drv_owned_in,
    1'b0,		//  bit       exp_owned_out,
    10,			//  integer   addr2data_dly,
    data,		//  bit [511:0] data,
    ACKD,		// bit [1:0] ack,
    cid		// var bit [2:0] cid,
  ) ;

  bus_drive.cpu_wb_access(
    address1,		// bit [39:0] address,
    0,			// bit [6:0] drv_shared_in,  // shared_in drive value
    0,			// bit       exp_shared_out, // shared_out expect value
    0,			// bit [6:0] drv_owned_in,
    0,			// bit       exp_owned_out,
    10,			// integer   addr2data_dly,
    data,		// bit [511:0] data,
    cid			// var bit [2:0] cid,
  ) ;
  
  @30(posedge CLOCK) ;
  
}

    
  
  
  
