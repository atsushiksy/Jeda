class foo {
  static int a ;
  final protected int b ;
  local int c ;
}

class foo1 extends foo {
  static int aa ;
  int x ;
  foo foos ;
  // int b ;
  // int bar ;
  
}

class foo2 extends foo1 {
  int bb ;
  foo1 foo1s ;
  
   
}

main() {
  foo foox = null ; // = new ;
  foo1 foo1x = new ;
  foo2 foo2x = new ;
  //foox.c++ ;
  //foox.b++ ;
  //foox.a++ ;
  
  foox = foo1x ;
  
  foox = foo2x ;
  
  foo1x = foo2x ;

}
