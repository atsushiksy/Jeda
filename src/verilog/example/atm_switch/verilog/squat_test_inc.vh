

/*  This block require debussy and signalscan PLIs 
initial begin
  if($test$plusargs("dump_debussy")) begin  
    $fsdbDumpfile("squat.fsdb");
    $fsdbDumpvars(0,squat_test);
  end
  if($test$plusargs("dump_signalscan")) begin  
    $recordfile( "squat", "incfiles=1", "compress");
    $recordvars("depth=0");
  end
end
*/
