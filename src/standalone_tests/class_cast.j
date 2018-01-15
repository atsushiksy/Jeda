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


test( foo booo ) {
  printf( "test\n" ) ;
}
test1( foo1 booo ) {
  printf( "test1\n" ) ;
}
test3( foo3 booo ) {
  printf( "test3\n" ) ;
}

main() {
  foo foox ; // = new ;
  foo1 foo1x = new ;
  foo2 foo2x = new ;
  foo3 foo3x = new ;
  foo3 fo3aray[5] ;
  foo3 fo3asoc[] ;
  foo3 fo3str[*] ;
  foo3 fo3lst<> ;
  foo3 fo3mary[2][2][2] ;
  
  
  foox = foo1x ;
  
  //foo1x = foox ;
  
  // foo2x = foox ;
  
  //foo3x = foox ;
  
  //fo3aray[2] = foox ;
  //fo3asoc[2] = foox ;
  //fo3str["foo"] = foox ;
  
  fo3lst.push( foo3x ) ;
  
  foo3x = fo3lst.pop() ;
  
  test( foo1x ) ;
  test1( foox ) ;
  //test3( foox ) ;
  //fo3mary[1][1][1] = foox ;
  
  /*
  foox = foo2x ;
  
  foo1x = foo2x ;
  */
  /*
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
  */
}
