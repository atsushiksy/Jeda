   semaphore safeLockAlloc() {
     semaphore lockValue;
   
     lockValue = new();
   
     printf("lock allocated\n");
     return lockValue;
   }  


main() {
  semaphore lc ;
  
  lc = safeLockAlloc() ;
  
  fork
  {
    lc.get(1) ;
    printf( "p0 got lc\n" ) ;
    lc.put(1) ;
    printf( "p0 put lc\n" ) ;
  }
  {
    lc.get(1) ;
    printf( "p1 got lc\n" ) ;
    lc.put(1) ;
    printf( "p1 put lc\n" ) ;
  }
  join
  
  printf( "semfunc done!\n" ) ;
  
}
