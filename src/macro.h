/**
 * @file macro.h
 * @brief 常用宏的封装
 * @author obeast-dragon
 */
#ifndef __OBEAST_MACRO_H__
#define __OBEAST_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define OBEAST_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define OBEAST_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define OBEAST_LIKELY(x) (x)
#define OBEAST_UNLIKELY(x) (x)
#endif

/// 断言宏封装
#define OBEAST_ASSERT(x)                                                                \
    if (OBEAST_UNLIKELY(!(x))) {                                                        \
        OBEAST_LOG_ERROR(OBEAST_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\nbacktrace:\n"                          \
                                          << obeast::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

/// 断言宏封装
#define OBEAST_ASSERT2(x, w)                                                            \
    if (OBEAST_UNLIKELY(!(x))) {                                                        \
        OBEAST_LOG_ERROR(OBEAST_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\n"                                      \
                                          << w                                         \
                                          << "\nbacktrace:\n"                          \
                                          << obeast::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

#endif
