#! /opt/gnu/bin/perl4.036
#/usr/bin/perl
#
#  perl gen_lfile.pl > jeda.l
#
#  This script does:
#     1) simply exapnds #INCLUDE construct for 
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

$srcfile = "jeda.l.pp";

{

  open(TMP, $srcfile) or die "Can't open $srcfile \n" ;
  while(<TMP>) {
    chop ;
    $line = $_ ;
    if( $line =~ /^\s*#\s*INCLUDE\s*\"(\S+)\"\s*$/ ) {
      $incfile = $1 ;
      #printf stderr "#INCLUDE $incfile\n" ;
      open INC, $incfile or
        die "Can't open $incfile\n" ;
        while( <INC> ) {
        print $_ ;
      }
      close INC ;
    }
    else {
      print "$line\n";
    }
  }
}
