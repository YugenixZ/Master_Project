# CMake generated Testfile for 
# Source directory: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check
# Build directory: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/VRP/check
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples-vrp-build "/usr/bin/cmake" "--build" "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build" "--config" "Debug" "--target" "vrp")
set_tests_properties(examples-vrp-build PROPERTIES  RESOURCE_LOCK "libscip" _BACKTRACE_TRIPLES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;18;add_test;/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;0;")
add_test(examples-vrp-eil13 "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/bin/examples/vrp" "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/../data/eil13.vrp")
set_tests_properties(examples-vrp-eil13 PROPERTIES  DEPENDS "examples-vrp-build" RESOURCE_LOCK "libscip" _BACKTRACE_TRIPLES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;38;add_test;/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;0;")
add_test(examples-vrp-eil7 "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/bin/examples/vrp" "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/../data/eil7.vrp")
set_tests_properties(examples-vrp-eil7 PROPERTIES  DEPENDS "examples-vrp-build" RESOURCE_LOCK "libscip" _BACKTRACE_TRIPLES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;38;add_test;/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/VRP/check/CMakeLists.txt;0;")
