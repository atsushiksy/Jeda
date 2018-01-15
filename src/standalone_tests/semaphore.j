void main ( )
{
 semaphore foo ;
 semaphore bar ;
 int    i;
 int    x[5] ;
 bit   [7:0] bbb ;
  i = 0 ;
  
  CLOCK.drive_clock( 5, 5 ) ;
  
  foo = new(1) ;
 
  foo.timeout( 5 ) ;
  
  for( i = 0 ; i < 100 ; i++ ) {
  fork 
  {
    printf( "thread 0: wait for semaphore.get(1)\n" ) ;
    foo.get(1) ;
    printf( "thread 0: got semaphore.get(1)\n" ) ; 
  }
  {
    printf( "thread 2: wait for semaphore.get(1)\n" ) ;
    foo.get(1) ;
    printf( "thread 2: got semaphore.get(1)\n" ) ;
  }
  {
    printf( "thread 1: sending semaphore.put\n" ) ;
    foo.put(1) ;
  }
  join
  }
  
  
}

