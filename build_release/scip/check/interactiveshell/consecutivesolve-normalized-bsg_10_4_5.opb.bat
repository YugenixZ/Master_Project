set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/PseudoBoolean/normalized-bsg_10_4_5.opb"
optimize
write statistics temp/normalized-bsg_10_4_5.opb_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/PseudoBoolean/normalized-bsg_10_4_5.opb"
optimize
write statistics temp/normalized-bsg_10_4_5.opb_r2.stats
quit
