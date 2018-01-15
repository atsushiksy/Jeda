/**********************************************************************************************
 *     This example shows how to use Jeda to write a modular, self-contained class object
 *     to model the operation of a AMD_flash device
 *     Author: EZ
 *     Copyright (c) 2003 Jeda Technologies, Inc
 **********************************************************************************************/

typedef class AMD_Flash;
AMD_Flash AmFlash;


/*
 * modeling AMD29LV081B, an async device
 * it doens't check timing yet.
 */
class  AMD_Flash {
  /* Device IOs */
  signal sig_adr;     // A0-A9, inout
  signal sig_data;    // DQ0-DQ7, inout
  signal sig_ce_n;    // chip enable, input
  signal sig_oe_n;    // output enable, input
  signal sig_we_n;    // write enable, input
  signal sig_reset_n; // input
  signal sig_ry_by_n; // ready /busy, output

  signal sig_clock;   // temp for sync use

  bit [31:0] mem[];   // the flash memory

  int f_debug=1;

  /*
   * when the object is created, signal instances are passwd along as arguments, so 
   * this class is DUT IO name dependent. As an example, if portset is 'W', the arguments
   * can be W.IO_AD[23:0]; W.IO_AD[31:24]; W.BOOT_FLASH_CE_L; for the first three arguments.
   */
  void new ( signal a_sig_adr, signal a_sig_data, signal a_sig_ce_n, signal a_sig_oe_n,
             signal a_sig_we_n, signal a_sig_reset_n, signal a_sig_clock ) {

    bit reset=0;

    sig_adr =  a_sig_adr;
    sig_data = a_sig_data;
    sig_ce_n = a_sig_ce_n;
    sig_oe_n = a_sig_oe_n;
    sig_we_n = a_sig_we_n;
    sig_reset_n = a_sig_reset_n;
    sig_clock =   a_sig_clock;


    // check for reset.
    fork {
      repeat(50) {
	@1 (posedge sig_clock);
	if (sig_reset_n.0 == 1'b0) 	reset = 1;
	if(reset) break;
      }
      if(!reset) error("\t\t FL_RESET is not set\n");

      activate();
    }
    knife
  }

  /* 
   * function as span off thread mimic flash behavior
   */
  void activate() {
    bit rw = 0;  // 0=read, 1= write
    bit [19:0] adr;
    bit [7:0] data;

    forever {
      @ (negedge sig_ce_n);
      if(f_debug) printf("FLASH: got ce_n at time %d\n", get_time());
      fork 
	{
	  // wait for oe_n for a read
	  @ (negedge sig_oe_n);
	  if(f_debug) printf("FLASH READ: got oe_n at time %d\n", get_time());
	  adr = sig_adr.0;
	  if(f_debug) printf("FLASH READ: got oe_n at time %d, adr=%h\n", get_time(), adr);
	  if(mem.check_index(adr)==0) {
	    if(f_debug) printf("FLASH READ WARNING: adr=%h Not Written, return x\n", get_time(), adr);
	    data = 8'hx;
	  }
	  else {
	    data = mem[adr];
	  }
	  sig_data.0 = data;

	  if(f_debug) printf("FLASH READ: adr=%h data=%h\n", adr, data);
	  // check for completion
	  @(posedge sig_ce_n);
	  if(f_debug) printf("FLASH READ: de-asert ce_n at time %d\n", get_time());
	  @(posedge sig_oe_n);
	  if(f_debug) printf("FLASH READ: de-asert oe_n at time %d\n", get_time());
	  if(f_debug) printf("FLASH READ: Completed\n");

	}
        {
	  @ (negedge sig_we_n);
	  if(f_debug) printf("FLASH WRITE: got we_n at time %d\n", get_time());
	  // make sure oe is not asserted
	  if(sig_oe_n.0 == 1'b0 ) error(" FLASH WRITE: oe_n is asserted \n");
	  adr = sig_adr.0;
	  data = sig_data.0;
	  mem[adr] = data;
	  if(f_debug) printf("FLASH WRITE: adr=%h data=%h\n", adr, data);
	  // to catch de-assert we_n
	  @(posedge sig_we_n);
	  if(f_debug) printf("FLASH WRITE: de-asert we_n at time %d\n", get_time());
	  @(posedge sig_ce_n);
	  if(f_debug) printf("FLASH WRITE: de-asert ce_n at time %d\n", get_time());
	  if(f_debug) printf("FLASH WRITE: Completed\n");
        }
      spoon
      
    }
  }

}
