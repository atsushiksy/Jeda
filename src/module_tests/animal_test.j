#include "animal.jh"
#include "dogs.jh"

local   int my_local=0;
global  bit [31:0] my_glb=0;
bit [31:0] my_glb1;

bit my_prin(Animal ani) {
      
  // try to reach protected, should not work
//  if(a.aa)

  ani.a++;
  ani.a=1'b0;
  return 0;
}

void test()
{
  Animal ani;
  Bear   bear;
  Animal aannii ;
  Dog   dog ;
  
  ani = new;
  bear = new;
  dog = new ;
  aannii = bear ;
  
  printf( "ani.a = %d\n", ani.a ) ;

  if(ani.a)
        ani.bus++;
//      ani.a=1'b1;
  
      my_prin(ani);

  printf( "bear.a = %b \n", bear.a ) ;
  if(bear.a==1'b0) 
    my_prin(bear);  
    
  breakpoint ;
  
  bear.move() ;

  printf( 
    "my_local %d my_glb %d ani.my_static %d dog.dog_static %d\n", 
    my_local, my_glb, ani.my_static, dog.dog_static
  ) ;
  
  my_local = 99 ;
  my_glb = 123 ;
  ani.my_static += 256 ;
  
  dog.dog_static += 5 ;
  printf( 
    "my_local %d my_glb %d ani.my_static %d dog.dog_static %d\n", 
    my_local, my_glb, ani.my_static, dog.dog_static
  ) ;
  
  breakpoint ;
  
  dog.move() ;
  
}


void main()
{
  test();
}
