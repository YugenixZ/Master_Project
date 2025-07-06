read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/stein27_inf.lp
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/stein27_inf.lp.mst
presolve
validatesolve +infinity +infinity
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/stein27_inf.lp
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/stein27_inf.lp.mst
set heuristics completesol beforepresol FALSE
optimize
validatesolve +infinity +infinity
quit
