# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zzzccc/zcFuzzing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zzzccc/zcFuzzing/build

# Include any dependencies generated for this target.
include CMakeFiles/cfl-llvm-rt-32.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cfl-llvm-rt-32.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cfl-llvm-rt-32.dir/flags.make

CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o: CMakeFiles/cfl-llvm-rt-32.dir/flags.make
CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o: ../cfl-llvm-rt.o.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o   -c /home/zzzccc/zcFuzzing/cfl-llvm-rt.o.c

CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zzzccc/zcFuzzing/cfl-llvm-rt.o.c > CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.i

CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zzzccc/zcFuzzing/cfl-llvm-rt.o.c -o CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.s

cfl-llvm-rt-32: CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o
cfl-llvm-rt-32: CMakeFiles/cfl-llvm-rt-32.dir/build.make

.PHONY : cfl-llvm-rt-32

# Rule to build all files generated by this target.
CMakeFiles/cfl-llvm-rt-32.dir/build: cfl-llvm-rt-32

.PHONY : CMakeFiles/cfl-llvm-rt-32.dir/build

CMakeFiles/cfl-llvm-rt-32.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cfl-llvm-rt-32.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cfl-llvm-rt-32.dir/clean

CMakeFiles/cfl-llvm-rt-32.dir/depend:
	cd /home/zzzccc/zcFuzzing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zzzccc/zcFuzzing /home/zzzccc/zcFuzzing /home/zzzccc/zcFuzzing/build /home/zzzccc/zcFuzzing/build /home/zzzccc/zcFuzzing/build/CMakeFiles/cfl-llvm-rt-32.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cfl-llvm-rt-32.dir/depend

