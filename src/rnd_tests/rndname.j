main() {
  int i ;
  bit [7:0] xx ;
  bit [7:0] xxx ;
  
  for( i = 0 ; i < 512 ; i++ ) {
    if( i < 256 ) {
      xx = RND( 
        {8'b00000000:8'b00001111,8'b11110000:8'b11111111}
        , > 8'b11110000  , < 8'b11111100, name = "foo"
      ) ;
    }
    else {
      xx = RND( < 8'b00001111, > 8'b00001100, name = "foo" ) ;
    }
  
    if( xx >= 8'b11111100 || xx <= 8'b00001100 ) 
      error( "xx %b out of constraints\n", xx ) ;
    printf( " xx: %b %h\n", xx, xx ) ;
  }
  
}
