int foo( int i = 0, int j = 0, int k = 0 ) {
  return i + j + k ;

}

int bar( int i, int j , int k = 0 ) {
  return i + j + k ;

}

main() {
  int x ;
  
  x = foo() ;
  printf( "foo() = %d\n", x ) ;
  x = foo(1) ;
  printf( "foo(1) = %d\n", x ) ;
  x = foo(1,2);
  printf( "foo(1,2) = %d\n", x ) ;
  x = foo(1,2,3) ;
  printf( "foo(1,2,3) = %d\n", x ) ;
  
  //x = bar() ;
  //x = bar(1) ;
  x = bar(1,2) ;
  printf( "bar(1,2) = %d\n", x ) ;
  x = bar(1,2,3) ;
  printf( "bar(1,2,3) = %d\n", x ) ;
  
}
