/**
 * @file test_socket.cc
 * @brief 测试Socket类，tcp服务器
 * @version 0.1
 * @date 2021-09-18
 */
#include <src/obeast.h>

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

void test_tcp_server() {
    int ret;

    auto addr = obeast::Address::LookupAnyIPAddress("0.0.0.0:12345");
    OBEAST_ASSERT(addr);

    auto socket = obeast::Socket::CreateTCPSocket();
    OBEAST_ASSERT(socket);

    ret = socket->bind(addr);
    OBEAST_ASSERT(ret);
    
    OBEAST_LOG_INFO(g_logger) << "bind success";

    ret = socket->listen();
    OBEAST_ASSERT(ret);

    OBEAST_LOG_INFO(g_logger) << socket->toString() ;
    OBEAST_LOG_INFO(g_logger) << "listening...";

    while(1) {
        auto client = socket->accept();
        OBEAST_ASSERT(client);
        OBEAST_LOG_INFO(g_logger) << "new client: " << client->toString();
        client->send("hello world", strlen("hello world"));
        client->close();
    }
}

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    obeast::IOManager iom(2);
    iom.schedule(&test_tcp_server);

    return 0;
}
