cmake_minimum_required(VERSION 3.9)

project(Rinpacking)

if(TARGET SCIP::SCIP)
  # find package by SCIP PATH
  find_package(SCIP CONFIG PATHS ${SCIP_BINARY_DIR} REQUIRED)
else()
  find_package(SCIP REQUIRED)
endif()

include_directories(BEFORE PUBLIC ${SCIP_INCLUDE_DIRS})

add_executable(ringpacking
   src/cmain.c
   src/reader_rpa.c
   src/probdata_rpa.c
   src/pricer_rpa.c
   src/cons_rpa.c
   src/pattern.c)

# link to math library if it is available
find_library(LIBM m)
if(NOT LIBM)
  set(LIBM "")
endif()

target_link_libraries(ringpacking ${SCIP_LIBRARIES} ${LIBM})
if( TARGET applications )
    add_dependencies( applications ringpacking )
endif()

#
# add check subdirectory for tests
#
add_subdirectory(check)

#
# add tests subdirectory for unit tests
#
add_subdirectory(tests)

enable_testing()
