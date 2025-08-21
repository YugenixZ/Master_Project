set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MINLP/circle.lp"
optimize
write statistics temp/circle.lp_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MINLP/circle.lp"
optimize
write statistics temp/circle.lp_r2.stats
quit
