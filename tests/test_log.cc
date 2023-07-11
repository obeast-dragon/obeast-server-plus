/**
 * @file test_log.cpp
 * @brief 日志类测试
 * @version 0.1
 * @date 2021-06-10
 */

#include "src/obeast.h"

#include <unistd.h>

obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT(); // 默认INFO级别

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    OBEAST_LOG_FATAL(g_logger) << "fatal msg";
    OBEAST_LOG_ERROR(g_logger) << "err msg";
    OBEAST_LOG_INFO(g_logger) << "info msg";
    OBEAST_LOG_DEBUG(g_logger) << "debug msg";

    OBEAST_LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    OBEAST_LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    OBEAST_LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    OBEAST_LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);
   
    sleep(1);
    obeast::SetThreadName("brand_new_thread");

    g_logger->setLevel(obeast::LogLevel::WARN);
    OBEAST_LOG_FATAL(g_logger) << "fatal msg";
    OBEAST_LOG_ERROR(g_logger) << "err msg";
    OBEAST_LOG_INFO(g_logger) << "info msg"; // 不打印
    OBEAST_LOG_DEBUG(g_logger) << "debug msg"; // 不打印


    obeast::FileLogAppender::ptr fileAppender(new obeast::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    OBEAST_LOG_FATAL(g_logger) << "fatal msg";
    OBEAST_LOG_ERROR(g_logger) << "err msg";
    OBEAST_LOG_INFO(g_logger) << "info msg"; // 不打印
    OBEAST_LOG_DEBUG(g_logger) << "debug msg"; // 不打印

    obeast::Logger::ptr test_logger = OBEAST_LOG_NAME("test_logger");
    obeast::StdoutLogAppender::ptr appender(new obeast::StdoutLogAppender);
    obeast::LogFormatter::ptr formatter(new obeast::LogFormatter("%d:%rms%T%p%T%c%T%f:%l %m%n")); // 时间：启动毫秒数 级别 日志名称 文件名：行号 消息 换行
    appender->setFormatter(formatter);
    test_logger->addAppender(appender);
    test_logger->setLevel(obeast::LogLevel::WARN);

    OBEAST_LOG_ERROR(test_logger) << "err msg";
    OBEAST_LOG_INFO(test_logger) << "info msg"; // 不打印

    // 输出全部日志器的配置
    g_logger->setLevel(obeast::LogLevel::INFO);
    OBEAST_LOG_INFO(g_logger) << "logger config:" << obeast::LoggerMgr::GetInstance()->toYamlString();

    return 0;
}
