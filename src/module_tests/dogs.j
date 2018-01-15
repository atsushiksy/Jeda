#include "animal.jh"

class Dog extends Animal {
  bit b;
  static bit [31:0] dog_static;

  initial {
    dog_static = 88 ;
  }
  
  void move () {
     if(a) b = a;
     my_static += 2;
         
    breakpoint ;
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
