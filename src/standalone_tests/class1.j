// we can do a list !!
typedef class Animal, Bear;

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

// base class
class Animal {
  protected  int  aa;
  bit a;
  static bit [31:0] my_static;
  bit [5:0] bus;

  new() {
    a=0;
    my_static=10;
    bus = 6'h0;
  }
  
  move() {
    a = 0 ;
  }
  
  boo() {
    a = 1 ;
  }
  
}

// child class
class Bear extends Animal {
  bit b;

  void move () {
     if(a) b = a;
     my_static += 2;
  }
  new () {
    b=0;
  }

  void do_protected(bit [5:0] val) {
     aa+=10;
  }
  
  void foo() {
    move() ;
    super.move() ;
    boo() ;
  }

}

void test()
{
  Animal ani;
  Bear   bear;
  Animal aannii ;

  ani = new;
  bear = new;
  
  aannii = bear ;
  
  printf( "ani.a = %d\n", ani.a ) ;

  if(ani.a)
        ani.bus++;
//      ani.a=1'b1;
  
      my_prin(ani);

  printf( "bear.a = %b \n", bear.a ) ;
  if(bear.a==1'b0) 
    my_prin(bear);  
    
  bear.move() ;
  
}


void main()
{
  test();
}
