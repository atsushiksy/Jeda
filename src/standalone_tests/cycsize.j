#ifndef WIDTH
  #define WIDTH  20
#endif

main() {
  bit [WIDTH:0] xx, yy, zz ;
  bit [WIDTH+3:0] i, n, t ;
  int flag ;
  
  n = 1 ;
  n <<= (WIDTH+3) ;
  t = 1 ;
  t <<= WIDTH+1 ;
  t -= 1 ;
  printf( "   %b %b\n", n, t ) ;
  flag = 1 ;
  zz = 0 ;
  printf( "n = %b\n", n ) ;
  for( i = 0 ; i < n ; i++ ) {
    xx = CYC() ;
    //printf( " xx: %b %h\n", xx, xx ) ;
    if( flag ) {
      yy = xx ;
      flag = 0 ;
    }
    else {
      if( xx == yy ) {
        error( "CYC cycle too short %d\n", zz ) ;
      }
    }
    zz++ ;
    if(i && (i & t)==0 ) {
      //printf( "\n" ) ;
      flag = 1 ;
      zz = 0 ;
    }
  }
  
}
