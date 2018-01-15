/*--(C)opyright 2003 B1LAB INC.
------------------------------------------------------------------------------
-- Project       : JVC MPEG2
-- Title         : testcae definition
------------------------------------------------------------------------------
-- File          : JVC mpeg2_testcase.j
-- Author        : MIKE YIN <MIKE_YIN@b1lab.com>
-- Created       : 2003/03/06
-- Source        :
-- Last modified : 
------------------------------------------------------------------------------
-- Description :
This VC can generate all kinds off the MPEG2 bitstream by configuration. 
Each default parameter can configure in the jvc_mpeg2_default_h.j.
Each standard parameter in MPEG2 is easy to set,only add a prefix "u_" before  
the parameter name.
example: u_scalable_mode=3 //it can define the scalable_mode.
note: distinguish the parameter is a Array or integer.for more information see
jvc_mpeg2_default_h.j file.

max_sim_time=0;    //define the simulation time ,0 is ignore.
#define sequence_num 1  //define the sequence number in the current bitstream.
   note:if num>2,you must set the void cofigure() in jvc_mpeg2_testcase_h.j
   to meet the configuration and sequence_i() should define to according to
   num.

******if the patameter is omitted,the default value is used.********************
warning : if the num of the below parameter is larger than the defined value
   in jvc_mpeg2_default_h.j,you must modify the define statment to according
   to your largest value.
   max_sequence_num max_group_num max_picture_num max_slice_num max_mb_num       

group_num=1;   //define in each sequence_i().each sequence have the same num group.
picture_num=1; //define in each sequence_i().each group have the same num picture.

some function have been defined,you can easily load them to test the feature.

*/

#define sequence_num 1    //define the sequenece number
void sequence_1(){
   group_num=1;
//   picture_num=40; //less 40
   picture_num=1; //less 40

//   u_slice_num=1;
//   u_mb_num=2;
   max_sim_time=0; 
   u_sequence_header_code=32'hf0000007;
   u_broken_link[0]=1;
   u_broken_link[2]=1;
//   u_user_data_0_load=1;
//   u_user_data_0_len=2;
//   u_seq_display_ext_load=0;
//   u_load_intra_quantiser_matrix=1;
//   u_load_non_intra_quantiser_matrix=1;
//   u_scalable_mode=1;
  

} //sequence_1()

void sequence_2(){
//   u_seq_display_ext_load=0;
//   u_scalable_mode=3;
//   picture_num=1;
} //sequence_2()
