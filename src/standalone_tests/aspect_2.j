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
  
  
  int momo( int z ) {
    printf( "x:%d y:%d z:%d\n", x, y, z ) ;
    return x+y+z ;
  }


}

aspect asp {
  
  int x = 0 ;  
    
  advice int moop( int p ) 
    return( zoo.momo ) 
  {
    zoo z ;
    
    printf( "advice moop called\n" ) ;
    
    this.x = 66 ;
    this.y = 55 ;
    return x ;
  }
}
