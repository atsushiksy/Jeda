/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2
-- Title          :  header file of generator definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_ig_h.j
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>
-- Created        :  2003/03/06
-- Source         :
-- Last modified  :
--------------------------------------------------------------------------------
-- Description :
--   1.  gen each block data stream.
--------------------------------------------------------------------------------
*/

//global int data_qe<>;
extern int seq_header();
extern int seq_ext();
extern int group_pic_header();
extern int pic_header();
extern int pic_coding_ext();
extern int pic_data();
extern int gen_seq_end();

class jvc_mpeg2_ig {

   bit l_data_item<>;
   int data_qe<>;
   int grp_counter,pic_counter,seq_counter;
   event   ev_qe_ready;
   new(){
      ev_qe_ready = new;
      grp_counter=0;
      pic_counter=0;
      seq_counter=0;

    } //new()

   int gen_seq_eq(int num){

      int stat;

      case (num) {
         0: stat =seq_header();
         1: stat =seq_ext(); 
         2: stat =user_data_0();
         3: stat =group_pic_header();
         4: stat =user_data_1(); 
         5: stat =pic_header();      
         6: stat =pic_coding_ext();  
         7: stat =user_data_2();    
         8: stat =pic_data();       
         9: stat =gen_seq_end();   
         default:;


      } //case (num)

      return stat;
   } //gen_seq_eq()
   
   
   
}//class jvc_mpeg2_ig





