set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/PseudoBoolean/normalized-bsg_10_4_5.opb"
write problem temp/normalized-bsg_10_4_5.opb.cip
presolve
write transproblem temp/normalized-bsg_10_4_5.opb_trans.cip
set heur emph def
read temp/normalized-bsg_10_4_5.opb_trans.cip
optimize
validatesolve "-4" "-4"
read temp/normalized-bsg_10_4_5.opb.cip
optimize
validatesolve "-4" "-4"
quit
