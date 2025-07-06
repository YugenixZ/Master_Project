set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/Negated.cip"
write problem temp/Negated.cip.mps
presolve
write transproblem temp/Negated.cip_trans.mps
set heur emph def
read temp/Negated.cip_trans.mps
optimize
validatesolve "1" "1"
read temp/Negated.cip.mps
optimize
validatesolve "1" "1"
quit
