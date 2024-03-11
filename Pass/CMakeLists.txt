cmake_minimum_required(VERSION 3.6)
include_directories(../include)
list(APPEND CMAKE_MODULE_PATH "/usr/lib/llvm-12/lib/cmake/llvm")
include(AddLLVM)
add_llvm_library(CflLLVMPass
  MODULE
  cfl-llvm-pass1.cpp
  plugin.cpp
  )
