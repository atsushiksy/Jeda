class foo {
  static int id = 0 ;
  bit [7:0] boo ;
  int xx[5] ;
  int a ;
  int b ;
  int c ;
  int d ;
  int e ;
  int f ;
  int g ;
  int h ;
  int i ;
  int j ;
  int k ;
  int l ;
  int m ;
  int n ;
  int o ;
  int p ;
  int q ;
  int r ;
  int s ;
  int t ;
  int u ;
  int v ;
  int w ;
  int x ;
  int y ;
  int z ;
  int a0 ;
  int b0 ;
  int c0 ;
  int d0 ;
  int e0 ;
  int f0 ;
  int g0 ;
  int h0 ;
  int i0 ;
  int j0 ;
  int k0 ;
  int l0 ;
  int m0 ;
  int n0 ;
  int o0 ;
  int p0 ;
  int q0 ;
  int r0 ;
  int s0 ;
  int t0 ;
  int u0 ;
  int v0 ;
  int w0 ;
  int x0 ;
  int y0 ;
  int z0 ;
  
  
  void new() {
    i = id++ ;
    j = 5 ;
    boo = 0xff ;
  }
  
  void bar( int xxx ) {
    printf( "i = %d\n", i ) ;
    printf( "j = %d\n", j ) ;
    printf( "boo = %b\n", boo ) ;
    i += xxx ;
    j += xxx ;
    boo += xxx ;
    printf( " i = %d\n", i ) ;
    printf( " j = %d\n", j ) ;
    printf( " boo = %b\n", boo ) ;
  }
  
}

main() {
  foo fi, fj ;
  foo fa[100] ;
  int i ;
  
  printf( "class_tbl_ext testing ..\n" ) ;
  fi = new ;
  
  for( i = 0 ; i < 100 ; i++ ) 
    fa[i] = new fi ;
  
  printf( "done!\n" ) ;
    
}
