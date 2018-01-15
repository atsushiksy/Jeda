/** Jeda Compiler C output **/
#include <stdio.h>
#include "jeda_runtime.h"
#include "jeda_pli.h"
void JD_standalone_link() {
  JD_standalone_link_CLOCK() ;
}
int JD_vcode_hash_value = 0 ;
void *JD_sa_port() ; /* allocate */
void *JD_sd_port() ; /* deallocate */
void *JD_dup_port() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_port() ; /* display */
#endif
void *JD_sa_signal() ; /* allocate */
void *JD_sd_signal() ; /* deallocate */
void *JD_dup_signal() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_signal() ; /* display */
#endif
void *JD_sa_array() ; /* allocate */
void *JD_sd_array() ; /* deallocate */
void *JD_dup_array() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_array() ; /* display */
#endif
void *JD_sa_marray() ; /* allocate */
void *JD_sd_marray() ; /* deallocate */
void *JD_dup_marray() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_marray() ; /* display */
#endif
void *JD_sa_assoc() ; /* allocate */
void *JD_sd_assoc() ; /* deallocate */
void *JD_dup_assoc() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_assoc() ; /* display */
#endif
void *JD_sa_stridx() ; /* allocate */
void *JD_sd_stridx() ; /* deallocate */
void *JD_dup_stridx() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_stridx() ; /* display */
#endif
void *JD_sa_list() ; /* allocate */
void *JD_sd_list() ; /* deallocate */
void *JD_dup_list() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_list() ; /* display */
#endif
void *JD_sa_regexp() ; /* allocate */
void *JD_sd_regexp() ; /* deallocate */
void *JD_dup_regexp() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_regexp() ; /* display */
#endif
void *JD_sa_file() ; /* allocate */
void *JD_sd_file() ; /* deallocate */
void *JD_dup_file() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_file() ; /* display */
#endif
void *JD_sa_random_48() ; /* allocate */
void *JD_sd_random_48() ; /* deallocate */
void *JD_dup_random_48() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_random_48() ; /* display */
#endif
void *JD_sa_random_mt() ; /* allocate */
void *JD_sd_random_mt() ; /* deallocate */
void *JD_dup_random_mt() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_random_mt() ; /* display */
#endif
void *JD_sa_geom_random() ; /* allocate */
void *JD_sd_geom_random() ; /* deallocate */
void *JD_dup_geom_random() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_geom_random() ; /* display */
#endif
void *JD_sa_exp_random() ; /* allocate */
void *JD_sd_exp_random() ; /* deallocate */
void *JD_dup_exp_random() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_exp_random() ; /* display */
#endif
void *JD_sa_pdf_random() ; /* allocate */
void *JD_sd_pdf_random() ; /* deallocate */
void *JD_dup_pdf_random() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_pdf_random() ; /* display */
#endif
void *JD_sa_semaphore() ; /* allocate */
void *JD_sd_semaphore() ; /* deallocate */
void *JD_dup_semaphore() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_semaphore() ; /* display */
#endif
void *JD_sa_event() ; /* allocate */
void *JD_sd_event() ; /* deallocate */
void *JD_dup_event() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_event() ; /* display */
#endif
void *JD_sa_ip_socket() ; /* allocate */
void *JD_sd_ip_socket() ; /* deallocate */
void *JD_dup_ip_socket() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_ip_socket() ; /* display */
#endif
void *JD_sa_sys_RND() ; /* allocate */
void *JD_sd_sys_RND() ; /* deallocate */
void *JD_dup_sys_RND() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_sys_RND() ; /* display */
#endif
void *JD_sa_sys_CYC() ; /* allocate */
void *JD_sd_sys_CYC() ; /* deallocate */
void *JD_dup_sys_CYC() ; /* duplicate */
#ifdef JEDA_DEBUGGER 
void *JD_disp_sys_CYC() ; /* display */
#endif
/* { type, index, name, dtype, level, parent } */ 
JD_debug_scope JD_scope_table[] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 }, /* first entry not used */
  /* scope for class_quant_matrix_ext.default_set() */
  /* scope for func main() */
  { -1, -1, 0, 0, 0, 0, 0, 0 }
} ;
int JD_scope_table_num = 1 ; /* number of top entries */
JD_debug_file *JD_file_table ;
JD_debug_file JD__file_table[] = {
  { -1, -1 }
};
int JD_file_table_size = 0 ;
void JD_ua_class_quant_matrix_ext() ;
void JD_uc_class_quant_matrix_ext_mf_default_set() ;
void JD_ua_check_class() ;
void JD_uf_main() ;
void JD_si_alloc_global( JD_toplevel *top ) {
  top->global_data = GLOBAL_DATA =  (JD_data **)calloc( 4, sizeof(JD_data *) ) ;
  top->global_obj =  (int **)calloc( 4, sizeof(int *) ) ;
  top->num_global_data = 1 ;
  ALLOC_OBJ = (void *)JD_ua_check_class ; 
  CALL_ALLOC_DT_SINGLE_OBJ ; 
  top->global_data[1] = TMP_DT ; /* global var "check0" */
  top->num_global_data++ ;
  ALLOC_OBJ = (void *)JD_ua_class_quant_matrix_ext ; 
  CALL_ALLOC_DT_SINGLE_OBJ ; 
  top->global_data[2] = TMP_DT ; /* global var "matrix0" */
  top->num_global_data++ ;
}
void JD_si_initialize() {
}
void JD_ua_class_quant_matrix_ext() {
  CALL_EXEC_FLUSH_VSTACK ; /* flush vstack before class new */ 
  ALLOC_OBJ = (void *)JD_ua_class_quant_matrix_ext ; 
  DEALLOC_OBJ = (void *)JD_dealloc_obj_table_table ; 
  printf( "point 01 \n" ) ;
  JD_exec_new() ;
  JD_run_garbage_collection() ;
  BIT_SIZE = 1 ;
  printf( "point 02 \n" ) ;
  JD_exec_alloc_single_bit() ;
  JD_run_garbage_collection() ;
  BIT_SIZE = 4 ;
  printf( "point 03 \n" ) ;
  JD_exec_alloc_single_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 25 ;
  BIT_SIZE = 1 ;
  printf( "point 04 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 2 ;
  CALL_ALLOC_MARRAY_DIM ; 
  printf( "point 05 \n" ) ;
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  printf( "point 06 \n" ) ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 25 ;
  BIT_SIZE = 1 ;
  printf( "point 07 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 2 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 1600 ;
  BIT_SIZE = 8 ;
  printf( "point 08 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 3 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 64 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 1600 ;
  BIT_SIZE = 8 ;
  printf( "point 09 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 3 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 64 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 1600 ;
  BIT_SIZE = 8 ;
  printf( "point 010 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 3 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 64 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 1600 ;
  BIT_SIZE = 8 ;
  printf( "point 011 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 3 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 64 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 25 ;
  BIT_SIZE = 1 ;
  printf( "point 012 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 2 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 25 ;
  BIT_SIZE = 1 ;
  printf( "point 013 \n" ) ;
  JD_exec_alloc_array_bit() ;
  JD_run_garbage_collection() ;
  ARRAY_SIZE = 2 ;
  CALL_ALLOC_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  ARRAY_SIZE = 5 ;
  CALL_SET_MARRAY_DIM ; 
  TMP_INT = 10 ; /* function "default_set" */ 
  JD_exec_alloc_func() ;
  return ; 
}  /* end of void JD_ua_class_quant_matrix_ext() */ 
void JD_uc_class_quant_matrix_ext_mf_default_set() {
  if( ACC_DT == NULL ) {  
    ERROR_EMPTY_OBJECT_ACCESS ; return ;
  } 
  TMP_INT = 10 ; /* function "default_set" */ 
  if( ACC_DT->data.obj.ect ) {  
    JD_data *dt ; 
    TMP_DT = ACC_DT->data.obj.ect->table[TMP_INT] ; 
    if( TMP_DT->data.func.func ) { 
      ACC_DT = TMP_DT->data.func.data ;  /* replace this */ 
      SET_ENTER_CHILD_MFUNC ;
      (TMP_DT->data.func.func)() ; 
      return ; 
    } 
  } 
  JD_exec_gen_m_frame() ;
  JD_flush_vstack() ;
  JD_exec_lvar_single_int() ;
  JD_exec_lvar_single_int() ;
  JD_exec_lvar_single_int() ;
  SET_LOCATION( 1, 0, 0, 17, 0 ) ; /* (scope file st line cwd) check.j */
  /* int const "0" */
  ACC_INT = 0 ; 
  ACC_TYPE = JD_D_int ; 
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  SET_LOCATION( 1, 0, 2, 19, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x0 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 1 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  TMP_INT = 0 ; /* class_quant_matrix_ext var "load" */ 
  JD_exec_storem_single_bit() ;
  SET_LOCATION( 1, 0, 3, 20, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x3 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 4 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  TMP_INT = 1 ; /* class_quant_matrix_ext var "extension_start_code_identifier" */ 
  JD_exec_storem_single_bit() ;
  SET_LOCATION( 1, 0, 4, 22, 0 ) ; /* (scope file st line cwd) check.j */
  /* int const "0" */
  ACC_INT = 0 ; 
  ACC_TYPE = JD_D_int ; 
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  L_l6: 
  SET_LOCATION( 1, 0, 4, 22, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "5" */
  ACC_INT = 5 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_0
  JD_exec_i_lt_i() ;
  if( ACC_INT == 0 ) goto L_l1 ;
  SET_LOCATION( 1, 0, 6, 23, 0 ) ; /* (scope file st line cwd) check.j */
  /* int const "0" */
  ACC_INT = 0 ; 
  ACC_TYPE = JD_D_int ; 
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  L_l7: 
  SET_LOCATION( 1, 0, 6, 23, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "5" */
  ACC_INT = 5 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_1
  JD_exec_i_lt_i() ;
  if( ACC_INT == 0 ) goto L_l3 ;
  SET_LOCATION( 1, 0, 8, 24, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x0 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 1 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 5) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  TMP_INT = 2 ; /* class_quant_matrix_ext var "load_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 9, 25, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x0 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 1 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 5) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  TMP_INT = 3 ; /* class_quant_matrix_ext var "load_non_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 10, 26, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x0 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 1 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 5) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  TMP_INT = 8 ; /* class_quant_matrix_ext var "load_chroma_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 11, 27, 0 ) ; /* (scope file st line cwd) check.j */
  ACC_A[0] = 0x0 ;
  ACC_B[0] = 0x0 ;
  ACC_SIZE = 1 ;
  ACC_TYPE = JD_D_bit ; 
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 5) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  TMP_INT = 9 ; /* class_quant_matrix_ext var "load_chroma_non_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 12, 28, 0 ) ; /* (scope file st line cwd) check.j */
  /* int const "0" */
  ACC_INT = 0 ; 
  ACC_TYPE = JD_D_int ; 
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  L_l8: 
  SET_LOCATION( 1, 0, 12, 28, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "64" */
  ACC_INT = 64 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_2
  JD_exec_i_lt_i() ;
  if( ACC_INT == 0 ) goto L_l5 ;
  SET_LOCATION( 1, 0, 14, 29, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "200" */
  ACC_INT = 200 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_3
  JD_exec_i_plus_i() ;
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  printf( "Line 29 ACC_DT: %x \n", ACC_DT ) ;
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 64 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 64) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 320) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(3) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  JD_exec_int_to_bit() ;
  TMP_INT = 4 ; /* class_quant_matrix_ext var "intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 15, 30, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "300" */
  ACC_INT = 300 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_4
  JD_exec_i_plus_i() ;
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 64 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 64) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 320) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(3) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  JD_exec_int_to_bit() ;
  TMP_INT = 5 ; /* class_quant_matrix_ext var "non_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 16, 31, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "400" */
  ACC_INT = 400 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_5
  JD_exec_i_plus_i() ;
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 64 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 64) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 320) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(3) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  JD_exec_int_to_bit() ;
  TMP_INT = 6 ; /* class_quant_matrix_ext var "chroma_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  SET_LOCATION( 1, 0, 17, 32, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  /* int const "500" */
  ACC_INT = 500 ; 
  ACC_TYPE = JD_D_int ; 
  #undef eval_end
  #define eval_end eval_end_6
  JD_exec_i_plus_i() ;
  JD_push_acc() ;
  if( THREAD->this && THREAD->this->data.obj.ect ) { 
    ACC_DT = THREAD->this ; 
    ACC_TYPE = JD_D_dt ;
  } 
  else { 
    ERROR_EMPTY_OBJECT_ACCESS ; 
  } 
  JD_push_acc() ;
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT2 = 0 ;
    TMP_INT2 += (ACC_INT * 1) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 64 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(1) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 64) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(2) ; 
    }
    #endif
    CALL_EXEC_POP_ACC ; /* index to the array */ 
    TMP_INT2 += (ACC_INT * 320) ; 
    #ifndef SKIP_ARRAY_BOUNDARY_CHECK
    if( ACC_INT >= 5 ) { 
      ERROR_NTH_M_ARRAY_INDEX_ACCESS_OVERFLOW(3) ; 
    }
    #endif
  CALL_EXEC_POP_ACC ; /* object */ 
  TMP_DT = ACC_DT ;
  CALL_EXEC_POP_ACC ; /* data to be stored */ 
  JD_exec_int_to_bit() ;
  TMP_INT = 7 ; /* class_quant_matrix_ext var "chroma_non_intra_quantiser_matrix" */ 
  JD_exec_storem_marray() ;
  L_l4: 
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  ACC_INT++ ;  /* inc instruction */
  TMP_INT = 1 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  JD_pop_acc() ;
  goto L_l8 ;
  L_l5: 
  L_l2: 
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  ACC_INT++ ;  /* inc instruction */
  TMP_INT = 3 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  JD_pop_acc() ;
  goto L_l7 ;
  L_l3: 
  L_l0: 
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_push_acc() ;
  ACC_INT++ ;  /* inc instruction */
  TMP_INT = 2 ; /* local variable */ 
  JD_exec_storel_single_int() ;
  JD_pop_acc() ;
  goto L_l6 ;
  L_l1: 
  TMP_INT = 3 ;
  JD_exec_pop_n() ;
  JD_exec_pop_this() ;
  CLEAR_ACC ; 
  ACC_TYPE = JD_D_void ; /* void instruction */
  SP = THREAD->stk_frame ;
  THREAD->stk_frame = STACK[SP]->data.int_value ;
  return ; 
}  /* end of void JD_uc_class_quant_matrix_ext_mf_default_set() */ 
void JD_ua_check_class() {
  CALL_EXEC_FLUSH_VSTACK ; /* flush vstack before class new */ 
  ALLOC_OBJ = (void *)JD_ua_check_class ; 
  DEALLOC_OBJ = (void *)JD_dealloc_obj_table_table ; 
  JD_exec_new() ;
  ALLOC_OBJ = (void *)JD_ua_class_quant_matrix_ext ; 
  JD_exec_alloc_single_obj() ;
  ALLOC_OBJ = (void *)JD_ua_class_quant_matrix_ext ; 
  JD_exec_alloc_single_obj() ;
  ALLOC_OBJ = (void *)JD_ua_class_quant_matrix_ext ; 
  JD_exec_alloc_single_obj() ;
  return ; 
}  /* end of void JD_ua_check_class() */ 
void JD_uf_main( JD_thread *mythread ) {
  if( mythread) switch( mythread->run_index ) {
    case 0: JD_sync_exec(mythread) ; break ;
    default: break ;
  } /* end of switch( thrd->index ) */
  JD_exec_gen_frame() ;
  JD_flush_vstack() ;
  ALLOC_OBJ = (void *)JD_ua_check_class ; 
  JD_exec_lvar_single_obj() ;
  SET_LOCATION( 1, 0, 21, 55, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  TMP_INT = 2 ; /* global var "matrix0" */ 
  JD_exec_storeg_single_obj() ;
  SET_LOCATION( 1, 0, 22, 56, 0 ) ; /* (scope file st line cwd) check.j */
  TMP_INT = 2 ; /* global var "matrix0" */ 
  JD_exec_loadg_single() ;
  NUM_ARG = 0 ;
  SET_ENTER_MFUNC ;
  JD_uc_class_quant_matrix_ext_mf_default_set() ; /* calling leaf function */
  SET_EXIT_FUNC ;
  SET_LOCATION( 1, 0, 23, 58, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_check_class() ; /* new_obj check_class */ 
  JD_pop_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_storeg_single_obj() ;
  SET_LOCATION( 1, 0, 24, 60, 0 ) ; /* (scope file st line cwd) check.j */
  printf( "point 0 \n" ) ;
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  printf( "point 1 \n" ) ;
  JD_pop_acc() ;
  printf( "point 2 \n" ) ;
  JD_push_acc() ;
  TMP_INT = 2 ; /* global var "matrix0" */ 
  printf( "point 3 \n" ) ;
  JD_exec_loadg_single() ;
  printf( "point 4 \n" ) ;
  JD_exec_copy_obj() ;
  printf( "point 5 \n" ) ;
  JD_push_acc() ;
  printf( "point 6 \n" ) ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  printf( "point 7 \n" ) ;
  TMP_INT = 0 ; /* check_class var "ext0" */ 
  JD_exec_storem_single_obj() ;
  SET_LOCATION( 1, 0, 25, 61, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 2 ; /* global var "matrix0" */ 
  JD_exec_loadg_single() ;
  JD_exec_copy_obj() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  TMP_INT = 1 ; /* check_class var "ext1" */ 
  JD_exec_storem_single_obj() ;
  SET_LOCATION( 1, 0, 26, 62, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 2 ; /* global var "matrix0" */ 
  JD_exec_loadg_single() ;
  JD_exec_copy_obj() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  TMP_INT = 2 ; /* check_class var "ext2" */ 
  JD_exec_storem_single_obj() ;
  SET_LOCATION( 1, 0, 27, 64, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_check_class() ; /* new_obj check_class */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 0 ; /* local variable */ 
  JD_exec_loadl_single() ;
  JD_exec_copy_obj() ;
  TMP_INT = 0 ; /* local variable */ 
  JD_exec_storel_single_obj() ;
  SET_LOCATION( 1, 0, 28, 65, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  TMP_INT = 0 ; /* class check_class var "ext0" */ 
  JD_exec_loadm_single() ;
  JD_exec_copy_obj() ;
  JD_push_acc() ;
  TMP_INT = 0 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT = 0 ; /* check_class var "ext0" */ 
  JD_exec_storem_single_obj() ;
  SET_LOCATION( 1, 0, 29, 66, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  TMP_INT = 1 ; /* class check_class var "ext1" */ 
  JD_exec_loadm_single() ;
  JD_exec_copy_obj() ;
  JD_push_acc() ;
  TMP_INT = 0 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT = 1 ; /* check_class var "ext1" */ 
  JD_exec_storem_single_obj() ;
  SET_LOCATION( 1, 0, 30, 67, 0 ) ; /* (scope file st line cwd) check.j */
  JD_ua_class_quant_matrix_ext() ; /* new_obj class_quant_matrix_ext */ 
  JD_pop_acc() ;
  JD_push_acc() ;
  TMP_INT = 1 ; /* global var "check0" */ 
  JD_exec_loadg_single() ;
  TMP_INT = 2 ; /* class check_class var "ext2" */ 
  JD_exec_loadm_single() ;
  JD_exec_copy_obj() ;
  JD_push_acc() ;
  TMP_INT = 0 ; /* local variable */ 
  JD_exec_loadl_single() ;
  TMP_INT = 2 ; /* check_class var "ext2" */ 
  JD_exec_storem_single_obj() ;
  TMP_INT = 1 ;
  JD_exec_pop_n() ;
  CLEAR_ACC ; 
  ACC_TYPE = JD_D_void ; /* void instruction */
  SP = THREAD->stk_frame ;
  THREAD->stk_frame = STACK[SP]->data.int_value ;
  JEDA_end_of_simulation() ;
  JD_thread_kill(THREAD) ;
  return ; 
}  /* end of void JD_uf_main() */ 
char **JD_Debug_name_table ;
char *JD__Debug_name_table[] = {
"check.j",  "" } ;
JD_export_func JD_export_table[] = {
  { NULL }
} ; 
JD_module_table JD_modules_entries[] = {
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, JD__Debug_name_table, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
} ;
