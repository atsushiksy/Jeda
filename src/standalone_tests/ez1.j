void main()
{
   bit [7:0] a , b;
   bit [8:0] c;
 
   a = 8'h1;
   b = 8'hf;
   c = a+b;
   printf( "a=%d\n", a );
   printf( "b=%d\n", b );
   printf( "a+b=%d\n", a+b );
   printf( "c=%d\n", c );
 
   if(16==16) printf("correct\n");
 
   if(c==16) printf("correct\n");
   else printf("wrong\n");
 
}
