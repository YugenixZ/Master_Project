set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/stein27_inf.lp"
write problem temp/stein27_inf.lp.opb
presolve
write transproblem temp/stein27_inf.lp_trans.opb
set heur emph def
read temp/stein27_inf.lp_trans.opb
optimize
validatesolve "+infinity" "+infinity"
read temp/stein27_inf.lp.opb
optimize
validatesolve "+infinity" "+infinity"
quit
