read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/stein27.fzn
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/stein27.fzn.mst
presolve
validatesolve 18 18
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/stein27.fzn
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/stein27.fzn.mst
set heuristics completesol beforepresol FALSE
optimize
validatesolve 18 18
quit
