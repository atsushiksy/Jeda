#include "ivl_vpi_user.h"

#include "jeda_vpi_func_ext.h"

void (*vlog_startup_routines[])() = {

#include "jeda_vpi_func_def.h"

  0
} ;
