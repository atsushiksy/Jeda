
typedef struct JD_lwp_s JD_lwp ;

struct JD_lwp_s {
  void (*ret)() ;           /* 0  */
  uint64_t  *ret_fp ;       /* 8  */
  uint64_t  *ret_sp ;       /* 16  */
  void (*func)() ;          /* 24 */ 
  JD_thread *arg ;          /* 32 */
  uint64_t  *fp ;           /* 40 */
  uint64_t  *sp ;           /* 48 */
  uint64_t rbp ;            /* 56 */
  uint64_t rbx ;            // 64
  uint64_t r12 ;            // 72
  uint64_t r13 ;            // 80
  uint64_t r14 ;            // 88
  uint64_t r15 ;            // 96
  void (*reta)() ;          /* 104 */
  void (*retb)() ;          /* 112 */
  uint64_t  *save_stack ;   /* 120 */
  uint64_t stack_size ;     /* 128 */
  uint64_t  **stack_base ;  /* 136 + 8 = 142 total size */
} ;

void lwp_sleep( JD_lwp *lwp ) {
  if( lwp ) {
    lwp = (JD_lwp *)malloc( sizeof(JD_lwp) ) ;
    lwp->arg = 0 ;
  }
  else {
    *lwp->stack_base = lwp->stack_size ;
    free( lwp ) ;
  }
}


void call_lwp_sleep() {
  JD_lwp lwp ;

  lwp_sleep( &lwp) ;
  
}