/* 
   
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
#include <stdlib.h>
#include <string.h>

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"
#include "../include/codegen_macro.h"
#include "./vinst.tab.h"

#include "../regexp/regexp.h"
#include "../include/regexp_exec.h"

extern vcode_info  *JD_top_inst ;

static JD_regexp *reg = NULL ;

/* return of 1: simple match at the function
   return of 2: match within the class function
   return of 3: match with instance class
*/
int pointcut_path_match( vinst_info *code, vcode_info *path )
{
  JD_name *p ;
  int rr ;
  int t = 0 ;
  
  p = code->fname ;
  while( path ) {
    t++ ;
    if( !p ) return 0 ;
    if( path->type == JD_pointcut_name ) {
      if( strcmp( path->is.pointcut_info.name, p->name ) ) return 0 ;
    }
    else {
      if( !reg ) reg = (void *)calloc( 1, sizeof(JD_regexp) ) ;
      reg->source = p->name ;
      reg->exp = path->is.pointcut_info.name ;
      reg->node = JD_regcomp(reg->exp) ;
      rr = JD_regexec( reg->node, reg->source ) ;
      JD_regdealloc(reg->node) ;
      if( !rr ) return 0 ;
      reg->node = NULL ;
    }
    path = path->is.pointcut_info.next ;
    if( path && 
        !path->is.pointcut_info.next && 
        p->class_name
      ) 
    {
      /* if the next one is the last 'name' pointcut, then check if the
         class name of current point matches to it 
      */
      if( path->type == JD_pointcut_name ) {
        if( !strcmp( path->is.pointcut_info.name, p->class_name->name ) )
        {
          return 3 ; /* instance class match */
        }
      }
      else {
        if( !reg ) reg = (void *)calloc( 1, sizeof(JD_regexp) ) ;
        reg->source = p->class_name->name ;
        reg->exp = path->is.pointcut_info.name ;
        reg->node = JD_regcomp(reg->exp) ;
        rr = JD_regexec( reg->node, reg->source ) ;
        JD_regdealloc(reg->node) ;
        if( rr ) {
          return 1 ; /* no specific object associated with the advice */
        }
        JD_regdealloc(reg->node) ;
        reg->node = NULL ;
      }
    }
    p = p->next ;
  }
  if( p ) return 0 ; /* require full match */
  if( t > 1 ) return 2 ;
  return 1 ;
}

void match_pointcut(
  vinst_info *c_code, vinst_info *call_code, vinst_info *r_code, 
  vcode_info *adv
) {
  vcode_info *pa, *pb ;
  vcode_info *call_info, *pointcut, *ptn ;
  advice_link *advice, *advp ;
  int t ;
  
  call_info = call_code->fname->info ;
  /* check function type match */
  if( adv->is.function_info.data_type->is.data_type.type != VI_void ) {
    if( !comp_data_type( 
           adv->is.function_info.data_type, 
           call_info->is.function_info.data_type
         )
      ) return ;
  }
  /* check argument type match */
  if( adv->is.function_info.arguments ) {
    pa = adv->is.function_info.arguments ;
    pb = call_info->is.function_info.arguments ;
    while( pa ) {
      if( !pb ) return ;
      if( pa->is.argument.var != pb->is.argument.var ) return ;
      if( 
        !comp_data_type( pa->is.argument.data_type, pb->is.argument.data_type) 
      ) return ;
      if( 
        !comp_array_range( 
          pa->is.argument.array_range, pb->is.argument.array_range
        ) 
      ) return ;
      pa = pa->next ;
      pb = pb->next ;
    }
    if( pb ) return ;
  }
  /* here we have data type and argument matches */
  pointcut = adv->is.function_info.pointcut ;
  while( pointcut ) {
    switch( pointcut->type ) {
      case JD_call_pointcut:
        if( 
          t = pointcut_path_match( c_code, pointcut->is.pointcut_info.next )  
        )
        {
          advice = ALLOC(advice_link) ;
          advice->advice = adv ;
          advice->match_type = t ;
          if( c_code->advice == NULL ) c_code->advice = advice ;
          else {
            advp = c_code->advice ;
            while( advp->next ) advp = advp->next ;
            advp->next = advice ;
          }
        }
        break ;
      case JD_return_pointcut:
        if( 
          t = pointcut_path_match( r_code, pointcut->is.pointcut_info.next )  
        )
        {
          advice = ALLOC(advice_link) ;
          advice->advice = adv ;
          advice->match_type = t ;
          if( r_code->advice == NULL ) r_code->advice = advice ;
          else {
            advp = r_code->advice ;
            while( advp->next ) advp = advp->next ;
            advp->next = advice ;
          }
        }
        break ;
      case JD_overwrite_pointcut:
        if( 
          t = pointcut_path_match( c_code, pointcut->is.pointcut_info.next )  
        )
        {
          advice = ALLOC(advice_link) ;
          advice->advice = adv ;
          advice->match_type = t ;
          if( c_code->advice == NULL ) c_code->advice = advice ;
          else {
            advp = c_code->advice ;
            while( advp->next ) advp = advp->next ;
            advp->next = advice ;
          }
          c_code->overwrite = 1 ;
        }
        break ;
    
    }
    pointcut = pointcut->next ;
  }
}

void search_pointcut( vcode_info *adv ) {
  vcode_info *this ;
  vinst_info *code, *c_code, *call_code, *r_code ;
  this = JD_top_inst ;
  c_code = call_code = r_code = NULL ;          
  
  while( this ) {
    switch( this->type ) {
      case JD_mfunc_code:
      case JD_func_code:
        code = this->is.named_code.code ;
        while( code ) {
          if( code->type == VVI_inst ) {
            if( code->itype == VI_call_advice ) {
              c_code = code ;
            }
            else if( code->itype == VI_call || code->itype == VI_call_m ) {
              call_code = code ;
            }
            else if( code->itype == VI_return_advice ) {
              r_code = code ;
              match_pointcut( c_code, call_code, r_code, adv ) ;  
              c_code = call_code = r_code = NULL ;          
            }
          }
          code = code->next ;
        }
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}

void parse_aspect( vcode_info *asp ) {
  vcode_info *this ;
  this = asp->is.class.members ;
  
  while( this ) {
    if( this->type == JD_advice_info ) {
      search_pointcut( this ) ;
    }
    this = this->next ;  
  }
}

void link_advice() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_aspect_info:
        parse_aspect( this ) ;
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}

static char *errseen = "" ;

void JD_regerror(s)
char *s;
{
	errseen = s;
}
