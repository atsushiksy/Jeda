main() {
  zoo z ;
  
  z = new ;
  z.x = 100 ;
  z.y = 55 ;
  
  printf( "z.momo:%d\n", z.momo(5) ) ;

}

class zoo {
  int x ;
  int y ;
  
  void vovo() {
    printf( "zoo.vovo\n" ) ;
  }
  
  int momo( int z ) {
    //vovo() ;
    printf( "z.momo x:%d y:%d z:%d\n", x, y, z ) ;
    return x+y+z ;
  }


}

aspect asp {
  
  int x = 0 ;  
    
  advice int moop( int p ) 
    return( zoo.momo ) 
  {
    zoo z ;
    int tt ;
    z = this ;
    //z.momo(5) ;
    printf( "%d %s advice moop called\n", pointcut_line(), pointcut_file() ) ;
    
    this.x = 66 ;
    this.y = 55 ;
    tt = return + 5 ;
    return x ;
  }
  
  advice void sooop() 
    call( zoo.vovo ) 
  {
    zoo z ;
    
    printf( "%d %s advice sooop called\n", pointcut_line(), pointcut_file() ) ;
    
    this.x = 55 ;
    this.y = 44 ;
  }

  advice void zoko() 
    call( ".".".".printf )
  {
    printf( "%d %s ", pointcut_line(), pointcut_file() ) ;
  
  }
}
