/* test list.sort() function */
void main() 
{
  float  flt_list<>;
  double dbl_list<>;
  int i, n ;
  
  for( i = 0 ; i < 100 ; i++ ) {
    if( i & 1 ) flt_list.push( i*5.5 ) ;
    else flt_list.bpush( i*10.5 ) ;
  }

  printf( "flt_list.num_element() = %0d\n", flt_list.num_element() ) ;
  
  printf( "calling flt_list.sort()\n" ) ;
   
  flt_list.sort() ;
   
  n = flt_list.num_element() ;
  printf( "flt_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "flt_list<%d> = %f \n", i, flt_list.pop() ) ;
  }
    
  
}
