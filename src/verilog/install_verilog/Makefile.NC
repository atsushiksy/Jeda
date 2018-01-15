#
# Makefile
#
#  'make' will link libpli.so (for dynamic lib version), ncelab and ncsim for
#    static link.
#
CC=gcc
# For GNU ld user, set LDFLAG=-export-dynamic
#LDFLAG=-export-dynamic
SHARED_CFLAGS=-O -shared

#

#include directories for external tools
#
VERILOG_INC=$(VERILOG_HOME)/include
JEDA_INC=$(JEDA_HOME)/include

#object files for tools
JEDA_LIB_OBJ = $(JEDA_HOME)/lib/jeda_pli.o

#libs for NC
INCA_DIR=$(VERILOG_HOME)/../inca
NCSIM_OBJ=$(INCA_DIR)/lib/ncsim.o
NCELAB_OBJ=$(INCA_DIR)/lib/ncelab.o

#
# The SIMVISION macro should not be changed.  This will include the shared
# libraries needed to link the simulator.
#
SHARED_LIBS=-L$(CDS_INST_DIR)/tools/lib -lcdsCommon_sh -lxcf_sh -lnmp_sh -lcla_sh
SIMVISION= -L$(VERILOG_HOME)/../lib -ludm -lsimcontrol -lmpi \
           $(VERILOG_HOME)/../lib/mpsc.a -lsman -lcdsCommon_sh \
           -ldataReg_sh -lnmp_sh -lcla_sh

LOCAL_INC_FILES= \
  $(JEDA_HOME)/include/jeda_pli_func_ext.h \
  $(JEDA_HOME)/include/jeda_pli_func_def.h 


# PLI objects

PLI_OBJECTS= 
  
VPI_SOURCE=vpi_user.c
VPI_OBJ=vpi_user.o

VERIUSER_SOURCE=veriuser.c
VERIUSER_OBJ=veriuser.o

#shared library 
LIBPLI=libpli.so

# static linked binary
NCELAB=ncelab
NCSIM=ncsim

GCC_DIR = /volume/dist/gnu/lib/gcc-lib/sparc-sun-solaris2.4/2.6.0

TARGETS= $(VERIUSER_OBJ) $(VPI_OBJ) $(LIBPLI) $(NCELAB) $(NCSIM)

all: $(TARGETS) 

dynamic: $(LIBPLI)

static: $(NCELAB) $(NCSIM)

$(VERIUSER_OBJ):  $(VERIUSER_SOURCE) $(LOCAL_INC_FILES)
	$(CC) $(VERIUSER_CFLAGS) -c $(VERIUSER_SOURCE) \
	-I $(VERILOG_INC) \
	-I $(LOCAL_INC_DIR) \
	-I $(DENALI_INC) \
	-I $(JEDA_INC)

$(VPI_OBJ): $(VPI_SOURCE)
	$(CC) -c $(VPI_SOURCE) -I $(VERILOG_INC)


$(LIBPLI): $(VERIUSER_OBJ) $(VPI_OBJ)
	ld -G -o $@ $(VERIUSER_OBJ) $(VPI_OBJ) \
	$(PLI_OBJECTS) \
	$(JEDA_LIB_OBJ) \
	/usr/lib/libpthread.so /usr/lib/libposix4.so  \
	$(GCC_DIR)/libgcc.a -lX11 -ll

$(NCELAB): $(VERIUSER_OBJ) $(VPI_OBJ)
	$(CC) $(LDFLAG) -o $@ $(VERIUSER_OBJ) $(VPI_OBJ) \
	$(PLI_OBJECTS) \
	$(JEDA_LIB_OBJ) \
	$(NCELAB_OBJ) \
	$(SHARED_LIBS) \
	-L$(INCA_DIR)/lib -lvoids \
	-L$(VERILOG_HOME)/../lib -lsman \
	/usr/lib/libpthread.so /usr/lib/libposix4.so  \
	$(GCC_DIR)/libgcc.a \
	-lX11 \
	-L /usr/dt/lib -lXm \
	-L /usr/openwin/lib -lXt -lX11 \
	-L /usr/lib -lsocket -lnsl -lelf -L/lib -lm -lintl -ldl -ll -ly

$(NCSIM): $(VERIUSER_OBJ) $(VPI_OBJ)
	$(CC) $(LDFLAG) -o $@ $(VERIUSER_OBJ) $(VPI_OBJ) \
	$(PLI_OBJECTS) \
	$(JEDA_LIB_OBJ) \
	$(NCSIM_OBJ) \
	-L$(INCA_DIR)/lib -lvoids \
	$(SIMVISION) \
	-L$(VERILOG_HOME)/../lib -lsman \
	/usr/lib/libpthread.so /usr/lib/libposix4.so  \
	$(GCC_DIR)/libgcc.a \
	-lX11 \
	-L /usr/dt/lib -lXm \
	-L /usr/openwin/lib -lXt -lX11 \
	-L /usr/lib -lsocket -lnsl -lelf -ldl -L/lib -lm -lintl -ll -ly

#
# The "clean" target removes the $(TARGETS) objects
#
clean:
	\rm -f $(TARGETS)
