class ss {
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

}

main() {
  ss s ;
  int i, j ;
  
  s = new ;
  
  for( i = 0 ; i < 8 ; i++ ) {
    s.a[3][4][i] = {random(), random(), random(), random(), random()} ;
    s.aa[3][4][i] = {random(), random(), random(), random(), random()} ;
    s.aaa[3][4][i] = {random(), random(), random(), random(), random()} ;
    s.aaaa[3][4][i] = {random(), random(), random(), random(), random()} ;
  }
  s.b = {random(), random(), random(), random(), random()} ;
  s.c = {random(), random(), random(), random(), random()} ;
  s.bb = {random(), random(), random(), random(), random()} ;
  s.cc = {random(), random(), random(), random(), random()} ;
  s.bbb = {random(), random(), random(), random(), random()} ;
  s.ccc = {random(), random(), random(), random(), random()} ;
  s.bbbb = {random(), random(), random(), random(), random()} ;
  s.cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "s.a = %b\n", s.a[3][4][5] ) ;
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
      if( s.a[3][4][5][i] != ((s.a[3][4][5] >> i) & 1) ) {
        error( 
          "Error: s.a[3][4][5][%d] (%b) != %b\n", i,
          s.a[3][4][5][i], (s.a[3][4][5] >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.a[3][4][3][j:i] != ((s.a[3][4][3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.a[3][4][3][%d:%d] (%b) != %b\n", j, i,
          s.a[3][4][3][j:i], (s.a[3][4][3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xa[3][4][0] = s.a[3][4][0] ;
      s.ya[3][4][0] = {random(), random(), random(), random(), random()} ;
      s.xa[3][4][0][j:i] = s.ya[3][4][0][j:i] ;
      if( s.xa[3][4][0][j:i] != ((s.ya[3][4][0] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xa[3][4][0][%d:%d] (%b) != %b\n", j, i,
          s.xa[3][4][0][j:i], (s.ya[3][4][0] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.xa[3][4][0][120:j+1] != s.a[3][4][0][120:j+1] ) {
        error( 
          "Error: s.xa[3][4][0][120:%d] (%b) != %b\n", j+1,
          s.xa[3][4][0][120:j+1], s.a[3][4][0][120:j+1] 
        ) ;
      }
      if( i && s.xa[3][4][0][i-1:0] != s.a[3][4][0][i-1:0] ) {
        error( 
          "Error: s.xa[3][4][0][%d:0] (%b) != %b\n", i-1,
          s.xa[3][4][0][i-1:0], s.a[3][4][0][i-1:0] 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aa[3][4][2][i] != ((s.aa[3][4][2] >> (120-i)) & 1) ) {
        error( 
          "Error: s.aa[3][4][2][%d] (%b) != %b\n", i,
          s.aa[3][4][2][i], ((s.aa[3][4][2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aa[3][4][7][i:j] != ((s.aa[3][4][7] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aa[3][4][7][%d:%d] (%b) != %b\n", j, i,
          s.aa[3][4][7][i:j], (s.aa[3][4][7] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaa[3][4][6] = s.aa[3][4][6] ;
      s.yaa[3][4][6] = {random(), random(), random(), random(), random()} ;
      s.xaa[3][4][6][i:j] = s.yaa[3][4][6][i:j] ;
      if( s.xaa[3][4][6][i:j] != ((s.yaa[3][4][6] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaa[3][4][6][%d:%d] (%b) != %b\n", j, i,
          s.xaa[3][4][6][i:j], (s.yaa[3][4][6] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.xaa[3][4][6][0:i-1] != s.aa[3][4][6][0:i-1] ) {
        error( 
          "Error: s.xaa[3][4][6][0:%d] (%b) != %b\n", i-1,
          s.xaa[3][4][6][0:i-1], s.aa[3][4][6][0:i-1] 
        ) ;
      }
      if( s.xaa[3][4][6][j+1:120] != s.aa[3][4][6][j+1:120] ) {
        error( 
          "Error: s.xaa[3][4][6][%d:120] (%b) != %b\n", i-1,
          s.xaa[3][4][6][j+1:120], s.aa[3][4][6][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "s.aaa[3][4][5] = %b\n", s.aaa[3][4][5] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaa[3][4][5][i+4] != ((s.aaa[3][4][5] >> i) & 1) ) {
        error( 
          "Error: s.aaa[3][4][5][%d] (%b) != %b\n", i+4,
          s.aaa[3][4][5][i+4], (s.aaa[3][4][5] >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaa[3][4][4][j+4:i+4] != ((s.aaa[3][4][4] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaa[3][4][4][%d:%d] (%b) != %b\n", j+4, i+4,
          s.aaa[3][4][4][j+4:i+4], (s.aaa[3][4][4] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaa[3][4][3] = s.aaa[3][4][3] ;
      s.yaaa[3][4][3] = {random(), random(), random(), random(), random()} ;
      s.xaaa[3][4][3][j+4:i+4] = s.yaaa[3][4][3][j+4:i+4] ;
      if( s.xaaa[3][4][3][j+4:i+4] != ((s.yaaa[3][4][3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaa[3][4][3][%d:%d] (%b) != %b\n", j, i,
          s.xaaa[3][4][3][j+4:i+4], (s.yaaa[3][4][3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.xaaa[3][4][3][124:j+1+4] != s.aaa[3][4][3][124:j+1+4] ) {
        error( 
          "Error: s.xaaa[3][4][3][124:%d] (%b) != %b\n", j+1+4,
          s.xaaa[3][4][3][124:j+1+4], s.aaa[3][4][3][124:j+1+4] 
        ) ;
      }
      if( i && s.xaaa[3][4][3][i-1+4:4] != s.aaa[3][4][3][i-1+4:4] ) {
        error( 
          "Error: s.xaaa[3][4][3][%d:0] (%b) != %b\n", i-1+4,
          s.xaaa[3][4][3][i-1+4:0], s.aaa[3][4][3][i-1+4:0] 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaaa[3][4][2][i+6] != ((s.aaaa[3][4][2] >> (120-i)) & 1) ) {
        error( 
          "Error: s.aaaa[3][4][2][%d] (%b) != %b\n", i+6,
          s.aaaa[3][4][2][i+6], ((s.aaaa[3][4][2] >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaaa[3][4][1][i+6:j+6] != ((s.aaaa[3][4][1] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaaa[3][4][1][%d:%d] (%b) != %b\n", j+6, i+6,
          s.aaaa[3][4][1][i+6:j+6], (s.aaaa[3][4][1] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaaa[3][4][0] = s.aaaa[3][4][0] ;
      s.yaaaa[3][4][0] = {random(), random(), random(), random(), random()} ;
      s.xaaaa[3][4][0][i+6:j+6] = s.yaaaa[3][4][0][i+6:j+6] ;
      if( s.xaaaa[3][4][0][i+6:j+6] != ((s.yaaaa[3][4][0] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaaa[3][4][0][%d:%d] (%b) != %b\n", j+6, i+6,
          s.xaaaa[3][4][0][i+6:j+6], (s.yaaaa[3][4][0] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.xaaaa[3][4][0][6:i-1+6] != s.aaaa[3][4][0][6:i-1+6] ) {
        error( 
          "Error: s.xaaaa[3][4][0][6:%d] (%b) != %b\n", i-1+6,
          s.xaaaa[3][4][0][6:i-1+6], s.aaaa[3][4][0][6:i-1+6] 
        ) ;
      }
      if( s.xaaaa[3][4][0][j+1+6:126] != s.aaaa[3][4][0][j+1+6:126] ) {
        error( 
          "Error: s.xaaaa[3][4][0][%d:126] (%b) != %b\n", i+1+6,
          s.xaaaa[3][4][0][j+1+6:126], s.aaaa[3][4][0][j+1+6:126] 
        ) ;
      }
    }
  }
  
}
