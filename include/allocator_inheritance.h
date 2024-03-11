#ifndef _HAVE_ALLOC_INL_H
#define _HAVE_ALLOC_INL_H

#include "./types.h"
#include "./config.h"
#include "./debug.h"

//Cf is cc 's fuzzing
#define MAX_ALLOC           0x40000000
#define ALLOC_OFF_HEAD  8
#define ALLOC_OFF_TOTAL (ALLOC_OFF_HEAD + 1)

/* Magic tokens used to mark used / freed chunks. */

#define ALLOC_MAGIC_C1  0xFF00FF00 /* Used head (dword)  */
#define ALLOC_MAGIC_F   0xFE00FE00 /* Freed head (dword) */
#define ALLOC_MAGIC_C2  0xF0       /* Used tail (byte)   */

/* Positions of guard tokens in relation to the user-visible pointer. */

#define ALLOC_C1(_ptr)  (((u32*)(_ptr))[-2])
#define ALLOC_S(_ptr)   (((u32*)(_ptr))[-1])
#define ALLOC_C2(_ptr)  (((u8*)(_ptr))[ALLOC_S(_ptr)])

//检查请求的大小是否过大，超出了允许的最大值
#define ALLOC_CHECK_SIZE(_s) do { \
    if ((_s) > MAX_ALLOC) \
      ABORT("Bad alloc request: %u bytes", (_s)); \
  } while (0)

//检查 malloc 是否成功
#define ALLOC_CHECK_RESULT(_r, _s) do { \
    if (!(_r)) \
      ABORT("Out of memory: can't allocate %u bytes", (_s)); \
  } while (0)

//申请一块空间 不初始化
static inline void* check_alloc_nozero(u32 size) {

  void* ret;

  if (!size) return NULL;

  ALLOC_CHECK_SIZE(size);
  ret = malloc(size);
  ALLOC_CHECK_RESULT(ret, size);

  return ret;

}


//申请一块空间 初始化
static inline void* check_alloc(u32 size) {

  void* mem;

  if (!size) return NULL;
  mem = check_alloc_nozero(size);

  return memset(mem, 0, size);

}

//申请一段具有特殊结构的空间
static inline void* CF_check_alloc_nozero(u32 size) {
  void* ret;
  ret = check_alloc_nozero(size + ALLOC_OFF_TOTAL);
  ret += ALLOC_OFF_HEAD;
  ALLOC_C1(ret) = ALLOC_MAGIC_C1;
  ALLOC_S(ret)  = size;
  ALLOC_C2(ret) = ALLOC_MAGIC_C2;
  return ret;

}

//这个宏用于分配足够的空间并将格式化的字符串打印到新分配的内存中
#define alloc_printf(_str...) ({ \
    u8* _tmp; \
    s32 _len = snprintf(NULL, 0, _str); \
    if (_len < 0) FATAL("Whoa, snprintf() fails?!"); \
    _tmp = check_alloc(_len + 1); \
    snprintf((char*)_tmp, _len + 1, _str); \
    _tmp; \
  })

//

/* Create a buffer with a copy of a string. Returns NULL for NULL inputs. */
static inline u8* check_strdup(u8* str) {

  void* ret;
  u32   size;

  if (!str) return NULL;

  size = strlen((char*)str) + 1;

  ALLOC_CHECK_SIZE(size);
  ret = malloc(size);
  ALLOC_CHECK_RESULT(ret, size);
  return memcpy(ret, str, size);

}

static inline void check_free(void* mem) {
  if (!mem) return;
  free(mem);
}


#endif /*_HAVE_ALLOC_INL_H*/