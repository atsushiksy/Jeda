/* 
   
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
   
*/

typedef struct JD_pli_object_s JD_pli_object ;
typedef struct JD_pli_request_s JD_pli_request ;
typedef struct JD_vcl_node_s JD_vcl_node ;
typedef struct JD_static_value_node_s JD_static_value_node ;

typedef struct JD_standalone_sim_event_s JD_standalone_sim_event ;

/*****************************************************************  
 * JD_pli_object structure
 * This data structure represents an object in PLI layer. 
 *****************************************************************/
typedef enum {
  JD_obj_unknown = 0,  
  JD_obj_port,            /* connection to a port or a port in port_set */
  JD_obj_veri_task,       /* connection to verilog task */
  JD_obj_jd_task,         /* connection to JD task call (from Verilog) */
  JD_obj_task_arg,        /* connection to argment for task call */
  JD_obj_variable,         /* connection to (global) variable */
  JD_obj_static_variable   /* connection to static variable */
} JD_pli_object_type ;

typedef int JD_edge_type ;

struct JD_pli_object_s {
  JD_pli_object_type type;  /* specify the type */
  char *instance ;     /* instance pointer to $JD_task */
  
  JD_pli_request *req_head ;
  JD_pli_request *req_tail ;
  
  JD_pli_object *next ;     /* next pointer to chain the object */

  JD_pli_object *ref_obj ; /* next pointer to chain the object under ref_clk */
  
  JD_vcl_node *vcl ;
  
  int clocked ;
  
  /* keep the record of drive to help debug the drive conflict */
  int line_num ;
  int file_index ;
  int weak_line_num ;
  int weak_file_index ;
  int strong_line_num ;
  int strong_file_index ;
  
  int ref_clock ;   /* flag that indicates this object is a ref clock */
  int sync_count ;
  
  /* various link for queueing */
  JD_pli_object *next_clocked ; /* next pointer for clocked list */
  JD_pli_object *next_active ; /* next pointer for active list */
  JD_pli_object *next_driven ; /* next pointer for driven list */
  JD_pli_object *next_on_clk ; /* next pointer for ref-clk list */
  JD_pli_object *next_on_ref ; /* next pointer for ref-clk list */
  
  int vcd_id ;  /* used by standalone engine to hold the VCD index */
  
  /* per type data */
  union {
    struct {
      int index ;
      char *name ;
      char *pset_name ;        /* port set name if so */
      int  portset ;           /* flag for port in portset */
      
      int  ignore_conflict ;   /* flag to ignore conflict */
      
      int dir ;
      JD_edge_type s_edge ;
      int depth ;
      JD_edge_type d_edge ;
      
      int  ub ;
      int  lb ;
      int  size ;
      int  iendian ;
      int  nw ;
      int  mask ;
            
      p_tfexprinfo i_exprinfo ; /* node value in PLI 1.0 format */
      handle   i_vhandle ;      /* access handle */
      int      i_offset ;       /* argument offset */
      p_tfexprinfo o_exprinfo ; /* node value in PLI 1.0 format */
      handle   o_vhandle ;      /* access handle */
      int      o_offset ;       /* argument offset */
            
      p_vecval last_val ;   /* keep the last value for edge */
      p_vecval *pipe ;      /* keep the value in pipe */

      p_vecval sync_val ;   /* sampled value at edge of ref clock */
      p_vecval lsync_val ;  /* keep the last sync_val */
            
      unsigned  *weak_vec ;    /* vector for weak driven bits */
      unsigned  *normal_vec ;   /* vector for normal driven bits */
      unsigned  *strong_vec ;  /* vector for weak driven bits */
            
      unsigned  *temp_vec ;  /* vector for drive bits calculation */
      unsigned  *a_vec ;  /* vector for drive bits calculation */
      unsigned  *b_vec ;  /* vector for drive bits calculation */
      unsigned  *mask_vec ;  /* vector for drive bits calculation */
      
      int driven ;             /* flag to say driven in a given */
      JD_edge_type   edge ;    /* enum of any, both, pos, neg
                                * only pos, neg is valid      */
                                      
      JD_edge_type   smpl_edge ;  /* edge at sampled value */
      
      int clk_index ;          /* reference clock of this port */
      JD_edge_type   clk_edge ; /* edge for reference clock */
            
      JD_pli_request *anyedge ; /* anyedge request chain on this port */
      JD_pli_request *posedge ; /* posedge request chain on this port */
      JD_pli_request *negedge ; /* negedge request chain on this port */
      JD_pli_request *bothedge ; /* bothedge request chain on this port */
            
      JD_pli_object *pos_sample ; /* pos-sample chain on this clock */
      JD_pli_object *neg_sample ; /* neg-sample chain on this clock */
      JD_pli_object *both_sample ; /* both-sample chain on this clock */
            
      JD_pli_request *drive_clock ; /* request to drive clock this port */
      JD_pli_request *drive_back ; /* request to drive back this port */

      JD_queue *queue ;  /* queue */
      
      /* just used in standalone mode */
      int            sample_skew ;
      int            drive_skew ;
      p_vecval final_val ;  /* final value for standalone */
      
    } port ;
          
    struct {
      char *name ;
      p_tfexprinfo called ;     /* call node value in PLI 1.0 format */
      handle call_handle ;      /* handle : access handle */
      p_tfexprinfo done ;       /*  call node value in PLI 1.0 format */
      handle done_handle ;      /* handle : access handle */
      int   busy ;              /* flag to say the task is in use */
      int   num_args ;          /* number of arguments */
      JD_pli_object *args ;     /* link list of task argument */
      JD_queue *queue ;  /* queue */
      int index ;
      char *thrd ;       /* pointer to thread structure for jeda func call */
    } task ;  /* both veri_task and jd_task */
          
    struct {
      int index ;              /* argument location in task call */
      p_tfexprinfo value ;     /* node value in PLI 1.0 format */
      handle val_handle ;      /*  access handle */
    } arg ;
          
    struct {
      char *class ;
      char *func ;
      char *name ;
      p_tfexprinfo value ;     /* node value in PLI 1.0 format */
      handle vhandle ;
      int index ;
      int size ; /* standalone only */
      p_vecval final_val ;  /* final value for standalone */
    } variable ;
          
  } is ;
} ;
  
/***********************************************************************
 * JD_pli_request structure
 * This data structure represents an request sent from JD in PLI layer. 
 ***********************************************************************/
typedef enum {
  JD_req_unknown = 0,  
  JD_req_sync,        /* request to sync on edge of a clock port */
  JD_req_sync_port,   /* request to sync on edge of a non-clock port */
  JD_req_sync_p_on_c,   /* request to sync on edge of port on ref-clk */
  JD_req_veri_task,   /* request to call verilog task */
  JD_req_jd_task,     /* request to call JD task (from Verilog) */
  JD_req_unit_delay,  /* request to wait for given unit delay */
  JD_req_drive_clock, /* request to drive clock continuously */
  JD_req_drive_back,  /* request to drive back one cycle after any
                         drive */
} JD_pli_request_type ;  

struct JD_pli_request_s {
  JD_pli_request_type type ;  /* specify the type */
        
  /* request is chained in double-linked list for easy deletion */
  JD_pli_request *next ;     /* next pointer to chain the request */
  JD_edge_type edge ;
        
  JD_thread     *thread ;    /* thread who generate this request,
                                    * or thread executing JD task call  */
  
  int   unit_delay ;         /* waiting time */
  int   dly_hi ;             /* simulation time to be waken after dly */
  int   dly_lo ;
  JD_pli_request *dly_next ; /* single link queue for delay request */
  
  /* for debug */
  int line_num ;
  int file_index ;
  
  /* per type data */
  union {
        
    struct {
      JD_pli_object *port ;  /* pointer to port obj */
      JD_edge_type   edge ;  /* enum of any, both, pos, neg */
      int           on_edge ;  /* for sync_port, sprcify if sync must
                                * be done on the ref-clock edge       */
      int num_cycle ;
    } sync ;  /* for req_sync, req_sync_port, and ref_clock */
          
    struct {
      JD_pli_object *task ;  /* pointer to veri_task obj */
    } call ;  /* both for veri_task and jd_task calls */
                    
    struct {
      JD_pli_object *port ;  /* pointer to port obj to be driven */
      int   period1 ;
      int   period2 ;
      int   initial ;
      int   startdly ;
      int   status ;    /* status of driving sequence
                         * 0: startdly period
                         * 1: period1
                         * 2: period2
                         * 3: stopped
                         */
    } drive_clock ;
    
    struct {
      unsigned int *a_vec ;
      unsigned int *b_vec ;
      JD_pli_object *port ;  /* pointer to port obj */
    } drive_back ;
           
  } is ;
} ;
  


struct JD_vcl_node_s {
  handle vhandle ;
  int  (*func)() ;
  char  *data ;
  int   mode ;
  JD_vcl_node *next ;
} ;

#define NUM_STATIC_HASH 512
#define STATIC_HASH(x) ((((int)x)>>2 )%NUM_STATIC_HASH)

struct JD_static_value_node_s {
  JD_data *dt ;
  int index ;
  int array_flag ;
  int array_id ;
  JD_static_value_node *next ;
} ;

typedef enum {
  JD_standalone_unknown = 0,  
  JD_standalone_drive,            
  JD_standalone_sample,       
  JD_standalone_delay
} JD_standalone_sim_type ;

struct JD_standalone_sim_event_s {
  JD_standalone_sim_type type ;
  unsigned int u_time ;
  unsigned int l_time ;
  JD_pli_object *obj ;
  p_vecval value ;  
  JD_standalone_sim_event *next ;
} ;
