main() {
  bit [WIDTH:0] xx ;
  bit [WIDTH+1:0] xxx ;
  
  repeat( 100 ) {
    xx = RND() ;
    xxx = RND() ;
  
    printf( " xx: %b xxx: %b\n", xx, xxx ) ;
  }
  
}
