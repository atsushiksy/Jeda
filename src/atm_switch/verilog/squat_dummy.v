//
// (c) Copyright 2000, Qualis Design Corporation
// All Rights Reserved
//
//    This source file may be used and distributed without restriction
//    provided that this copyright statement is not removed from the file
//    and that any derivative work contains this copyright notice.
//
// Description
//    Functional model of the SQUAT device
//
// Author:      $Author: atsushi $
// Revision:    $Revision: 1.1.1.1 $
//

module squat(
	     //
	     // 4 x Level 1 Utopia ATM layer Rx Interfaces
	     //
             Rx_clk_0,  Rx_clk_1,  Rx_clk_2,  Rx_clk_3,
             Rx_data_0, Rx_data_1, Rx_data_2, Rx_data_3,
             Rx_soc_0,  Rx_soc_1,  Rx_soc_2,  Rx_soc_3, 
             Rx_en_0,   Rx_en_1,   Rx_en_2,   Rx_en_3,  
             Rx_clav_0, Rx_clav_1, Rx_clav_2, Rx_clav_3,
             
	     //
	     // 4 x Level 1 Utopia ATM layer Tx Interfaces
	     //
             Tx_clk_0,   Tx_clk_1,   Tx_clk_2,   Tx_clk_3,
             Tx_data_0,  Tx_data_1,  Tx_data_2,  Tx_data_3,
             Tx_soc_0,   Tx_soc_1,   Tx_soc_2,   Tx_soc_3, 
             Tx_en_0,    Tx_en_1,    Tx_en_2,    Tx_en_3,  
             Tx_clav_0,  Tx_clav_1,  Tx_clav_2,  Tx_clav_3,
             
	     //
	     // Utopia Level 2 parallel management interface
	     //
	     BusMode, Addr, Sel, Data, Rd_DS, Wr_RW, Rdy_Dtack,
             
	     //
	     // Miscellaneous control interfaces
	     //
	     rst, clk);

//
// 4 x Level 1 Utopia Rx Interfaces
//
   output       Rx_clk_0,  Rx_clk_1,  Rx_clk_2,  Rx_clk_3;
   input [7:0]  Rx_data_0, Rx_data_1, Rx_data_2, Rx_data_3;
   input        Rx_soc_0,  Rx_soc_1,  Rx_soc_2,  Rx_soc_3; 
   output       Rx_en_0,   Rx_en_1,   Rx_en_2,   Rx_en_3;  
   input        Rx_clav_0, Rx_clav_1, Rx_clav_2, Rx_clav_3;

//
// 4 x Level 1 Utopia Tx Interfaces
//
   output       Tx_clk_0,   Tx_clk_1,   Tx_clk_2,   Tx_clk_3;
   output [7:0] Tx_data_0,  Tx_data_1,  Tx_data_2,  Tx_data_3;
   output       Tx_soc_0,   Tx_soc_1,   Tx_soc_2,   Tx_soc_3; 
   output       Tx_en_0,    Tx_en_1,    Tx_en_2,    Tx_en_3;  
   input        Tx_clav_0,  Tx_clav_1,  Tx_clav_2,  Tx_clav_3;

//
// Intel-style Utopia parallel management interface
//
   input        BusMode;
   input [11:0] Addr;
   input        Sel;
   inout [ 7:0] Data;
   input        Rd_DS;
   input        Wr_RW;
   output       Rdy_Dtack;
   
//
// Miscellaneous control interfaces
//
   input        rst;
   input        clk;


endmodule
