/*
  Jeda Runtime Data Structures
 */

/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
   Copyright (C) 2002, 2003 Jeda Technologies, Inc.
   Copyright (C) 2018 Atsushi Kasuya

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

/* debug */
/* #define JEDA_DATA_TRACE */

#ifdef ICARUS_VERILOG_MODE
#  define DOSTOP vpi_control(vpiStop,0)
#  define DOFINISH vpi_control(vpiFinish,0)
#else
#  define DOSTOP tf_dostop()
#  define DOFINISH tf_dofinish()
#endif

#ifdef _POSIX_THREADS
#  include <pthread.h>
#endif

#define PROGNAME "Jeda"

#define NULL_CHAR '\0'

/* remporary procedure of fatal error, eventually this will set a flag
   to return the control to Verilog.
 */
#define FATAL_ERROR JEDA_fatal_error()

#define WARNING_HANDLER JEDA_warning_handler()

/* debug feature not supported yet */

#define JD_DATA_HASH_SIZE 64
#define JD_DATA_HASH_SHIFT0 6
#define JD_DATA_HASH_SHIFT1 12
#define JD_DATA_HASH_SHIFT2 18
#define JD_DATA_HASH_SHIFT3 24

#define CALC_DATA_HASH(x)  ( ( x + (x>>JD_DATA_HASH_SHIFT0) + (x>>JD_DATA_HASH_SHIFT1) + (x>>JD_DATA_HASH_SHIFT2) + (x>>JD_DATA_HASH_SHIFT3) ) % JD_DATA_HASH_SIZE )
#define CALC_STRING_HASH(x,y) \
  x = 0 ; { char *p = y ; while(*p != '\0') x += *p++ ; } x %= JD_DATA_HASH_SIZE 

#define WAIT_QUEUE_SIZE_UNIT 32
#define EVENT_QUEUE_SIZE_UNIT 32

#ifdef VERILOG_IO
#  ifdef JEDA_VPI_MODE
#    define PRINTF vpi_printf
#  else
#    define PRINTF io_printf
#  endif

#else
#  define PRINTF printf
#endif

#ifdef JEDA_VPI_MODE
#  define SCAN_PLUSARGS JD_scan_plusargs
#else
#  define SCAN_PLUSARGS mc_scan_plusargs
#endif

/* debugger name table type code */
#define DBG_TYPE_FUNC  1
#define DBG_TYPE_CLASS  2
#define DBG_TYPE_BLOCK  3
#define DBG_TYPE_PORTSET  4
#define DBG_TYPE_PORT  5
#define DBG_TYPE_VAR  6
#define DBG_TYPE_ARG  7
#define DBG_TYPE_ENUM  8
#define DBG_TYPE_MVAR  9
#define DBG_TYPE_ASPECT  10
#define DBG_TYPE_ADVICE  11
#define DBG_TYPE_AVAR  12


#ifdef JEDA_DEBUGGER
  #define SET_LOCATION(a,b,c,d,e) JD_debugger_set_location( a, b, c, d, e )
  #define CODE_LOCATION JD_debugger_code_location() 
  #define DBG_CODE_LOCATION JD_debugger_dbg_code_location() 
  #define SET_POINTCUT_LOCATION JD_debugger_set_pointcut_location() ;
  #define GET_POINTCUT_LOCATION(x,y) JD_debugger_get_pointcut_location( x, y )

  #define GET_CURRENT_LOCATION(x,y) JD_debugger_get_current_location( x, y )
  #define SET_LOCAL_SCOPE(x) JD_debugger_set_local_scope( x )
  #define SET_ENUM(x) JD_debugger_set_enum( x )

  #define FORK_DEBUG_STACK(x) JD_debugger_fork_stack( x )
  #define SET_GLOBAL_ENUM(x,y) 
  #define SET_ENTER_FUNC JD_debugger_enter_function()
  #define SET_ENTER_MFUNC JD_debugger_enter_member_function()
  #define SET_ENTER_CHILD_MFUNC JD_debugger_enter_child_member_function()
  #define SET_EXIT_FUNC JD_debugger_exit_function()  
  #define SET_FORK_EXIT  JD_debugger_set_fork_exit()
  #define SET_FORK_EXEC  JD_debugger_set_fork_exec()
  #define SET_JOIN  JD_debugger_set_join()
  #define SET_KNIFE JD_debugger_set_knife()
  #define SET_SPOON JD_debugger_set_spoon()
  #define SET_ENTER_AFUNC JD_debugger_enter_aspect_function()
  #define SET_ENTER_ADVICE JD_debugger_enter_aspect_advice()
  
  /* enable verilog shadow var update */
  #define JEDA_VERILOG_VAR_UPDATE
  
#else

  #ifndef NO_CODE_LOCATION

extern int JD_code_file_index ;
extern int JD_code_line_num ;
extern int JD_pointcut_file_index ;
extern int JD_pointcut_line_num ;

    #define SET_LOCATION(a, b, c, d, e) JD_code_file_index = b ; JD_code_line_num = d
    #define CODE_LOCATION JD_code_location() 
    #define DBG_CODE_LOCATION JD_dbg_code_location() 
    #define SET_POINTCUT_LOCATION   JD_pointcut_file_index = JD_code_file_index ; JD_pointcut_line_num = JD_code_line_num 

  #else

    #define SET_LOCATION(a,b,c,d,e) 
    #define CODE_LOCATION  
    #define DBG_CODE_LOCATION  
    #define SET_POINTCUT_LOCATION 

  #endif

  #define GET_CURRENT_LOCATION(x,y) JD_get_current_location( x, y )
  #define SET_LOCAL_SCOPE(x) 
  #define SET_ENUM(x) 
  #define EXEC_BREAKPOINT
  #define GET_POINTCUT_LOCATION(x,y) JD_get_pointcut_location( x, y )

  #define FORK_DEBUG_STACK(x) 
  #define SET_GLOBAL_ENUM(x,y) 
  #define SET_ENTER_FUNC 
  #define SET_ENTER_MFUNC 
  #define SET_ENTER_CHILD_MFUNC 
  #define SET_EXIT_FUNC
  #define SET_FORK_EXIT
  #define SET_FORK_EXEC  
  #define SET_JOIN
  #define SET_KNIFE
  #define SET_SPOON
  #define SET_ENTER_ADVICE
  #define SET_ENTER_AFUNC

#endif

#ifdef DATA_GC_ASSERT
  #define ASSERT_STACK_ALIVE \
  { \
    int i ; \
    for( i = 0 ; i < SP ; i++ ) \
      JD_assert_unfree_data(STACK[i], __FILE__, __LINE__) ; \
  }
  #define ASSERT_DATA_ALIVE(x) \
    JD_assert_unfree_data(x, __FILE__, __LINE__)
#else
  #define ASSERT_STACK_ALIVE
  #define ASSERT_DATA_ALIVE(x) 
#endif

/* JD_NULL is used to represent null object */
#define JD_NULL (void *)0x01 

#define VSP_LIMIT 8
#define INIT_WD_SIZE 64

#define DEFAULT_STRING_SIZE 128

typedef struct JD_toplevel_s JD_toplevel ;

typedef struct JD_thread_s JD_thread ;

typedef struct JD_data_unit_s JD_data_unit ;

typedef struct JD_data_s JD_data ;

typedef struct JD_queue_s JD_queue ;

typedef struct JD_wait_queue_s JD_wait_queue ;

typedef struct JD_object_table_s JD_object_table ;

typedef struct JD_hash_entry_s JD_hash_entry ;

typedef struct JD_file_s JD_file ;

typedef struct JD_debug_scope_s JD_debug_scope ;
typedef struct JD_debug_file_s JD_debug_file ;

typedef struct JD_export_func_s JD_export_func ;

typedef struct JD_module_table_s JD_module_table ;

/* sys class structures */
typedef struct JD_event_structure_s JD_event_structure ;
typedef struct JD_semaphore_structure_s JD_semaphore_structure ;

/* debugger define & structure */
typedef struct JD_debug_stack_s JD_debug_stack ;
#ifdef JEDA_SELF_THREAD_CONTROL
/* Jeda embedded light weight process, only for sparc */
typedef struct JD_lwp_s JD_lwp ;

#ifdef JEDA_AMD64_SUPPORT
struct JD_lwp_s {
  uint64_t  *ret_sp ;       // 0  
  uint64_t ret_rbp ;        // 8 
  uint64_t ret_rbx ;        // 16
  uint64_t ret_r12 ;        // 24
  uint64_t ret_r13 ;        // 32
  uint64_t ret_r14 ;        // 40
  uint64_t ret_r15 ;        // 48
  void (*func)() ;          // 56 
  JD_thread *arg ;          // 64 
  uint64_t  *sp ;           // 72 
  uint64_t rbp ;            // 80
  uint64_t rbx ;            // 88
  uint64_t r12 ;            // 96
  uint64_t r13 ;            // 104
  uint64_t r14 ;            // 112
  uint64_t r15 ;            // 120
  void (*reta)() ;          // 128 
  void (*retc)() ;          // 136 
  uint64_t  *save_stack ;   // 144 
  uint64_t stack_size ;     // 152 
  uint64_t  **stack_base ;  // 160 + 8 = 168 total size 
} ;
#else
struct JD_lwp_s {
  void (*ret)() ;      /* 0  */
  int  *ret_fp ;       /* 4  */
  int  *ret_sp ;       /* 8  */
  void (*func)() ;     /* 12 */ 
  JD_thread *arg ;     /* 16 */
  int  *fp ;           /* 20 */
  int  *sp ;           /* 24 */
  void (*reta)() ;     /* 28 */
  void (*retb)() ;     /* 32 */
  int  *save_stack ;   /* 36 */
  int  stack_size ;    /* 40 */
  int  **stack_base ;  /* 44 */
  int  *save_ebx ;     /* 48 */
} ;
#endif
#endif

/*  Data types */
typedef enum {
  JD_D_unknown = 0,
  JD_D_void = 0,
  /* JD_D_x_int,  ** old, removed */
  JD_D_int,
  JD_D_sbit,
  JD_D_vbit,  /* bit in vstack */
  JD_D_bit,
  JD_D_float,
  JD_D_double,
  JD_D_object,
  JD_D_thread,
  JD_D_null,
  JD_D_string,
  JD_D_signal,
  JD_D_module,
  JD_D_func,
  JD_D_frame,
  JD_D_pointer,
  JD_D_dt,         /* for ACC_TYPE, type is in ACC_DT->type */
  JD_D_parent,     /* used in debugger to represent class, portset names */
  JD_D_enum        /* used in debugger to represent ename member name */
} JD_data_type ;

/*  Data sub-types */
typedef enum {
  JD_SD_single,
  JD_SD_array,
  JD_SD_marray,
  JD_SD_assoc,
  JD_SD_strided,
  JD_SD_list,
  JD_SD_array_entry,
  JD_SD_ptr_offset,
  JD_SD_const
} JD_subdata_type ;

/* verilog compatible two value structure 
   This structure may not be used..

typedef struct {
  int expr_ngroups; /* no of groups in expression value *
  int expr_vec_size; /* no of bits in expression value *
  s_vecval *expr_value_p; /* pointer to expression value *
} JD_bit;
*/

/*   
 JD_data_unit is the secondary data structure to conform list
 data type. The pointer to a continuous (or lined list of) data
 structure is hold in the JD_data structure in the case the
 data sub_type is list.
 */
struct JD_data_unit_s {
  JD_data_unit *pre ;
  JD_data_unit *next ;
  /* This field is also used to point back to the original JD_data
   * when subtype is JD_SD_array_entry. 
   */
  JD_data *data ;
} ;


/* 
 * JD_data is the primitive data structure of Jeda Virtual Machine
 *   This can contain any type of single data type, plus pointer to
 *   the array of JD_data_unit, or hash_table.
 *   Mark-Sweep GC with generation information will be used. 
 *   Design hope is that this structure becomes 8 word size for memory 
 *   efficiency.
 */
 
#define JD_DATA_MARK       0x80000000 

#define MARKED(d)  (d !=NULL && (d->flags_life & JD_DATA_MARK))
#define MARK(d) d->flags_life |= JD_DATA_MARK
#define UNMARK(d) d->flags_life &= ~JD_DATA_MARK

#define JD_DATA_CLONE    0x40000000

#define CLONED(d)  (d !=NULL && (d->flags_life & JD_DATA_CLONE))
#define CLONE(d) d->flags_life |= JD_DATA_CLONE
#define UNCLONE(d) d->flags_life &= ~JD_DATA_CLONE

/* allocate flag is set when an array is being allocated. When this flag
 * is set, the data will not be moved to generation1 even it's aged.
 * This prevents from the array data being moved to gen1 (partially) while
 * it's being allocated
 */
 
#define JD_DATA_ALLOC    0x20000000

#define ALLOCATING(d)  (d !=NULL && (d->flags_life & JD_DATA_ALLOC))
#define SETALLOCATE(d) d->flags_life |= JD_DATA_ALLOC
#define RESETALLOCATE(d) d->flags_life &= ~JD_DATA_ALLOC

/* Sysobject flag is set to the class defined with c code and is not the
 * subject to scan for garbage collection
 */
#define JD_DATA_SYSOBJ    0x10000000

#define IS_SYSOBJECT(d)  (d !=NULL && (d->flags_life & JD_DATA_SYSOBJ))
#define SETSYSOBJECT(d) d->flags_life |= JD_DATA_SYSOBJ

/* Static flag is set on static variable to detect the variable that
   has shadow reg in verilog side. This is only the static var, as
   global var by itself can be known.
 */
#define JD_DATA_STATIC    0x08000000

#define IS_STATICDATA(d)  (d !=NULL && (d->flags_life & JD_DATA_STATIC))
#define SETSTATICDATA(d) d->flags_life |= JD_DATA_STATIC

/* Duplicated flag is set when a data is duplicated to multiple threads'
   stack. This data can not be GC with quick method
 */
#define JD_DATA_DUPLICATED    0x04000000

#define IS_DUPLICATED(d)  (d !=NULL && (d->flags_life & JD_DATA_DUPLICATED))
#define SETDUPLICATED(d) d->flags_life |= JD_DATA_DUPLICATED

#define JD_LIFE_MASK       0x000000ff
#define LIFECNT(d)  (d->flags_life & JD_LIFE_MASK)

struct JD_data_s { 
  JD_data_type      type ;
  JD_subdata_type sub_type ;
  int flags_life ;                
  /* bit 31: mark flag,
     bit 30: clone flag,
     bit 8:0 life count 
  */ 
  JD_data *generation ;  
    /* generation link for gc 
     * This pointer is also used to link all the free entries.
     */
  
  JD_data *base ;  

  union  { /* keep this union less than 3 word */
    int  int_value ;
    long long_value ;
    float float_value ;
    double double_value ;
    /* when the size > 32, a and b hold the pointers to array of int. */
    struct {
      int size; /* no of bits in expression value */
      int *a; /* pointer to expression value */
      int *b; /* pointer to expression value */
    } bit ;
    /* when the size <= 32, a and b holds the data itself. */
    struct {
      int size; /* no of bits in expression value */
      int a; /* a expression value */
      int b; /* b expression value */
    } sbit ;  /* size <= 32 */
    char *string ;
    struct {
      void (*alloc)() ;
      JD_object_table *ect ;
    } obj ;
    struct {
      char *port ;
      int data0 ;    /* store ub */
      int data1 ;    /* store lb */
    } signal ;
    struct {
      int (*func0)() ;
      int (*func1)() ;
      char *data0 ;
    } module ;
    struct {
      void (*func)() ;
      JD_data *data ;
    } func ;
    JD_data *next ;
    struct {
      JD_hash_entry **table ;
    } hash ;
    /* JD_bit bit; */
    struct {
      JD_data_unit *body ;
      int size ;
      int *dim_size ;  /* for marray, to hold size per dimmension */
    } array ;
    struct {
      JD_data_unit *top ;
      JD_data_unit *bottom ;
      JD_semaphore_structure *sem ;
    } list ;
    struct {
      JD_data  *pdata ;
      int       offset ;
      char     *string ;
    } ptr ;
    JD_thread *thrd ;
    
    char *debug ;  /* pointer to debug info. used in frame type */
  } data ;
#ifdef JEDA_DATA_TRACE
  int file_name ;
  int location ;
#endif
} ;


/*
 * Object table that holds member variables for class instance
 */
 
struct JD_object_table_s {
  int num_entry ;
  int tbl_size ;
  JD_data **table ;
  JD_data *super ;
  JD_data *child ;
  int ref_count ;
  void (*dealloc)() ;
  void *(*duplicate)() ;
  void *(*copy)() ;
  void *(*display)() ;
} ;

/*
 * Queue
 */
 
struct JD_queue_s {
  /* d-linked list that chains all the queue. This is used to parse
     active thread for garbage collection  
   */
  JD_queue  *queue_fw ;	
  JD_queue  *queue_bw ;
  
  JD_thread *thrd_top ;  /* pointer to the top of queue thread */
  JD_thread *thrd_end ;  /* pointer to the end of queue thread */
} ;


struct JD_wait_queue_s {
  JD_thread **thrd_list ;  /* pointer to list of threads */
  /* int *n_array ; * array of location in the thread, not needed */
  int n ;  /* current point */
  int m ;  /* max array size */
} ;

/*
 * Thread
 */

typedef enum {
  JD_thrd_unknown = 0,
  JD_thrd_ready,
  JD_thrd_sleep,
  JD_thrd_zombie,
  JD_thrd_terminated
} JD_thread_status  ;
 
struct JD_thread_s {
  int id ;
  JD_thread_status status ;
  int run_index ;         /* index to jump to the location for fork */
  
  /* queue where this thread is sitting */
  JD_queue   *queue ;
  JD_queue   *squeue ;
  
  JD_thread *thrd_fw ;
  JD_thread *thrd_bw ;
  
  /* thread chain for timed queue. 
     When a thread is queued in the time out queue, it is placed
     in the queue according to the length of time out limit.
     The actual time count time_count is substructed by the time_count
     value of the threads (sum value) located before the thread.
  */
  JD_thread *time_thrd_fw ;
  JD_thread *time_thrd_bw ;
  int  time_limit ;        /* time out count */
  int  time_count ;        /* active time counter */
  int  timeout ;           /* flag that tells time out happened */
  
  int  sysflagcount ;      /* clear event when wake up */
                           /* unit for semaphore       */
  
  /* parent/child relation are created on fork operation */
  JD_thread *parent ;
  JD_thread *child ;
  /* all the siblings are chained with d-linked sibling list */
  JD_thread *sibling_fw ;
  JD_thread *sibling_bw ;
  int wait_child ;
  int thread_join ;
  
  int terminated ; /* set when thread is terminated */
  /* child connection for termination */
  JD_thread *term_parent ;
  JD_thread *term_child ;
  JD_thread *term_fw ;
  JD_thread *term_bw ;
    
  /* waiting */
  JD_wait_queue **event_queue ;
  int *n_array ; /* array of location in the queue array */
  int event_n ;  /* size of allocated array */
  int event_m ;  /* size of allocated array */
  
  /* Instruction counter */
  int  icnt ;
  
  /* stack pointer and stack */
  int  sp ;
  JD_data **stack ;
  int stack_size ;
  int  stk_frame ;  /* stack frame top, used to access local variale */
                    /* if stk_flame != 0, stack[stk_frame] holds the 
                     * previous stk_flame value.
                     * gen_frame -> return_l
                     */
  int  fork_frame ;  /* stack frame for fork */
    
  /* keep the instance when running a class member function */
  JD_data *this ; 
  
  void (*entry_func)() ;

  /* debugger structure */
  JD_debug_stack *dbg_stack ;
  
  int main_flag ;
  
#ifdef _POSIX_THREADS
  pthread_t pth ;
  pthread_cond_t s_cond ;
  
  /* mutex and cond used at the thread creation */
  pthread_mutex_t l_mutex ; 
  pthread_cond_t l_cond ;
#endif

#ifdef JEDA_SELF_THREAD_CONTROL
  JD_lwp * lwp ;
#endif

} ;

struct JD_toplevel_s {

  /* linked list of all the queues in the system */
  JD_queue *top_queue  ;  
  /* a thread may be linked to time queue */
  JD_thread *time_queue ; 

  /* queue for thread */
  JD_queue *ready_queue ;
  JD_queue *call_queue ;
  JD_queue *sleep_queue ;
  JD_queue *zombie_queue ;

  JD_toplevel *next ;

  JD_event_structure *event_chain_top ;

  JD_data  *data_top ;
  JD_data  *free_data_top ;
  JD_data  *generation0 ;
  JD_data  *generation1 ;
  
  int thrd_count ; /* numbering thread */
  JD_thread *top_thread ;
  JD_data **global_data ;
  
  int num_global_data ;

  /* array to the pointer of PLI object linked to global var */
  int **global_obj ;  
  
#ifdef _POSIX_THREADS
  pthread_mutex_t m_mutex ;
  pthread_mutex_t d_mutex ;
  pthread_cond_t m_cond ;
#endif
  
  char *vcl_link ;
  
  int tf_isynchronized ;

  int *ref_clock_pli_obj_head ;
  
  /* toggled ref clock object list */
  int *clocked_pli_obj_head ;
  int *clocked_pli_obj_tail ;
  
  /* changed signal object list */
  int *changed_pli_obj_head ;
  int *changed_pli_obj_tail ;
  
  int *driven_pli_obj_head ;
  
  /* p_tfexprinfo */ 
  char *CycleCount_exprinfo ; 
  
  /* handle */  
  int *CycleCount_vhandle ;      /* access handle */
  char *CycleCount_instance ;
  char *module_instance ;
  
  /* JD_pli_request * */
  char *tf_isetdelay_queue ;

  JD_debug_stack *top_dbg_stack ;

} ;

typedef enum {
  JD_assoc_hash,
  JD_string_index_hash
} JD_hash_type ;

struct JD_hash_entry_s {
  JD_hash_type type ;
  JD_data  *index ;
  int  idx ;
  char *string ;
  JD_data  *data ;
  JD_hash_entry *right ;
  JD_hash_entry *left ;
  /* int dummy ; */
} ;

struct JD_event_structure_s {
  JD_queue *queue ;  /* queue */
  int trigger_status ;  /* 0: off 1: pulse  2: on */
  int timeout ;         /* 0: no timeout, >1 timeout value */
  JD_event_structure *prev ; /* chain of all the event */
  JD_event_structure *next ; /* chain of all the event */
} ;


struct JD_semaphore_structure_s {
  JD_queue *queue ;  /* queue */
  int unit ;  /* current unit */
  int timeout ;         /* 0: no timeout, >1 timeout value */
} ;

struct JD_file_s {
  FILE *file ;
  int  opened ;
  int  pipe ;
} ;

struct JD_debug_scope_s {
  int type ;  
  int index ;
  int name ;
  int dtype ;
  int ub ;
  int lb ;
  int level ;
  int parent ;
} ;

struct JD_module_table_s {
  JD_debug_scope *g_scope ;
  int *g_size ;
  JD_debug_scope *l_scope ;
  int *l_size ;
  int *l_offset ;
  JD_debug_scope *scope ;
  int *s_size ;
  int *s_offset ;
  JD_debug_file *dfile ;
  int *dfile_size ;
  int *break_table ;
  char **name_table ;
  int *name_offset ;
} ;

struct JD_debug_file_s {
  int cwd ;  
  int name ;
} ;

struct JD_export_func_s {
  void (*entry_func)() ;
} ;

/* structure for debugger scope */
typedef enum {
  JD_dbg_unknown = 0,
  JD_dbg_global,
  JD_dbg_local,
  JD_dbg_func,
  JD_dbg_fork,
  JD_dbg_advice,
  JD_dbg_foobar
} JD_dbg_type ;


struct JD_debug_stack_s {
  JD_dbg_type type ;
  int file_index ;
  int line_num ;
  int cwd_index ;
  int local_scope ;
  int scope ;
  int lnum ;
  int cycle ;
  int inclass ;
  int inaspect ;
  int stk_frame ; /* keep thrd->stk_frame */
  JD_debug_stack *caller ;
  JD_debug_stack *callee ;
  JD_debug_stack *forker ;
  JD_debug_stack *forkee ;
  JD_debug_stack *sf ;     /* sibling front link */
  JD_debug_stack *sb ;     /* sibling back link */
  JD_debug_stack *hf ;     /* history front link */
  JD_debug_stack *hb ;     /* history back link */ 
  int active ;             /* if 1, the stack scope is active */
  JD_thread *thrd ;        /* if active == 1, point the thread */
  JD_data *this ;          /* keep thrd->this per scope */
  int thrd_id ;            /* if active == 0, hold the original thread id */
  JD_debug_stack *temp ;     /* temp pointer to up-trace */ 
  int flag ;                 /* 0: temp to call, 1: temp to fork */
} ;

extern JD_thread *THREAD ;
extern JD_thread *TMP_THREAD ;
extern JD_data **STACK ;
extern JD_data *ACC ;
extern int SP ;

extern JD_data **VSTACK ;
extern int *VSTATE ;
extern int VSP ;
extern int VS_TOP ;
extern unsigned int **VS_A ;
extern unsigned int **VS_B ;
extern int *VS_WD ;

extern int VSTACK_HIT ;
extern int VSTACK_MISS ;
extern int VSTACK_FULL ;

extern int INT_ZERO ;  /* always holds zero */
extern int INT_ONE ;   /* always holds one */
extern int BIT_SIZE ;  /* bit size for allocation */
extern int ARRAY_SIZE ;  /* array size for allocation */
extern void (*ALLOC_OBJ)() ;
extern void (*DEALLOC_OBJ)() ;

extern int U_BIT ;
extern int L_BIT ;
extern int I_ENDIAN ;
extern int U_INDEX ;
extern int L_INDEX ;

extern int *A0;
extern int *B0;
extern int S0;
extern int *A1;
extern int *B1;
extern int S1 ;
extern int RET ;
extern int ERR ;
extern unsigned int *TMP_A ;
extern unsigned int *TMP_B ;
extern int TMP_WD ;
extern unsigned int *ACC_A ;
extern unsigned int *ACC_B ;
extern int ACC_SIZE ;
extern int ACC_WD ;
extern unsigned int *ACCB_A ;
extern unsigned int *ACCB_B ;
extern int ACCB_SIZE ;
extern int ACCB_WD ;
extern int ACC_INT ;
extern int TMP_INT ;
extern int TMP_INT2 ;
extern char *TMP_STRING ;
extern int DRV_STRENGTH ;
extern int STORE_SIZE ;
extern double ACC_FLOAT ;
extern double TMP_FLOAT ;
extern char *ACC_STRING ;
extern JD_data *ACC_DT ;
extern JD_data *TMP_DT ;
extern int ACC_DT_ACTIVE ;
extern JD_data_type ACC_TYPE ;
extern unsigned int *AR ;
extern unsigned int *BR ;
extern int SR ;

extern int ADVICED ;
extern unsigned int *S_ACC_A ;
extern unsigned int *S_ACC_B ;
extern int          S_ACC_SIZE ;
extern int          S_ACC_WD ;
extern int          S_ACC_INT ;
extern double       S_ACC_FLOAT ;
extern char         *S_ACC_STRING ;
extern JD_data      *S_ACC_DT ;
extern JD_data_type S_ACC_TYPE ;

extern JD_data **GLOBAL_DATA ;

extern int NUM_ARG ;

extern void (*TARGET_FUNC)() ;

JD_data *JD_get_ptr_element(JD_data *dt) ;
