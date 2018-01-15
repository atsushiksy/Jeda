#! /usr/bin/perl
#
#   perl gen_bit_macro.pl < ../compiler/bit_operations.c > ../include/bit_macro.h
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

print "/********************************************/\n" ;
print "/* macro generated from bit_operations.c    */\n" ;
print "/********************************************/\n" ;
while( <> ) {
  if( /MACRO\s+(\w+)/ ) {
    $macro_name = $1 ;
    #print stderr "Got macro $macro_name\n" ;
    print "#define $macro_name \\\n" ;
    $line = <> ;
    chop $line ;
    while( !($line =~ /END MACRO/ ) ) {
      print "$line \\\n" ;
      $line = <> ;
      chop $line ;
    }
    print "\n" ;
  }
}
