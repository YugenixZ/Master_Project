set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/MANN_a9.clq.lp"
write problem temp/MANN_a9.clq.lp.mps
presolve
write transproblem temp/MANN_a9.clq.lp_trans.mps
set heur emph def
read temp/MANN_a9.clq.lp_trans.mps
optimize
validatesolve "16" "16"
read temp/MANN_a9.clq.lp.mps
optimize
validatesolve "16" "16"
quit
