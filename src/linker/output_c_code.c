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
#include "../include/code_exec_macro.h"
#include "../include/data_macro.h"
#include "../include/thread_macro.h"
#include "../include/exec_b_i_alu_ops.h"
#include "../include/exec_float_alu_ops.h"
#include "../include/exec_int_alu_ops.h"
#include "../include/exec_object_macro.h"
#include "../include/exec_pli_macro.h"
#include "./vinst.tab.h"

#define FATAL_ERROR exit(1)

#define INIT_WD_SIZE 64 

extern int JL_debug_flag ;

extern char *input_file_name ;
extern int lex_line_num ;

extern JD_label_name *JD_label_top ;
extern int inline_optimization ;
extern int trace_flag ;

int debug_name_lookup( char *) ;

int in_main_function = 0 ;
int in_member_function = 0 ;
int in_new_function = 0 ;

int in_leaf_function = 1 ;
static int code_overwrite = 0 ;
static int code_supress = 0 ;

extern char *JD_current_func ;
extern FILE *out ;

#ifdef PRODUCT_CODE
extern int main_defined ;
extern int module_linked ;
extern link_file_name *top_module_file ;
extern char *out_fname ;
#endif

int current_scope_n ;
int current_file_n ;
int current_st_n ;
int current_line_n ;
int current_cwd_n ;

static int eval_end_num = 0 ;

void output_c_code( vinst_info *code ) ;

static int get_strength( int strength ) {
  switch(strength) {
    case VI_strong: return 2 ;
    case VI_normal: return 1 ;
    case VI_weak: return 0 ;
  }
  return 1 ;
}

static void illegal_binary_ops() {
  fprintf( 
    stderr, "!!! illegal type for binary ops!#^*&^\n" 
  ) ;
}

/************************************/
/* stack operation instructions     */
/************************************/

/*
  'copy' 'alu' Decimal_num EOL
  <<op_subop_index>>
*/
static void output_copy( vinst_info *code ) {
  OUT1( "ACC_INT = %d ;\n", code->index ) ;
  OUT_EXEC_COPY_ALU ;
}

/* ARGSUSED */
static void output_dup( vinst_info *code ) {
  fprintf( stderr, "!!! dup instruction is not suported any more!#^*&^\n" ) ;
  exit(1) ;
}

static void output_pop( vinst_info *code ) {
  if( code->isubtype == VI_alu ) {
    OUT_EXEC_POP_ACC ;
  }
  else {
    OUT1( "TMP_INT = %d ;\n", code->index ) ;
    OUT_EXEC_POP_N ;
  }
}

static void output_push( vinst_info *code ) {
  if( code->isubtype == VI_alu ) {
    OUT_EXEC_PUSH_ACC ;
  }
  else {
    fprintf( 
      stderr, "!!! illegal isubtype for push!#^*&^\n" 
    ) ;
  }
}

/************************************/
/* jump instructions                */
/************************************/

static void output_jmp( vinst_info *code ) {
  OUT1( "goto L_%s ;\n", code->name ) ;
}

static void output_jz( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_string:
      OUT1( "if( ACC_STRING == NULL ) goto L_%s ;\n", code->name ) ;
      OUT1( 
        "if( ACC_STRING[0] == \'\\0\' ) goto L_%s ;\n", code->name 
      ) ;
      break ;
    case VI_int:
      OUT1( "if( ACC_INT == 0 ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_bit:
      OUT0( "switch( ACC_TYPE ) {\n" ) ;
      OUT0( "case JD_D_sbit:\n" ) ;
      OUT1( 
            "if( ACC_A[0]==0 && ACC_B[0]==0 ) goto L_%s ;\n", 
            code->name 
      ) ;
      OUT0( "break ;\n" ) ;
      OUT0( "case JD_D_bit:\n" ) ;
      OUT0( "case JD_D_vbit:\n" ) ;
      OUT0( "{ int i, j ;\n" ) ;
      OUT0( "j = LASTWORD(ACC_SIZE) ;\n" ) ;
      OUT0( "for( i = 0 ; i <= j ; i++ ) {\n" ) ;
      OUT0( "if( ACC_A[i] || ACC_B[i] ) break ;\n" ) ;
      OUT0( "}\n" ) ;
      OUT1( "if( i > j ) goto L_%s ;\n", code->name ) ;
      OUT0( "}\n" ) ;
      OUT0( "break ;\n" ) ;
      OUT0( "}\n" ) ;
      break ;
    case VI_float:
    case VI_double:
      OUT1( "if( ACC_FLOAT == 0.0 ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_obj:
      OUT1( "if( ACC_DT->data.obj.ect == NULL ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_signal:
      OUT1( "if( ACC_DT->data.signal.port == NULL ) goto L_%s ;\n", code->name ) ;
      break ;
    default:
    fprintf( 
      stderr, "!!! illegal type for jz!#^*&^\n" 
    ) ;
  }
}

static void output_jnz( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_string:
      OUT1( 
        "if( ACC_STRING != NULL && ACC_STRING[0] != '\0' ) goto L_%s ;\n", 
        code->name 
      ) ;
      break ;
    case VI_int:
      OUT1( "if( ACC_INT != 0 ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_bit:
      OUT0( "switch( ACC_TYPE ) {\n" ) ;
      OUT0( "  case JD_D_sbit:\n" ) ;
      OUT1( 
            "    if( ACC_A[0]!=0 || ACC_B[0]!=0 ) goto L_%s ;\n", 
            code->name 
      ) ;
      OUT0( "    break ;\n" ) ;
      OUT0( "  case JD_D_bit:\n" ) ;
      OUT0( "  case JD_D_vbit:\n" ) ;
      OUT0( "    { int i, j ;\n" ) ;
      OUT0( "      j = LASTWORD(ACC_SIZE) ;\n" ) ;
      OUT0( "      for( i = 0 ; i <= j ; i++ ) {\n" ) ;
      OUT1( "        if( ACC_A[i] || ACC_B[i] ) goto L_%s ;\n", code->name ) ;
      OUT0( "      }\n" ) ;
      OUT0( "    }\n" ) ;
      OUT0( "    break ;\n" ) ;
      OUT0( "}\n" ) ;
      break ;
    case VI_float:
    case VI_double:
      OUT1( "if( ACC_FLOAT != 0.0 ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_obj:
      OUT1( "if( ACC_DT->data.obj.ect != NULL ) goto L_%s ;\n", code->name ) ;
      break ;
    case VI_signal:
      OUT1( "if( ACC_DT->data.signal.port != NULL ) goto L_%s ;\n", code->name ) ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for jnz!#^*&^\n" 
      ) ;
  }
}


/************************************/
/* stack frame handling             */
/************************************/

/* ARGSUSED */
static void output_gen_frame( vinst_info *code ) {
  OUT_EXEC_FLUSH_VSTACK ;
  OUT_EXEC_GEN_FRAME ;
}
  
/* ARGSUSED */
static void output_gen_m_frame( vinst_info *code ) {
  OUT_EXEC_FLUSH_VSTACK ;
  OUT_EXEC_GEN_M_FRAME ;
}
  
/* ARGSUSED */
static void output_gen_a_frame( vinst_info *code ) {
  OUT_EXEC_FLUSH_VSTACK ;
  OUT_EXEC_GEN_A_FRAME ;
}

/* ARGSUSED */
static void output_pop_this( vinst_info *code ) {
  if( in_new_function ) {
    OUT_EXEC_POP_THIS_NEW ;
  }
  else {
    OUT_EXEC_POP_THIS ;
  }
}

/************************************/
/* concurrent instructions          */
/************************************/

static void output_exec( vinst_info *code ) {
  JD_label_name *label = JD_label_top ;
  
  while( label ) {
    if( !strcmp(label->label, code->name) ) break ;
    label = label->next ;
  }
  if( label ) {
    if( JD_current_func != NULL ) {
      OUT1( "TARGET_FUNC = (void *)%s ;\n", JD_current_func ) ;
      OUT2( "TMP_INT = %d ; /* label %s */ \n", label->index, code->name ) ;
      OUT_EXEC_EXEC ;
      if( JL_debug_flag ) {
        OUT0( "SET_FORK_EXEC ;\n" ) ;
      }
    }
    else {
      fprintf( stderr, "Error: fork outside function\n" ) ;
      exit(1) ;
    }
  }
  else {
    fprintf( stderr, "Unknown label %s\n", code->name ) ;
    exit(1) ;
  }
}
  
static void output_exit( vinst_info *code ) {
  if( JL_debug_flag ) {
    OUT0( "SET_FORK_EXIT ;\n" ) ;
  }
  if( code->isubtype == VI_p_and ) {
    OUT_EXEC_EXIT_P_AND ;
    OUT0( "return ; \n" ) ;
  }
  else if( code->isubtype == VI_p_or ) {
    OUT_EXEC_EXIT_P_OR ;
    OUT0( "return ; \n" ) ;
  }
  else {
    OUT_EXEC_EXIT ;
    OUT0( "return ; \n" ) ;
  }
}
  
/* ARGSUSED */
static void output_fork( vinst_info *code ) {
  OUT_EXEC_FORK ;
}
  
/* ARGSUSED */
static void output_join( vinst_info *code ) {
  OUT_EXEC_JOIN ;
  if( JL_debug_flag ) {
    OUT0( "SET_JOIN ;\n" ) ;
  }
}
  
/* ARGSUSED */
static void output_knife( vinst_info *code ) {
  OUT_EXEC_KNIFE ;
  if( JL_debug_flag ) {
    OUT0( "SET_KNIFE ;\n" ) ;
  }
}
  
/* ARGSUSED */
static void output_spoon( vinst_info *code ) {
  OUT_EXEC_SPOON ;
  if( JL_debug_flag ) {
    OUT0( "SET_SPOON ;\n" ) ;
  }
}
  
/* ARGSUSED */
static void output_terminate( vinst_info *code ) {
  OUT_EXEC_TERMINATE ;
}
  
/* ARGSUSED */
static void output_p_and( vinst_info *code ) {
  fprintf( stderr, "Old output_p_and() is called!!\n" ) ;
  exit(1) ;
}
  
/* ARGSUSED */
static void output_p_or( vinst_info *code ) {
  fprintf( stderr, "Old output_p_or() is called!!\n" ) ;
  exit(1) ;
}
  
/************************************/
/* constant instructions            */
/************************************/

static char *pstring( char *str ) {
  char *cp, *ret, *rp ;
  int len = strlen( str ) ;
  cp = str ;
  while( *cp != '\0' ) {
    if( *cp == '\\' || *cp == '\"' ) len++ ; 
    cp++ ;
  }
  ret = (char *)calloc( len+1, sizeof(char) ) ;
  cp = str ;
  rp = ret ;
  while( *cp != '\0' ) {
    if( *cp == '\\' || *cp == '\"' ) *rp++ = '\\' ; 
    *rp++ = *cp++ ;
  }
  return ret ;
}

static void output_load_const( vinst_info *code ) {
  int i ;
  switch( code->sd_type->is.sd_type.a ) {
    case VI_string:
    {
      int len ; 
      char *str ;
      /* str = code->const_data->is.string ; */
      str = pstring( code->const_data->is.string ) ; 
      len = strlen( str ) ;
      OUT0( "/* string const */\n" ) ;
      OUT1( "ACC_STRING = ALLOC_STRING(%d) ;\n", len ) ;
      OUT1( "strcat( ACC_STRING, \"%s\") ;\n", str ) ;
      OUT0( "ACC_TYPE = JD_D_string ; \n" ) ;
      free(str) ;
      break ;
    }
    case VI_int:
      OUT1( "/* int const \"%s\" */\n", code->const_data->string ) ;
      OUT1( "ACC_INT = %d ; \n", code->const_data->is.int_value  ) ;
      OUT0( "ACC_TYPE = JD_D_int ; \n" ) ;
      break ;
    case VI_bit:
    {
      expression_node *exp = code->const_data ;
      int n ;
      
      if( exp->type == JD_int_num ) {
        OUT1( "ACC_A[0] = 0x%x ;\n", exp->is.int_value ) ;
        OUT0( "ACC_B[0] = 0 ;\n" ) ;
        OUT0( "ACC_SIZE = 32 ;\n" ) ;
      }
      else { /* JD_bit_num */
        n = LASTWORD(exp->is.bit_const.size) ;
        if( n >= INIT_WD_SIZE ) {
          OUT1( "/* bit const \"%s\" */\n", exp->string ) ;
          OUT0( "{\n" ) ;
          OUT0( "  int i, n ;\n" ) ;
          OUT1( "  if( ACC_WD < %d ) { \n", n+1 ) ;
          OUT0( "    ACC_A = \n" ) ;
          OUT1( 
              "      (unsigned int *)realloc( ACC_A, %d ) ;\n", 
              NBYTE(exp->is.bit_const.size)
          ) ;
          OUT0( "    ACC_B = \n" ) ;
          OUT1( 
              "      (unsigned int *)realloc( ACC_B, %d ) ;\n", 
              NBYTE(exp->is.bit_const.size)
          ) ;
          OUT1( "    ACC_WD = %d ;\n", n+1 ) ;
          OUT0( "  }\n" ) ;
          OUT0( "}\n" ) ;
        }
        for( i = 0 ; i <= n ; i++ ) {
          OUT2( "ACC_A[%d] = 0x%x ;\n", i, exp->is.bit_const.a[i] ) ;
          OUT2( "ACC_B[%d] = 0x%x ;\n", i, exp->is.bit_const.b[i] ) ;
        }
        OUT1( "ACC_SIZE = %d ;\n", exp->is.bit_const.size ) ;
      }
      OUT0( "ACC_TYPE = JD_D_bit ; \n" ) ;
      break ;
    }
    case VI_float:
    {
      expression_node *exp = code->const_data ;
      OUT1( "/* float const \"%s\" */\n", exp->string ) ;
      OUT1( "ACC_FLOAT = %f ; \n", code->const_data->is.real_value  ) ;
      OUT0( "ACC_TYPE = JD_D_float ; \n" ) ;
      break ;
    }
    case VI_enum:
    {
      expression_node *exp = code->const_data ;
      OUT3(
        "ACC_INT = %d ; /* enum %s.%s */\n",
        exp->is.enum_const.index, 
        exp->is.enum_const.type, 
        exp->is.enum_const.member
      ) ;
      OUT0( "ACC_TYPE = JD_D_int ; \n" ) ;
      break ;
    }
    default:
      fprintf( 
        stderr, "!!! illegal type for load_const!#^*&^\n" 
      ) ;
      break ;
  }
}

/* ARGSUSED */
static void output_load_one( vinst_info *code ) {
  OUT0( "/* load_one */\n" ) ;
  OUT0( "ACC_INT = 1 ; \n" ) ;
  OUT0( "ACC_TYPE = JD_D_int ; \n" ) ;
}

/* ARGSUSED */
static void output_load_zero( vinst_info *code ) {
  OUT0( "/* load_zero */\n" ) ;
  OUT0( "ACC_INT = 0 ; \n" ) ;
  OUT0( "ACC_TYPE = JD_D_int ; \n" ) ;
}

static void output_load_port( vinst_info *code ) {
  if( code->isubtype ) { /* subbit */
    OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
    OUT0( "CALL_EXEC_LOAD_PORT_SUB ; \n" ) ;
  }
  else {
    OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
    OUT0( "CALL_EXEC_LOAD_PORT ; \n" ) ;
  }
}
  
static void output_load_pset_port( vinst_info *code ) {
  output_load_port( code ) ;
}

static void output_load_port_ptr( vinst_info *code ) {
  if( code->isubtype ) { /* subbit */
    OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
    OUT0( "CALL_EXEC_LOAD_PORT_PTR_SUB ; \n" ) ;
  }
  else {
    OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
    OUT0( "CALL_EXEC_LOAD_PORT_PTR ; \n" ) ;
  }
}

static void output_load_pset_port_ptr( vinst_info *code ) {
  output_load_port_ptr( code ) ;
}
  
static void output_load_sig_pt( vinst_info *code ) {
  if( code->isubtype ) { /* subbit */
    OUT0( "CALL_EXEC_LOAD_SIG_PT_SUB ; \n" ) ;
  }
  else {
    OUT0( "CALL_EXEC_LOAD_SIG_PT ; \n" ) ;
  }
}

static void output_load_super( vinst_info *code ) {
#ifdef PRODUCT_CODE
  if( !inline_optimization ) {
#endif
    OUT0( "if( THREAD->this && THREAD->this->data.obj.ect && \n" ) ;
    OUT0( "    THREAD->this->data.obj.ect->super && \n" ) ;
    OUT0( "    THREAD->this->data.obj.ect->super->data.obj.ect ) { \n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  OUT0( "  ACC_DT = THREAD->this->data.obj.ect->super ; \n" ) ;
  OUT0( "  ACC_TYPE = JD_D_dt ;\n" ) ;
#ifdef PRODUCT_CODE
  if( !inline_optimization ) {
#endif
    OUT0( "} \n" ) ;
    OUT0( "else { \n" ) ;
    OUT0( "  ERROR_EMPTY_OBJECT_ACCESS ; \n" ) ;
    OUT0( "} \n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  if( code->is_var ) {
    fprintf( stderr, "Unsupported var detected on load_super\n" ) ;
  }
}
  
static void output_load_this( vinst_info *code ) {
#ifdef PRODUCT_CODE
  if( !inline_optimization ) {
#endif
    OUT0( "if( THREAD->this && THREAD->this->data.obj.ect ) { \n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  OUT0( "  ACC_DT = THREAD->this ; \n" ) ;
  OUT0( "  ACC_TYPE = JD_D_dt ;\n" ) ;
#ifdef PRODUCT_CODE
  if( !inline_optimization ) {
#endif
    OUT0( "} \n" ) ;
    OUT0( "else { \n" ) ;
    OUT0( "  ERROR_EMPTY_OBJECT_ACCESS ; \n" ) ;
    OUT0( "} \n" ) ;
#ifdef PRODUCT_CODE
  }
#endif
  if( code->is_var ) {
    fprintf( stderr, "Unsupported var detected on load_this\n" ) ;
  }
}

static void just_output_load_this() {
  OUT0( "  ACC_DT = THREAD->this ; \n" ) ;
  OUT0( "  ACC_TYPE = JD_D_dt ;\n" ) ;
}
  
static void output_load_thread( vinst_info *code ) {
  JD_label_name *label = JD_label_top ;
  
  while( label ) {
    if( !strcmp(label->label, code->name) ) break ;
    label = label->next ;
  }
  if( label ) {
    OUT2( "TMP_INT = %d ; /* label %s */ \n", label->index, code->name ) ;
    OUT_EXEC_LOAD_THREAD ;
  }
  else {
    fprintf( stderr, "Unknown label %s\n", code->name ) ;
    exit(1) ;
  }
}
  
static void output_loadg_code( 
  vinst_info *code, int index, char *lg, char *name
) {
  int dim, pn ;
  int i, j ;
  vcode_info *dinfo ;

  if( code->array_access == NULL ) {
    OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
    if( code->is_var ) {
      OUT_EXEC_LOADG_SINGLE_VAR ;
    }
    else {
      OUT_EXEC_LOADG_SINGLE ;
    }
  }
  else {
    /* various array access */
    switch( code->array_access->is.array_range.type ) {
      case VI_array:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADG_ARRAY ;
        break ;
      case VI_marray:
        dim = code->array_access->is.array_range.size ;
        pn = 1 ;
        OUT0( "{  int tmp = 0 ;\n" ) ;
        for( i = 0 ; i < dim ; i++ ) {
          dinfo = code->array_access->is.array_range.range_list ;
          for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
          if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
          OUT1( "  tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
          if( !inline_optimization ) {
            OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
            OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
            OUT1( 
              "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 
            ) ;
            OUT0( "  }\n" ) ;
            OUT0( "#endif\n" ) ;
          }
          pn = pn * dinfo->is.int_value ;
        }
        OUT0( "  ACC_INT = tmp ;\n" ) ;
        OUT0( "}\n" ) ;
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADG_MARRAY ;
        break ;
      case VI_stridx:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADG_STRIDX ;
        break ;
      case VI_assoc:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADG_ASSOC ;
        break ;
      case VI_list:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADG_LIST ;
        break ;
    }
  }
}


static void output_loadg( vinst_info *code ) {
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    ERROR_ILLEGAL_ACCESS_TO_GLOBAL_VARIABLE_IN_MODULE( code->name ) ;
  }
  else
#endif
  output_loadg_code( code, code->code_name->index, "global", code->name ) ;
  
}

static void output_load_static( vinst_info *code ) {
  int index ;
  int len ;
  char *lg ;
  char *cls, *fnc, *nm ;
  char *full_name ;
  
  cls = code->class ;
  fnc = code->func ;
  nm = code->name ;
  
  index = code->code_name->index ;

  lg = "static" ;
  len = strlen(cls)+strlen(fnc)+strlen(nm)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, nm ) ;
  
  output_loadg_code( code, index, lg, full_name ) ;
  
  free(full_name) ;
}
  
static void output_loadl( vinst_info *code ) {
  int dim, pn ;
  int i, j ;
  vcode_info *dinfo ;
  if( code->array_access == NULL ) {
    OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
    if( code->is_var ) {
      OUT_EXEC_LOADL_SINGLE_VAR ;
    }
    else if( code->is_static ) {
      OUT_EXEC_LOADL_SINGLE_STATIC ;
    }
    else {
      OUT_EXEC_LOADL_SINGLE ;
    }
  }
  else {
    /* various array access */
    switch( code->array_access->is.array_range.type ) {
      case VI_array:
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_LOADL_ARRAY ;
        break ;
      case VI_marray:
        dim = code->array_access->is.array_range.size ;
        pn = 1 ;
        OUT0( "{  int tmp = 0 ;\n" ) ;
        for( i = 0 ; i < dim ; i++ ) {
          dinfo = code->array_access->is.array_range.range_list ;
          for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
          if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
          OUT1( "  tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
          if( !inline_optimization ) {
            OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
            OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
            OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
            OUT0( "  }\n" ) ;
            OUT0( "#endif\n" ) ;
          }
          pn = pn * dinfo->is.int_value ;
        }
        OUT0( "  ACC_INT = tmp ;\n" ) ;
        OUT0( "}\n" ) ;
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_LOADL_MARRAY ;
        break ;
      case VI_stridx:
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_LOADL_STRIDX ;
        break ;
      case VI_assoc:
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_LOADL_ASSOC ;
        break ;
      case VI_list:
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_LOADL_LIST ;
        break ;
    }
  }
}
  
static void output_loadm( vinst_info *code ) {
  int dim, pn ;
  int i, j ;
  vcode_info *dinfo ;
  char *lg, *name ;
  int index ;
  
  lg = code->class ;
  name = code->name ;
  index = code->index ;

  if( code->array_access == NULL ) {
    OUT3( "TMP_INT = %d ; /* class %s var \"%s\" */ \n", index, lg, name ) ;
    if( code->is_var ) {
      OUT_EXEC_LOADM_SINGLE_VAR ;
    }
    else {
      OUT_EXEC_LOADM_SINGLE ;
    }
  }
  else {
    /* various array access */
    switch( code->array_access->is.array_range.type ) {
      case VI_array:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADM_ARRAY ;
        break ;
      case VI_marray:
        dim = code->array_access->is.array_range.size ;
        pn = 1 ;
        OUT0( "{  int tmp = 0 ;\n" ) ;
        for( i = 0 ; i < dim ; i++ ) {
          dinfo = code->array_access->is.array_range.range_list ;
          for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
          if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
          OUT1( "  tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
          if( !inline_optimization ) {
            OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
            OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
            OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
            OUT0( "  }\n" ) ;
            OUT0( "#endif\n" ) ;
          }
          pn = pn * dinfo->is.int_value ;
        }
        OUT0( "  ACC_INT = tmp ;\n" ) ;
        OUT0( "}\n" ) ;
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADM_MARRAY ;
        break ;
      case VI_stridx:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADM_STRIDX ;
        break ;
      case VI_assoc:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADM_ASSOC ;
        break ;
      case VI_list:
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
        OUT_EXEC_LOADM_LIST ;
        break ;
    }
  }
}


/************************************/
/* store instructions               */
/************************************/
static void output_store_port( vinst_info *code ) {
  if( code->isubtype == VI_subbit ) {
    /* subbit access */
    OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
    OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
    OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
    OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
    switch( code->sd_type->is.sd_type.b ) {
      case VI_int:
        OUT_EXEC_INT_TO_BIT ;
        break ;
      case VI_bit:
        break ;
      case VI_float:
        OUT_EXEC_FLOAT_TO_BIT ;
        break ;
      default:
        input_file_name = code->filename ;
        lex_line_num = code->linenum ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
        break ;
    }
    if( code->itype == VI_store_pset_port ) {
      OUT3( 
        "TMP_INT = %d ; /* port %s.%s */ \n", 
        code->code_name->index, code->portset, code->port 
      ) ;
    }
    else {
      OUT2( 
        "TMP_INT = %d ; /* port %s */ \n", 
        code->code_name->index, code->port 
      ) ;
    }
    OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
    OUT_EXEC_STORE_PORT_SUBBIT ;
  }
  else {
    switch( code->sd_type->is.sd_type.b ) {
      case VI_int:
        OUT_EXEC_INT_TO_BIT ;
        break ;
      case VI_bit:
        break ;
      case VI_float:
        OUT_EXEC_FLOAT_TO_BIT ;
        break ;
      default:
        input_file_name = code->filename ;
        lex_line_num = code->linenum ;
        ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
        break ;
    }
    if( code->itype == VI_store_pset_port ) {
      OUT3( 
        "TMP_INT = %d ; /* port %s.%s */ \n", 
        code->code_name->index, code->portset, code->port 
      ) ;
    }
    else {
      OUT2( 
        "TMP_INT = %d ; /* port %s */ \n", 
        code->code_name->index, code->port 
      ) ;
    }
    OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
    OUT_EXEC_STORE_PORT ;
  }
}


static void output_store_sig_array_subbit( vinst_info *code ) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  switch(ary->is.array_range.type) {
    case VI_array:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name
        ) ;
        OUT_EXEC_STORE_SIG_G_ARRAY_SUBBIT ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ARRAY_SUBBIT ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_ARRAY_SUBBIT ;
      }
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "  ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "TMP_INT2 = tmp ; /* index to array */ \n" ) ;
      OUT0( "}\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_MARRAY_SUBBIT ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_MARRAY_SUBBIT ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_MARRAY_SUBBIT ;
      }
      break ;
    case VI_stridx:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_STRIDX_SUBBIT ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_STRIDX_SUBBIT ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_STRIDX_SUBBIT ;
      }
      break ;
    case VI_assoc:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ASSOC_SUBBIT ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ASSOC_SUBBIT ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_ASSOC_SUBBIT ;
      }
      break ;
  }
}

static void output_store_sig_array( vinst_info *code ) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  switch(ary->is.array_range.type) {
    case VI_array:
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2(
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name
        ) ;
        OUT_EXEC_STORE_SIG_G_ARRAY ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ARRAY ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_ARRAY ;
      }
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "  tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "  #ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "  }\n" ) ;
          OUT0( "  #endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "  TMP_INT2 = tmp ; /* index to array */ \n" ) ;
      OUT0( "}\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_MARRAY ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_MARRAY ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_MARRAY ;
      }
      break ;
    case VI_stridx:
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name ) ;
        OUT_EXEC_STORE_SIG_G_STRIDX ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_STRIDX ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_STRIDX ;
      }
      break ;
    case VI_assoc:
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ASSOC ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_ASSOC ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_ASSOC ;
      }
      break ;
  }
}

static void output_store_sig( vinst_info *code ) {
  if( code->isubtype == VI_subbit ) {
    /* subbit access */
    if( code->array_access )
      output_store_sig_array_subbit( code ) ;
    else {
      OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
      OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_SUBBIT ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G_SUBBIT ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L_SUBBIT ;
      }
    }
  }
  else {
    if( code->array_access )
      output_store_sig_array( code ) ;
    else {
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      if( code->itype == VI_store_sig_g ) {
        OUT2( 
          "TMP_INT = %d ; /* global %s */ \n",
          code->code_name->index, code->name
        ) ;
        OUT_EXEC_STORE_SIG_G ;
      }
      else if ( code->itype == VI_store_sig_static  ) {
        OUT4( 
          "TMP_INT = %d ; /* static %s.%s.%s */ \n",
          code->code_name->index, code->class, code->func, code->name 
        ) ;
        OUT_EXEC_STORE_SIG_G ;
      }
      else {
        OUT1( "TMP_INT = %d ; /* local var */ \n", code->index ) ;
        OUT_EXEC_STORE_SIG_L ;
      }
    }
  }
}

static void output_store_sig_m_single(  
  vinst_info *code, int index, char *lg, char *name
) {
  OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
  OUT0( "ACC_DT = NULL ; \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
  switch( code->sd_type->is.sd_type.b ) {
    case VI_int:
      OUT_EXEC_INT_TO_BIT ;
      break ;
    case VI_bit:
      break ;
    case VI_float:
      OUT_EXEC_FLOAT_TO_BIT ;
      break ;
    default:
      input_file_name = code->filename ;
      lex_line_num = code->linenum ;
      ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
      break ;
  }
  OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
  OUT3( "TMP_INT = %d ; /* %s.%s variable */ \n", index, lg, name ) ;
  OUT_EXEC_STORE_SIG_M ;
}

static void output_store_sig_m_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
  OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
  OUT0( "ACC_DT = NULL ; \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
  switch( code->sd_type->is.sd_type.b ) {
    case VI_int:
      OUT_EXEC_INT_TO_BIT ;
      break ;
    case VI_bit:
      break ;
    case VI_float:
      OUT_EXEC_FLOAT_TO_BIT ;
      break ;
    default:
      input_file_name = code->filename ;
      lex_line_num = code->linenum ;
      ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
      break ;
  }
  OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
  OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
  OUT_EXEC_STORE_SIG_M_SUBBIT ;
}

static void output_store_sig_m_array_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  switch(ary->is.array_range.type) {
    case VI_array:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_ARRAY_SUBBIT ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "  ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "TMP_INT2 = tmp ;\n" ) ;
      OUT0( "}\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_MARRAY_SUBBIT ;
      break ;
    case VI_stridx:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_STRIDX_SUBBIT ;
      break ;
    case VI_assoc:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_ASSOC_SUBBIT ;
      break ;
  }
}

static void output_store_sig_m_array(  
  vinst_info *code, int index, char *lg, char *name
) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  switch(ary->is.array_range.type) {
    case VI_array:
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_ARRAY ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        if( i ) OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "  ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "TMP_INT2 = tmp ;\n" ) ;
      OUT0( "}\n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_MARRAY ;
      break ;
    case VI_stridx:
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_STRIDX ;
      break ;
    case VI_assoc:
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "DRV_STRENGTH = %d ; \n", get_strength( code->strength ) ) ;
      OUT3( "TMP_INT = %d ; /* %s.%s */ \n", index, lg, name ) ;
      OUT_EXEC_STORE_SIG_M_ASSOC ;
      break ;
  }
}

static void output_store_sig_m( vinst_info *code ) {
  int index ;
  char *lg ;
  char *nm ;
  
  nm = code->name ;
  lg = code->class ;
  
  index = code->index ;
  
  if( code->isubtype == VI_subbit && code->array_access == NULL) {
    output_store_sig_m_subbit( code, index, lg, nm ) ;   
  }
  else if( code->array_access == NULL ) {
    /* single access */
    output_store_sig_m_single( code, index, lg, nm ) ;
  }
  else {
    /* array access */
    if( code->isubtype == VI_subbit ) 
      output_store_sig_m_array_subbit( code, index, lg, nm ) ;
    else
      output_store_sig_m_array( code, index, lg, nm ) ;
  }

}


/******************************************
 * Store Global 
 ******************************************/
 
static void output_storeg_single( 
  vinst_info *code, int index, char *lg, char *name
) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_INT ;
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_INT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_INT ;
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        case VI_string:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      if( code->sd_type->is.sd_type.b == VI_string ) {
        OUT_EXEC_STOREG_STRING_TO_SINGLE_BIT ;
      } 
      else {
        OUT_EXEC_STOREG_SINGLE_BIT ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_FLOAT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_FLOAT ;
          break ;
        case VI_float:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_FLT_DBL ;
      break ;
    case VI_string:
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_STOREG_SINGLE_STRING ;
          break ;
        case VI_null:
          OUT_EXEC_STOREG_SINGLE_STRING_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_signal:
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_STOREG_SINGLE_SIGNAL ;
          break ;
        case VI_null:
          OUT_EXEC_STOREG_SINGLE_SIGNAL_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_INT ;
      break ;
    case VI_obj:
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_STOREG_SINGLE_OBJ ;
          break ;
        case VI_null:
          OUT_EXEC_STOREG_SINGLE_OBJ_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_array:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_array:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_DT ;
      break ;
    case VI_marray:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_marray:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_DT ;
      break ;
    case VI_assoc:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_assoc:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_DT ;
      break ;
    case VI_stridx:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_stridx:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_DT ;
      break ;
    case VI_list:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_list:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_SINGLE_DT ;
      break ;  
  }
}

/* ARGSUSED */
static void output_storeg_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  int ub, lb ;

#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT2( 
      "TMP_INT = JD_%s_global_var_offset + %d ; /* global variable */ \n", 
      out_fname, code->code_name->index 
    ) ;
  }
  else {
#endif
    OUT1( "TMP_INT = %d ; /* global variable */ \n", code->code_name->index ) ;
#ifdef PRODUCT_CODE
  }
#endif
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT_EXEC_STOREG_SUBBIT ;
}

static void output_acc_data_convert( vinst_info *code ) 
{
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_INT ;
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_INT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        case VI_string:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_FLOAT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_FLOAT ;
          break ;
        case VI_float:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          break ;
        case VI_null:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          break ;
        case VI_null:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          break ;
        case VI_null:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_array:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_array:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_marray:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_marray:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_assoc:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_assoc:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_stridx:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_stridx:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_list:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_list:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;  
  }
}

static void output_storeg_array(  
  vinst_info *code, int index, char *lg, char *name
) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  switch(ary->is.array_range.type) {
    case VI_array:
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_ARRAY ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "TMP_INT2 = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "  TMP_INT2 += (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "  #ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "  }\n" ) ;
          OUT0( "  #endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_MARRAY ;
      break ;
    case VI_stridx:
      OUT0( "TMP_STRING = ACC_STRING ;\n" ) ;
      OUT0( "ACC_STRING = NULL ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_STRIDX ;
      break ;
    case VI_assoc:
      OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT_EXEC_STOREG_ASSOC ;
      break ;
  }
}

static void output_storeg_array_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  int ub, lb ;
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  switch(ary->is.array_range.type) {
    case VI_array:
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREG_ARRAY_SUBBIT ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "  ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "TMP_INT2 = tmp ;\n" ) ;
      OUT0( "}\n" ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREG_MARRAY_SUBBIT ;
      break ;
    case VI_stridx:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREG_STRIDX_SUBBIT ;
      break ;
    case VI_assoc:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
#ifdef PRODUCT_CODE
      if( !main_defined ) {
        OUT4( 
          "TMP_INT = JD_%s_global_var_offset + %d ; /* %s var \"%s\" */ \n", 
          out_fname, index, lg, name
        ) ;
      }
      else {
#endif
        OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
#ifdef PRODUCT_CODE
      }
#endif
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREG_ASSOC_SUBBIT ;
      break ;
  }
}


static void output_storeg( vinst_info *code ) {
  int index ;
  char *lg ;
  char *nm ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    ERROR_ILLEGAL_ACCESS_TO_GLOBAL_VARIABLE_IN_MODULE( code->name ) ;
  }
#endif
  nm = code->name ;
  lg = "global" ;
  
  index = code->code_name->index ;
  
  if( code->isubtype == VI_subbit && code->array_access == NULL ) {
    output_storeg_subbit( code, index, lg, nm ) ;   
  }
  else if( code->array_access == NULL ) {
    /* single access */
    output_storeg_single( code, index, lg, nm ) ;
  }
  else {
    /* array access */
    if( code->isubtype == VI_subbit )
      output_storeg_array_subbit( code, index, lg, nm ) ;
    else
      output_storeg_array( code, index, lg, nm ) ;
  }

}

static void output_store_static( vinst_info *code ) {
  int index ;
  int len ;
  char *lg ;
  char *cls, *fnc, *nm ;
  char *full_name ;
  
  cls = code->class ;
  fnc = code->func ;
  nm = code->name ;

  index = code->code_name->index ;

  lg = "static" ;
  len = strlen(cls)+strlen(fnc)+strlen(nm)+5 ;
  full_name = (char *)calloc( len, sizeof(char) ) ;
  strcat( full_name, cls ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, fnc ) ;
  strcat( full_name, "." ) ;
  strcat( full_name, nm ) ;
  
  if( code->isubtype == VI_subbit && code->array_access == NULL ) {
    output_storeg_subbit( code, index, lg, full_name ) ;   
  }
  else if( code->array_access == NULL ) {
    /* single access */
    output_storeg_single( code, index, lg, full_name ) ;
  }
  else {
    /* array access */
    if( code->isubtype == VI_subbit )
      output_storeg_array_subbit( code, index, lg, full_name ) ;
    else
      output_storeg_array( code, index, lg, full_name ) ;
  }
  
  free(full_name) ;
}

static void output_storel_single( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_INT ;
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_INT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_INT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_INT ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        case VI_string:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->sd_type->is.sd_type.b == VI_string ) {
          OUT_EXEC_STOREL_STRING_TO_SINGLE_BIT ;
      } 
      else {
        if( code->is_static ) {
          OUT_EXEC_STOREL_SINGLE_BIT_STATIC ;
        }
        else {
          OUT_EXEC_STOREL_SINGLE_BIT ;
        }
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_FLOAT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_FLOAT ;
          break ;
        case VI_float:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_FLT_DBL_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_FLT_DBL ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_STRING_STATIC ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_STRING ;
          }
          break ;
        case VI_null:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_STRING_STATIC_NULL ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_STRING_NULL ;
          }
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_SIGNAL_STATIC ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_SIGNAL ;
          }
          break ;
        case VI_null:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_SIGNAL_STATIC_NULL ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_SIGNAL_NULL ;
          }
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_INT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_INT ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_OBJ_STATIC ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_OBJ ;
          }
          break ;
        case VI_null:
          OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
          if( code->is_static ) {
            OUT_EXEC_STOREL_SINGLE_OBJ_STATIC_NULL ;
          }
          else {
            OUT_EXEC_STOREL_SINGLE_OBJ_NULL ;
          }
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_array:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_array:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_DT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_DT ;
      }
      break ;
    case VI_marray:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_marray:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_DT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_DT ;
      }
      break ;
    case VI_assoc:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_assoc:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_DT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_DT ;
      }
      break ;
    case VI_stridx:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_stridx:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_DT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_DT ;
      }
      break ;
    case VI_list:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_list:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      if( code->is_static ) {
        OUT_EXEC_STOREL_SINGLE_DT_STATIC ;
      }
      else {
        OUT_EXEC_STOREL_SINGLE_DT ;
      }
      break ;  
  }
}


static void output_storel_subbit( vinst_info *code ) {
  int ub, lb ;
  OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT_EXEC_STOREL_SUBBIT ;
}

static void output_storel_array_subbit( vinst_info *code ) {
  int ub, lb ;
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  switch(ary->is.array_range.type) {
    case VI_array:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREL_ARRAY_SUBBIT ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "  tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "  }\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "  TMP_INT2 = tmp ;\n" ) ;
      OUT0( "}\n" ) ;
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREL_MARRAY_SUBBIT ;
      break ;
    case VI_stridx:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREL_STRIDX_SUBBIT ;
      break ;
    case VI_assoc:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREL_ASSOC_SUBBIT ;
      break ;
  }
}

static void output_storel( vinst_info *code ) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  
  if( code->isubtype == VI_subbit && code->array_access == NULL ) {
    output_storel_subbit( code ) ;   
  }
  else if( code->array_access == NULL ) {
    /* single access */
    output_storel_single( code ) ;
  }
  else if( code->isubtype == VI_subbit ) {
    output_storel_array_subbit( code ) ;
  }
  else {
    vcode_info *ary = code->array_access ;
    switch(ary->is.array_range.type) {
      case VI_array:
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
          OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
        OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
        output_acc_data_convert( code ) ;
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_STOREL_ARRAY ;
        break ;
      case VI_marray:
        dim = ary->is.array_range.size ;
        pn = 1 ;
        OUT0( "TMP_INT2 = 0 ;\n" ) ;
        for( i = 0 ; i < dim ; i++ ) {
          dinfo = ary->is.array_range.range_list ;
          for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
          if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
          OUT1( "  TMP_INT2 += (ACC_INT * %d) ; \n", pn ) ;
          if( !inline_optimization ) {
            OUT0( "  #ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
            OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
            OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
            OUT0( "  }\n" ) ;
            OUT0( "  #endif\n" ) ;
          }
          pn = pn * dinfo->is.int_value ;
        }
        OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
        output_acc_data_convert( code ) ;
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_STOREL_MARRAY ;
        break ;
      case VI_stridx:
        OUT0( "TMP_STRING = ACC_STRING ;\n" ) ;
        OUT0( "ACC_STRING = NULL ;\n" ) ;
        OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
        output_acc_data_convert( code ) ;
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_STOREL_STRIDX ;
        break ;
      case VI_assoc:
        OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
        OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
        output_acc_data_convert( code ) ;
        OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
        OUT_EXEC_STOREL_ASSOC ;
        break ;
    }
  } 
}

/************************************/
/* Store Object Member              */
/************************************/

static void output_storem_single( 
  vinst_info *code, int index, char *lg, char *name
) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          break ;
        case VI_bit:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_BIT_TO_INT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        case VI_float:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_FLOAT_TO_INT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_INT ;
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_INT_TO_BIT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_FLOAT_TO_BIT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        case VI_string:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      if( code->sd_type->is.sd_type.b == VI_string ) {
        OUT_EXEC_STOREM_STRING_TO_SINGLE_BIT ;
      } 
      else {
        OUT_EXEC_STOREM_SINGLE_BIT ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_INT_TO_FLOAT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        case VI_bit:
          OUT0( "{ TMP_DT = ACC_DT ;\n" ) ;
          OUT_EXEC_POP_ACC ;
          OUT_EXEC_BIT_TO_FLOAT ;
          OUT_EXEC_PUSH_ACC ;
          OUT0( "ACC_DT = TMP_DT ; ACC_TYPE = JD_D_dt ;\n" ) ;
          OUT0( "}\n" ) ;
          break ;
        case VI_float:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_FLT_DBL ;
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_STRING ;
          break ;
        case VI_null:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_STRING_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_SIGNAL ;
          break ;
        case VI_null:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_SIGNAL_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_INT ;
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_OBJ ;
          break ;
        case VI_null:
          OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
          OUT_EXEC_STOREM_SINGLE_OBJ_NULL ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      break ;
    case VI_array:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_array:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_DT ;
      break ;
    case VI_marray:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_marray:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_DT ;
      break ;
    case VI_assoc:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_assoc:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_DT ;
      break ;
    case VI_stridx:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_stridx:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_DT ;
      break ;
    case VI_list:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_list:
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_SINGLE_DT ;
      break ;  
  }
}


/* ARGSUSED */
static void output_storem_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  int ub, lb ;
  OUT1( "TMP_INT = %d ; /* local variable */ \n", code->index ) ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT_EXEC_STOREM_SUBBIT ;
}

static void output_storem_array_subbit(  
  vinst_info *code, int index, char *lg, char *name
) {
  int ub, lb ;
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT0( "L_INDEX = ACC_INT ; /* lb index */ \n" ) ;
  OUT0( "CALL_EXEC_POP_ACC ; /* index to the ub */ \n" ) ;
  OUT0( "U_INDEX = ACC_INT ; /* ub index */ \n" ) ;
  switch(ary->is.array_range.type) {
    case VI_array:
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREM_ARRAY_SUBBIT ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "{  int tmp = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "tmp = tmp + (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "  ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "}\n" ) ;
          OUT0( "#endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "TMP_INT2 = tmp ;\n" ) ;
      OUT0( "}\n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREM_MARRAY_SUBBIT ;
      break ;
    case VI_stridx:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_STRING = ACC_STRING ; /* string index to array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ; /* clear to avoid GC */ \n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREM_STRIDX_SUBBIT ;
      break ;
    case VI_assoc:
      OUT0( "CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
      OUT0( "TMP_INT2 = ACC_INT ; /* index to array */ \n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object for stored */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ; /* object */ \n" ) ;
      OUT0( "ACC_DT = NULL ; \n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TO_BIT ;
          break ;
        case VI_bit:
          break ;
        case VI_float:
          OUT_EXEC_FLOAT_TO_BIT ;
          break ;
        default:
          input_file_name = code->filename ;
          lex_line_num = code->linenum ;
          ERROR_ILLEGAL_DATA_TYPE_FOR_ASSIGN ;
          break ;
      }
      OUT_EXEC_STOREM_ASSOC_SUBBIT ;
      break ;
  }
}

static void output_storem_array(  
  vinst_info *code, int index, char *lg, char *name
) {
  vcode_info *dinfo ;
  int i, j, pn, dim ;
  vcode_info *ary = code->array_access ;
  switch(ary->is.array_range.type) {
    case VI_array:
      if( !inline_optimization ) {
        OUT0( "#ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
        OUT1( "if( ACC_INT >= %d ) { \n", ary->is.array_range.size ) ;
        OUT0( "  ERROR_ARRAY_INDEX_ACCESS_OVERFLOW ; \n" ) ;
        OUT0( "}\n" ) ;
        OUT0( "#endif\n" ) ;
      }
      OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_ARRAY ;
      break ;
    case VI_marray:
      dim = ary->is.array_range.size ;
      pn = 1 ;
      OUT0( "TMP_INT2 = 0 ;\n" ) ;
      for( i = 0 ; i < dim ; i++ ) {
        dinfo = ary->is.array_range.range_list ;
        for( j = i ; j < dim-1 ; j++ ) dinfo = dinfo->next ;
        if( i ) OUT0( "  CALL_EXEC_POP_ACC ; /* index to the array */ \n" ) ;
        OUT1( "  TMP_INT2 += (ACC_INT * %d) ; \n", pn ) ;
        if( !inline_optimization ) {
          OUT0( "  #ifndef SKIP_ARRAY_BOUNDARY_CHECK\n" ) ;
          OUT1( "  if( ACC_INT >= %d ) { \n", dinfo->is.int_value ) ;
          OUT1( "    ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(%d) ; \n", i+1 ) ;
          OUT0( "  }\n" ) ;
          OUT0( "  #endif\n" ) ;
        }
        pn = pn * dinfo->is.int_value ;
      }
      OUT0( "CALL_EXEC_POP_ACC ; /* object */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT_EXEC_STOREM_MARRAY ;
      break ;
    case VI_stridx:
      OUT0( "TMP_STRING = ACC_STRING ; /* index to the array */ \n" ) ;
      OUT0( "ACC_STRING = NULL ;\n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
      OUT_EXEC_STOREM_STRIDX ;
      break ;
    case VI_assoc:
      OUT0( "TMP_INT2 = ACC_INT ;\n" ) ;
      OUT3( "TMP_INT = %d ; /* %s var \"%s\" */ \n", index, lg, name ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* object */ \n" ) ;
      OUT0( "TMP_DT = ACC_DT ;\n" ) ;
      OUT0( "CALL_EXEC_POP_ACC ; /* data to be stored */ \n" ) ;
      output_acc_data_convert( code ) ;
      OUT_EXEC_STOREM_ASSOC ;
      break ;
  }
}

static void output_storem( vinst_info *code ) {
  int index ;
  char *lg ;
  char *nm ;
  
  nm = code->name ;
  lg = code->class ;
  
  index = code->index ;
  
  if( code->isubtype == VI_subbit && code->array_access == NULL) {
    output_storem_subbit( code, index, lg, nm ) ;   
  }
  else if( code->array_access == NULL ) {
    /* single access */
    output_storem_single( code, index, lg, nm ) ;
  }
  else {
    /* array access */
    if( code->isubtype == VI_subbit ) 
      output_storem_array_subbit( code, index, lg, nm ) ;
    else
      output_storem_array( code, index, lg, nm ) ;
  }

}

/* store_obj: store data to a member of object at the top of stack */
static void output_store_obj( vinst_info *code ) {
  char *nm, *lg ;
  int index ;
  nm = code->name ;
  lg = code->class ;
  
  
  OUT_EXEC_PUSH_ACC ;  /* push data to be stored */
  OUT0( "ACC_DT = STACK[SP-1] ; /* object at the stack top */\n" ) ;
  OUT0( "ACC_TYPE = JD_D_dt ;\n" ) ;

  index = code->index ;
  
  output_storem_single( code, index, lg, nm ) ;

}


/****************************************************************/
/* store_adjust: ajusting the top of stack as the last store    */
/*  this element is used to implement '{<list>} = data ;        */
/****************************************************************/
static void output_store_adjust( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_STORE_ADJUST_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_STORE_ADJUST_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for store_adjust!#^*&^\n" 
      ) ;
      break ;
  }
}

/************************************/
/* signal access instructions       */
/************************************/

/* ARGSUSED */
static void output_sample( vinst_info *code ) {
  /* TBI */
  fprintf( stderr, "output_sample() not implemented yet!!\n" ) ;
}        

static void output_sync( vinst_info *code ) {
  switch( code->edge ) {
    case VI_posedge:
      OUT0( "TMP_INT2 = JEDA_POSEDGE ; \n" ) ;
      break ;
    case VI_negedge:
      OUT0( "TMP_INT2 = JEDA_NEGEDGE ; \n" ) ;
      break ;
    case VI_bothedge:
      OUT0( "TMP_INT2 = JEDA_BOTHEDGE ; \n" ) ;
      break ;
    case VI_noedge:
      OUT0( "TMP_INT2 = JEDA_NOEDGE ; \n" ) ;
      break ;
  }
  switch( code->isubtype ) {
    case VI_clk_of_pset_port:
      OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
      OUT_EXEC_SYNC_CLOCK_OF_PORT ;
      break ;
    case VI_clk_of_port:
      OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
      OUT_EXEC_SYNC_CLOCK_OF_PORT ;
      break ;
    case VI_clk_of_signal:
      OUT_EXEC_SYNC_CLOCK_OF_SIGNAL ;
      break ;
    case VI_pset_port:
      OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
      OUT_EXEC_SYNC_PORT ;
      break ;
    case VI_port:
      OUT1( "TMP_INT = %d ; \n", code->code_name->index ) ;
      OUT_EXEC_SYNC_PORT ;
      break ;
    case VI_signal:
      OUT_EXEC_SYNC_SIGNAL ;
      break ;
    default:
      fprintf( stderr, "Unknown type for lvar\n" ) ;
      FATAL_ERROR ;
      break ;
  }
}        

/************************************/
/* call/return instructions         */
/************************************/

static void output_call( vinst_info *code ) {
  JD_name *func ;
  int is_leaf ;

  func = code->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    is_leaf = 1 ;
  }
  else is_leaf = 0 ;

#ifdef PRODUCT_CODE
  if( code->index ) {
#endif  
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif  
  
  OUT1( "NUM_ARG = %0d ;\n", code->index ) ;
  
  if( is_leaf ) {
    if( code->func_type == JD_sys_func_name || 
        code->func_type == JD_sys_mfunc_name   ) 
    {
      OUT1( "JD_sf_%s() ; /* calling leaf function */\n", code->func ) ;
    }
    else 
    {
      OUT0( "SET_ENTER_FUNC ;\n" ) ;
      OUT1( "JD_uf_%s() ; /* calling leaf function */\n", code->func ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
  }
  else {
    if( code->func_type == JD_sys_func_name || 
        code->func_type == JD_sys_mfunc_name   ) 
    {
      OUT1( 
        "JD_sf_%s(NULL) ; /* calling non-leaf function */\n", code->func 
      ) ;
    }
    else {
      OUT0( "SET_ENTER_FUNC ;\n" ) ;
      OUT1( 
        "JD_uf_%s(NULL) ; /* calling non-leaf function */\n", code->func 
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
  }
  if( !inline_optimization && !inline_optimization &&
      code->func_type != JD_sys_func_name && 
      code->func_type != JD_sys_mfunc_name &&
      func->info && func->info->is.function_info.data_type && 
      func->info->is.function_info.data_type->is.data_type.type != VI_void )
  {
    OUT0 ( "#ifndef JEDA_SKIP_FUNC_RETURN_CHECK\n" ) ;
    OUT0 ( "if( ACC_TYPE == JD_D_void ) {\n" ) ;
    OUT1 ( 
      "  ERROR_FUNCTION_RETURNING_WITHOUT_VALUE(\"%s\") ; \n", code->func 
    ) ;
    OUT0 ( "}\n" ) ;
    OUT0 ( "#endif\n" ) ;
  }
}        

static void output_call_m( vinst_info *code ) {
  JD_name *func ;
  int is_leaf ;
  
  func = code->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    is_leaf = 1 ;
  }
  else is_leaf = 0 ;
  
#ifdef PRODUCT_CODE
  if( code->index ) {
#endif
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif

  OUT1( "NUM_ARG = %0d ;\n", code->index ) ;

  if( is_leaf ) 
  {
    if( code->func_type == JD_sys_func_name || 
        code->func_type == JD_sys_mfunc_name   ) 
    {
      OUT2( 
        "JD_sc_%s_mf_%s() ; /* calling sys function */\n", 
        code->class, code->func 
      ) ;
    }
    else {
      OUT0( "SET_ENTER_MFUNC ;\n" ) ;
      OUT2( 
        "JD_uc_%s_mf_%s() ; /* calling leaf function */\n", 
        code->class, code->func 
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
  }
  else {
    if( code->func_type == JD_sys_func_name || 
        code->func_type == JD_sys_mfunc_name   ) 
    {
      OUT2( 
        "JD_sc_%s_mf_%s(NULL) ; /* calling sys function */\n", 
        code->class, code->func 
      ) ;
    }
    else {
      OUT0( "SET_ENTER_MFUNC ;\n" ) ;
      OUT2( 
        "JD_uc_%s_mf_%s(NULL) ; /* calling non-leaf function */\n", 
        code->class, code->func 
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
  }
  if( !inline_optimization && strcmp( code->func, "new" ) &&
      !inline_optimization &&
      code->func_type != JD_sys_func_name && 
      code->func_type != JD_sys_mfunc_name &&
      func->info && func->info->is.function_info.data_type && 
      func->info->is.function_info.data_type->is.data_type.type != VI_void )
  {
    OUT0 ( "#ifndef JEDA_SKIP_FUNC_RETURN_CHECK\n" ) ;
    OUT0 ( "if( ACC_TYPE == JD_D_void ) {\n" ) ;
    OUT1 ( 
      "  ERROR_FUNCTION_RETURNING_WITHOUT_VALUE(\"%s\") ; \n", code->func 
    ) ;
    OUT0 ( "}\n" ) ;
    OUT0 ( "#endif\n" ) ;
  }
}        

static void output_call_a( vinst_info *code ) {
  JD_name *func ;
  int is_leaf ;
  
  func = code->fname ;
  if( func->info && 
      func->info->is.function_info.func_options &&
      ( func->info->is.function_info.func_options->is.attrib.is_leaf ||
        func->info->is.function_info.func_options->is.attrib.is_abstract )
    )
  {
    is_leaf = 1 ;
  }
  else is_leaf = 0 ;
  
#ifdef PRODUCT_CODE
  if( code->index ) {
#endif
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif

  OUT1( "NUM_ARG = %0d ;\n", code->index ) ;

  if( is_leaf ) 
  {
    OUT0( "SET_ENTER_AFUNC ;\n" ) ;
    OUT2( 
      "JD_uc_%s_mf_%s() ; /* calling leaf function */\n", 
      code->class, code->func 
    ) ;
    OUT0( "SET_EXIT_FUNC ;\n" ) ;
  }
  else {
    OUT0( "SET_ENTER_AFUNC ;\n" ) ;
    OUT2( 
      "JD_uc_%s_mf_%s(NULL) ; /* calling non-leaf function */\n", 
      code->class, code->func 
    ) ;
    OUT0( "SET_EXIT_FUNC ;\n" ) ;
  }
  if( !inline_optimization && 
      code->func_type != JD_sys_func_name && 
      code->func_type != JD_sys_mfunc_name &&
      func->info && func->info->is.function_info.data_type && 
      func->info->is.function_info.data_type->is.data_type.type != VI_void )
  {
    OUT0 ( "#ifndef JEDA_SKIP_FUNC_RETURN_CHECK\n" ) ;
    OUT0 ( "if( ACC_TYPE == JD_D_void ) {\n" ) ;
    OUT1 ( 
      "  ERROR_FUNCTION_RETURNING_WITHOUT_VALUE(\"%s\") ; \n", code->func 
    ) ;
    OUT0 ( "}\n" ) ;
    OUT0 ( "#endif\n" ) ;
  }
}        

static void output_veri_call( vinst_info *code ) {
  JD_name *func ;
  func = find_verilog_name( code->func ) ;
#ifdef PRODUCT_CODE
  if( code->index ) {
#endif
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif
  OUT1( "NUM_ARG = %0d ;\n", code->index ) ;
  OUT1( "TMP_INT = %d ;\n", func->info->index ) ;
  OUT0( "JD_veri_task_call() ;\n" ) ;
  
}        

/* ARGSUSED */
static void output_return( vinst_info *code ) {
  if( in_main_function ) {
    /* OUT0( "JD_thread_return() ;\n" ) ; */
  }
  else if( !in_leaf_function ) {
    OUT0( "if( mythread ) JD_thread_return() ;\n" ) ;
  }
  OUT0( "SP = THREAD->stk_frame ;\n" ) ;
  OUT0( "THREAD->stk_frame = STACK[SP]->data.int_value ;\n" ) ;
  if( in_main_function ) {
    OUT0( "JEDA_end_of_simulation() ;\n" ) ;
    OUT0( "JD_thread_kill(THREAD) ;\n" ) ;
  }  
  OUT0( "return ; \n" ) ;
}        

/* ARGSUSED */
static void output_return_l( vinst_info *code ) {
  OUT0( "SP = THREAD->stk_frame ;\n" ) ;
  OUT0( "THREAD->stk_frame = STACK[SP]->data.int_value ;\n" ) ;
  OUT0( "return ; \n" ) ;
}        

/* ARGSUSED */
static void output_return_n( vinst_info *code ) {
  OUT0( "return ; \n" ) ;
}        

/************************************/
/* output_lvar: 'lvar' instruction  */
/************************************/

static void output_lvar( vinst_info *code ) {
  int size, dim ;
  vcode_info *dinfo ;

#ifdef PRODUCT_CODE
  if( !code->no_flush ) {
#endif
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif
  
  switch( code->array_range->is.array_range.type ) {
  
  case VI_single:
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_SINGLE_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_SINGLE_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_SINGLE_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_SINGLE_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_SINGLE_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_SINGLE_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_SINGLE_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_SINGLE_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_SINGLE_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          } 
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    
  case VI_array:
    OUT1( "ARRAY_SIZE = %d ;\n", code->array_range->is.array_range.size ) ;
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_ARRAY_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_ARRAY_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_ARRAY_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_ARRAY_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ARRAY_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ARRAY_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_ARRAY_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_ARRAY_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_ARRAY_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    
  case VI_marray:
    dim = code->array_range->is.array_range.size ;
    size = 1 ;
    dinfo = code->array_range->is.array_range.range_list ;
    while(dinfo) {
      size *= dinfo->is.int_value ;
      dinfo = dinfo->next ;
    }
    OUT1( "ARRAY_SIZE = %d ;\n", size  ) ;
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_ARRAY_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_ARRAY_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_ARRAY_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_ARRAY_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ARRAY_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ARRAY_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_ARRAY_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_ARRAY_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_ARRAY_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) { 
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    OUT1( "ARRAY_SIZE = %d ;\n", dim ) ;
    OUT0( "CALL_ALLOC_MARRAY_DIM ; \n" ) ;
    dinfo = code->array_range->is.array_range.range_list ;
    while(dinfo) {
      OUT1( "ARRAY_SIZE = %d ;\n", dinfo->is.int_value ) ;
      OUT0( "CALL_SET_MARRAY_DIM ; \n" ) ;
      dinfo = dinfo->next ;
    }
    break ;

  case VI_assoc:
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_ASSOC_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_ASSOC_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_ASSOC_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_ASSOC_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ASSOC_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_ASSOC_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_ASSOC_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_ASSOC_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_ASSOC_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_STRIDED_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_STRIDED_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_STRIDED_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_STRIDED_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_STRIDED_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = JD_sa_%s() ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_STRIDED_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_STRIDED_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_STRIDED_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_STRIDED_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_LIST_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_LIST_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_LIST_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_LIST_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_LIST_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_LIST_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_LIST_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_LIST_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_LIST_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
  if( code->opt_flags && code->opt_flags->is.attrib.is_clone ) {
    OUT0( "CLONE(TMP_DT) ;  /* clone type */\n" ) ;
  }
}

/*********************************************/
/* output_static: 'lvar static' instruction  */
/*********************************************/

static void output_static( vinst_info *code ) {
  char *cls, *fnc, *nm ;
  
#ifdef PRODUCT_CODE
  if( !code->no_flush ) {
#endif
    OUT_EXEC_FLUSH_VSTACK ;
#ifdef PRODUCT_CODE
  }
#endif
  cls = code->class ;
  fnc = code->func ;
  nm = code->name ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT5( 
      "TMP_INT = JD_%s_global_var_offset + %d ; /* static var \"%s.%s.%s\" */ \n", 
      out_fname, code->code_name->index, cls, fnc, nm
    ) ;
  }
  else {
#endif
    OUT4( 
      "TMP_INT = %d ; /* static var \"%s.%s.%s\" */ \n", 
      code->code_name->index, cls, fnc, nm
    ) ;
#ifdef PRODUCT_CODE
  }
#endif
  OUT_EXEC_LVAR_STATIC ;
  
}

/************************************/
/* output_arg: 'arg' instruction    */
/************************************/
static void output_var_arg( vinst_info *code ) {
  OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
  OUT_EXEC_VAR_ARG ;
}

static void output_arg( vinst_info *code ) {
  int size ;
  
  if( code->is_var ) {
    output_var_arg( code ) ;
  }
  else switch( code->array_range->is.array_range.type ) {
  
  case VI_single:
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_LVAR_SINGLE_INT ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_INT ;
        break ;
      case VI_float:
        OUT_EXEC_LVAR_SINGLE_FLOAT ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_LVAR_SINGLE_DOUBLE ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = 
            code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_LVAR_SINGLE_BIT ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_SINGLE_OBJ ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_LVAR_SINGLE_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_OBJ ;
        break ;
      case VI_string:
        OUT_EXEC_LVAR_SINGLE_STRING ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_LVAR_SINGLE_SIGNAL ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_LVAR_SINGLE_INT ;
        OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
        OUT_EXEC_COPY_ARG_SINGLE_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    
  case VI_array:
  case VI_marray:
  case VI_assoc:
  case VI_stridx:
  case VI_list:
    OUT1( "TMP_INT = %d ; /* offset */ \n", code->offset ) ;
    OUT_EXEC_ARG_NON_SINGLE ;
    break ;
    
  }
}        


/************************************/
/* object alloc/access instructions */
/************************************/
/* ARGSUSED */
static void output_new( vinst_info *code ) {
  OUT_EXEC_NEW ;
}

static void output_new_obj( vinst_info *code ) {
  JD_name *sname ;
  
  sname = find_global_name( code->name ) ;
  if( sname && sname->type == JD_sys_class_name ) {
    OUT0( "{ JD_data **tbl ; \n" ) ;
    OUT2( 
      "  tbl = (void *)JD_sa_%s() ; /* new_sysobj %s */ \n", 
      code->name,
      code->name 
    ) ;
    OUT0( "  CLEAR_ACC ; \n" ) ;
    OUT0( "  ACC_DT = JD_alloc_data_entry() ;\n" ) ;
    OUT0( "  ACC_DT->type = JD_D_object ;\n" ) ;
    OUT0( "  ACC_TYPE = JD_D_dt ;\n" ) ;
    OUT1( "  ACC_DT->data.obj.alloc = (void *)JD_sa_%s ;\n", code->name ) ;
    OUT0( "  ACC_DT->data.obj.ect = ALLOC_OBJ_TABLE ; \n" ) ;
    OUT0( "  ACC_DT->data.obj.ect->ref_count = 1 ; \n" ) ;
    OUT0( "  ACC_DT->data.obj.ect->table = tbl ; \n" ) ;
    OUT0( "  SETSYSOBJECT(ACC_DT) ;\n" ) ;
    OUT1( 
      "  ACC_DT->data.obj.ect->dealloc = (void *)JD_sd_%s ;\n", code->name 
    ) ;
    OUT1( 
      "  ACC_DT->data.obj.ect->duplicate = (void *)JD_dup_%s ; \n", code->name 
    ) ;
    OUT0( "#ifdef JEDA_DEBUGGER \n" ) ;
    OUT1( 
      "  ACC_DT->data.obj.ect->display = (void *)JD_disp_%s ; \n", code->name 
    ) ;
    OUT0( "#endif\n" ) ;
    OUT0( "}\n" ) ;
  }
  else {
    OUT2( 
      "JD_ua_%s() ; /* new_obj %s */ \n", 
      code->name,
      code->name 
    ) ;
    OUT_EXEC_POP_ACC ;
  }
}

/* ARGSUSED */
static void output_gen_child( vinst_info *code ) {
  OUT_EXEC_GEN_CHILD ;
}

/* ARGSUSED */
static void output_copy_obj( vinst_info *code ) {
  OUT_EXEC_COPY_OBJ ;
}

static void output_get_super( vinst_info *code ) {
  OUT1( "TMP_INT = %d ;\n", code->index ) ;
  OUT_EXEC_GET_SUPER ;
}

static void output_get_child( vinst_info *code ) {
  OUT1( "TMP_INT = %d ;\n", code->index ) ;
  OUT_EXEC_GET_CHILD ;
}
  
/* ARGSUSED */
static void output_get_this( vinst_info *code ) {
  OUT_EXEC_GET_THIS ;
}
  

static void output_alloc( vinst_info *code ) {
  int size, dim ;
  vcode_info *dinfo ;
  
  switch( code->array_range->is.array_range.type ) {
  
  case VI_single:
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_SINGLE_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_SINGLE_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_SINGLE_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_SINGLE_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_SINGLE_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_SINGLE_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_SINGLE_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_SINGLE_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_SINGLE_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    
  case VI_array:
    OUT1( "ARRAY_SIZE = %d ;\n", code->array_range->is.array_range.size ) ;
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_ARRAY_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_ARRAY_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_ARRAY_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_ARRAY_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ARRAY_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ARRAY_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_ARRAY_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_ARRAY_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_ARRAY_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    
  case VI_marray:
    dim = code->array_range->is.array_range.size ;
    size = 1 ;
    dinfo = code->array_range->is.array_range.range_list ;
    while(dinfo) {
      size *= dinfo->is.int_value ;
      dinfo = dinfo->next ;
    }
    OUT1( "ARRAY_SIZE = %d ;\n", size  ) ;
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_ARRAY_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_ARRAY_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_ARRAY_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_ARRAY_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ARRAY_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ARRAY_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_ARRAY_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_ARRAY_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_ARRAY_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    OUT1( "ARRAY_SIZE = %d ;\n", dim ) ;
    OUT0( "CALL_ALLOC_MARRAY_DIM ; \n" ) ;
    dinfo = code->array_range->is.array_range.range_list ;
    while(dinfo) {
      OUT1( "ARRAY_SIZE = %d ;\n", dinfo->is.int_value ) ;
      OUT0( "CALL_SET_MARRAY_DIM ; \n" ) ;
      dinfo = dinfo->next ;
    }
    break ;

  case VI_assoc:
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_ASSOC_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_ASSOC_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_ASSOC_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_ASSOC_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ASSOC_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_ASSOC_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_ASSOC_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_ASSOC_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_ASSOC_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_STRIDED_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_STRIDED_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_STRIDED_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_STRIDED_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_STRIDED_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = JD_sa_%s() ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_STRIDED_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_STRIDED_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_STRIDED_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_STRIDED_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) {
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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
    switch( code->data_type->is.data_type.type ) {
      case VI_int:
        OUT_EXEC_ALLOC_LIST_INT ;
        break ;
      case VI_float:
        OUT_EXEC_ALLOC_LIST_FLOAT ;
        break ;
      case VI_double:
        OUT_EXEC_ALLOC_LIST_DOUBLE ;
        break ;
      case VI_bit:
        if( code->data_type->is.data_type.ub > code->data_type->is.data_type.lb ) 
          size = code->data_type->is.data_type.ub - code->data_type->is.data_type.lb ;
        else 
          size = code->data_type->is.data_type.lb - code->data_type->is.data_type.ub ;
        OUT1( "BIT_SIZE = %0d ;\n", size+1 ) ;
        OUT_EXEC_ALLOC_LIST_BIT ;
        break ;
      case VI_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_ua_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_LIST_OBJ ;
        break ;
      case VI_sys_class:
        OUT1( 
          "ALLOC_OBJ = (void *)JD_sa_%s ; \n", 
          code->data_type->is.data_type.name 
        ) ;
        OUT_EXEC_ALLOC_LIST_OBJ ;
        OUT0( "SETSYSOBJECT(TMP_DT) ;\n" ) ;
        break ;
      case VI_string:
        OUT_EXEC_ALLOC_LIST_STRING ;
        break ;
      case VI_signal:
        OUT_EXEC_ALLOC_LIST_SIGNAL ;
        break ;
      case VI_enum:
        OUT_EXEC_ALLOC_LIST_INT ;
        if( JL_debug_flag ) {
#ifdef PRODUCT_CODE
          if( !main_defined ) { 
            OUT2( 
              "SET_ENUM( JD_%s_Debug_name_table_offset+%d ) ;\n", 
              out_fname,
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
          }
          else {
#endif  
            OUT1( 
              "SET_ENUM( %d ) ;\n", 
              debug_name_lookup( code->data_type->is.data_type.name)
            ) ;
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

static void output_alloc_static( vinst_info *code ) {
  char *cls, *fnc, *nm ;
  
  cls = code->class ;
  fnc = code->func ;
  nm = code->name ;
  
#ifdef PRODUCT_CODE
  if( !main_defined ) {
    OUT5( 
      "TMP_INT = JD_%s_global_var_offset + %d ; /* static var \"%s.%s.%s\" */ \n", 
      out_fname, code->code_name->index, cls, fnc, nm
    ) ;
  }
  else {
#endif
    OUT4( 
      "TMP_INT = %d ; /* static var \"%s.%s.%s\" */ \n", 
      code->code_name->index, cls, fnc, nm
    ) ;
#ifdef PRODUCT_CODE
  }
#endif
  OUT_EXEC_ALLOC_STATIC ;
}

/************************************/
/* class overwrite instructions     */
/************************************/

static void output_alloc_func( vinst_info *code ) {
  OUT2( 
    "TMP_INT = %d ; /* function \"%s\" */ \n", 
    code->index, code->name
  ) ;
  OUT_EXEC_ALLOC_FUNC ;
}

static void output_overwrite( vinst_info *code ) {
  OUT5( 
    "TMP_INT = %d ; /* function \"%s.%s -> %s.%s\" */ \n", 
    code->index, code->parent, code->func, code->class, code->func
  ) ;
  OUT2( "ALLOC_OBJ = JD_uc_%s_mf_%s ;\n", code->class, code->func ) ;
  OUT_EXEC_OVERWRITE ;
}

static void output_chk_overwrite( vinst_info *code ) {
#ifdef PRODUCT_CODE
  if( !inline_optimization ) {
#endif
    OUT0( "if( ACC_DT == NULL ) {  \n" ) ;
    OUT0( "  ERROR_EMPTY_OBJECT_ACCESS ; return ;\n" ) ;
    OUT0( "} \n" );
#ifdef PRODUCT_CODE
  }
#endif
  OUT2( 
    "TMP_INT = %d ; /* function \"%s\" */ \n", 
    code->index, code->func
  ) ;
  OUT0( "if( ACC_DT->data.obj.ect ) {  \n" ) ;
  OUT0( "  JD_data *dt ; \n" ) ;
  OUT0( "  TMP_DT = ACC_DT->data.obj.ect->table[TMP_INT] ; \n" ) ;
  OUT0( "  if( TMP_DT->data.func.func ) { \n" ) ;
  OUT0( "    ACC_DT = TMP_DT->data.func.data ;  /* replace this */ \n" ) ;
  OUT0( "    SET_ENTER_CHILD_MFUNC ;\n" ) ;
  OUT0( "    (TMP_DT->data.func.func)() ; \n" ) ;
  OUT0( "    return ; \n" ) ;
  OUT0( "  } \n" ) ;
  OUT0( "} \n" ) ;
}

/* ARGSUSED */
static void output_error( vinst_info *code ) {
  OUT_EXEC_ERROR ;
}


/************************************/
/* breakpoint instructions          */
/************************************/
/* ARGSUSED */
static void output_breakpoint( vinst_info *code ) {
  if( code->name && code->name[0] != '\0' ) {
    OUT1( "JD_exec_breakpoint( \"%s\" ) ;\n", code->name ) ;
  }
  else {
    OUT0( "JD_exec_breakpoint( NULL ) ;\n" ) ;
  }
}
 
/************************************/
/* misc ALU operation instructions  */
/************************************/
/*  duplicate  (<type>)       :  ACC <- duplicate(ACC,stack[sp-1]) ; */
/*
  'duplicate' stype EOL
  <<aluops>>
*/
static void output_duplicate( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_string:
      OUT_EXEC_DUPLICATE_STRING ;
      break ;
    case VI_int:
    case VI_bit:
      OUT_EXEC_DUPLICATE_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for duplicate!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_list( vinst_info *code ) {
  int i ;
  
  OUT0( " TMP_DT = JD_alloc_data_entry() ; \n" ) ;
  OUT0( " TMP_DT->sub_type = JD_SD_list ; \n" ) ;
  for( i = 0 ; i < code->index ; i++ ) { 
    /* pop data and chain to list */
    OUT_EXEC_LIST ;
  }
  OUT0( "ACC_DT = TMP_DT ; \n" ) ;
  OUT0( "ACC_TYPE = JD_D_dt ; \n" ) ;
}
  
static void output_get_subbit( vinst_info *code ) {
  int ub, lb ;
  if( code->bit_type == NULL ) {
    OUT0( "I_ENDIAN = 0 ;\n" ) ;
  }
  else {
    if( code->bit_type->is.bit_type.type == VI_i_endian ) {
      OUT0( "I_ENDIAN = 1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
      
    }
    else { /* nz_lsb */
      OUT0( "I_ENDIAN = -1 ;\n" ) ;
      ub = code->bit_type->is.bit_type.ub ;
      lb = code->bit_type->is.bit_type.lb ;
      OUT1( "U_BIT = %d ; \n", ub ) ;
      OUT1( "L_BIT = %d ; \n", lb ) ;
    }
  }
  OUT_EXEC_GET_SUBBIT ;
}
  
static void output_inc( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_float:
      OUT0( "ACC_FLOAT++ ;  /* inc instruction */\n" ) ;
      break ;
    case VI_int:
      OUT0( "ACC_INT++ ;  /* inc instruction */\n" ) ;
      break ;
    case VI_bit:
      OUT_EXEC_INC_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for inc!#^*&^\n" 
      ) ;
      break ;
  }
}
  
static void output_dec( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_float:
      OUT0( "ACC_FLOAT-- ;  /* dec instruction */\n" ) ;
      break ;
    case VI_int:
      OUT0( "ACC_INT-- ;  /* dec instruction */\n" ) ;
      break ;
    case VI_bit:
      OUT_EXEC_DEC_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for dec!#^*&^\n" 
      ) ;
      break ;
  }
}
  
static void output_cmp( vinst_info *code ) {

  OUT_EXEC_TOP_OF_STACK ;  /* TMP_DT points to the top of stack data */
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT0(
            "ACC_INT = (ACC_INT==TMP_DT->data.int_value)?1:0 ; /* cmp ii*/\n" 
          ) ;
          break ;
        case VI_bit:
          OUT_EXEC_CMP_INT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_CMP_INT_FLOAT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CMP_BIT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_CMP_BIT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_CMP_BIT_FLOAT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CMP_FLOAT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_CMP_FLOAT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_CMP_FLOAT_FLOAT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_CMP_STRING_STRING ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_CMP_SIGNAL_SIGNAL ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          OUT0(
            "ACC_INT = (ACC_INT==TMP_DT->data.int_value)?1:0 ; /*cmp enm*/\n" 
          ) ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_CMP_OBJ_OBJ ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }
}
  
static void output_cmpx( vinst_info *code ) {

  OUT_EXEC_TOP_OF_STACK ;  /* TMP_DT points to the top of stack data */
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT0(
            "ACC_INT = (ACC_INT==TMP_DT->data.int_value)?1:0 ; /* cmp ii*/\n" 
          ) ;
          break ;
        case VI_bit:
          OUT_EXEC_CMPX_INT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CMPX_BIT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_CMPX_BIT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_cmpz( vinst_info *code ) {
  OUT_EXEC_TOP_OF_STACK ;  /* TMP_DT points to the top of stack data */
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT0(
            "ACC_INT = (ACC_INT==TMP_DT->data.int_value)?1:0 ; /* cmp ii*/\n" 
          ) ;
          break ;
        case VI_bit:
          OUT_EXEC_CMPZ_INT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CMPZ_BIT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_CMPZ_BIT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_concat( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CONCAT_INT_INT
          break ;
        case VI_bit:
          OUT_EXEC_CONCAT_INT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_CONCAT_BIT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_CONCAT_BIT_BIT ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_CONCAT_STRING_STRING ;
          break ;
        default:
          fprintf( 
            stderr, "!!! illegal type for cmp!#^*&^\n" 
          ) ;
          break ;
      }
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }
}
  
/* ARGSUSED */
static void output_to_int( vinst_info *code ) {
  OUT_EXEC_TO_INT ;
}
  
static void output_convert( vinst_info *code ) {
  int size ;
  
  switch( code->isubtype ) {
    case VI_bit:
      if( code->ub > code->lb ) size = code->ub - code->lb + 1 ;
      else size = code->lb - code->ub + 1 ;
      switch( code->sd_type->is.sd_type.a ) {
        case VI_int:
          OUT1( "TMP_INT = %d ;\n", size ) ;
          OUT_EXEC_INT_TO_BIT_SIZE ;
          break ;
        case VI_bit:
          OUT1( "TMP_INT = %d ;\n", size ) ;
          OUT_EXEC_BIT_TO_BIT_SIZE ;
          break ;
        case VI_float:
        case VI_double:
          OUT1( "TMP_INT = %d ;\n", size ) ;
          OUT_EXEC_FLOAT_TO_BIT_SIZE ;
          break ;
        case VI_string:
          OUT1( "TMP_INT = %d ;\n", size ) ;
          OUT_EXEC_STRING_TO_BIT_SIZE ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.a ) {
        case VI_int:
          OUT_EXEC_INT_TO_FLOAT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TO_FLOAT ;
          break ;
      }
      break ;
  }
}


static void output_lshift( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_LSHIFT_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_LSHIFT_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }
}
  
static void output_rshift( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_RSHIFT_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_RSHIFT_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }
}
  
static void output_urshift( vinst_info *code ) {
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_URSHIFT_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_RSHIFT_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for cmp!#^*&^\n" 
      ) ;
      break ;
  }
}
  
/* ARGSUSED */
static void output_null( vinst_info *code ) {
  OUT0( "CLEAR_ACC ; \n" ) ;
  OUT0( "ACC_TYPE = JD_D_null ; /* null instruction */\n" ) ;
}
  
/* ARGSUSED */
static void output_void( vinst_info *code ) {
  OUT0( "CLEAR_ACC ; \n" ) ;
  OUT0( "ACC_TYPE = JD_D_void ; /* void instruction */\n" ) ;
}
  
/**************************************/
/* binary ALU operation instructions  */
/**************************************/
static void output_Qeqeq( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_QEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_QEQEQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_QEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_QEQEQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_Qne( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_QNE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_QNE_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_QNE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_QNE_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}

static void output_and( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_AND_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_AND_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_AND_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_AND_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_div( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_DIV_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_DIV_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_DIV_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_DIV_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_DIV_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_DIV_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_DIV_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_DIV_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_DIV_F ;
          break ;
        default:
         illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_eor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_EOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_EOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_EOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_EOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_eqQeq( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_EQQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_EQQEQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_EQQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_EQQEQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_eqeq( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_EQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_EQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_EQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_EQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_EQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_EQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_EQEQ_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_EQEQ_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_EQEQ_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_STRING_EQEQ_STRING ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_SIGNAL_EQEQ_SIGNAL ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          OUT_EXEC_INT_EQEQ_INT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_OBJ_EQEQ_OBJ ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_null:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
        case VI_string:
        case VI_signal:
          OUT_EXEC_NULL_EQEQ_ACC ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_eqeqQ( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_EQEQQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_EQEQQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_EQEQQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_EQEQQ_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_eqeqeq( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_EQEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_EQEQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_EQEQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_EQEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_EQEQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_EQEQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_EQEQEQ_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_EQEQEQ_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_EQEQEQ_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_STRING_EQEQ_STRING ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_SIGNAL_EQEQ_SIGNAL ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          OUT_EXEC_INT_EQEQEQ_INT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_OBJ_EQEQEQ_OBJ ;
          break ;
        case VI_null:
          OUT_EXEC_POP_EQEQ_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_null:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
        case VI_string:
        case VI_signal:
          OUT_EXEC_NULL_EQEQ_ACC ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_ge( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_GE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_GE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_GE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_GE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_GE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_GE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_GE_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_GE_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_GE_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_gt( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_GT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_GT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_GT_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_GT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_GT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_GT_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_GT_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_GT_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_GT_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_le( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_LE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_LE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_LE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_LE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_LE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_LE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_LE_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_LE_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_LE_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_lt( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_LT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_LT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_LT_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_LT_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_LT_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_LT_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_LT_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_LT_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_LT_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_minus( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_MINUS_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_MINUS_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_MINUS_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_MINUS_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_MINUS_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_MINUS_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_MINUS_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_MINUS_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_MINUS_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_mod( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_MOD_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_MOD_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_MOD_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_MOD_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_MOD_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_MOD_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_MOD_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_MOD_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_MOD_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_nand( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_NAND_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_NAND_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_NAND_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_NAND_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_ne( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_NE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_NE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_NE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_NE_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_NE_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_NE_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_NE_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_NE_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_NE_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_STRING_NE_STRING ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_SIGNAL_NE_SIGNAL ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          OUT_EXEC_INT_NE_INT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_OBJ_NE_OBJ ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_null:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
        case VI_string:
        case VI_signal:
          OUT_EXEC_NULL_NE_ACC ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_neor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_NEOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_NEOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_NEOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_NEOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_neqeq( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_NEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_NEQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_NEQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_NEQEQ_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_NEQEQ_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_NEQEQ_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_NEQEQ_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_NEQEQ_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_NEQEQ_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_string:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_string:
          OUT_EXEC_STRING_NE_STRING ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_signal:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_signal:
          OUT_EXEC_SIGNAL_NE_SIGNAL ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_enum:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_enum:
          OUT_EXEC_INT_NEQEQ_INT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_obj:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
          OUT_EXEC_OBJ_NEQEQ_OBJ ;
          break ;
        case VI_null:
          OUT_EXEC_POP_NE_NULL ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_null:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_obj:
        case VI_string:
        case VI_signal:
          OUT_EXEC_NULL_NE_ACC ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_nor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_NOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_NOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_NOR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_NOR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_or( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_OR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_OR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_OR_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_OR_BIT ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_plus( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_PLUS_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_PLUS_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_PLUS_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_PLUS_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_PLUS_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_PLUS_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_PLUS_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_PLUS_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_PLUS_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
static void output_times( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_INT_TIMES_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_INT_TIMES_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_TIMES_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_bit:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_BIT_TIMES_INT ;
          break ;
        case VI_bit:
          OUT_EXEC_BIT_TIMES_BIT ;
          break ;
        case VI_float:
          OUT_EXEC_M_TIMES_M ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    case VI_float:
      switch( code->sd_type->is.sd_type.b ) {
        case VI_int:
          OUT_EXEC_M_TIMES_M ;
          break ;
        case VI_bit:
          OUT_EXEC_M_TIMES_M ;
          break ;
        case VI_float:
          OUT_EXEC_F_TIMES_F ;
          break ;
        default:
          illegal_binary_ops() ;
          break ;
      }
      break ;
    default:
      illegal_binary_ops() ;
      break ;
  }

}
  
/**************************************/
/* unary ALU operation instructions   */
/**************************************/

static void output_u_and( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_AND_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_AND_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_and ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_eor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_EOR_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_EOR_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_eor ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_minus( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_MINUS_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_MINUS_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_minus ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_nand( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_NAND_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_NAND_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_nand ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_neor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_NEOR_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_NEOR_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_neor ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_nor( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_NOR_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_NOR_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_nor ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_not( vinst_info *code ) {
  
  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_NOT_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_NOT_BIT ;
      break ;
    case VI_string:
      OUT0( "/* !(string) */\n" ) ;
      OUT0( "if( ACC_STRING == NULL ) ACC_INT = 1 ;\n" ) ;
      OUT0( 
        "else if( ACC_STRING[0] == \'\\0\' ) ACC_INT = 1 ;\n" 
      ) ;
      OUT0( "else ACC_INT = 0 ;\n" )
      OUT0( "ACC_TYPE = JD_D_int ;\n" ) ;
      break ;
    case VI_float:
    case VI_double:
      OUT0( "/* !(float) */\n" ) ;
      OUT1( "if( ACC_FLOAT == 0.0 ) ACC_INT = 1 ;\n", code->name ) ;
      OUT0( "else ACC_INT = 0 ;\n" )
      OUT0( "ACC_TYPE = JD_D_int ;\n" ) ;
      break ;
    case VI_obj:
      OUT0( "/* !(obj) */\n" ) ;
      OUT1( "if( ACC_DT->data.obj.ect == NULL ) ACC_INT = 1 ;\n", code->name ) ;
      OUT0( "else ACC_INT = 0 ;\n" )
      OUT0( "ACC_TYPE = JD_D_int ;\n" ) ;
      break ;
    case VI_signal:
      OUT0( "/* !(signal) */\n" ) ;
      OUT1( "if( ACC_DT->data.signal.port == NULL ) ACC_INT = 1 ;\n", code->name ) ;
      OUT0( "else ACC_INT = 0 ;\n" )
      OUT0( "ACC_TYPE = JD_D_int ;\n" ) ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_not ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_u_or( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_OR_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_OR_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_or ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_eval_end() {
  OUT0( "#undef eval_end\n" ) ;
  OUT1( "#define eval_end eval_end_%d\n", eval_end_num++ ) ;
}

static void output_u_tilde( vinst_info *code ) {

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT_EXEC_U_TILDE_INT ;
      break ;
    case VI_bit:
      OUT_EXEC_U_TILDE_BIT ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal type for u_tilde ops!#^*&^\n" 
      ) ;
      break ;
  }

}
  
static void output_eqCeq( vinst_info *code ) {
  OUT0( "{ JD_data *dta, *dtb ;\n" ) ;
  OUT0( "  dtb = POP_STACK ;\n" ) ;
  OUT0( "  dta = POP_STACK ;\n" ) ;
  switch( code->sd_type->is.sd_type.c ) {
    case VI_int:
      OUT0( "  AR = &ACC_INT ;\n" ) ;
      OUT0( "  BR = &INT_ZERO ;\n" ) ;
      OUT0( "  SR = 32 ;\n" ) ;
      break ;
    case VI_bit:
      OUT0( "  AR = ACC_A ;\n" ) ;
      OUT0( "  BR = ACC_B ;\n" ) ;
      OUT0( "  SR = ACC_SIZE ;\n" ) ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal data type for =:= ops!#^*&^\n" 
      ) ;
      break ;

  }
  
  switch( code->sd_type->is.sd_type.b ) {
    case VI_int:
      OUT0( "  A1 = &dtb->data.int_value ;\n" ) ;
      OUT0( "  B1 = &INT_ZERO ;\n" ) ;
      OUT0( "  S1 = 32 ;\n" ) ;
      break ;
    case VI_bit:
      OUT0( "  A1 = BIT_A(dtb) ;\n" ) ;
      OUT0( "  B1 = BIT_B(dtb) ;\n" ) ;
      OUT0( "  S1 = DT_BIT_SIZE(dtb) ;\n" ) ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal data type for =:= ops!#^*&^\n" 
      ) ;
      break ;
  }

  switch( code->sd_type->is.sd_type.a ) {
    case VI_int:
      OUT0( "  A0 = &dta->data.int_value ;\n" ) ;
      OUT0( "  B0 = &INT_ZERO ;\n" ) ;
      OUT0( "  S0 = 32 ;\n" ) ;
      break ;
    case VI_bit:
      OUT0( "  A0 = BIT_A(dta) ;\n" ) ;
      OUT0( "  B0 = BIT_B(dta) ;\n" ) ;
      OUT0( "  S0 = DT_BIT_SIZE(dta) ;\n" ) ;
      break ;
    default:
      fprintf( 
        stderr, "!!! illegal data type for =:= ops!#^*&^\n" 
      ) ;
      break ;
  }

  OUT0( "  JD_bit_eqCeq_bit_bit(AR,BR,SR,A0,B0,S0,A1,B1,S1) ;\n" ) ;
  OUT0( "  ACC_INT = RET ;\n" ) ;
  OUT0( "  ACC_TYPE = JD_D_int ;\n" ) ;

  OUT0( "} \n" ) ;
}

static void output_save_acc() {
  OUT0( "CALL_EXEC_SAVE_ACC ; /* acc to save area */ \n" ) ;
}

static void output_restore_acc() {
  OUT0( "CALL_EXEC_RESTORE_ACC ; /* acc from save area */ \n" ) ;
}


static int debug_cwd_id = 0 ;

void set_debug_cwd( vcode_info *this ) {
  debug_cwd_id = 
    debug_name_lookup( this->is.debug_cwd_info.dir ) ;
}

void output_debug_code( vinst_info *code ) {
  int i, m, n ;
  
  switch(code->vinfo->type) {
    case JD_debug_cwd_info:
       break ;
    case JD_debug_statement_info:
      if( code->vinfo->is.debug_info.filename ) {
        n = debug_name_lookup( code->vinfo->is.debug_info.filename ) ;
#ifdef PRODUCT_CODE
        if( !main_defined ) {
          OUT0( "SET_LOCATION( " ) ;
          OUT2( "JD_%s_scope_table_offset+%d, ",
            out_fname,
            code->vinfo->is.debug_info.scope
          ) ;
          OUT2( "JD_%s_Debug_name_table_offset+%d, ",
            out_fname,
            n
          ) ;
          OUT1( "%d, ", code->vinfo->is.debug_info.statementnum ) ; 
          OUT1( "%d, ", code->vinfo->is.debug_info.linenum ) ; 
          OUT2( "JD_%s_Debug_name_table_offset+%d ) ; ",
            out_fname,
            debug_cwd_id
          ) ;
          OUT1( "/* (scope file st line cwd) %s */\n",
            code->vinfo->is.debug_info.filename
          );
        }
        else {
#endif  
          OUT6(
            "SET_LOCATION( %d, %d, %d, %d, %d ) ; /* (scope file st line cwd) %s */\n",
            code->vinfo->is.debug_info.scope, 
            n, code->vinfo->is.debug_info.statementnum, 
            code->vinfo->is.debug_info.linenum,
            debug_cwd_id,
            code->vinfo->is.debug_info.filename
          );
#ifdef PRODUCT_CODE
        }
#endif  
        current_scope_n = code->vinfo->is.debug_info.scope ;
        current_file_n = n ;
        current_st_n = code->vinfo->is.debug_info.statementnum ;
        current_line_n = code->vinfo->is.debug_info.linenum ;
        current_cwd_n = debug_cwd_id ;
      }
      break ;
    case JD_debug_gen_func_frame_info:
      break ;
    case JD_debug_gen_mfunc_frame_info:
      break ;
    case JD_debug_argument_info:
      break ;
    case JD_debug_variable_info:
      break ;
    case JD_debug_enum_info:
      break ;
    case JD_debug_block_info:
      break ;
    case JD_debug_endblock_info:
      break ;
    case JD_debug_member_variable_info:
      break ;
    case JD_debug_exit_function_info:
      break ;
  }

}

/**********************************************************/
/* Advice before the actual function call                 */
/**********************************************************/
static void output_call_advice( vinst_info *code ) {
  advice_link *adv ;
  vcode_info *advice ;
  int is_leaf ;
  vinst_info *t_code ;
  
  adv = code->advice ;
  
  
  if( adv ) {
    OUT0( "if( !ADVICED ) {\n" ) ;
    OUT0( "  ADVICED = 1 ;\n" ) ;
  }
  while( adv ) {
    advice = adv->advice ;
    if( advice->is.function_info.func_options &&
        ( advice->is.function_info.func_options->is.attrib.is_leaf ||
          advice->is.function_info.func_options->is.attrib.is_abstract )
      )
    {
      is_leaf = 1 ;
    }
    else is_leaf = 0 ;
  
    OUT_EXEC_FLUSH_VSTACK ;
    
    switch( adv->match_type ) {
      case 1:
        break ;
      case 2:
        just_output_load_this( NULL ) ;
        break ;
      case 3:
        t_code = code->next ;
        while( t_code->itype != VI_enter_advice ) {
          output_c_code( t_code ) ;
          t_code = t_code->next ;
        }
        break ;
    }

    OUT5(
      "SET_LOCATION( %d, %d, %d, %d, %d ) ; /* (scope file st line cwd) */\n",
      current_scope_n, 
      current_file_n, current_st_n, 
      current_line_n,
      current_cwd_n
    ) ;
    OUT0( "  SET_POINTCUT_LOCATION ; \n" ) ;
    if( is_leaf ) 
    {
      OUT0( "SET_ENTER_ADVICE ;\n" ) ;
      OUT2( 
        "JD_uc_%s_ad_%s() ; /* calling leaf advice */\n", 
        advice->is.function_info.class->is.class.name, 
        advice->is.function_info.name
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
    else {
      OUT0( "SET_ENTER_ADVICE ;\n" ) ;
      OUT2( 
        "JD_uc_%s_ad_%s(NULL) ; /* calling non-leaf advice */\n", 
        advice->is.function_info.class->is.class.name, 
        advice->is.function_info.name
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
    }
    adv = adv->next ;
  }
  if( code->advice ) {
    OUT0( "    ADVICED = 0 ;\n" ) ;
    OUT0( "}  /* end of if (ADVIVED) */\n" ) ;
    OUT0( "else {\n" ) ;
    OUT0( "  WARNING_RECURSIVE_ADVICE_CALL_DETECTED ;\n" ) ;
    OUT0( "}\n" ) ;
    
  }
  if( code->overwrite ) code_overwrite = 1 ;
  code_supress = 1 ;
  
}

static void output_return_advice( vinst_info *code ) {
  advice_link *adv ;
  vcode_info *advice ;
  int is_leaf ;
  vinst_info *t_code ;
  
  code_overwrite = 0 ;
  
  adv = code->advice ;
  
  if( adv ) {
    OUT0( "if( !ADVICED ) {\n" ) ;
    OUT0( "  ADVICED = 1 ;\n" ) ;
  }
    
  while( adv ) {
    advice = adv->advice ;
    if( advice->is.function_info.func_options &&
        ( advice->is.function_info.func_options->is.attrib.is_leaf ||
          advice->is.function_info.func_options->is.attrib.is_abstract )
      )
    {
      is_leaf = 1 ;
    }
    else is_leaf = 0 ;
  
    OUT_EXEC_FLUSH_VSTACK ;
    
    switch( adv->match_type ) {
      case 1:
        output_save_acc( NULL ) ;
        output_void( NULL ) ;
        break ;
      case 2:
        output_save_acc( NULL ) ;
        just_output_load_this( NULL ) ;
        break ;
      case 3:
        output_save_acc( NULL ) ;
        t_code = code->next ;
        while( t_code->itype != VI_enter_advice ) {
          output_c_code( t_code ) ;
          t_code = t_code->next ;
        }
        break ;
    }

    OUT5(
      "SET_LOCATION( %d, %d, %d, %d, %d ) ; /* (scope file st line cwd) */\n",
      current_scope_n, 
      current_file_n, current_st_n, 
      current_line_n,
      current_cwd_n
    ) ;
    OUT0( "  SET_POINTCUT_LOCATION ; \n" ) ;
    if( is_leaf ) 
    {
      OUT0( "SET_ENTER_ADVICE ;\n" ) ;
      OUT2( 
        "JD_uc_%s_ad_%s() ; /* calling leaf advice */\n", 
        advice->is.function_info.class->is.class.name, 
        advice->is.function_info.name
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
      OUT0( "S_ACC_TYPE = JD_D_null ;\n" ) ;
    }
    else {
      OUT0( "SET_ENTER_ADVICE ;\n" ) ;
      OUT2( 
        "JD_uc_%s_ad_%s(NULL) ; /* calling non-leaf advice */\n", 
        advice->is.function_info.class->is.class.name, 
        advice->is.function_info.name
      ) ;
      OUT0( "SET_EXIT_FUNC ;\n" ) ;
      OUT0( "S_ACC_TYPE = JD_D_null ;\n" ) ;
    }
    adv = adv->next ;
  }
  if( code->advice ) {
    OUT0( "    ADVICED = 0 ;\n" ) ;
    OUT0( "}  /* end of if (ADVIVED) */\n" ) ;
    OUT0( "else {\n" ) ;
    OUT0( "  WARNING_RECURSIVE_ADVICE_CALL_DETECTED ;\n" ) ;
    OUT0( "}\n" ) ;
    
  }
  code_supress = 1 ;
}

static void output_enter_advice( vinst_info *code ) {
  code_supress = 0 ;
}

static void output_get_return( vinst_info *code ) {
  OUT0( "  JD_get_return() ; \n" ) ;
}

static int code_count = 0 ;

void output_c_code( vinst_info *code ) {
  int skip = 0 ;
  /* if( code->type == VVI_dummy_new ) {
    OUT_EXEC_POP_ACC ;
  }
  else */
  if( code->type == VVI_debug_info ) {
    output_debug_code( code ) ;
  } 
  
  if( code->type == VVI_inst ) {
    
#ifdef GEN_ASSERT_STACK_ALIVE
    if( code_count == 0 ) OUT0( "ASSERT_STACK_ALIVE ;\n" ) ;
    code_count++ ;
    if( code_count == GEN_ASSERT_STACK_ALIVE ) code_count = 0 ;
#endif    
    if( code_supress ) {
      if( code->itype != VI_enter_advice ) skip = 1 ;
    }
    else {
      if( code_overwrite && code->itype != VI_return_advice ) skip = 1 ;
    }
    
    if( !skip ) switch( code->itype ) {
      
      case VI_copy:
        output_copy( code ) ;
        break ;
      case VI_dup:
        output_dup( code ) ; /* not used */
        break ;
      case VI_pop:
        output_pop( code ) ; 
        break ;
      case VI_push:
        output_push( code ) ;
        break ;
        
      case VI_jmp:
        output_jmp( code ) ;
        break ;
      case VI_jz:
        output_jz( code ) ;
        break ;
      case VI_jnz:
        output_jnz( code ) ;
        break ;

      case VI_exec:
        output_exec( code ) ;
        break ;
      case VI_exit:
        output_exit( code ) ;
        break ;
      case VI_fork:
        output_fork( code ) ;
        break ;
      case VI_gen_child:
        output_gen_child( code ) ;
        break ;
      case VI_gen_frame:
        output_gen_frame( code ) ;
        break ;
      case VI_gen_m_frame:
        output_gen_m_frame( code ) ;
        break ;
      case VI_gen_a_frame:
        output_gen_a_frame( code ) ;
        break ;
      case VI_pop_this:
        output_pop_this( code ) ;
        break ;
      case VI_join:
        output_join( code ) ;
        break ;
      case VI_knife:
        output_knife( code ) ;
        break ;
      case VI_spoon:
        output_spoon( code ) ;
        break ;
        
      case VI_terminate:
        output_terminate( code ) ;
        break ;
      
      case VI_p_and:
        output_p_and( code ) ;
        break ;
      case VI_p_or:
        output_p_or( code ) ;
        break ;
      
      case VI_load_const:
        output_load_const( code ) ;
        break ;
      case VI_load_one:
        output_load_one( code ) ;
        break ;
      case VI_load_zero:
        output_load_zero( code ) ;
        break ;

      case VI_load_port:
        output_load_port( code ) ;
        break ;
      case VI_load_pset_port:
        output_load_pset_port( code ) ;
        break ;
      case VI_load_port_ptr:
        output_load_port_ptr( code ) ;
        break ;
      case VI_load_pset_port_ptr:
        output_load_pset_port_ptr( code ) ;
        break ;
      case VI_load_sig_pt:
        output_load_sig_pt( code ) ;
        break ;
      case VI_load_static:
        output_load_static( code ) ;
        break ;
      case VI_load_super:
        output_load_super( code ) ;
        break ;
      case VI_load_this:
        output_load_this( code ) ;
        break ;
      case VI_load_thread:
        output_load_thread( code ) ;
        break ;
      case VI_loadg:
        output_loadg( code ) ;
        break ;
      case VI_loadl:
        output_loadl( code ) ;
        break ;
      case VI_loadm:
        output_loadm( code ) ;
        break ;

      case VI_store_obj:
        output_store_obj( code ) ;
        break ;
      case VI_store_port:
        output_store_port( code ) ;
        break ;
      case VI_store_pset_port:
        output_store_port( code ) ;
        break ;
      case VI_store_sig_g:
#ifdef PRODUCT_CODE
        if( !main_defined ) {
          ERROR_ILLEGAL_ACCESS_TO_GLOBAL_PORT_IN_MODULE( code->name ) ;
        }
        else
#endif
        output_store_sig( code ) ;
        break ;
      case VI_store_sig_l:
        output_store_sig( code ) ;
        break ;
      case VI_store_sig_m:
        output_store_sig_m( code ) ;
        break ;
      case VI_store_static:
        output_store_static( code ) ;
        break ;
      case VI_storeg:
        output_storeg( code ) ;
        break ;
      case VI_storel:
        output_storel( code ) ;
        break ;
      case VI_storem:
        output_storem( code ) ;
        break ;
      
      case VI_store_sig_static:
        output_store_sig( code ) ;
        break ;
      
      case VI_store_adjust:
        output_store_adjust( code ) ;
        break ;
        
      case VI_sample:
        output_sample( code ) ;
        break ;
      case VI_sync:
        output_sync( code ) ;
        break ;
        
      case VI_call:
        output_call( code ) ;
        break ;
      case VI_call_m:
        output_call_m( code ) ;
        break ;
      case VI_call_a:
        output_call_a( code ) ;
        break ;
      case VI_veri_call:
        output_veri_call( code ) ;
        break ;
      case VI_return:
        output_return( code ) ;
        break ;
      case VI_return_l:
        output_return_l( code ) ;
        break ;
      case VI_return_n:
        output_return_n( code ) ;
        break ;
      
      case VI_save_acc:
        output_save_acc( code ) ;
        break ;
      case VI_restore_acc:
        output_restore_acc( code ) ;
        break ;
      case VI_arg:
        output_arg( code ) ;
        break ;
      case VI_lvar:
        output_lvar( code ) ;
        break ;
      case VI_static:
        output_static( code ) ;
        break ;

      case VI_new:
        output_new( code ) ;
        break ;
      case VI_new_obj:
        output_new_obj( code ) ;
        break ;
      case VI_copy_obj:
        output_copy_obj( code ) ;
        break ;
      case VI_alloc:
        output_alloc( code ) ;
        break ;
     case VI_alloc_static:
        output_alloc_static( code ) ;
        break ;
      case VI_get_super:
        output_get_super( code ) ;
        break ;
      case VI_get_child:
        output_get_child( code ) ;
        break ;
      case VI_get_this:
        output_get_this( code ) ;
        break ;

       case VI_alloc_func:
        output_alloc_func( code ) ;
        break ;
       case VI_overwrite:
        output_overwrite( code ) ;
        break ;
       case VI_chk_overwrite:
        output_chk_overwrite( code ) ;
        break ;
       case VI_error:
        output_error( code ) ;
        break ;

      case VI_breakpoint:
        output_breakpoint( code ) ;
        break ;
        
      case VI_duplicate:
        output_duplicate( code ) ;
        break ;

      case VI_list:
        output_list( code ) ;
        break ;
      case VI_get_subbit:
        output_get_subbit( code ) ;
        break ;
      case VI_inc:
        output_inc( code ) ;
        break ;
      case VI_dec:
        output_dec( code ) ;
        break ;
      case VI_cmp:
        output_cmp( code ) ;
        break ;
      case VI_cmpx:
        output_cmpx( code ) ;
        break ;
      case VI_cmpz:
        output_cmpz( code ) ;
        break ;
      case VI_concat:
        output_concat( code ) ;
        break ;
      case VI_lshift:
        output_lshift( code ) ;
        break ;
      case VI_to_int:
        output_to_int( code ) ;
        break ;
      case VI_convert:
        output_convert( code ) ;
        break ;
      case VI_rshift:
        output_rshift( code ) ;
        break ;
      case VI_urshift:
        output_urshift( code ) ;
        break ;
      case VI_null:
        output_null( code ) ;
        break ;
      case VI_void:
        output_void( code ) ;
        break ;
        

      /* binary */
      case VI_Qeqeq:
        output_eval_end() ;
        output_Qeqeq( code ) ;
        break ;
      case VI_Qne:
        output_eval_end() ;
        output_Qne( code ) ;
        break ;
      case VI_and:
        output_eval_end() ;
        output_and( code ) ;
        break ;
      case VI_div:
        output_eval_end() ;
        output_div( code ) ;
        break ;
      case VI_eor:
        output_eval_end() ;
        output_eor( code ) ;
        break ;
      case VI_eqQeq:
        output_eval_end() ;
        output_eqQeq( code ) ;
        break ;
      case VI_eqeq:
        output_eval_end() ;
        output_eqeq( code ) ;
        break ;
      case VI_eqeqQ:
        output_eval_end() ;
        output_eqeqQ( code ) ;
        break ;
      case VI_eqeqeq:
        output_eval_end() ;
        output_eqeqeq( code ) ;
        break ;
      case VI_ge:
        output_eval_end() ;
        output_ge( code ) ;
        break ;
      case VI_gt:
        output_eval_end() ;
        output_gt( code ) ;
        break ;
      case VI_le:
        output_eval_end() ;
        output_le( code ) ;
        break ;
      case VI_lt:
        output_eval_end() ;
        output_lt( code ) ;
        break ;
      case VI_minus:
        output_eval_end() ;
        output_minus( code ) ;
        break ;
      case VI_mod:
        output_eval_end() ;
        output_mod( code ) ;
        break ;
      case VI_nand:
        output_eval_end() ;
        output_nand( code ) ;
        break ;
      case VI_ne:
        output_eval_end() ;
        output_ne( code ) ;
        break ;
      case VI_neor:
        output_eval_end() ;
        output_neor( code ) ;
        break ;
      case VI_neqeq:
        output_eval_end() ;
        output_neqeq( code ) ;
        break ;
      case VI_nor:
        output_eval_end() ;
        output_nor( code ) ;
        break ;
      case VI_or:
        output_eval_end() ;
        output_or( code ) ;
        break ;
      case VI_plus:
        output_eval_end() ;
        output_plus( code ) ;
        break ;
      case VI_times:
        output_eval_end() ;
        output_times( code ) ;
        break ;
      /* unary */
      case VI_u_and:
        output_eval_end() ;
        output_u_and( code ) ;
        break ;
      case VI_u_eor:
        output_eval_end() ;
        output_u_eor( code ) ;
        break ;
      case VI_u_minus:
        output_eval_end() ;
        output_u_minus( code ) ;
        break ;
      case VI_u_nand:
        output_eval_end() ;
        output_u_nand( code ) ;
        break ;
      case VI_u_neor:
        output_eval_end() ;
        output_u_neor( code ) ;
        break ;
      case VI_u_nor:
        output_eval_end() ;
        output_u_nor( code ) ;
        break ;
      case VI_u_not:
        output_eval_end() ;
        output_u_not( code ) ;
        break ;
      case VI_u_or:
        output_eval_end() ;
        output_u_or( code ) ;
        break ;
      case VI_u_tilde:
        output_eval_end() ;
        output_u_tilde( code ) ;
        break ;
      case VI_eqCeq:
        output_eval_end() ;
        output_eqCeq( code ) ;
        break ;
      case VI_call_advice:
        output_call_advice( code ) ;
        break ;
      case VI_return_advice:
        output_return_advice( code ) ;
        break ;
      case VI_enter_advice:
        output_enter_advice( code ) ;
        break ;
      case VI_get_return:
        output_get_return( code ) ;
        break ;
      default:
        fprintf( stderr, "Unknown code type detected\n" ) ;
        exit(1) ;
        break ;
    }
  }
}
