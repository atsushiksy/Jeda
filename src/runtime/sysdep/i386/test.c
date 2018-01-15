
typedef struct JD_lwp_s JD_lwp ;

struct JD_lwp_s {
  void (*ret)() ;      /* 0  */
  int  *ret_fp ;       /* 4  */ 
  int  *ret_sp ;       /* 8  */ 
  void (*func)() ;     /* 12 */ 
  int  *arg ;          /* 16 */ 
  int  *fp ;           /* 20 */
  int  *sp ;           /* 24 */
  void (*reta)() ;     /* 28 */
  void (*retb)() ;     /* 32 */
  int  *save_stack ;   /* 36 */
  int  stack_size ;    /* 40 */
  int  *stack_base ;   /* 44 */
} ;

void lwp_sleep( JD_lwp *lwp ) {
  if( lwp ) {
    lwp = (JD_lwp *)alloc( sizeof(JD_lwp) ) ;
  }
  else {
    *lwp->stack_base = lwp->stack_size ;
    free( lwp ) ;
  }
}
