#!/bin/bash

# 编译和运行空指针解引用的测试
../../project/cfl-clang -g null_dereference.c -o null_dereference
../../project/cfl-clang -fsanitize=address -g null_dereference.c -o null_dereference_asan

# 编译和运行双重释放的测试
../../project/cfl-clang  -g double_free.c -o double_free
../../project/cfl-clang -fsanitize=address -g double_free.c -o double_free_asan

# 编译和运行除零错误的测试
../../project/cfl-clang  -g divide_by_zero.c -o divide_by_zero
../../project/cfl-clang -fsanitize=undefined -g divide_by_zero.c -o divide_by_zero_asan


# 编译和运行释放后使用的测试
../../project/cfl-clang  -g use_after_free.c -o use_after_free
../../project/cfl-clang -fsanitize=address -g use_after_free.c -o use_after_free_asan


# 编译和运行数组越界访问的测试
../../project/cfl-clang -g array_out_of_bounds.c -o array_out_of_bounds
../../project/cfl-clang -fsanitize=address -g array_out_of_bounds.c -o array_out_of_bounds_asan
