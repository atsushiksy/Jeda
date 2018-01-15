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

class sys_bus_driver {
  int dev_id ; // device id to be tested
  int last_a_winner ; // for address bus arbitration
  int last_d_winner ; // for data bus arbitration
  semaphore sys_a_sem[8] ;
  semaphore sys_d_sem[8] ;

  // the system port variables where this driver is talking to
  system_bus    sys_bus ;
  
  int debug ;
  
  void new( 
    int id,
    system_bus a_sys_bus
  ) {
    int i ;
    dev_id = id ;
    
    sys_bus = a_sys_bus ;
    
    last_a_winner = 0 ;
    last_d_winner = 0 ;
    for( i = 0 ; i < 8 ; i++ ) {
      sys_a_sem[i] = new ; // array of 8, per device
      sys_d_sem[i] = new ; // array of 8
    }
    
    debug = 0 ;
    
  }
  
  void sys_a_arb_loop() {
    int i, tmp ;
    bit [6:0] req_in ;
    bit       req_out ;
    bit [7:0] req ;
    
    
    fork
    while(1) {
      @(posedge sys_bus.clock) ;
      req_in = sys_bus.a_req_in.0 ; // other request
      req_out = sys_bus.a_req_out.0 ; // own request
      for( i = 0 ; i < 8 ; i++ ) {
        if( i == dev_id ) req[i] = req_out ;
        else {
          if( i < dev_id ) req[i] = req_in[i] ;
          else req[i] = req_in[i-1] ;
        }
      }
      @(negedge sys_bus.clock) ;
      if( req ) {
        tmp = (last_a_winner+1)%8 ; 
        for( i = 0 ; i < 8 ; i++ ) {
          if( req[tmp] ) break ;
          tmp = (tmp+1)%8 ;
        }
        last_a_winner = tmp ;
      }
    }
    knife
  }
  
  // data bus arbitration
  void sys_d_arb_loop() {
    int i, tmp ;
    bit [6:0] req_in ;
    bit       req_out ;
    bit [7:0] req ;
    
    
    fork
    while(1) {
      @(posedge sys_bus.clock) ;
      req_in = sys_bus.d_req_in.0 ; // other request
      req_out = sys_bus.d_req_out.0 ; // own request
      for( i = 0 ; i < 8 ; i++ ) {
        if( i == dev_id ) req[i] = req_out ;
        else {
          if( i < dev_id ) req[i] = req_in[i] ;
          else req[i] = req_in[i-1] ;
        }
      }
      @(negedge sys_bus.clock) ;
      if( req ) {
        tmp = (last_d_winner+1)%8 ; 
        for( i = 0 ; i < 8 ; i++ ) {
          if( req[tmp] ) break ;
          tmp = (tmp+1)%8 ;
        }
        last_d_winner = tmp ;
      }
    }
    knife
  }
  
  // get address bus as device did
  void get_sys_a_bus( int did ) {
    int offset ;
    
    if( did > dev_id ) offset = did - 1 ;
    else offset = did ;
    sys_a_sem[did].get() ;
    @(posedge sys_bus.clock) ;
    sys_bus.a_req_in.0[offset] = 1'b1 ;
    @1 sys_bus.a_req_in.0[offset] = 1'b0 weak;
    while( last_a_winner != did ) {
      sys_bus.a_req_in.0[offset] = 1'b1 ;
      @1 sys_bus.a_req_in.0[offset] = 1'b0 weak;
    }
    sys_a_sem[did].put() ;  
  }
  
  // get data bus as device did
  void get_sys_d_bus( int did ) {
    int offset ;
    
    if( did > dev_id ) offset = did - 1 ;
    else offset = did ;
    sys_d_sem[did].get() ;
    @(posedge sys_bus.clock) ;
    sys_bus.d_req_in.0[offset] = 1'b1 ;
    @1 sys_bus.d_req_in.0[offset] = 1'b0 weak;
    while( last_d_winner != did ) {
      sys_bus.d_req_in.0[offset] = 1'b1 ;
      @1 sys_bus.d_req_in.0[offset] = 1'b0 weak;
    }
    sys_d_sem[did].put() ;  
  }
  
  void drive_addr (
    bit [2:0] did,
    bit [2:0] cid,
    bit [39:0] address,
    bit [3:0] command,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit       exp_shared_out, // shared_out expect value
    bit [6:0] drv_owned_in,
    bit       exp_owned_out,
    // optional timing control
    event  arb_go = null,  // trigger in to start bus arb
    event  arb_done = null, // trigger out at arb done
    event  snoop_done = null // trigger out at snoop done
  )
  {
    if( arb_go ) arb_go.sync() ;
    get_sys_a_bus( did ) ;
    if( debug ) {
      printf( "drive_addr did %d won the bus @ %d", did, get_cycle() ) ;
    }
    if( arb_done ) arb_done.trigger(  ) ;
    @0 sys_bus.command.0 = command ;
    sys_bus.address.0 = address ;
    sys_bus.dev_id.0 = did ;
    sys_bus.cmd_id.0 = cid ;
    if( debug ) {
      printf( 
        "sysbus drive_addr cmd:%b a:%h did:%h cid:%h\n", 
        command, address, did, cid
      ) ;
    }
    @1 sys_bus.command.0 <= 0 weak ;
    @1 sys_bus.address.0 <= 0 weak ;
    @1 sys_bus.dev_id.0 <= 0 weak ;
    @1 sys_bus.cmd_id.0 <= 0 weak ;
    
    @3 sys_bus.shared_in.0 = drv_shared_in ;
    sys_bus.owned_in.0 = drv_owned_in ;
    @1 sys_bus.shared_in.0 <= 0 weak ;
    @1 sys_bus.owned_in.0 <= 0 weak ;
    
    assert (
      @1 ( 
        sys_bus.shared_out.0 ==? exp_shared_out,
        sys_bus.owned_out.0 ==? exp_owned_out
      )
    ) ;
    if( debug ) {
      printf( 
        "drive_addr exp S %b O %b : act S %b O %b @ %d\n",
        exp_shared_out, exp_owned_out,
        sys_bus.shared_out.0, sys_bus.owned_out.0, get_cycle()
      ) ;
    }
    if( snoop_done ) snoop_done.trigger() ;
  }
  
  void expect_addr (
    int dly,    // delay
    int window, // window
    bit [2:0] did,
    var bit [2:0] cid,
    bit [39:0] address,
    bit [3:0] command,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit       exp_shared_out, // shared_out expect value
    bit [6:0] drv_owned_in,
    bit       exp_owned_out,
    // optional timing control
    event  addr_done = null, // trigger out at addr done
    event  snoop_done = null // trigger out at snoop done
  )
  {
    int start = get_cycle() ;
    
    if( debug ) {
      printf( "expect_addr in: dly: %d, window: %d addr: %h command %b @ %d\n",
        dly, window, address, command, start
      ) ;
    }
    assert( 
      @dly,window (
        sys_bus.command.0 ==? command, sys_bus.address.0 ==? address,
        sys_bus.dev_id.0 ==? did
      )
    ) ;
    if( debug ) {
      printf( "expect_addr out: dly: %d, window: %d addr: %h command %b start %d @ %d\n",
        dly, window, address, command, start, get_cycle()
      ) ;
    }
    cid = sys_bus.cmd_id.0 ;
    if( addr_done ) addr_done.trigger() ;
    
    @2 sys_bus.shared_in.0 = drv_shared_in ;
    sys_bus.owned_in.0 = drv_owned_in ;
    @1 sys_bus.shared_in.0 <= 0 weak ;
    @1 sys_bus.owned_in.0 <= 0 weak ;
    
    assert (
      @1 ( sys_bus.shared_out.0 ==? exp_shared_out,
           sys_bus.owned_out.0 ==? exp_owned_out 
         )
    ) ;
    if( debug ) {
      printf( 
        "expect_addr cmd %b addr %h exp S %b O %b : act S %b O %b @ %d\n",
        command, address, exp_shared_out, exp_owned_out,
        sys_bus.shared_out.0, sys_bus.owned_out.0, get_cycle()
      ) ;
    }
    snoop_done.trigger() ;
    
  }
  
  void drive_data (
    bit [2:0] arb_id,
    bit [2:0] did,
    bit [2:0] cid,
    bit [1:0] ack,
    bit [511:0] data,
    // optional timing control
    event  arb_go = null,  // trigger in to start bus arb
    event  data_done = null // trigger out at arb done
  )
  {
    if( arb_go ) arb_go.sync() ;
    get_sys_d_bus( arb_id ) ;
    @0 sys_bus.ack.0 = ack ;
    sys_bus.ack_did.0 = did ;
    sys_bus.ack_cid.0 = cid ;
    sys_bus.data.0 = data ;
    if( debug ) {
      printf( 
        "sysbus drive_data ack:%b did:%h cid:%h data:%h\n", 
        ack, did, cid, data
      ) ;
    }
    @1 sys_bus.ack.0 <= 0 weak ;
    @1 sys_bus.ack_did.0 <= 0 weak ;
    @1 sys_bus.ack_cid.0 <= 0 weak ;
    @1 sys_bus.data.0 <= 0 weak ;
    if(  data_done ) data_done.trigger() ;
    
  }

  void expect_data (
    int dly,
    int window,
    bit [2:0] did,
    bit [2:0] cid,
    bit [1:0] ack,
    bit [511:0] data,
    // optional timing control
    event  get_data = null // trigger out at data
  )
  {
    int start = get_cycle() ;
    assert (
      @dly,window (
        sys_bus.ack.0 ==? ack, sys_bus.ack_did.0 ==? did,
        sys_bus.data.0 ==? data, sys_bus.ack_cid.0 ==? cid 
      )
    ) ;
    if( debug ) {
      printf( 
        "expect_data: dly %d window %d ack %b did %h cid %d data: %h start %d @ %d\n",
        dly, window, ack, did, cid, data, start, get_cycle() 
      ) ;
    }
    if( get_data ) get_data.trigger() ;
  }
  
  
}
