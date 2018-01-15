#! /usr/bin/perl
#  /homes/atsushi/perl/bin/perl
#
#  gen_parser.pl > jeda.y
#
#  This script does:
#     1) based on the keyword data file, it replace the pattern '<keyword>'
#        to the given number representation of the keyword
#     2) if -e option is declared, uncomment the error recovery section
#        started with ERROR_BEGIN and terminated with ERROR_END
#     3) expand action and decode $keyword in the action field.
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

$error_recover = 1 ;  

$supress_output = 0 ;

# read keyword file and generate conversion pattern
#open KEY, "jdb_keyword.data" or die "Can't open jdb_keyword.data\n" ;
#
#while( <KEY> ) {
#  if( /^\s*(\w+)/ ) {
#    $keyword = $1 ;
#    $token = "JDT_".$keyword ;
#    #printf stderr "Keyword: $keyword token: $token\n" ;
#    $keyword_tbl{$keyword} = $token ;
#  }
#}
#
#close KEY ;

open OPR, "jdb_operators.data" or 
  die "Can't open jeda_operators.data\n" ;
while( <OPR> ) {
  if( /^\s*#/ ) {
     #comment
  }
  elsif( /^\s*(\S+)\s*(\w+)/ ) {
    $keyword = $1 ;
    $token = $2 ;
    #printf stderr "Keyword: $keyword token: $token\n" ;
    $keyword_tbl{$keyword} = "JD_".$token ;
  }
  
}

open TMP, "jdb.y.pp" or die "Can't open jdb.y.pp\n" ;

while( <TMP> ) {
  chop ;
  $line = $_ ;
  if( $line =~ /^\s*\/\// ) {
    # comment line
    print "\n" ;
  }
  #elsif( $line =~ /\<\<\S+\>\>/ ) {
  #  ## skip action
  #}
  elsif( $line =~ /^\s*#\s*BEGIN\s+(\w+)/ ) {
    print "\n" ;
  }
  elsif( $line =~ /^\s*#\s*END/ ) {
    print "\n" ;
  }
  elsif( $line =~ /^\s*#\s*INCLUDE\s*\"(\S+)\"\s*$/ ) {
    $incfile = $1 ;
    #printf stderr "#INCLUDE $incfile\n" ;
    open INC, $incfile or
      die "Can't open $incfile\n" ;
    while( <INC> ) {
      print $_ ;
    }
    close INC ;
  }
  elsif( $line =~ /^\s*ERROR_BEGIN/ ) {
    if( $error_recover ) {
      print "  /* BEGIN Error Recovery section */\n" ;
    }
    else {
      print "  /* Error Recovery section is suppressed */\n" ;
      $supress_output = 1 ;
    }
  }
  elsif( $line =~ /^\s*ERROR_END/ ) {
    if( $error_recover ) {
      print "  /* End Error Recovery section */\n" ;
    }
    else {
      print "  /* till here */\n" ;
      $supress_output = 0 ;
    }
  }
  elsif( $supress_output ) {
    print "  /* $line */\n" ;
  }
  else {
    $R = $line ;
    while( $R =~ /\'(\S+)\'/ ) {
      $keyword = $1 ;
      # print stderr " got $keyword\n" ;
      $p = $` ;
      $R = $' ;
      print $p ;
      $token = $keyword_tbl{$keyword} ;
      if( $token ) {
        print "$token" ;
      }
      else {
        print "\'$keyword\'" ;
      }
    }
    print "$R\n" ;
  }

}
