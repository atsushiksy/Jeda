  bit [120:0] a[8][8][8], xa[8][8][8], ya[8][8][8] ;
  bit [30:0] b ;
  bit [8:0] c ;
  bit [0:120] aa[8][8][8], xaa[8][8][8], yaa[8][8][8] ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  bit [124:4] aaa[8][8][8], xaaa[8][8][8], yaaa[8][8][8] ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  bit [6:126] aaaa[8][8][8], xaaaa[8][8][8], yaaaa[8][8][8] ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;


main() {
  int i, j ;
  
  for( i = 0 ; i < 8 ; i++ ) {
    a[3][4][i] = {random(), random(), random(), random(), random()} ;
    aa[3][4][i] = {random(), random(), random(), random(), random()} ;
    aaa[3][4][i] = {random(), random(), random(), random(), random()} ;
    aaaa[3][4][i] = {random(), random(), random(), random(), random()} ;
  }
  b = {random(), random(), random(), random(), random()} ;
  c = {random(), random(), random(), random(), random()} ;
  bb = {random(), random(), random(), random(), random()} ;
  cc = {random(), random(), random(), random(), random()} ;
  bbb = {random(), random(), random(), random(), random()} ;
  ccc = {random(), random(), random(), random(), random()} ;
  bbbb = {random(), random(), random(), random(), random()} ;
  cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", a[3][4][5] ) ;
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
      if( a[3][4][5][i] != ((a[3][4][5] >> i) & 1) ) {
        error( 
          "Error: a[3][4][5][%d] (%b) != %b\n", i,
          a[3][4][5][i], (a[3][4][5] >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( a[3][4][3][j:i] != ((a[3][4][3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[3][4][3][%d:%d] (%b) != %b\n", j, i,
          a[3][4][3][j:i], (a[3][4][3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xa[3][4][0] = a[3][4][0] ;
      ya[3][4][0] = {random(), random(), random(), random(), random()} ;
      xa[3][4][0][j:i] = ya[3][4][0][j:i] ;
      if( xa[3][4][0][j:i] != ((ya[3][4][0] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[3][4][0][%d:%d] (%b) != %b\n", j, i,
          xa[3][4][0][j:i], (ya[3][4][0] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xa[3][4][0][120:j+1] != a[3][4][0][120:j+1] ) {
        error( 
          "Error: xa[3][4][0][120:%d] (%b) != %b\n", j+1,
          xa[3][4][0][120:j+1], a[3][4][0][120:j+1] 
        ) ;
      }
      if( i && xa[3][4][0][i-1:0] != a[3][4][0][i-1:0] ) {
        error( 
          "Error: xa[3][4][0][%d:0] (%b) != %b\n", i-1,
          xa[3][4][0][i-1:0], a[3][4][0][i-1:0] 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( aa[3][4][2][i] != ((aa[3][4][2] >> (120-i)) & 1) ) {
        error( 
          "Error: aa[3][4][2][%d] (%b) != %b\n", i,
          aa[3][4][2][i], ((aa[3][4][2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aa[3][4][7][i:j] != ((aa[3][4][7] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aa[3][4][7][%d:%d] (%b) != %b\n", j, i,
          aa[3][4][7][i:j], (aa[3][4][7] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaa[3][4][6] = aa[3][4][6] ;
      yaa[3][4][6] = {random(), random(), random(), random(), random()} ;
      xaa[3][4][6][i:j] = yaa[3][4][6][i:j] ;
      if( xaa[3][4][6][i:j] != ((yaa[3][4][6] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaa[3][4][6][%d:%d] (%b) != %b\n", j, i,
          xaa[3][4][6][i:j], (yaa[3][4][6] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaa[3][4][6][0:i-1] != aa[3][4][6][0:i-1] ) {
        error( 
          "Error: xaa[3][4][6][0:%d] (%b) != %b\n", i-1,
          xaa[3][4][6][0:i-1], aa[3][4][6][0:i-1] 
        ) ;
      }
      if( xaa[3][4][6][j+1:120] != aa[3][4][6][j+1:120] ) {
        error( 
          "Error: xaa[3][4][6][%d:120] (%b) != %b\n", i-1,
          xaa[3][4][6][j+1:120], aa[3][4][6][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa[3][4][5] = %b\n", aaa[3][4][5] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( aaa[3][4][5][i+4] != ((aaa[3][4][5] >> i) & 1) ) {
        error( 
          "Error: aaa[3][4][5][%d] (%b) != %b\n", i+4,
          aaa[3][4][5][i+4], (aaa[3][4][5] >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaa[3][4][4][j+4:i+4] != ((aaa[3][4][4] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaa[3][4][4][%d:%d] (%b) != %b\n", j+4, i+4,
          aaa[3][4][4][j+4:i+4], (aaa[3][4][4] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaa[3][4][3] = aaa[3][4][3] ;
      yaaa[3][4][3] = {random(), random(), random(), random(), random()} ;
      xaaa[3][4][3][j+4:i+4] = yaaa[3][4][3][j+4:i+4] ;
      if( xaaa[3][4][3][j+4:i+4] != ((yaaa[3][4][3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaa[3][4][3][%d:%d] (%b) != %b\n", j, i,
          xaaa[3][4][3][j+4:i+4], (yaaa[3][4][3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xaaa[3][4][3][124:j+1+4] != aaa[3][4][3][124:j+1+4] ) {
        error( 
          "Error: xaaa[3][4][3][124:%d] (%b) != %b\n", j+1+4,
          xaaa[3][4][3][124:j+1+4], aaa[3][4][3][124:j+1+4] 
        ) ;
      }
      if( i && xaaa[3][4][3][i-1+4:4] != aaa[3][4][3][i-1+4:4] ) {
        error( 
          "Error: xaaa[3][4][3][%d:0] (%b) != %b\n", i-1+4,
          xaaa[3][4][3][i-1+4:0], aaa[3][4][3][i-1+4:0] 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( aaaa[3][4][2][i+6] != ((aaaa[3][4][2] >> (120-i)) & 1) ) {
        error( 
          "Error: aaaa[3][4][2][%d] (%b) != %b\n", i+6,
          aaaa[3][4][2][i+6], ((aaaa[3][4][2] >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaaa[3][4][1][i+6:j+6] != ((aaaa[3][4][1] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaaa[3][4][1][%d:%d] (%b) != %b\n", j+6, i+6,
          aaaa[3][4][1][i+6:j+6], (aaaa[3][4][1] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaaa[3][4][0] = aaaa[3][4][0] ;
      yaaaa[3][4][0] = {random(), random(), random(), random(), random()} ;
      xaaaa[3][4][0][i+6:j+6] = yaaaa[3][4][0][i+6:j+6] ;
      if( xaaaa[3][4][0][i+6:j+6] != ((yaaaa[3][4][0] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaaa[3][4][0][%d:%d] (%b) != %b\n", j+6, i+6,
          xaaaa[3][4][0][i+6:j+6], (yaaaa[3][4][0] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaaaa[3][4][0][6:i-1+6] != aaaa[3][4][0][6:i-1+6] ) {
        error( 
          "Error: xaaaa[3][4][0][6:%d] (%b) != %b\n", i-1+6,
          xaaaa[3][4][0][6:i-1+6], aaaa[3][4][0][6:i-1+6] 
        ) ;
      }
      if( xaaaa[3][4][0][j+1+6:126] != aaaa[3][4][0][j+1+6:126] ) {
        error( 
          "Error: xaaaa[3][4][0][%d:126] (%b) != %b\n", i+1+6,
          xaaaa[3][4][0][j+1+6:126], aaaa[3][4][0][j+1+6:126] 
        ) ;
      }
    }
  }
  
}
