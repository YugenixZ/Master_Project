set display verblevel 0
set timing enabled FALSE
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/stein27.fzn"
optimize
write statistics temp/stein27.fzn_r1.stats
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/MIP/stein27.fzn"
optimize
write statistics temp/stein27.fzn_r2.stats
quit
