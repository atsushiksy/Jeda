/*--(C)opyright 2003 B1LAB INC.
------------------------------------------------------------------------------
-- Project       : JVC MPEG2
-- Title         : JVC MPEG2 environment definition
------------------------------------------------------------------------------
-- File          : JVC mpeg2_env.j
-- Author        : MIKE YIN <MIKE_YIN@b1lab.com>
-- Created       : 2003/03/06
-- Source        :
-- Last modified : 
------------------------------------------------------------------------------
*/

extern void sequence_1();
extern void sequence_2();
class jvc_mpeg2_testcase {
   
   void cofigure(int i){
      string trace_msg;  
      case(i){
         1       :sequence_1();
         2       :sequence_2();
         default :;
      }   
   
                                                              
      trace_msg=sprintf("start to configure tecase i=%d",i);                                                               
      trace(jvc_cofigure,jvc_B1LAB_DEBUG,trace_msg);                                     

  
      
   }  //cofigure(int i)      
              
              
} //jvc_mpeg2_testcase 
