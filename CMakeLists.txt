cmake_minimum_required(VERSION 3.9)

# version number is taken from SCIP
project(SCIPOptSuite)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
   # require at least gcc 5
   if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
     message(WARNING "GCC version not supported, should be at least 5!")
   endif()
endif()

option(AUTOBUILD "find and use dependencies on availability (ignores PAPILO, IPOPT, ZLIB, GMP, PAPILO, ZIMPL, READLINE, WORHP flags)" OFF)
set(AUTOBUILD_MSG "\nIf you have troubles configuring, you can consider setting AUTOBUILD=ON to try and find optional packages on availability.")
option(NO_EXTERNAL_CODE "should everything be disabled except the bare minimum necessary for SCIP and SoPLEX" OFF)
option(SOPLEX "should SOPLEX be included" ON)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Export all symbols into the DLL" OFF)

if(NOT NO_EXTERNAL_CODE)
   option(AUTOBUILD "find and use dependencies on availability (ignores PAPILO, IPOPT, ZLIB, GMP, PAPILO, ZIMPL, READLINE, WORHP flags)" OFF)
   option(PAPILO "should papilo library be linked" ON)
   option(ZIMPL "should zimpl be linked" ON)
   option(GMP "should GMP be linked" ON)
   option(GCG "should GCG be included" ON)
   option(UG "should ug be included" ON)
   option(BOOST "Use Boost (required to build the binary). Disable if you only want to build libsoplex." ON)
else()
   option(PAPILO "should papilo library be linked" OFF)
   option(ZIMPL "should zimpl be linked" OFF)
   option(GMP "should GMP be linked" OFF)
   option(GCG "should GCG be included" OFF)
   option(UG "should ug be included" OFF)
   option(ZLIB "should zlib be linked" OFF)
   option(READLINE "should readline be linked" OFF)
   option(IPOPT "should ipopt be linked" OFF)
   option(WORHP "should worhp be linked" OFF)
   option(BOOST "Use Boost (required to build the binary). Disable if you only want to build libsoplex." OFF)
   option(QUADMATH "should quadmath library be used" OFF)
   option(MPFR "Use MPFR" OFF)
   set(SYM none CACHE STRING "options for symmetry computation")
endif()

if(AUTOBUILD)
    message(WARNING "You are using the automatic build option to find and use dependencies automatically. The following flags are ignored: IPOPT, ZLIB, GMP, PAPILO, ZIMPL, READLINE, WORHP.")
endif() # AUTOBUILD

include_directories(
   soplex/src
   scip/src
   zimpl/src
   papilo/src
   ug/src
   gcg/src
   )

# make 'Release' the default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# path to e.g. findGMP module
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/papilo/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scip/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/soplex/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/zimpl/cmake/Modules)

if(ZIMPL OR AUTOBUILD)
  find_package(BISON QUIET)
  find_package(FLEX QUIET)
  find_package(GMP QUIET)
  # only add ZIMPL subdirectory if required packages could be found and if the zimpl subdirectory has been checked out
  if(${BISON_FOUND} AND ${FLEX_FOUND} AND ${GMP_FOUND} AND EXISTS ${PROJECT_SOURCE_DIR}/zimpl/CMakeLists.txt)
    add_subdirectory(zimpl)
    set(ZIMPL_DIR ${CMAKE_BINARY_DIR})
  else()
    set(ZIMPL OFF)
  endif()
endif()

if(PAPILO OR AUTOBUILD)
  find_package(Boost ${BOOST_MIN_VERSION} OPTIONAL_COMPONENTS iostreams program_options serialization QUIET)
  find_package(Threads QUIET)
  if(BOOST AND Boost_FOUND AND Threads_FOUND)
    set(PAPILO_NO_BINARIES on)
    add_subdirectory(papilo)
    set(PAPILO_DIR ${CMAKE_BINARY_DIR})
  elseif(NOT AUTOBUILD)
    if(NOT BOOST OR NOT Boost_FOUND)
      message(WARNING "Cannot find papilo dependency Boost. Please set PAPILO to off or specify BOOST_ROOT and set BOOST=on!")
    endif()
    if(NOT Threads_FOUND)
      message(WARNING "Cannot find papilo dependency Threads. Please set PAPILO to off or specify Threads_DIR!")
    endif()
    message(FATAL_ERROR "Missing papilo dependencies. Aborting. ${AUTOBUILD_MSG}")
  else()
    set(PAPILO OFF)
  endif()
endif()

if(SOPLEX)
  add_subdirectory(soplex)
  set(SOPLEX_DIR ${CMAKE_BINARY_DIR})
else()
  set(LPS "none" CACHE STRING "SCIP option for LP solver")
endif()

add_subdirectory(scip)

if(GCG AND EXISTS ${PROJECT_SOURCE_DIR}/gcg/CMakeLists.txt)
  add_subdirectory(gcg)
  set(GCG_DIR ${CMAKE_BINARY_DIR})
endif()

if(UG AND EXISTS ${PROJECT_SOURCE_DIR}/ug/CMakeLists.txt)
  add_subdirectory(ug)
endif()

if(PAPILO_NO_BINARIES)
   # We disabled binaries before and add them now.
   add_subdirectory(papilo/binaries)
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/scip/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${SCIPOptSuite_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${SCIPOptSuite_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${SCIPOptSuite_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Zuse Institute Berlin")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_PACKAGE_EXECUTABLES scip;SCIP soplex;SoPlex)
set(CPACK_PACKAGE_CONTACT "SCIP <scip@zib.de>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Toolbox for generating and solving mixed integer linear (MIP) and nonlinear programs (MINLP) and constraint integer programs (CIP)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://scipopt.org")
# autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
include(CPack)
enable_testing()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif()

