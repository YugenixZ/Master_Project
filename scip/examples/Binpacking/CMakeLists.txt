cmake_minimum_required(VERSION 3.9)

project(Binpacking)

if(TARGET SCIP::SCIP)
  # find package by SCIP PATH
  find_package(SCIP CONFIG PATHS ${SCIP_BINARY_DIR} REQUIRED)
else()
  find_package(SCIP REQUIRED)
endif()

include_directories(BEFORE PUBLIC ${SCIP_INCLUDE_DIRS})

add_executable(binpacking
   src/branch_ryanfoster.c
   src/cmain.c
   src/cons_samediff.c
   src/pricer_binpacking.c
   src/probdata_binpacking.c
   src/reader_bpa.c
   src/vardata_binpacking.c)

# link to math library if it is available
find_library(LIBM m)
if(NOT LIBM)
  set(LIBM "")
endif()

target_link_libraries(binpacking ${SCIP_LIBRARIES} ${LIBM})
if( TARGET examples )
    add_dependencies( examples binpacking )
endif()

#
# add check subdirectory for tests
#
add_subdirectory(check)

enable_testing()
