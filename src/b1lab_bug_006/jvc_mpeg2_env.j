/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2
-- Title          :  environment definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_env.j
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>
-- Created        :  2003/03/06
-- Source         :
-- Last modified  :
--------------------------------------------------------------------------------
-- Description :
--   1.  Import all VC top level types and defs.
--
--   2.  Defines VC at top level, with :
--       Topology, with usage illustration.
--       Parameters.
--       Events.
--------------------------------------------------------------------------------
*/
#include "jvc_mpeg2_env_h.j"

each_sequence      d_video,u_video ;
jvc_mpeg2_in_stub  in_stub;
jvc_mpeg2_testcase testcase;
jvc_mpeg2_ig       ig;
jvc_mpeg2_env      env;
jvc_mpeg2_monitor  monitor;

void main() {
   d_video=new();
   u_video=new();   
   env=new();
   ig=new();
   in_stub=new();
   testcase=new;
   monitor=new;
   CLOCK.drive_clock(2,2,0,0);  //set the default clock
   printf("mpeg2 video VC design by jeda tool according to ISO/IEC 13818-2:1995(E)\n");
   d_video.default_set();  //set d_video instance with default value
   scheduler() ;
}








