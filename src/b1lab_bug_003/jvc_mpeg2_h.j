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




class video_sequence{

   int l_each_sequence<>;   //????????????????
}
class each_sequence{
   class_seq_header           seq_header;
   class_seq_ext              seq_ext; 
   class_user_data            user_data_0,user_data_1,user_data_2;

   int                        seq_display_ext_flag,seq_scal_ext_flag;
   class_seq_display_ext      seq_display_ext;
   class_seq_scal_ext         seq_scal_ext;
   class_group_of_pic_header  group_of_pic_header;
   class_pic_header           pic_header;
   class_pic_coding_ext       pic_coding_ext;
   int                        quant_matrix_ext_flag,copyright_ext_flag;
   class_quant_matrix_ext     quant_matrix_ext;
   class_copyright_ext        copyright_ext;
   int                        pic_display_ext_flag;
   class_pic_display_ext      pic_display_ext;
   class_pic_spat_scal_ext    pic_spat_scal_ext;
   int                        pic_temp_scal_ext_flag,pic_spat_scal_ext_flag;
   class_pic_temp_scal_ext    pic_temp_scal_ext;
   class_pic_data             pic_data;
   int                        seq_end_code;
   bit                        market_bit;
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
      pic_display_ext         =new;
      pic_spat_scal_ext       =new;
      pic_temp_scal_ext       =new;
      pic_data                =new;
//      seq_display_ext_flag    =1;

   }
   void default_set(){


         
     group_of_pic_header.default_set();       
         
     
   } //default_set()
   void para_check(){
       group_of_pic_header.para_check(); 
    }
   void multi_para_check(){
      string s;
      s="horizontal_size";                                                              
      ck_zero(seq_header.horizontal_size_value+seq_ext.horizontal_size_extension,s);                     
      s="vertical_size";                                                                
      ck_zero(seq_header.vertical_size_value+seq_ext.vertical_size_extension,s);           
      s="bit_rate";
      ck_zero(seq_header.bit_rate_value+seq_ext.bit_rate_extension,s);                                                
      s="vbv_buffer_size";                                                               
      ck_zero(seq_header.vbv_buffer_size_value+seq_ext.vbv_buffer_size_extension,s);                                               



   } //multi.para_check()
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
     int i;               
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
                                
      printf("**************   profile=%s   level=%s   ************\n",profile,level);
      
   } //report_profile_level()     
}  //end of class_seq_ext
   
class class_seq_display_ext {
   bit             load;
//   bit[31:0]       extension_start_code;   
   bit[4:0]        extension_start_code_identifier;
   bit[2:0]        video_format;
   bit             colour_description;
   bit[7:0]        colour_primaries,transfer_characteristics,matrix_coefficients;
   bit[13:0]       display_horizontal_size;
   bit             marker_bit;   
   bit[13:0]       display_vertical_size;
   
   void default_set(){
 
   } //default_set()
   void para_check(){
   
   
   } //para_check()  
  
} //end of class_seq_display_ext 

class class_seq_scal_ext {
   bit       load;
   bit[4:0]  extension_start_code_identifier;
   bit[1:0]  scalable_mode;
   bit[3:0]  layer_id;
//   bit       spat_scal_flag;   
   bit[13:0] lower_layer_prediction_horizontal_size;
   bit[13:0] lower_layer_prediction_vertical_size;
   bit[4:0]  horizontal_subsampling_factor_m,horizontal_subsampling_factor_n;
   bit[4:0]  vertical_subsampling_factor_m,vertical_subsampling_factor_n;
   bit       temp_scal_flag;     
   bit       picture_mux_enable; 
   bit       mux_to_progressive_sequence;   
   bit[2:0]  picture_mux_order,picture_mux_factor;
   
   void default_set(){
     } 
   
   void para_check(){
   
   
   } //para_check()  
}  //end of class_seq_scal_ext


class class_group_of_pic_header {
   bit[31:0]      group_start_code;
   bit[24:0]      time_code;
   bit            closed_gop,broken_link;
   
   bit[24:0]      l_time_code[];
   int            grp_num;
   void default_set(){
      int i;
      group_start_code      =d_group_start_code;      
      time_code             =d_time_code;             
      closed_gop            =d_closed_gop;
      broken_link           =d_broken_link;
      grp_num=d_group_num;
      l_time_code_config();
   }
   void l_time_code_config(){
      for (i=0;i<grp_num;i++){
//      printf("grp_num=%d\n",grp_num);
      l_time_code[i]=d_picture_num*i+d_time_code;
      // breakpoint ;
      printf("l_time_code[%d]=%d\n",i,l_time_code[i]);
      printf("l_time_code[%d]=%h\n",i,l_time_code[i]);  //can't display
      printf("l_time_code[%d]=%b\n",i,l_time_code[i]);  //can't display

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
     }
   void para_check(){
     //0 can't be concatenation over 24 times in data.
   
   } //para_check()     
}  //class_user_data_0



class class_pic_header {
   bit[31:0]       picture_start_code;
   bit[9:0]        temporal_reference;
   bit[2:0]        picture_coding_type;
   bit[15:0]       vbv_delay;
   bit             full_pel_forward_vector,full_pel_backward_vector;
   bit[2:0]        forward_f_code,backward_f_code;
   bit             extra_bit_picture;
   bit[7:0]        extra_information_picture;
   void default_set(){
      }
   void para_check(){
   
   
   } //para_check()  
     
}  //class_pic_header

class class_pic_coding_ext {
   bit[31:0]       extension_start_code;
   bit[3:0]        extension_start_code_identifier;
   bit[3:0]        f_code00,f_code01,f_code10,f_code11;
   bit[1:0]        intra_dc_precision,picture_structure;
   bit             top_field_first,frame_pred_frame_dct,concealment_motion_vectors,
                   q_scale_type,intra_vlc_format,alternate_scan,repeat_first_field;
   bit             chroma_420_type,progressive_frame,composite_display_flag;
   bit             v_axis,sub_carrier;
   bit[2:0]        field_sequence;
   bit[6:0]        burst_amplitude;
   bit[7:0]        sub_carrier_phase;   

   void default_set(){
      }
   void para_check(){
   
   
   } //para_check()     
   
   

}  //class_pic_coding_ext

class class_quant_matrix_ext {
   bit[3:0]  extension_start_code_identifier;
   bit       load_intra_quantiser_matrix,load_non_intra_quantiser_matrix;
   bit[7:0]  intra_quantiser_matrix[64],non_intra_quantiser_matrix[64];
   bit[7:0]  chroma_intra_quantiser_matrix[64],chroma_non_intra_quantiser_matrix[64];
   bit       load_chroma_intra_quantiser_matrix;
   bit       load_chroma_non_intra_quantiser_matrix;

   void default_set(){
      
   }
   void para_check(){
   
   
   } //para_check()     

}  //class_quant_matrix_ext

class class_copyright_ext {
   bit[3:0]      extension_start_code_identifier;
   bit           copyright_flag,original_or_copy,marker_bit;
   bit[7:0]      copyright_identifier;
   bit[6:0]      reserved;
   bit[19:0]     copyright_number_1;
   bit[21:0]     copyright_number_2,copyright_number_3;
   
   void default_set(){
   }
   
   void para_check(){
   
   
   } //para_check()  
}  //class_copyright_ext

class class_pic_display_ext {
   bit[3:0]      extension_start_code_identifier;
   bit[15:0]     frame_centre_horizontal_offset,frame_centre_vertical_offset;
   bit           marker_bit;

   void default_set(){
   }        
   void para_check(){
   
   
   } //para_check()  
}  //class_pic_display_ext

class class_pic_spat_scal_ext {
   bit[3:0]  extension_start_code_identifier;
   bit[9:0]  lower_layer_temporal_reference;
   bit       marker_bit;
   bit[14:0] lower_layer_horizontal_offset,lower_layer_vertical_offset;
   bit[1:0]  spatial_temporal_weight_code_table_index;
   bit       lower_layer_progressive_frame,lower_layer_deinterlaced_field_select;

   void default_set(){ 
   }
   
   void para_check(){
   
   
   } //para_check()  
}  //class_pic_spat_scal_ext

class class_pic_temp_scal_ext {
   bit[3:0]        extension_start_code_identifier;
   bit[1:0]        reference_select_code;
   bit[9:0]        forward_temporal_reference,backward_temporal_reference;
   bit             marker_bit;
   void default_set(){
   }
   void para_check(){
   
   
   } //para_check()  
}  //class_pic_temp_scal_ext

class class_pic_data {
   void default_set(){
   
   }
   void para_check(){
   
   
   } //para_check()  
}

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
 
 


              


