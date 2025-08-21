set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/Negated.cip"
write problem temp/Negated.cip.pip
presolve
write transproblem temp/Negated.cip_trans.pip
set heur emph def
read temp/Negated.cip_trans.pip
optimize
validatesolve "1" "1"
read temp/Negated.cip.pip
optimize
validatesolve "1" "1"
quit
