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
        DefaultReadWithResponseConnection(boost::asio::io_service &io_service);

        ~DefaultReadWithResponseConnection();

        void perform_connection() override;

        boost::asio::ip::tcp::socket &get_socket() override;

    protected:
        void handle_read_finished(const boost::system::error_code ec, std::size_t bytes_transferred) override;
        void handle_write_finished(const boost::system::error_code ec, std::size_t bytes_transferred) override;
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