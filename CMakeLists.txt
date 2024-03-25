# 指定 CMake 的最小版本要求
cmake_minimum_required(VERSION 3.6)

# 设置项目名称和版本
project(MyFuzzer VERSION 1.0)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# 创建一个变量，包含所有的源文件
set(SOURCES
    cfl-fuzz.c
)

# 指定生成目标
add_executable(MyFuzzer ${SOURCES})

# 如果有需要包含的其他头文件目录，使用 include_directories 指定
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    # 如果有其他目录也包含头文件，继续添加路径
)

add_subdirectory(CFL-LLVM)