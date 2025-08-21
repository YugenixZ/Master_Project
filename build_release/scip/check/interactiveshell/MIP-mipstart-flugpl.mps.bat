read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/flugpl.mps
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/flugpl.mps.mst
presolve
validatesolve 1201500 1201500
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/flugpl.mps
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/flugpl.mps.mst
set heuristics completesol beforepresol FALSE
optimize
validatesolve 1201500 1201500
quit
