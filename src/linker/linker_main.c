/***************************************************************************
 *
 *  linker_main.c: main function for Jeda linker
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
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

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"

#ifdef PRODUCT_CODE
#include "lmpolicy.h"
#include "../include/license.h"
extern int main_defined ;
LP_HANDLE *JEDA_lp_handle ;
#endif

extern unsigned lex_line_num, lex_debug_line_num ;
extern char *input_file_name  ;
extern char *last_text ;

JD_name *add_global_name( char * ) ;

int error_count = 0 ;
int max_error = 0 ;
int error_flag = 0 ;
int inline_optimization = 0 ;

int parse_sys_utils = 0 ;

extern int yydebug;
extern FILE *yyin;  /* for lex input */

char *util_fname ;
char **util_fname_list ;
int num_util = 0 ;
int util_count = 0 ;
#ifdef PRODUCT_CODE
int max_num_util = 0 ;

int module_linked = 0 ;
#endif

char *out_fname = NULL ;
char *out_c_fname = NULL ;
char *out_v_fname = NULL ;
char *out_vtop_fname = NULL ;

FILE *out ;

char *current_dir = NULL ;

char *error_message ;

extern FILE *popen();

char *ccarg = NULL ;

int JL_debug_flag = 0 ;
int JL_optimize_level = 0 ;
int JL_twovalue_mode = 0 ;
int JL_inline_mode = 0 ;
int JL_nocheck_mode = 0 ;
int JL_verilog_top_mode = 0 ;
int JL_standalone_mode = 0 ;
int JL_vpi_mode = 0 ;  


void link_files() ;
void link_parser_init() ;

link_file_name *top_link_file ;
link_file_name **next_link_file ;

extern vcode_info  *JD_first_inst ;

#ifdef PRODUCT_CODE
  /* for separate compilation */
link_file_name *top_module_file  ;
link_file_name **next_module_file ;
#endif


static void set_ccarg( char *agv ) {
  char *arg ;
  int len ;

  len = strlen(agv)+3 ;
  if( ccarg != NULL ) len +=  strlen(ccarg) ;
  arg = (char *)calloc( len, sizeof(char) ) ;
  strcpy( arg, " " ) ;
  if( ccarg != NULL ) {
    strcat( arg, ccarg ) ;
    strcat( arg, " " ) ;
    free( ccarg ) ;
  }
  strcat( arg, agv ) ;
  ccarg = arg ;
}


main ( int argc, char **argv ) {
  int i ;
  char *homedir = (char *)getenv( JEDA_HOME ) ;
  
  if( homedir == NULL ) {
    fprintf( 
      stderr, 
      "Environment Variable $%s is not set. Using current dir\n", JEDA_HOME 
    ) ;
    homedir = "." ;
  }

#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  {
    char *licdir ;
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

  top_link_file = NULL ;
  next_link_file = &top_link_file ;

#ifdef PRODUCT_CODE
  /* for separate compilation */
  top_module_file = NULL ;
  next_module_file = &top_module_file ;
  
  /* Non product version may not work with -JL_U option. */
  util_fname_list = (char **)calloc(1024, sizeof(char *) ) ;
  max_num_util = 1024 ;
#endif
    
  for( i = 1 ; i < argc ; i++ ) {
  
    if( argv[i][0] != '-' ) {
      int j ;
      /* got input file, chain the file name and set file type by suffix */
      
      /* chain a new file name */
      link_file_name *new_file = ALLOC(link_file_name) ;
      new_file->name = argv[i] ;
      *next_link_file = new_file ;
      next_link_file = &new_file->next ;
      
      /* finding start of suffix */
      j = strlen( new_file->name ) - 1 ;
      while( j && new_file->name[j] != '.' ) j-- ;
      
      /* set type from suffix */
      if( j && !strcmp( &new_file->name[j], ".jo" ) ) 
        new_file->type = JD_obj_file ;
      else if( j && !strcmp( &new_file->name[j], ".c" ) ) 
        new_file->type = JD_c_file ;
      else if( j && !strcmp( &new_file->name[j], ".o" ) ) 
        new_file->type = JD_o_file ;
      else if( j && !strcmp( &new_file->name[j], ".a" ) ) 
        new_file->type = JD_a_file ;
      else if( j && !strcmp( &new_file->name[j], ".dl" ) ) 
        new_file->type = JD_dl_file ;
      else 
        new_file->type = JD_unknown_file ;
    }
    else {
      switch( argv[i][1] ) {
        case 'J':  /* -JL_ option?? */
          if( argv[i][2] == 'L' &&  argv[i][3] == '_' ) 
          switch( argv[i][4] ) {
          case 'G':
            JL_debug_flag = 1 ;
            break ;
          case 'T':
            JL_twovalue_mode = 1 ;
            break ;
#ifdef PRODUCT_CODE
          case 'I':
            JL_inline_mode = 1 ;
            inline_optimization = atoi(&argv[i][5]) ;
            fprintf( stderr, "Inline opt = %d\n", inline_optimization ) ;
            break ;
          case 'M':
            /* Specify module name with -JL_M <module_name> */
            {
              link_file_name *new_file = ALLOC(link_file_name) ;
              new_file->name = argv[++i] ;
              *next_module_file = new_file ;
              next_module_file = &new_file->next ;
              module_linked = 1 ;
            }
            break ;
#endif
          case 'V':
            JL_verilog_top_mode = 1 ;
            break ;
          case 'S':
            JL_standalone_mode = 1 ;
            JL_vpi_mode = 0 ;
            break ;
          case 'P':
            JL_vpi_mode = 1 ;
            break ;
          case 'o':
            out_fname = argv[++i] ;
            break ;
          case 'U':
            util_fname_list[num_util++] = argv[++i] ;
#ifdef PRODUCT_CODE
            if( num_util == max_num_util ) {
              util_fname_list = 
                (char **)realloc( ( max_num_util + 1024) * sizeof(char *) ) ;
              max_num_util += 1024 ;
            }
#endif
            break ;
          default:
            fprintf( 
              stderr, "Error(%s): Unknown linker switch %s\n", 
              JDLINK, argv[i]
            ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
            lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
            exit(1) ;
          }
          else {
            set_ccarg( argv[i] ) ;
          }
          break ;
        default:
          set_ccarg( argv[i] ) ;
	  break ;
      }
    }
    
  }

  input_file_name = "???"  ;
  last_text = (char *)strdup("???") ;
  
  current_dir = (char *)getcwd( NULL, 1024 ) ; /* ( buf, size ) */
  
  if( top_link_file == NULL ) {
    fprintf( stderr, "Usage: %s [switches] input..\n", JDLINK  ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  
  error_message = (char *)calloc( MAX_ERROR_MESSAGE, sizeof(char) ) ;
  
  if( out_fname == NULL ) {
    int i ;
    out_fname = (char *)calloc( strlen(top_link_file->name)+10, sizeof(char) ) ;
    strcpy( out_fname, top_link_file->name ) ;
    i = strlen( out_fname ) - 1 ;
    while( i && out_fname[i] != '.' ) i-- ;
    if( i ) {
      out_fname[i] = '\0' ;
    }
  }
  
  out_c_fname = (char *)calloc( strlen(out_fname)+10, sizeof(char) ) ;
  strcpy( out_c_fname, out_fname ) ;
  strcat( out_c_fname, ".jeda.c" ) ;
  printf( "OUT_C_FILE: %s\n", out_c_fname ) ;

  out_v_fname = (char *)calloc( strlen(out_fname)+10, sizeof(char) ) ;
  strcpy( out_v_fname, out_fname ) ;
  strcat( out_v_fname, ".jeda.v" ) ;
  
  if( JL_verilog_top_mode ) {
    out_vtop_fname = (char *)calloc( strlen(out_fname)+20, sizeof(char) ) ;
    strcpy( out_vtop_fname, out_fname ) ;
    strcat( out_vtop_fname, ".jeda_top.v" ) ;
  }
  
  out = fopen( out_c_fname, "w" ) ;
  if( out == NULL ) {
    fprintf( stderr, "Error: Can't open c output file \"%s\"\n", out_c_fname ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
    lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
    exit(1) ;
  }
  
  {  /* CLOCK always have index 0 */
    JD_name *name ;
    name = add_global_name( "CLOCK" ) ;
    name->type = JD_port_name ;
    name->index = 0 ;
  }
  
  {
    int len ;
    len = 256 + strlen( homedir ) ;
    util_fname = (char *)calloc( len, sizeof(char) ) ;
    strcpy( util_fname, homedir ) ;
    strcat( util_fname, "/include/jeda_utils.jo" ) ;
  }
  
  fprintf( out, "/** Jeda Compiler C output **/\n" ) ;
  fprintf( out, "#include <stdio.h>\n" ) ;
  fprintf( out, "#include \"jeda_runtime.h\"\n" ) ;
  fprintf( out, "#include \"jeda_pli.h\"\n" ) ;
  link_files() ;
  
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif

}

static link_file_name *link_file ;

void link_files() {
    
  link_file = top_link_file ;
  while( link_file ) { 
    if( link_file->type == JD_obj_file ) break ;
    link_file = link_file->next ;
  }
  if( link_file ) {
    yyin = fopen( util_fname, "r" ) ;
    if( !yyin ) {
      fprintf( stderr, "Error: Can't open util file \"%s\"\n", util_fname ) ;
      exit(1);
    }
    parse_sys_utils = 1 ;
    input_file_name = util_fname ;
 
    lex_line_num = 0 ;

    yyparse() ;
  
    close( yyin ) ;
    
    /* mark the vinst at the beginning of a file */
    
    if( JD_first_inst && JD_first_inst->local_name_flag == 0 ) {
      JD_first_inst->local_name_flag = 1 ;
      check_local_name(JD_first_inst) ;
    }
        
  }
  
  if( error_count == 0 ) {
    check_global_name() ;
  }

  last_text = "???" ;
  
#ifdef PRODUCT_CODE
  if( main_defined ) {
#endif
    printf( "OUT_VERILOG_FILE: %s\n", out_v_fname ) ;
    if( JL_verilog_top_mode ) {
      printf( "OUT_VERILOG_TOP_FILE: %s\n", out_vtop_fname ) ;
    }
#ifdef PRODUCT_CODE
  }
#endif

  if( error_count == 0 ) {
    ccodegen() ;
  }
  
  fprintf( stderr, " %d link errors\n", error_count ) ;
  
  
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
  lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
  if( error_count == 0 ) {
    close( out ) ;
    exit(0) ;
  }
  else exit(1) ;
  
}

int yyerror( char *message ) 
{
  int last = 0 ;
  
  if( last_text == NULL ) {
    last_text = "UNKNOWN" ;
    last = 1 ;
  }
  
  fprintf(
    stderr, "File: \"%s\", line %d near %s : ", 
    input_file_name, lex_line_num+1, last_text
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
  
  return 0 ;
}

int yywarning( char *message ) 
{
  int last = 0 ;
  
  if( last_text == NULL ) {
    last_text = "UNKNOWN" ;
    last = 1 ;
  }
  if( last_text == NULL ) last_text = "UNKNOWN" ;

  fprintf(
    stderr, "File: \"%s\", line %d near %s : ", 
    input_file_name, lex_line_num+1, last_text
  );

  fprintf(
    stderr, "%s\n", 
    message
  );

  if( last ) last_text = NULL ;
 
  return 0 ;
   
}


/*
 *
 *  ALLOC( ) is mapped to this
 *
 */
void *check_alloc( int size ) {
  int *ret ;
  
  ret = (int *)calloc(1, size ) ;
  
  if( ret == NULL ) {
    ERROR_FATAL_NO_MEMORY ;
  }
  
  return ret ;

}

void check_free( int *p ) {
  
  if( p ) free(p) ;

}


int yywrap(void) { 
  if( parse_sys_utils  ) {
    if( util_count < num_util ) {
      close( yyin ) ;
      yyin = fopen( util_fname_list[util_count], "r" ) ;
      if( !yyin ) {
        fprintf( 
          stderr, "Error: Can't open util file \"%s\"\n", 
          util_fname_list[util_count] 
        ) ;
        exit(1);
      }
      util_count++ ;
      return 0 ;
    }
    else parse_sys_utils = 0 ;
  }
  else link_file = link_file->next ;
  while( link_file ) {
    if( link_file->type == JD_obj_file ) {
      close( yyin ) ;
      if( JD_first_inst && JD_first_inst->local_name_flag == 0 ) {
        if(JD_first_inst) JD_first_inst->local_name_flag = 1 ;
        check_local_name(JD_first_inst) ;
        JD_first_inst = NULL ;
      }
      yyin = fopen( link_file->name, "r" ) ;
      if( !yyin ) {
        fprintf( stderr, "Error: Can't open file \"%s\"\n", link_file->name ) ;
#ifdef PRODUCT_CODE
#  ifndef NO_LM_LICENSE
        lp_checkin( JEDA_lp_handle ) ;
#  endif
#endif
        exit(1);
      } 
      else {
        fprintf( stderr, "Loading file \"%s\"\n", link_file->name ) ;
      }
      input_file_name = link_file->name ;
      lex_line_num = 0 ;
      return 0 ;
    }
  }
  return 1 ;
}
