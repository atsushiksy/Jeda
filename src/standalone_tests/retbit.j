bit [2:0] bitint( int x ) {
  return x ;
}


bit [33:0] bitint_w( int x ) {
  bit [63:0] xx ;
  
  xx = 64'hffffffff_ffffffff ;
  return x * 8 ;
}

bit [2:0] bitbit( bit [31:0] x ) {
  bit [63:0] xx ;
  xx = 64'hffffffff_ffffffff ;
  return x ;
}


bit [33:0] bitbit_w( bit [31:0] x ) {
  bit [63:0] xx ;
  
  xx = 64'hffffffff_ffffffff ;
  return x * 8 ;
}

bit [31:0] bitstring( string x ) {
  bit [31:0] xx ;
  
  xx = x ;
  return xx ;
}

bit [63:0] bitstring_w( string x ) {
  bit [63:0] xx ;
  
  xx = x ;
  return xx ;
}


main() {
  int i ;
  bit [2:0] y ;
  bit [33:0] yy ;
  bit [31:0] s ;
  bit [63:0] ss ;
  
  for( i = 0 ; i < 8 ; i++ ) {
    y = bitint(i) ;
    printf( "i = %d bitint = %b\n", i, y ) ;
  }

  for( i = 0 ; i < 8 ; i++ ) {
    yy = bitint_w(i) ;
    printf( "i = %d bitint_w = %b\n", i, yy ) ;
  }

  for( i = 0 ; i < 8 ; i++ ) {
    y = bitbit(i) ;
    printf( "i = %d bitbit = %b\n", i, y ) ;
  }

  for( i = 0 ; i < 8 ; i++ ) {
    yy = bitbit_w(i) ;
    printf( "i = %d bitbit_w = %b\n", i, yy ) ;
  }
  
  s = bitstring( "ABCD" ) ;
  
  printf( "s= %h\n", s ) ;

  ss = bitstring_w( "ABCDEFGH" ) ;
  
  printf( "ss= %h\n", ss ) ;
  
}
