/***************************************************************************
 *
 *  smain.c: main function for Jeda standalone mode
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
   Copyright (C) 2003 Jeda Technologies, Inc.

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
#include <time.h>
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

#include "pli.h"

#include "jeda_pli.h"

#ifdef PRODUCT_CODE
#include "lmpolicy.h"
#include "../include/license.h"
LP_HANDLE *JEDA_lp_handle ;
#endif

#define NULL_COUNT_MAX 100
#define	to_char(n)	((n) + '0')

extern JD_static_value_node **JD_static_value_hash_table ;

extern JD_toplevel *JD_current_toplevel ;
extern JD_toplevel *JD_top_of_toplevel ;

extern pthread_mutex_t *JD_m_mutex ;

extern JD_queue *JD_ready_queue ;
extern JD_queue *JD_sleep_queue ;
extern JD_queue *JD_zombie_queue ;

void JD_uf_main() ;
void JD_si_alloc_global(JD_toplevel *) ;
void JD_si_initialize() ;
void JD_standalone_link() ;
static void update_port_out( JD_pli_object *obj, p_vecval value ) ;

extern int JD_thread_empty ;

extern int JD_invoke_debugger ;

int JD_pli_current_cycle ;
int JD_synchronize_flag ;
int JD_dump_vcd_flag ;

static int main_argc ;
static char **main_argv ;

unsigned int JD_current_time_u ;
unsigned int JD_current_time_l ;

JD_standalone_sim_event *time_wheel_top = NULL ;

extern int JEDA_ref_clk_vcl_func() ;

static void update_port_in() ;


/******************************************/
/* VCD file generation                    */
/******************************************/
static FILE *vf = NULL ;
static int signal_index = 0 ;
static char *id_char ;
static int id_len ;
static int max_size = 128 ;
static char *b_buf = NULL ;
static  JD_pli_object *CLOCK_obj ;
static int cyclecount_vcd_id ;

void JD_vcd_open_file( char *fname ) {
  int i, j ;
  time_t clock ;
  char *date ;
  
  id_char = (char *)calloc( 126 - 31, sizeof(char) ) ;
  
  for( i = 0, j = 33 ; j < 127 ; i++, j++ ) {
    id_char[i] = j ;
  }
  id_len = 126 - 32 ;
  
  vf = fopen( fname, "w" ) ;
  
  if( !vf ) {
    fprintf( stderr, "Can't open VCD file \"%s\" \n", fname ) ;
    return ;
  }
  
  fprintf( stderr, "Creating VCD file \"%s\" \n", fname ) ;
  
  /* header creation */
  fprintf( vf, "$date\n" ) ;
  clock = time(0) ;
  date = ctime( &clock ) ;
  fprintf( vf, "  %s\n", date ) ;
  fprintf( vf, "$end\n" ) ;

  fprintf( vf, "$version\n" ) ;
  fprintf( vf, "  Jeda 1.0.x Standalone Mode\n" ) ;
  fprintf( vf, "$end\n" ) ;

  fprintf( vf, "$timescale\n" ) ;
  fprintf( vf, "  1 ns\n" ) ;
  fprintf( vf, "$end\n" ) ;

  fprintf( vf, "$scope module jeda_module $end\n" ) ;

}

void JD_vcd_close_file() {
  if( vf == NULL ) return ;
  fclose( vf ) ;
  vf = NULL ;
}

void JD_vcd_start_dump() {
  int i, x ;
  if( vf == NULL ) return ;
  fprintf( vf, "$enddefinitions $end\n" ) ;  
  fprintf( vf, "#0\n" ) ;  
  
  for( i= 0 ; i < signal_index ; i++ ) {
    if( i == cyclecount_vcd_id ) {
      fprintf( vf, "b0 " ) ;
    }
    else {
      fprintf( vf, "bx " ) ;
    }
    x = i ;
    fprintf( vf, "%c", id_char[(x%id_len)] ) ;
    x /= id_len ;
    while( x ) {
      fprintf( vf, "%c", id_char[(x%id_len)] ) ;
      x /= id_len ;
    }
    fprintf( vf, "\n" ) ;
  }
}

/* static version of bit_ge_bit, which does not check B0/B1 */
static int bit_ge_bit( int *A0, int S0, int *A1, int S1 ) {
  register int RET ;
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        return 0 ;
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          return 1 ;
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          return 1 ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          return 0 ;  /* A0 < A1 */
        }
      }
    }
    return 1 ; /* A0 == A1 */
  }
}

void JD_vcd_set_time( int t_h, int t_l ) {
  int ten, i, j ;
  int ta[2] ;
  int tb[2] ;
  if( vf == NULL ) return ;

  if( b_buf == NULL ) {
    b_buf = (char *)calloc( max_size+5, sizeof(char) ) ;
  }
  
  if( t_h ) {
    /* print in long format */
    ten = 10 ;
    j = 0 ;
    ta[1] = t_h ;
    ta[0] = t_l ;
    while( bit_ge_bit( ta, 64, &ten, 32 ) ) {
      JD_bit_div_bit_ts( tb, 64, ta, 64, &ten, 32, &i ) ; /* divide by 10 */
      b_buf[j++] = to_char(i) ;
      ta[0] = tb[0] ;
      ta[1] = tb[1] ;
    }
    b_buf[j] = to_char(ta[0] % 10) ;
    while( j && b_buf[j] == '0' ) j-- ; /* zero suppress */
    fprintf( vf, "#%s\n", &b_buf[j] ) ;  
  }
  else {
    fprintf( vf, "#%d\n", t_l ) ;  
  }
}

int JD_vcd_define_signal( char *name, int size ) {
  int x ;
  
  if( vf == NULL ) return 0 ;
  fprintf( vf, "$var wire %d ", size ) ;
  x = signal_index ;
  fprintf( vf, "%c", id_char[(x%id_len)] ) ;
  x /= id_len ;
  while( x ) {
    fprintf( vf, "%c", id_char[(x%id_len)] ) ;
    x /= id_len ;
  }
  fprintf( vf, " %s $end\n", name ) ;
  
  if( max_size < size ) max_size = size ;
  return signal_index++ ;
  
}

void JD_vcd_dump_var( int sig, p_vecval value, int size ) {
  int i, j, k, m, nw ;
  int a, b, x ;
  char c ;
  if( vf == NULL ) return ;
  if( b_buf == NULL ) {
    b_buf = (char *)calloc( max_size+5, sizeof(char) ) ;
  }
  
  b_buf[size] = 0 ;
  nw = NWORD(size) ;
  k = size-1 ;
  for( i = 0 ; i < nw ; i++ ) {
    a = value[i].avalbits ;
    b = value[i].bvalbits ;
    if( i == nw-1 ) m = (size<=32)?size:(size % 32) ;
    else m = 32 ;
    for( j = 0 ; j < m ; j++ ) {
      x = (a & 1) ;
      x |= ((b & 1)<<1) ;
      switch( x ) {
        case 0: b_buf[k] = '0' ; break ;
        case 1: b_buf[k] = '1' ; break ;
        case 2: b_buf[k] = 'z' ; break ;
        case 3: b_buf[k] = 'x' ; break ;
      }
      a >>= 1 ;
      b >>= 1 ;
      k-- ;
    }
  }
  k = 0 ;
  if( size > 1 ) {
    c = b_buf[0] ;
    if( c == 'x' || c == 'z' || c == '0' ) {
      for( k = 1 ; k < size ; k++ ) {
        if( b_buf[k] != c ) break ;
      }
      if( c != '0' || b_buf[k] != '1' ) k-- ;
    }
  }
  fprintf( vf, "b%s ", &b_buf[k] ) ;
  x = sig ;
  fprintf( vf, "%c", id_char[(x%id_len)] ) ;
  x /= id_len ;
  while( x ) {
    fprintf( vf, "%c", id_char[(x%id_len)] ) ;
    x /= id_len ;
  }
  fprintf( vf, "\n" ) ;
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

/*********************************************************/
/* Time wheel implementation                             */
/*********************************************************/
static void insert_sim_event( JD_standalone_sim_event *event ) 
{ 
  JD_standalone_sim_event *te ;
  if( time_wheel_top == NULL ) {
    time_wheel_top = event ;
  }
  else {
    te = time_wheel_top ;
    if( tf_compare_long( 
            event->l_time, event->u_time, te->l_time, te->u_time
          ) < 0
        )
    {
      time_wheel_top = event ;
      event->next = te ;
    }
    else {
      while( te->next ) {
        if( tf_compare_long( 
              event->l_time, event->u_time, te->next->l_time, te->next->u_time
            ) < 0
          )
        {
          break ;
        }
        else te = te->next ;
      }
      event->next = te->next ;
      te->next = event ;
    }
  }
}

static void process_event( JD_standalone_sim_event *event ) {
  switch( event->type ) {
    case JD_standalone_drive:
      update_port_out( 
        event->obj, event->obj->is.port.o_exprinfo->expr_value_p 
      ) ;
      break ;
    case JD_standalone_sample:
      update_port_in( event->obj, event->obj->is.port.final_val ) ;
      break ;
    case JD_standalone_delay:
      JD_misctf_reason_reactivate() ;
      break ;
    default:
      fprintf( stderr, "Error, unknown event %d\n", event->type ) ;
      exit( 1 ) ;
  }
  if( event->value ) free( event->value ) ;
  free( event ) ;
}

/*********************************************************/
/* Simulation loop                                       */
/*********************************************************/
static void main_top_loop() {
  JD_standalone_sim_event *te ;
  JD_pli_object *obj ;
  int tedge, pedge, nw, i ;
  while(1) {
    JD_synchronize_flag = 0 ;
    JD_top_loop() ;
    if( time_wheel_top == NULL ) break ;
    te = time_wheel_top ;
    JD_current_time_u = te->u_time ;
    JD_current_time_l = te->l_time ;
    JD_vcd_set_time( JD_current_time_u, JD_current_time_l ) ;
    while( te && 
           tf_compare_long( 
             te->l_time, te->u_time, JD_current_time_l, JD_current_time_u
           ) == 0
         )
    {
      time_wheel_top = te->next ;
      process_event( te ) ;
      te = time_wheel_top ;
    }
    if( JD_synchronize_flag ) {
      obj = (JD_pli_object *)JD_current_toplevel->clocked_pli_obj_head ;    
  
      while( obj ) {
        if( obj == CLOCK_obj ) {
          /* this is CLOCK, detecting the posedge */
          tedge = obj->is.port.i_exprinfo->expr_value_p[0].avalbits & 1 ;
          tedge |= 
            ( obj->is.port.i_exprinfo->expr_value_p[0].bvalbits<<1 ) & 2 ;
          pedge = obj->is.port.last_val[0].avalbits & 1 ;
          pedge |= ( obj->is.port.last_val[0].bvalbits << 1 ) & 2 ;
          if( (pedge == 0) && (tedge == 1) ) {
            /* posedge of CLOCK, increment CycleCount */
            ((p_tfexprinfo)(JD_current_toplevel->CycleCount_exprinfo))
              ->expr_value_p[0].avalbits++ ;
            JD_vcd_dump_var ( 
              cyclecount_vcd_id,
              ((p_tfexprinfo)(JD_current_toplevel->CycleCount_exprinfo)) 
                ->expr_value_p,
              32
            ) ;
          }
          obj = NULL ;
        }
        else obj = obj->next_clocked ;
      }
      
      JD_misctf_reason_sync() ;
    }
  }
  fprintf( stderr, "End of execution\n" ) ;
}


/*********************************************************/
/* main function                                         */
/*********************************************************/
main( argc, argv )
  int argc ;
  char *argv[] ;
{
  int i ;
  char *fname ;
  
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  {
    char *licdir ;
    char *homedir = (char *)getenv( "JEDA_HOME" ) ;
    int r ;
  
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
    if(  
      lp_checkout( 
        LPCODE, LM_RESTRICTIVE, LM_RUN_FEATURE, LM_VERSION, 
        1, licdir, &JEDA_lp_handle
      )
    ) {
      lp_perror( JEDA_lp_handle, "Checkout failed" ) ;
      exit( 1 ) ;
    }
    r = (int)time(0) ;
    JD_top_check( r, check_dsa_key( r, 0 ) ) ;
    JD_data_check( r, check_dsa_key( r, 0 ) ) ;
    JD_thread_check( r, check_dsa_key( r, 0 ) ) ;
    JD_exec_check( r, check_dsa_key( r, 0 ) ) ;
    JD_pli_check( r, check_dsa_key( r, 0 ) ) ;
  }
#  endif
#endif

  main_argc = argc ;
  main_argv = argv ;
  
  for( i = 1 ; i < argc ; i++ ) {
  
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
        case 'V':
          fname = argv[++i] ;
          JD_vcd_open_file( fname ) ;
          break ;
        default:
          fprintf( stderr, "Unknown option %s ignored\n", argv[i] ) ;
          break ;
      }
    }
  }
  
  /* JD_initialize_top() ; */
  JD_v_create_top() ;
  /* allocate standalone signal structure */
  JD_standalone_link() ;
  
  JD_v_create_main() ;
  
  JD_vcd_start_dump() ;
  
  main_top_loop() ;
  
  JD_vcd_close_file() ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
  exit(0) ;
}

/*********************************************************/
/* Standalone version of PLI support functions           */
/*********************************************************/
char *mc_scan_plusargs( char *arg ) {
  int len, alen ;
  int i ;
  if( !arg ) return NULL ;
  
  len = strlen(arg) ;
  for( i = 1 ; i < main_argc ; i++ ) {
    if( main_argv[i][0] == '+' ) {
      alen = strlen( &main_argv[i][1] ) ;
      if( !strncmp( arg, &main_argv[i][1], len) ) {
        if( alen > len ) {
          return &main_argv[i][len+1] ;
        }
        else return "" ;
      }
    }
  }
  return NULL ;
}

tf_dostop() {
  fprintf( stderr, "tf_dostop() called. exitting..\n" ) ;
  JD_vcd_close_file() ;
  exit(0) ;
}

tf_dofinish() {
  fprintf( stderr, "tf_dofinish() called. exitting..\n" ) ;
  JD_vcd_close_file() ;
  exit(0) ;
}

char *tf_getcstringp(int nparam) { 
  /* should not be called */
}

tf_isynchronize(char *inst ) { 
  JD_synchronize_flag = 1 ;
}

tf_getp(int pnum) { 
  /* should not be called */
}

tf_isetdelay(
  int delay,
  char *module
) 
{ 
  JD_standalone_sim_event *dly_event ;
  
  dly_event =
    (JD_standalone_sim_event *)calloc(1,sizeof(JD_standalone_sim_event) ) ;
  dly_event->type = JD_standalone_delay ;
  dly_event->u_time = JD_current_time_u ;
  dly_event->l_time = JD_current_time_l ;
  tf_add_long(
    &dly_event->l_time,
    &dly_event->u_time,
    delay,
    0
  )  ;
  insert_sim_event( dly_event ) ;
  
}

char *tf_getinstance() { 
  /* should not be called */
}

handle  acc_handle_tfarg(int n) {
  /* should not be called */
}

int tf_compare_long(
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

int tf_add_long(
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

void acc_vcl_add(
  handle object_p, int (*consumer)(), char *user_data, int vcl_flags
) { 
  /* the structure is already in obj, so do nothing */
}   

int tf_ievaluatep(int pnum, char *inst) { 
  /* do nothing */
} 

int tf_getlongtime( int *u ) { 
  *u = JD_current_time_u ;
  return JD_current_time_l ;
}

static void update_port_in( JD_pli_object *obj, p_vecval value ) 
{
  int changed ;
  int i, nw ;
  s_vc_record rc ;
  changed = 0 ;
  nw = NWORD(obj->is.port.size) ;
  for( i = 0 ; i < nw ; i++ ) {
    if( obj->is.port.i_exprinfo->expr_value_p[i].avalbits != value[i].avalbits ||
        obj->is.port.i_exprinfo->expr_value_p[i].bvalbits != value[i].bvalbits
    ) {
      changed = 1 ;
    }
    obj->is.port.i_exprinfo->expr_value_p[i].avalbits = value[i].avalbits ;
    obj->is.port.i_exprinfo->expr_value_p[i].bvalbits = value[i].bvalbits ;
  }
  if( changed ) {
    /* transfer it to input size */
    if( obj->vcl ) {
      /* call vcl func */
      rc.user_data = (char *)obj->vcl->data ;
      (*obj->vcl->func)( &rc ) ;
    }
    
  }
}

static void update_port_out( JD_pli_object *obj, p_vecval value ) 
{
  int changed ;
  int i, nw ;
  changed = 0 ;
  nw = NWORD(obj->is.port.size) ;
  
  for( i = 0 ; i < nw ; i++ ) {
    if( obj->is.port.final_val[i].avalbits != value[i].avalbits ||
        obj->is.port.final_val[i].bvalbits != value[i].bvalbits
    ) {
      changed = 1 ;
    }
    obj->is.port.final_val[i].avalbits = value[i].avalbits ;
    obj->is.port.final_val[i].bvalbits = value[i].bvalbits ;
  }
  if( changed ) {
    /* transfer it to input size */
    if( obj->is.port.sample_skew ) {
      JD_standalone_sim_event *smpl_event ;
  
      smpl_event =
        (JD_standalone_sim_event *)
        calloc(1,sizeof(JD_standalone_sim_event) ) ;
      smpl_event->type = JD_standalone_sample ;
      smpl_event->u_time = JD_current_time_u ;
      smpl_event->l_time = JD_current_time_l ;
      tf_add_long(
        &smpl_event->l_time,
        &smpl_event->u_time,
        obj->is.port.sample_skew,
        0
      )  ;
      smpl_event->obj = obj ;
      smpl_event->value = alloc_vecval( obj->is.port.size ) ;
      copy_vecval( 
        smpl_event->value, obj->is.port.final_val,
        obj->is.port.size
      ) ;
      insert_sim_event( smpl_event ) ;
    }
    else {
      update_port_in( obj, obj->is.port.final_val ) ;
    }
    JD_vcd_dump_var( obj->vcd_id, obj->is.port.final_val, obj->is.port.size ) ;
    
  }
}

static void update_variable( JD_pli_object *obj )
{
  int changed ;
  int i, nw ;
  changed = 0 ;
  nw = NWORD(obj->is.variable.size) ;
  
  for( i = 0 ; i < nw ; i++ ) {
    if( obj->is.variable.final_val[i].avalbits !=
                  obj->is.variable.value->expr_value_p[i].avalbits ||
        obj->is.variable.final_val[i].bvalbits != 
                  obj->is.variable.value->expr_value_p[i].bvalbits
    ) {
      changed = 1 ;
    }
    obj->is.variable.final_val[i].avalbits = 
      obj->is.variable.value->expr_value_p[i].avalbits ;
    obj->is.variable.final_val[i].bvalbits = 
      obj->is.variable.value->expr_value_p[i].bvalbits ;
  }
  if( changed ) {
    if( JD_dump_vcd_flag ) {
      /* create dump info */
      /* working here */
    }
  }
}

tf_ipropagatep(
  int index, char *instance
) { 
  JD_data *dt ;
  JD_pli_object *obj ;
  if( !instance ) return ;
  
  obj = (JD_pli_object *)instance ;
  
  switch( obj->type ) {
    case JD_obj_port:
      if( obj->is.port.drive_skew ) {
        JD_standalone_sim_event *drv_event ;
  
        drv_event =
          (JD_standalone_sim_event *)
          calloc(1,sizeof(JD_standalone_sim_event) ) ;
        drv_event->type = JD_standalone_drive ;
        drv_event->u_time = JD_current_time_u ;
        drv_event->l_time = JD_current_time_l ;
        tf_add_long(
          &drv_event->l_time,
          &drv_event->u_time,
          obj->is.port.drive_skew,
          0
        )  ;
        drv_event->obj = obj ;
        drv_event->value = alloc_vecval( obj->is.port.size ) ;
        copy_vecval( 
          drv_event->value, obj->is.port.o_exprinfo->expr_value_p,
          obj->is.port.size
        ) ;
        insert_sim_event( drv_event ) ;
      }
      else {
        update_port_out( obj, obj->is.port.o_exprinfo->expr_value_p ) ;
      }
      break ;
    case JD_obj_variable:
      update_variable( obj ) ;
      break ;
    case JD_obj_static_variable:
      update_variable( obj ) ;
      break ;
  }
}

p_tfexprinfo tf_exprinfo(int pnum, p_tfexprinfo pinfo) { 
  /* should not be called */
}

void acc_vcl_delete( 
  handle object_p, int (*consumer)(), char *user_data, int vcl_flags
) {
  /* do nothing */
}

/*
io_printf( char *c ) {
  printf( c ) ;
}
*/


void JD_standalone_link_CLOCK(){
  JD_data *dt ;
  JD_pli_object *obj ;
  JD_vcl_node *vcl ;
  
  JD_alloc_pset_port( 
    /* pset, name, ub, lb, index,      dir, 
       s_edge, depth, d_edge, clk_index         */
    NULL, "CLOCK",  0,  0,     0,   JEDA_INOUT, 
    JEDA_POSEDGE, 0, JEDA_POSEDGE, 0
  ) ;
  dt = GLOBAL_DATA[0] ;
  CLOCK_obj = obj = (JD_pli_object *)dt->data.signal.port ;
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( 1 ) ;
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( 1 ) ;
  obj->is.port.final_val = alloc_vecval( 1 ) ;
  obj->instance = (char *)obj ;
  JD_current_toplevel->CycleCount_exprinfo = 
    (char *)malloc( sizeof(s_tfexprinfo) );
  ((p_tfexprinfo)JD_current_toplevel->CycleCount_exprinfo)->expr_value_p
    = alloc_vecval( 32 ) ;
  ((p_tfexprinfo)JD_current_toplevel->CycleCount_exprinfo)
    ->expr_value_p[0].avalbits = 0 ;
  ((p_tfexprinfo)JD_current_toplevel->CycleCount_exprinfo)
    ->expr_value_p[0].bvalbits = 0 ;
  
  JD_add_vcl( obj, JEDA_ref_clk_vcl_func ) ;
  obj->ref_clock = 1 ;
    
  obj->next_on_clk = 
     (JD_pli_object *)JD_current_toplevel->ref_clock_pli_obj_head ;
  JD_current_toplevel->ref_clock_pli_obj_head = (int *)obj ;

  cyclecount_vcd_id = JD_vcd_define_signal( "CycleCount", 32 ) ;
  obj->vcd_id = JD_vcd_define_signal( "CLOCK", 1 ) ;
  
}

void JD_standalone_port_link(
  char *name, int index, int dir, int sample_skew, int drive_skew
) {
  JD_data *dt ;
  JD_pli_object *obj ;
  JD_vcl_node *vcl ;

  dt = GLOBAL_DATA[index] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.sample_skew = sample_skew ;
  obj->is.port.drive_skew = drive_skew ;
  obj->is.port.final_val = alloc_vecval( obj->is.port.size ) ;
  obj->instance = (char *)obj ;

  obj->vcd_id = JD_vcd_define_signal( name, obj->is.port.size ) ;
  
}

void JD_standalone_portset_link(
  char *pset_name, char *name, 
  int index, int dir, int sample_skew, int drive_skew
) {
  JD_data *dt ;
  JD_pli_object *obj ;
  char *s ;
  
  dt = GLOBAL_DATA[index] ;
  obj = (JD_pli_object *)dt->data.signal.port ;
  obj->is.port.i_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.i_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.o_exprinfo = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.port.o_exprinfo->expr_value_p = alloc_vecval( obj->is.port.size ) ;
  obj->is.port.sample_skew = sample_skew ;
  obj->is.port.drive_skew = drive_skew ;
  obj->is.port.final_val = alloc_vecval( obj->is.port.size ) ;
  obj->instance = (char *)obj ;
  
  s = (char *)calloc( strlen(pset_name)+strlen(name)+5, sizeof(char) ) ;
  strcat( s, pset_name ) ;
  strcat( s, "_" ) ;
  strcat( s, name ) ;
  obj->vcd_id = JD_vcd_define_signal( s, obj->is.port.size ) ;
  free( s ) ;
}

void JD_standalone_global_var_link(
  char *name, int index, int size
) {
  JD_data *dt ;
  JD_pli_object *obj ;
  char *s ;

  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_variable ;  
  obj->is.variable.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.variable.value->expr_value_p = alloc_vecval( size ) ;
  obj->is.variable.name = (char *)strdup(name) ;
  obj->is.variable.size = size ;
  obj->is.variable.final_val = alloc_vecval( size ) ;
  JD_current_toplevel->global_obj[index] = (int *)obj ;
  obj->instance = (char *)obj ;

  s = (char *)calloc( strlen(name)+10, sizeof(char) ) ;
  strcat( s, "var_" ) ;
  strcat( s, name ) ;
  obj->vcd_id = JD_vcd_define_signal( s, size ) ;
  free(s) ;
}

void JD_standalone_global_var_array_link(

) {
  /* array update can't be observed over vcd dump, so do nothing */
}

void JD_standalone_static_var_link(
  char *cls, char *fnc, char *name, int index, int size
) {
  JD_data *dt ;
  JD_pli_object *obj ;
  int hs ;
  char *s ;
  
  if( JD_static_value_hash_table == NULL ) {
    JD_static_value_hash_table = 
      (JD_static_value_node **)
      calloc(NUM_STATIC_HASH, sizeof(JD_static_value_node *) ) ;
  }
  obj = (JD_pli_object *)calloc( 1, sizeof(JD_pli_object) ) ;
  obj->type = JD_obj_static_variable ;  
  obj->is.variable.value = (p_tfexprinfo)malloc( sizeof(s_tfexprinfo) );
  obj->is.variable.value->expr_value_p = alloc_vecval( size ) ;
  obj->is.variable.final_val = alloc_vecval( size ) ;
  obj->is.variable.class = (char *)strdup(cls) ;
  obj->is.variable.func = (char *)strdup(fnc) ;
  obj->is.variable.name = (char *)strdup(name) ;
  obj->is.variable.size = size ;
  obj->instance = (char *)obj ;

  s = (char *)calloc( strlen(cls)+strlen(fnc)+strlen(name)+10, sizeof(char) ) ;
  strcat( s, "svar_" ) ;
  strcat( s, cls ) ;
  strcat( s, "_" ) ;
  strcat( s, fnc ) ;
  strcat( s, "_" ) ;
  strcat( s, name ) ;
  obj->vcd_id = JD_vcd_define_signal( s, size ) ;
  free(s) ;
}

void JD_standalone_static_var_array_link() {
  /* array update can't be observed over vcd dump, so do nothing */
}

void JD_standalone_veri_task_link() {
  /* standalone can't call verilog task, so do nothing */
}

#ifdef PRODUCT_CODE
/****************************************************************
 * license functions
 ****************************************************************/

LP_HANDLE *JD_pli_checkout_license(  
  char *feature, char *version, char *licdir 
) {
  LP_HANDLE *lp_handle ;
  if(  
    lp_checkout( 
      LPCODE, LM_RESTRICTIVE, feature, version, 
      1, licdir, &lp_handle
    )
  ) {
    lp_perror( lp_handle, "Checkout failed" ) ;
    exit( 1 ) ;
  }
  return lp_handle ;
}

void JD_pli_checkin_license(LP_HANDLE *lp_handle) {
  lp_checkin( lp_handle ) ;
}

#endif
