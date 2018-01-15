main() {
  bit [120:0] a[*], xa[*], ya[*] ;
  bit [30:0] b ;
  bit [8:0] c ;
  bit [0:120] aa[*], xaa[*], yaa[*] ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  bit [124:4] aaa[*], xaaa[*], yaaa[*] ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  bit [6:126] aaaa[*], xaaaa[*], yaaaa[*] ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;
  int i, j ;
  string str[8] ;
  
  str[0] = "rotiry" ;
  str[1] = "etyete" ;
  
  // printf( "a str[0] = %s\n", str[0] ) ;
  
  //for( i = 0 ; i < 8 ; i++ ) 
  //  printf( "str[%d] = %s\n", i, str[i] ) ;

    
  for( i = 0 ; i < 2 ; i++ ) {
    printf( "%d %s\n", i, str[i] ) ;
    a[str[i]] = {random(), random(), random(), random(), random()} ;
  }
  
  printf( "a = %b  %s\n", a[str[1]], str[1] ) ;  
  
  
  for( i = 0 ; i < 1 ; i+=10 ) {
      if( a[str[1]][i] != ((a[str[1]] >> i) & 1) ) {
        error( 
          "Error: a[str[1]][%d] (%b) != %b\n", i,
          a[str[1]][i], (a[str[1]] >> i) & 1 
        ) ;
      }
  }
  
}
