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
include CFL-LLVM/CMakeFiles/cfl-clang.dir/depend.make

# Include the progress variables for this target.
include CFL-LLVM/CMakeFiles/cfl-clang.dir/progress.make

# Include the compile flags for this target's objects.
include CFL-LLVM/CMakeFiles/cfl-clang.dir/flags.make

CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.o: CFL-LLVM/CMakeFiles/cfl-clang.dir/flags.make
CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.o: ../CFL-LLVM/cfl-clang.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.o"
	cd /home/zzzccc/zcFuzzing/build/CFL-LLVM && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cfl-clang.dir/cfl-clang.c.o   -c /home/zzzccc/zcFuzzing/CFL-LLVM/cfl-clang.c

CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cfl-clang.dir/cfl-clang.c.i"
	cd /home/zzzccc/zcFuzzing/build/CFL-LLVM && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zzzccc/zcFuzzing/CFL-LLVM/cfl-clang.c > CMakeFiles/cfl-clang.dir/cfl-clang.c.i

CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cfl-clang.dir/cfl-clang.c.s"
	cd /home/zzzccc/zcFuzzing/build/CFL-LLVM && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zzzccc/zcFuzzing/CFL-LLVM/cfl-clang.c -o CMakeFiles/cfl-clang.dir/cfl-clang.c.s

# Object files for target cfl-clang
cfl__clang_OBJECTS = \
"CMakeFiles/cfl-clang.dir/cfl-clang.c.o"

# External object files for target cfl-clang
cfl__clang_EXTERNAL_OBJECTS =

CFL-LLVM/cfl-clang: CFL-LLVM/CMakeFiles/cfl-clang.dir/cfl-clang.c.o
CFL-LLVM/cfl-clang: CFL-LLVM/CMakeFiles/cfl-clang.dir/build.make
CFL-LLVM/cfl-clang: CFL-LLVM/CMakeFiles/cfl-clang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cfl-clang"
	cd /home/zzzccc/zcFuzzing/build/CFL-LLVM && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cfl-clang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CFL-LLVM/CMakeFiles/cfl-clang.dir/build: CFL-LLVM/cfl-clang

.PHONY : CFL-LLVM/CMakeFiles/cfl-clang.dir/build

CFL-LLVM/CMakeFiles/cfl-clang.dir/clean:
	cd /home/zzzccc/zcFuzzing/build/CFL-LLVM && $(CMAKE_COMMAND) -P CMakeFiles/cfl-clang.dir/cmake_clean.cmake
.PHONY : CFL-LLVM/CMakeFiles/cfl-clang.dir/clean

CFL-LLVM/CMakeFiles/cfl-clang.dir/depend:
	cd /home/zzzccc/zcFuzzing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zzzccc/zcFuzzing /home/zzzccc/zcFuzzing/CFL-LLVM /home/zzzccc/zcFuzzing/build /home/zzzccc/zcFuzzing/build/CFL-LLVM /home/zzzccc/zcFuzzing/build/CFL-LLVM/CMakeFiles/cfl-clang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CFL-LLVM/CMakeFiles/cfl-clang.dir/depend

