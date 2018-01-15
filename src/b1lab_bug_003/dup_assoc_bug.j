class class_group_of_pic_header {
   bit[31:0]      group_start_code;
   bit[24:0]      time_code;
   bit            closed_gop,broken_link;
   
   bit[24:0]      l_time_code[];
   
   void l_time_code_config(){
     int i ;
      for (i=0;i<3;i++){
      l_time_code[i]=33'h017 ;
      printf("l_time_code[%d]=%d\n",i,l_time_code[i]);
      printf("l_time_code[%d]=%h\n",i,l_time_code[i]);  //can't display
      printf("l_time_code[%d]=%b\n",i,l_time_code[i]);  //can't display

      }  //for

   }
   
}


main() {
  class_group_of_pic_header p0, p1 ;
  
  p0 = new ;
  
  p1 = new p0 ;
  
  p1.l_time_code_config() ;
  p1.l_time_code_config() ;
  p1.l_time_code_config() ;
  p1.l_time_code_config() ;
  p1.l_time_code_config() ;

}
