/**
 * @file test_http_server.cc
 * @brief HttpServer测试
 * @version 0.1
 * @date 2021-09-28
 */
#include "src/obeast.h"

static obeast::Logger::ptr g_logger = OBEAST_LOG_ROOT();

#define XX(...) #__VA_ARGS__

obeast::IOManager::ptr worker;

void run() {
    g_logger->setLevel(obeast::LogLevel::INFO);
    //obeast::http::HttpServer::ptr server(new obeast::http::HttpServer(true, worker.get(), obeast::IOManager::GetThis()));
    obeast::http::HttpServer::ptr server(new obeast::http::HttpServer(true));
    obeast::Address::ptr addr = obeast::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/obeast/xx", [](obeast::http::HttpRequest::ptr req, obeast::http::HttpResponse::ptr rsp, obeast::http::HttpSession::ptr session) {
        rsp->setBody(req->toString());
        return 0;
    });

    sd->addGlobServlet("/obeast/*", [](obeast::http::HttpRequest::ptr req, obeast::http::HttpResponse::ptr rsp, obeast::http::HttpSession::ptr session) {
        rsp->setBody("Glob:\r\n" + req->toString());
        return 0;
    });

    sd->addGlobServlet("/obeastx/*", [](obeast::http::HttpRequest::ptr req, obeast::http::HttpResponse::ptr rsp, obeast::http::HttpSession::ptr session) {
        rsp->setBody(XX(<html>
                                <head><title> 404 Not Found</ title></ head>
                                <body>
                                <center><h1> 404 Not Found</ h1></ center>
                                <hr><center>
                                    nginx /
                                1.16.0 <
                            / center >
                            </ body>
                            </ html> < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >));
        return 0;
    });

    server->start();
}

int main(int argc, char **argv) {
    obeast::EnvMgr::GetInstance()->init(argc, argv);
    obeast::Config::LoadFromConfDir(obeast::EnvMgr::GetInstance()->getConfigPath());
    
    obeast::IOManager iom(1, true, "main");
    worker.reset(new obeast::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
