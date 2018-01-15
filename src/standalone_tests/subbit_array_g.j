  bit [120:0] a[8], xa[8], ya[8] ;
  bit [30:0] b ;
  bit [8:0] c ;
  bit [0:120] aa[8], xaa[8], yaa[8] ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  bit [124:4] aaa[8], xaaa[8], yaaa[8] ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  bit [6:126] aaaa[8], xaaaa[8], yaaaa[8] ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;

main() {
  int i, j ;
  
  for( i = 0 ; i < 8 ; i++ ) {
    a[i] = {random(), random(), random(), random(), random()} ;
    aa[i] = {random(), random(), random(), random(), random()} ;
    aaa[i] = {random(), random(), random(), random(), random()} ;
    aaaa[i] = {random(), random(), random(), random(), random()} ;
  }
  b = {random(), random(), random(), random(), random()} ;
  c = {random(), random(), random(), random(), random()} ;
  bb = {random(), random(), random(), random(), random()} ;
  cc = {random(), random(), random(), random(), random()} ;
  bbb = {random(), random(), random(), random(), random()} ;
  ccc = {random(), random(), random(), random(), random()} ;
  bbbb = {random(), random(), random(), random(), random()} ;
  cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", a[5] ) ;
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
      if( a[5][i] != ((a[5] >> i) & 1) ) {
        error( 
          "Error: a[5][%d] (%b) != %b\n", i,
          a[5][i], (a[5] >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( a[3][j:i] != ((a[3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[3][%d:%d] (%b) != %b\n", j, i,
          a[3][j:i], (a[3] >> i) & ((121'b1 << (j-i+1))-1) 
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
      xa[0] = a[0] ;
      ya[0] = {random(), random(), random(), random(), random()} ;
      xa[0][j:i] = ya[0][j:i] ;
      if( xa[0][j:i] != ((ya[0] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[0][%d:%d] (%b) != %b\n", j, i,
          xa[0][j:i], (ya[0] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xa[0][120:j+1] != a[0][120:j+1] ) {
        error( 
          "Error: xa[0][120:%d] (%b) != %b\n", j+1,
          xa[0][120:j+1], a[0][120:j+1] 
        ) ;
      }
      if( i && xa[0][i-1:0] != a[0][i-1:0] ) {
        error( 
          "Error: xa[0][%d:0] (%b) != %b\n", i-1,
          xa[0][i-1:0], a[0][i-1:0] 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( aa[2][i] != ((aa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: aa[2][%d] (%b) != %b\n", i,
          aa[2][i], ((aa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aa[7][i:j] != ((aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aa[7][%d:%d] (%b) != %b\n", j, i,
          aa[7][i:j], (aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaa[6] = aa[6] ;
      yaa[6] = {random(), random(), random(), random(), random()} ;
      xaa[6][i:j] = yaa[6][i:j] ;
      if( xaa[6][i:j] != ((yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaa[6][%d:%d] (%b) != %b\n", j, i,
          xaa[6][i:j], (yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaa[6][0:i-1] != aa[6][0:i-1] ) {
        error( 
          "Error: xaa[6][0:%d] (%b) != %b\n", i-1,
          xaa[6][0:i-1], aa[6][0:i-1] 
        ) ;
      }
      if( xaa[6][j+1:120] != aa[6][j+1:120] ) {
        error( 
          "Error: xaa[6][%d:120] (%b) != %b\n", i-1,
          xaa[6][j+1:120], aa[6][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa[5] = %b\n", aaa[5] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( aaa[5][i+4] != ((aaa[5] >> i) & 1) ) {
        error( 
          "Error: aaa[5][%d] (%b) != %b\n", i+4,
          aaa[5][i+4], (aaa[5] >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaa[4][j+4:i+4] != ((aaa[4] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaa[4][%d:%d] (%b) != %b\n", j+4, i+4,
          aaa[4][j+4:i+4], (aaa[4] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaa[3] = aaa[3] ;
      yaaa[3] = {random(), random(), random(), random(), random()} ;
      xaaa[3][j+4:i+4] = yaaa[3][j+4:i+4] ;
      if( xaaa[3][j+4:i+4] != ((yaaa[3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaa[3][%d:%d] (%b) != %b\n", j, i,
          xaaa[3][j+4:i+4], (yaaa[3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xaaa[3][124:j+1+4] != aaa[3][124:j+1+4] ) {
        error( 
          "Error: xaaa[3][124:%d] (%b) != %b\n", j+1+4,
          xaaa[3][124:j+1+4], aaa[3][124:j+1+4] 
        ) ;
      }
      if( i && xaaa[3][i-1+4:4] != aaa[3][i-1+4:4] ) {
        error( 
          "Error: xaaa[3][%d:0] (%b) != %b\n", i-1+4,
          xaaa[3][i-1+4:0], aaa[3][i-1+4:0] 
        ) ;
      }
    }
  }
  

  printf( "aaaa = %b\n", aaaa[2] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( aaaa[2][i+6] != ((aaaa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: aaaa[2][%d] (%b) != %b\n", i+6,
          aaaa[2][i+6], ((aaaa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaaa[1][i+6:j+6] != ((aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaaa[1][%d:%d] (%b) != %b\n", j+6, i+6,
          aaaa[1][i+6:j+6], (aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaaa[0] = aaaa[0] ;
      yaaaa[0] = {random(), random(), random(), random(), random()} ;
      xaaaa[0][i+6:j+6] = yaaaa[0][i+6:j+6] ;
      if( xaaaa[0][i+6:j+6] != 
            ((yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: xaaaa[0][%d:%d] (%b) != %b\n", j+6, i+6,
          xaaaa[0][i+6:j+6], (yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaaaa[0][6:i-1+6] != aaaa[0][6:i-1+6] ) {
        error( 
          "Error: xaaaa[0][6:%d] (%b) != %b\n", i-1+6,
          xaaaa[0][6:i-1+6], aaaa[0][6:i-1+6] 
        ) ;
      }
      if( xaaaa[0][j+1+6:126] != aaaa[0][j+1+6:126] ) {
        error( 
          "Error: xaaaa[0][%d:126] (%b) != %b\n", i+1+6,
          xaaaa[0][j+1+6:126], aaaa[0][j+1+6:126] 
        ) ;
      }
    }
  }
  
}
