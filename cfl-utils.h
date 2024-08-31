#ifndef UTILS
#define UTILS

#include "./include/types.h"
#include "./include/config.h"
#include "./include/debug.h"
#include "./include/allocator_inheritance.h"
#include "./include/hash.h"
#include "./cfl-fuzz.h"
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>    
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "./Mutations.h"

/* 设置各个文件标识符 主要是创建输出目录 并设置常用的如/dev/null等目录 */
static void setup_dirs_fds(struct Fuzzer_fd *fuzzer_fd)
{

  u8 *tmp;
  s32 fd;

  ACTF("Setting up output directories...");
  if (mkdir(fuzzer_fd->out_dir, 0700))
  {
    if (errno != EEXIST)
      PFATAL("Unable to create '%s'", fuzzer_fd->out_dir);
  }
  else
  {
    fuzzer_fd->out_dir_fd = open(fuzzer_fd->out_dir, O_RDONLY);
  }
  /* 用于存储初始和发现的测试用例的队列目录。*/
  tmp = alloc_printf("%s/queue", fuzzer_fd->out_dir);
  if (mkdir(tmp, 0700))
      if (errno != EEXIST)
        PT("Unable to create '%s'\n", tmp);
  check_free(tmp);
  /* 所有记录的崩溃。*/
  tmp = alloc_printf("%s/crashes", fuzzer_fd->out_dir);
  if (mkdir(tmp, 0700))
      if (errno != EEXIST)
        PT("Unable to create '%s'\n", tmp);
  check_free(tmp);
 /* 记录所有的超时*/
 tmp = alloc_printf("%s/timed_outs", fuzzer_fd->out_dir);
 if (mkdir(tmp, 0700))
    PT("Already exist . Unable to create '%s'\n", tmp);
 check_free(tmp);

  /* 一些有用的文件描述符。*/
  fuzzer_fd->dev_null_fd = open("/dev/null", O_RDWR);
  if (fuzzer_fd->dev_null_fd < 0)
    PFATAL("Unable to open /dev/null");

  fuzzer_fd->dev_urandom_fd = open("/dev/urandom", O_RDONLY);
  if (fuzzer_fd->dev_urandom_fd < 0)
    PFATAL("Unable to open /dev/urandom");
}



/* 设置用于传参的 ./cur_input 获取相应的文件标识符 */
static void setup_pass_param_file(struct Fuzzer_fd *fuzzer_fd) {

  u8 * fn = alloc_printf("%s/.cur_input", fuzzer_fd->out_dir);
  //如果存在先删除
  unlink(fn); /* Ignore errors */

  fuzzer_fd->cur_input = open(fn, O_RDWR | O_CREAT | O_EXCL, 0600);

  if (fuzzer_fd->cur_input < 0) PFATAL("Unable to create '%s'", fn);

  check_free(fn);

}

/* 将变异后的数据写入到 cur_input 中 即运行子程序时的标准输入流 并且写入之前记录的要传给子程序的参数 */
static void write_to_testcase(void* mem, u32 len,struct Fuzzer_fd *fuzzer_fd) {
    lseek(fuzzer_fd->cur_input,0,SEEK_SET);
    write(fuzzer_fd->cur_input, mem, len);
    if (ftruncate(fuzzer_fd->cur_input, len)) PFATAL("ftruncate() failed");
    lseek(fuzzer_fd->cur_input, 0, SEEK_SET);
}

/* 若 bitmap 发生更改 则将其写入文件 */
static void write_bitmap(struct Fuzzer_resource *fuzzer_res, struct Fuzzer_fd *fuzzer_fd ,int *bitmap_changed) {

  u8* fname;
  s32 fd;
  //更改了才写
  if (!(*bitmap_changed)) return;
  (*bitmap_changed) = 0;

  fname = alloc_printf("%s/fuzz_bitmap", fuzzer_fd->out_dir);
  fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  if (fd < 0) PFATAL("Unable to open '%s'", fname);

  check_write(fd, fuzzer_res->virgin_bits, MAP_SIZE, fname);

  close(fd);
  check_free(fname);

}

static u8  edges_only;                /* Ignore hit counts?                */

void Cmd_usage(u8* argv0) {

  PT("\n%s [ options ] --/path/to/fuzzed_app [ ... ]\n\n"

       "Required parameters:\n\n"

       "  -i dir        - input directory with test cases\n"
       "  -o dir        - output directory for fuzzer findings\n\n",
       argv0);

  exit(1);

}

//解析命令行参数
int Parse_command_line(int argc,char *argv[], char *options[], char *values[],char **program_argv,int *num_program_argv,char *tagget_program_path) {
    int i, num_options = 0;
    int isthere_path = 0;
    for (i = 1; i < argc && num_options < MAX_ARGS; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options[num_options] = &argv[i][1];
                values[num_options++] = argv[i + 1];
                i++;
            }else {
                options[num_options] = &argv[i][1];
                values[num_options++] = NULL;
            }
        } else if (argv[i][0] == '-' && argv[i][1] == '-') //这里假设程序 path 只输入在末尾
        {
            strcpy(tagget_program_path,&argv[i++][2]);
            while(i<argc){
                program_argv[(*num_program_argv)++] = argv[i++];
            }
            break;
        }
        else {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            Cmd_usage(argv[0]);
            exit(1);
        }
    }
    if(isthere_path){
        fprintf(stderr, "Please input your target program path with --");
        Cmd_usage(argv[0]);
    }
    else return num_options;
}



static u64 get_cur_time(void) {

  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);

}

#define FF(_b)  (0xff << ((_b) << 3))

static u32 count_bytes(u8* mem) {

  u32* ptr = (u32*)mem;
  u32  i   = (MAP_SIZE >> 2);
  u32  ret = 0;

  while (i--) {

    u32 v = *(ptr++);

    if (!v) continue;
    if (v & FF(0)) ret++;
    if (v & FF(1)) ret++;
    if (v & FF(2)) ret++;
    if (v & FF(3)) ret++;

  }

  return ret;

}



static void write_to_timeout(void* mem, u32 len,struct Fuzzer_fd *fuzzer_fd,struct Fuzzer_current_message *fuzzer_cur_msg) {
    u8 *tmp;
    u8 *fn;
    s32 fd;
    tmp = alloc_printf("%s/time_outs", fuzzer_fd->out_dir);
    fn = alloc_printf("%s/%d",tmp,fuzzer_cur_msg->write_timeout++);
    fd = open(fn, O_RDWR | O_CREAT | O_EXCL, 0600);
    write(fd, mem, len);
    //将文件指针指回文件开头
    lseek(fd,0, SEEK_SET);
    //释放资源
    check_free(tmp);
    check_free(fn);
    close(fd);
}

static void write_to_crash(void* mem, u32 len,struct Fuzzer_fd *fuzzer_fd,struct Fuzzer_current_message *fuzzer_cur_msg) {
    u8 *tmp;
    u8 *fn;
    s32 fd;
    tmp = alloc_printf("%s/crashes", fuzzer_fd->out_dir);
    fn = alloc_printf("%s/%d",tmp,fuzzer_cur_msg->total_crashes++);
    fd = open(fn, O_RDWR | O_CREAT | O_EXCL, 0600);
    write(fd, mem, len);
    //将文件指针指回文件开头
    lseek(fd,0, SEEK_SET);
    //释放资源
    check_free(tmp);
    check_free(fn);
    close(fd);
}

typedef struct {
    char *str;
    size_t length;
} RandomData;

// 生成随机字符串的函数
RandomData generate_random_data(size_t max_length) {
    // 初始化随机数种子
    srand(time(NULL));

    // 随机确定字符串长度，范围在 1 到 max_length 之间
    size_t length = rand() % max_length + 1;

    // 分配内存用于存储随机字符串
    char *str = (char *)malloc((length + 1) * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // 生成随机字符串
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length; i++) {
        int key = rand() % (sizeof(charset) - 1);
        str[i] = charset[key];
    }
    str[length] = '\0'; // 确保字符串以 null 结尾

    // 创建 RandomString 结构体并返回
    RandomData result;
    result.str = str;
    result.length = length;
    return result;
}


#endif