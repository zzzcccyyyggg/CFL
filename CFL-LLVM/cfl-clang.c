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
  u8 *slash, *tmp,*tmp2;

  if (cfl_path) {
    //使用 cfl_path 拼接出 CflLLVMPass.so 文件的完整路径 利用此路径来检查动态运行库的路径是否正确
    tmp = alloc_printf("%s/CflLLVMPass.so", cfl_path);
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
    tmp2 = alloc_printf("%s/cfl-llvm-rt.o", dir);
    if ((!access(tmp, R_OK)) && (!access(tmp2, R_OK))) {
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

  //使用以下编译选项保证编译的正确进行并加载 pass 进行插桩
  cc_params[cc_par_cnt++] = "-fno-legacy-pass-manager";
  //这里是忽略未定义符号 要是后边有办法把他加进去的话就删了
  cc_params[cc_par_cnt++] = "-Wl,--unresolved-symbols=ignore-all";
  cc_params[cc_par_cnt++] = alloc_printf("-fpass-plugin=%s/CflLLVMPass.so", obj_path);
  //-Qunused-arguments 是一个编译选项，用于告诉编译器忽略未使用的命令行参数，可以防止编译器生成未使用参数的警告
  cc_params[cc_par_cnt++] = "-Qunused-arguments";

  //接着对传入的参数进行解析与复制
  while (--argc) {
    u8* cur = *(++argv);
    //用来确保没有未定义的符号 项目中 部分东西比如__CFL_INIT_是链接.o 文件上的，这里忽略保证编译的正确执行
    if (!strcmp(cur, "-Wl,-z,defs") ||
        !strcmp(cur, "-Wl,--no-undefined")) continue;
    if(!strcmp(cur,"-O0")||strcmp(cur,"-O1")||strcmp(cur,"-O2")||strcmp(cur,"-O3")){
      optimization = 1;
    }
    cc_params[cc_par_cnt++] = cur;
  }
  //若没指定优化等级 则取 O0，这里也保证了 New Pass 能被正确调用
  if(!optimization){
    cc_params[cc_par_cnt++] = "-O0";
  }
  cc_params[cc_par_cnt++] = alloc_printf("%s/cfl-llvm-rt.o", obj_path);
//这里是 AFL deferred instrumentation 机制的实现方式 我这里尚未实现 但非常值得学习
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
    " _B = (char*)\"" PERSIST_SIG "\"; ";

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
