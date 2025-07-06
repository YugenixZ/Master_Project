set heur emph off
read "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip"/check/"instances/PseudoBoolean/factor-mod-size=9-P0=263-P1=409-P2=29-P3=379-B.opb"
write problem temp/factor-mod-size=9-P0=263-P1=409-P2=29-P3=379-B.opb.cip
presolve
write transproblem temp/factor-mod-size=9-P0=263-P1=409-P2=29-P3=379-B.opb_trans.cip
set heur emph def
read temp/factor-mod-size=9-P0=263-P1=409-P2=29-P3=379-B.opb_trans.cip
optimize
validatesolve "3" "3"
read temp/factor-mod-size=9-P0=263-P1=409-P2=29-P3=379-B.opb.cip
optimize
validatesolve "3" "3"
quit
