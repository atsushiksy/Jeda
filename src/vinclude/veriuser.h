#ifndef VERIUSER_H
#define VERIUSER_H

/*---------------------------------------------------------------------------*/
/*------------------------------- definitions -------------------------------*/
/*---------------------------------------------------------------------------*/

/*----------------------------- general defines -----------------------------*/

#define true                   1
#define TRUE                   1
#define false                  0
#define FALSE                  0
#define bool                   int

/*---------------------- defines for error interception ---------------------*/

#define ERR_MESSAGE            1
#define ERR_WARNING            2
#define ERR_ERROR              3
#define ERR_INTERNAL           4
#define ERR_SYSTEM             5

/*-------------- values for reason parameter to misctf routines -------------*/

#define reason_checktf         1
#define REASON_CHECKTF         reason_checktf
#define reason_sizetf          2
#define REASON_SIZETF          reason_sizetf
#define reason_calltf          3
#define REASON_CALLTF          reason_calltf
#define reason_save            4
#define REASON_SAVE            reason_save
#define reason_restart         5
#define REASON_RESTART         reason_restart
#define reason_disable         6
#define REASON_DISABLE         reason_disable
#define reason_paramvc         7
#define REASON_PARAMVC         reason_paramvc
#define reason_synch           8
#define REASON_SYNCH           reason_synch
#define reason_finish          9
#define REASON_FINISH          reason_finish
#define reason_reactivate     10
#define REASON_REACTIVATE      reason_reactivate
#define reason_rosynch        11
#define REASON_ROSYNCH         reason_rosynch
#define reason_paramdrc       15
#define REASON_PARAMDRC        reason_paramdrc
#define reason_endofcompile   16
#define REASON_ENDOFCOMPILE    reason_endofcompile
#define reason_scope          17
#define REASON_SCOPE          reason_scope
#define reason_interactive    18
#define REASON_INTERACTIVE     reason_interactive
#define reason_reset          19
#define REASON_RESET           reason_reset
#define reason_endofreset     20
#define REASON_ENDOFRESET      reason_endofreset
#define reason_force          21
#define REASON_FORCE           reason_force
#define reason_release        22
#define REASON_RELEASE         reason_release
#define reason_startofsave    27
#define reason_startofrestart 28
#define REASON_MAX            28

/*-- types used by tf_typep() and expr_type field in tf_exprinfo structure --*/
#define tf_nullparam           0
#define TF_NULLPARAM           tf_nullparam
#define tf_string              1
#define TF_STRING              tf_string
#define tf_specialparam        2
#define TF_SPECIALPARAM        tf_specialparam
#define tf_readonly           10
#define TF_READONLY            tf_readonly
#define tf_readwrite          11
#define TF_READWRITE           tf_readwrite
#define tf_rwbitselect        12
#define TF_RWBITSELECT         tf_rwbitselect
#define tf_rwpartselect       13
#define TF_RWPARTSELECT        tf_rwpartselect
#define tf_rwmemselect        14
#define TF_RWMEMSELECT         tf_rwmemselect
#define tf_readonlyreal       15
#define TF_READONLYREAL        tf_readonlyreal
#define tf_readwritereal      16
#define TF_READWRITEREAL       tf_readwritereal

/*---------- types used by node_type field in tf_nodeinfo structure ---------*/
#define tf_null_node         100
#define TF_NULL_NODE           tf_null_node
#define tf_reg_node          101
#define TF_REG_NODE            tf_reg_node
#define tf_integer_node      102
#define TF_INTEGER_NODE        tf_integer_node
#define tf_time_node         103
#define TF_TIME_NODE           tf_time_node
#define tf_netvector_node    104
#define TF_NETVECTOR_NODE      tf_netvector_node
#define tf_netscalar_node    105
#define TF_NETSCALAR_NODE      tf_netscalar_node
#define tf_memory_node       106
#define TF_MEMORY_NODE         tf_memory_node
#define tf_real_node         107
#define TF_REAL_NODE           tf_real_node

/*---------------------------------------------------------------------------*/
/*-------------------------- structure definitions --------------------------*/
/*---------------------------------------------------------------------------*/

/*----- structure used with tf_exprinfo() to get expression information -----*/
typedef struct t_tfexprinfo
{
  short expr_type;
  short padding;
  struct t_vecval *expr_value_p;
  double real_value;
  char *expr_string;
  int expr_ngroups;
  int expr_vec_size;
  int expr_sign;
  int expr_lhs_select; 
  int expr_rhs_select;
} s_tfexprinfo, *p_tfexprinfo;

/*------- structure for use with tf_nodeinfo() to get node information ------*/
typedef struct t_tfnodeinfo
{
  short node_type;
  short padding;
  union
    {
      struct t_vecval *vecval_p;
      struct t_strengthval *strengthval_p;
      char *memoryval_p;
      double *real_val_p;
    } node_value;
  char *node_symbol;
  int node_ngroups;
  int node_vec_size;
  int node_sign;
  int node_ms_index;
  int node_ls_index;
  int node_mem_size;
  int node_lhs_element;
  int node_rhs_element;
  int *node_handle;
} s_tfnodeinfo, *p_tfnodeinfo;

/*--------------------- data structure of vector values ---------------------*/
typedef struct t_vecval
{
  int avalbits;
  int bvalbits;
} s_vecval, *p_vecval;

/*--------------- data structure of scalar net strength values --------------*/
typedef struct t_strengthval
{
    int strength0;
    int strength1;
} s_strengthval, *p_strengthval;

/* Sized variables */
#ifndef PLI_TYPES
#define PLI_TYPES
typedef int             PLI_INT32;
typedef unsigned int    PLI_UINT32;
typedef short           PLI_INT16;
typedef unsigned short  PLI_UINT16;
typedef char            PLI_BYTE8;
typedef unsigned char   PLI_UBYTE8;
#endif

typedef PLI_INT32 (*p_tffn)();

typedef struct t_tfcell
{
    PLI_INT16 type;     /* USERTASK, USERFUNCTION, or USERREALFUNCTION  */
    PLI_INT16 data;     /* passed as data argument of callback function */
	p_tffn    checktf;  /* argument checking callback function          */
	p_tffn    sizetf;   /* function return size callback function       */
	p_tffn    calltf;   /* task or function call callback function      */
	p_tffn    misctf;   /* miscellaneous reason callback function       */
    char *    tfname;   /* name of system task or function              */

	/* The following fields are ignored by ModelSim Verilog */
    PLI_INT32 forwref;
    char *    tfveritool;
    char *    tferrmessage;
    PLI_INT32 hash;
    struct t_tfcell *left_p;
    struct t_tfcell *right_p;
    char *    namecell_p;
    PLI_INT32 warning_printed;
} s_tfcell, *p_tfcell;

/* values for 'type' field in tfcell structure */
#define usertask 1
#define USERTASK 1
#define userfunction 2
#define USERFUNCTION 2
#define userrealfunction 3
#define USERREALFUNCTION 3


/*---------------------------------------------------------------------------*/
/*--------------------------- routine definitions ---------------------------*/
/*---------------------------------------------------------------------------*/

#if WIN32
#define DLLISPEC __declspec(dllimport)
#define DLLESPEC __declspec(dllexport)
#else
#define DLLISPEC /* nothing */
#define DLLESPEC /* nothing */
#endif

#if defined(__STDC__) || defined(__cplusplus) || defined(MSC)

#ifndef PROTO_PARAMS
#define PROTO_PARAMS(params) params
#define DEFINED_PROTO_PARAMS
#endif
#ifndef EXTERN
#define EXTERN
#define DEFINED_EXTERN
#endif

#else

#ifndef PROTO_PARAMS
#define PROTO_PARAMS(params) (/* nothing */)
#define DEFINED_PROTO_PARAMS
#endif
#ifndef EXTERN
#define EXTERN extern
#define DEFINED_EXTERN
#endif

#endif /* __STDC__ */

#define XXTERN EXTERN DLLISPEC
#define EETERN EXTERN DLLESPEC

XXTERN void         io_mcdprintf PROTO_PARAMS((int mcd, char *format, ...));
XXTERN void         io_printf PROTO_PARAMS((char *format, ...));
XXTERN char        *mc_scan_plusargs PROTO_PARAMS((char *plusarg));
XXTERN int          tf_add_long PROTO_PARAMS((int *aof_lowtime1, int *aof_hightime1, int lowtime2, int hightime2));
XXTERN int          tf_asynchoff PROTO_PARAMS((void));
XXTERN int          tf_asynchon PROTO_PARAMS((void));
XXTERN int          tf_clearalldelays PROTO_PARAMS((void));
XXTERN int          tf_compare_long PROTO_PARAMS((unsigned int low1, unsigned int high1, unsigned int low2, unsigned int high2));
XXTERN int          tf_copypvc_flag PROTO_PARAMS((int nparam));
XXTERN void         tf_divide_long PROTO_PARAMS((int *aof_low1, int *aof_high1, int low2, int high2));
XXTERN int          tf_dofinish PROTO_PARAMS((void));
XXTERN int          tf_dostop PROTO_PARAMS((void));
XXTERN int          tf_error PROTO_PARAMS((char *fmt, ...));
XXTERN int          tf_evaluatep PROTO_PARAMS((int pnum));
XXTERN p_tfexprinfo tf_exprinfo PROTO_PARAMS((int pnum, p_tfexprinfo pinfo));
XXTERN char        *tf_getcstringp PROTO_PARAMS((int nparam));
XXTERN char        *tf_getinstance PROTO_PARAMS((void));
XXTERN int          tf_getlongp PROTO_PARAMS((int *aof_highvalue, int pnum));
XXTERN int          tf_getlongtime PROTO_PARAMS((int *aof_hightime));
XXTERN int          tf_getnextlongtime PROTO_PARAMS((int *aof_lowtime, int *aof_hightime));
XXTERN int          tf_getp PROTO_PARAMS((int pnum));
XXTERN int          tf_getpchange PROTO_PARAMS((int nparam));
XXTERN double       tf_getrealp PROTO_PARAMS((int pnum));
XXTERN double       tf_getrealtime PROTO_PARAMS((void));
XXTERN char        *tf_gettflist PROTO_PARAMS((void));
XXTERN int          tf_gettime PROTO_PARAMS((void));
XXTERN int          tf_gettimeprecision PROTO_PARAMS((void));
XXTERN int          tf_gettimeunit PROTO_PARAMS((void));
XXTERN char        *tf_getworkarea PROTO_PARAMS((void));
XXTERN int          tf_iasynchoff PROTO_PARAMS((char *inst));
XXTERN int          tf_iasynchon PROTO_PARAMS((char *inst));
XXTERN int          tf_iclearalldelays PROTO_PARAMS((char *inst));
XXTERN int          tf_icopypvc_flag PROTO_PARAMS((int nparam, char *inst));
XXTERN int          tf_ievaluatep PROTO_PARAMS((int pnum, char *inst));
XXTERN p_tfexprinfo tf_iexprinfo PROTO_PARAMS((int pnum, p_tfexprinfo pinfo, char *inst));
XXTERN char        *tf_igetcstringp PROTO_PARAMS((int nparam, char *inst));
XXTERN int          tf_igetlongp PROTO_PARAMS((int *aof_highvalue, int pnum, char *inst));
XXTERN int          tf_igetlongtime PROTO_PARAMS((int *aof_hightime, char *inst));
XXTERN int          tf_igetp PROTO_PARAMS((int pnum, char *inst));
XXTERN int          tf_igetpchange PROTO_PARAMS((int nparam, char *inst));
XXTERN double       tf_igetrealp PROTO_PARAMS((int pnum, char *inst));
XXTERN double       tf_igetrealtime PROTO_PARAMS((char *inst));
XXTERN int          tf_igettime PROTO_PARAMS((char *inst));
XXTERN int          tf_igettimeprecision PROTO_PARAMS((char *inst));
XXTERN int          tf_igettimeunit PROTO_PARAMS((char *inst));
XXTERN char        *tf_igetworkarea PROTO_PARAMS((char *inst));
XXTERN char        *tf_imipname PROTO_PARAMS((char *cell));
XXTERN int          tf_imovepvc_flag PROTO_PARAMS((int nparam, char *inst));
XXTERN p_tfnodeinfo tf_inodeinfo PROTO_PARAMS((int pnum, p_tfnodeinfo pinfo, char *inst));
XXTERN int          tf_inump PROTO_PARAMS((char *inst));
XXTERN int          tf_ipropagatep PROTO_PARAMS((int pnum, char *inst));
XXTERN int          tf_iputlongp PROTO_PARAMS((int pnum, int lowvalue, int highvalue, char *inst));
XXTERN int          tf_iputp PROTO_PARAMS((int pnum, int value, char *inst));
XXTERN int          tf_iputrealp PROTO_PARAMS((int pnum, double value, char *inst));
XXTERN int          tf_irosynchronize PROTO_PARAMS((char *inst));
XXTERN int          tf_isetdelay PROTO_PARAMS((int delay, char *inst));
XXTERN int          tf_isetlongdelay PROTO_PARAMS((int lowdelay, int highdelay, char *inst));
XXTERN int          tf_isetrealdelay PROTO_PARAMS((double realdelay, char *inst));
XXTERN int          tf_isetworkarea PROTO_PARAMS((char *workarea, char *inst));
XXTERN int          tf_isizep PROTO_PARAMS((int pnum, char *inst));
XXTERN char        *tf_ispname PROTO_PARAMS((char *cell));
XXTERN int          tf_istrdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, int delay, int delaytype, char *inst));
XXTERN char        *tf_istrgetp PROTO_PARAMS((int pnum, int format_char, char *inst));
XXTERN int          tf_istrlongdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, int lowdelay, int highdelay, int delaytype, char *inst));
XXTERN int          tf_istrrealdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, double realdelay, int delaytype, char *inst));
XXTERN int          tf_isynchronize PROTO_PARAMS((char *inst));
XXTERN int          tf_itestpvc_flag PROTO_PARAMS((int nparam, char *inst));
XXTERN int          tf_itypep PROTO_PARAMS((int pnum, char *inst));
XXTERN void         tf_long_to_real PROTO_PARAMS((int int_lo, int int_hi, double *aof_real));
XXTERN char        *tf_longtime_tostr PROTO_PARAMS((int lowtime, int hightime));
XXTERN int          tf_message PROTO_PARAMS((int level, char *facility, char *messno, char *message, ...));
XXTERN char        *tf_mipname PROTO_PARAMS((void));
XXTERN int          tf_movepvc_flag PROTO_PARAMS((int nparam));
XXTERN void         tf_multiply_long PROTO_PARAMS((int *aof_low1, int *aof_high1, int low2, int high2));
XXTERN p_tfnodeinfo tf_nodeinfo PROTO_PARAMS((int pnum, p_tfnodeinfo pinfo));
XXTERN int          tf_nump PROTO_PARAMS((void));
XXTERN int          tf_propagatep PROTO_PARAMS((int pnum));
XXTERN int          tf_putlongp PROTO_PARAMS((int pnum, int lowvalue, int highvalue));
XXTERN int          tf_putp PROTO_PARAMS((int pnum, int value));
XXTERN int          tf_putrealp PROTO_PARAMS((int pnum, double value));
XXTERN int          tf_read_restart PROTO_PARAMS((char *blockptr, int blocklen));
XXTERN void         tf_real_to_long PROTO_PARAMS((double real, int *aof_int_lo, int *aof_int_hi));
XXTERN int          tf_rosynchronize PROTO_PARAMS((void));
XXTERN void         tf_scale_longdelay PROTO_PARAMS((char *cell, int delay_lo, int delay_hi, int *aof_delay_lo, int *aof_delay_hi));
XXTERN void         tf_scale_realdelay PROTO_PARAMS((char *cell, double realdelay, double *aof_realdelay));
XXTERN int          tf_setdelay PROTO_PARAMS((int delay));
XXTERN int          tf_setlongdelay PROTO_PARAMS((int lowdelay, int highdelay));
XXTERN int          tf_setrealdelay PROTO_PARAMS((double realdelay));
XXTERN int          tf_setworkarea PROTO_PARAMS((char *workarea));
XXTERN int          tf_sizep PROTO_PARAMS((int pnum));
XXTERN char        *tf_spname PROTO_PARAMS((void));
XXTERN int          tf_strdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, int delay, int delaytype));
XXTERN char        *tf_strgetp PROTO_PARAMS((int pnum, int format_char));
XXTERN char        *tf_strgettime PROTO_PARAMS((void));
XXTERN int          tf_strlongdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, int lowdelay, int highdelay, int delaytype));
XXTERN int          tf_strrealdelputp PROTO_PARAMS((int nparam, int bitlength, int format_char, char *value_p, double realdelay, int delaytype));
XXTERN int          tf_subtract_long PROTO_PARAMS((int *aof_lowtime1, int *aof_hightime1, int lowtime2, int hightime2));
XXTERN int          tf_synchronize PROTO_PARAMS((void));
XXTERN int          tf_testpvc_flag PROTO_PARAMS((int nparam));
XXTERN int          tf_text PROTO_PARAMS((char *fmt, ...));
XXTERN int          tf_typep PROTO_PARAMS((int pnum));
XXTERN void         tf_unscale_longdelay PROTO_PARAMS((char *cell, int delay_lo, int delay_hi, int *aof_delay_lo, int *aof_delay_hi));
XXTERN void         tf_unscale_realdelay PROTO_PARAMS((char *cell, double realdelay, double *aof_realdelay));
XXTERN int          tf_warning PROTO_PARAMS((char *fmt, ...));
XXTERN int          tf_write_save PROTO_PARAMS((char *blockptr, int blocklen));
XXTERN char        *tf_getroutine PROTO_PARAMS((void));
XXTERN char        *tf_igetroutine PROTO_PARAMS((char *inst));

EETERN bool      err_intercept PROTO_PARAMS((int level, char *facility, char *code));

/*---------------------------------------------------------------------------*/
/*----------------------------------- Globals ---------------------------------*/
/*---------------------------------------------------------------------------*/

extern DLLESPEC char *veriuser_version_str;
extern DLLESPEC int (*endofcompile_routines[])();

#undef XXTERN
#undef EETERN
#undef DLLISPEC
#undef DLLESPEC
#ifdef DEFINED_PROTO_PARAMS
#undef DEFINED_PROTO_PARAMS
#undef PROTO_PARAMS
#endif

#ifdef DEFINED_EXTERN
#undef DEFINED_EXTERN
#undef EXTERN
#endif

#endif /* VERIUSER_H */
