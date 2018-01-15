#include "port.jh"

class ss {
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
  signal sa[*] ;
  signal saa[*] ;
  signal saaa[*] ;
  signal saaaa[*] ;
}

main() {
  int i, j ;
  ss s ;
  string str[8] ;
  
  s = new ;
  
  str[0] = "rotiry" ;
  str[1] = "etyete" ;
  str[2] = "tyer" ;
  str[3] = "tbbyr" ;
  str[4] = "tebet" ;
  str[5] = "tyr" ;
  str[6] = "ytyerty" ;
  str[7] = "eytyerty" ;
  
  // printf( "a str[0] = %s\n", str[0] ) ;
  
  //for( i = 0 ; i < 8 ; i++ ) 
  //  printf( "str[%d] = %s\n", i, str[i] ) ;

    
  for( i = 0 ; i < 8 ; i++ ) {
    printf( "%d %s\n", i, str[i] ) ;
    s.a[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aa[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aaa[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aaaa[str[i]] = {random(), random(), random(), random(), random()} ;
    s.sa[str[i]] = p.a ;
    s.saa[str[i]] = p.aa ;
    s.saaa[str[i]] = p.aaa ;
    s.saaaa[str[i]] = p.aaaa ;
  }
  s.b = {random(), random(), random(), random(), random()} ;
  s.c = {random(), random(), random(), random(), random()} ;
  s.bb = {random(), random(), random(), random(), random()} ;
  s.cc = {random(), random(), random(), random(), random()} ;
  s.bbb = {random(), random(), random(), random(), random()} ;
  s.ccc = {random(), random(), random(), random(), random()} ;
  s.bbbb = {random(), random(), random(), random(), random()} ;
  s.cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "s.a = %b  %s\n", s.a[str[5]], str[5] ) ;  
  
  s.sa.0[str[5]] = s.a[str[5]] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( s.sa.0[str[5]][i] != ((s.a[str[5]] >> i) & 1) ) {
        error( 
          "Error: s.sa.0[str[5]][%d] (%b) != %b\n", i,
          s.sa.0[str[5]][i], (s.a[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  s.sa.0[str[3]] = s.a[str[3]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.sa.0[str[3]][j:i] != 
            ((s.a[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.sa.0[str[3]][%d:%d] (%b) != %b\n", j, i,
          s.sa.0[str[3]][j:i], (s.a[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.sa.0[str[0]] = s.a[str[0]] ;
      @(posedge CLOCK) ;
      s.ya[str[0]] = {random(), random(), random(), random(), random()} ;
      s.sa.0[str[0]][j:i] = s.ya[str[0]][j:i] ;
      @(posedge CLOCK) ;
      if( s.sa.0[str[0]][j:i] != 
            ((s.ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.sa[str[0]][%d:%d] (%b) != %b\n", j, i,
          s.sa.0[str[0]][j:i], (s.ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.sa.0[str[0]][120:j+1] != s.a[str[0]][120:j+1] ) {
        error( 
          "Error: s.sa[str[0]][120:%d] (%b) != %b\n", j+1,
          s.sa.0[str[0]][120:j+1], s.a[str[0]][120:j+1] 
        ) ;
      }
      if( i && s.sa.0[str[0]][i-1:0] != s.a[str[0]][i-1:0] ) {
        error( 
          "Error: s.sa[str[0]][%d:0] (%b) != %b\n", i-1,
          s.sa.0[str[0]][i-1:0], s.a[str[0]][i-1:0] 
        ) ;
      }
    }
  }
  
  
  s.saa.0[str[2]] = s.aa[str[2]] ;
  @(posedge CLOCK) ;
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saa.0[str[2]][i] != ((s.aa[str[2]] >> (120-i)) & 1) ) {
        error( 
          "Error: s.saa.0[str[2]][%d] (%b) != %b\n", i,
          s.saa.0[str[2]][i], ((s.aa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  s.saa.0[str[7]] = s.aa[str[7]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saa.0[str[7]][i:j] != 
            ((s.aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saa.0[str[7]][%d:%d] (%b) != %b\n", j, i,
          s.saa.0[str[7]][i:j], 
          (s.aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saa.0[str[6]] = s.aa[str[6]] ;
      @(posedge CLOCK) ;
      s.yaa[str[6]] = {random(), random(), random(), random(), random()} ;
      s.saa.0[str[6]][i:j] = s.yaa[str[6]][i:j] ;
      @(posedge CLOCK) ;
      if( s.saa.0[str[6]][i:j] !=
            ((s.yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saa.0[str[6]][%d:%d] (%b) != %b\n", j, i,
          s.saa.0[str[6]][i:j], 
          (s.yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saa.0[str[6]][0:i-1] != s.aa[str[6]][0:i-1] ) {
        error( 
          "Error: s.saa.0[str[6]][0:%d] (%b) != %b\n", i-1,
          s.saa.0[str[6]][0:i-1], s.aa[str[6]][0:i-1] 
        ) ;
      }
      if( s.saa.0[str[6]][j+1:120] != s.aa[str[6]][j+1:120] ) {
        error( 
          "Error: s.saa.0[str[6]][%d:120] (%b) != %b\n", i-1,
          s.saa.0[str[6]][j+1:120], s.aa[str[6]][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "s.aaa[str[5]] = %b\n", s.aaa[str[5]] ) ;

  s.saaa.0[str[5]] = s.aaa[str[5]] ;
  @(posedge CLOCK) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaa.0[str[5]][i+4] != ((s.aaa[str[5]] >> i) & 1) ) {
        error( 
          "Error: s.saaa.0[str[5]][%d] (%b) != %b\n", i+4,
          s.saaa.0[str[5]][i+4], (s.aaa[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  s.saaa.0[str[4]] = s.aaa[str[4]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaa.0[str[4]][j+4:i+4] != 
            ((s.aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaa.0[str[4]][%d:%d] (%b) != %b\n", j+4, i+4,
          s.saaa.0[str[4]][j+4:i+4], 
          (s.aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaa.0[str[3]] = s.aaa[str[3]] ;
      @(posedge CLOCK) ;
      s.yaaa[str[3]] = {random(), random(), random(), random(), random()} ;
      s.saaa.0[str[3]][j+4:i+4] = s.yaaa[str[3]][j+4:i+4] ;
      @(posedge CLOCK) ;
      if( s.saaa.0[str[3]][j+4:i+4] != 
            ((s.yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) )
      {
        error( 
          "Error: s.saaa.0[str[3]][%d:%d] (%b) != %b\n", j, i,
          s.saaa.0[str[3]][j+4:i+4], 
          (s.yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.saaa.0[str[3]][124:j+1+4] != s.aaa[str[3]][124:j+1+4] ) {
        error( 
          "Error: s.saaa.0[str[3]][124:%d] (%b) != %b\n", j+1+4,
          s.saaa.0[str[3]][124:j+1+4], s.aaa[str[3]][124:j+1+4] 
        ) ;
      }
      if( i && s.saaa.0[str[3]][i-1+4:4] != s.aaa[str[3]][i-1+4:4] ) {
        error( 
          "Error: s.saaa[str[3]][%d:0] (%b) != %b\n", i-1+4,
          s.saaa.0[str[3]][i-1+4:0], s.aaa[str[3]][i-1+4:0] 
        ) ;
      }
    }
  }
  
  s.saaaa.0[str[2]] = s.aaaa[str[2]] ;
  @(posedge CLOCK) ;


  for( i = 0 ; i < 120 ; i++ ) {
      if( s.saaaa.0[str[2]][i+6] != ((s.aaaa[str[2]] >> (120-i)) & 1) ) {
        printf( 
          "Error: s.saaaa.0[str[2]][%d] (%b) != %b\n", i+6,
          s.saaaa.0[str[2]][i+6], ((s.aaaa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  s.saaaa.0[str[1]] = s.aaaa[str[1]] ;
  @(posedge CLOCK) ;

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.saaaa.0[str[1]][i+6:j+6] != 
            ((s.aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[str[1]][%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[str[1]][i+6:j+6], 
          (s.aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.saaaa.0[str[0]] = s.aaaa[str[0]] ;
      @(posedge CLOCK) ;
      s.yaaaa[str[0]] = {random(), random(), random(), random(), random()} ;
      s.saaaa.0[str[0]][i+6:j+6] = s.yaaaa[str[0]][i+6:j+6] ;
      @(posedge CLOCK) ;
      if( s.saaaa.0[str[0]][i+6:j+6] != 
            ((s.yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: s.saaaa.0[str[0]][%d:%d] (%b) != %b\n", j+6, i+6,
          s.saaaa.0[str[0]][i+6:j+6], 
          (s.yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.saaaa.0[str[0]][6:i-1+6] != s.aaaa[str[0]][6:i-1+6] ) {
        error( 
          "Error: s.saaaa.0[str[0]][6:%d] (%b) != %b\n", i-1+6,
          s.saaaa.0[str[0]][6:i-1+6], s.aaaa[str[0]][6:i-1+6] 
        ) ;
      }
      if( s.saaaa.0[str[0]][j+1+6:126] != s.aaaa[str[0]][j+1+6:126] ) {
        error( 
          "Error: s.saaaa.0[str[0]][%d:126] (%b) != %b\n", i+1+6,
          s.saaaa.0[str[0]][j+1+6:126], s.aaaa[str[0]][j+1+6:126] 
        ) ;
      }
    }
  }
  printf( "Test done!\n" ) ;
  
}
