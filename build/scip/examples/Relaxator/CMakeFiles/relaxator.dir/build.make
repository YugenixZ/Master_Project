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
include scip/examples/Relaxator/CMakeFiles/relaxator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include scip/examples/Relaxator/CMakeFiles/relaxator.dir/compiler_depend.make

# Include the progress variables for this target.
include scip/examples/Relaxator/CMakeFiles/relaxator.dir/progress.make

# Include the compile flags for this target's objects.
include scip/examples/Relaxator/CMakeFiles/relaxator.dir/flags.make

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/flags.make
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/cmain.c
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o -MF CMakeFiles/relaxator.dir/src/cmain.c.o.d -o CMakeFiles/relaxator.dir/src/cmain.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/cmain.c

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/relaxator.dir/src/cmain.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/cmain.c > CMakeFiles/relaxator.dir/src/cmain.c.i

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/relaxator.dir/src/cmain.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/cmain.c -o CMakeFiles/relaxator.dir/src/cmain.c.s

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/flags.make
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_lp.c
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o -MF CMakeFiles/relaxator.dir/src/relax_lp.c.o.d -o CMakeFiles/relaxator.dir/src/relax_lp.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_lp.c

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/relaxator.dir/src/relax_lp.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_lp.c > CMakeFiles/relaxator.dir/src/relax_lp.c.i

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/relaxator.dir/src/relax_lp.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_lp.c -o CMakeFiles/relaxator.dir/src/relax_lp.c.s

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/flags.make
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_nlp.c
scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o: scip/examples/Relaxator/CMakeFiles/relaxator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o -MF CMakeFiles/relaxator.dir/src/relax_nlp.c.o.d -o CMakeFiles/relaxator.dir/src/relax_nlp.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_nlp.c

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/relaxator.dir/src/relax_nlp.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_nlp.c > CMakeFiles/relaxator.dir/src/relax_nlp.c.i

scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/relaxator.dir/src/relax_nlp.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator/src/relax_nlp.c -o CMakeFiles/relaxator.dir/src/relax_nlp.c.s

# Object files for target relaxator
relaxator_OBJECTS = \
"CMakeFiles/relaxator.dir/src/cmain.c.o" \
"CMakeFiles/relaxator.dir/src/relax_lp.c.o" \
"CMakeFiles/relaxator.dir/src/relax_nlp.c.o"

# External object files for target relaxator
relaxator_EXTERNAL_OBJECTS =

bin/examples/relaxator: scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/cmain.c.o
bin/examples/relaxator: scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_lp.c.o
bin/examples/relaxator: scip/examples/Relaxator/CMakeFiles/relaxator.dir/src/relax_nlp.c.o
bin/examples/relaxator: scip/examples/Relaxator/CMakeFiles/relaxator.dir/build.make
bin/examples/relaxator: lib/libscip.so.9.2.2.0
bin/examples/relaxator: scip/examples/Relaxator/CMakeFiles/relaxator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable ../../../bin/examples/relaxator"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/relaxator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scip/examples/Relaxator/CMakeFiles/relaxator.dir/build: bin/examples/relaxator
.PHONY : scip/examples/Relaxator/CMakeFiles/relaxator.dir/build

scip/examples/Relaxator/CMakeFiles/relaxator.dir/clean:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator && $(CMAKE_COMMAND) -P CMakeFiles/relaxator.dir/cmake_clean.cmake
.PHONY : scip/examples/Relaxator/CMakeFiles/relaxator.dir/clean

scip/examples/Relaxator/CMakeFiles/relaxator.dir/depend:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2 /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/examples/Relaxator /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/examples/Relaxator/CMakeFiles/relaxator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scip/examples/Relaxator/CMakeFiles/relaxator.dir/depend

