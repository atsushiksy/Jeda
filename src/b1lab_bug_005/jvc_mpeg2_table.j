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
      mb_addr_increment_vlc[3][0]=4'b0011;
      mb_addr_increment_vlc[3][1]=4;
      mb_addr_increment_vlc[4][0]=4'b0010;
      mb_addr_increment_vlc[4][1]=4;
      mb_addr_increment_vlc[5][0]=5'b00011;
      mb_addr_increment_vlc[5][1]=5;
      mb_addr_increment_vlc[6][0]=5'b00010;
      mb_addr_increment_vlc[6][1]=5;
      mb_addr_increment_vlc[7][0]=7'b0000111;
      mb_addr_increment_vlc[7][1]=7;
      mb_addr_increment_vlc[8][0]=7'b0000110;
      mb_addr_increment_vlc[8][1]=7;
      mb_addr_increment_vlc[9][0]=8'b00001011;
      mb_addr_increment_vlc[9][1]=8;
      
      mb_addr_increment_vlc[10][0]=8'b00001010;       
      mb_addr_increment_vlc[10][1]=8;          
      mb_addr_increment_vlc[11][0]=8'b00001001;     
      mb_addr_increment_vlc[11][1]=8;          
      mb_addr_increment_vlc[12][0]=8'b00001000;     
      mb_addr_increment_vlc[12][1]=8;          
      mb_addr_increment_vlc[13][0]=8'b000001111;    
      mb_addr_increment_vlc[13][1]=8;          
      mb_addr_increment_vlc[14][0]=8'b00000110;  
      mb_addr_increment_vlc[14][1]=8;            
      mb_addr_increment_vlc[15][0]=10'b0000010111;  
      mb_addr_increment_vlc[15][1]=10;            
      mb_addr_increment_vlc[16][0]=10'b0000010110;  
      mb_addr_increment_vlc[16][1]=10;            
      mb_addr_increment_vlc[17][0]=10'b0000010101; 
      mb_addr_increment_vlc[17][1]=10;            
      mb_addr_increment_vlc[18][0]=10'b0000010100; 
      mb_addr_increment_vlc[18][1]=10;          
      mb_addr_increment_vlc[19][0]=10'b0000010011;
      mb_addr_increment_vlc[19][1]=10;          
      
      mb_addr_increment_vlc[20][0]=10'b0000010010;
      mb_addr_increment_vlc[20][1]=10;
      mb_addr_increment_vlc[21][0]=11'b00000100011;
      mb_addr_increment_vlc[21][1]=11;
      mb_addr_increment_vlc[22][0]=11'b00000100010;
      mb_addr_increment_vlc[22][1]=11;
      mb_addr_increment_vlc[23][0]=11'b00000100001;
      mb_addr_increment_vlc[23][1]=11;
      mb_addr_increment_vlc[24][0]=11'b00000100000;
      mb_addr_increment_vlc[24][1]=11;
      mb_addr_increment_vlc[25][0]=11'b00000100111;
      mb_addr_increment_vlc[25][1]=11;         
      mb_addr_increment_vlc[26][0]=11'b00000100110;
      mb_addr_increment_vlc[26][1]=11;         
      mb_addr_increment_vlc[27][0]=11'b00000100101;
      mb_addr_increment_vlc[27][1]=11;         
      mb_addr_increment_vlc[28][0]=11'b00000100100;
      mb_addr_increment_vlc[28][1]=11;         
      mb_addr_increment_vlc[29][0]=11'b00000100011;
      mb_addr_increment_vlc[29][1]=11;             
      
      mb_addr_increment_vlc[30][0]=11'b00000100010;
      mb_addr_increment_vlc[30][1]=11;             
      mb_addr_increment_vlc[31][0]=11'b00000100001;
      mb_addr_increment_vlc[31][1]=11;             
      mb_addr_increment_vlc[32][0]=11'b00000100000;
      mb_addr_increment_vlc[32][1]=11; 
      for(i=0;i<33;i++){            
         printf("mb_addr_increment_vlc[i]=%d %d\n",i,mb_addr_increment_vlc[i][0],mb_addr_increment_vlc[i][1]);
      }
   
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
      P_mb_vlc[1][1]=1;        
      P_mb_vlc[1][2]=6'b000100;
      P_mb_vlc[2][0]=3;  
      P_mb_vlc[2][1]=1;  
      P_mb_vlc[2][2]=6'b010000;
      P_mb_vlc[3][0]=5;  
      P_mb_vlc[3][1]=3;  
      P_mb_vlc[3][2]=6'b000010;
      P_mb_vlc[4][0]=5;  
      P_mb_vlc[4][1]=2;  
      P_mb_vlc[4][2]=6'b110100;
      P_mb_vlc[5][0]=5;  
      P_mb_vlc[5][1]=1;  
      P_mb_vlc[5][2]=6'b100100;
      P_mb_vlc[6][0]=6;  
      P_mb_vlc[6][1]=1;  
      P_mb_vlc[6][2]=6'b100010;
   
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
      B_mb_vlc[3][1]=3; 
      B_mb_vlc[3][2]=6'b001100;
      B_mb_vlc[4][0]=4; 
      B_mb_vlc[4][1]=2; 
      B_mb_vlc[4][2]=6'b010000;
      B_mb_vlc[5][0]=4; 
      B_mb_vlc[5][1]=3; 
      B_mb_vlc[5][2]=6'b010100;
      B_mb_vlc[6][0]=5; 
      B_mb_vlc[6][1]=3; 
      B_mb_vlc[6][2]=6'b000010;     
      B_mb_vlc[7][0]=5;
      B_mb_vlc[7][1]=2; 
      B_mb_vlc[7][2]=6'b111100;
      B_mb_vlc[8][0]=6; 
      B_mb_vlc[8][1]=3; 
      B_mb_vlc[8][2]=6'b110100;
      B_mb_vlc[9][0]=6; 
      B_mb_vlc[9][1]=2;
      B_mb_vlc[9][2]=6'b101100;
      B_mb_vlc[10][0]=6;
      B_mb_vlc[10][1]=1;
      B_mb_vlc[10][2]=6'b100010;
      
   //////////////Table B-5//////////////////////////////////////////////////////   
      I_mb_vlc_spal[0][0]=1;        
      I_mb_vlc_spal[0][1]=1;        
      I_mb_vlc_spal[0][2]=6'b000100;
      I_mb_vlc_spal[1][0]=2; 
      I_mb_vlc_spal[1][1]=1; 
      I_mb_vlc_spal[1][2]=6'b100100;
      I_mb_vlc_spal[2][0]=4; 
      I_mb_vlc_spal[2][1]=3; 
      I_mb_vlc_spal[2][2]=6'b000010;
      I_mb_vlc_spal[3][0]=4; 
      I_mb_vlc_spal[3][1]=2; 
      I_mb_vlc_spal[3][2]=6'b100010;
      I_mb_vlc_spal[4][0]=4; 
      I_mb_vlc_spal[4][1]=1; 
      I_mb_vlc_spal[4][2]=6'b000000;

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
      P_mb_vlc_spal[5][2]=6'b000010;
      P_mb_vlc_spal[6][0]=4; 
      P_mb_vlc_spal[6][1]=3; 
      P_mb_vlc_spal[6][2]=6'b010001;     
      P_mb_vlc_spal[7][0]=3;
      P_mb_vlc_spal[7][1]=2; 
      P_mb_vlc_spal[7][2]=6'b110100;
      P_mb_vlc_spal[8][0]=6; 
      P_mb_vlc_spal[8][1]=4; 
      P_mb_vlc_spal[8][2]=6'b100100;
      P_mb_vlc_spal[9][0]=7; 
      P_mb_vlc_spal[9][1]=6;
      P_mb_vlc_spal[9][2]=6'b100010;      
      P_mb_vlc_spal[10][0]=2;       
      P_mb_vlc_spal[10][1]=3;
      P_mb_vlc_spal[10][2]=6'b110101;
      P_mb_vlc_spal[11][0]=6; 
      P_mb_vlc_spal[11][1]=5; 
      P_mb_vlc_spal[11][2]=6'b100101;
      P_mb_vlc_spal[12][0]=6; 
      P_mb_vlc_spal[12][1]=6; 
      P_mb_vlc_spal[12][2]=6'b000001;
      P_mb_vlc_spal[13][0]=7; 
      P_mb_vlc_spal[13][1]=5; 
      P_mb_vlc_spal[13][2]=6'b000100;
      P_mb_vlc_spal[14][0]=7; 
      P_mb_vlc_spal[14][1]=2; 
      P_mb_vlc_spal[14][2]=6'b100100;
      P_mb_vlc_spal[15][0]=7; 
      P_mb_vlc_spal[15][1]=3; 
      P_mb_vlc_spal[15][2]=6'b000000;
   
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
      B_mb_vlc_spal[3][2]=6'b001100;
      B_mb_vlc_spal[4][0]=4; 
      B_mb_vlc_spal[4][1]=2; 
      B_mb_vlc_spal[4][2]=6'b010000;
      B_mb_vlc_spal[5][0]=4; 
      B_mb_vlc_spal[5][1]=3; 
      B_mb_vlc_spal[5][2]=6'b010100;
      B_mb_vlc_spal[6][0]=6; 
      B_mb_vlc_spal[6][1]=4; 
      B_mb_vlc_spal[6][2]=6'b001001;     
      B_mb_vlc_spal[7][0]=6;
      B_mb_vlc_spal[7][1]=7; 
      B_mb_vlc_spal[7][2]=6'b001101;
      B_mb_vlc_spal[8][0]=6; 
      B_mb_vlc_spal[8][1]=4; 
      B_mb_vlc_spal[8][2]=6'b010001;
      B_mb_vlc_spal[9][0]=6; 
      B_mb_vlc_spal[9][1]=5;
      B_mb_vlc_spal[9][2]=6'b010101;      
      B_mb_vlc_spal[10][0]=7;       
      B_mb_vlc_spal[10][1]=6;
      B_mb_vlc_spal[10][2]=6'b000010;
      B_mb_vlc_spal[11][0]=7; 
      B_mb_vlc_spal[11][1]=7; 
      B_mb_vlc_spal[11][2]=6'b111100;
      B_mb_vlc_spal[12][0]=7; 
      B_mb_vlc_spal[12][1]=4; 
      B_mb_vlc_spal[12][2]=6'b110100;
      B_mb_vlc_spal[13][0]=7; 
      B_mb_vlc_spal[13][1]=5; 
      B_mb_vlc_spal[13][2]=6'b101100;
      B_mb_vlc_spal[14][0]=8; 
      B_mb_vlc_spal[14][1]=4; 
      B_mb_vlc_spal[14][2]=6'b100010;
      B_mb_vlc_spal[15][0]=8; 
      B_mb_vlc_spal[15][1]=5; 
      B_mb_vlc_spal[15][2]=6'b110101;
      B_mb_vlc_spal[16][0]=9; 
      B_mb_vlc_spal[16][1]=12; 
      B_mb_vlc_spal[16][2]=6'b101101;     
      B_mb_vlc_spal[17][0]=9;
      B_mb_vlc_spal[17][1]=14; 
      B_mb_vlc_spal[17][2]=6'b000000;
      B_mb_vlc_spal[18][0]=9; 
      B_mb_vlc_spal[18][1]=13; 
      B_mb_vlc_spal[18][2]=6'b100100;
      B_mb_vlc_spal[19][0]=9; 
      B_mb_vlc_spal[19][1]=15;
      B_mb_vlc_spal[19][2]=6'b000100;
   
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
      coded_block_pattern_vlc[2][1]=4'b1100;
      coded_block_pattern_vlc[2][2]=8;
      coded_block_pattern_vlc[3][0]=4;
      coded_block_pattern_vlc[3][1]=4'b1011;
      coded_block_pattern_vlc[3][2]=16;
      coded_block_pattern_vlc[4][0]=4;
      coded_block_pattern_vlc[4][1]=4'b1010;
      coded_block_pattern_vlc[4][2]=32;
      coded_block_pattern_vlc[5][0]=5;
      coded_block_pattern_vlc[5][1]=5'b10011;
      coded_block_pattern_vlc[5][2]=12;
      coded_block_pattern_vlc[6][0]=5;
      coded_block_pattern_vlc[6][1]=5'b10010;
      coded_block_pattern_vlc[6][2]=48;     
      coded_block_pattern_vlc[7][0]=5;
      coded_block_pattern_vlc[7][1]=5'b10001;
      coded_block_pattern_vlc[7][2]=20;
      coded_block_pattern_vlc[8][0]=5;
      coded_block_pattern_vlc[8][1]=5'b10000;
      coded_block_pattern_vlc[8][2]=40;
      coded_block_pattern_vlc[9][0]=5;
      coded_block_pattern_vlc[9][1]=5'b01111;
      coded_block_pattern_vlc[9][2]=28;
      
      coded_block_pattern_vlc[10][0]=5;
      coded_block_pattern_vlc[10][1]=5'b01110;
      coded_block_pattern_vlc[10][2]=44;
      coded_block_pattern_vlc[11][0]=5;
      coded_block_pattern_vlc[11][1]=5'b01101;
      coded_block_pattern_vlc[11][2]=52;
      coded_block_pattern_vlc[12][0]=5;
      coded_block_pattern_vlc[12][1]=5'b01100;
      coded_block_pattern_vlc[12][2]=56;
      coded_block_pattern_vlc[13][0]=5;
      coded_block_pattern_vlc[13][1]=5'b01011;
      coded_block_pattern_vlc[13][2]=1;
      coded_block_pattern_vlc[14][0]=5;
      coded_block_pattern_vlc[14][1]=5'b01010;
      coded_block_pattern_vlc[14][2]=61;
      coded_block_pattern_vlc[15][0]=5;
      coded_block_pattern_vlc[15][1]=5'b01001;
      coded_block_pattern_vlc[15][2]=2;
      coded_block_pattern_vlc[16][0]=5;
      coded_block_pattern_vlc[16][1]=5'b01000;
      coded_block_pattern_vlc[16][2]=62;  
      coded_block_pattern_vlc[17][0]=6;
      coded_block_pattern_vlc[17][1]=6'b001111;
      coded_block_pattern_vlc[17][2]=24;
      coded_block_pattern_vlc[18][0]=6;
      coded_block_pattern_vlc[18][1]=6'b001110;
      coded_block_pattern_vlc[18][2]=36;
      coded_block_pattern_vlc[19][0]=6;
      coded_block_pattern_vlc[19][1]=6'b001101;
      coded_block_pattern_vlc[19][2]=3;
      
//??????????????????????????????????????????????????????????????????????????????????????////////////??????
   
   //////////////Table B-10/////////////////////////////////////////////////////

      motion_code_vlc[0][0]=11;    
      motion_code_vlc[0][1]=11'b00000011001;        
      motion_code_vlc[0][2]=-16;
      motion_code_vlc[1][0]=11;
      motion_code_vlc[1][1]=11'b00000011011;
      motion_code_vlc[1][2]=-15;
/*
      motion_code_vlc[2][0]=;
      motion_code_vlc[2][1]='b;
      motion_code_vlc[2][2]=;
      motion_code_vlc[3][0]=;
      motion_code_vlc[3][1]='b;
      motion_code_vlc[3][2]=6;
      motion_code_vlc[4][0]=;
      motion_code_vlc[4][1]='b;
      motion_code_vlc[4][2]=2;
      motion_code_vlc[5][0]=;
      motion_code_vlc[5][1]='b;
      motion_code_vlc[5][2]=2;
      motion_code_vlc[6][0]=;
      motion_code_vlc[6][1]='b;
      motion_code_vlc[6][2]=8;
      motion_code_vlc[7][0]=;
      motion_code_vlc[7][1]='b;
      motion_code_vlc[7][2]=0;
      motion_code_vlc[8][0]=;
      motion_code_vlc[8][1]='b;
      motion_code_vlc[8][2]=0;
      motion_code_vlc[9][0]=5;
      motion_code_vlc[9][1]=5'b;
      motion_code_vlc[9][2]=28;
      
 */     
  // ?????????????????????????????????????????????????33//////////////////////
   
   
   
   
   //////////////Table B-11/////////////////////////////////////////////////////
   //dmvector_vlc[3][3]
      motion_code_vlc[0][0]=11;    
      motion_code_vlc[0][1]=11'b00000011001;        
      motion_code_vlc[0][2]=-16;
      motion_code_vlc[1][0]=11;
      motion_code_vlc[1][1]=11'b00000011011;
      motion_code_vlc[1][2]=-15;
         
   
   
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
