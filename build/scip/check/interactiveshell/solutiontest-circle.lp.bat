read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/circle.lp
optimize
write solution temp/circle.lp.sol
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/circle.lp
read temp/circle.lp.sol
optimize
validatesolve 4.57424778 4.57424778
quit
