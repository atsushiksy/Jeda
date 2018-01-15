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

#ifdef JEDA_VPI_MODE

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

#include "veriuser.h"
#include "acc_user.h"

#ifdef ICARUS_VERILOG_MODE
#  include "ivl_vpi_user.h"
#else
#  include "vpi_user.h"
#endif

#include "code_exec_macro.h"

#include "pli.h"

#include "jeda_pli.h"

extern JD_toplevel *JD_current_toplevel ;

extern int JD_vcode_hash_value ;

extern JD_export_func JD_export_table[] ;

int JD_pli_current_cycle ;

int JEDA_task_call_vcl_func(p_cb_data rc) ;
JD_data *JD_get_ptr_element( JD_data *dt ) ;

void JD_v_create_top() ;  /* generate the top structure */
void JD_v_create_main() ; /* generate the main thread */

void JD_misctf_reason_sync() ;

extern pthread_mutex_t *JD_d_mutex ;

int JD_v_top_loop() ;         /* execute active threads */

/* JD_count_timeq: thread.c func */
void JD_count_timeq( JD_thread **queue ) ;

static int misctf_reason_sync( p_cb_data cbd ) ;
static int misctf_reason_reactivate( p_cb_data cbd ) ;

static void enter_delay_queue( JD_pli_request *req ) ;
static void add_obj_to_changed_list( JD_pli_object *obj ) ;
static int process_pli_request( JD_pli_request *req ) ;
static int process_req_sync( JD_pli_request *req ) ;
static int process_req_sync_port( JD_pli_request *req ) ;
static int process_req_sync_p_on_c( JD_pli_request *req ) ;

JD_static_value_node **JD_static_value_hash_table ;

static s_vpi_vlog_info Verilog_info ;

static int compare_long(
  unsigned int l,
  unsigned int u,
  unsigned int la,
  unsigned int ua
) { 
  if( u == ua ) {
    if( l == la ) return 0 ;
    if( l > la ) return 1 ;
    else return -1 ;
  }
  else {
    if( u > ua ) return 1 ;
    else return -1 ;
  }
}

static int add_long(
  int *l,
  int *u,
  int la,
  int ua
) 
{ 
  unsigned int ll, uu, lla, uua, t ;
  ll = *l ;
  uu = *u ;
  lla = la ;
  uua = ua ;
  uu = uu + uua ;
  t = ll + lla ;
  if( (t < ll) || (t < lla) ) {
    uu += 1 ;
  }
  *l = t ;
  *u = uu ;
}

/* JD_alloc_pset_port & JD_alloc_port functions are the same one as pli.c */
void JD_alloc_pset_port( 
  char *pset, char *name, int ub, int lb, int index, int dir, 
  int s_edge, int depth, int d_edge, int clk_index
) 
{
  JD_pli_object *obj ;
  int nw, i, j, mask ;
  JD_data *dt ;
  
  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_port ;  
  obj->is.port.dir = dir ;
  obj->is.port.index = index ;
  obj->is.port.s_edge = s_edge ;
  obj->is.port.depth = depth ;
  obj->is.port.d_edge = d_edge ;
  obj->is.port.name = (char *)strdup(name) ;
  if( pset ) obj->is.port.pset_name = (char *)strdup(pset) ;
  
  /* queue for thread waiting pli request */
  obj->is.port.queue = JD_alloc_queue(JD_current_toplevel) ;
  
  obj->is.port.ub = ub ;
  obj->is.port.lb = lb ;
  if( ub < lb ) {
    obj->is.port.iendian = 1 ;
    obj->is.port.size = lb - ub + 1 ;
  }
  else  obj->is.port.size = ub - lb + 1 ;

  nw = NWORD(obj->is.port.size) ;
  
  obj->is.port.nw = nw ;
  
  mask = BITMASK(obj->is.port.size) ;
    
  obj->is.port.mask = mask ;
    
  obj->is.port.last_val = (p_vecval)calloc( nw, sizeof(s_vecval) ) ;
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.last_val[i].avalbits = (i==nw-1)?mask:0xffffffff ;
    obj->is.port.last_val[i].bvalbits = (i==nw-1)?mask:0xffffffff ;
  }
  if( depth ) {
    p_vecval p ;
    obj->is.port.pipe = (p_vecval *)calloc( depth, sizeof(p_vecval) ) ;
    for( j = 0 ; j < depth ; j++ ) {
      p = obj->is.port.pipe[j] = (p_vecval)calloc( nw, sizeof(s_vecval) ) ;
      for( i = 0 ; i < nw ; i++ ) {
        p[i].avalbits = (i==nw-1)?mask:0xffffffff ;
        p[i].bvalbits = (i==nw-1)?mask:0xffffffff ;
      }
    }
  }
  obj->is.port.sync_val = (p_vecval)calloc( nw, sizeof(s_vecval) ) ;
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.sync_val[i].avalbits = (i==nw-1)?mask:0xffffffff ;
    obj->is.port.sync_val[i].bvalbits = (i==nw-1)?mask:0xffffffff ;
  }
  obj->is.port.lsync_val = (p_vecval)calloc( nw, sizeof(s_vecval) ) ;
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.lsync_val[i].avalbits = (i==nw-1)?mask:0xffffffff ;
    obj->is.port.lsync_val[i].bvalbits = (i==nw-1)?mask:0xffffffff ;
  }
  
  
  obj->is.port.weak_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.normal_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.strong_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.temp_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.a_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.b_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  obj->is.port.mask_vec = (unsigned *)calloc( nw, sizeof(unsigned) ) ;
  
  obj->is.port.clk_index = clk_index ;
  
  /* allocate JD_data and put into global variable array */
  GLOBAL_DATA[index] = dt = JD_alloc_data_entry() ;
  
  dt->type = JD_D_signal ;
  
  dt->data.signal.port = (char *)obj ;
  dt->data.signal.data0 = -1 ;
  dt->data.signal.data1 = -1 ;
  
  JD_move_data_to_gen1( dt ) ;
  
}

void JD_alloc_port( 
  char *name, int ub, int lb, int index, int dir, 
  int s_edge, int depth, int d_edge, int clk_index
) 
{
  JD_alloc_pset_port( 
    NULL, name, ub, lb, index, dir, s_edge, depth, d_edge, clk_index
  ) ;
}

static void update_current_cycle() {
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  value_p->format = vpiIntVal ;
#ifdef DEBUG_VPI_MODE
  PRINTF( "update_current_cycle() calling vpi_get_value\n" ) ;
#endif
  vpi_get_value( (void *)JD_current_toplevel->CycleCount_vhandle, value_p ) ;
  
#ifdef DEBUG_VPI_MODE
  PRINTF( "update_current_cycle() return from vpi_get_value\n" ) ;
#endif
  JD_pli_current_cycle = value_p->value.integer ;
  
}

void JEDA_pli_get_cycle( int *ret ) {
  *ret = JD_pli_current_cycle ;
}

static int verilog_time_high = 0 ;
static int verilog_time_lo = 0 ;

static void get_current_time() {
  s_vpi_time s_t ;
  p_vpi_time p_t = &s_t ;
  p_t->type = vpiSimTime ;
  vpi_get_time( NULL, p_t ) ;
  verilog_time_lo = p_t->low ;
  verilog_time_high = p_t->high ;
}

void JEDA_pli_get_time( int *high, int *lo ) {
  *lo = verilog_time_lo ;
  *high = verilog_time_high ;
}

int JEDA_ref_clk_vcl_func() ;
/* int JEDA_CLOCK_vcl_func() ; */
static int sync_port_routine() ;

void JD_add_vcl( JD_pli_object *obj, int (*func)() ) {
  JD_vcl_node *vcl ;
  s_cb_data cbd ;
 
#ifdef DEBUG_VPI_MODE
  PRINTF( "JD_add_vcl: func %x\n", func ) ;
#endif
 
  vcl = (JD_vcl_node *)calloc(1, sizeof(JD_vcl_node)) ;
  cbd.reason = cbValueChange ;
  cbd.cb_rtn = func ;
  cbd.value = NULL ;
  cbd.user_data = (char *)obj ;
  switch(obj->type) {
    case JD_obj_port:
      cbd.obj = (void *)obj->is.port.i_vhandle ;
      break ;
    case JD_obj_veri_task:
      cbd.obj = (void *)obj->is.task.done_handle ;
      break ;
  }
  vcl->data = (char *)vpi_register_cb(&cbd) ;
  vcl->next = (JD_vcl_node *)JD_current_toplevel->vcl_link ;
  JD_current_toplevel->vcl_link = (char *)vcl ;

  obj->vcl = vcl ;
}

static void delete_vcl( JD_pli_object *obj ) {
  JD_vcl_node *vcl, *tvcl ;
  
  vcl = obj->vcl ;
  obj->vcl = NULL ;
  vpi_remove_cb( (vpiHandle)vcl->data );
  if( (char *)vcl == JD_current_toplevel->vcl_link ) {
    JD_current_toplevel->vcl_link = (char *)vcl->next ;
  }
  else {
    tvcl = (JD_vcl_node *)JD_current_toplevel->vcl_link ;
    while( tvcl && tvcl->next != vcl ) tvcl = tvcl->next ;
    if( tvcl ) tvcl->next = vcl->next ;
    else {
      fprintf( stderr, "Missing vcl info in JEDA system!!\n" );
      exit(1) ;
    }
  }
  free(vcl->data) ;
  free(vcl) ;
}

/*********************************************************/
/* misc data allocation                                  */
/*********************************************************/
static p_vecval alloc_vecval( int nbit ) {
  p_vecval ret ;
  int nw, i ;
  unsigned int msk ;
  
  nw = NWORD( nbit ) ;
  msk = BITMASK( nbit ) ;
  ret = (p_vecval)calloc(nw, sizeof(s_vecval)) ;
  for( i = 0 ; i < nw ; i++ ) {
    ret[i].avalbits = (i==nw-1)?msk:0xffffffff ;
    ret[i].bvalbits = (i==nw-1)?msk:0xffffffff ;
  }
  return ret ;
}

static void copy_vecval( p_vecval d, p_vecval s, int nbit ) {
  int nw, i ;
  nw = NWORD( nbit ) ;
  for( i = 0 ; i < nw ; i++ ) {
    d[i].avalbits = s[i].avalbits ;
    d[i].bvalbits = s[i].bvalbits ;
  }
}

/* VPI callback tasks */
void JEDA_vpi_link_CLOCK(vpiHandle taskH) {
  JD_data *dt ;
  JD_pli_object *obj ;
  vpiHandle argI, argH ;

  JD_alloc_pset_port( 
    /* pset, name, ub, lb, index,      dir, 
       s_edge, depth, d_edge, clk_index         */
    NULL, "CLOCK",  0,  0,     0,   JEDA_INOUT, 
    JEDA_POSEDGE, 0, JEDA_POSEDGE, 0
  ) ;
  
  dt = GLOBAL_DATA[0] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  
  /* here we use the pli structure to hold the value */  
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( 1 ) ;
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( 1 ) ;

  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip first one (command id) */
  argH = vpi_scan(argI) ;
  obj->is.port.i_vhandle = (void *)argH ;
  argH = vpi_scan(argI) ;
  obj->is.port.o_vhandle = (void *)argH ;
  argH = vpi_scan(argI) ;
  JD_current_toplevel->CycleCount_vhandle = (void *)argH ;
  
  
  JD_add_vcl( obj, JEDA_ref_clk_vcl_func ) ;
  obj->ref_clock = 1 ;
    
  obj->next_on_clk = 
     (JD_pli_object *)JD_current_toplevel->ref_clock_pli_obj_head ;
  JD_current_toplevel->ref_clock_pli_obj_head = (int *)obj ;
  
}

void JEDA_vpi_link_port(vpiHandle taskH) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int index, dir ;
  char *name ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;

  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  
  name = (char *)strdup(value_p->value.str) ;
  argH = vpi_scan(argI) ;
  /* 3rd arg is dummy to match with portset */
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  index = value_p->value.integer ;
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  dir = value_p->value.integer ;
  
  dt = GLOBAL_DATA[index] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  
  if( strcmp(name, obj->is.port.name) ) {
    PRINTF( 
      "JEDA warning: port name mismatch on Jeda module \"%s\" : code \"%s\" \n",
      name, obj->is.port.name
    ) ;
  }
  else if ( obj->is.port.pset_name ) {
    PRINTF( 
      "JEDA warning: port name mismatch on Jeda module \"%s\" : code \"%s.%s\" \n",
      name, obj->is.port.pset_name, obj->is.port.name
    ) ;
  }
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
    
  argH = vpi_scan(argI) ;
  obj->is.port.i_vhandle = (void *)argH ;
  argH = vpi_scan(argI) ;
  obj->is.port.o_vhandle = (void *)argH ;
      
}


void JEDA_vpi_link_pset_port(vpiHandle taskH) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int index, dir ;
  char *name, *pset_name ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;

  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  pset_name = (char *)strdup(value_p->value.str) ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  name = (char *)strdup(value_p->value.str) ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  index = value_p->value.integer ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  dir = value_p->value.integer ;
    
  dt = GLOBAL_DATA[index] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  
  if( obj->is.port.pset_name == NULL ) {
    PRINTF( 
      "JEDA warning: port name mismatch on Jeda module \"%s.%s\" : code \"%s\" \n",
      pset_name, name, obj->is.port.name
    ) ;
  }
  else if ( strcmp( name, obj->is.port.name ) ||
            strcmp( pset_name,obj->is.port.pset_name ) ) {
    PRINTF( 
      "JEDA warning: port name mismatch on Jeda module \"%s.%s\" : code \"%s.%s\" \n",
      pset_name, name, obj->is.port.pset_name, obj->is.port.name
    ) ;
  }
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
    
  argH = vpi_scan(argI) ;
  obj->is.port.i_vhandle = (void *)argH ;
  argH = vpi_scan(argI) ;
  obj->is.port.o_vhandle = (void *)argH ;
      
}


static void add_pli_request( 
  JD_pli_object *obj, JD_pli_request *rq, JD_edge_type edge 
) {
  JD_pli_request *prq ;
  
  rq->edge = edge ;
  
  switch(edge) {
    case JEDA_NOEDGE:
      if( prq = obj->is.port.anyedge ) {
        while( prq->next ) prq = prq->next ;
        prq->next = rq ; 
      }
      else obj->is.port.anyedge = rq ;
      break ;
    case JEDA_POSEDGE:
      if( prq = obj->is.port.posedge ) {
        while( prq->next ) prq = prq->next ;
        prq->next = rq ; 
      }
      else obj->is.port.posedge = rq ;
      break ;
    case JEDA_NEGEDGE:
      if( prq = obj->is.port.negedge ) {
        while( prq->next ) prq = prq->next ;
        prq->next = rq ; 
      }
      else obj->is.port.negedge = rq ;
      break ;
    case JEDA_BOTHEDGE:
      if( prq = obj->is.port.bothedge ) {
        while( prq->next ) prq = prq->next ;
        prq->next = rq ; 
      }
      else obj->is.port.bothedge = rq ;
      break ;
  }
  
}

static void delete_pli_request( JD_pli_object *obj, JD_pli_request *rq ) {
  JD_pli_request *prq ;
  
  switch(rq->edge) {
    case JEDA_NOEDGE:
      if( prq = obj->is.port.anyedge ) {
        if( prq == rq ) obj->is.port.anyedge = rq->next ;
        else {
          while( prq->next && prq->next != rq ) prq = prq->next ;
          if( prq->next ) prq->next = rq->next ; 
        }
      }
      break ;
    case JEDA_POSEDGE:
      if( prq = obj->is.port.posedge ) {
        if( prq == rq ) obj->is.port.posedge = rq->next ;
        else {
          while( prq->next && prq->next != rq ) prq = prq->next ;
          if( prq->next ) prq->next = rq->next ; 
        }
      }
      break ;
    case JEDA_NEGEDGE:
      if( prq = obj->is.port.negedge ) {
        if( prq == rq ) obj->is.port.negedge = rq->next ;
        else {
          while( prq->next && prq->next != rq ) prq = prq->next ;
          if( prq->next ) prq->next = rq->next ; 
        }
      }
      break ;
    case JEDA_BOTHEDGE:
      if( prq = obj->is.port.bothedge ) {
        if( prq == rq ) obj->is.port.bothedge = rq->next ;
        else {
          while( prq->next && prq->next != rq ) prq = prq->next ;
          if( prq->next ) prq->next = rq->next ; 
        }
      }
      break ;
  }
}

/* JEDA_ref_clock_vcl: parse global data for port and
   set VCL on reference clock
 */
 
void JEDA_ref_clock_vcl() {
  int idx, clk_idx ;
  JD_data *dt, *cdt ;
  JD_pli_object *obj, *cobj ;
  JD_pli_request *rq ;
  
  for( idx = 0 ; idx < JD_current_toplevel->num_global_data ; idx++ ) {
    dt = GLOBAL_DATA[idx] ;
    
    if( dt && dt->type == JD_D_signal && dt->sub_type == JD_SD_single &&
        dt->data.signal.port 
      ) 
    {
      obj = (JD_pli_object *)dt->data.signal.port ;
      clk_idx = obj->is.port.clk_index ;
      cdt = GLOBAL_DATA[clk_idx] ;
      cobj = (JD_pli_object *)cdt->data.signal.port ;
      
      /* chain obj under ref_clock */
      /* link according to edge of ref clock */
      switch(obj->is.port.s_edge) {
        case JEDA_POSEDGE:
          obj->next_on_ref = cobj->is.port.pos_sample ;
          cobj->is.port.pos_sample = obj ;
          break ;
        case JEDA_NEGEDGE:
          obj->next_on_ref = cobj->is.port.neg_sample ;
          cobj->is.port.neg_sample = obj ;
          break ;
        case JEDA_BOTHEDGE:
          obj->next_on_ref = cobj->is.port.both_sample ;
          cobj->is.port.both_sample = obj ;
          break ;
      }
      if( clk_idx != idx ) { /* avoid own CLOCK */
        if( cobj->vcl == NULL ) {
          cobj->ref_clock = 1 ;
          JD_add_vcl( cobj, JEDA_ref_clk_vcl_func ) ;
          cobj->next_on_clk = 
            (JD_pli_object *)JD_current_toplevel->ref_clock_pli_obj_head ;
          JD_current_toplevel->ref_clock_pli_obj_head = (int *)cobj ;
        }
      }
    }
  }
}

/* top->clocked_pli_obj_head/tail link holds the ref_clock that is
   clocked at the time slot.
 */
static void add_obj_to_clocked_list( JD_pli_object *obj ) {
  JD_pli_object *pobj ;
  if( JD_current_toplevel->clocked_pli_obj_tail ) {
    pobj = (JD_pli_object *)JD_current_toplevel->clocked_pli_obj_tail ;
    pobj->next_clocked = obj ;
    JD_current_toplevel->clocked_pli_obj_tail = (int *)obj ;
  }
  else {
    JD_current_toplevel->clocked_pli_obj_head = 
    JD_current_toplevel->clocked_pli_obj_tail = (int *)obj ;
  }
  obj->clocked = 1 ;
}

/* this is used as tf_isynchronize() in PLI */
static void set_synchronize_cb()
{
  s_cb_data cbd ;
  s_vpi_time time_p ;
  
#ifdef DEBUG_VPI_MODE
  PRINTF( "set_synchronize_cb: set cbReadWriteSync\n" ) ;
#endif
  
  time_p.type = vpiSimTime ;
  time_p.high = 0 ;
  time_p.low = 0 ;
  /* cbd->reason = cbNextSimTime ; */
  cbd.reason = cbReadWriteSynch ;
  cbd.cb_rtn = misctf_reason_sync ;
  cbd.value = NULL ;
  cbd.time = &time_p ;
  cbd.user_data = (char *)&cbd ;
  vpi_register_cb(&cbd) ;
  /* At the call back, is simulation time already updated? 
   * If so, we need to keep the time value when this function
   * is called.
   */
  get_current_time() ;
}

/* VCL consumer routine for reference clock signal */
int JEDA_ref_clk_vcl_func(p_cb_data rc) {
  JD_pli_object *obj ;
  obj = (JD_pli_object *)rc->user_data ;
  
  /* chain this obj to the clocked linked list */
  if( !obj->clocked ) {
    add_obj_to_clocked_list( obj ) ;
  }
  if( !JD_current_toplevel->tf_isynchronized ) {
    /* only first one in the timeslot set synch call back */
    set_synchronize_cb() ;
    JD_current_toplevel->tf_isynchronized = 1 ;
  }
}


/* JEDA_vpi_link_global_var */
void JEDA_vpi_link_global_var(vpiHandle taskH) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int index ;
  vpiHandle argI, argH ;
  int size ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;

  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  
  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_variable ;  
    
  obj->is.variable.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.variable.name = (char *)strdup(value_p->value.str) ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  index = value_p->value.integer ;
  
  argH = vpi_scan(argI) ;
  
  size = vpi_get(vpiSize, argH) ; /* hope this one returns the vector size */
  
  obj->is.variable.value->expr_value_p = alloc_vecval( size ) ;
  
  obj->is.variable.vhandle = (void *)argH ;
  
  JD_current_toplevel->global_obj[index] = (int *)obj ;
  
}


/*   JD_update_global_var */
void JD_update_global_var( int index ) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int i, n ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  dt = GLOBAL_DATA[index] ;
  obj = (JD_pli_object *)JD_current_toplevel->global_obj[index] ;
  
  if( ! obj ) return ;
  
  switch( dt->type ) {
    case JD_D_int:
      obj->is.variable.value->expr_value_p[0].avalbits = dt->data.int_value ;
      obj->is.variable.value->expr_value_p[0].bvalbits = 0 ;
      break ;
    case JD_D_sbit:
      obj->is.variable.value->expr_value_p[0].avalbits = dt->data.sbit.a ;
      obj->is.variable.value->expr_value_p[0].bvalbits = dt->data.sbit.b ;
      break ;
    case JD_D_bit:
      n = LASTWORD(dt->data.bit.size) ;
      for( i = 0 ; i <= n ; i++ ) {
      obj->is.variable.value->expr_value_p[i].avalbits = dt->data.bit.a[i] ;
      obj->is.variable.value->expr_value_p[i].bvalbits = dt->data.bit.b[i] ;
      }
      break ;
  }
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.variable.value->expr_value_p ;
  
  vpi_put_value( (vpiHandle)obj->is.variable.vhandle, value_p, NULL, vpiNoDelay ) ;
  
}

/* JEDA_vpi_link_global_var_array */
/*  
  $JEDA_task( 
    JEDA_LINK_GLOBAL_VAR_ARRAY, "name", index, 
    var_<name>[arrayid], arrayid, size 
  ) ; 
*/ 
void JEDA_vpi_link_global_var_array(vpiHandle taskH) {
  
  /* Do nothing as most (if not all) of the wave viewer can't handle the
     array data
   */
  
}

/*   JD_update_global_var_array */
void JD_update_global_var_array( int index, int array_id ) {
  JD_data *dt ;
  JD_data_unit *ut ;
  JD_pli_object *obj, **array ;
  int i, n ;
  
  /* Do nothing as most (if not all) of the wave viewer can't handle the
     array data
   */

}

/* JEDA_vpi_link_veri_task */
/* 
  $JEDA_task( 
    JEDA_LINK_VERI_TASK, "<name>", index,
    veri_task_<name>_call, veri_task_<name>_done, num_args
    [ , arg_<name>_<arg_name> ]..
  ) ;
*/
void JEDA_vpi_link_veri_task(vpiHandle taskH) {
  JD_pli_object *obj ;
  JD_pli_object *arg, *parg ;
  int index ;
  int num_args ;
  int i ;
  int size ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_veri_task ;  
    
  obj->is.task.called = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) ) ;
  obj->is.task.called->expr_value_p = alloc_vecval( 1 ) ;
  
  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.task.name = (char *)strdup(value_p->value.str) ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  index = value_p->value.integer ;
  
  argH = vpi_scan(argI) ;
  
  obj->is.task.call_handle = (void *)argH ;

  argH = vpi_scan(argI) ;
  
  obj->is.task.done_handle = (void *)argH ;

  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.task.num_args = num_args = value_p->value.integer ;
  
  for( i = 0 ; i < num_args ; i++ ) {
    arg = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
    arg->type = JD_obj_task_arg ;
    arg->instance = obj->instance ;
    argH = vpi_scan(argI) ;
    arg->is.arg.val_handle = (void *)argH ;
    size = vpi_get(vpiSize, argH) ; /* hope this one returns the vector size */
    arg->is.arg.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) ) ;
    arg->is.arg.value->expr_value_p = alloc_vecval( size ) ;
    if( i ) parg->next = arg ;
    else obj->is.task.args = arg ;
    parg = arg ;
  }
  
  obj->is.task.queue = JD_alloc_queue(JD_current_toplevel) ;
  
  JD_current_toplevel->global_obj[index] = (int *)obj ;
  
}

/* set_veri_task_call()  
    copy args to Verilog world
    invoke Verilog task by set 1 to call signal
    set vcl on done signal
    sleep 
 */

static void set_veri_task_call( JD_pli_object *obj, JD_thread *thrd, int sp ) {
  JD_data *dt ;
  JD_pli_object *arg ; 
  int i, j, n, nw ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj->is.task.busy = 1 ;
    
  /* propergate arguments to verilog */
  arg = obj->is.task.args ;
  for( i = 0 ; i < obj->is.task.num_args ; i++ ) {
    dt = thrd->stack[sp - obj->is.task.num_args + i] ;
    if( IS_SMLINT(dt) ) {
      arg->is.arg.value->expr_value_p[0].avalbits = 
        SMLINT2INT(dt) ;
      arg->is.arg.value->expr_value_p[0].bvalbits = 0 ;
      for( j = 1 ; j < nw ; j++ ) {
        arg->is.arg.value->expr_value_p[j].avalbits = 0 ;
        arg->is.arg.value->expr_value_p[j].bvalbits = 0 ;
      }
    }
    else {
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      switch( dt->type ) {
        case JD_D_int:
          arg->is.arg.value->expr_value_p[0].avalbits = 
            dt->data.int_value ;
          arg->is.arg.value->expr_value_p[0].bvalbits = 0 ;
          for( j = 1 ; j < nw ; j++ ) {
            arg->is.arg.value->expr_value_p[j].avalbits = 0 ;
            arg->is.arg.value->expr_value_p[j].bvalbits = 0 ;
          }
          break ;
        case JD_D_sbit:
          arg->is.arg.value->expr_value_p[0].avalbits = dt->data.sbit.a ;
          arg->is.arg.value->expr_value_p[0].bvalbits = dt->data.sbit.b ;
          for( j = 1 ; j < nw ; j++ ) {
            arg->is.arg.value->expr_value_p[j].avalbits = 0 ;
            arg->is.arg.value->expr_value_p[j].bvalbits = 0 ;
          }
          break ;
        case JD_D_bit:
          n = LASTWORD(dt->data.bit.size) ;
          for( j = 0 ; j < nw ; j++ ) {
            if( j <= n ) {
              arg->is.arg.value->expr_value_p[j].avalbits = 
                dt->data.bit.a[j] ;
              arg->is.arg.value->expr_value_p[j].bvalbits = 
                dt->data.bit.b[j] ;
            }
            else {
              arg->is.arg.value->expr_value_p[j].avalbits = 0 ;
              arg->is.arg.value->expr_value_p[j].bvalbits = 0 ;
            }
          }
          break ;
        case JD_D_string:
          {
            char *cp ;
            int  wd, k ;
            cp = dt->data.string ;
            for( j = nw-1 ; j >= 0 ; j-- ) {
              arg->is.arg.value->expr_value_p[j].bvalbits = 0 ;
              wd = 0 ;
              for( k = 0 ; k < 4 ; k++ ) {
                if( cp && *cp != '\0' ) {
                  wd <<= 8 ;
                  wd |= *cp ;
                  cp++ ;
                }
              }
              arg->is.arg.value->expr_value_p[j].avalbits = wd ;
            }
          }
          break ;
      }
    }
    
    value_p->format = vpiVectorVal ;
    value_p->value.vector = (p_vpi_vecval)arg->is.arg.value->expr_value_p ;
  
    vpi_put_value( 
      (vpiHandle)arg->is.arg.val_handle, value_p, NULL, vpiNoDelay 
    ) ;
    arg = arg->next ;
  }
  /* set vcl on done */
  JD_add_vcl( obj, JEDA_task_call_vcl_func ) ;
    
  /* wiggle the call signal to let always loop active*/
  obj->is.task.called->expr_value_p[0].avalbits = 1 ;
  obj->is.task.called->expr_value_p[0].bvalbits = 0 ;
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.task.called->expr_value_p ;
  
  vpi_put_value( 
    (vpiHandle)obj->is.task.call_handle, value_p, NULL, vpiNoDelay
  ) ;
    
}

/*   JD_veri_task_call */
void JD_veri_task_call() {
  JD_pli_object *obj ;
  JD_data *dt ;
  int i ;
  
  obj = (JD_pli_object *)JD_current_toplevel->global_obj[TMP_INT] ;
  
  if( obj->is.task.busy == 0 ) {
    set_veri_task_call( obj, THREAD, SP ) ;
  }
  
  /* sleep in the queue */
  JD_enqueue( obj->is.task.queue, THREAD ) ;
  THREAD->queue = obj->is.task.queue ;
  THREAD->status = JD_thrd_sleep ;
  
  JD_thread_pause( THREAD ) ;
  
}

/*   JEDA_task_call_vcl_func 
   VCL consumer for verilog task call
*/
int JEDA_task_call_vcl_func( p_cb_data data ) {
  JD_pli_object *obj ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
    
  value_p->format = vpiVectorVal ;
  
  obj = (JD_pli_object *)data->user_data ;

  vpi_get_value( (void *)obj->is.task.done_handle, value_p ) ;
  
  if( value_p->value.vector[0].aval == 1 ) {
    /* rising edge */
    delete_vcl( obj ) ;
  
    /* link obj to changed list */
    add_obj_to_changed_list( obj ) ;
  
    /* synchronize if not done yet */
    if( !JD_current_toplevel->tf_isynchronized ) {
      /* only first one in the timeslot set synch call back */
      set_synchronize_cb() ;
      JD_current_toplevel->tf_isynchronized = 1 ;
    }
  }
}

static void task_call_action( JD_pli_object *obj ) {
  JD_thread *thrd ;
  int i, j, n ;
  JD_pli_object *arg ;
  JD_data *dt ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj->is.task.busy = 0 ;
  thrd = JD_dequeue_top( obj->is.task.queue ) ;
  
  value_p->format = vpiVectorVal ;
  
  /* copy back the var data */
  arg = obj->is.task.args ;
  for( i = 0 ; i < obj->is.task.num_args ; i++ ) {
    dt = thrd->stack[thrd->sp - obj->is.task.num_args + i] ;
    if( !IS_SMLINT(dt) && dt->type == JD_D_pointer ) {
      vpi_get_value( (void *)arg->is.arg.val_handle, value_p ) ;
      while( dt->type == JD_D_pointer ) {
        dt = JD_get_ptr_element(dt) ;
      }
      switch( dt->type ) {
        case JD_D_int:
          dt->data.int_value = value_p->value.vector[0].aval ;
          break ;
        case JD_D_sbit:
          dt->data.sbit.a = value_p->value.vector[0].aval ;
          dt->data.sbit.b = value_p->value.vector[0].bval ;
          break ;
        case JD_D_bit:
          n = LASTWORD(dt->data.bit.size) ;
          for( j = 0 ; j <= n ; j++ ) {
            dt->data.bit.a[j] = value_p->value.vector[j].aval ; 
            dt->data.bit.b[j] = value_p->value.vector[j].bval ;
          }
          break ;
      }
    }
    arg = arg->next ;
  }
  
  pthread_mutex_lock( JD_d_mutex ) ;
  thrd->status = JD_thrd_ready ;
  JD_enqueue( JD_current_toplevel->ready_queue, thrd ) ;
  pthread_mutex_unlock( JD_d_mutex ) ;
  
  /* check if another task is waiting in the queue */
  thrd = obj->is.task.queue->thrd_top ;
  if( thrd ) {
    set_veri_task_call( obj, thrd, thrd->sp ) ;
  }  
}

/* JEDA_vpi_link_jd_task */
/* 
  $JEDA_task( 
    JEDA_LINK_JD_TASK, "<name>", index,
    <name>_done, num_args
    [ , arg_<name>_<arg_name> ]..
  ) ;
*/
void JEDA_vpi_link_jd_task(vpiHandle taskH) {
  JD_pli_object *obj ;
  JD_pli_object *arg, *parg ;
  int num_args ;
  int i, j, n, nw, size ;
  JD_data *dt ;
  JD_thread *thrd ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_jd_task ;  
    
  obj->is.task.done = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) ) ;
  obj->is.task.done->expr_value_p = alloc_vecval( 1 ) ;
  
  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  /* obj->is.task.name = (char *)strdup(value_p->value.str) ; */
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.task.index = value_p->value.integer ;
  
  /* 4th arg is done */
  argH = vpi_scan(argI) ;
  obj->is.task.done_handle = (void *)argH ;

  /* 5th arg is numarg */
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.task.num_args = num_args = value_p->value.integer ;
  
  thrd = (void *)JD_create_thread( obj->is.task.num_args ) ;
  obj->is.task.thrd = (char *)thrd ;

  JD_enqueue( JD_current_toplevel->call_queue, thrd ) ;
  for( i = 0 ; i < num_args ; i++ ) {
    arg = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
    arg->type = JD_obj_task_arg ;
    arg->instance = obj->instance ;
    argH = vpi_scan(argI) ;
    arg->is.arg.val_handle = (void *)argH ;
    size = vpi_get(vpiSize, argH) ; /* hope this one returns the vector size */
    arg->is.arg.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) ) ;
    arg->is.arg.value->expr_value_p = alloc_vecval( size ) ;
    if( i ) parg->next = arg ;
    else obj->is.task.args = arg ;
    parg = arg ;
    
    value_p->format = vpiVectorVal ;
    vpi_get_value( (void *)arg->is.arg.val_handle, value_p ) ;
    nw = NWORD(size) ;
    dt = JD_alloc_data_entry() ;
    if( size <= 32 ) {
      dt->type = JD_D_sbit ;
      dt->data.sbit.size = size ;
      dt->data.sbit.a = value_p->value.vector[0].aval ;
      dt->data.sbit.b = value_p->value.vector[0].bval ;
    }
    else {
      dt->type = JD_D_bit ;
      dt->data.bit.size = size ;
      dt->data.bit.a = ALLOC_BIT( nw ) ;
      dt->data.bit.b = ALLOC_BIT( nw ) ;
      n = LASTWORD(size) ;
      for( j = 0 ; j <= n ; j++ ) {
        dt->data.bit.a[j] = value_p->value.vector[j].aval ; 
        dt->data.bit.b[j] = value_p->value.vector[j].bval ;
      }
    }
    thrd->stack[thrd->sp++] = dt ;
  }

  JD_create_thread_end() ; /* clean up & alloc virtual acc value */
  
  /* link obj to changed list */
  add_obj_to_changed_list( obj ) ;
  
  /* synchronize if not done yet */
  if( !JD_current_toplevel->tf_isynchronized ) {
    /* only first one in the timeslot set synch call back */
    set_synchronize_cb() ;
    JD_current_toplevel->tf_isynchronized = 1 ;
  }
}

  
static void do_jeda_func_call( JD_pli_object *aobj ) {
  int i, j, n, nw, size ;
  JD_data *dt ;
  JD_thread *thrd ;
  JD_pli_object *arg, *parg ;
  JD_pli_object *obj ;
  void (*func)() ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj = aobj ;   /* will lose stack frame so copy */
  
#ifdef JEDA_DEBUGGER
  JD_debugger_start_function() ;
#endif
  
  func = JD_export_table[obj->is.task.index].entry_func ;
  (func)( NULL ) ; /* call it */
  
  /* returned from the call. copy back the arg to verilog and
     trigger verilog done.
   */
  thrd = (JD_thread *)obj->is.task.thrd ;
  arg = obj->is.task.args ;
  for( i = 0 ; i < obj->is.task.num_args ; i++ ) {
    dt = thrd->stack[i] ;
    
    nw = arg->is.arg.value->expr_ngroups ;
    size = arg->is.arg.value->expr_vec_size ;
    if( dt->type == JD_D_sbit ) {
      arg->is.arg.value->expr_value_p[0].avalbits = dt->data.sbit.a ;
      arg->is.arg.value->expr_value_p[0].bvalbits = dt->data.sbit.b ;
    }
    else {
      n = LASTWORD(dt->data.bit.size) ;
      for( j = 0 ; j <= n ; j++ ) {
        arg->is.arg.value->expr_value_p[j].avalbits = dt->data.bit.a[j] ; 
        arg->is.arg.value->expr_value_p[j].bvalbits = dt->data.bit.b[j] ;
      }
    }
    value_p->format = vpiVectorVal ;
    value_p->value.vector = (p_vpi_vecval)arg->is.arg.value->expr_value_p ;
  
    vpi_put_value( 
      (vpiHandle)arg->is.arg.val_handle, value_p, NULL, vpiNoDelay 
    ) ;
    parg = arg ;
    arg = arg->next ;
    
    free(parg->is.arg.value->expr_value_p) ;
    free(parg->is.arg.value) ;
    free( parg ) ;
    
  }
  
  /* trigger Verilog */
  obj->is.task.done->expr_value_p[0].avalbits = 1 ;
  obj->is.task.done->expr_value_p[0].bvalbits = 0 ;
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.task.done->expr_value_p ;
  
  vpi_put_value( 
    (vpiHandle)obj->is.task.done_handle, value_p, NULL, vpiNoDelay
  ) ;
 
  /* clean up the object & return */
  free( obj ) ;
  
  JD_thread_exit( thrd ) ;
  
}

static void jeda_func_call_action( JD_pli_object *obj ) {
  JD_thread *thrd ;
  int i, j, n ;
  JD_pli_object *arg ;
  JD_data *dt ;
  
  thrd = (JD_thread *)obj->is.task.thrd ;
  
  JD_dequeue( JD_current_toplevel->call_queue, thrd ) ;
  
 #ifdef JEDA_SELF_THREAD_CONTROL
    thrd->lwp = 
      (void *)JD_create_lwp( (void *)do_jeda_func_call, (JD_thread *)obj ) ;
 #endif
    
    thrd->status = JD_thrd_ready ;
    JD_enqueue( JD_current_toplevel->ready_queue, thrd ) ;
    
 #ifndef JEDA_SELF_THREAD_CONTROL
   /* set thread as pthread to execute */
    thrd->main_flag = 1 ;
    pthread_create( 
      &thrd->pth, NULL, (void *)do_jeda_func_call, (void *)obj 
    ) ;
    pthread_detach( thrd->pth ) ;
    
    pthread_mutex_lock( &thrd->l_mutex ) ;  /* check if thrd is ready */
    while( thrd->main_flag ) {
      pthread_cond_wait( &thrd->l_cond, &thrd->l_mutex ) ; /* wait for ready */
    }
#endif
  
}

/* JEDA_vpi_link_static_var */
/* 
  $JEDA_task( 
    JEDA_LINK_STATIC_VAR, "class", "func", "var", index, 
    var_<class>_<func>_<var> 
  ) ;
*/
void JEDA_vpi_link_static_var(vpiHandle taskH) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int index ;
  JD_static_value_node *snode ;
  int hs ;
  vpiHandle argI, argH ;
  int size ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip 1st arg */
  
  if( JD_static_value_hash_table == NULL ) {
    JD_static_value_hash_table = 
      (JD_static_value_node **)
      calloc(NUM_STATIC_HASH, sizeof(JD_static_value_node *) ) ;
  }

  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_static_variable ;  
    
  obj->is.variable.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.variable.class = (char *)strdup(value_p->value.str) ;
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.variable.func = (char *)strdup(value_p->value.str) ;
  argH = vpi_scan(argI) ;
  value_p->format = vpiStringVal ;
  vpi_get_value( argH, value_p ) ;
  obj->is.variable.name = (char *)strdup(value_p->value.str) ;
  
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  index = value_p->value.integer ;
  
  argH = vpi_scan(argI) ;
  
  obj->is.variable.vhandle = (void *)argH ;
  size = vpi_get(vpiSize, argH) ; /* hope this one returns the vector size */
  obj->is.variable.value->expr_value_p = alloc_vecval( size ) ;
  
  JD_current_toplevel->global_obj[index] = (int *)obj ;
  dt = GLOBAL_DATA[index] ;
  SETSTATICDATA(dt) ;
  snode = (JD_static_value_node *)calloc(1,sizeof(JD_static_value_node) ) ;
  snode->dt = dt ;
  snode->index = index ;
  hs = STATIC_HASH(dt) ;
  if( JD_static_value_hash_table[hs] == NULL ) { 
    JD_static_value_hash_table[hs] = snode ;
  }
  else {
    snode->next = JD_static_value_hash_table[hs] ;
    JD_static_value_hash_table[hs] = snode ;
  }  
}

/* JEDA_vpi_link_static_var_array */
/* 
  $JEDA_task( 
    JEDA_LINK_STATIC_VAR_ARRAY, "class", "func", "var", index, 
    var_<class>_<func>_<var>, arrayid, arraysize
  ) ;
*/
void JEDA_vpi_link_static_var_array(vpiHandle taskH) {
  
  /* Do nothing as most (if not all) of the wave viewer can't handle the
     array data
   */
  
}

/*   JD_update_static_var */
void JD_update_static_var( JD_data *dt ) {
  int hs ;
  JD_pli_object *obj, **array ;
  int i, n ;
  JD_static_value_node *snode ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  hs = STATIC_HASH(dt) ;
  if( JD_static_value_hash_table[hs] == NULL ) { 
    return ;
  }
  else {
    snode = JD_static_value_hash_table[hs] ;
    while( snode ) {
      if( snode->dt == dt ) break ;
      snode = snode->next ;
    }
    if( !snode ) return ;
  }
  
  if( snode->array_flag ) {
    array = (JD_pli_object **)JD_current_toplevel->global_obj[snode->index] ;
    if( !array ) return ;
    obj = array[snode->array_id] ;
  }
  else obj = (JD_pli_object *)JD_current_toplevel->global_obj[snode->index] ;
  
  if( !obj ) return ;
  
  switch( dt->type ) {
    case JD_D_int:
      obj->is.variable.value->expr_value_p[0].avalbits = dt->data.int_value ;
      obj->is.variable.value->expr_value_p[0].bvalbits = 0 ;
      break ;
    case JD_D_sbit:
      obj->is.variable.value->expr_value_p[0].avalbits = dt->data.sbit.a ;
      obj->is.variable.value->expr_value_p[0].bvalbits = dt->data.sbit.b ;
      break ;
    case JD_D_bit:
      n = LASTWORD(dt->data.bit.size) ;
      for( i = 0 ; i <= n ; i++ ) {
        obj->is.variable.value->expr_value_p[i].avalbits = dt->data.bit.a[i] ;
        obj->is.variable.value->expr_value_p[i].bvalbits = dt->data.bit.b[i] ;
      }
      break ;
  }
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.variable.value->expr_value_p ;
  
  vpi_put_value( 
    (vpiHandle)obj->is.variable.vhandle, value_p, NULL, vpiNoDelay 
  ) ;

}

/******************************************************************/
/*
 driver routines
   Privide functionalities to interact with Verilog.
*/

#ifdef DEBUG_VPI_MODE  
struct __vpiSignal {
      char *base;
      char* scope;
	/* The name of this reg/net object */
      const char*name;
	/* The indices that define the width and access offset. */
      int msb, lsb;
	/* Flags */
      unsigned signed_flag  ;
      unsigned isint_ ;	// origial type was integer
	/* The represented value is here. */
      char *bits;
        /* This is the callback event functor */
      char *callback;
};
#endif

/********************************************************************
  JD_drive_port()
    Drive a port to a given value with given strength.
    Detect drive signal conflict and call error handling routine.
********************************************************************/
void JD_drive_port( 
  JD_pli_object *obj, unsigned *a_vec, unsigned *b_vec, int size,
  int partial, int ub, int lb,
  int strength  /* 0:weak, 1:normal, 2:strong */
)
{ 
  int i, j, k, n, m, aub, alb ;
  unsigned umsk, lmsk, msk, va, vb, 
    dmask, data_a, data_b, tmp_mask, drv_vec, forcex ;
  int err_flag = 0 ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  /* check partial flag, 
     if 1, then construct a valid vector and mask from ub, lb  
  */
#ifndef JEDA_NO_PLI_ACCESS_MODE
  n = LASTWORD(size) ;
  if( partial ) {
    if( obj->is.port.iendian ) {
      if( ub > lb ) {
        ERROR_WRONG_ENDIAN_PARTIAL_VECTOR_FOR_PORT(
          ub, lb, obj->is.port.pset_name, obj->is.port.name
        ) ;
        return ;
      }
      if( ub < obj->is.port.ub || lb > obj->is.port.lb ) {
        ERROR_OUT_OF_RANGE_PARTIAL_VECTOR_FOR_PORT(
          ub, lb, obj->is.port.pset_name, obj->is.port.name
        ) ;
        return ;
      }
      /* adjust bit location */
      aub = obj->is.port.lb - ub ;
      alb = obj->is.port.lb - lb ;
      STORE_SIZE = lb - ub + 1 ;
      for( i = 0, j = 0 ; i < obj->is.port.nw ; i++ ) {
        lmsk = umsk = 0xffffffff ;
        if( i > aub/32 ) lmsk = 0 ;
        else if( (i == aub/32) && (31-(aub%32)) ) lmsk >>= (31-(aub%32)) ;
        if( i < alb/32 ) umsk = 0 ;
        else if( (i == alb/32) && (alb%32) ) umsk <<= (alb%32) ;
        msk = lmsk & umsk ;
        if( i == obj->is.port.nw-1 ) msk &= obj->is.port.mask ;
        obj->is.port.mask_vec[i] = msk ;
        if( i >= alb/32 && j <= n+1 ) {
          if( j <= n ) {
            va = a_vec[j] ; vb = b_vec[j] ;
          }
          else {
            va = vb = 0 ;
          }
          if( alb%32 ) {
            va <<= (alb%32) ; vb <<= (alb%32) ;
            if( j ) {
              va |= ( a_vec[j-1] >> (32 - (alb%32) ) ) ; 
              vb |= ( b_vec[j-1] >> (32 - (alb%32) ) ) ; 
            }
          }
          obj->is.port.a_vec[i] = va & msk ;
          obj->is.port.b_vec[i] = vb & msk ;
          j++ ;
        }
        else {
          obj->is.port.a_vec[i] = 0 ;
          obj->is.port.b_vec[i] = 0 ;
        }
      }
    }
    else {
      if( lb > ub ) {
        ERROR_WRONG_ENDIAN_PARTIAL_VECTOR_FOR_PORT(
          ub, lb, obj->is.port.pset_name, obj->is.port.name
        ) ;
        return ;
      }
      if( ub > obj->is.port.ub || lb < obj->is.port.lb ) {
        ERROR_OUT_OF_RANGE_PARTIAL_VECTOR_FOR_PORT(
          ub, lb, obj->is.port.pset_name, obj->is.port.name
        ) ;
        return ;
      }
      /* adjust bit location */
      ub -= obj->is.port.lb ;
      lb -= obj->is.port.lb ;
      STORE_SIZE = ub - lb + 1 ;
      for( i = 0, j = 0 ; i < obj->is.port.nw ; i++ ) {
        lmsk = umsk = 0xffffffff ;
        if( i < lb/32 ) lmsk = 0 ;
        else if( (i == lb/32) && (lb%32) ) lmsk <<= (lb%32) ;
        if( i > ub/32 ) umsk = 0 ;
        else if( (i == ub/32) && (31-(ub%32)) ) umsk >>= (31-(ub%32)) ;
        msk = lmsk & umsk ;
        if( i == obj->is.port.nw-1 ) msk &= obj->is.port.mask ;
        obj->is.port.mask_vec[i] = msk ;
        if( i >= lb/32 && j <= n+1 ) {
          if( j <= n ) {
            va = a_vec[j] ; vb = b_vec[j] ;
          }
          else {
            va = vb = 0 ;
          }
          if( lb%32 ) {
            va <<= (lb%32) ; vb <<= (lb%32) ;
            if( j ) {
              va |= ( a_vec[j-1] >> (32 - (lb%32) ) ) ; 
              vb |= ( b_vec[j-1] >> (32 - (lb%32) ) ) ; 
            }
          }
          obj->is.port.a_vec[i] = va & msk ;
          obj->is.port.b_vec[i] = vb & msk ;
          j++ ;
        }
        else {
          obj->is.port.a_vec[i] = 0 ;
          obj->is.port.b_vec[i] = 0 ;
        }
      }
    }
  } 
  else {
    STORE_SIZE = obj->is.port.size ;
    for( i = 0 ; i < obj->is.port.nw ; i++ ) {
      obj->is.port.mask_vec[i] = msk = 
        (i==obj->is.port.nw-1) ? obj->is.port.mask : 0xffffffff ;
      if( i <= n ) {
        obj->is.port.a_vec[i] = a_vec[i] & msk ;
        obj->is.port.b_vec[i] = b_vec[i] & msk ;
      }
      else {
        obj->is.port.a_vec[i] = 0 ;
        obj->is.port.b_vec[i] = 0 ;
      }
    }
  }
   
  /*
   -Calculate the new drive vector, detect the port drive conflict if
    the port is driven 
  */     
  /* conflict checking */
  for( i = 0 ; i < obj->is.port.nw ; i++ ) {
    va = obj->is.port.a_vec[i] ;
    vb = obj->is.port.b_vec[i] ;
    msk = obj->is.port.mask_vec[i] ;
    data_a = obj->is.port.o_exprinfo->expr_value_p[i].avalbits ;
    data_b = obj->is.port.o_exprinfo->expr_value_p[i].bvalbits ;
    dmask = msk & ~(vb & ~va) ; /* mask z */
    switch( strength ) {
      case 0: /* weak */
        drv_vec = obj->is.port.weak_vec[i] ;
        dmask &= ~obj->is.port.normal_vec[i] ; /* mask bits normally driven */
        dmask &= ~obj->is.port.strong_vec[i] ; /* mask bits strongly driven */
        break ;
      case 1: /* normal */
        drv_vec = obj->is.port.normal_vec[i] ;
        dmask &= ~obj->is.port.strong_vec[i] ; /* mask bits strongly driven */
        break ;
      case 2: /* strong */
        drv_vec = obj->is.port.strong_vec[i] ; 
        break ;
    }
    tmp_mask = drv_vec & ~(data_b & ~data_a) ; /* mask z */
    if( ((data_a & tmp_mask & dmask) != (va & tmp_mask & dmask)) ||
        ((data_b & tmp_mask & dmask) != (vb & tmp_mask & dmask))    )
    {
      /* signal conflict detected */
      if( !err_flag ) {
        err_flag = 1 ;
        switch( strength ) {
          case 0: /* weak */
            ERROR_WEAK_DRIVE_CONFLICT(
              obj->is.port.pset_name, obj->is.port.name,
              obj->weak_line_num, obj->weak_file_index
            ) ;
            break ;
          case 1: /* normal */
            ERROR_NORMAL_DRIVE_CONFLICT(
              obj->is.port.pset_name, obj->is.port.name,
              obj->line_num, obj->file_index
            ) ;
            break ;
          case 2: /* strong */
            ERROR_STRONG_DRIVE_CONFLICT(
              obj->is.port.pset_name, obj->is.port.name,
              obj->strong_line_num, obj->strong_file_index
            ) ;
            break ;
        }
      }
      /* force the drive vector to X */
      forcex = ((data_a & tmp_mask & dmask) & (va & dmask)) |
            ((data_b & tmp_mask & dmask) & (vb & dmask))   ;
      va |= forcex ; vb |= forcex ; /* let it X */
    }
    /* tmp_mask = non-Z driven data & Z-drive, 
     * this case we keep the previous value 
     */
    tmp_mask = tmp_mask & ~( msk & (vb & ~va) ) ;
    /* drive data calculation */
    dmask = msk ;
    switch( strength ) {
      case 0: /* weak */
        dmask &= ~obj->is.port.normal_vec[i] ; /* mask bits normally driven */
        dmask &= ~obj->is.port.strong_vec[i] ; /* mask bits strongly driven */
        obj->is.port.weak_vec[i] |= dmask ;
        break ;
      case 1: /* normal */
        dmask &= ~obj->is.port.strong_vec[i] ; /* mask bits strongly driven */
        obj->is.port.normal_vec[i] |= dmask ;
        break ;
      case 2: /* strong */
        obj->is.port.strong_vec[i] |= dmask ;
        break ;
    }
    dmask &= ~tmp_mask ; /* mask for non-Z weakly driven already */
    data_a = (va & dmask) | (data_a & ~dmask) ;
    data_b = (vb & dmask) | (data_b & ~dmask) ;
    obj->is.port.o_exprinfo->expr_value_p[i].avalbits = data_a ;
    obj->is.port.o_exprinfo->expr_value_p[i].bvalbits = data_b ;
  }
         
  
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.port.o_exprinfo->expr_value_p ;
  
#ifdef DEBUG_VPI_MODE  
  PRINTF( 
    "JD_drive_port: calling vpi_put_value() port %s vec size=%d\n",
    obj->is.port.name, obj->is.port.size
  ) ;

  {
    unsigned wid;
    struct __vpiSignal*rfp;
    rfp = (struct __vpiSignal*)obj->is.port.o_vhandle;
    wid = (rfp->msb >= rfp->lsb)
        ? (rfp->msb - rfp->lsb + 1)
	: (rfp->lsb - rfp->msb + 1);
    PRINTF( 
      "ref: base %x scope %x name %s msb %d lsb %d (wid:%d sizeof ulong = %d) signed_flag %d isint_ %d\n",
      rfp->base, rfp->scope, rfp->name, rfp->msb, rfp->lsb, wid,
      sizeof(unsigned long),
      rfp->signed_flag, rfp->isint_
    ) ;
  }


#endif

  vpi_put_value( 
    (vpiHandle)obj->is.port.o_vhandle, value_p, NULL, vpiNoDelay
  ) ;
  
  switch( strength ) {
    case 0: /* weak */
      GET_CURRENT_LOCATION( &obj->weak_line_num, &obj->weak_file_index ) ;
      break ;
    case 1: /* normal */
      GET_CURRENT_LOCATION( &obj->line_num, &obj->file_index ) ;
      break ;
    case 2: /* strong */
      GET_CURRENT_LOCATION( &obj->strong_line_num, &obj->strong_file_index ) ;
      break ;
  }
  /* Queue the obj to driven_port link as to clear the drive flag at the end */
  if( obj->is.port.driven == 0 ) {
    /* set driven flag */
    obj->is.port.driven = 1 ;
    /* chain the object to driven_port link */
    obj->next_driven = 
      (JD_pli_object *)JD_current_toplevel->driven_pli_obj_head ;
    JD_current_toplevel->driven_pli_obj_head = (int *)obj ;
  }
#else
  /* JEDA_NO_PLI_ACCESS_MODE */
#endif
  
}

/* clear the driven vector from the port information at the end of time slot */
static void clear_driven_port( JD_pli_object *obj ) 
{
  int i ;
  
  for( i = 0 ; i < obj->is.port.nw ; i++ ) {
    obj->is.port.weak_vec[i] = 0 ;
    obj->is.port.normal_vec[i] = 0 ;
    obj->is.port.strong_vec[i] = 0 ;
  }
  obj->is.port.driven = 0 ;
  
}

/* 
  Sample the value on a port and return it to ACC.
*/
void JD_sample_port( 
  JD_pli_object *obj, int depth, int subbit, int ub,  int lb
)
{
  int i, n ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  if( depth > obj->is.port.depth ) {
    ERROR_DEPTH_TOO_DEEP( depth, obj->is.port.depth ) ;
    return ;
  }
  
#ifndef JEDA_NO_PLI_ACCESS_MODE

  if( !subbit ) {
    /* copy Verilog data vector in experinfo to ACC  */
    ACC_TYPE = (obj->is.port.size>32)?JD_D_bit:JD_D_sbit ;
    n = obj->is.port.nw ;
    if( ACC_WD < n ) {
      ACC_A = 
        (unsigned int *)realloc( ACC_A, 4 * n ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, 4 * n ) ;
      ACC_WD = n * 4 ;
    }
  
    if( !depth ) {
      value_p->format = vpiVectorVal ;
      /* get data */
      vpi_get_value( (void *)obj->is.port.i_vhandle, value_p ) ;
       
      for( i = 0 ; i < n ; i++ ) {
        ACC_A[i] = value_p->value.vector[i].aval ;
        ACC_B[i] = value_p->value.vector[i].bval ;
      }
    }
    else {
      p_vecval p ;
      p = obj->is.port.pipe[depth-1] ;
      for( i = 0 ; i < n ; i++ ) {
        ACC_A[i] = p[i].avalbits ;
        ACC_B[i] = p[i].bvalbits ;
      }
    }
    ACC_SIZE = obj->is.port.size ;
  }
  else { /* subbit case */
    int aub, alb ;
    int i, j, n, an, ls, us, ns, nwn ;
    unsigned int lmsk, umsk, uumsk, a, b, aa, bb ;
    p_vecval p ;

    if( !depth ) {
      /* Update infomation on obj->is.port.exprinfo with tf_ievaluatep() */
      value_p->format = vpiVectorVal ;
      /* get data */
      vpi_get_value( (void *)obj->is.port.i_vhandle, value_p ) ;
      n = obj->is.port.nw ;
      for( i = 0 ; i < n ; i++ ) {
        obj->is.port.i_exprinfo->expr_value_p[i].avalbits =
           value_p->value.vector[i].aval ;
        obj->is.port.i_exprinfo->expr_value_p[i].bvalbits =
           value_p->value.vector[i].bval ;
      }
      p = obj->is.port.i_exprinfo->expr_value_p ;
    }
    else {
      p = obj->is.port.pipe[depth-1] ;
    }
    if( obj->is.port.iendian ) {
      alb = obj->is.port.lb - lb ;
      aub = obj->is.port.lb - ub ;
      ACC_SIZE = lb-ub+1 ;
    }
    else {
      aub = ub - obj->is.port.lb ;
      alb = lb - obj->is.port.lb ;
      ACC_SIZE = ub-lb+1 ;
    }
    ACC_TYPE = (ACC_SIZE>32)?JD_D_bit:JD_D_sbit ;
    n = NWORD(ACC_SIZE) ;
    if( ACC_WD < n ) {
      ACC_A = 
        (unsigned int *)realloc( ACC_A, 4 * n ) ;
      ACC_B = 
        (unsigned int *)realloc( ACC_B, 4 * n ) ;
      ACC_WD = n * 4 ;
    }
    ACC_SIZE = ns = aub - alb +1 ;
    i = LASTWORD(alb+1) ;  /* add one to match LASTWORD(size) */
    an = LASTWORD(obj->is.port.size) ;
    ls = (alb%32) ;
    us = 32-ls ;
    uumsk = (1 << us )-1 ;
    if( uumsk ) uumsk = ~uumsk ;
    umsk = (ns%32)?(1 << (ns%32))-1:0xffffffff ;
    nwn = LASTWORD(ns) ;
    j = 0 ;
    for( ; j < nwn ; i++, j++ ) {
      a = (i<=an)?p[i].avalbits:0 ;
      b = (i<=an)?p[i].bvalbits:0 ;
      aa = (i<an)?p[i+1].avalbits:0 ;
      bb = (i<an)?p[i+1].bvalbits:0 ;
      ACC_A[j] = 
        ( (a>>ls) | ((aa<<us)&uumsk) ) ;
      ACC_B[j] = 
        ( (b>>ls) | ((bb<<us)&uumsk) ) ;
    }
    a = (i<=an)?p[i].avalbits:0 ;
    b = (i<=an)?p[i].bvalbits:0 ;
    aa = (i<an)?p[i+1].avalbits:0 ;
    bb = (i<an)?p[i+1].bvalbits:0 ;
    ACC_A[j] = ( (a>>ls) | ((aa<<us)&uumsk) ) & umsk ;
    ACC_B[j] = ( (b>>ls) | ((bb<<us)&uumsk) ) & umsk ;
  }
  
#else
  /* JEDA_NO_PLI_ACCESS_MODE */
#endif

}    

/* Sync on the given number of given edge of a clock port. */
void JD_sync_on_edge( 
  JD_pli_object *obj, JD_edge_type edge, int num_cycle
) {
  JD_pli_request *req ;
   
  /*  If num_cycle is zero, and current time slot is the required edge
      of the given clock, then return immediately
  */
  switch( edge ) {
    case JEDA_POSEDGE:
    case JEDA_NEGEDGE:
      if( obj->is.port.edge == edge ) 
      {
        if( num_cycle == 0 ) return ;
      }
      break ;
    case JEDA_BOTHEDGE:
      if( obj->is.port.edge == JEDA_POSEDGE || 
          obj->is.port.edge == JEDA_NEGEDGE    )
      {
        if( num_cycle == 0 ) return ;
      }
      break ;
  }
   
  /* Allocate a new JD_pli_requset req with type JD_req_sync */
  req = ALLOC_PLI_RQ ;
  req->type = JD_req_sync ;
  req->thread = THREAD ;
  req->is.sync.port = obj ;
  req->is.sync.edge = edge ;
  if( num_cycle ) num_cycle-- ;
  req->is.sync.num_cycle = num_cycle ;
   
  GET_CURRENT_LOCATION( &req->line_num, &req->file_index ) ;
   
  /* add the sync request to the obj */
  add_pli_request( obj, req, edge ) ;
   
  /* Put the caller thread to sleep */
  JD_enqueue( obj->is.port.queue, THREAD ) ;
  THREAD->queue = obj->is.port.queue ;
  THREAD->squeue = obj->is.port.queue ;
  THREAD->status = JD_thrd_sleep ;
   
  JD_thread_pause( THREAD ) ;
  
}

/*
  Sync on the given number of given edge of a port. The sample is 
  done on asynchronously (at any change on the port immediately).
*/
void JD_sync_on_port(
  JD_pli_object *obj, JD_edge_type edge, int num_cycle
) {
  JD_pli_request *req ;
      
  /*  If num_cycle is zero, and current time slot is the required edge
      of the given clock, then return immediately
  */
#ifdef JEDA_PLI_DEBUG
  PRINTF( 
    "JD_sync_on_port: THREAD %x edge %d current %d num %d @ %d\n",
    THREAD, edge, obj->is.port.edge, num_cycle, JD_pli_current_cycle
  ) ;
#endif
  switch( edge ) {
    case JEDA_POSEDGE:
    case JEDA_NEGEDGE:
      if( obj->is.port.edge == edge ) 
      {
        if( num_cycle == 0 ) return ;
      }
      break ;
    case JEDA_BOTHEDGE:
      if( obj->is.port.edge == JEDA_POSEDGE || 
          obj->is.port.edge == JEDA_NEGEDGE    )
      {
        if( num_cycle == 0 ) return ;
      }
      break ;
  }
  /* alloc JD_pli_request req with JD_req_sync_port */
  req = ALLOC_PLI_RQ ;
  req->type = JD_req_sync_port ;
  req->thread = THREAD ;
  req->is.sync.port = obj ;
  req->is.sync.edge = edge ;
  if( num_cycle ) num_cycle-- ;
  req->is.sync.num_cycle = num_cycle ;
  
  /* chain the request to the proper link in obj structure */
  add_pli_request( obj, req, edge ) ;

  /*
   if obj is not a ref-clock:
     add VCL monitor on the given port obj
  */
  if( obj->ref_clock ) {
    req->type = JD_req_sync ; // request to sync on clock signal
  }
  else {
    if( obj->sync_count == 0 ) 
      JD_add_vcl( obj, sync_port_routine ) ;
    req->type = JD_req_sync_port ;
  }
  obj->sync_count++ ;
  
  GET_CURRENT_LOCATION( &req->line_num, &req->file_index ) ;
   
            
  /* Put the caller thread to sleep */
  JD_enqueue( obj->is.port.queue, THREAD ) ;
  THREAD->queue = obj->is.port.queue ;
  THREAD->status = JD_thrd_sleep ;
   
  JD_thread_pause( THREAD ) ;
  
}

/* top->changed_pli_obj_head/tail link holds the non-ref clock port obj 
   that changed it's value
 */
static void add_obj_to_changed_list( JD_pli_object *obj ) {
  JD_pli_object *pobj ;
  if( JD_current_toplevel->changed_pli_obj_tail ) {
    pobj = (JD_pli_object *)JD_current_toplevel->changed_pli_obj_tail ;
    pobj->next_clocked = obj ;
    JD_current_toplevel->changed_pli_obj_tail = (int *)obj ;
  }
  else {
    JD_current_toplevel->changed_pli_obj_head = 
    JD_current_toplevel->changed_pli_obj_tail = (int *)obj ;
  }
  obj->clocked = 1 ;
}

/* VCL consumer routine for vcl on non-ref-clock signal */
static int sync_port_routine(p_cb_data rc) {
  JD_pli_object *obj ;
  obj = (JD_pli_object *)rc->user_data ;
  
  /* chain this obj to the clocked linked list */
  if( !obj->clocked ) {
    add_obj_to_changed_list( obj ) ;
  }
  if( !JD_current_toplevel->tf_isynchronized ) {
    /* only first one in the timeslot set synch call back */
    set_synchronize_cb() ;
    JD_current_toplevel->tf_isynchronized = 1 ;
  }
}


/*
  Sync on the given number of given edge of a port sampled at the
  proper edge of given ref-clock.
*/
void JD_sync_port_on_clk(
  JD_pli_object *obj, JD_edge_type edge, int num_cycle 
)
{
  JD_pli_request *req ;
  JD_pli_object *cobj ;
  JD_data *cdt ;
  /*  If num_cycle is zero, and current time slot is the required edge
      of the given clock, then return immediately
  */
  if( num_cycle == 0 && obj->is.port.smpl_edge == edge ) return ;

  /* alloc JD_pli_request req with JD_req_sync_port */
  req = ALLOC_PLI_RQ ;
  req->type = JD_req_sync_p_on_c ;
  req->thread = THREAD ;
  req->is.sync.port = obj ;
  req->is.sync.edge = edge ;
  if( num_cycle ) num_cycle-- ;
  req->is.sync.num_cycle = num_cycle ;
  
  /* chain the request to the proper link in ref-clock obj structure
     so that this request will be processed at the edge of the ref clock.
  */
  cdt = GLOBAL_DATA[obj->is.port.clk_index] ;
  cobj = (JD_pli_object *)cdt->data.signal.port ;
  add_pli_request( cobj, req, edge ) ;
  
  /* Put the caller thread to sleep */
  JD_enqueue( obj->is.port.queue, THREAD ) ;
  THREAD->queue = obj->is.port.queue ;
  THREAD->status = JD_thrd_sleep ;
   
  JD_thread_pause( THREAD ) ;

}

/*
  Set drive clock request on a port
*/
void JD_drive_clock(
  JD_pli_object *obj, int period1, int period2,
  unsigned *a_vec, unsigned *b_vec, int size, int startdly
)
{
  JD_pli_request *req ;
  int i, j, k, n, m ;
  unsigned umsk, lmsk, msk, va, vb, 
    dmask, data_a, data_b, tmp_mask, drv_vec, forcex ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;

  /* error if clock_driver is already set */
  if( obj->is.port.drive_clock ) {
    ERROR_DUPLICATE_DRIVE_CLOCK_CALL_ON_PORT( 
      obj->is.port.pset_name, obj->is.port.name
    ) ;
    return ;
  }
  
  /* alloc JD_pli_request req with JD_req_drive_clock */
  req = ALLOC_PLI_RQ ;
  req->type = JD_req_drive_clock ;
  req->is.drive_clock.port = obj ;
  req->is.drive_clock.period1 = period1 ;
  req->is.drive_clock.period2 = period2 ;
  req->is.drive_clock.startdly = startdly ;
  

  obj->is.port.drive_clock = req ;

  /* status of driving sequence
   * 0: startdly period
   * 1: period1
   * 2: period2
   * 3: stopped
   */   
  if( startdly ) {
    req->is.drive_clock.status = 0 ;
    req->unit_delay = startdly ;
  }
  else {
    req->is.drive_clock.status = 1 ;
    req->unit_delay = period1 ;
  }

  n = LASTWORD(size) ;
  for( i = 0 ; i < obj->is.port.nw ; i++ ) {
    obj->is.port.mask_vec[i] = msk = 
      (i==obj->is.port.nw-1) ? obj->is.port.mask : 0xffffffff ;
    
    if( req->is.drive_clock.status == 0 ) {
      if( i <= n ) {
        obj->is.port.a_vec[i] = a_vec[i] & msk ;
        obj->is.port.b_vec[i] = b_vec[i] & msk ;
      }
      else {
        obj->is.port.a_vec[i] = 0 ;
        obj->is.port.b_vec[i] = 0 ;
      }
    }
    else {
      if( i <= n ) {
        obj->is.port.a_vec[i] = (~a_vec[i]) & msk ;
        obj->is.port.b_vec[i] = (~b_vec[i]) & msk ;
      }
      else {
        obj->is.port.a_vec[i] = msk ;
        obj->is.port.b_vec[i] = 0 ;
      }
    }
  }

  for( i = 0 ; i < obj->is.port.nw ; i++ ) {
    va = obj->is.port.a_vec[i] ;
    vb = obj->is.port.b_vec[i] ;
    obj->is.port.o_exprinfo->expr_value_p[i].avalbits = va ;
    obj->is.port.o_exprinfo->expr_value_p[i].bvalbits = vb ;
  }
         
  value_p->format = vpiVectorVal ;
  value_p->value.vector = (p_vpi_vecval)obj->is.port.o_exprinfo->expr_value_p ;
  vpi_put_value( 
    (vpiHandle)obj->is.port.o_vhandle, value_p, NULL, vpiNoDelay 
  ) ;
  
  /* Delay request to Verilog, and put the request to queue */
  enter_delay_queue( req ) ;
  
}

/*
  Set drive clock request on a port
*/
void JD_stop_clock( JD_pli_object *obj )
{
  JD_pli_request *req ;
  
  req = obj->is.port.drive_clock ;
  
  if( req ) {
    req->is.drive_clock.status = 3 ;
  }
  else {
    ERROR_STOP_CLOCK_CALLED_ON_UNDRIVEN_PORT(
      obj->is.port.pset_name, obj->is.port.name
    ) ;
  }
  
}

/* 
  Sequence for clock drive
*/
static void drive_clock_seq( JD_pli_request *req )
{
  int i, j, k, n, m ;
  unsigned msk, va, vb ;
  JD_pli_object *obj ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  obj = req->is.drive_clock.port  ;
  
  if( req->is.drive_clock.status == 3 ) {
    /* stopped */
    obj->is.port.drive_clock = NULL ;
    DEALLOC_PLI_RQ( req ) ;
  }
  else {
  
    /* toggle the signal */
    for( i = 0 ; i < obj->is.port.nw ; i++ ) {
      msk = (i==obj->is.port.nw-1) ? obj->is.port.mask : 0xffffffff ;
    
      va = obj->is.port.o_exprinfo->expr_value_p[i].avalbits ;
      obj->is.port.o_exprinfo->expr_value_p[i].avalbits = (~va) & msk ;
      obj->is.port.o_exprinfo->expr_value_p[i].bvalbits = 0 ;
    }
  
    value_p->format = vpiVectorVal ;
    value_p->value.vector = 
      (p_vpi_vecval)obj->is.port.o_exprinfo->expr_value_p ;
    vpi_put_value( 
      (vpiHandle)obj->is.port.o_vhandle, value_p, NULL, vpiNoDelay 
    ) ;

    switch( req->is.drive_clock.status ) {
      case 0: /* startdly period */
        req->is.drive_clock.status = 1 ;
        req->unit_delay = req->is.drive_clock.period1 ;
        break ;
      case 1: /* period1 */
        req->is.drive_clock.status = 2 ;
        req->unit_delay = req->is.drive_clock.period2 ;
        break ;
      case 2: /* period2 */
        req->is.drive_clock.status = 1 ;
        req->unit_delay = req->is.drive_clock.period1 ;
        break ;
    }

    /* Delay request to Verilog, and put the request to queue */
    enter_delay_queue( req ) ;
  
  } /* end of else of req->is.drive_clock.status == 3 */
  
}

/* Delay request to Verilog, and put the request to queue in top_level */
static void enter_delay_queue( JD_pli_request *req ) 
{
  s_vpi_time time_s ;
  JD_pli_request *preq ;
  int found = 0 ;
  s_cb_data cbd ;
  
  /* time_p = (p_vpi_time)calloc(1, sizeof(s_vpi_time)) ; */
  time_s.type = vpiSimTime ;
  time_s.high = 0 ;
  time_s.low = req->unit_delay ;
  
  /* cbd = (p_cb_data)calloc(1,sizeof(s_cb_data)) ; */
  cbd.reason = cbAfterDelay ;
  cbd.cb_rtn =  misctf_reason_reactivate ;
  cbd.time = &time_s ;
  cbd.user_data = (char *)req ;
   
  vpi_register_cb( &cbd ) ;
  
  /* get current simulation time in long format */
  /*
  req->dly_lo = tf_getlongtime( &req->dly_hi ) ;
  add_long( &req->dly_lo, &req->dly_hi, req->unit_delay, 0  ) ;
  */
}


/*
  Set drive back request on a port
*/
void JD_drive_back(
  JD_pli_object *obj, 
  unsigned *a_vec, unsigned *b_vec, int size
)
{
  JD_pli_request *req ;
  int i, j, k, n, m ;
  unsigned umsk, lmsk, msk, va, vb, 
    dmask, data_a, data_b, tmp_mask, drv_vec, forcex ;

  if( obj->is.port.drive_back ) {
    ERROR_DUPLICATED_DRIVE_BACK_REQUEST(
      obj->is.port.pset_name, obj->is.port.name
    ) ;
    return ;
  }
  
  obj->is.port.drive_back = req = ALLOC_PLI_RQ ;
  
  req->type = JD_req_drive_back ;
   
  req->is.drive_back.port = obj ;
  
  /* keep the drive back value */
  req->is.drive_back.a_vec = 
    (unsigned int *)calloc( obj->is.port.nw, sizeof(unsigned int) ) ;
  req->is.drive_back.b_vec = 
    (unsigned int *)calloc( obj->is.port.nw, sizeof(unsigned int) ) ;
  
  n = LASTWORD(size) ;
  for( i = 0 ; i < obj->is.port.nw ; i++ ) {
    msk = 
      (i==obj->is.port.nw-1) ? obj->is.port.mask : 0xffffffff ;
    
    if( i <= n ) {
      req->is.drive_back.a_vec[i] = a_vec[i] & msk ;
      req->is.drive_back.b_vec[i] = b_vec[i] & msk ;
    }
    else {
      req->is.drive_back.a_vec[i] = 0 ;
      req->is.drive_back.b_vec[i] = 0 ;
    }
  }

}

void JD_unit_delay( int delay )
{
  JD_pli_request *req ;
      
  /* alloc JD_pli_request req with JD_req_sync_port */
  req = ALLOC_PLI_RQ ;
  req->type = JD_req_unit_delay ;
  req->thread = THREAD ;
  req->unit_delay = delay ;
  
  enter_delay_queue( req ) ;
  
  /* put this thread to sleep */
  JD_enqueue( JD_current_toplevel->sleep_queue, THREAD ) ;
  THREAD->queue = JD_current_toplevel->sleep_queue ;
  THREAD->status = JD_thrd_sleep ;
  
  JD_thread_pause( THREAD ) ;
  
}

static void unit_delay_end( JD_pli_request *req ) 
{
  JD_thread *thrd ;
  
  thrd = req->thread ;
  
#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_mutex_lock( JD_d_mutex ) ;
#endif
  JD_dequeue( JD_current_toplevel->sleep_queue, thrd ) ;
  JD_enqueue( JD_current_toplevel->ready_queue, thrd ) ;
  thrd->status = JD_thrd_ready ;
#ifndef JEDA_SELF_THREAD_CONTROL
  pthread_mutex_unlock( JD_d_mutex ) ;
#endif

  if( !JD_current_toplevel->tf_isynchronized ) {
    /* only first one in the timeslot set synch call back */
    set_synchronize_cb() ;
    JD_current_toplevel->tf_isynchronized = 1 ;
  }
  
}

/******************************************************************
 *  misctf support functions
 *
 ******************************************************************/
/* 
 * vpi call back with delay is set by vpi_register_cb() to call back
 * JD_misctf_reason_reactivate() on unit_delay() request.
 *
 */ 
static int misctf_reason_reactivate(p_cb_data cbd) {
  JD_pli_request *req ;
  
  /*
  free( cbd->time ) ;
  free( cbd ) ;
  */
  
  req = (JD_pli_request *)cbd->user_data ;
  
  switch(req->type) {
    case JD_req_unit_delay:
      /* wake up the thread and put it to ready queue */
      unit_delay_end( req ) ;
      break ;
      
    case JD_req_drive_clock:
      drive_clock_seq( req ) ;
      break ;
      
    default:
     /* <report error> */
     fprintf( stderr, "UNKNOWN pli req %d for reactivate\n", req->type ) ;
     exit(1) ;
     break ;
  }
  
}


/* functions support JD_misctf_reason_sync() */
static void gen_ref_clock_edge( JD_pli_object *obj ) {
  int tedge, pedge, nw, i ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  value_p->format = vpiVectorVal ;
  
#ifdef DEBUG_VPI_MODE
    PRINTF( 
      "  gen_ref_clock_edge calling vpi_get_value %x\n", 
      obj->is.port.i_vhandle 
    ) ;
#endif
  
  /* get data */
  vpi_get_value( (void *)obj->is.port.i_vhandle, value_p ) ;
  
#ifdef DEBUG_VPI_MODE
    PRINTF( 
      "  gen_ref_clock_edge return from vpi_get_value %x\n", 
      obj->is.port.i_vhandle
    ) ;
#endif
  
  nw = NWORD(obj->is.port.size) ;
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.i_exprinfo->expr_value_p[i].avalbits =
       value_p->value.vector[i].aval ;
    obj->is.port.i_exprinfo->expr_value_p[i].bvalbits =
       value_p->value.vector[i].bval ;
  }
  
  /* generate clock edge information on clock obj */
  tedge = obj->is.port.i_exprinfo->expr_value_p[0].avalbits & 1 ;
  tedge |= ( obj->is.port.i_exprinfo->expr_value_p[0].bvalbits<<1 ) & 2 ;
  pedge = obj->is.port.last_val[0].avalbits & 1 ;
  pedge |= ( obj->is.port.last_val[0].bvalbits << 1 ) & 2 ;
  
  /* update the last_val for next edge */
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.last_val[i].avalbits = 
      obj->is.port.i_exprinfo->expr_value_p[i].avalbits ;
    obj->is.port.last_val[i].bvalbits = 
      obj->is.port.i_exprinfo->expr_value_p[i].bvalbits ;
  }
    
  switch(pedge) {
    case 0: /* 0 */
      switch(tedge) {
        case 0:
        case 2:
        case 3:
          /* 0 -> 0, z, x : means no edge */
          obj->is.port.edge = JEDA_NOEDGE ;
          break ;
        case 1:
          obj->is.port.edge = JEDA_POSEDGE ;
          break ;
      }
      break ;
    case 1: /* 1 */
      switch(tedge) {
        case 0:
          obj->is.port.edge = JEDA_NEGEDGE ;
          break ;
        case 1:
        case 2:
        case 3:
          /* 1 -> 1, z, x : means no edge */
          obj->is.port.edge = JEDA_NOEDGE ;
          break ;
      }
      break ;
    case 2: /* Z */
    case 3: /* X */
      switch(tedge) {
        case 0:
          obj->is.port.edge = JEDA_NEGEDGE ;
          break ;
        case 1:
          obj->is.port.edge = JEDA_POSEDGE ;
          break ;
        case 2:
        case 3:
          /* x, z -> z, x : means no edge */
          obj->is.port.edge = JEDA_NOEDGE ;
          break ;
      }
      break ;
  }
  
}

static void sample_port_data( JD_pli_object *obj ) {
  int i, j, nw ;
  int tedge, pedge ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  value_p->format = vpiVectorVal ;
  /* get data */
  vpi_get_value( (void *)obj->is.port.i_vhandle, value_p ) ;
  
  nw = NWORD(obj->is.port.size) ;
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.lsync_val[i].avalbits = obj->is.port.sync_val[i].avalbits ;
    obj->is.port.lsync_val[i].bvalbits = obj->is.port.sync_val[i].bvalbits ;
  }
  for( i = 0 ; i < nw ; i++ ) {
    obj->is.port.sync_val[i].avalbits = 
      value_p->value.vector[i].aval ;
    obj->is.port.sync_val[i].bvalbits = 
      value_p->value.vector[i].bval ;
  }
  if( obj->is.port.depth ) {
    p_vecval p, pp ;
    for( j = obj->is.port.depth-1 ; j > 0 ; j-- ) {
      p = obj->is.port.pipe[j] ;
      pp = obj->is.port.pipe[j-1] ;
      for( i = 0 ; i < nw ; i++ ) {
        p[i].avalbits = pp[i].avalbits ;
        p[i].bvalbits = pp[i].bvalbits ;
      }
    }
    p = obj->is.port.pipe[0] ;
    for( i = 0 ; i < nw ; i++ ) {
      p[i].avalbits = obj->is.port.sync_val[i].avalbits ;
      p[i].bvalbits = obj->is.port.sync_val[i].bvalbits ;
    }
  }
  
  /* generate smpl_edge information (edge info as sampled on ref edge) */
  tedge = obj->is.port.sync_val[0].avalbits & 1 ;
  tedge |= ( obj->is.port.sync_val[0].bvalbits<<1 ) & 2 ;
  pedge = obj->is.port.lsync_val[0].avalbits & 1 ;
  pedge |= ( obj->is.port.lsync_val[0].bvalbits << 1 ) & 2 ;
    
  switch(pedge) {
    case 0: /* 0 */
      switch(tedge) {
        case 0:
        case 2:
        case 3:
          /* 0 -> 0, z, x : means no edge */
          obj->is.port.smpl_edge = JEDA_NOEDGE ;
          break ;
        case 1:
          obj->is.port.smpl_edge = JEDA_POSEDGE ;
          break ;
      }
      break ;
    case 1: /* 1 */
      switch(tedge) {
        case 0:
          obj->is.port.smpl_edge = JEDA_NEGEDGE ;
          break ;
        case 1:
        case 2:
        case 3:
          /* 0 -> 0, z, x : means no edge */
          obj->is.port.smpl_edge = JEDA_NOEDGE ;
          break ;
      }
      break ;
    case 2: /* Z */
      obj->is.port.smpl_edge = JEDA_POSEDGE ;
      break ;
    case 3: /* X */
      obj->is.port.smpl_edge = JEDA_POSEDGE ;
      break ;
  }

}

static void sample_port_on_ref_edge( JD_pli_object *obj ) {
  JD_pli_object *tobj ;
    
  /* update sync_val, lsync_val on port linked on proper queue 
     (pos_sample,neg_sample,both_sample) as current edge      */
  switch( obj->is.port.edge ) {
    case JEDA_POSEDGE:
      tobj = obj->is.port.pos_sample ;
      while( tobj ) {
        sample_port_data( tobj ) ;
        tobj = tobj->next_on_ref ;
      }
      tobj = obj->is.port.both_sample ;
      while( tobj ) {
        sample_port_data( tobj ) ;
        tobj = tobj->next_on_ref ;
      }
      break ;
    case JEDA_NEGEDGE:
      tobj = obj->is.port.neg_sample ;
      while( tobj ) {
        sample_port_data( tobj ) ;
        tobj = tobj->next_on_ref ;
      }
      tobj = obj->is.port.both_sample ;
      while( tobj ) {
        sample_port_data( tobj ) ;
        tobj = tobj->next_on_ref ;
      }
      break ;
  }

}    

static void clear_port_on_ref_edge( JD_pli_object *obj ) {
  JD_pli_object *tobj ;
  switch( obj->is.port.edge ) {
    case JEDA_POSEDGE:
      tobj = obj->is.port.pos_sample ;
      while( tobj ) {
        tobj->is.port.smpl_edge = JEDA_NOEDGE ;
        tobj = tobj->next_on_ref ;
      }
      tobj = obj->is.port.both_sample ;
      while( tobj ) {
        tobj->is.port.smpl_edge = JEDA_NOEDGE ;
        tobj = tobj->next_on_ref ;
      }
      break ;
    case JEDA_NEGEDGE:
      tobj = obj->is.port.neg_sample ;
      while( tobj ) {
        tobj->is.port.smpl_edge = JEDA_NOEDGE ;
        tobj = tobj->next_on_ref ;
      }
      tobj = obj->is.port.both_sample ;
      while( tobj ) {
        tobj->is.port.smpl_edge = JEDA_NOEDGE ;
        tobj = tobj->next_on_ref ;
      }
      break ;
  }
  obj->is.port.edge = JEDA_NOEDGE ;
  obj->clocked = 0 ;
}



static void process_pli_req_on_edge( JD_pli_object *obj ) {
  JD_pli_request *req, **reqp ;
    
  switch( obj->is.port.edge ) {
    case JEDA_POSEDGE:
      req = obj->is.port.posedge ;
      reqp = &obj->is.port.posedge ;
      while( req ) {
        if( process_pli_request( req ) ) {
          *reqp = req->next ;
          DEALLOC_PLI_RQ( req );
          req = *reqp ;
        }
        else {
          reqp = &req->next ;
          req = req->next ;
        }
      }
      req = obj->is.port.anyedge ;
      reqp = &obj->is.port.anyedge ;
      while( req ) {
        if( process_pli_request( req ) ) {
          *reqp = req->next ;
          DEALLOC_PLI_RQ( req );
          req = *reqp ;
        }
        else {
          reqp = &req->next ;
          req = req->next ;
        }
      }
      break ;
    case JEDA_NEGEDGE:
      req = obj->is.port.negedge ;
      reqp = &obj->is.port.negedge ;
      while( req ) {
        if( process_pli_request( req ) ) {
          *reqp = req->next ;
          DEALLOC_PLI_RQ( req );
          req = *reqp ;
        }
        else {
          reqp = &req->next ;
          req = req->next ;
        }
      }
      req = obj->is.port.anyedge ;
      reqp = &obj->is.port.anyedge ;
      while( req ) {
        if( process_pli_request( req ) ) {
          *reqp = req->next ;
          DEALLOC_PLI_RQ( req );
          req = *reqp ;
        }
        else {
          reqp = &req->next ;
          req = req->next ;
        }
      }
      break ;
  }
}     

/*********************************************
 * reason_sync misctf function 
 *  This is the major simulation loop. 
 *********************************************/    
static int is_CLOCK_posedge() {
  JD_data *dt ;
  JD_pli_object *obj ;
  
  dt = GLOBAL_DATA[0] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  
  if( obj->is.port.edge == JEDA_POSEDGE ) return 1 ;
  else return 0 ;
 
}


void JD_misctf_reason_sync() {

}

static int misctf_reason_sync( p_cb_data cbd ) {
  /* this supporse to be the end of simulation time slot */
  JD_pli_object *obj, *pobj, *nobj ;
  JD_thread *thrd, *zmbi ;
  int lcount ; /* loop count */
  
#ifdef DEBUG_VPI_MODE
  PRINTF( "Entering misctf_reason_sync\n" ) ;
#endif

  /*
  free( cbd->time ) ;
  free( cbd ) ;
  */
  
  update_current_cycle() ;
  /*
   * process the ref clock objects with vcl change detected
   *  - generate async edge info
   *  - sample port data whose ref-clock is this and if edge matched
   *  - process pli_request if edge matched
   */
  /* top->clocked_pli_obj_head/tail link holds the ref_clock that is
     clocked at the time slot.
  */
  obj = (JD_pli_object *)JD_current_toplevel->clocked_pli_obj_head ;    
  
  while( obj ) {
    
#ifdef DEBUG_VPI_MODE
    PRINTF( "  mosctf_reasen_sync loop obj %x a\n", obj ) ;
#endif
    /* get the value on this clock obj and generate edge information */
    gen_ref_clock_edge( obj ) ;
    
#ifdef DEBUG_VPI_MODE
    PRINTF( "  mosctf_reasen_sync loop obj %x b\n", obj ) ;
#endif
    /* sample values on port whose ref-clock is this object */
    sample_port_on_ref_edge( obj ) ;
    
#ifdef DEBUG_VPI_MODE
    PRINTF( "  mosctf_reasen_sync loop obj %x c\n", obj ) ;
#endif
    /* process pli request linked to the clock/edge */
    process_pli_req_on_edge( obj ) ;
    
    obj = obj->next_clocked ;
  }
  
  /* top->changed_pli_obj_head/tail link holds the non-ref clock port obj 
     that changed it's value
  */
  /* Do the similar edge detecton on all obj linked in chenged_pli_obj */
  obj = (JD_pli_object *)JD_current_toplevel->changed_pli_obj_head ;    
  pobj = NULL ;

#ifdef DEBUG_VPI_MODE
  PRINTF( "  misctf_reason_sync a\n" ) ;
#endif
  
  while( obj ) {
         
    nobj = obj->next_clocked ;
    
    /* process pli request linked to the clock/edge */
    switch(obj->type) {
      case JD_obj_port:
        /* get the value on this clock obj and generate edge information */
        gen_ref_clock_edge( obj ) ;
        process_pli_req_on_edge( obj ) ;
        pobj = obj ;
        break ;
      case JD_obj_veri_task:
        task_call_action( obj ) ;
        pobj = obj ;
        break ;
      case JD_obj_jd_task:
        /* action to call jeda task from verilog */
        jeda_func_call_action( obj ) ;
        if( pobj ) pobj->next_clocked = nobj ;
        else JD_current_toplevel->changed_pli_obj_head = (void *)nobj ;
        break ;
    }
    
    obj = nobj ;
    
  }
  
  /* find real time out link, check posedge of CLOCK (GLOBAL_DATA[0]) */
  if( JD_current_toplevel->time_queue && is_CLOCK_posedge() ) {
    JD_count_timeq( &JD_current_toplevel->time_queue ) ;
  }

#ifdef DEBUG_VPI_MODE
  PRINTF( "  misctf_reason_sync b\n" ) ;
#endif
    
  /*
   * loop the following till ready queue is empty  (see top_loop.c)
   *  JD_v_top_loop generates a thread (a real pthread thread) 'v_top_loop'
   *  which handshakes with the Jeda program thread (it's actually run as
   *  a pthread thread), till there's no active thread in the ready queue.
   *
   */
     
  JD_v_top_loop() ;
    
#ifdef DEBUG_VPI_MODE
  PRINTF( "  misctf_reason_sync c\n" ) ;
#endif
  /*
   * Clean up the flag, chained clocked port, etc, and return to Verilog
   */
  obj = (JD_pli_object *)JD_current_toplevel->clocked_pli_obj_head ;    
  
  while( obj ) {
         
    /* clear the edge info. & sampled edge info. on port whose ref-clock 
       is this object */
    clear_port_on_ref_edge( obj ) ;
        
    pobj = obj->next_clocked ;
    obj->next_clocked = NULL ;
    obj = pobj ;
  }
  JD_current_toplevel->clocked_pli_obj_head = 
  JD_current_toplevel->clocked_pli_obj_tail = NULL ;

  /* top->changed_pli_obj_head/tail link holds the non-ref clock port obj 
     that changed it's value
  */
  /* Do the similar edge detecton on all obj linked in chenged_pli_obj */
  obj = (JD_pli_object *)JD_current_toplevel->changed_pli_obj_head ;    
  
  while( obj ) {
         
    
    /* process pli request linked to the clock/edge */
    switch(obj->type) {
      case JD_obj_port:
        /* get the value on this clock obj and generate edge information */
        gen_ref_clock_edge( obj ) ;
        clear_port_on_ref_edge( obj ) ;
        break ;
      case JD_obj_veri_task:
        break ;
      case JD_obj_jd_task:
        /* action to call jeda task from verilog */
        break ;
    }
    
    pobj = obj->next_clocked ;
    obj->next_clocked = NULL ;
    obj = pobj ;
    
  }
  JD_current_toplevel->changed_pli_obj_head =
  JD_current_toplevel->changed_pli_obj_tail = NULL ;
  
#ifdef DEBUG_VPI_MODE
  PRINTF( "  misctf_reason_sync d\n" ) ;
#endif

  /* clean driven flag on port */
  obj = (JD_pli_object *)JD_current_toplevel->driven_pli_obj_head ;    
  
  while( obj ) {
    clear_driven_port( obj ) ;
    pobj = obj->next_driven ;
    obj->next_driven = NULL ;
    obj = pobj ;
  }
  
  JD_current_toplevel->driven_pli_obj_head = NULL ;
       
  /* clear syncronized flag */
  JD_current_toplevel->tf_isynchronized = 0 ;
#ifdef DEBUG_VPI_MODE
  PRINTF( "  misctf_reason_sync e\n" ) ;
#endif
  
}


/******************************************************************
 * functions process various pli request
 *
 ******************************************************************/

static int process_pli_request( JD_pli_request *req ) {
  int ret ;
  
  switch(req->type) {
    case JD_req_sync:
      /* request to sync on edge of a clock port */
      ret = process_req_sync( req ) ;
      break ;
    case JD_req_sync_port:
      /* request to sync on edge of a non-clock port */
      ret = process_req_sync_port( req ) ;
      break ;
    case JD_req_sync_p_on_c:
      /* request to sync on edge of port on ref-clk */
      ret = process_req_sync_p_on_c( req ) ;
      break ;
    default:
      /* We should not see any other request type here */
      ERROR_INTERNAL_UNKNOWN_PLI_REQUEST_TYPE_FOR_PORT(req->type) ;
      break ;
  }
  
  return ret ;
  
}

/****************************************************************************
 *  Rules of how sync is handled 
 *  if the current time slot matches the requested edge:
 *  0 : returns immediately
 *  N > 0 : returns at Nth edge
 *  
 * if the current time slot is off from the requested edge:
 *  0 : returns at next edge
 *  N > 0 : sync at next edge, then returns at Nth edge (total N+1th edge)
 *  
 ***************************************************************************/

/* request to sync on edge of a clock port */
/* This request is made via JD_sync_on_edge() function */
static int process_req_sync( JD_pli_request *req ) {
  
  if( req->is.sync.num_cycle ) {
    req->is.sync.num_cycle-- ;
    return 0 ; /* not done yet */
  }
  else {
    /* wakes up the thread */
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    JD_dequeue( req->thread->queue, req->thread ) ;
    req->thread->status = JD_thrd_ready ;
    JD_enqueue( JD_current_toplevel->ready_queue, req->thread ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ; /* release the mutex */
#endif

#ifdef JEDA_PLI_DEBUG
  PRINTF( 
    "JD_sync_on_port end: THREAD %x @ %d\n",
    req->thread, JD_pli_current_cycle
  ) ;
#endif

    return 1 ; /* finish this request */
  }
  
}

/* request to sync on edge of a non-clock port */
/* This request is made via JD_sync_port_on_clk() function */
static int process_req_sync_port( JD_pli_request *req ) {

  if( req->is.sync.num_cycle ) {
    req->is.sync.num_cycle-- ;
    return 0 ; /* not done yet */
  }
  else {
    /* wakes up the thread */
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_lock( JD_d_mutex ) ;
#endif
    JD_dequeue( req->thread->queue, req->thread ) ;
    req->thread->status = JD_thrd_ready ;
    JD_enqueue( JD_current_toplevel->ready_queue, req->thread ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
    pthread_mutex_unlock( JD_d_mutex ) ; /* release the mutex */
#endif
#ifdef JEDA_PLI_DEBUG
  PRINTF( 
    "JD_sync_on_port end: THREAD %x @ %d\n",
    req->thread, JD_pli_current_cycle
  ) ;
#endif
    /* check if this is the last sync on this non-clock port */
    req->is.sync.port->sync_count-- ;
    if( req->is.sync.port->sync_count == 0 ) {
      delete_vcl( req->is.sync.port ) ;
    }
    return 1 ; /* finish this request */
  }

}

/* request to sync on edge of port on ref-clk */
/* This request is made via JD_sync_on_port() function */
/* this task is called at the sample edge of ref-clk after sampling and
   edge calculation is done. But the actual edge may not match with the
   requested edge on the port. This must be checked within this function
 */
static int process_req_sync_p_on_c( JD_pli_request *req ) {
  JD_pli_object *obj, *ref ;
  JD_data *dt ;
  
  obj = req->is.sync.port ;
  
  dt = GLOBAL_DATA[obj->is.port.clk_index] ;
  ref = (JD_pli_object *)dt->data.signal.port ;
  
  if( ref->is.port.edge == obj->is.port.s_edge ) {
    /* on the right edge of ref clock */
    if( req->is.sync.num_cycle ) {
      req->is.sync.num_cycle-- ;
      return 0 ; /* not done yet */
    }
    else {
      /* wakes up the thread */
#ifndef JEDA_SELF_THREAD_CONTROL
      pthread_mutex_lock( JD_d_mutex ) ;
#endif
      JD_dequeue( req->thread->queue, req->thread ) ;
      req->thread->status = JD_thrd_ready ;
      JD_enqueue( JD_current_toplevel->ready_queue, req->thread ) ;
#ifndef JEDA_SELF_THREAD_CONTROL
      pthread_mutex_unlock( JD_d_mutex ) ; /* release the mutex */
#endif
    
      return 1 ; /* finish this request */
    }
  }
   
}

/* 
 * This procedure will be called from $JEDA_TASK(JEDA_LINK_INITIALIZE)
 */
void JEDA_vpi_link_create_main(vpiHandle taskH) {
  
  if( !vpi_get_vlog_info( &Verilog_info ) ) {
    fprintf( stderr, "Can't get verilog info with vpi_get_vlog_info()!!\n" ) ;
    exit(1) ;
  } 
  
  JEDA_ref_clock_vcl() ;
  
  JD_v_create_main() ;
  
  /* */ 
}

char *JD_scan_plusargs( char *arg ) {
  int len, alen ;
  int i ;
  if( !arg ) return NULL ;
  
  len = strlen(arg) ;
  for( i = 1 ; i < Verilog_info.argc ; i++ ) {
    if( Verilog_info.argv[i][0] == '+' ) {
      alen = strlen( &Verilog_info.argv[i][1] ) ;
      if( !strncmp( arg, &Verilog_info.argv[i][1], len) ) {
        if( alen > len ) {
          return &Verilog_info.argv[i][len+1] ;
        }
        else return "" ;
      }
    }
  }
  return NULL ;
}


void JEDA_vpi_link_create_top(vpiHandle taskH) {
  int vhash ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;

  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ; /* skip first one (command id) */
  argH = vpi_scan(argI) ; /* skip second (file name) */
  argH = vpi_scan(argI) ;
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  /* allocate data and execute the main */
  vhash = value_p->value.integer ; /* get vhash from verilog module */
  if( vhash != JD_vcode_hash_value ) {
    PRINTF( 
      "JEDA warning: mismatch between Jeda code and Jeda verilog module detected. This may cause a program crash.\n"
    ) ;
    exit(1) ;
  }
  
#ifdef DEBUG_VPI_MODE  
  PRINTF( "JEDA_vpi_link_create_top: calling JD_v_create_top()\n" ) ;
#endif

  JD_v_create_top() ;
  /* JD_current_toplevel->module_instance = (char *)tf_getinstance() ; */
}


/* end of ifdef JEDA_VPI_MODE */
#endif
