#include "./include/types.h"
#include "./include/config.h"
#include "./include/debug.h"
#include "./include/allocator_inheritance.h"
#include "./include/hash.h"
#include "cfl-fuzz.h"
#include "cfl-utils.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./Mutations.h"

static u8 count_class_lookup8[256] = {

    [0] = 0,
    [1] = 1,
    [2] = 2,
    [3] = 4,
    [4 ... 7] = 8,
    [8 ... 15] = 16,
    [16 ... 31] = 32,
    [32 ... 127] = 64,
    [128 ... 255] = 128

};

enum
{
    /* 00 */ FAULT_NONE,
    /* 01 */ FAULT_TMOUT,
    /* 02 */ FAULT_CRASH,
    /* 03 */ FAULT_NOBITS
};

/* 全局变量申请 */
static struct Fuzzer_resource fuzzer_res;
static struct Fuzzer_fd fuzzer_fd;
static struct Fuzzer_count fuzzer_count;
static struct Fuzzer_flag fuzzer_flag;
static struct Fuzzer_current_message fuzzer_cur_msg;

/* 非结构体 */
static u32 exec_tmout = EXEC_TIMEOUT; /* Configurable exec timeout (ms)   */
static u32 max_cycle  = 8;

static u8 *cmd_options[MAX_ARGS];
static u8 *cmd_values[MAX_ARGS];
static s32 num_options = 0;

static u8 *program_argv[MAX_ARGS];
static u8 target_program_path[MAX_PATH_LENGTH];
static u8 num_program_argv = 0;

// 用于Magic Numbers变异的一组常见边界值
const int MAGIC_NUMBERS[] = {0, 0xFF, 0x7F, 0x80, -1, INT_MAX};
const size_t MAGIC_NUMBERS_COUNT = sizeof(MAGIC_NUMBERS) / sizeof(MAGIC_NUMBERS[0]);
/* 全局变量申请 */

/* 处理 ctrl_c 等停止信号 这里为了简单起见 杀死子进程后便退出 放弃程序中可能存在的有用资源 */
static u16 count_class_lookup16[65536];


static void init_count_class16(void) {

  u32 b1, b2;

  for (b1 = 0; b1 < 256; b1++) 
    for (b2 = 0; b2 < 256; b2++)
      count_class_lookup16[(b1 << 8) + b2] = 
        (count_class_lookup8[b1] << 8) |
        count_class_lookup8[b2];

}
static inline void classify_counts(u64* mem) {

  u32 i = MAP_SIZE >> 3;
  init_count_class16();
  while (i--) {

    /* Optimize for sparse bitmaps. */

    if (unlikely(*mem)) {

      u16* mem16 = (u16*)mem;

      mem16[0] = count_class_lookup16[mem16[0]];
      mem16[1] = count_class_lookup16[mem16[1]];
      mem16[2] = count_class_lookup16[mem16[2]];
      mem16[3] = count_class_lookup16[mem16[3]];

    }

    mem++;

  }

}

void * remove_shm()
{

    shmctl(fuzzer_res.shm_id, IPC_RMID, NULL);
}

/* 初始化共享内存 */
static void setup_shm(struct Fuzzer_resource *fuzzer_res)
{

    u8 *shm_str;
    // 是否加载了已存在的 bitmap 若没加载 则初始化 若已加载 则不初始化
    memset(fuzzer_res->virgin_bits, 0, MAP_SIZE);
    // 记录了超时未访问的
    memset(fuzzer_res->virgin_tmout, 0, MAP_SIZE);
    // 记录了错误未访问的
    memset(fuzzer_res->virgin_crash, 0, MAP_SIZE);

    fuzzer_res->shm_id = shmget(IPC_PRIVATE, MAP_SIZE, IPC_CREAT | IPC_EXCL | 0600);

    if (fuzzer_res->shm_id < 0)
        PFATAL("shmget() failed");
// 没有进行自动关闭shm 后面要手动关闭下
//    atexit(remove_shm());

    shm_str = alloc_printf("%d", fuzzer_res->shm_id);

    setenv(SHM_ENV_VAR, shm_str, 1);

    check_free(shm_str);

    fuzzer_res->trace_bits = shmat(fuzzer_res->shm_id, NULL, 0);

    if (fuzzer_res->trace_bits == (void *)-1)
        PFATAL("shmat() failed");
}

/* 程序结束清楚队列内存 */
static void destroy_queue(struct Fuzzer_resource* fuzzer_res) {

    struct queue_entry *q = fuzzer_res->queue, *n;

    while (q) {
        n = q->next;
        check_free(q->fname);
        check_free(q->trace_mini);
        check_free(q);
        q = n;
    }

}

static void handle_stop_sig(int sig)
{
    // 把 fork_server 进程 和子进程全杀了
    fuzzer_flag.stop_soon = 1;

    if (fuzzer_res.child_pid > 0)
        kill(fuzzer_res.child_pid, SIGKILL);
    if (fuzzer_res.forksrv_pid > 0)
        kill(fuzzer_res.forksrv_pid, SIGKILL);

    exit(0);
}

/* 超时则杀死进程 注意杀的是 forkserver 还是子进程*/
/* 超时便会跳转到这里 */
static void handle_timeout(int sig)
{

    if (fuzzer_res.child_pid > 0)
    {

        fuzzer_flag.child_timed_out = 1;
        kill(fuzzer_res.child_pid, SIGKILL);
    }
    else if (fuzzer_res.child_pid == -1 && fuzzer_res.forksrv_pid > 0)
    {

        fuzzer_flag.child_timed_out = 1;
        kill(fuzzer_res.forksrv_pid, SIGKILL);
    }
}

/* 设置信号处理 */
static void setup_signal_handlers(void)
{

    struct sigaction sa;

    sa.sa_handler = NULL;
    sa.sa_flags = SA_RESTART;
    sa.sa_sigaction = NULL;
    // 在信号处理程序执行期间，不屏蔽任何额外的信号
    sigemptyset(&sa.sa_mask);

    /*设置 "stop" 信号的处理程序 */
    sa.sa_handler = handle_stop_sig;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    /* 设置执行超时通知的信号处理程序 */
    sa.sa_handler = handle_timeout;
    sigaction(SIGALRM, &sa, NULL);

    /* 设置忽略某些信号 */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);
}

// forkserver 初始化
static void init_forkserver()
{

    static struct itimerval timer;
    // state 管道用于从 forkserver 向 fuzzer 发送 control 则相反
    int state_pipe[2], control_pipe[2];
    int status;
    s32 rlen;

    PT("tring to start the fork server...");

    if (pipe(state_pipe) || pipe(control_pipe))
        PFATAL("pipe() failed");

    fuzzer_res.forksrv_pid = fork();

    if (fuzzer_res.forksrv_pid < 0)
        PFATAL("fork() failed");

    // forkserver 进程中
    if (fuzzer_res.forksrv_pid == 0)
    {
        setsid();
        dup2(fuzzer_fd.dev_null_fd, 1);
        dup2(fuzzer_fd.dev_null_fd, 2);

        if (dup2(fuzzer_fd.cur_input, 0) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        close(fuzzer_fd.cur_input);


        // dup2 函数将后一个参数的文件描述符重定向到前一个参数
        if (dup2(control_pipe[0], FORKSRV_FD) < 0)
            PFATAL("dup2() failed");
        if (dup2(state_pipe[1], FORKSRV_FD + 1) < 0)
            PFATAL("dup2() failed");
        // 关闭不需要的文件描述符
        close(control_pipe[0]);
        close(control_pipe[1]);
        close(state_pipe[0]);
        close(state_pipe[1]);

        close(fuzzer_fd.dev_null_fd);
        close(fuzzer_fd.dev_urandom_fd);

        execv(target_program_path, program_argv);

        // EXEC_FAIL_SIG 是一个预定义的常量，表示 execv() 函数执行失败的特殊签名值。
        *(u32 *)(fuzzer_res.trace_bits) = EXEC_FAIL_SIG;
        exit(0);
    }
    // forkserver 进程结束
    // 以下为父进程
    close(control_pipe[0]);
    close(state_pipe[1]);

    fuzzer_fd.forkserv_control_fd = control_pipe[1];
    fuzzer_fd.forkserv_state_fd = state_pipe[0];

    /* Wait for the fork server to come up, but don't wait too long. */

    timer.it_value.tv_sec = ((exec_tmout * FORK_WAIT_MULT) / 1000);
    timer.it_value.tv_usec = ((exec_tmout * FORK_WAIT_MULT) % 1000) * 1000;
    // setitimer 是一个 C 语言的标准库函数，用于设置和控制进程的定时器
    setitimer(ITIMER_REAL, &timer, NULL);
    // 父子进程握手
    rlen = read(fuzzer_fd.forkserv_state_fd, &status, 4);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);
    // 长度为 4 则握手成功 这里是正常的 父进程即 fuzz 进程直接退出函数了继续运行了
    if (rlen == 4)
    {
        OKF("All right - fork server is up.");
        return;
    }

    if (fuzzer_flag.child_timed_out)
        FATAL("Timeout while initializing fork server (adjusting -t may help)");

    if (waitpid(fuzzer_res.forksrv_pid, &status, 0) <= 0)
        PFATAL("waitpid() failed");

    if (WIFSIGNALED(status))
    {
        FATAL("Fork server crashed with signal %d", WTERMSIG(status));
    }
    //标志着forkserver的结束
    if (*(u32 *)fuzzer_res.trace_bits == EXEC_FAIL_SIG)
        FATAL("Unable to execute target application");

    FATAL("Fork server handshake failed");
}



/* 将新的测试用例添加至队列 */
static void add_to_queue(u8 *fname, u32 len)
{

    struct queue_entry *q = check_alloc(sizeof(struct queue_entry));

    q->fname = fname;
    q->len = len;
    q->depth = fuzzer_cur_msg.cur_depth + 1;

    if (q->depth > fuzzer_cur_msg.max_depth)
        fuzzer_cur_msg.max_depth = q->depth;

    if (fuzzer_res.queue_top)
    {

        fuzzer_res.queue_top->next = q;
        fuzzer_res.queue_top = q;
    }
    else
        fuzzer_res.queue = fuzzer_res.queue_top = q;

    fuzzer_cur_msg.queue_length++;
    fuzzer_cur_msg.pending_not_fuzzed++;
}

/* 执行程序 监视是否超时 返回状态信息 并更新 trace_bits[] */
static u8 run_target(u32 timeout)
{

    static struct itimerval it;
    static u32 prev_timed_out = 0;
    static u64 exec_ms = 0;
    int status = 0;
    fuzzer_flag.child_timed_out = 0;

    // shm 写零
    memset(fuzzer_res.trace_bits, 0, MAP_SIZE);
    MEM_BARRIER();

    s32 res;

    // 要求 fork server 启动 child
    if ((res = write(fuzzer_fd.forkserv_control_fd, &prev_timed_out, 4)) != 4)
    {
        RPFATAL(res, "Unable to request new process from fork server (OOM?)");
    }

    // 从 fork server 读 child pid
    if ((res = read(fuzzer_fd.forkserv_state_fd, &fuzzer_res.child_pid, 4)) != 4)
    {
        RPFATAL(res, "Unable to request new process from fork server (OOM?)");
    }

    if (fuzzer_res.child_pid <= 0)
        FATAL("Fork server is fial");

    /* 根据用户的请求配置超时，然后等待子进程终止。*/

    it.it_value.tv_sec = (timeout / 1000);
    it.it_value.tv_usec = (timeout % 1000) * 1000;

    // 设置定时器，若超时，到期时发送 SIGALRM 信号
    setitimer(ITIMER_REAL, &it, NULL);

    /* SIGALRM 处理程序只是杀死 child_pid 并设置 child_timed_out。*/
    // 从 forkserv_state_fd 读取 child 退出原因
    if ((res = read(fuzzer_fd.forkserv_state_fd, &status, 4)) != 4)
    {
        RPFATAL(res, "Unable to communicate with fork server");
    }

    if (!WIFSTOPPED(status))
        fuzzer_res.child_pid = 0;

    // 计算 child 运行耗时
    getitimer(ITIMER_REAL, &it);
    exec_ms = (u64)timeout - (it.it_value.tv_sec * 1000 +
                              it.it_value.tv_usec / 1000);

    // 取消定时器
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &it, NULL);

    fuzzer_count.total_execs++;

    MEM_BARRIER();
    // shm 分桶 分桶这个思想确实 不错 借鉴

#ifdef WORD_SIZE_64
    classify_counts((u64 *)fuzzer_res.trace_bits);
#else
    classify_counts((u32 *)fuzzer_res.trace_bits);
#endif /* ^WORD_SIZE_64 */

    /* 向调用者报告结果。*/

    // 若 child 是非正常结束的
    if (WIFSIGNALED(status))
    {
        fuzzer_flag.kill_signal = WTERMSIG(status);

        // 如果是被 SIGKILL 结束的，说明超时
        if (fuzzer_flag.child_timed_out && fuzzer_flag.kill_signal == SIGKILL){
            return FAULT_TMOUT;
        }
        // 其他原因退出，说明 crash
        printf("crashes happened, signal %d\n", WTERMSIG(status));
        return FAULT_CRASH;
    }
    //否则返回 NONE 状态 即正常运行 并未发生啥
    return FAULT_NONE;
}

/* 返回 1: 表示仅有某个特定元组的命中计数发生了变化。
返回 2: 表示发现了新的元组。
返回 0: 表示没有新的变化。 */
/* 这里自己写了个比较简单的 有空可以再康康 afl 的学习下如何优化 */
int static check_new_bits(u8* virgin_map){
  u8* current = fuzzer_res.trace_bits;
  u8* virgin = virgin_map;
  u8 ret = 0;
    for (u32 i = 0; i < MAP_SIZE; i++) {
        if (current[i]) {
            if (!virgin[i]) {
                ret = 1;
                virgin[i] = current[i];
            }
        }
    }
    return ret;
  }


static int filter(const struct dirent *dir) {
    return strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0;
}

/* 从文件中读取测试用例 初始化时调用 这里暂时是直接用全局变量*/
static void read_testcases( void ) {

  struct dirent **nl;
  s32 nl_cnt;
  u32 i;
  u8* fn;

  /* Auto-detect non-in-place resumption attempts. */

  fn = alloc_printf("%s/queue", fuzzer_fd.in_dir);
  if (!access(fn, F_OK)) fuzzer_fd.in_dir = fn; else check_free(fn);

  ACTF("Scanning '%s'...", fuzzer_fd.in_dir);

  /* 函数使用 scandir 函数扫描输入目录 in_dir 中的文件，并使用 alphasort 函数对文件名进行排序 */

  nl_cnt = scandir(fuzzer_fd.in_dir, &nl, filter, alphasort);

  if (nl_cnt < 0) {
    PFATAL("Unable to open '%s'", fuzzer_fd.in_dir);
  }

  for (i = 0; i < nl_cnt; i++) {
    //stat 用于存储文件信息
    struct stat st;

    u8* fn = alloc_printf("%s/%s", fuzzer_fd.in_dir, nl[i]->d_name);

    if (lstat(fn, &st) || access(fn, R_OK))
      PFATAL("Unable to access '%s'", fn);
    
    //是否是常规文件 文件大小是否为 0
    if (!st.st_size) {
      check_free(fn);
      continue;
    }

    add_to_queue(fn, st.st_size);

  }

  free(nl); /* not tracked */

  // add_too_queue 应该会将现有的队列的数目添加 若还是为 0 说明要么不存在符合条件的文件要么初始化失败
  if (!fuzzer_cur_msg.queue_length) {
    FATAL("Read_testcases Fail in '%s'", fuzzer_fd.in_dir);
  }
  fuzzer_cur_msg.queued_at_start = fuzzer_cur_msg.queue_length;

}

//这里对常规的 也就是尚未添加进入队列的数据进行的测试
static u8 calibrate(void* mem,u32 len){
    u8 *fn;
    u32 fd;
    write_to_testcase(mem, len,&fuzzer_fd);
    u8 fault = run_target(exec_tmout);
    if(fault == FAULT_TMOUT){
        fuzzer_cur_msg.queue_cur->timeout_times += 1;
        write_to_timeout(mem,len,&fuzzer_fd,&fuzzer_cur_msg);
    }else if (fault == FAULT_CRASH) {
        fn = alloc_printf("%s/queue/id_%06u", fuzzer_fd.out_dir, fuzzer_cur_msg.queue_length);
        fd = open(fn, O_RDWR | O_CREAT | O_EXCL, 0600);
        write(fd, mem, len);
        //将文件指针指回文件开头
        lseek(fd,0, SEEK_SET);
        fuzzer_cur_msg.total_crashes += 1;
        add_to_queue(fn,len);
        fuzzer_res.queue_top->is_crash = 1;
        fuzzer_res.queue_top->len = len;
        write_to_crash(mem,len,&fuzzer_fd,&fuzzer_cur_msg);
    }else{
        // 0是没有 1是已知路径数量增加 2是找到新的路径
        int new_bits_level = check_new_bits(fuzzer_res.virgin_bits);
        if(new_bits_level){
            fn = alloc_printf("%s/queue/id_%06u", fuzzer_fd.out_dir, fuzzer_cur_msg.queue_length);
            fd = open(fn, O_RDWR | O_CREAT | O_EXCL, 0600);
            write(fd, mem, len);
            //将文件指针指回文件开头
            lseek(fd,0, SEEK_SET);
            add_to_queue(fn,len);
            fuzzer_res.queue_top->has_new_coverage = 1;
            fuzzer_res.queue_top->len = len;
        }
    }
}
static void crash_mutation(void *mem,u32 len){
    u8 *tmp;
    tmp = check_alloc_nozero(len);
    memcpy(tmp, mem, len);
    for (int j = 0;j<3;j++){
        for (int i =0 ;i<10;i++){
            magic_numbers(tmp,len,MAGIC_NUMBERS,MAGIC_NUMBERS_COUNT);
        }
        calibrate(tmp,len);
    }
    check_free(tmp);
}
static void new_coverage_mutation(void *mem,u32 len){
    u8 *tmp;
    tmp = check_alloc_nozero(len);
    memcpy(tmp, mem, len);
    for (int j = 0;j<20;j++){
        for (int i =0 ;i< 10;i++){
            bit_flipping(tmp,len);
            magic_numbers(tmp,len,MAGIC_NUMBERS,MAGIC_NUMBERS_COUNT);
        }
        calibrate(tmp,len);
    }
    check_free(tmp);
}

static void apply_major_mutation(void *mem,u32 len){
    u8 *tmp;
    tmp = check_alloc_nozero(len);
    u32 len_tmp = len;
    memcpy(tmp, mem, len);
    for (int j = 0;j < 50;j++){
        for (int i =0 ;i< 10;i++){
            //insert_delete_mutation(&tmp,&len_tmp);
            random_mutation(tmp,len);
            magic_numbers(tmp,len,MAGIC_NUMBERS,MAGIC_NUMBERS_COUNT);
        }
        calibrate(tmp,len);
    }
    check_free(tmp);
}
//思路就是参考AFL的选择思想 但是这里直接取队列中的favored进行测试并根据其成绩进行变异 较简单 后面可以再进行改进
//这里时利用重定向输入参数 在forkserver时完成了子进程的重定向
void fuzz_test(){
    s32 len, fd;
    u8  *in_buf, *testcase_buf;

    fd = open(fuzzer_cur_msg.queue_cur->fname, O_RDONLY);
    if (fd < 0) PFATAL("Fail in fuzz_test. Unable to open '%s'", fuzzer_cur_msg.queue_cur->fname);
    len = fuzzer_cur_msg.queue_cur->len;

    //将文件内容映射到内存中
    in_buf = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (in_buf == MAP_FAILED) PFATAL("Fail in fuzz_test Unable to mmap '%s'", fuzzer_cur_msg.queue_cur->fname);
    close(fd);

    if(fuzzer_cur_msg.queue_cur->timeout_times){
        //释放内存并跳过
        int result = munmap(in_buf, len);
        if (result != 0) {
            // munmap函数调用失败，可以检查errno来获取错误信息
            perror("munmap failed");
        }
    }else if (fuzzer_cur_msg.queue_cur->is_crash) {
        crash_mutation(in_buf,len);
        fuzzer_cur_msg.queue_cur->was_fuzzed = 1;
        //释放内存并跳过
        int result = munmap(in_buf, len);
        if (result != 0) {
            // munmap函数调用失败，可以检查errno来获取错误信息
            perror("munmap failed");
        }
    }else if (fuzzer_cur_msg.queue_cur->has_new_coverage){
        new_coverage_mutation(in_buf,len);
        fuzzer_cur_msg.queue_cur->was_fuzzed = 1;
        //释放内存并跳过
        int result = munmap(in_buf, len);
        if (result != 0) {
            // munmap函数调用失败，可以检查errno来获取错误信息
            perror("munmap failed");
        }
    }else{
        apply_major_mutation(in_buf,len);
        int result = munmap(in_buf, len);
        if (result != 0) {
            // munmap函数调用失败，可以检查errno来获取错误信息
            perror("munmap failed");
        }
    }

}
static void prev_run(){
    struct queue_entry *current_queue = fuzzer_res.queue; // 从队列头部开始

    // 遍历队列中的每个元素
    while (current_queue != NULL) {
        // 运行当前的测试用例
        s32 len, fd;
        u8  *in_buf;

        fd = open(current_queue->fname, O_RDONLY);
        if (fd < 0) PFATAL("Fail in fuzz_test. Unable to open '%s'", current_queue->fname);
        len = current_queue->len;

        //将文件内容映射到内存中
        in_buf = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
        if (in_buf == MAP_FAILED) PFATAL("Fail in fuzz_test Unable to mmap '%s'", current_queue->fname);
        close(fd);

        write_to_testcase(in_buf, len, &fuzzer_fd);
        u8 fault = run_target(exec_tmout);

        // 根据运行的结果处理
        if (fault == FAULT_CRASH) {
            current_queue->is_crash = 1;
            write_to_crash(in_buf,len,&fuzzer_fd,&fuzzer_cur_msg);
        } else if (fault == FAULT_TMOUT) {
            current_queue->timeout_times += 1;
        } else {
            // 检查是否有新的覆盖范围
            int new_bits_level = check_new_bits(fuzzer_res.virgin_bits);
            if (new_bits_level) {
                current_queue->has_new_coverage = 1;
            }
            int result = munmap(in_buf, len);
            if (result != 0) {
                // munmap函数调用失败，可以检查errno来获取错误信息
                perror("munmap failed");
            }
        }

        // 移动到下一个队列项
        current_queue = current_queue->next;

    }
}

/* fuzz 主程序 */
int main(int argc, char** argv) {

  struct timeval tv; //存储时间信息

  fuzzer_res.child_pid = -1;

  PT(cCYA "cfl-fuzz " cBRI VERSION cRST " by <1539412714@qq.com>\n");

  gettimeofday(&tv,NULL);
  srandom(tv.tv_sec ^ tv.tv_usec ^ getpid()); // 设置随机数种子
  
  //解析命令行参数 字符串无法使用 switch 故这里使用 if 且假设只会将程序的 path 输入到末尾
  num_options = Parse_command_line(argc,argv,cmd_options,cmd_values,program_argv,&num_program_argv,target_program_path);

  for (int i = 0; i < num_options; i++) {
    if (!strcmp(cmd_options[i], "input")) {
      // 释放旧的内存（如果有的话）
      free(fuzzer_fd.in_dir);
      // 为新的输入路径动态分配和复制内存
      fuzzer_fd.in_dir = strdup(cmd_values[i]);
    } else if (!strcmp(cmd_options[i], "output")) {
      // 释放旧的内存（如果有的话）
      free(fuzzer_fd.out_dir);
      // 为新的输出路径动态分配和复制内存
      fuzzer_fd.out_dir = strdup(cmd_values[i]);
    }
  }
  
  if ((!fuzzer_fd.in_dir) || (!fuzzer_fd.out_dir)) Cmd_usage(argv[0]);

  //设置各种信号的处理程序
  setup_signal_handlers();

  //初始化共享内存 将相应参数存入 fuzzer_fd 中
  setup_shm(&fuzzer_res);
  
  // 设置常用的文件标识符 如 /dev/null 并
  setup_dirs_fds(&fuzzer_fd);
  
  //读取 input 目录中的测试用例
  read_testcases();

  setup_pass_param_file(&fuzzer_fd);

  init_forkserver();

  prev_run();

  fuzzer_count.start_time = get_cur_time();
  //正式开始运行
  int count = 1;
  while (1) {

    u8 skipped_fuzz;

    if (!fuzzer_cur_msg.queue_cur) {

      fuzzer_count.queue_cycle++;
      fuzzer_cur_msg.queue_cur = fuzzer_res.queue;

    }else{
        if(fuzzer_cur_msg.queue_cur->next){
            fuzzer_cur_msg.queue_cur = fuzzer_cur_msg.queue_cur->next;
        }else{
            fuzzer_cur_msg.queue_cur = fuzzer_res.queue;
            printf("all is tested done once");
        }

    }
    printf("zzzccc has try %d times\n",count++);
    fuzz_test();
  }

  /* 程序结束了 等待 forksrv_pid 进程被杀死 */
  if (waitpid(fuzzer_res.forksrv_pid, NULL, 0) <= 0) {
    WARNF("error waitpid\n");
  }

  write_bitmap(&fuzzer_res,&fuzzer_fd,&(fuzzer_flag.bitmap_changed));
  destroy_queue(&fuzzer_res);
  check_free(fuzzer_fd.target_path);
  free(fuzzer_fd.in_dir);
  free(fuzzer_fd.out_dir);
  OKF("We're done here. Have a nice day!\n");

  exit(0);
}