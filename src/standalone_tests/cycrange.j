#ifndef WIDTH
  #define WIDTH  20
#endif


main() {
  bit [WIDTH:0] xx, yy, zz ;
  bit [WIDTH+3:0] i, n, t ;
  int flag ;
  int j ;
  
  n = 40'b00110011 + 40'b01111011 - 40'b01111000 + 2 ;
  printf( "   %d %b\n", n, n ) ;
  flag = 1 ;
  zz = 0 ;
  
  printf( "n = %b\n", n ) ;
  for( j = 0 ; j < 100 ; j++ ) {
    for( i = 0 ; i < n ; i++ ) {
      xx = CYC({40'b0:40'b00110011, 40'b01111000:40'b01111011}) ;
      printf( " xx: %b %h\n", xx, xx ) ;
      if( flag ) {
        yy = xx ;
        flag = 0 ;
      }
      else {
        if( xx == yy ) {
          error( "CYC %h cycle too short %d\n", xx, zz ) ;
        }
      }
      zz++ ;
    }  
    printf( "\n" ) ;
    flag = 1 ;
      zz = 0 ;
  }
  
}
