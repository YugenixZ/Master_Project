disp branch
disp conflict
disp cons
disp
disp
disp heur
disp nlpis
disp nodesel
disp param
disp presol
disp pricers
disp problem
disp dualsol
disp prop
disp reader
disp relax
disp sepa
disp sols
disp solution
disp stat
disp trans
transproblem
disp transsolution
disp value
disp varbranchstatistics
disp ..
set emph counter
set emph cpsolver
set emph easycip
set emph feasibility
set emph hardlp
set emph optimality
set heur emph aggr
set heur emph fast
set heur emph off
set sepa emph aggr
set sepa emph fast
set sepa emph off
set presol emph aggr
set presol emph fast
set presol emph off
set misc printreason 0
set misc printreason T
set diffsave temp/tmp.set
set save temp/tmp.set
set default
set load temp/tmp.set
set default
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/bell5.mps
set lim no 10000
count
write allsolutions
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/bell5.mps
set cons countsol collect T
set lim no 10000
count
write allsolutions temp/tmp.txt
disp lpsolqual
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/bell5.mps
opt
disp solution
disp sols 1
disp sols -1
disp val c1
help
free
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/bell5.mps
set lim no 1
set lim objective 1e+07
o
set lim objective 1e+08
write lp temp/tmp.lp
write mip temp/tmp.lp
write nlp temp/tmp.lp
write problem temp/tmp.lp
write genproblem temp/tmp.lp
write transproblem temp/tmp.lp
write gentransproblem temp/tmp.lp
newstart
set lim objective 9e+06
set lp pricing d
set lp pricing
d
set lp pricing s
fix lp pricing F
set branch prio
c1
1
set branch direction
c1
1
set branch direction c1 -1
o
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/bell5.mps
change bounds c1
1
1
change add [linear]\ <new>:\ <c1>\ >=\ 1;
change objsense
max
change objsense
min
presolve
write cliquegraph temp/tmp.gml
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/vpm2.fzn
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/vpm2.fix
read /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/check/instances/MIP/vpm2.sol
optimize
checksol
disp mem
disp problem
disp stat
change freetrans
change minuc
quit
