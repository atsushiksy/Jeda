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
// bus snooper model
//  
//  address, tag structure
//    One line is 64 byte
//    1MByte direct map Cache
//    Address[39:0] are subdivided as:
// 
//    3             2 1                 
//    9             0 9                6 5           0
//   | TAG bit[19:0] |  Offset [13:0]   | byte offset |
// 
//  CPU interface
//    Request to CPU, Request from CPU are handled as single stream,
//    and order is maintained.
//  
//
//  System Bus
//    Bus transaction has device_id and command_id, completion can be
//     out of order
//
//    Snoop signal will be driven at 3rd cycle after the address cycle  
//
//    |   0   |   1   |   2   |   3   |  
//      <ADDR>
//                             <shared>
//                             <owned>
//
//    Shared signal will be driven when 
//       1) Foreign RTS to an shared entry
//       2) Own RTO to an entry which is already cached (Cancel data read)
//
//    Owned signal will be driven when
//       1) Foreign RTS/RTO to an owned entry
//       2) Onw WB to an entry which is not owned (Foreign RTO hits before
//           WB hits the system bus
//
#include "../include/sys_defines.jh"
#include "../utils/bus_class_0.jh"

class bus_snooper {
  int dev_id ;
  int last_a_winner ;        // for address bus arbitration
  int last_d_winner ;        // for data bus arbitration
  semaphore sys_a_sem ;
  semaphore sys_d_sem ;
  semaphore cmd_id_sem ;     // semaphore for cmd id initalized to be 8
  int cid_busy[8] ;          // busy flag for cmd id
  semaphore cpu_ack_sem ;    // semaphore for CPU ack (for ordering)
  semaphore sys_ack_sem ;    // semaphore for SYS ack (for ordering)
  semaphore wb_data_sem ;    // semaphore for CPU wb data (for ordering)
  bit [2:0]  tag_state[CACHE_SIZE] ;   // tag state
  bit [19:0]  tag_value[CACHE_SIZE] ;    // tag value
  semaphore tag_access_sem ; // avoid racing on tag access
  semaphore cpu_a_sem ;
  int cpu_a_req ;
  event   cpu_a_gnt ;
  semaphore cpu_d_sem ;
  int cpu_d_req ;
  event   cpu_d_gnt ;
  
  // the system and prosessor bus class where this snooper is talking to
  system_bus       sys_bus ;
  processor_bus    cpu_bus ;
  
  int debug ;
  
  void new( 
    int id,
    system_bus       a_sys_bus,
    processor_bus    a_cpu_bus
  ) {
    int i ;
    
    dev_id = id ;
    sys_bus = a_sys_bus ;
    cpu_bus = a_cpu_bus ;
    
    last_a_winner = 0 ;
    last_d_winner = 0 ;
    cpu_a_req = 0 ;
    cpu_d_req = 0 ;
    cmd_id_sem = new( 8 ) ;   // initialized to 8, 
                              // max 8 outstanding cmd can be issued 
    for( i = 0 ; i < 8 ; i++ ) cid_busy[i] = 0 ;
    cpu_ack_sem = new ;
    sys_ack_sem = new ;
    wb_data_sem = new ;
    sys_a_sem = new ;
    sys_d_sem = new ;
    cpu_a_sem = new ;
    cpu_d_sem = new ;
    tag_access_sem = new ;
    
    // alloc event
    cpu_a_gnt = new ;
    cpu_d_gnt = new ;
    for( i = 0 ; i < CACHE_SIZE ; i++ ) {
      tag_state[i] = TAG_I ; // invalid
      tag_value[i] = 0 ;
    }
    
    debug = 0 ;
    
  }
  
  // address bus arbitration
  local void sys_a_arb_loop() {
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
  local void sys_d_arb_loop() {
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
  
  // CPU interface arbitration
  local void cpu_a_arb_loop() {
    int last_winner = 0 ;
    bit a_req ;
    fork
    while(1) {
      @(posedge cpu_bus.clock) ;
      a_req = cpu_bus.cpu_a_req.0 ;
      if( cpu_a_req && ( (last_winner==0) || (a_req == 0) ) ) {
        cpu_bus.cpu_a_gnt.0 = 0 ;
        @(negedge cpu_bus.clock) ;
        cpu_a_req = 0 ;
        last_winner = 1 ;
        cpu_a_gnt.trigger() ;
      }
      else if( a_req ) {
        last_winner = 0 ;
        cpu_bus.cpu_a_gnt.0 = 1 ;
        @1 cpu_bus.cpu_a_gnt.0 <= 0 weak ;
      }
    }
    knife
  }
  
  local void cpu_d_arb_loop() {
    int last_winner = 0 ;
    bit d_req ;
    fork
    while(1) {
      @(posedge cpu_bus.clock) ;
      d_req = cpu_bus.cpu_d_req.0 ;
      if( cpu_d_req && ( (last_winner==0) || (d_req == 0) ) ) {
        cpu_bus.cpu_d_gnt.0 = 0 ;
        @(negedge cpu_bus.clock) ;
        cpu_d_req = 0 ;
        last_winner = 1 ;
        cpu_d_gnt.trigger() ;
      }
      else if( d_req ) {
        last_winner = 0 ;
        cpu_bus.cpu_d_gnt.0 = 1 ;
        @1 cpu_bus.cpu_d_gnt.0 <= 0 weak ;
      }
    }
    knife
  }
  
  local void self_rts ( bit [39:0] address, bit [2:0] c_id ) {
    bit [6:0] shared ;
    fork
    {
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      
      // printf( "get time in = %d %d\n", get_time(), get_cycle() ) ;
      @(negedge sys_bus.clock) ; // middle of 2 cycle
      // block tag access for a half cycle to make sure the access by
      // next pipe gets the value after update
      // printf( "get time = %d %d\n", get_time(), get_cycle() ) ;
      tag_access_sem.get() ; // block tag access
      
      @(posedge sys_bus.clock) ; // end of 3 cycle
      shared = sys_bus.shared_in.0 ; 
      //breakpoint ;
      if( shared ) {
        tag_state[address[OFFSET_BITS]] = TAG_S ;
        tag_value[address[OFFSET_BITS]] = address[TAG_BITS] ;
      }
      else {
        tag_state[address[OFFSET_BITS]] = TAG_E ;
        tag_value[address[OFFSET_BITS]] = address[TAG_BITS] ;    
      }
      if( debug ) {
        printf( 
          "self_rts: a:%h cid:%h sh %b tag_state[%h]: %b tag_value: %h\n",
          address, c_id, shared, address[OFFSET_BITS],
          tag_state[address[OFFSET_BITS]],
          address[TAG_BITS]
        ) ;
      }
      tag_access_sem.put() ; // block tag access
      // printf( "put time = %d %d\n", get_time(), get_cycle() ) ;
    }
    knife
  }
  
  local void self_rto ( bit [39:0] address, bit [2:0] c_id ) {
    bit shared ;
    bit [2:0] t_s ;
    bit [19:0] t_v ;
    fork
    {
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      tag_access_sem.get() ; 
      // semaphore is used to make sure that tag read is after the update
      // on the previous pipeline. 
      t_s = tag_state[address[OFFSET_BITS]] ;
      t_v = tag_value[address[OFFSET_BITS]] ;
      tag_access_sem.put() ; // block tag access
      if( (t_s != TAG_I) && (t_v == address[TAG_BITS]) ) {
        // shared signal is used for RTO to notify the consistent value is
        // already within the cache, thus no data transfer is necessary
        sys_bus.shared_out.0 = 1'b1 ;
        shared = 1 ;
        @1 sys_bus.shared_out.0 <= 1'b0 weak ;  // non blocking drive
      }
      else shared = 0 ;
      
      // printf( "get time in = %d %d\n", get_time(), get_cycle() ) ;
      @(negedge sys_bus.clock) ; // middle of 2 cycle
      // block tag access for a half cycle to make sure next pipe won't
      //   read the tag before this update
      // printf( "get time = %d %d\n", get_time(), get_cycle() ) ;
      tag_access_sem.get() ;
      
      @(posedge sys_bus.clock) ; // end of 3 cycle
      //breakpoint ;
      tag_state[address[OFFSET_BITS]] = TAG_M ;
      tag_value[address[OFFSET_BITS]] = address[TAG_BITS] ;
      if( debug ) {
        printf( 
          "self_rto: a:%h cid:%h sh %b tag_state[%h]: %b tag_value: %h\n",
          address, c_id, shared, address[OFFSET_BITS],
          tag_state[address[OFFSET_BITS]],
          address[TAG_BITS]
        ) ;
      }
      // release the tag access semaphore
      tag_access_sem.put() ;
      // printf( "put time = %d %d\n", get_time(), get_cycle() ) ;
    }
    knife
  }
  
  local void self_wb ( bit [39:0] address, bit [2:0] c_id ) {
    bit owned ;
    bit [2:0] t_s ;
    bit [19:0] t_v ;
    int time ;
    fork
    {
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      time = get_cycle() ;
      tag_access_sem.get() ; 
      // semaphore is used to make sure that tag read is after the update
      // on the previous pipeline. 
      t_s = tag_state[address[OFFSET_BITS]] ;
      t_v = tag_value[address[OFFSET_BITS]] ;
      tag_access_sem.put() ; // block tag access
      if( (t_v != address[TAG_BITS]) || ((t_s != TAG_O) && (t_s != TAG_M)) ) {
        // This is the case foreign rto hits just before wb from cpu side
        // happens. The ownership of an entry just follows the address bus
        // activity, thus this wb will be ignored. Cache in cpu side is 
        // responsible to hold the value till ACK/NACK is recieved.
        // owned signal is used for WB to notify the entry is no longer
        // valid and write data must be ignored
        printf( 
          "bus_snooper: self wb canceled tv:%h ta:%h t_s: %b @ %d %d\n",
           t_v, address[TAG_BITS], t_s, time, get_cycle() 
        ) ;
        //breakpoint ;
        sys_bus.owned_out.0 = 1'b1 ;
        @1 sys_bus.owned_out.0 <= 1'b0 weak ;
      }
      else {
        printf( 
          "bus_snooper: self wb tv:%h ta:%h t_s: %b @ %d\n",
           t_v, address[TAG_BITS], t_s, get_cycle() 
        ) ;
      }
      // printf( "get time in = %d %d\n", get_time(), get_cycle() ) ;
      @(negedge sys_bus.clock) ; // middle of 3 cycle
      // block tag access for a half cycle to make sure next pipe won't
      //   read the tag before this update
      // printf( "get time = %d %d\n", get_time(), get_cycle() ) ;
      tag_access_sem.get() ;
      
      @(posedge sys_bus.clock) ; // end of 3 cycle
      owned = sys_bus.owned_out.0 ; // self output
      if( !owned ) {
        // invalidate the tag entry
        tag_state[address[OFFSET_BITS]] = TAG_I ;
      }
      if( debug ) {
        printf( 
          "self_wb: a:%h cid:%h own %b tag_state[%h]: %b tag_value: %h\n",
          address, c_id, owned, address[OFFSET_BITS],
          tag_state[address[OFFSET_BITS]],
          address[TAG_BITS]
        ) ;
      }
      // release the tag access semaphore
      tag_access_sem.put() ;
      // printf( "put time = %d %d\n", get_time(), get_cycle() ) ;
    }
    knife
  }

  // Forgin System bus trafic
  local void foreign_read_to_cache( 
    bit [3:0] command, bit [39:0] address, bit [2:0] d_id, bit [2:0] c_id 
  ) {
    bit [2:0] ack ;
    int count ;
    bit [511:0] data ;
    
    cpu_a_sem.get() ;
    cpu_a_req = 1 ;
    cpu_a_gnt.sync() ;
    
    @0 cpu_bus.command.0 = command ;
    cpu_bus.address.0 = address ;
    cpu_bus.sys_valid.0 = 1'b1 ;
    cpu_a_sem.put() ;
    
    if( debug ) {
      printf( 
        "foreign_read_to_cache: cpu bus driven cmd %b adr %h @ %d\n",
        command, address, get_cycle() 
      ) ;
    }
    @1 cpu_bus.command.0 <= NOP weak ;
    @1 cpu_bus.address.0 <= 0 weak ;
    @1 cpu_bus.sys_valid.0 <= 0 weak ;
    sys_ack_sem.get() ; // wait to be top of ack queue
    
    if( command == INV ) ack = ACK ;
    else ack = ACKD ;
    
    if( @0,250(cpu_bus.cpu_ack.0 == ack ) )
    {
      ack = cpu_bus.cpu_ack.0 ;
      data = cpu_bus.data.0 ;
      if( debug ) {
        printf( 
          "foreign_read_to_cache: got cpu data ack %b d %h for adr %h @ %d\n",
          ack, data, get_cycle() 
        ) ;
      }
    }
    else {
      error( "foreign_read_to_cache: timeout detected\n" ) ;
      ack = NACK ;
      data = 0 ;
    }
    
    @(posedge cpu_bus.clock) ;
    sys_ack_sem.put() ; // wait to be top of ack queue
    
    if( command != INV ) {
      sys_d_sem.get() ;
      @(posedge sys_bus.clock) ;
      sys_bus.d_req_out.0 = 1'b1 ;
      @1 sys_bus.d_req_out.0 = 1'b0 weak;
      while( last_d_winner !== dev_id ) {
        sys_bus.d_req_out.0 = 1'b1 ;
        @1 sys_bus.d_req_out.0 = 1'b0 weak;
      }
      sys_d_sem.put() ;
    
      // drive data, ids, and data to system bus
      sys_bus.ack.0 = ack ;
      sys_bus.ack_did.0 = d_id ;
      sys_bus.ack_cid.0 = c_id ;
      sys_bus.data.0 = data ;
      if( debug ) {
        printf( 
          "foreign_read_to_cache: sys data driven did %h cid %h ack %b d %h for adr %h @ %d\n",
          d_id, c_id, ack, data, get_cycle() 
        ) ;
      }
      @1 sys_bus.ack.0 <= 0 weak ;
      @1 sys_bus.ack_did.0 <= 0 weak ;
      @1 sys_bus.ack_cid.0 <= 0 weak ;
      @1 sys_bus.data.0 <= 0 weak ;
      
    }
  }
  
  local void foreign_rts ( bit [39:0] address, bit [2:0] d_id, bit [2:0] c_id ) {
    bit [2:0] t_s ;
    bit [19:0] t_v ;
    bit [6:0] shared ;
    int flag = 0 ;
    fork
    {
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      tag_access_sem.get() ; 
      // semaphore is used to make sure that tag read is after the update
      // on the previous pipeline. 
      t_s = tag_state[address[OFFSET_BITS]] ;
      t_v = tag_value[address[OFFSET_BITS]] ;
      tag_access_sem.put() ; // block tag access
      printf( 
        "foreign_rts tag %h t_s: %b t_v: %h\n", address[OFFSET_BITS], t_s, t_v
      ) ;
      if( (t_v == address[TAG_BITS]) && (t_s != TAG_I) ) {
        case( t_s ) {
          TAG_S, TAG_E:
            {
              flag = 1 ;
              sys_bus.shared_out.0 = 1'b1 ;
              @1 sys_bus.shared_out.0 <= 1'b0 weak ;
            }
          TAG_O, TAG_M:
            {
              flag = 2 ;
              sys_bus.shared_out.0 = 1'b1 ;
              sys_bus.owned_out.0 = 1'b1 ;
              @1 sys_bus.shared_out.0 <= 1'b0 weak ;
              @1 sys_bus.owned_out.0 <= 1'b0 weak ;
            }
          default:
            {
              error( "Boo! tag = %b\n", t_s ) ;
            }
        }
      }
      
      // printf( "get time in = %d %d\n", get_time(), get_cycle() ) ;
      @(negedge sys_bus.clock) ; // middle of 2 cycle
      // block tag access for a half cycle to make sure next pipe won't
      //   read the tag before this update
      // printf( "get time = %d %d\n", get_time(), get_cycle() ) ;
      tag_access_sem.get() ;
      
      @(posedge sys_bus.clock) ; // end of 2 cycle
      shared = sys_bus.shared_in.0 ; 
      if( flag ) {
        if( flag == 1 )
          tag_state[address[OFFSET_BITS]] = TAG_S ;
        else
          tag_state[address[OFFSET_BITS]] = TAG_O ;
      }
      // release the tag access semaphore
      tag_access_sem.put() ;
      // printf( "put time = %d %d\n", get_time(), get_cycle() ) ;
      //breakpoint ;
      if( flag == 2 ) {
        // This entry is owned 
        foreign_read_to_cache( RTS, address, d_id, c_id ) ;
      }
    }
    knife
  }

  local void foreign_rto ( bit [39:0] address, bit [2:0] d_id, bit [2:0] c_id ) {
    bit [2:0] t_s ;
    bit [19:0] t_v ;
    bit [6:0] shared ;
    int flg = 0 ;
    fork
    {
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      tag_access_sem.get() ; 
      // semaphore is used to make sure that tag read is after the update
      // on the previous pipeline. 
      t_s = tag_state[address[OFFSET_BITS]] ;
      t_v = tag_value[address[OFFSET_BITS]] ;
      tag_access_sem.put() ; // block tag access
      // breakpoint ;
      printf( 
        "foreign_rto tag %h t_s: %b t_v: %h\n", address[OFFSET_BITS], t_s, t_v
      ) ;
      printf( "f_rto t_v:%h a[t]:%h\n", t_v, address[TAG_BITS] ) ;
      if( (t_v == address[TAG_BITS]) && (t_s != TAG_I) ) {
        case( t_s ) {
          TAG_S, TAG_E:
            {
              flg = 1 ;
            }
          TAG_O, TAG_M:
            {
              flg = 2 ;
              sys_bus.owned_out.0 = 1'b1 ;
              @1 sys_bus.owned_out.0 <= 1'b0 weak ;
            }
          default:
            {
              error( "Boo! tag = %b\n", t_s ) ;
            }
        }
      }
      if( debug )
        printf( "foreign_rto: flag = %0d\n", flg ) ;
      
      // printf( "get time in = %d %d\n", get_time(), get_cycle() ) ;
      @(negedge sys_bus.clock) ; // middle of 2 cycle
      // block tag access for a half cycle to make sure next pipe won't
      //   read the tag before this update
      // printf( "get time = %d %d\n", get_time(), get_cycle() ) ;
      tag_access_sem.get() ;
      
      @(posedge sys_bus.clock) ; // end of 2 cycle
      shared = sys_bus.shared_in.0 ; 
      if( flg ) {
        tag_state[address[OFFSET_BITS]] = TAG_I ;
      }
      // release the tag access semaphore
      tag_access_sem.put() ;
      // printf( "put time = %d %d\n", get_time(), get_cycle() ) ;
      // breakpoint ;
      printf( "foreign_rto: flag %0d shared:%b\n", flg, shared ) ;
      if( flg == 2 && !shared) {
        // This entry is owned 
        foreign_read_to_cache( RTO, address, d_id, c_id ) ;
      }
      else if( flg ) {
        foreign_read_to_cache( INV, address, d_id, c_id ) ;
      }
    }
    knife
  }

  // system bus snoop loop
  local void sys_snoop_loop() {
    bit [3:0] command ;
    bit [39:0] address ;
    bit [2:0]  d_id ;
    bit [2:0]  c_id ;
    fork
    while(1){
      @(posedge sys_bus.clock) ;
      // sample bus signals
      command = sys_bus.command.0 ;
      address = sys_bus.address.0 ;
      d_id = sys_bus.dev_id.0 ;
      c_id = sys_bus.cmd_id.0 ;
      
      if( command != NOP ) {
        printf( 
          "sys_snoop_loop: got command %h address %h @%0d\n", 
          command, address, get_cycle()
        ) ;
        if( dev_id == d_id ) { // own command
          case( command ) {
            RTS: self_rts( address, c_id ) ;
            RTO: self_rto( address, c_id ) ;
            WB:  self_wb( address, c_id ) ;
          }
        }
        else{  // foreign command
          case( command ) {
            RTS: foreign_rts( address, d_id, c_id ) ;
            RTO: foreign_rto( address, d_id, c_id ) ;
            // WB:  do nothig
          }
        }
      }
    }
    knife
  
  }
  
  // cpu request polling voids
  local bit [2:0] alloc_cid() {
    int i ;
    cmd_id_sem.get() ; // initalized to 8, no more than 8
                                         //  will be allocated.
    for( i = 0 ; i < 8 ; i++ ) 
      if( !cid_busy[i] ) break ;
    cid_busy[i] = 1 ;
    //if( debug )
    //  printf( "bus_snooper.alloc_cid: cid = %d @ %d\n", i, get_cycle() ) ;
    return( i ) ;
  }
  
  void dealloc_cid( bit[2:0] cid ) {
    fork
    {
      @7 (posedge sys_bus.clock) ;  // delay 7 cycle before releasing cid
      cid_busy[cid] = 0 ;
      cmd_id_sem.put() ;
    }
    knife
  }
  
  local void get_sys_a_bus() {
    sys_a_sem.get() ;
    @(posedge sys_bus.clock) ;
    sys_bus.a_req_out.0 = 1'b1 ;
    @1 sys_bus.a_req_out.0 = 1'b0 weak;
    while( last_a_winner != dev_id ) {
      sys_bus.a_req_out.0 = 1'b1 ;
      @1 sys_bus.a_req_out.0 = 1'b0 weak;
    }
    sys_a_sem.put() ;  
  }
  
  // CPU to System RTS
  local void cpu_sys_rts( bit[39:0] address ) {
    bit [2:0]  c_id ;
    bit [511:0] data = 0 ;
    bit [1:0] ack ;
    // get sys_bus command id
    c_id = alloc_cid() ;
    if( debug ) 
      printf( 
        "cpu_sys_rts: adr %h cid %d @ %d\n", address, c_id, get_cycle() 
      ) ;
    get_sys_a_bus() ;
    sys_bus.command.0 = RTS ;
    sys_bus.address.0 = address ;
    sys_bus.dev_id.0 = dev_id ;
    sys_bus.cmd_id.0 = c_id ;
    @1 sys_bus.command.0 <= 0 weak ;
    @1 sys_bus.address.0 <= 0 weak ;
    @1 sys_bus.dev_id.0 <= 0 weak ;
    @1 sys_bus.cmd_id.0 <= 0 weak ;
    @(posedge sys_bus.clock) ; // end of 0 cycle
    @(posedge sys_bus.clock) ; // end of 1 cycle
    @(posedge sys_bus.clock) ; // end of 2 cycle
    @(posedge sys_bus.clock) ; // end of 3 cycle
    fork
    {
      // wait for sys ack on the d_id,c_id
      if ( 
        p_or( 
          @0,255(
            sys_bus.ack.0 == ACKD, sys_bus.ack_did.0 == dev_id,
            sys_bus.ack_cid.0 == c_id
          ),
          @0,255(
            sys_bus.ack.0 == NACK, sys_bus.ack_did.0 == dev_id,
             sys_bus.ack_cid.0 == c_id
          )
        )
      ) {
        ack = sys_bus.ack.0 ;
        if( ack == ACKD ) data = sys_bus.data.0 ;
        else data = 0 ;
        if( debug ) {
          printf(
            "cpu_sys_rts: got sys ack %b data: %h %h @ %d\n", 
            ack, data, sys_bus.data.0, get_cycle()
          ) ;
        }
      }
      else
      {  // timeout error
        ack = NACK ;
        data = 0 ;
      }
    }
    {
      // this thread is to keep the order of ack to cpu as the order of request
      cpu_ack_sem.get() ;
    }
    join
    
    dealloc_cid( c_id ) ;
    
    cpu_d_sem.get() ;
    cpu_d_req = 1 ;
    
    cpu_d_gnt.sync() ;
    
    @1 cpu_bus.sys_ack.0 = ack ;
    cpu_bus.data.0 = data ;
    
    if( debug ) {
      printf( 
        "cpu_sys_rts: driven cpu ack %b data: %h @ %d\n",
        ack, data, get_cycle()
      ) ;
    }
    @1 cpu_bus.sys_ack.0 <= 0 weak ;
    @1 cpu_bus.data.0 <= 0 weak ;
    cpu_d_sem.put() ;
    cpu_ack_sem.put() ;
    
  }
  
  // CPU to System RTO
  local void cpu_sys_rto( bit[39:0] address ) {
    bit [2:0]  c_id ;
    bit [511:0] data = 0;
    bit [1:0] ack ;
    bit shared ;

    c_id = alloc_cid() ;
    get_sys_a_bus() ;
    sys_bus.command.0 = RTO ;
    sys_bus.address.0 = address ;
    sys_bus.dev_id.0 = dev_id ;
    sys_bus.cmd_id.0 = c_id ;
    @1 sys_bus.command.0 <= 0 weak ;
    @1 sys_bus.address.0 <= 0 weak ;
    @1 sys_bus.dev_id.0 <= 0 weak ;
    @1 sys_bus.cmd_id.0 <= 0 weak ;
    @(posedge sys_bus.clock) ; // end of 0 cycle
    @(posedge sys_bus.clock) ; // end of 1 cycle
    @(posedge sys_bus.clock) ; // end of 2 cycle
    @(posedge sys_bus.clock) ; // end of 3 cycle
    // sample self shared to check if valid data is already in the cache
    shared = sys_bus.shared_out.0 ; 
    fork
    {
      if( shared ) {  // just return ack with no data
        ack = ACK ;
        data = 0 ;
      } 
      else {
        // wait for sys ack on the d_id,c_id
        if ( 
          p_or( 
            @0,255( 
              sys_bus.ack.0 == ACKD, sys_bus.ack_did.0 == dev_id,
              sys_bus.ack_cid.0 == c_id  
            ),
            @0,255(
              sys_bus.ack.0 == ACK, sys_bus.ack_did.0 == dev_id,
              sys_bus.ack_cid.0 == c_id 
            )
          )
        ) {
          ack = sys_bus.ack.0 ;
          if( ack == ACKD ) data = sys_bus.data.0 ;
          else data = 0 ;
          if( debug ) {
            printf( 
              "cpu_sys_rto: got sys ack %b data: %h @ %d\n", 
              ack, data, get_cycle()
            ) ;
          }
        }
        else {
          ack = NACK ;  // timeout error 
        }
      }
    }
    {
      // this thread is to keep the order of ack to cpu as the order of request
      cpu_ack_sem.get() ;
    }
    join
    
    dealloc_cid( c_id ) ;
    
    cpu_d_sem.get() ;
    cpu_d_req = 1 ;
    
    cpu_d_gnt.sync() ;
    
    @2 cpu_bus.sys_ack.0 = ack ;
    cpu_bus.data.0 = data ;
    if( debug ) {
      printf( 
        "cpu_sys_rto: driven cpu ack %b data: %h @ %d\n", 
        ack, data, get_cycle() 
      ) ;
    }
    @1 cpu_bus.sys_ack.0 <= 0 weak ;
    @1 cpu_bus.data.0 <= 0 weak ;
    cpu_d_sem.put() ;
    cpu_ack_sem.put() ;
    
  }

  // CPU to System WB
  local void cpu_sys_wb( bit[39:0] address ) {
    bit [2:0]  c_id ;
    bit [511:0] data ;
    bit [2:0] ack ;
    bit owned ;
    event ordered = new ;
    
    fork
    {
      // get sys_bus semaphore
      c_id = alloc_cid() ;
      get_sys_a_bus() ;
      sys_bus.command.0 = WB ;
      sys_bus.address.0 = address ;
      sys_bus.dev_id.0 = dev_id ;
      sys_bus.cmd_id.0 = c_id ;
      @1 sys_bus.command.0 <= 0 weak ;
      @1 sys_bus.address.0 <= 0 weak ;
      @1 sys_bus.dev_id.0 <= 0 weak ;
      @1 sys_bus.cmd_id.0 <= 0 weak ;
      @(posedge sys_bus.clock) ; // end of 0 cycle
      @(posedge sys_bus.clock) ; // end of 1 cycle
      @(posedge sys_bus.clock) ; // end of 2 cycle
      @(posedge sys_bus.clock) ; // end of 3 cycle
      owned = sys_bus.owned_out.0 ; 
      if( owned ) printf( "cpu_sys_wb: WB cancel detected\n" ) ;
      fork
      {
        cpu_ack_sem.get() ;
        ordered.trigger_on() ;
      }
      knife
    }
    {
      wb_data_sem.get() ;
      // wait for data for wb from cpu, 
      //  time out value is a half of sys timeout
      if( @1,127( cpu_bus.cpu_ack.0 == WRBD ) ) {
        ack = ACKD ;
        data = cpu_bus.data.0 ;
      }
      else {
        ack = NACK ;
        data = 0 ;
      }
      wb_data_sem.put() ;
    }
    join
    

    // get sys_bus semaphore
    if( !owned ) { // only drive data if owned is not pulled
      sys_d_sem.get() ;
      @(posedge sys_bus.clock) ;
      sys_bus.d_req_out.0 = 1'b1 ;
      @1 sys_bus.d_req_out.0 = 1'b0 weak;
      while( last_d_winner !== dev_id ) {
        sys_bus.d_req_out.0 = 1'b1 ;
        @1 sys_bus.d_req_out.0 = 1'b0 weak;
      }
      sys_d_sem.put() ;
    
      // drive data, ids, and data to system bus
      printf( "driving data for WB to sys_bus\n" ) ;
      sys_bus.ack.0 = ack ;
      sys_bus.ack_did.0 = dev_id ;
      sys_bus.ack_cid.0 = c_id ;
      sys_bus.data.0 = data ;
      @1 sys_bus.ack.0 <= 0 weak ;
      @1 sys_bus.ack_did.0 <= 0 weak ;
      @1 sys_bus.ack_cid.0 <= 0 weak ;
      @1 sys_bus.data.0 <= 0 weak ;
    }
    
    dealloc_cid( c_id ) ;
    
    ordered.sync() ;
    cpu_d_sem.get() ;
    cpu_d_req = 1 ;
    
    cpu_d_gnt.sync() ;
    
    if( owned ) {
      ack = WRBC ; // WB cancel
      // printf( "drive WRBC on WB cancel\n" ) ;
    }
    else {
      if( ack == ACKD ) ack = ACK ;
      else ack = NACK ;
    } 
    @2 cpu_bus.sys_ack.0 = ack ;
    @1 cpu_bus.sys_ack.0 <= 0 weak ;
    cpu_d_sem.put() ;
    cpu_ack_sem.put() ;

  }

  local void cpu_request_polling_loop() {
    clone bit [3:0] command ;
    clone bit [39:0] address ;
    bit cpu_valid ;
    fork
      while(1) {
        @(posedge cpu_bus.clock) ;
        command = cpu_bus.command.0 ;
        address = cpu_bus.address.0 ;
        cpu_valid = cpu_bus.cpu_valid.0 ;
        if( cpu_valid ) {
          printf( 
            "cpu_bus: cmd:%h addr %h valid:%b\n", 
            command, address, cpu_valid
          ) ;
          fork
            case(command) {
              RTS: cpu_sys_rts( address ) ;
              RTO: cpu_sys_rto( address ) ;
              WB: cpu_sys_wb( address ) ;
            }
          knife
        }
      }
    knife
  }
  
  
  // start all loops
  void start_all_loops() {
    
    sys_a_arb_loop() ;
    sys_d_arb_loop() ;
    cpu_a_arb_loop() ;
    cpu_d_arb_loop() ;
    sys_snoop_loop() ;
    cpu_request_polling_loop() ;
    
  }
  

} // end of bus_snooper class
