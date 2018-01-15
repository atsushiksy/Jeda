/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
   Copyright (C) 2002, 2003 Jeda Technologies, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/
#ifdef DEBUG_MEM_PROBLEM
  void *jeda_debug_address = 0 ;
#define JEDA_DEBUG_ADDRESS
void jeda_free_error( int *x ) {
  free(x) ;
}
#endif

#define JEDA_TOP_LEVEL
#include <stdio.h>
#include <signal.h>
#include "runtime.h"
#include "mem_macro.h"
#include "runtime_error_msg.h"
#include "data.h"
#include "thread.h"
#include "bit_operations.h"
#include "bit_macro.h"
#include "exec_macro.h"
#include "data_macro.h"
#include "code_exec_macro.h"

#ifdef PRODUCT_CODE
#include "../include/license.h"
#endif

#define NULL_COUNT_MAX 100

#ifdef JEDA_DEBUGGER
  extern JD_debug_stack *JD_debug_current_stack ;
  extern void JD_debug_sig_handler() ;
  extern JD_debug_stack *JD_debugged_stack ;
#endif

#ifdef ICARUS_VERILOG_MODE
#  include "ivl_vpi_user.h"
#endif

static void v_top_loop() ;

char *SCAN_PLUSARGS() ;

void JD_event_flush() ;

int JD_ignore_empty_event_object = 0 ;

/* global variables that represent the chached image of VM */
JD_thread    *THREAD = NULL ;        /* current thread */
JD_thread    *TMP_THREAD = NULL ;    /* temp thread used to allocate thread */
JD_data      **STACK ;        /* stack for current thread */
JD_data      **GLOBAL_DATA ;  /* stack for current thread */
int          SP ;             /* stack pointer for current thread */

/* V-stack is a stack cache that holds values temporary, to avoid exess
   alloc/dealloc of bit array. It works as the register ring in SPARC,
   with VSP_LIMIT (= 8, defined in runtime.h) entries in it.
 */
JD_data      **VSTACK ;       /* V-stack */
int          VSP ;            /* number of entry used in V-stack */
int          VS_TOP ;         /* the top of V-stack */
unsigned int **VS_A ;         /* pre-allocated bit a array */
unsigned int **VS_B ;         /* pre-allocated bit b array */
int          *VS_WD ;         /* size of pre-allocated bit array */

int        VSTACK_HIT = 0 ;  /* just a counter to mesure the hit on V-stack */
int        VSTACK_MISS = 0 ; /* just a counter to mesure the miss on V-stack */
int        VSTACK_FULL = 0 ; /* just a counter to mesure the full on V-stack */

int          INT_ZERO ;       /* always holds zero */
int          INT_ONE ;        /* always holds one */
int          BIT_SIZE ;       /* bit size for allocation */
int          ARRAY_SIZE ;     /* temp reg to hold array size for allocation */
void         (*ALLOC_OBJ)() ; /* temp reg to hold the pointer to allocate obj */
void         (*DEALLOC_OBJ)() ; /* to hold the pointer to deallocate obj */

int          U_BIT ;       /* temp reg to hold upper bit for sub-bit access */
int          L_BIT ;       /* temp reg to hold lower bit for sub-bit access */
int          I_ENDIAN ;    /* temp reg to hold endian flag for sub-bit access */
int          U_INDEX ;     /* temp reg for ub in subbit access */
int          L_INDEX ;     /* temp reg for ub in subbit access */
int          DRV_STRENGTH ; /* drive strength info */
int          STORE_SIZE ;  /* keep the latest store size (for concat store) */

/* temp regs to be used for ALU operations */
int          *A0;
int          *B0;
int          S0;
int          *A1;
int          *B1;
int          S1 ;
int          RET ;
int          ERR ;

/* temp a/b buffer to hold bit data for ALU operations */
unsigned int *TMP_A ;
unsigned int *TMP_B ;
int          TMP_WD ;     
int          TMP_INT ;
int          TMP_INT2 ;
char         *TMP_STRING ;
double       TMP_FLOAT ;
JD_data      *TMP_DT ;
unsigned int *AR ;
unsigned int *BR ;
int          SR ;

/* ACC is formed as the following individual regs */
unsigned int *ACC_A ;
unsigned int *ACC_B ;
int          ACC_SIZE ;
int          ACC_WD ;
int          ACC_INT ;
double       ACC_FLOAT ;
char         *ACC_STRING ;
JD_data      *ACC_DT ;
JD_data_type ACC_TYPE ;

/* second set of bit for binary ops */
unsigned int *ACCB_A ;
unsigned int *ACCB_B ;
int          ACCB_SIZE ;
int          ACCB_WD ;

/* Save ACC area */
int ADVICED = 0 ;
unsigned int *S_ACC_A ;
unsigned int *S_ACC_B ;
int          S_ACC_SIZE ;
int          S_ACC_WD ;
int          S_ACC_INT ;
double       S_ACC_FLOAT ;
char         *S_ACC_STRING ;
JD_data      *S_ACC_DT ;
JD_data_type S_ACC_TYPE = JD_D_null ;

/* temp reg to hold number of arg on function call */
int          NUM_ARG ;

/* temp reg to hold the entry of function on thread creation */
void (*TARGET_FUNC)() ;

/* local flag to detect ready queue is empty */
int JD_thread_empty ;

JD_toplevel *JD_current_toplevel ;
JD_toplevel *JD_top_of_toplevel ;

pthread_mutex_t *JD_m_mutex ;
pthread_mutex_t *JD_d_mutex ;

pthread_cond_t *JD_main_cond ;

JD_queue *JD_ready_queue ;
JD_queue *JD_sleep_queue ;
JD_queue *JD_zombie_queue ;

void JD_uf_main() ;
void JD_si_alloc_global(JD_toplevel *) ;
void JD_si_initialize() ;

#ifdef JEDA_SELF_THREAD_CONTROL
JD_lwp *JD_create_lwp( void (*func)(), JD_thread *arg ) ;
#endif

static int end_of_simulation = 0 ;
static int finish_on_end = 1 ;
static int stop_on_end = 0 ;
static int finish_on_error = 0 ;
static int debug_on_error = 0 ;
static int stop_on_error = 1 ;
static int stop_on_first_error = 0 ;
static int debug_on_cntlc = 1 ;
static int debug_on_warning = 0 ;
static int keep_looping ;
int JD_invoke_debugger ;

char *JD_debugger_filename ;

#ifdef PRODUCT_CODE
  static int top_check_ok = 0 ;
  void JD_top_check( int r, int k ) {
    if( check_dsa_key( r, k ) ) {
      fprintf( stderr, "Jeda license code error (%d %d) detected\n", r, k ) ;
      exit(1) ;
    }
    top_check_ok = 1 ;
  }
#endif

static void allocate_regs() {
  int i ;
  
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  if ( top_check_ok == 0 ) {
    fprintf( stderr, "Jeda license code error detected\n" ) ;
    exit(1) ;
  }
#  endif
#endif
  VSTACK = (JD_data **)calloc( VSP_LIMIT, sizeof(JD_data *) ) ;
  VS_A = (unsigned int **)calloc( VSP_LIMIT, sizeof(int *) ) ;
  VS_B = (unsigned int **)calloc( VSP_LIMIT, sizeof(int *) ) ;
  VS_WD = (int *)calloc( VSP_LIMIT, sizeof(int) ) ;
  for( i = 0 ; i < VSP_LIMIT ; i++ ) {
    VSTACK[i] = JD_alloc_data_entry() ;
    VS_A[i] = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
    VS_B[i] = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
    VS_WD[i] = INIT_WD_SIZE ;
  }
  VSP = 0 ;            /* number of entry used in V-stack */
  VS_TOP = 0 ;         /* the top of V-stack */
  
  
  INT_ZERO = 0 ;       /* always holds zero */
  INT_ONE = 1 ;        /* always holds one */
  TMP_A = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  TMP_B = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  TMP_WD = INIT_WD_SIZE ;
  
  TMP_DT = NULL ;
  ACC_A = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  ACC_B = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  ACC_WD = INIT_WD_SIZE ;
  ACC_STRING = NULL ;
  ACC_DT = NULL ;
  ACC_TYPE = JD_D_unknown ;

  ACCB_A = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  ACCB_B = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  ACCB_WD = INIT_WD_SIZE ;
  
  
  S_ACC_A = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  S_ACC_B = (int *)calloc( INIT_WD_SIZE, sizeof(int) ) ;
  S_ACC_WD = INIT_WD_SIZE ;
  S_ACC_STRING = NULL ;
  S_ACC_DT = NULL ;
  S_ACC_TYPE = JD_D_unknown ;

  JD_static_random_init() ;

}

void JD_initialize_top() {
  JD_thread *thrd ;
  pthread_t top_loop_thread ;

  JD_top_of_toplevel = JD_alloc_top() ;
  
  JD_current_toplevel = JD_top_of_toplevel ;
  
  JD_ready_queue = JD_current_toplevel->ready_queue ;
  JD_sleep_queue = JD_current_toplevel->sleep_queue ;
  JD_zombie_queue = JD_current_toplevel->zombie_queue ;
  
  JD_m_mutex = &JD_current_toplevel->m_mutex ;
  JD_d_mutex = &JD_current_toplevel->d_mutex ;
  JD_main_cond = &JD_current_toplevel->m_cond ;
  
  thrd = JD_current_toplevel->top_thread ;

  allocate_regs() ;
    
  thrd->status = JD_thrd_ready ;
  JD_enqueue( JD_ready_queue, thrd ) ;
  
  thrd->run_index = 0 ;
    
  JD_si_alloc_global( JD_current_toplevel ) ;
  GLOBAL_DATA = JD_current_toplevel->global_data ;
  THREAD = thrd ;
  STACK = thrd->stack ;
  SP = thrd->sp ;
  JD_si_initialize() ;
  
#ifdef JEDA_SELF_THREAD_CONTROL
  thrd->lwp = JD_create_lwp( (void *)JD_uf_main, NULL ) ;
  v_top_loop() ;
#else
  thrd->main_flag = 1 ;
  pthread_create( 
    &thrd->pth, NULL, (void *)JD_uf_main, (void *)thrd 
  ) ;
  pthread_detach( thrd->pth ) ;
    
  pthread_mutex_lock( &thrd->l_mutex ) ;  /* check if thrd is ready */
  while( thrd->main_flag ) {
    pthread_cond_wait( &thrd->l_cond, &thrd->l_mutex ) ; /* wait for ready */
  }
  pthread_create( &top_loop_thread, NULL, (void *)v_top_loop, NULL ) ;
  pthread_join(top_loop_thread,NULL) ;

#endif
    
}

  
int JD_top_loop() {
  JD_thread *thrd, *zmbi ;
  pthread_t top_loop_thread ;
    
#ifdef JEDA_SELF_THREAD_CONTROL
  v_top_loop() ;
#else
  pthread_create( &top_loop_thread, NULL, (void *)v_top_loop, NULL ) ;
  
  pthread_join(top_loop_thread,NULL) ;
  
  return JD_thread_empty ;
#endif

}


/*****************************************************************/
/* Top loop procedures for execution with simulator              */
/*****************************************************************/

static void v_top_loop() {
  JD_thread *thrd, *zmbi ;
  void (*sig_handler)(int) ;

  /*
  PRINTF( "entering v_top_loop!\n" ) ;
  */

  keep_looping = 1 ;
  JD_invoke_debugger = 0 ;
  JD_debugger_filename = NULL ;
  
#ifdef JEDA_DEBUGGER
  if( debug_on_cntlc ) {
    sig_handler = (void *)signal( SIGINT, JD_debug_sig_handler ) ;
    if( sig_handler == SIG_ERR ) sig_handler = NULL ;
  }
#endif

#ifdef JEDA_DEBUG_THREAD
  /* PRINTF( "*** entered v_top_loop()\n" ) ; */
#endif

#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_mutex_lock( JD_m_mutex ) ;
#endif
  
  while( keep_looping && (thrd = JD_dequeue_top( JD_ready_queue )) ) {
    THREAD = thrd ;
    STACK = thrd->stack ;
    SP = thrd->sp ;
    
#ifdef JEDA_DEBUGGER
    JD_debug_current_stack = thrd->dbg_stack ;
#endif

    CALL_EXEC_POP_ACC ;
    if( thrd->timeout ) {
      PRINTF( "Thread %d: timeout detected\n", thrd->id ) ;
#ifdef JEDA_DEBUGGER
      if( debug_on_error ) {
        JD_debugged_stack = JD_debug_current_stack ;
        JD_debugger_main() ;
      }
#endif
      thrd->timeout = 0 ;
    }
    
#ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** pthread_mutex_unlock %x\n", thrd ) ; 
#endif

#ifdef JEDA_SELF_THREAD_CONTROL
    JD_call_lwp( thrd->lwp ) ;
#else
    thrd->main_flag = 1 ;
    pthread_mutex_unlock( JD_m_mutex ) ; /* release the mutex */
    pthread_cond_signal( &thrd->s_cond ) ; /* signal thrd to run */
    
#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** pthread_mutex_lock m_mutex called\n" ) ; 
#  endif
    pthread_mutex_lock( JD_m_mutex ) ;  /* check if thrd is done */
    while( thrd->main_flag ) {
      pthread_cond_wait( JD_main_cond, JD_m_mutex ) ; /* wait for next run */
    }
#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** pthread_mutex_lock m_mutex returned\n" ) ; 
#  endif

#endif
    /* update sp before push_acc which possible invoke GC */
    thrd->stack = STACK ;
    thrd->sp = SP ;
    CALL_EXEC_PUSH_ACC ;
    CALL_EXEC_FLUSH_VSTACK ;
    thrd->stack = STACK ;
    thrd->sp = SP ;
#ifdef JEDA_DEBUGGER
    if( JD_invoke_debugger ) {
      JD_invoke_debugger = 0 ;
      
      /* only stop if JD_debug_current_stack is active */
      if( JD_debug_current_stack ) {
        JD_debugged_stack = JD_debug_current_stack ;
        JD_debugger_main() ;
      }
    }
#endif
  }
  
#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_mutex_unlock( JD_m_mutex ) ;
#endif

#ifdef JEDA_DEBUGGER
  if( debug_on_cntlc && sig_handler) {
    (void)signal( SIGINT, sig_handler ) ;
  }
#endif
  while( (zmbi = JD_dequeue_top( JD_zombie_queue ) ) ) {
    DEALLOC_STACK(zmbi->stack, zmbi->stack_size) ;
    /* PRINTF( "*** zombi %x\n", zmbi ) ; */
#ifdef JEDA_SELF_THREAD_CONTROL
    if( zmbi->lwp ) {
      if( zmbi->lwp->save_stack ) free( zmbi->lwp->save_stack ) ;
      free(zmbi->lwp) ;
    }
#endif
    DEALLOC_THREAD(zmbi) ; 
  }
  
  JD_event_flush() ;  // clean up the events

}

void JD_exec_breakpoint( char *file )
{
  JD_debugger_filename = file ;
  JD_invoke_debugger = 1 ;
  DBG_CODE_LOCATION ;
  PRINTF( "breakpoint encountered!\n" ) ;
#ifndef JEDA_DEBUGGER
  DOSTOP ;
#endif
  JD_enqueue_top( JD_ready_queue, THREAD ) ;
  JD_thread_pause( THREAD ) ;
}

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;

extern JD_module_table JD_modules_entries[] ;
static debugger_construct_name_table() {
  int i, j, k, m, n, p ;
  int size ;
  size = 0 ;
  for( i = 0 ; JD_modules_entries[i].name_table ; i++ ) {
    for( j = 0 ; strcmp(JD_modules_entries[i].name_table[j],"") ; j++ ) {
      size ++ ;
    }
  }
  JD_Debug_name_table = 
    (char **)calloc( size+1, sizeof(char *) );
  n = 0 ;
  for( i = 0 ; JD_modules_entries[i].name_table ; i++ ) {
    if( JD_modules_entries[i].name_offset ) {
      *(JD_modules_entries[i].name_offset) = n ;
    }
    for( j = 0 ; strcmp(JD_modules_entries[i].name_table[j],"") ; j++ ) {
      JD_Debug_name_table[n++] =  JD_modules_entries[i].name_table[j] ;
    }
  }
}

#endif

void JD_v_create_top() {
  JD_thread *thrd ;

  JD_top_of_toplevel = JD_alloc_top() ;
  
  JD_current_toplevel = JD_top_of_toplevel ;
  
  JD_ready_queue = JD_current_toplevel->ready_queue ;
  JD_sleep_queue = JD_current_toplevel->sleep_queue ;
  JD_zombie_queue = JD_current_toplevel->zombie_queue ;
  
  JD_m_mutex = &JD_current_toplevel->m_mutex ;
  JD_d_mutex = &JD_current_toplevel->d_mutex ;
  JD_main_cond = &JD_current_toplevel->m_cond ;
  
  thrd = JD_current_toplevel->top_thread ;

  allocate_regs() ;
#ifdef JEDA_DEBUGGER
  thrd->dbg_stack = (JD_debug_stack *)calloc(1,sizeof(JD_debug_stack)) ;
  thrd->dbg_stack->active = 1 ;
  thrd->dbg_stack->thrd = thrd ;
  JD_current_toplevel->top_dbg_stack = thrd->dbg_stack ;
  JD_debug_current_stack = thrd->dbg_stack ;
  JD_debugger_construct_scope_structure() ;
  JD_debugger_read_breakable_points() ;
#endif

#ifdef PRODUCT_CODE
#  ifndef JEDA_DEBUGGER
  debugger_construct_name_table() ;
#  endif
#endif
    
  JD_si_alloc_global( JD_current_toplevel ) ;
  GLOBAL_DATA = JD_current_toplevel->global_data ;
  
  /* Can't call initializer now, as we haven't got the verilog link
  JD_si_initialize() ;
  */
  
    
}

void JD_v_create_main() {
  JD_thread *thrd ;
  pthread_t top_loop_thread ;
  char *mode ;

  thrd = JD_current_toplevel->top_thread ;
  GLOBAL_DATA = JD_current_toplevel->global_data ;
  THREAD = thrd ;
  STACK = thrd->stack ;
  SP = thrd->sp ;
  JD_si_initialize() ;
  
  thrd->status = JD_thrd_ready ;
  JD_enqueue( JD_ready_queue, thrd ) ;
  
  thrd->run_index = 0 ;
  
#ifdef JEDA_SELF_THREAD_CONTROL
  thrd->lwp = JD_create_lwp( (void *)JD_uf_main, NULL ) ;
#else
  thrd->main_flag = 1 ;
  pthread_create( 
    &thrd->pth, NULL, (void *)JD_uf_main, (void *)thrd 
  ) ;
  pthread_detach( thrd->pth ) ;
    
  pthread_mutex_lock( &thrd->l_mutex ) ;  /* check if thrd is ready */
  while( thrd->main_flag ) {
    pthread_cond_wait( &thrd->l_cond, &thrd->l_mutex ) ; /* wait for ready */
  }
#endif

  end_of_simulation = 0 ;
  
  
  mode = SCAN_PLUSARGS( "jeda_ignore_empty_even" ) ;
  if( mode != NULL && !strcmp( mode, "t" ) ) {
    JD_ignore_empty_event_object = 1 ;
  }
  
  mode = SCAN_PLUSARGS( "jeda_finish_on_en" ) ;
  if( mode != NULL ) {
    if( !strcmp( mode, "d" ) ) {
      finish_on_end = 1 ;
    }
  }
  mode = SCAN_PLUSARGS( "jeda_stop_on_en" ) ;
  if( mode != NULL ) {
    if( !strcmp( mode, "d" ) ) {
      stop_on_end = 1 ;
    }
  }
  mode = SCAN_PLUSARGS( "jeda_continue_on_en" ) ;
  if( mode != NULL ) {
    if( !strcmp( mode, "d" ) ) {
      stop_on_end = 0 ;
      finish_on_end = 0 ;
    }
  }
  mode = SCAN_PLUSARGS( "jeda_finish_on_erro" ) ;
  if( mode == NULL ) {
    finish_on_error = 0 ;
  }
  else if( !strcmp( mode, "r" ) ) {
    finish_on_end = 1 ;
  }
  mode = SCAN_PLUSARGS( "jeda_stop_on_erro" ) ;
  if( mode == NULL ) {
    stop_on_error = 0 ;
  }
  else if( !strcmp( mode, "r" ) ) {
    stop_on_error = 1 ;
  }
  mode = SCAN_PLUSARGS( "jeda_stop_on_first_erro" ) ;
  if( mode == NULL ) {
    stop_on_first_error = 0 ;
  }
  else if( !strcmp( mode, "r" ) ) {
    stop_on_first_error = 1 ;
  }
  mode = SCAN_PLUSARGS( "jeda_debug_on_erro" ) ;
  if( mode != NULL && !strcmp( mode, "r" ) ) {
    debug_on_error = 1 ;
  }
  mode = SCAN_PLUSARGS( "jeda_continue_on_erro" ) ;
  if( mode != NULL && !strcmp( mode, "r" ) ) {
    stop_on_error = 0 ;
    debug_on_error = 0 ;
    finish_on_error = 0 ;
  }
  mode = SCAN_PLUSARGS( "jeda_no_debugger_on_ctl" ) ;
  if( mode != NULL && !strcmp( mode, "c" ) ) {
    debug_on_cntlc = 0 ;
  }
  mode = SCAN_PLUSARGS( "jeda_debug_on_warnin" ) ;
  if( mode != NULL && !strcmp( mode, "g" ) ) {
    debug_on_warning = 1 ;
  }

}

void JD_v_top_loop() {
  JD_thread *thrd ;
  pthread_t top_loop_thread ;

  if( !end_of_simulation ) {
#ifdef JEDA_SELF_THREAD_CONTROL
    v_top_loop() ;
#else
    /* v_top_loop enables the ready threads */
    pthread_create( &top_loop_thread, NULL, (void *)v_top_loop, NULL ) ;
  
    pthread_join(top_loop_thread,NULL) ;
#endif
  }

}

void JEDA_end_of_simulation() {
  end_of_simulation = 1 ;
  
  if( finish_on_end ) {
#ifdef JEDA_DEBUGGER
    JD_dbg_close_window() ;
#endif
    DOFINISH ;
  }
  else if ( stop_on_end ) {
    DOSTOP ;
  }
  
}

void JEDA_exit_simulation() {
  keep_looping = 0 ;
  end_of_simulation = 1 ;
  DOFINISH ;
  JD_enqueue_top( JD_ready_queue, THREAD ) ;
  JD_thread_pause( THREAD ) ;
}

void JEDA_verif_error() {

  if( debug_on_error ) {
    /* call debugger */
    JD_invoke_debugger = 1 ;
    JD_enqueue_top( JD_ready_queue, THREAD ) ;
    JD_thread_pause( THREAD ) ;
  }


  if( finish_on_error ) {
#ifdef JEDA_DEBUGGER
    JD_dbg_close_window() ;
#endif
    DOFINISH ;
  }
  else if ( stop_on_error || stop_on_first_error ) DOSTOP ;
  
  if( stop_on_first_error ) {
    keep_looping = 0 ;
    JD_enqueue_top( JD_ready_queue, THREAD ) ;
    JD_thread_pause( THREAD ) ;
  }
  
}

void JEDA_timeout_error() {

  if( finish_on_error ) {
#ifdef JEDA_DEBUGGER
    JD_dbg_close_window() ;
#endif
    DOFINISH ;
  }
  else if ( stop_on_error || stop_on_first_error ) DOSTOP ;
    
}

void JEDA_fatal_error() {
  
  if( finish_on_error ) {
#ifdef JEDA_DEBUGGER
    JD_dbg_close_window() ;
#endif
    DOFINISH ;
  }
  else if ( stop_on_error ) DOSTOP ;
  
  end_of_simulation = 1 ;
  keep_looping = 0 ;
  JD_invoke_debugger = 1 ;

  JD_enqueue_top( JD_ready_queue, THREAD ) ;
  JD_thread_pause( THREAD ) ;
}

void JEDA_warning_handler() {
  
#ifdef JEDA_DEBUGGER
  if ( debug_on_warning ) {
    JD_invoke_debugger = 1 ;

    JD_enqueue_top( JD_ready_queue, THREAD ) ;
    JD_thread_pause( THREAD ) ;
  
  }
#endif
  
}

void JEDA_quit_simulation() {
  end_of_simulation = 1 ;
  
#ifdef JEDA_DEBUGGER
  JD_dbg_close_window() ;
#endif
  DOFINISH ;
}

void JEDA_print_error( char *msg ) {
  CODE_LOCATION ;
  PRINTF( msg ) ;
}

void JEDA_print_warning( char *msg ) {
  CODE_LOCATION ;
  PRINTF( msg ) ;
  JEDA_warning_handler() ;
}

#ifdef TOP_LOOP_MAIN
static void main_top_loop() {
  JD_thread *thrd, *zmbi ;
  int null_count = 0 ;
  while(1) {
    JD_top_loop() ;
    JD_count_timeq( &JD_current_toplevel->time_queue ) ;
    if( JD_thread_empty && JD_current_toplevel->time_queue == NULL )
      null_count++ ;
    else null_count = 0 ;
    if( null_count > NULL_COUNT_MAX ) break ;
  }
  fprintf( stderr, "End of execution\n" ) ;
}

main()
{

  JD_initialize_top() ;
  
  main_top_loop() ;
  
}

#else



#endif

