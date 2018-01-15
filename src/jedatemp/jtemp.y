/*
 *  Jeda Template Generator
 *
 *  The yacc part of partial (quick & dirty) verilog parser. 
 *
 *  Author: Atsushi Kasuya
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
%{

#include <stdio.h>
#include "jtemp.h"

module *top_module = NULL ;

%}

/* YYSTYPE */
%union {
    int             ival;
    char            *charp;
    io_port *port ;
    module  *mod ;
    struct {
      int ub ;
      int lb ;
    } range ;
    name_list  *namel ;
}

/* starting symbol */
%start source_text

/* tokens */
%token <charp> YYID                      /* verilog user defined identifier */
%token <ival>  YYINUMBER                  /* integer */
%token <charp> YYRNUMBER                 /* real */
%token <charp> YYSTRING                  /* string quoted by "" */
%token <charp> YYENDMODULE               /* endmodule */
%token <charp> YYMODULE                  /* module */
%token <charp> YYINOUT                   /* inout */
%token <charp> YYINPUT                   /* input */
%token <charp> YYOUTPUT                  /* output */
%token <charp> YYPRIMITIVE               /* primitive */
%token <charp> YYENDPRIMITIVE            /* endprimitive */
%token <charp> YYOTHER                   /* any other operators */
%token <charp> '('
%token <charp> ')'
%token <charp> '+'
%token <charp> '-'
%token <charp> '*'
%token <charp> '/'

%type <mod> source_text description module primitive
%type <port>   opt_module_items  module_items module_item
%type <port>   input_declaration output_declaration inout_declaration others
%type <port>   port_list_opt port port_list port_reference
%type <range>  range range_opt
%type <ival>   constant

%type <charp>  port_ref_list port_reference_arg identifier any

%type <namel>  variable_list

/* precedence table */
%left '-' '+'
%left  '*' '/'

%%

source_text
	: /* empty */ { $$ = NULL ; }
        | source_text description 
        {
          module *mp ;
          if( $1 ) {
            mp = $1 ;
            while( mp->next ) mp = mp->next ;
            mp->next = $2 ;
            $$ = $1 ;
          }
          else $$ = $2 ;
          top_module = $$ ;
        }
        ;

description 
	: module     { $$ = $1 ; }
	| primitive  { $$ = $1 ; }
	;


module 
	: YYMODULE YYID 
          port_list_opt ';'
          opt_module_items
	  YYENDMODULE
	{
	  $$ = (module *)calloc( 1, sizeof(module) ) ;
	  $$->name = (char *)strdup($2) ;
	  $$->port = $5 ;
	}
        ;
        
port_list_opt
	:  { $$ = 0 ; }
	| '(' port_list ')' { $$ = NULL ; }
	;

port_list
	: port  { $$ = NULL ; }
	| port_list ',' port { $$ = NULL ; }
	;

port
	: port_reference  { $$ = NULL ; }
	| '{' port_ref_list '}'  { $$ = NULL ; } 
        ;

port_ref_list
	: port_reference  { $$ = NULL ; }
	| port_ref_list ',' port_reference  { $$ = NULL ; }
	;

port_reference
	: YYID { $$ = NULL ; }
          port_reference_arg { $$ = NULL ; }
	;

port_reference_arg
        :
        { $$ = NULL ; }
        | '[' YYINUMBER ']' { $$ = NULL ; }
        | '[' YYINUMBER ':' YYINUMBER ']' { $$ = NULL ; }
        ;

opt_module_items
	:  /* empty */  { $$ = NULL ; }
	|  module_items  { $$ = $1 ; }
	;
	
module_items
        : module_item { $$ = $1 ; }
        | module_items module_item
        {
          io_port *pp ;
          if( $1 ) {
            pp = $1 ;
            while( pp->next ) pp = pp->next ;
            pp->next = $2 ;
            $$ = $1 ;
          }
          else $$ = $2 ;
        }
        ;

module_item
	: input_declaration   { $$ = $1 ; }
	| output_declaration  { $$ = $1 ; }
	| inout_declaration   { $$ = $1 ; }
	| others              { $$ = $1 ; }
	;


input_declaration
        : YYINPUT range_opt variable_list ';'
        { 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( $3 ) {
            namel = $3 ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 0 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = $2.ub ;
              port->lb = $2.lb ;
              namel = namel->next ;
            }
          }
          $$ = i_port ;
        }
        ;

output_declaration
        : YYOUTPUT range_opt variable_list ';'
        { 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( $3 ) {
            namel = $3 ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 1 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = $2.ub ;
              port->lb = $2.lb ;
              namel = namel->next ;
            }
          }
          $$ = i_port ;
        }
        ;

inout_declaration
        : YYINOUT range_opt variable_list ';'
        { 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( $3 ) {
            namel = $3 ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 2 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = $2.ub ;
              port->lb = $2.lb ;
              namel = namel->next ;
            }
          }
          $$ = i_port ;
        }
        ;

range_opt
        :
        { $$.ub = $$.lb = -1 ; }
        | range
        { $$ = $1 ; }
        ;

range
        : '[' constant ':' constant ']'
        {
          $$.ub = $2 ;
          $$.lb = $4 ;
        }
        ;

constant
	: YYINUMBER
	{
	  $$ = $1 ;
	}
	| constant '+' constant
	{
	  $$ = $1 + $3 ;
	}
	| constant '-' constant
	{
	  $$ = $1 - $3 ;
	}
	| constant '*' constant
	{
	  $$ = $1 * $3 ;
	}
	| constant '/' constant
	{
	  $$ = $1 / $3 ;
	}
	| '(' constant ')'
	{
	  $$ = $2 ;
	}
	;
	
variable_list
        : identifier
        {
          if( $1 ) {
            $$ = (name_list *)calloc( 1, sizeof(name_list) ) ;
            $$->name = (char *)strdup($1) ;
          }
          else $$ = NULL ;
        }
        | variable_list ',' identifier
        {
          name_list  *namel ;
          $$ = $1 ;
          if( $3 ) {
            namel = $$ ;
            while( namel->next ) namel = namel->next ;
            namel->next = (name_list *)calloc( 1, sizeof(name_list) ) ;
            namel->next->name = (char *)strdup($3) ;
          }
        }
        ;

identifier
        : YYID  { $$ = $1 } ;
        | identifier '.' YYID { $$ = NULL ; }
        ;
	
primitive
	: YYPRIMITIVE YYID
	  opt_module_items
	  YYENDPRIMITIVE
	  { $$ = NULL ; }
	;
		
others
	: any { $$ = NULL ; }
	| others any { $$ = NULL ; }
	;
	
any	:  YYOTHER { $$ = NULL ; }
	|  YYID  { $$ = NULL ; }
	|  YYINUMBER { $$ = NULL ; }
	|  YYRNUMBER { $$ = NULL ; }
	|  YYSTRING { $$ = NULL ; }
	|  '(' { $$ = NULL ; }
	|  ')' { $$ = NULL ; }
	|  '[' { $$ = NULL ; }
	|  ']' { $$ = NULL ; }
	|  '{' { $$ = NULL ; }
	|  '}' { $$ = NULL ; }
	|  '.' { $$ = NULL ; }
	|  ',' { $$ = NULL ; }
	|  ':' { $$ = NULL ; }
	|  ';' { $$ = NULL ; }
	;


%%

static void gen_port_inc( FILE *out, io_port *clock, module *mod ) {
  io_port *pt ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// portset file for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT0( "#ifndef DRIVE_SPEC\n" ) ;
  OUT0( "#define DRIVE_SPEC drive posedge skew 1\n" ) ;
  OUT0( "#endif\n" ) ;
  OUT0( "#ifndef SAMPLE_SPEC\n" ) ;
  OUT0( "#define SAMPLE_SPEC sample posedge \n" ) ;
  OUT0( "#endif\n" ) ;  
  OUT1( "portset %s ", mod->name ) ;
  OUT0( "{\n" ) ;
  pt = mod->port ;
  while( pt ) {
    switch( pt->dir ) {  /* 0:input 1:output 2: inout -> to be reversed */
      case 0: OUT0( "  output  " ) ; break ;
      case 1: OUT0( "  input " ) ; break ;
      case 2: OUT0( "  inout  " ) ; break ;
    }
    if( pt->ub >= 0 && pt->lb >= 0 ) {
      OUT2( "[%d:%d] ", pt->ub, pt->lb ) ;
    }
    OUT1( "%s ", pt->name ) ;
    switch( pt->dir ) {  /* 0:input 1:output 2: inout */
      case 0: OUT0( "DRIVE_SPEC " ) ; break ;
      case 1: OUT0( "SAMPLE_SPEC " ) ; break ;
      case 2: OUT0( "DRIVE_SPEC SAMPLE_SPEC " ) ; break ;
    }
    if( clock && strcmp( clock->name, pt->name) ) 
      OUT2( " @ %s.%s ", mod->name, clock->name ) ;
    OUT0( " ;\n" ) ;
    pt = pt->next ;
  }
  OUT0( "}\n" ) ;
}

static void gen_jeda_temp( FILE *out, module *mod ) {
  io_port *pt ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// Jeda template for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "#include \"%s_port.jh\"\n", mod->name ) ;
  OUT0( "\n" ) ;
  OUT0( "main() {\n" ) ;
  OUT1( "  // begining of the program for module %s\n", mod->name ) ;
  OUT0( "}\n" ) ;
  OUT0( "\n" ) ;
}


static void gen_verilog( FILE *out, io_port *clock, module *mod, char *inc_file ) {
  io_port *pt ;
  int first ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// verilog testbench for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "module %s_test ; \n", mod->name ) ;
  OUT0( "  parameter simulation_cycle = 100 ;\n" ) ;
  OUT0( "  wire CLOCK ;\n" ) ;
  OUT0( "  reg CLOCK_reg ;\n" ) ;
  OUT0( "  assign CLOCK = CLOCK_reg ;\n" ) ;
  pt = mod->port ;
  while( pt ) {
    OUT0( "  wire  " ) ;
    if( pt->ub >= 0 && pt->lb >= 0 ) {
      OUT2( "[%d:%d] ", pt->ub, pt->lb ) ;
    }
    
    OUT2( " %s_%s ;\n", mod->name, pt->name ) ;
    pt = pt->next ;
  }
  if( clock )
    OUT2( "  assign %s_%s = CLOCK ;\n", mod->name, clock->name ) ;
    
  OUT0( "  // Jeda module \n" ) ;
  OUT0( "  jeda_module jeda (\n" ) ;
  OUT0( "    .CLOCK(CLOCK)" ) ;
  pt = mod->port ;
  while( pt ) {
    OUT0( ",\n" ) ;
    OUT4( "    .%s_%s(%s_%s)", mod->name, pt->name, mod->name, pt->name ) ;
    pt = pt->next ;
  }
  OUT0( "\n  ) ; // end of jeda_module\n" ) ;
  
  OUT1( "  // module %s under test\n", mod->name ) ;
  OUT1( "  %s dut (\n", mod->name ) ;
  pt = mod->port ;
  first = 1 ;
  while( pt ) {
    if( first ) first = 0 ;
    else OUT0( ",\n" ) ;
    OUT3( "    .%s(%s_%s)", pt->name, mod->name, pt->name ) ;
    pt = pt->next ;
  }
  OUT0( "\n  ) ;\n\n" ) ;
  
  OUT0( "  // CLOCK toggling\n" ) ;
  OUT0( "  initial begin\n" ) ;
  OUT0( "    CLOCK_reg = 0 ;\n" ) ;
  OUT0( "    forever begin\n" ) ;
  OUT0( "      #(simulation_cycle/2) CLOCK_reg = ~CLOCK_reg ;\n" ) ;
  OUT0( "    end\n" ) ;
  OUT0( "  end\n\n" ) ;
  
  if( inc_file ) { 
    OUT1( "`include \"%s\"\n\n", inc_file ) ;
  }
  
  OUT0( "endmodule\n" ) ;
    
}

void gen_output( char *name, char *cname, char *inc_fname ) {
  module *mod ;
  io_port *clock ;
  char *port_fname, *veri_fname, *jeda_fname ;
  FILE *pout, *vout, *jout ;
  
  mod = top_module ;
  clock = NULL ;
  
  if( name  ) {
    while( mod ) {
      if( !strcmp(mod->name, name) ) break ;
      mod = mod->next ;
    }
    if( !mod ) {
      fprintf( stderr, "Can't find module name \"%s\"\n", name ) ;
      exit(1) ;
    }
  }
  else name = mod->name ;
  
  if( cname ) {
    clock = mod->port ;
    while( clock ) {
      if( !strcmp(clock->name, cname) ) break ;
      clock = clock->next ;
    }
    if( !clock ) {
      fprintf( stderr, "Can't find clock name \"%s\"\n", cname ) ;
      exit(1) ;
    }
  }
  
  port_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( port_fname, name ) ;
  strcat( port_fname, "_port.jh" ) ;
  pout = fopen( port_fname, "w" ) ;
  if( pout == NULL ) {
    fprintf( stderr, "Can't open port header file \"%s\"\n", port_fname ) ;
    exit(1) ;
  }  
  veri_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( veri_fname, name ) ;
  strcat( veri_fname, "_test.v" ) ;
  vout = fopen( veri_fname, "w" ) ;
  if( vout == NULL ) {
    fprintf( stderr, "Can't open verilog file \"%s\"\n", veri_fname ) ;
    exit(1) ;
  }  
  jeda_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( jeda_fname, name ) ;
  strcat( jeda_fname, "_test_template.j" ) ;
  jout = fopen( jeda_fname, "w" ) ;
  if( jout == NULL ) {
    fprintf( stderr, "Can't open jeda template file \"%s\"\n", jeda_fname ) ;
    exit(1) ;
  }  
  
  gen_port_inc( pout, clock, mod ) ;
  close( pout ) ;
  printf( "  Port file \"%s\" created\n", port_fname ) ;
  gen_jeda_temp( jout, mod ) ;
  close( jout ) ;
  printf( "  Jeda template file \"%s\" created\n", jeda_fname ) ;
  gen_verilog( vout, clock, mod, inc_fname ) ;
  close( vout ) ;
  printf( "  Verilog testbench file \"%s\" created\n", veri_fname ) ;
}
