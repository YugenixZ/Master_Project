set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/Negated.cip"
write problem temp/Negated.cip.fzn
presolve
write transproblem temp/Negated.cip_trans.fzn
set heur emph def
read temp/Negated.cip_trans.fzn
optimize
validatesolve "1" "1"
read temp/Negated.cip.fzn
optimize
validatesolve "1" "1"
quit
