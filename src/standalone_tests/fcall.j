
void test ()
{
   int a =50;
   bit [55:0] x ;
  printf( "a = %d\n", a );
  printf( "a = %b\n", a );
  printf( "a = %x\n", a );
  printf( "a = %h\n", a );
  
  x = a ;
  x = 56'hff00;
  printf( "x = %d\n", x );
  printf( "x = %b\n", x );
  printf( "x = %x\n", x );
  printf( "x = %h\n", x );
}

void main()
{

  printf("test\n");

  test();
}
