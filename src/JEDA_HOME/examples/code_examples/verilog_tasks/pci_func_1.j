/**********************************************************************************************
 *     This example shows how to call Verilog tasks from Jeda by verilog.task_name()
 *     
 *     Author: EZ
 *     Copyright (c) 2003 Jeda Technologies, Inc
 **********************************************************************************************/

#include "./dut_defs.jh"
#include "./verilog_class.jh"


/*
 *  adr range from 32'h0 to 32'h7fff to cover 32KB 
 */
void   w_pci_init( bit [31:0] ntransactions, 
                      bit [31:0] bcm_mem_min=32'h0, bit [31:0] bcm_mem_max=32'h7fff )

{
    bit [15:0] rstatus=0;
    bit        rflag=0;
    bit        cflag=0;
    bit [31:0] rdata=0;
    bit [31:0] data;
    bit [31:0] ran;
    bit [1:0]  ndevsel;
    bit [1:0]  nwaits;

    @(20);
    ran = random();
    printf("   setup PCI devices\n");

    /* Master */
    printf("   calling modify_latency\n");
    verilog.modify_latency(100);  // 16
    printf("   exiting calling modify_latency\n");
    verilog.modify_cycle(0,0,0,1,1,0,0);
    verilog.modify_clock_frequency(0); // 0-33MHz, 1-66MHz


    /* Slave */
    verilog.modify_mem_space(bcm_mem_min, bcm_mem_max);
    
    //transfer_max  abort_max   termination
    verilog.modify_termination(32,           32,         1);
  
    // lock, back, step, serren, perr_response,sepcial, mem, io, dual_addr, 64bit, int_ack)
    verilog.modify_response(0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0);

    verilog.clear_trdy_delays();

    // transfer_num, nwaits. Choose nwaits from 0 to 3
    nwaits = ran[3:2];   
    verilog.modify_trdy_delay(32, nwaits);

    verilog.modify_parity( 0);   // no parity error

    // set ndevsel as a random number of 0, 1 or 2
    ndevsel = ran[1:0];
    if ( ndevsel == 2'b11) ndevsel = 0;
    // tell slave to respond
    fork {
        verilog.respond( ntransactions, ndevsel, rstatus, rflag );
    }
    knife
    printf(" done with setup PCI devices\n");
    
    printf ("Configuring the PCI core in the W chip\n");

    // Verify the vendor ID.
    rdata = DUT_VDID;
    verilog.cfg_rd(PCI_DEVICE_VND_ID, rdata, 4'b0000, 0, 1, rstatus, rflag, cflag);
    printf("rdata=%h rstatus=%b rflag=%b cflag=%b\n", rdata, rstatus, rflag, cflag);
    if ( rflag != 1'b0) {
	pci_error(rstatus);
	error("W_pci_init: Reading W Chip Vendor ID Register failed");
    }
    printf("w_pci_init: read PCI_DEVICE_VND_ID adr=%h data=%h\n", PCI_DEVICE_VND_ID, rdata);
 
    if ( rdata != DUT_VDID ) {
	error("w_pci_init: Bad W chip id %d %d %d, Expected=%h , Recvd=%h\n",
                        ( rdata & 32'hf0000000 ) >> 28,
                        ( rdata & 32'h0ffff000 ) >> 12,
                        ( rdata & 32'h00000ffe) >> 1,
                        DUT_VDID,
                        rdata);
    }
    
    // Set IO Space Access Control    PCI Bus Master Control
    data    =   PCICMD_SYSERR|PCICMD_PARERR|PCICMD_BUSMSTR|PCICMD_MEMSPACE;
 
    printf ("DUT_chip_init: Setting Command & Status Register to value %h\n", data);
    printf("Winit: write PCI_STATUS_CMD adr=%h data=%h\n", PCI_STATUS_CMD, data);
    verilog.cfg_wr( PCI_STATUS_CMD,data, 4'b0000, 0, 1, rstatus, rflag);
    if ( rflag != 1'b0) {
	pci_error(rstatus);
	error("DUT_chip_init: Writing Command & Status Register failed");
    }

    verilog.cfg_rd( PCI_STATUS_CMD,rdata, 4'b0000, 0, 1, rstatus, rflag, cflag);
    if ( rflag != 1'b0 ) {
	pci_error(rstatus);
	error("DUT_chip_init: reading Command & Status Register failed");
    }
    printf ("DUT_chip_init: Reading Command & Status Register value %h\n", rdata);


    printf ("DUT_chip_init: Setting Base Addr Register0 to value %h\n", DUT_BASE );
    printf("DUTinit: write PCI_BASE_ADDR0 adr=%h data=%h\n", PCI_BASE_ADDR0, DUT_BASE);
    verilog.cfg_wr( PCI_BASE_ADDR0,DUT_BASE, 4'b0000, 0, 1, rstatus, rflag);
    if ( rflag != 1'b0) {
	pci_error(rstatus);
	error("Cchip_chip_init: Writing Base Register failed\n");
    }

    verilog.cfg_rd( PCI_BASE_ADDR0,rdata, 4'b0000, 0, 1, rstatus, rflag, cflag);
    if ( rflag != 1'b0 ) {
	pci_error(rstatus);
	error("DUT_chip_init: reading BASE0 Register failed");
    }
    printf ("DUT_chip_init: Reading BASE0 value %h\n", rdata);
     
    printf ("DUT_chip_init: Setting Header Type, Latency Timer, Cache Line to value %h\n", 
            W_PCI_LATE|W_PCI_LINES);
    printf("Winit: write PCI_HDR_LAT_CACH adr=%h data=%h\n", PCI_HDR_LAT_CACH, W_PCI_LATE|W_PCI_LINES);
    verilog.cfg_wr( PCI_HDR_LAT_CACH,W_PCI_LATE|W_PCI_LINES, 4'b0000, 0, 1, rstatus, rflag);
    if ( rflag != 1'b0) {
	pci_error(rstatus);
	error("DUT_chip_init: Writing Header Type, Latency Timer, Cache Line failed\n");
    }

}


/*
 * Task to report what kind of PCI error occured
 */
void pci_error( bit [15:0] rstatus)
{
  printf("PCI ERROR: rstatus %h\n", rstatus );

  if ( rstatus[8] )
    printf("pci_error: Data parity error detected\n");
  
  if ( rstatus[11] ){
    printf("pci_error: Signaled Target Abort\n");
  }

  if ( rstatus[12] )
    printf("pci_error: Received Target Abort\n");
  
  if ( rstatus[13] )
    printf("pci_error: Received Master Abort\n");

  if ( rstatus[14] )
    printf("pci_error: Signaled System Error\n");

  if ( rstatus[15] )
    printf("pci_error: Detected Parity Error\n");

}
