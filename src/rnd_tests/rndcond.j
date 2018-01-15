main() {
  bit [7:0] xx ;
  bit [7:0] xxx ;
  
  repeat( 100 ) {
    xx = RND( 
      {8'b00000000:8'b00001111,8'b11110000:8'b11111111}
      , > 8'b00001100, < 8'b11111100
    ) ;
  
    if( xx >= 8'b11111100 || xx <= 8'b00001100 ) 
      error( "xx %b out of constraints\n", xx ) ;
    printf( " xx: %b %h\n", xx, xx ) ;
  }
  
}
