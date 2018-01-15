#!/usr/bin/perl
#
#
#
$prt = 1 ;
$n = 0 ;
while( <> ) {
  if( /^\s*#\s*ifndef\s+JD_REFCOUNT_GC/ ) {
    $prt = 1 ;
    $n++ ;
  }
  elsif( /^\s*#\s*ifdef\s+JD_REFCOUNT_GC/ ) {
    $prt = 0 ;
    $n++ ;
  }
  elsif( $n ) {
    if( /^\s*#\s*ifdef/ ) { $n++ ; if( $prt ) { print $_ ; } }
    elsif( /^\s*#\s*ifndef/ ) { $n++ ; if( $prt ) { print $_ ; } }
    elsif( /^\s*#\s*endif/ ) {
      $n-- ;
      if( $n == 0 ) {  $prt = 1 ; }
      elsif( $prt ) { print $_ ; }
    }
    elsif( /^\s*#\s*else/ ) {
      if( $n == 1 ) {
        if( $prt ) { $prt = 0 ; }
        else { $prt = 1 ; }
      }
      elsif( $prt ) { print $_ ; }
    }
    elsif( $prt ) { print $_ ; }
  }
  else {
    $prt = 1 ;
    print $_ ;
  }
  

}