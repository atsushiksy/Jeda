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
#define sequence_num 1    //define the sequenece number
void sequence_1(){
   group_num=3;

   picture_num=1;
   max_sim_time=0; 
//   u_sequence_header_code=32'hf0000007;
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
   picture_num=1;
} //sequence_2()
