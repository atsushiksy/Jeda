  bit [120:0] aa[8][8][8] ;

class ss {
  bit [120:0] a[8][8][8] ;
  
  void test(int i) {
    bit p ;
    bit [120:0] rr ;
    
    a[3][4][5] = {random(), random(), random(), random(), random()} ;
    printf( "this.a = %b\n", a[3][4][5] ) ;

    rr = a[3][4][5] ;
    p = a[3][4][5][i] ;
    a[3][4][5][i] = ~p ;
    if( a[3][4][5][i] == p ) printf( "Error a[3][4][5][i] != %b\n", p ) ;
    if( a[3][4][5][120:i+1] != rr[120:i+1] )
      printf( "Error a[3][4][5][120:i+1] != %b\n", rr[120:i+1] ) ;
    if( a[3][4][5][i-1:0] != rr[i-1:0] )
      printf( "Error a[3][4][5][0] != %b\n", rr[i-1:0] ) ;
  }
}

main() {
  bit [120:0] a[8][8][8] ;
  ss s ;
  bit p ;
  bit [2:0] pp ;
  bit [120:0] rr ;
  int i, j, k ;
    
  s = new ;
  
  a[3][4][5] = {random(), random(), random(), random(), random()} ;
  aa[3][4][5] = {random(), random(), random(), random(), random()} ;
  s.a[3][4][5] = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", a[3][4][5] ) ;
  printf( "aa = %b\n", aa[3][4][5] ) ;
  printf( "s.a = %b\n", s.a[3][4][5] ) ;
  
  for( i = 1 ; i < 120 ; i++ ) {
    rr = a[3][4][5] ;
    p = a[3][4][5][i] ;
    a[3][4][5][i] = ~p ;
    if( a[3][4][5][i] == p ) printf( "Error a[3][4][5][i] != %b\n", p ) ;
    if( a[3][4][5][120:i+1] != rr[120:i+1] )
      printf( "Error a[3][4][5][120:i+1] != %b\n", rr[120:i+1] ) ;
    if( a[3][4][5][i-1:0] != rr[i-1:0] )
      printf( "Error a[3][4][5][0] != %b\n", rr[i-1:0] ) ;

    rr = aa[3][4][5] ;
    p = aa[3][4][5][i] ;
    aa[3][4][5][i] = ~p ;
    if( aa[3][4][5][i] == p ) printf( "Error aa[3][4][5][i] != %b\n", p ) ;
    if( aa[3][4][5][120:i+1] != rr[120:i+1] )
      printf( "Error aa[3][4][5][120:i+1] != %b\n", rr[120:i+1] ) ;
    if( aa[3][4][5][i-1:0] != rr[i-1:0] )
      printf( "Error aa[3][4][5][i-1:0] != %b\n", rr[i-1:0] ) ;
  

    rr = s.a[3][4][5] ;
    p = s.a[3][4][5][i] ;
    s.a[3][4][5][i] = ~p ;
    if( s.a[3][4][5][i] == p ) printf( "Error s.a[3][4][5][i] != %b\n", p ) ;
    if( s.a[3][4][5][120:i+1] != rr[120:i+1] )
      printf( "Error s.a[3][4][5][120:i+1] != %b\n", rr[120:i+1] ) ;
    if( s.a[3][4][5][i-1:0] != rr[i-1:0] )
      printf( "Error s.a[3][4][5][i-1:0] != %b\n", rr[i-1:0] ) ;


    s.test(i) ;
  
  }
}
