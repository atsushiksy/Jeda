class subbit_class {
  bit [120:0] a, xa, ya ;
  bit [30:0] b ;
  bit [8:0] c ;
  bit [0:120] aa, xaa, yaa ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  bit [124:4] aaa, xaaa, yaaa ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  bit [6:126] aaaa, xaaaa, yaaaa ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;
}


main() {
  int i, j ;
  subbit_class s ;
  
  s = new ;
  
  s.a = {random(), random(), random(), random(), random()} ;
  s.b = {random(), random(), random(), random(), random()} ;
  s.c = {random(), random(), random(), random(), random()} ;
  s.aa = {random(), random(), random(), random(), random()} ;
  s.bb = {random(), random(), random(), random(), random()} ;
  s.cc = {random(), random(), random(), random(), random()} ;
  s.aaa = {random(), random(), random(), random(), random()} ;
  s.bbb = {random(), random(), random(), random(), random()} ;
  s.ccc = {random(), random(), random(), random(), random()} ;
  s.aaaa = {random(), random(), random(), random(), random()} ;
  s.bbbb = {random(), random(), random(), random(), random()} ;
  s.cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", s.a ) ;
  
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( s.a[i] != ((s.a >> i) & 1) ) {
        error( 
          "Error: s.a[%d] (%b) != %b\n", i,
          s.a[i], (s.a >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.a[j:i] != ((s.a >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.a[%d:%d] (%b) != %b\n", j, i,
          s.a[j:i], (s.a >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  /* single test case for debugging
    
  i = 2 ;
  j = 97 ;
  
  if( a[j:i] != ((a >> i) & ((121'b1 << (j-i+1))-1)) ) {
    printf( 
      "Error: a[%d:%d] (%b) != %b\nmsk: %b\n", j, i,
      a[j:i], (a >> i) & ((121'b1 << (j-i+1))-1), ((121'b1 << (j-i+1))-1)
    ) ;
    printf( "sft: %b\n", (121'b1 << (j-i+1)) ) ;
  }
  else {
    printf( 
      "Pass: a[%d:%d] (%b) == %b\n", j, i,
      a[j:i], (a >> i) & ((121'b1 << (j-i+1))-1) 
    ) ;
  }
  */
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xa = s.a ;
      s.ya = {random(), random(), random(), random(), random()} ;
      s.xa[j:i] = s.ya[j:i] ;
      if( s.xa[j:i] != ((s.ya >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xa[%d:%d] (%b) != %b\n", j, i,
          s.xa[j:i], (s.ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }

  /* single test case for debugging
  i = 0 ;
  j = 32 ;
      xa = a ;
      ya = {random(), random(), random(), random(), random()} ;
      xa[j:i] = ya[j:i] ;
      if( xa[j:i] != ((ya >> i) & ((121'b1 << (j-i+1))-1)) ) {
        printf( 
          "Error: xa[%d:%d] (%b) != %b\n", j, i,
          xa[j:i], (s.ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      else {
        printf( 
          "Pass: xa[%d:%d] (%b) == %b\n", j, i,
          xa[j:i], (s.ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    
    printf( "xa[j:i] = %b ya[j:i] = %b\n", xa[j:i], ya[j:i] ) ;
    printf( "ya>>i = %b\n", (ya>>i) ) ;
    printf( "msk = %b\n", ((121'b1 << (j-i+1))-1) ) ;
  */
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aa[i] != ((s.aa >> (120-i)) & 1) ) {
        error( 
          "Error: s.aa[%d] (%b) != %b\n", i,
          s.aa[i], ((s.aa >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aa[i:j] != ((s.aa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aa[%d:%d] (%b) != %b\n", j, i,
          s.aa[i:j], (s.aa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaa = s.aa ;
      s.yaa = {random(), random(), random(), random(), random()} ;
      s.xaa[i:j] = s.yaa[i:j] ;
      if( s.xaa[i:j] != ((s.yaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaa[%d:%d] (%b) != %b\n", j, i,
          s.xaa[i:j], (s.yaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa = %b\n", s.aaa ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaa[i+4] != ((s.aaa >> i) & 1) ) {
        error( 
          "Error: s.aaa[%d] (%b) != %b\n", i+4,
          s.aaa[i+4], (s.aaa >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaa[j+4:i+4] != ((s.aaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaa[%d:%d] (%b) != %b\n", j+4, i+4,
          s.aaa[j+4:i+4], (s.aaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaa = s.aaa ;
      s.yaaa = {random(), random(), random(), random(), random()} ;
      s.xaaa[j+4:i+4] = s.yaaa[j+4:i+4] ;
      if( s.xaaa[j+4:i+4] != ((s.yaaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaa[%d:%d] (%b) != %b\n", j, i,
          s.xaaa[j+4:i+4], (s.yaaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaaa[i+6] != ((s.aaaa >> (120-i)) & 1) ) {
        error( 
          "Error: s.aaaa[%d] (%b) != %b\n", i+6,
          s.aaaa[i+6], ((s.aaaa >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaaa[i+6:j+6] != ((s.aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaaa[%d:%d] (%b) != %b\n", j+6, i+6,
          s.aaaa[i+6:j+6], (s.aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaaa = s.aaaa ;
      s.yaaaa = {random(), random(), random(), random(), random()} ;
      s.xaaaa[i+6:j+6] = s.yaaaa[i+6:j+6] ;
      if( s.xaaaa[i+6:j+6] != ((s.yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaaa[%d:%d] (%b) != %b\n", j+6, i+6,
          s.xaaaa[i+6:j+6], (s.yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
}
