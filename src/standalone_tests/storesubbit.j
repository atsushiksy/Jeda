main() {
  bit [7:0] foo ;
  bit [0:7] bar ;  
  bit [4:7] boo ;  
  bit [4:2] bob ;  
  
  printf( "foo[4:5] = 0\n" ) ;
  //foo[4:5] = 0 ;
  foo[5:4] = 0 ;
  
  printf( "bar[0:7] = 2'b00\n" ) ;
  bar[0:7] = 2'b00 ;
  
  printf( "boo[3:0] = 2'b00\n" ) ;
  //boo[3:0] = 2'b00 ;
  boo[5:7] = 2'b00 ;

  printf( "bob[4:0] = 2'b00\n" ) ;
  //bob[4:0] = 2'b00 ;
  bob[4:3] = 2'b00 ;


}
