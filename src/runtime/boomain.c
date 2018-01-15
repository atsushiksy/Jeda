extern void JEDA_load_dynamic_lib() ;

void io_printf() {
  printf() ;
}

int acc_initialize() {}
int tf_getp() {}
int acc_close() {}
int mc_scan_plusargs() {}
int tf_getcstringp() {}

main() {

  printf( "calling JEDA_load..\n" ) ;
  
  JEDA_load_dynamic_lib() ;
  
}

