#include "port.jh"
class ss {
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
  int i, j ;
  signal sa[8] ;
  signal saa[8] ;
  signal saaa[8] ;
  signal saaaa[8] ;

test_main() {
  
  for( i = 0 ; i < 8 ; i++ ) {
    a[i] = {random(), random(), random(), random(), random()} ;
    aa[i] = {random(), random(), random(), random(), random()} ;
    aaa[i] = {random(), random(), random(), random(), random()} ;
    aaaa[i] = {random(), random(), random(), random(), random()} ;
    sa[i] = p.a ;
    saa[i] = p.aa ;
    saaa[i] = p.aaa ;
    saaaa[i] = p.aaaa ;
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
  
  sa.0[5] = a[5] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( sa.0[5][i] != ((a[5] >> i) & 1) ) {
        error( 
          "Error: a[5][%d] (%b) != %b\n", i,
          sa.0[5][i], (a[5] >> i) & 1 
        ) ;
      }
  }
  
  sa.0[5] = a[3] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( sa.0[3][j:i] != ((a[3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: sa.0[3][%d:%d] (%b) != %b\n", j, i,
          sa.0[3][j:i], (a[3] >> i) & ((121'b1 << (j-i+1))-1) 
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
      sa.0[0] = a[0] ;
      @(posedge CLOCK) ;
      ya[0] = {random(), random(), random(), random(), random()} ;
      sa.0[0][j:i] = ya[0][j:i] ;
      @(posedge CLOCK) ;
      if( sa.0[0][j:i] != ((ya[0] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: sa.0[0][%d:%d] (%b) != %b\n", j, i,
          sa.0[0][j:i], (ya[0] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( sa.0[0][120:j+1] != a[0][120:j+1] ) {
        error( 
          "Error: sa.0[0][120:%d] (%b) != %b\n", j+1,
          sa.0[0][120:j+1], a[0][120:j+1] 
        ) ;
      }
      if( i && sa.0[0][i-1:0] != a[0][i-1:0] ) {
        error( 
          "Error: xa[0][%d:0] (%b) != %b\n", i-1,
          sa.0[0][i-1:0], a[0][i-1:0] 
        ) ;
      }
    }
  }
  
  saa.0[2] = aa[2] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saa.0[2][i] != ((aa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: saa.0[2][%d] (%b) != %b\n", i,
          saa.0[2][i], ((aa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  saa.0[2] = aa[7] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saa.0[7][i:j] != ((aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saa.0[7][%d:%d] (%b) != %b\n", j, i,
          saa.0[7][i:j], (aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saa.0[6] = aa[6] ;
      @(posedge CLOCK) ;
      yaa[6] = {random(), random(), random(), random(), random()} ;
      saa.0[6][i:j] = yaa[6][i:j] ;
      @(posedge CLOCK) ;
      if( saa.0[6][i:j] != ((yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saa.0[6][%d:%d] (%b) != %b\n", j, i,
          saa.0[6][i:j], (yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saa.0[6][0:i-1] != aa[6][0:i-1] ) {
        error( 
          "Error: xaa[6][0:%d] (%b) != %b\n", i-1,
          saa.0[6][0:i-1], aa[6][0:i-1] 
        ) ;
      }
      if( saa.0[6][j+1:120] != aa[6][j+1:120] ) {
        error( 
          "Error: xaa[6][%d:120] (%b) != %b\n", i-1,
          saa.0[6][j+1:120], aa[6][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa[5] = %b\n", aaa[5] ) ;

  saaa.0[5] = aaa[5] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saaa.0[5][i+4] != ((aaa[5] >> i) & 1) ) {
        error( 
          "Error: saaa.0[5][%d] (%b) != %b\n", i+4,
          saaa.0[5][i+4], (aaa[5] >> i) & 1 
        ) ;
      }
  }

  saaa.0[5] = aaa[4] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaa.0[4][j+4:i+4] != ((aaa[4] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saaa.0[4][%d:%d] (%b) != %b\n", j+4, i+4,
          saaa.0[4][j+4:i+4], (aaa[4] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaa.0[3] = aaa[3] ;
      @(posedge CLOCK) ;
      yaaa[3] = {random(), random(), random(), random(), random()} ;
      saaa.0[3][j+4:i+4] = yaaa[3][j+4:i+4] ;
      @(posedge CLOCK) ;
      if( saaa.0[3][j+4:i+4] != ((yaaa[3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saaa.0[3][%d:%d] (%b) != %b\n", j, i,
          saaa.0[3][j+4:i+4], (yaaa[3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( saaa.0[3][124:j+1+4] != aaa[3][124:j+1+4] ) {
        error( 
          "Error: saaa.0[3][124:%d] (%b) != %b\n", j+1+4,
          saaa.0[3][124:j+1+4], aaa[3][124:j+1+4] 
        ) ;
      }
      if( i && saaa.0[3][i-1+4:4] != aaa[3][i-1+4:4] ) {
        error( 
          "Error: saaa.0[3][%d:0] (%b) != %b\n", i-1+4,
          saaa.0[3][i-1+4:0], aaa[3][i-1+4:0] 
        ) ;
      }
    }
  }
  

  printf( "aaaa = %b\n", aaaa[2] ) ;
  
  saaaa.0[2] = aaaa[2] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saaaa.0[2][i+6] != ((aaaa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: saaaa.0[2][%d] (%b) != %b\n", i+6,
          saaaa.0[2][i+6], ((aaaa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  saaaa.0[1] = aaaa[1] ;
  @(posedge CLOCK) ;
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaaa.0[1][i+6:j+6] != 
            ((aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saaaa.0[1][%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[1][i+6:j+6], (aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaaa.0[0] = aaaa[0] ;
      @(posedge CLOCK) ;
      yaaaa[0] = {random(), random(), random(), random(), random()} ;
      saaaa.0[0][i+6:j+6] = yaaaa[0][i+6:j+6] ;
      @(posedge CLOCK) ;
      if( saaaa.0[0][i+6:j+6] != 
            ((yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saaaa.0[0][%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[0][i+6:j+6], (yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saaaa.0[0][6:i-1+6] != aaaa[0][6:i-1+6] ) {
        error( 
          "Error: saaaa.0[0][6:%d] (%b) != %b\n", i-1+6,
          saaaa.0[0][6:i-1+6], aaaa[0][6:i-1+6] 
        ) ;
      }
      if( saaaa.0[0][j+1+6:126] != aaaa[0][j+1+6:126] ) {
        error( 
          "Error: saaaa.0[0][%d:126] (%b) != %b\n", i+1+6,
          saaaa.0[0][j+1+6:126], aaaa[0][j+1+6:126] 
        ) ;
      }
    }
  }
  
  printf( "Test done!\n" ) ;
}

} // end of class

main() {
  ss s ;
  
  s = new ;
  
  s.test_main() ;
}
