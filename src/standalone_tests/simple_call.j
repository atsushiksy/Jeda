
void foo( int i ) {
  printf( "i = %d\n", i ) ;
}

void bar( int x , int y = 0 ) {
  x = 0 ;
}

void main ( )
{
 bit    [9:0] format;
 int    i;

  i = 0 ;
  
  format = i ;
  
  foo( i ) ;
  
  bar( i ) ;
  
}

