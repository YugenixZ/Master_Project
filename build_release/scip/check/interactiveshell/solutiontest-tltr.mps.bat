read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/tltr.mps
optimize
write solution temp/tltr.mps.sol
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/tltr.mps
read temp/tltr.mps.sol
optimize
validatesolve 48.0666666667 48.0666666667
quit
