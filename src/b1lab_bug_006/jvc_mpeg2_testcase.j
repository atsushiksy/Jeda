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
