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
     printf("jvc_mpeg2_testcase picture num is %d\n",picture_num); 
      case(i){
         1       :sequence_1();
         2       :sequence_2();
         default :{
            printf("cofigure function is less the sequence_num,repeat the last sequence data");
            sequence_2();
         } //default
      }   
   
                                                              
      trace_msg=sprintf("start to configure tecase i=%d",i);                                                               
      trace(jvc_cofigure,jvc_B1LAB_DEBUG,trace_msg);                                     

  
      
   }  //cofigure(int i)      
              
              
} //jvc_mpeg2_testcase 

/*
void change_grp_and_pic_num(int a,int b){
   group_num=a;
   picture_num=b;
printf("picture_num=%d\n",picture_num);
   u_video.group_of_pic_header.default_set();

} //change_grp_and_pic_num()

*/
