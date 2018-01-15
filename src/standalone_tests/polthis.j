#define SHARED_EVENTS 100

class chip_base {
  event event_array[] ;
  
  void new() {
    int i ;
    for( i = 0 ; i < SHARED_EVENTS ; i++ ) event_array[i] = new ;
  }
}

class tb_base extends chip_base {

  void connect_events( chip_base a, chip_base b = null ) 
  {  
    int jj; 
    if (b==null) b = this;
    for (jj=0; jj<SHARED_EVENTS; jj++) { a.event_array[jj] = b.event_array[jj]; } 
  }
}


main() {
  tb_base a, b ;
  
  
  b = new ;
  a = new ;  
  b.connect_events( a ) ;

}

