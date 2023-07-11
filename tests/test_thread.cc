/**
 * @file test_thread.cc
 * @brief 线程模块测试
 * @version 0.1
 * @date 2021-06-15
 */
#include "src/obeast.h"

obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

int count = 0;
obeast::Mutex s_mutex;

void func1(void *arg) {
    OBEAST_LOG_INFO(g_logger) << "name:" << obeast::Thread::GetName()
        << " this.name:" << obeast::Thread::GetThis()->getName()
        << " thread name:" << obeast::GetThreadName()
        << " id:" << obeast::GetThreadId()
        << " this.id:" << obeast::Thread::GetThis()->getId();
    OBEAST_LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        obeast::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    std::vector<obeast::Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        obeast::Thread::ptr thr(new obeast::Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    OBEAST_LOG_INFO(g_logger) << "count = " << count;
    return 0;
}

