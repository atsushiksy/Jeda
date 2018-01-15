/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
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
#include <stdlib.h>
#include <string.h>

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"
#include "../include/codegen_macro.h"
#include "./vinst.tab.h"

#define FATAL_ERROR exit(1)

extern int JL_debug_flag ;
extern JD_name **Global_name_top ;

void vmodule_gen() ;
void out_port_alloc( vcode_info *this )  ;
void out_portset_alloc( vcode_info *this, int *gindex )  ;

extern FILE *out ;

extern int in_main_function ;
extern int in_member_function ;
extern int in_new_function ;

extern int in_leaf_function ;

extern vcode_info  *JD_top_inst ;

#ifdef PRODUCT_CODE
extern int main_defined ;
extern int module_linked ;
extern link_file_name *top_module_file ;
extern char *out_fname ;
#endif

JD_label_name *JD_label_top ;

void output_c_code( vinst_info *code ) ;

int code_indent ;

char *JD_current_func = NULL ;

/* counting variables to allocate global space */
static int global_var_count ;

int get_vcode_hash_value() ;

/*************************************************************
 *  Debug scope stack to handle the level while constructing *
 *   the debug scope information                             *
 *************************************************************/

static int *dbg_scope_stack = NULL;
static int  dbg_scope_sp = 0 ;
static int  dbg_scope_stack_size = 0 ;

#define DBG_SCOPE_STACK_SIZE 256 

static void push_dbg_scope( int scope ) {
  if( dbg_scope_stack == NULL ) {
    dbg_scope_stack = (int *)calloc(DBG_SCOPE_STACK_SIZE, sizeof(int *)) ;
    dbg_scope_stack_size = DBG_SCOPE_STACK_SIZE ;
  }
  else if( dbg_scope_sp == dbg_scope_stack_size ) {
    dbg_scope_stack = 
      (int *)realloc(
        dbg_scope_stack, 
        (dbg_scope_stack_size+DBG_SCOPE_STACK_SIZE)*sizeof(int *)
      ) ;
  }
  dbg_scope_stack[dbg_scope_sp++] = scope ;
}

static int pop_dbg_scope() {
  dbg_scope_sp-- ;
  return dbg_scope_stack[dbg_scope_sp] ;
}

vhash_value_dcl(){
  OUT1( "int JD_vcode_hash_value = %d ;\n", get_vcode_hash_value() ) ;
}

void out_indent() {
  int i ;
  for( i = 0 ; i < code_indent ; i++ ) fprintf( out, "  " ) ;
}


static void out_extern_class_alloc_dcl( vcode_info *this ) {
  OUT1( "void JD_ua_%s() ;\n", this->is.class.name ) ;
}

static void out_class_alloc_dcl( vcode_info *this ) {
  OUT1( "void JD_ua_%s() ;\n", this->is.named_code.class ) ;
}

static void output_c_mfunc_dcl( vcode_info *this ) {
  OUT2( 
    "void JD_uc_%s_mf_%s() ;\n", 
    this->is.named_code.class, 
    this->is.named_code.func 
  ) ;
}

static void output_c_advice_dcl( vcode_info *this ) {
  OUT2( 
    "void JD_uc_%s_ad_%s() ;\n", 
    this->is.named_code.class, 
    this->is.named_code.func 
  ) ;
}

static void output_c_func_dcl( vcode_info *this ) {
  OUT1( "void JD_uf_%s() ;\n", this->is.named_code.func ) ;
}

static void out_sys_class_dcl( vcode_info *this ) {
  OUT1( "void *JD_sa_%s() ; /* allocate */\n", this->is.class.name ) ;
  OUT1( "void *JD_sd_%s() ; /* deallocate */\n", this->is.class.name ) ;
  OUT1( "void *JD_dup_%s() ; /* duplicate */\n", this->is.class.name ) ;
  OUT0( "#ifdef JEDA_DEBUGGER \n" ) ;
  OUT1( "void *JD_disp_%s() ; /* display */\n", this->is.class.name ) ;
  OUT0( "#endif\n" ) ;
}

static void sys_class_decl() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_function_info:
        break ;
      case JD_class_info:
        if( this->is.class.sys_flag ) out_sys_class_dcl( this ) ;
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}

#define NAME_HASH_SIZE 256 

static int member_index = 0 ;

static int output_ename_names( JD_name *nm, int scope ) {
  JD_name *mem ;
  int i, j ;
  int pid, id ;
  int n = 0 ;
  
  pid = debug_name_lookup( nm->name ) ;
  if( nm->child ) {
    for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
      mem = nm->child[j] ;
      while( mem ) {
        if( mem->type == JD_enum_member_name ) 
        {
          id = debug_name_lookup( mem->name ) ;
  /* { type, index, name, type, ub, lb, level, parent } index holds enum name */
          OUT6(
            "  { DBG_TYPE_ENUM, %d, %d, 0, 0, 0, %d, %d }, /* %s of enum %s */ \n", 
            pid, id, mem->index, scope, mem->name, nm->name
          ) ;
          n++ ;
        }
        mem = mem->next ;
      }
    }
  }
  return n ;
}

static char *nametypestr( vcode_info *type ) {
  switch(type->is.data_type.type) {
    case VI_void: return "void" ;
    case VI_int: return "int" ;
    case VI_long: return "long" ;
    case VI_float: return "float" ;
    case VI_double: return "double" ;
    case VI_bit: return "bit" ;
    case VI_class: return type->is.data_type.name ;
    case VI_sys_class: return type->is.data_type.name ;
    case VI_string: return "string" ;
    case VI_signal: return "string" ;
    case VI_class_type: return "class_type" ;
    case VI_enum: return type->is.data_type.name ;
  }
}

static int ub_of_type( vcode_info *type ) {
  return type->is.data_type.ub ;
}

static int lb_of_type( vcode_info *type ) {
  return type->is.data_type.lb ;
}

static void gen_global_table() {
  JD_name *nm, *mem  ;
  int i, j ;
  int id, did, pid, stid ;
  int fid ;
  int current_scope = 1 ;
  int scope_num = 1 ;
  
#ifdef PRODUCT_CODE
  if( main_defined ) 
#endif
    if( !JL_debug_flag ) return ;
  
  OUT0( "/* { type, index, name, dtype, ub, lb, level, parent } */ \n" ) ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    scope_num = 0 ;
    OUT1( "JD_debug_scope JD_%s_global_scope[] = {\n", out_fname ) ;
    if( !JL_debug_flag ) {
      OUT0( "  { -1, -1, 0, 0, 0, 0, 0, 0 }\n" ) ;
      OUT0( "} ;\n" ) ;
      OUT1( 
        "int JD_%s_global_scope_num = 1 ;  /* number of top entries */\n",
        out_fname
      ) ;
      return ;
    }
  }
  else {
#endif
    OUT0( "JD_debug_scope JD_global_scope[] = {\n" ) ;
#ifdef PRODUCT_CODE
  }
#endif  

  OUT0( "  { 0, 0, 0, 0, 0, 0, 0, 0 },  /* first entry is not used */\n" ) ;
  /* create entries for this and super. */
  id = debug_name_lookup( "this" ) ;
  OUT1( "  { DBG_TYPE_CLASS, 0, %d, 0, 0, 0, 0 }, /* this */\n", id ) ;
  scope_num++ ;
  id = debug_name_lookup( "super" ) ;
  OUT1( "  { DBG_TYPE_CLASS, 0, %d, 0, 0, 0, 0 }, /* super */\n", id ) ;
  scope_num++ ;
  
  for( i = 0 ; i < NAME_HASH_SIZE ; i++ ) {
    nm = Global_name_top[i] ;
    while( nm ) {
      switch( nm->type ) {
        case JD_variable_name:
          id = debug_name_lookup( nm->name ) ;
          did = 
            debug_name_lookup( 
              nametypestr(nm->info->is.variable_dcl.data_type) 
            ) ;
          /* { type, index, name, dtype, ub, lb, level, parent } */
          OUT6( 
            "  { DBG_TYPE_VAR, %d, %d, %d, %d, %d, 0, 0 }, /* %s */ \n", 
            nm->index, id, did, 
            ub_of_type(nm->info->is.variable_dcl.data_type),
            lb_of_type(nm->info->is.variable_dcl.data_type),
            nm->name
          ) ;
          scope_num++ ;
          break ;
        case JD_port_name:
          id = debug_name_lookup( nm->name ) ;
          /* { type, index, name, dtype, ub, lb, level, parent } */
          OUT3( 
            "  { DBG_TYPE_PORT, %d, %d, 0, 0, 0, 0, 0 }, /* %s */ \n", 
            nm->index, id, nm->name
          ) ;
          scope_num++ ;
          break ;
          
        case JD_portset_name:
          id = debug_name_lookup( nm->name ) ;
          /* { type, index, name, dtype, ub, lb, level, parent } */
          OUT3( 
            "  { DBG_TYPE_PORTSET, %d, %d, 0, 0, 0, 0, 0 }, /* %s */ \n", 
            scope_num, id, nm->name
          ) ;
          current_scope = scope_num++ ;
          if( nm->child ) {
            for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
              mem = nm->child[j] ;
              while( mem ) {
                switch( mem->type ) { 
                  case JD_port_name:
                    id = debug_name_lookup( mem->name ) ;
                    OUT5( 
                      "  { DBG_TYPE_PORT, %d, %d, 0, 0, 0, 0, %d }, /* %s.%s */ \n", 
                      mem->index, id, current_scope, nm->name, mem->name
                    ) ;
                    break ;
                  case JD_variable_name: 
                  case JD_mfunc_name:
                  case JD_enum_member_name: 
                    break ;
                }
                mem = mem->next ;
              }
            }
          }
          break ;
        case JD_class_name:
          id = debug_name_lookup( nm->name ) ;
          fid = debug_name_lookup( nm->info->srcfilename ) ;
          if( nm->info->is.class.parent ) 
            pid = debug_name_lookup( nm->info->is.class.parent ) ;
          else
            pid = 0 ;
          /* { type, index, name, dtype, ub, lb, level, parent } */
          OUT6( 
            "  { DBG_TYPE_CLASS, %d, %d, %d, %d, %d, 0, 0 }, /* %s */ \n", 
            scope_num, id, pid, nm->info->srclinenum, fid, nm->name
          ) ;
          current_scope = scope_num++ ;
          if( nm->child ) {
            for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
              mem = nm->child[j] ;
              while( mem ) {
                switch( mem->type ) { 
                  case JD_variable_name: 
                    id = debug_name_lookup( mem->name ) ;
                    did = 
                      debug_name_lookup( 
                        nametypestr(mem->info->is.variable_dcl.data_type)
                      ) ;
                    OUT8( 
                      "  { DBG_TYPE_VAR, %d, %d, %d, %d, %d, 0, %d }, /* %s.%s */ \n", 
                      mem->index, id, did, 
                      ub_of_type(mem->info->is.variable_dcl.data_type),
                      lb_of_type(mem->info->is.variable_dcl.data_type),
                      current_scope, nm->name, mem->name
                    ) ;
                    break ;
                  case JD_mfunc_name:
                    id = debug_name_lookup( mem->name ) ;
                    fid = debug_name_lookup( mem->info->srcfilename ) ;
                    OUT7( 
                      "  { DBG_TYPE_FUNC, %d, %d, 0, %d, %d, 0, %d }, /* %s.%s */\n", 
                      mem->index, id,  
                      mem->info->srclinenum, fid,
                      current_scope,
                      nm->name, mem->name
                    ) ;
                    break ;
                  case JD_enum_name:
                    (void)output_ename_names( mem, current_scope ) ;
                    break ;
                  case JD_port_name:
                  case JD_enum_member_name: 
                    break ;
                }
                mem = mem->next ;
              }
            }
          }
          break ;
        case JD_aspect_name:
          id = debug_name_lookup( nm->name ) ;
          fid = debug_name_lookup( nm->info->srcfilename ) ;
          pid = 0 ;
          /* { type, index, name, dtype, ub, lb, level, parent } */
          OUT6( 
            "  { DBG_TYPE_ASPECT, %d, %d, %d, %d, %d, 0, 0 }, /* %s */ \n", 
            scope_num, id, pid, nm->info->srclinenum, fid, nm->name
          ) ;
          current_scope = scope_num++ ;
          if( nm->child ) {
            for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
              mem = nm->child[j] ;
              while( mem ) {
                switch( mem->type ) { 
                  case JD_variable_name: 
                    id = debug_name_lookup( mem->name ) ;
                    did = 
                      debug_name_lookup( 
                        nametypestr(mem->info->is.variable_dcl.data_type)
                      ) ;
                    if( mem->static_name ) stid = mem->static_name->index ;
                    else stid = 0 ;
                    OUT8( 
                      "  { DBG_TYPE_AVAR, %d, %d, %d, %d, %d, 0, %d }, /* %s.%s */ \n", 
                      stid, id, did, 
                      ub_of_type(mem->info->is.variable_dcl.data_type),
                      lb_of_type(mem->info->is.variable_dcl.data_type),
                      current_scope, nm->name, mem->name
                    ) ;
                    break ;
                  case JD_mfunc_name:
                    id = debug_name_lookup( mem->name ) ;
                    fid = debug_name_lookup( mem->info->srcfilename ) ;
                    OUT7( 
                      "  { DBG_TYPE_FUNC, %d, %d, 0, %d, %d, 0, %d }, /* %s.%s */\n", 
                      mem->index, id,  
                      mem->info->srclinenum, fid,
                      current_scope,
                      nm->name, mem->name
                    ) ;
                    break ;
                  case JD_advice_name:
                    id = debug_name_lookup( mem->name ) ;
                    fid = debug_name_lookup( mem->info->srcfilename ) ;
                    OUT7( 
                      "  { DBG_TYPE_ADVICE, %d, %d, 0, %d, %d, 0, %d }, /* %s.%s */\n", 
                      mem->index, id,  
                      mem->info->srclinenum, fid,
                      current_scope,
                      nm->name, mem->name
                    ) ;
                    break ;
                  case JD_enum_name:
                    (void)output_ename_names( mem, current_scope ) ;
                    break ;
                  case JD_port_name:
                  case JD_enum_member_name: 
                    break ;
                }
                mem = mem->next ;
              }
            }
          }
          break ;
        case JD_enum_name:
          scope_num += output_ename_names( nm, 0 ) ;
          break ;
        case JD_func_name:
          id = debug_name_lookup( nm->name ) ;
          fid = debug_name_lookup( nm->info->srcfilename ) ;
          OUT5( 
            "  { DBG_TYPE_FUNC, %d, %d, 0, %d, %d, 0, 0 }, /* %s */ \n", 
            scope_num, id, nm->info->srclinenum, fid,
            nm->name
          ) ;
          scope_num++ ;
          break ;
      }
      nm = nm->next ;
    }
  }
  OUT0( "  { -1, -1, 0, 0, 0, 0, 0, 0 }\n" ) ;
  OUT0( "} ;\n" ) ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( 
      "int JD_%s_global_scope_num = %d ;  /* number of top entries */\n",
      out_fname, scope_num + 1
    ) ;
  }
  else {
#endif
    OUT1( 
      "int JD_global_scope_num = %d ;  /* number of top entries */\n",
      scope_num + 1
    ) ;
#ifdef PRODUCT_CODE
  }
#endif  
}

static void gen_local_table() {
  vcode_info *this ;
  JD_name *nm, *mem ;
  int i, j ;
  int table_index = 1 ;
  int current_index ;
  int id, did, fid ;
  int current_scope = 0 ;
  int scope_num = 1 ;
  int in_scope_num = 0 ;
  int pid ;
  
#ifdef PRODUCT_CODE
  if( main_defined ) 
#endif
    if( !JL_debug_flag ) return ;
    
  this = JD_top_inst ;
  OUT0( "/* { type, index, name, dtype, level, parent } */ \n" ) ;
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT1( "JD_debug_scope JD_%s_local_scopes[] = {\n", out_fname ) ;
    if( !JL_debug_flag ) {
      OUT0( "  { -1, -1, 0, 0, 0, 0, 0, 0 }\n" ) ;
      OUT0( "} ; \n" ) ;
      OUT1( 
        "int JD_%s_local_scope_num = 1 ;  /* number of top entries */\n",
        out_fname
      ) ;
      return ;
    }
  }
  else {
#endif
    OUT0( "JD_debug_scope JD_local_scopes[] = {\n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  /* entry 0 represent empty local name */
  OUT0( 
    "  { 0, 0, 0, 0, 0, 0, 0, 0 },  /* entry 0 is for no local name */\n" 
  ) ;  
  OUT0( "  { -1, 0, 0, 0, 0, 0, 0, 0 },  /* end of entry 0 */\n" ) ;  
  while( this ) {
    if( this->local_name_flag ) { 
    
      /* local_name_flag is set at the file boundary */
      if( this->local_name_top ) {
        this->local_name_table_index = current_index = scope_num ;
        scope_num++ ;
        in_scope_num = 0 ;
        /* local_name_table_index is used on SET_LOCAL_SCOPE macro */
        for( i = 0 ; i < NAME_HASH_SIZE ; i++ ) {
          nm = this->local_name_top[i] ;
          while( nm ) {
            switch( nm->type ) {
              case JD_variable_name:
                id = debug_name_lookup( nm->name ) ;
                did = 
                  debug_name_lookup( 
                    nametypestr(nm->info->is.variable_dcl.data_type)
                  ) ;
                /* { type, index, name, dtype, level, parent } */
                OUT6( 
                  "  { DBG_TYPE_VAR, %d, %d, %d, %d, %d, 0, 0 }, /* %s */ \n", 
                  nm->index, id, did, 
                  ub_of_type(nm->info->is.variable_dcl.data_type),
                  lb_of_type(nm->info->is.variable_dcl.data_type),
                  nm->name
                ) ;
                in_scope_num++ ;
                break ;
              case JD_port_name:
                /* port can't be local name!! */
                break ;
              case JD_portset_name:
                /* portset can't be local name!! */
                break ;
              case JD_class_name:
                id = debug_name_lookup( nm->name ) ;
                fid = debug_name_lookup( nm->info->srcfilename ) ;
                if( nm->info->is.class.parent ) 
                  pid = debug_name_lookup( nm->info->is.class.parent ) ;
                else
                  pid = 0 ;
                /* { type, index, name, dtype, ub, lb, level, parent } */
                OUT6( 
                  "  { DBG_TYPE_CLASS, %d, %d, %d, %d, %d, 0, 0 }, /* %s */ \n", 
                  scope_num, id, pid, nm->info->srclinenum, fid,
                  nm->name
                ) ;
                current_scope = in_scope_num++ ;
                if( nm->child ) {
                  for( j = 0 ; j < NAME_HASH_SIZE ; j++ ) {
                    mem = nm->child[j] ;
                    while( mem ) {
                      switch( mem->type ) { 
                        case JD_variable_name: 
                          id = debug_name_lookup( mem->name ) ;
                          did = 
                            debug_name_lookup( 
                              nametypestr(mem->info->is.variable_dcl.data_type)
                            ) ;
                          OUT8( 
                            "  { DBG_TYPE_VAR, %d, %d, %d, %d, %d, 0, %d }, /* %s.%s */ \n", 
                            mem->index, id, did,  
                            ub_of_type(nm->info->is.variable_dcl.data_type),
                            lb_of_type(nm->info->is.variable_dcl.data_type),
                            current_scope,
                            nm->name, mem->name
                          ) ;
                          break ;
                        case JD_mfunc_name:
                          id = debug_name_lookup( mem->name ) ;
                          fid = debug_name_lookup( mem->info->srcfilename ) ;

                          OUT7( 
                            "  { DBG_TYPE_FUNC, %d, %d, 0, %d, %d, 0, %d }, /* %s.%s */ \n", 
                            mem->index, id, mem->info->srclinenum, fid,
                            current_scope, nm->name, mem->name
                          ) ;
                          break ;
                        case JD_enum_name:
                          (void)output_ename_names( mem, current_scope ) ;
                          break ;
                        case JD_port_name:
                        case JD_enum_member_name: 
                          break ;
                      }
                      mem = mem->next ;
                    }
                  }
                }
                break ;
              case JD_enum_name:
                in_scope_num += output_ename_names( nm, 0 ) ;
                break ;
            }
            nm = nm->next ;
          }
        }
        OUT0( "  { -1, 0, 0, 0, 0, 0, 0, 0 },\n" ) ;
      }
      else this->local_name_table_index = current_index = 0 ;
    }
    else this->local_name_table_index = current_index ;
    this = this->next ;
  }
  OUT0( "  { -1, -1, 0, 0, 0, 0, 0, 0 }\n" ) ;
  OUT0( "} ; \n" ) ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( 
      "int JD_%s_local_scope_num = %d ;  /* number of top entries */\n",
      out_fname, scope_num + 1
    ) ;
  }
  else {
#endif
    OUT1( 
      "int JD_local_scope_num = %d ;  /* number of top entries */\n",
      scope_num + 1
    ) ;
#ifdef PRODUCT_CODE
  }
#endif

}


static int debug_cwd_id = 0 ;

static void breakable_func( vcode_info *this ) {
  vinst_info *code ;
  int n ;
  int count = 0 ;
  code = this->is.named_code.code ;
  while(code) {
    if( code->type == VVI_debug_info ) {
      if( code->vinfo->type == JD_debug_statement_info ) {
        if( code->vinfo->is.debug_info.filename ) {
          n = debug_name_lookup( code->vinfo->is.debug_info.filename ) ;
          OUT3( 
            "%d, %d, %d, ", 
            debug_cwd_id, n, code->vinfo->is.debug_info.linenum
          ) ;
        }
      }
      if( ++count >= 8 ) {
        OUT0( "\n  " ) ;
        count = 0 ;
      }
    } 
    code = code->next ;
  }
}

static void breakable_table_gen() {
  vcode_info *this ;
  
#ifdef PRODUCT_CODE
  if( main_defined ) 
#endif
    if( !JL_debug_flag ) return ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT1( "int JD_%s_breakable_table[] = {\n  ", out_fname ) ;

    if( !JL_debug_flag ) {
      OUT0( "\n  -1, -1, -1" ) ;
      OUT0( "\n} ; \n" ) ;
      return ;
    }
  }
  else {
#endif
    OUT0( "int JD_breakable_table[] = {\n  " ) ;
#ifdef PRODUCT_CODE
  }
#endif
  this = JD_top_inst ;
  while( this ) {
    switch( this->type ) {
      case JD_mfunc_code:
        breakable_func( this ) ;
        break ;
      case JD_func_code:
        breakable_func( this ) ;
        break ;
      case JD_debug_cwd_info:
        debug_cwd_id = 
          debug_name_lookup( this->is.debug_cwd_info.dir ) ;
        break ;
    }
    this = this->next ;
  }
  OUT0( "\n  -1, -1, -1" ) ;
  OUT0( "\n} ; \n" ) ;

}

static void expoft_table_gen() {
  vcode_info *this ;
  vcode_info *attrib ;
  
  OUT0( "JD_export_func JD_export_table[] = {\n" ) ;
  this = JD_top_inst ;
  while( this ) {
    switch( this->type ) {
      case JD_function_info:
        attrib = this->is.function_info.func_options ;
        if( attrib && attrib->is.attrib.is_export ) {
          OUT1( "  { JD_uf_%s },\n", this->is.function_info.name ) ;
        }
        break ;
    }
    this = this->next ;
  }
  OUT0( "  { NULL }" ) ;
  OUT0( "\n} ; \n" ) ;

}

static void func_decl() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_port_info:
        break ;
      case JD_portset_info:
        break ;
      case JD_connection_info:
        break ;
      case JD_variable_dcl:
        break ;
      case JD_enum_dcl:
        break ;
      case JD_function_info:
        break ;
      case JD_veri_task_info:
        break ;
      case JD_class_info:
#ifdef PRODUCT_CODE
        if( (module_linked || !main_defined )&& this->is.class.attribs && 
            this->is.class.attribs->is.attrib.is_extern )
        out_extern_class_alloc_dcl( this ) ;
#endif
        break ;
      case JD_static_var_info:
        break ;
      case JD_stat_init_code:
        break ;
      case JD_init_code:
        break ;
      case JD_class_init_code:
        break ;
      case JD_class_alloc_code:
        out_class_alloc_dcl( this ) ;
        break ;
      case JD_mfunc_code:
        output_c_mfunc_dcl( this ) ;
        break ;
      case JD_afunc_code:
        output_c_mfunc_dcl( this ) ;
        break ;
      case JD_advice_code:
        output_c_advice_dcl( this ) ;
        break ;
      case JD_func_code:
        output_c_func_dcl( this ) ;
        break ;
      case JD_debug_info:
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}


static void output_var_alloc( 
  vcode_info *data_type, vcode_info *array_range, int index
)
{
  int dim, size;
  vcode_info *dinfo ;
  
  switch( array_range->is.array_range.type ) {
  
    case VI_single:
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_SINGLE_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_SINGLE_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_SINGLE_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_SINGLE_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_SINGLE_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_SINGLE_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_SINGLE_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_SINGLE_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_SINGLE_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for variable\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      break ;
    
    case VI_array:
      OUT1( "  ARRAY_SIZE = %d ;\n", array_range->is.array_range.size ) ;
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_ARRAY_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_ARRAY_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_ARRAY_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_ARRAY_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_ARRAY_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_ARRAY_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for variable\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      break ;
    
    case VI_marray:
      dim = array_range->is.array_range.size ;
      size = 1 ;
      dinfo = array_range->is.array_range.range_list ;
      while(dinfo) {
        size *= dinfo->is.int_value ;
        dinfo = dinfo->next ;
      }
      OUT1( "  ARRAY_SIZE = %d ;\n", size  ) ;
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_ARRAY_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_ARRAY_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_ARRAY_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ARRAY_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_ARRAY_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_ARRAY_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_ARRAY_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for variable\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      OUT1( "  ARRAY_SIZE = %d ;\n", dim ) ;
      OUT0( "CALL_ALLOC_MARRAY_DIM ; \n" ) ;
      dinfo = array_range->is.array_range.range_list ;
      while(dinfo) {
        OUT1( "  ARRAY_SIZE = %d ;\n", dinfo->is.int_value ) ;
        OUT0( "CALL_SET_MARRAY_DIM ; \n" ) ;
        dinfo = dinfo->next ;
      }
      break ;

    case VI_assoc:
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_ASSOC_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_ASSOC_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_ASSOC_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "  BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_ASSOC_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ASSOC_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_ASSOC_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_ASSOC_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_ASSOC_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_ASSOC_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE 
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for lvar\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      break ;
    
    case VI_stridx:
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_STRIDED_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_STRIDED_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_STRIDED_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "  BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_STRIDED_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_STRIDED_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_STRIDED_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_STRIDED_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_STRIDED_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_STRIDED_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for lvar\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      break ;
    
    case VI_list:
      switch( data_type->is.data_type.type ) {
        case VI_int:
          OUT0( "CALL_ALLOC_DT_LIST_INT ; \n" ) ;
          break ;
        case VI_float:
          OUT0( "CALL_ALLOC_DT_LIST_FLOAT ; \n" ) ;
          break ;
        case VI_double:
          OUT0( "CALL_ALLOC_DT_LIST_DOUBLE ; \n" ) ;
          break ;
        case VI_bit:
          if( data_type->is.data_type.ub > data_type->is.data_type.lb ) 
            size = data_type->is.data_type.ub - data_type->is.data_type.lb ;
          else 
            size = data_type->is.data_type.lb - data_type->is.data_type.ub ;
          OUT1( "  BIT_SIZE = %0d ;\n", size+1 ) ;
          OUT0( "CALL_ALLOC_DT_LIST_BIT ; \n" ) ;
          break ;
        case VI_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_LIST_OBJ ; \n" ) ;
          break ;
        case VI_sys_class:
          OUT1( 
            "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
            data_type->is.data_type.name 
          ) ;
          OUT0( "CALL_ALLOC_DT_LIST_OBJ ; \n" ) ;
          OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
          break ;
        case VI_string:
          OUT0( "CALL_ALLOC_DT_LIST_STRING ; \n" ) ;
          break ;
        case VI_signal:
          OUT0( "CALL_ALLOC_DT_LIST_SIGNAL ; \n" ) ;
          break ;
        case VI_enum:
          OUT0( "CALL_ALLOC_DT_LIST_INT ; \n" ) ;
          if( JL_debug_flag ) {
            int n = debug_name_lookup( data_type->is.data_type.name ) ;
#ifdef PRODUCT_CODE
            if( !main_defined ) {
              OUT3( 
                "SET_GLOBAL_ENUM( JD_%s_Debug_name_table_offset+%d, %d ) ;\n",
                out_fname, n, index 
              ) ;
            }
            else {
#endif  
              OUT2( "SET_GLOBAL_ENUM( %d, %d ) ;\n", n, index ) ;
#ifdef PRODUCT_CODE
            }
#endif  
          }
          break ;
        default:
          fprintf( stderr, "Unknown type for lvar\n" ) ;
          FATAL_ERROR ;
          break ;
      }
      break ;
    
  }
}

static void output_gl_variable( vcode_info *this, int index ) {
  vcode_info *attrib ;
  char *nm, *gl ;
  
  nm = this->is.variable_dcl.name ;
  attrib = this->is.variable_dcl.var_attrib ;

  if( attrib && attrib->is.attrib.is_local ) {
    gl = "local" ;
  }
  else {
    gl = "global" ;
  }
  
  if( this->index != index ) {
    fprintf( 
      stderr, "%s variable name \"%s\" index mismatch\n", gl, nm
    ) ;
    exit(1) ;
  }
  
  output_var_alloc( 
    this->is.variable_dcl.data_type, this->is.variable_dcl.array_range,
    index 
  ) ;
  
  /* static data should be connected to gen1 to avoid GC */
  if( this->is.variable_dcl.array_range->is.array_range.type == VI_single &&
      this->is.variable_dcl.data_type->is.data_type.type != VI_class
  ) {
    OUT0( "JD_move_data_to_gen1( TMP_DT ) ; \n" ) ;
  }
  else if ( 
    ( this->is.variable_dcl.array_range->is.array_range.type == VI_array ||
      this->is.variable_dcl.array_range->is.array_range.type == VI_marray   ) &&
      this->is.variable_dcl.data_type->is.data_type.type != VI_class
  ) {
    OUT0( "JD_move_array_data_to_gen1( TMP_DT ) ; \n" ) ;
  }

#ifdef PRODUCT_CODE
  
  
  if( !main_defined ) {
    OUT4( 
      "top->global_data[JD_%s_global_var_offset+%d] = TMP_DT ; /* %s var \"%s\" */\n",
      out_fname,index, gl, nm
    ) ;
  }
  else {
#endif
    OUT3( 
      "top->global_data[%d] = TMP_DT ; /* %s var \"%s\" */\n",
      index, gl, nm
    ) ;
#ifdef PRODUCT_CODE
  }
#endif

}

static void output_static_variable( vcode_info *this, int index ) {
  char *cls, *fnc, *nm ;
  
  cls = this->is.static_var_dcl.class ;
  fnc = this->is.static_var_dcl.func ;
  nm = this->is.static_var_dcl.name ;
  
  if( this->index != index ) {
    fprintf( 
      stderr, "static variable name \"%s.%s.%s\" index mismatch\n", cls, fnc, nm
    ) ;
    exit(1) ;
  }
  
  output_var_alloc( 
    this->is.static_var_dcl.data_type, this->is.static_var_dcl.array_range,
    index 
  ) ;
  
  /* static data should be connected to gen1 to avoid GC */
  OUT0( "JD_move_data_to_gen1( TMP_DT ) ; \n" ) ; 
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT5( 
      "top->global_data[JD_%s_global_var_offset+%d] = TMP_DT ; /* static var \"%s.%s.%s\" */\n",
      out_fname, index, cls, fnc, nm
    ) ;
  }
  else {
#endif
    OUT4( 
      "top->global_data[%d] = TMP_DT ; /* static var \"%s.%s.%s\" */\n",
      index, cls, fnc, nm
    ) ;
#ifdef PRODUCT_CODE
  }
#endif

}

/* var_static_alloc allocate global, local, and static variable */
static void var_static_alloc() {
  vcode_info *this ;
  int gindex = 1 ;
#ifdef PRODUCT_CODE
  link_file_name *f ;
  
  
  if( !main_defined ) {
    /* JD_si_alloc_global is called from top_loop */
    OUT1( "void JD_%s_si_alloc_global( JD_toplevel *top ) {\n", out_fname ) ;
    code_indent++ ;
    OUT1( "JD_%s_global_var_offset = top->num_global_data ;\n", out_fname ) ;
  }
  else {
    f = top_module_file ;
    while( f ) {
     OUT1( "extern int JD_%s_global_var_count ;\n", f->name ) ;
     f = f->next ;
    }
    /* JD_si_alloc_global is called from top_loop */
    OUT0( "void JD_si_alloc_global( JD_toplevel *top ) {\n" ) ;
    code_indent++ ;
    if( module_linked ) {
      OUT0( "int n ;\n" ) ;
      OUT1( "n = %d \n", global_var_count + 1 ) ;
      f = top_module_file ;
      while( f ) {
       OUT1( "   + JD_%s_global_var_count\n", f->name ) ;
       f = f->next ;
      }
      OUT0( "    ; \n" ) ;
      
      OUT0( 
        "top->global_data = GLOBAL_DATA =  (JD_data **)calloc( n, sizeof(JD_data *) ) ;\n"
      ) ;
      OUT0( 
        "top->global_obj =  (int **)calloc( n, sizeof(int *) ) ;\n"
      ) ;
      OUT0( "top->num_global_data = 1 ;\n" ) ;
    }
    else {
#endif  
      OUT1( 
        "top->global_data = GLOBAL_DATA =  (JD_data **)calloc( %d, sizeof(JD_data *) ) ;\n",
        global_var_count + 1
      ) ;
      OUT1( 
        "top->global_obj =  (int **)calloc( %d, sizeof(int *) ) ;\n",
        global_var_count + 1
      ) ;
      OUT0( "top->num_global_data = 1 ;\n" ) ;
#ifdef PRODUCT_CODE
    }
  }
#endif  
  
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_port_info ) {
      out_port_alloc( this ) ;
      OUT0( "top->num_global_data++ ;\n" ) ;
      gindex++ ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_portset_info ) {
      out_portset_alloc( this, &gindex ) ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_veri_task_info ) {
      OUT1( 
        "top->num_global_data++ ; /* verilog task %s */\n", 
        this->is.function_info.name 
      ) ;
      gindex++ ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_variable_dcl ) {
      output_gl_variable( this, this->index ) ;
      OUT0( "top->num_global_data++ ;\n" ) ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_static_var_info ) {
      output_static_variable( this, this->index ) ;
      OUT0( "top->num_global_data++ ;\n" ) ;
    }
    this = this->next ;
  }

#ifdef PRODUCT_CODE
  if( main_defined && module_linked ) {
    f = top_module_file ;
    while( f ) {
     OUT1( "JD_%s_si_alloc_global( top ) ;\n", f->name ) ;
     f = f->next ;
    }
  }
#endif
    
  /* OUT1( "top->num_global_data = %d ;\n", gindex ) ; */
  code_indent-- ;
  
  OUT0( "}\n" ) ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( "int JD_%s_global_var_count = %d ;\n", out_fname, gindex ) ;
  }
#endif

}

static void out_init_code( vcode_info *this ) {
  vinst_info *code ;
  
  code = this->is.init_code.code ;

  while(code) {
    if( code->type == VVI_label ) {
      OUT1( "L_%s: \n", code->name );
    }
    else {
#ifdef PRODUCT_CODE
      if( !code->ignore )
#endif
        output_c_code( code ) ;
    }
    code = code->next ;
  }

}

static void init_code_gen() {
  vcode_info *this ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    /* JD_si_initialize is called from top_loop */
    OUT1( "void JD_%s_si_initialize() {\n", out_fname ) ;
  }
  else {
#endif
  /* JD_si_initialize is called from top_loop */
    OUT0( "void JD_si_initialize() {\n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  code_indent++ ;

  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_port_info:
        break ;
      case JD_portset_info:
        break ;
      case JD_connection_info:
        break ;
      case JD_variable_dcl:
        break ;
      case JD_enum_dcl:
        break ;
      case JD_function_info:
        break ;
      case JD_veri_task_info:
        break ;
      case JD_class_info:
        break ;
      case JD_static_var_info:
        break ;
      case JD_stat_init_code:
        out_init_code( this ) ;
        break ;
      case JD_init_code:
        out_init_code( this ) ;
        break ;
      case JD_class_init_code:
        out_init_code( this ) ;
        break ;
      case JD_class_alloc_code:
        break ;
      case JD_mfunc_code:
        break ;
      case JD_func_code:
        break ;
      case JD_debug_info:
        break ;
      default:
        break ;
    }
    this = this->next ;
  }


#ifdef PRODUCT_CODE
  if( main_defined && module_linked ) {
    link_file_name *f ;
    f = top_module_file ;
    while( f ) {
      OUT1( "JD_%s_si_initialize() ;\n", f->name  ) ;
      f = f->next ;
    }
  }
#endif


  code_indent-- ;
  
  OUT0( "}\n" ) ;

}

static void out_class_alloc_code( vcode_info *this ) {
  vinst_info *code ;

  OUT1( "void JD_ua_%s() {\n", this->is.named_code.class ) ;
  code_indent++ ;
  
  OUT0( "CALL_EXEC_FLUSH_VSTACK ; /* flush vstack before class new */ \n" ) ;
  
  code = this->is.named_code.code ;
  while(code) {
    if( code->type == VVI_label ) {
      OUT1( "L_%s: \n", code->name );
    }
    else {
#ifdef PRODUCT_CODE
      if( !code->ignore ) {
#endif
        if( code->type == VVI_inst && 
            ( code->itype == VI_new || code->itype == VI_gen_child ) )
        {
          /* ALLOC_OBJ & DEALLOC_OBJ should be used at exec_new */
          OUT1( 
            "ALLOC_OBJ = (void *)JD_ua_%s ; \n", this->is.named_code.class 
          ) ;
          OUT0( "DEALLOC_OBJ = (void *)JD_dealloc_obj_table_table ; \n" ) ;
        }
        output_c_code( code ) ;
#ifdef PRODUCT_CODE
      }
#endif
    }
    code = code->next ;
  }
  code_indent-- ;
  
  OUT1( "}  /* end of void JD_ua_%s() */ \n", this->is.named_code.class ) ;
}

static void output_c_mfunc_code( vcode_info *this ) {
  vinst_info *code ;
  int lcount ;
  int len ;
  JD_name *func ;
  JD_label_name **last_label, *new_label ;
  
  JD_label_top = NULL ;
  
  last_label = &JD_label_top ;
  in_member_function = 1 ;

  func = this->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    in_leaf_function = 1 ;
  }
  else {
    in_leaf_function = 0 ;
  }
  
  if( !strcmp(this->is.named_code.func, "new") ) 
    in_new_function = 1 ;
  if( in_leaf_function == 0 )
  {
    OUT2( 
      "void JD_uc_%s_mf_%s( JD_thread *mythread ) {\n", 
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    len = 
      strlen(this->is.named_code.class) + 
      strlen(this->is.named_code.func) +  20 ;
    JD_current_func = (char *)calloc( len, sizeof(char) ) ;
    sprintf( 
      JD_current_func, "JD_uc_%s_mf_%s",
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    code_indent++ ;
    OUT0( "if( mythread ) switch( mythread->run_index ) {\n" ) ;
    code_indent++ ;
    code = this->is.named_code.code ;
    lcount = 1 ;
    while(code) {
      if( code->type == VVI_label && code->itype ) {
        OUT1( "case %d: JD_sync_exec(mythread) ; \n", lcount );
        if( JL_debug_flag ) {
          OUT0( "  FORK_DEBUG_STACK( mythread->run_index ) ;\n" ) ;
        }
        OUT1( "  goto L_%s ; \n", code->name ) ;
        new_label = (JD_label_name *)calloc(1,sizeof(JD_label_name)) ;
        *last_label = new_label ;
        new_label->label = code->name ;
        new_label->index = lcount++ ;
        last_label = &new_label->next ;
      }
      code = code->next ;
    }
    if( lcount == 1 ) {
      OUT0( "default: break ;\n" ) ;
    }
    code_indent-- ;
    OUT0( "} /* end of switch( thrd->index ) */\n" ) ;
  }
  else {
    OUT2( 
      "void JD_uc_%s_mf_%s() {\n", 
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    code_indent++ ;
  }
  if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
    if( !main_defined ) {
      OUT2(
        "SET_LOCAL_SCOPE( JD_%s_local_scope_offset+%d ) ;\n", 
        out_fname, this->local_name_table_index 
      ) ;
    }
    else {
#endif
      OUT1( "SET_LOCAL_SCOPE( %d ) ;\n",this->local_name_table_index ) ;
#ifdef PRODUCT_CODE
    }
#endif
  }
  code = this->is.named_code.code ;
  lcount = 0 ;
  while(code) {
    if( code->type == VVI_label ) {
      OUT1( "L_%s: \n", code->name );
    }
    else {
#ifdef PRODUCT_CODE
      if( !code->ignore ) 
#endif
        output_c_code( code ) ;
    }
    code = code->next ;
  }
  code_indent-- ;
  
  OUT2( 
    "}  /* end of void JD_uc_%s_mf_%s() */ \n",
    this->is.named_code.class, this->is.named_code.func 
  ) ;
  
  if(JD_current_func) {
    free(JD_current_func) ;
    JD_current_func = NULL ;
  }
  in_member_function = 0 ;
  in_new_function = 0 ;
  
}

static void output_c_advice_code( vcode_info *this ) {
  vinst_info *code ;
  int lcount ;
  int len ;
  JD_name *func ;
  JD_label_name **last_label, *new_label ;
  
  JD_label_top = NULL ;
  
  last_label = &JD_label_top ;
  in_member_function = 1 ;

  func = this->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    in_leaf_function = 1 ;
  }
  else {
    in_leaf_function = 0 ;
  }
  
  if( in_leaf_function == 0 )
  {
    OUT2( 
      "void JD_uc_%s_ad_%s( JD_thread *mythread ) {\n", 
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    len = 
      strlen(this->is.named_code.class) + 
      strlen(this->is.named_code.func) +  20 ;
    JD_current_func = (char *)calloc( len, sizeof(char) ) ;
    sprintf( 
      JD_current_func, "JD_uc_%s_mf_%s",
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    code_indent++ ;
    OUT0( "if( mythread ) switch( mythread->run_index ) {\n" ) ;
    code_indent++ ;
    code = this->is.named_code.code ;
    lcount = 1 ;
    while(code) {
      if( code->type == VVI_label && code->itype ) {
        OUT1( "case %d: JD_sync_exec(mythread) ; \n", lcount );
        if( JL_debug_flag ) {
          OUT0( "  FORK_DEBUG_STACK( mythread->run_index ) ;\n" ) ;
        }
        OUT1( "  goto L_%s ; \n", code->name ) ;
        new_label = (JD_label_name *)calloc(1,sizeof(JD_label_name)) ;
        *last_label = new_label ;
        new_label->label = code->name ;
        new_label->index = lcount++ ;
        last_label = &new_label->next ;
      }
      code = code->next ;
    }
    if( lcount == 1 ) {
      OUT0( "default: break ;\n" ) ;
    }
    code_indent-- ;
    OUT0( "} /* end of switch( thrd->index ) */\n" ) ;
  }
  else {
    OUT2( 
      "void JD_uc_%s_ad_%s() {\n", 
      this->is.named_code.class, 
      this->is.named_code.func 
    ) ;
    code_indent++ ;
  }
  if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
    if( !main_defined ) {
      OUT2(
        "SET_LOCAL_SCOPE( JD_%s_local_scope_offset+%d ) ;\n", 
        out_fname, this->local_name_table_index 
      ) ;
    }
    else {
#endif
      OUT1( "SET_LOCAL_SCOPE( %d ) ;\n",this->local_name_table_index ) ;
#ifdef PRODUCT_CODE
    }
#endif
  }
  code = this->is.named_code.code ;
  lcount = 0 ;
  while(code) {
    if( code->type == VVI_label ) {
      OUT1( "L_%s: \n", code->name );
    }
    else {
#ifdef PRODUCT_CODE
      if( !code->ignore ) 
#endif
        output_c_code( code ) ;
    }
    code = code->next ;
  }
  code_indent-- ;
  
  OUT2( 
    "}  /* end of void JD_uc_%s_ad_%s() */ \n",
    this->is.named_code.class, this->is.named_code.func 
  ) ;
  
  if(JD_current_func) {
    free(JD_current_func) ;
    JD_current_func = NULL ;
  }
  in_member_function = 0 ;
  in_new_function = 0 ;
  
}

static void output_c_func_code( vcode_info *this ) {
  vinst_info *code ;
  int lcount ;
  int len ;
  JD_name *func ;
  JD_label_name **last_label, *new_label ;
  
  JD_label_top = NULL ;
  
  last_label = &JD_label_top ;
  
  func = this->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    in_leaf_function = 1 ;
  }
  else {
    in_leaf_function = 0 ;
  }
  
  if( !strcmp( this->is.named_code.func, "main" ) || in_leaf_function == 0 )
  {
    OUT1( 
      "void JD_uf_%s( JD_thread *mythread ) {\n", this->is.named_code.func 
    ) ;
    code_indent++ ;
    OUT0( "if( mythread) switch( mythread->run_index ) {\n" ) ;
    if( !strcmp( this->is.named_code.func, "main" ) ) {
      OUT0( "  case 0: JD_sync_exec(mythread) ; break ;\n" ) ;
      in_main_function = 1 ;
    }
    else in_main_function = 0 ;
    len = strlen(this->is.named_code.func) +  20 ;
    JD_current_func = (char *)calloc( len, sizeof(char) ) ;
    sprintf( 
      JD_current_func, "JD_uf_%s",
      this->is.named_code.func 
    ) ;
    code_indent++ ;
    code = this->is.named_code.code ;
    lcount = 1 ;
    while(code) {
      if( code->type == VVI_label && code->itype ) {
        OUT1( "case %d: JD_sync_exec(mythread) ; \n", lcount );
        if( JL_debug_flag ) {
          OUT0( "  FORK_DEBUG_STACK( mythread->run_index ) ;\n" ) ;
        }
        OUT1( "  goto L_%s ; \n", code->name ) ;
        new_label = (JD_label_name *)calloc(1,sizeof(JD_label_name)) ;
        *last_label = new_label ;
        new_label->label = code->name ;
        new_label->index = lcount++ ;
        last_label = &new_label->next ;
      }
      code = code->next ;
    }
    if( lcount == 1 ) {
      OUT0( "default: break ;\n" ) ;
    }
    code_indent-- ;
    OUT0( "} /* end of switch( thrd->index ) */\n" ) ;
  }
  else {
    OUT1( "void JD_uf_%s() {\n", this->is.named_code.func ) ;
    code_indent++ ;
  }
  if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
    if( !main_defined ) {
      OUT2(
        "SET_LOCAL_SCOPE( JD_%s_local_scope_offset+%d ) ;\n", 
        out_fname, this->local_name_table_index 
      ) ;
    }
    else {
#endif
      OUT1( "SET_LOCAL_SCOPE( %d ) ;\n",this->local_name_table_index ) ;
#ifdef PRODUCT_CODE
    }
#endif
  }
  code = this->is.named_code.code ;
  lcount = 0 ;
  while(code) {
    if( code->type == VVI_label ) {
      OUT1( "L_%s: \n", code->name );
    }
    else {
#ifdef PRODUCT_CODE
      if( !code->ignore ) 
#endif
        output_c_code( code ) ;
    }
    code = code->next ;
  }
  code_indent-- ;
  
  OUT1( "}  /* end of void JD_uf_%s() */ \n", this->is.named_code.func ) ;
  
  if(JD_current_func) {
    free(JD_current_func) ;
    JD_current_func = NULL ;
  }
  in_main_function = 0 ;
  
}

gen_file_table() {
  vcode_info *this ;
  int m, n, k ;
  this = JD_top_inst ;
  
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT1( "JD_debug_file JD_%s_file_table[] = {\n", out_fname ) ;
  }
  else {
    OUT0( "JD_debug_file *JD_file_table ;\n" ) ;
    OUT0( "JD_debug_file JD__file_table[] = {\n" ) ;
  }
#else
  OUT0( "JD_debug_file JD_file_table[] = {\n" ) ;
#endif
  k = 0 ;
  while( this ) {
    if( this->type == JD_debug_file_info ) {
      m = debug_name_lookup( this->is.debug_file_info.dir ) ;
      n = debug_name_lookup( this->is.debug_file_info.file ) ;
      OUT4( 
        "  { %d, %d },  /* %s %s */\n", 
        m, n, this->is.debug_file_info.dir, this->is.debug_file_info.file  
      ) ;
      k++ ;
    }
    this = this->next ;
  }
  OUT0( "  { -1, -1 }\n" ) ;
  OUT0( "};\n" ) ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( "int JD_%s_file_table_size = %d ;\n", out_fname, k ) ;
  }
  else {
    OUT1( "int JD_file_table_size = %d ;\n", k ) ;
  }
#endif

}

static void func_code_gen() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_port_info:
        break ;
      case JD_portset_info:
        break ;
      case JD_connection_info:
        break ;
      case JD_variable_dcl:
        break ;
      case JD_enum_dcl:
        break ;
      case JD_function_info:
        break ;
      case JD_veri_task_info:
        break ;
      case JD_class_info:
        break ;
      case JD_static_var_info:
        break ;
      case JD_stat_init_code:
        break ;
      case JD_init_code:
        break ;
      case JD_class_init_code:
        break ;
      case JD_class_alloc_code:
        out_class_alloc_code( this ) ;
        break ;
      case JD_mfunc_code:
        output_c_mfunc_code( this ) ;
        break ;
      case JD_afunc_code:
        output_c_mfunc_code( this ) ;
        break ;
      case JD_advice_code:
        output_c_advice_code( this ) ;
        break ;
      case JD_func_code:
        output_c_func_code( this ) ;
        break ;
      case JD_debug_cwd_info:
        set_debug_cwd( this ) ;
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}

extern JD_name *Debug_name_head ;

void debug_table_gen()
{
  int i, n ;
  JD_name *name = Debug_name_head ;
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT1( "char *JD_%s_Debug_name_table[] = {\n", out_fname ) ;
  }
  else {
    OUT0( "char **JD_Debug_name_table ;\n" ) ;
    OUT0( "char *JD__Debug_name_table[] = {\n" ) ;
  }
#else  
  OUT0( "char *JD_Debug_name_table[] = {\n" ) ;
#endif  
  
  n = i = 0 ;
  while( name ) {
    OUT1( "\"%s\", ", name->name ) ;
    n++ ;
    if( ++i == 8 ) {
      OUT0( "\n" ) ;
      i = 0 ;
    }
    name = name->link ;
  }
  OUT0( " \"\" } ;\n" ) ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( "int JD_%s_Debug_name_table_size = %d ;\n", out_fname, n ) ;
  }
  else {
    if( module_linked ) {
      link_file_name *f ;
      f = top_module_file ;
      while( f ) {
       OUT1( "extern char *JD_%s_Debug_name_table[] ;\n", f->name ) ;
       OUT1( "extern int JD_%s_Debug_name_table_size ;\n", f->name ) ;
       OUT1( "extern int JD_%s_Debug_name_table_offset ;\n", f->name ) ;
       f = f->next ;
      }
    }
  }
#endif  

}

static int debug_scope_num = 1 ;
static int debug_current_scope = 1 ;

void output_debug_scope( vinst_info *code ) {
  int current_index = 0 ;
  int i, m, n, did, len, sid ;
  char *cp, *c, *cls ;
  
  switch(code->vinfo->type) {
    case JD_debug_cwd_info:
       break ;
    case JD_debug_statement_info:
      if( code->vinfo->is.debug_info.filename ) {
        /* attatch current scope num to the debug info. */
        code->vinfo->is.debug_info.scope = debug_current_scope ;
      }
      break ;
    case JD_debug_gen_func_frame_info:
      n = 
        debug_name_lookup( code->vinfo->is.debug_gen_func_frame_info.func ) ;
      /* { type, index, name, level, parent } */
      OUT2( "  { DBG_TYPE_FUNC, %d, %d, 0, 0, 0, 0, 0 },\n", 
        debug_scope_num, n
      ) ;
      debug_current_scope = debug_scope_num ;
      debug_scope_num++ ;
      current_index = 0 ;
      break ;
    case JD_debug_gen_mfunc_frame_info:
    case JD_debug_gen_afunc_frame_info:
      n = 
        debug_name_lookup( code->vinfo->is.debug_gen_mfunc_frame_info.mfunc ) ;
      cp = c = code->vinfo->is.debug_gen_mfunc_frame_info.mfunc ;
      len = 0 ;
      while( *c != '.' ) {
        len++ ;
        c++ ;
      }
      cls = (char *)calloc( len+1, sizeof(char) ) ;
      strncat(cls, cp, len) ;
      m = debug_name_lookup( cls ) ;
      free( cls ) ;
      /* { type, index, name, type, level, parent } */
      OUT3( 
        "  { DBG_TYPE_FUNC, %d, %d, %d, 0, 0, 0, 0 },\n", 
        debug_scope_num, n, m
      ) ;
      push_dbg_scope( debug_current_scope ) ;
      debug_current_scope = debug_scope_num ;
      debug_scope_num++ ;
      current_index = 0 ;
      break ;
    case JD_debug_gen_advice_frame_info:    
      n = 
        debug_name_lookup( code->vinfo->is.debug_gen_mfunc_frame_info.mfunc ) ;
      cp = c = code->vinfo->is.debug_gen_mfunc_frame_info.mfunc ;
      len = 0 ;
      while( *c != '.' ) {
        len++ ;
        c++ ;
      }
      cls = (char *)calloc( len+1, sizeof(char) ) ;
      strncat(cls, cp, len) ;
      m = debug_name_lookup( cls ) ;
      free( cls ) ;
      /* { type, index, name, type, level, parent } */
      OUT3( 
        "  { DBG_TYPE_ADVICE, %d, %d, %d, 0, 0, 0, 0 },\n", 
        debug_scope_num, n, m
      ) ;
      push_dbg_scope( debug_current_scope ) ;
      debug_current_scope = debug_scope_num ;
      debug_scope_num++ ;
      current_index = 0 ;
      break ;
    case JD_debug_argument_info:
      n = 
        debug_name_lookup( code->vinfo->is.debug_var_argument_info.name ) ;
      did =
        debug_name_lookup( code->vinfo->is.debug_var_argument_info.type ) ;
      /* { type, index, name, level, parent } */
      OUT8( 
        "  { DBG_TYPE_ARG, %d, %d, %d, %d, %d, %d, %d }, /* %s */\n",
        code->vinfo->is.debug_var_argument_info.level, 
        n, did, 
        code->vinfo->is.debug_var_argument_info.ub,
        code->vinfo->is.debug_var_argument_info.lb,
        current_index++, 
        debug_current_scope,
        code->vinfo->is.debug_var_argument_info.name
      ) ;
      break ;
    case JD_debug_variable_info:
      n = 
        debug_name_lookup( code->vinfo->is.debug_var_argument_info.name ) ;
      did =
        debug_name_lookup( code->vinfo->is.debug_var_argument_info.type ) ;
      /* { type, index, name, type, ub, lb, level, parent } */
      OUT8( 
        "  { DBG_TYPE_VAR, %d, %d, %d, %d, %d, %d, %d }, /* %s */\n",
        code->vinfo->is.debug_var_argument_info.level, 
        n, did, 
        code->vinfo->is.debug_var_argument_info.ub,
        code->vinfo->is.debug_var_argument_info.lb,
        current_index++, 
        debug_current_scope,
        code->vinfo->is.debug_var_argument_info.name
      ) ;
      break ;
    case JD_debug_enum_info:
      m = 
        debug_name_lookup( code->vinfo->is.debug_enum_info.name ) ;
      for( i = 0 ; i < code->vinfo->is.debug_enum_info.num_member ; i++ ) {
        n = 
          debug_name_lookup( code->vinfo->is.debug_enum_info.member[i] ) ;
        /* { type, index, name, type, level, parent } index holds enum name */
        OUT6( 
          "  { DBG_TYPE_ENUM, %d, %d, 0, 0, 0, %d, %d }, /* %s of enum %s */\n",
          m, n, i, debug_current_scope,
          code->vinfo->is.debug_enum_info.member[i],
          code->vinfo->is.debug_enum_info.name
        ) ;
      }
      break ;
    case JD_debug_block_info:
      /* { type, index, name, type, level, parent } */
      OUT3( 
        "  { DBG_TYPE_BLOCK, %d, 0, 0, 0, 0, %d, %d },\n", debug_scope_num,
         code->vinfo->is.debug_block_info.level, debug_current_scope
      ) ;
      push_dbg_scope( debug_current_scope ) ;
      debug_current_scope = debug_scope_num ;
      debug_scope_num++ ;
      current_index = 0 ;
      break ;
    case JD_debug_endblock_info:
      debug_current_scope = pop_dbg_scope() ;
      OUT1( 
        "  /* end of block level = %d */\n", 
        code->vinfo->is.debug_endblock_info.level 
      ) ;
      break ;
    case JD_debug_member_variable_info:
      /* TBF:   This looks broken, need to spill data type info and ub/lb */
      n = 
        debug_name_lookup( code->vinfo->is.debug_member_variable_info.class ) ;
      m = 
        debug_name_lookup( code->vinfo->is.debug_member_variable_info.name ) ;
        /* { type, index, name, type, level, parent } */
      OUT7(
        "  { DBG_TYPE_MVAR, %d, %d, %d, 0, 0, %d, %d }, /* %s.%s */\n",
        code->vinfo->is.debug_member_variable_info.index, 
        m, n, 
        code->vinfo->is.debug_member_variable_info.level, 
        debug_current_scope,
        code->vinfo->is.debug_member_variable_info.class,
        code->vinfo->is.debug_member_variable_info.name
      ) ;
      break ;
    case JD_debug_aspect_variable_info:
      n = 
        debug_name_lookup( code->vinfo->is.debug_member_variable_info.class ) ;
      m = 
        debug_name_lookup( code->vinfo->is.debug_member_variable_info.name ) ;
        /* { type, index, name, type, level, parent } */
      if( code->vinfo->is.debug_member_variable_info.static_name )
        sid = code->vinfo->is.debug_member_variable_info.static_name->index ;
      else
        sid = 0 ;
      OUT7(
        "  { DBG_TYPE_AVAR, %d, %d, %d, 0, 0, %d, %d }, /* %s.%s */\n",
        sid, 
        m, n, 
        code->vinfo->is.debug_member_variable_info.level, 
        debug_current_scope,
        code->vinfo->is.debug_member_variable_info.class,
        code->vinfo->is.debug_member_variable_info.name
      ) ;
      break ;
    case JD_debug_exit_function_info:
      debug_current_scope = 0 ;
      OUT1( 
        "  /* end of function level = %d */\n", 
        code->vinfo->is.debug_endblock_info.level 
      ) ;
      break ;
  }

}

static void output_c_func_scope( vcode_info *this ) {
  vinst_info *code ;
  
  
  OUT1( 
    "  /* scope for func %s() */\n", 
    this->is.named_code.func
  ) ;
  code = this->is.named_code.code ;
  while(code) {
    if( code->type == VVI_debug_info ) {
      output_debug_scope( code ) ;
    }
    code = code->next ;
  }
    
}

static void output_c_mfunc_scope( vcode_info *this ) {
  vinst_info *code ;

  OUT2( 
    "  /* scope for %s.%s() */\n", 
    this->is.named_code.class, 
    this->is.named_code.func 
  ) ;
  code = this->is.named_code.code ;
  while(code) {
    if( code->type == VVI_debug_info ) {
      output_debug_scope( code ) ;
    }
    code = code->next ;
  }
  
}

void gen_scope_table() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  OUT0( "/* { type, index, name, dtype, level, parent } */ \n" ) ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT1( "JD_debug_scope JD_%s_scope_table[] = {\n", out_fname ) ;
  }
  else {
#endif
    OUT0( "JD_debug_scope JD_scope_table[] = {\n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
    
  OUT0( "  { 0, 0, 0, 0, 0, 0, 0, 0 }, /* first entry not used */\n" ) ;
    
  while( this ) {
    switch( this->type ) {
      case JD_port_info:
        break ;
      case JD_portset_info:
        break ;
      case JD_connection_info:
        break ;
      case JD_variable_dcl:
        break ;
      case JD_enum_dcl:
        break ;
      case JD_function_info:
        break ;
      case JD_veri_task_info:
        break ;
      case JD_class_info:
        break ;
      case JD_static_var_info:
        break ;
      case JD_stat_init_code:
        break ;
      case JD_init_code:
        break ;
      case JD_class_init_code:
        break ;
      case JD_class_alloc_code:
        break ;
      case JD_mfunc_code:
      case JD_afunc_code:
      case JD_advice_code:
        output_c_mfunc_scope( this ) ;
        break ;
      case JD_func_code:
        output_c_func_scope( this ) ;
        break ;
      case JD_debug_cwd_info:
        set_debug_cwd( this ) ;
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

  OUT0( "  { -1, -1, 0, 0, 0, 0, 0, 0 }\n" ) ;
  OUT0( "} ;\n" ) ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( 
      "int JD_%s_scope_table_num = %d ; /* number of top entries */\n", 
      out_fname, debug_scope_num 
    ) ;
    OUT1( 
      "int JD_%s_scope_table_offset ; \n", 
      out_fname 
    ) ;
  }
  else {
#endif
    OUT1( 
      "int JD_scope_table_num = %d ; /* number of top entries */\n", 
      debug_scope_num 
    ) ;
#ifdef PRODUCT_CODE
  }
#endif
}

static int is_verilog_var( vcode_info *this ) {
  vcode_info *attrib ;
  char *nm ;
  vcode_info *array_range ;
  int dtype ;

  nm = this->is.variable_dcl.name ;
  dtype = this->is.variable_dcl.data_type->is.data_type.type ;
  attrib = this->is.variable_dcl.var_attrib ;

  if( attrib && attrib->is.attrib.is_local ) return 0 ; /* no output on local */
    
  array_range = this->is.variable_dcl.array_range ;
  
  switch( array_range->is.array_range.type ) {
    case VI_single:
      switch( dtype ) {
        case VI_int:
        case VI_bit:
          return 1 ;
          break ;
      }
      break ;
    case VI_array:
      switch( dtype ) {
        case VI_int:
        case VI_bit:
          return 1 ;
          break ;
      }
      break ;
  }
  return 0 ;
}

static int is_verilog_static( vcode_info *this ) {
  vcode_info *attrib ;
  char *nm ;
  vcode_info *array_range ;
  int dtype ;

  dtype = this->is.static_var_dcl.data_type->is.data_type.type ;
  attrib = this->is.static_var_dcl.attribs ;

  if( attrib && attrib->is.attrib.is_local ) return 0 ; /* no output on local */
    
  array_range = this->is.static_var_dcl.array_range ;
  
  switch( array_range->is.array_range.type ) {
    case VI_single:
      switch( dtype ) {
        case VI_int:
        case VI_bit:
          return 1 ;
          break ;
      }
      break ;
    case VI_array:
      switch( dtype ) {
        case VI_int:
        case VI_bit:
          return 1 ;
          break ;
      }
      break ;
  }
  return 0 ;
}

static void assign_index() {
  vcode_info *this ;
  int gindex = 1 ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) gindex = 0 ;
#endif
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_port_info ) {
      this->index = gindex++ ;
      this->code_name->index = this->index ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_portset_info ) {
      vcode_info *that ;
      that = this->is.portset.port_infos ;
      while( that ) {
        that->index = gindex++ ;
        that->code_name->index = that->index ;
        that = that->next ;
      }
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_veri_task_info ) {
      this->index = gindex++ ;
      this->code_name->index = this->index ;
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_variable_dcl ) {
      if( is_verilog_var(this) ) {
        this->index = gindex++ ;
        this->code_name->index = this->index ;
      }
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_static_var_info ) {
      if( is_verilog_static(this) ) {
        this->index = gindex++ ;
        this->code_name->index = this->index ;
      }
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_variable_dcl ) {
      if( !is_verilog_var(this) ) {
        this->index = gindex++ ;
        this->code_name->index = this->index ;
      }
    }
    this = this->next ;
  }
  this = JD_top_inst ;
  while( this ) {
    if( this->type == JD_static_var_info ) {
      if( !is_verilog_static(this) ) {
        this->index = gindex++ ;
        this->code_name->index = this->index ;
      }
    }
    this = this->next ;
  }
  global_var_count = gindex ;
}


#ifdef PRODUCT_CODE

static void peephole_code( vcode_info *this ) {
  vinst_info *code ;
  
  code = this->is.named_code.code ;
  code->no_flush = 1 ;
  while( code->next) {
    if( code->type == VVI_inst && code->next->type == VVI_inst ) {
      if( code->itype == VI_pop && code->isubtype == VI_alu && 
          code->next->itype == VI_push && code->next->isubtype == VI_alu )
      {
        /* case of 'pop alu' followed by 'push alu', 
           this happens in non blocking assignment */
        code->ignore = 1 ;
        code->next->ignore = 1 ;
      } 
      if( code->itype == VI_lvar && code->next->itype == VI_lvar ) {
        code->next->no_flush = 1 ;
      }
    }
    code = code->next ;
  }
  
}

static void peephole_optimize() {
  vcode_info *this ;
  
  this = JD_top_inst ;
  
  while( this ) {
    switch( this->type ) {
      case JD_port_info:
        break ;
      case JD_portset_info:
        break ;
      case JD_connection_info:
        break ;
      case JD_variable_dcl:
        break ;
      case JD_enum_dcl:
        break ;
      case JD_function_info:
        break ;
      case JD_veri_task_info:
        break ;
      case JD_class_info:
        break ;
      case JD_static_var_info:
        break ;
      case JD_stat_init_code:
        break ;
      case JD_init_code:
        break ;
      case JD_class_init_code:
        break ;
      case JD_class_alloc_code:
        peephole_code( this ) ;
        break ;
      case JD_mfunc_code:
        peephole_code( this ) ;
        break ;
      case JD_afunc_code:
        peephole_code( this ) ;
        break ;
      case JD_advice_code:
        peephole_code( this ) ;
        break ;
      case JD_func_code:
        peephole_code( this ) ;
        break ;
      case JD_debug_cwd_info:
        break ;
      default:
        break ;
    }
    this = this->next ;
  }

}

gen_module_table_vars() {
  link_file_name *f ;

  if( !main_defined ) {
    OUT1( "int JD_%s_global_var_offset ;\n", out_fname ) ;
    OUT1( "int JD_%s_Debug_name_table_offset ;\n", out_fname ) ;
    OUT1( "int JD_%s_local_scope_offset ;\n", out_fname ) ;
    return ;
  }
  if( !JL_debug_flag ) return ;
   
  f = top_module_file ;
  while( f ) {
    OUT1( "extern JD_debug_scope JD_%s_global_scope[] ;\n", f->name ) ;
    OUT1( "extern int JD_%s_global_scope_num ;\n", f->name ) ;
    OUT1( "extern JD_debug_scope JD_%s_local_scopes[] ;\n", f->name ) ;
    OUT1( "extern int JD_%s_local_scope_num ;\n", f->name ) ;
    OUT1( "extern int JD_%s_local_scope_offset ;\n", f->name ) ;
    OUT1( "extern JD_debug_scope JD_%s_scope_table[] ;\n", f->name ) ;
    OUT1( "extern int JD_%s_scope_table_num ;\n", f->name ) ;
    OUT1( "extern int JD_%s_scope_table_offset ;\n", f->name ) ;
    OUT1( "extern JD_debug_file JD_%s_file_table[] ;\n", f->name ) ;
    OUT1( "extern int JD_%s_file_table_size ;\n", f->name ) ;
    OUT1( "extern int JD_%s_breakable_table[] ;\n", f->name ) ;
    OUT1( "extern char *JD_%s_Debug_name_table[] ;\n", f->name ) ;
    OUT1( "extern int JD_%s_Debug_name_table_size ;\n", f->name ) ;
    OUT1( "extern int JD_%s_Debug_name_table_offset ;\n", f->name ) ;
    OUT1( "extern int JD_%s_global_var_count ;\n", f->name ) ;
    f = f->next ;
  }

}

gen_module_table() {
  link_file_name *f ;
   
   
  OUT0( "JD_module_table JD_modules_entries[] = {\n" ) ;
   
  if( !JL_debug_flag ) {
  OUT0( "  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, JD__Debug_name_table, NULL },\n" ) ;
  f = top_module_file ;
  while( f ) {
    OUT0( "  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, \n" ) ;
    OUT1( " JD_%s_Debug_name_table,\n", f->name ) ;
    OUT1( " &JD_%s_Debug_name_table_offset} ,\n", f->name ) ;
    f = f->next ;
  }
  OUT0( "  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }\n" ) ;
    OUT0( "} ;\n" ) ;
    return ;
  }
  
  OUT0( " {JD_global_scope, &JD_global_scope_num,\n" ) ;
  OUT0( " JD_local_scopes, &JD_local_scope_num, NULL,\n" ) ;
  OUT0( " JD_scope_table, &JD_scope_table_num,  NULL,\n" ) ;
  OUT0( " JD__file_table, &JD_file_table_size, \n" ) ;
  OUT0( " JD_breakable_table, \n" ) ;
  OUT0( " JD__Debug_name_table, NULL }, \n" ) ;
  f = top_module_file ;
  while( f ) {
    OUT1( "{ JD_%s_global_scope,\n", f->name ) ;
    OUT1( " &JD_%s_global_scope_num,\n", f->name ) ;
    OUT1( " JD_%s_local_scopes,\n", f->name ) ;
    OUT1( " &JD_%s_local_scope_num,\n", f->name ) ;
    OUT1( " &JD_%s_local_scope_offset,\n", f->name ) ;
    OUT1( " JD_%s_scope_table,\n", f->name ) ;
    OUT1( " &JD_%s_scope_table_num,\n", f->name ) ;
    OUT1( " &JD_%s_scope_table_offset,\n", f->name ) ;
    OUT1( " JD_%s_file_table,\n", f->name ) ;
    OUT1( " &JD_%s_file_table_size,\n", f->name ) ;
    OUT1( " JD_%s_breakable_table,\n", f->name ) ;
    OUT1( " JD_%s_Debug_name_table,\n", f->name ) ;
    OUT1( " &JD_%s_Debug_name_table_offset} ,\n", f->name ) ;
    f = f->next ;
  }
  OUT0( "  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }\n" ) ;
  OUT0( "} ;\n" ) ;

}

#endif

void ccodegen() {
#ifdef PRODUCT_CODE
  peephole_optimize() ;
#endif
  assign_index() ;
#ifdef PRODUCT_CODE
  if( main_defined ) {
    gen_module_table_vars() ; 
#endif
    vmodule_gen() ;
    vhash_value_dcl() ;
    sys_class_decl() ; 
    link_advice() ;
    gen_global_table() ;
    gen_local_table() ;
    gen_scope_table() ;
    gen_file_table() ;
    func_decl() ;
    var_static_alloc() ;
    init_code_gen() ;
    func_code_gen() ;
    debug_table_gen() ;
    breakable_table_gen() ;
    expoft_table_gen() ;
#ifdef PRODUCT_CODE
    gen_module_table() ; 
  }
  else {
    gen_module_table_vars() ; 
    sys_class_decl() ; 
    link_advice() ;
    gen_global_table() ;
    gen_local_table() ;
    gen_scope_table() ;
    gen_file_table() ;
    func_decl() ;
    var_static_alloc() ;
    init_code_gen() ;
    func_code_gen() ;
    debug_table_gen() ;
    breakable_table_gen() ;
  }
#endif
}
