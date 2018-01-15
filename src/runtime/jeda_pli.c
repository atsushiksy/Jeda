/*************************************************************/
/* jeda_pli.c: linked to Verilog and handle the $JEDA_task   */
/*************************************************************/
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
#include <stdlib.h>
#include "runtime.h"
#include "jeda_pli.h"
#include "veriuser.h"
#include "acc_user.h"
#include <dlfcn.h>

#include <pthread.h>

#ifdef PRODUCT_CODE
#include "lmpolicy.h"
#include "../include/license.h"
LP_HANDLE *JEDA_lp_handle ;
#endif

typedef void (*pli_function)(void) ;
typedef void (*check_function)(int r, int k) ;

static pli_function JD_misctf_reason_reactivate_p ;
static pli_function JD_misctf_reason_sync_p ;
static pli_function JEDA_link_CLOCK_p ;
static pli_function JEDA_link_port_p ;
static pli_function JEDA_link_pset_port_p ;
static pli_function JEDA_link_global_var_p ;
static pli_function JEDA_link_global_var_array_p ;
static pli_function JEDA_link_veri_task_p ;
static pli_function JEDA_link_jd_task_p ;
static pli_function JEDA_link_static_var_p ;
static pli_function JEDA_link_static_var_array_p ;
static pli_function JEDA_link_create_top_p ;
static pli_function JEDA_link_create_main_p ;

#ifdef JEDA_STATIC_LINK

void JD_misctf_reason_reactivate() ;
void JD_misctf_reason_sync() ;
void JEDA_link_CLOCK() ;
void JEDA_link_port() ;
void JEDA_link_pset_port() ;
void JEDA_link_global_var() ;
void JEDA_link_global_var_array() ;
void JEDA_link_veri_task() ;
void JEDA_link_jd_task() ;
void JEDA_link_static_var() ;
void JEDA_link_static_var_array() ;
void JEDA_link_create_top() ;
void JEDA_link_create_main() ;

#endif

void JEDA_load_dynamic_lib() ;

void  JEDA_stop_here() {
  /* just null function to trigger the debugger after dl is loaded */
}


#ifndef JEDA_SELF_THREAD_CONTROL
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
#endif

int JEDA_task_misctf( int data, int reason ) {

  acc_initialize() ;
  
  if (reason == reason_reactivate )
    (*JD_misctf_reason_reactivate_p)() ;


  if ( reason ==reason_synch )
    (*JD_misctf_reason_sync_p)() ;
    
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  if ( reason == reason_finish ) {
    lp_checkin( JEDA_lp_handle ) ;
  }
#  endif
#endif
  
  acc_close() ;
    
}


int JEDA_task() {
  int index ;
  
  acc_initialize() ;
  
  index = tf_getp(1) ; /* first arg is the index to the task */
  switch( index ) {
    case JEDA_INIT:
      JEDA_load_dynamic_lib() ;
      break ;
    case JEDA_CLOCK:
      (*JEDA_link_CLOCK_p)() ;
      break ;
    case JEDA_LINK_PORT:
      (*JEDA_link_port_p)() ;
      break ;
    case JEDA_LINK_PSET_PORT:
      (*JEDA_link_pset_port_p)() ;
      break ;
    case JEDA_LINK_GLOBAL_VAR:
      (*JEDA_link_global_var_p)() ;
      break ;
    case JEDA_LINK_GLOBAL_VAR_ARRAY:
      (*JEDA_link_global_var_array_p)() ;
      break ;
    case JEDA_LINK_VERI_TASK:
      (*JEDA_link_veri_task_p)() ;
      break ;
    case JEDA_LINK_JD_TASK:
      (*JEDA_link_jd_task_p)() ;
      break ;
    case JEDA_LINK_STATIC_VAR:
      (*JEDA_link_static_var_p)() ;
      break ;
    case JEDA_LINK_STATIC_VAR_ARRAY:
      (*JEDA_link_static_var_array_p)() ;
      break ;
    case JEDA_LINK_CREATE_MAIN:
      (*JEDA_link_create_main_p)() ;
      break ;
  }
  acc_close() ;
}


/* JEDA_load_dynamic_lib(): 
 *   Loading dynamic lib as the runtime image
 */
void JEDA_load_dynamic_lib() 
{
  char *nm, *cwd, *dl_fname, *dname ;
  void *library ;
  char *dlerr ;
  
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  {
    char *licdir ;
    char *homedir = (char *)getenv( "JEDA_HOME" ) ;
    
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
  }
#  endif
#endif
  
#ifndef JEDA_STATIC_LINK
  dl_fname = (char *)mc_scan_plusargs( "jeda_code=" ) ;
  
  if( dl_fname == NULL ) {
    /* look for the d-lib from argument */
    nm = (char *)tf_getcstringp(2) ;
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
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }

  dlerror() ;
  
  JD_misctf_reason_reactivate_p = 
    dlsym(library, "JD_misctf_reason_reactivate" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JD_misctf_reason_reactivate\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JD_misctf_reason_sync_p = dlsym(library, "JD_misctf_reason_sync" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JD_misctf_reason_sync\" : %s\n", dlerror() 
    );
    exit(1) ;
  }
  JEDA_link_CLOCK_p = dlsym(library, "JEDA_link_CLOCK" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_CLOCK\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_port_p = dlsym(library, "JEDA_link_port" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_port\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_pset_port_p = dlsym(library, "JEDA_link_pset_port" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_pset_port\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_global_var_p = dlsym(library, "JEDA_link_global_var" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_global_var\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_global_var_array_p = dlsym(library, "JEDA_link_global_var_array" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_global_var_array\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_veri_task_p = dlsym(library, "JEDA_link_veri_task" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_veri_task\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_jd_task_p = dlsym(library, "JEDA_link_jd_task" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_jd_task\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_static_var_p = dlsym(library, "JEDA_link_static_var" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_static_var\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_static_var_array_p = dlsym(library, "JEDA_link_static_var_array" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_static_var_array\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_create_main_p = dlsym(library, "JEDA_link_create_main" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_create_main\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  JEDA_link_create_top_p = dlsym(library, "JEDA_link_create_top" ) ;
  dlerr = dlerror() ;
  if( dlerr ) {
    PRINTF( 
      "Jeda: Can't find \"JEDA_link_create_top\" : %s\n", dlerror() 
    );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }

#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  {
    check_function func_p ;
    int r ;
    
    r = (int)time(NULL) ;
    func_p = dlsym(library, "JD_top_check" ) ;
    dlerr = dlerror() ;
    if( dlerr ) {
      PRINTF( 
        "Jeda: Can't find \"JD_top_check\" : %s\n", dlerror() 
      );
      lp_checkin( JEDA_lp_handle ) ;
      exit(1) ;
    }
    (*func_p)( r, check_dsa_key( r, 0 ) ) ;
    func_p = dlsym(library, "JD_data_check" ) ;
    dlerr = dlerror() ;
    if( dlerr ) {
      PRINTF( 
        "Jeda: Can't find \"JD_data_check\" : %s\n", dlerror() 
      );
      lp_checkin( JEDA_lp_handle ) ;
      exit(1) ;
    }
    (*func_p)( r, check_dsa_key( r, 0 ) ) ;
    func_p = dlsym(library, "JD_thread_check" ) ;
    dlerr = dlerror() ;
    if( dlerr ) {
      PRINTF( 
        "Jeda: Can't find \"JD_thread_check\" : %s\n", dlerror() 
      );
      lp_checkin( JEDA_lp_handle ) ;
      exit(1) ;
    }
    (*func_p)( r, check_dsa_key( r, 0 ) ) ;
    func_p = dlsym(library, "JD_exec_check" ) ;
    dlerr = dlerror() ;
    if( dlerr ) {
      PRINTF( 
        "Jeda: Can't find \"JD_exec_check\" : %s\n", dlerror() 
      );
      lp_checkin( JEDA_lp_handle ) ;
      exit(1) ;
    }
    (*func_p)( r, check_dsa_key( r, 0 ) ) ;
    func_p = dlsym(library, "JD_pli_check" ) ;
    dlerr = dlerror() ;
    if( dlerr ) {
      PRINTF( 
        "Jeda: Can't find \"JD_pli_check\" : %s\n", dlerror() 
      );
      lp_checkin( JEDA_lp_handle ) ;
      exit(1) ;
    }
    (*func_p)( r, check_dsa_key( r, 0 ) ) ;
  }
#  endif
#endif

#else


  JD_misctf_reason_reactivate_p = JD_misctf_reason_reactivate ;
  JD_misctf_reason_sync_p = JD_misctf_reason_sync ;
  JEDA_link_CLOCK_p = JEDA_link_CLOCK ;
  JEDA_link_port_p = JEDA_link_port ;
  JEDA_link_pset_port_p = JEDA_link_pset_port ;
  JEDA_link_global_var_p = JEDA_link_global_var ;
  JEDA_link_global_var_array_p = JEDA_link_global_var_array ;
  JEDA_link_veri_task_p = JEDA_link_veri_task ;
  JEDA_link_jd_task_p = JEDA_link_jd_task ;
  JEDA_link_static_var_p = JEDA_link_static_var ;
  JEDA_link_static_var_array_p = JEDA_link_static_var_array ;
  JEDA_link_create_main_p = JEDA_link_create_main ;
  JEDA_link_create_top_p = JEDA_link_create_top ;
  {
    int r ;
    r = (int)time(0) ;
    JD_top_check( r, check_dsa_key( r, 0 ) ) ;
    JD_data_check( r, check_dsa_key( r, 0 ) ) ;
    JD_thread_check( r, check_dsa_key( r, 0 ) ) ;
    JD_exec_check( r, check_dsa_key( r, 0 ) ) ;
    JD_pli_check( r, check_dsa_key( r, 0 ) ) ;
  }
#endif
  
  JEDA_stop_here() ;
  
  (*JEDA_link_create_top_p)() ;
  
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
