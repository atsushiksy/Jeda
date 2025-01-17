//////////////////////////////////////////////////////
// verilog testbench for module ff 
//   created by jedatemp
//////////////////////////////////////////////////////
module ff_test ; 
  parameter simulation_cycle = 100 ;
  wire CLOCK ;
  reg CLOCK_reg ;
  assign CLOCK = CLOCK_reg ;
  wire   ff_clk ;
  wire   ff_D ;
  wire   ff_E ;
  wire   ff_R_ ;
  wire   ff_Q ;
  wire   ff_Q_ ;
  assign ff_clk = CLOCK ;
  // Jeda module 
  jeda_module jeda (
    .CLOCK(CLOCK),
    .ff_clk(ff_clk),
    .ff_D(ff_D),
    .ff_E(ff_E),
    .ff_R_(ff_R_),
    .ff_Q(ff_Q),
    .ff_Q_(ff_Q_)
  ) ; // end of jeda_module
  // module ff under test
  ff dut (
    .clk(ff_clk),
    .D(ff_D),
    .E(ff_E),
    .R_(ff_R_),
    .Q(ff_Q),
    .Q_(ff_Q_)
  ) ;

  // CLOCK toggling
  initial begin
    CLOCK_reg = 0 ;
    forever begin
      #(simulation_cycle/2) CLOCK_reg = ~CLOCK_reg ;
    end
  end

endmodule
