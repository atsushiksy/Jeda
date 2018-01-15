#include "port.jh"
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
  int i, j ;
  string str[8] ;
  signal sa[*] ;
  signal saa[*] ;
  signal saaa[*] ;
  signal saaaa[*] ;

main() {
  
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
    a[str[i]] = {random(), random(), random(), random(), random()} ;
    aa[str[i]] = {random(), random(), random(), random(), random()} ;
    aaa[str[i]] = {random(), random(), random(), random(), random()} ;
    aaaa[str[i]] = {random(), random(), random(), random(), random()} ;
    sa[str[i]] = p.a ;
    saa[str[i]] = p.aa ;
    saaa[str[i]] = p.aaa ;
    saaaa[str[i]] = p.aaaa ;
  }
  b = {random(), random(), random(), random(), random()} ;
  c = {random(), random(), random(), random(), random()} ;
  bb = {random(), random(), random(), random(), random()} ;
  cc = {random(), random(), random(), random(), random()} ;
  bbb = {random(), random(), random(), random(), random()} ;
  ccc = {random(), random(), random(), random(), random()} ;
  bbbb = {random(), random(), random(), random(), random()} ;
  cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b  %s\n", a[str[5]], str[5] ) ;  
  
  sa.0[str[5]] = a[str[5]] ;
  
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( sa.0[str[5]][i] != ((a[str[5]] >> i) & 1) ) {
        error( 
          "Error: sa.0[str[5]][%d] (%b) != %b\n", i,
          sa.0[str[5]][i], (a[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  sa.0[str[3]] = a[str[3]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( sa.0[str[3]][j:i] != ((a[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: sa.0[str[3]][%d:%d] (%b) != %b\n", j, i,
          sa.0[str[3]][j:i], (a[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      sa.0[str[0]] = a[str[0]] ;
      @(posedge CLOCK) ;
      ya[str[0]] = {random(), random(), random(), random(), random()} ;
      sa.0[str[0]][j:i] = ya[str[0]][j:i] ;
      @(posedge CLOCK) ;
      if( sa.0[str[0]][j:i] != ((ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[str[0]][%d:%d] (%b) != %b\n", j, i,
          sa.0[str[0]][j:i], (ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( sa.0[str[0]][120:j+1] != a[str[0]][120:j+1] ) {
        error( 
          "Error: xa[str[0]][120:%d] (%b) != %b\n", j+1,
          sa.0[str[0]][120:j+1], a[str[0]][120:j+1] 
        ) ;
      }
      if( i && sa.0[str[0]][i-1:0] != a[str[0]][i-1:0] ) {
        error( 
          "Error: xa[str[0]][%d:0] (%b) != %b\n", i-1,
          sa.0[str[0]][i-1:0], a[str[0]][i-1:0] 
        ) ;
      }
    }
  }
  
  
  saa.0[str[2]] = aa[str[2]] ;
  @(posedge CLOCK) ;
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( saa.0[str[2]][i] != ((aa[str[2]] >> (120-i)) & 1) ) {
        error( 
          "Error: saa.0[str[2]][%d] (%b) != %b\n", i,
          saa.0[str[2]][i], ((aa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  saa.0[str[7]] = aa[str[7]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saa.0[str[7]][i:j] != 
            ((aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saa.0[str[7]][%d:%d] (%b) != %b\n", j, i,
          saa.0[str[7]][i:j], (aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saa.0[str[6]] = aa[str[6]] ;
      @(posedge CLOCK) ;
      yaa[str[6]] = {random(), random(), random(), random(), random()} ;
      saa.0[str[6]][i:j] = yaa[str[6]][i:j] ;
      @(posedge CLOCK) ;
      if( saa.0[str[6]][i:j] !=
            ((yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saa.0[str[6]][%d:%d] (%b) != %b\n", j, i,
          saa.0[str[6]][i:j], 
          (yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saa.0[str[6]][0:i-1] != aa[str[6]][0:i-1] ) {
        error( 
          "Error: saa.0[str[6]][0:%d] (%b) != %b\n", i-1,
          saa.0[str[6]][0:i-1], aa[str[6]][0:i-1] 
        ) ;
      }
      if( saa.0[str[6]][j+1:120] != aa[str[6]][j+1:120] ) {
        error( 
          "Error: saa.0[str[6]][%d:120] (%b) != %b\n", i-1,
          saa.0[str[6]][j+1:120], aa[str[6]][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa[str[5]] = %b\n", aaa[str[5]] ) ;

  saaa.0[str[5]] = aaa[str[5]] ;
  @(posedge CLOCK) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( saaa.0[str[5]][i+4] != ((aaa[str[5]] >> i) & 1) ) {
        error( 
          "Error: saaa.0[str[5]][%d] (%b) != %b\n", i+4,
          saaa.0[str[5]][i+4], (aaa[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  saaa.0[str[4]] = aaa[str[4]] ;
  @(posedge CLOCK) ;
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaa.0[str[4]][j+4:i+4] != 
            ((aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saaa.0[str[4]][%d:%d] (%b) != %b\n", j+4, i+4,
          saaa.0[str[4]][j+4:i+4], (aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaa.0[str[3]] = aaa[str[3]] ;
      @(posedge CLOCK) ;
      yaaa[str[3]] = {random(), random(), random(), random(), random()} ;
      saaa.0[str[3]][j+4:i+4] = yaaa[str[3]][j+4:i+4] ;
      @(posedge CLOCK) ;
      if( saaa.0[str[3]][j+4:i+4] != 
            ((yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) )
      {
        error( 
          "Error: saaa.0[str[3]][%d:%d] (%b) != %b\n", j, i,
          saaa.0[str[3]][j+4:i+4], (yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( saaa.0[str[3]][124:j+1+4] != aaa[str[3]][124:j+1+4] ) {
        error( 
          "Error: saaa.0[str[3]][124:%d] (%b) != %b\n", j+1+4,
          saaa.0[str[3]][124:j+1+4], aaa[str[3]][124:j+1+4] 
        ) ;
      }
      if( i && saaa.0[str[3]][i-1+4:4] != aaa[str[3]][i-1+4:4] ) {
        error( 
          "Error: xaaa[str[3]][%d:0] (%b) != %b\n", i-1+4,
          saaa.0[str[3]][i-1+4:0], aaa[str[3]][i-1+4:0] 
        ) ;
      }
    }
  }
  
  saaaa.0[str[2]] = aaaa[str[2]] ;
  @(posedge CLOCK) ;


  for( i = 0 ; i < 120 ; i++ ) {
      if( saaaa.0[str[2]][i+6] != ((aaaa[str[2]] >> (120-i)) & 1) ) {
        printf( 
          "Error: saaaa.0[str[2]][%d] (%b) != %b\n", i+6,
          saaaa.0[str[2]][i+6], ((aaaa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  saaaa.0[str[1]] = aaaa[str[1]] ;
  @(posedge CLOCK) ;

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( saaaa.0[str[1]][i+6:j+6] != 
            ((aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saaaa.0[str[1]][%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[str[1]][i+6:j+6], 
          (aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      saaaa.0[str[0]] = aaaa[str[0]] ;
      @(posedge CLOCK) ;
      yaaaa[str[0]] = {random(), random(), random(), random(), random()} ;
      saaaa.0[str[0]][i+6:j+6] = yaaaa[str[0]][i+6:j+6] ;
      @(posedge CLOCK) ;
      if( saaaa.0[str[0]][i+6:j+6] != 
            ((yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) 
      {
        error( 
          "Error: saaaa.0[str[0]][%d:%d] (%b) != %b\n", j+6, i+6,
          saaaa.0[str[0]][i+6:j+6], 
          (yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && saaaa.0[str[0]][6:i-1+6] != aaaa[str[0]][6:i-1+6] ) {
        error( 
          "Error: saaaa.0[str[0]][6:%d] (%b) != %b\n", i-1+6,
          saaaa.0[str[0]][6:i-1+6], aaaa[str[0]][6:i-1+6] 
        ) ;
      }
      if( saaaa.0[str[0]][j+1+6:126] != aaaa[str[0]][j+1+6:126] ) {
        error( 
          "Error: saaaa.0[str[0]][%d:126] (%b) != %b\n", i+1+6,
          saaaa.0[str[0]][j+1+6:126], aaaa[str[0]][j+1+6:126] 
        ) ;
      }
    }
  }
  printf( "Test done!\n" ) ;
  
}
