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


   u_video.seq_ext.ck_profile_level(u_profile_and_level_indication);
   k=ig.l_data_item.num_element();
   if (k!=0) {
      for (i=0;i<k;i++){       
            ig.l_data_item.pop();
      }                       
   }  //clear ig.l_data_item
   
   s="sequence_header_code";
   pack_qe(32,u_sequence_header_code,s);
   s="horizontal_size_value";
   pack_qe(12,u_horizontal_size_value,s);
   s="vertical_size_value";
   pack_qe(12,u_vertical_size_value,s);
   s="aspect_ratio_information";
   pack_qe(4,u_aspect_ratio_information,s);
   s="frame_rate_code";
   pack_qe(4,u_frame_rate_code,s);
   s="bit_rate_value";
   pack_qe(18,u_bit_rate_value,s);
   s="market_bit";
   pack_qe(1,u_market_bit,s);
   s="vbv_buffer_size_value";
   pack_qe(8,u_vbv_buffer_size_value,s);
   s="constrained_parameters_flag";
   pack_qe(1,u_constrained_parameters_flag,s);

   if (u_load_intra_quantiser_matrix==0){
      s="load_intra_quantiser_matrix";
      pack_qe(1,u_load_intra_quantiser_matrix,s);
      }
   else {
      printf("load_intra_quantiser_matrix=1\n");
      for (i=0;i<64;i++){
         k=u_intra_quantiser_matrix[i];
         s=sprintf("non_intra_quantiser_matrix NO=%d,value=%d\n",i,k);
         pack_qe(8,k,s);
      }
   }
   if (u_load_non_intra_quantiser_matrix==0){
      s="load_non_intra_quantiser_matrix";
      pack_qe(8,u_load_non_intra_quantiser_matrix,s);
      }
   else {
      printf("non_intra_quantiser_matrix=1\n");
      for (i=0;i<64;i++){
         k=u_non_intra_quantiser_matrix[i];
         s=sprintf("non_intra_quantiser_matrix NO=%d,value=%d\n",i,k);
         pack_qe(8,k,s);
      }
   }

   trace_msg=sprintf("sequence header is finish");
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
}

int seq_ext() {
   s="sequence extension_start_code";
   pack_qe(32,u_extension_start_code,s);
   s="sequence_extension_id";
   pack_qe(4,u_sequence_extension_id,s);
   s="profile_and_level_indication";
   pack_qe(8,u_profile_and_level_indication,s);
   s="progressive_sequence";
   pack_qe(1,u_progressive_sequence,s);
   s="chroma_format ";
   pack_qe(2,u_chroma_format ,s);
   s="horizontal_size_extension";
   pack_qe(2,u_horizontal_size_extension,s);
   s="vertical_size_extension";
   pack_qe(2,u_vertical_size_extension,s);
   s="bit_rate_extension";
   pack_qe(12,u_bit_rate_extension,s);
   s="market_bit";
   pack_qe(1,u_market_bit,s);
   s="vbv_buffer_size_extension";
   pack_qe(8,u_vbv_buffer_size_extension,s);
   s="low_delay";
   pack_qe(1,u_low_delay,s);


   s="frame_rate_extension_n ";
   pack_qe(2,u_frame_rate_extension_n,s);
   s="frame_rate_extension_d";
   pack_qe(5,u_frame_rate_extension_d,s);

   trace_msg="seqence extension is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
}

int user_data_0(){
   if (u_seq_display_ext_load==1) {
      s="display extension_start_code";
      pack_qe(32,u_extension_start_code,s);
      s="sequence_display_extension_id";
      pack_qe(4,u_sequence_display_extension_id,s);
      s="video_format";
      pack_qe(3,u_video_format,s);
      s="colour_description";
      pack_qe(1,u_colour_description,s);
      s="colour_primaries";
      pack_qe(8,u_colour_primaries,s);
      s="transfer_characteristics";
      pack_qe(8,u_transfer_characteristics,s);
      s="matrix_coefficients";
      pack_qe(8,u_matrix_coefficients,s);
      s="display_horizontal_size";
      pack_qe(14,u_display_horizontal_size,s);
      s="market_bit";
      pack_qe(1,u_market_bit,s);
      s="display_vertical_size";
      pack_qe(14,u_display_vertical_size,s);

      trace_msg="sequence display extension is finish";
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);

      } //u_seq_display_ext_load==1
   else {
      if (u_seq_scal_ext_load==1){
         s="sequence scalable extension_start_code";
         pack_qe(32,u_extension_start_code,s);
         s="sequence_scalable_extension_id";
         pack_qe(4,u_sequence_scalable_extension_id,s);
         s="scalable_mode";
         pack_qe(2,u_scalable_mode,s);
         s="layer_id";
         if (u_scalable_mode==2'b01){  //spatial scalability
            pack_qe(4,u_layer_id,s);
            s="lower_layer_prediction_horizontal_size";
            pack_qe(14,u_lower_layer_prediction_horizontal_size,s);
            s="market_bit";
            pack_qe(1,u_market_bit,s);
            s="lower_layer_prediction_vertical_size";
            pack_qe(14,u_lower_layer_prediction_vertical_size,s);
            s="horizontal_subsampling_factor_m";
            pack_qe(5,u_horizontal_subsampling_factor_m,s);
            s="horizontal_subsampling_factor_n";
            pack_qe(5,u_horizontal_subsampling_factor_n,s);
            s="vertical_subsampling_factor_m";
            pack_qe(5,u_vertical_subsampling_factor_m,s);
            s="vertical_subsampling_factor_n";
            pack_qe(5,u_vertical_subsampling_factor_n,s);

            trace_msg="sequence spatial scalability is finish";
            trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);

         }  //  u_scalable_mode==2'b01

         if (u_scalable_mode==2'b11){ //temporal scalability
            s="picture_mux_enable";
            pack_qe(1,u_picture_mux_enable,s);
            if (u_picture_mux_enable==1) {
               s="mux_to_progressive_sequence";
               pack_qe(1,u_mux_to_progressive_sequence,s);
   
            }
            s="picture_mux_order";                     
            pack_qe(3,u_picture_mux_order,s);          
            s="picture_mux_factor";                    
            pack_qe(3,u_picture_mux_factor,s);         
                                                       
            trace_msg="sequence temporal scalability is finish";
            trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);   

         } //u_scalable_mode==2'b11
      }
   }

   if (u_user_data_0_load==1){
      printf("user_data_0_load==1\n");
      for (i=0;i<(u_user_data_0_len);i++){
         printf("u_user_data_0_len=%d\n",u_user_data_0_len);
         k=u_user_data_0[i];
         s=sprintf("user_data_0 NO=%d,value=%h\n",i,k);
         pack_qe(8,k,s);
   }
   trace_msg="user_data_0 is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   }

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
      pack_qe(1,u_closed_gop[(ig.grp_counter-1)],s);
      s="broken_link";
      pack_qe(1,u_broken_link[(ig.grp_counter-1)],s);

      trace_msg="group of picture header is finish";
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
      return 1;
   }

}

int user_data_1(){

   if (u_user_data_1_load==1){
      printf("user_data_1_load==1\n");
      for (i=0;i<(u_user_data_1_len);i++){
         printf("u_user_data_1_len=%d\n",u_user_data_1_len);
         k=u_user_data_1[i];
         s=sprintf("user_data_1 NO=%d,value=%h\n",i,k);
         pack_qe(8,k,s);
      }
      trace_msg="user_data_1 is finish";      
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
      return 1;                                  
      }
   else {
      trace_msg="user_data_1 is empty";        
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg); 
      return 0;                                
   }
}

int pic_header() {
   int m=0,n=0;
   ig.pic_counter=ig.pic_counter+1;
   m=ig.grp_counter-1;
   n=ig.pic_counter-1;
   
   s="picture_start_code";
   pack_qe(32,u_picture_start_code,s);
   s="temporal_reference";
   pack_qe(10,u_temporal_reference[m][n],s);
   s="picture_coding_type";
   i=u_picture_coding_type[m][n];  
   pack_qe(3,i,s);
   s="vbv_delay";
   pack_qe(16,u_vbv_delay[m][n],s);

   if ((i==2)||(i==3)){
      s="full_pel_forward_vector";
      pack_qe(1,u_full_pel_forward_vector[m][n],s);
      s="forward_f_code";
      pack_qe(3,u_forward_f_code[m][n],s);
   } //if

   if (i==3){
      s="full_pel_backward_vector";
      pack_qe(1,u_full_pel_backward_vector[m][n],s);
      s="backward_f_code";
      pack_qe(3,u_backward_f_code[m][n],s);
   } //if

   if (u_extra_bit_picture[m][n]==1) {
      s="extra_information_picture";
      pack_qe(8,u_extra_information_picture[m][n],s);
   } //if

   trace_msg="picture header is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);

   return 1;
}

int pic_coding_ext() {
   int m=0,n=0;
   m=ig.grp_counter-1;
   n=ig.pic_counter-1;

   s="picture extension_start_code";
   pack_qe(32,u_extension_start_code,s);
   s="picture_coding_extension_id";
   pack_qe(4,u_picture_coding_extension_id,s);

   s="f_code00";
   pack_qe(4,u_f_code00[m][n],s);
   s="f_code01";
   pack_qe(4,u_f_code01[m][n],s);
   s="f_code10";
   pack_qe(4,u_f_code10[m][n],s);
   s="f_code11";
   pack_qe(4,u_f_code11[m][n],s);
   s="intra_dc_precision";
   pack_qe(2,u_intra_dc_precision[m][n],s);

   s="picture_structure";
   pack_qe(2,u_picture_structure[m][n],s);
   s="top_field_first";
   pack_qe(1,u_top_field_first[m][n],s);     
   s="frame_pred_frame_dct";
   pack_qe(1,u_frame_pred_frame_dct[m][n],s);
   s="concealment_motion_vectors";
   pack_qe(1,u_concealment_motion_vectors[m][n],s);
   s="q_scale_type";
   pack_qe(1,u_q_scale_type[m][n],s);
   s="intra_vlc_format";
   pack_qe(1,u_intra_vlc_format[m][n],s);
   s="alternate_scan";
   pack_qe(1,u_alternate_scan[m][n],s);

   s="repeat_first_field";
   pack_qe(1,u_repeat_first_field[m][n],s);
   s="chroma_420_type";
   pack_qe(1,u_chroma_420_type[m][n],s);
   s="progressive_frame";
   pack_qe(1,u_progressive_frame[m][n],s);
   s="composite_display_flag";
   pack_qe(1,u_composite_display_flag[m][n],s);

   if (u_composite_display_flag[m][n]){
      s="v_axis";
      pack_qe(1,u_v_axis[m][n],s);
      s="field_sequence";
      pack_qe(3,u_field_sequence[m][n],s);
      s="sub_carrier";
      pack_qe(1,u_concealment_motion_vectors[m][n],s);

      s="burst_amplitude";
      pack_qe(7,u_burst_amplitude[m][n],s);
      s="sub_carrier_phase";
      pack_qe(8,u_sub_carrier_phase[m][n],s);

   } //if


   trace_msg="picture header extension is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);

    return 1;
}

int user_data_2(){
   int m=0,n=0,t=0,num=0;
   m=ig.grp_counter-1;
   n=ig.pic_counter-1; 
   if (u_quant_matrix_ext_load){
      s="quant_matrix extension_start_code";    
      pack_qe(32,u_extension_start_code,s); 
                    
      s="quant_matrix_extension_id";                                          
      pack_qe(4,u_quant_matrix_extension_id,s);                         
      s="load_intra_quantiser_matrix";                           
      pack_qe(1,u_load_intra_quantiser_matrix_q[m][n],s);
      if (u_load_intra_quantiser_matrix_q[m][n]){
         printf("load_intra_quantiser_matrix=1\n");                       
         for (i=0;i<64;i++){                                              
            k=u_intra_quantiser_matrix_q[m][n][i];                            
            s=sprintf("intra_quantiser_matrix[%d][%d] NO=%d,value=%d\n",m,n,i,k); 
            pack_qe(8,k,s);                                               
         } //for                                                              
      } //if
      
      s="load_non_intra_quantiser_matrix";                                           
      pack_qe(1,u_load_non_intra_quantiser_matrix_q[m][n],s);                          
      if (u_load_non_intra_quantiser_matrix_q[m][n]){                                  
         printf("load_non_intra_quantiser_matrix=1\n");                              
         for (i=0;i<64;i++){                                                     
            k=u_non_intra_quantiser_matrix_q[m][n][i];                                 
            s=sprintf("non_intra_quantiser_matrix[%d][%d] NO=%d,value=%d\n",m,n,i,k);
            pack_qe(8,k,s);                                                      
         } //for                                                                 
      } //if                                                                     
      
      s="load_chroma_intra_quantiser_matrix";                                            
      pack_qe(1,u_load_chroma_intra_quantiser_matrix[m][n],s);                           
      if (u_load_chroma_intra_quantiser_matrix[m][n]){                                   
         printf("load_chroma_intra_quantiser_matrix=1\n");                               
         for (i=0;i<64;i++){                                                          
            k=u_chroma_intra_quantiser_matrix[m][n][i];                                  
            s=sprintf("chroma_intra_quantiser_matrix[%d][%d] NO=%d,value=%d\n",m,n,i,k); 
            pack_qe(8,k,s);                                                           
         } //for                                                                      
      } //if                                                                                                                                                     
    
      s="load_chroma_non_intra_quantiser_matrix";                                           
      pack_qe(1,u_load_chroma_non_intra_quantiser_matrix[m][n],s);                          
      if (u_load_chroma_non_intra_quantiser_matrix[m][n]){                                  
         printf("load_chroma_non_intra_quantiser_matrix=1\n");                              
         for (i=0;i<64;i++){                                                            
            k=u_chroma_non_intra_quantiser_matrix[m][n][i];                                 
            s=sprintf("chroma_non_intra_quantiser_matrix[%d][%d] NO=%d,value=%d\n",m,n,i,k);
            pack_qe(8,k,s);                                                             
         } //for                                                                        
      } //if                                                                            
      
      } //u_quant_matrix_ext_load
   else {
      if (u_copyright_ext_load ){
         s="copyright_ext extension_start_code";              
         pack_qe(32,u_extension_start_code,s);         
         s="copyright_extension_id";                   
         pack_qe(4,u_copyright_extension_id,s);        
         s="copyright_flag";                 
         pack_qe(1,u_copyright_flag[m][n],s);

         s="copyright_identifier";
         pack_qe(8,u_copyright_identifier[m][n],s);  
         s="original_or_copy";            
         pack_qe(1,u_original_or_copy[m][n],s); 
         s="reserved";                    
         pack_qe(7,u_video.copyright_ext.reserved,s);   

         s="market_bit";
         pack_qe(1,u_market_bit,s);         
         s="copyright_number_1[m][n]";            
         pack_qe(20,u_copyright_number_1[m][n],s); 
           
         
         s="market_bit";                          
         pack_qe(1,u_market_bit,s);         
         s="copyright_number_2";            
         pack_qe(22,u_copyright_number_2[m][n],s);
         
         s="market_bit";                    
         pack_qe(1,u_market_bit,s);         
         s="copyright_number_3";            
         pack_qe(22,u_copyright_number_3[m][n],s);
                     
      } //u_copyright_ext_load
      else {
         if (u_pic_display_ext_load) {
            s="pic_display extension_start_code";    
            pack_qe(32,u_extension_start_code,s);
            s="picture_display_extension_id";            
            pack_qe(4,u_picture_display_extension_id,s); 
            
            num=u_video.pic_display_ext.gen_number_of_frame_centre_offsets(
               u_progressive_sequence,u_repeat_first_field[m][n],
               u_top_field_first[m][n],u_picture_structure[m][n]);
            
            for (i=0;i<num;i++){            
               s="frame_centre_horizontal_offset";                    
               pack_qe(16,u_frame_centre_horizontal_offset[m][n],s);
               s="market_bit";                                      
               pack_qe(1,u_market_bit,s);                           
               s="frame_centre_vertical_offset";                    
               pack_qe(16,u_frame_centre_vertical_offset[m][n],s);  
               s="market_bit";                                      
               pack_qe(1,u_market_bit,s);                                    
            } //for
       
         } //u_pic_display_ext_load
         else {
            if (u_pic_spat_scal_ext_load){
               s="pic_spat_scal extension_start_code";    
               pack_qe(32,u_extension_start_code,s);
               s="picture_spatial_scalable_extension_id";            
               pack_qe(4,u_picture_spatial_scalable_extension_id,s); 
               
               s="lower_layer_temporal_reference";                  
               pack_qe(10,u_lower_layer_temporal_reference[m][n],s);
               s="market_bit";                                      
               pack_qe(1,u_market_bit,s);                           
               s="lower_layer_horizontal_offset";                    
               pack_qe(15,u_lower_layer_horizontal_offset[m][n],s);  
               s="market_bit";                                      
               pack_qe(1,u_market_bit,s);                           
            
               s="lower_layer_vertical_offset";                  
               pack_qe(15,u_lower_layer_vertical_offset[m][n],s);
               s="spatial_temporal_weight_code_table_index";                                     
               pack_qe(1,u_spatial_temporal_weight_code_table_index[m][n],s);                               
               
               s="lower_layer_progressive_frame";                 
               pack_qe(1,u_lower_layer_progressive_frame[m][n],s);
               s="lower_layer_deinterlaced_field_select";                 
               pack_qe(1,u_lower_layer_deinterlaced_field_select[m][n],s);      
                       
      
            } //u_pic_spat_scal_ext_load
            else {
               if (u_pic_temp_scal_ext_load){
                  s="pic_temp_scal extension_start_code";    
                  pack_qe(32,u_extension_start_code,s);
                  s="picture_temporal_scalable_extension_id";           
                  pack_qe(4,u_picture_temporal_scalable_extension_id,s);
                                                                       
                  s="reference_select_code";                  
                  pack_qe(2,u_reference_select_code[m][n],s);
                  s="forward_temporal_reference";                  
                  pack_qe(10,u_forward_temporal_reference[m][n],s); 
                  s="market_bit";                                      
                  pack_qe(1,u_market_bit,s);                           
                  s="backward_temporal_reference";                  
                  pack_qe(10,u_backward_temporal_reference[m][n],s);        
               } //u_pic_temp_scal_ext_load
               else t=1; // no data gen            
            } //else u_pic_spat_scal_ext_load         
         }  //else u_pic_display_ext_load    
      } //else u_copyright_ext_load
   
   } //else
   
   
   
   
   
   if (u_user_data_2_load==1){
      printf("user_data_2_load==1\n");
      for (i=0;i<(u_user_data_2_len);i++){
         printf("u_user_data_2_len=%d\n",u_user_data_2_len);
         k=u_user_data_2[i];
         s=sprintf("user_data_2 NO=%d,value=%h\n",i,k);
         pack_qe(8,k,s);
      }



      trace_msg="user_data_2 is finish";
      trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   }


   if ((t==0)||(u_user_data_2_load==1)) return 1;  //data has gen
   else return 0;
   

}




int slice(){
   int m=0,n=0,p=0;
   m=ig.pic_counter-1;
   n=ig.slice_counter;
   in_stub.is_pake_add_0=0;  //zer0 can't be added at the end of slice data
   ig.slice_counter=ig.slice_counter+1;
  
      s="slice extension_start_code"; 
      p=prefix_value+u_slice_start_code_value[m][n];        
      pack_qe(32,u_slice_start_code_value[m][n],s); 
      
      if (u_horizontal_size>2800){
         s="slice_vertical_position_extension";                  
         pack_qe(3,u_slice_vertical_position_extension[m][n],s); 
      } //u_horizontal_size>2800      
     
      if (u_seq_scal_ext_load) {
         if (u_scalable_mode==2'b00) {
            s="priority_breakpoint";                 
            pack_qe(7,u_priority_breakpoint[m][n],s);         
         } //u_scalable_mode==2'b00(data partition)      
      } //u_seq_scal_ext_load
                        
      s="quantiser_scale_code";                               
      pack_qe(5,u_quantiser_scale_code[m][n],s); 
       
      if (u_intra_slice_flag[m][n]) {
         s="intra_slice_flag";                  
         pack_qe(1,u_intra_slice_flag[m][n],s); 
         
         s="intra_slice";                  
         pack_qe(1,u_intra_slice[m][n],s); 
         s="reserved_bit";
         pack_qe(7,0,s);  
         if(u_extra_bit_slice[m][n]) {
            s="extra_bit_slice";                 
            pack_qe(1,u_extra_bit_slice[m][n],s);
            s="extra_information_slice";                 
            pack_qe(8,u_extra_information_slice[m][n],s);            
         } //extra_bit_slice[m][n]
                  
      } // u_intra_slice_flag[m][n] 
      
      s="extra_bit_slice_0";                           
      pack_qe(1,u_extra_bit_slice_0,s);              
    
   trace_msg="slice data extension is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;

} //silce()
int mb(){
   int i ; // localize 'i' to avoid the for loop mess up
   int m=0,n=0,p=0,q=0,pre_add=0;
   ig.mb_counter=ig.mb_counter+1;
   m=ig.grp_counter-1;
   n=ig.pic_counter-1;

   p=ig.slice_counter-1;
   q=ig.mb_counter-1;
   

   if (n==0) {
      pre_add=0;
   }
   else {
      pre_add=u_mb_addr_value[n][p][q-1];
   }
   
   u_video.mb.gen_mb_escape_num(pre_add,u_mb_addr_value[n][p][q]);
   printf("u_mb_escape_num=%d",u_mb_escape_num);
   if (u_mb_escape_num>0) {      
      for (i=0;i<u_mb_escape_num;i++) {
         s="macroblock_escape";                 
         pack_qe(11,11'b00000001000,s);
      }      
   }
   u_get_vlc_table(Table_B_1,u_mb_addr_increment_index);
   s="macroblock_address_increment_vlc";                  
   pack_qe(u_vlc_len,u_vlc_value,s); 
   
   macroblock_mode();
    
   if (u_macroblock_quant) {
      s="quantiser_scale_code_mb";                 
      pack_qe(5,u_quantiser_scale_code_mb[n][p][q],s);
   }
   if ((u_macroblock_motion_forward)||(u_macroblock_intra&&u_concealment_motion_vectors[m][n])) {                          // [m][n] is added by A.K.
      motion_vectors(0);        
   }                                                  
   if (u_macroblock_motion_backward) {
      motion_vectors(1);
   }
   if (u_macroblock_intra&&u_concealment_motion_vectors[m][n]) { // [m][n] is added by A.K.
      s="market_bit";           
      pack_qe(1,u_market_bit,s);
   }
   if (u_macroblock_pattern) {
      code_block_pattern();   
   }
  
   for (i=0;i<u_block_num;i++) {
      u_get_pattern_code(u_chroma_format,u_coded_block_pattern_1[n][p][q],u_coded_block_pattern_2[n][p][q]);
      block(i);
   }
      
  
  
   trace_msg="mb data extension is finish";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
} //mb()

void macroblock_mode() {
   int m,n,p,q;                                                                                   
   m=ig.grp_counter-1;                                                                                        
   n=ig.pic_counter-1;                                                                                        
   p=ig.slice_counter-1;                                                                                      
   q=ig.mb_counter-1;                                                                                         
                                                                                                                                                                                                                         
   if (u_seq_scal_ext_load) {                                                                                 
      if (u_scalable_mode==2'b01) {  //spatial_scal                                                           
         if (u_pic_spat_scal_ext_load) {                                                                      
            case(u_picture_coding_type[m][n]) {                                                               
               I: u_get_vlc_table(Table_B_5,u_macroblock_type_index[m][n][p]);                                         
               P: u_get_vlc_table(Table_B_6,u_macroblock_type_index[m][n][p]);                                         
               B: u_get_vlc_table(Table_B_7,u_macroblock_type_index[m][n][p]);                                         
            }                                                                                                 
         }                                                                                                    
         else {                                                                                               
            case(u_picture_coding_type[m][n]) {                                                               
               I: u_get_vlc_table(Table_B_2,u_macroblock_type_index[m][n][p]);                                         
               P: u_get_vlc_table(Table_B_3,u_macroblock_type_index[m][n][p]);                                         
               B: u_get_vlc_table(Table_B_4,u_macroblock_type_index[m][n][p]);                                         
            }                                                                                                 
         } // (u_pic_spat_scal_ext_load)                                                                      
      }                                                                                                       
      else {                                                                                                  
         if (u_scalable_mode==2'b10) {    //SNR_scal                                                          
            u_get_vlc_table(Table_B_8,u_macroblock_type_index[m][n][p]);                                                   
         }                                                                                                    
         else {                                                                                               
            case(u_picture_coding_type[m][n]) {                                                               
               I: u_get_vlc_table(Table_B_2,u_macroblock_type_index[m][n][p]);                                         
               P: u_get_vlc_table(Table_B_3,u_macroblock_type_index[m][n][p]);                                         
               B: u_get_vlc_table(Table_B_4,u_macroblock_type_index[m][n][p]);                                         
            }                                                                                                 
         } //u_scalable_mode==2'b10                                                                           
      }  //else of u_scalable_mode==2'b01                                                                                                                                                                              
   }                                                                                                          
   else {                                                                                                     
      case(u_picture_coding_type[m][n]) {                                                                     
         I: u_get_vlc_table(Table_B_2,u_macroblock_type_index[m][n][p]);                                               
         P: u_get_vlc_table(Table_B_3,u_macroblock_type_index[m][n][p]);                                               
         B: u_get_vlc_table(Table_B_4,u_macroblock_type_index[m][n][p]);                                               
      }                                                                                                       
   } //else of u_seq_scal_ext_load                                                                            
                                                                                                              
   s="macroblock_type_vlc";                                                                                   
   pack_qe(u_vlc_len,u_vlc_value,s);                                                                          
   if ((u_spatial_temporal_weight_code_flag)&&(u_spatial_temporal_weight_code_inedx_0[n][p][q]!==0)){         
                                                                                                              
      s="spatial_temporal_weight_code";                                                                       
      pack_qe(2,u_spatial_temporal_weight_code[n][p][q],s);                                                   
   }                                                                                                          
                                                                                                              
   if ((u_macroblock_motion_forward)||(u_macroblock_motion_backward)) {                                       
      if (u_picture_structure[m][n]==Frame_picture) {                                                         
         if (u_frame_pred_frame_dct[m][n]==0) {                                                               
            s="frame_motion_type";                                                                            
            pack_qe(2,u_frame_motion_type[n][p][q],s);                                                        
            u_get_frame_motion_type(u_frame_motion_type[n][p][q]);                                                        
         }                                                                                                    
      }                                                                                                       
      else {                                                                                                  
         s="field_motion_type";                                                                               
         pack_qe(2,u_field_motion_type[n][p][q],s);                                                           
         u_get_field_motion_type(u_field_motion_type[n][p][q]);                                                 
      }                                                                                                       
   } //((u_macroblock_motion_forward)||(u_macroblock_motion_backward))                                        
                                                                                                              
   if ((u_picture_structure[m][n]==Frame_picture)&&(u_frame_pred_frame_dct[m][n]==0)                          
      &&(u_macroblock_intra||u_macroblock_pattern)) {                                                         
      s="dct_type";                                                                                           
      pack_qe(1,u_dct_type[n][p][q],s);                                                                       
   }                                                                                                          
} //macroblock_mode()                                                                                         
       
void motion_vectors(int ss) {                                       
   int n,p,q;                                                          
   n=ig.pic_counter-1;                                                 
   p=ig.slice_counter-1;                                               
   q=ig.mb_counter-1;                                                  
                                                                       
   if (u_motion_vector_counter) {                                      
      if ((u_my_format==0)&&(u_dmv!=1)) {                              
         s=sprintf("motion_vertical_field_select_0%d",ss);                       
         pack_qe(1,u_motion_vertical_field_select_0s[n][p][q],s);      
         motion_vector(0,ss);                                          
      }                                                                
   }                                                                   
   else {                                                              
      s=sprintf("motion_vertical_field_select_0%d",ss);                       
      pack_qe(1,u_motion_vertical_field_select_0s[n][p][q],s);         
      motion_vector(0,ss);                                             
      s=sprintf("motion_vertical_field_select_1%d",ss);                       
      pack_qe(1,u_motion_vertical_field_select_1s[n][p][q],s);         
      motion_vector(1,ss);                                                                                        
   }                                                                   
} //motion_vectors()                                                   
   
void motion_vector(int r,int ss) {   //mpeg2-ii:6.2.5.1
   int f_code=0,l=0;
   int m,n,p,q;
   m=ig.grp_counter-1;
   n=ig.pic_counter-1;
   p=ig.slice_counter-1;
   q=ig.mb_counter-1;

   //get the motion_code value from the index in Table_B_10                                              
   u_get_vlc_table(Table_B_10,u_motion_code_rs0_index[n][p][q]);           
   s="motion_code_rs0_vlc";                                                
   pack_qe(u_vlc_len,u_vlc_value,s);                                       
                                                                           
   if (ss==0) f_code=u_f_code00[m][n];  //f_code=f_code[0][0]                                   
   else f_code=u_f_code10[m][n];        //f_code=f_code[1][0] 
                                   
   if ((f_code!=1)&&(u_vlc_value!=0)) {                                    
      l=u_get_motion_residual_len(m,n,ss,0);  //get the value length                             
      s="motion_residual";                                                 
      pack_qe(l,u_motion_residual_rs0_index[n][p][q],s);                             
   }                                                                       
   if (u_dmv) {                                                            
      u_get_vlc_table(Table_B_11,u_dmvector_0_index[n][p][q]);             
      s="dmvector_0_index";                                                
      pack_qe(u_vlc_len,u_vlc_value,s);                                    
   }                                                                       
                                                                           
   u_get_vlc_table(Table_B_10,u_motion_code_rs1_index[n][p][q]);           
   s="motion_code_rs1_vlc";                                                
   pack_qe(u_vlc_len,u_vlc_value,s);                                       
   if (ss==0) f_code=u_f_code01[m][n];                                     
   else f_code=u_f_code11[m][n];                                           
   if ((f_code!=1)&&(u_vlc_value!=0)) {                                    
      l=u_get_motion_residual_len(m,n,ss,0);                               
      s="motion_residual";                                                 
      pack_qe(l,u_motion_residual_rs1_index[n][p][q],s);                             
   }                                                                       
   if (u_dmv) {                                                            
      u_get_vlc_table(Table_B_11,u_dmvector_1_index[n][p][q]);             
      s="dmvector_1_index";                                                
      pack_qe(u_vlc_len,u_vlc_value,s);                                    
   }                                                                       
                                                                           
} //motion_vector()                                                        

void code_block_pattern() { 
   int n,p,q;                                             
   n=ig.pic_counter-1;                                                      
   p=ig.slice_counter-1;                                                    
   q=ig.mb_counter-1;                                                       
                                                                            
   u_get_vlc_table(Table_B_9,u_coded_block_pattern_420_index[n][p][q]);     
   s="coded_block_pattern_420_vlc";                                         
   pack_qe(u_vlc_len,u_vlc_value,s);                                        
                                                                            
   if (u_chroma_format==2'b10) {   //4:2:2                                  
      s="coded_block_pattern_1";                                            
      pack_qe(2,u_coded_block_pattern_1[n][p][q],s);                        
   }  //(u_chroma_format==2'b10)                                            
                                                                            
   if (u_chroma_format==2'b11) {   //4:4:4                                  
      s="coded_block_pattern_2";                                            
      pack_qe(6,u_coded_block_pattern_2[n][p][q],s);                        
   }  //(u_chroma_format==2'b11)                                            
} //code_block_pattern()                                                    
   
void block(int i) { 
   int z=0; 
   int m,n,p,q; 
   m=ig.grp_counter-1;
   n=ig.pic_counter-1;                                                
   p=ig.slice_counter-1;                                                    
   q=ig.mb_counter-1;                                                                                       
     
                                                                                                
   if (u_pattern_code[i]) {                                                                                
      if (u_macroblock_intra) {                                                                            
         if (i<4) {                                                                                        
            u_get_vlc_table(Table_B_12,u_dct_dc_size_luminance_index[p][q][i]);                            
            s="dct_dc_size_luminance_vlc";                                                                 
            pack_qe(u_vlc_len,u_vlc_value,s);                                                              
            if (u_luminance_size!=0) {                                                                     
               s="luminance dct_dc_differential";                                                          
               pack_qe(u_luminance_size,u_dct_dc_differential[p][q][i],s);                                      
            }                                                                                              
         } //if (i<4)                                                                                      
         else {                                                                                            
            u_get_vlc_table(Table_B_13,u_dct_dc_size_chrominance_index[p][q][i]);                          
            s="dct_dc_size_chrominance_vlc";                                                               
            pack_qe(u_vlc_len,u_vlc_value,s);                                                              
            if (u_chrominance_size!=0) {                                                                   
               s="chrominance dct_dc_differential";                                                        
               pack_qe(u_chrominance_size,u_dct_dc_differential[p][q][i],s);                               
            }                                                                                                                                              
         }                                                                                                 
      } //if (u_macroblock_intra)                                                                          
   } //if (pattern_code[i])                                                                                
   else {                                                                                                  
      if (u_First_DCT_coefficeint_index[p][q][i]==0) {                                                     
         printf("error:u_First_DCT_coefficeint_index is 'end of the block',index=1 will be used");         
         u_get_vlc_table(Table_B_14,1);                                                                    
         s="First_DCT_coefficeint_vlc";                                                                    
         pack_qe(u_vlc_len,u_vlc_value,s);                                                                 
         add_s(i);                                                                                          
                                                                                                           
                                                                                                        
                                                                                                           
      }                                                                                                    
      else {                                                                                               
         if (u_First_DCT_coefficeint_index[p][q][i]==17) {                                                 
            printf("warning: First_DCT_coefficeint_index is escape");                                      
            u_get_vlc_table(Table_B_16_1,u_escape_run_index[i]);                                                                                       
            s="escape_run_index";                                                                          
            pack_qe(u_vlc_len,u_vlc_value,s);                                                              
            u_get_vlc_table(Table_B_16_2,u_escape_level_index[i]);                                         
            s="escape_level_index";                                                                        
            pack_qe(u_vlc_len,u_vlc_value,s);                                                                                             
         }  // escape                                                                                      
         else {                                                                                            
            u_get_vlc_table(Table_B_14,u_First_DCT_coefficeint_index[p][q][i]);                                                                                       
            s="First_DCT_coefficeint_vlc";                                                                 
            pack_qe(u_vlc_len,u_vlc_value,s);                                                              
         }                                                                                                 
      }                                                                                                    
   } //u_First_DCT_coefficeint                                                                             
   if (u_intra_vlc_format[m][n]) {  //Table B-15                                                           
      z=u_Subsequenet_DCT_coefficeint_index[p][q][i];                                                      
      u_get_vlc_table(Table_B_15,z);                                                                       
      s="Subsequenet_DCT_coefficeint_vlc";                                                                 
      pack_qe(u_vlc_len,u_vlc_value,s);                                                                    
      if (z!=0) {                                                                                          
         add_s(i);                                                                                          
         u_get_vlc_table(Table_B_15,0);                                                                    
         s="End of block Table_B_15 vlc";                                                                  
         pack_qe(u_vlc_len,u_vlc_value,s);                                                                 
      } //if                                                                                               
   }                                                                                                       
   else {                                                                                                  
       z=u_Subsequenet_DCT_coefficeint_index[p][q][i];                                                     
       u_get_vlc_table(Table_B_14,z);                                                                      
       s="Subsequenet_DCT_coefficeint_vlc";                                                                
       pack_qe(u_vlc_len,u_vlc_value,s);                                                                   
       if (z!=0) {                                                                                         
          add_s(i);                                                                                         
          u_get_vlc_table(Table_B_14,0);                                                                   
          s="End of block Table_B_14 vlc";                                                                 
          pack_qe(u_vlc_len,u_vlc_value,s);                                                                
       } //if                                                                                              
                                                                                                           
   }                                                                                                       
} //block()                                                                                                
void add_s(int i){
   s="signed_level";               
   pack_qe(1,u_signed_level[i],s);    
} 



/*------------------------------------------------------------------------------
gen_seq_end() get left len int ig.l_data_item,if have the left value at the end
of the bitstream,the left value shall be send to accomplish the the bitstream.
add the number(bus_wide-len) zero after the end_code to send out all the data.
------------------------------------------------------------------------------*/
int gen_seq_end(){
   //add the zero,num is  bus_wide. 
   s="sequence_end_code";
   pack_qe(32,u_sequence_end_code,s);
   i=ig.l_data_item.num_element()%bus_wide;
   if (i!=0) {
      s=sprintf("add the %d of 0 after the sequence_end_code",i);
      pack_qe((bus_wide-i),0,s);   
   }     
   trace_msg="sequence end code  is generation";
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   return 1;
} //gen_seq_end()


void pack_qe(int size,int data,string parameter ){
//   int i=0;
   bit[31:0] n=0,m=1;
   bit a=0;
   m=m<<size-1;
   for (i=0;i<size;i++ ){

      n=m>>i;
//      printf("n=%b\n",n);
//      printf("d=%b\n",data);

      n=n&data;

      if (n==0) a=0;
      else a=1;
      ig.l_data_item.push(a);
//      printf("a=%b\n",a);

   }

   trace_msg=sprintf("%-40s =%h\n",parameter,data);
   trace(jvc_ig,jvc_B1LAB_DEBUG,trace_msg);
   printf("l_data_item length=%d\n",ig.l_data_item.num_element());

} //pack_qe()


