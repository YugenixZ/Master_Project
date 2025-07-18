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
include scip/applications/MinIISC/CMakeFiles/miniisc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include scip/applications/MinIISC/CMakeFiles/miniisc.dir/compiler_depend.make

# Include the progress variables for this target.
include scip/applications/MinIISC/CMakeFiles/miniisc.dir/progress.make

# Include the compile flags for this target's objects.
include scip/applications/MinIISC/CMakeFiles/miniisc.dir/flags.make

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/flags.make
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/benders.c
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o -MF CMakeFiles/miniisc.dir/src/benders.c.o.d -o CMakeFiles/miniisc.dir/src/benders.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/benders.c

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniisc.dir/src/benders.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/benders.c > CMakeFiles/miniisc.dir/src/benders.c.i

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniisc.dir/src/benders.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/benders.c -o CMakeFiles/miniisc.dir/src/benders.c.s

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/flags.make
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/miniisc.c
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o -MF CMakeFiles/miniisc.dir/src/miniisc.c.o.d -o CMakeFiles/miniisc.dir/src/miniisc.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/miniisc.c

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniisc.dir/src/miniisc.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/miniisc.c > CMakeFiles/miniisc.dir/src/miniisc.c.i

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniisc.dir/src/miniisc.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/miniisc.c -o CMakeFiles/miniisc.dir/src/miniisc.c.s

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/flags.make
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o: /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/readargs.c
scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o: scip/applications/MinIISC/CMakeFiles/miniisc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o -MF CMakeFiles/miniisc.dir/src/readargs.c.o.d -o CMakeFiles/miniisc.dir/src/readargs.c.o -c /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/readargs.c

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniisc.dir/src/readargs.c.i"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/readargs.c > CMakeFiles/miniisc.dir/src/readargs.c.i

scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniisc.dir/src/readargs.c.s"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC/src/readargs.c -o CMakeFiles/miniisc.dir/src/readargs.c.s

# Object files for target miniisc
miniisc_OBJECTS = \
"CMakeFiles/miniisc.dir/src/benders.c.o" \
"CMakeFiles/miniisc.dir/src/miniisc.c.o" \
"CMakeFiles/miniisc.dir/src/readargs.c.o"

# External object files for target miniisc
miniisc_EXTERNAL_OBJECTS =

bin/applications/miniisc: scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/benders.c.o
bin/applications/miniisc: scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/miniisc.c.o
bin/applications/miniisc: scip/applications/MinIISC/CMakeFiles/miniisc.dir/src/readargs.c.o
bin/applications/miniisc: scip/applications/MinIISC/CMakeFiles/miniisc.dir/build.make
bin/applications/miniisc: lib/libscip.so.9.2.2.0
bin/applications/miniisc: /usr/lib/x86_64-linux-gnu/libm.so
bin/applications/miniisc: scip/applications/MinIISC/CMakeFiles/miniisc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable ../../../bin/applications/miniisc"
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/miniisc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scip/applications/MinIISC/CMakeFiles/miniisc.dir/build: bin/applications/miniisc
.PHONY : scip/applications/MinIISC/CMakeFiles/miniisc.dir/build

scip/applications/MinIISC/CMakeFiles/miniisc.dir/clean:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC && $(CMAKE_COMMAND) -P CMakeFiles/miniisc.dir/cmake_clean.cmake
.PHONY : scip/applications/MinIISC/CMakeFiles/miniisc.dir/clean

scip/applications/MinIISC/CMakeFiles/miniisc.dir/depend:
	cd /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2 /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/scip/applications/MinIISC /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC /scratch/htc/yzhou/yzhou_SCIP/scipoptsuite-9.2.2/build/scip/applications/MinIISC/CMakeFiles/miniisc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scip/applications/MinIISC/CMakeFiles/miniisc.dir/depend

