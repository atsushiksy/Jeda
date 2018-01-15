/*                                                                              
--(C)opyright 2003 B1LAB INC.                                                   
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2                                                  
-- Title          :  header file of scheduler definition                                     
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_scheduler_h.j                                            
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>                              
-- Created        :  2003/03/06                                                 
-- Source         :                                                             
-- Last modified  :                                                             
--------------------------------------------------------------------------------
-- Description :                                                                
--   1.   schedule the running of each components                              
--------------------------------------------------------------------------------
--
--Object:
--  jvc_mpeg2_scheduler
--
--TOPOLOGY
--
--                                 |-------|
--                                 | start |
--                                 |-------|
--                                     |
--                                     |
--                      |-------------------------------|
--                      |         copy_seq()            |
--                      |      testcase.cofigure(1)     |
--                      |-------------------------------|
--                                     |
--                                     |
-                       |------------------------------|                      
--                      |fork                          |          
--                      |       gen_seq_eq_loop()      |
--                      |        tx_data_loop()        |
--                      |join                          |
--                      |------------------------------|
--                                     |
--                                     |
--                      |------------------------------| 
--                      |      start stop_timer()      | 
--                      |------------------------------| 
--                                     |
--                                     |
--                      |------------------------------| 
--                      |       monitor.check()|       | 
--                      |------------------------------| 
--                                     |   
--                                     |       
--                                 |-------| 
--                                 |  end  | 
--                                 |-------| 
--
*/                                                                              

void scheduler() {
  copy_seq();              //creat a new user video sequence struct
  env.seq_num=sequence_num;
  printf("schedular_h.j sequence_num=%d\n",env.seq_num); 
  testcase.cofigure(1) ;   // configure user testcase file jvc_mpeg2_testcase_h.j
  u_video.para_check();
  
  fork 
  {
     gen_seq_eq_loop();   // start gen sequence element loop

     tx_data_loop();      // start transmit data loop
  }
  join

   stop_timer();          //stop timer begin counter
   monitor.check();       //check the result and report the message

} //scheduler() 



