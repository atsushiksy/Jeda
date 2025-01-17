module ff (clk, D, E, R_, Q, Q_) ;
  input clk ;
  input D ;
  input E ;
  input R_ ;
  output Q ;
  output Q_ ;
  reg Q ;
  
  initial begin
    Q = 0 ;
  end
  
  assign Q_ = ~Q ;
  
  always @( R_ ) if( R_ === 0 ) #1 Q = 0 ;
  
  always @(posedge clk) if (R_ !== 0 && E === 1 ) #1 Q = D ;
  
endmodule
