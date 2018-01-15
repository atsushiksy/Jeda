
#define PRODUCT_CODE

#include "license.h"

main() {
  int i, r, k ;
  
  for( i = 0 ; i < 100 ; i++ ) {  
    r = (int)time() ;
    k = check_dsa_key( r, 0 ) ;
    printf( "r: %d K: %d check: %d \n", r, k, check_dsa_key( r, k ) ) ;
    sleep(1) ;
  }
  
}
