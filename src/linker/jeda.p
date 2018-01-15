#! /usr/bin/perl
# 
###########################################################################
# 'jeda' : Jeda main script that compile and link the Jeda source code
#
###########################################################################
#  
#  Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the
#  Free Software Foundation; either version 2, or (at your option) any
#  later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
#   USA.  
#   
sub Usage {
print stderr <<MESSAGE;
Usage:
  jeda [options | filename]...

  options
    -out <name>    define output name, actual output file will have proper 
                     sufix. (default: prefix of the first file name)
    -jc            jeda compile only to create jeda object file
    -dl            create dynamic library file (.dl)
    -exe           create standalone executable
    -cc <cc-path>  specify cc command path (default 'gcc')
    -vpi           compile/link as VPI mode
    -ivl           compile/link as Icarus Verilog VPI mode
    -debug         just printout system command sequence
  
    -D<defines>    defines macro for jeda compile and c compile
    -I<path>       defines include file search path for jeda & c compile
    -g             debug mode compile for jeda & c
    -h             auto-header generation on jeda compiler
    -U <util>     specify the user util file name (<util>.j, <util>.jo)
    -JL_<flag>     option for jeda linker
    
  filename:
   Suffixes of source file names are used to determine which kind of 
   process to be done.
     .j    Jeda source; j-compile, j-link, c-compile, c-link
     .jo   Jeda object; j-link, c-compile, c-link
     .c     C source; c-compile, c-link
     .o     Object file; c-link
     .a     Archive file; c-link
     .so    Shared obj file; c-link
     .dl    dynamic lib file; c-link
     
  example:
    1) compile jeda source (foo.j) and generate jeda object(foo.jo)
      jeda foo.j
    
    2) compile foo.jd and link with bar.jdo to create C source foo.jeda.c
      jeda foo.j bar.jo
      
    3) compile foo.j and link with bar.jo to foo.jeda.c, compile bee.c,
       then link all with zoolib.a to create zumoo.so
       
       jeda -out zumoo -so foo.j bar.jo bee.c zoolib.a
    
    4) link foo.jo, bar.jo to zomoo.jeda.c, then create zumoo.jeda.o obj
      
       jeda -out zumoo -o foo.jo bar.jo
       
  processes:
    'jeda' command invokes various sub-command (process) over files to 
    complete the Jeda compile/link process.
      j-compile   Run jeda compiler (.jd) to generate Jeda obj (.jo)
      j-link      Run jeda linker over Jeda obj file(s) to generate C code
      c-compile   Run C compiler to generate object file.
      c-link      Run C linker to generate executable or library file.
  
    
MESSAGE

}


sub init {
  $JEDA_HOME = $ENV{"JEDA_HOME"} ;
  $JEDA_COMP = "jedacmp" ;
  $JEDA_LINK = "jedalnk" ;
  $CC_NAME = "gcc" ;
  $out_file = "" ;
  $debug_flag = 0 ;
  $vpi_flag = 0 ;
  $ivl_flag = 0 ;
  ##$debug_flag = 1 ; #testing
  $debugger_enable = 0 ;
  $PP_ARGS = "" ;
  $CMP_ARGS = "" ;
  $JL_ARGS = "" ;
  $RUNLIB = "$JEDA_HOME/lib/run_obj/*.o" ;
  $RUNDBGLIB = "$JEDA_HOME/lib/dbg_obj/*.o" ;
  $VPIRUNLIB = "$JEDA_HOME/lib/vpi_obj/*.o" ;
  $VPIRUNDBGLIB = "$JEDA_HOME/lib/vpidbg_obj/*.o" ;
  $EXELIB = "$JEDA_HOME/lib/jedaexe.a" ;
  $EXEDBGLIB = "$JEDA_HOME/lib/jedadbg.a" ;
  $READLINELIB = "$JEDA_HOME/lib/libreadline.a" ;
  $HISTORYLIB = "$JEDA_HOME/lib/libhistory.a" ;
  $READLINEOBJ = "$JEDA_HOME/lib/rl_obj/*.o" ;
  $PLIOBJ = "$JEDA_HOME/lib/jeda_pli.o" ;
  $SMAINOBJ = "$JEDA_HOME/lib/smain.o" ;
  $IVLTBLOBJ = "$JEDA_HOME/lib/ivl_sys_table.o $JEDA_HOME/lib/jeda_vpi_static.o" ;
}

sub read_args {
  my $i = 0 ;

  while( $i <= $#ARGV  ) {
    $arg = $ARGV[$i++] ;
    #print stderr "Arg $arg\n" ;
    if( $arg =~ /^\-/ ) {
      $flag = $' ;
      #print stderr "flag $flag\n" ;
      if( $flag eq "out" ) { 
        $out_file = $ARGV[$i++] ;
      }
      elsif( $flag eq "jc" ) { 
        $jc_flag = 1 ;
      }
      elsif( $flag eq "dl" ) { 
        $dl_flag = 1 ;
      }
      elsif( $flag eq "exe" ) { 
        $exe_flag = 1 ;
        $JL_ARGS .= " -JL_S" ; # set linker standalone flag as well
      }
      elsif( $flag eq "debug" ) { 
        $debug_flag = 1 ;
      }
      elsif( $flag eq "vpi" ) { 
        $JL_ARGS .= " -JL_P" ; # set linker vpi flag
        $vpi_flag = 1 ;
      }
      elsif( $flag eq "ivl" ) { 
        $JL_ARGS .= " -JL_P" ; # set linker vpi flag
        $vpi_flag = 1 ;
        $ivl_flag = 1 ;
        $dl_flag = 1 ;
      }
      elsif( $flag eq "h" ) { 
        $CMP_ARGS .= " $arg" ;
      }
      elsif( $flag =~ "^I" ) { 
        $PP_ARGS .= " $arg" ;
        $ifile = $' ;
        if( $ifile eq "" ) {
          $PP_ARGS .= " ".$ARGV[$i++] ;
        }
      }
      elsif( $flag =~ "^D" ) { 
        $PP_ARGS .= " $arg" ;
      }
      elsif( $flag eq "g" ) { 
        $PP_ARGS .= " $arg" ;
        $JL_ARGS .= " -JL_G" ; # set linker debug flag as well
        $debugger_enable = 1 ; # link to debugger version of libs
      }
      elsif( $flag eq "U" ) {
        $util = $ARGV[$i++] ;
        $CMP_ARGS .= " -U".$util.".jh " ;
        $JL_ARGS .= " -JL_U ".$util.".jo " ;
      }
      elsif( $flag =~ "^JL_" ) { #arg for Jeda linker
        if( $flag eq "JL_o" ) {
          print stderr "JL_o option can't be specified on jeda script\n" ;
          exit(1) ;
        }
        $rm = $' ;
        $JL_ARGS .= " $arg" ;
        if( $rm eq "U" ) {
          $JL_ARGS .= " ".$ARGV[$i++] ;
        }
      }
      else {
        print stderr "Unknown option $arg\n" ;
        Usage() ;
        exit(1) ;
      }
    }
    else {
      print stderr "file: $arg\n" ;
      if( $arg =~ /\.j$/ ) {
        $prefix = $` ;
        push @JEDASRCFILES, $arg ;
        if( $out_file eq "" ) {
          $out_file = $prefix ;
        }
      } 
      elsif( $arg =~ /\.jo$/ ) {
        $prefix = $` ;
        push @JEDAOBJFILES, $arg ;
        if( $out_file eq "" ) {
          $out_file = $prefix ;
        }
      } 
      elsif( $arg =~ /\.c$/ ) {
        $prefix = $` ;
        push @CSRCFILES, $arg ;
        if( $out_file eq "" ) {
          $out_file = $prefix ;
        }
      } 
      elsif( $arg =~ /\.a$/ ) {
        push @CLINKFILES, $arg ;
      } 
      elsif( $arg =~ /\.o$/ ) {
        push @CLINKFILES, $arg ;
      }
      elsif( $arg =~ /\.so$/ ) {
        push @CLINKFILES, $arg ;
      }
      elsif( $arg =~ /\.dl$/ ) {
        push @CLINKFILES, $arg ;
      }
      else {
        print stderr "Unkown file type $arg\n" ;
        exit(1) ;
      }
    }
  }

}

sub j_compile {
  my $jfile, $command, $prefix, $ofile ;
  
  foreach $jfile (@JEDASRCFILES) {
    $command = "$JEDA_HOME/bin/$JEDA_COMP $CMP_ARGS $PP_ARGS $jfile" ;
    if( $debug_flag ) {
      printf stderr "$command\n" ;
    }
    else {
      system ($command) ;
      if ($? != 0) {
        die "$command : Jeda compile failed\n";
      }
    }
    if( !$jc_flag ) {
      $jfile =~ /\.j$/ ;
      $prefix = $` ;
      $ofile = $prefix.".jo" ;
      #print stderr $ofile ;
      push @JEDAOBJFILES, $ofile ;
    }
  }
  
}

sub j_link {
  my $lfile, $command ;
  
  my $files = "" ;
  foreach $lfile (@JEDAOBJFILES) {
    #print stderr $lfile ;
    $files .= " ".$lfile ;
  }
  $ofile = $out_file ;
  if( $files ne "" ) {
    $command = "$JEDA_HOME/bin/$JEDA_LINK -JL_o $ofile $JL_ARGS $files" ;
    if( $debug_flag ) {
      printf stderr "$command\n" ;
    }
    else {
      system $command ;
      if ($? != 0) {
        die "$command : Jeda link failed\n" ;
      }
    }
    if( !$jc_flag ) {
      push @CSRCFILES, $ofile.".jeda.c" ;
    }
  }
}


sub c_compile {
  my $cfile, $command ;
  
  $idir = "$JEDA_HOME/include" ;
  
  foreach $cfile (@CSRCFILES) {
    $cfile =~ /\.c$/ ;
    $prefix = $` ;
    $ofile = $prefix.".o" ;
    push @COBJFILES, $ofile ;
    if( $vpi_flag ) {
      $DEFVPI = "-DJEDA_VPI_MODE" ;
    }
    else {
      $DEFVPI = "" ;
    }
    if( $debugger_enable ) {
      $command = 
        "$CC_NAME -I$idir -c -g -D_REENTRANT -DJEDA_DEBUGGER $DEFVPI -fPIC $PP_ARGS $cfile -o $ofile" ;
    }
    else {
      $command = 
        "$CC_NAME -I$idir -c -O2 -D_REENTRANT $DEFVPI -fPIC $PP_ARGS $cfile -o $ofile" ;
    }
    if( $debug_flag ) {
      printf stderr "$command\n" ;
    }
    else {
      print "C compile: $command \n" ;
      system $command ;
      if ($? != 0) {
        die "$command : Jeda compile failed\n";
      }
    }
  }
}

sub c_link {
  my $lfile, $command ;
  
  my $files = "" ;
  foreach $lfile (@COBJFILES) {
    $files .= " ".$lfile ;
  }
  foreach $lfile (@CLINKFILES) {
    $files .= " ".$lfile ;
  }
  if( $dl_flag ) {
    if( $ivl_flag ) {
      # Link a module for icarus verilog
      $ofile = $out_file.".vpi" ;
      $RLIB = $VPIRUNLIB ;
      $DRLIB = $VPIRUNDBGLIB ;
      if( $debugger_enable ) {
        $command = 
          "$CC_NAME -shared -D_REENTRANT -o $ofile $files $IVLTBLOBJ $DRLIB $READLINEOBJ -lnsl -lm -ll -lc -lpthread -ltermcap  -lvpi " ;
      }
      else {
        $command = 
          "$CC_NAME -shared -D_REENTRANT -o $ofile $files $IVLTBLOBJ $RLIB -lnsl -lm -ll -lc -lpthread -ltermcap  -lvpi" ;
      }  
      if( $debug_flag ) {
        printf stderr "$command\n" ;
      }
      else {
        print "Link command: $command \n" ;
        system $command ;
        if ($? != 0) {
          die "$command : Jeda link failed\n";
        }
      }
      
    } else {
      $ofile = $out_file.".dl" ;
      if( $vpi_flag ) {
        $RLIB = $VPIRUNLIB ;
        $DRLIB = $VPIRUNDBGLIB ;
      }
      else {
        $RLIB = $RUNLIB ;
        $DRLIB = $RUNDBGLIB ;
      }
      if( $debugger_enable ) {
        $command = 
          "$CC_NAME -shared -D_REENTRANT -o $ofile $files $DRLIB $READLINEOBJ -lnsl -lm -ll -lc -lpthread -ltermcap  " ;
      }
      else {
        $command = 
          "$CC_NAME -shared -D_REENTRANT -o $ofile $files $RLIB -lnsl -lm -ll -lc -lpthread -ltermcap  " ;
      }  
      if( $debug_flag ) {
        printf stderr "$command\n" ;
      }
      else {
        print "Link command: $command \n" ;
        system $command ;
        if ($? != 0) {
          die "$command : Jeda link failed\n";
        }
      }
    }
  }
  if( $exe_flag ) {
    $ofile = $out_file ;
    if( $debugger_enable ) {
      $command = 
        "$CC_NAME -g -D_REENTRANT -o $ofile $files $SMAINOBJ $EXEDBGLIB $READLINELIB $HISTORYLIB -lnsl -lm -ll -lc -lpthread -ltermcap  " ;
    }
    else {
      $command = 
        "$CC_NAME -g -D_REENTRANT -o $ofile $files $SMAINOBJ $EXELIB -lnsl -lm -ll -lc -lpthread -ltermcap  " ;
    }
    if( $debug_flag ) {
      printf stderr "$command\n" ;
    }
    else {
      printf stderr "Link command: $command\n" ;
      system $command ;
      if ($? != 0) {
        die "$command : Jeda link failed\n";
      }
    }
  }  
}

###########################################################################
## beginning of the script
###########################################################################

init() ;

read_args() ;

j_compile() ;

j_link() ;

c_compile() ;

c_link() ;

