/*                                                                              
--(C)opyright 2003 B1LAB INC.                                                   
--------------------------------------------------------------------------------
-- Project        :  jVC MPEG2                                                  
-- Title          :  Header file for transmit and receive IO stub.                                     
--------------------------------------------------------------------------------
-- File           :  jvc_mpeg2_io_stub_h.j                                            
-- Author         :  MIKE YIN <MIKE_YIN@b1lab.com>                              
-- Created        :  2003/03/06                                                 
-- Source         :                                                             
-- Last modified  :                                                             
--------------------------------------------------------------------------------
-- Description:
--  This file defines transfer stub of mpeg2.
--  including jvc_mpeg2_in_stub and jvc_mpeg2_out_stub
-- Description :                                                                
--   1.  Import all VC top level types and defs.                                
--                                                                              
--   2.  Defines VC at top level, with :                                        
--       Topology, with usage illustration.                                     
--       Parameters.                                                            
--       Events.                                                                
--------------------------------------------------------------------------------

*/ 
#define bus_wide 32
                                                                             
class jvc_mpeg2_in_stub{
//   int  stub_tx_qe<>;
   int  tx_bus_item=0;
   bit  l_tx_item<>;
   event   ev_qe_send_ok;
   new(){
      ev_qe_send_ok=new;
   }
   void tx_data(){
      string trace_msg;
//      static int left_len=0,left_data=0;
      int k=0,l=0,num=0,len=0,left_bit=0;
      l_tx_item=ig.l_data_item;
      len=l_tx_item.num_element();
      left_bit=len%bus_wide;
      if (left_bit==0) num=len/bus_wide;
      else num=len/bus_wide+1;


      for (k=0;k<num;k++){
         int bit_len=0;
         
         if ((left_bit!=0)&(k==num-1)) {
            tx_bus_item=0;  //the no useful bit value is 0;
            bit_len=left_bit;
            }
         else bit_len=bus_wide;
         tx_bus_item=unpack_on_bus(bit_len);
         
      trace_msg=sprintf("tx data bus item is %h\n",tx_bus_item); 
                                                                             
      trace(jvc_stub,jvc_B1LAB_DEBUG,trace_msg);                                  
      }
     
      
      trace_msg="tx data is ready on bus";                                
//      trace(jvc_monitor,jvc_B1LAB_DEBUG,trace_msg);                                                       


      ev_qe_send_ok.trigger();
      ig_data_item_flush_all();

      trace_msg="ev_qe_send_ok has sent";                                                               
      trace(jvc_stub,jvc_B1LAB_DEBUG,trace_msg);                                     

   }
int unpack_on_bus(int len){
   int i=0;
   bit[31:0] n=0,m=0;
   
   for (i=bus_wide;i>(bus_wide-len);i--){
      m=l_tx_item.bpop();

      m=m<<(i-1);
      n=m^n;

   }
      return n;
} //pack_qe()

void ig_data_item_flush_all(){
   int i=0,len=0;
   len =ig.l_data_item.num_element();
   for (i=0;i<len;i++){
      ig.l_data_item.pop();
   }
//   len =ig.l_data_item.num_element();


}
  
/*   
   void add_left_item(int len,int data){
      int i=0,m=0;  
      m=bus_wide-len;
      for(i=0;i<len;i++){
         n=data[m:m];
         l_tx_item=bpush(m);    //msb sends firstly.
         m=m-1;
      } //for
  
   } //
*/ 
}

class jvc_mpeg2_out_stub{

} //jvc_mpeg2_out_stub


