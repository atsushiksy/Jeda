/*
 * Various code to support random generation functions
 */
/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
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
#include <math.h>
#include "runtime.h"
#include "runtime_error_msg.h"
#include "mem_macro.h"
#include "data.h"
#include "exec_macro.h"
#include "code_exec_macro.h"

#include "random.h"


/*********************************************************************
 *  rand48 family random function
 
 The rand48() family of functions generages pseudo-random numbers using
 a linear congruential algorithm working on integers 48 bits in size.
 The particular formula employed is r(n+1) = (a * r(n) + c) mod m where
 the default values are for the multiplicand a = 0xfdeece66d = 25214903917
 and the addend c = 0xb = 11. The modulus is always fixed at m = 2 ** 48.
 r(n) is called the seed of the random number generator.
 *********************************************************************/
 /* This is ported from rand48 library written by Martin Birgmeier.
    The code copied from the library is in rand48_port.h (with the
    original copyright info. and included here.
    The code is included to (hopefully) avoid possible name conflict and
    possible compatibility issue among the various systems.
  */

#include "rand48_port.h"

/***********************************************************************
 * Functions that support Jeda system functions
 ***********************************************************************/
 
static JD_48_random *alloc_48_random() {
  JD_48_random *rd ;
  
  rd = (JD_48_random *)calloc(1,sizeof(JD_48_random)) ;
  rd->seed[0] = RAND48_SEED_0 ;
  rd->seed[1] = RAND48_SEED_1 ;
  rd->seed[2] = RAND48_SEED_2 ;
  return (rd) ;
  
}

static void init_48_random( int seed, JD_48_random *rd ) {
  rd->seed[0] = seed & 0xffff ;
  rd->seed[1] = (unsigned)seed>>16 ;
  rd->seed[2] = 0 ;
}

static JD_48_random *a_48_random_data = NULL ;

void JD_sf_rand48()
{
  JD_data *dt ;
  int seed ;
  
  if( a_48_random_data == NULL ) {
    a_48_random_data = alloc_48_random() ;
  }
  
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    init_48_random(seed, a_48_random_data) ;
    
  }
  
  my_dorand48(a_48_random_data->seed) ;
  
  ACC_INT = 
    ((long) a_48_random_data->seed[2] << 15) + 
    ((long) a_48_random_data->seed[1] >> 1) ;
  ACC_INT &= 0x7fffffff ; 
  ACC_TYPE = JD_D_int ;
}

void JD_sf_urand48()
{
  JD_data *dt ;
  int seed ;
  
  if( a_48_random_data == NULL ) {
    a_48_random_data = alloc_48_random() ;
  }
  
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    if( seed != 0 )
      init_48_random(seed, a_48_random_data) ;
    
  }
  
  my_dorand48(a_48_random_data->seed) ;
  
  ACC_INT = 
    ((long) a_48_random_data->seed[2] << 15) + 
    ((long) a_48_random_data->seed[1] >> 1) ;
  ACC_TYPE = JD_D_int ;
}

/*************************************************************
 * class random_48                                           *
 *************************************************************/
/* allocate random_48 */
void *JD_sa_random_48() {
  JD_48_random *rd ;
  
  rd = alloc_48_random() ;
  return(rd) ;
  
}

char *JD_disp_random_48( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_48_random *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "%sseed[0]=%d, seed[1]=%d, seed[2]=%d%s",
    indent, rd->seed[0], rd->seed[1], rd->seed[2], eol
  ) ;
  return ret ;
}

void JD_sc_random_48_mf_new() {
  JD_data *dt ;
  int seed ;
  JD_48_random *rd ;
  
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;

    if( seed != 0 )
      init_48_random(seed, rd) ;
  }
  
}

/* deallocate random_ucb */
void JD_sd_random_48(JD_48_random *rd) {
  if( rd ) free(rd) ;
}

void *JD_dup_random_48(JD_48_random *rd) {
  JD_48_random *nw ;
  int i ;
  
  nw = alloc_48_random() ;
  
  nw->seed[0] = rd->seed[0] ;
  nw->seed[1] = rd->seed[1] ;
  nw->seed[2] = rd->seed[2] ;
  
  return(nw) ;
}

void JD_sc_random_48_mf_random()
{
  JD_48_random *rd ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  my_dorand48(rd->seed) ;
  
  ACC_INT = 
    ((long) rd->seed[2] << 15) + 
    ((long) rd->seed[1] >> 1) ;
  ACC_INT &= 0x7fffffff ; 
  ACC_TYPE = JD_D_int ;
  
}

void JD_sc_random_48_mf_urandom()
{
  JD_48_random *rd ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  my_dorand48(rd->seed) ;
  
  ACC_INT = 
    ((long) rd->seed[2] << 15) + 
    ((long) rd->seed[1] >> 1) ;
  ACC_TYPE = JD_D_int ;
  
}


/*********************************************************************
 *  Mersenne Twister random function (Of period of 2**19937-1)
 *   developped by Makoto Matsumoto and Takuji Nishimura.
 *  Refer to the websuite for futher information.
 *  http://www.math.keio.ac.jp/~matumoto/emt.html
 *********************************************************************/


#include "mt19937_port.h"

/***********************************************************************
 * Functions that support Jeda system functions
 ***********************************************************************/

static JD_mt_random *a_mt_random_data = NULL ;

/*************************************************************************
 *  Jeda uses MT as random() function, not BSD random() as BSD has
 *   initialization problem that generates same bit pattern in specific
 *   bit field at the beginning of the random sequence.
 *************************************************************************/
void JD_sf_random()
{
  JD_data *dt ;
  int seed ;
  
  if( a_mt_random_data == NULL ) {
    a_mt_random_data = alloc_mt_random() ;
    sgenrand(4357, a_mt_random_data); 
      /* any nonzero integer can be used as a seed */
  }
  
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    /* sgenrand(seed, a_mt_random_data) ; */
    init_genrand(seed, a_mt_random_data) ;
    
  }
    
  ACC_INT = genrand(a_mt_random_data) ;
  ACC_INT &= 0x7fffffff ; 
  ACC_TYPE = JD_D_int ;
}

void JD_sf_urandom()
{
  JD_data *dt ;
  int seed ;
  
  if( a_mt_random_data == NULL ) {
    a_mt_random_data = alloc_mt_random() ;
    sgenrand(4357, a_mt_random_data); 
      /* any nonzero integer can be used as a seed */
  }
  
  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }

    /* sgenrand(seed, a_mt_random_data) ; */
    init_genrand(seed, a_mt_random_data) ;
    
  }
    
  ACC_INT = genrand(a_mt_random_data) ;
  ACC_TYPE = JD_D_int ;
}

/*************************************************************
 * class random_mt                                          *
 *************************************************************/
/* allocate random_mt */
void *JD_sa_random_mt() {
  JD_mt_random *rd ;
  
  rd = alloc_mt_random() ;
  sgenrand(4357, rd); 
  
  return(rd) ;
  
}

char *JD_disp_random_mt( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_mt_random *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "%smti = %d, mt[0]=%d, mt[1]=%d, mt[2]=%d .. mt[623]=%d%s",
    indent, rd->mti, rd->mt[0], rd->mt[1], rd->mt[2], rd->mt[623], eol
  ) ;
  return ret ;
}

void JD_sc_random_mt_mf_new() {
  JD_data *dt ;
  int seed ;
  JD_mt_random *rd ;

  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;

    if( seed != 0 ) {
      /* sgenrand(seed, rd) ; */
      init_genrand(seed, rd) ;
    }
  }
  
}

/* deallocate random_mt */
void JD_sd_random_mt(JD_mt_random *rd) {
  if( rd ) free(rd) ;
}

void *JD_dup_random_mt(JD_mt_random *rd) {
  JD_mt_random *nw ;
  int i ;
  
  nw = alloc_mt_random() ;
  
  for( i = 0 ; i < N ; i++ )
    nw->mt[i] = rd->mt[i] ;
  
  nw->mti = rd->mti ;
  
  return(nw) ;
}

void JD_sc_random_mt_mf_random()
{
  JD_mt_random *rd ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  
  ACC_INT = genrand(rd) ;
  ACC_INT &= 0x7fffffff ; 
  ACC_TYPE = JD_D_int ;
  
}

void JD_sc_random_mt_mf_urandom()
{
  JD_mt_random *rd ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  
  ACC_INT = genrand(rd) ;
  ACC_TYPE = JD_D_int ;
  
}

/*********************************************************************/
/* other random distributions implemented with double mt             */
/*********************************************************************/

/*************************************************************
 * class geom_random                                          *
 *************************************************************/
/* allocate geom_random */
void *JD_sa_geom_random() {
  JD_mt_drandom *rd ;
  
  rd = alloc_mt_drandom() ;
  dsgenrand(4357, rd) ;
  
  return(rd) ;
  
}

char *JD_disp_geom_random( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_mt_drandom *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "%sp = %f, mti = %d, mt[0]=%d, mt[1]=%d, mt[2]=%d .. mt[623]=%d%s",
    indent, rd->p, 
    rd->mti, rd->mt[0], rd->mt[1], rd->mt[2], rd->mt[623], rd->p, eol
  ) ;
  return ret ;
}

void JD_sc_geom_random_mf_new() {
  JD_data *dt ;
  unsigned long seed ;
  double p, p_numerator, p_denominator ;
  JD_mt_drandom *rd ;

  if( NUM_ARG >= 2 ) {
    dt = GET_STACK(NUM_ARG) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    p_numerator = (double)seed ;
    dt = GET_STACK(NUM_ARG-1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    p_denominator = (double)seed ;
    if( ACC_DT->data.obj.ect == NULL ) {
      ERROR_EMPTY_OBJECT_ACCESS ;
    }
    p = p_numerator/p_denominator ;
    rd =  (void *)ACC_DT->data.obj.ect->table ;
    rd->p = p ;  /* this is actually p, which is 1/mean */
  }
  if( NUM_ARG == 3 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;
        
    if( seed != 0 ) {
      /* dsgenrand(seed, rd) ; */
      init_dgenrand(seed, rd) ;
    }
  }
  
}

/* deallocate geom_random */
void JD_sd_geom_random(JD_mt_drandom *rd) {
  if( rd ) free(rd) ;
}

void *JD_dup_geom_random(JD_mt_drandom *rd) {
  JD_mt_drandom *nw ;
  int i ;
  
  nw = alloc_mt_drandom() ;
  
  for( i = 0 ; i < N ; i++ )
    nw->mt[i] = rd->mt[i] ;
  
  nw->mti = rd->mti ;
  nw->p = rd->p ;
  
  return(nw) ;
}

void JD_sc_geom_random_mf_random()
{
  JD_data *dt ;
  JD_mt_drandom *rd ;
  int ret ;
  double p ;
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  if( rd == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  p = rd->p ; /* stored in here, but actually not mean, p */
  ret = ceil(log(dgenrand(rd))/log(1.0-p));
  ACC_INT = ret ;
  ACC_TYPE = JD_D_int ;
  
}

/*************************************************************
 * class exp_random                                          *
 *************************************************************/
/* allocate exp_random */
void *JD_sa_exp_random() {
  JD_mt_drandom *rd ;
  
  rd = alloc_mt_drandom() ;
  dsgenrand(4357, rd) ;
  
  return(rd) ;
  
}

char *JD_disp_exp_random( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_mt_drandom *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "mean = %d, mti = %d, mt[0]=%d, mt[1]=%d, mt[2]=%d .. mt[623]=%d%s",
    rd->mean, rd->mti, rd->mt[0], rd->mt[1], rd->mt[2], rd->mt[623], rd->p, eol
  ) ;
  return ret ;
}

void JD_sc_exp_random_mf_new() {
  JD_data *dt ;
  unsigned long seed ;
  int mean ;
  JD_mt_drandom *rd ;

  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      mean = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      mean = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;
    if( rd ) rd->mean = mean ;
  }
  else if( NUM_ARG == 2 ) {
    dt = GET_STACK(2) ;
    if( IS_SMLINT(dt) ) {
      mean = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      mean = dt->data.int_value ;
    }
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
  
    rd = (void *)ACC_DT->data.obj.ect->table ;
  
    if( rd ) {
      rd->mean = mean ;
    
    
      if( seed != 0 ) {
        /* dsgenrand(seed, rd) ; */
        init_dgenrand(seed, rd) ;
      }
    }
  }
}

/* deallocate exp_random */
void JD_sd_exp_random(JD_mt_drandom *rd) {
  if( rd ) free(rd) ;
}

void *JD_dup_exp_random(JD_mt_drandom *rd) {
  JD_mt_drandom *nw ;
  int i ;
  
  nw = alloc_mt_drandom() ;
  
  for( i = 0 ; i < N ; i++ )
    nw->mt[i] = rd->mt[i] ;
  
  nw->mti = rd->mti ;
  nw->mean = rd->mean ;
  
  return(nw) ;
}

void JD_sc_exp_random_mf_random()
{
  JD_mt_drandom *rd ;
  double ret ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  ret = ceil(-log(dgenrand(rd))*rd->mean) ;
  ACC_INT = ret ;
  ACC_INT &= 0x7fffffff ; 
  ACC_TYPE = JD_D_int ;
  
}

/*************************************************************
 * class pdf_random                                          *
 *************************************************************/
/* allocate pdf_random */
void *JD_sa_pdf_random() {
  JD_mt_drandom *rd ;
  
  rd = alloc_mt_drandom() ;
  dsgenrand(4357, rd) ;
  
  rd->waits = (double *)calloc(DEFAULT_PDF_SIZE, sizeof(double) ) ;
  rd->values = (int *)calloc(DEFAULT_PDF_SIZE, sizeof(int) ) ;
  rd->size = DEFAULT_PDF_SIZE ;
  rd->n = 0 ;
  rd->sum = 0.0 ;
  return(rd) ;
  
}

char *JD_disp_pdf_random( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_mt_drandom *rd ;
  int i ;
  char buf[100] ;
  rd = (void *)dt->data.obj.ect->table ;
  ret = (char *)calloc(strlen(indent)+(rd->n*100)+100,sizeof(char) ) ;
  sprintf( ret, "%s sum = %f pdf: ", indent, rd->sum ) ;
  for( i = 0 ; i < rd->n ; i++ ) {
    sprintf( buf, "(%f %d) ", rd->waits[i], rd->values[i] ) ;
    strcat( ret, buf ) ;
  }
  sprintf(
    buf, " mti = %d, mt[0]=%d, mt[1]=%d, mt[2]=%d .. mt[623]=%d%s",
    rd->mean, rd->mti, rd->mt[0], rd->mt[1], rd->mt[2], rd->mt[623], rd->p, eol
  ) ;
  strcat( ret, buf ) ;
  return ret ;
}

void JD_sc_pdf_random_mf_new() {
  JD_data *dt ;
  unsigned long seed ;
  JD_mt_drandom *rd ;

  if( NUM_ARG == 1 ) {
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      seed = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      seed = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;
    
    if( seed != 0 ) {
      /* dsgenrand(seed, rd) ; */
      init_dgenrand(seed, rd) ;
    }
  }
  
}

/* deallocate pdf_random */
void JD_sd_pdf_random(JD_mt_drandom *rd) {
  if( rd ) {
    if( rd->waits ) free(rd->waits) ;
    if( rd->values ) free(rd->values) ;
    free(rd) ;
  }
}

void *JD_dup_pdf_random(JD_mt_drandom *rd) {
  JD_mt_drandom *nw ;
  int i ;
  
  nw = alloc_mt_drandom() ;
  nw->waits = (double *)calloc(rd->size, sizeof(double) ) ;
  nw->values = (int *)calloc(rd->size, sizeof(int) ) ;
  nw->size = rd->size ;
  
  for( i = 0 ; i < N ; i++ )
    nw->mt[i] = rd->mt[i] ;
  
  nw->mti = rd->mti ;
  for( i = 0 ; i < rd->n ; i++ ) {
    nw->waits[i] = rd->waits[i] ;
    nw->values[i] = rd->values[i] ;
  }
  nw->n = rd->sum ;
  nw->sum = rd->sum ;
  return(nw) ;
}

void JD_sc_pdf_random_mf_set() {
  JD_data *dt ;
  double prob, tp ; 
  int value, vp ;
  JD_mt_drandom *rd ;
  

  if( NUM_ARG == 2 ) {
    dt = GET_STACK(2) ;
    if( IS_SMLINT(dt) ) {
      prob = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      prob = dt->data.int_value ;
    }
    dt = GET_STACK(1) ;
    if( IS_SMLINT(dt) ) {
      value = SMLINT2INT(dt) ;  
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_RANDOM_SEED ;
      }
      value = dt->data.int_value ;
    }
    rd = (void *)ACC_DT->data.obj.ect->table ;
    
    if( rd ) {
      rd->sum += prob ;
    
      if( rd->n == rd->size ) {
        rd->size += DEFAULT_PDF_SIZE ;
        rd->waits = (double *)realloc(rd->waits, rd->size*sizeof(double) ) ;
        rd->values = (int *)realloc(rd->values, rd->size*sizeof(int) ) ;
      }
      rd->waits[rd->n] = rd->sum ; /* keeps the sum as the wait */
      rd->values[rd->n++] = value ;
    }
  }
  
}

void JD_sc_pdf_random_mf_random()
{
  JD_mt_drandom *rd ;
  double r ;
  int i, value ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  r = dgenrand(rd) * rd->sum ;
  /* search the one that is landed, it may be slow X-( */
  for( i = 0 ; i < rd->n ; i++ ) {
    value = rd->values[i] ;
    if( r < rd->waits[i] ) break ;
  }
  ACC_INT = value ;
  ACC_TYPE = JD_D_int ;
  
}

#include "static_rnd.h"

/*
  static_random_data is used to initialize the individual random sequences

3. Initialization of the static random sequences.
  The initialization (seed) of the static random sequences are done at the 
  beginning of the simulation by the internal random function. Thus, each 
  random sequence has different seed value at the beginning. 
  The seed of the internal random function is normally a constant,
  so the random sequences will be the same under the same condition.
  This internal random function can be initialized with the plus argument:
  
    1) +JEDA_RANDOMIZE  
      This plus argument will randomize the internal sequence with a number
      calculated from time and the process number. This mechanism is useful
      to run random regression.
      
    2) +JEDA_RND_SEED=<number>
      This plus argument set <number> as the seed of the internal random 
      function. This is useful to run the simulation under the same seed
      value.
      
   Here's the pseudo code for the initialization:
   
   {
     string seedstr ;
     int seed ;
     random_mt init_rnd ;
     
     if( get_plusarg( "JEDA_RANDOMIZE" ) ) {
       seed = generate_random_value_from_time_and_process_id() ;
     }
     seedstr = get_plusarg( "JEDA_RND_SEED" ) ;
     if( seedstr != null ) {
       seed = atoi( seedstr ) ;
     }
     init_rnd = new( seed ) ;
     
     for( i = 0 ; i < total_number_of_rand ; i++ ) {
       rnd_array[i].initialize( init_rnd.random() ) ;
     }  
   }

*/

static JD_mt_random *static_random_data = NULL ;

void JD_static_random_init() {
  int seed ;
  char *rtn ;
  static_random_data = alloc_mt_random() ;
  sgenrand(4357, static_random_data); 
  rtn = (char *)SCAN_PLUSARGS( "JEDA_RND_SEED=" ) ;
  if( rtn ) {
    seed = atoi(rtn) ;
    init_genrand(seed, static_random_data) ;
    PRINTF( "Jeda RND/CYC seed = %d\n", seed ) ;
  }
  else {
    rtn = (char *)SCAN_PLUSARGS( "JEDA_RANDOMIZ" ) ;
    if( rtn && !strcmp( rtn, "E" ) ) {
      seed = time(0) + getpid() ;
      init_genrand(seed, static_random_data) ;
      PRINTF( "Jeda RND/CYC seed = %d\n", seed ) ;
    }
  }
}


/*************************************************************
 * class sys_RND                                             *
 *************************************************************/
/* allocate sys_RND */
void *JD_sa_sys_RND() {
  JD_static_random *rd ;
  
  rd = (void *)calloc(1,sizeof(JD_static_random)) ;
  rd->seed = genrand(static_random_data) ;
  return(rd) ;
  
}

char *JD_disp_sys_RND( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_static_random *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "%sRND size=%d, limit=%d, width=%d%s",
    indent, rd->size, rd->limit, rd->width, eol
  ) ;
  return ret ;
}

void JD_sc_sys_RND_mf_new() {
  JD_data *dt ;
  int tmp ;
  JD_static_random *rd ;

  rd = (void *)ACC_DT->data.obj.ect->table ;

  if( !(NUM_ARG == 1) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) {
    tmp = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    tmp = dt->data.int_value ;
  }
  rd->width = tmp ;
  rd->rnd = alloc_mt_random() ;
  init_genrand(rd->seed, rd->rnd) ;
}

/* deallocate random_mt */
void JD_sd_sys_RND(JD_static_random *rd) {
  JD_static_rnd_range *rng, *nrng ;
  if( rd ) {
    if( rd->rnd ) free( rd->rnd ) ;
    rng = rd->range ;
    while( rng ) {
      nrng = rng->next ;
      if( rng->u_data ) free( rng->u_data ) ;
      if( rng->l_data ) free( rng->l_data ) ;
      free( rng ) ;
      rng = nrng ;
    }
    free(rd) ;
  }
}

void *JD_dup_sys_RND(JD_static_random *rd) {
  JD_static_random *nw ;
  int i ;
  
  ERROR_ILLEGAL_DUP_CALL_FOR_SYS_RND ;
  
  return(NULL) ;
}

void JD_sc_sys_RND_mf_set_limit()
{
  JD_data *dt ;
  int tmp ;
  JD_static_random *rd ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  if( !(NUM_ARG == 1) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) {
    tmp = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    tmp = dt->data.int_value ;
  }
  
  rd->limit = tmp ;
  
}

void JD_sc_sys_RND_mf_set_range_d()
{
  JD_data *dt ;
  int i ;
  JD_static_random *rd ;
  JD_static_rnd_range *rng, *prng ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  rng = (void *)calloc(1,sizeof(JD_static_rnd_range)) ;
  
  if( !(NUM_ARG == 5) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(5) ;     /* ub */
  if( IS_SMLINT(dt) ) {
    rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->u_size = 32 ;
    rng->u_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->u_size = 32 ;
      rng->u_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->u_size = dt->data.sbit.size ;
        rng->u_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->u_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->u_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
  
  dt = GET_STACK(4) ;     /* lb */
  if( IS_SMLINT(dt) ) {
    rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->l_size = 32 ;
    rng->l_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->l_size = 32 ;
      rng->l_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->l_size = dt->data.sbit.size ;
        rng->l_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->l_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        rng->l_size = dt->data.bit.size ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->l_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
  
  dt = GET_STACK(3) ;  /* num_element */
  if( IS_SMLINT(dt) ) {
    rng->num_element = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->num_element = dt->data.int_value ;
  }
  
  dt = GET_STACK(2) ;  /* weight */
  if( IS_SMLINT(dt) ) {
    rng->weight = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->weight = dt->data.int_value ;
  }
  
  dt = GET_STACK(1) ;  /* flag */
  if( IS_SMLINT(dt) ) {
    rng->flag = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->flag = dt->data.int_value ;
  }
  
  switch( rng->flag ) {
    case 0:
      rng->weight_s = rd->num_element ;
      rd->num_element += rng->num_element ;
      rng->weight_e = rd->num_element - 1 ;
      break ;
    case 1:
      rng->weight_s = rd->num_element ;
      rd->num_element += ( rng->num_element * rng->weight ) ;
      rng->weight_e = rd->num_element - 1 ;
      break ;
    case 2:
      rng->weight_s = rd->num_element ;
      rd->num_element += (rng->num_element * rng->num_element * rng->weight) ;
      rng->weight_e = rd->num_element - 1 ;
      break ;
  }
  if( rd->num_element < 0 ) {
    ERROR_WEIGHT_TIMES_NUM_ELEMENT_OVERFLOW ;
  }
  if( rd->range ) {
    prng = rd->range ;
    while( prng->next ) prng = prng->next; 
    prng->next = rng ;
  }
  else rd->range = rng ;
  
}

void JD_sc_sys_RND_mf_set_range_s()
{
  JD_data *dt ;
  int i ;
  JD_static_random *rd ;
  JD_static_rnd_range *rng, *prng ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  rng = (void *)calloc(1,sizeof(JD_static_rnd_range)) ;
  
  if( !(NUM_ARG == 3) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(3) ;     /* ub */
  if( IS_SMLINT(dt) ) {
    rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->u_size = 32 ;
    rng->u_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->u_size = 32 ;
      rng->u_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->u_size = dt->data.sbit.size ;
        rng->u_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->u_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        rng->u_size = dt->data.bit.size ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->u_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
    
  rng->num_element = 1 ;
  
  dt = GET_STACK(2) ;  /* weight */
  if( IS_SMLINT(dt) ) {
    rng->weight = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->weight = dt->data.int_value ;
  }
  
  dt = GET_STACK(1) ;  /* flag */
  if( IS_SMLINT(dt) ) {
    rng->flag = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->flag = dt->data.int_value ;
  }
  
  switch( rng->flag ) {
    case 0:
      rng->weight_s = rng->weight_e = rd->num_element ;
      rd->num_element += 1 ;
      break ;
    case 1:
      rng->weight_s = rd->num_element ;
      rd->num_element +=  rng->weight ;
      rng->weight_e = rd->num_element - 1 ;
      break ;
  }
  if( rd->num_element < 0 ) {
    ERROR_WEIGHT_TIMES_NUM_ELEMENT_OVERFLOW ;
  }
  if( rd->range ) {
    prng = rd->range ;
    while( prng->next ) prng = prng->next; 
    prng->next = rng ;
  }
  else rd->range = rng ;
  
}

static void set_acc_value(
  JD_static_random *rd, JD_static_rnd_range *rng, int rnd
)
{
  int i ;
  unsigned int n, t, p, mask ;
  n = rnd - rng->weight_s ;
  switch( rng->flag ) {
    case 0: 
      break ;
    case 1:
      n /= rng->weight ;
      break ;
    case 2:
      n /= rng->weight ;
      n /= rng->num_element ;
      break ;
  }
  if( rd->width == -1 ) {
    ACC_INT = rng->u_data[0] + n ; 
    ACC_TYPE = JD_D_int ;
  }
  else {
    for( i = 0 ; i < NWORD(rd->width) ; i++ ) {
      if( i == NWORD(rd->width)-1 ) {
        mask = BITMASK(rd->width) ;
      }
      else {
        mask = 0xffffffff ;
      }
      if( i < NWORD(rng->u_size) ) {
        t = p = rng->u_data[i] ;
      }
      else {
        t = p = 0 ;
      }
      t += n ;
      ACC_A[i] = ( t & mask );
      if( t < p ) n = 1 ;
      else n = 0 ;
      ACC_B[i] = 0 ;
    }
    ACC_SIZE = rd->width ;
    ACC_TYPE = JD_D_bit ;
  }
}

static int eval_cond( int cond, JD_data *dt ) {
  int *a1, *b1, size1 ;
  int *a2, *b2, size2 ;
  int zero = 0 ;
  int ret ;
  if( ACC_TYPE == JD_D_int ) {
    a1 = &ACC_INT ;
    b1 = &zero ;
    size1 = 32 ;
  }
  else {
    a1 = ACC_A ;
    b1 = ACC_B ;
    size1 = ACC_SIZE ;
  }
  switch( dt->type ) {
    case JD_D_int:
      a2 = &dt->data.int_value ;
      b2 = &zero ;
      size2 = 32 ;
      break ;
    case JD_D_sbit:
      a2 = &dt->data.sbit.a ;
      b2 = &dt->data.sbit.b ;
      size2 = dt->data.sbit.size ;
      break ;
    case JD_D_bit:
      a2 = dt->data.bit.a ;
      b2 = dt->data.bit.b ;
      size2 = dt->data.bit.size ;
      break ;
  }
  switch( cond ) {
    case 1:  /* < */
      ret = JD_bit_lt_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 2:  /* > */
      ret = JD_bit_gt_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 3:  /* <= */
      ret = JD_bit_le_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 4:  /* >= */
      ret = JD_bit_ge_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 5:  /* != */
      ret = JD_bit_ne_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 6:  /* =?= */
      ret = JD_bit_eqQeq_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
    case 7:  /* !?= */
      ret = JD_bit_Qne_bit( a1, b1, size1, a2, b2, size2 ) ;
      break ;
  }
  return ret ;
}

static int check_cond() {
  JD_data *dt ;
  JD_data tmp ;
  int i ;
  int cond ;
  if( NUM_ARG == 0 ) return 1 ;
  for( i = NUM_ARG ; i > 0 ; ) {
    dt = GET_STACK(i--) ;  /* flag */
    if( IS_SMLINT(dt) ) {
      cond = SMLINT2INT(dt) ;
    }
    else {
      if( dt->type != JD_D_int ) {
        ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
      }
      cond = dt->data.int_value ;
    }
    dt = GET_STACK(i--) ;  /* flag */
    if( IS_SMLINT(dt) ) {
      tmp.data.int_value = SMLINT2INT(dt) ;
      tmp.type = JD_D_int ;
      dt = &tmp ;
    }
    if( !eval_cond( cond, dt ) ) return 0 ;
  }
  return 1 ;
}

void JD_sc_sys_RND_mf_gen_random()
{
  JD_data *dt ;
  int rnd ;
  int loop ;
  int limit ;
  JD_static_random *rd ;
  JD_static_rnd_range *range ;
  int i, mask ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  if( rd->num_element ) {
    loop = 1 ;
    if( rd->limit ) limit = rd->limit ;
    else limit = 0 ;
    while( loop  ) {
      rnd = (genrand(rd->rnd) % rd->num_element) ;
      range = rd->range ;
      while( range ) {
        if( rnd >= range->weight_s && rnd <= range->weight_e ) {
          set_acc_value( rd, range, rnd ) ;
          if( check_cond() ) return ;
        }
        range = range->next ;
      }
      loop++ ;
      if( limit ) {
        limit-- ;
        if( limit == 0 ) loop = 0 ;
      }
    }
    ERROR_RND_LIMIT_OVERFLOW ;
  }
  else {
    loop = 1 ;
    if( rd->limit ) limit = rd->limit ;
    else limit = 0 ;
    while( loop ) {
      if( rd->width == -1 ) {
        ACC_INT = genrand(rd->rnd) & 0x7fffffff ; 
        ACC_TYPE = JD_D_int ;
      }
      else {
        for( i = 0 ; i < NWORD(rd->width) ; i++ ) {
          if( i == NWORD(rd->width)-1 ) mask = BITMASK(rd->width) ;
          else mask = 0xffffffff ;
          ACC_A[i] = ( genrand(rd->rnd) & mask );
          ACC_B[i] = 0 ;
        }
        ACC_SIZE = rd->width ;
        ACC_TYPE = JD_D_bit ;
      }
      if( check_cond() ) return ;
      loop++ ;
      if( limit ) {
        limit-- ;
        if( limit == 0 ) loop = 0 ;
      }
    }
    ERROR_RND_LIMIT_OVERFLOW ;
  }
}


/*************************************************************
 * class sys_CYC                                             *
 *************************************************************/
#define B0 0x00000001
#define B1 0x00000002
#define B2 0x00000004
#define B3 0x00000008
#define B4 0x00000010
#define B5 0x00000020
#define B6 0x00000040
#define B7 0x00000080
#define B8 0x00000100

/* primitive polinimials modulo 2 */
static int primpoli[] =
{
   B0,                                            /*  1, 0 */
  (B1),                                     /*  2, 1, 0 */
  (B1),                                     /*  3, 1, 0 */
  (B1),                                     /*  4, 1, 0 */
  (B2),                                     /*  5, 2, 0 */
  (B1),                                     /*  6, 1, 0 */
  (B1),                                     /*  7, 1, 0 */
  (B4 | B3 | B2),                           /*  8, 4, 3, 2, 0 */
  (B4),                                     /*  9, 4, 0 */
  (B3),                                     /* 10, 3, 0 */
  (B2),                                     /* 11, 2, 0 */
  (B6 | B4 | B1),                           /* 12, 6, 4, 1, 0 */
  (B4 | B3 | B1),                           /* 13, 4, 3, 1, 0 */
  (B5 | B3 | B1),                           /* 14, 5, 3, 1, 0 */
  (B1),                                     /* 15, 1, 0 */
  (B5 | B3 | B2),                           /* 16, 5, 3, 2, 0 */
  (B3),                                     /* 17, 3, 0 */
  (B5 | B2 | B1),                           /* 18, 5, 2, 1, 0 */
  (B5 | B2 | B1),                           /* 19, 5, 2, 1, 0 */
  (B3),                                     /* 20, 3, 0 */
  (B2),                                     /* 21, 2, 0 */
  (B1),                                     /* 22, 1, 0 */
  (B5),                                     /* 23, 5, 0 */
  (B4 | B3 | B1),                           /* 24, 4, 3, 1, 0 */
  (B3),                                     /* 25, 3, 0 */
  (B6 | B2 | B1),                           /* 26, 6, 2, 1, 0 */
  (B5 | B2 | B1),                           /* 27, 5, 2, 1, 0 */
  (B3),                                     /* 28, 3, 0 */
  (B2),                                     /* 29, 2, 0 */
  (B6 | B4 | B1),                           /* 30, 6, 4, 1, 0 */
  (B3),                                     /* 31, 3, 0 */
  (B7 | B5 | B3 | B2 | B1),                 /* 32, 7, 5, 3, 2, 1, 0 */
  (B6 | B4 | B1),                           /* 33, 6, 4, 1, 0 */
  (B7 | B6 | B5 | B2 | B1),                 /* 34, 7, 6, 5, 2, 1, 0 */
  (B2),                                     /* 35, 2, 0 */
  (B6 | B5 | B4 | B2 | B1),                 /* 36, 6, 5, 4, 2, 1, 0 */
  (B5 | B4 | B3 | B2 | B1),                 /* 37, 5, 4, 3, 2, 1, 0 */
  (B5 | B4 | B1),                           /* 38, 5, 4, 1, 0 */
  (B4),                                     /* 39, 4, 0 */
  (B5 | B4 | B3),                           /* 40, 5, 4, 3, 0 */
  (B3),                                     /* 41, 3, 0 */
  (B5 | B4 | B3 | B2 | B1),                 /* 42, 5, 4, 3, 2, 1, 0 */
  (B6 | B4 | B3),                           /* 43, 6, 4, 3, 0 */
  (B6 | B5 | B2),                           /* 44, 6, 5, 2, 0 */
  (B4 | B3 | B1),                           /* 45, 4, 3, 1, 0 */
  (B8 | B5 | B3 | B2 | B1),                 /* 46, 8, 5, 3, 2, 1, 0 */
  (B5),                                     /* 47, 5, 0 */
  (B7 | B5 | B4 | B2 | B1),                 /* 48, 7, 5, 4, 2, 1, 0 */
  (B6 | B5 | B4),                           /* 49, 6, 5, 4, 0 */
  (B4 | B3 | B2),                           /* 50, 4, 3, 2, 0 */
  (B6 | B3 | B1),                           /* 51, 6, 3, 1, 0 */
  (B3),                                     /* 52, 3, 0 */
  (B6 | B2 | B1),                           /* 53, 6, 2, 1, 0 */
  (B6 | B5 | B4 | B3 | B2),                 /* 54, 6, 5, 4, 3, 2, 0 */
  (B6 | B2 | B1),                           /* 55, 6, 2, 1, 0 */
  (B7 | B4 | B2),                           /* 56, 7, 4, 2, 0 */
  (B5 | B3 | B2),                           /* 57, 5, 3, 2, 0 */
  (B6 | B5 | B1),                           /* 58, 6, 5, 1, 0 */
  (B6 | B5 | B4 | B3 | B1),                 /* 59, 6, 5, 4, 3, 1, 0 */
  (B1),                                     /* 60, 1, 0 */
  (B5 | B2 | B1),                           /* 61, 5, 2, 1, 0 */
  (B6 | B5 | B3),                           /* 62, 6, 5, 3, 0 */
  (B1),                                     /* 63, 1, 0 */
  (B4 | B3 | B1),                           /* 64, 4, 3, 1, 0 */
  (B4 | B3 | B1),                           /* 65, 4, 3, 1, 0 */
  (B8 | B6 | B5 | B3 | B2),                 /* 66, 8, 6, 5, 3, 2, 0 */
  (B5 | B2 | B1),                           /* 67, 5, 2, 1, 0 */
  (B7 | B5 | B1),                           /* 68, 7, 5, 1, 0 */
  (B6 | B5 | B2),                           /* 69, 6, 5, 2, 0 */
  (B5 | B3 | B1),                           /* 70, 5, 3, 1, 0 */
  (B5 | B3 | B1),                           /* 71, 5, 3, 1, 0 */
  (B6 | B4 | B3 | B2 | B1),                 /* 72, 6, 4, 3, 2, 1, 0 */
  (B4 | B3 | B2),                           /* 73, 4, 3, 2, 0 */
  (B7 | B4 | B3),                           /* 74, 7, 4, 3, 0 */
  (B6 | B3 | B1),                           /* 75, 6, 3, 1, 0 */
  (B5 | B4 | B2),                           /* 76, 5, 4, 2, 0 */
  (B6 | B5 | B2),                           /* 77, 6, 5, 2, 0 */
  (B7 | B2 | B1),                           /* 78, 7, 2, 1, 0 */
  (B4 | B3 | B2),                           /* 79, 4, 3, 2, 0 */
  (B7 | B5 | B3 | B2 | B1),                 /* 80, 7, 5, 3, 2, 1, 0 */
  (B4),                                     /* 81, 4, 0 */
  (B8 | B7 | B6 | B4 | B1),                 /* 82, 8, 7, 6, 4, 1, 0 */
  (B7 | B4 | B2),                           /* 83, 7, 4, 2, 0 */
  (B8 | B7 | B5 | B3 | B1),                 /* 84, 8, 7, 5, 3, 1, 0 */
  (B8 | B2 | B1),                           /* 85, 8, 2, 1, 0 */
  (B6 | B5 | B2),                           /* 86, 6, 5, 2, 0 */
  (B7 | B5 | B1),                           /* 87, 7, 5, 1, 0 */
  (B8 | B5 | B4 | B3 | B1),                 /* 88, 8, 5, 4, 3, 1, 0 */
  (B6 | B5 | B3),                           /* 89, 6, 5, 3, 0 */
  (B5 | B3 | B2),                           /* 90, 5, 3, 2, 0 */
  (B7 | B6 | B5 | B3 | B2),                 /* 91, 7, 6, 5, 3, 2, 0 */
  (B6 | B5 | B2),                           /* 92, 6, 5, 2, 0 */
  (B2),                                     /* 93, 2, 0 */
  (B6 | B5 | B1),                           /* 94, 6, 5, 1, 0 */
  (B6 | B5 | B4 | B2 | B1),                 /* 95, 6, 5, 4, 2, 1, 0 */
  (B7 | B6 | B4 | B3 | B2),                 /* 95, 7, 6, 4, 3, 2, 0 */
  0
} ;

/* allocate sys_CYC */
void *JD_sa_sys_CYC() {
  JD_static_random *rd ;
  
  rd = (void *)calloc(1,sizeof(JD_static_random)) ;
  rd->seed = genrand(static_random_data) ;
  return(rd) ;
  
}

char *JD_disp_sys_CYC( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_static_random *rd ;
  int i ;
  ret = (char *)calloc(strlen(indent)+100,sizeof(char) ) ;
  rd = (void *)dt->data.obj.ect->table ;
  sprintf( 
    ret, "%sCYC size=%d, limit=%d, width=%d%s",
    indent, rd->size, rd->limit, rd->width, eol
  ) ;
  return ret ;
}

void JD_sc_sys_CYC_mf_new() {
  JD_data *dt ;
  int tmp ;
  JD_static_random *rd ;
  
  rd = (void *)ACC_DT->data.obj.ect->table ;

  if( !(NUM_ARG == 1) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) {
    tmp = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    tmp = dt->data.int_value ;
  }
  rd->width = tmp ;
  rd->rnd = alloc_mt_random() ;
  init_genrand(rd->seed, rd->rnd) ;
}

/* deallocate random_mt */
void JD_sd_sys_CYC(JD_static_random *rd) {
  JD_static_rnd_range *rng, *nrng ;
  if( rd ) {
    if( rd->rnd ) free( rd->rnd ) ;
    rng = rd->range ;
    while( rng ) {
      nrng = rng->next ;
      if( rng->u_data ) free( rng->u_data ) ;
      if( rng->l_data ) free( rng->l_data ) ;
      free( rng ) ;
      rng = nrng ;
    }
    free(rd) ;
  }
}

void *JD_dup_sys_CYC(JD_static_random *rd) {
  JD_static_random *nw ;
  int i ;
  
  ERROR_ILLEGAL_DUP_CALL_FOR_SYS_RND ;
  
  return(NULL) ;
}

void JD_sc_sys_CYC_mf_set_limit()
{
  JD_data *dt ;
  int tmp ;
  JD_static_random *rd ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  if( !(NUM_ARG == 1) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) {
    tmp = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    tmp = dt->data.int_value ;
  }
  
  rd->limit = tmp ;
  
}

void JD_sc_sys_CYC_mf_set_range_d()
{
  JD_data *dt ;
  int i ;
  JD_static_random *rd ;
  JD_static_rnd_range *rng, *prng ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  rng = (void *)calloc(1,sizeof(JD_static_rnd_range)) ;
  
  if( !(NUM_ARG == 3) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(3) ;     /* ub */
  if( IS_SMLINT(dt) ) {
    rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->u_size = 32 ;
    rng->u_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->u_size = 32 ;
      rng->u_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->u_size = dt->data.sbit.size ;
        rng->u_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->u_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        rng->u_size = dt->data.bit.size ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->u_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
  
  dt = GET_STACK(2) ;     /* lb */
  if( IS_SMLINT(dt) ) {
    rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->l_size = 32 ;
    rng->l_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->l_size = 32 ;
      rng->l_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->l_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->l_size = dt->data.sbit.size ;
        rng->l_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->l_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        rng->l_size = dt->data.bit.size ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->l_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
  
  dt = GET_STACK(1) ;  /* num_element */
  if( IS_SMLINT(dt) ) {
    rng->num_element = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type != JD_D_int ) {
      ERROR_ILLEGAL_DATA_TYPE_FOR_SYS_RND ;
    }
    rng->num_element = dt->data.int_value ;
  }
  
  rng->weight_s = rd->num_element ;
  rd->num_element += rng->num_element ;
  rng->weight_e = rd->num_element - 1 ;
  if( rd->num_element < 0 ) {
    ERROR_WEIGHT_TIMES_NUM_ELEMENT_OVERFLOW ;
  }
  
  if( rd->range ) {
    prng = rd->range ;
    while( prng->next ) prng = prng->next; 
    prng->next = rng ;
  }
  else rd->range = rng ;
  
}

void JD_sc_sys_CYC_mf_set_range_s()
{
  JD_data *dt ;
  int i ;
  JD_static_random *rd ;
  JD_static_rnd_range *rng, *prng ;

  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;

  rng = (void *)calloc(1,sizeof(JD_static_rnd_range)) ;
  
  if( !(NUM_ARG == 3) ) {
    ERROR_ILLEGAL_NUM_ARGS ;
  }
  dt = GET_STACK(1) ;     /* ub */
  if( IS_SMLINT(dt) ) {
    rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
    rng->u_size = 32 ;
    rng->u_data[0] = SMLINT2INT(dt) ;
  }
  else {
    if( dt->type == JD_D_int ) {
      rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
      rng->u_size = 32 ;
      rng->u_data[0] = dt->data.int_value ;
    }
    else {
      /* bit */
      if( dt->type == JD_D_sbit ) {
        rng->u_data = (void *)calloc( 1, sizeof(int) ) ;
        rng->u_size = dt->data.sbit.size ;
        rng->u_data[0] = dt->data.sbit.a ;
      }
      else {
        rng->u_data = 
          (void *)calloc( NWORD(dt->data.bit.size), sizeof(int) ) ;
        rng->u_size = dt->data.bit.size ;
        for( i = 0 ; i < NWORD(dt->data.bit.size) ; i++ ) 
          rng->u_data[i] = dt->data.bit.a[i] ;
      }
    }
  }
    
  rng->num_element = 1 ;
  
  
  rng->weight_s = rng->weight_e = rd->num_element ;
  rd->num_element += 1 ;
  if( rd->num_element < 0 ) {
    ERROR_WEIGHT_TIMES_NUM_ELEMENT_OVERFLOW ;
  }
  
  if( rd->range ) {
    prng = rd->range ;
    while( prng->next ) prng = prng->next; 
    prng->next = rng ;
  }
  else rd->range = rng ;
  
}

static void set_mask( JD_static_random *rd ) {
  int i , nw , bl, m ;
  m = primpoli[rd->act_width-1] ;
  nw = NWORD(rd->act_width) ;
  bl = (rd->act_width - 1) % 32 ;
  for(i=0 ; i <  nw ; i++ ) {
    if( i == nw - 1 ) {
      rd->udata[i] = (1 << bl) ;
      rd->mdata[i] = BITMASK(rd->act_width) ;
    }
    else {
      rd->mdata[i] = 0xffffffff ;
    }
    rd->ppdata[i] = m ;
    m = 0 ;
  }

}


static void gen_cycle_random( JD_static_random *rd ) {
  int i, w, n ;
  int t[3] ;
  w = rd->act_width - 1 ;
  
  if( rd->data[0] & 1 ) {
    t[0] = rd->data[0] ^ rd->ppdata[0] ;
    t[1] = rd->data[1] ^ rd->ppdata[2] ;
    t[2] = rd->data[2] ^ rd->ppdata[2] ;
    rd->data[0] = ((t[0]>>1) & 0x7fffffff) | rd->udata[0] | (rd->data[1]<<31) ;
    rd->data[1] = ((t[1]>>1) & 0x7fffffff) | rd->udata[1] | (rd->data[2]<<31) ;
    rd->data[2] = ((t[2]>>1) & 0x7fffffff) | rd->udata[2] ;
  }
  else {
    rd->data[0] = ((rd->data[0]>>1) & 0x7fffffff) | (rd->data[1]<<31) ;
    rd->data[1] = ((rd->data[1]>>1) & 0x7fffffff) | (rd->data[2]<<31) ;
    rd->data[2] = ((rd->data[2]>>1) & 0x7fffffff) ;
  }
    
  if( rd->data[0] == rd->zdata[0] && 
      rd->data[1] == rd->zdata[1] && 
      rd->data[2] == rd->zdata[2]    ) 
  {
    rd->next_zero = 1 ;
  }
  
  for( i = 0 ; i < 3 ; i++ ) t[i] = rd->rnd_data[i] = rd->data[i] ;
  
  for( i = 0 ; i < rd->swap_point ; i++ ) {
    rd->rnd_data[0] = (rd->rnd_data[0]>>1) | (rd->rnd_data[1]<<31) ;
    rd->rnd_data[1] = (rd->rnd_data[1]>>1) | (rd->rnd_data[2]<<31) ;
    rd->rnd_data[2] = (rd->rnd_data[2]>>1) ;
  }
  n = rd->act_width - rd->swap_point ;
  for( i = 0 ; i < n ; i++ ) {
    t[2] = (t[2] << 1) | (t[1]>>31) ;
    t[1] = (t[1] << 1) | (t[0]>>31) ;
    t[0] = (t[0] << 1) ;
  }
  for( i = 0 ; i < 3 ; i++ ) {
    rd->rnd_data[i] |= t[i] ;
    rd->rnd_data[i] &= rd->mdata[i] ;
    /*
    rd->rnd_data[i] = 
      ((rd->rnd_data[i] & 0x55555555)<<1) | 
      ((rd->rnd_data[i] & 0xaaaaaaaa)>>1) ;
    */
  }
  
}

void JD_sc_sys_CYC_mf_gen_random()
{
  JD_data *dt ;
  int rnd ;
  int loop ;
  int limit ;
  JD_static_random *rd ;
  JD_static_rnd_range *range ;
  int val ;
  int i, mask, p ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  rd =  (void *)ACC_DT->data.obj.ect->table ;
  if( rd->num_element ) {
    if( rd->act_width == 0 ) {
      int t, w ;
      t = 1 ; 
      w = 1 ;
      while( t < rd->num_element ) {
        w++ ;
        t = (1<<w)-1 ;
        if( w > 32 ) {
          ERROR_NUMBER_OF_ELEMENT_TOO_LARGE_FOR_CYC ;
        }
      }
      rd->act_width = w ;
      set_mask(rd) ;
    }
    loop = 1 ;
    if( rd->limit ) limit = rd->limit ;
    else limit = 0 ;
    while( loop ) {
      if( rd->data[0] == rd->sdata[0] ) {
        rd->data[0] = rd->sdata[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
        rd->swap_point = genrand(rd->rnd) % rd->act_width ;
        /*
        printf( "new data %x swap_point %d\n", rd->data[0], rd->swap_point ) ;
        */
     }
      gen_cycle_random(rd) ;
      while( rd->rnd_data[0]-1 > rd->num_element ) {
        if( rd->data[0] == rd->sdata[0] ) {
          rd->data[0] = rd->sdata[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
          rd->swap_point = genrand(rd->rnd) % rd->act_width ;
          /* 
          printf( 
            "  new data %h swap_point %d\n", rd->data[0], rd->swap_point 
          ) ;
          */
        }
        gen_cycle_random(rd) ;
      }
      range = rd->range ;
      val = rd->rnd_data[0] - 1 ;
      while( range ) {
        if( val >= range->weight_s && val <= range->weight_e ) {
          set_acc_value( rd, range, val ) ;
          if( check_cond() ) return ;
        }
        range = range->next ;
      }
      loop++ ;
      if( limit ) {
        if( --limit == 0 ) {
          loop = 0 ;
        }
      }
    }
    ERROR_RND_LIMIT_OVERFLOW ;
  }
  else {
    if( rd->act_width == 0 ) {
      rd->act_width = rd->width ;
      set_mask(rd) ;
    }
    loop = 1 ;
    if( rd->limit ) limit = rd->limit ;
    else limit = 0 ;
    rd->pdata[0] = rd->data[0] ;
    rd->pdata[1] = rd->data[1] ;
    rd->pdata[2] = rd->data[2] ;
    while( loop ) {
      if( !rd->next_zero && 
          rd->data[0] == rd->sdata[0] && 
          rd->data[1] == rd->sdata[1] && 
          rd->data[2] == rd->sdata[2]    ) 
      {
        rd->data[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
        rd->data[1] = (genrand(rd->rnd) & rd->mdata[1]) ;
        rd->data[2] = (genrand(rd->rnd) & rd->mdata[2]) ;
        while( rd->data[0] == 0 && rd->data[1] == 0 && rd->data[2] == 0 ) {
          rd->data[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
          rd->data[1] = (genrand(rd->rnd) & rd->mdata[1]) ;
          rd->data[2] = (genrand(rd->rnd) & rd->mdata[2]) ;
        }
        rd->sdata[0] = rd->pdata[0] = rd->data[0] ;
        rd->sdata[1] = rd->pdata[1] = rd->data[1] ;
        rd->sdata[2] = rd->pdata[2] = rd->data[2] ;
        rd->zdata[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
        rd->zdata[1] = (genrand(rd->rnd) & rd->mdata[1]) ;
        rd->zdata[2] = (genrand(rd->rnd) & rd->mdata[2]) ;
        while( rd->zdata[0] == 0 && rd->zdata[1] == 0 && rd->zdata[2] == 0 ) {
          rd->zdata[0] = (genrand(rd->rnd) & rd->mdata[0]) ;
          rd->zdata[1] = (genrand(rd->rnd) & rd->mdata[1]) ;
          rd->zdata[2] = (genrand(rd->rnd) & rd->mdata[2]) ;
        }
        rd->swap_point = genrand(rd->rnd) % rd->act_width ;
        /* debug */
        /*
          printf( "swap_point %d data %x\n", rd->swap_point, rd->data[0] ) ;
        */
      }
      if( rd->next_zero ) {
        rd->next_zero = 0 ;
        rd->rnd_data[0] = 0 ;
        rd->rnd_data[1] = 0 ;
        rd->rnd_data[2] = 0 ;
      }
      else {
        gen_cycle_random(rd) ;
        if( rd->data[0] == rd->pdata[0] && 
            rd->data[1] == rd->pdata[1] && 
            rd->data[2] == rd->pdata[2]    ) 
        {
          loop = 0 ;
        }
      }
      if( rd->width == -1 ) {
        ACC_INT = rd->rnd_data[0] & 0x7fffffff ; 
        ACC_TYPE = JD_D_int ;
      }
      else {
        for( i = 0 ; i < NWORD(rd->width) ; i++ ) {
          if( i == NWORD(rd->width)-1 ) mask = BITMASK(rd->width) ;
          else mask = 0xffffffff ;
          ACC_A[i] = ( rd->rnd_data[i] & mask );
          ACC_B[i] = 0 ;
        }
        ACC_SIZE = rd->width ;
        ACC_TYPE = JD_D_bit ;
      }
      if( check_cond() ) return ;
      if( limit ) {
        if( --limit == 0 ) {
          loop = 0 ;
        }
      }
    }
    ERROR_RND_LIMIT_OVERFLOW ;
  }
}

