main() {
  file f ;
  random_mt rnd ;
  random_mt rrd ;
  regexp reng ;
  string xx, yy ;
  int i, j, k, n ;
  
  f = new ;
  rnd = new ;
  rrd = new ;
  reng = new ;
  
  f.open( "test.data", "w" ) ;
  j = 0 ;
  for( i =  0 ; i < 1000 ; i++ ) {
    f.fprintf( "%d ", rnd.random() ) ;
    if( ++j == 4 ) {
      f.fprintf( "\n" ) ;
      j = 0 ;
    }
  }
  
  f.close() ;
  
  
  f.open( "test.data", "r" ) ;
  
  xx = f.readline() ;
  printf( "line: %s", xx ) ;
  
  while( reng.match( xx, "\d+") ) {
    printf( "line: %s", xx ) ;
    reng.match( xx, "^(\d+)\s+(\d+)\s+(\d+)\s+(\d+)" ) ;
    for( i = 0 ; i < 4 ; i++ ) {
      yy = reng.backref(i) ;
      printf( "%d %s\n", i, yy ) ;
      j = atoi(yy) ;
      k = rrd.random() ;
      if( j != k ) {
        printf( "Error: n:%d %d != %d\n", n, j, k ) ;
      }
    }
    
    xx = f.readline() ;
  }
  
  
}
