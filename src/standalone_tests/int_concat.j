main() {
  int x ;
  bit [3:0] bit5 ;
  bit [7:0] bit8 ;
  bit [31:0] bit31 ;
  bit [250:0] bit250 ;
  x = 32'hffffffff ;
  
  bit8 = 0 ;
  bit5 = 0 ;
  
  bit250 = 251'bz ;
  printf( "bit250 = %b\n", bit250 ) ;
  bit250 = {bit5,x,bit8} ;
  printf( "%b %b %b %b\n", {bit5,x,bit8},bit5,x,bit8 ) ;
  printf( "%b \n", bit250 ) ;

}
