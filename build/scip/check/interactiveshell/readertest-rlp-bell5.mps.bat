set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/bell5.mps"
write problem temp/bell5.mps.rlp
presolve
write transproblem temp/bell5.mps_trans.rlp
set heur emph def
read temp/bell5.mps_trans.rlp
optimize
validatesolve "8966406.49" "8966406.49"
read temp/bell5.mps.rlp
optimize
validatesolve "8966406.49" "8966406.49"
quit
