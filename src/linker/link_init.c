/*
 * Initialize Linker
 */
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"
#include "../include/codegen_macro.h"

extern FILE *out ;

JD_sys_class_dcl() {
  OUT0( "void *JD_sa_event() ;\n" ) ;
  OUT0( "void JD_sd_event() ;\n" ) ;
  OUT0( "void JD_dup_event() ;\n" ) ;
  OUT0( "void *JD_sa_regexp() ;\n" ) ;
  OUT0( "void JD_sd_regexp() ;\n" ) ;
  OUT0( "void JD_dup_regexp() ;\n" ) ;
  OUT0( "void *JD_sa_file() ;\n" ) ;
  OUT0( "void JD_sd_file() ;\n" ) ;
  OUT0( "void JD_dup_file() ;\n" ) ;
  OUT0( "void *JD_sa_random_ucb() ;\n" ) ;
  OUT0( "void JD_sd_random_ucb() ;\n" ) ;
  OUT0( "void JD_dup_random_ucb() ;\n" ) ;
  OUT0( "void *JD_sa_random_48() ;\n" ) ;
  OUT0( "void JD_sd_random_48() ;\n" ) ;
  OUT0( "void JD_dup_random_48() ;\n" ) ;
  OUT0( "void *JD_sa_random_lsfr() ;\n" ) ;
  OUT0( "void JD_sd_random_lsfr() ;\n" ) ;
  OUT0( "void JD_dup_random_lsfr() ;\n" ) ;
  OUT0( "void *JD_sa_semaphore() ;\n" ) ;
  OUT0( "void JD_sd_semaphore() ;\n" ) ;
  OUT0( "void JD_dup_semaphore() ;\n" ) ;
}

static void event_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "event" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "trigger", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "trigger_on", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "trigger_pulse", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "sync", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "sync_clear", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "check", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "check_clear", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "timeout", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void port_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "port" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "pointer", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "drive_clock", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "drive_back", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "expect", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
}

static void signal_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "signal" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "drive", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "drive_clock", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "drive_back", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "expect", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
}

static void array_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "array" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "dim_size", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "num_element", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void assoc_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "assoc" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "num_element", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "index_array", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "check_index", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "delete_index", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "flush_all", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void stridx_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "stridx" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "num_element", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "index_array", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "check_index", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "delete_index", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "flush_all", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void list_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "list" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "push", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "bpush", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "pop", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "bpop", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "sort", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "num_element", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "send", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "receive", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "enter", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "check_enter", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "exit", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "timeout", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void string_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "string" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "sprintf", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "search", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "substring", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "length", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "atoi", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "atobin", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "atooct", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "atohex", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "itoa", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "append", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "chop", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void regexp_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "regexp" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "match", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "prematch", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "postmatch", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "thismatch", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "backref", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void thread_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "thread" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "start", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "stop", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "suspend", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "resume", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "wait", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "call", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "stay", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void file_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "file" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "open", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "rewind", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "close", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "fprintf", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "freadb", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "freadh", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "freadd", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "freado", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void random_ucb_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "random_ucb" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "new", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "random", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "urandom", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void random_48_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "random_48" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "new", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "random", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "urandom", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void random_lsfr_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "random_lsfr" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "new", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
  name = add_name_to_scope( "random", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void semaphore_init() {
  JD_name *sname ;
  JD_name *name ;
  sname = add_global_name( "semaphore" ) ;
  sname->type = JD_sys_class_name ;
  
  name = add_name_to_scope( "new", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "get", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "check", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "put", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
  
  name = add_name_to_scope( "timeout", &sname->child ) ;
  name->type = JD_sys_mfunc_name ;
    
}

static void funcs_init() {
  JD_name *name ;
  
  name = add_global_name( "printf" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "error" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "warning" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "expect" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "random" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "urandom" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "rand48" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "urand48" ) ;
  name->type = JD_sys_func_name ;

  name = add_global_name( "unit_delay" ) ;
  name->type = JD_sys_func_name ;

}

void link_parser_init() {
  /*
  event_init() ;
  port_init() ;
  signal_init() ;
  array_init() ;
  assoc_init() ;
  stridx_init() ;
  list_init() ;
  string_init() ;
  regexp_init() ;
  thread_init() ;
  file_init() ;
  random_ucb_init() ;
  random_48_init() ;
  random_lsfr_init() ;
  semaphore_init() ;
  funcs_init() ;
  */
}
