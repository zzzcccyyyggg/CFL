#define CFL_MAIN

#include "./include/config.h"
#include "./include/types.h"
#include "./include/debug.h"
#include "./include/allocator_inheritance.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//存储运行时的库的路径
static u8*  obj_path;               
//afl-clang 是对 clang 的 wrapper 这里是存储传递给 clang 的参数
static u8** cc_params;              
//cc_params 的参数的数量 初始包含 cc 的路径
static u32  cc_par_cnt = 1;         
static int optimization = 0;
/* 用于查找运行时库。如果找不到，则中止程序。*/
static void find_obj(u8* argv0) {
  //尝试获取环境变量 CFL_PATH
  //cfl is cc's Fuzzing llvm
  //slash 斜线
  u8 *cfl_path = getenv("CFL_PATH");
  u8 *slash, *tmp;

  if (cfl_path) {
    //使用 cfl_path 拼接出 CflLLVMPass.so 文件的完整路径 利用此路径来检查动态运行库的路径是否正确
    tmp = alloc_printf("%s/CflLLVMPass.so", cfl_path);
    /*
    函数原型 int access(const char *pathname, int mode);、
    const char *pathname: 这是一个指向以 null 结尾的字符串的指针，它指定了要检查的文件的路径名。
    int mode: 这是一个整数，指定了要检查的操作。
      F_OK: 检查文件是否存在。
      R_OK: 检查文件是否对调用进程有可读权限。
      W_OK: 检查文件是否对调用进程有可写权限。
      X_OK: 检查文件是否对调用进程有可执行权限。
    */
    if (!access(tmp, R_OK)) {
      //如果文件可读，则设置 obj_path 为 cfl_path，并释放临时变量，然后返回
      obj_path = cfl_path;
      PT("obj_path is %s",obj_path);
      check_free(tmp);
      return;
    }

    check_free(tmp);
  }
  //如果 CFL_PATH 不可用，尝试从 argv0（程序路径）中获取目录
  //strrchr 从后往前搜索
  //判断是否是个路径 并且这里利用 slash 来改变 dir 的长度 只能说很妙
  slash = strrchr(argv0, '/');
  if (slash) {
    u8 *dir;
    *slash = 0;
    dir = check_strdup(argv0);
    *slash = '/';

    tmp = alloc_printf("%s/CflLLVMPass.so", dir);

    if (!access(tmp, R_OK)) {
      obj_path = dir;
      check_free(tmp);
      return;
    }

    check_free(tmp);
    check_free(dir);
  }
  FATAL("Unable to find 'cfl-llvm-rt.o' or 'CflLLVMPass.so'. Please set CFL_PATH");
}


/* 对传给 wrapper 的参数初始化、解析并进行部分处理 */

static void edit_params(u32 argc, char** argv) {

  u8 fortify_set = 0, asan_set = 0, x_set = 0, bit_mode = 0;
  u8 *name;

  cc_params = check_alloc((argc + 128) * sizeof(u8*));
  //同一段代码编译为了 Clang 与 Clang++ 故这里做一个判断
  name = strrchr(argv[0], '/');
  if (!name) name = argv[0]; else name++;
  //判断是 clang 还是 clang++
  if (!strcmp(name, "cfl-clang++")) {
    u8* alt_cxx = getenv("CFL_CXX");
    cc_params[0] = alt_cxx ? alt_cxx : (u8*)"clang++";
  } else {
    u8* alt_cc = getenv("CFL_CC");
    cc_params[0] = alt_cc ? alt_cc : (u8*)"clang";
  }

//加载 pass 进行插桩
  cc_params[cc_par_cnt++] = "-fno-legacy-pass-manager";
  //这里是忽略未定义符号 要是后边有办法把他加进去的话就删了
  cc_params[cc_par_cnt++] = "-Wl,--unresolved-symbols=ignore-all";
  cc_params[cc_par_cnt++] = alloc_printf("-fpass-plugin=%s/CflLLVMPass.so", obj_path);

//-Qunused-arguments 是一个编译选项，用于告诉编译器忽略未使用的命令行参数，可以防止编译器生成未使用参数的警告
  cc_params[cc_par_cnt++] = "-Qunused-arguments";

//接着对传入的参数进行解析与复制
  while (--argc) {
    u8* cur = *(++argv);
    if (!strcmp(cur, "-m32")) bit_mode = 32;
    if (!strcmp(cur, "-m64")) bit_mode = 64;
    //-x 选项用于指定后续的源代码文件的语言
    if (!strcmp(cur, "-x")) x_set = 1;
    //AddressSanitizer 是一个快速的内存错误检测器  MemorySanitizer 是一个用于检测未初始化内存读取错误的工具
    if (!strcmp(cur, "-fsanitize=address") ||
        !strcmp(cur, "-fsanitize=memory")) asan_set = 1;
    //增强标准库的安全性
    if (strstr(cur, "FORTIFY_SOURCE")) fortify_set = 1;
    //用来确保没有未定义的符号 项目中 部分东西比如__CFL_INIT_是动态传入的
    if (!strcmp(cur, "-Wl,-z,defs") ||
        !strcmp(cur, "-Wl,--no-undefined")) continue;
    if(!strcmp(cur,"-O0")||strcmp(cur,"-O1")||strcmp(cur,"-O2")||strcmp(cur,"-O3")){
      optimization = 1;
    }
    cc_params[cc_par_cnt++] = cur;

  }
  //若没指定优化等级 则取 O0
  if(!optimization){
    cc_params[cc_par_cnt++] = "-O0";
  }

//加强安全性
  if (getenv("CFL_HARDEN")) {
    cc_params[cc_par_cnt++] = "-fstack-protector-all";
  }

//判断是否生成调试信息
  if (getenv("DEBUG")) {
    cc_params[cc_par_cnt++] = "-g";
  }

  if (getenv("CFL_NO_BUILTIN")) {
    //禁用内置的这些函数的优化实现 而是用过标准库
    cc_params[cc_par_cnt++] = "-fno-builtin-strcmp";
    cc_params[cc_par_cnt++] = "-fno-builtin-strncmp";
    cc_params[cc_par_cnt++] = "-fno-builtin-strcasecmp";
    cc_params[cc_par_cnt++] = "-fno-builtin-strncasecmp";
    cc_params[cc_par_cnt++] = "-fno-builtin-memcmp";
  }
/*
避免编译器优化掉签名：
使用 __attribute__((used)) 是为了告诉编译器，即使这个签名在代码中没有被显式使用，也不要将其优化掉。这个属性确保了签名变量会被保留在最终的二进制文件中。

避免链接器丢弃这部分代码：
当链接器被 -Wl,--gc-sections 这样的参数调用时，它会尝试丢弃那些看起来“无用”的代码段。为了防止链接器丢弃签名，代码通过将签名赋值给一个 volatile 指针来强制保留这部分代码。volatile 关键字告诉编译器，这部分内存的内容可能会意外改变，因此编译器和链接器都不应该优化掉对应的代码。

在全局命名空间中声明 __afl_persistent_loop()：
函数需要在全局命名空间中被声明，以便可以从任何地方调用它。但是，在类的方法内部这样做很困难，因为 C++ 禁止在这种上下文中使用 :: 和 extern "C"。同时，__attribute__((alias(...))) 也不起作用。因此，代码使用了 __asm__ 汇编别名技巧来完成这一点
*/
//通过-D 可以传递宏定义

//__AFL_INIT()
  cc_params[cc_par_cnt++] = "-D__CFL_INIT()="
    "do { static volatile char *_A __attribute__((used)); "
    " _A = (char*)\"" DEFER_SIG "\"; "
    "__attribute__((visibility(\"default\"))) "
    "void _I(void) __asm__(\"__cfl_manual_init\"); "
    "_I(); } while (0)";

//__AFL_LOOP()
  cc_params[cc_par_cnt++] = "-D__CFL_LOOP(_A)="
    "({ static volatile char *_B __attribute__((used)); "
    " _B = (char*)\"" PERSIST_SIG "\"; "
#ifdef __APPLE__
    "__attribute__((visibility(\"default\"))) "
    "int _L(unsigned int) __asm__(\"___cfl_persistent_loop\"); "
#else
    "__attribute__((visibility(\"default\"))) "
    "int _L(unsigned int) __asm__(\"__cfl_persistent_loop\"); "
#endif /* ^__APPLE__ */
    "_L(_A); })";

  // if (x_set) {
  //   cc_params[cc_par_cnt++] = "-x";
  //   cc_params[cc_par_cnt++] = "none";
  // }

#ifndef __ANDROID__
  switch (bit_mode) {

    case 0:
      cc_params[cc_par_cnt++] = alloc_printf("%s/cfl-llvm-rt.o", obj_path);
      break;

    case 32:
      cc_params[cc_par_cnt++] = alloc_printf("%s/cfl-llvm-rt-32.o", obj_path);

      if (access(cc_params[cc_par_cnt - 1], R_OK))
        FATAL("-m32 is not supported by your compiler");

      break;

    case 64:
      cc_params[cc_par_cnt++] = alloc_printf("%s/cfl-llvm-rt-64.o", obj_path);

      if (access(cc_params[cc_par_cnt - 1], R_OK))
        FATAL("-m64 is not supported by your compiler");

      break;

  }
#endif

  cc_params[cc_par_cnt] = NULL;
}


int main(int argc, char** argv) {

/* isatty(int fd) 用于检测一个文件描述符是否是一个终端设备 */
#ifdef MESSAGES_TO_STDOUT
  if (isatty(1) && !getenv("CFL_QUIET") ) {
    PT(cCYA "cfl-clang " cBRI VERSION  cRST " by <1539412714@qq.com>\n");
  }
#else
  if (isatty(2) && !getenv("CFL_QUIET") ) {
    PT(cCYA "cfl-clang " cBRI VERSION  cRST " by <1539412714@qq.com>\n");
  }
#endif

  if (argc < 2) {
    PT("\nerror:please use me just like the clang or clang++\n\n");
    exit(1);
  }
  find_obj(argv[0]);
  edit_params(argc, argv);
  /*
  int execvp(const char *file, char *const argv[]);
  file 是要执行的程序的名称。如果这个程序的名字包含了斜杠（/），execvp 会将它视为路径名。否则，会在环境变量 PATH 指定的目录中搜索这个程序。
  argv 是传递给要执行程序的参数的数组，这个数组必须以 NULL 指针结束。
  */
  execvp(cc_params[0], (char**)cc_params);

  FATAL("Oops, failed to execute '%s' - check your PATH", cc_params[0]);

  return 0;
}
