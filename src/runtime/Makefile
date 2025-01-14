MK = gmake

MACHINE = i386
SYSDEP = sysdep/i386

VERILOG = icarus

# for debugging GC run with
# -DDATA_GC_DEBUG will invoke GC on all the data allocation (very slow)
# -DDATA_GC_CHECK will invoke stack check on GC
# -DGEN_ASSERT_STACK_ALIVE=10 will generate ASSERT_STACK_ALIVE in the code
#                          every 10 instruction
# -DDATA_GC_ASSERT will invoke the stack check occasionally upon execution
# -DDATA_GC_GLOBAL_CHECK will check if global data is not in free structure
# -DDEBUG_MEM_PROBLEM will call jeda_free_error when free address matches
#    with jeda_debug_address. This jeda_debug_address is set to zero, so
#    it must be set upon run.
#
#DFLAG = -DDATA_GC_CHECK -DDATA_GC_DEBUG -DNOTIFY_GC_EXEC -DJEDA_PLI_DEBUG
#        -DJEDA_DEBUG_THREAD -DDATA_GC_DEBUG -DDATA_GC_ASSERT 
# -DNOTIFY_GC_EXEC -DDEBUG_MEM_PROBLEM

ifeq ($(MACHINE),sparc)
  DFLAG = -D_REENTRANT -DJEDA_SELF_THREAD_CONTROL
  LWP_SRC = ${SYSDEP}/jeda_lwp.S
  LWP_OBJ = ${LWP_SRC:.S=.o}
else
  DFLAG = -D_REENTRANT 
endif

ifeq ($(VERILOG),icarus)
  ICARUSFLAG = -DICARUS_VERILOG_MODE 
  IVLTBL_SRC = ivl_sys_table.c
  IVLTBL_OBJ = ${IVLTBL_SRC:.c=.o}
else

endif

#CC = gcc  -g -lang-c-c++-comments -I ../include -I ../vinclude -D_POSIX_THREADS -D_POSIX_C_SOURCE=199506L -DJEDA_VERILOG_LINK ${DFLAG} -fPIC -DVERILOG_IO
CC = gcc  -O2 -lang-c-c++-comments -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK ${DFLAG} -fPIC -DVERILOG_IO

#SCC = gcc  -g -lang-c-c++-comments -I ../include -I ../vinclude -D_POSIX_THREADS -D_POSIX_C_SOURCE=199506L -DJEDA_VERILOG_LINK ${DFLAG} -fPIC 

SCC = gcc  -g -lang-c-c++-comments -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK -DJEDA_STANDALONE_MODE ${DFLAG} -fPIC 

DCC = gcc  -g -lang-c-c++-comments -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK ${DFLAG} -fPIC -DVERILOG_IO -DJEDA_DEBUGGER

SDCC = gcc  -g -lang-c-c++-comments -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK ${DFLAG} -fPIC  -DJEDA_STANDALONE_MODE -DJEDA_DEBUGGER

VCC = gcc  -O2 -lang-c-c++-comments ${ICARUSFLAG} -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK -DJEDA_VPI_MODE ${DFLAG} -fPIC -DVERILOG_IO

VDCC = gcc  -g -lang-c-c++-comments ${ICARUSFLAG} -I ../include -I ../vinclude -I ../readline/readline-4.2 -DREADLINE_LIBRARY -D_POSIX_THREADS  -DJEDA_VERILOG_LINK -DJEDA_VPI_MODE ${DFLAG} -fPIC -DVERILOG_IO -DJEDA_DEBUGGER

RUN_SRC = data.c thread.c code_exec.c top_loop.c printf.c \
          exec_b_i_alu_ops.c exec_float_alu_ops.c exec_int_alu_ops.c \
          exec_object.c exec_pli.c core_sys_funcs.c pli.c random.c string.c \
          regexp_exec.c socket.c vpi.c

DBG_SRC = debugger.c debugger_print.c debugger_ui.c debugger_eval.c \
          debugger_scope.c
RUN_OBJ = ${RUN_SRC:.c=.o}

SDIR = sobj/
SRUN_OBJ = $(addprefix ${SDIR}, ${RUN_OBJ} )

VDIR = vobj/
VPI_OBJ = $(addprefix ${VDIR}, ${RUN_OBJ} )

VDDIR = vdobj/
VPIDBG_OBJ = $(addprefix ${VDDIR}, ${ALL_OBJ} )

ALL_SRC = ${RUN_SRC} ${DBG_SRC}
ALL_OBJ = ${ALL_SRC:.c=.o}

DDIR = dobj/
DBG_OBJ = $(addprefix ${DDIR}, ${ALL_OBJ} )

SDDIR = sdobj/
SDBG_OBJ = $(addprefix ${SDDIR}, ${ALL_OBJ} )

MAIN_SRC = smain.c
MAIN_OBJ = ${MAIN_SRC:.c=.o}
SMAIN_OBJ = $(addprefix ${SDIR}, ${MAIN_OBJ} )

REG_DIR = ../regexp
REG_OBJ = ${REG_DIR}/regexp.o 

REG_INC = ${REG_DIR}/regexp.h

INC_DIR = ../include
RUN_INC = ${INC_DIR}/runtime.h   ${INC_DIR}/mem_macro.h \
          ${INC_DIR}/runtime_error_msg.h ${INC_DIR}/bit_operations.h \
          ${INC_DIR}/pli.h ${INC_DIR}/jeda_pli.h \
          ${INC_DIR}/exec_macro.h ${INC_DIR}/data.h ${INC_DIR}/thread.h \
          ${INC_DIR}/ip_socket.h

RAND_INC = ${INC_DIR}/random.h ${INC_DIR}/rand48_port.h \
           ${INC_DIR}/mt19937_port.h

RUNTIME_INC = ${INC_DIR}/jeda_runtime.h

DBG_INC = ${INC_DIR}/debugger.h ${INC_DIR}/debugger_error_msg.h

RUN_DIR = ../runtime

MACRO_INC =  ${INC_DIR}/bit_macro.h  

DATA_MACRO_INC =  ${INC_DIR}/data_macro.h  
CODEEXEC_MACRO_INC =  ${INC_DIR}/code_exec_macro.h  
THREAD_MACRO_INC =  ${INC_DIR}/thread_macro.h  

EXECOBJ_MACRO_INC =  ${INC_DIR}/exec_object_macro.h  

COMPILE_INC = ${INC_DIR}/code_exec_header.h ${INC_DIR}/exec_b_i_alu_ops.h \
    ${INC_DIR}/exec_b_i_alu_header.h ${INC_DIR}/exec_float_alu_ops.h \
    ${INC_DIR}/exec_float_alu_header.h ${INC_DIR}/exec_int_alu_ops.h \
    ${INC_DIR}/exec_int_alu_header.h ${INC_DIR}/exec_object_header.h \
    ${INC_DIR}/exec_pli_macro.h

JEDA_UTIL_INC = ${INC_DIR}/jeda_utils.jh

VERIUSER_C =  ${INC_DIR}/veriuser.c

RELEASE_FILES = ${RUN_INC} ${COMPILE_INC} ${MACRO_INC} ${DATA_MACRO_INC} \
        ${CODEEXEC_MACRO_INC} ${EXECOBJ_MACRO_INC} ${THREAD_MACRO_INC} \
        ${VERIUSER_C} 

BIT_OBJ = bit_operations.o
BIT_SRC = ../compiler/bit_operations.c

JPLI_SRC = jeda_pli.c
JPLI_OBJ = ${JPLI_SRC:.c=.o}

JPLI_STATIC_OBJ = jeda_pli_static.o

JVPI_SRC = jeda_vpi.c
JVPI_OBJ = ${JVPI_SRC:.c=.o}

JVPI_STATIC_OBJ = jeda_vpi_static.o


LEX_PINC = jdb_operator_lex.h
LEX_INC =  y_tab.h  

LEX_SRC = lex.yy.c
LEX_OBJ = ${LEX_SRC:.c=.o}

YACC_PINC = jdb_operator_token.h
YACC_SRC = y_tab.c
YACC_OBJ = ${YACC_SRC:.c=.o}
YACC_HDR = y_tab.h

SDBG_YACC_OBJ = $(addprefix ${SDDIR}, ${YACC_OBJ} )
VDBG_YACC_OBJ = $(addprefix ${VDDIR}, ${YACC_OBJ} )

RUN_LIB = jedaexe.a
DBG_LIB = jedadbg.a

TARGET = ${ALL_OBJ} ${SRUN_OBJ} ${BIT_OBJ} ${JPLI_OBJ} ${JPLI_STATIC_OBJ} \
         ${SMAIN_OBJ} ${JVPI_STATIC_OBJ} \
         ${REG_OBJ} ${DBG_OBJ} ${YACC_OBJ} ${LEX_OBJ} ${SDBG_OBJ} \
         ${SDBG_YACC_OBJ} ${VDBG_YACC_OBJ} ${LWP_OBJ} ${VPI_OBJ} ${VPIDBG_OBJ} ${JVPI_OBJ}

#redefine to compile all 4 objects
.c.o:
	${CC} -c -o $@ $<
	${SCC} -c -o $(addprefix ${SDIR}, $@) $<
	${DCC} -c -o $(addprefix ${DDIR}, $@) $<
	${SDCC} -c -o $(addprefix ${SDDIR}, $@) $<
	${VCC} -c -o $(addprefix ${VDIR}, $@) $<
	${VDCC} -c -o $(addprefix ${VDDIR}, $@) $<

.S.o:
	${CC} -c -o $@ $<
	cp $@ ${SDIR}
	cp $@ ${DDIR}
	cp $@ ${SDDIR}
	cp $@ ${VDIR}
	cp $@ ${VDDIR}

all: .release

.release:  ${RELEASE_FILES} ${RUNTIME_INC} ${JPLI_OBJ} ${JPLI_STATIC_OBJ} \
           ${ALL_OBJ} ${BIT_OBJ} ${RUN_LIB} ${SMAIN_OBJ} ${DBG_OBJ} \
           ${YACC_OBJ} ${LEX_OBJ} ${DBG_LIB} ${LWP_OBJ} \
           ${VPI_OBJ} ${VPIDBG_OBJ} ${JVPI_OBJ} ${JVPI_STATIC_OBJ} ${IVLTBL_OBJ}
	cp ${RELEASE_FILES} ${RUNTIME_INC} ../release/include
	cp ${RUN_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ} ../release/lib/run_obj
	cp ${DBG_OBJ} ${BIT_OBJ} ${REG_OBJ} ${YACC_OBJ} ${LEX_OBJ} ${LWP_OBJ} ../release/lib/dbg_obj
	cp ${VPI_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ} ../release/lib/vpi_obj
	cp ${VPIDBG_OBJ} ${BIT_OBJ} ${REG_OBJ} ${VDBG_YACC_OBJ} ${LEX_OBJ} ${LWP_OBJ} ../release/lib/vpidbg_obj
	cp ${RUN_LIB} ../release/lib
	cp ${DBG_LIB} ../release/lib
	cp ${JPLI_OBJ} ../release/lib
	cp ${JPLI_STATIC_OBJ} ../release/lib
	cp ${JVPI_STATIC_OBJ} ../release/lib
	cp ${JVPI_OBJ} ${IVLTBL_OBJ} ../release/lib
	cp ${SMAIN_OBJ} ../release/lib
	cp ${REG_OBJ} ../release/lib
	touch .release

${JPLI_OBJ}: ${INC_DIR}/jeda_pli.h

${JPLI_STATIC_OBJ}: ${JPLI_SRC} ${INC_DIR}/jeda_pli.h
	${CC} -DJEDA_STATIC_LINK -c -o $@ ${JPLI_SRC}

${JVPI_STATIC_OBJ}: ${JVPI_SRC} ${INC_DIR}/jeda_pli.h
	${VCC} -DJEDA_STATIC_LINK -c -o $@ ${JVPI_SRC}

${JVPI_OBJ}: ${INC_DIR}/jeda_pli.h
	${VCC} -c -o $@ ${JVPI_SRC}

${RUN_LIB}: ${SRUN_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ}
	${RM} -f $@
	ar -r $@ ${SRUN_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ}

${DBG_LIB}: ${SDBG_OBJ} ${BIT_OBJ} ${REG_OBJ} ${SDBG_YACC_OBJ} ${LEX_OBJ} ${LWP_OBJ}
	${RM} -f $@
	ar -r $@ ${SDBG_OBJ} ${BIT_OBJ} ${REG_OBJ} ${SDBG_YACC_OBJ} ${LEX_OBJ} ${LWP_OBJ}

jedarun.dl: ${RUN_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ}
	${RM} $@
	gcc -shared -o $@ ${RUN_OBJ} ${BIT_OBJ} ${REG_OBJ} ${LWP_OBJ}

${ALL_OBJ}: ${RUN_INC} ${MACRO_INC} \
            ${DATA_MACRO_INC} ${CODEEXEC_MACRO_INC} ${EXECOBJ_MACRO_INC} \
            ${COMPILE_INC} ${THREAD_MACRO_INC} ${REG_INC} ${RAND_INC} \
            ${DBG_INC}

clean:
	${RM} ${TARGET}                

    
${MACRO_INC}: ../compiler/bit_operations.c gen_bit_macro.pl
	perl gen_bit_macro.pl < ../compiler/bit_operations.c > $@
	

${DATA_MACRO_INC}: data.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < data.c > $@

${THREAD_MACRO_INC}: thread.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < thread.c > $@

${CODEEXEC_MACRO_INC}: code_exec.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < code_exec.c > $@

${EXECOBJ_MACRO_INC}: exec_object.c  gen_code_out_macro.pl
	perl gen_code_out_macro.pl < exec_object.c > $@

${INC_DIR}/exec_pli_macro.h: exec_pli.c  gen_code_out_macro.pl
	perl gen_code_out_macro.pl < exec_pli.c > $@

${INC_DIR}/code_exec_header.h: code_exec.c gen_header.pl
	perl gen_header.pl < code_exec.c > ${INC_DIR}/code_exec_header.h

${INC_DIR}/exec_b_i_alu_ops.h: exec_b_i_alu_ops.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < exec_b_i_alu_ops.c > $@

${INC_DIR}/exec_b_i_alu_header.h: exec_b_i_alu_ops.c gen_header.pl
	perl gen_header.pl < exec_b_i_alu_ops.c > $@

${INC_DIR}/exec_float_alu_ops.h:  exec_float_alu_ops.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < exec_float_alu_ops.c > $@

${INC_DIR}/exec_float_alu_header.h: exec_float_alu_ops.c gen_header.pl
	perl gen_header.pl < exec_float_alu_ops.c > $@

${INC_DIR}/exec_int_alu_ops.h: exec_int_alu_ops.c gen_code_out_macro.pl
	perl gen_code_out_macro.pl < exec_int_alu_ops.c > $@

${INC_DIR}/exec_int_alu_header.h: exec_int_alu_ops.c gen_header.pl
	perl gen_header.pl < exec_int_alu_ops.c > $@

${INC_DIR}/exec_object_header.h: exec_object.c gen_header.pl
	perl gen_header.pl < exec_object.c > $@

${BIT_OBJ}: ${BIT_SRC}
	${CC} -c ${BIT_SRC} -o ${BIT_OBJ} 

${REG_OBJ}:
	cd ${REG_DIR} ; ${MK}

#${SRUN_OBJ}: ${RUN_SRC} ${RUN_INC} ${MACRO_INC} \
#            ${DATA_MACRO_INC} ${CODEEXEC_MACRO_INC} ${EXECOBJ_MACRO_INC} \
#            ${COMPILE_INC} ${THREAD_MACRO_INC} ${REG_INC} ${RAND_INC}
#	${SCC} -c -o $@ $(addsuffix .c, $(notdir $*))

${SMAIN_OBJ}: ${MAIN_SRC} ${RUN_INC} ${MACRO_INC}
	${SCC} -c -o $@ $<

#${DBG_OBJ}: ${ALL_SRC} ${RUN_INC} ${MACRO_INC} \
#            ${DATA_MACRO_INC} ${CODEEXEC_MACRO_INC} ${EXECOBJ_MACRO_INC} \
#            ${COMPILE_INC} ${THREAD_MACRO_INC} ${REG_INC} ${RAND_INC} \
#            ${DBG_INC}
#	${DCC} -c -o $@ $(addsuffix .c, $(notdir $*))

#${SDBG_OBJ}: ${ALL_SRC} ${RUN_INC} ${MACRO_INC} \
#            ${DATA_MACRO_INC} ${CODEEXEC_MACRO_INC} ${EXECOBJ_MACRO_INC} \
#            ${COMPILE_INC} ${THREAD_MACRO_INC} ${REG_INC} ${RAND_INC} \
#            ${DBG_INC}
#	${SDCC} -c -o $@ $(addsuffix .c, $(notdir $*))

# delived file generation
jdb_operator_lex.h: jdb_operators.data gen_opr_lex.pl
	perl gen_opr_lex.pl < jdb_operators.data > $@

jdb_operator_token.h: jdb_operators.data gen_opr_token.pl
	perl gen_opr_token.pl < jdb_operators.data > $@

jdb.y: jdb.y.pp jdb_operators.data \
            jdb_action.data gen_parser.pl gen_action.pl ${YACC_PINC}
	perl gen_parser.pl < jdb.y.pp | perl gen_action.pl > $@

${YACC_SRC}: jdb.y gen_jd_parse.pl
	bison  -o y_tab_c_t -p JDB_yy -d jdb.y
	perl gen_jd_parse.pl < y_tab_c_t > y_tab.c

y_tab.h: y_tab.c 
	cp y_tab_c_t.h $@

${SDBG_YACC_OBJ}: ${YACC_SRC} ${DBG_INC} ${RUN_INC}
	${SDCC} -c -o $@ ${YACC_SRC}

${VDBG_YACC_OBJ}: ${YACC_SRC} ${DBG_INC} ${RUN_INC}
	${VDCC} -c -o $@ ${YACC_SRC}


## lex generation

jdb.l: gen_lfile.pl jdb.l.pp ${LEX_PINC}
	perl gen_lfile.pl > $@


${LEX_SRC}: jdb.l gen_jd_parse.pl ${LEX_INC}
	flex -PJDB_yy -olex.yy.c.t jdb.l
	perl gen_jd_parse.pl < $@.t > $@

ifeq ($(VERILOG),icarus)

${IVLTBL_OBJ}: ${IVLTBL_SRC}
	${VCC} -c -o $@ ${IVLTBL_SRC}
	
endif
