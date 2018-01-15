#! /usr/bin/perl
#
#   gen_sig_table.pl < jeda_port_attrib_keyword.data > jeda_port_attrib_keyword_table.h
#

print "static keyword_entry port_keyword_table[] = {\n" ;
while( <> ) {
  if( /^\s*(\w+)/ ) {
    $token = $1 ;
    print stderr "Got token $token\n" ;
    print "  {  \"$token\",\t\tJD_$token },\n" ;
  }
}
print "  {  NULL,  0 }\n" ;
print "} ; \n" ;
