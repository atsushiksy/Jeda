/*
--(C)opyright 2003 B1LAB INC.
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2  
-- Title          :  MPEG2 class definition
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_h.j
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>
-- Created        :  2003/03/06
-- Source         : 
-- Last modified  :  
--------------------------------------------------------------------------------
-- Description : 
--   1. define the class of each block 
--   2. define the fanctoin to load the default value for each class
--      
--------------------------------------------------------------------------------
*/
enum gen_type =defaul,radom,all;
   enum Table_B=Table_B_1,Table_B_2,Table_B_3,Table_B_4,Table_B_5,Table_B_6,Table_B_7,
        Table_B_8,Table_B_9,Table_B_10,Table_B_11,Table_B_12,Table_B_13,Table_B_14,
        Table_B_15,Table_B_16_1,Table_B_16_2;

//local int i=0,h=0;
int h=0;
void trace(jvc_mpeg2_trace_module module_name ,jvc_mpeg2_trace_level level ,
   string mesg )  {

   string str_level;
   string str_module;
   
   case (module_name){
      jvc_cofigure   : str_module="jvc_cofigure";
      jvc_ig         : str_module="jvc_ig"; 
      jvc_stub       : str_module="jvc_stub";
      jvc_monitor    : str_module="jvc_monitor";
      jvc_ALL_MODULE : str_module="jvc_ALL_MODULE";
   
   }

   case (level){
      jvc_B1LAB_NONE       : str_level="jvc_B1LAB_NONE";
      jvc_B1LAB_WARNING    : str_level="jvc_B1LAB_WARNING";
      jvc_B1LAB_ERROR_ONLY : str_level="jvc_B1LAB_ERROR_ONLY";
      jvc_B1LAB_FATAL      : str_level="jvc_B1LAB_FATAL";
      jvc_B1LAB_DEBUG      : str_level="jvc_B1LAB_DEBUG";
      jvc_B1LAB_ALL_LEVEL  : str_level="jvc_B1LAB_ALL_LEVEL";
   
   }   

      //only display the messages for the required modules
   if ( (module_name==jvc_ig)| (module_name==jvc_stub)| (module_name==jvc_monitor) | (module_name==jvc_cofigure)) {  
  
      //only display the messages with higher levels
//         if level.as_a(int) <= trace_level.as_a(int) {
      printf("[%-4d] %-12s>> %-20s>> %-80s\n",get_cycle(),str_module,str_level,mesg);

   }  //if
   
} //trace




class each_sequence{
   class_seq_header           seq_header;
   class_seq_ext              seq_ext; 
   class_user_data            user_data_0,user_data_1,user_data_2;

   class_seq_display_ext      seq_display_ext;
   class_seq_scal_ext         seq_scal_ext;
   class_group_of_pic_header  group_of_pic_header;
   class_pic_header           pic_header;
   class_pic_coding_ext       pic_coding_ext;
   class_quant_matrix_ext     quant_matrix_ext;
   class_copyright_ext        copyright_ext;

   class_pic_display_ext      pic_display_ext;
   class_pic_spat_scal_ext    pic_spat_scal_ext;

   class_pic_temp_scal_ext    pic_temp_scal_ext;
   class_slice                slice;
   class_mb                   mb;
   bit[31:0]                  seq_end_code;
   bit                        market_bit;
   
   bit[13:0]                  horizontal_size,vertical_size;
   bit[29:0]                  bit_rate;
   bit[17:0]                  vbv_buffer_size;
   
//   
   new(){
      seq_header              =new;
      seq_ext                 =new;
      user_data_0             =new;
      user_data_1             =new;
      user_data_2             =new;
      seq_display_ext         =new;
      seq_scal_ext            =new;
      group_of_pic_header     =new;
      pic_header              =new;
      pic_coding_ext          =new;
      quant_matrix_ext        =new;
      copyright_ext           =new;
      pic_display_ext         =new;
      pic_spat_scal_ext       =new;
      pic_temp_scal_ext       =new;
//      pic_data                =new;
      slice                   =new;
      mb                      =new;


   }
   void default_set(){
     printf("default parameter configure is generating\n");
     group_of_pic_header.grp_num=d_group_num;
     pic_header.pic_num=d_picture_num;
//     slice.slice_num=d_slice_num;
//     mb.mb_num      =d_mb_num;

     seq_header         .default_set();  
     seq_ext            .default_set();
     user_data_0        .default_set();
     user_data_1        .default_set();
     user_data_2        .default_set();           
     seq_display_ext    .default_set();       
     seq_scal_ext       .default_set();       
         
     group_of_pic_header.default_set();       
     pic_header         .default_set();       
     pic_coding_ext     .default_set();       
     quant_matrix_ext   .default_set();
     copyright_ext      .default_set(); 
     pic_display_ext    .default_set();       
     pic_spat_scal_ext  .default_set();       
     pic_temp_scal_ext  .default_set();       
//     pic_data           .default_set(); 
     slice              .default_set();              
     mb                 .default_set();

     seq_end_code       =d_sequence_end_code;
     market_bit         =d_market_bit;     
     user_data_0.load   =d_user_data_0_load;
     user_data_0.len    =d_user_data_0_len;  
     user_data_1.load   =d_user_data_1_load;
     user_data_2.load   =d_user_data_2_load;
     
        
   } //default_set()
   void para_check(){
      printf("The parameter check start\n");
      multi_para_check();
      seq_header         .para_check(); 
      seq_ext            .para_check();
      user_data_0        .para_check();
      user_data_1        .para_check();
      user_data_2        .para_check();
      seq_display_ext    .para_check(); 
      seq_scal_ext       .para_check(); 
      group_of_pic_header.para_check(); 
      pic_header         .para_check(); 
      pic_coding_ext     .para_check(); 
      quant_matrix_ext   .para_check();
      copyright_ext      .para_check();
      pic_display_ext    .para_check(); 
      pic_spat_scal_ext  .para_check(); 
      pic_temp_scal_ext  .para_check(); 
//      pic_data           .para_check();
      slice              .para_check();
      mb                 .para_check();
   }
   void multi_para_check(){
      
      string s;
      para_calculate();
      s="horizontal_size";                                                              
      ck_zero(horizontal_size,s);                     
      s="vertical_size";                                                                
      ck_zero(vertical_size,s);           
      s="bit_rate";
      ck_zero(bit_rate,s);                                                
      s="vbv_buffer_size";                                                               
      ck_zero(vbv_buffer_size,s);                                               



   } //multi.para_check()
   
   void para_calculate(){
//      int h=0;
      h=seq_ext.horizontal_size_extension<<12;
      horizontal_size=seq_header.horizontal_size_value+h;
      h=seq_ext.vertical_size_extension<<12;
      vertical_size=seq_header.vertical_size_value+h;
      h=seq_ext.bit_rate_extension<<18;
      bit_rate=seq_header.bit_rate_value+h;
      h=seq_ext.vbv_buffer_size_extension<<10;    
      vbv_buffer_size=seq_header.vbv_buffer_size_value+h;
   } //para_calculate()
}  //end of each_sequence
     
class class_seq_header {
   bit[31:0]         sequence_header_code;
   bit[11:0]         horizontal_size_value;
   bit[11:0]         vertical_size_value;
   bit[3:0]          aspect_ratio_information;
   bit[3:0]          frame_rate_code;
   bit[17:0]         bit_rate_value;
   bit               marker_bit;
   bit[9:0]          vbv_buffer_size_value;
   bit               constrained_parameters_flag;
   bit               load_intra_quantiser_matrix;
   bit               load_non_intra_quantiser_matrix;
   bit[7:0]          intra_quantiser_matrix[64]; 
   bit[7:0]          non_intra_quantiser_matrix[64];
   void default_set(){
     bit[7:0] d_matrix[64]; 
//     int i;               
     sequence_header_code            =d_sequence_header_code;       
     horizontal_size_value           =d_horizontal_size_value ;      
     vertical_size_value             =d_vertical_size_value;         
     aspect_ratio_information        =d_aspect_ratio_information;    
     frame_rate_code                 =d_frame_rate_code;             
     bit_rate_value                  =d_bit_rate_value;              
             
     vbv_buffer_size_value           =d_vbv_buffer_size_value;       
     constrained_parameters_flag     =d_constrained_parameters_flag; 
     load_intra_quantiser_matrix     =d_load_intra_quantiser_matrix; 
     load_non_intra_quantiser_matrix =d_load_non_intra_quantiser_matrix; 
                                
     for (i=0;i<64;i++){   
       d_matrix[i]=i+1;     
     }       
                             
     intra_quantiser_matrix          =d_matrix;   
     non_intra_quantiser_matrix      =d_matrix;
     
   }//default()
   void para_check(){
      string s,trace_msg;
                                                                 
      ck_seq_head_code(sequence_header_code);                                           
      ck_aspect_ratio_infor(u_aspect_ratio_information);                               
      ck_frame_rate_code(u_frame_rate_code);                                           
//      ck_bit_rate(u_bit_rate);                                                         
      s="constrained_parameters_flag";
      ck_one(constrained_parameters_flag,s);
      if (load_intra_quantiser_matrix==1) {
         s="intra_quantiser_matrix";
         ck_matrix(intra_quantiser_matrix,s);
      }
      if (load_non_intra_quantiser_matrix==1) {
         s="non_intra_quantiser_matrix";
         ck_matrix(non_intra_quantiser_matrix,s);
      }
   } //para_check()    
                                                                              
   void ck_seq_head_code(bit[31:0] head_code){                                                                                
      if (head_code!=32'h000001b3){                                                    
         trace_msg=sprintf("sequence_header_code=%h\n",head_code);                     
         trace(jvc_ig,jvc_B1LAB_ERROR_ONLY,trace_msg);                                 
      }                                                                                
   } //ck_seq_head_code                                                                
                                                                                       
   void ck_aspect_ratio_infor(bit[3:0] aspect_ratio){
      s="aspect_ratio_information";                                               
      if(aspect_ratio==0) ck_zero(aspect_ratio_information,s);                    
      if(aspect_ratio>3) {                                                                   
         trace_msg="aspect_ratio_information";                                      
         reserved_rep(trace_msg);                                                   
      } //aspect_ratio>3                                                                                              
   } //ck_aspect_ratio_infor                                                           
                                                                                       
   void ck_frame_rate_code(bit[3:0] frame_rate){                                         
      s="frame_rate_code";                                                                                 
      if(frame_rate==0) ck_zero(frame_rate_code,s);                               
      if(frame_rate>4'b1001) {                                                         
         trace_msg="frame_rate_code";                                                  
         reserved_rep(trace_msg);                                                      
      } // frame_rate>4'b1001                                                                                                             
   } //ck_frame_rate_code   
                                                                            
   
    
   
} //end of class_seq_header 

class class_seq_ext {
   bit[31:0]       extension_start_code;
   bit[3:0]        extension_start_code_identifier;
   bit[7:0]        profile_and_level_indication;
   bit             progressive_sequence;
   bit[1:0]        chroma_format;
   bit[1:0]        horizontal_size_extension;
   bit[1:0]        vertical_size_extension;
   bit[12:0]       bit_rate_extension;
   bit             marker_bit;   
   bit[7:0]        vbv_buffer_size_extension;
   bit             low_delay;
   bit[1:0]        frame_rate_extension_n;
   bit[4:0]        frame_rate_extension_d;
   
   void default_set(){
      extension_start_code=d_extension_start_code;
      
      extension_start_code           =d_extension_start_code;        
      extension_start_code_identifier=d_sequence_extension_id;
      profile_and_level_indication   =d_profile_and_level_indication;  
      progressive_sequence           =d_progressive_sequence;         
      chroma_format                  =d_chroma_format;                
      horizontal_size_extension      =d_horizontal_size_extension;      
      vertical_size_extension        =d_vertical_size_extension;        
      bit_rate_extension             =d_bit_rate_extension;             
      vbv_buffer_size_extension      =d_vbv_buffer_size_extension;      
      low_delay                      =d_low_delay;                      
      frame_rate_extension_n         =d_frame_rate_extension_n;         
      frame_rate_extension_d         =d_frame_rate_extension_d;         
   }
   void para_check(){
     // ck_profile_level(profile_and_level_indication);
   
   } //para_check()  
   void ck_profile_level(int a){
      string profile,level,s;
      int m=0,n=0;
      m=8'b11110000&a;
      n=8'b00001111&a;
      case(m){
         8'b01010000:profile="Simple";           
         8'b01000000:profile="Main";             
         8'b00110000:profile="SNR Scalable";     
         8'b00100000:profile="Spatial Scalable"; 
         8'b00010000:profile="High";             
         default:{                               
            s="MPEG2 profile";                   
            reserved_rep(s);                     
//         printf("profile is reserved type\n");   
         }                                       
      } //case
      case(n){
         8'b00001010:level="Low";             
         8'b00001000:level="Main";            
         8'b00000110:level="High 1440";       
         8'b00001000:level="Main";            
         8'b00000100:level="High";            
         default:{                            
            s="MPEG2 level";                  
            reserved_rep(s);                  
         }                                    
         //printf("level is reserved type\n");   
      } //case 
      printf("************** sequence stream generation starts ************\n");                          
      printf("**************   profile=%s   level=%s   ************\n",profile,level);
      printf("**************  current group number is %d  current picture number is %d     ************\n"
         ,group_num,picture_num);
      printf("**************  current slice number is %d  current macroblock number is %d  ************\n"
         ,u_slice_num,u_mb_num);
            
   } //report_profile_level()     
}  //end of class_seq_ext
   
class class_seq_display_ext {
   bit             load;
//   bit[31:0]       extension_start_code;   
   bit[3:0]        extension_start_code_identifier;
   bit[2:0]        video_format;
   bit             colour_description;
   bit[7:0]        colour_primaries,transfer_characteristics,matrix_coefficients;
   bit[13:0]       display_horizontal_size;
   bit             marker_bit;   
   bit[13:0]       display_vertical_size;
   
   void default_set(){
      load                            =d_seq_display_ext_load;
      extension_start_code_identifier =d_sequence_display_extension_id;                              
      video_format                    =d_video_format;                                                 
      colour_description              =d_colour_description;                                           
      colour_primaries                =d_colour_primaries;
      transfer_characteristics        =d_transfer_characteristics;
      matrix_coefficients             =d_matrix_coefficients;
      display_horizontal_size         =d_display_horizontal_size;                                                                                     
      display_vertical_size           =d_display_vertical_size;  
   } //default_set()
   void para_check(){
   
   
   } //para_check()  
   
} //end of class_seq_display_ext 

class class_seq_scal_ext {
   bit       load;
   bit[3:0]  extension_start_code_identifier;
   bit[1:0]  scalable_mode;
   bit[3:0]  layer_id;
  
   bit[13:0] lower_layer_prediction_horizontal_size;
   bit[13:0] lower_layer_prediction_vertical_size;
   bit[4:0]  horizontal_subsampling_factor_m,horizontal_subsampling_factor_n;
   bit[4:0]  vertical_subsampling_factor_m,vertical_subsampling_factor_n;
   bit       temp_scal_flag;     
   bit       picture_mux_enable; 
   bit       mux_to_progressive_sequence;   
   bit[2:0]  picture_mux_order,picture_mux_factor;
   
   void default_set(){
      load                                  =d_seq_scal_ext_load;
      extension_start_code_identifier       =d_sequence_scalable_extension_id;                        
      scalable_mode                         =d_scalable_mode;                                                 
      layer_id                              =d_layer_id;                                                      
      lower_layer_prediction_horizontal_size=d_lower_layer_prediction_horizontal_size;
      lower_layer_prediction_vertical_size  =d_lower_layer_prediction_vertical_size;                        
      horizontal_subsampling_factor_m       =d_horizontal_subsampling_factor_m;
      horizontal_subsampling_factor_n       =d_horizontal_subsampling_factor_n;
      vertical_subsampling_factor_m         =d_vertical_subsampling_factor_m;
      vertical_subsampling_factor_n         =d_vertical_subsampling_factor_n;   
      picture_mux_enable                    =d_picture_mux_enable;                                            
      mux_to_progressive_sequence           =d_mux_to_progressive_sequence;                                   
      picture_mux_order                     =d_picture_mux_order;
      picture_mux_factor                    =d_picture_mux_factor;                          
   } 
   
   void para_check(){
   
   
   } //para_check()  
}  //end of class_seq_scal_ext


class class_group_of_pic_header {
   bit[31:0]      group_start_code;
   bit[24:0]      time_code;
   bit            closed_gop[];
   bit            broken_link[];
   
   bit[24:0]      l_time_code[];
   int            grp_num;  //number of group of picture in each sequence
      

   void default_set(){
//      int i;
      group_start_code      =d_group_start_code;
      for (i=0;i<max_group_num;i++){
      l_time_code[i]=d_picture_num*i+d_time_code;
      closed_gop[i] =d_closed_gop;
      broken_link[i]=d_broken_link;
//      printf("broken_link[%d]=%d\n",i,broken_link[i]);
//      printf("closed_gop[%d]=%d\n",i,closed_gop[i]);
//      printf("l_time_code[%d]=%d\n",i,l_time_code[i]);
//      printf("l_time_code[%d]=%h\n",i,l_time_code[i]);  //can't displayd
//      printf("l_time_code[%d]=%b\n",i,l_time_code[i]);  //can't display
      }  //for

   }
   void para_check(){
   
   
   } //para_check()  
}  //end of class_group_of_pic_header

class class_user_data {
   bit         load;
   bit[31:0]   user_data_start_code;
   bit[7:0]    data[];
   int         len;
   
   void default_set(){
      int i=0;
      len=d_user_data_0_len;
      user_data_start_code=d_user_data_start_code;     
      
      for (i=0;i<len;i++){      
         data[i]=i+100;      
      }
   }
   void para_check(){
     //0 can't be concatenation over 24 times in data.
   
   } //para_check()     
}  //class_user_data_0



class class_pic_header {
   int             pic_num;   //number of picture in each group of picture
   bit[31:0]       picture_start_code;
   bit[9:0]        temporal_reference[max_group_num][max_picture_num];
   bit[2:0]        picture_coding_type[max_group_num][max_picture_num];
   bit[15:0]       vbv_delay[max_group_num][max_picture_num];
   bit             full_pel_forward_vector[max_group_num][max_picture_num];
   bit             full_pel_backward_vector[max_group_num][max_picture_num];
   bit[2:0]        forward_f_code[max_group_num][max_picture_num];
   bit[2:0]        backward_f_code[max_group_num][max_picture_num];
   bit             extra_bit_picture[max_group_num][max_picture_num];
   bit[7:0]        extra_information_picture[max_group_num][max_picture_num];
   
   void default_set(){
//      int i=0,h=0;
      picture_start_code        =d_picture_start_code;
      
      
      for (i=0;i<max_group_num;i++){
        for (h=0;h<max_picture_num;h++){
           temporal_reference[i][h]=16*i*h+d_temporal_reference;
           picture_coding_type[i][h]=d_picture_coding_type;
           vbv_delay[i][h]=d_vbv_delay;
           full_pel_forward_vector[i][h]=d_full_pel_forward_vector;
           full_pel_backward_vector[i][h]=d_full_pel_backward_vector;
           forward_f_code[i][h]=d_forward_f_code;
           backward_f_code[i][h]=d_backward_f_code;
           extra_bit_picture[i][h]=d_extra_bit_picture;
           extra_information_picture[i][h]=d_extra_information_picture;
//           printf("temporal_reference[%d][%d]=%d\n",i,h,temporal_reference[i][h]);
        } //for
      }  //for
    }
           
   void para_check(){
   
   
   } //para_check()  
     
}  //class_pic_header

class class_pic_coding_ext {
   bit[31:0]       extension_start_code;
   bit[3:0]        extension_start_code_identifier;
   bit[3:0]        f_code00[max_group_num][max_picture_num];
   bit[3:0]        f_code01[max_group_num][max_picture_num];
   bit[3:0]        f_code10[max_group_num][max_picture_num];
   bit[3:0]        f_code11[max_group_num][max_picture_num];
   bit[1:0]        intra_dc_precision[max_group_num][max_picture_num];
   bit[1:0]        picture_structure[max_group_num][max_picture_num];
   bit             top_field_first[max_group_num][max_picture_num];
   bit             frame_pred_frame_dct[max_group_num][max_picture_num];
   bit             concealment_motion_vectors[max_group_num][max_picture_num];
   bit             q_scale_type[max_group_num][max_picture_num];
   bit             intra_vlc_format[max_group_num][max_picture_num];
   bit             alternate_scan[max_group_num][max_picture_num];
   bit             repeat_first_field[max_group_num][max_picture_num];
   bit             chroma_420_type[max_group_num][max_picture_num];
   bit             progressive_frame[max_group_num][max_picture_num];
   bit             composite_display_flag[max_group_num][max_picture_num];
   bit             v_axis[max_group_num][max_picture_num];
   bit             sub_carrier[max_group_num][max_picture_num];
   bit[2:0]        field_sequence[max_group_num][max_picture_num];
   bit[6:0]        burst_amplitude[max_group_num][max_picture_num];
   bit[7:0]        sub_carrier_phase[max_group_num][max_picture_num];   

   void default_set(){
//      int i=0,h=0;
      extension_start_code           =d_extension_start_code;                                           
      extension_start_code_identifier=d_picture_coding_extension_id;
      
      for (i=0;i<max_group_num;i++){
        for (h=0;h<max_picture_num;h++){
           
           f_code00[i][h]=d_f_code00;
           f_code01[i][h]=d_f_code01;
           f_code10[i][h]=d_f_code10;
           f_code11[i][h]=d_f_code11;
           intra_dc_precision[i][h]=d_intra_dc_precision;
           picture_structure[i][h]=d_picture_structure;
           top_field_first[i][h]=d_top_field_first;
           frame_pred_frame_dct[i][h]=d_frame_pred_frame_dct;
           concealment_motion_vectors[i][h]=d_concealment_motion_vectors;
           q_scale_type[i][h]=d_q_scale_type;
           intra_vlc_format[i][h]=d_intra_vlc_format;
           alternate_scan[i][h]=d_alternate_scan;
           repeat_first_field[i][h]=d_repeat_first_field;
           chroma_420_type[i][h]=d_chroma_420_type;
           progressive_frame[i][h]=d_progressive_frame;
           composite_display_flag[i][h]=d_composite_display_flag;
           
           v_axis[i][h]=d_v_axis;
           sub_carrier[i][h]=d_sub_carrier;
           field_sequence[i][h]=d_field_sequence;
           burst_amplitude[i][h]=d_burst_amplitude;
           sub_carrier_phase[i][h]=d_sub_carrier_phase;
           
//           printf("temporal_reference[%d][%d]=%d\n",i,h,temporal_reference[i][h]);
        } //for
      }  //for
                                        
   }
   void para_check(){
   
   
   } //para_check()     
   
   

}  //class_pic_coding_ext

class class_quant_matrix_ext {
   bit       load;
   
   bit[3:0]  extension_start_code_identifier;
   bit       load_intra_quantiser_matrix[max_group_num][max_picture_num];
   bit       load_non_intra_quantiser_matrix[max_group_num][max_picture_num];
   bit[7:0]  intra_quantiser_matrix[max_group_num][max_picture_num][64];
   bit[7:0]  non_intra_quantiser_matrix[max_group_num][max_picture_num][64];
   bit[7:0]  chroma_intra_quantiser_matrix[max_group_num][max_picture_num][64];
   bit[7:0]  chroma_non_intra_quantiser_matrix[max_group_num][max_picture_num][64];
   bit       load_chroma_intra_quantiser_matrix[max_group_num][max_picture_num];
   bit       load_chroma_non_intra_quantiser_matrix[max_group_num][max_picture_num];

   void default_set(){
      int t=0;
      load                                  =d_quant_matrix_ext_load;      
      extension_start_code_identifier       =d_quant_matrix_extension_id;                                             
      
      for (i=0;i<max_group_num;i++){
        for (h=0;h<max_picture_num;h++){
           load_intra_quantiser_matrix[i][h]=d_load_intra_quantiser_matrix;
           load_non_intra_quantiser_matrix[i][h]=d_load_non_intra_quantiser_matrix;
           load_chroma_intra_quantiser_matrix[i][h]=d_load_chroma_intra_quantiser_matrix;
           load_chroma_non_intra_quantiser_matrix[i][h]=d_load_chroma_non_intra_quantiser_matrix;
           for (t=0;t<64;t++){
              intra_quantiser_matrix[i][h][t]=t+200;
              non_intra_quantiser_matrix[i][h][t]=t+300;
              chroma_intra_quantiser_matrix[i][h][t]=t+400;
              chroma_non_intra_quantiser_matrix[i][h][t]=t+500;          
           } //for
               
//           printf("chroma_intra_quantiser_matrix[%d][%d][%d]=%d\n",i,h,t,chroma_intra_quantiser_matrix[i][h][t]);
        } //for
      }  //for
    
   }
   void para_check(){
   
   
   } //para_check()     

}  //class_quant_matrix_ext

class class_copyright_ext {
   bit           load;
   bit[3:0]      extension_start_code_identifier;
   bit           copyright_flag[max_group_num][max_picture_num];
   bit           original_or_copy[max_group_num][max_picture_num];
   bit           marker_bit;
   bit[7:0]      copyright_identifier[max_group_num][max_picture_num];
   bit[6:0]      reserved;
   bit[19:0]     copyright_number_1[max_group_num][max_picture_num];
   bit[21:0]     copyright_number_2[max_group_num][max_picture_num];
   bit[21:0]     copyright_number_3[max_group_num][max_picture_num];
   
   void default_set(){
      load                           =d_copyright_ext_load;
      extension_start_code_identifier=d_copyright_extension_id;
      reserved                       =7'b0000000; 
      for (i=0;i<max_group_num;i++){
        for (h=0;h<max_picture_num;h++){           
           copyright_flag[i][h]         =d_copyright_flag;       
           original_or_copy[i][h]       =d_original_or_copy;     
           copyright_identifier[i][h]   =d_copyright_identifier;                                             
           copyright_number_1[i][h]     =d_copyright_number_1;                   
           copyright_number_2[i][h]     =d_copyright_number_2;   
           copyright_number_3[i][h]     =d_copyright_number_3;   
        } //for
      }  //for      
               
   }
   
   void para_check(){
   
   
   } //para_check()  
}  //class_copyright_ext

class class_pic_display_ext {
   bit           load;
   bit[3:0]      extension_start_code_identifier;
   bit[15:0]     frame_centre_horizontal_offset[max_group_num][max_picture_num];
   bit[15:0]     frame_centre_vertical_offset[max_group_num][max_picture_num];
   bit           marker_bit;
//   int           number_of_frame_centre_offsets;
   void default_set(){
      load                           =d_pic_display_ext_load;
      extension_start_code_identifier=d_picture_display_extension_id; 
//      number_of_frame_centre_offsets =d_number_of_frame_centre_offsets;
      for (i=0;i<max_group_num;i++){          
        for (h=0;h<max_picture_num;h++){                                       
           frame_centre_horizontal_offset[i][h]=d_frame_centre_horizontal_offset;
           frame_centre_vertical_offset[i][h]  =d_frame_centre_vertical_offset;         
        } //for        
      }  //for                                                         
   } 
          
   void para_check(){
   
   
   } //para_check()  
   
   int gen_number_of_frame_centre_offsets(bit a,bit b,bit c,bit[1:0] d){
      if (a){
         if (b) {
            if (c) {
               return 3;
            }
            else {
               return 2;
            }            
         }
         else {
            return 1;                  
         }
      }
      else {
         if ((d==1)||(d==2)) {
            return 1;
         }
         else {
            if (b) {
               return 3;
            } else {
               return 2;
            }
         }      
      }
   } //gen_number_of_frame_centre_offsets()
}  //class_pic_display_ext

class class_pic_spat_scal_ext {
   bit       load;
   bit[3:0]  extension_start_code_identifier;
   bit[9:0]  lower_layer_temporal_reference[max_group_num][max_picture_num];
   bit       marker_bit;
   bit[14:0] lower_layer_horizontal_offset[max_group_num][max_picture_num];
   bit[14:0] lower_layer_vertical_offset[max_group_num][max_picture_num];
   bit[1:0]  spatial_temporal_weight_code_table_index[max_group_num][max_picture_num];
   bit       lower_layer_progressive_frame[max_group_num][max_picture_num];
   bit       lower_layer_deinterlaced_field_select[max_group_num][max_picture_num];


//bit[31:0] slice_start_code[max_group_num][max_picture_num];
   void default_set(){
      load                                    =d_pic_spat_scal_ext_load; 
      extension_start_code_identifier         =d_picture_spatial_scalable_extension_id; 
      
      for (i=0;i<max_group_num;i++){                                             
        for (h=0;h<max_picture_num;h++){                                         
           lower_layer_temporal_reference[i][h] =d_lower_layer_temporal_reference;           
           lower_layer_horizontal_offset[i][h]  =d_lower_layer_horizontal_offset;            
           lower_layer_vertical_offset[i][h]    =d_lower_layer_vertical_offset;              
           spatial_temporal_weight_code_table_index[i][h]=d_spatial_temporal_weight_code_table_index; 
           lower_layer_progressive_frame[i][h]        =d_lower_layer_progressive_frame;            
           lower_layer_deinterlaced_field_select[i][h]=d_lower_layer_deinterlaced_field_select; 
//slice_start_code[i][h]=d_slice_start_code;
   
        } //for                                                                  
      }  //for                                                                      
   }
   
   void para_check(){
   
   
   } //para_check()  
}  //class_pic_spat_scal_ext

class class_pic_temp_scal_ext {
   bit             load;
   bit[3:0]        extension_start_code_identifier;
   bit[1:0]        reference_select_code[max_group_num][max_picture_num];
   bit[9:0]        forward_temporal_reference[max_group_num][max_picture_num];
   bit[9:0]        backward_temporal_reference[max_group_num][max_picture_num];
   bit             marker_bit;
   void default_set(){
      load                           =d_pic_temp_scal_ext_load;
      extension_start_code_identifier=d_picture_temporal_scalable_extension_id;
      
      for (i=0;i<max_group_num;i++){                                                           
        for (h=0;h<max_picture_num;h++){                                                       
           reference_select_code[i][h]         =d_reference_select_code;      
           forward_temporal_reference[i][h]    =d_forward_temporal_reference; 
           backward_temporal_reference[i][h]   =d_backward_temporal_reference;          
        } //for                                                                                
      }  //for                                                                                                        
                            
   }
   void para_check(){
   
   
   } //para_check()  
}  //class_pic_temp_scal_ext



class class_slice {
   bit[31:0]  slice_num[max_picture_num];    //[1:175] 

   bit[31:0]  slice_vertical_position<>;
   bit[31:0]  slice_start_code[max_picture_num][max_slice_num];
   bit[2:0]   slice_vertical_position_extension[max_picture_num][max_slice_num];
   bit[6:0]   priority_breakpoint[max_picture_num][max_slice_num];
   bit        intra_slice_flag[max_picture_num][max_slice_num];
   bit        intra_slice[max_picture_num][max_slice_num];
   bit[6:0]   reserved_bit[max_picture_num][max_slice_num];
   bit        extra_bit_slice[max_picture_num][max_slice_num];
   bit[7:0]   extra_information_slice[max_picture_num][max_slice_num];
   bit        extra_bit_slice_0[max_picture_num][max_slice_num];
   
//   bit[31:0]  len[max_picture_num][max_slice_num];    //[1:175]      

   
   void default_set(){ 
      bit[31:0] t=0;
     int h=0;
   for(i=0;i<max_picture_num;i++){
      slice_num[i]=d_slice_num;      
   }
   
   } //default_set() 
   void slice_vertical_position_config(){
      int l=0;
//      l=slice_vertical_position.num_element();
      

   } //slice_vertical_position_config()                 
   void para_check(){  
                       
                       
   } //para_check()    
                       

} //class_slice


class class_mb {
     
   int       mb_num[max_picture_num][max_slice_num];
   class_table table;


      bit[31:0] len[max_picture_num][max_slice_num][max_mb_num];
   bit[31:0] macroblock_escape_num[max_picture_num][max_slice_num][max_mb_num];  //>=0
   bit[4:0]  quantiser_scale_code[max_picture_num][max_slice_num][max_mb_num]; 
   
   bit[10:0]  mb_addr_increment[max_picture_num][max_slice_num][max_mb_num];  //[1:33]
  
   
 //  bit[5:0]  mb_addr_increment_value[max_picture_num][max_slice_num][max_mb_num];  // 
   
    
   ///////////////macroblock_type///////////////////////////
   bit[8:0]  macroblock_type_index[max_picture_num][max_slice_num][max_mb_num];
   bit[1:0]  spatial_temporal_weight_code[max_picture_num][max_slice_num][max_mb_num];
   bit[1:0]  frame_motion_type[max_picture_num][max_slice_num][max_mb_num];
   bit[1:0]  field_motion_type[max_picture_num][max_slice_num][max_mb_num];
   bit       dct_type[max_picture_num][max_slice_num][max_mb_num];
   
   ////////////////Table_B//////////////////////////////////////////////////////
   bit[31:0] vlc_len;
   int       vlc_value,vlc_value_2,vlc_level;
   bit       macroblock_quant,macroblock_motion_forward,macroblock_motion_backward;
   bit       macroblock_pattern,macroblock_intra,spatial_temporal_weight_code_flag;
  
   
   void default_set(){ 
   for(i=0;i<max_picture_num;i++){
      for (h=0;i<max_slice_num;i++){
      mb_num[i][h]=d_mb_num;
      }
            
   }

   } //default_set() 
   int gen_macroblock_escape_num(int pre_ad,int now_ad){
      int a=0;
      a=(now_ad-pre_ad)/33;
      return a;
   
   } // gen_macroblock_escape_num() 

   
             
   void para_check(){  
                       
                       
   } //para_check()    
} //class_marcroblock










//------------------------------------------------------------------------------
//function
void ck_zero(int a,string s){
   if (a==0){
      trace_msg=sprintf("%s=%h\n",s,0); 
      trace(jvc_ig,jvc_B1LAB_ERROR_ONLY,trace_msg);             
   }  
} //ck_zero()

void ck_one(int a,string s){                       
   if (a==1){                                       
      trace_msg=sprintf("%s=%h\n",s,1);             
      trace(jvc_ig,jvc_B1LAB_ERROR_ONLY,trace_msg); 
   }                                                
} //ck_one()                                       
                                                    

void reserved_rep(string s){
   trace_msg=sprintf("%s is reserved type",s);
   trace(jvc_ig,jvc_B1LAB_WARNING,trace_msg);

} //reserved_rep

void ck_matrix(bit[7:0] a[64],string s){
   int i;
   for (i=0;i<=64;i++){
      if (a[i]==0){
         trace_msg=sprintf("%s have the zero item index=%h\n",s,i);
         trace(jvc_ig,jvc_B1LAB_ERROR_ONLY,trace_msg);
      }
   }

} //ck_matrix()
 
int gen_random(int a,int b){   //gen a value between a and b(include a and b)[a,b]
   int t=0;
   if (a==b) {
      printf("gen_random() the input first value is equal to the last value\n");
      return -1;
   }
   if (a>b) t=a-b+1;
   else t=b-a+1;
   
   t=random()%t;
   t=t+a;
   printf("random =%d ,between %d and %d \n",t,a,b);
   return t;
} //gen_random(int a,int b) 


              


