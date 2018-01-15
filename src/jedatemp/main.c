/***************************************************************************
 *
 *  main.c: main function for jeda template generator
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
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
#include <stddef.h>
/* #include <errno.h>
#include <stdlib.h>
#include <time.h> */
#include "../include/compiler_macro.h"

int error_count = 0 ;
int max_error = 0 ;
int error_flag = 0 ;
int inline_optimization = 0 ;

int parse_sys_utils = 0 ;

extern int yydebug;
extern FILE *yyin;  /* for lex input */

char *current_dir = NULL ;

char *error_message ;

extern FILE *popen();

void gen_output( char *name, char *cname, char *ifname ) ;

char *in_fname = NULL ;
char *veri_out_fname = NULL ;
char *port_out_fname = NULL ;
char *temp_out_fname = NULL ;
char *module_name = NULL ;
char *clock_name = NULL ;
char *inc_fname = NULL ;

usage() {
  fprintf( stderr, "Usage: jedatemp [switches] input\n"  ) ;
  fprintf( stderr, "  -m module_name\n"  ) ;
  fprintf( stderr, "  -c clock_name\n"  ) ;
  fprintf( stderr, "  -i include_file\n"  ) ;
  fprintf( stderr, "  -h print usage\n"  ) ;
}

main ( int argc, char **argv ) {
  int i ;
  char *command ;
  char *ucommand ;
  int len ;
  char *homedir = (char *)getenv( JEDA_HOME ) ;
  
  if( homedir == NULL ) {
    fprintf( 
      stderr, 
      "Environment Variable $%s is not set. Using current dir\n", JEDA_HOME 
    ) ;
    homedir = "." ;
  }

    
  for( i = 1 ; i < argc ; i++ ) {
  
    if( argv[i][0] != '-' ) {
      if( in_fname != NULL ) {
        usage() ;
        exit(1) ;
      }  
      else {
        in_fname = argv[i] ;
      }
    }
    else {
      switch( argv[i][1] ) {
        case 'm':  /* -m option */
          module_name = argv[++i] ;
          break ;
        case 'c':  /* -g option */
          clock_name = argv[++i] ;
          break ;
	case 'i':
          inc_fname = argv[++i] ;
	  break ;
	case 'h':
	  usage() ;
	  exit(0) ;
	  break ;
      }
    }
    
  }

  current_dir = (char *)getcwd( NULL, 1024 ) ; /* ( buf, size ) */
  
  if( in_fname == NULL ) {
    usage() ;
    exit(1) ;
  }
  
  len = strlen(in_fname) + 256 + strlen( homedir ) ;
  
  command = (char *)calloc( len, sizeof(char) ) ;
  strcpy( command, homedir ) ;
  strcat( command, "/bin/vpp " ) ;
  strcat( command, in_fname ) ;
  yyin = popen( command, "r" ) ;
  if( !yyin ) {
    fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", command ) ;
    exit(1);
  } 
 

  yyparse() ;
  
  close( yyin ) ;
  
  
  
  if( error_count == 0 ) {
    gen_output( module_name, clock_name, inc_fname ) ;
    exit(0) ;
  }
  else exit(1) ;
  
}

extern unsigned lex_line_num ;
extern char *last_text ;

int yyerror( char *message ) 
{
  int last = 0 ;
  
  if( last_text ) {
    fprintf(
      stderr, "%s near %s (%d)\n", 
      message, last_text, lex_line_num
    );
  }
  else {
    fprintf(
      stderr, "%s at %d\n", 
      message, lex_line_num
    );
  }
  error_count++ ;
  
  error_flag = 1 ;
  
  if( max_error && error_count > max_error ) {
    exit(1) ;
  }
  
}
