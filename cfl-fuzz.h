#ifndef CFL_FUZZ_H
#define CFL_FUZZ_H

#include "./include/types.h"
#include "./include/config.h"
#include "./include/debug.h"
#include "./include/allocator_inheritance.h"
#include "./include/hash.h"

struct queue_entry
{
    
    u8 *fname; /* 对应的testcase的文件名 */

    u32 len;   /* 长度  */

    u8  was_fuzzed,    /*  是否被模糊测试过了  */
        has_new_coverage,  /*  是否有覆盖到了未曾覆盖的道路    */
        timeout_times;/* 超时次数 */
    u32 bitmap_size, /* bitmap 大小    */
        hash;        /* 经过"分桶"后的 hash  */

    u64 exec_us, /* 执行时间 (us)              */
        depth;   /* 通过多少次变异得到的 */

    struct queue_entry *next; /*  队列的下一项    */
    int is_crash;
};

struct Fuzzer_resource
{
    s32 forksrv_pid,    /* PID of the fork server           */
        child_pid; /* PID of the fuzzed program        */

    u8 *trace_bits;

    s32 shm_id;

    u8 virgin_bits[MAP_SIZE],                /* 尚未被 fuzzing 触及的区域 */
        virgin_tmout[MAP_SIZE],                     /* 在超时情况下尚未被观察到的区域   */
        virgin_crash[MAP_SIZE];                     /* 在崩溃情况下尚未被观察到的区域  */
    struct queue_entry *queue, /* Fuzzing queue (linked list)      */
        *queue_top;          /* Top of the list                  */
};

struct Fuzzer_fd
{
    s32 cur_input;               /* 用于传递参数与testcase给子程序 */
    s32 dev_urandom_fd;          /* /dev/urandom 设备文件的持久化文件描述符 */
    s32 dev_null_fd;             /* /dev/null */
    s32 forkserv_control_fd;     /* 存储 fork server 控制管道的写入端 */
    s32 forkserv_state_fd;       /* 用于存储 fork server 状态管道的读取端的文件描述符 */
    s32 out_dir_fd;
    u8 *in_dir;                  /* testcases 输入目录 */
    u8 *out_dir;                 /* 输出目录 */
    u8 *target_path;             /* 目标二进制程序路径 */
};

struct Fuzzer_flag
{
    u8 stop_soon, /* 是否识别到ctrl_c等信号 */
        child_timed_out,          /* 子进程是否超时         */
        bitmap_changed,       /* bitmap 是否改变了        */
        kill_signal;
};

struct Fuzzer_count
{
    u64 total_execs,          /* 程序执行的总次数                  */
        start_time,           /* 主循环开始运行的时刻              */
        queue_cycle;          /* Queue round counter              */
};

struct Fuzzer_current_message
{
    u32 queue_length, /* Total number of queued testcases */
        write_timeout, /* 已经有多少个超时的案例被写入了 */
        total_crashes,
        pending_not_fuzzed,  /* Queued but not done yet          */
        cur_depth,           /* Current path depth               */
        max_depth;          /* Max path depth                   */


    struct queue_entry *queue_cur;                   /* 正在处理的队列项  */

    u32 queued_at_start;
};


#endif