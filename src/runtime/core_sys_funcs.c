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
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "runtime.h"
#include "runtime_error_msg.h"
#include "mem_macro.h"
#include "data.h"
#include "exec_macro.h"
#include "code_exec_macro.h"
#include "thread.h"
#include "bit_macro.h"

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

extern JD_toplevel *JD_current_toplevel ;
extern JD_queue *JD_ready_queue ;
extern JD_queue *JD_sleep_queue ;
extern JD_queue *JD_zombie_queue ;

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif

void JEDA_exit_simulation() ;

extern int JD_ignore_empty_event_object ;

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

static void dt_to_bit( 
  JD_data *dt, int *size, int *a, int *b, int **bit_a, int **bit_b
) 
{
  *size = -1 ;
  
  if( IS_SMLINT(dt) ) {
    *a = SMLINT2INT(dt) ;
    *b = 0 ;
    *size = 32 ;
    return ;
  }
  if( dt->type == JD_D_int ) {
    *a = dt->data.int_value ;
    *b = 0 ;
    *size = 32 ;
  }
  else if( dt->type == JD_D_sbit ) {
    *a = dt->data.sbit.a ;
    *b = dt->data.sbit.b ;
    *size = dt->data.sbit.size ; 
  }
  if( dt->type == JD_D_vbit || dt->type == JD_D_bit ) {
    if( dt->data.bit.size <= 32 ) {
      *a = dt->data.bit.a[0] ;
      *b = dt->data.bit.b[0] ;
      *size = dt->data.bit.size ; 
    }
    else {
      *bit_a = dt->data.bit.a ;
      *bit_b = dt->data.bit.b ;
      *size = dt->data.bit.size ; 
    }
  }
}

/****************************************************************
 * exit function
 ****************************************************************/

JD_sf_exit() {
    
  JEDA_exit_simulation() ;
  
}

/****************************************************************
 * Unit delay function
 ****************************************************************/

JD_sf_unit_delay() {
  JD_data *dly ;
  int idly ;
  
  dly = JD_get_stack(1) ; 
  idly = dt_to_int( dly ) ;
  if( idly < 0 ) {
    ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 1, "unit_delay" ) ;
  }
  JD_unit_delay( idly ) ;
}


/****************************************************************
 * Port/Signal class member functions
 ****************************************************************/
JD_data *JD_get_ptr_element() ;

JD_sc_port_mf_drive_clock() {
  JD_data *dly, *p1, *p2, *dt ;
  int idly, ip1, ip2 ;
  JD_pli_object *obj ;
  int size ;
  int a, b ;
  int *bit_a, *bit_b ;
  int index = 1 ;
  
  dt = ACC_DT ;
  while( dt->type == JD_D_pointer ) {
    dt = JD_get_ptr_element(dt) ;
  }
  obj = (JD_pli_object *)dt->data.signal.port ;
  if( obj == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  if( NUM_ARG == 4 ) {
    dly = JD_get_stack(index++) ; /* n */
    idly = dt_to_int( dly ) ;
    if( idly < 0 ) {
      ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 4, "drive_clock" ) ;
    }
  }
  else dly = 0 ;
  if( NUM_ARG >= 3 ) {
    dt = JD_get_stack(index++) ; /* n */
    dt_to_bit( dt, &size, &a, &b, &bit_a, &bit_b ) ;
    if( size < 0 ) {
      ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 3, "drive_clock" ) ;
    }
  }
  else {
    size = 1 ;
    a = 0 ;
    b = 0 ;
  }
  if( NUM_ARG >= 2 ) {
    p2 = JD_get_stack(index++) ; /* n */
    ip2 = dt_to_int( p2 ) ;
    if( ip2 < 0 ) {
      ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 2, "drive_clock" ) ;
    }
  }
  p1 = JD_get_stack(index++) ; /* n */
  ip1 = dt_to_int( p1 ) ;
  if( ip1 < 0 ) {
    ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 1, "drive_clock" ) ;
  }
  if( NUM_ARG == 1 ) {
    ip2 = ip1 ;
  }
  if( size <= 32 ) {
    JD_drive_clock( obj, ip1, ip2, &a, &b, size, idly ) ;
  }
  else {
    JD_drive_clock( obj, ip1, ip2, bit_a, bit_a, size, idly ) ;
  }
}

/*****************************************************/
/* functions for event                               */
/*****************************************************/

void JD_event_flush() {
  JD_event_structure *evnt ;
  evnt = JD_current_toplevel->event_chain_top ;
  
  while(evnt) {
    if( evnt->trigger_status == 1 ) {
      evnt->trigger_status = 0 ; 
    }
    evnt = evnt->next ;
  }
}

/* allocate event */
void *JD_sa_event() {
  JD_event_structure *evnt ;
  
  evnt = (void *)calloc( 1, sizeof(JD_event_structure) ) ;
  /* all the events are chained for flush operation */
  evnt->next = JD_current_toplevel->event_chain_top ;
  if( JD_current_toplevel->event_chain_top ) 
    JD_current_toplevel->event_chain_top->prev = evnt ;
  JD_current_toplevel->event_chain_top = evnt ;
  
  evnt->queue = JD_alloc_queue(JD_current_toplevel) ;
  
  return evnt ;
}

#ifdef JEDA_DEBUGGER

char *JD_disp_event( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_event_structure *evnt ;
  int i, len;
  char buf[100] ;
  JD_thread *thrd ;
  
  evnt = (void *)dt->data.obj.ect->table ;
  thrd = evnt->queue->thrd_top ;
  len = 0 ;
  while( thrd ){
    len += strlen(JD_Debug_name_table[thrd->dbg_stack->file_index]) + 100 ;
    thrd = thrd->thrd_bw ; 
  }
  ret = (char *)calloc(strlen(indent)+len+100,sizeof(char) ) ;
  sprintf( 
    ret, "%sstatus %s timeout=%d threads: ", 
    indent, 
    evnt->trigger_status==2?"ON":(evnt->trigger_status==1?"PULSE":"OFF"),
    evnt->timeout
  ) ;
  thrd = evnt->queue->thrd_top ;
  while( thrd ){
    sprintf( 
      buf, "ID(%d) @ %d line %d in \"", 
      thrd->id, thrd->dbg_stack->cycle, thrd->dbg_stack->line_num
    ) ;
    strcat( ret, buf ) ;
    strcat( ret, JD_Debug_name_table[thrd->dbg_stack->file_index]) ;
    thrd = thrd->thrd_bw ; 
    if( thrd ) strcat( ret, "\", " ) ;
    else strcat( ret, "\" " ) ;
  }
  sprintf( buf, "%s", eol ) ;
  strcat( ret, buf ) ;
  return ret ;
}

#endif

void JD_sd_event(JD_event_structure *evnt) {
  JD_thread *thrd ;
  /* if threads are waiting, move them to ready queue with warning */
  while( (thrd = JD_dequeue_top(evnt->queue) ) ) {
    if( thrd->time_limit ) {
      JD_enqueue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
  }
  
  if( evnt->prev ) evnt->prev->next = evnt->next ;
  else JD_current_toplevel->event_chain_top = evnt->next ;
  if( evnt->next ) evnt->next->prev = evnt->prev ;
  JD_dealloc_queue(evnt->queue, JD_current_toplevel) ;
  free(evnt) ;
}

void *JD_dup_event(JD_event_structure *evnt) {
  JD_event_structure *new ;
  new = JD_sa_event() ;
  new->trigger_status = evnt->trigger_status ;
  new->timeout = evnt->timeout ;
  return new ;
}

void JD_sc_event_mf_trigger_pulse() {
  JD_event_structure *evnt ;
  JD_thread *thrd ;
  int loop ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    return ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) return ;
  
  /* trigger_pulse() only activate the threads which is actually waiting */
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  loop = 1 ;
  while( loop && (thrd = JD_dequeue_top(evnt->queue) ) ) {
    if( thrd->time_limit ) {
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    if( thrd->sysflagcount ) {
      return ; /* sync_clear */
    }
  }
}

void JD_sc_event_mf_trigger_on() {
  JD_event_structure *evnt ;
  JD_thread *thrd ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    return ;
  }
  

  /* trigger() only activate the threads which is actually waiting */
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  evnt->trigger_status = 2 ; /* ON */

  while( (thrd = JD_dequeue_top(evnt->queue) ) ) {
    if( thrd->time_limit ) {
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    if( thrd->sysflagcount ) {
      /* event must be cleard */
      evnt->trigger_status = 0 ; /* off */
      return ;
    }
  }

}

void JD_sc_event_mf_trigger_off() {
  JD_event_structure *evnt ;
  JD_thread *thrd ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    return ;
  }
  

  /* trigger() only activate the threads which is actually waiting */
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  evnt->trigger_status = 0 ; /* OFF */

}

void JD_sc_event_mf_trigger() {
  JD_event_structure *evnt ;
  JD_thread *thrd ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    return ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) return ;
  
  /* trigger() only activate the threads which is actually waiting */
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  evnt->trigger_status = 1 ; /* PULSE */
  while( (thrd = JD_dequeue_top(evnt->queue) ) ) {
    if( thrd->time_limit ) {
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    if( thrd->sysflagcount ) {
      /* event must be cleard */
      evnt->trigger_status = 0 ; /* off */
      return ;
    }
  }
}

void JD_sc_event_mf_sync() {
  JD_event_structure *evnt ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( !JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    return ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) {
     ACC_INT = 1 ;
     ACC_TYPE = JD_D_int ;
     return ;
  }
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  if( evnt->trigger_status ) {
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ; /* trigger is active */
  }
  
  JD_enqueue( evnt->queue, THREAD ) ;
  THREAD->queue = evnt->queue ;
  THREAD->status = JD_thrd_sleep ;
  
  if( evnt->timeout ) {
    THREAD->time_limit = evnt->timeout ;
    JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
  }
  else THREAD->time_limit = 0 ;
  
  THREAD->sysflagcount = 0 ;
  
  JD_thread_pause( THREAD ) ;
  if( THREAD->timeout ) ACC_INT = 0 ;
  else ACC_INT = 1 ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_event_mf_sync_clear() {
  JD_event_structure *evnt ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( !JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) {
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  if( evnt->trigger_status ) {
    evnt->trigger_status = 0 ; /* clear it */
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ; /* trigger is active */
  }
  
  JD_enqueue( evnt->queue, THREAD ) ;
  THREAD->queue = evnt->queue ;
  THREAD->status = JD_thrd_sleep ;
  
  if( evnt->timeout ) {
    THREAD->time_limit = evnt->timeout ;
    JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
  }
  else THREAD->time_limit = 0 ;
  
  THREAD->sysflagcount = 1 ;
  
  JD_thread_pause( THREAD ) ;
  if( THREAD->timeout ) ACC_INT = 0 ;
  else ACC_INT = 1 ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_event_mf_check() {
  JD_event_structure *evnt ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( !JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) {

    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  if( evnt->trigger_status ) {
     /* trigger is active */
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
  else {
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }  
}

void JD_sc_event_mf_check_clear() {
  JD_event_structure *evnt ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    if( !JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == JD_NULL ) {

    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  
  if( evnt->trigger_status ) {
     /* trigger is active */
    evnt->trigger_status = 0 ; /* clear it */
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
  else {
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }  
}

void JD_sc_event_mf_timeout() {
  JD_data *dt ;
  JD_event_structure *evnt ;
  JD_thread *thrd ;
  int t ;
  
  dt = GET_STACK(1) ;
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == NULL ) {
    if( !JD_ignore_empty_event_object ) {
      WARNING_EMPTY_EVENT_OBJECT_ACCESS_IGNORED ;
    }
  }
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == (void *)JD_NULL ) {
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  /* trigger() only activate the threads which is actually waiting */
  evnt =  (void *)ACC_DT->data.obj.ect->table ;
  t = dt_to_int(dt) ;
  if( t >= 0 ) evnt->timeout = t ;
  else {
    /* event.timeout(): Illegal argument */
    ERROR_ILLEGAL_ARGUMENT_FOR( "event.timeout()" ) ;
  }
}

/*****************************************************/
/* functions for semaphore                           */
/*****************************************************/
/* allocate semaphore */
void *JD_sa_semaphore() {
  JD_semaphore_structure *sem ;
  
  sem = (void *)calloc( 1, sizeof(JD_semaphore_structure) ) ;
  
  sem->queue = JD_alloc_queue(JD_current_toplevel) ;

  return sem ;

}

#ifdef JEDA_DEBUGGER

char *JD_disp_semaphore( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_semaphore_structure *sem ;
  int i, len;
  char buf[100] ;
  JD_thread *thrd ;
  
  sem = (void *)dt->data.obj.ect->table ;
  thrd = sem->queue->thrd_top ;
  len = 0 ;
  while( thrd ){
    len += strlen(JD_Debug_name_table[thrd->dbg_stack->file_index]) + 100 ;
    thrd = thrd->thrd_bw ; 
  }
  ret = (char *)calloc(strlen(indent)+len+100,sizeof(char) ) ;
  sprintf( 
    ret, "%sunit=%d timeout=%d threads: ", 
    indent, sem->unit, sem->timeout
  ) ;
  thrd = sem->queue->thrd_top ;
  while( thrd ){
    sprintf( 
      buf, "ID(%d) @ %d line %d in \"", 
      thrd->id, thrd->dbg_stack->cycle, thrd->dbg_stack->line_num
    ) ;
    strcat( ret, buf ) ;
    strcat( ret, JD_Debug_name_table[thrd->dbg_stack->file_index]) ;
    thrd = thrd->thrd_bw ; 
    if( thrd ) strcat( ret, "\", " ) ;
    else strcat( ret, "\" " ) ;
  }
  sprintf( buf, "%s", eol ) ;
  strcat( ret, buf ) ;
  return ret ;
}

#endif

void JD_sc_semaphore_mf_new() {
  JD_semaphore_structure *sem ;
  JD_data *dt ;
  int unit ;
  if( ACC_DT->data.obj.ect == NULL ) return ;
  /* new( [int unit = 1] ) */
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    unit = dt_to_int(dt) ;
    if( unit < 0 ) {
      /* event.timeout(): Illegal argument */
      ERROR_ILLEGAL_ARGUMENT_FOR( "semaphore.new()" ) ;
      return ;
    }
  }
  else {
    unit = 1 ;
  }
  sem =  (void *)ACC_DT->data.obj.ect->table ;
  sem->unit = unit ;
  
}

void *JD_dup_semaphore(JD_semaphore_structure *sem) {
  JD_semaphore_structure *new ;
  new = JD_sa_semaphore() ;
  new->unit = sem->unit ;
  new->timeout = sem->timeout ;
  return new ;
}

void JD_sd_semaphore(JD_semaphore_structure *sem) {
  JD_thread *thrd ;
  int unit ;
  /* if threads are waiting, move them to ready queue with warning */
  while( (thrd = JD_dequeue_top(sem->queue) ) ) {
    if( thrd->time_limit ) {
      JD_enqueue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
  }
  
  JD_dealloc_queue(sem->queue, JD_current_toplevel) ;
  free(sem) ;
}


void JD_sc_semaphore_mf_put() {
  JD_data *dt ;
  JD_semaphore_structure *sem ;
  JD_thread *thrd ;
  int unit ;
  
  if( ACC_DT->data.obj.ect == NULL ) return ;
  /* put( [int unit = 1] ) */
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    /* if semaphore == null, all the action is ignored, and put ignored */
    unit = dt_to_int(dt) ;
    if( unit < 0 ) {
      /* event.timeout(): Illegal argument */
      ERROR_ILLEGAL_ARGUMENT_FOR( "semaphore.put()" ) ;
      return ;
    }
  } 
  else unit = 1 ;
  /* trigger() only activate the threads which is actually waiting */
  sem =  (void *)ACC_DT->data.obj.ect->table ;
  unit += sem->unit ;
  while( (thrd = sem->queue->thrd_top) && thrd->sysflagcount <= unit ) {
    thrd = JD_dequeue_top(sem->queue) ;
    unit -= thrd->sysflagcount ;
    if( thrd->time_limit ) {
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    thrd->sysflagcount = 0 ;
  }
  sem->unit = unit ;
}

void JD_sc_semaphore_mf_get() {
  JD_data *dt ;
  JD_semaphore_structure *sem ;
  JD_thread *thrd ;
  int unit ;
  
  /* if semaphore == null, all the action is ignored, and get always
    complete successfully */
  if( ACC_DT->data.obj.ect == NULL ) {
    /* TBI: report null semaphore access */
    return ;
  }
  /* get( [int unit = 1] ) */
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    unit = dt_to_int(dt) ;
    if( unit < 0 ) {
      /* event.timeout(): Illegal argument */
      ERROR_ILLEGAL_ARGUMENT_FOR( "semaphore.get()" ) ;
      return ;
    }
  }
  else unit = 1 ;
  
  sem =  (void *)ACC_DT->data.obj.ect->table ;
  if( sem->unit >= unit ) {
    sem->unit -= unit ;
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
  else {
    JD_enqueue( sem->queue, THREAD ) ;
    THREAD->queue = sem->queue ;
    THREAD->status = JD_thrd_sleep ;
  
    if( sem->timeout ) {
      THREAD->time_limit = sem->timeout ;
      JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
    }
    else THREAD->time_limit = 0 ;
  
    THREAD->sysflagcount = unit ;
    JD_thread_pause( THREAD ) ;
    if( THREAD->timeout ) ACC_INT = 0 ;
    else ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }

}

void JD_sc_semaphore_mf_check() {
  JD_data *dt ;
  JD_semaphore_structure *sem ;
  JD_thread *thrd ;
  int unit ;
  
  /* if semaphore == null, all the action is ignored, and check always
    complete successfully */
  if( ACC_DT->data.obj.ect == NULL ) {
    /* TBI: report null semaphore access */
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  /* check( [int unit = 1] ) */
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    unit = dt_to_int(dt) ;
    if( unit < 0 ) {
      /* event.timeout(): Illegal argument */
      ERROR_ILLEGAL_ARGUMENT_FOR( "semaphore.check()" ) ;
      return ;
    }
  }
  else unit = 1 ;

  sem =  (void *)ACC_DT->data.obj.ect->table ;
  if( sem->unit >= unit ) {
    sem->unit -= unit ;
    ACC_INT = 1 ;
    ACC_TYPE = JD_D_int ;
  }
  else {
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
  }

}

void JD_sc_semaphore_mf_timeout() {
  JD_data *dt ;
  JD_semaphore_structure *sem ;
  JD_thread *thrd ;
  int t ;
  
  /* if event == null, all the action is ignored, and sync handles
     as always ON */
  if( ACC_DT->data.obj.ect == NULL ) return ;
  
  /* trigger() only activate the threads which is actually waiting */
  sem =  (void *)ACC_DT->data.obj.ect->table ;
  dt = GET_STACK(1) ;
  t = dt_to_int(dt) ;
  if( t >= 0 ) sem->timeout = t ;
  else {
    /* event.timeout(): Illegal argument */
    ERROR_ILLEGAL_ARGUMENT_FOR( "semaphore.timeout()" ) ;
  }
}

/*****************************************************/
/* functions for list                                */
/*****************************************************/

/* JD_alloc_data_element: 
   this function is used to allocate the base entry for list data element
 */
JD_data *JD_alloc_data_element( JD_data *dt ) {
  JD_data *ret ;
  ret = TMP_DT = JD_alloc_data_entry() ;
  ret->type = dt->type ;
  ret->sub_type = JD_SD_single ;
  ret->base = dt ;
  switch( dt->type ) {
    case JD_D_sbit:
      ret->data.sbit.size = (int)dt->base ;
      ret->data.sbit.a = 0 ;
      ret->data.sbit.b = 0 ;
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        int i, n ;
        ret->type = JD_D_bit ;
        ret->data.bit.size = (int)dt->base ;
        n = NWORD(ret->data.bit.size) ;
        ret->data.bit.a = ALLOC_BIT( n ) ;
        ret->data.bit.b = ALLOC_BIT( n ) ;
      }
      break ;
    case JD_D_int:
      ret->data.int_value = 0 ;
      break ;
    case JD_D_float:
      ret->data.float_value = 0.0 ;
      break ;
    case JD_D_double:
      ret->data.double_value = 0.0 ;
      break ;
    case JD_D_object:
      ret->data.obj.alloc = (void *)dt->base ;
      ret->data.obj.ect = NULL ;
      break ;
    case JD_D_string:
      ret->data.string = NULL ;
      break ;
    case JD_D_thread:
      ret->data.thrd = NULL ;
      break ;
    case JD_D_signal:
      ret->data.signal.port = NULL ;
      ret->data.signal.data0 = 0 ;
      ret->data.signal.data1 = 0 ;
      break ;
    case JD_D_module:
      ret->data.module.func0 = NULL ;
      ret->data.module.func1 = NULL ;
      ret->data.module.data0 = NULL ;
      break ;
    case JD_D_pointer:
      ret->data.ptr.pdata = NULL ;
      if( dt->data.ptr.string ) 
        ret->data.ptr.string = NULL ;
      ret->data.ptr.offset = 0 ;
      break ;
    case JD_D_func:
      ret->data.func.func = NULL ;
      ret->data.func.data = NULL ;
      break ;
    default:
      break ;
  }
  return ret ;
}

/* JD_store_data_element: 
   this function is used to store a data to the base entry 
   for list data element
 */
void JD_store_data_element( JD_data *dt, JD_data *st ) {
  unsigned int msk ;
  int i, n ;
  switch( dt->type ) {
    case JD_D_sbit:
      msk = BITMASK(dt->data.sbit.size) ;
      if( IS_SMLINT( st ) ) {
        dt->data.sbit.a = SMLINT2INT(st) & msk ;
        dt->data.sbit.b = 0 ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          dt->data.sbit.a = st->data.sbit.a & msk ;
          dt->data.sbit.b = st->data.sbit.b & msk ;
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          dt->data.sbit.a = st->data.bit.a[0] & msk ;
          dt->data.sbit.b = st->data.bit.b[0] & msk ;
          break ;
        case JD_D_int:
          dt->data.sbit.a = st->data.int_value & msk ;
          dt->data.sbit.b = 0 ;
          break ;
        case JD_D_float:
          dt->data.sbit.a = st->data.float_value ;
          dt->data.sbit.a &= msk ;
          dt->data.sbit.b = 0 ;
          break ;
        case JD_D_double:
          dt->data.sbit.a = st->data.double_value ;
          dt->data.sbit.a &= msk ;
          dt->data.sbit.b = 0 ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
          break ;
      }
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      if( IS_SMLINT( st ) ) {
        dt->data.bit.a[0] = SMLINT2INT(st) ;
        dt->data.bit.b[0] = 0 ;
        n = LASTWORD(dt->data.bit.size) ;
        for( i = 1 ; i <= n ; i++ ) {
          dt->data.bit.a[i] = 0 ;
          dt->data.bit.b[i] = 0 ;
        }
      }
      else switch( st->type ) {
        case JD_D_sbit:
          dt->data.bit.a[0] = st->data.sbit.a ;
          dt->data.bit.b[0] = st->data.sbit.b ;
          n = LASTWORD(dt->data.bit.size) ;
          for( i = 1 ; i <= n ; i++ ) {
            dt->data.bit.a[i] = 0 ;
            dt->data.bit.b[i] = 0 ;
          }
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          if (dt->data.bit.size > dt->data.bit.size) {
            n = LASTWORD(dt->data.bit.size) ;
            for( i = 0 ; i <= n ; i++ ) {
              dt->data.bit.a[i] = st->data.bit.a[i] ;
              dt->data.bit.b[i] = st->data.bit.b[i] ;
            }
            n = LASTWORD(dt->data.bit.size) ;
            for( ; i <= n ; i++ ) {
              dt->data.bit.a[i] = 0 ;
              dt->data.bit.b[i] = 0 ;
            }
          }
          else {
            n = LASTWORD(dt->data.bit.size) ;
            msk = BITMASK(dt->data.bit.size) ;
            for( i = 0 ; i < n ; i++ ) {
              dt->data.bit.a[i] = st->data.bit.a[i] ;
              dt->data.bit.b[i] = st->data.bit.b[i] ;
            }
            dt->data.bit.a[n] = dt->data.bit.a[n] & msk ;
            dt->data.bit.b[n] = st->data.bit.b[n] & msk ;
          }
          break ;
        case JD_D_int:
          dt->data.bit.a[0] = st->data.int_value ;
          dt->data.bit.b[0] = 0 ;
          n = LASTWORD(dt->data.bit.size) ;
          for( i = 1 ; i <= n ; i++ ) {
            dt->data.bit.a[i] = 0 ;
            dt->data.bit.b[i] = 0 ;
          }
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
          break ;
      }
      break ;
    case JD_D_int:
      if( IS_SMLINT( st ) ) {
        dt->data.int_value = SMLINT2INT(st) ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          dt->data.int_value = st->data.sbit.a ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
          if( st->data.sbit.b ) {
            WARNING_X_OR_Z_DATA_ASSIGNED_TO_INTEGER ;
          }
#endif
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          dt->data.int_value = st->data.bit.a[0] ;
          n = LASTWORD(st->data.bit.size) ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
          for( i = 0 ; i <= n ; i++ ) {
            if( st->data.bit.b[i] ) {
              WARNING_X_OR_Z_DATA_ASSIGNED_TO_INTEGER ;
              break ;
            }
          }
#endif
          break ;
        case JD_D_int:
          dt->data.int_value = st->data.int_value ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
          break ;
      }
      break ;
    case JD_D_float:
      if( IS_SMLINT( st ) ) {
        dt->data.float_value = SMLINT2INT(st) ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          dt->data.float_value = st->data.sbit.a ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
          if( st->data.sbit.b ) {
            WARNING_X_OR_Z_DATA_ASSIGNED_TO_FLOAT ;
          }
#endif
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          {
            int i, n ;
            double sft ;
            int mask ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
            n = LASTWORD(st->data.bit.size) ;
            for( i = 0 ; i <= n ; i++ ) {
              if( st->data.bit.b[i] ) {
                WARNING_X_OR_Z_DATA_ASSIGNED_TO_FLOAT ;
                n = 0 ;
              }
            }
#endif
            sft = 2.0 ;
            for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
            dt->data.float_value = 0.0 ;
            n = NWORD(st->data.bit.size) ;
            for( i = 0 ; i < n ; i++ ) {
              dt->data.float_value *= sft ;
              dt->data.float_value += st->data.bit.a[i] ;
            }
          }
          break ;
        case JD_D_int:
          dt->data.float_value = st->data.int_value ;
          break ;
        case JD_D_float:
          dt->data.float_value = st->data.float_value ;
          break ;
        case JD_D_double:
          dt->data.float_value = st->data.double_value ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
          break ;
      }
      break ;
    case JD_D_double:
      if( IS_SMLINT( st ) ) {
        dt->data.double_value = SMLINT2INT(st) ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          dt->data.double_value = st->data.sbit.a ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
          if( st->data.sbit.b ) {
            WARNING_X_OR_Z_DATA_ASSIGNED_TO_FLOAT ;
          }
#endif
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          {
            int i, n ;
            double sft ;
            int mask ;
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
            n = LASTWORD(st->data.bit.size) ;
            for( i = 0 ; i <= n ; i++ ) {
              if( st->data.bit.b[i] ) {
                WARNING_X_OR_Z_DATA_ASSIGNED_TO_FLOAT ;
                n = 0 ;
              }
            }
#endif
            sft = 2.0 ;
            for( i = 0 ; i < 32 ; i++ ) sft *= 2.0 ;
            dt->data.double_value = 0.0 ;
            n = NWORD(st->data.bit.size) ;
            for( i = 0 ; i < n ; i++ ) {
              dt->data.double_value *= sft ;
              dt->data.double_value += st->data.bit.a[i] ;
            }
          }
          break ;
        case JD_D_int:
          dt->data.double_value = st->data.int_value ;
          break ;
        case JD_D_float:
          dt->data.double_value = st->data.float_value ;
          break ;
        case JD_D_double:
          dt->data.double_value = st->data.double_value ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
          break ;
      }
      break ;
    case JD_D_object:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
      if( IS_SMLINT( st ) || st->type != JD_D_object ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
      }
#endif
      if( dt->data.obj.ect && (--dt->data.obj.ect->ref_count)==0 ) {
        (dt->data.obj.ect->dealloc)( (void *)dt->data.obj.ect ) ;
        DEALLOC_OBJ_TABLE(dt->data.obj.ect) ;
      }
      dt->data.obj.ect = (void *)st->data.obj.ect ;
      if( dt->data.obj.ect ) dt->data.obj.ect->ref_count++ ;
      break ;
    case JD_D_string:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
      if( IS_SMLINT( st ) || st->type != JD_D_string ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
      }
#endif
      if( dt->data.string ) DEALLOC_STRING(dt->data.string) ;
      dt->data.string = (char *)strdup(st->data.string) ;
      break ;
    case JD_D_thread:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
      if( IS_SMLINT( st ) || st->type != JD_D_thread ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
      }
#endif
      dt->data.thrd = st->data.thrd ;
      break ;
    case JD_D_signal:
#ifndef SKIP_RUNTIME_DATA_TYPE_CHECK
      if( IS_SMLINT( st ) || st->type != JD_D_signal ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_STORE ;
      }
#endif
      dt->data.signal.port = st->data.signal.port ;
      dt->data.signal.data0 = st->data.signal.data0 ;
      dt->data.signal.data1 = st->data.signal.data1 ;
      break ;
    case JD_D_module:
    case JD_D_pointer:
      break ;
    default:
      break ;
  }
}

int JD_cmp_data( JD_data *dt, JD_data *st ) {
  unsigned int msk ;
  int i, n ;
  switch( dt->type ) {
    case JD_D_sbit:
    case JD_D_vbit:
    case JD_D_bit:
      if( IS_SMLINT(st) ) {
        i = SMLINT2INT(st) ;
        A0 = BIT_A(dt) ;
        B0 = BIT_B(dt) ;
        S0 = DT_BIT_SIZE(dt) ;
        A1 = &i ;
        B1 = &INT_ZERO ;
        S1 = 32 ;
#define eval_end  eval_end0
        CALL_BIT_EQEQEQ_BIT ;
        return RET ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
        case JD_D_vbit:
        case JD_D_bit:
          A0 = BIT_A(dt) ;
          B0 = BIT_B(dt) ;
          S0 = DT_BIT_SIZE(dt) ;
          A1 = BIT_A(st) ;
          B1 = BIT_B(st) ;
          S1 = DT_BIT_SIZE(st) ;
#undef eval_end  
#define eval_end  eval_end1
          CALL_BIT_EQEQEQ_BIT ;
          return RET ;
          break ;
        case JD_D_int:
          A0 = BIT_A(dt) ;
          B0 = BIT_B(dt) ;
          S0 = DT_BIT_SIZE(dt) ;
          A1 = &st->data.int_value ;
          B1 = &INT_ZERO ;
          S1 = 32 ;
#undef eval_end  
#define eval_end  eval_end2
          CALL_BIT_EQEQEQ_BIT ;
          return RET ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
          break ;
      }
      break ;
    case JD_D_int:
      if( IS_SMLINT(st) ) {
        i = SMLINT2INT(st) ;
        return (dt->data.int_value == i)?1:0 ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
        case JD_D_vbit:
        case JD_D_bit:
          A0 = &dt->data.int_value ;
          B0 = &INT_ZERO ;
          S0 = 32 ;
          A1 = BIT_A(st) ;
          B1 = BIT_B(st) ;
          S1 = DT_BIT_SIZE(st) ;
#undef eval_end  
#define eval_end  eval_end3
          CALL_BIT_EQEQEQ_BIT ;
          return RET ;
          break ;
        case JD_D_int:
          return (dt->data.int_value == st->data.int_value)?1:0 ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
          break ;
      }
      break ;
    case JD_D_float:
      if( IS_SMLINT(st) ) {
        double flt = SMLINT2INT(st) ;
        return (dt->data.float_value==flt)?1:0 ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          { 
            double flt ;
            if( st->data.sbit.b ) return 0 ;
            flt = st->data.sbit.a ;
            return (dt->data.float_value==flt)?1:0 ;
          }
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          { 
            double flt ;
            if( JD_check_bit_set( st->data.bit.b, st->data.bit.size ) ) {
              return 0 ;
            }
            flt = JD_bit_2_real(st->data.bit.a, st->data.bit.size) ;
            return (dt->data.float_value==flt)?1:0 ;
          }
          break ;
        case JD_D_int:
          { 
            double flt ;
            flt = st->data.int_value ;
            return (dt->data.float_value==flt)?1:0 ;
          }
          break ;
        case JD_D_float:
          return (dt->data.float_value == st->data.float_value)?1:0 ;
          break ;
        case JD_D_double:
          return (dt->data.float_value == st->data.double_value)?1:0 ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
          break ;
      }
      break ;
    case JD_D_double:
      if( IS_SMLINT(st) ) {
        double flt = SMLINT2INT(st) ;
        return (dt->data.double_value==flt)?1:0 ;
      }
      else switch( st->type ) {
        case JD_D_sbit:
          { 
            double flt ;
            if( st->data.sbit.b ) return 0 ;
            flt = st->data.sbit.a ;
            return (dt->data.double_value==flt)?1:0 ;
          }
          break ;
        case JD_D_vbit:
        case JD_D_bit:
          { 
            double flt ;
            if( JD_check_bit_set( st->data.bit.b, st->data.bit.size ) ) {
              return 0 ;
            }
            flt = JD_bit_2_real(st->data.bit.a, st->data.bit.size) ;
            return (dt->data.double_value==flt)?1:0 ;
          }
          break ;
        case JD_D_int:
          { 
            double flt ;
            flt = st->data.int_value ;
            return (dt->data.double_value==flt)?1:0 ;
          }
          break ;
        case JD_D_float:
          return (dt->data.double_value == st->data.float_value)?1:0 ;
          break ;
        case JD_D_double:
          return (dt->data.double_value == st->data.double_value)?1:0 ;
          break ;
        default:
          ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
          break ;
      }
      break ;
    case JD_D_object:
      if( IS_SMLINT(st) || st->type != JD_D_object ) 
        ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
      return (dt->data.obj.ect == st->data.obj.ect)?1:0 ;
      break ;
    case JD_D_string:
      if( IS_SMLINT(st) || st->type != JD_D_string ) 
        ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
      if( dt->data.string == NULL ) {
        return (st->data.string == NULL)?1:0 ;
      }
      else {
        if( st->data.string == NULL ) return 0 ;
        if( !strcmp(dt->data.string, st->data.string) ) return 1 ;
        else return 0 ;
      }
      break ;
    case JD_D_thread:
      if( IS_SMLINT(st) || st->type != JD_D_thread ) 
        ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
      return (dt->data.thrd == st->data.thrd)?1:0 ;
      break ;
    case JD_D_signal:
      if( IS_SMLINT(st) || st->type != JD_D_signal ) 
        ERROR_ILLEGAL_DATA_TYPE_FOR_COMPARE ;
      if (
        dt->data.signal.port == st->data.signal.port &&
        dt->data.signal.data0 == st->data.signal.data0 &&
        dt->data.signal.data1 == st->data.signal.data1
      ) return 1 ;
      else return 0 ;
      break ;
    case JD_D_module:
    case JD_D_pointer:
      break ;
    default:
      break ;
  }
}


/* thread is waiting for receiving data */
static void wakeup_thread_in_mbox( JD_data *dt ) {
  JD_data *st ;
  JD_data_unit *ut ;
  JD_thread *thrd ;
  
  
  if( ACC_DT->data.list.sem->queue->thrd_top ) {
    ut =  dt->data.list.top ;
    if( ut == NULL ) {
      dt = JD_alloc_data_element( ACC_DT ) ;
      WARNING_POP_DATA_FROM_EMPTY_LIST ;
    }
    else {
      ACC_DT->data.list.top = ut->next ;
      if( ut->next ) ut->next->pre = NULL ;
      else ACC_DT->data.list.bottom = NULL ;
      dt = ut->data ;
      DEALLOC_DATA_UNIT(ut,1) ;
    }
    thrd = JD_dequeue_top(ACC_DT->data.list.sem->queue) ;
    if( thrd->time_limit ) {
      JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
    }
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_ready_queue, thrd ) ;
    thrd->sysflagcount = 0 ;
    thrd->stack[thrd->sp-1] = dt ; /* set acc with data */
  }
  
}

/************************************************************/
/* list.push( data )                                        */
/*   ACC : list data, STACK[-1]:data                        */
/************************************************************/
void JD_sc_list_mf_push()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  
  dt =  JD_alloc_data_element( ACC_DT ) ;
  st = JD_get_stack(1) ; /* data */
  
  JD_store_data_element( dt, st ) ;
  
  ut = ALLOC_DATA_UNIT(1) ;
  
  ut->data = dt ;
  
  ut->next = ACC_DT->data.list.top ;
  if( ut->next ) ut->next->pre = ut ;
  ACC_DT->data.list.top = ut ;
  if( ACC_DT->data.list.bottom == NULL ) ACC_DT->data.list.bottom = ut ;
  /*
  printf( " list_push:" ) ;
  ut =  ACC_DT->data.list.top ;
  while( ut != NULL ) {
    printf( "(%d %d) ", ut->data->type, ut->data->data.int_value ) ;
    ut = ut->next ;
  }
  printf( "\n" ) ;
  */
  if( ACC_DT->data.list.sem ) {
    wakeup_thread_in_mbox( ACC_DT ) ;
  }


}

void JD_sc_list_mf_bpush()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  
  dt =  JD_alloc_data_element( ACC_DT ) ;
  st = JD_get_stack(1) ; /* data */
  
  JD_store_data_element( dt, st ) ;
  
  ut = ALLOC_DATA_UNIT(1) ;
  
  ut->data = dt ;
  
  ut->pre = ACC_DT->data.list.bottom ;
  if( ut->pre ) ut->pre->next = ut ;
  ACC_DT->data.list.bottom = ut ;
  if( ACC_DT->data.list.top == NULL ) ACC_DT->data.list.top = ut ;
  
  if( ACC_DT->data.list.sem ) {
    wakeup_thread_in_mbox( ACC_DT ) ;
  }
}
  
void JD_sc_list_mf_pop()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  
  ut =  ACC_DT->data.list.top ;
  if( ut == NULL ) {
    dt = JD_alloc_data_element( ACC_DT ) ;
    WARNING_POP_DATA_FROM_EMPTY_LIST ;
  }
  else {
    ACC_DT->data.list.top = ut->next ;
    if( ut->next ) ut->next->pre = NULL ;
    else ACC_DT->data.list.bottom = NULL ;
    dt = ut->data ;
    DEALLOC_DATA_UNIT(ut,1) ;
  }
  /* ut =  ACC_DT->data.list.top ; */
  CALL_EXEC_SET_ACC(dt) ;
  /*
  printf( " list_pop:" ) ;
  while( ut != NULL ) {
    printf( "(%d %d) ", ut->data->type, ut->data->data.int_value ) ;
    ut = ut->next ;
  }
  printf( "\n" ) ;
  */
}

void JD_sc_list_mf_bpop()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  
  ut =  ACC_DT->data.list.bottom ;
  if( ut == NULL ) {
    dt = JD_alloc_data_element( ACC_DT ) ;
    WARNING_POP_DATA_FROM_EMPTY_LIST ;
  }
  else {
    ACC_DT->data.list.bottom = ut->pre ;
    if( ut->pre ) ut->pre->next = NULL ;
    else ACC_DT->data.list.top = NULL ;
    dt = ut->data ;
    DEALLOC_DATA_UNIT(ut,1) ;
  }
  CALL_EXEC_SET_ACC(dt) ;

}

void JD_sc_list_mf_num_element()
{
  JD_data_unit *ut ;
  int n ;
  
  n = 0 ;
  ut =  ACC_DT->data.list.top ;
  while( ut ) {
    n++ ;
    ut = ut->next ;
  }
  ACC_INT = n ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_list_mf_send()
{
  JD_sc_list_mf_push() ;
}

void JD_sc_list_mf_receive()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  JD_semaphore_structure *sem ;
  
  if( ACC_DT->data.list.bottom == NULL ) {
    if( ACC_DT->data.list.sem == NULL ) {
      ACC_DT->data.list.sem = JD_sa_semaphore() ;
      ACC_DT->data.list.sem->unit = 1 ; /* specify this is used for mbox */
    }
    sem = ACC_DT->data.list.sem ;
    if( sem->unit && sem->unit != 1 ) {
      ERROR_LIST_RECEIVE_IS_CALLED_ON_LIST_IN_NON_MAILBOX_MODE ;
    }
    sem->unit = 1 ;
    JD_enqueue( sem->queue, THREAD ) ;
    THREAD->queue = sem->queue ;
    THREAD->status = JD_thrd_sleep ;
  
    if( sem->timeout ) {
      THREAD->time_limit = sem->timeout ;
      JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
    }
    else THREAD->time_limit = 0 ;
  
    THREAD->sysflagcount = 0 ;
    
    /* set dummy data in the case of timeout */
    dt = JD_alloc_data_element( ACC_DT ) ;
    CALL_EXEC_SET_ACC(dt) ;
    
    JD_thread_pause( THREAD ) ;
  }
  else {
    ut =  ACC_DT->data.list.bottom ;
    ACC_DT->data.list.bottom = ut->pre ;
    if( ut->pre ) ut->pre->next = NULL ;
    else ACC_DT->data.list.top = NULL ;
    dt = ut->data ;
    DEALLOC_DATA_UNIT(ut,1) ;
    CALL_EXEC_SET_ACC(dt) ;
  }

}

void JD_sc_list_mf_timeout()
{
  JD_data *dt ;
  int t ;
    
  if( ACC_DT->data.list.sem == NULL ) {
    ACC_DT->data.list.sem = JD_sa_semaphore() ;
    ACC_DT->data.list.sem->unit = 0 ; /* */
  }
  dt = GET_STACK(1) ;
  t = dt_to_int(dt) ;
  if( t >= 0 ) ACC_DT->data.list.sem->timeout = t ; /* */
  else {
    /* event.timeout(): Illegal argument */
    ERROR_ILLEGAL_ARGUMENT_FOR( "list.timeout()" ) ;
  }
}


void JD_sc_list_mf_enter()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  JD_semaphore_structure *sem ;
  int flag ;
  int i ;
  
  THREAD->timeout = 0 ;

  flag = 0 ;
  for( i = 0 ; (i < NUM_ARG) && (flag == 0) ; i++ ) {
    dt = STACK[SP-i-1] ;
    ut = ACC_DT->data.list.top ;
    while( ut ) {
      st = ut->data ;
      if( JD_cmp_data( dt, st ) ) {
        flag = 1 ;
        break ;
      }
      ut = ut->next ;
    }
  }
  if( flag ) {
    if( ACC_DT->data.list.sem == NULL ) {
      ACC_DT->data.list.sem = JD_sa_semaphore() ;
      ACC_DT->data.list.sem->unit = 2 ; /* specify this is used for mbox */
    }
    sem = ACC_DT->data.list.sem ;
    if( sem->unit && sem->unit != 2 ) {
      ERROR_LIST_ENTER_IS_CALLED_ON_LIST_IN_MAILBOX_MODE ;
    }
    sem->unit = 2 ;
    JD_enqueue( sem->queue, THREAD ) ;
    THREAD->queue = sem->queue ;
    THREAD->status = JD_thrd_sleep ;
  
    if( sem->timeout ) {
      THREAD->time_limit = sem->timeout ;
      JD_enqueue_timeq( &JD_current_toplevel->time_queue, THREAD ) ;
    }
    else THREAD->time_limit = 0 ;
  
    THREAD->sysflagcount = NUM_ARG ;
    
    /* set dummy data in the case of timeout */
    dt = JD_alloc_data_element( ACC_DT ) ;
    CALL_EXEC_SET_ACC(dt) ;
    
    
    JD_thread_pause( THREAD ) ;
    
    NUM_ARG = THREAD->sysflagcount ;
    
  }
  
  if( THREAD->timeout == 0 ) {
    for( i = 0 ; i < NUM_ARG ; i++ ) {
      dt =  JD_alloc_data_element( ACC_DT ) ;
      st = JD_get_stack(i+1) ; /* data */
  
      JD_store_data_element( dt, st ) ;
  
      ut = ALLOC_DATA_UNIT(1) ;
  
      ut->data = dt ;
  
      ut->pre = ACC_DT->data.list.bottom ;
      if( ut->pre ) ut->pre->next = ut ;
      ACC_DT->data.list.bottom = ut ;
      if( ACC_DT->data.list.top == NULL ) ACC_DT->data.list.top = ut ;
      
    }
    ACC_TYPE = JD_D_int ;
    ACC_INT = 1 ;
  }
  else {
    ACC_TYPE = JD_D_int ;
    ACC_INT = 0 ;
  }
  
}

void JD_sc_list_mf_check_enter()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  JD_semaphore_structure *sem ;
  int flag ;
  int i ;
    
  flag = 0 ;
  for( i = 0 ; (i < NUM_ARG) && (flag == 0) ; i++ ) {
    dt = STACK[SP-i-1] ;
    ut = ACC_DT->data.list.top ;
    while( ut ) {
      st = ut->data ;
      if( JD_cmp_data( st, dt ) ) {
        flag = 1 ;
        break ;
      }
      ut = ut->next ;
    }
  }
  if( !flag ) {
    for( i = 0 ; i < NUM_ARG ; i++ ) {
      dt =  JD_alloc_data_element( ACC_DT ) ;
      st = JD_get_stack(i+1) ; /* data */
  
      JD_store_data_element( dt, st ) ;
  
      ut = ALLOC_DATA_UNIT(1) ;
  
      ut->data = dt ;
  
      ut->pre = ACC_DT->data.list.bottom ;
      if( ut->pre ) ut->pre->next = ut ;
      ACC_DT->data.list.bottom = ut ;
      if( ACC_DT->data.list.top == NULL ) ACC_DT->data.list.top = ut ;
  
    }
    ACC_TYPE = JD_D_int ;
    ACC_INT = 1 ;
  }
  else {
    ACC_TYPE = JD_D_int ;
    ACC_INT = 0 ;
  }
  
}


/* list_re_enter: this function is called to check if the thread
   in the queue is OK to be entered.
   
 */
static void list_re_enter()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  JD_semaphore_structure *sem ;
  JD_thread *thrd, *nthrd ;
  int flag ;
  int i ;
  
  sem = ACC_DT->data.list.sem ;
  
  thrd = sem->queue->thrd_top ;
  while (thrd) {
    nthrd = thrd->thrd_bw ;
    flag = 0 ;
    for( i = 0 ; (i < thrd->sysflagcount) && (flag == 0) ; i++ ) {
      dt = thrd->stack[thrd->sp-i-1] ;
      ut = ACC_DT->data.list.top ;
      while( ut ) {
        st = ut->data ;
        if( JD_cmp_data( st, dt ) ) {
          flag = 1 ;
          break ;
        }
        ut = ut->next ;
      }
    }
    if( !flag ) {
      /* OK to enter */
      JD_dequeue( sem->queue, thrd ) ;
      if( thrd->time_limit ) {
        JD_dequeue_timeq( &JD_current_toplevel->time_queue, thrd ) ;
      }
      thrd->status = JD_thrd_ready ;
      JD_enqueue( JD_ready_queue, thrd ) ;
    }
    thrd = nthrd ;
  }
  
}

void JD_sc_list_mf_exit()
{
  JD_data *dt, *st ;
  JD_data_unit *ut, *nt ;
  JD_semaphore_structure *sem ;
  int flag ;
  int i ;
  
  for( i = 0 ; i < NUM_ARG ; i++ ) {
    dt = STACK[SP-NUM_ARG+i] ;
        
    ut = ACC_DT->data.list.top ;
    flag = 0 ;
    while( ut ) {
      nt = ut->next ;
      st = ut->data ;
      if( JD_cmp_data( st, dt ) ) {
        if( ut->pre ) ut->pre->next = ut->next ;
        else ACC_DT->data.list.top = ut->next ;
        if( ut->next ) ut->next->pre = ut->pre ;
        else ACC_DT->data.list.bottom = ut->pre ;
        flag = 1 ;
        DEALLOC_DATA_UNIT(ut,1) ;
        break ;
      }
      ut = nt ;
    }
    if( !flag ) {
      ERROR_LIST_EXIT_DATA_NTH_NOT_FOUND_IN_THE_LIST(i+1) ;
    }
  }
  if( ACC_DT->data.list.sem ) {
    list_re_enter() ;
  }
    printf( "\n" ) ;
}

void JD_sc_list_mf_remove()
{
  JD_data *dt, *st ;
  JD_data_unit *ut ;
  int i, n ;
  
  dt = JD_get_stack(1) ; 
  n = dt_to_int( dt ) ;
  if( n < 0 ) {
    ERROR_ILLEGAL_NTH_ARGMENT_FOR_SYSFUNC( 1, "list.remove" ) ;
  }
  ut =  ACC_DT->data.list.top ;
  if( ut == NULL ) {
    WARNING_ACCESS_TO_EMPTY_LIST_LOCATION ;
  }
  else {
    for( i = 0 ; i < n && ut ; i++ ) {
      ut = ut->next ;
    }
    if( ut ) {
      if( ut->pre ) ut->pre->next = ut->next ;
      else ACC_DT->data.list.top = ut->next ;
      if( ut->next ) ut->next->pre = ut->pre ;
      else ACC_DT->data.list.bottom = ut->pre ;
      DEALLOC_DATA_UNIT(ut,1) ;
    }
    else {
      WARNING_ACCESS_TO_EMPTY_LIST_LOCATION ;
    }
  }

}


/*********************************************************************
 * Implement quick sort for list data       
 *********************************************************************/

/* 
 compare_data compares dt & st, returns -1 if dt<st, 0 if dt==st, 1 if dt>st
 */
static int compare_data( JD_data *dt, JD_data *st ) {
  int ret ;
  switch( dt->type ) { /* assumes dt & st has the same type */
    case JD_D_sbit:
      if( dt->data.sbit.b || st->data.sbit.b ) {
        WARNING_X_VALUE_DETECTED_IN_SORT ;
        return 0 ;
      }
      if( dt->data.sbit.a == st->data.sbit.b ) ret = 0 ;
      else if( dt->data.sbit.a > st->data.sbit.a ) ret = 1 ;
      else ret = -1 ;
      break ;
    case JD_D_bit:
      A0 = BIT_A(dt) ;
      B0 = BIT_B(dt) ;
      S0 = DT_BIT_SIZE(dt) ;
      A1 = BIT_A(st) ;
      B1 = BIT_B(st) ;
      S1 = DT_BIT_SIZE(st) ;
#undef eval_end  
#define eval_end  eval_end0
      CALL_BIT_EQEQ_BIT ;
      if( RET < 0 ) {
        WARNING_X_VALUE_DETECTED_IN_SORT ;
        ret = 0 ;
      }
      else if( RET ) ret = 0 ;
      else {
        A0 = BIT_A(dt) ;
        B0 = BIT_B(dt) ;
        S0 = DT_BIT_SIZE(dt) ;
        A1 = BIT_A(st) ;
        B1 = BIT_B(st) ;
        S1 = DT_BIT_SIZE(st) ;
#undef eval_end  
#define eval_end  eval_end1
        CALL_BIT_LT_BIT ;
        if( RET ) ret = -1 ;
        else ret = 1 ; 
      }
      break ;
    case JD_D_int:
      if (dt->data.int_value == st->data.int_value) ret = 0 ;
      else if (dt->data.int_value < st->data.int_value) ret = -1 ;
      else ret = 1 ;
      break ;
    case JD_D_float:
      if (dt->data.float_value == st->data.float_value) ret = 0 ;
      else if (dt->data.float_value < st->data.float_value) ret = -1 ;
      else ret = 1 ;
      break ;
    case JD_D_double:
      if (dt->data.double_value == st->data.double_value) ret = 0 ;
      else if (dt->data.double_value < st->data.double_value) ret = -1 ;
      else ret = 1 ;
      break ;
    case JD_D_string:
      if( dt->data.string == NULL ) {
        ret = (st->data.string == NULL)?0:-1 ;
      }
      else {
        if( st->data.string == NULL ) ret = 1 ;
        else ret = strcmp(dt->data.string, st->data.string) ;
      }
      break ;
    default:
      WARNING_ILLEGAL_DATA_TYPE_FOR_SORT ;
      ret = 0 ;
      break ;
  }
  
  return ret ;
}

/*********************************************************************
 * those following two functions are the famous quick sort functions
 * you learned at the Algorithm class.
 *********************************************************************/
static int partition( 
  JD_data_unit **ua, int left, int right 
) {
  int lo, high ;
  JD_data_unit *pivot, *tmp ;
  int done = 0 ;
  
  lo = left+1 ;
  high = right ;
  pivot = ua[left] ;
  while( !done ) {
    while( compare_data(ua[lo]->data, pivot->data) < 0 ) {
      lo++ ;
      if ( lo > right ) break ;
    }
    while( compare_data(ua[high]->data, pivot->data) > 0 ) {
      high-- ;
      if( high < 0 ) break ;
    }
    if( lo < high ) {
      /* swap lo & high */
      tmp = ua[lo] ;
      ua[lo] = ua[high] ;
      ua[high] = tmp ;
    }
    else done = 1 ;
  }
  if( high >= 0 ) {
    /* swap left & high */
    tmp = ua[left] ;
    ua[left] = ua[high] ;
    ua[high] = tmp ;
  }
  return high ;
}
  
static void quicksort( 
  JD_data_unit **ua, int left, int right, int level 
) {
  int pivot_pos ;
  int i ;
  
  /*
  for( i = 0 ; i < level ; i++ ) printf( " " ) ;
  printf( "quicksort level: %d left:%d right:%d \n", level, left, right ) ;
  for( i = 0 ; i < level ; i++ ) printf( " " ) ;
  for( i = 0 ; i < 8 ; i++ ) 
    printf( "%d ", ua[i]->data->data.int_value ) ;
  printf( "\n" ) ;
  */
  
  if( left < right ) {
    pivot_pos = partition( ua, left, right ) ;
    /*
    for( i = 0 ; i < level ; i++ ) printf( " " ) ;
    for( i = 0 ; i < 8 ; i++ ) 
      printf( "%d ", ua[i]->data->data.int_value ) ;
    printf( "\n" ) ;
    */
    if( pivot_pos >= 0 ) {
      quicksort( ua, left, pivot_pos-1, level+1 ) ;
      quicksort( ua, pivot_pos+1, right, level+1 ) ;
    }
  }

}
  
void JD_sc_list_mf_sort()
{
  JD_data *dt, *st ;
  JD_data_unit *ut, **ua ;
  int flag ;
  int i, n ;
  int pivot_pos ;
  
  ut = ACC_DT->data.list.top ;
  
  n = 0 ;
  while( ut ) {
    n++ ;
    ut = ut->next ;
  }
  
  if( !n ) return ;
  
  /* create an array for sorting */
  ua = (JD_data_unit **)calloc( n, sizeof(JD_data_unit *) ) ;
  
  ut = ACC_DT->data.list.top ;
  for( i = 0 ; i < n ; i++ ) {
    ua[i] = ut ;
    ut = ut->next ;
  }
  
  quicksort( ua, 0, n-1, 0 ) ;
  
  /* re-chain the list on the sorted result */
  
  for( i = 0 ; i < n ; i++ ) {
    ua[i]->next = (i<(n-1))?ua[i+1]:NULL ; 
    ua[i]->pre = (i)?ua[i-1]:NULL ;
  }
  
  ACC_DT->data.list.top = ua[0] ;
  ACC_DT->data.list.bottom = ua[n-1] ;
   
  free( ua ) ;
  
}
  
  
/********************************************************************/
/* Array member functions                                           */
/********************************************************************/
void JD_sc_array_mf_num_element() {
  ACC_INT = ACC_DT->data.array.size ;
  ACC_TYPE = JD_D_int ;
}

/********************************************************************/
/* Multi-dimentional Array member functions                         */
/********************************************************************/
void JD_sc_marray_mf_num_element() {
  int i, n, d ;
  ACC_INT = ACC_DT->data.array.size ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_marray_mf_dimension() {
  ACC_INT = ACC_DT->data.array.dim_size[0] ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_marray_mf_dim_size( /* int n */ ) {
  JD_data *st ;
  int n ;
  
  ACC_INT = 0 ;
  ACC_TYPE = JD_D_int ;
  st = JD_get_stack(1) ; /* n */
  n = dt_to_int(st) ;
  if( n >= ACC_DT->data.array.dim_size[0] ) {
    ERROR_DIMENSION_TOO_LARGE ;
    return ;
  }
  ACC_INT = ACC_DT->data.array.dim_size[n+1] ;
}


/********************************************************************/
/* Associative Array member functions                               */
/********************************************************************/
int count_hash_entry( JD_hash_entry *hash ) {
  int ret = 1 ; /* self */
  if( hash->left ) ret += count_hash_entry( hash->left ) ;
  if( hash->right ) ret += count_hash_entry( hash->right ) ;
  return ret ;
}

/* generic function used by debugger */
int JD_count_hush_element( JD_data *dt ) {
  int i, n ;
  n = 0 ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( dt->data.hash.table[i] ) {
      n += count_hash_entry( dt->data.hash.table[i] ) ;
    }
  }
  return n ;
}

void JD_sc_assoc_mf_num_element() {
  int i, n ;
  n = 0 ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( ACC_DT->data.hash.table[i] ) {
      n += count_hash_entry( ACC_DT->data.hash.table[i] ) ;
    }
  }
  ACC_INT = n ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_assoc_mf_check_index() {
  JD_data *st ;
  int n ;
  JD_hash_entry *hsh ;
  int hs ;
  st = JD_get_stack(1) ; /* n */
  n = dt_to_int(st) ;
  hs = CALC_DATA_HASH(n) ;
  hsh = ACC_DT->data.hash.table[hs] ;
  while( hsh ) {
    if( hsh->idx == n ) break ;
    if( hsh->idx > n ) hsh = hsh->right ;
    else hsh = hsh->left ;
  }
  if( hsh ) ACC_INT = 1 ;
  else ACC_INT = 0 ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_assoc_mf_delete_index() {
  JD_data *st ;
  int n ;
  JD_hash_entry *hsh, **phsh, *rhsh, *lhsh ;
  int hs ;
  int deleted = 0 ;
  st = JD_get_stack(1) ; /* n */
  n = dt_to_int(st) ;
  hs = CALC_DATA_HASH(n) ;
  hsh = ACC_DT->data.hash.table[hs] ;
  phsh = &ACC_DT->data.hash.table[hs] ;
  while( hsh ) {
    if( hsh->idx == n ) {
      rhsh = hsh->right ;
      lhsh = hsh->left ;
      if( rhsh ) {
        /* connect right node to its parent, then connect left node to
           the left most node under the right node 
         */
        *phsh = rhsh ;
        if( lhsh ) {
          while ( rhsh->left ) rhsh= rhsh->left ;
          rhsh->left = lhsh ;
        }
      }
      else if( lhsh ) {
        *phsh = lhsh ;
      }
      else *phsh = NULL ;
      /* data will be garbage-collected later */
      DEALLOC_HASH_ENTRY(hsh) ;
      hsh = NULL ;
      deleted = 1 ;
    }
    else {
      if( hsh->idx > n ) {
        phsh = &hsh->right ;
        hsh = hsh->right ;
      }
      else {
        phsh = &hsh->left ;
        hsh = hsh->left ;
      }
    }
  }
  if( !deleted ) {
    WARNING_INDEX_NOT_FOUND_IN_DELETE_INDEX_FUNC ;
  }
}

static void gen_int_index_list( JD_hash_entry *hsh, JD_data *dt ) {
  JD_data_unit *ut, *nt ;
  JD_data *st ;
  int i, n ;
  n = hsh->idx ;
  st = JD_alloc_data_entry() ;
  st->type = JD_D_int ;
  st->sub_type = JD_SD_single ;
  st->data.int_value = n ;
  st->base = dt ;
  ut = ALLOC_DATA_UNIT(1) ;
  ut->data = st ;
  nt = dt->data.list.top ;
  dt->data.list.top = ut ;
  ut->next = nt ;
  if( nt ) nt->pre = ut ;
  if( hsh->right ) {
    gen_int_index_list( hsh->right, dt ) ;
  }
  if( hsh->left ) {
    gen_int_index_list( hsh->left, dt ) ;
  }
}

JD_sc_assoc_mf_index_list() 
{
  int i ;
  TMP_DT = JD_alloc_data_entry() ;
  TMP_DT->type = JD_D_int ;
  TMP_DT->sub_type = JD_SD_list ;
  TMP_DT->data.list.top = NULL ;
  TMP_DT->data.list.bottom = NULL ;
  TMP_DT->data.list.sem = NULL ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( ACC_DT->data.hash.table[i] ) {
      gen_int_index_list( ACC_DT->data.hash.table[i], TMP_DT )  ;
    }
  }
  ACC_DT = TMP_DT ;
  ACC_TYPE = JD_D_dt ;
}

static void flush_an_entry( JD_hash_entry *hsh ) {
  if( hsh->left ) flush_an_entry( hsh->left ) ;
  if( hsh->right ) flush_an_entry( hsh->right ) ;
  /* data will be garbage-collected later */
  DEALLOC_HASH_ENTRY(hsh) ;
}

JD_sc_assoc_mf_flush_all()
{
  int i, n ;
  JD_data_unit *ut ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( ACC_DT->data.hash.table[i] ) {
      flush_an_entry( ACC_DT->data.hash.table[i] ) ;
      ACC_DT->data.hash.table[i] = NULL ;
    }
  }
}


      
JD_sc_stridx_mf_num_element() {
  JD_sc_assoc_mf_num_element() ; /* same as assoc array */
}
      
JD_sc_stridx_mf_check_index() 
{
  JD_data *st ;
  int n ;
  JD_hash_entry *hsh ;
  int hs ;
  st = JD_get_stack(1) ; /* n */
  CALC_STRING_HASH(hs,st->data.string) ;
  hsh = ACC_DT->data.hash.table[hs] ;
  while( hsh ) {
    n = strcmp( hsh->string, st->data.string ) ;
    if( n == 0 ) break ;
    if( n > 0 ) hsh = hsh->right ;
    else hsh = hsh->left ;
  }
  if( hsh ) ACC_INT = 1 ;
  else ACC_INT = 0 ;
  ACC_TYPE = JD_D_int ;
}
        
JD_sc_stridx_mf_delete_index()
{
  JD_data *st ;
  int n ;
  JD_hash_entry *hsh, **phsh, *rhsh, *lhsh ;
  int hs ;
  int deleted = 0 ;
  st = JD_get_stack(1) ; /* n */
  CALC_STRING_HASH(hs,st->data.string) ;
  hsh = ACC_DT->data.hash.table[hs] ;
  phsh = &ACC_DT->data.hash.table[hs] ;
  while( hsh ) {
    n = strcmp( hsh->string, st->data.string ) ;
    if( n == 0 ) {
      rhsh = hsh->right ;
      lhsh = hsh->left ;
      if( rhsh ) {
        /* connect right node to its parent, then connect left node to
           the left most node under the right node 
         */
        *phsh = rhsh ;
        if( lhsh ) {
          while ( rhsh->left ) rhsh= rhsh->left ;
          rhsh->left = lhsh ;
        }
      }
      else if( lhsh ) {
        *phsh = lhsh ;
      }
      else *phsh = NULL ;
      /* data will be garbage-collected later */
      DEALLOC_HASH_ENTRY(hsh) ;
      hsh = NULL ;
      deleted = 1 ;
    }
    else {
      if( n > 0 ) {
        phsh = &hsh->right ;
        hsh = hsh->right ;
      }
      else {
        phsh = &hsh->left ;
        hsh = hsh->left ;
      }
    }
  }
  if( !deleted ) {
    WARNING_INDEX_NOT_FOUND_IN_DELETE_INDEX_FUNC ;
  }
}

static void gen_string_index_list( JD_hash_entry *hsh, JD_data *dt ) {
  JD_data_unit *ut, *nt ;
  JD_data *st ;
  st = JD_alloc_data_entry() ;
  st->type = JD_D_string ;
  st->sub_type = JD_SD_single ;
  st->data.string = ALLOC_STRING(strlen(hsh->string)) ;
  strcat(st->data.string, hsh->string) ;
  st->base = dt ;
  ut = ALLOC_DATA_UNIT(1) ;
  ut->data = st ;
  nt = dt->data.list.top ;
  dt->data.list.top = ut ;
  ut->next = nt ;
  if( nt ) nt->pre = ut ;
  if( hsh->right ) {
    gen_string_index_list( hsh->right, dt ) ;
  }
  if( hsh->left ) {
    gen_string_index_list( hsh->left, dt ) ;
  }
}
     
JD_sc_stridx_mf_index_list()
{
  int i ;
  TMP_DT = JD_alloc_data_entry() ;
  TMP_DT->type = JD_D_string ;
  TMP_DT->sub_type = JD_SD_list ;
  TMP_DT->data.list.top = NULL ;
  TMP_DT->data.list.bottom = NULL ;
  TMP_DT->data.list.sem = NULL ;
  for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
    if( ACC_DT->data.hash.table[i] ) {
      gen_string_index_list( ACC_DT->data.hash.table[i], TMP_DT )  ;
    }
  }
  ACC_DT = TMP_DT ;
  ACC_TYPE = JD_D_dt ;
}
        
JD_sc_stridx_mf_flush_all()
{
  JD_sc_assoc_mf_flush_all() ; /* same as assoc array */
}           


/*************************************************************
 *  file object
 *************************************************************/
void *JD_sa_file() {
  return (void *)calloc( 1, sizeof(JD_file) ) ;
}

char *JD_disp_file( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_file *fl ;
  int i ;
  fl = (void *)dt->data.obj.ect->table ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  sprintf( 
    ret, "%s FILE=%d %s %s", indent, 
    fl->opened?"opened":"closed", eol
   ) ;
  return ret ;
}

void JD_sd_file( JD_file *file ) {
  if( file ) {
    if( file->opened ) fclose(file->file) ;
    free(file) ;
  }
}

void *JD_dup_file(JD_file *file) {
  JD_file *nw ;
  nw = (JD_file *)calloc( 1, sizeof(JD_file) ) ;
  
  return(nw) ;
}

void JD_sc_file_mf_open() {
  JD_file *file ;
  JD_data *path, *mode ;
  int ret ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  file =  (void *)ACC_DT->data.obj.ect->table ;
  if( file->opened ) {
    if (file->pipe) pclose(file->file) ;
    else fclose(file->file) ;
    file->opened = 0 ;
  }
  mode = GET_STACK(1) ;
  path = GET_STACK(2) ;
  
  if( path->data.string == NULL || path->data.string[0] == '\0' ) {
    PRINTF( 
      "file.open() failed due to the empty file name\n"
    ) ;
    ACC_TYPE = JD_D_int ;
    ACC_INT = 0 ;
    return ;
  }
  /* opening special name file, stderr, stdin, stdout */
  if( !strcmp(path->data.string, "stderr" ) ) {
    file->file = stderr ;
    ACC_TYPE = JD_D_int ;
    ACC_INT = 1 ;
    return ;
  }
  if( !strcmp(path->data.string, "stdin" ) ) {
    file->file = stdin ;
    ACC_TYPE = JD_D_int ;
    ACC_INT = 1 ;
    return ;
  }
  if( !strcmp(path->data.string, "stdout" ) ) {
    file->file = stdout ;
    ACC_TYPE = JD_D_int ;
    ACC_INT = 1 ;
    return ;
  }
  /* check if this is pipe access */
  {
    char *cp ;
    cp = path->data.string ;
    while( isspace( *cp ) ) cp++ ;
    if( *cp == '|' ) {
      cp++ ;
      file->file = popen( cp, mode->data.string ) ;
      if( file->file == NULL ) {
        PRINTF( 
          "file.open( %s, %s ) failed\n",path->data.string, mode->data.string
        ) ;
        ACC_INT = 0 ;
      }
      else {
        ACC_INT = 1 ;
        file->opened = 1 ;
        file->pipe = 1 ;
      }
      ACC_TYPE = JD_D_int ;
      return ;
    }
    cp = &path->data.string[strlen(path->data.string)-1] ;
    while( isspace( *cp ) ) cp-- ;
    if( *cp == '|' ) {
      *cp = '\0' ;
      file->file = popen( path->data.string, mode->data.string ) ;
      if( file->file == NULL ) {
        PRINTF( 
          "file.open( %s, %s ) failed\n",path->data.string, mode->data.string
        ) ;
        ACC_INT = 0 ;
      }
      else {
        ACC_INT = 1 ;
        file->opened = 1 ;
        file->pipe = 1 ;
      }
      ACC_TYPE = JD_D_int ;
      return ;
    }
  }
  
  /* normal access */
  file->file = fopen( path->data.string, mode->data.string ) ;
  if( file->file == NULL ) {
    PRINTF( 
      "file.open( %s, %s ) failed\n",path->data.string, mode->data.string
    ) ;
    ACC_INT = 0 ;
  }
  else {
    ACC_INT = 1 ;
    file->opened = 1 ;
  }
  ACC_TYPE = JD_D_int ;
}

void JD_sc_file_mf_rewind() {
  JD_file *file ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  file =  (void *)ACC_DT->data.obj.ect->table ;
  if( !file->opened ) {
    PRINTF( "file.rewind() failed, file not opened\n" ) ;
    ACC_TYPE = JD_D_void ;
    return ;
  }
  if( file->pipe ) {
    PRINTF( "file.rewind() failed, file is a pipe\n" ) ;
    ACC_TYPE = JD_D_void ;
    return ;
  }
  rewind(file->file) ;
  ACC_TYPE = JD_D_void ;
}

void JD_sc_file_mf_close() {
  JD_file *file ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  file =  (void *)ACC_DT->data.obj.ect->table ;
  if( !file->opened ) {
    PRINTF( "file.close() failed, file not opened\n" ) ;
    ACC_TYPE = JD_D_void ;
    return ;
  }
  if (file->pipe) pclose(file->file) ;
  else fclose(file->file) ;
  file->file = NULL ;
  file->opened = 0 ;
  ACC_TYPE = JD_D_void ;
}

void JD_sc_file_mf_fprintf() {
  JD_file *file ;
  JD_data *fmt_dt ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  file =  (void *)ACC_DT->data.obj.ect->table ;
  if( !file->opened ) {
    PRINTF( "file.fprintf() failed, file not opened\n" ) ;
    ACC_TYPE = JD_D_void ;
    return ;
  }
  fmt_dt = STACK[SP - NUM_ARG] ;  
  JD_sys_fprintf (
    2, NULL, file->file, fmt_dt->data.string, NUM_ARG
  ) ;
}

void JD_sc_file_mf_readline() {
  JD_file *file ;
  char *tmp ;
  int size ;
  int i, c ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  file =  (void *)ACC_DT->data.obj.ect->table ;
  if( !file->opened ) {
    PRINTF( "file.readline() failed, file not opened\n" ) ;
    ACC_TYPE = JD_D_void ;
    return ;
  }
  tmp = (char *)calloc( DEFAULT_STRING_SIZE, sizeof(char) ) ;
  size = DEFAULT_STRING_SIZE ;
  i = 0 ;
  
  while ((c=getc(file->file)) != EOF) {
    if( i+1 >= size ) {
      size+=DEFAULT_STRING_SIZE ;
      tmp = (char *)realloc(tmp, size) ;
    }
    if( c=='\n') {
      tmp[i++] = c ;
      tmp[i] = 0 ;
      ACC_STRING = tmp ;
      ACC_TYPE = JD_D_string ;
      return ;
    }
    else tmp[i++] = c ;
  }
  if( tmp[0] == 0 ) {
    free(tmp) ;
    ACC_STRING = NULL ;
  }
  else ACC_STRING = tmp ;
  ACC_TYPE = JD_D_string ;
}

JD_sf_bit2string() {
  JD_data *dt ;
  unsigned int tmp ;
  int i, j, k, n, m, nw ;
  CLEAR_ACC ;
  ACC_TYPE = JD_D_string ;
  dt = STACK[SP - 1] ;  
  j = 0 ;
  if( IS_SMLINT(dt) ) {
    ERROR_ILLEGAL_ARGUMENT_FOR( "bit2string()" ) ;
  }
  else switch( dt->type ) {
    case JD_D_sbit:
      n = dt->data.sbit.size/8 ; /* this way we ignore non byte boundary */
      ACC_STRING = ALLOC_STRING(n+2) ;
      if( dt->data.sbit.b ){ ACC_STRING[0] = '\0' ; return ; }
      for(  ; n > 0 ; n-- ) {
        k = (dt->data.sbit.a >> ((n-1)*8) ) & 0xff ;
        if( k ) ACC_STRING[j++] = k ;  /* skip null */
      }
      ACC_STRING[j] = '\0' ;
      break ;
    case JD_D_bit:
      n = dt->data.bit.size/8 ; /* this way we ignore non byte boundary */
      ACC_STRING = ALLOC_STRING(n+2) ;
      nw = LASTWORD(ACC_SIZE) ;
      for(  ; nw >= 0 ; nw-- ) {
        if( dt->data.bit.b[nw] ) { ACC_STRING[0] = '\0' ; return ; }
        for( m = 4 ; m > 0 ; m-- ) {
          k = (dt->data.bit.a[nw] >> ((m-1)*8) ) & 0xff ;
          if( k ) ACC_STRING[j++] = k ; /* skip null */
        }
      }
      ACC_STRING[j] = '\0' ;
      break ;
    default:
      ERROR_ILLEGAL_ARGUMENT_FOR( "bit2string()" ) ;
      break ;
  }
}

void JD_sf_get_plusarg() {
  JD_data *dt ;
  int n ;
  char *ptn ;
  char *rtn ;
  char c ;
  
  dt = GET_STACK(1) ;
  if( dt->data.string == NULL ) {
    ERROR_ILLEGAL_ARGUMENT_FOR( "get_plusarg()" ) ;
    ACC_TYPE = JD_D_null ;
    return ;
  }
  n = strlen(dt->data.string) ;
  if( n < 2 ) {
    ERROR_ILLEGAL_ARGUMENT_FOR( "get_plusarg()" ) ;
    ACC_TYPE = JD_D_null ;
    return ;
  }
  ptn = (char *)strdup( dt->data.string ) ;
  c = ptn[n-1] ;
  ptn[n-1] = '\0' ;
  rtn = (char *)SCAN_PLUSARGS( ptn ) ;
  if( rtn != NULL && c == rtn[0] ) {
    /* pattern found */
    ACC_TYPE = JD_D_string ;
    ACC_STRING = (char *)strdup( &rtn[1] ) ;
  }
  else ACC_TYPE = JD_D_null ;
  
  free(ptn) ;
  
}

/* assert() checks the given argument is true, or report a Verification error */
void JD_sf_assert() {
  JD_data *dt ;
  int yes = 0 ;
  
  dt = GET_STACK(1) ;

  if( IS_SMLINT(dt) ) {
    if( SMLINT2INT(dt) ) yes = 1 ;
    else yes = 0 ;
  }
  else {
    while( dt->type == JD_D_pointer ) {
      dt = JD_get_ptr_element(dt) ;
    }
  
    switch( dt->type ) {
      case JD_D_sbit:
        if( dt->data.sbit.a && (!dt->data.sbit.b ) ) yes = 1 ;
        break ;
      case JD_D_vbit:
      case JD_D_bit:
        {
          int i, n ;
          n = NWORD(dt->data.bit.size) ;
          for( i = 0 ; i < n ; i++ ) {
            if( dt->data.bit.a[i] ) yes = 1 ;
            if( dt->data.bit.b[i] ) {
              yes = 0 ;
              break ;
            }
          }
        }
        break ;
      case JD_D_int:
        if( dt->data.int_value ) yes = 1 ;
        break ;
      case JD_D_float:
        if( dt->data.float_value != 0 ) yes = 1 ;
        break ;
      case JD_D_double:
        if( dt->data.double_value != 0 ) yes = 1 ;
        break ;
      case JD_D_object:
        if( dt->data.obj.ect ) yes = 1 ;
        break ;
      case JD_D_string:
        if( dt->data.string && dt->data.string[0] != '\0' ) yes = 1 ;
        break ;
      case JD_D_thread:
        if( dt->data.thrd ) yes = 1 ;
        break ;
      case JD_D_signal:
        if( dt->data.signal.port ) yes = 1 ;
        break ;
      default:
        break ;
    }
  
  }
  
  if( !yes ) {
    int cycle ;

    JEDA_pli_get_cycle( &cycle ) ;

    JD_code_location() ;
    PRINTF( "JEDA VERIFICATION ERROR @ %d : assert() failed\n  ", cycle ) ;  

    VERIF_ERROR ;
  
  }
}


/* system_exec() : fork & execute the given string as the command line.
   it does not wait for the completion
*/ 
void JD_sf_system_exec() {
  JD_data *dt ;
  char *command ;
  char **argv ;
  int i, j, k, l, m ;
  int pid ;
  
    
  dt = GET_STACK(1) ;
  
  if( dt->data.string == NULL ) {
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  command = (char *)strdup( dt->data.string ) ;
  
  /* PRINTF( "system_exec() %s\n", command ) ; */
  
  l = strlen(command) ;
  j = 0 ;
  k = 0 ;
  for( i = 0 ; i < l ; i++ ) {
    if( !isspace(command[i]) ) {
      if( k == 0 ) {
        j++ ;
        k = 1 ;
      }
    }
    else {
      k = 0 ;
    }
  }
  argv = (char **)calloc( j+1, sizeof(char *) ) ;
  m = 0 ;
  k = 0 ;
  for( i = 0 ; i < l ; i++ ) {
    if( !isspace(command[i]) ) {
      if( k == 0 ) {
        argv[m++] = &command[i] ;
        k = 1 ;
      }
    }
    else {
      command[i] = '\0' ;
      k = 0 ;
    }
  }
  
  /* debug */
  PRINTF( "system_exec() invoking \"" ) ;
  for( i = 0 ; i < j ; i++ )
    PRINTF( "%s ", argv[i] ) ;
  PRINTF( "\" \n" ) ;
  
  pid = fork() ;
  if( pid < 0 ) {
    PRINTF( "JD_sf_system_exec Can't fork\n" ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  if( pid == 0 ) { // child 
    execvp( argv[0], argv ) ;
    PRINTF( "JD_sf_system_exec Can't exec %d\n", errno );
    perror( "JD_sf_system_exec" ) ;
    exit( -1 );
  }
  ACC_INT = 1 ;
  ACC_TYPE = JD_D_int ;
  
}

/* aspect extention */
void JD_sf_pointcut_line() {
  int f, l ;
  
  GET_POINTCUT_LOCATION( &l, &f ) ;

  ACC_TYPE = JD_D_int ;
  ACC_INT = l ;

}

void JD_sf_pointcut_file() {
  int f, l ;
  char *fn ;
  
  GET_POINTCUT_LOCATION( &l, &f ) ;
  if( f >= 0 ) {
    ACC_TYPE = JD_D_string ;
    ACC_STRING = (char *)strdup( JD_Debug_name_table[f] ) ;
  }
  else ACC_TYPE = JD_D_null ;
    
}

#ifdef PRODUCT_CODE
/****************************************************************
 * license functions
 ****************************************************************/
#include "lmpolicy.h"
LP_HANDLE *JEDA_lp_handle ;

LP_HANDLE *JD_pli_checkout_license(  
  char *feature, char *version, char *licdir 
) ;

void JD_pli_checkin_license(LP_HANDLE *lp_handle) ;

JD_sf_checkout_license() {
  LP_HANDLE *lp_handle ;
  JD_data *feature ;
  JD_data *version ;
  char *licdir ;
  char *homedir = (char *)getenv( "JEDA_HOME" ) ;
  
  feature = JD_get_stack(2) ; 
  version = JD_get_stack(1) ; 
  if( homedir == NULL ) {
    fprintf( 
      stderr, 
      "Environment Variable $%s is not set. Using current dir\n", "JEDA_HOME" 
    ) ;
    homedir = "." ;
  }
  licdir = (char *)calloc( 256 + strlen( homedir ), sizeof(char) ) ;
  strcpy( licdir, homedir ) ;
  strcat( licdir, "/license/" ) ;
  lp_handle = 
    JD_pli_checkout_license(
      feature->data.string, version->data.string, licdir
    ) ;
  
  ACC_INT = (int)lp_handle ;
  ACC_TYPE = JD_D_int ;
  
}

JD_sf_checkin_license() {
  LP_HANDLE *lp_handle ;
  JD_data *lic ;
  lic = JD_get_stack(1) ; 
  if( IS_SMLINT(lic) ) {
    lp_handle = (LP_HANDLE *)SMLINT2INT(lic) ;
  }
  else lp_handle = (LP_HANDLE *)lic->data.int_value ;
  JD_pli_checkin_license( lp_handle ) ;
}

#endif
