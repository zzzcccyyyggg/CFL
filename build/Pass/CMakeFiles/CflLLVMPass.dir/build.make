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
include Pass/CMakeFiles/CflLLVMPass.dir/depend.make

# Include the progress variables for this target.
include Pass/CMakeFiles/CflLLVMPass.dir/progress.make

# Include the compile flags for this target's objects.
include Pass/CMakeFiles/CflLLVMPass.dir/flags.make

Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o: Pass/CMakeFiles/CflLLVMPass.dir/flags.make
Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o: ../Pass/cfl-llvm-pass1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o -c /home/zzzccc/zcFuzzing/Pass/cfl-llvm-pass1.cpp

Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.i"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zzzccc/zcFuzzing/Pass/cfl-llvm-pass1.cpp > CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.i

Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.s"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zzzccc/zcFuzzing/Pass/cfl-llvm-pass1.cpp -o CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.s

Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.o: Pass/CMakeFiles/CflLLVMPass.dir/flags.make
Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.o: ../Pass/plugin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.o"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CflLLVMPass.dir/plugin.cpp.o -c /home/zzzccc/zcFuzzing/Pass/plugin.cpp

Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CflLLVMPass.dir/plugin.cpp.i"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zzzccc/zcFuzzing/Pass/plugin.cpp > CMakeFiles/CflLLVMPass.dir/plugin.cpp.i

Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CflLLVMPass.dir/plugin.cpp.s"
	cd /home/zzzccc/zcFuzzing/build/Pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zzzccc/zcFuzzing/Pass/plugin.cpp -o CMakeFiles/CflLLVMPass.dir/plugin.cpp.s

# Object files for target CflLLVMPass
CflLLVMPass_OBJECTS = \
"CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o" \
"CMakeFiles/CflLLVMPass.dir/plugin.cpp.o"

# External object files for target CflLLVMPass
CflLLVMPass_EXTERNAL_OBJECTS =

Pass/CflLLVMPass.so: Pass/CMakeFiles/CflLLVMPass.dir/cfl-llvm-pass1.cpp.o
Pass/CflLLVMPass.so: Pass/CMakeFiles/CflLLVMPass.dir/plugin.cpp.o
Pass/CflLLVMPass.so: Pass/CMakeFiles/CflLLVMPass.dir/build.make
Pass/CflLLVMPass.so: /usr/lib/llvm-12/lib/libLLVM-12.so.1
Pass/CflLLVMPass.so: Pass/CMakeFiles/CflLLVMPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zzzccc/zcFuzzing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared module CflLLVMPass.so"
	cd /home/zzzccc/zcFuzzing/build/Pass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CflLLVMPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Pass/CMakeFiles/CflLLVMPass.dir/build: Pass/CflLLVMPass.so

.PHONY : Pass/CMakeFiles/CflLLVMPass.dir/build

Pass/CMakeFiles/CflLLVMPass.dir/clean:
	cd /home/zzzccc/zcFuzzing/build/Pass && $(CMAKE_COMMAND) -P CMakeFiles/CflLLVMPass.dir/cmake_clean.cmake
.PHONY : Pass/CMakeFiles/CflLLVMPass.dir/clean

Pass/CMakeFiles/CflLLVMPass.dir/depend:
	cd /home/zzzccc/zcFuzzing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zzzccc/zcFuzzing /home/zzzccc/zcFuzzing/Pass /home/zzzccc/zcFuzzing/build /home/zzzccc/zcFuzzing/build/Pass /home/zzzccc/zcFuzzing/build/Pass/CMakeFiles/CflLLVMPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Pass/CMakeFiles/CflLLVMPass.dir/depend

