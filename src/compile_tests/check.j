local int p ;
main() {
  int x  ; // = RND({5:3@5, 7@5, 9:10@@5}, name = "foo");
  
  x = RND({1@5,2@5,3@5}) ;
  
  randcase {
    10: x = 0 ;
    5 : x = 5 ;
    20: x = 3 ;
  }
  
  x = CYC( name = "foo" ) ;
  x = RND( {0xff5f00ff:0xff5f0000@25, 0xff6f0000:0xff6f0fff@25, 5@5},size=8) ;
  x = CYC( {0xff5f0000:0xff5f00ff, 0xff6f0fff:0xff6f0000, 21}, <50 ) ;
}
