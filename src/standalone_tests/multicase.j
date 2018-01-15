
main() {
  int i ;
  
  
  for( i = 0 ; i < 10 ; i++ ) {
    case(i) {
      0: printf( "case 0 i = %d\n", i ) ;
      2: printf( "case 2 i = %d\n", i ) ;
      4: printf( "case 4 i = %d\n", i ) ;
      6: printf( "case 6 i = %d\n", i ) ;
      8: printf( "case 8 i = %d\n", i ) ;
      default: printf( "default i = %d\n", i ) ;
    
    }
    case( i ) {
      0, 2, 4: printf( "case 0, 2, 4: i=%d\n", i ) ;
      1, 3, 5: printf( "case 1, 3, 5: i=%d\n", i ) ;
      6, 8: printf( "case 6, 8: i=%d\n", i ) ;
      7, 9: printf( "case 7, 9: i=%d\n", i ) ;
      default: printf( "boo! i = %d\n", i ) ;
    
    }
  
  }

}
