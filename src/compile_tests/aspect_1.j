main() {
  zooo zof ;
  
  zof = new() ;
  
  zof.poop() ;
  
  printf( "main end\n" ) ;

}

class zooo {
  int xx ;
  
  void poop() {
    // xx = 0 ;
  }
  
}

class zoo {
  int yy ;
  static int xxx ;
  semaphore foo ;
  
  void moop() {
    zooo soso ;
    
    // soso = this ;
  }
  
  void poom() {
    foo.get() ;
    
  }

}

aspect asp {
  
  int x = 0 ;
  
  initial {
    //x = 100 ;
    //x = x + 5 ;
  }
  void momo( int p ) {
    x = p ;
    // this.x = p ;
  }
  
  
  advice void adv() call( main ) {
    zoo zot ;
     //zot = this ;
    printf( "advice adv called\n" ) ;
  }
  
  advice void moop() 
    call( zoo.moop ) 
    call( zoo.poom ) 
  {
    zoo zot ;
    
    zot = this ;
    this.yy = 0 ;
    printf( "advice moop called\n" ) ;
  }
}

foo() {
  
  asp.x = 0 ;
  
  asp.momo( 5 ) ;

}
