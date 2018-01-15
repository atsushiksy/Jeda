void main ( )
{
 event foo ;
 event bar ;
 int    i;
 int    x[5] ;
 bit   [7:0] bbb ;
  i = 0 ;
  
  foo = new ;
 
  foo.timeout( 5 ) ;
  
  for( i = 0 ; i < 100 ; i++ ) {
  fork 
  {
    printf( "thread 0: wait for sync %d\n", i ) ;
    foo.sync() ;
    printf( "thread 0: got sync\n" ) ;
  }
  {
    printf( "thread 2: wait for sync %d\n", i ) ;
    foo.sync() ;
    printf( "thread 2: got sync\n" ) ;
  }
  {
    printf( "thread 1: sending trigger %d\n", i ) ;
    foo.trigger_pulse() ;
  }
  join
  }
  
  //bar.sync() ;
  
}

