int xxxx ;

main() {

  foo() ;

}

class zooo {
  static int xp ;
  
  void mmm() {
    int x ;
  }
}

aspect asp {
  
  int x = 0 ;
  
  void momo( int p ) {
    x = p ;
     this.x = p ;
     // if( this ) error ( "" ) ;
    printf( "in asp.momo x = %d\n", x ) ;
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

foo() {
  
  asp.x = 0 ;
  
  asp.momo( 5 ) ;

}
