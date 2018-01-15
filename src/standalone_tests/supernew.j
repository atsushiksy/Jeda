class foo {
  int x = 0 ;
  int y ;
  
  new() {
    y = 0 ;  
  }
  
}

class foo1 {
  int x = 0 ;
  int y ;
  
  new(int i) {
    y = i ;  
  }
  
}

class bar extends foo {
  int z ;
  
  //new() {
  //  z = 0 ;
  //}
  foobar() {
    printf( "bar.foobar A x = %d, y = %d, z = %d\n", x, y, z ) ;
    
    x = 15 ; y = 32 ; z = 94 ;
    
    printf( "bar.foobar B  x = %d, y = %d, z = %d\n", x, y, z ) ;
  }
}

class bar1 extends foo1 {
  int z ;
  
  new() {
    super.new(0) ;
    z = 0 ;
  }
  
  foobar() {
    printf( "bar1.foobar A x = %d, y = %d, z = %d\n", x, y, z ) ;
    
    x = 5 ; y = 3 ; z = 9 ;
    
    printf( "bar1.foobar B  x = %d, y = %d, z = %d\n", x, y, z ) ;
  }
}


main() {
  int i ;
  bar barbar ;
  bar1 barbar1;
  
  barbar = new ;
  barbar1 = new ;

  for( i = 0 ; i < 10 ; i++ ) {
    barbar.foobar() ;
  
    barbar1.foobar() ;
  }
}
  
