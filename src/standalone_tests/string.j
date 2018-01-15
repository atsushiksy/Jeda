main() {

  string foo ;
  string boo ;
  int i, j, k;
  
  foo = sprintf( "boo" ) ;
  
  boo =  sprintf( "length of foo = %0d\n", strlen(foo) ) ;
  
  printf( boo ) ;
  
  foo = substring( boo, 5 ) ;
  
  printf( "%s\n", foo ) ;
  
  foo = substring( boo, 3, 5 ) ;
  
  printf( "%s\n", foo ) ;
  
  printf( "%b\n", abintobit( "10110xz0000zz010101" ) ) ;
  
  printf( "%o\n", aocttobit( "546256xz004zz014501" ) ) ;
  printf( "%h\n", ahextobit( "145af0xzccbezz06987101" ) ) ;
  
  printf( "%d\n", atoi( "495456" ) ) ;
  
  for( i = 0 ; i < 1000 ; i++ ) {
    j = urandom() ;
    foo = sprintf( "%d", j ) ;
    k = atoi( foo ) ;
    if( j != k ) printf( "error: %d != %d\n", j, k ) ;
    else printf( "Pass: %d == %d\n", j, k ) ;
  }
  
}
