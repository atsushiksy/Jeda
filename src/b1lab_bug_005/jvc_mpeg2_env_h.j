/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2
-- Title          :  header file of environment definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_env_h.j
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>
-- Created        :  2003/03/06
-- Source         : 
-- Last modified  :  
--------------------------------------------------------------------------------
-- Description : 
--   1.  Import all VC top level types and defs.
--
--   2.  Defines VC at top level, with 
--       Topology, with usage illustration. 
--       Parameters.
--       Events.
--------------------------------------------------------------------------------

-- Topology                                                                     
--                               [0]____________                                 
--                              | jvc_mpeg2_env |                                
--                              |_______________|                                
--                                      |                                       
--          ____________________________|____________________________           
--         |                |           |           |                |          
--   [1]___|_______   [2]___|_______    |    [3]____|______   [4]____|______    
--  |              | |              |   |   |              | |              |   
--  |   jvc_mpeg2_ | | jvc_mpeg2_ig |   |   |   jvc_mpeg2_ | |  jvc_mpeg2_  |   
--  |   _testcase  | |              |   |   |   in_stub    | |  out_stub    |   
--  |______________| |______________|   |   |______________| |______________|   
--                                      |                                       
--                 _____________________|______________________                 
--                |                                            |                
--       [5]______|__________                         [6]______|_____________   
--      | jvc_mpeg2_Scheduler|                       |  jvc_mpeg2_monitor    |  
--      |____________________|                       |_______________________|  
--                                                                                                           
--------------------------------------------------------------------------------  
*/

     
#include "jvc_mpeg2_default_h.j" 
#include "jvc_mpeg2_table.j"      
#include "jvc_mpeg2_h.j"                                  
  
#include "jvc_mpeg2_testcase_h.j" 
#include "jvc_mpeg2_testcase.j"  
#include "jvc_mpeg2_scheduler_h.j"
#include "jvc_mpeg2_ig_h.j"        
#include "jvc_mpeg2_io_stub_h.j"   
          
#include "jvc_mpeg2_monitor_h.j" 
#include "jvc_mpeg2_scheduler.j"

  
#include "jvc_mpeg2_ig.j"          
   

extern void scheduler();
extern void main();
class jvc_mpeg2_env {
   int  seq_num;        //number of the sequence header 

   int max_sim_cycle;   //max simulation cycle
      //a default value of 0 implies this threshold will be ignored.
   bit call_stop;       //when call_stop is 1 in any cycle,the program will stop 
 
   bit no_grp_header;
  
   jvc_mpeg2_trace_level trace_level  ;

   //   jvc_mpeg2_trace_module l_trace_module_name<>; 
                                                    //message for related modules

   new(){
      no_grp_header=0;
      seq_num=d_sequence_num;

      trace_level=jvc_B1LAB_ALL_LEVEL; //display all messages
      max_sim_cycle=0;
      call_stop=0;
  
   } //new()
} //jvc_mpeg2_env
   

//define 6 kind trace level used to report information during running
enum jvc_mpeg2_trace_level=
   jvc_B1LAB_NONE,               //no message
   jvc_B1LAB_WARNING,            //warning message
   jvc_B1LAB_ERROR_ONLY,         //check error,etc
   jvc_B1LAB_FATAL,              //errors that cause run time error
   jvc_B1LAB_DEBUG,              //messages for debugging
   jvc_B1LAB_ALL_LEVEL;          //show all above message


//define Module name that shall be used for trace messages
enum jvc_mpeg2_trace_module=
   jvc_cofigure,                //module of  cofigure
   jvc_ig,                      //module of  ig
   jvc_stub,                    //module of  stub
   jvc_monitor,                 //module of  monitor
   jvc_ALL_MODULE;              //above module




