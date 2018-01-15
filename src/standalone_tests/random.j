main() {
  int i ;
  int r0, r1 ;
  random_48  rnd48 ;
  random_mt  rndmt ;
  
  random( 849635 ) ;
  rndmt = new( 849635 ) ;
  rndmt.random() ;
  
  for( i = 0 ; i < 1000 ; i++ ) {
    r0 = random() ;
    r1 = rndmt.random() ;
    printf( "%12d %12d\n", r0, r1 ) ;
  }
  
  rndmt = null ;
  
  printf( "\n\n" ) ;
  
  rand48( 48543 ) ;
  rnd48 = new( 48543 ) ;
  rnd48.random() ;
  
  for( i = 0 ; i < 1000 ; i++ ) {
    r0 = rand48() ;
    r1 = rnd48.random() ;
    printf( "%12d %12d\n", r0, r1 ) ;
  }
  
  printf( "\n\n" ) ;
    
}
