# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build

# Include any dependencies generated for this target.
include scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/compiler_depend.make

# Include the progress variables for this target.
include scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/progress.make

# Include the compile flags for this target's objects.
include scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/flags.make

scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o: scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/flags.make
scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/cons/nonlinear/nlhdlr_concave.c
scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o: scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o -MF CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o.d -o CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/cons/nonlinear/nlhdlr_concave.c

scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/cons/nonlinear/nlhdlr_concave.c > CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.i

scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/cons/nonlinear/nlhdlr_concave.c -o CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.s

# Object files for target unittest-cons-nonlinear-nlhdlr_concave
unittest__cons__nonlinear__nlhdlr_concave_OBJECTS = \
"CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o"

# External object files for target unittest-cons-nonlinear-nlhdlr_concave
unittest__cons__nonlinear__nlhdlr_concave_EXTERNAL_OBJECTS =

scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave: scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/src/cons/nonlinear/nlhdlr_concave.c.o
scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave: scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/build.make
scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave: /usr/lib/x86_64-linux-gnu/libcriterion.so
scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave: lib/libscip.so.9.2.2.0
scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave: scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/build: scip/tests/cons/nonlinear/unittest-cons-nonlinear-nlhdlr_concave
.PHONY : scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/build

scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/clean:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && $(CMAKE_COMMAND) -P CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/cmake_clean.cmake
.PHONY : scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/clean

scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/depend:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2 /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scip/tests/CMakeFiles/unittest-cons-nonlinear-nlhdlr_concave.dir/depend

