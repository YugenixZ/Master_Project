if(NOT TARGET libgcg)
  include("${CMAKE_CURRENT_LIST_DIR}/gcg-targets.cmake")
endif()

if(0)
   set(SCIP_DIR "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip")
   find_package(SCIP QUIET CONFIG)
endif()

set(GCG_LIBRARIES libgcg)
set(GCG_INCLUDE_DIRS "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/gcg/src")
set(GCG_FOUND TRUE)
