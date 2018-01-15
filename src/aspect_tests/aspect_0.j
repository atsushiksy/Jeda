main() {
  zooo zoof ;
  zooooo zooof ;
  
  breakpoint ;
  
  zoof = new() ;
  zooof = new() ;
  
  zoof.poop() ;
  zooof.poop() ;
  
  
  asp.momo( 5 ) ;
  
  zoof.poom() ;
  
  zoof.moop() ;

  zoof.driver() ;

  zoof.poom() ;
  
  zoof.moop() ;

  
  printf( "main end\n" ) ;

}

class zooo {
  int xx = 99 ;
  semaphore sem ;
  
  void new() {
    sem = new ;
  }
  
  void driver() {
    sem.get() ;
    printf( "zooo.driver got sem\n" ) ;
  }
  
  void poop() {
    printf( "zooo.poop\n" ) ;
  }

  void poom() {
    printf( "zooo.poom %d\n", xx ) ;
  }

  void moop() {
    printf( "zooo.moop %d\n", xx ) ;
  }
  
}

class zooooo {
  int xx ;
  
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
  
  
  advice void adv() call( "^zoo"."poop" ) {
    printf( "advice adv called\n" ) ;
  }
  
  advice void moop() 
    call( zooo.moop ) 
    call( zooo.poom ) 
  {
    this.xx = x++ ;
    printf( "advice moop called\n" ) ;
  }
  
  advice void getsemc() 
    call( zooo.".*".sem.get )
  {
    this.sem.put() ;
    printf( "advice getsemc called\n" ) ;
  }
  
  advice void getsem() 
    return( zooo.".*".sem.get )
  {
    this.xx = x++ ;
    printf( "advice getsem called\n" ) ;
  }
  
}

foo() {
  
  asp.x = 0 ;
  
  asp.momo( 5 ) ;

}
