#ifdef VERILOGXL
#undef VERILOGXL
#endif

#include "acc_user.h"
#include "vpi_user.h"
#include "vpi_user_cds.h"

#ifdef JEDA_VPI
#include "jeda_vpi_func_ext.h"
#endif

void (*vlog_startup_routines[VPI_MAXARRAY])() = 
{
    /*** add user entries here ***/

#ifdef JEDA_VPI
#include "jeda_vpi_func_def.h"
#endif
  0 /*** final entry must be 0 ***/

};
