/* test list.sort() function */
void main() 
{
  float  flt_list<>;
  double dbl_list<>;
  int i, n ;
  
  for( i = 0 ; i < 100 ; i++ ) {
    if( i & 1 ) dbl_list.push( i*5.5 ) ;
    else dbl_list.bpush( i*10.5 ) ;
  }

  printf( "dbl_list.num_element() = %0d\n", dbl_list.num_element() ) ;
  
  printf( "calling dbl_list.sort()\n" ) ;
   
  dbl_list.sort() ;
   
  n = dbl_list.num_element() ;
  printf( "dbl_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "dbl_list<%d> = %f \n", i, dbl_list.pop() ) ;
  }
    
  
}
