#include "port.jh"

  signal sa ;
  bit [120:0] a, xa, ya ;
  bit [30:0] b ;
  bit [8:0] c ;
  signal saa ;
  bit [0:120] aa, xaa, yaa ;
  bit [0:30] bb ;
  bit [0:8] cc ;
  signal saaa ;
  bit [124:4] aaa, xaaa, yaaa ;
  bit [36:6] bbb ;
  bit [10:2] ccc ;
  signal saaaa ;
  bit [6:126] aaaa, xaaaa, yaaaa ;
  bit [9:39] bbbb ;
  bit [1:9] cccc ;
  int i, j ;

main() {
  
  sa = p.a ;
  saa = p.aa ;
  saaa = p.aaa ;
  saaaa = p.aaaa ;
  
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
  // printf( "a = %b\n", aa[5:0] ) ;
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
  
  sa.0 = a ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( sa.0[i] != ((a >> i) & 1) ) {
        error( 
          "Error: sa[%d] (%b) != %b\n", i,
          sa.0[i], (a >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( sa.0[j:i] != ((a >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[%d:%d] (%b) != %b\n", j, i,
          sa.0[j:i], (a >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  /* single test case for debugging
    
  i = 2 ;
  j = 97 ;
  
  if( a[j:i] != ((a >> i) & ((121'b1 << (j-i+1))-1)) ) {
    error( 
      "Error: a[%d:%d] (%b) != %b\nmsk: %b\n", j, i,
      a[j:i], (a >> i) & ((121'b1 << (j-i+1))-1), ((121'b1 << (j-i+1))-1)
    ) ;
    error( "sft: %b\n", (121'b1 << (j-i+1)) ) ;
  }
  else {
    error( 
      "Pass: a[%d:%d] (%b) == %b\n", j, i,
      a[j:i], (a >> i) & ((121'b1 << (j-i+1))-1) 
    ) ;
  }
  */
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      sa.0 = a ;
      ya = {random(), random(), random(), random(), random()} ;
      @(posedge CLOCK) ;
      sa.0[j:i] = ya[j:i] ;
      @(posedge CLOCK) ;
      if( sa.0[j:i] != ((ya >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: sa.0[%d:%d] (%b) != %b\n", j, i,
          sa.0[j:i], (ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( sa.0[120:j+1] != a[120:j+1] ) {
        error( 
          "Error: sa.0[120:%d] (%b) != %b\n", j+1,
          sa.0[120:j+1], a[120:j+1] 
        ) ;
      }
      if( i && sa.0[i-1:0] != a[i-1:0] ) {
        error( 
          "Error: sa.0[%d:0] (%b) != %b\n", i-1,
          sa.0[i-1:0], a[i-1:0] 
        ) ;
      }
    }
  }
  
  saa.0 = aa ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saa.0[i] != ((aa >> (120-i)) & 1) ) {
        error( 
          "Error: saa.0[%d] (%b) != %b\n", i,
          saa.0[i], ((aa >> (120-i)) & 1) 
        ) ;
      }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saa.0[i:j] != ((aa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aa[%d:%d] (%b) != %b\n", j, i,
          saa.0[i:j], (aa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saa.0 = aa ;
      @(posedge CLOCK) ;
      yaa = {random(), random(), random(), random(), random()} ;
      saa.0[i:j] = yaa[i:j] ;
      @(posedge CLOCK) ;
      if( saa.0[i:j] != ((yaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saa.0[%d:%d] (%b) != %b\n", i, j,
          saa.0[i:j], (yaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saa.0[0:i-1] != aa[0:i-1] ) {
        error( 
          "Error: saa.0[0:%d] (%b) != %b\n", i-1,
          saa.0[0:i-1], aa[0:i-1] 
        ) ;
      }
      if( saa.0[j+1:120] != aa[j+1:120] ) {
        error( 
          "Error: saa[%d:120] (%b) != %b\n", i+1,
          saa.0[j+1:120], aa[j+1:120] 
        ) ;
      }
    }
  }
  
  
  saaa.0 = aaa ;
  @(posedge CLOCK) ;
  
  printf( "aaa = %b\n", aaa ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( saaa.0[i+4] != ((aaa >> i) & 1) ) {
        error( 
          "Error: a[%d] (%b) != %b\n", i+4,
          saaa.0[i+4], (aaa >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaa.0[j+4:i+4] != ((aaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saaa.0[%d:%d] (%b) != %b\n", j+4, i+4,
          saaa.0[j+4:i+4], (aaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaa.0 = aaa ;
      @(posedge CLOCK) ;
      yaaa = {random(), random(), random(), random(), random()} ;
      saaa.0[j+4:i+4] = yaaa[j+4:i+4] ;
      @(posedge CLOCK) ;
      if( saaa.0[j+4:i+4] != ((yaaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saaa.0[%d:%d] (%b) != %b\n", j, i,
          saaa.0[j+4:i+4], (yaaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( saaa.0[124:j+1+4] != aaa[124:j+1+4] ) {
        error( 
          "Error: saaa.0[124:%d] (%b) != %b\n", j+1+4,
          saaa.0[124:j+1+4], aaa[124:j+1+4] 
        ) ;
      }
      if( i && saaa.0[i-1+4:4] != aaa[i-1+4:4] ) {
        error( 
          "Error: xa[%d:0] (%b) != %b\n", i-1+4,
          saaa.0[i-1+4:0], aaa[i-1+4:0] 
        ) ;
      }
      else {
      
      }
    }
  }
  
  printf( "aaaa = %b\n", aaaa ) ;

  saaaa.0 = aaaa ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saaaa.0[i+6] != ((aaaa >> (120-i)) & 1) ) {
        error( 
          "Error: saaaa.0[%d] (%b) != %b\n", i+6,
          saaaa.0[i+6], ((aaaa >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaaa.0[i+6:j+6] != ((aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: saaaa.0[%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[i+6:j+6], (aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaaa.0 = aaaa ;
      @(posedge CLOCK) ;
      yaaaa = {random(), random(), random(), random(), random()} ;
      saaaa.0[i+6:j+6] = yaaaa[i+6:j+6] ;
      @(posedge CLOCK) ;
      if( saaaa.0[i+6:j+6] != ((yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaaa[%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[i+6:j+6], (yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saaaa.0[6:i-1+6] != aaaa[6:i-1+6] ) {
        error( 
          "Error: saaaa.0[6:%d] (%b) != %b\n", i-1+6,
          saaaa.0[6:i-1+6], aaaa[6:i-1+6] 
        ) ;
      }
      if( saaaa.0[j+1+6:126] != aaaa[j+1+6:126] ) {
        error( 
          "Error: saaaa.0[%d:126] (%b) != %b\n", i+1+6,
          saaaa.0[j+1+6:126], aaaa[j+1+6:126] 
        ) ;
      }
    }
  }
  
  printf( "Test done\n" ) ;
  
}
