set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/blend2.mps"
write problem temp/blend2.mps.rlp
presolve
write transproblem temp/blend2.mps_trans.rlp
set heur emph def
read temp/blend2.mps_trans.rlp
optimize
validatesolve "7.598985" "7.598985"
read temp/blend2.mps.rlp
optimize
validatesolve "7.598985" "7.598985"
quit
