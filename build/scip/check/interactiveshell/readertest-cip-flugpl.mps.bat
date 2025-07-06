set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/flugpl.mps"
write problem temp/flugpl.mps.cip
presolve
write transproblem temp/flugpl.mps_trans.cip
set heur emph def
read temp/flugpl.mps_trans.cip
optimize
validatesolve "1201500" "1201500"
read temp/flugpl.mps.cip
optimize
validatesolve "1201500" "1201500"
quit
