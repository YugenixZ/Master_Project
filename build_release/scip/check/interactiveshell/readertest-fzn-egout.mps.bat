set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/egout.mps"
write problem temp/egout.mps.fzn
presolve
write transproblem temp/egout.mps_trans.fzn
set heur emph def
read temp/egout.mps_trans.fzn
optimize
validatesolve "568.1007" "568.1007"
read temp/egout.mps.fzn
optimize
validatesolve "568.1007" "568.1007"
quit
