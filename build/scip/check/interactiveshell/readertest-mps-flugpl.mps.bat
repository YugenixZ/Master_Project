set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/flugpl.mps"
write problem temp/flugpl.mps.mps
presolve
write transproblem temp/flugpl.mps_trans.mps
set heur emph def
read temp/flugpl.mps_trans.mps
optimize
validatesolve "1201500" "1201500"
read temp/flugpl.mps.mps
optimize
validatesolve "1201500" "1201500"
quit
