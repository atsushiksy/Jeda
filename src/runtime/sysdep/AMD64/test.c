
typedef struct JD_lwp_s JD_lwp ;
typedef struct JD_thread_s JD_thread ;

typedef unsigned long long uint64_t ;

struct JD_lwp_s {
  uint64_t  *ret_sp ;       // 0  
  uint64_t ret_rbp ;        // 8 
  uint64_t ret_rbx ;        // 16
  uint64_t ret_r12 ;        // 24
  uint64_t ret_r13 ;        // 32
  uint64_t ret_r14 ;        // 40
  uint64_t ret_r15 ;        // 48
  void (*func)() ;          // 56 
  JD_thread *arg ;          // 64 
  uint64_t  *sp ;           // 72 
  uint64_t rbp ;            // 80
  uint64_t rbx ;            // 88
  uint64_t r12 ;            // 96
  uint64_t r13 ;            // 104
  uint64_t r14 ;            // 112
  uint64_t r15 ;            // 120
  void (*reta)() ;          // 128 
  void (*retc)() ;          // 136 
  uint64_t  *save_stack ;   // 144 
  uint64_t stack_size ;     // 152 
  uint64_t  **stack_base ;  // 160 + 8 = 168 total size 
} ;

void JD_call_lwp_return() ;
void JD_sleep_lwp_jump() ;
void   JD_call_lwp( JD_lwp *lwp ) ;
void   JD_sleep_lwp( JD_lwp *lwp ) ;

void JD_uf_main( JD_lwp *lwp ) {

    printf( "JD_uf_main entered with %llx\n", lwp ) ;

    JD_sleep_lwp(lwp) ;

    printf( "JD_uf_main entered 2nd time with %llx\n", lwp ) ;

    JD_sleep_lwp(lwp) ;

    printf( "JD_uf_main entered 3nd time\n" ) ;
    JD_sleep_lwp(lwp) ;

    printf( "JD_uf_main entered 4 time\n" ) ;
    JD_sleep_lwp(lwp) ;

    printf( "JD_uf_main entered 5 time\n" ) ;
    JD_sleep_lwp(lwp) ;

    printf( "JD_uf_main entered 6 time\n" ) ;

}

/* This function is called from assembler code */
void JD_stack_base_error( int *bp, int *sp ) {
  printf(
    "Jeda internal error: lwp offset overflow. bp = %x sp = %x\n",
    bp, sp
  ) ;
  exit(1) ;
}

static int *stack_base = 0 ;

JD_lwp *JD_create_lwp( void (*func)(), JD_thread *arg ) {
  JD_lwp *lwp ;
  lwp = (JD_lwp *)calloc(1,sizeof(JD_lwp)) ;
  lwp->func = func ;
  if( arg ) {
    lwp->arg = arg ;
  }
  else {
    lwp->arg = lwp ;
  }
  lwp->stack_base = &stack_base ;
  return lwp ;
}

void main()
{

  JD_lwp *lwp ;
  lwp = JD_create_lwp( (void *)JD_uf_main, 0 ) ;


  printf( "main calling 0 with %llx\n", lwp) ;
  JD_call_lwp( lwp ) ;
  printf( "main return 0\n") ;
  printf( "main calling 1\n") ;
  JD_call_lwp( lwp ) ;
  printf( "main return 1\n") ;
  printf( "main calling 2\n") ;
  JD_call_lwp( lwp ) ;
  printf( "main return 2\n") ;


}



void lwp_sleep( JD_lwp *lwp ) {
  if( lwp ) {
    lwp = (JD_lwp *)malloc( sizeof(JD_lwp) ) ;
    lwp->ret_sp = 0 ;
    lwp->ret_rbp = 0 ;
    lwp->ret_rbx = 0 ;
    lwp->ret_r12 = 0 ;
    lwp->ret_r13 = 0 ;
    lwp->ret_r14 = 0 ;
    lwp->ret_r15 = 0 ;
    lwp->func = 0 ;
    lwp->arg = 0 ;
  }
  else {
    *lwp->stack_base = lwp->stack_size ;
    free( lwp ) ;
  }
}

static  JD_lwp lwp ;


JD_lwp* call_lwp_sleep() {

  lwp_sleep( &lwp) ;
  
  return &lwp ;

}