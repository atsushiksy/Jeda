/***************************************************************************
 *
 *  jd_main.c: main function for Jeda compler
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
   Copyright (C) 2003 Jeda Technologies, Inc.

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
/* #include <unistd.h> */
#include "../include/compiler_macro.h"
#include "../include/parse_types.h"
#include "../include/parse_tree.h"
#include "../include/error_msg.h"
#include "../include/symtab.h"

#ifdef PRODUCT_CODE
#include "lmpolicy.h"
#include "../include/license.h"
LP_HANDLE *JEDA_lp_handle ;
#endif

#define MAX_UTIL_NUM 256 

char *strrchr() ;

extern unsigned lex_line_num, lex_debug_line_num;
extern char *input_file_name  ;
extern char *last_text ;

int compile_pass ;
int jeda_util_parsing = 0 ;
int error_count = 0 ;
int max_error = 0 ;
int error_flag = 0 ;
int jeda_util_comp_flag = 0 ;

int RET ; /* used by bit_operation.c */

extern int yydebug;
extern FILE *yyin;  /* for lex input */

char *out_fname = NULL ;
char *in_fname = NULL ;
char *header_fname = NULL ;

int num_util = 0 ;
char *util_fname[MAX_UTIL_NUM] ;

int  header_out_flag = 0 ;
int  debug_flag = 0 ;

int  current_dir_flag = 0 ;

FILE *out ;
FILE *hout ;

char *current_dir = NULL ;

char *error_message ;
block_node *root_block ;

extern FILE *popen();


void jedapp_pipe( char *arg ) ;

extern void parse_init() ;

main ( int argc, char **argv ) {
  int i ;
  char *pparg = NULL ;
  char *arg ;
  int len ;

#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  {
    char *licdir ;
    char *homedir = (char *)getenv( JEDA_HOME ) ;
  
    if( homedir == NULL ) {
      fprintf( 
        stderr, 
        "Environment Variable $%s is not set. Using current dir\n", JEDA_HOME 
      ) ;
      homedir = "." ;
    }
    licdir = (char *)calloc( 256 + strlen( homedir ), sizeof(char) ) ;
    strcpy( licdir, homedir ) ;
    strcat( licdir, "/license/" ) ;
    if( 
      lp_checkout( 
        LPCODE, LM_RESTRICTIVE, LM_DEVELOP_FEATURE, LM_VERSION, 
        1, licdir, &JEDA_lp_handle
      )
    ) {
      lp_perror( JEDA_lp_handle, "Checkout failed" ) ;
      exit( 1 ) ;
    }
  }
#  endif
#endif
    
  for( i = 1 ; i < argc ; i++ ) {
  
    if( argv[i][0] != '-' ) {
      if( out_fname != NULL ) {
        fprintf( stderr, "Usage: %s [switches] input [output]\n", JDCMP  ) ;
        exit(1) ;
      }  
      else if( in_fname != NULL ) {
        out_fname = argv[i] ;
      }
      else {
        in_fname = argv[i] ;
      }
    }
    else {
      switch( argv[i][1] ) {
        case 'h':  /* -h option */
          header_out_flag = 1 ;
          break ;
        case 'g':  /* -g option */
          debug_flag = 1 ;
          break ;
        case 'C': /* -C (current dir) option */
          current_dir_flag = 1 ;
          break ;
        case 'D':
        case 'I':
          len = strlen(argv[i])+3 ;
          if( pparg != NULL ) len +=  strlen(pparg) ;
	  arg = (char *)calloc( len, sizeof(char) ) ;
          strcpy( arg, " " ) ;
          if( pparg != NULL ) {
            strcat( arg, pparg ) ;
            strcat( arg, " " ) ;
            free( pparg ) ;
	  }
	  strcat( arg, argv[i] ) ;
	  pparg = arg ;
	  break ;
	case 'X':
	  jeda_util_comp_flag = 1 ;
	  break ;
	case 'U':
	  if( num_util == MAX_UTIL_NUM ) {
            fprintf( 
              stderr, "Too many user util files, max = %d\n", MAX_UTIL_NUM 
            ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
           lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
           exit(1) ;
	  }
	  len = strlen(argv[i])+3 ;
	  util_fname[num_util] = (char *)calloc( len, sizeof(char) ) ;
	  strcat( util_fname[num_util], &argv[i][2] ) ;
	  num_util++ ;
	  break ;
      }
    }
    
  }

  input_file_name = "???"  ;
  last_text = NULL ;
  
  current_dir = (char *)getcwd( NULL, 1024 ) ; /* ( buf, size ) */
  
  if( in_fname == NULL ) {
    fprintf( stderr, "Usage: %s [switches] input [output]\n", JDCMP  ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  
  error_message = (char *)calloc( MAX_ERROR_MESSAGE, sizeof(char) ) ;
  
  /* 9/11/2002
    Ofer Lichit's contribution for better output filename construction 
  */
  if( out_fname == NULL ) {
    char *p = in_fname;

    if ( current_dir_flag ) {
      p = strrchr( in_fname, '/' );
      if( p != NULL ) p++;
      else p = in_fname;
    }

    out_fname = (char *)calloc( strlen(p)+10, sizeof(char) ) ;
    strcpy( out_fname, p ) ;

    p = strrchr( out_fname, '.' );
    if( p != NULL ) *p = '\0' ;

    strcat( out_fname, JDOEXT ) ;
    printf( "OUT_FILE: %s\n", out_fname ) ;
    
  }

  if( header_out_flag ) {
    char *p = in_fname;

    if ( current_dir_flag ) {
      p = strrchr( in_fname, '/' );
      if( p != NULL ) p++;
      else p = in_fname;
    }

    header_fname = (char *)calloc( strlen(p)+10, sizeof(char) ) ;
    strcpy( header_fname, p ) ;
    p = strrchr ( header_fname, '.' );
    if ( p != NULL ) *p = '\0' ;

    strcat( header_fname, JDHEXT ) ;
    printf( "HEADER_FILE: %s\n", header_fname ) ;    
  }
  
  jedapp_pipe( pparg ) ;
  
}


void jedapp_pipe( char *arg ) {
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
  
  len = strlen(in_fname) + 256 + strlen( homedir ) ;
  if( arg ) len += strlen(arg) ;
  
  command = (char *)calloc( len, sizeof(char) ) ;
  strcpy( command, homedir ) ;
  strcat( command, "/bin/" ) ;
  strcat( command, JDPP ) ;
  strcat( command, " -lang-c-c++-comments" );
  if( arg !=NULL ) strcat( command, arg ) ;
  strcat( command, " " ) ;
  strcat( command, in_fname ) ;
  yyin = popen( command, "r" ) ;
  if( !yyin ) {
    fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", command ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1);
  } 
  
  
  
  compile_pass = 0 ;
 
  init_scope() ; /* initialize symbol table */

#ifdef YYRESTART
       yyrestart( yyin ) ;  
       /* without calling this, # at the begining is not handled correctly. */
#endif

  yyparse() ;
  
  last_text = NULL ;
  close( yyin ) ;
  
  if( error_count == 0 ) {
    compile_pass = 1 ;
    
    parse_init() ;
    
    rewind_scope() ;
    
    if( !jeda_util_comp_flag ) {
      int i ;
      len = 256 + (2*strlen( homedir )) ;
      if( arg ) len += strlen(arg) ;
      ucommand = (char *)calloc( len, sizeof(char) ) ;
      strcpy( ucommand, homedir ) ;
      strcat( ucommand, "/bin/" ) ;
      strcat( ucommand, JDPP ) ;
      strcat( ucommand, " -lang-c-c++-comments" );
      if( arg !=NULL ) strcat( ucommand, arg ) ;
      strcat( ucommand, " " ) ;
      strcat( ucommand, homedir ) ;
      strcat( ucommand, "/include/" ) ;
      strcat( ucommand, "jeda_utils.jh" ) ;

      jeda_util_parsing = 1 ;
      yyin = popen( ucommand, "r" ) ;
      if( !yyin ) {
        fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", ucommand ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
        lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
        exit(1);
      }
#ifdef YYRESTART
         yyrestart( yyin ) ;  
         /* without calling this, # at the begining is not handled correctly. */
#endif
      yyparse() ; 
      close( yyin ) ;
      free( ucommand ) ;
      for ( i = 0 ; i < num_util ; i++ ) {
        len = strlen(util_fname[i]) + 256 + strlen( homedir ) ;
        if( arg ) len += strlen(arg) ;
        ucommand = (char *)calloc( len, sizeof(char) ) ;
        strcpy( ucommand, homedir ) ;
        strcat( ucommand, "/bin/" ) ;
        strcat( ucommand, JDPP ) ;
        strcat( ucommand, " -lang-c-c++-comments" );
        if( arg !=NULL ) strcat( ucommand, arg ) ;
        strcat( ucommand, " " ) ;
        strcat( ucommand, util_fname[i] ) ;
        yyin = popen( ucommand, "r" ) ;
        if( !yyin ) {
          fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", ucommand ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
          lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
          exit(1);
        }
#ifdef YYRESTART
         yyrestart( yyin ) ;  
         /* without calling this, # at the begining is not handled correctly. */
#endif
        yyparse() ; 
        close( yyin ) ;
        free( ucommand ) ;
      }
      jeda_util_parsing = 0 ;
    
    }
    last_text = NULL ;
    
    
    yyin = popen( command, "r" ) ;
    if( !yyin ) {
      fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", command ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
      lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
      exit(1);
    } 
    
    { 
      /*
      int c ;
      fprintf( stderr, "Compiler pass 1..\n", error_count ) ;
      while ((c=fgetc(yyin)) != EOF)
        fputc(c, stderr);
      */
      
#ifdef YYRESTART
       yyrestart( yyin ) ;  
       /* without calling this, # at the begining is not handled correctly. */
#endif

      yyparse() ; 
    }
    close( yyin ) ;
  }
  last_text = NULL ;

  if( error_count == 0 ) {
    out = fopen( out_fname, "w" ) ;
    if( out == NULL ) {
      fprintf( 
        stderr, "%s : cannot open output file %s\n", JDCMP, out_fname
      );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
      lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
      exit(1) ;
    }
    compile_pass = 2 ;

    rewind_scope() ;
    
    if( !jeda_util_comp_flag ) {
      int i ;
      len = 256 + (2*strlen( homedir )) ;
      if( arg ) len += strlen(arg) ;
      ucommand = (char *)calloc( len, sizeof(char) ) ;
      strcpy( ucommand, homedir ) ;
      strcat( ucommand, "/bin/" ) ;
      strcat( ucommand, JDPP ) ;
      strcat( ucommand, " -lang-c-c++-comments" );
      if( arg !=NULL ) strcat( ucommand, arg ) ;
      strcat( ucommand, " " ) ;
      strcat( ucommand, homedir ) ;
      strcat( ucommand, "/include/" ) ;
      strcat( ucommand, "jeda_utils.jh" ) ;

      jeda_util_parsing = 1 ;
      yyin = popen( ucommand, "r" ) ;
      if( !yyin ) {
        fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", ucommand ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
        lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
        exit(1);
      }
#ifdef YYRESTART
         yyrestart( yyin ) ;  
         /* without calling this, # at the begining is not handled correctly. */
#endif
      yyparse() ; 
      close( yyin ) ;
      free( ucommand ) ;
      for ( i = 0 ; i < num_util ; i++ ) {
        len = strlen(util_fname[i]) + 256 + strlen( homedir ) ;
        if( arg ) len += strlen(arg) ;
        ucommand = (char *)calloc( len, sizeof(char) ) ;
        strcpy( ucommand, homedir ) ;
        strcat( ucommand, "/bin/" ) ;
        strcat( ucommand, JDPP ) ;
        strcat( ucommand, " -lang-c-c++-comments" );
        if( arg !=NULL ) strcat( ucommand, arg ) ;
        strcat( ucommand, " " ) ;
        strcat( ucommand, util_fname[i] ) ;
        yyin = popen( ucommand, "r" ) ;
        if( !yyin ) {
          fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", ucommand ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
          lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
          exit(1);
        }
#ifdef YYRESTART
         yyrestart( yyin ) ;  
         /* without calling this, # at the begining is not handled correctly. */
#endif
        yyparse() ; 
        close( yyin ) ;
        free( ucommand ) ;
      }
      jeda_util_parsing = 0 ;
    
    }
    last_text = NULL ;

    yyin = popen( command, "r" ) ;
    if( !yyin ) {
      fprintf( stderr, "Error: Can't open pipe on \"%s\"\n", command ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
      lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
      exit(1);
    } 
    
    { 
      /*
      int c ;
      fprintf( stderr, "Compiler pass 1..\n", error_count ) ;
      while ((c=fgetc(yyin)) != EOF)
        fputc(c, stderr);
      */
      
#ifdef YYRESTART
       yyrestart( yyin ) ;  
       /* without calling this, # at the begining is not handled correctly. */
#endif

      yyparse() ; 
    }
    close( yyin ) ;
  }
  
  
  free( command ) ;
  
  if( error_count == 0 ) {
    codegen() ;
    fprintf( stderr, " %d compile errors\n", error_count ) ;
   
    
    if( header_out_flag ) {
      hout = fopen( header_fname, "w" ) ;
      if( hout == NULL ) {
       fprintf( 
          stderr, "%s : cannot open header output file %s\n", 
          JDCMP, header_fname
        );
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
        lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
        exit(1) ;
      }
      headergen() ; 
    }
    
  }
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
  if( error_count ) {
    exit(1) ;
  }
  else {
    exit(0) ;
  }
  
}

int yyerror( char *message ) 
{
  int last = 0 ;
  
  if( last_text == NULL ) {
    last_text = "UNKNOWN" ;
    last = 1 ;
  }
  
  if( compile_pass == 0 )
    fprintf(
      stderr, "File: \"%s\", line %d near %s : ", 
      input_file_name, lex_line_num, last_text
    );
    
  fprintf(
    stderr, "%s\n", 
    message
  );
  error_count++ ;
  
  error_flag = 1 ;
  
  if( max_error && error_count > max_error ) {
    exit(1) ;
  }
  if( last ) last_text = NULL ;
  
}

int yywarning( char *message ) 
{
  int last = 0 ;
  
  if( last_text == NULL ) {
    last_text = "UNKNOWN" ;
    last = 1 ;
  }

  if( compile_pass == 0 )
    fprintf(
      stderr, "File: \"%s\", line %d near %s : ", 
      input_file_name, lex_line_num, last_text
    );

  fprintf(
    stderr, "%s\n", 
    message
  );

  if( last ) last_text = NULL ;
  
}
