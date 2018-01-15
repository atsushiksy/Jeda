   void PioRead (var bit [31:0] data)
   {
     data[31:24] = 8'hff;
   }
   
   
   void main () {
     bit [31:0] data;
   
     PioRead (data);
     
     printf( "%h %b\n", data, data ) ;
   }  
