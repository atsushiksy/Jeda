MK = gmake

PATCH = patch

vpp/src/vpp: Makefile.patch create.patch vpp.lex.patch
	gzip -dc vpp-1.2.tar.gz | tar xvf -
	cd vpp/src ; ${PATCH} -b -z .org Makefile ../../Makefile.patch
	cd vpp/src ; ${PATCH} -b -z .org create.c ../../create.patch
	cd vpp/src ; ${PATCH} -b -z .org vpp.lex ../../vpp.lex.patch
	cd vpp/src ; ${MK}
