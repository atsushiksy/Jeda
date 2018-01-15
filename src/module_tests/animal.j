
class Animal {
  protected  int  aa;
  bit a;
  static bit [31:0] my_static;
  bit [5:0] bus;

  initial {
    my_static = 55 ;
  }
  
  new() {
    a=0;
    my_static=10;
    bus = 6'h0;
  }
  
  move() {
    a = 0 ;    
    breakpoint ;
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
