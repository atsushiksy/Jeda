/* 
   Copyright (C) 2003 Jeda Technologies, Inc.
*/
#ifdef PRODUCT_CODE

#define LM_DEVELOP_FEATURE  "jeda_develop"
#define LM_RUN_FEATURE "jeda_run"
#define LM_VERSION "3.1"

#define JEDA_DSA_KEY_0  0xd08fb13f
#define JEDA_DSA_KEY_1  0xf57db6b4
#define JEDA_DSA_KEY_2  0xfac6bdad
#define JEDA_DSA_KEY_3  0x24b900ce

#define JEDA_DSA_NUM  0x102

/* 3 0 */
#define JEDA_DSA_MASK_0 0x0008
/* 6 0 */
#define JEDA_DSA_MASK_1 0x0040
/* 7 0 */
#define JEDA_DSA_MASK_2 0x0080
/* 13 0 */
#define JEDA_DSA_MASK_3 0x2000

#define JEDA_DSA_BIT31 0x40000000

#define JEDA_DSA_NEXT(x,y) (x&JEDA_DSA_BIT31)?(((x^y)<<1)|1):(x<<1) ;

static int check_dsa_key( int x, int y ) {
  int r0, r1, r2, r3 ;
  int i ;
  r0 = x + JEDA_DSA_KEY_0 ;
  r1 = x + JEDA_DSA_KEY_1 ;
  r2 = x + JEDA_DSA_KEY_2 ;
  r3 = x + JEDA_DSA_KEY_3 ;
  
  for( i = 0 ; i < JEDA_DSA_NUM ; i++ ) {
    r0 = JEDA_DSA_NEXT(r0, JEDA_DSA_MASK_0) ;
    r1 = JEDA_DSA_NEXT(r1, JEDA_DSA_MASK_1) ;
    r2 = JEDA_DSA_NEXT(r2, JEDA_DSA_MASK_2) ;
    r3 = JEDA_DSA_NEXT(r3, JEDA_DSA_MASK_3) ;
  }
  
  return (r0+r1+r2+r3)-y ;
}

#endif
