
%type <integer_value>	int_value var_attrib 
%type <integer_value>	func_option opt_var 
%type <integer_value>	opt_static edge 
%type <integer_value>	type_code direction 
%type <integer_value>	class_attrib strength 
%type <string_value>	name label 
%type <string>	opt_extention 
%type <exp>	const_data number 
%type <inst>	object_files verilog_task_info 
%type <inst>	class_attribs opt_class_attribs 
%type <inst>	empty_line debug_information 
%type <inst>	virtual_code inital_header 
%type <inst>	vinst_units vinst_unit 
%type <inst>	port_info opt_port_attrib 
%type <inst>	port_attribs port_attrib 
%type <inst>	portset_info port_infos 
%type <inst>	connection_info variable_dcl 
%type <inst>	data_type array_range 
%type <inst>	range_list opt_var_attribs 
%type <inst>	var_attribs enum_dcl 
%type <inst>	enum_members function_info 
%type <inst>	opt_func_options func_options 
%type <inst>	opt_arguments arguments 
%type <inst>	argument class_info 
%type <inst>	opt_class_members class_members 
%type <inst>	class_member aspect_info 
%type <inst>	advice_info pointcut_name 
%type <inst>	pointcuts pointcut 
%type <inst>	pointcut_names opt_aspect_members 
%type <inst>	aspect_members aspect_member 
%type <inst>	static_var_dcl static_init_code 
%type <inst>	init_code class_init_code 
%type <inst>	class_alloc_code mfunc_code 
%type <inst>	func_code afunc_code 
%type <inst>	advice_code stype 
%type <inst>	dtype ttype 
%type <inst>	opt_flags opt_array_access 
%type <inst>	opt_bit_type 
%type <code>	opt_vinsts vinsts 
%type <code>	vinst 
%type <name>	pointcut_specs pointcut_spec 
%type <name>	opt_class_pointcut 
