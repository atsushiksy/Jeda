static keyword_entry keyword_table[] = {
  {  "Qeqeq",		VI_Qeqeq },
  {  "abstract",		VI_abstract },
  {  "aspect",		VI_aspect },
  {  "advice",		VI_advice },
  {  "advice_end",		VI_advice_end },
  {  "afunc",		VI_afunc },
  {  "alloc",		VI_alloc },
  {  "alloc_func",		VI_alloc_func },
  {  "alloc_static",		VI_alloc_static },
  {  "alu",		VI_alu },
  {  "and",		VI_and },
  {  "arg",		VI_arg },
  {  "array",		VI_array },
  {  "assoc",		VI_assoc },
  {  "bit",		VI_bit },
  {  "bothedge",		VI_bothedge },
  {  "breakpoint",		VI_breakpoint },
  {  "call",		VI_call },
  {  "call_m",		VI_call_m },
  {  "call_a",		VI_call_a },
  {  "call_advice",		VI_call_advice },
  {  "cancel",		VI_cancel },
  {  "chk_overwrite",		VI_chk_overwrite },
  {  "class",		VI_class },
  {  "class_alloc",		VI_class_alloc },
  {  "class_alloc_end",		VI_class_alloc_end },
  {  "class_initial_begin",		VI_class_initial_begin },
  {  "class_initial_end",		VI_class_initial_end },
  {  "class_type",		VI_class_type },
  {  "clk_of_port",		VI_clk_of_port },
  {  "clk_of_pset_port",		VI_clk_of_pset_port },
  {  "clk_of_signal",		VI_clk_of_signal },
  {  "clone",		VI_clone },
  {  "cmp",		VI_cmp },
  {  "cmpx",		VI_cmpx },
  {  "cmpz",		VI_cmpz },
  {  "concat",		VI_concat },
  {  "connection",		VI_connection },
  {  "copy",		VI_copy },
  {  "copy_obj",		VI_copy_obj },
  {  "convert",		VI_convert },
  {  "dec",		VI_dec },
  {  "depth",		VI_depth },
  {  "div",		VI_div },
  {  "double",		VI_double },
  {  "drive",		VI_drive },
  {  "dup",		VI_dup },
  {  "duplicate",		VI_duplicate },
  {  "enter_advice",		VI_enter_advice },
  {  "enum",		VI_enum },
  {  "eor",		VI_eor },
  {  "eqQeq",		VI_eqQeq },
  {  "eqeq",		VI_eqeq },
  {  "eqeqQ",		VI_eqeqQ },
  {  "eqeqeq",		VI_eqeqeq },
  {  "eqCeq",		VI_eqCeq },
  {  "Qne",		VI_Qne },
  {  "error",		VI_error },
  {  "exec",		VI_exec },
  {  "exit",		VI_exit },
  {  "export",		VI_export },
  {  "extends",		VI_extends },
  {  "extern",		VI_extern },
  {  "final",		VI_final },
  {  "float",		VI_float },
  {  "fork",		VI_fork },
  {  "formal",		VI_formal },
  {  "func",		VI_func },
  {  "function",		VI_function },
  {  "func_end",		VI_func_end },
  {  "ge",		VI_ge },
  {  "gen_child",		VI_gen_child },
  {  "gen_frame",		VI_gen_frame },
  {  "gen_m_frame",		VI_gen_m_frame },
  {  "gen_a_frame",		VI_gen_a_frame },
  {  "get_this",		VI_get_this },
  {  "get_sig_subbit",		VI_get_sig_subbit },
  {  "get_subbit",		VI_get_subbit },
  {  "get_super",		VI_get_super },
  {  "get_child",		VI_get_child },
  {  "get_return",		VI_get_return },
  {  "global",		VI_global },
  {  "gt",		VI_gt },
  {  "i_endian",		VI_i_endian },
  {  "inc",		VI_inc },
  {  "input",		VI_input },
  {  "inout",		VI_inout },
  {  "initial_begin",		VI_initial_begin },
  {  "initial_end",		VI_initial_end },
  {  "initial_init",		VI_initial_init },
  {  "int",		VI_int },
  {  "jedacmp",		VI_jedacmp },
  {  "jmp",		VI_jmp },
  {  "jnz",		VI_jnz },
  {  "join",		VI_join },
  {  "jz",		VI_jz },
  {  "knife",		VI_knife },
  {  "le",		VI_le },
  {  "leaf",		VI_leaf },
  {  "list",		VI_list },
  {  "load_const",		VI_load_const },
  {  "load_one",		VI_load_one },
  {  "load_port",		VI_load_port },
  {  "load_port_ptr",		VI_load_port_ptr },
  {  "load_pset_port",		VI_load_pset_port },
  {  "load_pset_port_ptr",		VI_load_pset_port_ptr },
  {  "load_sig_pt",		VI_load_sig_pt },
  {  "load_static",		VI_load_static },
  {  "load_super",		VI_load_super },
  {  "load_this",		VI_load_this },
  {  "load_thread",		VI_load_thread },
  {  "load_zero",		VI_load_zero },
  {  "loadg",		VI_loadg },
  {  "loadl",		VI_loadl },
  {  "loadm",		VI_loadm },
  {  "local",		VI_local },
  {  "long",		VI_long },
  {  "lshift",		VI_lshift },
  {  "lt",		VI_lt },
  {  "lvar",		VI_lvar },
  {  "marray",		VI_marray },
  {  "member_signal",		VI_member_signal },
  {  "mfunc",		VI_mfunc },
  {  "minus",		VI_minus },
  {  "mod",		VI_mod },
  {  "nand",		VI_nand },
  {  "ne",		VI_ne },
  {  "negedge",		VI_negedge },
  {  "neor",		VI_neor },
  {  "neqeq",		VI_neqeq },
  {  "new",		VI_new },
  {  "new_obj",		VI_new_obj },
  {  "noedge",		VI_noedge },
  {  "nor",		VI_nor },
  {  "normal",		VI_normal },
  {  "null",		VI_null },
  {  "nz_lsb",		VI_nz_lsb },
  {  "obj",		VI_obj },
  {  "or",		VI_or },
  {  "output",		VI_output },
  {  "overwrite",		VI_overwrite },
  {  "p_and",		VI_p_and },
  {  "p_or",		VI_p_or },
  {  "path",		VI_path },
  {  "plus",		VI_plus },
  {  "pop",		VI_pop },
  {  "pop_this",		VI_pop_this },
  {  "port",		VI_port },
  {  "portset",		VI_portset },
  {  "posedge",		VI_posedge },
  {  "protected",		VI_protected },
  {  "pset_port",		VI_pset_port },
  {  "ptset",		VI_ptset },
  {  "push",		VI_push },
  {  "restore_acc",		VI_restore_acc },
  {  "return",		VI_return },
  {  "return_advice",		VI_return_advice },
  {  "return_l",		VI_return_l },
  {  "return_n",		VI_return_n },
  {  "rshift",		VI_rshift },
  {  "sample",		VI_sample },
  {  "save_acc",		VI_save_acc },
  {  "signal",		VI_signal },
  {  "single",		VI_single },
  {  "skew",		VI_skew },
  {  "spoon",		VI_spoon },
  {  "stat_var",		VI_stat_var },
  {  "static",		VI_static },
  {  "static_init",		VI_static_init },
  {  "static_init_end",		VI_static_init_end },
  {  "store_adjust",		VI_store_adjust },
  {  "store_obj",		VI_store_obj },
  {  "store_port",		VI_store_port },
  {  "store_pset_port",		VI_store_pset_port },
  {  "store_sig_g",		VI_store_sig_g },
  {  "store_sig_l",		VI_store_sig_l },
  {  "store_sig_m",		VI_store_sig_m },
  {  "store_sig_static",		VI_store_sig_static },
  {  "store_static",		VI_store_static },
  {  "storeg",		VI_storeg },
  {  "storel",		VI_storel },
  {  "storem",		VI_storem },
  {  "stridx",		VI_stridx },
  {  "string",		VI_string },
  {  "strong",		VI_strong },
  {  "subbit",		VI_subbit },
  {  "sync",		VI_sync },
  {  "sys_class",		VI_sys_class },
  {  "terminate",		VI_terminate },
  {  "times",		VI_times },
  {  "to_int",		VI_to_int },
  {  "u_and",		VI_u_and },
  {  "u_eor",		VI_u_eor },
  {  "u_minus",		VI_u_minus },
  {  "u_nand",		VI_u_nand },
  {  "u_neor",		VI_u_neor },
  {  "u_nor",		VI_u_nor },
  {  "u_not",		VI_u_not },
  {  "u_or",		VI_u_or },
  {  "u_tilde",		VI_u_tilde },
  {  "urshift",		VI_urshift },
  {  "var",		VI_var },
  {  "variable",		VI_variable },
  {  "veri_call",		VI_veri_call },
  {  "verilog_task",		VI_verilog_task },
  {  "void",		VI_void },
  {  "weak",		VI_weak },
  {  NULL,  0 }
} ; 