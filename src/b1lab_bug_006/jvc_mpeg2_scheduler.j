/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2
-- Title          :  scheduler definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_scheduler.j
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
--                         |---------------------|
--                         |  gen_seq_eq_loop()  |
--                         |---------------------|
--                                    |
--             |--------------------->|
-              |              |-----------------|
--             |              | ig.gen_seq_eq() |
--             |              | gen_seq_num++1  |
--             ^              |-----------------|
--             ^                      |
--             |                      |
--             |           y     |---------|
--             |----------------<| state=0 |
--                               |---------|
--                                    |N
--                                    |
--                       N    |---------------|
--               |-----------<| gen_seq_num=10|
--               |            |---------------|
--               |                    |
--               |                    |
--               |           |---------------|    Y
--               |           | ig.mb_counter |>-------------|
--               |           |  <u_mb_num    |              |
--               |           |---------------|              |
--               |                    |N                    |
--               |                    |                     |
--               |           |----------------|     |---------------|
--               |           | ig.mb_counter=0|     | gen_seq_num=9 |
--               |           |----------------|     |---------------|
--               |                    |
--               |                    |
--               |           |-----------------| Y
--               |           | ig.slice_counter|>-----------|
--               |           | <u_slice_num    |            |
--               |           |-----------------|            |
--               |                    |N                    |
--               |                    |                     |
--               |           |-----------------|    |---------------|
--               |           |ig.slice_counter |    | gen_seq_num=8 |
--               |           |    =0           |    |---------------|
--               |           |-----------------|
--               |                    |
--               |                    | 
--               |           |---------------|    Y                  
--               |           |ig.pic_counter |>-------------|        
--               |           |<picture_num   |              |        
--               |           |---------------|              |        
--               |                    |N                    |       
--               |                    |                     |       
--               |           |----------------|     |---------------|
--               |           |ig.pic_counter=0|     | gen_seq_num=5 |
--               |           |----------------|     |---------------|
--               |                    |                              
--               |                    |                                                      
--               |           |-----------------| Y                   
--               |           | ig.grp_counter  |>-----------|        
--               |           | <group_num      |            |        
--               |           |-----------------|            |        
--               |                    |N                    |        
--               |                    |                     |        
--               |           |-----------------|    |---------------|
--               |           |ig.grp_counter=0 |    | gen_seq_num=3 |
--               |           |ig.seq_counter++1|    |---------------|
--               |           |-----------------|                     
--               |                    |                              
--               |                    |                              
--               |           |-----------------| Y                   
--               |           | ig.seq_counter  |>-----------|        
--               |           |  <env.seq_num   |            |        
--               |           |-----------------|            |        
--               |                    |N                    |        
--               V                    |                     |        
--               |        N  |---------------|     |---------------|
--               |-----<-----| gen_seq_num=11|     | gen_seq_num=0 |
--               V           |---------------|     |  cofigure(i)  |
--               |                   |Y            |---------------|
--               |                   |                            
--               |                   |                            
--               |           |---------------|                    
--               |           |env.call_stop=1|                    
--               |           |---------------|
--               |                   |        
--               |                   |        
--               |           |---------------|
--               |           |ig.ev_qe_ready |
--               |------>----|  .trigger()   |
--                           |---------------|
--                                   |        
--                                   |        
--                               |--------|    
--                               | knife  |  
--                               |--------|  
--
--
--
*/
void gen_seq_eq_loop() {
   static int gen_seq_num=0;   //gen the *th function
   int state;
   string trace_msg;

  fork
      while(1) {
         @(posedge CLOCK);
//       in_stub.ev_qe_send_ok.sync();
         do{
            state= ig.gen_seq_eq(gen_seq_num);
            if (state!=2) gen_seq_num=gen_seq_num+1;

         } while (state==0); //do


         if (gen_seq_num==10){          //the mb data has send
//            if (ig.mb_counter<u_mb_num[][]) {
            //printf("******************************u_mb_num=%d\n",u_mb_num);

            if (ig.mb_counter<2) {
               gen_seq_num=9;
            }
            else {
               ig.mb_counter=0;
             //<<  if (ig.slice_counter<u_slice_num[][]) {
               //printf("******************************u_slice_num=%d\n",u_slice_num);
               if (ig.slice_counter<2) {
                  gen_seq_num=8;
               }
               else {
                  in_stub.is_pake_add_0=d_is_pake_add_0;
                  ig.slice_counter=0;
                  if (ig.pic_counter<picture_num){
                     gen_seq_num=5;          //start a picture header
                  }
                  else{
                     ig.pic_counter=0;
                     if (ig.grp_counter<group_num){

                        gen_seq_num=3;       //start a new group

                     }
                     else{
                        ig.grp_counter=0;
                        ig.seq_counter=ig.seq_counter+1;
                        if (ig.seq_counter<env.seq_num){
                           gen_seq_num=0;   //start a new sequence
                           testcase.cofigure(ig.seq_counter+1);
                        }
                    }
                  }
               } //
            }
         } //if (gen_seq_num==10)
         if (gen_seq_num==11){
            env.call_stop=1;  //data is over, call stop
            printf("stop is called  call_stop=%d\n",env.call_stop);
         }
         ig.ev_qe_ready.trigger();   //inform the tx data can begin .
         trace_msg="ev_qe has trigger";
         trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
      }//while(1)

   knife

} //gen_seq_eq_loop()

void tx_data_loop() {
   fork
      while(1) {

         @(posedge CLOCK);
         ig.ev_qe_ready.sync();  //sync the  ev_qe_ready

         in_stub.tx_data ();    //send data
      } //while(1)

   knife
}   //tx_data_loop()


void copy_seq(){
   u_video=new d_video;
/*
   u_video.seq_header           =new d_video.seq_header;
   u_video.seq_ext              =new d_video.seq_ext;
   u_video.seq_display_ext      =new d_video.seq_display_ext;
   u_video.seq_scal_ext         =new d_video.seq_scal_ext;
   u_video.user_data_0          =new d_video.user_data_0;
   u_video.user_data_1          =new d_video.user_data_1;
   u_video.user_data_2          =new d_video.user_data_2;
   u_video.group_of_pic_header  =new d_video.group_of_pic_header;
   u_video.pic_header           =new d_video.pic_header;
   u_video.pic_coding_ext       =new d_video.pic_coding_ext;
   u_video.quant_matrix_ext     =new d_video.quant_matrix_ext;
   u_video.copyright_ext        =new d_video.copyright_ext;
   u_video.pic_display_ext      =new d_video.pic_display_ext;
   u_video.pic_spat_scal_ext    =new d_video.pic_spat_scal_ext;
   u_video.pic_temp_scal_ext    =new d_video.pic_temp_scal_ext;
   u_video.slice                =new d_video.slice;
   u_video.mb                   =new d_video.mb;
*/
 }  //copy_seq()


void stop_timer(){
   forever{
      @1;
      if ((env.max_sim_cycle!=0)&(env.max_sim_cycle==get_cycle())){
         env.call_stop=1;
      } //if
      if (env.call_stop==1){break;}
   } //forever
} //stop_timer()
