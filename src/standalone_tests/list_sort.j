/* test list.sort() function */
void main() 
{
  int int_list<>;
  string str_list<>;
  float  flt_list<>;
  double dbl_list<>;
  int i, n ;
  
  for( i = 0 ; i < 100 ; i++ ) {
    if( i & 1 ) int_list.push( i ) ;
    else int_list.bpush( i ) ;
  }

  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;
  
  for( i = 0 ; i < 100 ; i++ ) {
    int_list.push( i ) ;
  }

  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }

  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;
  
  for( i = 0 ; i < 20 ; i++ ) {
    int_list.push( i ) ;
  }

  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  

  for( i = 0 ; i < 20 ; i++ ) {
    int_list.push( i ) ;
  }
  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  for( i = 0 ; i < 50 ; i++ ) {
    int_list.push( (i*48492)%50 ) ;
  }

  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  
  
}
