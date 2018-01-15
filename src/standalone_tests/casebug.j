int case_miss( int num ) {
  int x ;
  case( num ) {
    1: x = 2 ;
    2: x = 1 ;
  }
  return x ;
}

void main() {
  int x, y ;
  
  fork
  {
  
    do {
      x = case_miss(0) ;
      y++ ;
    } while( y < 1000 ) ;
  }
  join  
  
  printf( "test done\n" ) ;
}

