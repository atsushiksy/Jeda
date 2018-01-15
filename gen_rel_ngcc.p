#!/bin/csh -f
setenv CVSROOT /net/kinggidra/export/room/jeda/cvsroot
echo -n $1 >> rel_record.data
echo -n " " >> rel_record.data
echo $2 >> rel_record.data
cd /volume/jchip-verif/atsushi
\rm -r -f $2
cvs -q export -r $1 -d $2 jeda
#cp /homes/atsushi/jeda/src/vpp/vpp-1.2.tar.gz $2/src/vpp
#cp /homes/atsushi/jeda/src/jedapp/gcc-2.95.2.tar.gz $2/src/jedapp
cp /homes/atsushi/jeda/src/readline/readline-4.2.tar.gz $2/src/readline
cp /homes/atsushi/jeda/doc/html/jeda_parser.spec $2/doc/html
cp /homes/atsushi/jeda/doc/html/jeda_parser.spec $2/doc/html_j
\rm -f $2.tar $2.tar.gz
tar cvf $2.tar $2
gzip $2.tar
echo -n $1 >> rel_record.data
echo -n " " >> rel_record.data
echo $2 >> rel_record.data
