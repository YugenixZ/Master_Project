set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/bell5.mps"
optimize
write statistics temp/bell5.mps_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/bell5.mps"
optimize
write statistics temp/bell5.mps_r2.stats
quit
