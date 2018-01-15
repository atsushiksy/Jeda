/*                                                                              
--(C)opyright 2003 B1LAB INC.                                                   
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2                                                  
-- Title          :  Header file for transmit and receive IO stub.                                     
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_io_stub_h.j                                            
-- Author         :                               
-- Created        :  2003/03/06                                                 
-- Source         :                                                             
-- Last modified  :                                                             
--------------------------------------------------------------------------------
-- Description:
-- 
-- 
--                         
--                         
--                         
--                         
--                         
--                         
--                         
--------------------------------------------------------------------------------

*/ 
class class_table {
   bit[10:0] mb_addr_increment_vlc[33][2];
   bit[5:0]  I_mb_vlc[2][3];                  
   bit[5:0]  P_mb_vlc[7][3];                  
   bit[5:0]  B_mb_vlc[11][3];                 
   bit[5:0]  I_mb_vlc_spal[5][3];             
   bit[5:0]  P_mb_vlc_spal[16][3];            
   bit[5:0]  B_mb_vlc_spal[20][3];            
   bit[5:0]  I_P_B_mb_vlc_SNR[3][3];          
   bit[8:0]  coded_block_pattern_vlc[64][3];  
   int       motion_code_vlc[33][3];          
   int       dmvector_vlc[3][3];              
   bit[8:0]  dct_dc_size_luminance_vlc[12][3];
   bit[8:0]  dct_dc_size_chrominance[12][3];  
   int       DCT_coefficents_zero_vlc[128][4];
   int       DCT_coefficents_one_vlc[128][4]; 
   
    
   void load(){
      mb_addr_increment_vlc[0][0]=1'b1;
      mb_addr_increment_vlc[0][1]=1;
      mb_addr_increment_vlc[1][0]=3'b001;
      mb_addr_increment_vlc[1][1]=3;
      mb_addr_increment_vlc[2][0]=3'b010;
      mb_addr_increment_vlc[2][1]=3;
      
 
      
   ////////////////I_mb_vlc[][0]=len(the num bit of vlc value)
   ////////////////I_mb_vlc[][1]=vlc value
   ////////////////I_mb_vlc[][1]=meaning
   /////////////Table B-2///////////////////////////////////////////////////////   
      I_mb_vlc[0][0]=1;
      I_mb_vlc[0][1]=1;
      I_mb_vlc[0][2]=6'b000010;
      I_mb_vlc[1][0]=2;
      I_mb_vlc[1][1]=1;
      I_mb_vlc[1][2]=6'b100010;
   
   //////////////Table B-3//////////////////////////////////////////////////////   
      P_mb_vlc[0][0]=1;        
      P_mb_vlc[0][1]=1;        
      P_mb_vlc[0][2]=6'b010100;
      P_mb_vlc[1][0]=2;        
 
   
   //////////////Table B-4//////////////////////////////////////////////////////  
      B_mb_vlc[0][0]=2;        
      B_mb_vlc[0][1]=2;        
      B_mb_vlc[0][2]=6'b011000;
      B_mb_vlc[1][0]=2;  
      B_mb_vlc[1][1]=3;  
      B_mb_vlc[1][2]=6'b011100;
      B_mb_vlc[2][0]=3; 
      B_mb_vlc[2][1]=2; 
      B_mb_vlc[2][2]=6'b001000;
      B_mb_vlc[3][0]=3; 
    
      
   //////////////Table B-5//////////////////////////////////////////////////////   
      I_mb_vlc_spal[0][0]=1;        
      I_mb_vlc_spal[0][1]=1;        
      I_mb_vlc_spal[0][2]=6'b000100;
      I_mb_vlc_spal[1][0]=2; 
      I_mb_vlc_spal[1][1]=1; 
      I_mb_vlc_spal[1][2]=6'b100100;
      I_mb_vlc_spal[2][0]=4; 


   //////////////Table B-6//////////////////////////////////////////////////////
      P_mb_vlc_spal[0][0]=2;        
      P_mb_vlc_spal[0][1]=2;  
      P_mb_vlc_spal[0][2]=6'b010100;
      P_mb_vlc_spal[1][0]=3; 
      P_mb_vlc_spal[1][1]=3; 
      P_mb_vlc_spal[1][2]=6'b010101;
      P_mb_vlc_spal[2][0]=7; 
      P_mb_vlc_spal[2][1]=4; 
      P_mb_vlc_spal[2][2]=6'b000100;
      P_mb_vlc_spal[3][0]=6; 
      P_mb_vlc_spal[3][1]=7; 
      P_mb_vlc_spal[3][2]=6'b000101;
      P_mb_vlc_spal[4][0]=4; 
      P_mb_vlc_spal[4][1]=2; 
      P_mb_vlc_spal[4][2]=6'b010000;
      P_mb_vlc_spal[5][0]=7; 
      P_mb_vlc_spal[5][1]=7; 
  
   
   //////////////Table B-7//////////////////////////////////////////////////////
      B_mb_vlc_spal[0][0]=2;        
      B_mb_vlc_spal[0][1]=2;  
      B_mb_vlc_spal[0][2]=6'b011000;
      B_mb_vlc_spal[1][0]=2; 
      B_mb_vlc_spal[1][1]=3; 
      B_mb_vlc_spal[1][2]=6'b011100;
      B_mb_vlc_spal[2][0]=3; 
      B_mb_vlc_spal[2][1]=2; 
      B_mb_vlc_spal[2][2]=6'b001000;
      B_mb_vlc_spal[3][0]=3; 
      B_mb_vlc_spal[3][1]=3; 
 
   
   //////////////Table B-8//////////////////////////////////////////////////////     
      I_P_B_mb_vlc_SNR[0][0]=1;
      I_P_B_mb_vlc_SNR[0][1]=1;
      I_P_B_mb_vlc_SNR[0][2]=6'b000100;
      I_P_B_mb_vlc_SNR[1][0]=2;
      I_P_B_mb_vlc_SNR[1][1]=1;
      I_P_B_mb_vlc_SNR[1][2]=6'b100100;
      I_P_B_mb_vlc_SNR[2][0]=3;
      I_P_B_mb_vlc_SNR[2][1]=1;
      I_P_B_mb_vlc_SNR[2][2]=6'b000000;
      
   //////////////
   //////////////Table B-9//////////////////////////////////////////////////////
      coded_block_pattern_vlc[0][0]=3;        
      coded_block_pattern_vlc[0][1]=3'b111;        
      coded_block_pattern_vlc[0][2]=60;
      coded_block_pattern_vlc[1][0]=4;
      coded_block_pattern_vlc[1][1]=4'b1101;
      coded_block_pattern_vlc[1][2]=4;
      coded_block_pattern_vlc[2][0]=4;

      
//??????????????????????????????????????????????????????????????????????????????????????////////////??????
   
   //////////////Table B-10/////////////////////////////////////////////////////

      motion_code_vlc[0][0]=11;    
      motion_code_vlc[0][1]=11'b00000011001;        
      motion_code_vlc[0][2]=-16;
      motion_code_vlc[1][0]=11;
      motion_code_vlc[1][1]=11'b00000011011;
      motion_code_vlc[1][2]=-15;

  // ?????????????????????????????????????????????????33//////////////////////
   
   
   
   
   //////////////Table B-11/////////////////////////////////////////////////////
   //dmvector_vlc[3][3]
      dmvector_vlc[0][0]=11;    
      dmvector_vlc[0][1]=11'b00000011001;        
      dmvector_vlc[0][2]=-16;
      dmvector_vlc[1][0]=11;
      dmvector_vlc[1][1]=11'b00000011011;
      dmvector_vlc[1][2]=-15;
         
   
   
   //////////////Table B-12/////////////////////////////////////////////////////
   //dct_dc_size_luminance_vlc[12][3]
      dct_dc_size_luminance_vlc[0][0]=3;
      dct_dc_size_luminance_vlc[0][1]=3'b100;
      dct_dc_size_luminance_vlc[0][2]=0;
      dct_dc_size_luminance_vlc[1][0]=2;
      dct_dc_size_luminance_vlc[1][1]=2'b00;
      dct_dc_size_luminance_vlc[1][2]=1;
      
      //??????????????????????????????????????????
      
   //////////////Table B-13///////////dct_dc_size_chrominance[12][3]////////////   
      dct_dc_size_chrominance[0][0]=2;
      dct_dc_size_chrominance[0][1]=2'b00;
      dct_dc_size_chrominance[0][2]=0;
      dct_dc_size_chrominance[1][0]=2;
      dct_dc_size_chrominance[1][1]=2'b01;
      dct_dc_size_chrominance[1][2]=1;
      
      //??????????????????????????????????????????
      
   //////////////Table B-14///////////DCT_coefficents_zero_vlc[128][4]//////////   
      DCT_coefficents_zero_vlc[0][0]=2;
      DCT_coefficents_zero_vlc[0][1]=2'b10;
      DCT_coefficents_zero_vlc[0][2]=0;
      DCT_coefficents_zero_vlc[0][3]=0;
      DCT_coefficents_zero_vlc[1][0]=1;
      DCT_coefficents_zero_vlc[1][1]=1'b1;
      DCT_coefficents_zero_vlc[1][2]=0;
      DCT_coefficents_zero_vlc[1][3]=1;
      
      
      
      //??????????????????????????????????????????   
      
   
   
   
      
   } //load
   
   
   
} //class_table_b
