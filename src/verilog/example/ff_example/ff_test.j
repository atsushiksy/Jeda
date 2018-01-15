//////////////////////////////////////////////////////
// Jeda template for module ff 
//   created by jedatemp
//////////////////////////////////////////////////////
#include "ff_port.jh"

main() {
  // begining of the program for module ff
  int p ;

  printf( " ff test started!\n" ) ;

  //ff.clk.drive_clock( 50, 50, 0, 0 ) ;  // driving clk with #50, #50 duty

  p = 1 ;
  ff.R_.0 = 0 ;
  ff.D.0 = 0 ;
  @1(posedge ff.clk) ;
  ff.R_.0 = 1 ;
  ff.E.0 = 1 ;
  repeat( 5 ) {
    @(posedge ff.clk) ;
    printf( 
      "posedge of ff.clk p = %d ff.Q=%b FF.Q_=%b\n", p, ff.Q.0, ff.Q_.0 
    ) ;
    printf( "cycle %d time %d\n", get_cycle(), get_time() ) ;
    ff.D.0 = p ;
    p ^= 1 ;
  }
}

