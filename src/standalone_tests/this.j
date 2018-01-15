   class PioRegister {
       bit [24:0] address;
   
       void new(bit [24:0] my_address)
       {
	   this.address = my_address;
       }
       
       void print_addr() {
         printf( "addr = %h\n", this.address ) ;
       }
   }  


main() {
  PioRegister foo ;
  
  foo = new( 20 ) ;
  
  printf( "foo.address = %h\n", foo.address ) ;
  
  foo.print_addr() ;

}
