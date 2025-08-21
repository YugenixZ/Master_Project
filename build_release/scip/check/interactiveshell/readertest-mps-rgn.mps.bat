set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/rgn.mps"
write problem temp/rgn.mps.mps
presolve
write transproblem temp/rgn.mps_trans.mps
set heur emph def
read temp/rgn.mps_trans.mps
optimize
validatesolve "82.19999924" "82.19999924"
read temp/rgn.mps.mps
optimize
validatesolve "82.19999924" "82.19999924"
quit
