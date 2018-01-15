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


JEDA_FLAG = -g

COMPILE.j = jedax -jc -h  

#COMPILE_LINK.j = jedax -dl -JL_V -DDATA_GC_ASSERT
COMPILE_LINK.jmo = jedax -h -jmo -JL_V   
COMPILE_LINK.j = jedax -dl -JL_V   
COMPILE_LINK_EXE.j = jedax -exe -JL_V  

########################################################################

MODULE_SRC = animal.j dogs.j
MODULE_OBJ = ${MODULE_SRC:.j=.jmo}
MODULE_FLAG = $(patsubst %,-m %, ${MODULE_OBJ})

TEST_SRC := $(wildcard *_test.j)
TEST_OBJ = ${TEST_SRC:.j=.jo}
TEST_EXE = $(subst .jo, , ${TEST_OBJ})

all: ${TEST_EXE}

dogs.jmo: animal.jh

clean:
	${RM} ${MODULE_OBJ} ${TEST_OBJ} ${TEST_EXE}


${TEST_EXE}: ${MODULE_OBJ}   
	echo ${MODULES}
	echo ${MODULE_FLAG}
	${COMPILE_LINK_EXE.j} ${JEDA_FLAG} ${MODULE_FLAG} ${@F}.j 
