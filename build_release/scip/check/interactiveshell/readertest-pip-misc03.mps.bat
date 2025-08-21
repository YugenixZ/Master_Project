set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/misc03.mps"
write problem temp/misc03.mps.pip
presolve
write transproblem temp/misc03.mps_trans.pip
set heur emph def
read temp/misc03.mps_trans.pip
optimize
validatesolve "3360" "3360"
read temp/misc03.mps.pip
optimize
validatesolve "3360" "3360"
quit
