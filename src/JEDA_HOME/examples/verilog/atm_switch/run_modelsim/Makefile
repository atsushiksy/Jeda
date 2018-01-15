########################################################################
## standard jeda compilation 
########################################################################
.SUFFIXES: .j .jo .jh .so

.j.jo:
	cd ${<D} ; ${COMPILE.j} ${JEDA_FLAG} ${<F}

.j.jh:
	cd ${<D} ; ${COMPILE.j} ${JEDA_FLAG} ${<F}

.jo.so:
	cd ${<D} ; \
        ${COMPILE_LINK.j}  ${JEDA_FLAG} ${<F} ${L0_UTIL_OBJ} ${L1_UTIL_OBJ} \
        ${L2_UTIL_OBJ} ${MODEL_OBJ} ${TESTS_OBJ} ${SUITE_OBJ}

JEDA_FLAG = -g

COMPILE.j = jedax -jc -h -g

COMPILE_LINK.j = jedax -mso -JL_V -g

########################################################################

INC_FILES := $(wildcard ../include/*.jh) 

L0_UTIL_SRC := $(wildcard ../utils/*_0.j)
L0_UTIL_OBJ = ${L0_UTIL_SRC:.j=.jo}
L0_UTIL_HDR = ${L0_UTIL_SRC:.j=.jh}

L1_UTIL_SRC := $(wildcard ../utils/*_1.j)
L1_UTIL_OBJ = ${L1_UTIL_SRC:.j=.jo}
L1_UTIL_HDR = ${L1_UTIL_SRC:.j=.jh}

L2_UTIL_SRC := $(wildcard ../utils/*_2.j)
L2_UTIL_OBJ = ${L2_UTIL_SRC:.j=.jo}
L2_UTIL_HDR = ${L2_UTIL_SRC:.j=.jh}

TEST_SRC := $(wildcard ../tests/*.j)
TEST_OBJ = ${TEST_SRC:.j=.jo}
TEST_HDR = ${TEST_SRC:.j=.jh}
TEST_SO = ${TEST_OBJ:.jo=.so}

SUITE_SRC = ../suites/squat_test_suite.j
SUITE_OBJ = ${SUITE_SRC:.j=.jo}

all: ${TEST_SO}

${TEST_SO}: ${PORT_INC} ${SUITE_OBJ} ${TEST_OBJ} ${MODEL_OBJ} ${L2_UTIL_OBJ} \
            ${L1_UTIL_OBJ} ${L0_UTIL_OBJ}

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


