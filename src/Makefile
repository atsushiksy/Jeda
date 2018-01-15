MK = gmake

all: 
	${RM} -r -f release
	mkdir release
	cd release ; mkdir bin ; mkdir lib ; mkdir include ; mkdir doc
	cd release/lib ; mkdir run_obj ; mkdir dbg_obj ; mkdir rl_obj
	cd release/lib ; mkdir vpi_obj ; mkdir vpidbg_obj
	cd release/doc ; mkdir html
	cd release/include ; mkdir readline
	${RM} -f runtime/.release
	cd readline ; ${MK}
	cd compiler ; ${MK}
	cd runtime ; ${MK}
	cd linker ; ${MK}
	cd jedapp ; ${MK}
	cd jedatemp ; ${MK}
	cd vpp ; ${MK}
	cp compiler/jedacmp release/bin
	cp linker/jedalnk release/bin
	cp jedapp/build_gcc/gcc/jedapp release/bin
	cp jedatemp/jedatemp release/bin
	cp vpp/vpp/src/vpp release/bin
	cp linker/jeda release/bin
	cp runtime/debug_window.tcl release/bin
	cd include ; ../compiler/jedacmp -X jeda_utils.jh
	cp include/jeda_utils.jo release/include
	cp include/jeda_utils.jh release/include
	cp include/jeda_pli_func_def.h release/include
	cp include/jeda_pli_func_ext.h release/include
	cp include/jeda_vpi_func_def.h release/include
	cp include/jeda_vpi_func_ext.h release/include
	cp include/pli.tab release/include
	cp ../doc/html/*.html release/doc/html
	cp ../doc/spec/jeda_parser.spec ../doc/html
	cp ../doc/spec/jeda_parser.spec release/doc/html
	


clean:
	cd compiler ; ${MK} clean
	cd runtime ; ${MK} clean
	cd linker ; ${MK} clean
