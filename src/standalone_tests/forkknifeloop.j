
void forkknife( int n ) {
  event foo ;
  event bar ;
  event boo ;

  printf( "forkknife(%d)\n", n ) ;

  foo = new ;
  bar = new ;
  boo = new ;
  
  fork 
  {
    printf( "thread 0: wait for boo sync %d\n", n ) ;
    boo.sync() ;
    printf( "thread 0: got sync\n" ) ;
  }
  {
    printf( "thread 2: wait for boo sync %d\n", n ) ;
    boo.sync() ;
    printf( "thread 2: got sync\n" ) ;
  }
  {
    printf( "thread 1: wait for bar sync %d\n", n ) ;
    bar.sync() ;
    printf( "thread 1: got sync\n" ) ;
    printf( "thread 1: sending foo trigger %d\n", n ) ;
    foo.trigger() ;
  }
  knife
  
  printf( "forkknife(%d) sending bar.trigger\n", n ) ;
  bar.trigger() ;
  printf( "forkknife(%d): wait for foo sync\n", n ) ;
  foo.sync() ;
  
  printf( "forkknife(%d) sending boo.trigger\n", n ) ;
  boo.trigger() ;

  thread_join() ;
  
  printf( "forkknife(%d) done.\n", n ) ;
}


void forkknife2( int n, event bar ) {
  event foo ;
  event boo ;

  printf( "forkknife(%d)\n", n ) ;

  foo = new ;
  boo = new ;
  
  fork 
  {
    printf( "thread 0: wait for boo sync %d\n", n ) ;
    boo.sync() ;
    printf( "thread 0: got sync\n" ) ;
  }
  {
    printf( "thread 2: wait for boo sync %d\n", n ) ;
    boo.sync() ;
    printf( "thread 2: got sync\n" ) ;
  }
  {
    printf( "thread 1: wait for bar sync %d\n", n ) ;
    bar.sync_clear() ;
    printf( "thread 1: got sync\n" ) ;
    printf( "thread 1: sending foo trigger %d\n", n ) ;
    boo.trigger() ;
  }
  knife
    
  printf( "forkknife(%d) done.\n", n ) ;
}

main() {
  int i ;
  event bar ;
  
  bar = new ;
  
  for( i = 0 ; i < 20 ; i++ ) {
    forkknife(i) ;
  }
  
  for( i = 0 ; i < 20 ; i++ ) {
    forkknife2(i, bar ) ;
    printf( "main calling thread_pause() %d.\n", i ) ;
    thread_pause() ;
    printf( "main sending bar.trigger %d.\n", i ) ;
    bar.trigger() ;
    thread_join() ;
    printf( "main thread joined %d.\n", i ) ;
  }
  printf( "all done.\n" ) ;
}
