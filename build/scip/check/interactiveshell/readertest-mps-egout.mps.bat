set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/egout.mps"
write problem temp/egout.mps.mps
presolve
write transproblem temp/egout.mps_trans.mps
set heur emph def
read temp/egout.mps_trans.mps
optimize
validatesolve "568.1007" "568.1007"
read temp/egout.mps.mps
optimize
validatesolve "568.1007" "568.1007"
quit
