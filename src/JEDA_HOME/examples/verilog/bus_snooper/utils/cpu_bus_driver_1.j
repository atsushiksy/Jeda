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
#include "../include/sys_defines.jh"
#include "../utils/bus_class_0.jh"

class cpu_bus_driver {
  int dev_id ;
  semaphore cpu_a_sem ;
  semaphore cpu_d_sem ;
  semaphore cpu_ack_sem ;  // semaphore for CPU access ack (for ordering)
  semaphore sys_ack_sem ;  // semaphore for SYS access ack (for ordering)
  semaphore wb_data_sem ;  // semaphore for CPU wb data (for ordering)
  
  // CPU bus this driver is talking to
  processor_bus cpu_bus ;
  
  int debug ;
  
  void new(
    int id,
    processor_bus a_cpu_bus
  ) {
    dev_id = id ;
    cpu_bus = a_cpu_bus ;
    
    cpu_a_sem = new ;
    cpu_d_sem = new ;
    cpu_ack_sem = new ;
    sys_ack_sem = new ;
    wb_data_sem = new ;
    
    debug = 0 ;
  }

  void get_cpu_a_bus() {
    bit gnt ;
    cpu_a_sem.get() ;
    gnt = 0 ;
    while( 1 ) {
      gnt = cpu_bus.cpu_a_gnt.0 ;
      if( gnt ) break ;
      cpu_bus.cpu_a_req.0 = 1'b1 ;
      @(posedge cpu_bus.clock) ;
    }
    cpu_bus.cpu_a_req.0 = 1'b0 weak ;
    fork
    {
      @(posedge cpu_bus.clock) ;
      cpu_a_sem.put() ;
    }
    knife
  }


  void get_cpu_d_bus() {
    bit gnt ;
    cpu_d_sem.get() ;
    gnt = 0 ;
    while( 1 ) {
      gnt = cpu_bus.cpu_d_gnt.0 ;
      if( gnt ) break ;
      cpu_bus.cpu_d_req.0 = 1'b1 ;
      @(posedge cpu_bus.clock) ;
    }
    cpu_bus.cpu_d_req.0 <= 1'b0 weak ;
    
    fork
    {
      @(posedge cpu_bus.clock) ;
      cpu_d_sem.put() ;
    }
    knife
  }

  void drive_addr (
    bit [3:0] command,
    bit [39:0] address,
    // optional timing control
    event  arb_go = null,  // trigger in to start bus arb
    event  arb_done = null // trigger out at arb done
  )
  {
    if( arb_go ) {
      arb_go.sync() ;
      if( debug ) 
        printf( "cpu_bus.drive_addr got arb_go @ %d\n", get_cycle() ) ;
    }
    get_cpu_a_bus() ;
    if( arb_done ) arb_done.trigger() ;
    @0 cpu_bus.command.0 = command ;
    cpu_bus.address.0 = address ;
    cpu_bus.cpu_valid.0 = 1'b1 ;
    if( debug ) 
      printf( 
        "cpu_bus.drive_addr driven cmd %b addr %h @ %d\n", 
        command, address, get_cycle() 
      ) ;
    @1 cpu_bus.command.0 <= 0 weak ;
    @1 cpu_bus.address.0 <= 0 weak ;
    @1 cpu_bus.cpu_valid.0 <= 0 weak ;
  }

  void expect_addr (
    int dly,
    int window,
    bit [3:0] command,
    bit [39:0] address  
  )
  {
    assert (
      @dly, window ( 
        cpu_bus.sys_valid.0 == 1'b1, 
        cpu_bus.command.0 ==? command,
        cpu_bus.address.0 ==? address )
    ) ;
  }
  
  void drive_cpu_data (
    bit [2:0] cpu_ack,
    bit [511:0] data,
    event arb_go,
    event arb_done
  )
  {
    if( arb_go ) arb_go.sync() ;
    get_cpu_d_bus() ;
    if( arb_done ) arb_done.trigger() ;
    @0 cpu_bus.cpu_ack.0 = cpu_ack ;
    cpu_bus.data.0 = data ;
    if( debug ) {
      printf( 
        "drive_cpu_data ack:%b data:%h @ %d\n", 
        cpu_ack, data, get_cycle()
      ) ;
    }
    @1 cpu_bus.cpu_ack.0 <= 0 weak ;
    @1 cpu_bus.data.0 <= 0 weak ;
  }
  
  void expect_sys_data (
    int dly,
    int window,
    bit [2:0] sys_ack,
    bit [511:0] data
  ) {
    int start ;
    start = get_cycle() ;
    if( debug ) {
      printf( 
        "expect_sys_data ack:%b data:%h @ %d\n", 
        sys_ack, data, get_cycle()
      ) ;
    }
    assert (
      @dly,window (
        cpu_bus.sys_ack.0 ==? sys_ack, cpu_bus.data.0 ==? data
      )
    ) ;
    if( debug ) {
      printf( 
        "expect_sys_data end ack:%b data:%h start %d @ %d\n", 
        sys_ack, data, start, get_cycle()
      ) ;
    }
  }
  
}
