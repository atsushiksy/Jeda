/****************************************************************************
      Jeda Programming Language Syntax Specification

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
   
*****************************************************************************/


1. Lexical Elements
  
  1.1 white space
    White spece does no meaning and acts as separator.
  
  1.2 comments
    Two types of comment are supported.
    
    /* text */   : A traditional C style comment
    //           : A single line comment
    
  1.3 Identifiers
    
    [a-zA-Z_][a-zA-Z_0-9]*
    
  1.4 Keywords
    

    	abstract	array	assoc	bit	bothedge	break
    	breakpoint	case	casex	casez	class	class_type
    	clone	connectionset	continue	default	depth	do
    	double	drive	else	enum	export	extends
    	extern	false	final	float	for	forever
    	fork	global	if	initial	inout	input
    	int	join	knife	list	local	long
    	marray	negedge	new	node	null	output
    	overwrite	p_and	p_or	port	portset	posedge
    	protected	repeat	return	sample	signal	skew
    	spoon	static	string	stridx	strong	super
    	teminate	terminate	this	true	typedef	var
    	verilog	void	weak	while

    ** 'long' is a keyword, but not used now.

  1.5 Separators and Operators
  

    	&~	~&	|~	~|	^~	~^
    	&&	||	>>	>>>	<<	==
    	<=	>=	!=	===	!==	=?=
    	!?=	?==	==?	=:=	++	--
    	@@	:=	+=	-=	*=	/=
    	%=	<<=	>>=	>>>=	&=	|=
    	^=	~&=	&~=	~|=	|~=	~^=
    	^~=	?	:	!	%	^
    	&	|	+	-	<	>
    	=	*	/	.	~	@
    	#	$	{	}	(	)
    	[	]	\'
  
  1.6 Constant
  
    1.6.1 Integer Constant
      Integer Constant start with non-zero digit followed by any digit.
      
        0
        [1-9][0-9]*
      
    1.6.2 C style numerical Constant
      C style numerical number constant starts with 0 and a formant character
      followed by digit characters. 0hff or 0Hff style can be used for hex.
      0xb0100 style can be used for binary.
      
       0[dDoOxXhHbB][0-9a-fA-FzZxX]+
      
    1.6.3 Verilog style numerical Literal
      Verilog style numerical numbers are also supported. 
      
        [0-9]*'[dDoOxXhHbB][0-9a-fA-FzZxX]+
      

    1.6.4 String Constant
      String constant is a sequence of caractors within " (double quote). 
      More than one line of string can be specified with \ (backslash) 
      followed by \n (new line).
      
    1.6.5 null Literal
      null represents empty status of a Jeda object.
      
       null
       
2. Program structure
        
    
    jeda:  translation_units
      ;
      
    translation_units:
      translation_unit
      |
      translation_units translation_unit
      ;
      
    translation_unit:
      port_definition
      |
      portset_definition
      |
      connectionset_definition
      |
      local_global_variable_declaration
      |
      top_initial_statement
      |
      type_definition
      |
      function_definition
      |
      class_definition
      |
      verilog_class_definition
      ;
    

3. Port and port_set

    
    port_definition:
      port_declaration
      ;
      
    port_declaration:
      port_direction 
      opt_bit_range 
      port_name 
      opt_port_attribs 
      semicolon
      ;
    
    
    port_direction:
      'input'  
      |
      'output'  
      |
      'inout'   
      ;
    
    opt_bit_range:
      /* empty */
      |
      bit_range
      ;
      
    bit_range:
      '[' constant ':' constant ']'
      ;
      
    port_name: 
      Identifier 
      ;
    
    opt_port_attribs:
      /* empty */
      |
      port_attribs
      ;
      
    port_attribs:
      port_attrib
      |
      port_attribs port_attrib
      ;
      

  3.1 port attribute

    
    port_attrib:
      clock_attrib
      |
      drive_attrib
      |
      sample_attrib
      |
      depth_attrib
      |
      node_attrib
      ;
      
    clock_attrib:
      '@' clock_port_name
      ;
      
    clock_port_name: 
      Identifier 
      |
      Identifier '.' Identifier
      ;
    
    drive_attrib:
      'drive' edge_specification opt_skew
      ;
    
    sample_attrib:
      'sample' edge_specification opt_skew 
      ;
        
    opt_skew:
      /* empty */
      |
      'skew' constant
      ;
    
    depth_attrib:
      'depth' constant
      ;
    
    node_attrib:
      'node' String_literal
      ;
    
    
       
4. port_set
        
    
    portset_definition:
      'portset' 
      portset_name opt_port_attribs
      '{'
        port_declarations
      '}' 
      ;
    
    portset_name: 
      Identifier
      ;
    
    port_declarations:
      port_declaration
      |
      port_declarations 
      port_declaration
      ;
      

5. Connection Set

    
    connectionset_definition:
      'connectionset' 
      '{' 
        port_connections
      '}' 
      ;
      
    port_connections:
      port_connection
      |
      port_connections port_connection
      ;
      
    port_connection:
      portset_port  String_literal semicolon 
      ;
      
    portset_port:
       Identifier '.' Identifier 
       |
       Identifier
       ;
    

6. Variables and enum type declarations
  
    
    opt_var_enum_definitions:
      /* empty */
      |
      var_enum_definitions
      ;
    
    var_enum_definitions:
      var_enum_definition
      |
      var_enum_definitions var_enum_definition
      ;
      
    var_enum_definition:
      var_definition
      |
      enum_type_definition 
      ;
    

7. Variable Declaration
        
    
    var_definition:
      opt_var_func_attrib var_func_type var_name_list semicolon
      ;
    
    var_name_list:
      var_name 
      |
      var_name_list ',' var_name
      ;
      
    var_name:
      Identifier opt_array_range opt_var_init
      ;
    
    /*
      An array can be statically allocated (e.g. ary[5]), string indexed
      associative array (e.g. sary[*]), associative array (e.g. asary[]),
      and list (e.g. list <> ).
    */
    opt_array_range:
      /* emptry */
      |
      array_ranges  
      |
      '['  '*'  ']' 
      |
      '['  ']'
      |
      '<'  '>' 
      ;
      
    array_ranges:
      array_range
      |
      array_ranges array_range
      ;
      
    array_range:
      '[' constant ']'
      ;
    
    opt_var_init:
      /* empty */
      |
      var_init 
      ;
      
    var_init:
      '=' expression 
      |
      '=' new_expression
      ;
    
       
8. Basic Data Types
         
    
    opt_func_type:
      /* empty */
      |
      var_func_type opt_array_range
      ;
      
    var_func_type:  
      data_type  
      |
      user_data_type 
      ;
      
    data_type:
      'void'
      |
      'int' 
      |
      'float'  
      |
      'double'  
      |
      'bit' opt_bit_range
      |
      'string'
      |
      'signal'
      |
      'class_type'
      ;
      
    user_data_type: 
      Identifier
      ;
    

9. Type Definition
  
    
    type_definition:
      enum_type_definition 
      |
      class_type_definition
      ;
      
    enum_type_definition:
      'enum' Identifier '=' enum_members_list semicolon
      ;
    
    enum_members_list:
      enum_member
      |
      enum_members_list ',' enum_member
      ;
      
    enum_member: 
      Identifier 
      ;
      
    
    class_type_definition:
      'typedef' 'class' class_name_list semicolon 
      ;
      
    class_name_list:
      class_name
      |
      class_name_list ',' class_name
      ;
    
    
    data_type_name:
      'array' 
      |
      'marray' 
      |
      'assoc' 
      |
      'stridx'  
      |
      'list'  
      |
      'string'
      |
      'signal'
      |
      'port'
      ;
    
    class_name:
      Identifier
      |
      '.' data_type_name
      ;
    
    class_name_ref:
      Identifier
      ;
    

10. Initial statement.  
 
    
    top_initial_statement:
      'initial' 
      statement
      ;
      
    initial_statement:
      'initial' 
      statement 
      ;
    
 
11. Function Declaration
  
    
    function_definition:
      opt_var_func_attrib 
      opt_func_type 
      func_name
      '(' 
        opt_formal_args 
      ')' 
      opt_outer_block
      ;
    
    func_name:
      Identifier 
      ;
    
    
    opt_outer_block:
      semicolon
      |
      block
      ;
    
    opt_formal_args:
      /* empty */
      |
      formal_args
      ;
      
    formal_args:
      formal_arg
      |
      formal_args ',' formal_arg 
      ;
    
    formal_arg:
      opt_var var_func_type arg_name opt_array_range opt_arg_init 
      |
      var_func_type
      |
      '?'
      |
      '(' formal_args ')' '+'
      |
      '(' formal_args ')' '*'
      |
      '(' formal_args ')' '|'
      |
      '[' formal_args ']' 
      ;
    
    opt_var:
      /* empty */
      |
      'var' 
      ;
      
    arg_name:
      Identifier
      ;
    
    opt_arg_init:
      /* empty */
      |
      arg_init 
      ;
      
    arg_init:
      '=' constant 
      |
      '=' string_constant
      |
      '=' bool_null_expression
      |
      '=' enum_expression
      ;
    
    enum_expression:
      Identifier
      ;
    

11.1 Attributes for variable, function and class
   
    
    /* This definition is globally shared by all the types
       for persing reason, and semantics must be handled at action level per 
       each type.
    */
    
    opt_var_func_attrib:
      /* empty */
      | 
      var_func_attribs 
      ;
      
    var_func_attribs: 
      var_func_attrib
      |
      var_func_attribs var_func_attrib
      ;
      
    var_func_attrib:
      'static'
      | 
      'extern'
      | 
      'local'
      | 
      'global'
      | 
      'protected'
      | 
      'abstract'
      | 
      'final'
      | 
      'export'
      | 
      'clone'
      |
      'overwrite'
      ;
      

12. Class Declaration
    
    
    class_definition:
      opt_var_func_attrib 'class' class_name 
      opt_extention
      '{'
        opt_class_items
      '}' 
      ;
    
    opt_extention:
      /* empty */
      |
      'extends' class_name_ref
      ;
      
    opt_class_items:
      /* empty */
      |
      class_items
      ;
      
    class_items:
      class_item
      |
      class_items class_item
      ;
    
    class_item:
      var_definition
      |
      enum_type_definition 
      |
      initial_statement
      |
      function_definition
      |
      new_function_definition
      ;
      
    new_function_definition:
      opt_void 'new'
      '(' 
        opt_formal_args 
      ')' 
      opt_new_outer_block
      ;
    
    opt_new_outer_block:
      semicolon
      |
      new_block
      ;
    
    new_block:
      '{'
        opt_var_enum_definitions
        opt_super_new_call
        opt_statements
      '}'
      ;
    
    opt_super_new_call:
      /* empty */
      |
      'super' '.' 'new' opt_new_args semicolon
      ;
    
    opt_void:
      /* empty */
      |
      'void'
      ;
      

13. Statement
  
    
    statement:
      null_statement
      |
      if_else_statement 
      |
      while_statement 
      |
      for_statement 
      |
      repeat_statement 
      |
      do_while_statement 
      |
      forever_statement 
      |
      case_statement 
      |
      casex_statement 
      |
      casez_statement 
      |
      break_statement 
      |
      continue_statement
      |
      fork_join_statement
      |
      terminate_statement
      |
      breakpoint_statement
      |
      expression_statement
      |
      assign_statement
      |
      function_call_statement
      |
      return_statement
      |
      verilog_task_call_statement
      |
      block
      ;
    

  13.1 Block
  
    
    block:
      '{'
        opt_var_enum_definitions
        opt_statements
      '}'
      ;
    
    opt_statements:
      /* empty */
      |
      statements
      ;
    
    statements:
      statement
      |
      statements statement
      ;
    

  13.2 Statement Prefix
  
    
    opt_statement_prefix:
      /* empty */
      |
      cycle_element
      |
      label
      |
      label cycle_element
      ;
    
    label:
      Identifier ':'
      ;
      
    cycle_element:
      '@' opt_expression opt_clock_definition 
      ;
      
    opt_clock_definition:
      /* empty */
      |
      '(' edge_specification  port_expression ')'
      ;
      
    edge_specification:
      'posedge' 
      |
      'negedge' 
      |
      'bothedge' 
      |
      /* empty */
      ;
    
    port_expression:
      varport_expression 
      ;
    
  
  13.3. Null statement

    
    null_statement:
      opt_statement_prefix semicolon
      ;
    

  13.4 If else statement

    
    if_else_statement:
      opt_statement_prefix 'if' '(' expression ')' statement
      opt_else
      ;
    
    opt_else:
      /* empty */
      |
      'else' statement
      ;


  13.5 While statement

    
    while_statement:
      opt_statement_prefix 'while' '(' expression ')' 
      statement 
      ;
    

  13.6 for statement

    
    for_statement:
      opt_statement_prefix 'for' '(' 
        opt_multi_expressions ';' opt_expression ';' opt_multi_expressions
      ')' 
      statement 
      ;
      
    opt_multi_expressions:
      /* empty */
      |
      multi_expressions
      ;
    
    multi_expressions:
      for_expression
      |
      multi_expressions ',' for_expression
      ;
      
    for_expression:
      state_expression 
      |
      assign_expression
      ;
      

  13.7 repeat statement

    
    repeat_statement:
      opt_statement_prefix 'repeat' '(' expression ')' 
      statement 
      ;
    

  13.8 Do while statement

    
    do_while_statement:
      opt_statement_prefix 'do' 
      statement 'while' '(' expression ')' semicolon 
      ;
    

  13.9 Forever statement
    
    
    forever_statement:
      opt_statement_prefix 'forever' 
      statement 
      ;
    

  13.10 Case statement

    
    case_statement:
      opt_statement_prefix 'case' '(' expression ')' '{'
        opt_case_items
      '}' 
      ;
    
    opt_case_items:
      /* empty */
      |
      case_items
      ;
    
    case_items:
      case_item
      |
      case_items case_item
      ;
        
    case_item:
      expression_list ':' statement
      |
      'default' ':' statement 
      ;
    
    expression_list:
      expression
      |
      expression_list ',' expression
      ;
    

  13.11 Casex statement

    
    casex_statement:
      opt_statement_prefix 'casex' '(' expression ')' '{'
        opt_case_items
      '}'
      ;
    

  13.12 Casez statement

    
    casez_statement:
      opt_statement_prefix 'casez' '(' expression ')' '{'
        opt_case_items
      '}' 
      ;
    

  13.13 Break statement

    
    break_statement:
      opt_statement_prefix 'break' opt_exit_label semicolon
      ;
    
    opt_exit_label:
      /* empty */
      |
      Identifier
      ;
      

  13.14 Continue statement

    
    continue_statement:
      opt_statement_prefix 'continue' opt_exit_label semicolon 
      ;
    

  13.15 Fork join statement

    
    fork_join_statement:
      opt_statement_prefix 'fork'
        statements
      joins  
      ;
    
    joins:
      /* empty */
      'join'
      |
      'spoon' 
      |
      'knife' 
      ;
    

  13.16 Terminate statement

    
    terminate_statement:
      opt_statement_prefix 'terminate' semicolon 
      ;
    

  13.17 Breakpoint statement

    /* working here */
    breakpoint_statement:
      opt_statement_prefix 'breakpoint' opt_string semicolon 
      ;
    
    opt_string:
      /* nothing */
      |
      String_literal
      ;
      

  13.18 increment and decrement statement

    
    expression_statement:
      opt_statement_prefix state_expression semicolon 
      ;
    

  13.19 Assign statement

    
    assign_statement:
      opt_statement_prefix assign_expression opt_strength semicolon
      ;
      
    opt_strength:
      /* nothing */
      |
      'weak'
      |
      'strong'
      ;
    
  
  13.20 Function call statement

    
    function_call_statement:
      opt_statement_prefix function_call_expression semicolon 
      ;
    
    return_statement:
      'return' opt_expression semicolon
      ;
    

14. Operators and precedence rule

    /* precedence table */
    %right '>>=' '>>>=' '<<=' '&=' '|=' '^=' '~&=' '&~=' '~^=' '^~=' '~|=' '|~='
    %right '=' '+=' '-=' '*=' '/=' '%='
    %right '?' ':' 
    %left '||' 
    %left '&&' 
    %left '|' 
    %left '^'  
    %left '&'  
    %left '=:=' 
    %left '=?=' '!?=' '==' '!=' '===' '!==' '==?' '?=='
    %left '>' '<=' '<' '>=' 
    %left '<<' '>>' '>>>'
    %left '-' '+'
    %left  '*' '/' '%'
    %left  '!' '~' 
    %left  '~&' '&~' '~|' '|~' '~^' '^~'  
    %right '++' '--' 
    %left  '.'
    

15. Constant and expression
  15.1 Constant

    
    constant:
      number
      |
      constant '-' constant
      |
      constant '+' constant
      |
      constant '*' constant   
      |
      constant '/' constant   
      |
      constant '%' constant   
      |
      constant '&' constant   
      |
      constant '|' constant   
      |
      constant '^' constant   
      |
      constant '&~' constant  
      |
      constant '~&' constant  
      |
      constant '|~' constant  
      |
      constant '~|' constant  
      |
      constant '^~' constant  
      |
      constant '~^' constant  
      |
      constant '&&' constant  
      |
      constant '||' constant  
      |
      constant '>>' constant  
      |
      constant '>>>' constant 
      |
      constant '<<' constant  
      |
      constant '<' constant   
      |
      constant '>' constant   
      |
      constant '==' constant  
      |
      constant '<=' constant  
      |
      constant '>=' constant  
      |
      constant '!=' constant  
      |
      constant '===' constant 
      |
      constant '!==' constant 
      |
      constant '=?=' constant 
      |
      constant '!?=' constant 
      |
      constant '==?' constant 
      |
      constant '?==' constant 
      |
      '-' constant
      |
      '~' constant
      |
      '!' constant
      |
      '&' constant
      |
      '|' constant
      |
      '^' constant
      |
      '~&' constant
      |
      '&~' constant
      |
      '~|' constant
      |
      '|~' constant
      |
      '^~' constant 
      |
      '~^' constant 
      |
      constant '=:=' constant ':' constant 
      |
      constant '?' constant ':' constant 
      |
      '(' constant ')' 
      |
      constant_concatenation
      ;
    
    constant_concatenation:
      '{' constant_list '}' 
      |
      '{' constant '{' constant '}' '}'
      ;
      
    constant_list:
      constant
      |
      constant_list ',' constant
      ;
    
    /***********************************
     *  string constant
     ***********************************/
      
    string_constant:
      String_literal
      |
      '{' string_list '}' 
      |
      '{' constant '{' string_constant '}' '}'
      |
      constant '?' string_constant ':' string_constant 
      |
      '(' string_constant ')' 
      ;
    
    string_list:
      string_constant
      |
      string_list ',' string_constant
        

  15.2 Number representation

    
    number:
      Real_num
      |
      Real_or_Depth
      |
      Decimal_num
      |
      V_decimal_num
      |
      C_decimal_num
      |
      V_hex_num
      |
      C_hex_num
      |
      V_octal_num
      |
      C_octal_num
      |
      V_bin_num
      |
      C_bin_num
      ;
    
   
  15.3 Expression

    
    expression:
      number
      |
      string_constant
      |
      timed_expression 
      |
      parallel_eval_expression
      |
      varport_expression
      |
      bool_null_expression
      |
      function_call_expression
      |
      state_expression
      |
      concatenation_expression
      |
      list_expression
      |
      expression '-' expression   
      |
      expression '+' expression   
      |
      expression '*' expression   
      |
      expression '/' expression   
      |
      expression '%' expression   
      |
      expression '&' expression   
      |
      expression '|' expression   
      |
      expression '^' expression   
      |
      expression '&~' expression  
      |
      expression '~&' expression  
      |
      expression '|~' expression  
      |
      expression '~|' expression  
      |
      expression '^~' expression  
      |
      expression '~^' expression  
      |
      expression '&&' expression  
      |
      expression '||' expression  
      |
      expression '>>' expression  
      |
      expression '>>>' expression 
      |
      expression '<<' expression  
      |
      expression '<' expression   
      |
      expression '>' expression   
      |
      expression '==' expression  
      |
      expression '<=' expression  
      |
      expression '>=' expression  
      |
      expression '!=' expression  
      |
      expression '===' expression 
      |
      expression '!==' expression 
      |
      expression '=?=' expression 
      |
      expression '!?=' expression 
      |
      expression '==?' expression 
      |
      expression '?==' expression 
      |
      '-' expression
      |
      '~' expression
      |
      '!' expression
      |
      '&' expression
      |
      '|' expression
      |
      '^' expression
      |
      '~&' expression
      |
      '&~' expression
      |
      '~|' expression
      |
      '|~' expression
      |
      '^~' expression 
      |
      '~^' expression 
      |
      expression '=:=' expression ':' expression 
      |
      expression '?' expression ':' expression 
      |
      '(' expression ')'
      ;
    
    
    opt_expression:
      /* empty */
      |
      expression
      ;
    
  
  15.4 Timed expression
  
     
    timed_expression:
      time_range '(' expression_list ')'
      ;
    
    time_range:
      '@' expression opt_clock_definition 
      |
      '@' opt_expression ',' expression opt_clock_definition
      |
      '@@' expression opt_clock_definition 
      |
      '@@' opt_expression ',' expression opt_clock_definition 
      ;
    
        
  15.5 null expression

    
    bool_null_expression:
      'null' 
      |
      'true'
      |
      'false'
      ;
    

  15.6 increment and decrement expression

      
    state_expression:
      varport_expression '++' 
      |
      varport_expression '--' 
      |
      '++' varport_expression 
      |
      '--' varport_expression  
      ;
    

  15.7 concatenation expression

    concatenation_expression:
      '{' expression_list '}' 
      |
      '{' expression '{' expression '}' '}'
      ;
    

  15.8 list expression

    list_expression:
      '\'' '{' expression_list '}' 
      ;
    

  15.9 Variable and port expression

    
    varport_expression:
      a_ver_expression
      |
      varport_expression '.' Identifier opt_depth opt_access_range
      |
      varport_expression '.' 'super'
      ;
      
    a_ver_expression:
      Identifier opt_depth opt_access_range
      |
      'super'
      |
      'this'
      ;
    
      
    opt_depth:
      /* empty */
      |
      Real_or_Depth
      ;
      
    opt_access_range:
      /* empty */
      |
      access_range
      ;
    
    access_range:
      array_access_ranges
      |
      array_access_ranges bit_access_range
      |
      bit_access_range
      ;
      
    bit_access_range:
      '[' expression ':' expression ']'
      ;
      
    array_access_ranges:
      array_access_range
      |
      array_access_ranges array_access_range
      ;
      
    array_access_range:
      '[' expression ']'  
      ; 
     

  15.10 Function call

      
    function_call_expression:
      varport_expression '(' opt_actual_arguments ')'  
      ;
    
    opt_actual_arguments:
      /* empty */
      |
      actual_arguments
      ;
      
    actual_arguments:
      actual_argument 
      |
      actual_arguments ',' actual_argument
      ;
      
    actual_argument:
      expression 
      |
      '*'
      ;
    
    /***********************************
     *  new
     ***********************************/

  
16. Assign 
  
    
    assign_expression:
      left_expression assign_opr expression  
      |
      left_expression '=' new_expression
      |
      list_expression '=' expression
      ;
      
    left_expression:
      varport_expression 
      ;
      
    assign_opr:
      '=' 
      |
      '<=' 
      |
      '+=' 
      |
      '-=' 
      |
      '*=' 
      |
      '/=' 
      |
      '%='  
      |
      '>>=' 
      |
      '>>>=' 
      |
      '<<=' 
      |
      '&=' 
      |
      '|=' 
      |
      '^=' 
      |
      '~&=' 
      |
      '&~=' 
      |
      '~|=' 
      |
      '|~='
      |
      '~^=' 
      |
      '^~=' 
      ;
    

