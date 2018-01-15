/* test list.sort() function */
void main() 
{
  int int_list<>;
  int int_assoc[] ;
  int i, n ;
  
  //for( i = 0 ; i < 900 ; i++ ) {  // -> infinite loop
  for( i = 0 ; i < 1000 ; i++ ) {
    if( i & 1 ) int_assoc[i] = i * 10 ;
    else int_assoc[i*1000] = i ;
  }

  printf( "int_assoc.num_element() = %0d\n", int_assoc.num_element() ) ;
  
  printf( "calling int_assoc.index_list()\n" ) ;
   
  int_list = int_assoc.index_list() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;
  
  int_list = int_assoc.index_list() ;

  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
    
  for( i = 0 ; i < 50 ; i++ ) {
    if( i & 1 ) {
      printf( "calling int_assoc.delete_index(%d)\n", i ) ;
      int_assoc.delete_index(i) ;
      printf( "int_assoc.num_element() = %0d\n", int_assoc.num_element() ) ;
    }
  }
  
  
  printf( "calling int_assoc.delete_index(%d)\n", 1 ) ;
  int_assoc.delete_index(1) ;
  
  printf( "int_assoc.num_element() = %0d\n", int_assoc.num_element() ) ;
  
  printf( "calling int_assoc.index_list()\n" ) ;
   
  int_list = int_assoc.index_list() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;
  
  int_list = int_assoc.index_list() ;

  printf( "int_list.num_element() = %0d\n", int_list.num_element() ) ;
  
  printf( "calling int_list.sort()\n" ) ;
   
  int_list.sort() ;
   
  n = int_list.num_element() ;
  printf( "int_list.num_element() = %0d\n", n ) ;

  for( i = 0 ; i < n ; i++ ) {
    printf( "intlist<%d> = %d\n", i, int_list.pop() ) ;
  }
  
}
