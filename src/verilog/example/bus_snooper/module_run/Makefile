########################################################################
## standard jeda compilation 
########################################################################
.SUFFIXES: .j .jo .jh .dl .jmo

.j.jo:
	cd ${<D} ; ${COMPILE.j} ${JEDA_FLAG} ${<F}

.j.jh:
	cd ${<D} ; \
	${COMPILE_LINK.jmo}  ${JEDA_FLAG} ${<F}

.j.jmo:
	cd ${<D} ; \
	${COMPILE_LINK.jmo}  ${JEDA_FLAG} ${<F}

.jo.dl:
	cd ${<D} ; \
	${COMPILE_LINK.j}  ${JEDA_FLAG} ${<F} ${MODULE_OBJ}

JEDA_FLAG = -O4

COMPILE.j = jedax -jc -h 

#COMPILE_LINK.j = jedax -dl -JL_V -DDATA_GC_ASSERT
COMPILE_LINK.jmo = jedax -h -jmo -JL_V   
COMPILE_LINK.j = jedax -dl -JL_V  
COMPILE_LINK_EXE.j = jedax -exe -JL_V 

########################################################################

INC_FILES := $(wildcard ../include/*.jh) 

L0_UTIL_SRC := $(wildcard ../utils/*_0.j)
L0_UTIL_OBJ = ${L0_UTIL_SRC:.j=.jmo}
L0_UTIL_HDR = ${L0_UTIL_SRC:.j=.jh}

L1_UTIL_SRC := $(wildcard ../utils/*_1.j)
L1_UTIL_OBJ = ${L1_UTIL_SRC:.j=.jmo}
L1_UTIL_HDR = ${L1_UTIL_SRC:.j=.jh}

L2_UTIL_SRC := $(wildcard ../utils/*_2.j)
L2_UTIL_OBJ = ${L2_UTIL_SRC:.j=.jmo}
L2_UTIL_HDR = ${L2_UTIL_SRC:.j=.jh}

MODEL_SRC := $(wildcard ../models/*.j)
MODEL_OBJ = ${MODEL_SRC:.j=.jmo}
MODEL_HDR = ${MODEL_SRC:.j=.jh}

MODULE_SRC = ${L0_UTIL_SRC} ${L1_UTIL_SRC} ${L2_UTIL_SRC} ${MODEL_SRC}
MODULES = $(subst .j,.jmo, ${MODULE_SRC})
MODULE_FLAG = $(patsubst %,-m %, ${MODULES})

TEST_SRC := $(wildcard ../tests/*.j)
TEST_OBJ = ${TEST_SRC:.j=.jo}
TEST_HDR = ${TEST_SRC:.j=.jh}
TEST_DL = ${TEST_OBJ:.jo=.dl}
TEST_EXE = $(subst .jo, , ${TEST_OBJ})

SUITE_SRC = ../suites/snoop_test_standalone_suite.j

SUITE_OBJ = ${SUITE_SRC:.j=.jo}

#all: ${TEST_EXE}


${TEST_EXE}: ${TEST_OBJ} ${MODEL_OBJ} ${L2_UTIL_OBJ} ${L1_UTIL_OBJ} ${L0_UTIL_OBJ} ${SUITE_OBJ}
	cd ${@D} ; \
	${COMPILE_LINK_EXE.j} ${JEDA_FLAG} ${MODULE_FLAG} ${@F}.jo ${SUITE_OBJ}

${TEST_OBJ}: ${INC_FILES} ${MODEL_HDR} ${L0_UTIL_HDR} \
             ${L1_UTIL_HDR} ${L2_UTIL_HDR}

${SUITE_OBJ}: ${INC_FILES} ${MODEL_HDR} ${L0_UTIL_HDR} \
              ${L1_UTIL_HDR} ${L2_UTIL_HDR}

${L0_UTIL_OBJ}: ${INC_FILES}

${L0_UTIL_HDR}: ${INC_FILES}

${L1_UTIL_OBJ}: ${INC_FILES} ${L0_UTIL_HDR}

${L1_UTIL_HDR}: ${INC_FILES} ${L0_UTIL_HDR}

${L2_UTIL_OBJ}: ${INC_FILES} ${L0_UTIL_HDR} ${L1_UTIL_HDR}

${L2_UTIL_HDR}: ${INC_FILES} ${L0_UTIL_HDR} ${L1_UTIL_HDR}

${MODEL_OBJ}: ${INC_FILES} ${L0_UTIL_HDR} ${L1_UTIL_HDR} ${L2_UTIL_HDR}

${MODEL_HDR}: ${INC_FILES} ${L0_UTIL_HDR} ${L1_UTIL_HDR} ${L2_UTIL_HDR}

clean:
	${RM} ${L0_UTIL_OBJ} ${L1_UTIL_OBJ} ${L2_UTIL_OBJ} \
	${MODEL_OBJ} ${SUITE_OBJ} ${TEST_OBJ} ${TEST_DL} \
	${L0_UTIL_HDR} ${L1_UTIL_HDR} ${L2_UTIL_HDR} ${MODEL_HDR}
