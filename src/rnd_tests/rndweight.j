main() {
  bit [7:0] xx ;
  bit [7:0] xxx ;
  
  repeat( 100 ) {
    xx = RND( {8'b00000000:8'b00001111@5,8'b11110000:8'b11111111@95}) ;
  
    printf( " xx: %b %h\n", xx, xx ) ;
  }
  
}
