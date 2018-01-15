main() {

  float fl ;
  int ar[] ;
  int i ;
  
  // i = ar[fl] ;
  
  i = ar[5+2] ;
  
  i = ar[ar[ar[5+2]]] ;
  
  printf( "%0d", ar[ar[ar[5+2]]] ) ;
  
  ar[5+i] = 0 ;
  
}
