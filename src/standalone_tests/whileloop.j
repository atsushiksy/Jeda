main() {
  int i = 0 ;
  bit [66:0] x ;
  
  x = 67'b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111_111 ;
  printf( " x (d) = %d\n", x ) ;
  printf( " x (h) = %h\n", x ) ;
  
  
  //printf( " x << i = %b\n", x << i ) ;
  
  i = 0 ;
  
  while( i++< 10 ) {
    printf( " i(d) = %d\n", i ) ;
    printf( " i(h) = %h\n", i ) ;
    printf( " i(o) = %o\n", i ) ;
    printf( " i(b) = %b\n", i ) ;
    printf( " x << i = %b\n", x << i ) ;
  }
  
  
  x = 0 ;
  // x = 67'h07ffff_ffff_ffff_fff ;
  
  while(  
  // x < 67'b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111_111    
    x < 67'h0fffff_ffff_ffff_fff
  ) {
    printf( " x = %h\n", x ) ;
    printf( " x = %b\n", x ) ;
    printf( " x = %o\n", x ) ;
    printf( " x = %d\n", x ) ;
    printf( " x << 1 = %h\n", x<<1 ) ;
    x = (x << 1) | 1'b1 ;
    //if( x == 67'h07ffff_ffff_ffff_fff ) {
      //break ;
    //  printf( "next!!\n" ) ;
    //}
  }
    x = (x << 1) | 1'b1 ;
    printf( " x = %h\n", x ) ;
    printf( " x = %b\n", x ) ;
    printf( " x = %o\n", x ) ;
    printf( " x = %d\n", x ) ;
  printf( "All done\n" ) ;
}
