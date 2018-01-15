void main ( )
{
 bit    [9:0] format;
 int    i;
 int    x[5] ;
 
  i = 0 ;
  
  format = i ;
  
  fork
  {
    printf( "Fork 1\n" ) ;
  }
  {
    printf( "Fork 2\n" ) ;
  }
  knife
  
  printf( "second fork after knife\n" ) ;
  
  fork
  {
    printf( "Fork 3\n" ) ;
  }
  {
    printf( "Fork 4\n" ) ;
  }
  spoon
  
  printf( "third fork after spoon\n" ) ;
  
  fork
  {
    printf( "Fork 5\n" ) ;
  }
  {
    printf( "Fork 6\n" ) ;
  }
  join
 
  i = x[3] ; 
}

