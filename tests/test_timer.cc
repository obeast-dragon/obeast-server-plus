/**
 * @file test_timer.cc
 * @brief IO协程测试器定时器测试
 * @version 0.1
 * @date 2021-06-19
 */

#include "src/obeast.h"

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

static int timeout = 1000;
static obeast::Timer::ptr s_timer;

void timer_callback() {
    OBEAST_LOG_INFO(g_logger) << "timer callback, timeout = " << timeout;
    timeout += 1000;
    if(timeout < 5000) {
        s_timer->reset(timeout, true);
    } else {
        s_timer->cancel();
    }
}

void test_timer() {
    obeast::IOManager iom;

    // 循环定时器
    s_timer = iom.addTimer(1000, timer_callback, true);
    
    // 单次定时器
    iom.addTimer(500, []{
        OBEAST_LOG_INFO(g_logger) << "500ms timeout";
    });
    iom.addTimer(5000, []{
        OBEAST_LOG_INFO(g_logger) << "5000ms timeout";
    });
}

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    test_timer();

    OBEAST_LOG_INFO(g_logger) << "end";

    return 0;
}
