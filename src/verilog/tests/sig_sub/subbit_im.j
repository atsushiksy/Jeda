#include "port.jh"

class subbit_class {
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
}

main() {
  int i, j ;
  subbit_class s ;
  
  s = new ;
  
  s.sa = p.a ;
  s.saa = p.aa ;
  s.saaa = p.aaa ;
  s.saaaa = p.aaaa ;
  
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
  
  s.sa.0 = s.a ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( s.sa.0[i] != ((s.a >> i) & 1) ) {
        error( 
          "Error: s.sa[%d] (%b) != %b\n", i,
          s.sa.0[i], (s.a >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.sa.0[j:i] != ((s.a >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.a[%d:%d] (%b) != %b\n", j, i,
          s.sa.0[j:i], (s.a >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.sa.0 = s.a ;
      s.ya = {random(), random(), random(), random(), random()} ;
      @(posedge CLOCK) ;
      s.sa.0[j:i] = s.ya[j:i] ;
      @(posedge CLOCK) ;
      if( s.sa.0[j:i] != ((s.ya >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.sa.0[%d:%d] (%b) != %b\n", j, i,
          s.sa.0[j:i], (s.ya >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.sa.0[120:j+1] != s.a[120:j+1] ) {
        error( 
          "Error: s.sa.0[120:%d] (%b) != %b\n", j+1,
          s.sa.0[120:j+1], s.a[120:j+1] 
        ) ;
      }
      if( i && s.sa.0[i-1:0] != s.a[i-1:0] ) {
        error( 
          "Error: s.sa.0[%d:0] (%b) != %b\n", i-1,
          s.sa.0[i-1:0], s.a[i-1:0] 
        ) ;
      }
    }
  }
  
  s.saa.0 = s.aa ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saa.0[i] != ((s.aa >> (120-i)) & 1) ) {
        error( 
          "Error: s.saa.0[%d] (%b) != %b\n", i,
          s.saa.0[i], ((s.aa >> (120-i)) & 1) 
        ) ;
      }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saa.0[i:j] != ((s.aa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.saa[%d:%d] (%b) != %b\n", j, i,
          s.saa.0[i:j], (s.aa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saa.0 = s.aa ;
      @(posedge CLOCK) ;
      s.yaa = {random(), random(), random(), random(), random()} ;
      s.saa.0[i:j] = s.yaa[i:j] ;
      @(posedge CLOCK) ;
      if( s.saa.0[i:j] != ((s.yaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.saa.0[%d:%d] (%b) != %b\n", i, j,
          s.saa.0[i:j], (s.yaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saa.0[0:i-1] != s.aa[0:i-1] ) {
        error( 
          "Error: s.saa.0[0:%d] (%b) != %b\n", i-1,
          s.saa.0[0:i-1], s.aa[0:i-1] 
        ) ;
      }
      if( s.saa.0[j+1:120] != s.aa[j+1:120] ) {
        error( 
          "Error: s.saa[%d:120] (%b) != %b\n", i+1,
          s.saa.0[j+1:120], s.aa[j+1:120] 
        ) ;
      }
    }
  }
  
  
  s.saaa.0 = s.aaa ;
  @(posedge CLOCK) ;
  
  printf( "s.aaa = %b\n", s.aaa ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaa.0[i+4] != ((s.aaa >> i) & 1) ) {
        error( 
          "Error: s.saaa[%d] (%b) != %b\n", i+4,
          s.saaa.0[i+4], (s.aaa >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaa.0[j+4:i+4] != ((s.aaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.saaa.0[%d:%d] (%b) != %b\n", j+4, i+4,
          s.saaa.0[j+4:i+4], (s.aaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaa.0 = s.aaa ;
      @(posedge CLOCK) ;
      s.yaaa = {random(), random(), random(), random(), random()} ;
      s.saaa.0[j+4:i+4] = s.yaaa[j+4:i+4] ;
      @(posedge CLOCK) ;
      if( s.saaa.0[j+4:i+4] != ((s.yaaa >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.saaa.0[%d:%d] (%b) != %b\n", j, i,
          s.saaa.0[j+4:i+4], (s.yaaa >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.saaa.0[124:j+1+4] != s.aaa[124:j+1+4] ) {
        error( 
          "Error: s.saaa.0[124:%d] (%b) != %b\n", j+1+4,
          s.saaa.0[124:j+1+4], s.aaa[124:j+1+4] 
        ) ;
      }
      if( i && s.saaa.0[i-1+4:4] != s.aaa[i-1+4:4] ) {
        error( 
          "Error: s.saaa[%d:0] (%b) != %b\n", i-1+4,
          s.saaa.0[i-1+4:0], s.aaa[i-1+4:0] 
        ) ;
      }
      else {
      
      }
    }
  }
  
  printf( "s.aaaa = %b\n", s.aaaa ) ;

  s.saaaa.0 = s.aaaa ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaaa.0[i+6] != ((s.aaaa >> (120-i)) & 1) ) {
        error( 
          "Error: s.saaaa.0[%d] (%b) != %b\n", i+6,
          s.saaaa.0[i+6], ((s.aaaa >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaaa.0[i+6:j+6] != 
            ((s.aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[i+6:j+6], (s.aaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaaa.0 = s.aaaa ;
      @(posedge CLOCK) ;
      s.yaaaa = {random(), random(), random(), random(), random()} ;
      s.saaaa.0[i+6:j+6] = s.yaaaa[i+6:j+6] ;
      @(posedge CLOCK) ;
      if( s.saaaa.0[i+6:j+6] != 
            ((s.yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[i+6:j+6], (s.yaaaa >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saaaa.0[6:i-1+6] != s.aaaa[6:i-1+6] ) {
        error( 
          "Error: s.saaaa.0[6:%d] (%b) != %b\n", i-1+6,
          s.saaaa.0[6:i-1+6], s.aaaa[6:i-1+6] 
        ) ;
      }
      if( s.saaaa.0[j+1+6:126] != s.aaaa[j+1+6:126] ) {
        error( 
          "Error: s.saaaa.0[%d:126] (%b) != %b\n", i+1+6,
          s.saaaa.0[j+1+6:126], s.aaaa[j+1+6:126] 
        ) ;
      }
    }
  }
  
  printf( "Test done\n" ) ;
  
}
