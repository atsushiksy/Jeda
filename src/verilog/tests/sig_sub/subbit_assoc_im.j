#include "port.jh"

class ss {
  bit [120:0] a[], xa[], ya[] ;
  bit [30:0] b ;
  bit [8:0] c ;
  bit [0:120] aa[], xaa[], yaa[] ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  bit [124:4] aaa[], xaaa[], yaaa[] ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  bit [6:126] aaaa[], xaaaa[], yaaaa[] ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;
  signal sa[] ;
  signal saa[] ;
  signal saaa[] ;
  signal saaaa[] ;
}

main() {
  int i, j ;
  ss s ;
  
  s = new ;
  
  for( i = 0 ; i < 8 ; i++ ) {
    s.a[i] = {random(), random(), random(), random(), random()} ;
    s.aa[i] = {random(), random(), random(), random(), random()} ;
    s.aaa[i] = {random(), random(), random(), random(), random()} ;
    s.aaaa[i] = {random(), random(), random(), random(), random()} ;
    s.sa[i] = p.a ;
    s.saa[i] = p.aa ;
    s.saaa[i] = p.aaa ;
    s.saaaa[i] = p.aaaa ;
  }
  s.b = {random(), random(), random(), random(), random()} ;
  s.c = {random(), random(), random(), random(), random()} ;
  s.bb = {random(), random(), random(), random(), random()} ;
  s.cc = {random(), random(), random(), random(), random()} ;
  s.bbb = {random(), random(), random(), random(), random()} ;
  s.ccc = {random(), random(), random(), random(), random()} ;
  s.bbbb = {random(), random(), random(), random(), random()} ;
  s.cccc = {random(), random(), random(), random(), random()} ;
  
  s.sa.0[5] = s.a[5] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( s.sa.0[5][i] != ((s.a[5] >> i) & 1) ) {
        error( 
          "Error: s.a[5][%d] (%b) != %b\n", i,
          s.sa.0[5][i], (s.a[5] >> i) & 1 
        ) ;
      }
  }
  
  s.sa.0[5] = s.a[3] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.sa.0[3][j:i] != ((s.a[3] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.sa.0[3][%d:%d] (%b) != %b\n", j, i,
          s.sa.0[3][j:i], (s.a[3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.sa.0[0] = s.a[0] ;
      @(posedge CLOCK) ;
      s.ya[0] = {random(), random(), random(), random(), random()} ;
      s.sa.0[0][j:i] = s.ya[0][j:i] ;
      @(posedge CLOCK) ;
      if( s.sa.0[0][j:i] != ((s.ya[0] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.sa.0[0][%d:%d] (%b) != %b\n", j, i,
          s.sa.0[0][j:i], (s.ya[0] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.sa.0[0][120:j+1] != s.a[0][120:j+1] ) {
        error( 
          "Error: s.sa.0[0][120:%d] (%b) != %b\n", j+1,
          s.sa.0[0][120:j+1], s.a[0][120:j+1] 
        ) ;
      }
      if( i && s.sa.0[0][i-1:0] != s.a[0][i-1:0] ) {
        error( 
          "Error: s.sa[0][%d:0] (%b) != %b\n", i-1,
          s.sa.0[0][i-1:0], s.a[0][i-1:0] 
        ) ;
      }
    }
  }
  
  s.saa.0[2] = s.aa[2] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saa.0[2][i] != ((s.aa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: s.saa.0[2][%d] (%b) != %b\n", i,
          s.saa.0[2][i], ((s.aa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  s.saa.0[2] = s.aa[7] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saa.0[7][i:j] != 
            ((s.aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saa.0[7][%d:%d] (%b) != %b\n", j, i,
          s.saa.0[7][i:j], (s.aa[7] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saa.0[6] = s.aa[6] ;
      @(posedge CLOCK) ;
      s.yaa[6] = {random(), random(), random(), random(), random()} ;
      s.saa.0[6][i:j] = s.yaa[6][i:j] ;
      @(posedge CLOCK) ;
      if( s.saa.0[6][i:j] != 
            ((s.yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saa.0[6][%d:%d] (%b) != %b\n", j, i,
          s.saa.0[6][i:j], (s.yaa[6] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saa.0[6][0:i-1] != s.aa[6][0:i-1] ) {
        error( 
          "Error: s.saa[6][0:%d] (%b) != %b\n", i-1,
          s.saa.0[6][0:i-1], s.aa[6][0:i-1] 
        ) ;
      }
      if( s.saa.0[6][j+1:120] != s.aa[6][j+1:120] ) {
        error( 
          "Error: s.saa[6][%d:120] (%b) != %b\n", i-1,
          s.saa.0[6][j+1:120], s.aa[6][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "s.aaa[5] = %b\n", s.aaa[5] ) ;

  s.saaa.0[5] = s.aaa[5] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaa.0[5][i+4] != ((s.aaa[5] >> i) & 1) ) {
        error( 
          "Error: s.saaa.0[5][%d] (%b) != %b\n", i+4,
          s.saaa.0[5][i+4], (s.aaa[5] >> i) & 1 
        ) ;
      }
  }

  s.saaa.0[5] = s.aaa[4] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaa.0[4][j+4:i+4] != 
            ((s.aaa[4] >> i) & ((121'b1 << (j-i+1))-1)) )
      {
        error( 
          "Error: s.saaa.0[4][%d:%d] (%b) != %b\n", j+4, i+4,
          s.saaa.0[4][j+4:i+4], (s.aaa[4] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaa.0[3] = s.aaa[3] ;
      @(posedge CLOCK) ;
      s.yaaa[3] = {random(), random(), random(), random(), random()} ;
      s.saaa.0[3][j+4:i+4] = s.yaaa[3][j+4:i+4] ;
      @(posedge CLOCK) ;
      if( s.saaa.0[3][j+4:i+4] != 
            ((s.yaaa[3] >> i) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaa.0[3][%d:%d] (%b) != %b\n", j, i,
          s.saaa.0[3][j+4:i+4], (s.yaaa[3] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.saaa.0[3][124:j+1+4] != s.aaa[3][124:j+1+4] ) {
        error( 
          "Error: s.saaa.0[3][124:%d] (%b) != %b\n", j+1+4,
          s.saaa.0[3][124:j+1+4], s.aaa[3][124:j+1+4] 
        ) ;
      }
      if( i && s.saaa.0[3][i-1+4:4] != s.aaa[3][i-1+4:4] ) {
        error( 
          "Error: s.saaa.0[3][%d:0] (%b) != %b\n", i-1+4,
          s.saaa.0[3][i-1+4:0], s.aaa[3][i-1+4:0] 
        ) ;
      }
    }
  }
  

  printf( "s.aaaa = %b\n", s.aaaa[2] ) ;
  
  s.saaaa.0[2] = s.aaaa[2] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaaa.0[2][i+6] != ((s.aaaa[2] >> (120-i)) & 1) ) {
        error( 
          "Error: s.saaaa.0[2][%d] (%b) != %b\n", i+6,
          s.saaaa.0[2][i+6], ((s.aaaa[2] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  s.saaaa.0[1] = s.aaaa[1] ;
  @(posedge CLOCK) ;
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaaa.0[1][i+6:j+6] != 
            ((s.aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[1][%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[1][i+6:j+6], 
          (s.aaaa[1] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaaa.0[0] = s.aaaa[0] ;
      @(posedge CLOCK) ;
      s.yaaaa[0] = {random(), random(), random(), random(), random()} ;
      s.saaaa.0[0][i+6:j+6] = s.yaaaa[0][i+6:j+6] ;
      @(posedge CLOCK) ;
      if( s.saaaa.0[0][i+6:j+6] != 
            ((s.yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[0][%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[0][i+6:j+6], 
          (s.yaaaa[0] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saaaa.0[0][6:i-1+6] != s.aaaa[0][6:i-1+6] ) {
        error( 
          "Error: s.saaaa.0[0][6:%d] (%b) != %b\n", i-1+6,
          s.saaaa.0[0][6:i-1+6], s.aaaa[0][6:i-1+6] 
        ) ;
      }
      if( s.saaaa.0[0][j+1+6:126] != s.aaaa[0][j+1+6:126] ) {
        error( 
          "Error: s.saaaa.0[0][%d:126] (%b) != %b\n", i+1+6,
          s.saaaa.0[0][j+1+6:126], s.aaaa[0][j+1+6:126] 
        ) ;
      }
    }
  }
  
  printf( "Test done!\n" ) ;
  
}
