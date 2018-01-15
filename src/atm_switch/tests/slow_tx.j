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
#include "../utils/mif_driver_0.jh" 
#include "../utils/cell_driver_1.jh" 
#include "../utils/cell_receiver_1.jh" 
#include "../utils/cell_checker_1.jh"
#include "../utils/switch_tester_2.jh" 

extern switch_tester  tester ;

void slowtx_test1() {
  int i, j ;
  
  // send 4 sells one by one but each cell will stay more than 100 cycle
  for( i = 0 ; i < 20 ; i++ ) {
    for( j = 0 ; j < 4 ; j++ ) {
      fork
        tester.send_cell( 0, 0, 1 ) ;
        tester.send_cell( 0, 1, 2 ) ;
        tester.send_cell( 0, 2, 3 ) ;
        tester.send_cell( 0, 3, 4 ) ;
      join
    }
    // now wait for the completion, before another try
    tester.wait_done() ;
  }
  
}

void squat_test() {
  printf( "Enter squat_test!\n" ) ;
  
  tester.set_config_reg() ;
  // set slow responce 
  tester.start_receiver_checker( 100, 100 ) ;
  
  slowtx_test1() ;
  
  
  printf( "squat_test done!\n" ) ;

}
