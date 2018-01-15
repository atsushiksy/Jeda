#! /homes/atsushi/perl/bin/perl
#  
#  gen_syntax.pl < jeda.y.pp
#
#  This script does:
#     1) based on #BEGIN <section>, generate <section>.stx files to
#        syntax directory
#     2) sections within "  {" and "  }" (two space and bracket) are
#        supressed (assumed to be an action portion of the parse tree).
#     3) Supress #ACTION to #END 
#
# 
#  
#  Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
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
#

$file_opened = 0 ;

while( <> ) {
  chop ;
  $line = $_ ;
  if( $line =~ /^\s*\/\// ) {
    # comment line
    # print "\n" ;
  }
  elsif( $line =~ /\<\S+\>/ ) {
    ## skip action
  }
  elsif( $line =~ /^\s*#\s*BEGIN\s+(\w+)/ ) {
    $section = $1 ;
    if( $file_opened ) {
      close STX ;
    }
    $file_name = ">syntax/$section.stx" ;
    open STX, $file_name or die "Can't open $file_name\n" ;
    $file_opened = 1 ;
    print stderr "  Writting to $file_name..\n" ;
  }
  elsif( $line =~ /^\s*#\s*END/ ) {
    close STX ;
    $file_opened = 0 ;
  }
  elsif( $line =~ /^  {/ ) {
    $supress_output = 1 ;
  }
  elsif( $line =~ /^  }/ ) {
    $supress_output = 0;
  }
  elsif( $line =~ /^\s*ERROR_BEGIN/ ) {
    $supress_output = 1 ;
  }
  elsif( $line =~ /^\s*ERROR_END/ ) {
    $supress_output = 0 ;
  }
  elsif( $file_opened && !$supress_output ) {
    if( $line =~ /^\s*#\s*INCLUDE\s*\"(\S+)\"\s*$/ ) {
      $incfile = $1 ;
      printf stderr "#INCLUDE $incfile\n" ;
      open INC, $incfile or
        die "Can't open $incfile\n" ;
      while( <INC> ) {
        print STX $_ ;
      }
      close INC ;
    }
    else {
      print STX "$line\n" ;
    }
  }

}
