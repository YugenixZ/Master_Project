set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/Negated.cip"
write problem temp/Negated.cip.rlp
presolve
write transproblem temp/Negated.cip_trans.rlp
set heur emph def
read temp/Negated.cip_trans.rlp
optimize
validatesolve "1" "1"
read temp/Negated.cip.rlp
optimize
validatesolve "1" "1"
quit
