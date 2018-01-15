#! /usr/bin/perl
#
#   gen_node_type.pl < jdb_nonterminal_node_type.data > jdb_nonterminal_node_type.h
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

while( <> ) {
  if( /^\s*#/ ) {
     #comment
  }
  elsif( /^\s*\<(\w+)\>/ ) {
    $type = $1 ;
    # print stderr "Got type $type\n" ;
    $new = 1 ;
    $count = 0 ;
  }
  elsif( /^\s*(\w+)/ ) {
    $nontem = $1 ;
    if( $new || $count++ == 1 ) {
      print "\n\%type <$type>\t" ;
      $new = 0 ;
      $count = 0 ;
    }
    print "$nontem " ;
    
  }
}
print "\n" ;
