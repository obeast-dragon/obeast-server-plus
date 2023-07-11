/**
 * @file test_fiber2.cc
 * @brief 协程测试，用于演示非对称协程
 * @version 0.1
 * @date 2021-06-15
 */
#include "src/obeast.h"
#include "src/obeast.h"
#include <string>
#include <vector>

obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

void run_in_fiber2() {
    OBEAST_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    OBEAST_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    OBEAST_LOG_INFO(g_logger) << "run_in_fiber begin";

    /**
     * 非对称协程，子协程不能创建并运行新的子协程，下面的操作是有问题的，
     * 子协程再创建子协程，原来的主协程就跑飞了
     */
    obeast::Fiber::ptr fiber(new obeast::Fiber(run_in_fiber2, 0, false));
    fiber->resume();

    OBEAST_LOG_INFO(g_logger) << "run_in_fiber end";
}

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    OBEAST_LOG_INFO(g_logger) << "main begin";

    obeast::Fiber::GetThis();

    obeast::Fiber::ptr fiber(new obeast::Fiber(run_in_fiber, 0, false));
    fiber->resume();

    OBEAST_LOG_INFO(g_logger) << "main end";
    return 0;
}
