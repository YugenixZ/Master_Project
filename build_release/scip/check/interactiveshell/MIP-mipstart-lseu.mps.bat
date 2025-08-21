read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/lseu.mps
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/lseu.mps.mst
presolve
validatesolve 1120 1120
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/lseu.mps
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/mipstarts/lseu.mps.mst
set heuristics completesol beforepresol FALSE
optimize
validatesolve 1120 1120
quit
