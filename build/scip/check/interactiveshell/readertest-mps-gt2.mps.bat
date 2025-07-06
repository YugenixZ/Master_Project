set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/gt2.mps"
write problem temp/gt2.mps.mps
presolve
write transproblem temp/gt2.mps_trans.mps
set heur emph def
read temp/gt2.mps_trans.mps
optimize
validatesolve "21166" "21166"
read temp/gt2.mps.mps
optimize
validatesolve "21166" "21166"
quit
