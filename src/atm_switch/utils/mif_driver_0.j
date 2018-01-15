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

#define TIME_OUT_LIMIT 1000

// Management interface driver
class mif_driver {
  signal busmode ;
  signal addr ;
  signal sel ;
  signal data ;
  signal rd_ds ;
  signal wr_rw ;
  signal rdy_dtack ;
  signal rst ;
  signal clk ;

  void new(
    signal a_busmode,
    signal a_addr,
    signal a_sel,
    signal a_data,
    signal a_rd_ds,
    signal a_wr_rw,
    signal a_rdy_dtack,
    signal a_rst,
    signal a_clk
  ) {
    busmode = a_busmode ;
    addr = a_addr ;
    sel = a_sel ;
    data = a_data ;
    rd_ds = a_rd_ds ;
    wr_rw = a_wr_rw ;
    rdy_dtack = a_rdy_dtack ;
    rst = a_rst ;
    clk = a_clk ;
    
    // drive signals to inactive state
    busmode.0 = 1'b1 ; // intel compatible mode, we only use this
    addr.0 = 0 ;
    sel.0 = 1'b1 ; // active low
    data.0 = 8'bz ;
    rd_ds.0 = 1'b1 ; // active low
    wr_rw.0 = 1'b1 ; // active low
    rst.0 = 1'b1 ; // active hi, put under reset
    
  }
  
  void drive_reset( int duration ) {
    rst.0 = 1'b1 ;
    @duration(posedge clk) ;
    rst.0 = 1'b0 ;
    printf( "DUT comes out from reset at %d\n", get_cycle() ) ;
  }
  
  bit [7:0] read( bit[11:0] adr, bit [7:0] exp_data = 8'bx ) 
  {
    bit [7:0] ret ;
    @(posedge clk) ;
    addr.0 = adr ;
    sel.0 = 1'b1 ; // active low
    rd_ds.0 = 1'b1 ;
    wr_rw.0 = 1'b1 ;
    @1 sel.0 = 1'b0 ; // active low
    @1 rd_ds.0 = 1'b0 ; // active low
    assert( @1,TIME_OUT_LIMIT( rdy_dtack.0 === 1'b0 ) ) ; // wait rdy
    @(posedge clk) ;
    ret = data.0 ;
    if( !(ret ==? exp_data) ) { // dont care if x on exp_data
      error( 
        "mif.read expect data error, exp %h act %h at %d\n",
        exp_data, ret
      ) ;
    }
    @(posedge clk) ;
    addr.0 = random() ;
    sel.0 = 1'b1 ; // active low
    rd_ds.0 = 1'b1 ;
    wr_rw.0 = 1'b1 ;
    return ret ;
  }
  
  void write( bit[11:0] adr, bit [7:0] dat ) {
    int n ;
    @(posedge clk) ;
    addr.0 = adr ;
    sel.0 = 1'b1 ; // active low
    rd_ds.0 = 1'b1 ;
    wr_rw.0 = 1'b1 ;
    data.0 = dat ;
    @1 sel.0 = 1'b0 ; // active low
    @1 wr_rw.0 = 1'b0 ; // active low
    assert( @1,TIME_OUT_LIMIT( rdy_dtack.0 === 1'b0 ) ) ; // wait rdy
    @(posedge clk) ;
    wr_rw.0 = 1'b1 ;
    @(posedge clk) ;
    addr.0 = random() ;
    sel.0 = 1'b1 ; // active low
    data.0 = 8'bz ;
  }
}
