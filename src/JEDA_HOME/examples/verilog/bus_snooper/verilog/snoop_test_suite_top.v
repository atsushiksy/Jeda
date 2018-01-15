/////////////////////////////////////////////////////////////////
// Verilog top file for Jeda Demo 
/////////////////////////////////////////////////////////////////
module jeda_test_top;
  wire CLOCK;
  tri0 CPU_BUS_clock = CLOCK ;
  tri0 [3:0] CPU_BUS_command;
  tri0 [39:0] CPU_BUS_address;
  tri0 CPU_BUS_cpu_valid;
  tri0 CPU_BUS_sys_valid;
  tri0 CPU_BUS_cpu_a_req;
  tri0 CPU_BUS_cpu_a_gnt;
  tri0 [2:0] CPU_BUS_cpu_ack;
  tri0 [2:0] CPU_BUS_sys_ack;
  tri0 [511:0] CPU_BUS_data;
  tri0 CPU_BUS_cpu_d_req;
  tri0 CPU_BUS_cpu_d_gnt;
  tri0 SYS_BUS_clock = CLOCK ;
  tri0 [3:0] SYS_BUS_command;
  tri0 [39:0] SYS_BUS_address;
  tri0 [2:0] SYS_BUS_dev_id;
  tri0 [2:0] SYS_BUS_cmd_id;
  tri0 [6:0] SYS_BUS_shared_in;
  tri0 SYS_BUS_shared_out;
  tri0 [6:0] SYS_BUS_owned_in;
  tri0 SYS_BUS_owned_out;
  tri0 [6:0] SYS_BUS_a_req_in;
  tri0 SYS_BUS_a_req_out;
  tri0 [1:0] SYS_BUS_ack;
  tri0 [2:0] SYS_BUS_ack_did;
  tri0 [2:0] SYS_BUS_ack_cid;
  tri0 [511:0] SYS_BUS_data;
  tri0 [6:0] SYS_BUS_d_req_in;
  tri0 SYS_BUS_d_req_out;

  // Jeda Module Instance
  jeda_module jeda (
    .CLOCK(CLOCK),
    .CPU_BUS_clock(CPU_BUS_clock),
    .CPU_BUS_command(CPU_BUS_command),
    .CPU_BUS_address(CPU_BUS_address),
    .CPU_BUS_cpu_valid(CPU_BUS_cpu_valid),
    .CPU_BUS_sys_valid(CPU_BUS_sys_valid),
    .CPU_BUS_cpu_a_req(CPU_BUS_cpu_a_req),
    .CPU_BUS_cpu_a_gnt(CPU_BUS_cpu_a_gnt),
    .CPU_BUS_cpu_ack(CPU_BUS_cpu_ack),
    .CPU_BUS_sys_ack(CPU_BUS_sys_ack),
    .CPU_BUS_data(CPU_BUS_data),
    .CPU_BUS_cpu_d_req(CPU_BUS_cpu_d_req),
    .CPU_BUS_cpu_d_gnt(CPU_BUS_cpu_d_gnt),
    .SYS_BUS_clock(SYS_BUS_clock),
    .SYS_BUS_command(SYS_BUS_command),
    .SYS_BUS_address(SYS_BUS_address),
    .SYS_BUS_dev_id(SYS_BUS_dev_id),
    .SYS_BUS_cmd_id(SYS_BUS_cmd_id),
    .SYS_BUS_shared_in(SYS_BUS_shared_in),
    .SYS_BUS_shared_out(SYS_BUS_shared_out),
    .SYS_BUS_owned_in(SYS_BUS_owned_in),
    .SYS_BUS_owned_out(SYS_BUS_owned_out),
    .SYS_BUS_a_req_in(SYS_BUS_a_req_in),
    .SYS_BUS_a_req_out(SYS_BUS_a_req_out),
    .SYS_BUS_ack(SYS_BUS_ack),
    .SYS_BUS_ack_did(SYS_BUS_ack_did),
    .SYS_BUS_ack_cid(SYS_BUS_ack_cid),
    .SYS_BUS_data(SYS_BUS_data),
    .SYS_BUS_d_req_in(SYS_BUS_d_req_in),
    .SYS_BUS_d_req_out(SYS_BUS_d_req_out)
  ) ;


  /* Example of vcd dump call */
`ifdef DUMP_VCD
  initial begin 
    $dumpfile("jeda_test.dump");
    $dumpvars(0,jeda_test_top);
  end
  /* */
`endif

`ifdef DUMP_DEBUSSY
  initial begin
    $fsdbDumpvars(0, jeda_test_top);
  end
`endif

endmodule
