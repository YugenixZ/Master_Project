read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/m3.osil
optimize
write solution temp/m3.osil.sol
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MINLP/m3.osil
read temp/m3.osil.sol
optimize
validatesolve 37.8 37.8
quit
