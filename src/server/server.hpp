#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <parser.hpp>
#include <execution_ioc.hpp>

namespace db
{
    using DefaultExecutionIoC = ExecutionIoC<DefaultParser>;

    /**
     * @brief Interface for a database connection.
     *
     * A database connection represents an active connection to a database server. It provides methods for executing queries and
     * other operations on the database.
     */
    class Connection
    {
    public:
        /**
         * @brief Performs the connection to the database.
         *
         * This method should establish a connection to the database server and perform any necessary authentication or
         * initialization steps.
         */
        virtual void perform_connection() = 0;

        /**
         * @brief Returns the underlying socket of the connection.
         *
         * The returned socket can be used for sending and receiving data over the connection.
         *
         * @return The underlying socket of the connection.
         */
        virtual boost::asio::ip::tcp::socket &get_socket() = 0;
    };

    /**
     * @brief Implementation of the Connection class for reading with response.
     *
     * This class implements the Connection interface for a connection that reads data from the database server in response to
     * a request. It uses an IO service to handle asynchronous operations, and provides methods for sending and receiving data.
     */
    class DefaultReadWithResponseConnection : public Connection, public boost::enable_shared_from_this<DefaultReadWithResponseConnection>
    {
    private:
        /** The underlying socket of the connection. */
        boost::asio::ip::tcp::socket socket_;

        /** The buffer for receiving data from the server. */
        boost::asio::streambuf buffer_;

        /** The execution IO context for executing queries and other operations on the database. */
        boost::shared_ptr<DefaultExecutionIoC> execution_ioc_;

    public:
        /**
         * @brief Constructs a DefaultReadWithResponseConnection object.
         *
         * @param io_service The IO service to use for asynchronous operations.
         * @param execution_ioc The execution IO context for executing queries and other operations on the database.
         */
        DefaultReadWithResponseConnection(boost::asio::io_service &io_service, boost::shared_ptr<DefaultExecutionIoC> execution_ioc);

        /**
         * @brief Destroys a DefaultReadWithResponseConnection object.
         */
        ~DefaultReadWithResponseConnection();

        /**
         * @brief Performs the connection to the database.
         *
         * This method establishes a connection to the database server and initializes the underlying socket.
         */
        void perform_connection() override;

        /**
         * @brief Returns the underlying socket of the connection.
         *
         * @return The underlying socket of the connection.
         */
        boost::asio::ip::tcp::socket &get_socket() override;

    private:
        /**
         * @brief Callback function for handling the completion of a read operation.
         *
         * This function is called when a read operation completes, either successfully or with an error. It processes the
         * received data and sends it to the execution IO context for processing.
         *
         * @param ec The error code indicating whether the read operation succeeded or failed.
         * @param bytes_transferred The number of bytes transferred during the read operation.
         */
        void handle_read_finished(const boost::system::error_code ec, std::size_t bytes_transferred);

        /**
         * @brief Callback function for handling the completion of a write operation.
         *
         * This function is called when a write operation completes, either successfully or with an error. It processes the
         * response data from the database server and sends it to the client.
         *
         * @param ec The error code indicating whether the write operation succeeded or failed.
         * @param bytes_transferred The number of bytes transferred during the write operation.
         */
        void handle_write_finished(const boost::system::error_code ec, std::size_t bytes_transferred);
    };

    /**
     * @brief Interface for a database server.
     *
     * A database server represents a server that listens for incoming connections from clients. It provides methods for
     * accepting connections, and for managing the connections once they are established.
     */
    class TcpServer
    {
    public:
        /**
         * @brief Starts the server listening for incoming connections.
         *
         * This method should start the server listening for incoming connections on the specified port. Once a connection is
         * established, the server should call the provided handler function to manage the connection.
         */
        virtual void run() = 0;

    private:
        virtual void accept() = 0;
        virtual void handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec) = 0;
    };

    /**
     * @brief Implementation of the TcpServer class for a TCP server.
     *
     * This class implements the TcpServer interface for a TCP server. It uses an IO service to handle asynchronous
     * operations, and provides methods for accepting connections and managing the connections once they are established.
     */
    class DefaultTcpServer : public TcpServer
    {
    public:
        /**
         * @brief Constructs a DefaultTcpServer object.
         *
         * @param config The configuration for the server, including the port to listen on and any other necessary settings.
         * @param execution_ioc The execution IO context for executing queries and other operations on the database.
         */
        DefaultTcpServer(const Config &config, boost::shared_ptr<DefaultExecutionIoC> execution_ioc);

        /**
         * @brief Starts the server listening for incoming connections.
         *
         * This method starts the server listening for incoming connections on the specified port. Once a connection is
         * established, the server calls the provided handler function to manage the connection.
         */
        void run() override;

    private:
        /** The IO service used for asynchronous operations. */
        boost::asio::io_service io_service_;

        /** The TCP acceptor for listening for incoming connections. */
        boost::asio::ip::tcp::acceptor acceptor_;

        /** The buffer for receiving data from the client. */
        boost::asio::streambuf buffer_;

        /** The execution IO context for executing queries and other operations on the database. */
        boost::shared_ptr<DefaultExecutionIoC> execution_ioc_;

        /** The timer for scheduling periodic tasks. */
        boost::asio::deadline_timer timer_;

        /** The configuration for the server. */
        Config config_;

    private:
        /**
         * @brief Callback function for handling the completion of an accept operation.
         *
         * This function is called when an accept operation completes, either successfully or with an error. It creates a new
         * connection object and calls the provided handler function to manage the connection.
         *
         * @param conn The connection object for the incoming client.
         * @param ec The error code indicating whether the accept operation succeeded or failed.
         */
        void handle_accept(boost::shared_ptr<Connection> conn, const boost::system::error_code &ec);

        /**
         * @brief Schedules the next task.
         *
         * This method schedules the next task, if any, based on the configuration of the server.
         *
         * @param ec The error code indicating whether the previous task completed successfully or with an error.
         */
        void schedule(const boost::system::error_code &ec);

        /**
         * @brief Starts the server listening for incoming connections.
         *
         * This method called when the server is started. It starts the server listening for incoming connections on the specified port.
         */
        void accept() override;
    };
}