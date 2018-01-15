#! /usr/bin/perl
#
#   gen_table.pl < jeda_keyword.data > jeda_keyword_table.h
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

print "static keyword_entry keyword_table[] = {\n" ;
while( <> ) {
  if( /^#/ ) {
    # comment
  }
  elsif( /^\s*(\w+)/ ) {
    $token = $1 ;
    print stderr "Got token $token\n" ;
    print "  {  \"$token\",\t\tJDT_$token },\n" ;
  }
}
print "  {  NULL,  0 }\n" ;
print "} ; \n" ;
