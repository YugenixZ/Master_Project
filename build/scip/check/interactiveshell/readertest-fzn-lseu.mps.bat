set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/lseu.mps"
write problem temp/lseu.mps.fzn
presolve
write transproblem temp/lseu.mps_trans.fzn
set heur emph def
read temp/lseu.mps_trans.fzn
optimize
validatesolve "1120" "1120"
read temp/lseu.mps.fzn
optimize
validatesolve "1120" "1120"
quit
