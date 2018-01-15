/*                                                                              
--(C)opyright 2003 B1LAB INC.                                                   
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2                                                  
-- Title          :  header file of default file                        
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_default_h.j                                           
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>                              
-- Created        :  2003/03/06                                                 
-- Source         :                                                             
-- Last modified  :                                                             
--------------------------------------------------------------------------------
-- Description :                                                                
--   define the default value for each parameter and abbreviation for instance.                                                
--------------------------------------------------------------------------------
*/
#define d_sequence_num                 1
#define d_group_num                    1
#define d_picture_num                  2

#define max_sequence_num               5    //0 is ignore
#define max_group_num                  5    //0 is ignore
#define max_picture_num                5    //0 is ignore
//#define max_slice_num                  175  //0 is ignore
#define max_slice_num                  5  //0 is ignore

#define max_mb_num                     36   //0 is ignore,in each slice

//seq_end_code
#define d_sequence_end_code            32'h000001b7  //stable  
#define d_market_bit                   1'b1

    
//seq_header
#define d_sequence_header_code         32'h000001b3  //stable
#define d_horizontal_size_value        12'd720 
     
#define d_vertical_size_value          12'd576      
#define d_aspect_ratio_information     4'b0001     
#define d_frame_rate_code              4'b0011      
#define d_bit_rate_value               18'd100      //
#define d_vbv_buffer_size_value        10'd100     //
#define d_constrained_parameters_flag  1'b0   
#define d_load_intra_quantiser_matrix  1'b0 
#define d_load_non_intra_quantiser_matrix 1'b0

//seq_ext 
#define d_extension_start_code         32'h000001b5  //stable
#define d_sequence_extension_id        4'b0001
#define d_profile_and_level_indication 8'b01001000
#define d_progressive_sequence         1'b1
#define d_chroma_format                2'b01
#define d_horizontal_size_extension    2'b00
#define d_vertical_size_extension      2'b00
#define d_bit_rate_extension           12'b010101010101              
#define d_vbv_buffer_size_extension    8'b01010101
#define d_low_delay                    1'b0
#define d_frame_rate_extension_n       2'b00
#define d_frame_rate_extension_d       2'b00






//user_data_0  
#define d_user_data_start_code             32'h000001b2  //stable        
#define d_user_data_0_load                 1'b0
#define d_user_data_0_len                  32'd8    
//user_data_1
#define d_user_data_1_load                 1'b0       
//#define d_user_data_1_len                =d_user_data_0_len   
//user_data_2
#define d_user_data_2_load                 1'b0      
//#define d_user_data_2_len                =d_user_data_0_len    


//seq_display_ext 
#define d_seq_display_ext_load          1'b1 
#define d_sequence_display_extension_id 4'b0010 
#define d_video_format                  3'b101
#define d_colour_description            1'b0
#define d_colour_primaries              8'b00000001 
#define d_transfer_characteristics      8'b00000001
#define d_matrix_coefficients           8'b00000001 
#define d_display_horizontal_size       14'b01010101010101 
#define d_display_vertical_size         14'b01010101010101

//seq_scal_ext

#define d_seq_scal_ext_load                      1'b1
#define d_sequence_scalable_extension_id         4'b0101              
#define d_scalable_mode                          2'b01       
#define d_layer_id                               4'b0001       
//#define d_is_spat_scal                           1'b0       
#define d_lower_layer_prediction_horizontal_size 14'b01010101010101
#define d_lower_layer_prediction_vertical_size   14'b01010101010101
#define d_horizontal_subsampling_factor_m        5'b00001      
#define d_horizontal_subsampling_factor_n        5'b00001      
#define d_vertical_subsampling_factor_m          5'b00001        
#define d_vertical_subsampling_factor_n          5'b00001
//#define d_is_temp_scal                           1'b0    
#define d_picture_mux_enable                     1'b0    
#define d_mux_to_progressive_sequence            1'b0    
#define d_picture_mux_order                      3'b001  
#define d_picture_mux_factor                     3'b001  



//group_of_pic_header
#define d_group_start_code                32'h000001b8
#define d_time_code                       25'h0000010          
#define d_closed_gop                      1'b1
#define d_broken_link                     1'b0

//pic_header
#define d_picture_start_code              32'h00000100
#define d_temporal_reference              10'b0101010101
#define d_picture_coding_type             3'b001 //{001,010,011} only
#define d_vbv_delay                       16'hffff
#define d_full_pel_forward_vector         1'b0
#define d_full_pel_backward_vector        1'b0
#define d_forward_f_code                  3'b111
#define d_backward_f_code                 3'b111
#define d_extra_bit_picture               1'b0
#define d_extra_information_picture       8'h01

//pic_coding_ext
         
#define d_picture_coding_extension_id     4'b1000
#define d_f_code00                        4'b1111                
#define d_f_code01                        4'b1111
#define d_f_code10                        4'b1111
#define d_f_code11                        4'b1111
#define d_intra_dc_precision              2'b00
#define d_picture_structure               2'b01  //00 
#define d_top_field_first                 1'b0     
#define d_frame_pred_frame_dct            1'b1
#define d_concealment_motion_vectors      1'b0
#define d_q_scale_type                    1'b0
#define d_intra_vlc_format                1'b0
#define d_alternate_scan                  1'b0
#define d_repeat_first_field              1'b0
#define d_chroma_420_type                 1'b0
#define d_progressive_frame               1'b0
#define d_composite_display_flag          1'b0
#define d_v_axis                          1'b0
#define d_sub_carrier                     1'b0
#define d_field_sequence                  3'b000
#define d_burst_amplitude                 7'b0000000
#define d_sub_carrier_phase               8'b00000000

 //quant_matrix_ext
#define d_quant_matrix_ext_load           1'b0
#define d_quant_matrix_extension_id       4'b0011                  
#define d_load_intra_quantiser_matrix     1'b0
#define d_load_non_intra_quantiser_matrix 1'b0  
#define d_load_chroma_intra_quantiser_matrix  1'b0  
#define d_load_chroma_non_intra_quantiser_matrix  1'b0 


//copyright_ext
#define d_copyright_ext_load            1'b0
#define d_copyright_extension_id        4'b0100       
#define d_copyright_flag                1'b0
#define d_original_or_copy              1'b0
#define d_copyright_identifier          8'h00        
//#define d_reserved                      7'b0000000            
#define d_copyright_number_1            20'h01010            
#define d_copyright_number_2            20'h01010
#define d_copyright_number_3            20'h01010

//pic_display_ext
#define d_pic_display_ext_load           1'b0
#define d_picture_display_extension_id   4'b0111
#define d_frame_centre_horizontal_offset 16'h000f
#define d_frame_centre_vertical_offset   16'h000f
//#define d_number_of_frame_centre_offsets 1'b1  //0 is forbidden


//pic_spat_scal_ext
#define d_pic_spat_scal_ext_load                1'b0
#define d_picture_spatial_scalable_extension_id 4'b1001         
#define d_lower_layer_temporal_reference        10'b0101010101  
#define d_lower_layer_horizontal_offset         15'h00000
#define d_lower_layer_vertical_offset           15'h00000
#define d_spatial_temporal_weight_code_table_index 2'b00
#define d_lower_layer_progressive_frame            1'b0
#define d_lower_layer_deinterlaced_field_select    1'b0

//pic_temp_scal_ext 
#define d_pic_temp_scal_ext_load                 1'b0
#define d_picture_temporal_scalable_extension_id 4'b1010
#define d_reference_select_code                  2'b00
#define d_forward_temporal_reference             10'b0101010101
#define d_backward_temporal_reference            10'b0101010101

//slice 
#define d_slice_start_code                      32'h00000101  //[01:AF]
#define d_slice_vertical_position_extension     3'b001
#define d_priority_breakpoint                   7'b0000001
#define d_intra_slice_flag                      5'b00001     //[1:31]
#define d_intra_slice                           1'b1        //[0:1]
//#define d_reserved_bit                          7'b0000000   //
#define d_extra_bit_slice                       1'b1        //1
#define d_extra_information_slice               8'h01     //arbitrariness
#define d_extra_bit_slice_0                     1'b0        //0

#define d_slice_len                             1         //len of each slice in each picture

//mb

#define d_macroblock_escape_num                 0     
#define d_mb_addr_increment_value               1

#define d_macroblock_len                        2        //len of each mb in each slice









//abbreviation for instance
//------------------------------------------------------------------------------

#define group_num           u_video.group_of_pic_header.grp_num
#define picture_num         u_video.pic_header.pic_num         
#define max_sim_time        env.max_sim_cycle                  
#define u_sequence_end_code u_video.sequence_end_code  
#define u_market_bit        u_video.market_bit

#define u_horizontal_size   u_video.horizontal_size 
#define u_vertical_size     u_video.vertical_size   
#define u_bit_rate          u_video.bit_rate        
#define u_vbv_buffer_size   u_video.vbv_buffer_size 

//seq_header                           
#define u_sequence_header_code             u_video.seq_header.sequence_header_code           
#define u_horizontal_size_value            u_video.seq_header.horizontal_size_value          
#define u_vertical_size_value              u_video.seq_header.vertical_size_value            
#define u_aspect_ratio_information         u_video.seq_header.aspect_ratio_information       
#define u_frame_rate_code                  u_video.seq_header.frame_rate_code                
#define u_bit_rate_value                   u_video.seq_header.bit_rate_value                 
#define u_vbv_buffer_size_value            u_video.seq_header.vbv_buffer_size_value          
#define u_constrained_parameters_flag      u_video.seq_header.constrained_parameters_flag    
#define u_load_intra_quantiser_matrix      u_video.seq_header.load_intra_quantiser_matrix    
#define u_load_non_intra_quantiser_matrix  u_video.seq_header.load_non_intra_quantiser_matrix
#define u_intra_quantiser_matrix           u_video.seq_header.intra_quantiser_matrix
#define u_non_intra_quantiser_matrix       u_video.seq_header.non_intra_quantiser_matrix

//seq_ext
#define u_extension_start_code         u_video.seq_ext.extension_start_code        
#define u_sequence_extension_id        u_video.seq_ext.extension_start_code_identifier       
#define u_profile_and_level_indication u_video.seq_ext.profile_and_level_indication  
#define u_progressive_sequence         u_video.seq_ext.progressive_sequence        
#define u_chroma_format                u_video.seq_ext.chroma_format               
#define u_horizontal_size_extension    u_video.seq_ext.horizontal_size_extension   
#define u_vertical_size_extension      u_video.seq_ext.vertical_size_extension     
#define u_bit_rate_extension           u_video.seq_ext.bit_rate_extension          
#define u_vbv_buffer_size_extension    u_video.seq_ext.vbv_buffer_size_extension   
#define u_low_delay                    u_video.seq_ext.low_delay                   
#define u_frame_rate_extension_n       u_video.seq_ext.frame_rate_extension_n      
#define u_frame_rate_extension_d       u_video.seq_ext.frame_rate_extension_d      

//user_data_0
#define u_user_data_start_code u_video.user_data_0.user_data_start_code         
#define u_user_data_0_load     u_video.user_data_0.load     
#define u_user_data_0_len      u_video.user_data_0.len 
#define u_user_data_0          u_video.user_data_0.data
//user_data_1         
#define u_user_data_1_load     u_video.user_data_1.load
#define u_user_data_1_len      u_video.user_data_1.len
#define u_user_data_1          u_video.user_data_1.data
//user_data_2         
#define u_user_data_2_load     u_video.user_data_2.load
#define u_user_data_2_len      u_video.user_data_2.len
#define u_user_data_2          u_video.user_data_2.data







//seq_display_ext 
#define u_seq_display_ext_load           u_video.seq_display_ext.load          
#define u_sequence_display_extension_id  u_video.seq_display_ext.extension_start_code_identifier
#define u_video_format                   u_video.seq_display_ext.video_format                 
#define u_colour_description             u_video.seq_display_ext.colour_description           
#define u_colour_primaries               u_video.seq_display_ext.colour_primaries             
#define u_transfer_characteristics       u_video.seq_display_ext.transfer_characteristics     
#define u_matrix_coefficients            u_video.seq_display_ext.matrix_coefficients          
#define u_display_horizontal_size        u_video.seq_display_ext.display_horizontal_size      
#define u_display_vertical_size          u_video.seq_display_ext.display_vertical_size  

//seq_scal_ext   
#define u_seq_scal_ext_load                      u_video.seq_scal_ext.load                                   
#define u_sequence_scalable_extension_id         u_video.seq_scal_ext.extension_start_code_identifier        
#define u_scalable_mode                          u_video.seq_scal_ext.scalable_mode                         
#define u_layer_id                               u_video.seq_scal_ext.layer_id                              
//#define u_is_spat_scal                           u_video.seq_scal_ext.is_spat_scal                        
#define u_lower_layer_prediction_horizontal_size u_video.seq_scal_ext.lower_layer_prediction_horizontal_size
#define u_lower_layer_prediction_vertical_size   u_video.seq_scal_ext.lower_layer_prediction_vertical_size  
#define u_horizontal_subsampling_factor_m        u_video.seq_scal_ext.horizontal_subsampling_factor_m       
#define u_horizontal_subsampling_factor_n        u_video.seq_scal_ext.horizontal_subsampling_factor_n       
#define u_vertical_subsampling_factor_m          u_video.seq_scal_ext.vertical_subsampling_factor_m         
#define u_vertical_subsampling_factor_n          u_video.seq_scal_ext.vertical_subsampling_factor_n         
//#define u_is_temp_scal                         u_video.seq_scal_ext.is_temp_scal                        
#define u_picture_mux_enable                     u_video.seq_scal_ext.picture_mux_enable                    
#define u_mux_to_progressive_sequence            u_video.seq_scal_ext.mux_to_progressive_sequence           
#define u_picture_mux_order                      u_video.seq_scal_ext.picture_mux_order                     
#define u_picture_mux_factor                     u_video.seq_scal_ext.picture_mux_factor                    
 

                                                                      









//group_of_pic_header                   
#define u_group_start_code u_video.group_of_pic_header.group_start_code           
#define u_time_code        u_video.group_of_pic_header.time_code                  
#define u_closed_gop       u_video.group_of_pic_header.closed_gop                 
#define u_broken_link      u_video.group_of_pic_header.broken_link                
#define u_l_time_code      u_video.group_of_pic_header.l_time_code

                                     
//pic_header                      
#define u_picture_start_code        u_video.pic_header.picture_start_code         
#define u_temporal_reference        u_video.pic_header.temporal_reference         
#define u_picture_coding_type       u_video.pic_header.picture_coding_type        
#define u_vbv_delay                 u_video.pic_header.vbv_delay                  
#define u_full_pel_forward_vector   u_video.pic_header.full_pel_forward_vector    
#define u_full_pel_backward_vector  u_video.pic_header.full_pel_backward_vector   
#define u_forward_f_code            u_video.pic_header.forward_f_code             
#define u_backward_f_code           u_video.pic_header.backward_f_code            
#define u_extra_bit_picture         u_video.pic_header.extra_bit_picture          
#define u_extra_information_picture u_video.pic_header.extra_information_picture  
                                      
//pic_coding_ext                       
#define u_picture_coding_extension_id u_video.pic_coding_ext.extension_start_code_identifier
#define u_f_code00                    u_video.pic_coding_ext.f_code00                   
#define u_f_code01                    u_video.pic_coding_ext.f_code01                   
#define u_f_code10                    u_video.pic_coding_ext.f_code10                   
#define u_f_code11                    u_video.pic_coding_ext.f_code11                   
#define u_intra_dc_precision          u_video.pic_coding_ext.intra_dc_precision         
#define u_picture_structure           u_video.pic_coding_ext.picture_structure          
#define u_top_field_first             u_video.pic_coding_ext.top_field_first            
#define u_frame_pred_frame_dct        u_video.pic_coding_ext.frame_pred_frame_dct       
#define u_concealment_motion_vectors  u_video.pic_coding_ext.concealment_motion_vectors 
#define u_q_scale_type                u_video.pic_coding_ext.q_scale_type               
#define u_intra_vlc_format            u_video.pic_coding_ext.intra_vlc_format           
#define u_alternate_scan              u_video.pic_coding_ext.alternate_scan             
#define u_repeat_first_field          u_video.pic_coding_ext.repeat_first_field         
#define u_chroma_420_type             u_video.pic_coding_ext.chroma_420_type            
#define u_progressive_frame           u_video.pic_coding_ext.progressive_frame          
#define u_composite_display_flag      u_video.pic_coding_ext.composite_display_flag     
#define u_v_axis                      u_video.pic_coding_ext.v_axis                     
#define u_sub_carrier                 u_video.pic_coding_ext.sub_carrier                
#define u_field_sequence              u_video.pic_coding_ext.field_sequence             
#define u_burst_amplitude             u_video.pic_coding_ext.burst_amplitude            
#define u_sub_carrier_phase           u_video.pic_coding_ext.sub_carrier_phase          

 //quant_matrix_ext  
#define u_quant_matrix_ext_load                  u_video.quant_matrix_ext.load                                                  
#define u_quant_matrix_extension_id              u_video.quant_matrix_ext.extension_start_code_identifier                                             
#define u_load_intra_quantiser_matrix_q          u_video.quant_matrix_ext.load_intra_quantiser_matrix        
#define u_intra_quantiser_matrix_q               u_video.quant_matrix_ext.intra_quantiser_matrix                                        
#define u_load_non_intra_quantiser_matrix_q      u_video.quant_matrix_ext.load_non_intra_quantiser_matrix   
#define u_non_intra_quantiser_matrix_q           u_video.quant_matrix_ext.non_intra_quantiser_matrix   

#define u_load_chroma_intra_quantiser_matrix     u_video.quant_matrix_ext.load_chroma_intra_quantiser_matrix                                        
#define u_load_chroma_non_intra_quantiser_matrix u_video.quant_matrix_ext.load_chroma_non_intra_quantiser_matrix                                      
#define u_chroma_intra_quantiser_matrix          u_video.quant_matrix_ext.chroma_intra_quantiser_matrix                                        
#define u_chroma_non_intra_quantiser_matrix      u_video.quant_matrix_ext.chroma_non_intra_quantiser_matrix 
//copyright_ext                                                                      
#define u_copyright_ext_load     u_video.copyright_ext.load                            
#define u_copyright_extension_id u_video.copyright_ext.extension_start_code_identifier 
#define u_copyright_flag         u_video.copyright_ext.copyright_flag                  
#define u_original_or_copy       u_video.copyright_ext.original_or_copy                
#define u_copyright_identifier   u_video.copyright_ext.copyright_identifier            
//#define u_reserved               u_video.copyright_ext.reserved                        
#define u_copyright_number_1     u_video.copyright_ext.copyright_number_1              
#define u_copyright_number_2     u_video.copyright_ext.copyright_number_2              
#define u_copyright_number_3     u_video.copyright_ext.copyright_number_3              
                                                                                       
                                                                                         
//pic_display_ext
#define u_pic_display_ext_load                   u_video.pic_display_ext.load                                                                
#define u_picture_display_extension_id   u_video.pic_display_ext.extension_start_code_identifier   
#define u_frame_centre_horizontal_offset u_video.pic_display_ext.frame_centre_horizontal_offset
#define u_frame_centre_vertical_offset   u_video.pic_display_ext.frame_centre_vertical_offset  
//#define u_number_of_frame_centre_offsets u_video.pic_display_ext.number_of_frame_centre_offsets

                                                   
//pic_spat_scal_ext  u_lower_layer_progressive_frame
#define u_pic_spat_scal_ext_load                   u_video.pic_spat_scal_ext.load                                
#define u_picture_spatial_scalable_extension_id    u_video.pic_spat_scal_ext.extension_start_code_identifier   
#define u_lower_layer_temporal_reference           u_video.pic_spat_scal_ext.lower_layer_temporal_reference          
#define u_lower_layer_horizontal_offset            u_video.pic_spat_scal_ext.lower_layer_horizontal_offset           
#define u_lower_layer_vertical_offset              u_video.pic_spat_scal_ext.lower_layer_vertical_offset             
#define u_spatial_temporal_weight_code_table_index u_video.pic_spat_scal_ext.spatial_temporal_weight_code_table_index
#define u_lower_layer_progressive_frame            u_video.pic_spat_scal_ext.lower_layer_progressive_frame           
#define u_lower_layer_deinterlaced_field_select    u_video.pic_spat_scal_ext.lower_layer_deinterlaced_field_select   
                                                 
//pic_temp_scal_ext 
#define u_pic_temp_scal_ext_load                   u_video.pic_temp_scal_ext.load                                 
#define u_picture_temporal_scalable_extension_id u_video.pic_temp_scal_ext.extension_start_code_identifier
#define u_reference_select_code                  u_video.pic_temp_scal_ext.reference_select_code                 
#define u_forward_temporal_reference             u_video.pic_temp_scal_ext.forward_temporal_reference            
#define u_backward_temporal_reference            u_video.pic_temp_scal_ext.backward_temporal_reference           
