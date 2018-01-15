main() {
  int i = 0 ;
  bit [66:0] x ;
  
  x = 67'b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111_111 ;
  printf( " x (d) = %d\n", x ) ;
  printf( " x (h) = %h\n", x ) ;
  /**/
  
  //printf( " x << i = %b\n", x << i ) ;
  
  
  for( i = 0 ; i < 10 ; i++ ) {
    printf( " i(d) = %d\n", i ) ;
    printf( " i(h) = %h\n", i ) ;
    printf( " i(o) = %o\n", i ) ;
    printf( " i(b) = %b\n", i ) ;
    printf( " x << i = %b\n", x << i ) ;
  }
  
  
  
  for( x = 0 ; 
  x < 67'b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111_111 ; x = (x << 1) | 1'b1 ) {
    printf( " x = %h\n", x ) ;
    printf( " x = %b\n", x ) ;
    printf( " x = %o\n", x ) ;
    printf( " x = %d\n", x ) ;
    printf( " x << 1 = %h\n", x<<1 ) ;
  }
  
  x = (x << 1) | 1'b1 ;
  printf( " x (d) = %d\n", x ) ;
  printf( " x = %h\n", x ) ;

  x = (x << 1) | 1'b1 ;
  printf( " x (d) = %d\n", x ) ;
  printf( " x = %h\n", x ) ;

  printf( "All done\n" ) ;
}
