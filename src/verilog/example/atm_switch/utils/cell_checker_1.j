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

class cell_checker {
  int id ;
  single_cell_queue rcv_que ;
  cell_queue_array chk_que ;
  semaphore done_sem ;

  new( 
    int a_id,
    single_cell_queue a_rcv_que, 
    cell_queue_array a_chk_que,
    semaphore a_done_sem
  )
  {
    id = a_id ;
    rcv_que = a_rcv_que ;
    chk_que = a_chk_que ;
    done_sem = a_done_sem ;
  }
  
  void check_loop() {
    int i ;
    cell_class cell, ccell ;
    int iport ;
    fork
    forever {
      cell = rcv_que.que.receive() ;
      iport = cell.data[3][2:0] ;
      printf( 
        "checker %d got a cell %h %h %h %h from %d at %d\n",
        id, cell.data[0], cell.data[1], cell.data[2], cell.data[3],
        iport, get_cycle() 
      ) ;
      if( chk_que.num_element( iport ) ) {
        ccell = chk_que.receive( iport ) ;
        for( i = 0 ; i < 53 ; i++ ) {
          if( cell.data[i] != ccell.data[i] ) {
            error(
              "Data[%d] got %h expect %h\n", i, cell.data[i], ccell.data[i]
            ) ;
          }
        }
        done_sem.put() ;
      }
      else {
        error( "No check cell found on checker %d from %d", id, iport ) ;
      }
    }
    knife
  }
  
}
