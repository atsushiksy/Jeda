void main ()
{
  bit [33:0] data ;
  bit [66:0] data1 ;
  

    data = 33'bx ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
    
    data = 33'hx5 ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
        
    data = 'bx11 ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
    
    data = 'hxa ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
    
    data = 33'bz ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
    
    data = 33'hz5 ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
        
    data = 'bz11 ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
    
    data = 'hza ;
    data1 = data ;
    
    printf( "data:%b\n", data ) ;
    printf( "data1:%b\n", data1 ) ;
}
