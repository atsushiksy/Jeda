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

void random_test_thread( int num, int cycle, semaphore complete_sem )
{
  bit [39:0] address;
  bit [2:0] cid ;
  bit [511:0] data0 ;
  bit [511:0] data1 ;
  int dly0, dly1, dly2, dly3, dly4, dly5 ;
  bit [2:0] e_did ;
  bit [2:0] e_cid ;
  int seq_no ;
  int flg ;
  
  printf( "random_test_thread: %d %d @ %d\n", num, cycle, get_cycle() ) ;
  fork
  {
  
    // cycle = get_cycle()+1 ;
    while( get_cycle() < cycle ) {
  
      // allocate unique address for tag entry
      address = {urandom(),urandom(),6'b0} ;
      while( !tag_region.check_enter( address[OFFSET_BITS] ) )
        address = {urandom(),urandom(),6'b0} ;
    
      // allocate unique did & cid pair
      flg = 1 ;
      while( flg ) {
        e_did = random() % 8 ;
        e_cid = random() % 8 ;
        if( e_did != bus_drive.dev_id ) { // did is not dut's dev_id
          if( did_cid_region.check_enter( {e_did,e_cid} ) )
            flg = 0 ;
        }
      }
    
      seq_no = random() % 5 ;
      // seq_no = 0 ;
      
      dly0 = random() % 30 ; // interval
    
      printf( 
        "random_test_thread %d: @%0d seq_no:%0d did:%h cid:%h address:%h\n",
        num, get_cycle(), seq_no, e_did, e_cid, address
      ) ;
    
      data0 = {urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom()} ;
               
      data1 = {urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom(),
              urandom(),urandom(),urandom(),urandom()} ;
    
      case( seq_no ) {
        0:
        {
          dly1 = random() % 10 ; 
          dly2 = random() % 30 ; 
          dly3 = random() % 10 ;
          bus_drive.cpu_rto_foreign_rts(
            dly0,		// int dly,
            address,	// bit [39:0] address,
            dly1,		// int rto_addr2data_dly,
            data0,	// bit [511:0] data,
            dly2,		// int rto2rts_dly,
            e_did,	// bit [3:0] rts_did,
            e_cid,	// bit [3:0] rts_cid,
            dly3		// int rts_addr2data_dly
          ) ;

          printf( 
            "random_test_thread %d: @%0d seq_no:%0d cpu_rto_foreign_rts completed\n",
            num, get_cycle(), seq_no
          ) ;
        }
      
        1:
        {
          dly1 = random() % 10 ; 
          dly2 = random() % 30 ; 
          dly3 = random() % 10 ;
          bus_drive.cpu_rto_foreign_rto(
            dly0,		// int dly,
            address,	// bit [39:0] address,
            dly1,		// int rto_addr2data_dly,
            6'b0,	// bit [6:0] drv_shared_in,
            data0,	// bit [511:0] data,
            dly2,		// int rto2rto_dly,
            e_did,	// bit [3:0] rto_did,
            e_cid,	// bit [3:0] rto_cid,
            dly3		// int frto_addr2data_dly
          ) ;
          printf( 
            "random_test_thread %d: @%0d seq_no:%0d cpu_rto_foreign_rto completed\n",
            num, get_cycle(), seq_no
          ) ;
        }
      
        2:
        {
          dly1 = random() % 10 ; 
          dly2 = random() % 30 ; 
          dly3 = random() % 10 ;
          bus_drive.cpu_rts_foreign_rto(
            dly0,		// int dly,
            address,	// bit [39:0] address,
            dly1,		// int rts_addr2data_dly,
            6'b0,		// bit [6:0] drv_shared_in,
            data0,	// bit [511:0] data,
            dly2,		// int rts2rto_dly,
            e_did,	// bit [3:0] rto_did,
            e_cid,	// bit [3:0] rto_cid,
            dly3		// int frto_addr2data_dly
          ) ;
          printf( 
            "random_test_thread %d: @%0d seq_no:%0d cpu_rts_foreign_rto completed\n",
            num, get_cycle(), seq_no
          ) ;
        }
        3:
        {
          dly1 = random() % 30 ; 
          dly2 = random() % 10 ; 
          dly3 = random() % 10 ;
          bus_drive.cpu_wb_cancel_on_foreign_rto (
            dly0,		// int dly,
            address,		// bit [39:0] address,
            data0,		// bit [511:0] rd_data,
            data1,		// bit [511:0] wr_data,
            e_did,		// bit [3:0] rto_did,
            e_cid,		// bit [3:0] rto_cid,
            dly1,		// int cpu_wb_start_dly,
            dly2,		// int forgin_rto_a2d_dly,
            dly3		// int cpu_wb_a2d_dly
          ) ;
          printf( 
            "random_test_thread %d: @%0d seq_no:%0d cpu_wb_cancel_on_foreign_rto completed\n",
            num, get_cycle(), seq_no
          ) ;
        }
        4:
        { 
          // just delay
          @dly0(posedge CLOCK) ;
        }
      } // end of case
      
      tag_region.exit( address[OFFSET_BITS] ) ;
      did_cid_region.exit( {e_did,e_cid} ) ;
      printf( 
        "random_test_thread %d: returning did:%h cid:%h @ %d\n", 
        num, e_did, e_cid, get_cycle()
      ) ;
    } // end of while
  
    complete_sem.put() ;
  
  }
  knife
}

void snooper_test (
)
{
  int i ;
  semaphore complete_sem = new(0) ;
  int num_thread = 4 ;
  
  for( i = 0 ; i < num_thread ; i++ ) {
    random_test_thread( 
      i, 500, complete_sem 
    ) ;
  }
  
  // wait for thread completion
  complete_sem.get( num_thread ) ;
  
  
  // some delay
  @30(posedge CLOCK) ;

}

    
  
  
  
