  bit [120:0] aa[8][8][8] ;

class ss {
  bit [120:0] a[8][8][8] ;
  bit [120:0] aa[8][8][8] ;
  
  void set_aa( int i, int j, int k, bit [120:0] rr ) {
    aa[i][j][k] = rr ;
    printf( "set_aa[%d][%d][%d] %d = %d\n", i, j, k, rr, aa[i][j][k] ) ;
  }
  
  bit [120:0] read_aa( int i, int j, int k ) {
    printf( "read_aa[%d][%d][%d] = %d\n", i, j, k, aa[i][j][k] ) ;
    return aa[i][j][k] ;
  }
  
  void test() {
    bit p ;
    a[3][4][5] = {random(), random(), random(), random(), random()} ;
    printf( "this.a = %b\n", a[3][4][5] ) ;

    p = a[3][4][5][1] ;
    a[3][4][5][1] = ~p ;
    if( a[3][4][5][1] == p ) printf( "Error a[3][4][5][1] != %b\n", p ) ;
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
  
  for( i = 0 ; i < 8 ; i++ ) {
  for( j = 0 ; j < 8 ; j++ ) {
  for( k = 0 ; k < 8 ; k++ ) {
    //rr = {random(), random(), random(), random(), random()} ;
    //rr = i * 100 + j * 10 + k ;
    a[i][j][k] = i * 100 + j * 10 + k ;
    aa[i][j][k] = i * 100 + j * 10 + k ;
    s.a[i][j][k] = i * 100 + j * 10 + k ;
    s.set_aa(i,j,k,i * 100 + j * 10 + k) ;
  }
  }
  }

  for( i = 0 ; i < 8 ; i++ ) {
  for( j = 0 ; j < 8 ; j++ ) {
  for( k = 0 ; k < 8 ; k++ ) {
    rr = s.read_aa(i,j,k) ;
    printf( "s.read_aa[%d][%d][%d] = %d\n", i,j,k,rr);
  }
  }
  }  
  
  /*
  for( i = 0 ; i < 8 ; i++ ) {
  for( j = 0 ; j < 8 ; j++ ) {
  for( k = 0 ; k < 8 ; k++ ) {
    rr = i * 100 + j * 10 + k ;
    if( a[i][j][k] != rr ) {
      printf( "rr = %d\n", rr ) ;
      printf( "a = %d\n", a[i][j][k] ) ;
      printf( "Error a[%d][%d][%d] %d != %d\n", i , j, k, a[i][j][k], rr);
    }
    if( a[i][j][k] != i * 100 + j * 10 + k )
      printf( 
        "Error a[%d][%d][%d] %d != %d\n", 
        i, j, k, a[i][j][k], i * 100 + j * 10 + k
      );
  }
  }
  }
  
  
  for( i = 0 ; i < 8 ; i++ ) {
  for( j = 0 ; j < 8 ; j++ ) {
  for( k = 0 ; k < 8 ; k++ ) {
    rr = a[i][j][k] ;
    printf( "a[%d][%d][%d] = %d\n", i,j,k,a[i][j][k]);
    if( aa[i][j][k] != rr ) 
      printf( "Error: aa[%d][%d][%d] %d != %d\n", i,j,k,aa[i][j][k],rr);
    if( s.a[i][j][k] != rr ) 
      printf( "Error: s.a[%d][%d][%d] %d != %d\n", i,j,k,s.a[i][j][k],rr);
    if( s.read_aa(i,j,k) != rr ) 
      printf( "Error: s.read_aa[%d][%d][%d] != %d\n", i,j,k,rr);
  }
  }
  }  
  
  */
  
  /*
  p = a[3][4][5][1] ;
  a[3][4][5][1] = ~p ;
  if( a[3][4][5][1] == p ) printf( "Error a[3][4][5][1] != %b\n", p ) ;

  p = aa[3][4][5][1] ;
  aa[3][4][5][1] = ~p ;
  if( aa[3][4][5][1] == p ) printf( "Error aa[3][4][5][1] != %b\n", p ) ;
  

  p = s.a[3][4][5][1] ;
  s.a[3][4][5][1] = ~p ;
  if( s.a[3][4][5][1] == p ) printf( "Error s.a[3][4][5][1] != %b\n", p ) ;

  s.test() ;
  */
  
}
