/********************************************/
/* macro generated by gen_code_out_macro_prod.pl */
/********************************************/
#  define CALL_EXEC_STORE_PORT  JD_exec_store_port()
#define OUT_EXEC_STORE_PORT \
    OUT0( "JD_exec_store_port() ;\n" ) ; 
#  define CALL_EXEC_STORE_PORT_SUBBIT  JD_exec_store_port_subbit()
#define OUT_EXEC_STORE_PORT_SUBBIT \
    OUT0( "JD_exec_store_port_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G  JD_exec_store_sig_g()
#define OUT_EXEC_STORE_SIG_G \
    OUT0( "JD_exec_store_sig_g() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_SUBBIT  JD_exec_store_sig_g_subbit()
#define OUT_EXEC_STORE_SIG_G_SUBBIT \
    OUT0( "JD_exec_store_sig_g_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_ARRAY  JD_exec_store_sig_g_array()
#define OUT_EXEC_STORE_SIG_G_ARRAY \
    OUT0( "JD_exec_store_sig_g_array() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_ARRAY_SUBBIT  JD_exec_store_sig_g_array_subbit()
#define OUT_EXEC_STORE_SIG_G_ARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_g_array_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_MARRAY  JD_exec_store_sig_g_marray()
#define OUT_EXEC_STORE_SIG_G_MARRAY \
    OUT0( "JD_exec_store_sig_g_marray() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_MARRAY_SUBBIT  JD_exec_store_sig_g_marray_subbit()
#define OUT_EXEC_STORE_SIG_G_MARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_g_marray_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_ASSOC  JD_exec_store_sig_g_assoc()
#define OUT_EXEC_STORE_SIG_G_ASSOC \
    OUT0( "JD_exec_store_sig_g_assoc() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_ASSOC_SUBBIT  JD_exec_store_sig_g_assoc_subbit()
#define OUT_EXEC_STORE_SIG_G_ASSOC_SUBBIT \
    OUT0( "JD_exec_store_sig_g_assoc_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_STRIDX  JD_exec_store_sig_g_stridx()
#define OUT_EXEC_STORE_SIG_G_STRIDX \
    OUT0( "JD_exec_store_sig_g_stridx() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_G_STRIDX_SUBBIT  JD_exec_store_sig_g_stridx_subbit()
#define OUT_EXEC_STORE_SIG_G_STRIDX_SUBBIT \
    OUT0( "JD_exec_store_sig_g_stridx_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L  JD_exec_store_sig_l()
#define OUT_EXEC_STORE_SIG_L \
    OUT0( "JD_exec_store_sig_l() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_SUBBIT  JD_exec_store_sig_l_subbit()
#define OUT_EXEC_STORE_SIG_L_SUBBIT \
    OUT0( "JD_exec_store_sig_l_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_ARRAY  JD_exec_store_sig_l_array()
#define OUT_EXEC_STORE_SIG_L_ARRAY \
    OUT0( "JD_exec_store_sig_l_array() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_ARRAY_SUBBIT  JD_exec_store_sig_l_array_subbit()
#define OUT_EXEC_STORE_SIG_L_ARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_l_array_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_MARRAY  JD_exec_store_sig_l_marray()
#define OUT_EXEC_STORE_SIG_L_MARRAY \
    OUT0( "JD_exec_store_sig_l_marray() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_MARRAY_SUBBIT  JD_exec_store_sig_l_marray_subbit()
#define OUT_EXEC_STORE_SIG_L_MARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_l_marray_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_ASSOC  JD_exec_store_sig_l_assoc()
#define OUT_EXEC_STORE_SIG_L_ASSOC \
    OUT0( "JD_exec_store_sig_l_assoc() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_ASSOC_SUBBIT  JD_exec_store_sig_l_assoc_subbit()
#define OUT_EXEC_STORE_SIG_L_ASSOC_SUBBIT \
    OUT0( "JD_exec_store_sig_l_assoc_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_STRIDX  JD_exec_store_sig_l_stridx()
#define OUT_EXEC_STORE_SIG_L_STRIDX \
    OUT0( "JD_exec_store_sig_l_stridx() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_L_STRIDX_SUBBIT  JD_exec_store_sig_l_stridx_subbit()
#define OUT_EXEC_STORE_SIG_L_STRIDX_SUBBIT \
    OUT0( "JD_exec_store_sig_l_stridx_subbit() ;\n" ) ; 
#ifdef JEDA_INLINE_OPTIMIZATION
#  define CALL_EXEC_STORE_SIG_M  { \
   {  /* TMP_INT holds local variable index */\
     JD_data *dt ;\
     JD_data_unit *ut ;\
     JD_pli_object *obj ;\
     if( TMP_DT->data.obj.ect == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     dt = TMP_DT->data.obj.ect->table[TMP_INT] ;\
     while( dt->type == JD_D_pointer ) {\
       dt = JD_get_ptr_element(dt) ;\
     }\
     obj = (JD_pli_object *)dt->data.signal.port ;\
     if( obj == NULL ) {\
       ERROR_NULL_SIGNAL_ACCESS ;\
     }\
     if( dt->data.signal.data0 < 0 ) {\
       JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;\
     }\
     else {\
       U_BIT = dt->data.signal.data1 ;\
       L_BIT = dt->data.signal.data0 ;\
       JD_drive_port(\
         obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH \
       ) ;\
     }\
   }\
  }
#else
#  define CALL_EXEC_STORE_SIG_M  JD_exec_store_sig_m()
#endif
#define OUT_EXEC_STORE_SIG_M \
  if( inline_optimization > 1 ) {  \
    OUT0( "    /* Macro for JD_exec_store_sig_m */ \n" ) ; \
    OUT0( "  {  /* TMP_INT holds local variable index */\n" ) ;\
    OUT0( "    JD_data *dt ;\n" ) ;\
    OUT0( "    JD_data_unit *ut ;\n" ) ;\
    OUT0( "    JD_pli_object *obj ;\n" ) ;\
    OUT0( "    if( TMP_DT->data.obj.ect == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    dt = TMP_DT->data.obj.ect->table[TMP_INT] ;\n" ) ;\
    OUT0( "    while( dt->type == JD_D_pointer ) {\n" ) ;\
    OUT0( "      dt = JD_get_ptr_element(dt) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    obj = (JD_pli_object *)dt->data.signal.port ;\n" ) ;\
    OUT0( "    if( obj == NULL ) {\n" ) ;\
    OUT0( "      ERROR_NULL_SIGNAL_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    if( dt->data.signal.data0 < 0 ) {\n" ) ;\
    OUT0( "      JD_drive_port( obj, ACC_A, ACC_B, ACC_SIZE, 0, 0, 0, DRV_STRENGTH ) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    else {\n" ) ;\
    OUT0( "      U_BIT = dt->data.signal.data1 ;\n" ) ;\
    OUT0( "      L_BIT = dt->data.signal.data0 ;\n" ) ;\
    OUT0( "      JD_drive_port(\n" ) ;\
    OUT0( "        obj, ACC_A, ACC_B, ACC_SIZE, 1, U_BIT, L_BIT, DRV_STRENGTH \n" ) ;\
    OUT0( "      ) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "  }\n" ) ;\
  } \
  else { \
    OUT0( "JD_exec_store_sig_m() ;\n" ) ; \
  }
#  define CALL_EXEC_STORE_SIG_M_SUBBIT  JD_exec_store_sig_m_subbit()
#define OUT_EXEC_STORE_SIG_M_SUBBIT \
    OUT0( "JD_exec_store_sig_m_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_ARRAY  JD_exec_store_sig_m_array()
#define OUT_EXEC_STORE_SIG_M_ARRAY \
    OUT0( "JD_exec_store_sig_m_array() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_ARRAY_SUBBIT  JD_exec_store_sig_m_array_subbit()
#define OUT_EXEC_STORE_SIG_M_ARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_m_array_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_MARRAY  JD_exec_store_sig_m_marray()
#define OUT_EXEC_STORE_SIG_M_MARRAY \
    OUT0( "JD_exec_store_sig_m_marray() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_MARRAY_SUBBIT  JD_exec_store_sig_m_marray_subbit()
#define OUT_EXEC_STORE_SIG_M_MARRAY_SUBBIT \
    OUT0( "JD_exec_store_sig_m_marray_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_ASSOC  JD_exec_store_sig_m_assoc()
#define OUT_EXEC_STORE_SIG_M_ASSOC \
    OUT0( "JD_exec_store_sig_m_assoc() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_ASSOC_SUBBIT  JD_exec_store_sig_m_assoc_subbit()
#define OUT_EXEC_STORE_SIG_M_ASSOC_SUBBIT \
    OUT0( "JD_exec_store_sig_m_assoc_subbit() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_STRIDX  JD_exec_store_sig_m_stridx()
#define OUT_EXEC_STORE_SIG_M_STRIDX \
    OUT0( "JD_exec_store_sig_m_stridx() ;\n" ) ; 
#  define CALL_EXEC_STORE_SIG_M_STRIDX_SUBBIT  JD_exec_store_sig_m_stridx_subbit()
#define OUT_EXEC_STORE_SIG_M_STRIDX_SUBBIT \
    OUT0( "JD_exec_store_sig_m_stridx_subbit() ;\n" ) ; 
#  define CALL_EXEC_LOAD_PORT  JD_exec_load_port()
#define OUT_EXEC_LOAD_PORT \
    OUT0( "JD_exec_load_port() ;\n" ) ; 
#  define CALL_EXEC_LOAD_PORT_SUB  JD_exec_load_port_sub()
#define OUT_EXEC_LOAD_PORT_SUB \
    OUT0( "JD_exec_load_port_sub() ;\n" ) ; 
#  define CALL_EXEC_LOAD_PORT_PTR  JD_exec_load_port_ptr()
#define OUT_EXEC_LOAD_PORT_PTR \
    OUT0( "JD_exec_load_port_ptr() ;\n" ) ; 
#  define CALL_EXEC_LOAD_PORT_PTR_SUB  JD_exec_load_port_ptr_sub()
#define OUT_EXEC_LOAD_PORT_PTR_SUB \
    OUT0( "JD_exec_load_port_ptr_sub() ;\n" ) ; 
#  define CALL_EXEC_LOAD_SIG_PT  JD_exec_load_sig_pt()
#define OUT_EXEC_LOAD_SIG_PT \
    OUT0( "JD_exec_load_sig_pt() ;\n" ) ; 
#  define CALL_EXEC_LOAD_SIG_PT_SUB  JD_exec_load_sig_pt_sub()
#define OUT_EXEC_LOAD_SIG_PT_SUB \
    OUT0( "JD_exec_load_sig_pt_sub() ;\n" ) ; 
#  define CALL_EXEC_SYNC_PORT  JD_exec_sync_port()
#define OUT_EXEC_SYNC_PORT \
    OUT0( "JD_exec_sync_port() ;\n" ) ; 
#  define CALL_EXEC_SYNC_CLOCK_OF_PORT  JD_exec_sync_clk_of_port()
#define OUT_EXEC_SYNC_CLOCK_OF_PORT \
    OUT0( "JD_exec_sync_clk_of_port() ;\n" ) ; 
#ifdef JEDA_INLINE_OPTIMIZATION
#  define CALL_EXEC_SYNC_CLOCK_OF_SIGNAL  { \
   {  \
     JD_data *dt, *clk ;\
     JD_pli_object *obj ;\
     dt = ACC_DT ;\
     if( dt == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     while( dt->type == JD_D_pointer ) {\
       dt = JD_get_ptr_element(dt) ;\
     }\
     obj = (JD_pli_object *)dt->data.signal.port ;\
     if( obj == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     clk = GLOBAL_DATA[obj->is.port.clk_index] ;\
     if( clk == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     obj = (JD_pli_object *)clk->data.signal.port ;\
     if( obj == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     CALL_EXEC_POP_ACC ; /* num_cycle */\
     JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;\
     if( THREAD->terminated ) {\
       JD_thread_exit( THREAD ) ;\
     }\
   }\
  }
#else
#  define CALL_EXEC_SYNC_CLOCK_OF_SIGNAL  JD_exec_sync_clk_of_signal()
#endif
#define OUT_EXEC_SYNC_CLOCK_OF_SIGNAL \
  if( inline_optimization > 1 ) {  \
    OUT0( "    /* Macro for JD_exec_sync_clk_of_signal */ \n" ) ; \
    OUT0( "  {  \n" ) ;\
    OUT0( "    JD_data *dt, *clk ;\n" ) ;\
    OUT0( "    JD_pli_object *obj ;\n" ) ;\
    OUT0( "    dt = ACC_DT ;\n" ) ;\
    OUT0( "    if( dt == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    while( dt->type == JD_D_pointer ) {\n" ) ;\
    OUT0( "      dt = JD_get_ptr_element(dt) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    obj = (JD_pli_object *)dt->data.signal.port ;\n" ) ;\
    OUT0( "    if( obj == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    clk = GLOBAL_DATA[obj->is.port.clk_index] ;\n" ) ;\
    OUT0( "    if( clk == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    obj = (JD_pli_object *)clk->data.signal.port ;\n" ) ;\
    OUT0( "    if( obj == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    CALL_EXEC_POP_ACC ; /* num_cycle */\n" ) ;\
    OUT0( "    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;\n" ) ;\
    OUT0( "    if( THREAD->terminated ) {\n" ) ;\
    OUT0( "      JD_thread_exit( THREAD ) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "  }\n" ) ;\
  } \
  else { \
    OUT0( "JD_exec_sync_clk_of_signal() ;\n" ) ; \
  }
#ifdef JEDA_INLINE_OPTIMIZATION
#  define CALL_EXEC_SYNC_SIGNAL  { \
   {  \
     JD_data *dt ;\
     JD_pli_object *obj ;\
     dt = ACC_DT ;\
     if( dt == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     while( dt->type == JD_D_pointer ) {\
       dt = JD_get_ptr_element(dt) ;\
     }\
     obj = (JD_pli_object *)dt->data.signal.port ;\
     if( obj == NULL ) {\
       ERROR_EMPTY_OBJECT_ACCESS ;\
     }\
     CALL_EXEC_POP_ACC ; /* num_cycle */\
     JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;\
   }\
  }
#else
#  define CALL_EXEC_SYNC_SIGNAL  JD_exec_sync_signal()
#endif
#define OUT_EXEC_SYNC_SIGNAL \
  if( inline_optimization > 1 ) {  \
    OUT0( "    /* Macro for JD_exec_sync_signal */ \n" ) ; \
    OUT0( "  {  \n" ) ;\
    OUT0( "    JD_data *dt ;\n" ) ;\
    OUT0( "    JD_pli_object *obj ;\n" ) ;\
    OUT0( "    dt = ACC_DT ;\n" ) ;\
    OUT0( "    if( dt == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    while( dt->type == JD_D_pointer ) {\n" ) ;\
    OUT0( "      dt = JD_get_ptr_element(dt) ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    obj = (JD_pli_object *)dt->data.signal.port ;\n" ) ;\
    OUT0( "    if( obj == NULL ) {\n" ) ;\
    OUT0( "      ERROR_EMPTY_OBJECT_ACCESS ;\n" ) ;\
    OUT0( "    }\n" ) ;\
    OUT0( "    CALL_EXEC_POP_ACC ; /* num_cycle */\n" ) ;\
    OUT0( "    JD_sync_on_port( obj, TMP_INT2, ACC_INT ) ;\n" ) ;\
    OUT0( "  }\n" ) ;\
  } \
  else { \
    OUT0( "JD_exec_sync_signal() ;\n" ) ; \
  }
