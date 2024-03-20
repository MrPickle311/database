#include <server.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include <command.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/find.hpp>
#include <repository.hpp>
#include <boost/lexical_cast.hpp>

namespace db
{
    DefaultReadWithResponseConnection::DefaultReadWithResponseConnection(boost::asio::io_service &io_service,
                                                                         boost::shared_ptr<DefaultExecutionIoC> execution_ioc)
        : socket_{io_service},
          buffer_{},
          execution_ioc_{execution_ioc}
    {
    }

    DefaultReadWithResponseConnection::~DefaultReadWithResponseConnection()
    {
        std::cout << "Destroyed tcp_connection\n";
    }

    void DefaultReadWithResponseConnection::perform_connection()
    {
        std::cout << "Performing connection \n";
        boost::asio::async_read_until(socket_, buffer_, boost::asio::string_view{"|"},
                                      boost::bind(&ReadWithResponseConnection::handle_read_finished,
                                                  shared_from_this(),
                                                  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket &DefaultReadWithResponseConnection::get_socket()
    {
        return socket_;
    }

    void DefaultReadWithResponseConnection::handle_read_finished(const boost::system::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec)
        {
            std::string response;
            std::istream is(&this->buffer_);
            std::string received_data(std::istreambuf_iterator<char>(is), {});
            std::string trimmed_data = boost::trim_right_copy_if(received_data, [](char c)
                                                                 { return c != '|'; });
            trimmed_data = trimmed_data.substr(0, trimmed_data.size() - 1);

            std::cout << "Received data[" << trimmed_data << "]" << std::endl;

            try
            {
                std::vector<boost::shared_ptr<Command>> commands = this->execution_ioc_->getParser().extract_commands(trimmed_data);
                for (auto &&command : commands)
                {
                    response = command->execute();
                }
                response = "[1][" + response + "]" + "[]\n";
            }
            catch (const DatabaseException &e)
            {
                std::cout << "Error executing command  " << e.get_message() << std::endl;
                response = "[0][" + e.get_message() + "][" + e.get_code() + "]\n";
            }
            catch (const boost::bad_lexical_cast &e)
            {
                response = "[0][" + std::string{e.what()} + "][BAD_CAST]\n";
            }

            std::vector<char> data(response.length());
            std::copy(response.begin(), response.end(), data.begin());

            boost::asio::async_write(socket_, boost::asio::buffer(data),
                                     boost::bind(&ReadWithResponseConnection::handle_write_finished,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            std::cerr << "Error connection read handle : " << ec.message() << std::endl;
        }
    }

    void DefaultReadWithResponseConnection::handle_write_finished(const boost::system::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec)
        {
            std::cout << "Wysłano dane: " << std::endl;
        }
        else
        {
            std::cerr << "Błąd wysyłania danych: " << ec.message() << std::endl;
        }

        this->socket_.close();
    }

    DefaultTcpServer::DefaultTcpServer(const short port, boost::shared_ptr<DefaultExecutionIoC> execution_ioc)
        : io_service_{10},
          acceptor_(this->io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          execution_ioc_{execution_ioc},
          timer_{this->io_service_, boost::posix_time::seconds(10)}
    {
        DataImporter::load("db_data");
        accept();
        timer_.async_wait(boost::bind(&DefaultTcpServer::schedule, this, boost::asio::placeholders::error));
    }

    void DefaultTcpServer::run()
    {
        io_service_.run();
    }

    void DefaultTcpServer::accept()
    {
        std::cout << "Do accept" << std::endl;
        boost::shared_ptr<Connection> connection = boost::make_shared<DefaultReadWithResponseConnection>(this->io_service_, this->execution_ioc_);
        acceptor_.async_accept(connection->get_socket(),
                               boost::bind(&DefaultTcpServer::handle_accept, this, connection, boost::asio::placeholders::error));
    }

    void DefaultTcpServer::handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec)
    {

        if (!ec)
        {
            std::cout << "Connected with client " << conn->get_socket().remote_endpoint() << std::endl;
            conn->perform_connection();
        }
        else
        {
            std::cout << "Accept error " << ec << std::endl;
            return;
        }
        accept();
    }
    void DefaultTcpServer::schedule(const boost::system::error_code &ec)
    {
        std::cout << "Doing scheduled stuff" << std::endl;
        DataExporter::get_instance().save("db_data");
        timer_.expires_from_now(boost::posix_time::seconds(10));
        timer_.async_wait(boost::bind(&DefaultTcpServer::schedule, this, boost::asio::placeholders::error));
    }
}
