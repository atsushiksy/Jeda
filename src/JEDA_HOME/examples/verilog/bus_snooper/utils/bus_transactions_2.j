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
#include "../utils/cpu_bus_driver_1.jh"
#include "../utils/sys_bus_driver_1.jh"

class bus_transactions{
  int dev_id ;
  cpu_bus_driver cpu_dr ;
  sys_bus_driver sys_dr ;
  int debug = 0 ;
  
  void new(
    int did,
    processor_bus cpu_bus,
    system_bus sys_bus
  ) 
  {
    dev_id = did ;
    
    cpu_dr = new(did, cpu_bus) ;
    
    sys_dr = new(did, sys_bus) ;
    
    // start arbitration loop
    sys_dr.sys_a_arb_loop() ;
    sys_dr.sys_d_arb_loop() ;
  }
  
  // cpu_read_access: can execute CPU -> SYSTEM RTS,RTO
  void cpu_read_access( 
    bit [3:0]  command,
    bit [39:0] address,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit       exp_shared_out, // shared_out expect value
    bit [6:0] drv_owned_in,
    bit       exp_owned_out,
    int   addr2data_dly,
    bit [511:0] data,
    bit [2:0] ack,
    var bit [2:0] cid,
    // optional timing control
    event  cpu_arb_go = null,  // trigger in to start bus arb
    event  cpu_arb_done = null, // trigger out at arb done
    event  sys_addr_done = null, // trigger out at addr done
    event  sys_snoop_done = null, // trigger out at snoop done
    event  sys_data_arb_go = null,  // trigger in to start bus arb
    event  sys_data_done = null, // trigger out at arb done
    event  cpu_ack_done = null   // triger out at ack/data done
  
  )
  {
    event my_snoop_done = new ;
    bit sample_shared_out ;
    bit [2:0] arb_id ;
    
    // randomly select arb id
    arb_id = random() ;
    while( arb_id == dev_id ) arb_id = random() ;
    
    if( sys_snoop_done == null ) sys_snoop_done = my_snoop_done ;
    
    // printf( "cpu_read_access data = %h\n", data ) ;
    
    cpu_dr.drive_addr(
      command,
      address,
      cpu_arb_go,
      cpu_arb_done
    ) ;
    
    fork
    sys_dr.expect_addr(
      1,			// dly
      100,			// window
      dev_id,
      cid,			// var cid
      address,
      command,
      drv_shared_in,
      exp_shared_out,
      drv_owned_in,
      exp_owned_out,
      sys_addr_done,
      sys_snoop_done
    ) ;
    {
      // printf( "cpu_read_access 1 data = %h\n", data ) ;
      sys_snoop_done.sync() ;
      sample_shared_out = sys_dr.sys_bus.shared_out.0 ;
    }
    join
        
    fork
    {
      if( (command == RTO) && (sample_shared_out == 1'b1) ) {
        // no data transfer
        ack = ACK ;
        data = 512'bx ;    
      }
      else {
        repeat( addr2data_dly ) @(posedge sys_dr.sys_bus.clock) ;
        sys_dr.drive_data(
          arb_id, dev_id, cid, ack, data,
          sys_data_arb_go, sys_data_done
        ) ;
      }
    } 
    {
      // semaphore for ACK ordering
      cpu_dr.cpu_ack_sem.get() ;
    }
    join
    
    cpu_dr.expect_sys_data(
      0, 250+addr2data_dly, // dly, window
      ack,
      data
    ) ;
    
    fork 
    {
      @(posedge cpu_dr.cpu_bus.clock) ;
      cpu_dr.cpu_ack_sem.put() ;
    }
    knife
    
  }

  // cpu_wb_access: can do CPU -> SYSTEM Write Back transactions
  void cpu_wb_access( 
    bit [39:0] address,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit       exp_shared_out, // shared_out expect value
    bit [6:0] drv_owned_in,
    bit       exp_owned_out,
    int   addr2data_dly,
    bit [511:0] data,
    var bit [2:0] cid,
    // optional timing control
    event  cpu_arb_go = null,  // trigger in to start bus arb
    event  cpu_arb_done = null, // trigger out at arb done
    event  sys_addr_done = null, // trigger out at addr done
    event  sys_snoop_done = null, // trigger out at snoop done
    event  sys_data_arb_go = null,  // trigger in to start bus arb
    event  sys_data_done = null, // trigger out at arb done
    event  cpu_ack_done = null   // triger out at ack/data done
  )
  {
    bit [2:0] ack ;
    bit [511:0] edata ;
    
    cpu_dr.drive_addr(
      WB,
      address,
      cpu_arb_go,
      cpu_arb_done
    ) ;
    
    sys_dr.expect_addr(
      1,			// dly
      100,			// window
      dev_id,
      cid,			// var cid
      address,
      WB,
      drv_shared_in,
      exp_shared_out,
      drv_owned_in,
      exp_owned_out,
      sys_addr_done,
      sys_snoop_done
    ) ;
    
    fork
    {
      cpu_dr.wb_data_sem.get() ;
      @addr2data_dly (posedge sys_dr.sys_bus.clock) ;
      cpu_dr.drive_cpu_data(
        WRBD, data,
        sys_data_arb_go, 
        sys_data_done
      ) ;
      cpu_dr.wb_data_sem.put() ;
      if( exp_owned_out !== 1'b1 ) {
        sys_dr.expect_data(
          0, 250,  // dly, window
          dev_id,
          cid,
          ACKD,
          data
        ) ;
      }
    } 
    knife
    
    // semaphore for ACK ordering
    cpu_dr.cpu_ack_sem.get() ;
    
    if( exp_owned_out == 1'b1 ) {
      // no data transfer
      ack = WRBC ;
      edata = 512'bx ;    
    }
    else {
      ack = ACK ;
      edata = 512'bx ;
    }
    
    cpu_dr.expect_sys_data(
      0, 250, // dly, window
      ack,
      edata
    ) ;
    
    fork 
    {
      @(posedge cpu_dr.cpu_bus.clock) ;
      cpu_dr.cpu_ack_sem.put() ;
    }
    knife

  }

  // foreign RTS that may be owned by the dut
  void foreign_rts_access (
    bit [3:0] did,
    bit [3:0] cid,
    bit [39:0] address,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit [6:0] drv_owned_in,
    int   addr2data_dly,
    bit [511:0] data,
    bit [2:0] ack,
    // optional timing control
    event  sys_arb_go = null,  // trigger in to start bus arb
    event  sys_arb_done = null, // trigger out at addr done
    event  sys_snoop_done = null, // trigger out at snoop done
    event  cpu_addr_done = null,  // trigger out at address done
    event  cpu_d_arb_go = null, // trigger in to start data arb
    event  cpu_d_arb_done = null,   // triger out at data arb done
    event  sys_get_data = null     // trigger out at data done 
  )
  {
    bit shared_out, owned_out ;
    event my_snoop_done  = new ;
    
    if( sys_snoop_done != null ) my_snoop_done = sys_snoop_done ;
    
    fork
      {
        sys_dr.drive_addr (
          did, cid, address, RTS, drv_shared_in, 1'bx,
          drv_owned_in, 1'bx, sys_arb_go, sys_arb_done, my_snoop_done
        ) ;
      }
      
      {
        // sample shared_out and owned_out from the snooper
        my_snoop_done.sync() ;
        shared_out = sys_dr.sys_bus.shared_out.0 ;
        owned_out = sys_dr.sys_bus.owned_out.0 ;       
      }
    join

    if( debug ) {
      printf( 
        "foreign_rts_access: driven sysbus did %h cid %h command: %b addr %h s %b o %b @ %d \n",
        did, cid, RTS, address, shared_out, owned_out, get_cycle() 
      ) ;
    }
    
    if( owned_out ) {
      if( drv_owned_in ) 
        error( 
          "foreign_rts_access: RTS transaction failed. owned conflict\n" 
        ) ;
      else {
        // wait for command/address showed up on cpu bus
        cpu_dr.expect_addr(
          1, 250, RTS, address
        ) ;
        if( debug ) {
          printf( 
            "foreign_rts_access: got cpu command: %b addr %h @ %d \n",
            RTS, address, get_cycle() 
          ) ;
        }
        cpu_addr_done.trigger() ;
        // semaphore for ACK ordering
        cpu_dr.sys_ack_sem.get() ;
        repeat( addr2data_dly ) @(posedge cpu_dr.cpu_bus.clock) ;
        // drive data
        //breakpoint ;
        cpu_dr.drive_cpu_data(
          ack, data, cpu_d_arb_go, cpu_d_arb_done
        ) ;
        if( debug ) {
          printf( 
            "foreign_rts_access: driven ack:%b data %h for addr %h @ %d \n",
            ack, data, address, get_cycle() 
          ) ;
        }
        cpu_dr.sys_ack_sem.put() ;
        //breakpoint ;
        // wait for data on sys bus
        sys_dr.expect_data (
          1, 250, did, cid, ack, data, sys_get_data 
        ) ;
        if( debug ) {
          printf( 
             "foreign_rts_access: got did:%h cid: %h ack:%b data %h for addr %h @ %d \n",
            did, cid, ack, data, address, get_cycle() 
          ) ;
        }
      }
    }  
  }
  
  // foreign RTO which must be owned by the dut
  //  this transaction will fail if this entry is not owned by dut.
  void foreign_rto_owned_access (
    bit [3:0] did,
    bit [3:0] cid,
    bit [39:0] address,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit [6:0] drv_owned_in,
    int   addr2data_dly,
    bit [511:0] data,
    bit [2:0] ack,
    // optional timing control
    event  sys_arb_go = null,  // trigger in to start bus arb
    event  sys_arb_done = null, // trigger out at addr done
    event  sys_snoop_done = null, // trigger out at snoop done
    event  cpu_addr_done = null,  // trigger out at address done
    event  cpu_d_arb_go = null, // trigger in to start data arb
    event  cpu_d_arb_done = null,   // triger out at data arb done
    event  sys_get_data = null     // trigger out at data done
  )
  {
    bit shared_out, owned_out ;
    event my_snoop_done = new ;
    
    if( sys_snoop_done != null ) my_snoop_done = sys_snoop_done ;
    
    sys_dr.drive_addr (
      did, cid, address, RTO, drv_shared_in, 1'b0,
      drv_owned_in, 1'b1, sys_arb_go, sys_arb_done, my_snoop_done
    ) ;
        
    if( !drv_shared_in ) {
      // wait for command/address showed up on cpu bus
      cpu_dr.expect_addr(
        1, 250, RTO, address
      ) ;
      cpu_addr_done.trigger() ;
      // semaphore for ACK ordering
      cpu_dr.sys_ack_sem.get() ;
      repeat( addr2data_dly ) @(posedge cpu_dr.cpu_bus.clock) ;
      // drive data
      cpu_dr.drive_cpu_data(
        ack, data, cpu_d_arb_go, cpu_d_arb_done
      ) ;
      cpu_dr.sys_ack_sem.put() ;
      // wait for data on sys bus
      sys_dr.expect_data (
        1, 250, did, cid, ack, data, sys_get_data 
      ) ;
    }  
  }
  
  // foreign RTO shared by the dut.
  //  this transaction will fail if the entry is not shared by dut.
  void foreign_rto_shared_access (
    bit [3:0] did,
    bit [3:0] cid,
    bit [39:0] address,
    bit [6:0] drv_shared_in,  // shared_in drive value
    bit [6:0] drv_owned_in,
    // optional timing control
    event  sys_arb_go = null,  // trigger in to start bus arb
    event  sys_arb_done = null, // trigger out at addr done
    event  sys_snoop_done = null, // trigger out at snoop done
    event  cpu_addr_done = null,  // trigger out at address done
    event  cpu_d_arb_go = null,
    event  cpu_d_done = null
  )
  {
    event my_snoop_done = new ;
    
    if( sys_snoop_done != null ) my_snoop_done = sys_snoop_done ;
    
    sys_dr.drive_addr (
      did, cid, address, RTO, drv_shared_in, 1'b0,
      drv_owned_in, 1'b0, sys_arb_go, sys_arb_done, my_snoop_done
    ) ;
        
    // wait for invalidate command showed up on cpu bus
    cpu_dr.expect_addr(
      1, 250, INV, address
    ) ;
    cpu_addr_done.trigger() ;
    cpu_dr.sys_ack_sem.get() ;
    cpu_dr.drive_cpu_data(
      ACK, 0, cpu_d_arb_go, cpu_d_done
    ) ;
    cpu_dr.sys_ack_sem.put() ;
  }

  //////////////////////////////////////////////////////////////////////
  // Multi bus access transactions that setup the tag entry, and then
  //  do a specific sequence.
  //////////////////////////////////////////////////////////////////////
  
  // RTO from CPU set the tag to be owned state,
  //  Then foreign RTS should be handled as cache read
  void cpu_rto_foreign_rts(
    int dly,
    bit [39:0] address,
    int rto_addr2data_dly,
    bit [511:0] data,
    int rto2rts_dly,
    bit [3:0] rts_did,
    bit [3:0] rts_cid,
    int rts_addr2data_dly
  )
  {
    bit [2:0] cid ;
    
    repeat(dly) @(posedge CLOCK) ;
    cpu_read_access(
      RTO,			// command
      address,			// address
      0,			// drv_shared_in
      1'bx,			// exp_shared_out
      0,			// drv_owned_in
      0,			// exp_owned_out
      rto_addr2data_dly,	// addr2data_dly
      data,
      ACKD,
      cid
    ) ;
    
    @rto2rts_dly(posedge CLOCK) ;
    
    foreign_rts_access(
      rts_did,
      rts_cid,
      address,
      0,		// drv_shared_in
      0,		// drv_owned_in,
      rts_addr2data_dly,
      data,
      ACKD
    ) ;
    
    repeat(10) @(posedge CLOCK) ;
    // wb to invalidate the entry
    cpu_wb_access(
      address, 
      0,
      0,
      0,
      0,
      5,
      data,
      cid
    ) ;
    
  }

  // RTO from CPU set the tag to be owned state,
  //  Then foreign RTO should be handled as cache read/invalidate
  void cpu_rto_foreign_rto(
    int dly,
    bit [39:0] address,
    int rto_addr2data_dly,
    bit [6:0] drv_shared_in,
    bit [511:0] data,
    int rto2rto_dly,
    bit [3:0] rto_did,
    bit [3:0] rto_cid,
    int frto_addr2data_dly
  )
  {
    bit [2:0] cid ;
    
    @dly(posedge CLOCK) ;
    cpu_read_access(
      RTO,			// command
      address,			// address
      0,			// drv_shared_in
      1'bx,			// exp_shared_out
      0,			// drv_owned_in
      0,			// exp_owned_out
      rto_addr2data_dly,	// addr2data_dly
      data,
      ACKD,
      cid
    ) ;
    
    @rto2rto_dly(posedge CLOCK) ;
    
    foreign_rto_owned_access(
      rto_did,
      rto_cid,
      address,
      drv_shared_in,		// drv_shared_in
      0,		// drv_owned_in,
      frto_addr2data_dly,
      data,
      ACKD
    ) ;
  }

  // RTS from CPU set the tag to be shared state,
  //  Then foreign RTO should be handled as cache invalidate
  void cpu_rts_foreign_rto(
    int dly,
    bit [39:0] address,
    int rts_addr2data_dly,
    bit [6:0] drv_shared_in,
    bit [511:0] data,
    int rts2rto_dly,
    bit [3:0] rto_did,
    bit [3:0] rto_cid,
    int frto_addr2data_dly
  )
  {
    bit [2:0] cid ;
    
    @dly(posedge CLOCK) ;
    cpu_read_access(
      RTS,			// command
      address,			// address
      0,			// drv_shared_in
      0,			// exp_shared_out
      0,			// drv_owned_in
      0,			// exp_owned_out
      rts_addr2data_dly,	// addr2data_dly
      data,
      ACKD,
      cid
    ) ;
    
    @rts2rto_dly(posedge CLOCK) ;
    
    foreign_rto_shared_access(
      rto_did,
      rto_cid,
      address,
      drv_shared_in,	// drv_shared_in
      0			// drv_owned_in,
    ) ;
  }
  
  void cpu_wb_cancel_on_foreign_rto (
    int dly,
    bit [39:0] address,
    bit [511:0] rd_data,
    bit [511:0] wr_data,
    bit [3:0] rto_did,
    bit [3:0] rto_cid,
    int cpu_wb_start_dly,
    int forgin_rto_a2d_dly,
    int cpu_wb_a2d_dly
  )
  {
    bit [2:0] cid ;
    event sys_arb_done = new ;
    event cpu_arb_go = new ;
    
    @dly(posedge CLOCK) ;

    // RTO to own the entry
    cpu_read_access(
      RTO,			//  bit [3:0]  command,
      address,			//  bit [39:0] address,
      7'b0,			//  bit [6:0] drv_shared_in,
      1'b0,			//  bit       exp_shared_out,
      7'b0,			//  bit [6:0] drv_owned_in,
      1'b0,			//  bit       exp_owned_out,
      10,			//  int   addr2data_dly,
      rd_data,			//  bit [511:0] data,
      ACKD,			// bit [2:0] ack,
      cid			// var bit [2:0] cid,
    ) ;
    
    if( debug )
      printf( 
        "cpu_wb_cancel_on_foreign_rto: RTO done @ %d\n",
        get_cycle()
      ) ;
    fork
    {
      // foreign RTO 
      foreign_rto_owned_access (
        rto_did,		// bit [3:0] did,
        rto_cid,		// bit [3:0] cid,
        address,		// bit [39:0] address,
        0,			// bit [6:0] drv_shared_in,  
        0,			// bit [6:0] drv_owned_in,
        forgin_rto_a2d_dly,	// int   addr2data_dly,
        wr_data,		// bit [511:0] data,
        ACKD,			// bit [2:0] ack,
        *,			// event  sys_arb_go = null,  
        sys_arb_done,		// event  sys_arb_done = null, 
        *,			// event  sys_snoop_done = null, 
        *,			// event  cpu_addr_done = null,  
        *,			// event  cpu_d_arb_go = null, 
        *,			// event  cpu_d_arb_done = null,   
        *			// event  sys_get_data = null    
      ) ;
    }
    
    {
      // bridge thread to catch the sys_arb_done trigger
      sys_arb_done.sync() ;
      if( debug )
        printf( 
          "cpu_wb_cancel_on_foreign_rto: foreign_rto arb done @ %d\n",
          get_cycle()
        ) ;
      cpu_arb_go.trigger_on() ;
    }
    
    {
      @cpu_wb_start_dly(posedge CLOCK) ;
      // schedule cpu addr drive after foreign_rto on system bus
      cpu_wb_access(
        address,		// bit [39:0] address,
        0,			// bit [6:0] drv_shared_in,
        0,			// bit       exp_shared_out,
        0,			// bit [6:0] drv_owned_in,
        1'b1,			// bit       exp_owned_out,
        cpu_wb_a2d_dly,		// int   addr2data_dly,
        wr_data,		// bit [511:0] data,
        cid,			// var bit [2:0] cid,
        cpu_arb_go,		// event  cpu_arb_go = null,
        *,			// event  cpu_arb_done = null, 
        *,			// event  sys_addr_done = null, 
        *,			// event  sys_snoop_done = null, 
        *,			// event  sys_data_arb_go = null,
        *,			// event  sys_data_done = null,
        *			// event  cpu_ack_done = null
      ) ;
    }
    
    join
  
  
  }
}
