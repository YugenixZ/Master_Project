if(NOT TARGET libscip)
  include("${CMAKE_CURRENT_LIST_DIR}/scip-targets.cmake")
endif()

if(0)
   set(ZIMPL_DIR "")
   find_package(ZIMPL QUIET CONFIG)
endif()

if(0)
   set(SOPLEX_DIR "")
   find_package(SOPLEX QUIET CONFIG)
endif()

set(SCIP_LIBRARIES libscip)
set(SCIP_INCLUDE_DIRS "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/src;/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip")
set(SCIP_FOUND TRUE)
