class cls {
  int aa ;
}

local void foo ( int i, var bit [5:0] x, var cls p ) {
  /*
  x = i ;
  //@1 p.aa <= 0 ;
  p.aa = 4126 ;
  {
    int y = 0 ;
  }
  */
  x = 2314 ;
}

main() {
  int i ;
  bit [5:0] x ;
  static cls p ;
  
  p = new ;
  i = 0 ;
  
  foo( i, x, p ) ;

  printf( "p.aa = %d\n", p.aa ) ;
  printf( "x = %d\n",x ) ;
}
