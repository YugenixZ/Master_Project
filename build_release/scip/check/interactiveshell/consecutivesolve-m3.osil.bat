set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MINLP/m3.osil"
optimize
write statistics temp/m3.osil_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MINLP/m3.osil"
optimize
write statistics temp/m3.osil_r2.stats
quit
