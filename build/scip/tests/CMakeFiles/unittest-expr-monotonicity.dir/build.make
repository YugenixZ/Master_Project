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
include scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/compiler_depend.make

# Include the progress variables for this target.
include scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/progress.make

# Include the compile flags for this target's objects.
include scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/flags.make

scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o: scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/flags.make
scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/expr/monotonicity.c
scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o: scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o -MF CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o.d -o CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/expr/monotonicity.c

scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/expr/monotonicity.c > CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.i

scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests/src/expr/monotonicity.c -o CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.s

# Object files for target unittest-expr-monotonicity
unittest__expr__monotonicity_OBJECTS = \
"CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o"

# External object files for target unittest-expr-monotonicity
unittest__expr__monotonicity_EXTERNAL_OBJECTS =

scip/tests/expr/unittest-expr-monotonicity: scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/src/expr/monotonicity.c.o
scip/tests/expr/unittest-expr-monotonicity: scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/build.make
scip/tests/expr/unittest-expr-monotonicity: /usr/lib/x86_64-linux-gnu/libcriterion.so
scip/tests/expr/unittest-expr-monotonicity: lib/libscip.so.9.2.2.0
scip/tests/expr/unittest-expr-monotonicity: scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable expr/unittest-expr-monotonicity"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unittest-expr-monotonicity.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/build: scip/tests/expr/unittest-expr-monotonicity
.PHONY : scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/build

scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/clean:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests && $(CMAKE_COMMAND) -P CMakeFiles/unittest-expr-monotonicity.dir/cmake_clean.cmake
.PHONY : scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/clean

scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/depend:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2 /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/tests /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scip/tests/CMakeFiles/unittest-expr-monotonicity.dir/depend

