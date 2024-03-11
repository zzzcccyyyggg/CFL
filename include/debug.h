#ifndef _HAVE_DEBUG_H
#define _HAVE_DEBUG_H

#include "./types.h"
#include "./config.h"
#define MESSAGES_TO_STDOUT


/*******************
 * Terminal colors *
 *******************/

//在支持 ANSI 颜色代码的终端上设置文字颜色和背景颜色
//https://blog.csdn.net/u013554213/article/details/102831883
#ifdef USE_COLOR

#  define cBLK "\x1b[0;30m"
#  define cRED "\x1b[0;31m"
#  define cGRN "\x1b[0;32m"
#  define cBRN "\x1b[0;33m"
#  define cBLU "\x1b[0;34m"
#  define cMGN "\x1b[0;35m"
#  define cCYA "\x1b[0;36m"
#  define cLGR "\x1b[0;37m"
#  define cGRA "\x1b[1;90m"
#  define cLRD "\x1b[1;91m"
#  define cLGN "\x1b[1;92m"
#  define cYEL "\x1b[1;93m"
#  define cLBL "\x1b[1;94m"
#  define cPIN "\x1b[1;95m"
#  define cLCY "\x1b[1;96m"
#  define cBRI "\x1b[1;97m"
#  define cRST "\x1b[0m"

#  define bgBLK "\x1b[40m"
#  define bgRED "\x1b[41m"
#  define bgGRN "\x1b[42m"
#  define bgBRN "\x1b[43m"
#  define bgBLU "\x1b[44m"
#  define bgMGN "\x1b[45m"
#  define bgCYA "\x1b[46m"
#  define bgLGR "\x1b[47m"
#  define bgGRA "\x1b[100m"
#  define bgLRD "\x1b[101m"
#  define bgLGN "\x1b[102m"
#  define bgYEL "\x1b[103m"
#  define bgLBL "\x1b[104m"
#  define bgPIN "\x1b[105m"
#  define bgLCY "\x1b[106m"
#  define bgBRI "\x1b[107m"

#else

#  define cBLK ""
#  define cRED ""
#  define cGRN ""
#  define cBRN ""
#  define cBLU ""
#  define cMGN ""
#  define cCYA ""
#  define cLGR ""
#  define cGRA ""
#  define cLRD ""
#  define cLGN ""
#  define cYEL ""
#  define cLBL ""
#  define cPIN ""
#  define cLCY ""
#  define cBRI ""
#  define cRST ""

#  define bgBLK ""
#  define bgRED ""
#  define bgGRN ""
#  define bgBRN ""
#  define bgBLU ""
#  define bgMGN ""
#  define bgCYA ""
#  define bgLGR ""
#  define bgGRA ""
#  define bgLRD ""
#  define bgLGN ""
#  define bgYEL ""
#  define bgLBL ""
#  define bgPIN ""
#  define bgLCY ""
#  define bgBRI ""

#endif /* ^USE_COLOR */

/***********************
 * Misc terminal codes *
 ***********************/

#define TERM_HOME     "\x1b[H"
#define TERM_CLEAR    TERM_HOME "\x1b[2J"
#define cEOL          "\x1b[0K"
#define CURSOR_HIDE   "\x1b[?25l"
#define CURSOR_SHOW   "\x1b[?25h"

/*************************
 * Box drawing sequences *
 *************************/

#ifdef FANCY_BOXES

#  define SET_G1   "\x1b)0"       /* Set G1 for box drawing    */
#  define RESET_G1 "\x1b)B"       /* Reset G1 to ASCII         */
#  define bSTART   "\x0e"         /* Enter G1 drawing mode     */
#  define bSTOP    "\x0f"         /* Leave G1 drawing mode     */
#  define bH       "q"            /* Horizontal line           */
#  define bV       "x"            /* Vertical line             */
#  define bLT      "l"            /* Left top corner           */
#  define bRT      "k"            /* Right top corner          */
#  define bLB      "m"            /* Left bottom corner        */
#  define bRB      "j"            /* Right bottom corner       */
#  define bX       "n"            /* Cross                     */
#  define bVR      "t"            /* Vertical, branch right    */
#  define bVL      "u"            /* Vertical, branch left     */
#  define bHT      "v"            /* Horizontal, branch top    */
#  define bHB      "w"            /* Horizontal, branch bottom */

#else

#  define SET_G1   ""
#  define RESET_G1 ""
#  define bSTART   ""
#  define bSTOP    ""
#  define bH       "-"
#  define bV       "|"
#  define bLT      "+"
#  define bRT      "+"
#  define bLB      "+"
#  define bRB      "+"
#  define bX       "+"
#  define bVR      "+"
#  define bVL      "+"
#  define bHT      "+"
#  define bHB      "+"

#endif /* ^FANCY_BOXES */

//通过定义或取消定义 MESSAGES_TO_STDOUT，开发者可以控制所有 PT 调用的输出位置，这对于调试或日志记录来说非常有用。
#ifdef MESSAGES_TO_STDOUT
//print to out
#  define PT(x...)    printf(x)
#else 
//print to error
#  define PT(x...)    fprintf(stderr, x)
#endif /* ^MESSAGES_TO_STDOUT */

#define WARNF(x...) do { \
    PT(cYEL "[!] " cBRI "WARNING: " cRST x); \
    PT(cRST "\n"); \
  } while (0)

/* Show a prefixed "success" message. */

#define OKF(x...) do { \
    PT(cLGN "[+] " cRST x); \
    PT(cRST "\n"); \
  } while (0)

/*
__FUNCTION__ 或 __func__：这是一个字符串，代表当前函数的名称。在 C99 之后，__func__ 被标准化为一个局部变量，而在 C++ 中通常可以使用 __FUNCTION__ 或者 __func__ 来获取当前函数的名称。

__FILE__：这是一个字符串，包含当前源代码文件的文件名。无论源代码被包含或导入到其他文件中，__FILE__ 总是展示包含这个宏的原始文件的名称。

__LINE__：这是一个整数，表示当前源代码中的行号。它表示包含 __LINE__ 宏的当前行在源文件中的行数。
*/
#define FATAL(x...) do { \
    PT(bSTOP RESET_G1 CURSOR_SHOW cRST cLRD "\n[-] PROGRAM ABORT : " \
         cBRI x); \
    PT(cLRD "\n         Location : " cRST "%s(), %s:%u\n\n", \
         __FUNCTION__, __FILE__, __LINE__); \
    exit(1); \
  } while (0)

/* Die by calling abort() to provide a core dump. */

#define ABORT(x...) do { \
    PT(bSTOP RESET_G1 CURSOR_SHOW cRST cLRD "\n[-] PROGRAM ABORT : " \
         cBRI x); \
    PT(cLRD "\n    Stop location : " cRST "%s(), %s:%u\n\n", \
         __FUNCTION__, __FILE__, __LINE__); \
    abort(); \
  } while (0)

/* Die by calling abort() to provide a core dump. */

#endif/* _HAVE_DEBUG_H */