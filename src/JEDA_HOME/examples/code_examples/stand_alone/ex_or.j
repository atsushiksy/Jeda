

main()
{
  bit [2:0] a;
  bit [2:0] b;

  a = 3'b101;
  b = 3'b111;

  printf("-- result --\n");
  printf("%b\n", a^b);
  printf("%b\n", ^a);
  printf("%b\n", ^b);

}


