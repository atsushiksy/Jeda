#! /homes/atsushi/perl/bin/perl
#  
#  gen_spec.pl < spec/language_spec.prep > spec/language.spec
#
#  This script does:
#     1) When detect #BEGIN <section>, copy syntax/<section>.stx files with
#        some formating
#     2) When detect #KEYWORD <file>, copy <file> files with
#        some formating
#        
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
  if( $line =~ /^\s*#\s*BEGIN\s+(\S+)/ ) {
    $section = $1 ;
    if( $file_opened ) {
      close STX ;
    }
    $file_name = "syntax/$section.stx" ;
    open STX, $file_name or die "Can't open $file_name\n" ;
    # print   "    |**** YACC parser for $section **********\n" ;
    while( <STX> ) {
      print "    $_" ;
    }
    #print   "    |**** end $section **********\n" ;
    close STX ;
  }
  elsif( $line =~ /^\s*#\s*KEYWORD\s+(\S+)/ ) {
    $file_name = $1 ;
    open KEY, $file_name or die "Can't open $file_name\n" ;
    #print   "    |**** keyword/operator in $file_name **********" ;
    $count = 6 ;
    while( <KEY> ) {
      chop ;
      $line = $_ ;
      if( $line =~ /^#/ ) {
        # comment
      }
      elsif( $line =~ /^\s*(\S+)/ ){
        if( $count > 5 ) {
          print   "\n    " ;
          $count = 0 ;
        }
        $key = $1 ;
        print "\t$key" ;
        $count++ ;
      }
    }
    if( $count ) { print   "\n" ; }
    #print   "    |**** end $file_name **********\n" ;
    close KEY ;
  }
  else {
    print "$line\n" ;
  }

}
