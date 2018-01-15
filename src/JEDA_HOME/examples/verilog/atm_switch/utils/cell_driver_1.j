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

#define MAX_ENABLE_DELAY 1000

// cell driver for UTOPIA Rx port
class cell_driver {
  int id ;
  signal rx_clk ;
  signal data ;
  signal soc ;
  signal en ;
  signal clav ;
  semaphore clav_sem ;
  semaphore drive_sem ;
  
  void new(
    int a_id,
    signal a_rx_clk,
    signal a_data,
    signal a_soc,
    signal a_en,
    signal a_clav
  ) {
    id = a_id ;
    rx_clk = a_rx_clk ;
    data = a_data ;
    soc = a_soc ;
    en = a_en ;
    clav = a_clav ;
    
    // drive signals to inactive state
    clav.0 = 1'b0 ; // active high 
    soc.0 = 1'b0 ;
    data.0 = 0 ;
    
    // using two semaphore to allow back to back drive
    clav_sem = new ;  // mutex for driving clav  
    drive_sem = new ;  // mutex for sending a cell  
  }
  
  void drive_cell( int dly, cell_class cell ) {
    int i, j ;
    clav_sem.get() ;
    if( dly ) @dly(posedge rx_clk) ;
    clav.0 = 1'b1 ;
    @(posedge rx_clk) ;
    drive_sem.get() ;
    for( i = 0 ; i < 53 ; i++ ) {
      soc.0 = i?1'b0:1'b1 ;
      data.0 = cell.data[i] ;
      
      if( i == 52 ) {
        clav.0 = 1'b0 weak ;
        clav_sem.put() ;
      }
      assert( 
        @1,MAX_ENABLE_DELAY (en.1 == 1'b0) // using pipe one signal
      ) ; // note that using 
    }
    drive_sem.put() ;
    printf( "a cell driven on %d at %d\n", id, get_cycle() ) ;
  }
  
}
