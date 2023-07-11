/**
 * @file test_util.cpp
 * @brief util与macro测试
 * @version 0.1
 * @date 2021-06-12
 */
#include "src/obeast.h"

obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

void test2() {
    std::cout << obeast::BacktraceToString() << std::endl;
}
void test1() {
    test2();
}

void test_backtrace() {
    test1();
}

int main() {
    OBEAST_LOG_INFO(g_logger) << obeast::GetCurrentMS();
    OBEAST_LOG_INFO(g_logger) << obeast::GetCurrentUS();
    OBEAST_LOG_INFO(g_logger) << obeast::ToUpper("hello");
    OBEAST_LOG_INFO(g_logger) << obeast::ToLower("HELLO");
    OBEAST_LOG_INFO(g_logger) << obeast::Time2Str();
    OBEAST_LOG_INFO(g_logger) << obeast::Str2Time("1970-01-01 00:00:00"); // -28800

    std::vector<std::string> files;
    obeast::FSUtil::ListAllFile(files, "./obeast", ".cpp");
    for (auto &i : files) {
        OBEAST_LOG_INFO(g_logger) << i;
    }

    // todo, more...

    test_backtrace();

    OBEAST_ASSERT2(false, "assert");
    return 0;
}
