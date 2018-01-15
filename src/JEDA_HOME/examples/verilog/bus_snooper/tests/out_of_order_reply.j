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

void a_snooper_test ()
{
  bit [39:0] addr0, addr1, addr2, addr3, addr4, addr5, addr6, addr7 ;
  bit [2:0] cid0, cid1, cid2, cid3, cid4, cid5, cid6, cid7 ;
  bit [511:0] data0, data1, data2, data3, data4, data5, data6, data7 ;
  event cpu_addr0, cpu_addr1, cpu_addr2, cpu_addr3, 
        cpu_addr4, cpu_addr5, cpu_addr6, cpu_addr7 ;
  event sys_addr0, sys_addr1, sys_addr2, sys_addr3, 
        sys_addr4, sys_addr5, sys_addr6, sys_addr7 ;
  event sys_data0, sys_data1, sys_data2, sys_data3, 
        sys_data4, sys_data5, sys_data6, sys_data7 ;
  event sys_d_done0, sys_d_done1, sys_d_done2, sys_d_done3, 
        sys_d_done4, sys_d_done5, sys_d_done6, sys_d_done7 ;
  event complete0, complete1, complete2, complete3, 
        complete4, complete5, complete6, complete7 ;
  
  cpu_addr0 = new ;
  cpu_addr1 = new ;
  cpu_addr2 = new ;
  cpu_addr3 = new ;
  cpu_addr4 = new ;
  cpu_addr5 = new ;
  cpu_addr6 = new ;
  cpu_addr7 = new ;
  sys_addr0 = new ;
  sys_addr1 = new ;
  sys_addr2 = new ;
  sys_addr3 = new ;
  sys_addr4 = new ;
  sys_addr5 = new ;
  sys_addr6 = new ;
  sys_addr7 = new ;
  sys_data0 = new ;
  sys_data1 = new ;
  sys_data2 = new ;
  sys_data3 = new ;
  sys_data4 = new ;
  sys_data5 = new ;
  sys_data6 = new ;
  sys_data7 = new ;
  sys_d_done0 = new ;
  sys_d_done1 = new ;
  sys_d_done2 = new ;
  sys_d_done3 = new ;
  sys_d_done4 = new ;
  sys_d_done5 = new ;
  sys_d_done6 = new ;
  sys_d_done7 = new ;
  complete0 = new ;
  complete1 = new ;
  complete2 = new ;
  complete3 = new ;
  complete4 = new ;
  complete5 = new ;
  complete6 = new ;
  complete7 = new ;
  
  
  // setting unique addresses such that the corresponding tag entries
  // are not conflicting
  addr0 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr0[OFFSET_BITS]) )
    addr0 = {urandom(),urandom(),6'b0} ;
  data0 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr1 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr1[OFFSET_BITS]) )
    addr1 = {urandom(),urandom(),6'b0} ;
  data1 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr2 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr2[OFFSET_BITS]) )
    addr2 = {urandom(),urandom(),6'b0} ;
  data2 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr3 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr3[OFFSET_BITS]) )
    addr3 = {urandom(),urandom(),6'b0} ;
  data3 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr4 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr4[OFFSET_BITS]) )
    addr4 = {urandom(),urandom(),6'b0} ;
  data4 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr5 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr5[OFFSET_BITS]) )
    addr5 = {urandom(),urandom(),6'b0} ;
  data5 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr6 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr6[OFFSET_BITS]) )
    addr6 = {urandom(),urandom(),6'b0} ;
  data6 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;

  addr7 = {urandom(),urandom(),6'b0} ;
  while( !tag_region.check_enter(addr7[OFFSET_BITS]) )
    addr7 = {urandom(),urandom(),6'b0} ;
  data7 = {urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom(),
          urandom(),urandom(),urandom(),urandom()} ;
  
  // fork off 8 RTS, in one order, then control the reply in
  // the reversed order. Sequence of those ordering is controled
  // via event variables.
  fork
  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr0,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data0,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid0,		// var bit [2:0] cid,
      *,		//  event  cpu_arb_go = null,  
      cpu_addr0,	//  event  cpu_arb_done = null, 
      sys_addr0,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data0,	//  event  sys_data_arb_go = null,  
      sys_d_done0,	//  event  sys_data_done = null,
      *			//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.0 done!\n" ) ;
    complete0.trigger_on() ;
  }
  
  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr1,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data1,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid1,		// var bit [2:0] cid,
      cpu_addr0,	//  event  cpu_arb_go = null,  
      cpu_addr1,	//  event  cpu_arb_done = null, 
      sys_addr1,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data1,	//  event  sys_data_arb_go = null,  
      sys_d_done1,  	//  event  sys_data_done = null,
      *			//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.1 done!\n" ) ;
    complete1.trigger_on() ;
  }
  
  { 
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr2,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data2,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid2,		// var bit [2:0] cid,
      cpu_addr1,	//  event  cpu_arb_go = null,  
      cpu_addr2,	//  event  cpu_arb_done = null, 
      sys_addr2,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data2,	//  event  sys_data_arb_go = null,  
      sys_d_done2,  	//  event  sys_data_done = null,
      *		//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.2 done!\n" ) ;
    complete2.trigger_on() ;
  }

  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr3,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data3,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid3,		// var bit [2:0] cid,
      cpu_addr2,	//  event  cpu_arb_go = null,  
      cpu_addr3,	//  event  cpu_arb_done = null, 
      sys_addr3,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data3,	//  event  sys_data_arb_go = null,  
      sys_d_done3,  	//  event  sys_data_done = null,
      *		//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.3 done!\n" ) ;
    complete3.trigger_on() ;
  }

  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr4,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data4,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid4,		// var bit [2:0] cid,
      cpu_addr3,	//  event  cpu_arb_go = null,  
      cpu_addr4,	//  event  cpu_arb_done = null, 
      sys_addr4,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data4,	//  event  sys_data_arb_go = null,  
      sys_d_done4,  	//  event  sys_data_done = null,
      *			//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.4 done!\n" ) ;
    complete4.trigger_on() ;
  }
  
  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr5,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data5,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid5,		// var bit [2:0] cid,
      cpu_addr4,	//  event  cpu_arb_go = null,  
      cpu_addr5,	//  event  cpu_arb_done = null, 
      sys_addr5,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data5,	//  event  sys_data_arb_go = null,  
      sys_d_done5,  	//  event  sys_data_done = null,
      *		//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.5 done!\n" ) ;
    complete5.trigger_on() ;
  }
  
  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr6,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data6,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid6,		// var bit [2:0] cid,
      cpu_addr5,	//  event  cpu_arb_go = null,  
      cpu_addr6,	//  event  cpu_arb_done = null, 
      sys_addr6,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data6,	//  event  sys_data_arb_go = null,  
      sys_d_done6,  	//  event  sys_data_done = null,
      *		//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.6 done!\n" ) ;
    complete6.trigger_on() ;
  }
  
  {
    bus_drive.cpu_read_access(
      RTS,		//  bit [3:0]  command,
      addr7,		//  bit [39:0] address,
      7'b0,		//  bit [6:0] drv_shared_in,
      1'b0,		//  bit       exp_shared_out,
      7'b0,		//  bit [6:0] drv_owned_in,
      1'b0,		//  bit       exp_owned_out,
      0,			//  integer   addr2data_dly,
      data7,		//  bit [511:0] data,
      ACKD,		// bit [1:0] ack,
      cid7,		// var bit [2:0] cid,
      cpu_addr6,	//  event  cpu_arb_go = null,  
      cpu_addr7,	//  event  cpu_arb_done = null, 
      sys_addr7,	//  event  sys_addr_done = null, 
      *,		//  event  sys_snoop_done = null, 
      sys_data7,	//  event  sys_data_arb_go = null,  
      sys_d_done7,  	//  event  sys_data_done = null,
      *		//  event  cpu_ack_done = null
    ) ;
    printf( "RTS No.7 done!\n" ) ;
    complete7.trigger_on() ;
  }

  knife   // proceed without waiting
    
  // order control sequence
  // wait all the commands on system bus
  sys_addr0.sync() ;
  sys_addr1.sync() ;
  sys_addr2.sync() ;
  sys_addr3.sync() ;
  sys_addr4.sync() ;
  sys_addr5.sync() ;
  sys_addr6.sync() ;
  sys_addr7.sync() ;
  
  printf( "Got all commands on system bus\n" ) ;
  
  @10(posedge CLOCK) ; // some delay    
  // returning ack/data in the out of order
  
  fork 
  {
    // sending ack in reverse order
    @5(posedge CLOCK ) ;
    printf( "Sending ack7 on system bus @%0d\n", get_cycle() ) ;
    sys_data7.trigger_on() ;
    sys_d_done7.sync() ;
    
    @5(posedge CLOCK ) ;
    printf( "Sending ack6 on system bus @%0d\n", get_cycle() ) ;
    sys_data6.trigger_on() ;
    sys_d_done6.sync() ;
    
    @5(posedge CLOCK ) ;
    printf( "Sending ack5 on system bus @%0d\n", get_cycle() ) ;
    sys_data5.trigger_on() ;
    sys_d_done5.sync() ;
    
    @5(posedge CLOCK ) ;
    printf( "Sending ack4 on system bus @%0d\n", get_cycle() ) ;
    sys_data4.trigger_on() ;
    sys_d_done4.sync() ;
    
    @5(posedge CLOCK ) ;
    printf( "Sending ack3 on system bus @%0d\n", get_cycle() ) ;
    sys_data3.trigger_on() ;
    sys_d_done3.sync() ;
    
    @5(posedge CLOCK ) ;
    printf( "Sending ack2 on system bus @%0d\n", get_cycle() ) ;
    sys_data2.trigger_on() ;
    sys_d_done2.sync() ;
    
    repeat( 5 ) @(posedge CLOCK ) ;
    printf( "Sending ack1 on system bus @%0d\n", get_cycle() ) ;
    sys_data1.trigger_on() ;
    sys_d_done1.sync() ;
    
    repeat( 5 ) @(posedge CLOCK ) ;
    printf( "Sending ack0 on system bus @%0d\n", get_cycle() ) ;
    sys_data0.trigger_on() ;
    sys_d_done0.sync() ;
  }
  {
    complete0.sync() ;
    complete1.sync() ;
    complete2.sync() ;
    complete3.sync() ;
    complete4.sync() ;
    complete5.sync() ;
    complete6.sync() ;
    complete7.sync() ;
  }
  join
  
  // some delay
  @30(posedge CLOCK) ;
  
}

    
  
  
  
void snooper_test ()
{
  int i ;
  
  for( i = 0 ; i < 100 ; i++ ) {
    a_snooper_test() ;
  }
  
}
