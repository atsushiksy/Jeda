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
#include "../include/squat_port.jh" 

class switch_tester {
  mif_driver mif ;
  cell_driver drv[4] ;
  cell_receiver rcv[4] ;
  cell_checker chkr[4] ;
  cell_queue_array chk_que[4] ;
  random_distribution drv_rand[4] ;
  random_distribution rcv_rand[4] ;
  // keep the config reg value
  bit [15:0] config_reg [256] ;
  int cell_count[4] ;
  int total_count ;
  semaphore done_sem ;
  
  void new()
  {
    int i ;
    // instanciate drivers
    mif = 
      new(
        squat.BusMode,
        squat.Addr,
        squat.Sel,
        squat.Data,
        squat.Rd_DS,
        squat.Wr_RW,
        squat.Rdy_Dtack,
        squat.rst,
        squat.clk
      ) ;
      
    drv[0] =
      new(
        0, // id
        squat.Rx_clk_0,
        squat.Rx_data_0,
        squat.Rx_soc_0,
        squat.Rx_en_0,
        squat.Rx_clav_0
      ) ;
    drv[1] =
      new(
        1, // id
        squat.Rx_clk_1,
        squat.Rx_data_1,
        squat.Rx_soc_1,
        squat.Rx_en_1,
        squat.Rx_clav_1
      ) ;
    drv[2] =
      new(
        2, // id
        squat.Rx_clk_2,
        squat.Rx_data_2,
        squat.Rx_soc_2,
        squat.Rx_en_2,
        squat.Rx_clav_2
      ) ;
    drv[3] =
      new(
        3, // id
        squat.Rx_clk_3,
        squat.Rx_data_3,
        squat.Rx_soc_3,
        squat.Rx_en_3,
        squat.Rx_clav_3
      ) ;
    rcv[0] =
      new(
        0, // id
        squat.Tx_clk_0,
        squat.Tx_data_0,
        squat.Tx_soc_0,
        squat.Tx_en_0,
        squat.Tx_clav_0
      ) ;
    rcv[1] =
      new(
        1, // id
        squat.Tx_clk_1,
        squat.Tx_data_1,
        squat.Tx_soc_1,
        squat.Tx_en_1,
        squat.Tx_clav_1
      ) ;
    rcv[2] =
      new(
        2, // id
        squat.Tx_clk_2,
        squat.Tx_data_2,
        squat.Tx_soc_2,
        squat.Tx_en_2,
        squat.Tx_clav_2
      ) ;
    rcv[3] =
      new(
        3, // id
        squat.Tx_clk_3,
        squat.Tx_data_3,
        squat.Tx_soc_3,
        squat.Tx_en_3,
        squat.Tx_clav_3
      ) ;

    total_count = 0  ;
    done_sem = new( 0 ) ; // initial zero, shared by all the checker
    done_sem.timeout( 5000 ) ; // 5000 cycle to barf error
    for( i = 0 ; i < 4 ; i++ ) {
      chk_que[i] = new( 4 ) ;
      chkr[i] = new( i, rcv[i].rcv_que, chk_que[i], done_sem ) ;
      cell_count[i] = 0 ;
    }   
    
  }
    
  void read_config_reg() {
    int i ;
    bit [15:0] pattern ;
    bit [11:0] addr ;
    for( i = 0 ; i < 256 ; i++ ) {
      addr = 0 ;    
      addr[8:1] = i ;
      pattern[7:0] = mif.read( addr ) ;
      addr[0] = 1'b1 ;
      pattern[15:8] = mif.read( addr ) ;
      if( config_reg[i] != pattern ) {
        error( 
          "config reg read error [%d] read %h expect %h\n", 
          i, pattern, config_reg[i]
        ) ;
      }
    }
  }
  
  void check_config_reg() {
    int i ;
    bit [15:0] pattern ;
    bit [11:0] addr ;
    // marching 1 test
    pattern = 16'h0001 ;
    for( i = 0 ; i < 256 ; i++ ) {
      addr = 0 ;    
      addr[8:1] = i ;
      mif.write( addr, pattern[7:0] ) ;
      addr[0] = 1'b1 ;
      mif.write( addr, pattern[15:8] ) ;
      config_reg[i] = pattern ;
      pattern = {pattern[14:0],pattern[15]} ; // rotate
    }
    read_config_reg() ; // check contents with config_reg
    
    // marching 0 test
    pattern = 16'hfffe ;
    for( i = 0 ; i < 256 ; i++ ) {
      addr = 0 ;    
      addr[8:1] = i ;
      mif.write( addr, pattern[7:0] ) ;
      addr[0] = 1'b1 ;
      mif.write( addr, pattern[15:8] ) ;
      config_reg[i] = pattern ;
      pattern = {pattern[14:0],pattern[15]} ; // rotate
    }
    read_config_reg() ; // check contents with config_reg
    
    // random test
    for( i = 0 ; i < 256 ; i++ ) {
      addr = 0 ;    
      pattern = random() ;
      addr[8:1] = i ;
      mif.write( addr, pattern[7:0] ) ;
      addr[0] = 1'b1 ;
      mif.write( addr, pattern[15:8] ) ;
      config_reg[i] = pattern ;
      
    }
    read_config_reg() ; // check contents with config_reg
  }
  
  
  start_receiver_checker( int rcv_dly_c, int rcv_dly_r ) {
    int i ;
    
    for( i = 0 ; i < 4 ; i++ ) {
      rcv[i].receive_loop() ;
      rcv[i].receive_enable_loop( rcv_dly_c, rcv_dly_r ) ;
      chkr[i].check_loop() ;
    }
  }

  start_receiver_checker_rnd() {
    int i ;
    
    for( i = 0 ; i < 4 ; i++ ) {
      rcv[i].receive_loop() ;
      rcv[i].receive_enable_loop_rnd( rcv_rand[i] ) ;
      chkr[i].check_loop() ;
    }
  }
  
  // setup config reg with default value
  void set_config_reg() {
    int i ;
    bit [15:0] pattern ;
    bit [11:0] addr ;
    // marching 1 test
    pattern = 16'h0001 ;
    for( i = 0 ; i < 256 ; i++ ) {
      case( i ) {
        0: pattern[15:12] = 4'b0000 ;
        1: pattern[15:12] = 4'b0001 ;
        2: pattern[15:12] = 4'b0010 ;
        3: pattern[15:12] = 4'b0100 ;
        4: pattern[15:12] = 4'b1000 ;
        5: pattern[15:12] = 4'b0011 ;
        6: pattern[15:12] = 4'b0110 ;
        7: pattern[15:12] = 4'b1100 ;
        8: pattern[15:12] = 4'b0101 ;
        9: pattern[15:12] = 4'b1010 ;
        10: pattern[15:12] = 4'b1001 ;
        11: pattern[15:12] = 4'b0111 ;
        12: pattern[15:12] = 4'b1011 ;
        13: pattern[15:12] = 4'b1101 ;
        14: pattern[15:12] = 4'b1110 ;
        15: pattern[15:12] = 4'b1111 ;
        default: pattern[15:12] = random() ;
      }
      pattern[11:0] = random() ;
      addr = 0 ;    
      addr[8:1] = i ;
      mif.write( addr, pattern[7:0] ) ;
      addr[0] = 1'b1 ;
      mif.write( addr, pattern[15:8] ) ;
      config_reg[i] = pattern ;
      
      // printf( "reg[%d]: %b %b\n", i, pattern[15:12], pattern[11:0] ) ;
    }
  }
  
  void set_rand_config() {
    int i, p, q, r, s ;
    uniform_distribution ur ;
    constant_distribution cr ;
    constant_loop_distribution clr ;
    geometric_distribution gr ;
    expornential_distribution er ;
    pdf_distribution pr[4] ;
    string conf ;
    regexp reg ;
    
    reg = new ; // regular expression engine
    
    // read plus argment and define the random distribution for drive/receive
    
    // drive random config
    conf = get_plusarg( "drive_uniform=" ) ;
    if( conf != null ) {
      reg.match( conf, "(\d+),(\d+)" ) ;
      p = atoi( reg.backref(0) ) ;
      q = atoi( reg.backref(1) ) ;
      printf( "drive random distribusion = uniform (min:%d, max:%d)\n", p, q ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        ur = new( p, q ) ;
        drv_rand[i] = ur ;
      }
    }
    conf = get_plusarg( "drive_const=" ) ;
    if( conf != null ) {
      p = atoi(conf) ;
      printf( "drive random distribusion = const (%d)\n", p ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        cr = new( p ) ;
        drv_rand[i] = cr ;
      }
    }
    conf = get_plusarg( "drive_const_loop=" ) ;
    if( conf != null ) {
      int loop_num[], value[] ;
      int num = 0 ;
      
      printf( "drive random distribusion = const loop( \n" ) ;
      while( reg.match( conf, "(\d+),(\d+)" ) ) {
        p = atoi( reg.backref(0) ) ;
        q = atoi( reg.backref(1) ) ;
        loop_num[num] = p ;
        value[num++] = q ;
        conf = reg.postmatch() ;
        printf( "%d:%d ", p, q ) ;
      }
      printf( ")\n" ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        clr = new( loop_num, value, num ) ;
        drv_rand[i] = clr ;
      }
    }
    conf = get_plusarg( "drive_geometric=" ) ;
    if( conf != null ) {
      reg.match( conf, "(\d+),(\d+)" ) ;
      p = atoi( reg.backref(0) ) ;
      q = atoi( reg.backref(1) ) ;
      printf( 
        "drive random distribusion = geometric (prov =%d/%d)\n", p, q 
      ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        gr = new( p, q ) ;
        drv_rand[i] = gr ;
      }
    }
    conf = get_plusarg( "drive_expornential=" ) ;
    if( conf != null ) {
      p = atoi(conf) ;
      printf( "drive random distribusion = expornential (mean=%d)\n", p ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        er = new( p ) ;
        drv_rand[i] = er ;
      }
    }
    conf = get_plusarg( "drive_pdf=" ) ;
    if( conf != null ) {
      
      printf( "drive random distribusion = pdf( \n" ) ;
      pr[0] = new ;
      pr[1] = new ;
      pr[2] = new ;
      pr[3] = new ;
      while( reg.match( conf, "(\d+),(\d+)" ) ) {
        p = atoi( reg.backref(0) ) ;
        q = atoi( reg.backref(1) ) ;
        pr[0].set( p , q ) ;
        pr[1].set( p , q ) ;
        pr[2].set( p , q ) ;
        pr[3].set( p , q ) ;
        conf = reg.postmatch() ;
        printf( "%d:%d ", p, q ) ;
      }
      printf( ")\n" ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        drv_rand[i] = pr[i] ;
      }
    }
    
    if( drv_rand[0] == null ) {
      printf( "drive random distribusion = const (%d)\n", 1 ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        cr = new( 1 ) ;
        drv_rand[i] = cr ;
      }
    }

    // receive random config
    conf = get_plusarg( "receive_uniform=" ) ;
    if( conf != null ) {
      reg.match( conf, "(\d+),(\d+)" ) ;
      p = atoi( reg.backref(0) ) ;
      q = atoi( reg.backref(1) ) ;
      printf( 
        "receive random distribusion = uniform (min:%d, max:%d)\n", p, q 
      ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        ur = new( p, q ) ;
        rcv_rand[i] = ur ;
      }
    }
    conf = get_plusarg( "receive_const=" ) ;
    if( conf != null ) {
      p = atoi(conf) ;
      printf( "receive random distribusion = const (%d)\n", p ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        cr = new( p ) ;
        rcv_rand[i] = cr ;
      }
    }
    conf = get_plusarg( "receive_const_loop=" ) ;
    if( conf != null ) {
      int loop_num[], value[] ;
      int num = 0 ;
      
      printf( "receive random distribusion = const loop( \n" ) ;
      while( reg.match( conf, "(\d+),(\d+)" ) ) {
        p = atoi( reg.backref(0) ) ;
        q = atoi( reg.backref(1) ) ;
        loop_num[num] = p ;
        value[num++] = q ;
        conf = reg.postmatch() ;
        printf( "%d:%d ", p, q ) ;
      }
      printf( ")\n" ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        clr = new( loop_num, value, num ) ;
        rcv_rand[i] = clr ;
      }
    }
    conf = get_plusarg( "receive_geometric=" ) ;
    if( conf != null ) {
      reg.match( conf, "(\d+),(\d+)" ) ;
      p = atoi( reg.backref(0) ) ;
      q = atoi( reg.backref(1) ) ;
      printf( 
        "receive random distribusion = geometric (prov =%d/%d)\n", p, q 
      ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        gr = new( p, q ) ;
        rcv_rand[i] = gr ;
      }
    }
    conf = get_plusarg( "receive_expornential=" ) ;
    if( conf != null ) {
      p = atoi(conf) ;
      printf( "receive random distribusion = expornential (mean=%d)\n", p ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        er = new( p ) ;
        rcv_rand[i] = er ;
      }
    }
    conf = get_plusarg( "receive_pdf=" ) ;
    if( conf != null ) {
      printf( "receive random distribusion = pdf( \n", p ) ;
      pr[0] = new ;
      pr[1] = new ;
      pr[2] = new ;
      pr[3] = new ;
      while( reg.match( conf, "(\d+),(\d+)" ) ) {
        p = atoi( reg.backref(0) ) ;
        q = atoi( reg.backref(1) ) ;
        pr[0].set( p , q ) ;
        pr[1].set( p , q ) ;
        pr[2].set( p , q ) ;
        pr[3].set( p , q ) ;
        conf = reg.postmatch() ;
        printf( "%d:%d ", p, q ) ;
      }
      printf( ")\n" ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        rcv_rand[i] = pr[i] ;
      }
    }
    if( rcv_rand[0] == null ) {
      printf( "receive random distribusion = const (%d)\n", 1 ) ;
      for( i = 0 ; i < 4 ; i++ ) {
        cr = new( 1 ) ;
        rcv_rand[i] = cr ;
      }
    }
  }
  
  send_cell( int dly, int inport, int vpi, int err = 0 ) {
    cell_class cell ;
    bit [15:0] pattern ;
    bit [3:0] oport ;
    int i ;
    
    if( inport > 3 ) error( "inport %d too large\n", inport ) ;
    if( vpi > 255 ) error( "vpi %d too large\n", vpi ) ;
    
    cell = new ;
    cell.rand_cell( vpi, inport ) ;
    if( err ) cell.set_hec_err() ;
    drv[inport].drive_cell( dly, cell ) ;
    printf( 
      "cell %h %h %h %h %h sent from %d %d\n",
      cell.data[0], cell.data[1], cell.data[2], cell.data[3], cell.data[4],
      inport, get_cycle() 
    ) ;
    // send the expected cell to the checker
    pattern = config_reg[vpi] ;
    oport = pattern[15:12] ;
    cell.data[0] = pattern[12:4] ;
    cell.data[1][7:4] = pattern[3:0] ;
    cell.set_hec() ;
    printf( 
      "cell %h %h %h %h sent to checker %b\n",
      cell.data[0], cell.data[1], cell.data[2], cell.data[3], cell.data[4],
      oport
    ) ;
    if( !err ) {
      for( i = 0 ; i < 4 ; i++ ) {
        if( oport[i] ) {
          chk_que[i].send( inport, cell ) ;
          rcv[i].receive_num++ ;
          total_count++ ;
        }
      }
    }
  
  }
  
  send_rnd_cells( int inport, int num ) {
    int dly, vpi, err ;
    repeat( num ) {
      dly = drv_rand[inport].gen_random() ;
      vpi = random() % 256 ;
      err = (random() % 100 ) > 20?1:0 ;
      
      // breakpoint ;
      
      send_cell( dly, inport, vpi, err ) ;
      
    }
  }
  
  wait_done() {
    if( total_count ) {
      done_sem.get( total_count ) ;
      total_count = 0 ;
    }
  }
  
}
