int xxxx ;

main() {
  zooo zof ;  // instance of zooo
  
  printf( "function main()\n" ) ;
  
  foo() ;
  
  zof = new ;
  
  zof.mmm() ;

}

foo() {
  
  asp.x = 0 ;
  
  asp.momo( 5 ) ;

  printf( "function foo()\n" ) ;
}

class zooo {
  static int xp ;
  
  void mmm() {
    int x ;
    
    printf( "zooo.mmm()\n" ) ;
  }
}

aspect asp {
  
  int x = 0 ;
  
  void momo( int p ) {
    x = p ;
    printf( "in asp.momo x = %d\n", x ) ;
  }
  
  advice void prt() 
    call( ".*".printf ) 
    call( ".*".".*".printf ) 
  {
    printf( 
      "prt() %s %d :", 
      pointcut_file(), pointcut_line() 
    ) ;
  }
  
  advice void adv() call( main.foo ) {
    printf( "advice adv called\n" ) ;
  }
  
  advice void moop() 
    call( zoo.moop ) 
    call( zoo.poom ) 
  {
    //this.yy = 0 ;
    printf( "advice moop called\n" ) ;
  }
}

