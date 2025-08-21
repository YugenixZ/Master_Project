set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/p0548.mps"
write problem temp/p0548.mps.lp
presolve
write transproblem temp/p0548.mps_trans.lp
set heur emph def
read temp/p0548.mps_trans.lp
optimize
validatesolve "8691" "8691"
read temp/p0548.mps.lp
optimize
validatesolve "8691" "8691"
quit
