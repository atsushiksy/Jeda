/* 
   Jeda Liner parse tree & other structure
 */
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

#ifndef INC_LINKER

#define INC_LINKER

#define JDLINK  "jdlink"

typedef enum {
  JD_null_inst,
  JD_if_inst
} JD_vinst_type ;

typedef enum {
  JD_obj_file,
  JD_c_file,
  JD_o_file,
  JD_a_file,
  JD_dl_file,
  JD_unknown_file
} JD_file_type ;


struct link_file_name_s {
  char *name ;
  JD_file_type type ;
  struct link_file_name_s *next ;
} ;

typedef enum {
  VVI_enum_dcl,
  VVI_debug_info,
  VVI_label,
  VVI_inst,
  VVI_dummy_new
} VVI_type ;

typedef enum {
  JD_sub_info = 0,
  JD_debug_info,
  JD_debug_statement_info,
  JD_debug_cwd_info,
  JD_debug_file_info,
  JD_debug_gen_func_frame_info,
  JD_debug_gen_mfunc_frame_info,
  JD_debug_gen_afunc_frame_info,
  JD_debug_gen_advice_frame_info,
  JD_debug_argument_info,
  JD_debug_variable_info,
  JD_debug_enum_info,
  JD_debug_block_info,
  JD_debug_endblock_info,
  JD_debug_member_variable_info,
  JD_debug_aspect_variable_info,
  JD_debug_exit_function_info,
  JD_port_info,
  JD_portset_info,
  JD_connection_info,
  JD_variable_dcl,
  JD_enum_dcl,
  JD_function_info,
  JD_class_info,
  JD_veri_task_info,
  JD_static_var_info,
  JD_stat_init_code,
  JD_init_code,
  JD_class_init_code,
  JD_class_alloc_code,
  JD_mfunc_code,
  JD_func_code,
  JD_afunc_code,
  JD_advice_code,
  JD_port_access_attrib,
  JD_port_depth_attrib,
  JD_port_path_attrib,
  JD_port_clock_attrib,
  JD_string_const,
  JD_bit_const,
  JD_int_const,
  JD_float_const,
  JD_aspect_info,
  JD_advice_info,
  JD_call_pointcut,
  JD_return_pointcut,
  JD_overwrite_pointcut,
  JD_pointcut_name,
  JD_pointcut_string
} JD_vcode_type ;

typedef enum  {
  JD_string_lit,
  JD_real_num,
  JD_int_num,
  JD_bit_num,
  JD_enum_member
} JD_expression_type ;

/* name tables */

typedef enum {
  JD_unknown_name,
  JD_func_name,
  JD_mfunc_name,
  JD_class_name,
  JD_aspect_name,
  JD_advice_name,
  JD_enum_name,
  JD_enum_member_name,
  JD_port_name,
  JD_portset_name,
  JD_variable_name,
  JD_verilog_task_name,
  JD_sys_func_name,
  JD_sys_mfunc_name,
  JD_sys_class_name
} JD_name_type ;


typedef struct link_file_name_s link_file_name ;
typedef struct vinst_info_s vinst_info ;
typedef struct vcode_info_s vcode_info ;
typedef struct expression_node_s expression_node ;

typedef struct JD_name_s JD_name ;
typedef struct JD_label_name_s JD_label_name ;

typedef struct advice_link_s advice_link ;


/******************************************************
 *  Virtual Code Info structure
 ******************************************************/
struct vinst_info_s {
  VVI_type type ;
  char *name ;
  int  inum ;
  JD_name *fname ;
  
  char *filename ;
  int  linenum ;
  
  vcode_info  *vinfo ;
  int  itype ;
  int  isubtype ;
  int  index ;
  int  offset ;
  int  strength ;
  int  ub ;
  int  lb ;
  vcode_info  *data_type ;
  vcode_info  *array_range ;
  vcode_info  *opt_flags ;
  vcode_info  *sd_type ;
  expression_node  *const_data ;
  int  is_var ;
  int  is_static ;
  int  is_leaf ;
  JD_name_type func_type ;
  char *label ;
  vcode_info  *bit_type ;
  char *parent ;
  char *class ;
  char *func ;
  int edge ;
  char *portset ;
  char *port ;
  vcode_info *array_access ;
  vinst_info *next ;
  JD_name *code_name ;
  advice_link *advice ;
  int overwrite ;
  int ignore ;
  int no_flush ;
} ;

struct vcode_info_s {
  JD_vcode_type type ;
  char *filename ;
  vcode_info *next ;
  int index ;
  int srclinenum ;
  char *srcfilename ;
  JD_name *fname ;
  
  int local_name_flag ;
  JD_name **local_name_top ;
  int local_name_table_index ;

  JD_name *code_name ;
  
  union {
    struct {
      char *name ;
      vcode_info *next ;
    } pointcut_info ;
    
    struct {
      char *string ;
      char *filename ;
      int linenum ;
      int statementnum ;
      int scope ;
    } debug_info ;
    
    struct {
      char *dir ;
      char *file ;
    } debug_cwd_info ;

    struct {
      char *dir ;
      char *file ;
    } debug_file_info ;
    
    struct {
      char *func ;
    } debug_gen_func_frame_info ;

    struct {
      char *mfunc ;
    } debug_gen_mfunc_frame_info ;
    
    struct {
      char *name ;
      int level ;
      char *type ;
      int ub ;
      int lb ;
    } debug_var_argument_info ;

    struct {
      char *name ;
      int num_member ;
      char **member ;
    } debug_enum_info ;
    
    struct {
      int level ;
    } debug_block_info ;

    struct {
      int level ;
    } debug_endblock_info ;

    struct {
      char *class ;
      char *name ;
      int index ;
      int level ;
      int linenum ;
      int statementnum ;
      JD_name *static_name ;
    } debug_member_variable_info ;
    
    /* port */
    struct {
      int direction ;
      int ub ;
      int lb ;
      char *name ;
      vcode_info *attribs ;
      vcode_info *portset ;
    } port ;
    
    struct {
      int drive_sample ;
      int edge ;
      int skew ;
    } port_access_attrib ;
    
    int int_value ;
    
    char *string ;
    
    struct {
      char *pset ;
      char *port ;
    } port_clock_attrib ;
    
    /* portset */
    struct {
      char *name ;
      vcode_info *port_infos ;
      vcode_info *attribs ;
    } portset ;
    
    /* connection info */
    struct {
      char *portset ;
      char *port ;
      char *path ;
    } connection_info ;
    
    /* variable dcl */
    struct {
      int index ;
      vcode_info *data_type ;
      char *name ;
      vcode_info *array_range ;
      vcode_info *var_attrib ;
      vcode_info *class ;
    } variable_dcl ;
    
    struct {
      int type ;
      int ub ;
      int lb ;
      char *name ;
      vcode_info *user_dtype ;
    } data_type ;
    
    struct {
      int type ;
      int size ;
      vcode_info *range_list ;
    } array_range ;
    
    struct {
      int is_static ;
      int is_clone ;
      int is_extern ;
      int is_local ;
      int is_global ;
      int is_protected ;
      int is_final ;
      int is_abstract ;
      int is_export ;
      int is_leaf ;
      int is_overwrite ;
    } attrib ; 
    
    struct {
      char *name ;
      vcode_info *members ;
    } enum_dcl ;
    
    struct {
      vcode_info *func_options ;
      vcode_info *data_type ;
      char *name ;
      int sys_flag ;
      vcode_info *class ;
      vcode_info *arguments ;
      char *path ;
      vcode_info *pointcut ;
    } function_info ;
    
    struct {
      int var ;
      vcode_info *data_type ;
      char *name ;
      vcode_info *array_range ;
    } argument ;
    
    struct {
      char *name ;
      int sys_flag ;
      char *parent ;
      vcode_info *attribs ;
      vcode_info *members ;
    } class ;
    
    struct {
      char *name ;
      vcode_info *arguments ;
      char *path ;
    } veri_task ;
    
    struct {
      char *class ;
      char *func ;
      char *name ;
      vcode_info *data_type ;
      vcode_info *array_range ;
      vcode_info *attribs ;
    } static_var_dcl ;
    
    struct {
      char *name ;
      vinst_info *code ;
    } init_code ;
    
    struct {
      char *class ;
      char *func ;
      vinst_info *code ;
    } named_code ;
    
    struct {
      int a ;
      int b ;
      int c ;
    } sd_type ;
    
    struct {
      int type ;
      int ub ;
      int lb ;
    } bit_type ;
        
    struct {
      int size ;
      int *a ;
      int *b ;
    } bit_const ;
    
    double real_const ;

  } is ;

} ; /* end of vcode_info_s */


/* expression node for constant */

struct expression_node_s
{
  JD_expression_type type ;
  char *filename ;  /* debug info */
  int  linenum ;
  char *string ;
  union {
    expression_node *expression ;
    char *string ;
    struct {
      int size ;
      int *a ;
      int *b ;
    } bit_const ;
    int int_value ;
    double real_value ;
    struct {
      char *type ;
      char *member ;
      int index ;
    } enum_const ;
  } is ;
}  ;

struct JD_name_s
{
  JD_name_type type ;
  int hash ;
  char *name ;
  vcode_info *info ;
  JD_name **child ;
  JD_name *next ;
  int defined ;
  int abstract ;
  int index ;  /* used to locate global/static variable from name */
  JD_label_name *label_top ;
  char *filename ;
  JD_name *link ; /* used to link name by order */
  JD_name *class_name ; /* point to the class name of an instance */
  JD_name *static_name ;  /* point to the actual static name */
} ;

struct JD_label_name_s {
  char *label ;
  int index ;
  JD_label_name *next ;
} ;

struct advice_link_s {
  vcode_info *advice ;
  int match_type ;
  advice_link *next ;
} ;

void scan_v_decimal(  char *number, expression_node *exp ) ;
void scan_c_decimal( char *number, expression_node *exp ) ;
void scan_v_hex(  char *number, expression_node *exp ) ;
void scan_c_hex(  char *number, expression_node *exp ) ;
void scan_v_octal(  char *number, expression_node *exp ) ;
void scan_c_octal(  char *number, expression_node *exp ) ;
void scan_v_binary(  char *number, expression_node *exp ) ;
void scan_c_binary(  char *number, expression_node *exp ) ;

int check_port_attribs( vcode_info *a, vcode_info *b ) ;
int comp_data_type( vcode_info *a, vcode_info *b ) ;
int comp_array_range( vcode_info *a, vcode_info *b ) ;
int comp_attrib( vcode_info *a, vcode_info *b ) ;
int comp_info( vcode_info *a, vcode_info *b ) ;
void comp_portset_info( vcode_info *a, JD_name *nmb  ) ;
void comp_enum_info( vcode_info *a, JD_name *nmb  ) ;
int check_enum_info( vcode_info *a, vcode_info *b  ) ;
void comp_func_info( char *class, vcode_info *a, vcode_info *b  ) ;
void comp_veri_task_info( vcode_info *a, vcode_info *b  ) ;
char *get_info_name( vcode_info *info ) ;
JD_name_type get_info_type( vcode_info *info ) ;
void comp_member( char *class, vcode_info *a, vcode_info *b ) ;
void append_member( vcode_info *a, JD_name *bnm ) ;
int comp_class( vcode_info *a, JD_name *bnm ) ;


JD_name *find_name_in_scope( char *name, JD_name **scope ) ;
JD_name *find_name( char *name ) ;
JD_name *find_local_name( char *name ) ;
JD_name *find_global_name( char *name ) ;
JD_name *find_static_name( char *, char *,char * ) ;
JD_name *find_local_static_name( char *, char *,char * ) ;
JD_name *add_name_to_scope( char *name, JD_name ***scopep ) ;
JD_name *add_global_name( char *name ) ;
JD_name *add_local_name( char *name ) ;
JD_name *add_static_name( char *, char *,char * ) ;
JD_name *add_local_static_name( char *, char *,char * ) ;
JD_name *add_child_name( char *name, JD_name *parent ) ;
JD_name *find_verilog_name( char *name ) ;
JD_name *add_verilog_name( char *name ) ;
void check_name_definition( JD_name **scope ) ;
void check_global_name() ;
void check_local_name() ;
#endif
