#include <iostream>
#include <server.hpp>
#include <boost/make_shared.hpp>

int main(int argc, char *argv[])
{
    try
    {
        boost::shared_ptr<db::DefaultExecutionIoC> exection_ioc = boost::make_shared<db::DefaultExecutionIoC>();
        boost::shared_ptr<db::TcpServer> server = boost::make_shared<db::DefaultTcpServer>(1234, exection_ioc);
        server->run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Wystąpił wyjątek: " << e.what() << std::endl;
    }
    return 0;
}
