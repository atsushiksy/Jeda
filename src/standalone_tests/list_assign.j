main() {
  int x ;
  bit [5:0] bit5 ;
  bit [8:0] bit8 ;
  bit [31:0] bit31 ;
  
  '{bit5,bit8} = 32'habadbeef ;
  
  printf( "%b %b %h\n", bit5, bit8, {bit5,bit8} );
  '{bit5[4:2],bit31,bit8[8:1]} = 32'habcdef01 ;

  printf( 
    "%b %b %b %h\n", bit5[4:2], bit31, bit8[8:1], 
    {bit5[4:2], bit31, bit8[8:1]} 
  );
  
  x = 32'hffffffff ;
  
  printf( "%b %h\n", {bit5[4:2],x, bit8[8:1]}, {bit5[4:2],x, bit8[8:1]} ) ;

}
