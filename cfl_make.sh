#!/bin/bash

# 定义源目录和目标目录变量
BUILD_DIR="build"
PROJECT_DIR="../project"
CFL_LLVM_DIR="./CFL-LLVM"

# 确保构建目录存在
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# 运行 CMake 和 Make
cmake .
make

# 复制文件到项目目录
cp ./MyFuzzer $PROJECT_DIR/MyFuzzer
cp $CFL_LLVM_DIR/cfl-clang $PROJECT_DIR/cfl-clang
cp $CFL_LLVM_DIR/Pass/CflLLVMPass.so $PROJECT_DIR/CflLLVMPass.so
# 复制不同架构的对象文件
cp $CFL_LLVM_DIR/CMakeFiles/cfl-llvm-rt.dir/cfl-llvm-rt.o.c.o $PROJECT_DIR/cfl-llvm-rt.o
cp $CFL_LLVM_DIR/CMakeFiles/cfl-llvm-rt-64.dir/cfl-llvm-rt.o.c.o $PROJECT_DIR/cfl-llvm-rt-64.o
cp $CFL_LLVM_DIR/CMakeFiles/cfl-llvm-rt-32.dir/cfl-llvm-rt.o.c.o $PROJECT_DIR/cfl-llvm-rt-32.o

# 返回到原始目录
cd -

echo "所有文件已成功复制。"