#!/usr/bin/perl

while(<>) {
  if( /(\S+)/ ) {
    print " '$1', \n" ;
  }
}
