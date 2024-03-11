//标志下使用了自己定义的相关TYPE
#ifndef _HAVE_TYPES_H
#define _HAVE_TYPES_H

#include <stdint.h>
#include <stdlib.h>

//无符号 unsigned integer
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
//u64的处理
#ifdef __x86_64__
typedef unsigned long long u64;
#else
typedef uint64_t u64;
#endif 

//有符号 signed integer
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

//利用下划线来区分宏的参数和其他变量 并利用ifndef来防止多次导入头文件使得重复定义
#ifndef MIN
#  define MIN(_a,_b) ((_a) > (_b) ? (_b) : (_a))
#  define MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#endif /* !MIN */

//按字节调转顺序
#ifndef BYTESWAP16
#   define BYTESWAP16(_x) ({ \
        u16 _ret = (_x); \
        (u16)((_ret << 8) | (_ret >> 8)); \
    })

#   define BYTESWAP32(_x) ({ \
        u32 _ret = (_x); \
        (u32)((_ret << 24) | (_ret >> 24) | \
            ((_ret << 8) & 0x00FF0000) | \
            ((_ret >> 8) & 0x0000FF00)); \
    })
#endif

//常用获取随机数
#ifndef CFL_R
#  define CFL_R(x) (random() % (x))
#endif

//内存屏障 暂时没有很理解这东西是啥
#define MEM_BARRIER() \
  __asm__ volatile("" ::: "memory")
  
//likely和unlikely宏可以用来显式地告诉编译器大多数情况下哪个分支将被执行，
//从而编译器可以做出更好的指令流水线安排，以提高程序的执行速度。
//例如，对于错误处理代码，通常错误发生的频率较低，
//因此可以使用unlikely来注明错误处理分支不太可能执行
#define likely(_x)   __builtin_expect(!!(_x), 1)
#define unlikely(_x)  __builtin_expect(!!(_x), 0)

#endif /* ! _HAVE_TYPES_H */
