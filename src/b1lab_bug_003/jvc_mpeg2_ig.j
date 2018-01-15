/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2
-- Title          :  generator definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_ig.j
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>
-- Created        :  2003/03/06
-- Source         :
-- Last modified  :
--------------------------------------------------------------------------------
-- Description :
--   1.  gen the video sequence.
--------------------------------------------------------------------------------
*/

string s,trace_msg;
int i=0;
bit[7:0] k=0;
int seq_header(){
     
   trace_msg=sprintf("sequence header is finish");
   
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);






    u_video.group_of_pic_header.l_time_code_config();   
    return 1;
}  
   
int seq_ext() {
      trace_msg="seqence extension is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
}

int user_data_0(){
     trace_msg="user_data_0 is finish";  
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);                                                                   
 
          
   return 1;

} //user_data_0()






int group_pic_header() {
   ig.grp_counter=ig.grp_counter+1;  //0 is ignore 
   if (env.no_grp_header==1){
      env.no_grp_header=0;
      return 0;
   }
   else {
      s="group_start_code";           
      pack_qe(32,u_group_start_code,s);

      s="time_code";

      u_time_code=u_l_time_code[(ig.grp_counter-1)];          
      pack_qe(25,u_time_code,s); 
  
      s="closed_gop";           
      pack_qe(1,u_closed_gop,s);
      s="broken_link";           
      pack_qe(1,u_broken_link,s);
           
              
      trace_msg="group of picture header is generation";      
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
      return 1;
   }

}

int user_data_1(){                       
   
                                             
   
   
                                                 
   trace_msg="user_data_1 is generation";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);      
   return 1;                                     
                                                 
}                                                




int pic_header() {

   ig.pic_counter=ig.pic_counter+1;
   trace_msg="picture header is generation";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);

   return 1;


}

int pic_coding_ext() {

                                                              
   trace_msg="picture header extension is generation";                                                               
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);                                     

    return 1;
}

int user_data_2(){ 
 
                       
   
                                                  
   trace_msg="user_data_2 is generation"; 
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);       
   return 1;                                      
                                                  
}                                                 




int pic_data() {

   trace_msg="picture data extension is generation";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
  }

int gen_seq_end(){
   trace_msg="sequence end code  is generation";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;


}


void pack_qe(int size,int data,string parameter ){
   int i=0;
   bit[31:0] n=0,m=1;
   bit a=0;
   m=m<<size-1;
   for (i=0;i<size;i++ ){
      
      n=m>>i;

      n=n&data;

      if (n==0) a=0;
      else a=1;
      ig.l_data_item.push(a);

      	
   }
                                                                          
   trace_msg=sprintf("%-40s =%h\n",parameter,data);                                                                       
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);                                  
     	
} //pack_qe()  



