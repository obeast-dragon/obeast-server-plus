/**
 * @file test_http_connection.cc
 * @brief HTTP客户端类测试
 * @version 0.1
 * @date 2021-12-09
 */
#include "src/obeast.h"
#include <iostream>

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

void test_pool() {
    obeast::http::HttpConnectionPool::ptr pool(new obeast::http::HttpConnectionPool(
        "www.midlane.top", "", 80, 10, 1000 * 30, 5));

    obeast::IOManager::GetThis()->addTimer(
        1000, [pool]() {
            auto r = pool->doGet("/", 300);
            std::cout << r->toString() << std::endl;
        },
        true);
}

void run() {
    obeast::Address::ptr addr = obeast::Address::LookupAnyIPAddress("www.midlane.top:80");
    if (!addr) {
        OBEAST_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    obeast::Socket::ptr sock = obeast::Socket::CreateTCP(addr);
    bool rt                 = sock->connect(addr);
    if (!rt) {
        OBEAST_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    obeast::http::HttpConnection::ptr conn(new obeast::http::HttpConnection(sock));
    obeast::http::HttpRequest::ptr req(new obeast::http::HttpRequest);
    req->setPath("/");
    req->setHeader("host", "www.midlane.top");
    // 小bug，如果设置了keep-alive，那么要在使用前先调用一次init
    req->setHeader("connection", "keep-alive");
    req->init();
    std::cout << "req:" << std::endl
              << *req << std::endl;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if (!rsp) {
        OBEAST_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    std::cout << "rsp:" << std::endl
              << *rsp << std::endl;

    std::cout << "=========================" << std::endl;

    auto r = obeast::http::HttpConnection::DoGet("http://www.midlane.top/wiki/", 300);
    std::cout << "result=" << r->result
              << " error=" << r->error
              << " rsp=" << (r->response ? r->response->toString() : "")
              << std::endl;

    std::cout << "=========================" << std::endl;
    test_pool();
}

int main(int argc, char **argv) {
    obeast::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
