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


test_main() {
  int i, j ;
  string str[8] ;
  
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
    a[str[i]] = {random(), random(), random(), random(), random()} ;
    aa[str[i]] = {random(), random(), random(), random(), random()} ;
    aaa[str[i]] = {random(), random(), random(), random(), random()} ;
    aaaa[str[i]] = {random(), random(), random(), random(), random()} ;
  }
  b = {random(), random(), random(), random(), random()} ;
  c = {random(), random(), random(), random(), random()} ;
  bb = {random(), random(), random(), random(), random()} ;
  cc = {random(), random(), random(), random(), random()} ;
  bbb = {random(), random(), random(), random(), random()} ;
  ccc = {random(), random(), random(), random(), random()} ;
  bbbb = {random(), random(), random(), random(), random()} ;
  cccc = {random(), random(), random(), random(), random()} ;
  
  printf( "a = %b\n", a[str[5]] ) ;  
  
  for( i = 0 ; i < 120 ; i+=10 ) {
      if( a[str[5]][i] != ((a[str[5]] >> i) & 1) ) {
        error( 
          "Error: a[str[5]][%d] (%b) != %b\n", i,
          a[str[5]][i], (a[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( a[str[3]][j:i] != ((a[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: a[str[3]][%d:%d] (%b) != %b\n", j, i,
          a[str[3]][j:i], (a[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xa[str[0]] = a[str[0]] ;
      ya[str[0]] = {random(), random(), random(), random(), random()} ;
      xa[str[0]][j:i] = ya[str[0]][j:i] ;
      if( xa[str[0]][j:i] != ((ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xa[str[0]][%d:%d] (%b) != %b\n", j, i,
          xa[str[0]][j:i], (ya[str[0]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xa[str[0]][120:j+1] != a[str[0]][120:j+1] ) {
        error( 
          "Error: xa[str[0]][120:%d] (%b) != %b\n", j+1,
          xa[str[0]][120:j+1], a[str[0]][120:j+1] 
        ) ;
      }
      if( i && xa[str[0]][i-1:0] != a[str[0]][i-1:0] ) {
        error( 
          "Error: xa[str[0]][%d:0] (%b) != %b\n", i-1,
          xa[str[0]][i-1:0], a[str[0]][i-1:0] 
        ) ;
      }
    }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
      if( aa[str[2]][i] != ((aa[str[2]] >> (120-i)) & 1) ) {
        error( 
          "Error: aa[str[2]][%d] (%b) != %b\n", i,
          aa[str[2]][i], ((aa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aa[str[7]][i:j] != ((aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aa[str[7]][%d:%d] (%b) != %b\n", j, i,
          aa[str[7]][i:j], (aa[str[7]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
    
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaa[str[6]] = aa[str[6]] ;
      yaa[str[6]] = {random(), random(), random(), random(), random()} ;
      xaa[str[6]][i:j] = yaa[str[6]][i:j] ;
      if( xaa[str[6]][i:j] != ((yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaa[str[6]][%d:%d] (%b) != %b\n", j, i,
          xaa[str[6]][i:j], (yaa[str[6]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaa[str[6]][0:i-1] != aa[str[6]][0:i-1] ) {
        error( 
          "Error: xaa[str[6]][0:%d] (%b) != %b\n", i-1,
          xaa[str[6]][0:i-1], aa[str[6]][0:i-1] 
        ) ;
      }
      if( xaa[str[6]][j+1:120] != aa[str[6]][j+1:120] ) {
        error( 
          "Error: xaa[str[6]][%d:120] (%b) != %b\n", i-1,
          xaa[str[6]][j+1:120], aa[str[6]][j+1:120] 
        ) ;
      }
    }
  }
  
  

  
  printf( "aaa[str[5]] = %b\n", aaa[str[5]] ) ;

  for( i = 0 ; i < 120 ; i++ ) {
      if( aaa[str[5]][i+4] != ((aaa[str[5]] >> i) & 1) ) {
        error( 
          "Error: aaa[str[5]][%d] (%b) != %b\n", i+4,
          aaa[str[5]][i+4], (aaa[str[5]] >> i) & 1 
        ) ;
      }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaa[str[4]][j+4:i+4] != ((aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaa[str[4]][%d:%d] (%b) != %b\n", j+4, i+4,
          aaa[str[4]][j+4:i+4], (aaa[str[4]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaa[str[3]] = aaa[str[3]] ;
      yaaa[str[3]] = {random(), random(), random(), random(), random()} ;
      xaaa[str[3]][j+4:i+4] = yaaa[str[3]][j+4:i+4] ;
      if( xaaa[str[3]][j+4:i+4] != ((yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaa[str[3]][%d:%d] (%b) != %b\n", j, i,
          xaaa[str[3]][j+4:i+4], (yaaa[str[3]] >> i) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( xaaa[str[3]][124:j+1+4] != aaa[str[3]][124:j+1+4] ) {
        error( 
          "Error: xaaa[str[3]][124:%d] (%b) != %b\n", j+1+4,
          xaaa[str[3]][124:j+1+4], aaa[str[3]][124:j+1+4] 
        ) ;
      }
      if( i && xaaa[str[3]][i-1+4:4] != aaa[str[3]][i-1+4:4] ) {
        error( 
          "Error: xaaa[str[3]][%d:0] (%b) != %b\n", i-1+4,
          xaaa[str[3]][i-1+4:0], aaa[str[3]][i-1+4:0] 
        ) ;
      }
    }
  }
  


  for( i = 0 ; i < 120 ; i++ ) {
      if( aaaa[str[2]][i+6] != ((aaaa[str[2]] >> (120-i)) & 1) ) {
        printf( 
          "Error: aaaa[str[2]][%d] (%b) != %b\n", i+6,
          aaaa[str[2]][i+6], ((aaaa[str[2]] >> (120-i)) & 1) 
        ) ;
      }
  }
  

  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      if( aaaa[str[1]][i+6:j+6] != ((aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: aaaa[str[1]][%d:%d] (%b) != %b\n", j+6, i+6,
          aaaa[str[1]][i+6:j+6], (aaaa[str[1]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
    }
  }
  
  for( i = 0 ; i < 120 ; i++ ) {
    for( j = i ; j < 120 ; j++ ) {
      xaaaa[str[0]] = aaaa[str[0]] ;
      yaaaa[str[0]] = {random(), random(), random(), random(), random()} ;
      xaaaa[str[0]][i+6:j+6] = yaaaa[str[0]][i+6:j+6] ;
      if( xaaaa[str[0]][i+6:j+6] != ((yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1)) ) {
        error( 
          "Error: xaaaa[str[0]][%d:%d] (%b) != %b\n", j+6, i+6,
          xaaaa[str[0]][i+6:j+6], (yaaaa[str[0]] >> (120-j)) & ((121'b1 << (j-i+1))-1) 
        ) ;
      }
      if( i && xaaaa[str[0]][6:i-1+6] != aaaa[str[0]][6:i-1+6] ) {
        error( 
          "Error: xaaaa[str[0]][6:%d] (%b) != %b\n", i-1+6,
          xaaaa[str[0]][6:i-1+6], aaaa[str[0]][6:i-1+6] 
        ) ;
      }
      if( xaaaa[str[0]][j+1+6:126] != aaaa[str[0]][j+1+6:126] ) {
        error( 
          "Error: xaaaa[str[0]][%d:126] (%b) != %b\n", i+1+6,
          xaaaa[str[0]][j+1+6:126], aaaa[str[0]][j+1+6:126] 
        ) ;
      }
    }
  }
  
}

}

main() {
  ss s ;
  
  s = new ;
  
  s.test_main() ;

}
