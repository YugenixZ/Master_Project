read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/vpm2.fzn
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/vpm2.fzn.mst
presolve
validatesolve 13.75 13.75
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/vpm2.fzn
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/vpm2.fzn.mst
set heuristics completesol beforepresol FALSE
optimize
validatesolve 13.75 13.75
quit
