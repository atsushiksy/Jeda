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

#include "../utils/cell_classes_0.jh" 
#include "../utils/rand_classes_0.jh" 

#define MAX_ENABLE_DELAY 1000

// cell receiver for UTOPIA Tx port
class cell_receiver {
  int id ;
  signal tx_clk ;
  signal data ;
  signal soc ;
  signal en ;
  signal clav ;
  single_cell_queue rcv_que ;
  int receive_count ;
  int receive_num ;
  
  void new(
    int a_id,
    signal a_tx_clk,
    signal a_data,
    signal a_soc,
    signal a_en,
    signal a_clav
  ) {
    id = a_id ;
    tx_clk = a_tx_clk ;
    data = a_data ;
    soc = a_soc ;
    en = a_en ;
    clav = a_clav ;
    rcv_que = new ;
    
    // drive signals to inactive state
    clav.0 = 1'b0 ; // active high 
    receive_count = 0 ;
    receive_num = 0 ;
  }
    
  // control the speed of the receive side
  void receive_enable_loop( int rcv_dly_c, int rcv_dly_r ) {
    int dly ;
    fork
    forever {
      while( receive_num == 0 ) @(posedge tx_clk) ;
      dly = rcv_dly_c + (rcv_dly_r?(random()%rcv_dly_r):0) ;
      if(dly) @dly(posedge tx_clk) ;
      receive_count++ ;
      receive_num-- ;
    }
    knife
  }
  
  // control the speed of the receive side one by one with random distribution
  void receive_enable_loop_rnd( random_distribution rnd ) {
    int dly ;
    receive_count++ ;
    fork
    forever {
      while( receive_count || receive_num == 0 ) @(posedge tx_clk) ;
      dly = rnd.gen_random() ;
      if(dly) @dly(posedge tx_clk) ;
      receive_count++ ;
      receive_num-- ;
    }
    knife
  }
  
  
  void receive_loop() {
    int i ;
    cell_class cell ;
    
    fork
    forever {
      if( receive_count == 0 ) {
        while( receive_count == 0 ) @(posedge tx_clk) ;
        clav.0 = 1'b1 ;
        @(posedge tx_clk) ;
      }
      clav.0 = 1'b1 ;
      receive_count-- ;
      cell = new ;
      
      for( i = 0 ; i < 53 ; i++ ) {
      
        if( i == 52 && receive_count == 0) {
          clav.0 = 1'b0 ;
        }
        assert( 
          @1,MAX_ENABLE_DELAY (en.0 == 1'b0) 
        ) ; // note that using 
        assert( soc.0 == i?1'b0:1'b1 ) ;
        cell.data[i] = data.0 ;

      }
      printf( "a cell received on %d at %d\n", id, get_cycle() ) ;
      rcv_que.que.send( cell ) ;
    }
    knife
  }
  
}
