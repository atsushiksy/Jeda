/*
 *  Copyright (c) 2001 Atsushi Kasuya
 *  All right reserved.
 * 
 *  Redistribution and use in source and binary forms are permitted
 *  provided that this notice is preserved and that due credit is
 *  given to Atsushi Kasuya. 
 *  This software is provided "as is" without express or implied warranty.
 *
 */
#include "../include/cpu_bus.jh"
#include "../include/system_bus.jh"
#include "../include/sys_defines.jh"
#include "../models/bus_snooper_model.jh"
#include "../utils/bus_class_0.jh"
#include "../utils/cpu_bus_driver_1.jh"
#include "../utils/sys_bus_driver_1.jh"
#include "../utils/bus_transactions_2.jh"

#include "../include/global_var.jh"

#define HALF_SYSCYCLE_PERIOD  50

extern void snooper_test() ;

void main()
{
  processor_bus cpu_bus ;
  system_bus sys_bus ;
  bus_snooper dut ;
  
  
  cpu_bus = 
    new (
      CPU_BUS.clock,
      CPU_BUS.command,
      CPU_BUS.address,
      CPU_BUS.cpu_valid,
      CPU_BUS.sys_valid,
      CPU_BUS.cpu_a_req,
      CPU_BUS.cpu_a_gnt,
      CPU_BUS.cpu_ack,
      CPU_BUS.sys_ack,
      CPU_BUS.data,
      CPU_BUS.cpu_d_req,
      CPU_BUS.cpu_d_gnt
    ) ;
  
  sys_bus =
    new (
      SYS_BUS.clock,
      SYS_BUS.command,
      SYS_BUS.address,
      SYS_BUS.dev_id,
      SYS_BUS.cmd_id,
      SYS_BUS.shared_in,
      SYS_BUS.shared_out,
      SYS_BUS.owned_in,
      SYS_BUS.owned_out,
      SYS_BUS.a_req_in,
      SYS_BUS.a_req_out,
      SYS_BUS.ack,
      SYS_BUS.ack_did,
      SYS_BUS.ack_cid,
      SYS_BUS.data,
      SYS_BUS.d_req_in,
      SYS_BUS.d_req_out
    ) ;
    
  printf( "Start driving clock signals\n" ) ;
  
  CLOCK.drive_clock( 
    HALF_SYSCYCLE_PERIOD, HALF_SYSCYCLE_PERIOD, 0,       0 
   // period1,            period2,              initial, dly
  ) ; 
  SYS_BUS.clock.drive_clock( 
    HALF_SYSCYCLE_PERIOD, HALF_SYSCYCLE_PERIOD, 0, 0 
  ) ; 
  CPU_BUS.clock.drive_clock( 
    HALF_SYSCYCLE_PERIOD, HALF_SYSCYCLE_PERIOD, 0, 0 
  ) ; 
  
  fork
  forever {
    SYS_BUS.command.0 = 0 weak ;
    SYS_BUS.address.0 = 0 weak ;
    SYS_BUS.dev_id.0 = 0 weak ;
    SYS_BUS.cmd_id.0 = 0 weak ;
    SYS_BUS.shared_in.0 = 0 weak ;
    SYS_BUS.shared_out.0 = 0 weak ;
    SYS_BUS.owned_in.0 = 0 weak ;
    SYS_BUS.owned_out.0 = 0 weak ;
    SYS_BUS.a_req_in.0 = 0 weak ;
    SYS_BUS.a_req_out.0 = 0 weak ;
    SYS_BUS.ack.0 = 0 weak ;
    SYS_BUS.ack_did.0 = 0 weak ;
    SYS_BUS.ack_cid.0 = 0 weak ;
    SYS_BUS.data.0 = 0 weak ;
    SYS_BUS.d_req_in.0 = 0 weak ;
    SYS_BUS.d_req_out.0 = 0 weak ;
    @(posedge SYS_BUS.clock) ;
  }
  forever {
    CPU_BUS.command.0 = 0 weak ;
    CPU_BUS.address.0 = 0 weak ;
    CPU_BUS.data.0 = 0 weak ;
    CPU_BUS.cpu_valid.0 = 0 weak ;
    CPU_BUS.sys_valid.0 = 0 weak ;
    CPU_BUS.cpu_a_req.0 = 0 weak ;
    CPU_BUS.cpu_a_gnt.0 = 0 weak ;
    CPU_BUS.cpu_ack.0 = 0 weak ;
    CPU_BUS.sys_ack.0 = 0 weak ;
    CPU_BUS.cpu_d_req.0 = 0 weak ;
    CPU_BUS.cpu_d_gnt.0 = 0 weak ;
    @(posedge CPU_BUS.clock) ;
  }
  knife

  dut = new( 0, sys_bus, cpu_bus ) ;
  
  dut.start_all_loops() ;
  
  bus_drive = new( 0, cpu_bus, sys_bus ) ;
  
  printf( "calling snooper_test()\n" ) ;
  
  // call common test task
  snooper_test() ;
  
  printf( "snooper_test() done.\n" ) ;
  
}
