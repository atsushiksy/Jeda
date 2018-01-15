########################################################################
## standard jeda compilation 
########################################################################
.SUFFIXES: .j .jo .jh .dl

.j.jo:
	cd ${<D} ; ${COMPILE.j} ${JEDA_FLAG} ${<F}

.j.jh:
	cd ${<D} ; ${COMPILE.j} ${JEDA_FLAG} ${<F}

.jo.dl:
	cd ${<D} ; \
	${COMPILE_LINK.j}  ${JEDA_FLAG} ${<F} ${L0_UTIL_OBJ} ${L1_UTIL_OBJ} \
	${L0_MODEL_OBJ} ${L1_MODEL_OBJ} ${SUITE_OBJ}

JEDA_FLAG = -g

COMPILE.j = jedax -jc -h

#COMPILE_LINK.j = jedax -dl -JL_V -DDATA_GC_ASSERT
COMPILE_LINK.j = jedax -dl -JL_V 

########################################################################

#Change the verilog path to a proper one
VERILOG = ../../XL/LDV2.1*/verilog

TEMP_FILES = ff_port.jh ff_test.v ff_test_template.j

TEST_SRC = ff_test.j
TEST_OBJ = ${TEST_SRC:.j=.jo}
TEST_DL = ${TEST_OBJ:.jo=.dl}


all: ${TEST_DL}

${TEST_DL}: ${TEST_OBJ}

${TEST_OBJ}: ${TEMP_FILES}

${TEMP_FILES}: ff.v
	jedatemp -m ff -c clk ff.v

run_xl:
	${VERILOG} -f vfiles +jeda_code=ff_test.dl

compile_vcs:
	./cmpvcs

run_vcs:
	./simv +jeda_code=ff_test.dl

clean:
	rm -f *.jeda*.* *.jo *.jh *.dl *.so *template.j


