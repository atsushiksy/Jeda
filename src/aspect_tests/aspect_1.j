main() {
  zooo zoof ;
  zooooo zooof ;
  
  breakpoint ;
  
  zoof = new() ;
  zooof = new() ;
  
  zoof.poop( 66 ) ;
  zooof.poop() ;
  
  
  asp.momo( 5 ) ;
  
  printf( "main end\n" ) ;

}

class zooo {
  int xx ;
  static int p ;
  semaphore sm ;
  
  void driver() {
    sm.get() ;
  }
  
  void poop( int x ) {
    printf( "zooo.poop  %d \n", x ) ;
  }

  void poom( int x, int y ) {
    printf( "zooo.poom  %d %d\n", x, y ) ;
  }

  void moop( int x ) {
    printf( "zooo.poop %d\n", x ) ;
  }
  
}

class zooooo {
  int xx ;
  zooo zozo ;
  
  void ccc() {
    zozo = new() ;
    zozo.sm.get() ;
  }
  
  void poop() {
    printf( "zooooo.poop\n" ) ;
  }
  
}


aspect asp {
  
  int x = 0 ;
  int x0 = 0 ;
  int x1 = 1 ;
  int x2 = 2 ;
  int x3 = 3 ;
  
  initial {
    x = 100 ;
    x = x + 5 ;
  }
  
  void momo( int p ) {
    x = p ;
    printf( "asp.x = %d\n", x ) ;
  }
  
  
  advice void adv( int x ) call( "^zoo"."poop" ) {
    printf( "advice adv called %d\n", x ) ;
  }
  
  advice void moop() 
    call( zooo.moop ) 
    call( zooo.poom ) 
  {
    zooo z = this ;
    printf( "advice moop called\n" ) ;
  }
  
}

foo() {
  
  asp.x = 0 ;
  
  asp.momo( 5 ) ;

}
