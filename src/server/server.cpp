#include <server.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

namespace db
{
    DefaultTcpServer::DefaultTcpServer(const short port)
        : io_service_{},
          acceptor_(this->io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        accept();
    }

    void DefaultTcpServer::run()
    {
        io_service_.run();
    }

    void DefaultTcpServer::accept()
    {
        std::cout << "Do accept" << std::endl;
        boost::shared_ptr<Connection> connection = boost::make_shared<DefaultReadWithResponseConnection>(this->io_service_);
        acceptor_.async_accept(connection->get_socket(),
                               boost::bind(&DefaultTcpServer::handle_accept, this , connection, boost::asio::placeholders::error));
    }

    void DefaultTcpServer::handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec)
    {

        if (!ec)
        {
            std::cout << "Połączono z klientem " << conn->get_socket().remote_endpoint() << std::endl;
            conn->perform_connection();
        }
        else
        {
            std::cout << "Błąd wykonywania " << ec << std::endl;
            return;
        }
        accept();
    }
}
