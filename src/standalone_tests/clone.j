main() {
  clone int x = 4216 ;
  clone bit [7:0] y = 58 ;
  
  fork 
  {
    printf( "th0: x = %d\n", x ) ;
    printf( "th0: y = %d\n", y ) ;
    x = 5421 ;
    y = 54 ;
    printf( "th0: x = %d\n", x ) ;
    printf( "th0: y = %d\n", y ) ;
  }
  {
    printf( "th1: x = %d\n", x ) ;
    printf( "th1: y = %d\n", y ) ;
    x = 1234 ;
    y = 56 ;
    printf( "th1: x = %d\n", x ) ;
    printf( "th1: y = %d\n", y ) ;
  }
  join
  
  printf( "x = %d\n", x ) ;
  printf( "y = %d\n", y ) ;

}
