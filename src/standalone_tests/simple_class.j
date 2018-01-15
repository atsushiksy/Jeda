class foo {
  static int id = 0 ;
  int i ;
  int j ;
  bit [7:0] boo ;
  
  void new() {
    i = 0 ;
    j = 5 ;
    boo = 0xff ;
  }
  
  void bar( int x ) {
    printf( "i = %d\n", i ) ;
    printf( "j = %d\n", j ) ;
    printf( "boo = %b\n", boo ) ;
    i += x ;
    j += x ;
    boo += x ;
    printf( " i = %d\n", i ) ;
    printf( " j = %d\n", j ) ;
    printf( " boo = %b\n", boo ) ;
  }
  
}

class foooo extends foo {
  int zozo ;
  
  void bar() {
    zozo = 1 ;
    j = 5 ;
    printf( "child bar\n" ) ;
    super.bar(6) ;
    printf( "end of child bar\n" ) ;
    
  }

}

main() {
  foo fi ;
  foo fo ;
  
  foooo ofo ;
  
  fi = new ;
  
  ofo = new ;
  
  fo = new fi ;
  
  fi.bar( 4 ) ;
  
  
  fi.bar( 6 ) ;

  ofo.bar() ;
  

}
  
