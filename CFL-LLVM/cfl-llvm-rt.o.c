#include "./include/config.h"
#include "./include/types.h"
#include "./include/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
// __cfl_area_initial 是一块用于代码插桩输出的内存（非共享），直到执行__afl_map_shm 将__afl_area_ptr 指向一块共享内存为止。
u8  __cfl_area_initial[MAP_SIZE];
u8* __cfl_area_ptr = __cfl_area_initial;

__thread u32 __cfl_prev_loc;

/* SHM setup. */
static void __cfl_map_shm(void) {

  u8 *id_str = getenv(SHM_ENV_VAR);

/*
AFL 是一种基于覆盖率的模糊测试工具，它需要在被测试程序中插入一些 instrumentation 代码来收集执行路径覆盖率信息。

这些 instrumentation 代码需要一块内存区域来存储覆盖率信息，通常称为 __afl_area 或 __afl_area_ptr。

在程序初始化阶段，AFL 暂时使用一个叫做 __afl_area_initial 的内存区域，因为此时还无法确定最终需要多大的区域。

一旦程序完成初始化，AFL 就会分配一个合适大小的最终内存区域 __afl_area。

这段代码的作用就是将原有的 __afl_area_initial 区域替换为最终的 __afl_area 区域。

之所以需要这种"hacky"替换方式，是因为在某些项目中 (如 OpenSSL), 很早的初始化代码就需要访问 AFL instrumentation 的内存区域，所以必须先使用一个临时区域 __afl_area_initial。
*/

  if (id_str) {

    u32 shm_id = atoi(id_str);

    __cfl_area_ptr = shmat(shm_id, NULL, 0);

    if (__cfl_area_ptr == (void *)-1) _exit(1);

    /* Write something into the bitmap so that even with low AFL_INST_RATIO,
       our parent doesn't give up on us. */
    __cfl_area_ptr[0] = 1;

  }

}
//实现 forkserver
static void __cfl_start_forkserver(void) {

  static u8 tmp[4];
  s32 child_pid;
  //通过管道通知父进程我们准备好了，如果通知失败即找不到父进程，直接返回当作正常程序执行
  if (write(FORKSRV_FD + 1, tmp, 4) != 4) return;
  // 不断循环 与 fuzz 主程序保持交互
  while (1) {
    int status;

    //如果对端进程 (通常是 fork 服务器) 没有向 FORKSRV_FD 写入数据，那么读取操作将一直阻塞，直到有数据可读或者出现错误
    if (read(FORKSRV_FD, &tmp, 4) != 4) _exit(1);
    child_pid = fork();

    if (child_pid < 0) _exit(1);

    /* 在子进程中 将两个 pipe 都关闭 */

    if (!child_pid) {

      close(FORKSRV_FD);
      close(FORKSRV_FD + 1);
      return;

    }

    /* 在父进程中阻塞等待子进程恢复 */
    if (write(FORKSRV_FD + 1, &child_pid, 4) != 4) _exit(1);
    /* 如果进程收到信号而终止 一般返回 -1 如内存不足等 但若是由自身原因导致终止一般返回相应的原因*/
    if (waitpid(child_pid, &status, 0) < 0)
      _exit(1);
    //将错误发送给父进程
    if (write(FORKSRV_FD + 1, &status, 4) != 4) _exit(1);

  }

}

/* This one can be called from user code when deferred forkserver mode
    is enabled. */
void __cfl_manual_init(void) {
  //静态变量 只会初始化一次
  static u8 init_done;

  if (!init_done) {

    __cfl_map_shm();
    __cfl_start_forkserver();
    init_done = 1;

  }

}

 /* Proper initialization routine. */

 __attribute__((constructor(0))) void __cfl_auto_init(void) {

   __cfl_manual_init();

 }