//////////////////////////////////////////////////////
// verilog testbench for module squat 
//   created by jedatemp
//////////////////////////////////////////////////////
module squat_test ; 
  parameter simulation_cycle = 100 ;
  wire CLOCK ;
  reg CLOCK_reg ;
  assign CLOCK = CLOCK_reg ;
  wire   squat_Rx_clk_0 ;
  wire   squat_Rx_clk_1 ;
  wire   squat_Rx_clk_2 ;
  wire   squat_Rx_clk_3 ;
  wire  [7:0]  squat_Rx_data_0 ;
  wire  [7:0]  squat_Rx_data_1 ;
  wire  [7:0]  squat_Rx_data_2 ;
  wire  [7:0]  squat_Rx_data_3 ;
  wire   squat_Rx_soc_0 ;
  wire   squat_Rx_soc_1 ;
  wire   squat_Rx_soc_2 ;
  wire   squat_Rx_soc_3 ;
  wire   squat_Rx_en_0 ;
  wire   squat_Rx_en_1 ;
  wire   squat_Rx_en_2 ;
  wire   squat_Rx_en_3 ;
  wire   squat_Rx_clav_0 ;
  wire   squat_Rx_clav_1 ;
  wire   squat_Rx_clav_2 ;
  wire   squat_Rx_clav_3 ;
  wire   squat_Tx_clk_0 ;
  wire   squat_Tx_clk_1 ;
  wire   squat_Tx_clk_2 ;
  wire   squat_Tx_clk_3 ;
  wire  [7:0]  squat_Tx_data_0 ;
  wire  [7:0]  squat_Tx_data_1 ;
  wire  [7:0]  squat_Tx_data_2 ;
  wire  [7:0]  squat_Tx_data_3 ;
  wire   squat_Tx_soc_0 ;
  wire   squat_Tx_soc_1 ;
  wire   squat_Tx_soc_2 ;
  wire   squat_Tx_soc_3 ;
  wire   squat_Tx_en_0 ;
  wire   squat_Tx_en_1 ;
  wire   squat_Tx_en_2 ;
  wire   squat_Tx_en_3 ;
  wire   squat_Tx_clav_0 ;
  wire   squat_Tx_clav_1 ;
  wire   squat_Tx_clav_2 ;
  wire   squat_Tx_clav_3 ;
  wire   squat_BusMode ;
  wire  [11:0]  squat_Addr ;
  wire   squat_Sel ;
  wire  [7:0]  squat_Data ;
  wire   squat_Rd_DS ;
  wire   squat_Wr_RW ;
  wire   squat_Rdy_Dtack ;
  wire   squat_rst ;
  wire   squat_clk ;
  assign squat_clk = CLOCK ;
  // Jeda module 
  jeda_module jeda (
    .CLOCK(CLOCK),
    .squat_Rx_clk_0(squat_Rx_clk_0),
    .squat_Rx_clk_1(squat_Rx_clk_1),
    .squat_Rx_clk_2(squat_Rx_clk_2),
    .squat_Rx_clk_3(squat_Rx_clk_3),
    .squat_Rx_data_0(squat_Rx_data_0),
    .squat_Rx_data_1(squat_Rx_data_1),
    .squat_Rx_data_2(squat_Rx_data_2),
    .squat_Rx_data_3(squat_Rx_data_3),
    .squat_Rx_soc_0(squat_Rx_soc_0),
    .squat_Rx_soc_1(squat_Rx_soc_1),
    .squat_Rx_soc_2(squat_Rx_soc_2),
    .squat_Rx_soc_3(squat_Rx_soc_3),
    .squat_Rx_en_0(squat_Rx_en_0),
    .squat_Rx_en_1(squat_Rx_en_1),
    .squat_Rx_en_2(squat_Rx_en_2),
    .squat_Rx_en_3(squat_Rx_en_3),
    .squat_Rx_clav_0(squat_Rx_clav_0),
    .squat_Rx_clav_1(squat_Rx_clav_1),
    .squat_Rx_clav_2(squat_Rx_clav_2),
    .squat_Rx_clav_3(squat_Rx_clav_3),
    .squat_Tx_clk_0(squat_Tx_clk_0),
    .squat_Tx_clk_1(squat_Tx_clk_1),
    .squat_Tx_clk_2(squat_Tx_clk_2),
    .squat_Tx_clk_3(squat_Tx_clk_3),
    .squat_Tx_data_0(squat_Tx_data_0),
    .squat_Tx_data_1(squat_Tx_data_1),
    .squat_Tx_data_2(squat_Tx_data_2),
    .squat_Tx_data_3(squat_Tx_data_3),
    .squat_Tx_soc_0(squat_Tx_soc_0),
    .squat_Tx_soc_1(squat_Tx_soc_1),
    .squat_Tx_soc_2(squat_Tx_soc_2),
    .squat_Tx_soc_3(squat_Tx_soc_3),
    .squat_Tx_en_0(squat_Tx_en_0),
    .squat_Tx_en_1(squat_Tx_en_1),
    .squat_Tx_en_2(squat_Tx_en_2),
    .squat_Tx_en_3(squat_Tx_en_3),
    .squat_Tx_clav_0(squat_Tx_clav_0),
    .squat_Tx_clav_1(squat_Tx_clav_1),
    .squat_Tx_clav_2(squat_Tx_clav_2),
    .squat_Tx_clav_3(squat_Tx_clav_3),
    .squat_BusMode(squat_BusMode),
    .squat_Addr(squat_Addr),
    .squat_Sel(squat_Sel),
    .squat_Data(squat_Data),
    .squat_Rd_DS(squat_Rd_DS),
    .squat_Wr_RW(squat_Wr_RW),
    .squat_Rdy_Dtack(squat_Rdy_Dtack),
    .squat_rst(squat_rst),
    .squat_clk(squat_clk)
  ) ; // end of jeda_module
  // module squat under test
  squat dut (
    .Rx_clk_0(squat_Rx_clk_0),
    .Rx_clk_1(squat_Rx_clk_1),
    .Rx_clk_2(squat_Rx_clk_2),
    .Rx_clk_3(squat_Rx_clk_3),
    .Rx_data_0(squat_Rx_data_0),
    .Rx_data_1(squat_Rx_data_1),
    .Rx_data_2(squat_Rx_data_2),
    .Rx_data_3(squat_Rx_data_3),
    .Rx_soc_0(squat_Rx_soc_0),
    .Rx_soc_1(squat_Rx_soc_1),
    .Rx_soc_2(squat_Rx_soc_2),
    .Rx_soc_3(squat_Rx_soc_3),
    .Rx_en_0(squat_Rx_en_0),
    .Rx_en_1(squat_Rx_en_1),
    .Rx_en_2(squat_Rx_en_2),
    .Rx_en_3(squat_Rx_en_3),
    .Rx_clav_0(squat_Rx_clav_0),
    .Rx_clav_1(squat_Rx_clav_1),
    .Rx_clav_2(squat_Rx_clav_2),
    .Rx_clav_3(squat_Rx_clav_3),
    .Tx_clk_0(squat_Tx_clk_0),
    .Tx_clk_1(squat_Tx_clk_1),
    .Tx_clk_2(squat_Tx_clk_2),
    .Tx_clk_3(squat_Tx_clk_3),
    .Tx_data_0(squat_Tx_data_0),
    .Tx_data_1(squat_Tx_data_1),
    .Tx_data_2(squat_Tx_data_2),
    .Tx_data_3(squat_Tx_data_3),
    .Tx_soc_0(squat_Tx_soc_0),
    .Tx_soc_1(squat_Tx_soc_1),
    .Tx_soc_2(squat_Tx_soc_2),
    .Tx_soc_3(squat_Tx_soc_3),
    .Tx_en_0(squat_Tx_en_0),
    .Tx_en_1(squat_Tx_en_1),
    .Tx_en_2(squat_Tx_en_2),
    .Tx_en_3(squat_Tx_en_3),
    .Tx_clav_0(squat_Tx_clav_0),
    .Tx_clav_1(squat_Tx_clav_1),
    .Tx_clav_2(squat_Tx_clav_2),
    .Tx_clav_3(squat_Tx_clav_3),
    .BusMode(squat_BusMode),
    .Addr(squat_Addr),
    .Sel(squat_Sel),
    .Data(squat_Data),
    .Rd_DS(squat_Rd_DS),
    .Wr_RW(squat_Wr_RW),
    .Rdy_Dtack(squat_Rdy_Dtack),
    .rst(squat_rst),
    .clk(squat_clk)
  ) ;

  // CLOCK toggling
  initial begin
    CLOCK_reg = 0 ;
    forever begin
      #(simulation_cycle/2) CLOCK_reg = ~CLOCK_reg ;
    end
  end

`include "../verilog/squat_test_inc.vh"

endmodule
