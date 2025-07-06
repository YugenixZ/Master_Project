set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/flugpl.mps"
write problem temp/flugpl.mps.rlp
presolve
write transproblem temp/flugpl.mps_trans.rlp
set heur emph def
read temp/flugpl.mps_trans.rlp
optimize
validatesolve "1201500" "1201500"
read temp/flugpl.mps.rlp
optimize
validatesolve "1201500" "1201500"
quit
