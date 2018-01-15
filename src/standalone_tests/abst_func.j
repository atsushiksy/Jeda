typedef class foo, foo1, foo2 ;

int x ;

initial {
  x = 0 ;
}

// class foo {
abstract class foo {
  static int a ;
  final protected int b ;
  local int c ;
  
  initial {
    a = 0 ;
  }
  
  void call_bar( int x ) {
    bar( x ) ;
  }
  
  //overwrite void fororo() {
  //   ;
  //}
  
  abstract void bar ( int x ) ;
  /*
    void bar ( int x ) 
   {
    int b ;
    
    a = b ; 
    b = a ;
  }
  */
  
}

class foo1 extends foo {
  static int aa ;
  int x ;
  //foo foos ;
  // int b ;
  // int bar ;
  
  initial {
    int x = 0 ;
    aa = x ;
  }
  
  overwrite void call_bar( int x ) {
    printf( "my_call_bar\n" ) ;
  }
  
  /*
  overwrite int call_barx( int x ) {
    printf( "my_call_bar\n" ) ;
  }
  */
  
  void bar ( int y ) {
    printf( "class foo1.bar( %d )\n", y ) ;
  }
  
}


class foo2 extends foo1 {
  int bb ;
  
  final void bar ( int y ) {
    printf( "class foo2.bar( %d )\n", y ) ;
  }
  
}

class foo3 extends foo {
  void test () {
    call_bar( 0 ) ; /* calling parent member func */
  }
  
  void bar ( int y ) {
    printf( "class foo1.bar( %d )\n", y ) ;
  }
}

func() {
  int y ;
  static int x = 5642 ;
  
  x = 1 ;
}

main() {
  foo foox ; // = new ;
  foo1 foo1x = new ;
  foo2 foo2x = new ;
  
  
  foox = foo1x ;
  
  /*
  foox = foo2x ;
  
  foo1x = foo2x ;
  */
  
  printf( "calling foox.call_bar\n" ) ;
  foox.call_bar( 1 ) ;
  printf( "calling foox.bar\n" ) ;
  foox.bar( 1 ) ;
  printf( "calling foo1x.call_bar\n" ) ;
  foo1x.call_bar( 1 ) ;
  printf( "calling foo1x.bar\n" ) ;
  foo1x.bar( 1 ) ;
  printf( "calling foo2x.call_bar\n" ) ;
  foo2x.call_bar( 1 ) ;
  printf( "calling foo2x.bar\n" ) ;
  foo2x.bar( 1 ) ;

}
