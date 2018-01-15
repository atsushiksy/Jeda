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
  
  abstract void bar ( int x ) ;
  /* {
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
  
  void bar ( int y ) {
    //int b ;
    static int zz ;
    
    a = b ; 
    b = a ;
    aa = b ; 
    b = aa ;
    
    // c++ ;
    
    {
      int x, y ;
      // static int zz ;
      
      x = y ;
    }
    a = b ; 
    b = a ;
  }
  
}

class foo2 extends foo1 {
  int bb ;
  
  void bar () {
    int b ;
    
    a = b ; 
    b = a ;
    bb = b ; 
    b = bb ;
    aa = b ;
    b = aa ;
    {
      int x, y ;
      
      x = y ;
    }
    a = b ; 
    b = a ;
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
  //foox.c++ ;
  //foox.b++ ;
  //foox.a++ ;
  
  foox = foo1x ;
  
  foox = foo2x ;
  
  foo1x = foo2x ;

}
