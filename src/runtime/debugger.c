/*
 * Jeda Debugger runtime support 
 */
/* 
   
   Copyright (C) 2001 Juniper Networks Inc.
   Copyright (C) 2002 Jeda Technologies, Inc.

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
/* #include <math.h> */
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
#include "debugger.h"
#include "debugger_error_msg.h"

extern JD_queue *JD_ready_queue ;
extern JD_toplevel *JD_current_toplevel ;

extern int JD_pli_current_cycle ;

extern int JD_invoke_debugger ;

#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif
extern int JD_dbg_eval_error ;

extern int JD_breakable_table[] ;
JD_debug_stack *JD_debug_current_stack = NULL ;
JD_debug_stack *JD_debugged_stack = NULL ;

#define CURRENT_STACK JD_debug_current_stack
#define DBG_STACK JD_debugged_thread

JD_dbg_break_point **JD_debug_breakable_points ;
JD_dbg_break_point **JD_debug_break_points ;
int JD_debug_num_break_point = 0 ;
JD_dbg_break_point *JD_debug_last_break_point ;
JD_dbg_break_point *JD_debug_top_break_point = NULL ;

static int pointcut_file_index = -1 ;
static int pointcut_line_num = -1 ;

/*************************************************************************
 * This part of debugger consists of a set of code to handle the runtime
 * acrivities, and name table handling for user code execution
 **************************************************************************/

JD_thread *JD_debugged_thread ;
static int interrupted = 0 ;
static JD_thread *step_thread = NULL ;
static int step_thread_flag = 0 ;
static int step_any_flag = 0 ;
static int next_flag = 0 ;
static JD_debug_stack *next_stack = NULL ;

/* this is called on SIGINT upon Ctl-C */
void JD_debug_sig_handler() {
  interrupted = 1 ;  
}

/* this will be called when breakpoint statement is executed */
void JD_debugger_exec_breakpoint() {
  JD_invoke_debugger = 1 ;
  JD_enqueue_top( JD_ready_queue, THREAD ) ;
  JD_thread_pause( THREAD ) ;
}

/* debug stack is dynamically created upon execution of jeda code with 
   embedded debuger code in the compiled C code.
 */
static JD_debug_stack *alloc_debug_stack() {
  JD_debug_stack *ret ;
  ret = (JD_debug_stack *)calloc(1,sizeof(JD_debug_stack)) ;    
  return ret ;
}

static void dealloc_debug_stack( JD_debug_stack *f ) {
  free( f ) ;
}

static void clear_breakpoint( JD_dbg_break_point *bp  ) {
  int hs ;
  JD_dbg_break_point *tp ;
  hs = BREAK_POINT_HASH(bp->cwdnum, bp->filenum, bp->linenum) ; 
  if( bp->fw ) {
    bp->fw->bw = bp->bw ;
    if( bp->bw ) bp->bw->fw = bp->fw ;
  }
  else {
    JD_debug_break_points[hs] = bp->bw ;
    if( bp->bw ) bp->bw->fw = NULL ;
  }
  bp->set = 0 ;
  bp->temp_flag = 0 ;
  JD_debug_num_break_point-- ;
  if( JD_debug_top_break_point == bp ) 
    JD_debug_top_break_point = bp->next_list ;
  else {
    tp = JD_debug_top_break_point ;
    while( tp && tp->next_list != bp ) tp = tp->next_list ;
    if( tp ) tp->next_list = bp->next_list ;
  }
}

extern int JD_code_file_index ;
extern int JD_code_line_num ;
 
/* before execution of every statement, set_location is called to tell
   the debugger where it is.
 */
void JD_debugger_set_location( int scp, int n, int s, int l, int c ) {
  if( CURRENT_STACK == NULL ) {
    return ;
    /*
    CURRENT_STACK = alloc_debug_stack() ;
    CURRENT_STACK->thrd = THREAD ;
    THREAD->dbg_stack = CURRENT_STACK ;
    */
  }
  if( CURRENT_STACK->active == 2 )  /* status of not started */
    CURRENT_STACK->active = 1 ;
  CURRENT_STACK->scope = scp ;
  CURRENT_STACK->file_index = JD_code_file_index = n ;
  CURRENT_STACK->line_num = JD_code_line_num = l ;
  /* CURRENT_STACK->statement_num = s ; */
  CURRENT_STACK->cwd_index = c ;
  
  CURRENT_STACK->cycle = JD_pli_current_cycle ;
  
  CURRENT_STACK->stk_frame = THREAD->stk_frame ;
  
  if( interrupted || step_any_flag || 
      ( step_thread_flag && THREAD==step_thread) ||
      ( next_flag && next_stack == JD_debug_current_stack )
    ) 
  {
    interrupted = 0 ;
    step_any_flag = 0 ;
    step_thread_flag = 0 ;
    next_flag = 0 ;
    JD_invoke_debugger = 1 ;
    JD_debug_last_break_point = NULL ;
    JD_enqueue_top( JD_ready_queue, THREAD ) ;
    JD_thread_pause( THREAD ) ;
  }
  else {
    /* do the breakpoint search here */
    if( JD_debug_num_break_point ) {
      int hs ;
      JD_dbg_break_point *bp ;
    
      hs = BREAK_POINT_HASH(c, n, l) ;
      bp = JD_debug_break_points[hs] ;
      while( bp ) {
        if( bp->cwdnum==c && bp->filenum==n ) {
          while( bp ) {
            if( bp->linenum == l ) {
              JD_invoke_debugger = 1 ;
              JD_debug_last_break_point = bp ;
              if( bp->temp_flag ) {
                clear_breakpoint( bp ) ;
              }
              bp = NULL ;
              PRINTF( 
                "jdb breakpoint at %s:%d\n",
                JD_Debug_name_table[n], l
              ) ;
              JD_enqueue_top( JD_ready_queue, THREAD ) ;
              JD_thread_pause( THREAD ) ;
            }
            else bp = bp->nextpoint ;
          }
          bp = NULL ;
        }
        else bp = bp->next ;
      }
    }
  }
}

/* called from debugger UI for step command */
void JD_debugger_set_next(){
  if( JD_debug_current_stack ) {
    next_flag = 1 ;
    next_stack = JD_debug_current_stack ;
  }
}

/* called from debugger UI for step command */
void JD_debugger_set_step(){
  if( JD_debug_current_stack ) {
    step_thread_flag = 1 ;
    step_thread = JD_debug_current_stack->thrd ;
  }
}

/* called from debugger UI for stepany command */
void JD_debugger_set_step_any(){
  step_any_flag = 1 ;
}

/* following code are called from error report code to get where the code is
   when it encounters an error
 */
void JD_debugger_code_location() {
  if( CURRENT_STACK ) {
    PRINTF( 
      "At line %d in \"%s\" : ", 
      CURRENT_STACK->line_num,
      JD_Debug_name_table[CURRENT_STACK->file_index] 
    ) ;
  }
}

void JD_debugger_dbg_code_location() {
  if( CURRENT_STACK ) {
    PRINTF( 
      "At line %d in \"%s\" @ %d : ", 
      CURRENT_STACK->line_num,
      JD_Debug_name_table[CURRENT_STACK->file_index],
      JD_pli_current_cycle
    ) ;
  }
}

void JD_debugger_set_pointcut_location() {
  pointcut_file_index = CURRENT_STACK->file_index ;
  pointcut_line_num = CURRENT_STACK->line_num ;
}

void JD_debugger_get_pointcut_location( int *line_num, int *file_index ) {
  *line_num = pointcut_line_num ;
  *file_index = pointcut_file_index ;
}

/* This one tells where the execution is. Used in pli.c to mark who's the
   previous driver is. (Reported when it detect the drive conflict.
 */
void JD_debugger_get_current_location( int *line_num, int *file_index ) {
  if( CURRENT_STACK ) {
    *line_num = CURRENT_STACK->line_num ;
    *file_index = CURRENT_STACK->file_index ;
  }
  else {
    *line_num = -1 ;
    *file_index = -1 ;
  }
}

void JD_debugger_get_location_of_stack( 
  JD_debug_stack *dbg_stack, int *line_num, int *file_index 
) {
  if( dbg_stack ) {
    *line_num = dbg_stack->line_num ;
    *file_index = dbg_stack->file_index ;
  }
  else {
    *line_num = -1 ;
    *file_index = -1 ;
  }
}

/* called when a function is called. this happens before the debug stack
   construction for the function call, so keep it in a temp location
 */
void JD_debugger_set_local_scope( int scope ) {
  CURRENT_STACK->local_scope = scope ;
}


/* various calls from embedded debugger code to keep track of what's happening
   in the execution of the code
 */
void JD_debugger_set_enum( int index ) {
  /* We probably figure out by the data type info. So do nothing */
}

/*********************************************************************/
/* Debug stack handling funcitons                                    */
/*********************************************************************/
/**************************************************************************
 *
 *  Debug stack allocation/deallocation:
 *
 *  1) at the beginning of a function, JD_debugger_gen_func_frame or
 *     JD_debugger_gen_mfunc_frame is called to allocate a new debug stack.
 *     active thread points to the currently active debug stack.
 *     pointer from debug stack to thread also exists.
 *     If the function is called (not main()), then caller, callee chain
 *     will be created.
 *
 *  2) At exec (starting a thread for fork), a new debug stack is created 
 *     and linked under forked link. Record time/cycle info to it. If there 
 *     are more than one thread, they are lined together with sibling link.
 *
 *  3) When a forked thread is terminating itself, it checks if it has any
 *     history stack. If it has history, it keep it linked to the parent
 *     as is. At this point, if parent are waiting, a copy of the parent
 *     will be created at the moveing point as 4). Or parent's stack is 
 *     already a copy in the histry link, because of active forkee.
 *     (representing a point of fork)
 *
 *  4) At join/knife/spoon, existance of thread under forked link is checked.
 *     If there's still running thread, A copy of the current stack is
 *     created (with active = 0, thread = NULL), move the forked link to it,
 *     and chained to the history link of the current active stack.
 *   
 *  5) At returning from a func call, the history link is checked. If history
 *     exists, a copy of caller stack is created, move the caller/callee
 *     link, and connected to the caller's history link. 
 *     
 *  6) Debug stack is cleaned up when a thread structure is deallocated.
 *     It will clean up upper thread if the cleared stack is the last one
 *     in the level.
 *
 **************************************************************************/


/* print out the debug stuck tree for debug */
void JD_debugger_print_debug_stack( JD_debug_stack *f, int indent, char *id ) {
  int i ;
  JD_debug_stack *n ;
  
  if( indent > 100 ) {
    n = NULL ;
    n->hb = NULL ; /* force to segfault */
  }
  for( i = 0 ; i < indent ; i++ ) PRINTF( "  " ) ;
  if( f->line_num >= 0 ) {
    PRINTF( 
      "%s%sline %d in \"%s\"\n", 
      id, f->active?"":"(inactive)",
      f->line_num,JD_Debug_name_table[f->file_index] 
    ) ;
  }
  else {
    PRINTF( "%sLine ?? in ????\n", id ) ;
  }
  /*
  if( f->hf == NULL ) {
    n = f->hb ;
    while( n ) {
      JD_debugger_print_debug_stack( n, indent, "+" ) ;
      n->hb ;
    }
  }
  */
  if( f->callee ) 
    JD_debugger_print_debug_stack( f->callee, indent+1, "call->" ) ;
  if( f->forkee ) 
    JD_debugger_print_debug_stack( f->forkee, indent+1, "fork->" ) ;
}


/* print out the trace info                                      */
/* up-trace the debug stack and print out from the top to bottom */
/* use temp pointer to re-come down from the top                 */
void JD_debugger_print_trace_stack( JD_debug_stack *f ) {
  int i ;
  JD_debug_stack *n ;
  JD_debug_stack *p ;
  n = f ;
  p = n ;  /* set stoper */
  while( n->caller || n->forker ) {
    if( n->caller ) {
      n->caller->temp = n ;
      n->caller->flag = 0 ; 
      n = n->caller ;
    }
    if( n->forker ) {
      n->forker->temp = n ;
      n->forker->flag = 1 ;
      n = n->forker ; 
    }
  }
  
  while( n ) {
    PRINTF( 
      "TrdID(%d)%sline %d in \"%s\"", 
      n->active?n->thrd->id:n->thrd_id,
      n->active?"":"(inactive)", 
      n->line_num, JD_Debug_name_table[n->file_index] 
    ) ;
    if( n != p && n->temp ) {
      if( n->flag == 0 ) PRINTF( " call-> " ) ;
      else PRINTF( " fork-> " ) ;
    }
    if( n != p ) n = n->temp ;
    else n = NULL ;
  }
  JEDA_pli_get_cycle( &i ) ;
  PRINTF( " @ %d\n", i ) ;
}

char *JD_debugger_gen_pref_stack( JD_debug_stack *f ) {
  int i ;
  int len ;
  JD_debug_stack *n, *p ;
  char *buf, *tbuf ;
  
  len = 100 ;
  n = f ;
  p = n  ;  /* set stoper */
  while( n->caller || n->forker ) {
    if( n->caller ) {
      len += strlen(JD_Debug_name_table[n->file_index]) + 100 ;
      n->caller->temp = n ;
      n->caller->flag = 0 ; 
      n = n->caller ;
    }
    if( n->forker ) {
      n->forker->temp = n ;
      n->forker->flag = 1 ;
      n = n->forker ; 
    }
  }
  
  buf = (char *)calloc( len, sizeof(char) ) ;
  
  
  while( n != p ) {
    tbuf = 
      (char *)calloc(
        strlen(JD_Debug_name_table[n->file_index])+100, sizeof(char)
      ) ;
    
    if( n->active ) {
      sprintf( tbuf,
        "line %d in \"%s\"", 
        n->line_num, JD_Debug_name_table[n->file_index] 
      ) ;
    }
    else {
      sprintf( tbuf,
        "(inactive @ %d) line %d in \"%s\"", 
        n->cycle, n->line_num, JD_Debug_name_table[n->file_index] 
      ) ;
    }
    strcat( buf, tbuf ) ;
    if( n->temp ) {
      if( n->flag == 0 ) strcat( buf, " call->" ) ;
      else strcat( buf, " fork->" ) ;
    }
    n = n->temp ;
  }
  
  return( buf ) ;
}


void JD_debugger_backtrace() {
  JD_debugger_print_trace_stack( JD_debugged_stack ) ;
}

void JD_debugger_up_scope( int n ) {
  
  while( n ) {
    if( JD_debugged_stack->caller || JD_debugged_stack->forker ) {
      if( JD_debugged_stack->caller ) {
         JD_debugged_stack->caller->temp = JD_debugged_stack ;
         JD_debugged_stack->caller->flag = 0 ; 
         JD_debugged_stack = JD_debugged_stack->caller ;
      }
      else if( JD_debugged_stack->forker ) {
         JD_debugged_stack->forker->temp = JD_debugged_stack ;
         JD_debugged_stack->forker->flag = 1 ;
         JD_debugged_stack = JD_debugged_stack->forker ; 
      }
      n-- ;
    }
    else {
      DBG_ERROR_CANNOT_GO_UP_FUTHER ;
      break ;
    }
  }

}

void JD_debugger_down_scope( int n ) {
  
  while( n ) {
    if( JD_debugged_stack->temp ) {
      JD_debugged_stack = JD_debugged_stack->temp ;
      n-- ;
    }
    else {
      DBG_ERROR_CANNOT_GO_DOWN_FUTHER ;
      break ;
    }
  }

}

static JD_debug_stack *copy_debug_stack( JD_debug_stack *s ) {
  JD_debug_stack *f = (JD_debug_stack *)calloc(1,sizeof(JD_debug_stack)) ;
  
  f->type = s->type ;
  f->file_index = s->file_index ;
  f->line_num = s->line_num ;
  f->cwd_index = s->cwd_index ;
  f->local_scope = s->local_scope ;
  f->scope = s->scope ;
  f->caller = s->caller ;
  f->callee = s->callee ;
  f->forker = s->forker ;
  f->forkee = s->forkee ;
  f->thrd = s->thrd ;
  f->this = s->this ;
  return f ;
}

/* exit_function: called at the end of function */
/* 
   when this function has active record (active thread or history record 
   then we create caller's copy and keep it under caller's history record
  
   [caller]->hb
             V
             [callee]->hb  -- returning point
                       hf<-[callee]->hb  --  active fork point
                                      ..
                                   ->forkee  -- forkee point
                                     V
                                     forker<-[forkee]->sb  -- active fork frm
                                                       V
                                                       sf<-[forkee]->sb
                                                                       ..
 
 */

/* cleanup non active debug stack on fork_exit */
/* this debug stack is not active and linked in the history buffer */
static void cleanup_debug_stack( 
  JD_debug_stack *f 
  /* , int level */ 
) {
  JD_debug_stack *p, *n, *pp ;
  
  /*
  JD_debugger_print_debug_stack( JD_current_toplevel->top_dbg_stack, 0, " " ) ;
  */

  /* debug code  
  if( level > 5 ) {
    p = NULL ;
    p->hb = NULL ; 
  }
  */
  
  p = f->hf ;
  n = f->hb ;
  if( p ) {
    p->hb = n ;
    if( n ) n->hf = p ;
  }
  else if( n ) {
    n->hf = NULL ;
    if( n->caller ) {
      /* debug code
      if( n->caller == n ) {
        pp = NULL ; 
        pp->hf = NULL ;
      }
      */
      pp = n->caller ;
      pp->callee = n ;
      /* debug code
      if( n->forker ) {
        pp = NULL ; 
        pp->hf = NULL ;
      }
      */
    }
    else {
      /* debug code
      if( n->forker == n ) {
        pp = NULL ; 
        pp->hf = NULL ;
      }
      */
      pp = n->forker ;
      pp->forkee = n ;
    }
  }
  else {
    if( f->caller ) {
      pp = f->caller ;
      pp->callee = NULL ;
      if( pp->active == 0 ) {
        /* fprintf( stderr, "recursive call of cleanup_debug_stack\n" ) ; */
        cleanup_debug_stack( pp /* , level+1 */ ) ;
        /* fprintf( stderr, "returned of cleanup_debug_stack\n" ) ; */
      }
      if( f->forker ) {
        pp = NULL ; 
        pp->hf = NULL ;
      }
    }
    else if( f->forker ) {
      pp = f->forker ;
      pp->forkee = NULL ;
      if( pp->active == 0 ) {
        /* fprintf( stderr, "recursive call of cleanup_debug_stack\n" ) ; */
        cleanup_debug_stack( pp /* , level+1 */ ) ;
        /* fprintf( stderr, "returned of cleanup_debug_stack\n" ) ; */
      }
    }
  }
  dealloc_debug_stack( f ) ;
}

/* Construct a stack frame at the beginning of func call */
void JD_debugger_enter_function() {
  JD_debug_stack *frame ;
  frame =  alloc_debug_stack() ;
  frame->type = JD_dbg_func ;
  if( CURRENT_STACK == NULL ) {
    CURRENT_STACK = frame ;
  }
  else {
    CURRENT_STACK->callee = frame ;
    frame->caller = CURRENT_STACK ;
    frame->scope = CURRENT_STACK->scope ;
    frame->local_scope = CURRENT_STACK->local_scope ;
  }
  CURRENT_STACK = frame ;
  frame->active = 1 ;
  frame->thrd = THREAD ;
  THREAD->dbg_stack = frame ;
}

void JD_debugger_start_function() {
  JD_debug_stack *frame ;
  frame =  alloc_debug_stack() ;
  frame->type = JD_dbg_func ;
  CURRENT_STACK = frame ;
  frame->active = 1 ;
  frame->thrd = THREAD ;
  THREAD->dbg_stack = frame ;
}

void JD_debugger_enter_member_function() {
  JD_debugger_enter_function() ;
  CURRENT_STACK->this = ACC_DT ;
  CURRENT_STACK->inclass = 1 ;
}

/* this is called when a member function is overwritten by the child */
void JD_debugger_enter_child_member_function() {
  CURRENT_STACK->this = ACC_DT ;
}

void JD_debugger_enter_aspect_function() {
  JD_debugger_enter_function() ;
  CURRENT_STACK->inaspect = 1 ;
}

void JD_debugger_enter_aspect_advice() {
  JD_debugger_enter_function() ;
  CURRENT_STACK->type = JD_dbg_advice ;
  CURRENT_STACK->inaspect = 1 ;
}

void JD_debugger_exit_function() {
  JD_debug_stack *f = CURRENT_STACK ;
  JD_debug_stack *n, *p, *h ;
  p = f->caller ;
  /* update next info to the caller */
  if( next_flag && next_stack == f ) next_stack = p ;
  if( f->hb ) {  /* still have history to carry */
    h = f->hb ;
    n = copy_debug_stack( p ) ;
    n->active = 0 ;
    f->active = 0 ;
    n->callee = h ;

    while( h ) {
      h->caller = n ;
      h = h->hb ;
    }
    h = f->hb ;
    h->hf = NULL ;
    
    n->hb = p->hb ;
    if( p->hb ) p->hb->hf = n ;
    p->hb = n ;
    n->hf = p ;
    p->callee = NULL ;
    CURRENT_STACK = p ;
    THREAD->dbg_stack = CURRENT_STACK ;
    dealloc_debug_stack(f) ;
  }
  else {
    CURRENT_STACK = f->caller ;
    CURRENT_STACK->callee = NULL ;
    THREAD->dbg_stack = CURRENT_STACK ;
    dealloc_debug_stack(f) ;
  }
}

/* Called at the entry the task top on fork entry swich jump 
 */
void JD_debugger_fork_stack( int x ) {
  /* allocation of debug stack is done at thread structure allocation 
    in JD_debugger_set_fork_exec
   */
}

JD_debugger_set_fork_exec() {
  JD_debug_stack *f, *n ;
  JD_data *dt ;
  JD_thread *thrd ;
  dt = (JD_data *)JD_get_stack(1) ;
  thrd = dt->data.thrd ;
  f = CURRENT_STACK ;
  n = alloc_debug_stack() ;
  n->type = JD_dbg_fork ;
  n->forker = f ;
  n->thrd = thrd ;
  n->active = 2 ;  /* status of not started */
  n->scope = f->scope ;
  n->local_scope = f->local_scope ;
  n->file_index = f->file_index ;
  n->line_num = f->line_num ;
  n->cwd_index = f->cwd_index ;
  n->this = f->this ;
  n->inclass = f->inclass ;
  n->inaspect = f->inaspect ;
  thrd->dbg_stack = n ;
  if( f->forkee ) { 
    /* other fork frame exist, link this at the top of sibling link */
    n->sb = f->forkee ;
    f->forkee->sf = n ;
    f->forkee = n ;
  }
  else f->forkee = n ;

}

/* JD_debugger_set_fork_exit: called just before JD_exec_exit call */
JD_debugger_set_fork_exit() {
  JD_debug_stack *f, *n, *p, *h, *t ;
  f = CURRENT_STACK ;
  p = f->forker ; 
    
  if ( f->hb || f->hf ) {
    /* here this thread has active history stack */
    f->active = 0 ;
    if( p->active ) {
      /* create a history stack only if this is the last active one in
         the parent sibling link
      */
      t = p->forkee ;
      while( t ) {
        if( t->active ) break ;
        t = t->sb ;
      }
      if( t == NULL ) {
        n = copy_debug_stack( p ) ;
        n->active = 0 ;
        n->hb = p->hb ;
        if( p->hb ) p->hb->hf = n ;
        p->hb = n ;
        n->hf = p ;
        p->forkee = NULL ;
        t = n->forkee ;
        while( t ) {
          t->forker = n ;
          t = t->sb ;
        }
      }
    }
  }
  else {
    /* no history, reform sibling link */
    if( f->sf ) {
      f->sf->sb = f->sb ;
      if( f->sb ) f->sb->sf = f->sf ;
    }
    else if( f->sb ) {
      p->forkee = f->sb ;
      f->sb->sf = NULL ;
    }
    else {  /* both sf, sb == NULL */
      p->forkee = NULL ;
      if( p->active == 0 ) cleanup_debug_stack( p /*, 0 */ ) ;
    }
    dealloc_debug_stack( f ) ;
    /* need to clean history link if necessary */
  }
  CURRENT_STACK = NULL ;  /* don't know who's next */
}

/* set_join: called when join is completed */
JD_debugger_set_join()
{
  JD_debug_stack *f, *n, *p ;
  f = CURRENT_STACK ;
  
  if( f->forkee ) {
    /* still active thread exists */
    n = copy_debug_stack( f ) ;
    n->active = 0 ;
    n->thrd_id = f->thrd->id ;
    n->hb = f->hb ;
    if( f->hb ) f->hb->hf = n ;
    f->hb = n ;
    n->hf = f ;
    p = f->forkee ;
    while( p ) {
      p->forker = n ;
      p = p->sb ;
    }
    f->forkee = NULL ;
  }
  
}

JD_debugger_set_spoon()
{
  JD_debugger_set_join() ;
}

JD_debugger_set_knife()
{
  JD_debugger_set_join() ;
}

/*
 * 
   Breakable table 
   breakpoint structure
   breakpoint request -> setup
   
 *
 *
 */
 
/*
   JD_debug_breakable_points: Array of break_point, hashed by file, cwd
   
      Each entry: The breakable point for the same file/cwd is linked on
                  nextpoint link in the sorted order
                  
                  The breakable point for the different file/cwd is linked
                  on the next link
     
     
     <entry>.next--------><entry>.next------>..
            .nextpoint
               |
               |
               +-><entry>.nextpoint---><entry>.nextpoint--->.. } same file
 
 */

JD_debugger_read_breakable_points() {
  int i ;
  int hs ;
  JD_dbg_break_point *newp, *prev ;
  
  JD_debug_breakable_points = 
    (JD_dbg_break_point **)
    calloc( BREAK_POINT_HASH_SIZE, sizeof(JD_dbg_break_point *) ) ;
  JD_debug_break_points = 
    (JD_dbg_break_point **)
    calloc( BREAK_POINT_HASH_SIZE, sizeof(JD_dbg_break_point *) ) ;
  i = 0 ;
  
  while( JD_breakable_table[i] > 0 ) {
    newp = (JD_dbg_break_point *)calloc( 1, sizeof(JD_dbg_break_point) ) ;
    newp->cwdnum = JD_breakable_table[i++] ;
    newp->filenum = JD_breakable_table[i++] ;
    newp->linenum = JD_breakable_table[i++] ;
    hs = BREAK_POINT_HASH(0, newp->filenum, 0) ;
    prev = JD_debug_breakable_points[hs] ;
    if( prev ) {
      if( prev->cwdnum == newp->cwdnum && prev->filenum == newp->filenum ) {
        /* same file, look for the sorted location */
        if( prev->linenum > newp->linenum ) {
          JD_debug_breakable_points[hs] = newp ;
          newp->nextpoint = prev ;
        }
        else {
          while( prev->nextpoint ) {
            if( prev->nextpoint->linenum > newp->linenum ) break ;
            prev = prev->nextpoint ;
          }
          newp->nextpoint = prev->nextpoint ;
          prev->nextpoint = newp ;
        }    
      }
      else {
        JD_debug_breakable_points[hs] = newp ;
        newp->next = prev ;
      }
    }
    else {
      JD_debug_breakable_points[hs] = newp ;
    }
  }
}

int JD_debugger_lookup_name_table( char *name ) {
  int i = 0 ;
  
  while( strcmp(JD_Debug_name_table[i],"" ) ) {
    if( !strcmp(JD_Debug_name_table[i], name ) ) return i ;
    i++ ;
  }
  return -1 ;
}

int JD_debugger_set_breakpoint( 
  char *cwd, char *fname, int linenum, int temp_flag
) {
  int cwdnum, filenum ;
  int hs ;
  JD_dbg_break_point *bp, *prev, *tp ;
  
  if( cwd ) {
    cwdnum = JD_debugger_lookup_name_table( cwd ) ;
    if( cwdnum < 0 ) {
      DBG_ERROR_UNKNOWN_DIR( cwd ) ;
      return -1 ;
    }
  }
  else cwdnum = -1 ;
  
  filenum = JD_debugger_lookup_name_table( fname ) ;
  if( filenum < 0 ) {
    DBG_ERROR_UNKNOWN_FILE( fname ) ; 
    return -1 ;
  }
  
  hs = BREAK_POINT_HASH(0, filenum, 0) ;

  bp = JD_debug_breakable_points[hs] ;
  
  while(bp) {
    if( bp->filenum == filenum && ( cwdnum < 0 || cwdnum == bp->cwdnum ) ) {
      while( bp && bp->linenum < linenum ) {
        bp = bp->nextpoint ;
      }
      if( bp ) {
        if( bp->set ) {
          DBG_WARNING_BREAKPOINT_IS_ALREADY_SET( fname, linenum ) ;
          return linenum ;
        }
        hs = BREAK_POINT_HASH(bp->cwdnum, bp->filenum, bp->linenum) ;
        prev = JD_debug_break_points[hs] ;
        if( prev ) {
          prev->fw = bp ;
          bp->bw = prev ;
        }
        bp->set = 1 ;
        bp->temp_flag = temp_flag ;
        JD_debug_num_break_point++ ;
        JD_debug_break_points[hs] = bp ;
        if( JD_debug_top_break_point ) {
          tp = JD_debug_top_break_point ;
          while( tp->next_list ) tp = tp->next_list ;
          tp->next_list = bp ;
        }
        else JD_debug_top_break_point = bp ;
        bp->next_list = NULL ;
        return linenum ;
      }
      else {
        DBG_ERROR_BREAKPOINT_CAN_NOT_BE_LOCATED_IN_FILE( linenum, fname ) ; 
        return -1 ;
      }
    }
    bp = bp->next ;
  }
  DBG_ERROR_UNKNOWN_FILE( fname ) ; 
  return -1 ;
}

int JD_debugger_clear_breakpoint( char *cwd, char *fname, int linenum ) {
  int cwdnum, filenum ;
  int hs ;
  JD_dbg_break_point *bp, *prev ;
  
  if( cwd ) {
    cwdnum = JD_debugger_lookup_name_table( cwd ) ;
    if( cwdnum < 0 ) {
      DBG_ERROR_UNKNOWN_DIR( cwd ) ;
      return -1 ;
    }
  }
  else cwdnum = -1 ;
  
  filenum = JD_debugger_lookup_name_table( fname ) ;
  if( filenum < 0 ) {
    DBG_ERROR_UNKNOWN_FILE( fname ) ; 
    return -1 ;
  }
  
  hs = BREAK_POINT_HASH(0, filenum, 0) ;

  bp = JD_debug_breakable_points[hs] ;
  
  while(bp) {
    if( bp->filenum == filenum && ( cwdnum < 0 || cwdnum == bp->cwdnum ) ) {
      while( bp && bp->linenum != linenum ) {
        bp = bp->nextpoint ;
      }
      if( bp ) {
        if( !bp->set ) {
          DBG_ERROR_BREAKPOINT_IS_NOT_SET( fname, linenum ) ;
          return -1 ;
        }
        clear_breakpoint(bp) ;
        DBG_MESSAGE_BREAKPOINT_IS_CLEARED( fname, linenum ) ;
        return bp->linenum ;
      }
      else {
        DBG_ERROR_BREAKPOINT_CAN_NOT_BE_LOCATED_IN_FILE( linenum, fname ) ; 
      }
    }
    bp = bp->next ;
  }
  DBG_ERROR_UNKNOWN_FILE( cwd ) ; 
  return -1 ;
}
  
JD_debug_stack *search_thread_id( JD_debug_stack *stack, int id ) {
  JD_debug_stack *ret, *tmp ;
  
  if( stack->active && stack->thrd->id == id ) return stack ;
  
  if( stack->callee && (ret = search_thread_id( stack->callee, id )) )
    return ret ;
  if( stack->forkee ) {
    tmp = stack->forkee ;
    while( tmp ) {
      ret = search_thread_id( tmp, id ) ;
      if( ret ) return ret ;
      tmp = tmp->sb ;
    }
  }
  
  tmp = stack->hb ;
  if( tmp ) {
    ret = search_thread_id( tmp, id ) ;
    if( ret ) return ret ;
  }
  return NULL ;
  
}

void JD_debugger_switch_thread( int id ) {
  JD_debug_stack *stack ;

  stack = JD_current_toplevel->top_dbg_stack ;

  stack = search_thread_id( stack, id ) ;
  
  if( stack ) JD_debugged_stack = stack ;
  else {
    DBG_ERROR_CANNOT_FIND_THREAD_ID( id ) ;
  }  
}
