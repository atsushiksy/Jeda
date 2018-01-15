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
class cell_class {
  bit [7:0] data[53] ;
  int err ;
  static bit [7:0] syndrom[256] ;
  
  initial {
    int i ;
    bit [7:0] sndrm ;
    
    for( i = 0 ; i < 256 ; i++ ) {
      sndrm = i ;
      repeat(8) {
        if( sndrm[7] === 1'b1 )
          sndrm = (sndrm<<1) ^ 8'h07 ;
        else
          sndrm = sndrm << 1 ;
      }
      syndrom[i] = sndrm ;
      // printf( "j syndrom[%d] = %h\n", i, sndrm ) ;
    }
  }
  
  bit [7:0] hec( 
    bit [7:0] hdr1, bit [7:0] hdr2, bit [7:0] hdr3, bit [7:0] hdr4
   ) {
    bit [7:0] ret ;
    ret = 0 ;
    ret = syndrom[ret^hdr1] ;
    ret = syndrom[ret^hdr2] ;
    ret = syndrom[ret^hdr3] ;
    ret = syndrom[ret^hdr4] ;
    ret ^= 8'h55 ;
    return ret ;
  }
  
  void set_hec () {
    data[4] = hec( data[0], data[1], data[2], data[3] ) ;
  }
  
  // create a cell with given gfc and random contents
  //   use pt to indicate the input port for checking
  void rand_cell( bit [7:0] vpi, bit [2:0] pt ) {
    int i  ;
    for( i = 0 ; i < 53 ; i++ ) {
      data[i] = random() ;
    }
    data[0][3:0] = vpi[7:4] ;
    data[1][7:4] = vpi[3:0] ;
    data[3][2:0] = pt ;
    set_hec() ;
  }
  
  void set_hec_err() {
    data[4] = ~data[4] ;
    err = 1 ;
  }
  
}

class single_cell_queue {
  cell_class que<> ;
}

class cell_queue_array {
  single_cell_queue que[] ;
  int num_array ;
  
  void new( int num ) {
    int i ;
    num_array = num ;
    
    for( i = 0 ; i < num ; i++ ) {
      que[i] = new ;
    }
  }
  
  void send( int id, cell_class cell ) {
    if( id > num_array ) 
      error( "cell queue index %d too large, max %d\n", id, num_array ) ;
    else {
      que[id].que.send( cell ) ;
    }
  }
  
  int num_element( int id ) {
    int ret ;
    if( id > num_array ) {
      error( "cell queue index %d too large, max %d\n", id, num_array ) ;
    }
    else {
      ret = que[id].que.num_element() ;
    }
    return ret ;
  }
  
  cell_class receive( int id ) {
    cell_class ret ;
    if( id > num_array ) {
      error( "cell queue index %d too large, max %d\n", id, num_array ) ;
    }
    else {
      ret = que[id].que.receive() ;
    }
    return ret ;
  }
  
}
