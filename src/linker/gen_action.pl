#! /usr/bin/perl
#  /homes/atsushi/perl/bin/perl
#
#  gen_action.pl  < jeda.y.t > jeda.y
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
 
open ASF, "vinst_parser_action.data" 
  or die "Can't open vinst_parser_action.data\n" ;
  

while( <ASF> ) {
  $line = $_ ;
  if( $line =~ /^\s*#/ ) {
    # comment line
  }
  elsif( $line =~ /^\s*\<\<(\w+)\>\>/ ) {
    $block = $1 ;
    $loop = 1 ;
    while($loop) {
      $line  = <ASF> ;
      $action{$block} .= "  ".$line ;
      if( $line =~ /^\}/ ) {
        $loop = 0 ;
      }
      # print stderr $line ;
    }
    # print stderr "$block defined\n" ;
  }
}

close ASF ;

# print stderr "action data parse completed\n" ;

while( <> ) {
  $line = $_ ;
  if( $line =~ /^\s*\/\// ) {
    # comment line
    print "\n" ;
  }
  elsif( $line =~ /^\s*\<\<(\w+)\>\>/ ) {
    $block = $1 ;
    $act = $action{$block} ;
    if( $act eq "" ) {
      print stderr "Undefined action $block detected\n" ;
    }
    else {
       print $act ;
    }
  }
  else {
    print $line ;
  }
}
close PSF ;


 

