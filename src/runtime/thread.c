/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.

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

#ifndef NO_INCLUDE

#include <stdio.h>
#include <pthread.h>
#include "runtime.h"
#include "runtime_error_msg.h"
#include "exec_macro.h"
#include "data.h"
#include "code_exec_macro.h"
#include "debugger.h"
#ifdef PRODUCT_CODE
#include "veriuser.h"
#include "acc_user.h"
#include "pli.h"
#endif
#include "mem_macro.h"

#endif

#ifdef PRODUCT_CODE
#include "../include/license.h"
  static int thread_check_ok = 0 ;
  void JD_thread_check( int r, int k ) {
    if( check_dsa_key( r, k ) ) {
      fprintf( stderr, "Jeda license code error (%d %d) detected\n", r, k ) ;
      exit(1) ;
    }
    thread_check_ok = 1 ;
  }
#endif

extern JD_toplevel *JD_current_toplevel ;

#ifndef JEDA_SELF_THREAD_CONTROL
  extern pthread_mutex_t *JD_m_mutex ;
  extern pthread_mutex_t *JD_d_mutex ;
  extern pthread_mutex_t *JD_s_mutex ;
  extern pthread_cond_t *JD_main_cond ;
#endif

extern JD_queue *JD_ready_queue ;
extern JD_queue *JD_sleep_queue ;
extern JD_queue *JD_zombie_queue ;

static void exit_term_link( JD_thread *this ) ;

#ifdef JEDA_SELF_THREAD_CONTROL
void JD_call_lwp_return() ;
void JD_sleep_lwp_jump() ;

static int *stack_base = NULL ;

JD_lwp *JD_create_lwp( void (*func)(), JD_thread *arg ) {
  JD_lwp *lwp ;
  lwp = (JD_lwp *)calloc(1,sizeof(JD_lwp)) ;
  lwp->func = func ;
  lwp->arg = arg ;
  lwp->ret = JD_call_lwp_return ;
  lwp->stack_base = &stack_base ;
  return lwp ;
}
#endif

/* This function is called from assembler code */
void JD_stack_base_error( int *bp, int *sp ) {
  printf(
    "Jeda internal error: lwp offset overflow. bp = %x sp = %x\n",
    bp, sp
  ) ;
  exit(1) ;
}

#ifdef JEDA_SELF_THREAD_CONTROL
void JD_print_lwp( JD_lwp *lwp ) {
  int i ;
  
  PRINTF( "lwp %x\n", lwp ) ;
  PRINTF( "  ret %x\n", lwp->ret ) ;
  PRINTF( "  ret_fp %x\n", lwp->ret_fp ) ;
  PRINTF( "  ret_sp %x\n", lwp->ret_sp ) ;
  PRINTF( "  func %x\n", lwp->func ) ;
  PRINTF( "  arg %x\n", lwp->arg ) ;
  PRINTF( "  fp %x\n", lwp->fp ) ;
  PRINTF( "  sp %x\n", lwp->sp ) ;
  PRINTF( "  reta %x\n", lwp->reta ) ;
  PRINTF( "  retb %x\n", lwp->retb ) ;
  PRINTF( "  stack_size %x\n", lwp->stack_size ) ;
  for( i = 0 ; i < lwp->stack_size/4 ; i++ ) {
    PRINTF( "    stack[%d] = %x\n", i, lwp->save_stack[i] ) ;
  }
}
#endif

JD_queue *JD_alloc_queue( JD_toplevel *top ) {
  JD_queue *new ;
  
  new = ALLOC_QUEUE ;
  
  if( top->top_queue ) top->top_queue->queue_fw = new ;
  new->queue_bw = top->top_queue ;
  top->top_queue = new ;

  return new ;
  
}

void JD_dealloc_queue( JD_queue *old, JD_toplevel *top ) {
  
  if( old == NULL ) return ;
  if( old->queue_fw ) old->queue_fw->queue_bw = old->queue_bw ;
  else top->top_queue = old->queue_bw ;
  if( old->queue_bw ) old->queue_bw->queue_fw = old->queue_fw ;
  
  DEALLOC_QUEUE(old) ;
  
}

/* Enter a thread to the end of queue */
void JD_enqueue( JD_queue *queue, JD_thread *thrd ) 
{
  if( queue->thrd_end ) {
    /* not empty */
    queue->thrd_end->thrd_bw = thrd ;
    thrd->thrd_fw = queue->thrd_end ;
    queue->thrd_end = thrd ;
  } 
  else {
    queue->thrd_top = queue->thrd_end = thrd ;
    /* thrd->thrd_fw = thrd->thrd_bw = NULL ; //  not needed?*/
  }
  thrd->queue = queue ;
}

/* Enter a thread to the top of queue */
void JD_enqueue_top( JD_queue *queue, JD_thread *thrd ) 
{
  if( queue->thrd_top ) {
    /* not empty */
    queue->thrd_top->thrd_fw = thrd ;
    thrd->thrd_bw = queue->thrd_top ;
    queue->thrd_top = thrd ;
  } 
  else {
    queue->thrd_top = queue->thrd_end = thrd ;
    /* thrd->thrd_fw = thrd->thrd_bw = NULL ; //  not needed?*/
  }
  thrd->queue = queue ;
}

/* removing a thread from a queue */
void JD_dequeue( JD_queue *queue, JD_thread *thrd ) 
{
  if( thrd->queue != queue ) {
    ERROR_INTERNAL_TREAD_QUEUE_INCONSISTENT ;
  }
  if( thrd->thrd_fw == NULL ) {
    /* thrd is on the top of queue */
    queue->thrd_top = thrd->thrd_bw ;
    if( thrd->thrd_bw ) thrd->thrd_bw->thrd_fw = NULL ;
    else queue->thrd_end = NULL ;
  }
  else {
    thrd->thrd_fw->thrd_bw = thrd->thrd_bw ;
    if( thrd->thrd_bw ) thrd->thrd_bw->thrd_fw = thrd->thrd_fw ;
    else queue->thrd_end = thrd->thrd_fw ;
  }
  thrd->queue = NULL ;
  thrd->thrd_fw = thrd->thrd_bw = NULL ;
  
}

/* removing the top thread from a queue */
JD_thread *JD_dequeue_top( JD_queue *queue ) 
{
  JD_thread *top ;
  
  if( queue == NULL ) return NULL ;
  
  top = queue->thrd_top ;
  if( top ) {
    queue->thrd_top = top->thrd_bw ;
    if( top->thrd_bw ) top->thrd_bw->thrd_fw = NULL ;
    else queue->thrd_end = NULL ;
    top->queue = NULL ;
    top->thrd_fw = top->thrd_bw = NULL ;
  }
  return top ;
}

/* Timer queue handling procedures */
/*
   Timer queue conforms a linked list of decremental counter. This means
   a wait time of a thread in the queue is sum of wait count of threads
   in front of the queue.
   
   -->A(time_count=5)-->B(time_count=3)-->C(time_count=2)-->..
   
   Actual wait cycle of the thread C is 5+3+2 = 10.
   The count decrement can be done just by decrementing the count in the
   top of the queue. Isn't this neat?

*/
	
/* Entering time queue:
  To queue a thread, find a proper location in the linked list that
  the adjusted wait time nicely fits into the others in the queue.  
  
*/
void JD_enqueue_timeq( JD_thread **queue, JD_thread *thrd ) 
{
  JD_thread *fw, **fwp, *bw ;
  
  thrd->time_count = thrd->time_limit ;
  
  fwp = queue ;          /* fw writting pointer */
  fw = NULL ;             /* fw pointer to be written */
  bw = *queue ;            /* bw to check if its fit */
  while( (bw != NULL ) && (bw->time_count <= thrd->time_count) ) {
    thrd->time_count -= bw->time_count ;
    fw = bw ;
    fwp = &fw->time_thrd_bw ;
    bw = bw->time_thrd_bw ;
  }
  if( bw ) {
    bw->time_count -= thrd->time_count ;
    bw->time_thrd_fw = thrd ;
  }
  *fwp = thrd ;
  thrd->time_thrd_fw = fw ;
  thrd->time_thrd_bw = bw ;
}

/* To remove a thread from the time queue, it is necessary to add
   the remained count to the next thread in the queue (if any)
*/
void JD_dequeue_timeq( JD_thread **queue, JD_thread *thrd ) 
{
  JD_thread *fw, *bw ;
  
  fw = thrd->time_thrd_fw ;
  bw = thrd->time_thrd_bw ;
  
  if( fw ) {
    fw->time_thrd_bw = bw ;
    if( bw ) {
      bw->time_thrd_fw = fw ;
      bw->time_count += thrd->time_count ;
    }
  }
  else {
    *queue = bw ;
    if( bw ) {
      bw->time_thrd_fw = NULL ;
      bw->time_count += thrd->time_count ;
    }
  }
  
  thrd->time_thrd_fw = thrd->time_thrd_bw = NULL ;

}

void JD_count_timeq( JD_thread **queue ) {
  JD_thread *bw ;
  JD_thread *thrd ;
  thrd = *queue ;
  
  if( thrd ) {
    thrd->time_count-- ;
    if( thrd->time_count == 0 ) {
      while(thrd && thrd->time_count == 0 ) {
        thrd->timeout = 1 ;  /* set timeout flag */
        thrd->thread_join = 0 ;
        /* clearing queue */
        bw = thrd->time_thrd_bw ;
        thrd->time_thrd_bw == NULL ;
        JD_dequeue( thrd->queue, thrd ) ;
        thrd->status = JD_thrd_ready ;
        JD_enqueue( JD_ready_queue, thrd ) ;
        JEDA_timeout_error() ;
        thrd = bw ;
        if( thrd ) thrd->time_thrd_fw = NULL ;
      }
      *queue = thrd ;
    }
  }
}


JD_data **JD_alloc_stack( int size ) {
  JD_data **new ;
  
  new = ALLOC_STACK(size) ;
    
  return new ;
}

JD_data **JD_realloc_stack( JD_data **old, int old_size, int additional ) {
  JD_data **new, **p ;
  
  new = REALLOC_STACK(old, old_size+additional, old_size) ;
  
  p = &new[old_size] ; /* beginning of the extended entry */
  
  bzero( p, additional*sizeof(JD_data *) ) ; /* clear the extended entries */
    
  return new ;
  
}

void JD_free_pthread_key() {
  PRINTF( "JD_free_pthread_key called\n" ) ;
}

JD_thread *JD_alloc_thread( int id ) {
  JD_thread *new ;
  
  new = ALLOC_THREAD ;

  new->stack = JD_alloc_stack( INITAL_STACK_SIZE ) ;
  new->stack_size = INITAL_STACK_SIZE ;
  new->id = id ;
  
  /* dummy data for ACC */
  new->stack[new->sp++] = JD_alloc_int() ;
  
#ifndef JEDA_SELF_THREAD_CONTROL
  /*
  pthread_mutex_init( &new->s_mutex, NULL ) ;
  pthread_mutex_lock( JD_m_mutex ) ;
  */
  /* pthread_mutex_init( &new->l_mutex, NULL ) ; */
  pthread_cond_init( &new->s_cond, NULL ) ;
    
  pthread_mutex_init( &new->l_mutex, NULL ) ;
  pthread_cond_init( &new->l_cond, NULL ) ;
#endif

  return new ;
  
}

JD_toplevel *JD_alloc_top()
{
  JD_toplevel *new ;
  JD_thread *thrd ;

#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  if ( thread_check_ok == 0 ) {
    fprintf( stderr, "Jeda license code error detected\n" ) ;
    exit(1) ;
  }
#  endif
#endif
  
  new = ALLOC_TOP ;
  
  /* alocate JD_data pool */
  JD_inital_data_allocation() ;
  
  /* copy the data info for multi-main handling */
  JD_get_data_info(
    &new->data_top, 
    &new->free_data_top, 
    &new->generation0, 
    &new->generation1 
  ) ;
  
  new->thrd_count = 0 ;
  
  /* allocate queue */
  new->ready_queue = JD_alloc_queue( new ) ;
  new->call_queue = JD_alloc_queue( new ) ;
  new->sleep_queue = JD_alloc_queue( new ) ;
  new->zombie_queue = JD_alloc_queue( new ) ;
  
  /* allocate top thread */
  new->top_thread = JD_alloc_thread( new->thrd_count++ ) ;

#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_mutex_init( &new->m_mutex, NULL ) ;
  pthread_mutex_init( &new->d_mutex, NULL ) ;
  pthread_cond_init( &new->m_cond, NULL ) ;
#endif
  
  return new ;
  
}

/****************************************************************/
/* create thread for Verilog -> jeda call                       */
/****************************************************************/
JD_thread *JD_create_thread( int numarg ) {
  JD_thread *ret ;
  JD_debug_stack *n ;
    
  ret = TMP_THREAD = ALLOC_THREAD ; 
  /* TMP_THREAD is under the GC marking so that we don't lose the allocated
     data in the middle of allocation
   */
  ret->id =  JD_current_toplevel->thrd_count++ ;
  
#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_cond_init( &ret->s_cond, NULL ) ;
  pthread_mutex_init( &ret->l_mutex, NULL ) ;
  pthread_cond_init( &ret->l_cond, NULL ) ;
#endif
 
  ret->main_flag = 0 ;
  
  ret->thread_join = 0 ;
  
  ret->parent = NULL ;
  ret->sibling_fw = NULL ;
  ret->sibling_bw = NULL ;
  ret->entry_func = NULL ;
  
  ret->this = NULL ;
    
  /* temp copy of debug stack, recreated new one and linked at execution */
  ret->dbg_stack = NULL ;
  
  /* link for termination */
  ret->term_parent = NULL ;
  ret->term_fw = NULL ;
  ret->term_bw = NULL ;
  
  
  ret->stack_size = INITAL_STACK_SIZE ;
  while( ret->stack_size < numarg ) ret->stack_size += INITAL_STACK_SIZE ;
  ret->stack = JD_alloc_stack( ret->stack_size ) ;
    
  return ret ;
}


void JD_create_thread_end() {
  /* dummy data for ACC */
  TMP_THREAD->stack[TMP_THREAD->sp++] = JD_alloc_int() ;
  TMP_THREAD = NULL ; /* allocation done, so clear it */
}

/****************************************************************/
/* Wait queue operations                                        */
/****************************************************************/

void JD_enqueue_wait( JD_wait_queue *wq, JD_thread *th ) {
  
  /* node allocation for wait queue */
  if( wq->n == wq->m ) {
    wq->thrd_list = 
      (JD_thread **)realloc( 
                      wq->thrd_list, 
                      sizeof(JD_thread *) * (wq->m + WAIT_QUEUE_SIZE_UNIT) 
                    ) ;
    /*  thread side always remove all the wait queue, so no need to
        keep the location on thread side.. (sure??)
    mq->n_array = (int *)realloc( 
                           mq->n_array,                     
                            sizeof(int *) * 
                              (mq->m + WAIT_QUEUE_SIZE_UNIT) 
                         ) ;
    */
    wq->m += WAIT_QUEUE_SIZE_UNIT ;
  }
  
  /* node allocation for thread side */
  if( th->event_n == th->event_m ) {
    th->event_queue = 
      (JD_wait_queue **)realloc( 
                          th->event_queue, 
                          sizeof(JD_wait_queue *) * 
                            (th->event_m + EVENT_QUEUE_SIZE_UNIT) 
                        ) ;
                        
    th->n_array = (int *)realloc( 
                           th->n_array,                     
                            sizeof(int *) * 
                              (th->event_m + EVENT_QUEUE_SIZE_UNIT) 
                         ) ;
    th->event_m += EVENT_QUEUE_SIZE_UNIT ;
  }
  wq->thrd_list[wq->n] = th ;
  /* wq->n_array[wq->n] = th->event_n ; */
  th->event_queue[th->event_n] = wq ;
  th->n_array[th->event_n++] = wq->n++ ;

}

void JD_dequeue_wait( JD_thread *th ) {
  int qn, tn, j, i, n ;
  JD_wait_queue *wq ;
  
  tn = th->event_n ;
  
  for( j = 0 ; j < tn ; j++ ) {
    wq = th->event_queue[j] ;
    n = wq->n ;
    qn = th->n_array[j] ;
    for( i = qn ; i < n ; i++ ) {
      wq->thrd_list[i] = wq->thrd_list[i+1] ;
    }
  }
  th->event_n = 0 ;
}

/****************************************************************/
/* Internal thread control utilities using pthread lib          */
/****************************************************************/


void JD_thread_exit( JD_thread *this ) {
  /* JD_dequeue( JD_ready_queue, this ) ; */
  JD_enqueue( JD_zombie_queue, this ) ;
  this->queue = JD_zombie_queue ;
  this->status = JD_thrd_zombie ;
  
#ifdef JEDA_SELF_THREAD_CONTROL
  this->lwp->func = JD_sleep_lwp_jump ;
  JD_sleep_lwp( this->lwp ) ;
#else
# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_exit %x unlocking top\n", this ) ; 
# endif
  this->main_flag = 0 ;
  pthread_mutex_unlock( JD_m_mutex ) ;  /* release  main mutex */
  pthread_cond_signal( JD_main_cond ) ; /* signal main to run */
# ifndef JEDA_NO_PTHREAD_EXIT
  pthread_exit(0) ;  /* end of thread */
# endif

#endif

}

void JD_thread_kill( JD_thread *this ) {
  /* JD_dequeue( JD_ready_queue, this ) ; */
  JD_enqueue( JD_zombie_queue, this ) ;
  this->queue = JD_zombie_queue ;
  this->status = JD_thrd_zombie ;
  
#ifdef JEDA_SELF_THREAD_CONTROL
  this->lwp->func = JD_sleep_lwp_jump ;
  JD_sleep_lwp( this->lwp ) ;
#else

# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_kill %x unlocking top\n", this ) ; 
# endif
  this->main_flag = 0 ;
  pthread_mutex_unlock( JD_m_mutex ) ;  /* release  main mutex */
  pthread_cond_signal( JD_main_cond ) ; /* signal main to run */
# ifndef JEDA_NO_PTHREAD_EXIT
  pthread_exit(0) ;  /* end of thread */
# endif

#endif

}

/* put itself to sleep */
void JD_thread_sleep( JD_thread *this ) {
  /* JD_dequeue( JD_ready_queue, this ) ; */
  JD_enqueue( JD_sleep_queue, this ) ;
  this->queue = JD_sleep_queue ;
  this->status = JD_thrd_sleep ;
  
#ifdef JEDA_SELF_THREAD_CONTROL
  this->lwp->func = JD_sleep_lwp_jump ;
  JD_sleep_lwp( this->lwp ) ;
#else

# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_sleep %x unlocking top\n", this ) ;
# endif

  this->main_flag = 0 ;

  pthread_mutex_unlock( JD_m_mutex ) ;
  pthread_cond_signal( JD_main_cond ) ; /* signal main to run */

# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_sleep %x locking self\n", this ) ;
# endif

  pthread_mutex_lock( JD_m_mutex ) ;
  while( !this->main_flag ) {
    pthread_cond_wait( &this->s_cond, JD_m_mutex ) ; /* wait for next run */
  }
  
# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_sleep %x wakeup\n", this ) ;
# endif

#endif
  
  if( this->terminated ) {
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    exit_term_link( this ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_exit( this ) ;
  }
}

void JD_thread_pause( JD_thread *this ) {

#ifdef JEDA_SELF_THREAD_CONTROL
  this->lwp->func = JD_sleep_lwp_jump ;
  JD_sleep_lwp( this->lwp ) ;
#else

#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_pause %x unlocking top\n", this ) ; 
#  endif

  this->main_flag = 0 ;
  pthread_mutex_unlock( JD_m_mutex ) ;
  pthread_cond_signal( JD_main_cond ) ; /* signal main to run */

#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_pause %x locking self\n", this ) ; 
#  endif

  pthread_mutex_lock( JD_m_mutex ) ;
  while( !this->main_flag ) {
    pthread_cond_wait( &this->s_cond, JD_m_mutex ) ; /* wait for next run */
  }
  
#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_pause %x wakeup\n", this ) ; 
#  endif
#endif

  /* at this point, main thread let this child go */
  if( this->terminated ) {
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    exit_term_link( this ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_exit( this ) ;
  }
}

void JD_thread_return() {
#ifdef JEDA_SELF_THREAD_CONTROL
  THREAD->lwp->func = JD_sleep_lwp_jump ;
  JD_sleep_lwp( THREAD->lwp ) ;
#else

#  ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_thread_return %x unlocking top\n", THREAD ) ; 
#  endif
  THREAD->main_flag = 0 ;
  pthread_mutex_unlock( JD_m_mutex ) ;
  pthread_cond_signal( JD_main_cond ) ; /* signal main to run */
#endif

}


/****************************************************************/
/* Copy thread to fork child                                    */
/****************************************************************/
JD_thread *JD_copy_thread( JD_thread *parent ) {
  JD_thread *ret, *child ;
  JD_data *dt, *nt ;
  int i, j ;
  
  ret = TMP_THREAD = ALLOC_THREAD ; 
  /* TMP_THREAD is under the GC marking so that we don't lose the allocated
     data in the middle of allocation
   */
  ret->id =  JD_current_toplevel->thrd_count++ ;
  
#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_cond_init( &ret->s_cond, NULL ) ;
  pthread_mutex_init( &ret->l_mutex, NULL ) ;
  pthread_cond_init( &ret->l_cond, NULL ) ;
#endif
 
  ret->main_flag = 0 ;
  
  ret->thread_join = 0 ;
  
  ret->parent = parent ;
  ret->sibling_fw = NULL ;
  ret->sibling_bw = parent->child ;
  if( parent->child ) parent->child->sibling_fw = ret ;
  parent->child = ret ;
  ret->entry_func = parent->entry_func ;
  
  ret->this = parent->this ;
  
  /* temp copy of debug stack, recreated new one and linked at execution */
  ret->dbg_stack = parent->dbg_stack ;
  
  /* link for termination */
  ret->term_parent = parent ;
  ret->term_fw = NULL ;
  ret->term_bw = parent->term_child ;
  if( parent->term_child ) parent->term_child->term_fw = ret ;
  parent->term_child = ret ;
  
  /* allocate stack */
  i = parent->stack_size - parent->stk_frame ;
  
  ret->stack_size = INITAL_STACK_SIZE ;
  while( ret->stack_size < i ) ret->stack_size += INITAL_STACK_SIZE ;
  ret->stack = JD_alloc_stack( ret->stack_size ) ;
  
  /* copy stack data below the stk_frame */
  for( i = parent->stk_frame ; i < parent->fork_frame ; i++ ) {
    dt = parent->stack[i] ;
    if( IS_SMLINT(dt) ) nt = dt ;
    else if( CLONED(dt) ) {
      nt = JD_duplicate_data( dt ) ;
      /* need to set CLONE flag to inherit the charactor */
      CLONE(nt) ;  /* clone type */
    }
    else {
      nt = dt ;
#ifdef ENABLE_JEDA_DATA_CACHE
      SETDUPLICATED(dt) ;
#endif
    }
    ret->stack[ret->sp++] = nt ;
  }
  /* dummy data for ACC */
  ret->stack[ret->sp++] = JD_alloc_int() ;
  TMP_THREAD = NULL ; /* allocation done, so clear it */
  return ret ;
}


/* function to sync up the execution with the main loop */
void JD_sync_exec(JD_thread *mythread){

#ifndef JEDA_SELF_THREAD_CONTROL

# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_sync_exec %x unlocking self\n", mythread ) ; 
# endif

  pthread_mutex_lock( &mythread->l_mutex ) ;
  mythread->main_flag = 0 ;
  pthread_mutex_unlock( &mythread->l_mutex ) ;
  pthread_cond_signal( &mythread->l_cond ) ; /* signal to the creater */
  pthread_mutex_lock( JD_m_mutex ) ;
  while( !mythread->main_flag ) {
    pthread_cond_wait( &mythread->s_cond, JD_m_mutex ) ; /* wait for next run */
  }
  
# ifdef JEDA_DEBUG_THREAD
    PRINTF( "*** JD_sync_exec %x wakeup\n", mythread ) ; 
# endif
#endif


  if( mythread->terminated ) {
  
#ifdef JEDA_SELF_THREAD_CONTROL
    mythread->lwp->func = JD_sleep_lwp_jump ;
    JD_sleep_lwp( mythread->lwp ) ;
#else
    pthread_mutex_lock( JD_d_mutex ) ;
    exit_term_link( mythread ) ;
    pthread_mutex_unlock( JD_d_mutex ) ;
    JD_thread_exit( mythread ) ;
#endif

  }
}


/****************************************************************/
/* Thread related Instructions                                  */
/****************************************************************/
void JD_exec_fork() {
  /**MACRO EXEC_FORK**/
  {
    CALL_EXEC_FLUSH_VSTACK ;
    THREAD->fork_frame = SP ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
  }
  /**END MACRO EXEC_FORK**/
}

void JD_exec_exec() {
  /**MACRO EXEC_EXEC**/
  {
    JD_thread *thrd ;
    JD_data *dt ;
    
    TMP_DT = dt = JD_alloc_data_entry() ; /* alloc first, to avoid GC */
    dt->type = JD_D_thread ;
    thrd = JD_copy_thread( THREAD ) ;
    /* DBG_PRINT2( TMP_INT, thrd ) ; */
    dt->data.thrd = thrd ;
    thrd->run_index = TMP_INT ;
 
 #ifdef JEDA_SELF_THREAD_CONTROL
    thrd->lwp = JD_create_lwp( (void *)TARGET_FUNC, thrd ) ;
 #endif
    
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    
 #ifndef JEDA_SELF_THREAD_CONTROL
   /* set thread as pthread to execute */
    thrd->main_flag = 1 ;
    pthread_create( 
      &thrd->pth, NULL, (void *)TARGET_FUNC, (void *)thrd 
    ) ;
    pthread_detach( thrd->pth ) ;
    
    pthread_mutex_lock( &thrd->l_mutex ) ;  /* check if thrd is ready */
    while( thrd->main_flag ) {
      pthread_cond_wait( &thrd->l_cond, &thrd->l_mutex ) ; /* wait for ready */
    }
#endif

    PUSH_TO_STACK(dt) ;
  }
  /**END MACRO EXEC_EXEC**/
}

static void exit_term_link( JD_thread *this ) {

  if( !this->term_parent ) return ;

  if( this->term_fw == NULL ) {
    this->term_parent->term_child = this->term_bw ;
    if( this->term_bw ) this->term_bw->term_fw = NULL ;
  }
  else {
    this->term_fw->term_bw = this->term_bw ;
    if( this->term_bw ) 
      this->term_bw->term_fw = this->term_fw ;
  }
  if( this->term_child ) { /* link child to parent */
    JD_thread *thrd = this->term_parent->term_child ;
    if( thrd ) {
      while( thrd->term_bw ) thrd = thrd->term_bw ;
      thrd->term_bw = this->term_child ;
      this->term_child->term_fw = thrd ;
    }
    else {
      this->term_parent->term_child = this->term_child ;
    }
    /* set granma as mam */
    thrd = this->term_child ;
    while( thrd ) {
      thrd->term_parent = this->term_parent ;
      thrd = thrd->term_bw ;
    }
  }
  if( this->term_parent->thread_join && 
      this->term_parent->term_child == NULL )
  {
    this->term_parent->thread_join = 0 ;
    if( this->time_limit ) {
      this->time_limit = 0 ;
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, this ) ;
    }
    JD_dequeue( JD_sleep_queue, this->term_parent ) ;
    this->term_parent->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, this->term_parent ) ;
  }

}

void JD_exec_exit() {
  /**MACRO EXEC_EXIT**/
  {
    JD_thread *thrd, *child ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    if( thrd = THREAD->parent ) {
      if( (--thrd->wait_child) == 0 ) {
        child = thrd->child ;
        while(child) {
          if( child != THREAD ) {
            child->parent = NULL ;
          }
          child = child->sibling_bw ;
        }
        JD_dequeue( JD_sleep_queue, thrd ) ;
        thrd->status = JD_thrd_ready ;
        JD_enqueue( JD_ready_queue, thrd ) ;
        thrd->child = NULL ;
      }
      else {
        if( THREAD->sibling_fw == NULL ) {
          THREAD->sibling_bw->sibling_fw = NULL ;
          THREAD->parent->child = THREAD->sibling_bw ;
        }
        else {
          THREAD->sibling_fw->sibling_bw = THREAD->sibling_bw ;
          if( THREAD->sibling_bw ) 
            THREAD->sibling_bw->sibling_fw = THREAD->sibling_fw ;
          THREAD->parent = NULL ;
        }
      }
    }
    exit_term_link( THREAD ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_exit( THREAD ) ;
  }
  /**END MACRO EXEC_EXIT**/
}

void JD_exec_exit_p_and() {
  /**MACRO EXEC_EXIT_P_AND**/
  {
    JD_thread *thrd, *child ;
    JD_data *dt ;
    int value ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    if( thrd = THREAD->parent ) {
      dt = thrd->stack[thrd->sp-1] ; /* top of stack is acc value */
      if( IS_SMLINT(dt) ) {
        value = SMLINT2INT(dt) ;
        value &= ACC_INT ;
        thrd->stack[thrd->sp-1] = INT2SMLINT(value) ;
      }
      else {
        value = dt->data.int_value ;
        value &= ACC_INT ;
        dt->data.int_value = value ;
      }
      if( value == 0 || (--thrd->wait_child) == 0 ) {
        child = thrd->child ;
        while(child) {
          if( child != THREAD ) {
            child->parent = NULL ;
            child->terminated = 1 ;
          }
          child = child->sibling_bw ;
        }
        JD_dequeue( JD_sleep_queue, thrd ) ;
        thrd->status = JD_thrd_ready ;
        JD_enqueue( JD_ready_queue, thrd ) ;
        thrd->child = NULL ;
      }
      else {
        if( THREAD->sibling_fw == NULL ) {
          THREAD->sibling_bw->sibling_fw = NULL ;
          THREAD->parent->child = THREAD->sibling_bw ;
        }
        else {
          THREAD->sibling_fw->sibling_bw = THREAD->sibling_bw ;
          if( THREAD->sibling_bw ) 
            THREAD->sibling_bw->sibling_fw = THREAD->sibling_fw ;
          THREAD->parent = NULL ;
        }
      }
    }
    exit_term_link( THREAD ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_exit( THREAD ) ;
  }
  /**END MACRO EXEC_EXIT_P_AND**/
}

void JD_exec_exit_p_or() {
  /**MACRO EXEC_EXIT_P_OR**/
  {
    JD_thread *thrd, *child ;
    JD_data *dt ;
    int value ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    if( thrd = THREAD->parent ) {
      dt = thrd->stack[thrd->sp-1] ; /* top of stack is acc value */
      if( IS_SMLINT(dt) ) {
        value = SMLINT2INT(dt) ;
        value |= ACC_INT ;
        thrd->stack[thrd->sp-1] = INT2SMLINT(value) ;
      }
      else {
        value = dt->data.int_value ;
        value |= ACC_INT ;
        dt->data.int_value = value ;
      }
      /* DBG_PRINT5( 
        THREAD, &THREAD->parent, THREAD->parent, 
        dt->data.int_value, thrd->wait_child 
      ) ; */
      if( value || (--thrd->wait_child) == 0 ) {
        child = thrd->child ;
        while(child) {
          if( child != THREAD ) {
            child->parent = NULL ;
            child->terminated = 1 ;
            /* DBG_PRINT4( THREAD, child, &child->parent, child->parent ) ; */
          }
          child = child->sibling_bw ;
        }
        thrd->child = NULL ;
        JD_dequeue( JD_sleep_queue, thrd ) ;
        thrd->status = JD_thrd_ready ;
        JD_enqueue( JD_ready_queue, thrd ) ;
      }
      else {
        if( THREAD->sibling_fw == NULL ) {
          THREAD->sibling_bw->sibling_fw = NULL ;
          THREAD->parent->child = THREAD->sibling_bw ;
        }
        else {
          THREAD->sibling_fw->sibling_bw = THREAD->sibling_bw ;
          if( THREAD->sibling_bw ) 
            THREAD->sibling_bw->sibling_fw = THREAD->sibling_fw ;
          THREAD->parent = NULL ;
        }
      }
    }
    exit_term_link( THREAD ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_exit( THREAD ) ;
  }
  /**END MACRO EXEC_EXIT_P_OR**/
}

void JD_exec_join() {
  /**MACRO EXEC_JOIN**/
  {
    THREAD->wait_child = SP - THREAD->fork_frame ;
    SP = THREAD->fork_frame ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_sleep( THREAD ) ;
  }
  /**END MACRO EXEC_JOIN**/
}

void JD_exec_spoon() {
  /**MACRO EXEC_SPOON**/
  {
    THREAD->wait_child = 1 ;
    SP = THREAD->fork_frame ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
    JD_thread_sleep( THREAD ) ;
  }
  /**END MACRO EXEC_SPOON**/
}

void JD_exec_knife() {
  /**MACRO EXEC_KNIFE**/
  {
    JD_thread *thrd ;
    thrd = THREAD->child ;
    while( thrd ) {
      thrd->parent = NULL ;
      thrd = thrd->sibling_bw ;
    }
    THREAD->child = NULL ;
    SP = THREAD->fork_frame ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif
  }
  /**END MACRO EXEC_KNIFE**/
}

static void terminate_thread( JD_thread *thrd )
{
  JD_thread *cthrd, *nthrd ;
    
  cthrd = thrd->term_child ;
    
  while( cthrd ) {
    nthrd = cthrd->term_bw ;
    terminate_thread( cthrd ) ;
    cthrd = nthrd ;
  }
  
  /* remove from term link, thread stays in the system, but does not
     participate to thread_join()
  */
  thrd->terminated = 1 ;
  thrd->term_parent = NULL ;
  thrd->term_fw = NULL ;
  thrd->term_bw = NULL ;
  thrd->term_child = NULL ;
     
    
}

void JD_exec_terminate() {
  /**MACRO EXEC_TERMINATE**/
  {
    JD_thread *cthrd, *nthrd ;
    
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    cthrd = THREAD->term_child ;
    
    while( cthrd ) {
      nthrd = cthrd->term_bw ;
      terminate_thread( cthrd ) ;
      cthrd = nthrd ;
    }
    
    THREAD->term_child =  NULL ;
    
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ;
#endif

  }
  /**END MACRO EXEC_TERMINATE**/
}


/****************************************************************
 * Thread schedule system functions
 ****************************************************************/

JD_sf_thread_pause() {
  JD_enqueue( JD_ready_queue, THREAD ) ;
  JD_thread_pause(THREAD) ;
}

static int dt_to_int( JD_data *dt ) {
  int ret = -1 ;
  
  if( IS_SMLINT(dt) ) {
    return( SMLINT2INT(dt) ) ;
  }
  if( dt->type == JD_D_int ) {
    ret = dt->data.int_value ;
  }
  else if( dt->type == JD_D_sbit ) {
    if( dt->data.sbit.b == 0 ) ret = dt->data.sbit.a ;
  }
  if( dt->type == JD_D_vbit || dt->type == JD_D_bit ) {
    int wd ;
    int i ;
    wd = LASTWORD(dt->data.bit.size) ;
    for( i = 0 ; i <= wd ; i++ ) {
      if( dt->data.bit.b[i] ) break ;
      if( i && dt->data.bit.a[i] ) break ;
    }
    if( i > wd ) ret = dt->data.bit.a[0] ;
  }
  
  return ret ;
}

JD_sf_thread_join() {
  JD_data *dly ;
  int idly ;

  if( THREAD->term_child ) {
    if( NUM_ARG  ) {
      dly = JD_get_stack(1) ; 
      idly = dt_to_int( dly ) ;
      if( idly > 0 ) {
        THREAD->time_limit = idly ;
        JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
      }
      else THREAD->time_limit = 0 ;
    }
    else THREAD->time_limit = 0 ;
    THREAD->thread_join = 1 ;
    JD_thread_sleep(THREAD) ;
    if( THREAD->timeout ) ACC_INT = 0 ;
    else ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
}


