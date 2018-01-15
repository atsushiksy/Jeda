main() {
  int lic ;

  lic = checkout_license( "jeda_develop", "1.0" ) ;
  
  
  printf( "Got lic %h\n", lic ) ;
  
  checkin_license( lic ) ;
  
  printf( "done.\n" ) ;  

}
