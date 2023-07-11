/**
 * @file test_daemon.cc
 * @brief 守护进程测试
 * @version 0.1
 * @date 2021-12-09
 */

#include "src/obeast.h"

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

obeast::Timer::ptr timer;
int server_main(int argc, char **argv) {
    OBEAST_LOG_INFO(g_logger) << obeast::ProcessInfoMgr::GetInstance()->toString();
    obeast::IOManager iom(1);
    timer = iom.addTimer(
        1000, []() {
            OBEAST_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if (++count > 10) {
                exit(1);
            }
        },
        true);
    return 0;
}

int main(int argc, char **argv) {
    return obeast::start_daemon(argc, argv, server_main, argc != 1);
}
