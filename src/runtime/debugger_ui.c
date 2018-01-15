/*
 * Jeda Debugger User Interface
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_DBG_PORT  7775

#ifdef DEBUGGER_MAIN
#  define PRINTF printf
#else
#  include "runtime.h"
#  include "mem_macro.h"
#  include "runtime_error_msg.h"
#  include "data.h"
#  include "thread.h"
#  include "bit_operations.h"
#  include "bit_macro.h"
#  include "exec_macro.h"
#  include "data_macro.h"
#  include "code_exec_macro.h"
#  include "debugger.h"
#  include "debugger_error_msg.h"
#endif

/* using GNU readline library in gdb-5.0 */
#include <readline.h>
#include <history.h>
 
#ifndef DEBUGGER_MAIN
extern char *JD_debugger_filename ;
extern int JD_dbg_eval_error ;
#endif

/****************************************************************************/
/* Debugger user if code is copied from gdb-5.0/readline/examples/fileman.c */
/****************************************************************************/
extern char *getwd ();
extern char *xmalloc();

extern int JD_dbg_print_fnum ;

int JD_debug_window = 0 ;

/* The names of functions that actually do the manipulation. */
static int com_continue (), com_step (), com_step_a (), 
    com_help (), com_print (), com_break (),
    com_list (), com_quit(), com_assign (), 
    com_thread (), com_up (), com_down (), com_backtrace (),
    com_show (), com_tbreak (), com_next(), com_clear(),
    com_window(), com_update() ;

typedef struct {
  char *name;			/* User printable name of the function. */
  Function *func;		/* Function to call to do the job. */
  char *doc;			/* Documentation for this function.  */
} COMMAND;

static COMMAND commands[] = {
  { "continue", com_continue, "Continue simulation" },
  { "c", com_continue, "    :Synonym for 'continue'" },
  { ".", com_continue, "    :Synonym for 'continue'" },
  { "next", com_next, "    :Execute next line" },
  { "n", com_next, "    :Synonym for 'next'" },
  { "step", com_step, "    :Step execution on current thread" },
  { "s", com_step, "    :Synonym for 'step'" },
  { ",", com_step, "    :Synonym for 'step'" },
  { "stepany", com_step_a, "    :Step execution on any thread" },
  { "sa", com_step_a, "    :Synonym for 'stepany'" },
  { "help", com_help, "    :Display this text" },
  { "?", com_help, "    :Synonym for `help'" },
  { "print", com_print, "<exp> :Print expression" },
  { "p", com_print, "Synonym for `print'" },
  { "break", com_break, "[<file>:]<linnum> :Set breakpoint" },
  { "b", com_break, "Synonym for `break'" },
  { "clear", com_clear, "[<file>:]<linnum> :Clear breakpoint" },
  { "tbreak", com_tbreak, "[<file>:]<linnum> : :Set temporal breakpoint" },
  { "tb", com_tbreak, "[<file>:]<linnum> : :Synonym for `tbreak'" },
  { "backtrace", com_backtrace, "    :backtrace: show execution stack" },
  { "bt", com_backtrace, "    :Synonym for 'backtrace'" },
  { "where", com_backtrace, "    :Synonym for 'backtrace'" },
  { "list", com_list, "([<file>:]<start>[,<end>] | [<file>:]<func_name> | [-]) :List source file" },
  { "l", com_list, "([<file>:]<start>[,<end>] | [<file>:]<func_name> | [-]) :Synonym for `list'" },
  { "assign", com_assign, "<exp> = <exp> :Assign value to a variable" },
  { "thread", com_thread, "<num> :Switch current thread" },
  { "up", com_up, "[<num>] :Up scope" },
  { "down", com_down, "[<num>] :Down scope" },
  { "show", com_show, "(vars | lvars | threads | lthreads) : Show variable or thread information" },
  { "quit", com_quit, "    :Quit simulation" },
  { "window", com_window, "    :Open debug window" },
  { "update", com_update, "    :Update window information" },
  { (char *)NULL, (Function *)NULL, (char *)NULL }
};

/* Forward declarations. */
static char *stripwhite ();
static COMMAND *find_command ();
static int execute_line ();
static initialize_readline ();

/* When non-zero, this local means the user is done using debugger. */
static int done;

static char *
dupstr (s)
     char *s;
{
  char *r;

  r = xmalloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}

static char *prev_command = NULL;

/* debugger main loop */

#ifdef DEBUGGER_MAIN
main()
#else
void JD_debugger_main() 
#endif
{
  char *line, *s;
  
  initialize_readline ();	/* Bind our completer. */

  done = 0 ;
  
#ifndef DEBUGGER_MAIN
  JD_debugger_backtrace() ;
  
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
  else {
    JD_dbg_list_file( 4, NULL, NULL, 0, 5 ) ;
  }
  
  if( JD_debugger_filename != NULL ) {
    FILE *fl ;
    int c ;
    char buf[256] ;
    char *cp ;
    
    PRINTF( "Jeda Debugger executing \"%s\"..\n", JD_debugger_filename ) ;
    fl = fopen( JD_debugger_filename, "r" ) ;
    if( fl ) {
      while( done == 0 && fgets( buf, 256, fl ) != NULL ) {
        s = stripwhite (buf);
        cp = s ;
        while( *cp ) {
          if( *cp == '\n' ) { *cp = '\0' ; break ; }
          cp++ ;
        }
        if( *s && *s != '#' ) {
          execute_line(s) ;
        }
      }
      fclose(fl) ;
    }
    else {
      PRINTF( "Jeda Debugger: Can not open \"%s\" \n", JD_debugger_filename ) ;
    }
  }
#endif
  
  /* if execution of file set done, then return to simulation without
     interactive session
   */
    
  /* Loop reading and executing lines until the user quits. */
  for ( ; done == 0; )
  {
    line = readline ("jdb> ");

    if (!line)
      break;

    /* Remove leading and trailing whitespace from the line.
       Then, if there is anything left, add it to the history list
       and execute it. */
    s = stripwhite (line);

    if (*s) {
      if( prev_command ) free( prev_command ) ;
      prev_command = strdup(s) ;
      add_history (s);
      execute_line (s);
    }
    else if( prev_command ) {
      add_history (prev_command);
      execute_line (prev_command);
    }

    free (line);
  }
}

int JD_dbg_prompt_user_reply() {
  char *line, *s;
  int ret = 1 ;

  line = readline ("-- to continue: <cr> , to quit : q<cr> --\n");

  if (!line)
      return 1 ;

  s = stripwhite (line);

  if (*s) {
    if( *s = 'q' ) ret = 0 ;
  }
  free (line);
    
  return ret ;
    
}



/* Execute a command line. */
static int
execute_line (line)
     char *line;
{
  register int i;
  COMMAND *command;
  char *word;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && whitespace (line[i]))
    i++;
  word = line + i;

  while (line[i] && !whitespace (line[i]))
    i++;

  if (line[i])
    line[i++] = '\0';

  command = find_command (word);

  if (!command)
    {
      PRINTF ( "%s: No such command for jdb.\n", word);
      return (-1);
    }

  /* Get argument to command, if any. */
  while (whitespace (line[i]))
    i++;

  word = line + i;

#ifndef DEBUGGER_MAIN
  JD_dbg_eval_error = 0 ;
#endif

  /* Call the function. */
  return ((*(command->func)) (word));
}

/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */
static COMMAND *
find_command (name)
     char *name;
{
  register int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp (name, commands[i].name) == 0)
      return (&commands[i]);

  return ((COMMAND *)NULL);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
static char *
stripwhite (string)
     char *string;
{
  register char *s, *t;

  for (s = string; whitespace (*s); s++)
    ;
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
    t--;
  *++t = '\0';

  return s;
}

/* **************************************************************** */
/*                                                                  */
/*                  Interface to Readline Completion                */
/*                                                                  */
/* **************************************************************** */

static char *command_generator ();
static char **jdb_completion ();

/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
static initialize_readline ()
{
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "jdb";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = (CPPFunction *)jdb_completion;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
static char **
jdb_completion (text, start, end)
     char *text;
     int start, end;
{
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = (char **)completion_matches (text, command_generator);

  return (matches);
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
static char *
command_generator (text, state)
     char *text;
     int state;
{
  static int list_index, len;
  char *name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
    {
      list_index = 0;
      len = strlen (text);
    }

  /* Return the next name which partially matches from the command list. */
  while (name = commands[list_index].name)
    {
      list_index++;

      if (strncmp (name, text, len) == 0)
        return (dupstr(name));
    }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}


/* Print out help for ARG, or for all of the commands if ARG is
   not present. */
static int com_help (arg)
     char *arg;
{
  register int i;
  int printed = 0;

  for (i = 0; commands[i].name; i++)
    {
      if (!*arg || (strcmp (arg, commands[i].name) == 0))
        {
          PRINTF ("%s\t\t%s.\n", commands[i].name, commands[i].doc);
          printed++;
        }
    }

  if (!printed)
    {
      PRINTF ("No commands match `%s'.  Possibilties are:\n", arg);

      for (i = 0; commands[i].name; i++)
        {
          /* Print in six columns. */
          if (printed == 6)
            {
              printed = 0;
              PRINTF ("\n");
            }

          PRINTF ("%s\t", commands[i].name);
          printed++;
        }

      if (printed)
        PRINTF ("\n");
    }
  return (0);
}


#ifndef DEBUGGER_MAIN

extern int JD_debugger_assign_command ;
extern int JD_dbg_print_format ;
extern JD_debug_file JD_file_table[] ;

static int com_quit (arg)
     char *arg;
{
  PRINTF( " quiting..\n" ) ;
  JEDA_quit_simulation() ; // calling $finish 
  done = 1;
  return (0);
}

static int com_continue (arg)
     char *arg;
{
  PRINTF( " continueing..\n" ) ;
  done = 1;
  return (0);
}

static int com_next (char *arg){
  JD_debugger_set_next() ;
  done = 1;
  return (0);  
}

static int com_step (char *arg){
  JD_debugger_set_step() ;
  done = 1;
  return (0);  
}

static int com_step_a (char *arg)
{
  JD_debugger_set_step_any() ;
  done = 1;
  return (0);  
}

static int com_print (char *arg){
  char *cp = arg ;
  
  JD_dbg_print_format = 0 ;
  
  while( *cp != '\0' && isspace(*cp) ) cp++ ;
  
  if( *cp == '/' ) {
    /* format specified */
    cp++ ;
    switch(*cp) {
      case 'b':
      case 'B':
        JD_dbg_print_format = 1 ;
        break ;
      case 'x':
      case 'X':
      case 'h':
      case 'H':
        JD_dbg_print_format = 2 ;
        break ;
      case 'd':
      case 'D':
        JD_dbg_print_format = 3 ;
        break ;
      case 'o':
      case 'O':
        JD_dbg_print_format = 4 ;
        break ;
      default:
        DBG_ERROR_UNKNOWN_FORMAT_CHAR( *cp ) ;
        return(0) ;
    }
    cp++ ;
    
  }
  
  JD_debugger_set_word( cp ) ; /* used by parser */
  
  JD_debugger_assign_command = 0 ;
  
  JDB_yyparse() ;
  
}

extern JD_debug_stack *JD_debugged_stack ;
#ifdef PRODUCT_CODE
extern char **JD_Debug_name_table ;
#else
extern char *JD_Debug_name_table[] ;
#endif

/* We expect [ <file>: ]location as the arg */
static void lookup_file_locatoin_arg( 
  char *arg, char **filename, int *linenum 
) {
  if( arg ) {
    if( isdigit(*arg) ) {
      *linenum = atoi(arg) ;
      *filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
    }
    else {
      char *cp, *fp ;
      cp = arg ;
      while( *cp != '\0' && *cp != ':' ) cp++ ;
      if( *cp == '\0' ) {
        DBG_ERROR_EXPECTING_FILE_COLUMN_LINENUM( arg ) ;
        *linenum = -1 ;
        return ;
      }
      *cp = '\0' ;
      /* we want to chop any directory info */
      fp = cp ;
      fp-- ;
      while( fp != arg && *fp != '/' ) fp-- ;
      if( fp == arg ) *filename = arg ;
      else *filename = fp+1 ;
      cp++ ;
      while( *cp != '\0' && isspace(*cp) ) cp++ ;
      if( *cp == '\0' ) {
        DBG_ERROR_ILLEGAL_ARG_FOR_FILE_LOCATION( arg ) ;
        *linenum = -1 ;
        *filename = NULL ;
        return ;
      }
      if( isdigit(*cp) ) {
        *linenum = atoi(cp) ;
      }
      else {
        DBG_ERROR_ILLEGAL_ARG_FOR_FILE_LOCATION( arg ) ;
        *linenum = -1 ;
      }
    }
  }
  else {
    *linenum = JD_debugged_stack->line_num ;
    *filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
  }
}

static JD_debug_file *lookup_file_table( char *filename ) {
  int i ;
  int len ;
  char *cp, *sp ;
  
  
  for( i = 0 ; JD_file_table[i].cwd > 0 ; i++ ) {
    if( !strcmp( filename, JD_Debug_name_table[JD_file_table[i].name] ) ) {
      return &JD_file_table[i] ;
    }
    /* get the pure file name */
    cp = sp = JD_Debug_name_table[JD_file_table[i].name] ;
    while( *cp != '\0' ) cp++ ;
    cp-- ;
    /* search pure file name part */
    while( cp > sp ) {
      if( *cp == '/' ) break ;
      cp-- ;
    }
    if( *cp == '/' ) cp++ ;
    else cp = NULL ;
    if( cp && !strcmp( filename, cp ) )
    {
      return &JD_file_table[i] ;
    }
  }
}

static int com_break (char *arg){
  int linenum ;
  char *filename, *cwd ;
  JD_debug_file *f ;
  
  lookup_file_locatoin_arg( arg, &filename, &linenum ) ;
  
  if( linenum < 0 ) return ;
  
  f = lookup_file_table( filename ) ;
  if( !f ) {
    DBG_ERROR_CANT_FIND_DIR_INFO_FOR_FILE( filename ) ;
    return ;
  }
  else {
    filename = JD_Debug_name_table[f->name] ;
    cwd = JD_Debug_name_table[f->cwd] ;
  }
  
  JD_debugger_set_breakpoint( cwd, filename, linenum, 0 ) ;
  
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
}

static int com_clear(char *arg) {
  int linenum ;
  char *filename, *cwd ;
  JD_debug_file *f ;
  
  lookup_file_locatoin_arg( arg, &filename, &linenum ) ;
  
  if( linenum < 0 ) return ;
  
  f = lookup_file_table( filename ) ;
  if( !f ) {
    DBG_ERROR_CANT_FIND_DIR_INFO_FOR_FILE( filename ) ;
    return ;
  }
  else {
    filename = JD_Debug_name_table[f->name] ;
    cwd = JD_Debug_name_table[f->cwd] ;
  }
  
  JD_debugger_clear_breakpoint( cwd, filename, linenum, 0 ) ;
  
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
}

static int com_tbreak (char *arg){
  int linenum ;
  char *filename ;
  
  lookup_file_locatoin_arg( arg, &filename, &linenum ) ;
  
  if( linenum >= 0 )
    JD_debugger_set_breakpoint( NULL, filename, linenum, 1 ) ;
  
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
}

static void lookup_list_arg( 
  char *arg, char **filename, int *linenum, int *num, int *type
) {
  char *cp ;
  if( !arg || *arg == '\0' ) {
    *type = 0 ;
  }
  else {
    if( isdigit(*arg) ) {
      *type = 2 ;
      *linenum = atoi(arg) ;
      cp = arg ;
      while( *cp != '\0' && !isspace(*cp) && *cp != ',' ) cp++ ;
      while( (*cp != '\0' && isspace(*cp) ) || *cp == ',' ) cp++ ;
      if( *cp == '\0' ) *num = 10 ;
      else {
        if( *cp == ',' ) cp++ ;
        while( *cp != '\0' && isspace(*cp) ) cp++ ;
        if( *cp == '\0' ) *num = 10 ;
        else *num = atoi(cp) ;
      }
      *filename = JD_Debug_name_table[JD_debugged_stack->file_index] ;
      return ;
    }
    else {
      if( *arg == '-' ) {
        *linenum = -10 ;
        *num = 10 ;
        *type = 1 ;
        return ;
      }
      cp = arg ;
      while( *cp != '\0' && *cp != ':' && !isspace(*cp) ) cp++ ;
      if( *cp == '\0' || isspace(*cp) ) { /* no : detected */
        *filename = NULL ;
        JD_dbg_lookup_funcname( arg, filename, linenum ) ;
        if( *linenum > 0 ) *type = 3 ;
        else {
          DBG_FUNC_NAME_NOT_FOUND( arg ) ;
          *type = -1 ;
        }
        return ;
      }
      *cp = '\0' ;
      *filename = arg ;
      cp++ ;
      while( *cp != '\0' && isspace(*cp) ) cp++ ;
      if( *cp == '\0' ) {
        DBG_ERROR_ILLEGAL_ARG_FOR_FILE_LOCATION( arg ) ;
        *linenum = -1 ;
        *filename = NULL ;
        return ;
      }
      if( isdigit(*cp) ) {
        *linenum = atoi(cp) ;
        while( *cp != '\0' && !isspace(*cp) && *cp != ',' ) cp++ ;
        while( (*cp != '\0' && isspace(*cp) ) || *cp == ',' ) cp++ ;
        if( *cp != '\0' ) *num = atoi(cp) ;
        else *num = *linenum + 10 ;
        if( *num < *linenum ) *num = *linenum + 10 ;
        *type = 3 ;
      }
      else {
        JD_dbg_lookup_funcname( cp, filename, linenum ) ;
        if( *linenum >= 0 ) {
          *num = *linenum + 10 ;
          *type = 3 ;
        }
        else {
          DBG_FUNC_NAME_NOT_FOUND( cp ) ;
          *type = -1 ;
        }
      }
    }
  }
}

static int com_list (char *arg){
  int linenum, num, type ;
  char *filename, *cwd ;
  JD_debug_file *f ;
  
  lookup_list_arg( arg, &filename, &linenum, &num, &type ) ;
  if( type >= 0 ) {
    cwd = NULL ;
    if( type == 2 || type == 3 ) {
      f = lookup_file_table( filename ) ;
      if( !f ) {
        DBG_ERROR_CANT_FIND_DIR_INFO_FOR_FILE( filename ) ;
        type = -1 ;
      }
      else {
        filename = JD_Debug_name_table[f->name] ;
        cwd = JD_Debug_name_table[f->cwd] ;
      }
    }
    if( type >= 0 ) {
      JD_dbg_list_file( type, cwd, filename, linenum, num ) ;
    }
  }
  
}

static int com_assign (char *arg){

  JD_debugger_set_word( arg ) ; /* used by parser */
  
  JD_debugger_assign_command = 1 ;
  
  JDB_yyparse() ;
  
}

static int com_thread (char *arg){
  int n ;
  if( arg && isdigit(*arg) ) {
    n = atoi(arg) ;
    JD_debugger_switch_thread( n ) ;
  }
  else {
    DBG_ERROR_ILLEGAL_ARG_FOR_THREAD_COMMAND ;
  }
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
  else
    JD_dbg_list_file( 4, NULL, NULL, 0, 5 ) ;
}

static void get_arg_n( char *arg, int *n ) {
  if( arg && isdigit(*arg) ) {
    *n = atoi(arg) ;
  }
  else *n = 1 ;
}

static int com_up (char *arg){
  int n ;
  
  get_arg_n( arg, &n ) ;
  JD_debugger_up_scope(n) ;
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
  else
    JD_dbg_list_file( 4, NULL, NULL, 0, 5 ) ;
}

static int com_down (char *arg){
  int n ;
  
  get_arg_n( arg, &n ) ;
  JD_debugger_down_scope(n) ;
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
  else
    JD_dbg_list_file( 4, NULL, NULL, 0, 5 ) ;
}

static int com_show(char *arg)
{
  if( arg ) {
    if( !strcmp( arg, "vars" ) || !strcmp( arg, "var" ) ) {
      JD_debugger_show_vars() ;
    }
    else if( !strcmp( arg, "lvars" ) || !strcmp( arg, "lvar" ) ) {
      JD_debugger_show_local_vars() ;
    }
    else if( !strcmp( arg, "threads" ) || !strcmp( arg, "thread" ) ) {
      JD_debugger_show_threads() ;
    }
    else if( !strcmp( arg, "lthreads" ) || !strcmp( arg, "lthread" ) ) {
      JD_debugger_show_local_threads() ;
    }
    else {
      DBG_ERROR_USAGE_SHOW_VAR_LVAR_THREAD_LTHREAD ;
    }
  }
  else {
    DBG_ERROR_USAGE_SHOW_VAR_LVAR_THREAD_LTHREAD ;
  }
}

static int com_backtrace() {
  JD_debugger_backtrace() ;
}

/* variables for debug window */
static int sockfd, w_socket ;
static struct sockaddr_in sockaddr, w_sockaddr ;
static int w_pid ;
static int m_pid = 0 ;
static int pipefd[2] ;

static int open_socket() {
  int st = -1 ;
  int n = 0 ;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if( sockfd < 0 ) {
    PRINTF( "Can't create socket\n" ) ;
    return 0 ;
  }
  sockaddr.sin_family = AF_INET ;
  sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1") ; /* local host */
  while( st < 0 && n < 100 ) {
    sockaddr.sin_port = htons(DEFAULT_DBG_PORT + n) ;
    st = bind( sockfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr) );
    n++ ;
  }
  if( st < 0 ) {
    PRINTF( 
      "Can't bind socket to the port %d\n",
      sockaddr.sin_port
    ) ;
    close( sockfd ) ;
    return 0 ;
  }
  else {
    /*
    PRINTF( 
      "Bind socket to the port %d\n",
      sockaddr.sin_port
    ) ;
    */
  }
  return sockaddr.sin_port ;
}

com_window() {
  int port, a_len ;
  char pid_c[100] ;
  char port_c[100] ;
  char *tclfile ;
  char *homedir ;
  
  homedir = (char *)getenv( "JEDA_HOME" ) ;
  
  if( homedir == NULL ) {
    PRINTF(  
      "Environment Variable \"JEDA_HOME\" is not set.\n" 
    ) ;
    return ;
  }

  if( pipe(pipefd) < 0 ) {
    printf( "Can't open pipe\n" ) ;
    return ;
  }

  if( !JD_debug_window ) {
    port = open_socket() ;
    if( !port ) return ;
    
    if( listen( sockfd, 5 ) < 0 ) {
      PRINTF( "listen error on port %d\n", port ) ;
      close( sockfd ) ;
      return ;
    }
    
    w_pid = fork() ;
  
    if( w_pid == 0 ) {
      /* child process */
      m_pid = fork() ;
      if( m_pid == 0 ) {
        /* forward original stdin to the new pipe */
        int c ;
        close(1) ;
        dup(pipefd[1]) ;
        while( 1 ) {
          c = getchar() ;
          if( c != '\f' ) putchar( c ) ;
        }
      }
      else {
        /* parent in fork */
        close( 1 ) ;
        dup(pipefd[1]) ;
        sprintf( port_c, "%d", port ) ;
        sprintf( pid_c, "%d", m_pid ) ;
        tclfile = (char *)calloc( strlen(homedir)+100, sizeof(char) ) ;
        strcat( tclfile, homedir ) ;
        strcat( tclfile, "/bin/debug_window.tcl" ) ;
        /* 
        fprintf( stderr, "execlp wish -f %s %s %s\n", tclfile, port_c, pid_c) ;
        */
        execlp( "wish", "-f", tclfile, port_c, pid_c, 0 ) ;
        exit(0) ;
      }
    }
    else {
      /* get stdin from the pipe */
      close(0) ;
      dup(pipefd[0]) ;
      a_len = sizeof(w_sockaddr) ;
      w_socket =   
        accept( sockfd, (struct sockaddr *)&w_sockaddr, &a_len ) ;
      if( w_socket < 0) {
        PRINTF( "accept error on port %d\n", port ) ;
        close( sockfd ) ;
        kill( w_pid, SIGKILL ) ;
        return ;
      }
    }
    JD_dbg_print_fnum = w_socket ;
    JD_debug_window = 1 ;
  
  }
  else {
    PRINTF( "debug window is already opened\n" ) ;
  }
}

com_update(char *arg) {
  int pid ;
  
  if( arg ) {
    pid = atoi(arg) ;
    if( pid ) m_pid = pid ; /* update unknown m_pid */
  }
  
  if( JD_debug_window )
    JD_debugger_window_show_current() ;
}

JD_dbg_close_window() {
  if( JD_debug_window ) {
    close( sockfd ) ;
    close( w_socket ) ;
    kill( w_pid, SIGKILL );
    if( m_pid )
      kill( m_pid, SIGKILL );
    JD_debug_window = 0 ;
  }
}

JD_dbg_write_window( char *str ) {
  /* PRINTF( "soc: %s", str ) ; */
  write( w_socket, str, strlen(str) ) ;
}

#else

com_quit (arg)
     char *arg;
{
  printf( "com_quit\n" ) ;
  done = 1;
  return (0);
}

com_continue (arg)
     char *arg;
{
  printf( "com_continue\n" ) ;
  done = 1;
  return (0);
}


com_step (char *arg){
  printf( "com_step\n" ) ;
}

com_step_a (char *arg)
{
  printf( "com_step_a\n" ) ;
}
 
com_print (char *arg){
  printf( "com_print %s\n", arg ) ;
}

com_break (char *arg){
  printf( "com_break\n" ) ;
}

com_list (char *arg){
  printf( "com_list\n" ) ;
}

com_assign (char *arg){
  printf( "com_assign\n" ) ;
}

com_thread (char *arg){
  printf( "com_thread\n" ) ;
}

com_up (){
  printf( "com_up\n" ) ;
}

com_down (){
  printf( "com_down\n" ) ;
}

com_backtrace() {
  printf( "com_backtrace\n" ) ;
}

com_show() {
  printf( "com_show\n" ) ;
}

com_tbreak() {
  printf( "com_tbreak\n" ) ;
}

com_next() {
  printf( "com_next\n" ) ;
}

com_clear() {
  printf( "com_clear\n" ) ;
}

static int pipefd[2] ;

com_window() {
  int id ;
  
  if( pipe(pipefd) < 0 ) {
    printf( "Can't open pipe\n" ) ;
    exit(1) ;
  }

  if( !JD_debug_window ) {
    id = fork() ;
  
    if( id == 0 ) {
      /* child process */
      id = fork() ;
      if( id == 0 ) {
        /* forward original stdin to the new pipe */
        int c ;
        close(1) ;
        dup(pipefd[1]) ;
        while( 1 ) {
          c = getchar() ;
          if( c != '\f' ) putchar( c ) ;
        }
      }
      else {
        /* parent in fork */
        close( 1 ) ;
        dup(pipefd[1]) ;
        execlp( "wish", "-f", "debug_window.tcl", 0 ) ;
        exit(0) ;
      }
    }
    else {
      /* receive stdin from the pipe */
      close(0) ;
      dup(pipefd[0]) ;
    }
  
  }
  else {
    printf( "debug window is already opened\n" ) ;
  }
}

#endif

