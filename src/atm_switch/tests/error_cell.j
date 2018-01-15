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

int gen_error() {
  int x ;
  
  x = random() ;
  
  // 50% probabirity of error
  if( x & 8 ) return 0 ;
  else return 1 ;
  
}

void errorcell_test1() {
  int i ;
  
  // send 200 sells with some with hec error
  for( i = 0 ; i < 200 ; i++ ) {
    fork
      tester.send_cell( 0, 0, 1, gen_error() ) ;
      tester.send_cell( 0, 1, 2, gen_error() ) ;
      tester.send_cell( 0, 2, 3, gen_error() ) ;
      tester.send_cell( 0, 3, 4, gen_error() ) ;
    join
  }
  
}

void squat_test() {
  printf( "Enter squat_test!\n" ) ;
  
  tester.set_config_reg() ;
  // set slow responce 
  tester.start_receiver_checker( 100, 100 ) ;
  
  errorcell_test1() ;
  
  
  printf( "squat_test done!\n" ) ;

}
