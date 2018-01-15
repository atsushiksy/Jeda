void foo( bit [8*10:0] sss ) {
  printf( "foo: %h\n", sss ) ;
}

main() {
  string s ;
  bit [8*10-1:0] str_bit ;
  bit [8*10-1:0] str_bita[5] ;
  bit [8*10-1:0] str_bits[] ;
  bit [8*10-1:0] str_bitr[*] ;
  
  s = "abcdefg" ;
  
  str_bit = s ;
  printf( "str_bit = %h\n", str_bit ) ;
  
  str_bita[4] = "12345678" ;
  printf( "str_bita = %h\n", str_bita[4] ) ;
  
  str_bits[548] = "eoewiypwreoiyr" ;
  printf( "str_bits = %h\n", str_bits[548] ) ;
  
  str_bitr[s] = "retiyeore" ;
  printf( "str_bitr = %h\n", str_bitr[s] ) ;

  //foo( s ) ;
  
  
  s = bit2string( str_bit ) ;
  printf( "%s\n", s ) ;
 
  s = bit2string( str_bita[4] ) ;
  printf( "%s\n", s ) ;

  s = bit2string( str_bits[548] ) ;
  printf( "%s\n", s ) ;

  s = bit2string( str_bitr["abcdefg"] ) ;
  printf( "%s\n", s ) ;

}
