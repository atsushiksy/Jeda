/********************************************/
/* macro generated by gen_code_out_macro_prod.pl */
/********************************************/
#  define CALL_EXEC_NEW  JD_exec_new()
#define OUT_EXEC_NEW \
    OUT0( "JD_exec_new() ;\n" ) ; 
#  define CALL_EXEC_GEN_CHILD  JD_exec_gen_child()
#define OUT_EXEC_GEN_CHILD \
    OUT0( "JD_exec_gen_child() ;\n" ) ; 
#  define CALL_EXEC_GET_THIS  JD_exec_get_this()
#define OUT_EXEC_GET_THIS \
    OUT0( "JD_exec_get_this() ;\n" ) ; 
#  define CALL_EXEC_GET_SUPER  JD_exec_get_super()
#define OUT_EXEC_GET_SUPER \
    OUT0( "JD_exec_get_super() ;\n" ) ; 
#  define CALL_EXEC_GET_CHILD  JD_exec_get_child()
#define OUT_EXEC_GET_CHILD \
    OUT0( "JD_exec_get_child() ;\n" ) ; 
#  define CALL_EXEC_COPY_OBJ  JD_exec_copy_obj()
#define OUT_EXEC_COPY_OBJ \
    OUT0( "JD_exec_copy_obj() ;\n" ) ; 
#  define CALL_EXEC_SET_OBJECT  JD_set_object()
#define OUT_EXEC_SET_OBJECT \
    OUT0( "JD_set_object() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STATIC  JD_exec_alloc_static()
#define OUT_EXEC_ALLOC_STATIC \
    OUT0( "JD_exec_alloc_static() ;\n" ) ; 
#ifdef JEDA_INLINE_OPTIMIZATION
#  define CALL_EXEC_ALLOC_FUNC  { \
   {\
     TMP_DT = JD_alloc_data_entry() ;\
     TMP_DT->type = JD_D_func ;\
     TMP_DT->data.func.func = NULL ;\
     TMP_DT->data.func.data = NULL ;\
     CALL_EXEC_SET_OBJECT ;\
   }\
  }
#else
#  define CALL_EXEC_ALLOC_FUNC  JD_exec_alloc_func()
#endif
#define OUT_EXEC_ALLOC_FUNC \
  if( inline_optimization ) {  \
    OUT0( "    /* Macro for JD_exec_alloc_func */ \n" ) ; \
    OUT0( "  {\n" ) ;\
    OUT0( "    TMP_DT = JD_alloc_data_entry() ;\n" ) ;\
    OUT0( "    TMP_DT->type = JD_D_func ;\n" ) ;\
    OUT0( "    TMP_DT->data.func.func = NULL ;\n" ) ;\
    OUT0( "    TMP_DT->data.func.data = NULL ;\n" ) ;\
    OUT0( "    CALL_EXEC_SET_OBJECT ;\n" ) ;\
    OUT0( "  }\n" ) ;\
  } \
  else { \
    OUT0( "JD_exec_alloc_func() ;\n" ) ; \
  }
#  define CALL_EXEC_OVERWRITE  JD_exec_overwrite()
#define OUT_EXEC_OVERWRITE \
    OUT0( "JD_exec_overwrite() ;\n" ) ; 
#  define CALL_EXEC_ERROR  JD_exec_error()
#define OUT_EXEC_ERROR \
    OUT0( "JD_exec_error() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_INT  JD_exec_alloc_single_int()
#define OUT_EXEC_ALLOC_SINGLE_INT \
    OUT0( "JD_exec_alloc_single_int() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_BIT  JD_exec_alloc_single_bit()
#define OUT_EXEC_ALLOC_SINGLE_BIT \
    OUT0( "JD_exec_alloc_single_bit() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_FLOAT  JD_exec_alloc_single_float()
#define OUT_EXEC_ALLOC_SINGLE_FLOAT \
    OUT0( "JD_exec_alloc_single_float() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_DOUBLE  JD_exec_alloc_single_double()
#define OUT_EXEC_ALLOC_SINGLE_DOUBLE \
    OUT0( "JD_exec_alloc_single_double() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_STRING  JD_exec_alloc_single_string()
#define OUT_EXEC_ALLOC_SINGLE_STRING \
    OUT0( "JD_exec_alloc_single_string() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_SIGNAL  JD_exec_alloc_single_signal()
#define OUT_EXEC_ALLOC_SINGLE_SIGNAL \
    OUT0( "JD_exec_alloc_single_signal() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_OBJ  JD_exec_alloc_single_obj()
#define OUT_EXEC_ALLOC_SINGLE_OBJ \
    OUT0( "JD_exec_alloc_single_obj() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_SINGLE_THREAD  JD_exec_alloc_single_thread()
#define OUT_EXEC_ALLOC_SINGLE_THREAD \
    OUT0( "JD_exec_alloc_single_thread() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_INT  JD_exec_alloc_array_int()
#define OUT_EXEC_ALLOC_ARRAY_INT \
    OUT0( "JD_exec_alloc_array_int() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_BIT  JD_exec_alloc_array_bit()
#define OUT_EXEC_ALLOC_ARRAY_BIT \
    OUT0( "JD_exec_alloc_array_bit() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_FLOAT  JD_exec_alloc_array_float()
#define OUT_EXEC_ALLOC_ARRAY_FLOAT \
    OUT0( "JD_exec_alloc_array_float() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_DOUBLE  JD_exec_alloc_array_double()
#define OUT_EXEC_ALLOC_ARRAY_DOUBLE \
    OUT0( "JD_exec_alloc_array_double() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_STRING  JD_exec_alloc_array_string()
#define OUT_EXEC_ALLOC_ARRAY_STRING \
    OUT0( "JD_exec_alloc_array_string() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_SIGNAL  JD_exec_alloc_array_signal()
#define OUT_EXEC_ALLOC_ARRAY_SIGNAL \
    OUT0( "JD_exec_alloc_array_signal() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_OBJ  JD_exec_alloc_array_obj()
#define OUT_EXEC_ALLOC_ARRAY_OBJ \
    OUT0( "JD_exec_alloc_array_obj() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ARRAY_THREAD  JD_exec_alloc_array_thread()
#define OUT_EXEC_ALLOC_ARRAY_THREAD \
    OUT0( "JD_exec_alloc_array_thread() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_INT  JD_exec_alloc_assoc_int()
#define OUT_EXEC_ALLOC_ASSOC_INT \
    OUT0( "JD_exec_alloc_assoc_int() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_BIT  JD_exec_alloc_assoc_bit()
#define OUT_EXEC_ALLOC_ASSOC_BIT \
    OUT0( "JD_exec_alloc_assoc_bit() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_FLOAT  JD_exec_alloc_assoc_float()
#define OUT_EXEC_ALLOC_ASSOC_FLOAT \
    OUT0( "JD_exec_alloc_assoc_float() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_DOUBLE  JD_exec_alloc_assoc_double()
#define OUT_EXEC_ALLOC_ASSOC_DOUBLE \
    OUT0( "JD_exec_alloc_assoc_double() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_STRING  JD_exec_alloc_assoc_string()
#define OUT_EXEC_ALLOC_ASSOC_STRING \
    OUT0( "JD_exec_alloc_assoc_string() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_SIGNAL  JD_exec_alloc_assoc_signal()
#define OUT_EXEC_ALLOC_ASSOC_SIGNAL \
    OUT0( "JD_exec_alloc_assoc_signal() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_OBJ  JD_exec_alloc_assoc_obj()
#define OUT_EXEC_ALLOC_ASSOC_OBJ \
    OUT0( "JD_exec_alloc_assoc_obj() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_ASSOC_THREAD  JD_exec_alloc_assoc_thread()
#define OUT_EXEC_ALLOC_ASSOC_THREAD \
    OUT0( "JD_exec_alloc_assoc_thread() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_INT  JD_exec_alloc_strided_int()
#define OUT_EXEC_ALLOC_STRIDED_INT \
    OUT0( "JD_exec_alloc_strided_int() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_BIT  JD_exec_alloc_strided_bit()
#define OUT_EXEC_ALLOC_STRIDED_BIT \
    OUT0( "JD_exec_alloc_strided_bit() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_FLOAT  JD_exec_alloc_strided_float()
#define OUT_EXEC_ALLOC_STRIDED_FLOAT \
    OUT0( "JD_exec_alloc_strided_float() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_DOUBLE  JD_exec_alloc_strided_double()
#define OUT_EXEC_ALLOC_STRIDED_DOUBLE \
    OUT0( "JD_exec_alloc_strided_double() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_STRING  JD_exec_alloc_strided_string()
#define OUT_EXEC_ALLOC_STRIDED_STRING \
    OUT0( "JD_exec_alloc_strided_string() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_SIGNAL  JD_exec_alloc_strided_signal()
#define OUT_EXEC_ALLOC_STRIDED_SIGNAL \
    OUT0( "JD_exec_alloc_strided_signal() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_OBJ  JD_exec_alloc_strided_obj()
#define OUT_EXEC_ALLOC_STRIDED_OBJ \
    OUT0( "JD_exec_alloc_strided_obj() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_STRIDED_THREAD  JD_exec_alloc_strided_thread()
#define OUT_EXEC_ALLOC_STRIDED_THREAD \
    OUT0( "JD_exec_alloc_strided_thread() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_INT  JD_exec_alloc_list_int()
#define OUT_EXEC_ALLOC_LIST_INT \
    OUT0( "JD_exec_alloc_list_int() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_BIT  JD_exec_alloc_list_bit()
#define OUT_EXEC_ALLOC_LIST_BIT \
    OUT0( "JD_exec_alloc_list_bit() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_FLOAT  JD_exec_alloc_list_float()
#define OUT_EXEC_ALLOC_LIST_FLOAT \
    OUT0( "JD_exec_alloc_list_float() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_DOUBLE  JD_exec_alloc_list_double()
#define OUT_EXEC_ALLOC_LIST_DOUBLE \
    OUT0( "JD_exec_alloc_list_double() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_STRING  JD_exec_alloc_list_string()
#define OUT_EXEC_ALLOC_LIST_STRING \
    OUT0( "JD_exec_alloc_list_string() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_SIGNAL  JD_exec_alloc_list_signal()
#define OUT_EXEC_ALLOC_LIST_SIGNAL \
    OUT0( "JD_exec_alloc_list_signal() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_OBJ  JD_exec_alloc_list_obj()
#define OUT_EXEC_ALLOC_LIST_OBJ \
    OUT0( "JD_exec_alloc_list_obj() ;\n" ) ; 
#  define CALL_EXEC_ALLOC_LIST_THREAD  JD_exec_alloc_list_thread()
#define OUT_EXEC_ALLOC_LIST_THREAD \
    OUT0( "JD_exec_alloc_list_thread() ;\n" ) ; 
#ifdef JEDA_INLINE_OPTIMIZATION
#  define CALL_EXEC_LOADM_SINGLE  { \
   {  /* TMP_INT holds local variable index */\
     JD_data *dt ;\
     if( ACC_DT->data.obj.ect == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     dt = ACC_DT->data.obj.ect->table[TMP_INT] ;\
     while( dt->type == JD_D_pointer ) {\
       dt = JD_get_ptr_element(dt) ;\
     }\
     CALL_EXEC_COPY_ACC(dt) ;\
   }\
  }
#else
#  define CALL_EXEC_LOADM_SINGLE  JD_exec_loadm_single()
#endif
#define OUT_EXEC_LOADM_SINGLE \
  if( inline_optimization ) {  \
    OUT0( "    /* Macro for JD_exec_loadm_single */ \n" ) ; \
    OUT0( "  {  /* TMP_INT holds local variable index */\n" ) ;\
    OUT0( "    JD_data *dt ;\n" ) ;\
    OUT0( "    if( ACC_DT->data.obj.ect == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    dt = ACC_DT->data.obj.ect->table[TMP_INT] ;\n" ) ;\
    OUT0( "    while( dt->type == JD_D_pointer ) {\n" ) ;\
    OUT0( "      dt = JD_get_ptr_element(dt) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    CALL_EXEC_COPY_ACC(dt) ;\n" ) ;\
    OUT0( "  }\n" ) ;\
  } \
  else { \
    OUT0( "JD_exec_loadm_single() ;\n" ) ; \
  }
#  define CALL_EXEC_LOADM_SINGLE_VAR  JD_exec_loadm_single_var()
#define OUT_EXEC_LOADM_SINGLE_VAR \
    OUT0( "JD_exec_loadm_single_var() ;\n" ) ; 
#  define CALL_EXEC_LOADM_ARRAY  JD_exec_loadm_array()
#define OUT_EXEC_LOADM_ARRAY \
    OUT0( "JD_exec_loadm_array() ;\n" ) ; 
#  define CALL_EXEC_LOADM_MARRAY  JD_exec_loadm_marray()
#define OUT_EXEC_LOADM_MARRAY \
    OUT0( "JD_exec_loadm_marray() ;\n" ) ; 
#  define CALL_EXEC_LOADM_ASSOC  JD_exec_loadm_assoc()
#define OUT_EXEC_LOADM_ASSOC \
    OUT0( "JD_exec_loadm_assoc() ;\n" ) ; 
#  define CALL_EXEC_LOADM_STRIDX  JD_exec_loadm_stridx()
#define OUT_EXEC_LOADM_STRIDX \
    OUT0( "JD_exec_loadm_stridx() ;\n" ) ; 
#  define CALL_EXEC_LOADM_LIST  JD_exec_loadm_list()
#define OUT_EXEC_LOADM_LIST \
    OUT0( "JD_exec_loadm_list() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_INT  JD_exec_storem_single_int()
#define OUT_EXEC_STOREM_SINGLE_INT \
    OUT0( "JD_exec_storem_single_int() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_FLT_DBL  JD_exec_storem_single_flt_dbl()
#define OUT_EXEC_STOREM_SINGLE_FLT_DBL \
    OUT0( "JD_exec_storem_single_flt_dbl() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_BIT  JD_exec_storem_single_bit()
#define OUT_EXEC_STOREM_SINGLE_BIT \
    OUT0( "JD_exec_storem_single_bit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_STRING_TO_SINGLE_BIT  JD_exec_storem_string_to_single_bit()
#define OUT_EXEC_STOREM_STRING_TO_SINGLE_BIT \
    OUT0( "JD_exec_storem_string_to_single_bit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_STRING  JD_exec_storem_single_string()
#define OUT_EXEC_STOREM_SINGLE_STRING \
    OUT0( "JD_exec_storem_single_string() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_STRING_NULL  JD_exec_storem_single_string_null()
#define OUT_EXEC_STOREM_SINGLE_STRING_NULL \
    OUT0( "JD_exec_storem_single_string_null() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_OBJ  JD_exec_storem_single_obj()
#define OUT_EXEC_STOREM_SINGLE_OBJ \
    OUT0( "JD_exec_storem_single_obj() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_OBJ_NULL  JD_exec_storem_single_obj_null()
#define OUT_EXEC_STOREM_SINGLE_OBJ_NULL \
    OUT0( "JD_exec_storem_single_obj_null() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_THREAD  JD_exec_storem_single_thread()
#define OUT_EXEC_STOREM_SINGLE_THREAD \
    OUT0( "JD_exec_storem_single_thread() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_THREAD_NULL  JD_exec_storem_single_thread_null()
#define OUT_EXEC_STOREM_SINGLE_THREAD_NULL \
    OUT0( "JD_exec_storem_single_thread_null() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_SIGNAL  JD_exec_storem_single_signal()
#define OUT_EXEC_STOREM_SINGLE_SIGNAL \
    OUT0( "JD_exec_storem_single_signal() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_SIGNAL_NULL  JD_exec_storem_single_signal_null()
#define OUT_EXEC_STOREM_SINGLE_SIGNAL_NULL \
    OUT0( "JD_exec_storem_single_signal_null() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SINGLE_DT  JD_exec_storem_single_dt()
#define OUT_EXEC_STOREM_SINGLE_DT \
    OUT0( "JD_exec_storem_single_dt() ;\n" ) ; 
#  define CALL_EXEC_STOREM_SUBBIT  JD_exec_storem_subbit()
#define OUT_EXEC_STOREM_SUBBIT \
    OUT0( "JD_exec_storem_subbit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_ARRAY  JD_exec_storem_array()
#define OUT_EXEC_STOREM_ARRAY \
    OUT0( "JD_exec_storem_array() ;\n" ) ; 
#  define CALL_EXEC_STOREM_MARRAY  JD_exec_storem_marray()
#define OUT_EXEC_STOREM_MARRAY \
    OUT0( "JD_exec_storem_marray() ;\n" ) ; 
#  define CALL_EXEC_STOREM_ASSOC  JD_exec_storem_assoc()
#define OUT_EXEC_STOREM_ASSOC \
    OUT0( "JD_exec_storem_assoc() ;\n" ) ; 
#  define CALL_EXEC_STOREM_STRIDX  JD_exec_storem_stridx()
#define OUT_EXEC_STOREM_STRIDX \
    OUT0( "JD_exec_storem_stridx() ;\n" ) ; 
#  define CALL_EXEC_STOREM_ARRAY_SUBBIT  JD_exec_storem_array_subbit()
#define OUT_EXEC_STOREM_ARRAY_SUBBIT \
    OUT0( "JD_exec_storem_array_subbit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_MARRAY_SUBBIT  JD_exec_storem_marray_subbit()
#define OUT_EXEC_STOREM_MARRAY_SUBBIT \
    OUT0( "JD_exec_storem_marray_subbit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_ASSOC_SUBBIT  JD_exec_storem_assoc_subbit()
#define OUT_EXEC_STOREM_ASSOC_SUBBIT \
    OUT0( "JD_exec_storem_assoc_subbit() ;\n" ) ; 
#  define CALL_EXEC_STOREM_STRIDX_SUBBIT  JD_exec_storem_stridx_subbit()
#define OUT_EXEC_STOREM_STRIDX_SUBBIT \
    OUT0( "JD_exec_storem_stridx_subbit() ;\n" ) ; 
