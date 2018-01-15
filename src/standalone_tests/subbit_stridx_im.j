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
    s.a[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aa[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aaa[str[i]] = {random(), random(), random(), random(), random()} ;
    s.aaaa[str[i]] = {random(), random(), random(), random(), random()} ;
  }
  s.b = {random(), random(), random(), random(), random()} ;
  s.c = {random(), random(), random(), random(), random()} ;
  s.bb = {random(), random(), random(), random(), random()} ;
  s.cc = {random(), random(), random(), random(), random()} ;
  s.bbb = {random(), random(), random(), random(), random()} ;
  s.ccc = {random(), random(), random(), random(), random()} ;
  s.bbbb = {random(), random(), random(), random(), random()} ;
  s.cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "s.a = %b\n", s.a[str[5]] ) ;  
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( s.a[str[5]][i] != ((s.a[str[5]] >> i) & 1) ) {
        error( 
          "Error: s.a[str[5]][%d] (%b) != %b\n", i,
          s.a[str[5]][i], (s.a[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.a[str[3]][j:i] != ((s.a[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.a[str[3]][%d:%d] (%b) != %b\n", j, i,
          s.a[str[3]][j:i], (s.a[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xa[str[0]] = s.a[str[0]] ;
      s.ya[str[0]] = {random(), random(), random(), random(), random()} ;
      s.xa[str[0]][j:i] = s.ya[str[0]][j:i] ;
      if( s.xa[str[0]][j:i] != ((s.ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xa[str[0]][%d:%d] (%b) != %b\n", j, i,
          s.xa[str[0]][j:i], (s.ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.xa[str[0]][120:j+1] != s.a[str[0]][120:j+1] ) {
        error( 
          "Error: s.xa[str[0]][120:%d] (%b) != %b\n", j+1,
          s.xa[str[0]][120:j+1], s.a[str[0]][120:j+1] 
        ) ;
      }
      if( i && s.xa[str[0]][i-1:0] != s.a[str[0]][i-1:0] ) {
        error( 
          "Error: s.xa[str[0]][%d:0] (%b) != %b\n", i-1,
          s.xa[str[0]][i-1:0], s.a[str[0]][i-1:0] 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aa[str[2]][i] != ((s.aa[str[2]] >> (120-i)) & 1) ) {
        error( 
          "Error: s.aa[str[2]][%d] (%b) != %b\n", i,
          s.aa[str[2]][i], ((s.aa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aa[str[7]][i:j] != ((s.aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aa[str[7]][%d:%d] (%b) != %b\n", j, i,
          s.aa[str[7]][i:j], (s.aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaa[str[6]] = s.aa[str[6]] ;
      s.yaa[str[6]] = {random(), random(), random(), random(), random()} ;
      s.xaa[str[6]][i:j] = s.yaa[str[6]][i:j] ;
      if( s.xaa[str[6]][i:j] != ((s.yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaa[str[6]][%d:%d] (%b) != %b\n", j, i,
          s.xaa[str[6]][i:j], (s.yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.xaa[str[6]][0:i-1] != s.aa[str[6]][0:i-1] ) {
        error( 
          "Error: s.xaa[str[6]][0:%d] (%b) != %b\n", i-1,
          s.xaa[str[6]][0:i-1], s.aa[str[6]][0:i-1] 
        ) ;
      }
      if( s.xaa[str[6]][j+1:120] != s.aa[str[6]][j+1:120] ) {
        error( 
          "Error: s.xaa[str[6]][%d:120] (%b) != %b\n", i-1,
          s.xaa[str[6]][j+1:120], s.aa[str[6]][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "s.aaa[str[5]] = %b\n", s.aaa[str[5]] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaa[str[5]][i+4] != ((s.aaa[str[5]] >> i) & 1) ) {
        error( 
          "Error: s.aaa[str[5]][%d] (%b) != %b\n", i+4,
          s.aaa[str[5]][i+4], (s.aaa[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaa[str[4]][j+4:i+4] != ((s.aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaa[str[4]][%d:%d] (%b) != %b\n", j+4, i+4,
          s.aaa[str[4]][j+4:i+4], (s.aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaa[str[3]] = s.aaa[str[3]] ;
      s.yaaa[str[3]] = {random(), random(), random(), random(), random()} ;
      s.xaaa[str[3]][j+4:i+4] = s.yaaa[str[3]][j+4:i+4] ;
      if( s.xaaa[str[3]][j+4:i+4] != ((s.yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaa[str[3]][%d:%d] (%b) != %b\n", j, i,
          s.xaaa[str[3]][j+4:i+4], (s.yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( s.xaaa[str[3]][124:j+1+4] != s.aaa[str[3]][124:j+1+4] ) {
        error( 
          "Error: s.xaaa[str[3]][124:%d] (%b) != %b\n", j+1+4,
          s.xaaa[str[3]][124:j+1+4], s.aaa[str[3]][124:j+1+4] 
        ) ;
      }
      if( i && s.xaaa[str[3]][i-1+4:4] != s.aaa[str[3]][i-1+4:4] ) {
        error( 
          "Error: s.xaaa[str[3]][%d:0] (%b) != %b\n", i-1+4,
          s.xaaa[str[3]][i-1+4:0], s.aaa[str[3]][i-1+4:0] 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( s.aaaa[str[2]][i+6] != ((s.aaaa[str[2]] >> (120-i)) & 1) ) {
        printf( 
          "Error: s.aaaa[str[2]][%d] (%b) != %b\n", i+6,
          s.aaaa[str[2]][i+6], ((s.aaaa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( s.aaaa[str[1]][i+6:j+6] != ((s.aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.aaaa[str[1]][%d:%d] (%b) != %b\n", j+6, i+6,
          s.aaaa[str[1]][i+6:j+6], (s.aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      s.xaaaa[str[0]] = s.aaaa[str[0]] ;
      s.yaaaa[str[0]] = {random(), random(), random(), random(), random()} ;
      s.xaaaa[str[0]][i+6:j+6] = s.yaaaa[str[0]][i+6:j+6] ;
      if( s.xaaaa[str[0]][i+6:j+6] != ((s.yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: s.xaaaa[str[0]][%d:%d] (%b) != %b\n", j+6, i+6,
          s.xaaaa[str[0]][i+6:j+6], (s.yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && s.xaaaa[str[0]][6:i-1+6] != s.aaaa[str[0]][6:i-1+6] ) {
        error( 
          "Error: s.xaaaa[str[0]][6:%d] (%b) != %b\n", i-1+6,
          s.xaaaa[str[0]][6:i-1+6], s.aaaa[str[0]][6:i-1+6] 
        ) ;
      }
      if( s.xaaaa[str[0]][j+1+6:126] != s.aaaa[str[0]][j+1+6:126] ) {
        error( 
          "Error: s.xaaaa[str[0]][%d:126] (%b) != %b\n", i+1+6,
          s.xaaaa[str[0]][j+1+6:126], s.aaaa[str[0]][j+1+6:126] 
        ) ;
      }
    }
  }
  
}
