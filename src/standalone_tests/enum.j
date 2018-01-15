
enum Type = boo, foo, woo ;

void test_enum()
{
 
   enum Type = UNI, MUL, BRD;
   int  j;
   Type my_type;
 
 
   {
     enum Type = III, JJJ, KKK;
     // enum iType = UNI, MUL, BRD;
     // enum Type = boo, foo, woo ;
     enum sType = boo, foo, woo ;
     sType stype ;
     Type  iType ;
     
     iType = III ;
     stype = boo ;
     
   }
   j=1;
   my_type = MUL;
 
}


main() {
  test_enum() ;

}
