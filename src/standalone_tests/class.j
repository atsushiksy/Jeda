class foo {
  static int id = 0 ;
  int i ;
  int j ;
  bit [7:0] boo ;
  int x[5] ;
  
  void new() {
    i = id++ ;
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

main() {
  foo fi, fj ;
  foo fa[5] ;
  int i ;
  
  fi = new ;
  
  fa[0] = new;
  
  fj = fi ;
  
  i = fi.x[3] ;
  
  fi.j = fa[0].i ;
  
  
  for( i = 0 ; i < 500 ; i++ ) {
    printf( "**** i = %d\n", i ) ;
    fi.bar( 4 ) ;
  
  
    fi.bar( 6 ) ;

    fj.bar(1) ;
  
    fi = new ;
  
    fi.bar(5) ;
  
    fj.bar(1) ;
  }
}
  
