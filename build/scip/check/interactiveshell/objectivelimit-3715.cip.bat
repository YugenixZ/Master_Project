set load "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/coverage/settings/"default".set
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/Issue/3715.cip"
set limits objective "0.01"
optimize
validatesolve "0" "0"
quit
