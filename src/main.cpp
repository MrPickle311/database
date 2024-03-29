#include <iostream>
#include <server.hpp>
#include <boost/make_shared.hpp>
#include <parser.hpp>
#include <tbb/concurrent_hash_map.h>

int main(int argc, char *argv[])
{
    try
    {
        db::DefaultParser p{
            db::BigTokenizer::get_instance(),
            db::SmallTokenizer::get_instance(),
            db::GenericCommandFactory::get_instance()};
        boost::shared_ptr<db::DefaultExecutionIoC> exection_ioc = boost::make_shared<db::DefaultExecutionIoC>(p);
        db::ConfigParser config_parser;
        boost::shared_ptr<db::TcpServer> server = boost::make_shared<db::DefaultTcpServer>(config_parser.parse(argv[1]), exection_ioc);
        server->run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Wystąpił wyjątek: " << e.what() << std::endl;
    }
    return 0;
}
