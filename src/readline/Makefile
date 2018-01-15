MK = gmake

all: readline-4.2/libreadline.a
	cp readline-4.2/libreadline.a ../release/lib
	cp readline-4.2/libhistory.a ../release/lib
	cp readline-4.2/*.o ../release/lib/rl_obj
	cp readline-4.2/readline.h ../release/include/readline
	cp readline-4.2/history.h ../release/include/readline
	cp readline-4.2/rlstdc.h ../release/include/readline
	cp readline-4.2/keymaps.h ../release/include/readline
	cp readline-4.2/tilde.h ../release/include/readline
	cp readline-4.2/rltypedefs.h ../release/include/readline

readline-4.2/libreadline.a: Makefile.in.diff configure.new
	${RM} -r -f readline-4.2
	gzip -cd readline-4.2.tar.gz | tar xvfp -
	cd readline-4.2 ; patch -b -z .org Makefile.in -i ../Makefile.in.diff
	cp ./configure.new readline-4.2
	cd readline-4.2 ; ./configure.new
	cd readline-4.2 ; ${MK}
