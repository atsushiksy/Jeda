 
#include "veriuser.h"
#include "vxl_veriuser.h"

char *veriuser_version_str = 
  "Standard Verilog with Jeda support\n";
 
#  include "jeda_pli_func_ext.h"

int (*endofcompile_routines[])() = 
{
    0 /*** final entry must be 0 ***/
};

bool err_intercept(level,facility,code)
int level; char *facility; char *code;
{ return(true); }


s_tfcell veriusertfs[] =
{
    /*** Template for an entry:
    { usertask|userfunction, data,
      checktf(), sizetf(), calltf(), misctf(),
      "$tfname", forwref?, Vtool?, ErrMsg? },
    Example:
    { usertask, 0, my_check, 0, my_func, my_misctf, "$my_task" },
    ***/

    /*** add user entries here ***/

#  include "jeda_pli_func_def.h"

    {0} /*** final entry must be 0 ***/
};

