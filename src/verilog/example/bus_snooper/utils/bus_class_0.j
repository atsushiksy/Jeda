/* 
   
   Copyright (C) 2001 Juniper Networks Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/
class processor_bus {
  signal clock ;
  signal command ;
  signal address ;
  signal cpu_valid ;
  signal sys_valid ;
  signal cpu_a_req ;
  signal cpu_a_gnt ;
  signal cpu_ack ;
  signal sys_ack ;
  signal data ;
  signal cpu_d_req ;
  signal cpu_d_gnt ;
  
  void new (
    signal a_clock,
    signal a_command,
    signal a_address,
    signal a_cpu_valid,
    signal a_sys_valid,
    signal a_cpu_a_req,
    signal a_cpu_a_gnt,
    signal a_cpu_ack,
    signal a_sys_ack,
    signal a_data,
    signal a_cpu_d_req,
    signal a_cpu_d_gnt
  )
  {
    clock = a_clock ;
    command = a_command ;
    address = a_address ;
    cpu_valid = a_cpu_valid ;
    sys_valid = a_sys_valid ;
    cpu_a_req = a_cpu_a_req ;
    cpu_a_gnt = a_cpu_a_gnt ;
    cpu_ack = a_cpu_ack ;
    sys_ack = a_sys_ack ;
    data = a_data;
    cpu_d_req = a_cpu_d_req ;
    cpu_d_gnt = a_cpu_d_gnt ;
  }
}


class system_bus {
  signal clock ;
  signal command ;
  signal address ;
  signal dev_id ;
  signal cmd_id ;
  signal shared_in ;
  signal shared_out ;
  signal owned_in ;
  signal owned_out ;
  signal a_req_in ;
  signal a_req_out ;
  signal ack ;
  signal ack_did ;
  signal ack_cid ;
  signal data ;
  signal d_req_in ;
  signal d_req_out ;
  
  void new (
    signal a_clock,
    signal a_command,
    signal a_address,
    signal a_dev_id,
    signal a_cmd_id,
    signal a_shared_in,
    signal a_shared_out,
    signal a_owned_in,
    signal a_owned_out,
    signal a_a_req_in,
    signal a_a_req_out,
    signal a_ack,
    signal a_ack_did,
    signal a_ack_cid,
    signal a_data,
    signal a_d_req_in,
    signal a_d_req_out
  ) {
    clock = a_clock ;
    command = a_command ;
    address = a_address ;
    dev_id = a_dev_id ;
    cmd_id = a_cmd_id ;
    shared_in = a_shared_in ;
    shared_out = a_shared_out ;
    owned_in = a_owned_in ;
    owned_out = a_owned_out ;
    a_req_in = a_a_req_in ;
    a_req_out = a_a_req_out ;
    ack = a_ack ;
    ack_did = a_ack_did ;
    ack_cid = a_ack_cid ;
    data = a_data ;
    d_req_in = a_d_req_in ;
    d_req_out = a_d_req_out ;
  }
  
}
