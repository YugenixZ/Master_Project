set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/MANN_a9.clq.lp"
write problem temp/MANN_a9.clq.lp.pip
presolve
write transproblem temp/MANN_a9.clq.lp_trans.pip
set heur emph def
read temp/MANN_a9.clq.lp_trans.pip
optimize
validatesolve "16" "16"
read temp/MANN_a9.clq.lp.pip
optimize
validatesolve "16" "16"
quit
