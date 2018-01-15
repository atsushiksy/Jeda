/***************************************************************************
 *
 *  parse_tree.h: Various data structure declarations for parse tree
 *
 *  Author: Atsushi Kasuya 
 *
 *
 ***************************************************************************/
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
#ifndef INC_PARSE_TREE

#define INC_PARSE_TREE

typedef struct expression_node_s expression_node ;

typedef struct statement_node_s statement_node ;

typedef struct block_node_s block_node ;

typedef struct named_node_s named_node ;

typedef struct var_declaration_s var_declaration ;

typedef struct connection_node_s connection_node ;

/* typedef struct name_scope_s name_scope ; */

typedef struct label_data_s label_data ;

typedef struct file_data_s file_data ;

#define NMATTR_STATIC     0x00000001 
#define NMATTR_EXTERN     0x00000002 
#define NMATTR_LOCAL      0x00000004 
#define NMATTR_PROTECTED  0x00000008 
#define NMATTR_VIRTUAL    0x00000010
#define NMATTR_FINAL      0x00000020 

/* expression node super structure */
struct expression_node_s
{
  JD_expression_type type ;
  JD_exp_data_type   data_type ;
  named_node *user_data_type ;
  char *string ;
  struct expression_node_s *next ;   /* link for argument */
  char *filename ;  /* debug info */
  int  linenum ;
  union {
    expression_node *expression ;
    char *string ;
    unsigned long constant ;
    struct {
      int size ;
      int *a ;
      int *b ;
    } bit_const ;
    int int_value ;
    double real_value ;
    struct {
      expression_node *delay ;
      expression_node *window ;
      expression_node *clock ;
      expression_node *exp_list ;
    } time ;
    struct {
      expression_node *this ;
    } unary ;
    struct {
      expression_node *left ;
      expression_node *right ;
    } binary ;
    struct {
      expression_node *first ;
      expression_node *second ;
      expression_node *third ;
    } trinary ;
    struct {
      expression_node *exp_list ;
    } parallel ;
    struct {
      expression_node *this ;
      expression_node *next ;
    } exp_list ;
    struct {
      named_node *name ;
      expression_node *pre ;
      expression_node *next ;
      expression_node *depth ;
      expression_node *range ;
      expression_node *bit_range ;
      named_node *class ;
      int member_offset ;
      int offset ;
      expression_node *args ;
      int var_arg ;
    } var ;
    struct {
      named_node *name ;
      expression_node *pre ;
      expression_node *args ;
    } var_ ;
    struct {
      named_node *name ;
    } portset ;
    /* use 'var' instead
    struct {
      named_node *name ;
      expression_node *pre ;
      expression_node *args ;
    } func_call ;
    */
    struct {
      expression_node *args ;
      expression_node *obj ;
    } new ;
    struct {
      statement_node *ock ;
    } new_bl ;
    struct {
      expression_node *this ;
      expression_node *next ;
    } concat ;
    named_node *port ;
    struct {
      JD_port_attr attr ;
      JD_edge_type edge ;
      named_node *port_name ;
      expression_node *skew ;   
      expression_node *depth ;   
      char *node ;
      expression_node *next ;
    } port_attr ;
    struct {
      expression_node *ub ;
      expression_node *lb ;
      int int_ub ;
      int int_lb ;
      int i_endian ;  /* = 1 if ub < lb , intel/ibm endian */
      int num_index ;
      int bit_index ;
    } range ;
    struct {
      char *label ;
      int  continue_num ;     /* continue label number */
      int  break_num ;        /* break label number */
      expression_node *cycle ;
      expression_node *clock ;
      JD_edge_type  edge ;
    } smt_prefix ;
    struct {
      expression_node *lvalue ;
      JD_assign_operator ops ;
      expression_node *expression ;
      JD_strength strength ;
    } assign ;
    struct {
      int size ;
      int width ;
      int dst_width ;
      int num_element ;
      int limit ;
      expression_node *range ;
      expression_node *cond ;
      char *name ;
      int index ;
      expression_node *nxt ;
    } rnd ;
    struct {
      expression_node *ub ;
      expression_node *lb ;
      int num_element ;
      int width ;
      int weight ;
      int weight_flag ;
      int ignore ;
      expression_node *nxt ;
    } rnd_range ;
    struct {
      int flag ;
      expression_node *exp ;
      expression_node *nxt ;
    } rnd_cond ;
  } is ;
}  ;

/* statement node super structure */

struct statement_node_s 
{
  JD_statement_type type ;
  expression_node *prefix ;
  statement_node *next ;   /* next statement */
  char *filename ;  /* debug info */
  int  linenum ;
  int  output_done ; /* flag to tell this statement is already written */
  union {
    struct {
      statement_node *statement ;
    } init ;
    struct {
      expression_node *cond ;
      statement_node *statement ;
      statement_node *elsif_statements ;
      statement_node *else_statement ;
    } if_ ;
    struct {
      expression_node *cond ;
      statement_node *statement ;
      statement_node *next ;
    } elsif ;
    struct {
      statement_node *statement ;
    } else_ ;
    struct {
      expression_node *cond ;
      statement_node *statement ;
    } while_ ;
    struct {
      expression_node *initial ;
      expression_node *cond ;
      expression_node *last ;
      statement_node *statement ;
    } for_ ;
    struct {
      expression_node *number ;
      statement_node *statement ;
    } repeat ;
    struct {
      statement_node *statement ;
      expression_node *cond ;
    } dowhile ;
    struct {
      statement_node *statement ;
    } forever ;
    struct {
      expression_node *cond ;
      statement_node *items ;
    } case_statement ;
    struct {
      expression_node *cond ;
      statement_node *statement ;
      int weight ;
    } case_item ;
    struct {
      char *label ;
      int label_num ;
      int level ;
      int repeat ;
    } break_continue ;
    struct {
      JD_join_type join_flag ; 
      statement_node *statement_list ;
    } fork_join ;
    struct {
      statement_node *this ;
      statement_node *next ;
    } list ;
    struct {
      expression_node *expression ;
    } exp ;
    struct {
      expression_node *expression ;
    } funccall ;
    struct {
      expression_node *expression ;
      named_node *dtype ; 
      named_node *ptype ;
    } ret ;
    struct {
      expression_node *expression ;
    } assign ;
    struct {
      int level ;
      statement_node *vars ;
      expression_node *super_new ;
      statement_node *statements ;
    } block ;
    struct {
      named_node *port ;
      expression_node *path ;
    } portconnect ;
    struct {
      var_declaration *declare ;
    } variable ;
    struct {
      named_node *edf ;
    } enum_def ;
    struct {
      named_node *data_type ;
      statement_node *child ;
    } formal_arg ;
    struct {
      char *filename ;
    } breakpoint ;
  } is ;

  /* for MT and other optimization, not currently used */
  int mt_safe ;
  statement_node *bckref ;   /* back reference for mt_safe */
  statement_node *fwdref ;   /* forward ref for mt_safe */
} ;



/* block node super structure */

struct block_node_s 
{
  
  JD_block_type type ;
  named_node *name ;
  named_node *scope ;
  block_node *parent ;
  block_node *next ;
  int level ;    /* this var holds the depth of stack frame */
  char *filename ;  /* debug info */
  int  linenum ;
  union {
    expression_node *port ; 
    struct {
      expression_node *port_list ; 
    } portset ;
    struct {
      connection_node *connect_list ;
    } connection ;
    struct {
      statement_node *dcl ;
    } local_global_var ;
    struct {
      statement_node *statement ; 
    } initial ;
    struct {
      statement_node *ck ; 
    } blo ;   /* blo.ck */
    struct {
      var_declaration *edf ;
    } enum_defs ;
    struct {
      statement_node *args ;
      statement_node *block ;
      char *path ;           /* for verilog task path */
      statement_node *pointcut ;   /* for advice */      
    } func ;
    struct {
      statement_node *edf ;
      statement_node *initial ; 
      statement_node *members ;
      block_node *member_funcs ;
      int is_abstract ;
      int is_extern ;
      int is_local ;
      int is_final ;
    } class ;
    struct {
      block_node *funcs ;
    } user_veri ;      
  } is ;
  
} ;

/* named_node also represents the system data type 
     void, int, long, float, double, bit [ <range> ], boolean
   They are recognized by name_type. In such case, data_type
   does not mean anything.
   
   named_nodes form the scope tree while parsing.
   the tree is formed both on pass0 and pass1,
   to be able to handle user-defined data type (i.e. class and enum).
   
*/
struct named_node_s {  
  char *name ;
  JD_data_name_type  type ;
  JD_sub_type sub_type ;
  int array_size ;             
    /* Note: array_size is not used since we have multi-dimention */ 
  named_node  *data_type ;
  named_node  *next ;
  char *filename ;  /* debug info */
  int  linenum ;
  /* pointers to conform scope structure */
  named_node  *parent_scope ;
  named_node  *next_in_scope ;
  named_node  *child_scope ;
  named_node  *next_brother ;
  /* named_node stack */
  named_node  *prev_scope ;
  /* point to the scope node of its own */
  named_node  *scope ;
  int scope_id ;
  int index ;  /* for enum member index */
  /* flags for the name */
  int is_abstract ;
  int is_unabstracted ;
  int is_local ;
  int is_extern ;
  int is_protected ;
  int is_final ;
  int is_overwrite ;
  int is_static ;
  int is_clone ;
  int is_export ;
  int is_clock ;
  int local_var ;
  int is_member ;
  int is_leaf ;
  /* bit range info shared by bit/port */
  int  ub ;
  int  lb ;
  int  i_endian ;
  union {
    struct {
      named_node *portset ;
      JD_port_type type ;
      named_node *clock ;
      JD_edge_type sample_edge ;
      int inputskew ;
      int depth ;
      JD_edge_type drive_edge ;
      int outputskew ;
      char *node_path ;
      named_node *next ;
    } port ;
    struct {
      statement_node *args ;
      block_node  *func_block ;
      block_node  *class_block ;
      int index ;
      statement_node *pointcut ;
    } func ;
    block_node *block ;
    named_node  *enum_body ;
    named_node  *enum_members ;
    struct {
      expression_node *range ;
      expression_node *init ;
      int is_var ;
      int index ;
      int local ;
    } var ;
    struct {
      named_node *parent ;
      block_node *block ;
    } class_name ;
    expression_node *label ;
    /* expression_node *range ;  old */
  } info ;
} ;

struct connection_node_s {
  named_node *port ;
  char *node_string ;
  connection_node *next ;
  char *filename ;  /* debug info */
  int  linenum ;
} ;

/* old
struct name_scope_s {
  name_scope  *parent ;
  named_node  *name_list ;
  named_node  *last_node ;
} ;
*/

struct var_declaration_s {
  named_node *data_type ;
  named_node *var_list ;
  var_declaration *next ;
  int is_arg ;
  int is_var ;
  int is_static ;
  int is_local ;
  int is_global ;
  int is_clone ;
  int is_extern ;
  int is_protexted ;
  int is_final ;
  char *filename ;  /* debug info */
  int  linenum ;
} ;

struct label_data_s {
  char *label ;
  int break_num ;
  int continue_num ;
  int level ;
  int repeat ;
  int var_level ;
  int this_level ;
  label_data *prev ;
  char *filename ;  /* debug info */
  int  linenum ;
} ;

struct file_data_s {
  char *filename ;  /* file info */
  file_data *next ;
} ;

void set_mfunc_def_flags( named_node *mfunc ) ;
void set_new_def_flags( named_node *mfunc ) ;
void set_var_def_flags( named_node *variable, int in_class, int in_aspect ) ;

void scan_port_attrib (
  named_node *ret, expression_node *list_attrib,
  char *filename, int linenum
) ;

void formal_arg_check ( 
  statement_node *arg1,
  statement_node *arg2,
  char *filename,
  int  linenum
) ;

void check_member_func( 
  named_node *data_type,
  named_node *name,
  statement_node *arg_list
) ;

void set_func_def_flags( named_node *func ) ;

void push_label( 
  char *label, int continue_num, int break_num, int level, int repeat
) ;

void pop_label() ;

label_data *find_label( char *label ) ;

void *check_alloc( int size ) ;

void scan_v_decimal(  char *number, expression_node *exp ) ;

void scan_c_decimal( char *number, expression_node *exp ) ;

void scan_v_hex(  char *number, expression_node *exp );

void scan_c_hex(  char *number, expression_node *exp ) ;

void scan_v_octal(  char *number, expression_node *exp ) ;

void scan_c_octal(  char *number, expression_node *exp ) ;

void scan_v_binary(  char *number, expression_node *exp ) ;

void scan_c_binary(  char *number, expression_node *exp );

named_node *find_member( 
  expression_node *, char *, int *, named_node **
) ;

void set_var_type( expression_node *, expression_node *, expression_node * ) ;

void actual_arg_check ( 
  named_node *,
  expression_node *,
  expression_node *,
  char *,
  int 
) ;

int is_array_exp_type( JD_exp_data_type ) ;

#endif /* INC_PARSE_TREE */
