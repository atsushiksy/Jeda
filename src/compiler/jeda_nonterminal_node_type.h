
%type <block>	translation_units translation_unit 
%type <block>	portset_definition connectionset_definition 
%type <block>	local_global_variable_declaration top_initial_statement 
%type <block>	function_definition class_definition 
%type <block>	verilog_class_definition verilog_task_definition 
%type <block>	verilog_task_definitions new_function_definition 
%type <block>	port_definition aspect_definition 
%type <block>	advice_definition 
%type <expression>	actual_arguments actual_argument 
%type <expression>	new_expression opt_new_args 
%type <expression>	opt_new_object assign_expression 
%type <expression>	left_expression opt_bit_range 
%type <expression>	bit_range opt_array_range 
%type <expression>	array_ranges array_range 
%type <expression>	opt_var_init var_init 
%type <expression>	opt_arg_init arg_init 
%type <expression>	opt_statement_prefix cycle_element 
%type <expression>	opt_clock_definition opt_multi_expressions 
%type <expression>	multi_expressions for_expression 
%type <expression>	constant number 
%type <expression>	expression opt_expression 
%type <expression>	timed_expression time_range 
%type <expression>	parallel_eval_expression timed_expression_list 
%type <expression>	a_ver_expression opt_access_range 
%type <expression>	access_range bit_access_range 
%type <expression>	array_access_ranges array_access_range 
%type <expression>	function_call_expression opt_actual_arguments 
%type <expression>	bool_null_expression state_expression 
%type <expression>	concatenation_expression list_expression 
%type <expression>	varport_expression port_declarations 
%type <expression>	port_declaration opt_port_attribs 
%type <expression>	port_attribs port_attrib 
%type <expression>	port_expression constant_concatenation 
%type <expression>	constant_list expression_list 
%type <expression>	clock_attrib drive_attrib 
%type <expression>	sample_attrib depth_attrib 
%type <expression>	node_attrib opt_depth 
%type <expression>	opt_skew string_constant 
%type <expression>	string_list opt_super_new_call 
%type <expression>	enum_expression pointcut_pattern 
%type <expression>	pointcut_element rand_val 
%type <expression>	rand_value rand_values 
%type <expression>	rand_range rand_arg 
%type <expression>	rand_parm rand_args 
%type <expression>	opt_rand_args static_random 
%type <expression>	rand_cond rnd_constant 
%type <expression>	rnd_string 
%type <statement>	initial_statement opt_class_items 
%type <statement>	class_items class_item 
%type <statement>	verilog_task_call_statement opt_aspect_items 
%type <statement>	aspect_items aspect_item 
%type <statement>	statement block 
%type <statement>	opt_statements statements 
%type <statement>	opt_var_enum_definitions var_enum_definitions 
%type <statement>	var_enum_definition var_definition 
%type <statement>	opt_formal_args formal_args 
%type <statement>	formal_arg opt_outer_block 
%type <statement>	null_statement if_else_statement 
%type <statement>	opt_else while_statement 
%type <statement>	for_statement repeat_statement 
%type <statement>	do_while_statement forever_statement 
%type <statement>	case_statement opt_case_items 
%type <statement>	case_items case_item 
%type <statement>	casex_statement casez_statement 
%type <statement>	randcase_statement opt_randcase_items 
%type <statement>	randcase_items randcase_item 
%type <statement>	break_statement continue_statement 
%type <statement>	fork_join_statement terminate_statement 
%type <statement>	breakpoint_statement expression_statement 
%type <statement>	assign_statement return_statement 
%type <statement>	function_call_statement enum_type_definition 
%type <statement>	type_definition opt_new_outer_block 
%type <statement>	new_block pointcut_spec 
%type <statement>	pointcut_specifications 
%type <integer_value>	opt_var 
%type <assign_opr>	assign_opr 
%type <string_value>	opt_exit_label label 
%type <string_value>	opt_string 
%type <join>	joins 
%type <edge>	edge_specification 
%type <vfattr>	opt_var_func_attrib var_func_attribs 
%type <vfattr>	var_func_attrib opt_extern 
%type <name>	opt_func_type var_func_type 
%type <name>	data_type data_type_name 
%type <name>	user_data_type enum_members_list 
%type <name>	enum_member class_type_definition 
%type <name>	class_name_list var_name_list 
%type <name>	var_name func_name 
%type <name>	arg_name class_name 
%type <name>	class_name_ref portset_name 
%type <name>	port_name portset_port 
%type <name>	opt_extention opt_void 
%type <name>	clock_port_name aspect_name 
%type <name>	advice_name 
%type <exp_type>	parallel_primitive 
%type <strength>	opt_strength 
%type <portdir>	port_direction 
%type <connection>	port_connections port_connection 
