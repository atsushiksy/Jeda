#define max_group_num                  5    //0 is ignore
#define max_picture_num                5    //0 is ignore
#define d_quant_matrix_ext_load           1'b0
#define d_quant_matrix_extension_id       4'b0011                  
#define d_load_intra_quantiser_matrix     1'b0
#define d_load_non_intra_quantiser_matrix 1'b0  
#define d_load_chroma_intra_quantiser_matrix  1'b0  
#define d_load_chroma_non_intra_quantiser_matrix  1'b0 

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
      int i, h ;
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

}

class check_class {
  class_quant_matrix_ext  ext0 ;
  class_quant_matrix_ext  ext1 ;
  class_quant_matrix_ext  ext2 ;
}

check_class check0 ;
class_quant_matrix_ext matrix0 ;

main() {
  check_class check1 ;

  matrix0 = new ;
  matrix0.default_set() ;
  
  check0 = new ;
  
  check0.ext0 = new matrix0 ;
  check0.ext1 = new matrix0 ;
  check0.ext2 = new matrix0 ;

  check1 = new check0 ;
  check1.ext0 = new check0.ext0 ;
  check1.ext1 = new check0.ext1 ;
  check1.ext2 = new check0.ext2 ;
  
  printf( "b1lab bug 004 check done\n" ) ;

} 

