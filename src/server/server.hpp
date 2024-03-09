#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

namespace db
{
    class Connection
    {
    public:
        virtual void perform_connection() = 0;
        virtual boost::asio::ip::tcp::socket &get_socket() = 0;
    };

    class ReadWithResponseConnection : public Connection
    {
    public:
        // virtual void read()
        virtual void handle_read_finished(const boost::system::error_code ec, std::size_t bytes_transferred) = 0;
        virtual void handle_write_finished(const boost::system::error_code ec, std::size_t bytes_transferred) = 0;
    };

    class DefaultReadWithResponseConnection : public ReadWithResponseConnection, public boost::enable_shared_from_this<DefaultReadWithResponseConnection>
    {
    private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::streambuf buffer_;

    public:
        DefaultReadWithResponseConnection(boost::asio::io_service &io_service) : socket_(io_service), buffer_()
        {
        }

        ~DefaultReadWithResponseConnection()
        {
            std::cout << "Destroyed tcp_connection\n";
        }

        void perform_connection() override
        {

            boost::asio::async_read_until(socket_, buffer_, boost::asio::string_view{"|"},
                                          boost::bind(&ReadWithResponseConnection::handle_read_finished,
                                                      shared_from_this(),
                                                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }

        boost::asio::ip::tcp::socket &get_socket() override
        {
            return socket_;
        }

    protected:
        void handle_read_finished(const boost::system::error_code ec, std::size_t bytes_transferred) override
        {
            if (!ec)
            {
                std::istream is(&this->buffer_);
                std::string line;
                std::getline(is, line);
                std::cout << "Odebrano: " << line << std::endl;


                std::cout << "zapis\n";

                const std::string response = "abc123";
                std::vector<char> data(response.length());
                std::copy(response.begin(), response.end(), data.begin());

                boost::asio::async_write(socket_, boost::asio::buffer(data),
                                         boost::bind(&ReadWithResponseConnection::handle_write_finished,
                                                     shared_from_this(),
                                                     boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                std::cerr << "Błąd odebrania danych: " << ec.message() << std::endl;
            }
        }
        void handle_write_finished(const boost::system::error_code ec, std::size_t bytes_transferred) override
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
    };

    class TcpServer
    {
    public:
        virtual void run() = 0;

    private:
        virtual void accept() = 0;
        virtual void handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec) = 0;
    };

    class DefaultTcpServer : public TcpServer
    {
    public:
        DefaultTcpServer(const short port);

        void run();

    private:
        void accept() override;
        void handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec) override;

    private:
        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::streambuf buffer_;
    };
}