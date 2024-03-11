#ifndef _HAVE_CONFIG_H
#define _HAVE_CONFIG_H
#include "./types.h"

#ifndef VERSION
#define VERSION "1.0"
#endif
/* In-code signatures for deferred and persistent mode. */

#define PERSIST_SIG         "##SIG_AFL_PERSISTENT##"
#define DEFER_SIG           "##SIG_AFL_DEFER_FORKSRV##"

/*
映射区域大小必须大于2,因为区域至少需要容纳AFL插桩代码的状态信息。

实际上,映射区域的大小通常需要控制在2^18(256KB)以下,以获得较好的性能。如果遇到复杂程序导致映射区域过大,建议调整AFL_INST_RATIO(即编译时插桩代码密度)来解决,而不是继续增加映射区域。

每次修改MAP_SIZE_POW2的值后,都需要重新编译目标二进制程序,否则可能会导致段错误(SEGV)。

合理设置映射区域大小对于AFL的覆盖率追踪效率和性能是至关重要的。过小的区域可能导致映射冲突,过大的区域则会浪费内存并降低性能
*/
#define MAP_SIZE_POW2       16
#define MAP_SIZE            (1 << MAP_SIZE_POW2)

/* Environment variable used to pass SHM ID to the called program. */

#define SHM_ENV_VAR         "__AFL_SHM_ID"

#endif // _HAVE_CONFIG_H