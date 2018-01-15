/*****************************************************************/
/* jeda_vpi.c: linked to Verilog and handle the $JEDA_vpi_task   */
/*****************************************************************/
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
#include <stdio.h>
#include <stdlib.h>
#include "runtime.h"
#include "jeda_pli.h"
#include "veriuser.h"
#include "acc_user.h"
#ifdef ICARUS_VERILOG_MODE
#  include "ivl_vpi_user.h"
#else
#  include "vpi_user.h"
#endif
#include <dlfcn.h>
#include <pthread.h>

typedef void (*vpi_function)(vpiHandle taskH) ;

static vpi_function JEDA_link_CLOCK_p ;
static vpi_function JEDA_link_port_p ;
static vpi_function JEDA_link_pset_port_p ;
static vpi_function JEDA_link_global_var_p ;
static vpi_function JEDA_link_global_var_array_p ;
static vpi_function JEDA_link_veri_task_p ;
static vpi_function JEDA_link_jd_task_p ;
static vpi_function JEDA_link_static_var_p ;
static vpi_function JEDA_link_static_var_array_p ;
static vpi_function JEDA_link_create_top_p ;
static vpi_function JEDA_link_create_main_p ;

#ifdef JEDA_STATIC_LINK

void JEDA_vpi_link_CLOCK() ;
void JEDA_vpi_link_port() ;
void JEDA_vpi_link_pset_port() ;
void JEDA_vpi_link_global_var() ;
void JEDA_vpi_link_global_var_array() ;
void JEDA_vpi_link_veri_task() ;
void JEDA_vpi_link_jd_task() ;
void JEDA_vpi_link_static_var() ;
void JEDA_vpi_link_static_var_array() ;
void JEDA_vpi_link_create_top() ;
void JEDA_vpi_link_create_main() ;

#endif

void JEDA_load_dynamic_lib() ;
int JEDA_vpi_task() ;

s_vpi_vlog_info JEDA_Verilog_info ;

void  JEDA_stop_here() {
  /* just null function to trigger the debugger after dl is loaded */
}


void JEDA_dummy_pthread_call() {
  /*
  pthread_cond_init( NULL, NULL ) ;
  pthread_cond_signal( NULL ) ;
  pthread_cond_wait( NULL, NULL ) ;
  pthread_create( NULL, NULL, NULL, NULL ) ;
  pthread_detach( NULL ) ;
  */
  pthread_exit(0) ;
  /*
  pthread_mutex_init( NULL, NULL ) ;
  pthread_mutex_lock( NULL ) ;
  pthread_mutex_unlock( NULL ) ;
  */
}


/* Seting up $JEDA_vpi_task callback to JEDA_vpi_task() */
void JEDA_setup_vpi_callbacks() {
  static s_vpi_systf_data s_systf_data ;
  static p_vpi_systf_data p_d = &s_systf_data ;
  
  p_d->type = vpiSysTask ;

#ifndef ICARUS_VERILOG_MODE
  p_d->sysfunctype = 0 ;
#endif

  p_d->tfname = "$JEDA_vpi_task" ;
  p_d->calltf = (int(*)()) JEDA_vpi_task ;
  p_d->compiletf = NULL ;
  p_d->sizetf = NULL ;
  p_d->user_data = NULL ;
  
  vpi_register_systf( p_d ) ;
  
  if( !vpi_get_vlog_info( &JEDA_Verilog_info ) ) {
    fprintf( stderr, "Can't get verilog info with vpi_get_vlog_info()!!\n" ) ;
    exit(1) ;
  }

}

int JEDA_vpi_task() {
  int index ;
  vpiHandle taskH ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  taskH = vpi_handle(vpiSysTfCall,NULL) ;
  argI = vpi_iterate(vpiArgument, taskH) ;
  argH = vpi_scan(argI) ;
  
  value_p->format = vpiIntVal ;
  vpi_get_value( argH, value_p ) ;
  
  index = value_p->value.integer ; /* first arg is the index to the task */
  
#ifdef JEDA_VPI_MODE
  /* PRINTF( "JEDA_vpi_task: index %d\n", index ) ; */
#endif
  
  switch( index ) {
    case JEDA_INIT:
      JEDA_load_dynamic_lib(taskH) ;
      break ;
    case JEDA_CLOCK:
      (*JEDA_link_CLOCK_p)(taskH) ;
      break ;
    case JEDA_LINK_PORT:
      (*JEDA_link_port_p)(taskH) ;
      break ;
    case JEDA_LINK_PSET_PORT:
      (*JEDA_link_pset_port_p)(taskH) ;
      break ;
    case JEDA_LINK_GLOBAL_VAR:
      (*JEDA_link_global_var_p)(taskH) ;
      break ;
    case JEDA_LINK_GLOBAL_VAR_ARRAY:
      (*JEDA_link_global_var_array_p)(taskH) ;
      break ;
    case JEDA_LINK_VERI_TASK:
      (*JEDA_link_veri_task_p)(taskH) ;
      break ;
    case JEDA_LINK_JD_TASK:
      (*JEDA_link_jd_task_p)(taskH) ;
      break ;
    case JEDA_LINK_STATIC_VAR:
      (*JEDA_link_static_var_p)(taskH) ;
      break ;
    case JEDA_LINK_STATIC_VAR_ARRAY:
      (*JEDA_link_static_var_array_p)(taskH) ;
      break ;
    case JEDA_LINK_CREATE_MAIN:
      (*JEDA_link_create_main_p)(taskH) ;
      break ;
  }
  
}

char *JD_vpi_scan_plusargs( char *arg ) {
  int len, alen ;
  int i ;
  if( !arg ) return NULL ;
  
  len = strlen(arg) ;
  for( i = 1 ; i < JEDA_Verilog_info.argc ; i++ ) {
    if( JEDA_Verilog_info.argv[i][0] == '+' ) {
      alen = strlen( &JEDA_Verilog_info.argv[i][1] ) ;
      if( !strncmp( arg, &JEDA_Verilog_info.argv[i][1], len) ) {
        if( alen > len ) {
          return &JEDA_Verilog_info.argv[i][len+1] ;
        }
        else return "" ;
      }
    }
  }
  return NULL ;
}

/* JEDA_load_dynamic_lib(): 
 *   Loading dynamic lib as the runtime image
 */
void JEDA_load_dynamic_lib(vpiHandle taskH) 
{
  char *nm, *cwd, *dl_fname, *dname ;
  void *library ;
  char *dlerr ;
  vpiHandle argI, argH ;
  static s_vpi_value value_s ;
  static p_vpi_value value_p = &value_s ;
  
  
#ifndef JEDA_STATIC_LINK
  dl_fname = JD_vpi_scan_plusargs( "jeda_code=" ) ;
  
  if( dl_fname == NULL ) {
    /* look for the d-lib from argument */
    argI = vpi_iterate(vpiArgument, taskH) ;
    argH = vpi_scan(argI) ; /* skip first one (command id) */
    argH = vpi_scan(argI) ; /* file name */
    value_p->format = vpiStringVal ;
    vpi_get_value( argH, value_p ) ;
    nm = value_p->value.str ;
    cwd = (char *)getcwd( NULL, 1024 ) ;
    dl_fname = (char *)calloc( strlen(cwd)+strlen(nm) + 256, sizeof(char) ) ;
    strcpy( dl_fname, cwd ) ;
    /* strcat( dl_fname, "/jeda_work/" ) ; */
    strcat( dl_fname, "/" ) ;
    strcat( dl_fname, nm ) ;
    strcat( dl_fname, ".dl" ) ;
  }
  else {
    if( dl_fname[0] != '/' ) {
      dname = dl_fname ;
      cwd = (char *)getcwd( NULL, 1024 ) ;
      dl_fname = 
        (char *)calloc( strlen(cwd)+strlen(dname) + 256, sizeof(char) ) ;
      strcpy( dl_fname, cwd ) ;
      strcat( dl_fname, "/" ) ;
      strcat( dl_fname, dname ) ;
    }
  }
    
  PRINTF( "Jeda: loading dynamic lib %s\n", dl_fname ) ; 
  
  library = dlopen( dl_fname, RTLD_NOW | RTLD_GLOBAL ) ;
  
  if( !library ) {
    PRINTF( 
      "Jeda: Can't open dynamic lib %s : %s\n", dl_fname, dlerror() 
    );
    exit(1) ;
  }

  dlerror() ;
  
  /* PRINTF( "Jeda: linking dynamic lib %s\n", dl_fname ) ; */
  
  JEDA_link_CLOCK_p = dlsym(library, "JEDA_vpi_link_CLOCK" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_CLOCK\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_port_p = dlsym(library, "JEDA_vpi_link_port" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_port\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_pset_port_p = dlsym(library, "JEDA_vpi_link_pset_port" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_pset_port\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_global_var_p = dlsym(library, "JEDA_vpi_link_global_var" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_global_var\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_global_var_array_p = 
    dlsym(library, "JEDA_vpi_link_global_var_array" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_global_var_array\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_veri_task_p = dlsym(library, "JEDA_vpi_link_veri_task" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_veri_task\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_jd_task_p = dlsym(library, "JEDA_vpi_link_jd_task" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_jd_task\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_static_var_p = dlsym(library, "JEDA_vpi_link_static_var" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_static_var\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_static_var_array_p = 
    dlsym(library, "JEDA_vpi_link_static_var_array" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_static_var_array\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_create_main_p = dlsym(library, "JEDA_vpi_link_create_main" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_create_main\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_create_top_p = dlsym(library, "JEDA_vpi_link_create_top" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_vpi_link_create_top\" : %s\n", dlerror() 
    );
    exit(1) ;
  }

#else


  JEDA_link_CLOCK_p = JEDA_vpi_link_CLOCK ;
  JEDA_link_port_p = JEDA_vpi_link_port ;
  JEDA_link_pset_port_p = JEDA_vpi_link_pset_port ;
  JEDA_link_global_var_p = JEDA_vpi_link_global_var ;
  JEDA_link_global_var_array_p = JEDA_vpi_link_global_var_array ;
  JEDA_link_veri_task_p = JEDA_vpi_link_veri_task ;
  JEDA_link_jd_task_p = JEDA_vpi_link_jd_task ;
  JEDA_link_static_var_p = JEDA_vpi_link_static_var ;
  JEDA_link_static_var_array_p = JEDA_vpi_link_static_var_array ;
  JEDA_link_create_main_p = JEDA_vpi_link_create_main ;
  JEDA_link_create_top_p = JEDA_vpi_link_create_top ;

#endif
  
  JEDA_stop_here() ;
  
  (*JEDA_link_create_top_p)(taskH) ;
  
}
