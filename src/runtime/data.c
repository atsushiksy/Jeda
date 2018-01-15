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
#ifndef NO_INCLUDE

#include <stdio.h>
#include "runtime.h"
#include "exec_macro.h"
#include "runtime_error_msg.h"
#include "thread.h"

#ifdef PREALLOCATE_CODE

#include "veriuser.h"
#include "acc_user.h"
#include "pli.h"

#endif
#include "mem_macro.h"

#endif

#ifdef PRODUCT_CODE
#include "../include/license.h"
  static int data_check_ok = 0 ;
  void JD_data_check( int r, int k ) {
    if( check_dsa_key( r, k ) ) {
      fprintf( stderr, "Jeda license code error (%d %d) detected\n", r, k ) ;
      exit(1) ;
    }
    data_check_ok = 1 ;
  }
#endif

#ifdef NOTIFY_GC_STATUS
  extern int JD_pop_stack_int_num ;
  extern int JD_vstack_ovflw_int_num ;
  extern int JD_vstack_ovflw_smlint_num ;
  extern int JD_vstack_ovflw_nonint_num ;
  extern int JD_push_acc_vstack_ovflw_int_num ;
  extern int JD_flush_vstack_int_num ;
  extern int JD_flush_vstack_smlint_num ;
  extern int JD_flush_vstack_nonint_num ;
  extern int JD_pop_acc_dt_num ;
  extern int JD_pop_acc_non_single_num ;
#endif

extern JD_toplevel *JD_current_toplevel ;
extern int JD_code_file_index ;
extern int JD_code_line_num ;

void JD_run_garbage_collection() ;

JD_data *JD_duplicate_data( JD_data *dt ) ;
JD_data *JD_duplicate_single_data( JD_data *dt ) ;

void JD_sd_semaphore(JD_semaphore_structure *sem) ;

static void gc_parse_object( JD_object_table * ) ;
static void gc_parse_hash( JD_hash_entry * ) ;
static void dealloc_data_field(JD_data *) ;
static void gc_mark_data(
  JD_data *d
) ;

#define MARK_DATA(x) gc_mark_data(x)
#define MARK_CHECK 

#ifdef PRODUCT_CODE

static JD_data **mark_data_stack = NULL ;
static int mark_sp = 0 ;
static int mark_data_size = 0 ;

#define MARK_STACK_SIZE 17408
#define MARK_STACK_SIZE_INC 4096

#undef MARK_DATA(x)
#define MARK_DATA(x) add_mark_data(x) 
#undef MARK_CHECK
#define MARK_CHECK mark_check()

static void add_mark_data( JD_data *d ) {
  if( mark_data_stack == NULL ) {
    mark_data_stack = (JD_data **)malloc(MARK_STACK_SIZE*sizeof(JD_data *)) ;
    mark_data_size = MARK_STACK_SIZE ;
  }
  if( mark_sp == mark_data_size ) {
    mark_data_stack = 
      (JD_data **)realloc(
        mark_data_stack, (mark_data_size+MARK_STACK_SIZE_INC)*sizeof(JD_data *)
      ) ;
    mark_data_size += MARK_STACK_SIZE_INC ;
    fprintf(stderr, "Increase mark_stack to %d\n", mark_data_size ) ;
  }

  mark_data_stack[mark_sp++] = d ;
}

static void mark_check() {
  JD_data *d ;
  while( mark_sp > 0 ) {
    d = mark_data_stack[--mark_sp] ;
    gc_mark_data(d) ;
  }
}

#endif

/* defines the number of initial entries for data */
/* 1024 is picked to form n times pagesize based on 8 byte per
 * data entry
 */
/* multipled by 4 for a bit speed up */
#define  INITIAL_JD_DATA  16384  /* 4096 * 4 */
#define  EXTEND_JD_DATA   16384  /* 4096 * 4 */
#define  EXTEND_THRESHOLD 4096

/* when a data servived the garbage collection for the given times,
   then it will be mode to generation1.
 */
#define  GO_GEN1  50 


static JD_data  *data_top ;
static JD_data  *free_data_top ;
static JD_data  *generation0 ;
static JD_data  *generation1 ;
static JD_data  *last_generation0 = NULL ;


static int jd_data_cache_ptr ;

static int num_thread ;
static int num_queue ;
static int num_single ;
static int num_int ;
static int num_bit ;
static int num_sbit ;
static int num_vbit ;
static int num_signal ;
static int num_string ;
static int num_other ;
static int num_object ;
static int num_sysobject ;
static int num_pointer ;
static int num_array ;
static int num_marray ;
static int num_assoc ;
static int num_strix ;
static int num_list ;
static int num_cache_hit = 0 ;
static int num_cache_miss = 0 ;

#ifdef PREALLOCATE_CODE
#define INITIAL_STACK_NUM 1024
JD_data **JD_stack_top = NULL ;
#define INITIAL_THREAD_NUM 1024
JD_thread *JD_thread_top = NULL ;
#define INITIAL_BIT_NUM 8192
#define STANDARD_BIT_SIZE 4
int *JD_bit_top = NULL ;
#define INITIAL_HASH_TABLE_NUM 1024
JD_hash_entry **JD_hash_table_top = NULL ;
#define INITIAL_HASH_ENTRY_NUM 8192
JD_hash_entry *JD_hash_entry_top = NULL ;
#define INITIAL_DATA_UNIT_NUM 8192
JD_data_unit *JD_data_unit_top = NULL ;
#define INITIAL_OBJ_TBL_NUM 4096
JD_object_table *JD_obj_table_top = NULL ;
#define INITIAL_OBJ_TBL_TBL_NUM 4096
JD_data **JD_obj_table_table_top = NULL ;
#define INITIAL_PLI_RQ_NUM 1024
JD_pli_request *JD_pli_rq_top = NULL ;
#endif

static int data_count ;

static char *file_name = "" ;
static int line_num = -1 ;

/* how many temp GC stack entries */
#define TEMP_GC_STACK_SIZE 256
static JD_data  **work_data = NULL ;
static int  work_data_size = 0 ;
static int work_data_sp = 0 ;

static void clear_generation0() ;

static void push_work_data( JD_data *dt ) {
  if( work_data == NULL ) {
     work_data = 
       (JD_data **)calloc(TEMP_GC_STACK_SIZE, sizeof(JD_data *) ) ;
     work_data_size = TEMP_GC_STACK_SIZE ;
  }
  if( work_data_sp == work_data_size ) {
     work_data = 
       (JD_data **)realloc(
         work_data, (work_data_size+TEMP_GC_STACK_SIZE)*sizeof(JD_data *) 
       ) ;
     work_data_size += TEMP_GC_STACK_SIZE ;
  }
  work_data[work_data_sp++] = dt ;
}

static void pop_work_data() {
  if( work_data_sp == 0 ) {
    printf( 
      "JEDA(%s:%d) data fatal error:pop_work_data == 0!!\n", 
      file_name, line_num 
    ) ;
    exit(1) ;
  }
  work_data_sp-- ;
}

static JD_hash_entry  **work_hash = NULL ;
static int  work_hash_size = 0 ;
static int work_hash_sp = 0 ;

static void push_work_hash( JD_hash_entry *ent ) {
  if( work_hash == NULL ) {
     work_hash = 
       (JD_hash_entry **)calloc(TEMP_GC_STACK_SIZE, sizeof(JD_hash_entry *) ) ;
     work_hash_size = TEMP_GC_STACK_SIZE ;
  }
  if( work_hash_sp == work_hash_size ) {
     work_hash = 
       (JD_hash_entry **)realloc(
         work_hash, (work_hash_size+TEMP_GC_STACK_SIZE)*sizeof(JD_hash_entry *) 
       ) ;
     work_hash_size += TEMP_GC_STACK_SIZE ;
  }
  work_hash[work_hash_sp++] = ent ;
}

static void pop_work_hash() {
  if( work_hash_sp == 0 ) {
    printf( 
      "JEDA(%s:%d) data fatal error:work_hash_sp == 0!!\n", 
      file_name, line_num 
    ) ;
    exit(1) ;
  }
  work_hash_sp-- ;
}

void JD_data_fatal_error() {
  printf( "JEDA(%s:%d) data fatal error!!\n", file_name, line_num ) ;
  exit(1) ;
}

/* 
 *  JD_set_data_info are used to switch the top level 
 *  info. for GC to run more than one Jeda top structure.
 */
void JD_set_data_info(
  JD_data  *n_dt,
  JD_data  *n_ft,
  JD_data  *n_g0,
  JD_data  *n_g1
)
{
  data_top = n_dt ;
  free_data_top = n_ft ;
  generation0 = n_g0 ;
  generation1 = n_g1 ;
}

void JD_get_data_info(
  JD_data  **n_dt,
  JD_data  **n_ft,
  JD_data  **n_g0,
  JD_data  **n_g1
)
{
  *n_dt = data_top ;
  *n_ft = free_data_top ;
  *n_g0 = generation0 ;
  *n_g1 = generation1 ;
}


/* 
 * Initial data allocation.
 * A chunk of data (may be the size of multiple pages) is allocated before
 * the operation in a continuous memory space, in a hope of increasing cache
 * hit rate.
 */
void JD_inital_data_allocation()
{
  int i ;
  
  data_top = (JD_data *)calloc( INITIAL_JD_DATA, sizeof( JD_data ) ) ;
  if( data_top == NULL ) {
    ERROR_RUNTIME_FATAL_NO_MEMORY ;
  }
  
  /* first entry point to the next block of data */
  data_top[0].generation = NULL ;

  /* connect all the new entry to generation0 link */
  for( i = 1 ; i < INITIAL_JD_DATA-2 ; i++ ) {
    data_top[i].generation = &data_top[i+1] ;  /* free link */
  }
  free_data_top = &data_top[1] ;
  generation0 = NULL ;
  last_generation0 = NULL ; 
  generation1 = NULL ;

#ifdef PREALLOCATE_CODE
  JD_stack_top = 
    (JD_data **)calloc( 
      (INITAL_STACK_SIZE * INITIAL_STACK_NUM), sizeof(JD_data *) 
    ) ;
  for( i = 0 ; i < INITIAL_STACK_NUM-1 ; i++ ) {
    JD_stack_top[i*INITAL_STACK_SIZE] = 
      (JD_data *)&JD_stack_top[(i+1)*INITAL_STACK_SIZE] ;
  }
  
  JD_thread_top = 
    (JD_thread *)calloc( INITIAL_THREAD_NUM, sizeof(JD_thread) ) ;
  for( i = 0 ; i < INITIAL_THREAD_NUM-1 ; i++ ) {
    JD_thread_top[i].queue = (void *)&JD_thread_top[i+1] ;
  }
  
  /*
  JD_bit_top = 
    (int *)calloc( INITIAL_BIT_NUM * STANDARD_BIT_SIZE, sizeof( int ) ) ;
  
  for( i = 0 ; i < INITIAL_BIT_NUM-1 ; i++ ) {
    JD_bit_top[i*STANDARD_BIT_SIZE] = 
      (int)&JD_bit_top[(i+1)*STANDARD_BIT_SIZE] ;
  }
  */
  
  JD_hash_table_top = 
    (void *)calloc( 
      INITIAL_HASH_TABLE_NUM * JD_DATA_HASH_SIZE, sizeof( JD_hash_entry * ) 
    ) ;  
  for( i = 0 ; i < INITIAL_HASH_TABLE_NUM-1 ; i++ ) {
    JD_hash_table_top[i*JD_DATA_HASH_SIZE] = 
      (void *)&JD_hash_table_top[(i+1)*JD_DATA_HASH_SIZE] ;
  }
  
  JD_hash_entry_top =
    (JD_hash_entry *)calloc( 
      INITIAL_HASH_ENTRY_NUM, sizeof( JD_hash_entry ) 
    ) ;
  for( i = 0 ; i < INITIAL_HASH_ENTRY_NUM-1 ; i++ ) {
    JD_hash_entry_top[i].right = 
      (void *)&JD_hash_entry_top[i+1] ;
  }

  JD_data_unit_top =
    (JD_data_unit *)calloc( 
      INITIAL_DATA_UNIT_NUM, sizeof( JD_data_unit ) 
    ) ;
  for( i = 0 ; i < INITIAL_DATA_UNIT_NUM-1 ; i++ ) {
    JD_data_unit_top[i].pre = 
      (void *)&JD_data_unit_top[i+1] ;
  }

  JD_obj_table_top =
    (JD_object_table *)calloc( 
      INITIAL_OBJ_TBL_NUM, sizeof( JD_object_table ) 
    ) ;
  for( i = 0 ; i < INITIAL_OBJ_TBL_NUM-1 ; i++ ) {
    JD_obj_table_top[i].super = 
      (void *)&JD_obj_table_top[i+1] ;
  }

  JD_obj_table_table_top =
    (JD_data **)calloc( 
      INITIAL_OBJ_TBL_TBL_NUM * INITAL_OBJ_TABLE_SIZE, sizeof( JD_data * ) 
    ) ;
  for( i = 0 ; i < INITIAL_OBJ_TBL_TBL_NUM-1 ; i++ ) {
    JD_obj_table_table_top[i * INITAL_OBJ_TABLE_SIZE] = 
      (void *)&JD_obj_table_table_top[(i+1) * INITAL_OBJ_TABLE_SIZE] ;
  }
  
  JD_pli_rq_top =
    (JD_pli_request *)calloc( 
      INITIAL_PLI_RQ_NUM, sizeof( JD_pli_request ) 
    ) ;
  for( i = 0 ; i < INITIAL_PLI_RQ_NUM-1 ; i++ ) {
    JD_pli_rq_top[i].next = 
      (void *)&JD_pli_rq_top[i+1] ;
  }

#endif
    
}


#ifdef PREALLOCATE_CODE
JD_data **JD_allocate_stack( int size ) {
  JD_data **ret ;
  if( JD_stack_top && size == INITAL_STACK_SIZE ) {
    ret = JD_stack_top ;
    JD_stack_top = (void *)JD_stack_top[0] ;
  } 
  else ret = (JD_data **)calloc(size, sizeof(JD_data *)) ;
  
  return ret ;
}

void JD_deallocate_stack( JD_data **old, int size ) {
  if( size == INITAL_STACK_SIZE ) {
    *old = (void *)JD_stack_top ;
    JD_stack_top = old ;
  } 
  else free(old) ;
  
}

JD_data **JD_reallocate_stack( JD_data **old, int size, int old_size ) {
  JD_data **ret ;

  if( old_size == INITAL_STACK_SIZE ) {
    ret = (JD_data **)calloc(size, sizeof(JD_data *)) ;
    bcopy( old, ret, old_size*sizeof(JD_data *)) ;
    *old = (void *)JD_stack_top ;
    JD_stack_top = old ;
  } 
  else {
    ret = (JD_data **)realloc(old, (size)*sizeof(JD_data *)) ;
  }
  
  return ret ;
  
}

JD_thread *JD_allocate_thread()
{
  JD_thread *ret ;
  if( JD_thread_top ) {
    ret = JD_thread_top ;
    JD_thread_top = (JD_thread *)ret->queue ;
    bzero(ret, sizeof(JD_thread) ) ;
  }
  else ret = (JD_thread *)calloc(1,sizeof(JD_thread)) ;

  return ret ;
}

void JD_deallocate_thread( JD_thread *thrd )
{
  thrd->queue = (void *)JD_thread_top ;
  JD_thread_top = thrd ;
}

/*
int *JD_allocate_bit( int size )
{
  int *ret ;
  if( JD_bit_top && size <= STANDARD_BIT_SIZE ) {
    ret = JD_bit_top ;
    JD_bit_top = (int *)*ret ;
    *ret = 0 ;
  }
  else ret = (int *)calloc(size,sizeof(int)) ;

  return ret ;
}

void JD_deallocate_bit( int *bit, int size )
{
  if( size <= (STANDARD_BIT_SIZE*32) ) {
    *bit = (int)JD_bit_top ;
    JD_bit_top = bit ;
  }
  else free(bit) ;
}
*/

JD_hash_entry **JD_allocate_hash_tbl( int size )
{
  JD_hash_entry **ret ;
  int i ;
  
  if( JD_hash_table_top ) {
    ret = JD_hash_table_top ;
    JD_hash_table_top = (void*)*ret ;
    bzero(ret, JD_DATA_HASH_SIZE*sizeof(JD_hash_entry *) ) ;
  }
  else ret = (JD_hash_entry **)calloc(size,sizeof(JD_hash_entry *)) ;
  
  return ret ;
  
}

void JD_deallocate_hash_tbl( JD_hash_entry **hst )
{
  *hst = (void *)JD_hash_table_top ;
  JD_hash_table_top = (void *)hst ;  
}

JD_hash_entry *JD_allocate_hash_entry()
{
  JD_hash_entry *ret ;
  if( JD_hash_entry_top ) {
    ret = JD_hash_entry_top ;
    JD_hash_entry_top = (void *)ret->right ;
    ret->right = NULL ;
    bzero(ret, sizeof(JD_hash_entry) ) ;
  }
  else ret = (JD_hash_entry *)calloc(1,sizeof(JD_hash_entry)) ;
  return ret ;
}

void JD_deallocate_hash_entry(JD_hash_entry *hsh)
{
  hsh->right = (void *)JD_hash_entry_top ;
  JD_hash_entry_top = hsh ;
}

JD_data_unit *JD_allocate_data_unit( int size )
{
  JD_data_unit *ret ;
  if( JD_data_unit_top && size == 1) {
    ret = JD_data_unit_top ;
    JD_data_unit_top = (void *)ret->pre ;
    bzero(ret, sizeof(JD_data_unit) ) ;
  }
  else {
    ret = 
      (JD_data_unit *)calloc(size, sizeof(JD_data_unit) ) ;
  }
  return ret ;
}

void JD_deallocate_data_unit(JD_data_unit *du, int size)
{
  if( size == 1 ) {
    du->pre = (void *)JD_data_unit_top ;
    JD_data_unit_top = du ;
  }
  else free(du) ;
}

JD_object_table *JD_allocate_obj_table()
{
  JD_object_table *ret ;
  if( JD_obj_table_top ) {
    ret = JD_obj_table_top ;
    JD_obj_table_top = (void *)ret->super ;
    bzero(ret, sizeof(JD_object_table) ) ;
  }
  else ret = (JD_object_table *)calloc(1,sizeof(JD_object_table)) ;
  
  return ret ;
}

void JD_deallocate_obj_table(JD_object_table *obj)
{
  obj->super = (void *)JD_obj_table_top ;
  JD_obj_table_top = obj ;
}

JD_data **JD_allocate_obj_table_table( int size )
{
  JD_data **ret ;
  if( JD_obj_table_table_top && size == INITAL_OBJ_TABLE_SIZE ) {
    ret = JD_obj_table_table_top ;
    JD_obj_table_table_top = (void *)JD_obj_table_table_top[0] ;
  }
  else ret = (JD_data **)calloc(size, sizeof(JD_data *)) ;

  return ret ;
}

void JD_deallocate_obj_table_table(JD_data **tbl, int size)
{
  if( size == INITAL_OBJ_TABLE_SIZE ) {
    *tbl = (void *)JD_obj_table_table_top ;
    JD_obj_table_table_top = tbl ;
  }
  else free(tbl) ;
}

void JD_reallocate_obj_table_table(JD_object_table *tb)
{
  JD_data **old ;
  int i ;
  old = tb->table ;
  tb->tbl_size += INITAL_OBJ_TABLE_SIZE ;
  tb->table = (JD_data **)calloc((tb->tbl_size),sizeof(JD_data *)) ;
  for( i = 0 ; i < tb->num_entry ; i++ ) {
    tb->table[i] = old[i] ;
  }
  if( tb->num_entry == INITAL_OBJ_TABLE_SIZE ) {
    *old = (void *)JD_obj_table_table_top ;
    JD_obj_table_table_top = old ;
  }
  else free(old) ;
}

void *JD_allocate_pli_rq()
{
  JD_pli_request *ret ;
  if( JD_pli_rq_top ) {
    ret = (void *)JD_pli_rq_top ;
    JD_pli_rq_top = (void *)ret->next ;
    bzero(ret, sizeof(JD_pli_request) ) ;
  }
  else ret = (JD_pli_request *)calloc(1,sizeof(JD_pli_request)) ;

  return (void *)ret ;
}

void JD_deallocate_pli_rq( void *dt )
{
  JD_pli_request *rq ;
  rq = (void *)dt ;
  rq->next = (void *)JD_pli_rq_top ;
  JD_pli_rq_top = (void *)rq ;
}
  
#endif

/* Free all the data block */
/*
void JD_data_deallocation()
{
  JD_data  *dt, *next ;
  
  dt = data_top ;
  
  while( dt ) {
    next = dt->generation
    FREE(dt) ;
    dt = next ;
  }
  
}
*/


static extend_data_allocation() {
  int i ;
  JD_data  *top, *new_data ;
  JD_data  *free_data_end ;
  
#ifdef JD_DATA_VERBOSE
  PRINTF( 
    "extend_data_allocation: extending %d entries\n", EXTEND_JD_DATA 
  ) ;
#endif
#ifdef NOTIFY_GC_EXEC
  PRINTF( 
    "extend_data_allocation: extending %d entries\n", EXTEND_JD_DATA 
  ) ;
#endif

  new_data = (JD_data *)calloc( EXTEND_JD_DATA, sizeof( JD_data ) ) ;
  
  if( new_data == NULL ) {
    ERROR_RUNTIME_FATAL_NO_MEMORY ;
  }
  /* chaining the first entry of the block to the next block of data */
  top = &data_top[0] ;
  while( top->generation != NULL ) top = top->generation ;
  top->generation = new_data ;
  new_data[0].generation = NULL ;
  
  /* connect all the new entry to generation0 link */
  for( i = 1 ; i < EXTEND_JD_DATA-2 ; i++ ) {
    new_data[i].generation = &new_data[i+1] ;
  }
  if( free_data_top ) {
    free_data_end = free_data_top ;
    while( free_data_end->generation )
      free_data_end = free_data_end->generation ;
    free_data_end->generation = &new_data[1] ;
  }
  else {
    free_data_top = &new_data[1] ;
  }
  
}


void JD_check_vstack(JD_data *d) {
  int vsp ;
  
  if( VSP ) {
    vsp = VSP ;
    while(vsp) {
      if( VS_TOP < vsp ) {
        if( d == VSTACK[VSP_LIMIT+VS_TOP-vsp] ) JD_data_fatal_error() ;
      }
      else {
        if( d == VSTACK[VS_TOP-vsp] ) JD_data_fatal_error() ;
      }
      vsp-- ;
    }
  }

  if( ACC_TYPE == JD_D_dt ) {
    if( d == ACC_DT) JD_data_fatal_error() ;
  }
}


static void check_data( JD_data *d, JD_data *dd ) {
  int j ;
  
  if( d ) {
    if( d == dd ) JD_data_fatal_error() ;
    switch( d->sub_type ) {
      case JD_SD_array:
        for( j = 0 ; j < d->data.array.size ; j++ ) {
          check_data( d->data.array.body[j].data, dd ) ;
        }
        break ;
      case JD_SD_single:
        if( d->type == JD_D_object && d->data.obj.ect ) {
          for( j = 0 ; j < d->data.obj.ect->num_entry ; j++ ) {
            check_data( d->data.obj.ect->table[j], dd ) ;
          }
        }
        break ;
    }
  }
}

void JD_check_stack( JD_data *dd )
{
  JD_data **stack ;
  int i, j ;
  JD_data *d ;
  if( THREAD && THREAD->this && THREAD->this == dd ) JD_data_fatal_error() ;
  for( i = 0 ; i < SP ; i++ ) {
    check_data(STACK[i], dd) ;
  }
}

void JD_check_unfree( JD_data *dt ) {
  JD_data *fp = free_data_top ;
  
  while( fp ) {
    if( fp == dt ) JD_data_fatal_error() ;
    fp = fp->generation ;
  }
  
}

void JD_check_ungen0( JD_data *dt ) {
  JD_data *fp = generation0 ;
  
  while( fp ) {
    if( fp == dt ) JD_data_fatal_error() ;
    fp = fp->generation ;
  }
  
}

void JD_check_unfree_object( JD_object_table *object )
{
  int i ;
  JD_data *d ;
  
  if( object ) {
    for( i = 0 ; i < object->num_entry ; i++ ) {
      d = object->table[i] ;
      JD_check_unfree(d) ;
    }
  }
}

void JD_check_unfree_hash( JD_hash_entry *hash )
{
  JD_data *d ;
  d = hash->index ;
  if(d) JD_check_unfree(d) ;
  d = hash->data ;
  if(d) JD_check_unfree(d) ;
  if( hash->right ) gc_parse_hash( hash->right ) ;
  if( hash->left ) gc_parse_hash( hash->left ) ;
}


void JD_check_unfree_data(
  JD_data *d
)
{
  int j ;
  JD_data_unit *u ;
  JD_hash_entry *hsh ;
  
  if( MARKED(d) ) JD_data_fatal_error() ;
  JD_check_unfree(d) ;
  
  switch( d->sub_type ) {
    case JD_SD_single:
      switch( d->type ) {
        case JD_D_object:
          if( !IS_SYSOBJECT(d) ) 
            JD_check_unfree_object( d->data.obj.ect ) ;
          break ;
        case JD_D_pointer:
          JD_check_unfree_data(d->data.ptr.pdata) ;
          break ;
      }
      break ;
    case JD_SD_array:
    case JD_SD_marray:
      if( d->type == JD_D_object ) {
        for( j = 0 ; j < d->data.array.size ; j++ ) {
          JD_check_unfree_data( d->data.array.body[j].data ) ;
        }
      }
      break ;
    case JD_SD_list:
      if(d->type == JD_D_object ) {
        u = d->data.list.top ;
        while( u ) {
          JD_check_unfree_data( u->data ) ;
          u = u->next ;
        }
      }
      break ;
    case JD_SD_assoc:
    case JD_SD_strided: /* string indexed */
      for( j = 0 ; j < JD_DATA_HASH_SIZE ; j++ ) {
        hsh = d->data.hash.table[j] ;
        if( hsh ) {
          JD_check_unfree_hash( hsh ) ;
        }
      }
      break ;
  } /* end of switch */

}


JD_assert_unfree_data( JD_data *d, char *fname, int lnum ) {
  file_name = fname ;
  line_num = lnum ;
  JD_check_unfree_data(d) ;
  file_name = "" ;
  line_num = -1 ;
}

static void check_free_link() {
  JD_data *fp = free_data_top ;
  
  while( fp ) {
    if( MARKED(fp) ) JD_data_fatal_error() ;
    fp = fp->generation ;
  }

}


static void count_type_data(
  JD_data *d
)
{
  
  switch( d->sub_type ) {
    case JD_SD_single:
      switch( d->type ) {
        case JD_D_object:
          if( !IS_SYSOBJECT(d) )
            num_object++ ;
          else
            num_sysobject++ ;
          break ;
        case JD_D_pointer:
            num_pointer++ ;
          break ;
        default:
          num_single++ ;
          switch( d->type ) {
            case JD_D_int:
              num_int++ ;
              break ;
            case JD_D_sbit:
              num_sbit++ ;
              break ;
            case JD_D_bit:
              num_bit++ ;
              break ;
            case JD_D_vbit:
              num_vbit++ ;
              break ;
            case JD_D_string:
              num_string++ ;
              break ;
            case JD_D_signal:
              num_signal++ ;
              break ;
            default:
              num_other++ ;
              break ;
          }
          break ;
      }
      break ;
    case JD_SD_array:
      num_array++ ;
      break ;
    case JD_SD_marray:
      num_marray++ ;
      break ;
    case JD_SD_list:
      num_list++ ;
      break ;
    case JD_SD_assoc:
      num_assoc++ ;
      break ;
    case JD_SD_strided: /* string indexed */
      num_strix++ ;
      break ;
  } /* end of switch */

}

/* marc all the JD_data that is in use */
/* This function scans all the threads in all the active queues within
 * the queue-link. Then mark/unmark data which is found in stack or
 * acc in the thread. It also mark the data that is pointed by object
 * table in the case of class instance.
 */
static void gc_mark_data(
  JD_data *d
)
{
  int j ;
  JD_data_unit *u ;
  JD_hash_entry *hsh ;
  
#ifdef DATA_GC_CHECK
  JD_check_unfree(d) ;
#endif

  MARK(d) ;
  
  switch( d->sub_type ) {
    case JD_SD_single:
      switch( d->type ) {
        case JD_D_object:
          if( !IS_SYSOBJECT(d) ) {
            gc_parse_object( d->data.obj.ect ) ;
          }
          break ;
        case JD_D_pointer:
          if( !MARKED(d->data.ptr.pdata ) ) {
            MARK_DATA(d->data.ptr.pdata) ;
          }
          break ;
      }
      break ;
    case JD_SD_array:
      for( j = 0 ; j < d->data.array.size ; j++ ) {
        MARK_DATA( d->data.array.body[j].data ) ;
      }
      break ;
    case JD_SD_marray:
      for( j = 0 ; j < d->data.array.size ; j++ ) {
        MARK_DATA( d->data.array.body[j].data ) ;
      }
      break ;
    case JD_SD_list:
      u = d->data.list.top ;
      while( u ) {
        MARK_DATA( u->data ) ;
        u = u->next ;
      }
      break ;
    case JD_SD_assoc:
      for( j = 0 ; j < JD_DATA_HASH_SIZE ; j++ ) {
        hsh = d->data.hash.table[j] ;
        if( hsh ) {
          gc_parse_hash( hsh ) ;
        }
      }
      break ;
    case JD_SD_strided: /* string indexed */
      for( j = 0 ; j < JD_DATA_HASH_SIZE ; j++ ) {
        hsh = d->data.hash.table[j] ;
        if( hsh ) {
          gc_parse_hash( hsh ) ;
        }
      }
      break ;
  } /* end of switch */

}

void JD_gc_vstack() {
  int vsp ;
  JD_data *d ;
  
  if( VSP ) {
    vsp = VSP ;
    while(vsp) {
      if( VS_TOP < vsp ) d = VSTACK[VSP_LIMIT+VS_TOP-vsp] ;
      else d = VSTACK[VS_TOP-vsp] ;
      if( !MARKED(d) ) {
        gc_mark_data(d) ;
      }
      vsp-- ;
    }
  }
  
  for( vsp = 0 ; vsp < VSP_LIMIT ; vsp++ ) {
    d = VSTACK[vsp] ;
    d->flags_life &= ~(JD_LIFE_MASK) ;
    MARK(d) ;
  }
  
  if( ACC_TYPE == JD_D_dt && ACC_DT ) {
    gc_mark_data(ACC_DT) ;
  }
  if(TMP_DT) gc_mark_data(TMP_DT) ;
  if(work_data) {
    int i ;
    for( i = 0 ; i < work_data_sp ; i++ ) 
      gc_mark_data(work_data[i]) ;
  }
  if(work_hash) {
    int i ;
    for( i = 0 ; i < work_hash_sp ; i++ ) 
      gc_parse_hash(work_hash[i]) ;
  }
  if( S_ACC_TYPE == JD_D_dt && S_ACC_DT ) {
    gc_mark_data(S_ACC_DT) ;
  }

}

void JD_gc_thread(
  JD_thread *thrd, int sp
)
{
  JD_data **stack ;
  int i, j ;
  JD_data *d ;
  num_thread++ ;
  if( thrd->this && !MARKED(thrd->this) ) gc_mark_data(thrd->this) ;
  stack = thrd->stack ;
  for( i = 0 ; i < sp ; i++ ) {
    d = stack[i] ;
    if( !IS_SMLINT(d) && !MARKED(d) ) {
      gc_mark_data(d) ;
    }
  } /* end of for( i = 0 ; i < sp ; i++ ) */
}

void JD_gc_mark(
  JD_queue *top_queue
) 
{
  JD_queue  *queue ;
  JD_thread *thrd ;
  
  queue = top_queue ;
  
  while( queue ) {
    num_queue++ ;
    thrd = queue->thrd_top ;
    while(thrd) {
      JD_gc_thread(thrd, thrd->sp) ;
      thrd = thrd->thrd_bw ;
    } /* end of while (thrd) */
    queue = queue->queue_bw ;
  } /* end of while queue */

}

static void gc_parse_object( JD_object_table *object )
{
  int i ;
  JD_data *d ;
  
  if( object ) {
    if( object->super && !MARKED(object->super) ) MARK_DATA(object->super) ;
    if( object->child && !MARKED(object->child) ) MARK_DATA(object->child) ;
    for( i = 0 ; i < object->num_entry ; i++ ) {
      d = object->table[i] ;
      if( d && !MARKED(d) ) {
        MARK_DATA(d) ;
      }
    }
  }
}

static void gc_parse_hash( JD_hash_entry *hash )
{
  JD_data *d ;
  d = hash->index ;
  if( d && !MARKED(d) ) {
    MARK_DATA(d) ;
  }  
  d = hash->data ;
  if( d && !MARKED(d) ) {
    MARK_DATA(d) ;
  }  
  if( hash->right ) gc_parse_hash( hash->right ) ;
  if( hash->left ) gc_parse_hash( hash->left ) ;
}

static void check_generation0() {
  register JD_data *data, **pdata, *next ;
  register int cnt ;
  
  data = generation0 ;

  while( data ) {
    next = data->generation ;
    if( MARKED(data) ) JD_data_fatal_error() ;
    data = next ;
  }

}

/* clean up VSTACK so that they are scaned */
static void clear_flag() {
  register JD_data *data ;
  int i ;
  
  /*
  data = generation1 ;

  while( data ) {
    data->flags_life &= ~(JD_LIFE_MASK | JD_DATA_MARK) ;
    data = data->generation ;
  }
  */
  if( VSTACK ) {
    for( i = 0 ; i < VSP_LIMIT ; i++ ) {
      if( VSTACK[i] ) VSTACK[i]->flags_life &= ~(JD_LIFE_MASK | JD_DATA_MARK) ;
    }
  }
}

void JD_move_data_to_gen1( JD_data *this ) {
  register JD_data *data ;
  
  if( generation0 == this ) {
    generation0 = this->generation ;
    this->generation = generation1 ;
    generation1 = this ;
    return ;
  } 
  data = generation0 ;
  while( data->generation ) {
    if( data->generation == this ) {
      data->generation = this->generation ;
      
      if( this->generation == NULL ) 
        last_generation0 = data ;
      
      this->generation = generation1 ;
      generation1 = this ;
      this->flags_life &= ~(JD_LIFE_MASK | JD_DATA_MARK) ;
      break ;
    }
    data = data->generation ;
  }
  
}

void JD_move_array_data_to_gen1( JD_data *this ) {
  int i ;
  for( i = 0 ; i < this->data.array.size ; i++ ) {
    JD_move_data_to_gen1(this->data.array.body[i].data) ;
  }
  JD_move_data_to_gen1(this) ;
}

void JD_report_gc_status() {
  JD_data *data, **pdata, *next ;
  int gen0_cnt, gen1_cnt, free_cnt ;
  
  gen0_cnt = 0 ;
  gen1_cnt = 0 ;
  free_cnt = 0 ;
  data = generation0 ;
  while( data ) {
    gen0_cnt++ ;
    data = data->generation ;
  }
  data = generation1 ;
  while( data ) {
    gen1_cnt++ ;
    data = data->generation ;
  }
  data = free_data_top ;
  while( data ) {
    free_cnt++ ;
    data = data->generation ;
  }
  printf( 
    "gen0: %d gen1: %d free: %d cache_hit:%d miss:%d\n", 
    gen0_cnt, gen1_cnt, free_cnt, num_cache_hit, num_cache_miss
  ) ;
}
static void clear_generation0() 
{
  register JD_data *data ;
    
  data = generation0 ;

  while( data ) {
    if( !MARKED(data) ) 
    {
      data->flags_life &= ~(JD_DATA_MARK) ;
    }
    data = data->generation ;
  }
}

/*
static void data_break() {

}
*/

/* scan generation0 chain and correct garbage */
static void scan_generation0() 
{
  register JD_data *data, **pdata, *next ;
  register int cnt ;
  /*
  int dbg = 0 ;
  int dbg_cnt = 0 ;
  int dbg_set = -1 ;
  */
  
  data_count = 0 ;
  
  data = generation0 ;
  pdata = &generation0 ; 
  last_generation0 = NULL ;
  
#ifdef DATA_GC_CHECK
  check_free_link() ;
#endif

  while( data ) {
    next = data->generation ;
    /*  debug code
    if( ((int)next) & 3 ) {
      next = NULL ;
      next->generation = NULL ;
    }
    if( dbg ) {
      fprintf( stderr, "gc: %d %d %d\n", dbg_cnt++, data_count, MARKED(data) ) ;
      if( dbg_cnt == dbg_set ) data_break() ;
    }
    */
    
    if( !MARKED(data) ) 
    {

#ifdef NOTIFY_GC_STATUS
      count_type_data(data) ;
#endif

#ifdef DATA_GC_CHECK
      JD_check_vstack(data) ;
      JD_check_stack(data) ;
      JD_check_unfree(data) ;
#endif
      dealloc_data_field(data) ;
      data->flags_life = 0 ;
      data->generation = free_data_top ;
      free_data_top = data ;
      data->type = JD_D_unknown ;
      data->sub_type = JD_SD_single ;
      data_count++ ;
    }
    else {
      cnt = LIFECNT(data)+1 ;
      if( data->sub_type == JD_SD_single &&
          data->type != JD_D_object && data->type != JD_D_pointer &&
          ( cnt > GO_GEN1) ) 
      {
        data->generation = generation1 ;
        generation1 = data ;
        data->flags_life &= ~(JD_LIFE_MASK | JD_DATA_MARK) ;
      }
      else if( ( data->sub_type == JD_SD_array || 
                 data->sub_type == JD_SD_marray   ) &&
          data->type != JD_D_object && data->type != JD_D_pointer &&
          ALLOCATING(data) &&
          ( cnt > GO_GEN1) ) 
      {
        JD_data_unit *ut ;
        int i ;
        *pdata = data->generation ;
        data->generation = generation1 ;
        generation1 = data ;
        data->flags_life &= ~(JD_LIFE_MASK | JD_DATA_MARK) ;
        JD_move_array_data_to_gen1(data) ;
      }
      else {
        *pdata = last_generation0 = data ;
        data->flags_life = 
          (data->flags_life & ~(JD_LIFE_MASK | JD_DATA_MARK) ) | cnt ;
        data->generation = NULL ;
        pdata = &data->generation ;
      }
    }
    data = next ;
  }
}

/* Hash index and data are all JD_data structure, so
   we don't have to worry about collecting them.
   Mark & sweep will collect them. (really?!)
   So here, just clear the b-tree structure.
*/
static void dealloc_hash_entry( JD_hash_entry *hash ) {
  if( hash->left ) dealloc_hash_entry( hash->left ) ;
  if( hash->right ) dealloc_hash_entry( hash->right ) ;
  DEALLOC_HASH_ENTRY( hash ) ;
}

JD_dealloc_obj_table_table( JD_data **table, int num_entry, int tbl_size ) {
  int i ;
  
  /* not needed, they are JD_data that will be GCed by regular GC mechanism
  for( i = 0 ; i < num_entry ; i++ ) {
    dealloc_data_field( table[i] ) ;
  }
  */
  DEALLOC_OBJ_TABLE_TABLE(
    table, tbl_size
  ) ;
  
}

static void dealloc_data_field( JD_data *d ) {
  int j ;
  JD_data_unit *u, *un ;
  JD_hash_entry *hsh ;

  switch( d->sub_type ) {
    case JD_SD_single:
      switch( d->type ) {
        case JD_D_bit:
          DEALLOC_BIT(d->data.bit.a, d->data.bit.size) ;
          DEALLOC_BIT(d->data.bit.b, d->data.bit.size) ;
          break ;
        case JD_D_string:
          DEALLOC_STRING(d->data.string) ;
          break ;
        case JD_D_object:
          if( d->data.obj.ect ) {
            if( d->data.obj.ect->ref_count == 0 ) JD_data_fatal_error() ;
            d->data.obj.ect->ref_count-- ;
            if( d->data.obj.ect->ref_count == 0 ) {
              (d->data.obj.ect->dealloc)( 
                (void *)d->data.obj.ect->table, 
                d->data.obj.ect->num_entry,
                d->data.obj.ect->tbl_size
              ) ;
              DEALLOC_OBJ_TABLE(d->data.obj.ect) ;
            }
          }
          break ;
      }
      break ;
    case JD_SD_marray:
      DEALLOC_DIM_SIZE(d->data.array.dim_size) ;
      /* FALLTHROUGH */
    case JD_SD_array:
      DEALLOC_DATA_UNIT(d->data.array.body, d->data.array.size);
      break ;
    case JD_SD_list:
      u = d->data.list.top ;
      while( u ) {
        un = u->next ;
        DEALLOC_DATA_UNIT(u,1);
        u = un ;
      }
      /* dealloc semaphore structure */
      if( d->data.list.sem ) JD_sd_semaphore( d->data.list.sem ) ;
      break ;
    case JD_SD_assoc:
    case JD_SD_strided: /* string indexed */
        for( j = 0 ; j < JD_DATA_HASH_SIZE ; j++ ) {
          if( d->data.hash.table[j] )
            dealloc_hash_entry( d->data.hash.table[j] ) ;
        }
        DEALLOC_HASH_TBL(d->data.hash.table) ;
      break ;
  } /* end of switch */
}

/***************************************************************
 * The following entry should be better to be a compiled macro
 * to avoid the calling overhead 
 ****************************************************************/

/* allocating a simple single entry */ 
JD_data *JD_alloc_data_entry() 
{
  JD_data *ret ;


#ifdef DATA_GC_CHECK
  check_free_link() ;
#endif
#ifdef DATA_GC_DEBUG
  JD_run_garbage_collection() ;
#endif

  if( free_data_top == NULL ) {
    JD_run_garbage_collection() ;
    if( (free_data_top == NULL) || (data_count < EXTEND_THRESHOLD) ) {
#ifdef NOTIFY_GC_EXEC
      PRINTF( 
        "(data_count = %d) < %d : extending entries\n", 
        data_count, EXTEND_THRESHOLD 
      ) ;
#endif
      extend_data_allocation() ;
    }
  }
  ret = free_data_top ;

  free_data_top = free_data_top->generation ;
  /* Here we connect the data entry by the order of allocation in order
     to find the previous entry on the stack for queck GC
   */
  /* link to generation 0 for GC target */
  if( generation0 == NULL ) generation0 = ret ;
  if( last_generation0 ) last_generation0->generation = ret ;
  last_generation0 = ret ;
  ret->generation = NULL ;
  ret->data.sbit.size = 0 ;
  ret->data.sbit.a = 0 ;
  ret->data.sbit.b = 0 ;
  
#ifdef JEDA_DATA_TRACE
  ret->file_name = JD_code_file_index ;
  ret->location = JD_code_line_num ;
#endif
  return ret ;
}

#ifdef ENABLE_JEDA_DATA_CACHE
JD_dealloc_data_on_cache( JD_data *dt, JD_data *cp ) {
  int i ;
  JD_data *pt, *ct ;
  if( dt->sub_type != JD_SD_single ) return ;
  if( IS_DUPLICATED(dt) ) return ;
  if( 
    dt->type == JD_D_int || dt->type == JD_D_sbit || dt->type == JD_D_signal ||
    dt->type == JD_D_null || dt->type == JD_D_void || 
    dt->type == JD_D_unknown || dt->type == JD_D_enum 
  ) {
    if( cp && cp->generation == dt ) {
       /* JD_check_unfree( dt ) ; */
       cp->generation = dt->generation ;
       if( cp->generation == NULL ) last_generation0 = cp ;
       dt->generation = free_data_top ;
       free_data_top = dt ;
       /* JD_check_ungen0(dt) ; */
#ifdef NOTIFY_GC_STATUS
       num_cache_hit++ ;
#endif
       return ;
    }
#ifdef NOTIFY_GC_STATUS
    num_cache_miss++ ;
#endif
  }
}
#endif

/* alocate & copy a single entry of various type */

void JD_copy_single_data( JD_data *ret, JD_data *dt ) {

  ret->type = dt->type ;
  ret->sub_type = JD_SD_single ;
  switch( dt->type ) {
    case JD_D_sbit:
      ret->data.sbit.size = dt->data.sbit.size ;
      ret->data.sbit.a = dt->data.sbit.a ;
      ret->data.sbit.b = dt->data.sbit.b ;
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        int i, n ;
        ret->type = JD_D_bit ;
        ret->data.bit.size = dt->data.bit.size ;
        n = NWORD(dt->data.bit.size) ;
        ret->data.bit.a = ALLOC_BIT( n ) ;
        ret->data.bit.b = ALLOC_BIT( n ) ;
        for( i = 0 ; i < n ; i++ ) {
          ret->data.bit.a[i] = dt->data.bit.a[i] ;
          ret->data.bit.b[i] = dt->data.bit.b[i] ;
        }
      }
      break ;
    case JD_D_int:
      ret->data.int_value = dt->data.int_value ;
      break ;
    case JD_D_float:
      ret->data.float_value = dt->data.float_value ;
      break ;
    case JD_D_double:
      ret->data.double_value = dt->data.double_value ;
      break ;
    case JD_D_object:
      {
        int i , n;
        JD_data *odt ;
        ret->data.obj.alloc = dt->data.obj.alloc ;
        if( CLONED(dt) && dt->data.obj.ect ) {
          n = dt->data.obj.ect->tbl_size ;
          ret->data.obj.ect = ALLOC_OBJ_TABLE ;
          if( dt->data.obj.ect->duplicate == NULL ) {
            /* if duplicate function is not specified, handle it as
               regular object and duplicate the members
             */
            ret->data.obj.ect->table = ALLOC_OBJ_TABLE_TABLE(n) ;
            ret->data.obj.ect->tbl_size = dt->data.obj.ect->tbl_size ;
            n = dt->data.obj.ect->num_entry ;
            ret->data.obj.ect->num_entry = 0 ;
            if( dt->data.obj.ect->super ) {
              ret->data.obj.ect->super = 
                JD_duplicate_single_data( dt->data.obj.ect->super ) ;
            }
            for( i = 0 ; i < n ; i++ ) {
              odt = dt->data.obj.ect->table[i] ;
              ret->data.obj.ect->table[i] = JD_duplicate_data( odt )  ;
              ret->data.obj.ect->num_entry++ ;
            }
          }
          else {
            /* duplicate function is set, use it to create the dup */
            ret->data.obj.ect->duplicate = dt->data.obj.ect->duplicate ;
            ret->data.obj.ect->copy = dt->data.obj.ect->copy ;
            ret->data.obj.ect->table = 
              (void *)(ret->data.obj.ect->duplicate)(
                (void *)dt->data.obj.ect->table
              ) ;
          }
        }
        else {
          ret->data.obj.ect = dt->data.obj.ect ;
          if( ret->data.obj.ect ) ret->data.obj.ect->ref_count++ ;
        }
      }
      break ;
    case JD_D_string:
      ret->data.string = (char *)strdup(dt->data.string) ;
      break ;
    case JD_D_thread:
      ret->data.thrd = dt->data.thrd ;
      break ;
    case JD_D_signal:
      ret->data.signal.port = dt->data.signal.port ;
      ret->data.signal.data0 = dt->data.signal.data0 ;
      ret->data.signal.data1 = dt->data.signal.data1 ;
      break ;
    case JD_D_module:
      ret->data.module.func0 = dt->data.module.func0 ;
      ret->data.module.func1 = dt->data.module.func1 ;
      ret->data.module.data0 = dt->data.module.data0 ;
      break ;
    case JD_D_pointer:
      ret->data.ptr.pdata = dt->data.ptr.pdata ;
      if( dt->data.ptr.string ) 
        ret->data.ptr.string = (char *)strdup(dt->data.ptr.string) ;
      ret->data.ptr.offset = dt->data.ptr.offset ;
      break ;
    case JD_D_func:
      ret->data.func.func = dt->data.func.func ;
      ret->data.func.data = dt->data.func.data ;
      break ;
    default:
      break ;
  }
}

/* allocating a single entry of various type, used for assoc & strindex array */
JD_data *JD_alloc_single_data( JD_data *dt ) {
  JD_data *ret ;

  ret = JD_alloc_data_entry() ;
  ret->type = dt->type ;
  ret->sub_type = JD_SD_single ;
  ret->base = dt ;
  if( IS_SYSOBJECT(dt) ) SETSYSOBJECT(ret) ;
  switch( dt->type ) {
    case JD_D_sbit:
      ret->data.sbit.size = (int)dt->base ; /* holds size */
      ret->data.sbit.a = 0 ;
      ret->data.sbit.b = 0 ;
      break ;
    case JD_D_vbit:
    case JD_D_bit:
      {
        int i, n ;
        ret->type = JD_D_bit ;
        ret->data.sbit.size = (int)dt->base ; /* holds size */
        n = NWORD(ret->data.sbit.size) ;
        ret->data.bit.a = ALLOC_BIT( n ) ;
        ret->data.bit.b = ALLOC_BIT( n ) ;
        for( i = 0 ; i < n ; i++ ) {
          ret->data.bit.a[i] = 0 ;
          ret->data.bit.b[i] = 0 ;
        }
      }
      break ;
    case JD_D_int:
      ret->data.int_value = 0 ;
      break ;
    case JD_D_float:
      ret->data.float_value = 0 ;
      break ;
    case JD_D_double:
      ret->data.double_value = 0 ;
      break ;
    case JD_D_object:
      ret->data.obj.alloc = (void *)dt->base ; /* holds alloc func pointer */
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
      ret->data.module.data0 = 0 ;
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

/* Create a copy of single data */
JD_data *JD_duplicate_single_data( JD_data *dt ) {
  JD_data *ret ;
  int i, n ;
  
  ret = JD_alloc_data_entry() ;
  
  JD_copy_single_data( ret, dt ) ;
  
  return ret ;
}


JD_hash_entry *JD_duplicate_hash_entry( JD_hash_entry *ent, JD_data *base ) {
  JD_hash_entry *ret ;
  
  if( ent == NULL ) return NULL ;
  
  ret = ALLOC_HASH_ENTRY ;
  ret->type = ent->type ;
  push_work_hash( ret ) ;
  if( ent->index ) 
    ret->index = JD_duplicate_single_data( ent->index ) ; 
  ret->idx = ent->idx ;
  if( ent->string ) 
    ret->string = (char *)strdup(ent->string) ;
  if( ent->data ) 
    ret->data = JD_duplicate_single_data( ent->data ) ;
  ret->data->base = base ;
  if( ent->right ) ret->right = JD_duplicate_hash_entry(ent->right, base) ;
  if( ent->left ) ret->left = JD_duplicate_hash_entry(ent->left, base) ;
  pop_work_hash() ;
  return ret ;
}

JD_data_unit *JD_duplicate_list_entry( JD_data_unit *ut, JD_data *base ) {
  JD_data_unit *ret, *nt ;
  if( ut == NULL )return NULL ;
  ret = ALLOC_DATA_UNIT(1) ;
  ret->data = JD_duplicate_single_data( ut->data ) ;
  ret->data->base = base ;
  return ret ;
}


/* Create a copy of single data */
JD_data *JD_duplicate_data( JD_data *dt ) {
  JD_data *ret, *st ;
  int i, n ;
  if( dt->sub_type == JD_SD_single ) {
    ret = JD_duplicate_single_data( dt ) ;
  }
  else {
    ret = JD_alloc_data_entry() ;
    push_work_data( ret ) ;
    ret->type = dt->type ;
    ret->sub_type = dt->sub_type ;
    ret->base = dt->base ;
    switch( dt->sub_type ) {
      case JD_SD_marray:
        ret->data.array.dim_size = ALLOC_DIM_SIZE(dt->data.array.dim_size[0]) ;
        {
          int i ;
          for( i = 0 ; i < dt->data.array.dim_size[0]+1 ; i++ ) {
            ret->data.array.dim_size[i] = dt->data.array.dim_size[i] ;
          }
        }
        /*FALLTHROUGH*/
      case JD_SD_array:
        n = dt->data.array.size ;
        ret->data.array.body = ALLOC_DATA_UNIT(n) ;
        ret->data.array.size = 0 ;
        for( i = 0 ; i < n ; i++ ) {
          ret->data.array.body[i].data = st =
            JD_duplicate_single_data( dt->data.array.body[i].data ) ;
          st->base = ret ;
          ret->data.array.size++ ;
        }
        break ;
      case JD_SD_assoc:
      case JD_SD_strided:
        ret->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
        for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
          ret->data.hash.table[i] = 
            JD_duplicate_hash_entry( dt->data.hash.table[i], ret ) ;
        }
        break ;
      case JD_SD_list:
        {
          JD_data_unit *ut, *pt, *nt, **nnt ;
          ut = dt->data.list.top ;
          nnt = &ret->data.list.top ;
          pt = NULL ;
          while( ut ) {
            nt = JD_duplicate_list_entry( ut, ret ) ;
            nt->pre = pt ;
            nt->next = NULL ;
            *nnt = nt ;
            pt = nt ;
            nnt = &nt->next ;
            ut = ut->next ;
          }
          ret->data.list.bottom = pt ;
        }
        break ;
    } 
    pop_work_data() ;
  }
  
  work_data = NULL ;
  return ret ;
  
}

JD_data *JD_copy_data( JD_data *ret, JD_data *dt ) {
  JD_data *st ;
  int i, n ;
  if( dt->sub_type == JD_SD_single ) {
    JD_copy_single_data( ret, dt ) ;
  }
  else {
    push_work_data( ret ) ;
    ret->type = dt->type ;
    ret->sub_type = dt->sub_type ;
    switch( dt->sub_type ) {
      case JD_SD_marray:
        ret->data.array.dim_size = ALLOC_DIM_SIZE(dt->data.array.dim_size[0]) ;
        {
          int i ;
          for( i = 0 ; i < dt->data.array.dim_size[0]+1 ; i++ ) {
            ret->data.array.dim_size[i] = dt->data.array.dim_size[i] ;
          }
        }
        /*FALLTHROUGH*/
      case JD_SD_array:
        n = dt->data.array.size ;
        ret->data.array.body = ALLOC_DATA_UNIT(n) ;
        ret->data.array.size = 0 ;
        for( i = 0 ; i < n ; i++ ) {
          ret->data.array.body[i].data = st =
            JD_duplicate_single_data( dt->data.array.body[i].data ) ;
          st->base = ret ;
          ret->data.array.size++ ;
        }
        break ;
      case JD_SD_assoc:
      case JD_SD_strided:
        ret->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
        for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
          ret->data.hash.table[i] = 
            JD_duplicate_hash_entry( dt->data.hash.table[i], ret ) ;
        }
        break ;
      case JD_SD_list:
        {
          JD_data_unit *ut, *pt, *nt, **nnt ;
          ut = ret->data.list.top ;
          ret->data.list.top = NULL ;
          while( ut ) {
            nt = ut->next ;
            DEALLOC_DATA_UNIT(ut,1) ;
            ut = nt ;
          }
          ut = dt->data.list.top ;
          nnt = &ret->data.list.top ;
          pt = NULL ;
          while( ut ) {
            nt = JD_duplicate_list_entry( ut, ret ) ;
            nt->pre = pt ;
            nt->next = NULL ;
            *nnt = nt ;
            pt = nt ;
            nnt = &nt->next ;
            ut = ut->next ;
          }
          ret->data.list.bottom = pt ;
        }
        break ;
    } 
    pop_work_data() ;

  }
  
  return ret ;
  
}

/* Copy non-single data */
void JD_duplicate_data_to( JD_data *ret, JD_data *dt ) {
  JD_data *st ;
  int i, n ;
  push_work_data( ret ) ;
  switch( ret->sub_type ) {
    case JD_SD_marray:
      ret->data.array.dim_size = ALLOC_DIM_SIZE(dt->data.array.dim_size[0]) ;
      {
        int i ;
        for( i = 0 ; i < dt->data.array.dim_size[0]+1 ; i++ ) {
          ret->data.array.dim_size[i] = dt->data.array.dim_size[i] ;
        }
      }
      /*FALLTHROUGH*/
    case JD_SD_array:
      n = dt->data.array.size ;
      ret->data.array.body = ALLOC_DATA_UNIT(n) ;
      ret->data.array.size = 0 ;
      for( i = 0 ; i < n ; i++ ) {
        ret->data.array.body[i].data = st =
          JD_duplicate_single_data( dt->data.array.body[i].data ) ;
        st->base = ret ;
        ret->data.array.size++ ;
      }
      break ;
    case JD_SD_assoc:
    case JD_SD_strided:
      ret->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
      for( i = 0 ; i < JD_DATA_HASH_SIZE ; i++ ) {
        ret->data.hash.table[i] = 
          JD_duplicate_hash_entry( dt->data.hash.table[i], ret ) ;
      }
      break ;
    case JD_SD_list:
      {
        JD_data_unit *ut, *pt, *nt, **nnt ;
        ut = ret->data.list.top ;
        ret->data.list.top = NULL ;
        while( ut ) {
          nt = ut->next ;
          DEALLOC_DATA_UNIT(ut,1) ;
          ut = nt ;
        }
        ut = dt->data.list.top ;
        nnt = &ret->data.list.top ;
        pt = NULL ;
        while( ut ) {
          nt = JD_duplicate_list_entry( ut, ret ) ;
          nt->pre = pt ;
          nt->next = NULL ;
          *nnt = nt ;
          pt = nt ;
          nnt = &nt->next ;
          ut = ut->next ;
        }
        ret->data.list.bottom = pt ;
      }
      break ;
  } 
  pop_work_data() ;
}

/*************************************************************/
/*  Various data allocation                                  */
/*************************************************************/
void JD_push_tmp_dt() {
  /**MACRO PUSH_TMP_DT**/
  PUSH_TO_STACK(TMP_DT) ;
  /**END MACRO PUSH_TMP_DT**/
}

void JD_alloc_single_int() {
  /**MACRO ALLOC_DT_SINGLE_INT**/
  TMP_DT = JD_alloc_data_entry() ;
  TMP_DT->type = JD_D_int ;
  TMP_DT->data.int_value = 0 ;
  TMP_DT->sub_type = JD_SD_single ;
  /**END MACRO ALLOC_DT_SINGLE_INT**/
}

void JD_alloc_single_bit() {
  /**MACRO ALLOC_DT_SINGLE_BIT**/
  {
    int i, n ;
    TMP_DT = JD_alloc_data_entry() ;
    TMP_DT->sub_type = JD_SD_single ;
    if( BIT_SIZE <= 32 ) {
      TMP_DT->data.sbit.size = BIT_SIZE ;
      TMP_DT->type = JD_D_sbit ;
      TMP_DT->data.sbit.a = TMP_DT->data.sbit.b = 
        ((unsigned)0xffffffff)>>(32-BIT_SIZE) ;
    }
    else {
      TMP_DT->type = JD_D_bit ;
      TMP_DT->data.bit.size = BIT_SIZE ;
      n = ((BIT_SIZE-1)/32)+1 ;
      TMP_DT->data.bit.a = ALLOC_BIT( n ) ;
      TMP_DT->data.bit.b = ALLOC_BIT( n ) ;
      for( i = 0 ; i < n-1 ; i++ ) 
        TMP_DT->data.bit.a[i] = TMP_DT->data.bit.b[i] = 0xffffffff ;
      TMP_DT->data.bit.a[i] = TMP_DT->data.bit.b[i] = 
        (unsigned)0xffffffff >> ((32-(BIT_SIZE%32))%32) ;
    }
  }
  /**END MACRO ALLOC_DT_SINGLE_BIT**/
}

void JD_alloc_single_float() {
  /**MACRO ALLOC_DT_SINGLE_FLOAT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_float ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.float_value = 0.0 ;
  }
  /**END MACRO ALLOC_DT_SINGLE_FLOAT**/  
}

void JD_alloc_single_double() {
  /**MACRO ALLOC_DT_SINGLE_DOUBLE**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_double ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.double_value = 0.0 ;
  }
  /**END MACRO ALLOC_DT_SINGLE_DOUBLE**/  
}

void JD_alloc_single_string() {
  /**MACRO ALLOC_DT_SINGLE_STRING**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_string ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.string = NULL ;
  }
  /**END MACRO ALLOC_DT_SINGLE_STRING**/  
}

void JD_alloc_single_signal() {
  /**MACRO ALLOC_DT_SINGLE_SIGNAL**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_signal ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.signal.port = NULL ;
  }
  /**END MACRO ALLOC_DT_SINGLE_SIGNAL**/  
}

/* ALLOC_OBJ should hold the pointer to the alloc function */
void JD_alloc_single_obj() {
  /**MACRO ALLOC_DT_SINGLE_OBJ**/
  {  
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_object ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.obj.alloc = ALLOC_OBJ ;
    TMP_DT->data.obj.ect = NULL ;  
    /* pointing to the object alloc func */
  }
  /**END MACRO ALLOC_DT_SINGLE_OBJ**/  
}

void JD_alloc_single_thread() {
  /**MACRO ALLOC_DT_SINGLE_THREAD**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_thread ;
    TMP_DT->sub_type = JD_SD_single ;
    TMP_DT->data.thrd = NULL ;
  }
  /**END MACRO ALLOC_DT_SINGLE_THREAD**/  
}

JD_data *JD_alloc_single( JD_data *dt, char *info ) {
  JD_data *ret ;
  switch(dt->type) {
    case JD_D_int:
      ret = JD_alloc_data_entry() ;
      ret->type = JD_D_int ;
      ret->data.int_value = 0 ;
      ret->sub_type = JD_SD_single ;
      break ;
    case JD_D_sbit:
      ret = JD_alloc_data_entry() ;
      ret->sub_type = JD_SD_single ;
      ret->data.sbit.size = (int)info ;
      ret->type = JD_D_sbit ;
      ret->data.sbit.a = ret->data.sbit.b = 
        ((unsigned)0xffffffff)>>(32 - ret->data.sbit.size) ;
    case JD_D_bit:
      {
        int i, n ;
        ret = JD_alloc_data_entry() ;
        ret->sub_type = JD_SD_single ;
        ret->type = JD_D_bit ;
        ret->data.bit.size = (int)info ;
        n = ((ret->data.bit.size-1)/32)+1 ;
        ret->data.bit.a = ALLOC_BIT( n ) ;
        ret->data.bit.b = ALLOC_BIT( n ) ;
        for( i = 0 ; i < n-1 ; i++ ) 
          ret->data.bit.a[i] = ret->data.bit.b[i] = 0xffffffff ;
        ret->data.bit.a[i] = ret->data.bit.b[i] = 
          (unsigned)0xffffffff >> ((32-(ret->data.bit.size%32))%32) ;
      }
      break ;
    case JD_D_float:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_float ;
      ret->sub_type = JD_SD_single ;
      ret->data.float_value = 0.0 ;
      break ;
    case JD_D_double:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_double ;
      ret->sub_type = JD_SD_single ;
      ret->data.double_value = 0.0 ;
      break ;
    case JD_D_string:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_string ;
      ret->sub_type = JD_SD_single ;
      ret->data.string = NULL ;
      break ;
    case JD_D_signal:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_signal ;
      ret->sub_type = JD_SD_single ;
      ret->data.signal.port = NULL ;
      break ;
    case JD_D_object:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_object ;
      ret->sub_type = JD_SD_single ;
      ret->data.obj.alloc = (void *)info ;  
      ret->data.obj.ect = NULL ;
      break ;
    case JD_D_thread:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_thread ;
      ret->sub_type = JD_SD_single ;
      ret->data.thrd = NULL ;
      break ;
    case JD_D_func:
      ret = JD_alloc_data_entry();
      ret->type = JD_D_func ;
      ret->sub_type = JD_SD_single ;
      ret->data.func.func = NULL ;
      ret->data.func.data = NULL ;
      break ;
  }
  if( IS_SYSOBJECT(dt) ) SETSYSOBJECT(ret) ;
  return ret ;
}

/*************************************************/
/* array data ARRAY_SIZE should hold the size    */
/*************************************************/
JD_data *JD_alloc_int() {
  JD_data *dt ;
  dt = JD_alloc_data_entry() ;
  dt->type = JD_D_int ;
  dt->data.int_value = 0 ;
  dt->sub_type = JD_SD_single ;
  return dt ;
}

JD_data *JD_alloc_bit( int BIT_SIZE ) {
  JD_data *dt ;
  int i, n ;
  dt = JD_alloc_data_entry() ;
  dt->sub_type = JD_SD_single ;
  if( BIT_SIZE <= 32 ) {
    dt->data.sbit.size = BIT_SIZE ;
    dt->type = JD_D_sbit ;
    dt->data.sbit.a = dt->data.sbit.b = 
      ((unsigned)0xffffffff)>>(32-BIT_SIZE) ;
  }
  else {
    dt->type = JD_D_bit ;
    dt->data.bit.size = BIT_SIZE ;
    n = ((BIT_SIZE-1)/32)+1 ;
    dt->data.bit.a = ALLOC_BIT( n ) ;
    dt->data.bit.b = ALLOC_BIT( n ) ;
    for( i = 0 ; i < n-1 ; i++ ) 
      dt->data.bit.a[i] = dt->data.bit.b[i] = 0xffffffff ;
    dt->data.bit.a[i] = dt->data.bit.b[i] = 
      (unsigned)0xffffffff >> ((32-(BIT_SIZE%32))%32) ;
  }
  return dt ;
}


void JD_alloc_array_int() {
  /**MACRO ALLOC_DT_ARRAY_INT**/
  {
    JD_data *st ;
    JD_data_unit *du ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_int ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_int() ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_INT**/  
}

void JD_alloc_array_bit() {
  /**MACRO ALLOC_DT_ARRAY_BIT**/
  {
    JD_data *st ;
    JD_data_unit *du ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = (BIT_SIZE <= 32)? JD_D_sbit: JD_D_bit ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_bit( BIT_SIZE ) ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_BIT**/  
}

void JD_alloc_array_float() {
  /**MACRO ALLOC_DT_ARRAY_FLOAT**/
  {
    JD_data *st ;
    JD_data_unit *du ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_float ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_float ;
      st->sub_type = JD_SD_single ;
      st->data.float_value = 0.0 ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_FLOAT**/  
}

void JD_alloc_array_double() {
  /**MACRO ALLOC_DT_ARRAY_DOUBLE**/
  {
    JD_data *st ;
    JD_data_unit *du ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_double ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_double ;
      st->sub_type = JD_SD_single ;
      st->data.float_value = 0.0 ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_DOUBLE**/  
}

void JD_alloc_array_string() {
  /**MACRO ALLOC_DT_ARRAY_STRING**/
  {
    JD_data *st ;
    JD_data_unit *du ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_string ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_string ;
      st->sub_type = JD_SD_single ;
      st->data.string = NULL ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_STRING**/  
}

void JD_alloc_array_signal() {
  /**MACRO ALLOC_DT_ARRAY_SIGNAL**/
  {
    JD_data_unit *du ;
    JD_data *st ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_signal ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_signal ;
      st->sub_type = JD_SD_single ;
      st->data.signal.port = NULL ;
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_SIGNAL**/  
}

void JD_alloc_array_obj() {
  /**MACRO ALLOC_DT_ARRAY_OBJ**/
  {  
    JD_data_unit *du ;
    JD_data *st ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_object ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = 0 ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_object ;
      st->sub_type = JD_SD_single ;
      st->data.obj.ect = NULL ;
      st->data.obj.alloc = ALLOC_OBJ ;  /* pointing to the object alloc func */
      st->base = TMP_DT ;
      TMP_DT->data.array.size++ ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_OBJ**/  
}

void JD_alloc_array_thread() {
  /**MACRO ALLOC_DT_ARRAY_THREAD**/
  {
    JD_data_unit *du ;
    JD_data *st ;
    int i ;
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_thread ;
    TMP_DT->sub_type = JD_SD_array ;
    TMP_DT->data.array.size = ARRAY_SIZE ;
    TMP_DT->data.array.body = ALLOC_DATA_UNIT(ARRAY_SIZE) ;
    SETALLOCATE(TMP_DT) ;
    for( i = 0 ; i < ARRAY_SIZE ; i++ ) {
      du = &TMP_DT->data.array.body[i] ;
      du->data = st = JD_alloc_data_entry();
      st->type = JD_D_thread ;
      st->sub_type = JD_SD_single ;
      st->data.thrd = NULL ;
      st->base = TMP_DT ;
    }
    RESETALLOCATE(TMP_DT) ;
  }
  /**END MACRO ALLOC_DT_ARRAY_THREAD**/  
}


/*************************************************/
/* multi-dimention array data                    */
/*************************************************/
void JD_alloc_marray_dim() {
  /**MACRO ALLOC_MARRAY_DIM**/
  {
    TMP_DT->sub_type = JD_SD_marray ;
    TMP_DT->data.array.dim_size = ALLOC_DIM_SIZE(ARRAY_SIZE+1) ;
    TMP_DT->data.array.dim_size[0] = 0 ;
  }
  /**END MACRO ALLOC_MARRAY_DIM**/
}

void JD_set_marray_dim() {
  /**MACRO SET_MARRAY_DIM**/
  {
    int n = TMP_DT->data.array.dim_size[0] + 1 ;
    TMP_DT->data.array.dim_size[n] = ARRAY_SIZE ;
    TMP_DT->data.array.dim_size[0] = n ;
  }
  /**END MACRO SET_MARRAY_DIM**/
}

/*************************************************/
/* assoc array data                              */
/*************************************************/
void JD_alloc_assoc_int() {
  /**MACRO ALLOC_DT_ASSOC_INT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_int ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_INT**/  
}

void JD_alloc_assoc_bit() {
  /**MACRO ALLOC_DT_ASSOC_BIT**/
  {
    TMP_DT = JD_alloc_data_entry();
    if( BIT_SIZE <= 32 ) 
      TMP_DT->type = JD_D_sbit ;
   else
      TMP_DT->type = JD_D_bit ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
    TMP_DT->base = (JD_data *)BIT_SIZE ; /* holds size */
  }
  /**END MACRO ALLOC_DT_ASSOC_BIT**/  
}

void JD_alloc_assoc_float() {
  /**MACRO ALLOC_DT_ASSOC_FLOAT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_float ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_FLOAT**/  
}

void JD_alloc_assoc_double() {
  /**MACRO ALLOC_DT_ASSOC_DOUBLE**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_double ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_DOUBLE**/  
}

void JD_alloc_assoc_string() {
  /**MACRO ALLOC_DT_ASSOC_STRING**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_string ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_STRING**/  
}

void JD_alloc_assoc_signal() {
  /**MACRO ALLOC_DT_ASSOC_SIGNAL**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_signal ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_SIGNAL**/  
}

/* ALLOC_OBJ should hold the pointer to the alloc function */
void JD_alloc_assoc_obj() {
  /**MACRO ALLOC_DT_ASSOC_OBJ**/
  {  
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_object ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
    TMP_DT->base = (JD_data *)ALLOC_OBJ ;  /* pointing to the object alloc func */
  }
  /**END MACRO ALLOC_DT_ASSOC_OBJ**/  
}

void JD_alloc_assoc_thread() {
  /**MACRO ALLOC_DT_ASSOC_THREAD**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_thread ;
    TMP_DT->sub_type = JD_SD_assoc ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_ASSOC_THREAD**/  
}


/*************************************************/
/* string indexed array data                     */
/*************************************************/
void JD_alloc_strided_int() {
  /**MACRO ALLOC_DT_STRIDED_INT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_int ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_INT**/  
}

void JD_alloc_strided_bit() {
  /**MACRO ALLOC_DT_STRIDED_BIT**/
  {
    TMP_DT = JD_alloc_data_entry();
    if( BIT_SIZE <= 32 ) 
      TMP_DT->type = JD_D_sbit ;
   else
      TMP_DT->type = JD_D_bit ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
    TMP_DT->base = (JD_data *)BIT_SIZE ; /* holds size */
  }
  /**END MACRO ALLOC_DT_STRIDED_BIT**/  
}

void JD_alloc_strided_float() {
  /**MACRO ALLOC_DT_STRIDED_FLOAT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_float ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_FLOAT**/  
}

void JD_alloc_strided_double() {
  /**MACRO ALLOC_DT_STRIDED_DOUBLE**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_double ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_DOUBLE**/  
}

void JD_alloc_strided_string() {
  /**MACRO ALLOC_DT_STRIDED_STRING**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_string ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_STRING**/  
}

void JD_alloc_strided_signal() {
  /**MACRO ALLOC_DT_STRIDED_SIGNAL**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_signal ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_SIGNAL**/  
}

/* ALLOC_OBJ should hold the pointer to the alloc function */
void JD_alloc_strided_obj() {
  /**MACRO ALLOC_DT_STRIDED_OBJ**/
  {  
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_object ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
    TMP_DT->base = (JD_data *)ALLOC_OBJ ;  /* pointing to the object alloc func */
  }
  /**END MACRO ALLOC_DT_STRIDED_OBJ**/  
}

void JD_alloc_strided_thread() {
  /**MACRO ALLOC_DT_STRIDED_THREAD**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_thread ;
    TMP_DT->sub_type = JD_SD_strided ;
    TMP_DT->data.hash.table = ALLOC_HASH_TBL(JD_DATA_HASH_SIZE) ;
  }
  /**END MACRO ALLOC_DT_STRIDED_THREAD**/  
}


/*************************************************/
/* list data                                     */
/*************************************************/
void JD_alloc_list_int() {
  /**MACRO ALLOC_DT_LIST_INT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_int ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_INT**/  
}

void JD_alloc_list_bit() {
  /**MACRO ALLOC_DT_LIST_BIT**/
  {
    TMP_DT = JD_alloc_data_entry();
    if( BIT_SIZE <= 32 )
      TMP_DT->type = JD_D_sbit ;
    else
      TMP_DT->type = JD_D_bit ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
    TMP_DT->base = (JD_data *)BIT_SIZE ; /* holds size */
  }
  /**END MACRO ALLOC_DT_LIST_BIT**/  
}

void JD_alloc_list_float() {
  /**MACRO ALLOC_DT_LIST_FLOAT**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_float ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_FLOAT**/  
}

void JD_alloc_list_double() {
  /**MACRO ALLOC_DT_LIST_DOUBLE**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_double ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_DOUBLE**/  
}

void JD_alloc_list_string() {
  /**MACRO ALLOC_DT_LIST_STRING**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_string ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_STRING**/  
}

void JD_alloc_list_signal() {
  /**MACRO ALLOC_DT_LIST_SIGNAL**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_signal ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_SIGNAL**/  
}

/* ALLOC_OBJ should hold the pointer to the alloc function */
void JD_alloc_list_obj() {
  /**MACRO ALLOC_DT_LIST_OBJ**/
  {  
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_object ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
    TMP_DT->base = (JD_data *)ALLOC_OBJ ;  
    /* pointing to the object alloc func */
  }
  /**END MACRO ALLOC_DT_LIST_OBJ**/  
}

void JD_alloc_list_thread() {
  /**MACRO ALLOC_DT_LIST_THREAD**/
  {
    TMP_DT = JD_alloc_data_entry();
    TMP_DT->type = JD_D_thread ;
    TMP_DT->sub_type = JD_SD_list ;
    TMP_DT->data.list.top = TMP_DT->data.list.bottom = NULL ;
    TMP_DT->data.list.sem = NULL ;
  }
  /**END MACRO ALLOC_DT_LIST_THREAD**/  
}


JD_check_global_unfree() {
  int i ;
  
  for( i = 0 ; i <  JD_current_toplevel->num_global_data ; i++ ) {
    if( JD_current_toplevel->global_data[i] != NULL )
      JD_check_unfree_data( JD_current_toplevel->global_data[i] ) ;
  }
}

void JD_run_garbage_collection(

)
{
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  if ( data_check_ok == 0 ) {
    fprintf( stderr, "Jeda license code error detected\n" ) ;
    exit(1) ;
  }
#  endif
#endif
#ifdef DATA_GC_CHECK
  check_free_link() ;
#endif

#ifdef NOTIFY_GC_STATUS
  num_thread = 0 ;
  num_queue = 0 ;
  num_single = 0 ;
  num_int = 0 ;
  num_bit = 0 ;
  num_sbit = 0 ;
  num_vbit = 0 ;
  num_signal = 0 ;
  num_string = 0 ;
  num_other = 0 ;
  num_object = 0 ;
  num_sysobject = 0 ;
  num_pointer = 0 ;
  num_array = 0 ;
  num_marray = 0 ;
  num_assoc = 0 ;
  num_strix = 0 ;
  num_list = 0 ;
  printf( "Before GC:\n" ) ;
  JD_report_gc_status() ;
#endif

#ifdef NOTIFY_GC_EXEC
  printf( "garbage_collection running..\n" ) ;
#endif
  if( THREAD )JD_gc_thread(THREAD,SP) ;
  MARK_CHECK ;
  if(JD_current_toplevel) {
    int i ;
    JD_gc_mark( JD_current_toplevel->top_queue ) ;
    MARK_CHECK ;
    for( i = 0 ; i <  JD_current_toplevel->num_global_data ; i++ ) {
      if( JD_current_toplevel->global_data[i] != NULL )
        gc_mark_data( JD_current_toplevel->global_data[i] ) ;
    }
    MARK_CHECK ;
  }
  if( TMP_THREAD )JD_gc_thread(TMP_THREAD,TMP_THREAD->sp) ;
  MARK_CHECK ;
  JD_gc_vstack() ;
  MARK_CHECK ;
  scan_generation0() ;
  clear_flag() ; 
  
#ifdef DATA_GC_GLOBAL_CHECK
  JD_check_global_unfree() ;
#endif
#ifdef DATA_GC_CHECK
  check_generation0() ;
#endif
#ifdef NOTIFY_GC_STATUS
  printf( "After GC:\n" ) ;
  JD_report_gc_status() ;
  printf( 
    "Que:%d Trd:%d Single:%d Object:%d Sysobj:%d Ptr:%d Ary:%d Mry:%d Asc:%d Str:%d\n",
    num_queue, num_thread, num_single, num_object, num_sysobject, num_pointer,
    num_array, num_marray, num_assoc, num_strix
  ) ;
  printf( 
    "int:%d bit:%d sbit:%d vbit:%d signal:%d string:%d other:%d\n",
    num_int, num_bit, num_sbit, num_vbit, num_signal, num_string, num_other
  ) ;
  printf( "JD_pop_stack_int_num:%d\n", JD_pop_stack_int_num ) ;
  printf( "JD_vstack_ovflw_int_num:%d\n", JD_vstack_ovflw_int_num ) ;
  printf(
     "JD_push_acc_vstack_ovflw_int_num:%d\n", JD_push_acc_vstack_ovflw_int_num 
  ) ;
  printf( "JD_flush_vstack_int_num:%d\n", JD_flush_vstack_int_num ) ;
  printf( "JD_pop_acc_dt_num:%d\n", JD_pop_acc_dt_num ) ;
  printf( "JD_pop_acc_non_single_num:%d\n", JD_pop_acc_non_single_num ) ;
  num_cache_hit = 0 ;
  num_cache_miss = 0 ;
  JD_pop_stack_int_num = 0 ;
  JD_vstack_ovflw_int_num = 0 ;
  JD_push_acc_vstack_ovflw_int_num = 0 ;
  JD_flush_vstack_int_num = 0 ;
  JD_pop_acc_dt_num = 0 ;
  JD_pop_acc_non_single_num = 0 ;
#endif

#ifdef NOTIFY_GC_EXEC
  printf( "garbage_collection done\n" ) ; 
#endif
}
