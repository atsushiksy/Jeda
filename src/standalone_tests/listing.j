class foo {
  int i ;
}

class bar {
  int i ;
}

main() {
  foo foo1, foo2, foo3 ;
  bar bar1, bar2, bar3 ;
  foo foolist<> ;
  string stringlist<> ;
  
  foolist = '{foo1,foo2,foo3} ;

  stringlist = '{"foo", "foo", "foo"} ;
  
}
