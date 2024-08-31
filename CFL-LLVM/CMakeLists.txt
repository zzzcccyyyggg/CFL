cmake_minimum_required(VERSION 3.6)
project(CflLLVMPass)

#CflLLVMPass.so 
# LLVM uses C++17.
set(CMAKE_CXX_STANDARD 17)
find_package(LLVM REQUIRED CONFIG)
# Include the part of LLVM's CMake libraries that defines
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} "./include")
link_directories(${LLVM_LIBRARY_DIRS})
# Our pass lives in this subdirectory.
add_subdirectory(Pass)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM library dirs: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -funroll-loops -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign -DAFL_PATH=\\\"${HELPER_PATH}\\\" -DBIN_PATH=\\\"${BIN_PATH}\\\" -DVERSION=\\\"${VERSION}\\\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign -DVERSION=\\\"${VERSION}\\\" -Wno-variadic-macros")


# Executables and objects
add_executable(cfl-clang cfl-clang.c)
set_target_properties(cfl-clang PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS}")

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS ${CMAKE_BINARY_DIR}/lib)

add_library(cfl-llvm-rt OBJECT cfl-llvm-rt.o.c)
set_target_properties(cfl-llvm-rt PROPERTIES OUTPUT_NAME "cfl-llvm-rt")

# 构建 32 位目标文件
add_library(cfl-llvm-rt-32 OBJECT cfl-llvm-rt.o.c)
target_compile_options(cfl-llvm-rt-32 PRIVATE -m32)
set_target_properties(cfl-llvm-rt-32 PROPERTIES
    OUTPUT_NAME "cfl-llvm-rt-32"
)


# 构建 64 位目标文件
add_library(cfl-llvm-rt-64 OBJECT cfl-llvm-rt.o.c)
target_compile_options(cfl-llvm-rt-64 PRIVATE -m64)
set_target_properties(cfl-llvm-rt-64 PROPERTIES OUTPUT_NAME "cfl-llvm-rt-64")

