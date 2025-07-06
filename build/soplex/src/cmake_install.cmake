# Install script for directory: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soplex" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/array.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/basevectors.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/changesoplex.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/classarray.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/classset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/clufactor.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/clufactor.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/clufactor_rational.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/clufactor_rational.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/cring.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/dataarray.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/datahashtable.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/datakey.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/dataset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/didxset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/dsvector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/dsvectorbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/dvector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/enter.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/exceptions.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/fmt.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/idlist.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/idxset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/islist.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/leave.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lpcol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lpcolbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lpcolset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lpcolsetbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lprow.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lprowbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lprowset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/lprowsetbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/mpsinput.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/nameset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/notimer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/random.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/rational.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/ratrecon.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/ratrecon.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slinsolver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slinsolver_rational.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slufactor.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slufactor.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slufactor_rational.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/slufactor_rational.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/sol.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/solbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/solverational.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/solvereal.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/sorter.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxalloc.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxautopr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxautopr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxbasis.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxbasis.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxboundflippingrt.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxboundflippingrt.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxbounds.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxchangebasis.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdantzigpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdantzigpr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdefaultrt.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdefaultrt.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdefines.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdefines.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdesc.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdevexpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxdevexpr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxequilisc.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxequilisc.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxfastrt.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxfastrt.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxfileio.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxfileio.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxgeometsc.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxgeometsc.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxgithash.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxharrisrt.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxharrisrt.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxhybridpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxhybridpr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxid.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxleastsqsc.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxleastsqsc.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxlp.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxlpbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxlpbase_rational.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxlpbase_real.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxmainsm.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxmainsm.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxout.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxpapilo.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxparmultpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxparmultpr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxpricer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxquality.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxratiotester.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxscaler.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxscaler.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxshift.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsimplifier.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsolve.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsolver.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsolver.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxstarter.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxstarter.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsteepexpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsteeppr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsteeppr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsumst.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxsumst.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxvecs.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxvectorst.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxvectorst.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxweightpr.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxweightpr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxweightst.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxweightst.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/spxwritestate.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/ssvector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/ssvectorbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/stablesum.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/statistics.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/statistics.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/svector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/svectorbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/svset.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/svsetbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/testsoplex.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/timer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/timerfactory.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/unitvector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/unitvectorbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/updatevector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/updatevector.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/usertimer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/validation.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/validation.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/vector.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/vectorbase.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/wallclocktimer.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex_interface.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex/soplex/config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex_interface.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soplex/external/fmt" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/chrono.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/color.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/compile.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/core.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/format-inl.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/format.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/locale.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/os.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/ostream.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/posix.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/printf.h"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/fmt/ranges.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soplex/external/zstr" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/zstr/zstr.hpp"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/soplex/src/soplex/external/zstr/strict_fstream.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex"
         RPATH "/usr/local/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/bin/soplex")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex"
         OLD_RPATH "::::::::::::::"
         NEW_RPATH "/usr/local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/soplex")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libsoplex.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libsoplex-pic.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so.7.1.4.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so.7.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libsoplexshared.so.7.1.4.0"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libsoplexshared.so.7.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so.7.1.4.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so.7.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/lib/libsoplexshared.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsoplexshared.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex/soplex-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex/soplex-targets.cmake"
         "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex/src/CMakeFiles/Export/7b30a661feffd7bbb1d77d2bef836267/soplex-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex/soplex-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex/soplex-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex" TYPE FILE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex/src/CMakeFiles/Export/7b30a661feffd7bbb1d77d2bef836267/soplex-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex" TYPE FILE FILES "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex/src/CMakeFiles/Export/7b30a661feffd7bbb1d77d2bef836267/soplex-targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/soplex" TYPE FILE FILES
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex/CMakeFiles/soplex-config.cmake"
    "/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/soplex-config-version.cmake"
    )
endif()

