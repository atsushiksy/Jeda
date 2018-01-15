main() {
  int i;
  int dt[6] ;

  for( i = 0 ; i < 100000 ; i++ ) {
    randcase {
      1: dt[0]++ ;
      1: dt[1]++ ;
      1: dt[2]++ ;
      1: dt[3]++ ;
      1: dt[4]++ ;
      95: dt[5]++ ;
    }
  
  }
  
  for( i = 0 ; i < 6 ; i++ ) {
    printf( " dt[%d] = %d\n", i, dt[i] ) ;
  }
  
}
