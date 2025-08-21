set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/SAT/bart10.shuffled.cnf"
optimize
write statistics temp/bart10.shuffled.cnf_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/SAT/bart10.shuffled.cnf"
optimize
write statistics temp/bart10.shuffled.cnf_r2.stats
quit
