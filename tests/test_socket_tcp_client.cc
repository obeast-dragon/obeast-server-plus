/**
 * @file test_socket_tcp_client.cc
 * @brief 测试Socket类，tcp客户端
 * @version 0.1
 * @date 2021-09-18
 */
#include<src/obeast.h>

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

void test_tcp_client() {
    int ret;

    auto socket = obeast::Socket::CreateTCPSocket();
    OBEAST_ASSERT(socket);

    auto addr = obeast::Address::LookupAnyIPAddress("0.0.0.0:12345");
    OBEAST_ASSERT(addr);

    ret = socket->connect(addr);
    OBEAST_ASSERT(ret);

    OBEAST_LOG_INFO(g_logger) << "connect success, peer address: " << socket->getRemoteAddress()->toString();

    std::string buffer;
    buffer.resize(1024);
    socket->recv(&buffer[0], buffer.size());
    OBEAST_LOG_INFO(g_logger) << "recv: " << buffer;
    socket->close();

    return;
}

int main(int argc, char *argv[]) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());

    obeast::IOManager iom;
    iom.schedule(&test_tcp_client);

    return 0;
}
