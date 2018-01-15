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



main() {
  int i, j ;
  
  a = {random(), random(), random(), random(), random()} ;
  b = {random(), random(), random(), random(), random()} ;
  c = {random(), random(), random(), random(), random()} ;
  aa = {random(), random(), random(), random(), random()} ;
  bb = {random(), random(), random(), random(), random()} ;
  cc = {random(), random(), random(), random(), random()} ;
  aaa = {random(), random(), random(), random(), random()} ;
  bbb = {random(), random(), random(), random(), random()} ;
  ccc = {random(), random(), random(), random(), random()} ;
  aaaa = {random(), random(), random(), random(), random()} ;
  bbbb = {random(), random(), random(), random(), random()} ;
  cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", a ) ;
  /*
  printf( "aa = %b\n", aa ) ;
  printf( "aaa = %b\n", aaa ) ;
  printf( "aaaa = %b\n", aaaa ) ;
  printf( "b = %b\n", b ) ;
  printf( "bb = %b\n", bb ) ;
  printf( "bbb = %b\n", bbb ) ;
  printf( "bbbb = %b\n", bbbb ) ;
  printf( "c = %b\n", c ) ;
  printf( "cc = %b\n", cc ) ;
  printf( "ccc = %b\n", ccc ) ;
  printf( "cccc = %b\n", cccc ) ;
  */
  
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( a[i] != ((a >> i) & 1) ) {
        error( 
          "Error: a[%d] (%b) != %b\n", i,
          a[i], (a >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( a[j:i] != ((a >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[%d:%d] (%b) != %b\n", j, i,
          a[j:i], (a >> i) & ((121'b1 << (j-i+1))-1) 
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
      xa = a ;
      ya = {random(), random(), random(), random(), random()} ;
      xa[j:i] = ya[j:i] ;
      if( xa[j:i] != ((ya >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[%d:%d] (%b) != %b\n", j, i,
          xa[j:i], (ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }

  
  for( i = 0 ; i < 120 ; i++ ) {
      if( aa[i] != ((aa >> (120-i)) & 1) ) {
        error( 
          "Error: aa[%d] (%b) != %b\n", i,
          aa[i], ((aa >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aa[i:j] != ((aa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aa[%d:%d] (%b) != %b\n", j, i,
          aa[i:j], (aa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaa = aa ;
      yaa = {random(), random(), random(), random(), random()} ;
      xaa[i:j] = yaa[i:j] ;
      if( xaa[i:j] != ((yaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[%d:%d] (%b) != %b\n", j, i,
          xaa[i:j], (yaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa = %b\n", aaa ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( aaa[i+4] != ((aaa >> i) & 1) ) {
        error( 
          "Error: a[%d] (%b) != %b\n", i+4,
          aaa[i+4], (aaa >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaa[j+4:i+4] != ((aaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[%d:%d] (%b) != %b\n", j+4, i+4,
          aaa[j+4:i+4], (aaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaa = aaa ;
      yaaa = {random(), random(), random(), random(), random()} ;
      xaaa[j+4:i+4] = yaaa[j+4:i+4] ;
      if( xaaa[j+4:i+4] != ((yaaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaa[%d:%d] (%b) != %b\n", j, i,
          xaaa[j+4:i+4], (yaaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( aaaa[i+6] != ((aaaa >> (120-i)) & 1) ) {
        error( 
          "Error: aaaa[%d] (%b) != %b\n", i+6,
          aaaa[i+6], ((aaaa >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaaa[i+6:j+6] != ((aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaaa[%d:%d] (%b) != %b\n", j+6, i+6,
          aaaa[i+6:j+6], (aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaaa = aaaa ;
      yaaaa = {random(), random(), random(), random(), random()} ;
      xaaaa[i+6:j+6] = yaaaa[i+6:j+6] ;
      if( xaaaa[i+6:j+6] != ((yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaaa[%d:%d] (%b) != %b\n", j+6, i+6,
          xaaaa[i+6:j+6], (yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
}
