/*                                                                              
--(C)opyright 2003 B1LAB INC.                                                   
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2                                                  
-- Title          :  header file of monitor                        
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_ig_h.j                                           
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>                              
-- Created        :  2003/03/06                                                 
-- Source         :                                                             
-- Last modified  :                                                             
--------------------------------------------------------------------------------
-- Description :                                                                
--    collect the parameter from the dut and check it.                                             
--------------------------------------------------------------------------------
*/                                                                              

class jvc_mpeg2_monitor {

   void check(){
    
      string trace_msg;                                                              
      trace_msg="monitorcheck starts";                                                               
      trace(jvc_monitor,jvc_B1LAB_DEBUG,trace_msg);                                                                                
   }  //monitor_check()                       
                                           
} //jvc_mpeg2_monitor






