/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

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
#include "runtime.h"
#include "runtime_error_msg.h"
#include "mem_macro.h"
#include "data.h"
#include "exec_macro.h"
#include "code_exec_macro.h"
#include "../regexp/regexp.h"
#include "regexp_exec.h"

int regexp_match(
  char *rexp, char *str, int *nparens, char ***startp, char ***endp
) ;

/******************************************************************
 * regexp class implementation
 ******************************************************************/
 
/* allocate regexp instance */
void *JD_sa_regexp() {
  return (void *)calloc( 1, sizeof(JD_regexp) ) ;
}

char *JD_disp_regexp( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_regexp *reg ;
  int i ;
  reg = (void *)dt->data.obj.ect->table ;
  if( reg->source ) i = strlen(reg->source) ;
  else i = 0 ;
  ret = (char *)calloc(strlen(indent)+i+100,sizeof(char) ) ;
  sprintf( 
    ret, "%slast source=\"%s\" %s", indent, 
    i?reg->source:"<empty>", 
    eol
  ) ;
  return ret ;
}

void JD_sd_regexp( JD_regexp *exp ) {
  if( exp ) {
    if( exp->source ) free(exp->source) ;
    if( exp->exp ) free(exp->exp) ;
    if( exp->node ) JD_regdealloc(exp->node) ;
    free(exp) ;
  }
}

void *JD_dup_regexp(JD_regexp *exp) {
  JD_regexp *nw ;
  int i ;
  
  nw =  (void *)calloc( 1, sizeof(JD_regexp) ) ;
  
  if( exp->node ) {
    if( exp->source ) nw->source = (char *)strdup(exp->source) ;
    if( exp->exp ) nw->exp = (char *)strdup(exp->exp) ;
    if( exp->node ) {
      nw->node = JD_regcomp(exp->exp) ;
      if( nw->node && exp->node->nparens ) JD_regexec(exp->node, exp->source) ;
    }
  }
  
  return(nw) ;
}

void JD_sc_regexp_mf_match( /* string source, string regexp */ ) {
  JD_regexp *exp ;
  JD_data *src, *reg ;
  int ret ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  exp =  (void *)ACC_DT->data.obj.ect->table ;
  if( exp->source ) free(exp->source) ;
  reg = GET_STACK(1) ;
  src = GET_STACK(2) ;
  exp->source = src->data.string?(char *)strdup(src->data.string):"" ;
  if( exp->exp ) {
    if( reg->data.string && exp->node && !strcmp(reg->data.string, exp->exp) ) {
      /* expression is the same, no need to compile */
    }
    else {
      free(exp->exp) ;
      exp->exp = reg->data.string?(char *)strdup(reg->data.string):"" ;
      if( exp->node ) JD_regdealloc(exp->node) ;
      exp->node = JD_regcomp(exp->exp) ;
    }
  }
  else {
    exp->exp = reg->data.string?(char *)strdup(reg->data.string):"" ;
    if( exp->node ) JD_regdealloc(exp->node) ;
    exp->node = JD_regcomp(exp->exp) ;
  }
  /* calling regexp engine ported from Perl5 */
  ret = 
    JD_regexec( exp->node, exp->source ) ;
  
  ACC_INT = ret ;
  ACC_TYPE = JD_D_int ;
}

/* extract string before match */
void JD_sc_regexp_mf_prematch() {
  JD_regexp *exp ;
  int len ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  exp =  (void *)ACC_DT->data.obj.ect->table ;
  if( exp->source && exp->node && exp->node->startp && exp->node->endp ) {
    len = (int) (exp->node->startp[0] - exp->source) ;
    if( len ) {
      ACC_STRING = (char *)calloc( len+1, sizeof(char) ) ;
      strncpy( ACC_STRING, exp->source, len ) ;
    }
    else ACC_STRING = NULL ;
  }
  else ACC_STRING = NULL ;
  ACC_TYPE = JD_D_string ;
  
}

/* extract string after match */
void JD_sc_regexp_mf_postmatch() {
  JD_regexp *exp ;
  char *endp ;
  int len ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  exp =  (void *)ACC_DT->data.obj.ect->table ;
  if( exp->source && exp->node && exp->node->startp && exp->node->endp ) {
    endp = &exp->source[strlen(exp->source)] ;
    len = (int)(endp - exp->node->endp[0]) ;
    if( len ) {
      ACC_STRING = (char *)calloc( len+1, sizeof(char) ) ;
      strncpy( ACC_STRING, exp->node->endp[0], len ) ;
    }
    else ACC_STRING = NULL ;
  }
  else ACC_STRING = NULL ;
  ACC_TYPE = JD_D_string ;
  
}

/* extract string on the match */
void JD_sc_regexp_mf_thismatch() {
  JD_regexp *exp ;
  int len ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  exp =  (void *)ACC_DT->data.obj.ect->table ;
  if( exp->source && exp->node && exp->node->startp && exp->node->endp ) {
    len = (int)(exp->node->endp[0] - exp->node->startp[0]) ;
    if( len ) {
      ACC_STRING = (char *)calloc( len+1, sizeof(char) ) ;
      strncpy( ACC_STRING, exp->node->startp[0], len ) ;
    }
    else ACC_STRING = NULL ;
  }
  else ACC_STRING = NULL ;
  ACC_TYPE = JD_D_string ;
  
}

/* extract string on the match */
void JD_sc_regexp_mf_backref( /* int n */ ) {
  JD_regexp *exp ;
  JD_data *dt ;
  int len, n ;
  
  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) n = SMLINT2INT(dt) ;
  else n = dt->data.int_value ;
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  exp =  (void *)ACC_DT->data.obj.ect->table ;
  if( exp->source && exp->node && exp->node->startp && exp->node->endp ) {
    len = (int)(exp->node->endp[n+1] - exp->node->startp[n+1]) ;
    if( len ) {
      ACC_STRING = (char *)calloc( len+1, sizeof(char) ) ;
      strncpy( ACC_STRING, exp->node->startp[n+1], len ) ;
    }
    else ACC_STRING = NULL ;
  }
  else ACC_STRING = NULL ;
  ACC_TYPE = JD_D_string ;
  
}

static char *errseen = "" ;

void JD_regerror(s)
char *s;
{
	errseen = s;
}
