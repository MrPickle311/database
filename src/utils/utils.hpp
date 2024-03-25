#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace db
{
    /**
     * \class DatabaseException
     * \brief Class representing an exception related to database operations.
     *
     * The class inherits from std::exception and is used to throw exceptions when errors occur during database interaction.
     */

    class DatabaseException : public std::exception
    {
    public:
        /**
         * Constructor for the DatabaseException class.
         *
         * @param message The error message.
         * @param code The error code (optional).
         */
        DatabaseException(const std::string &message, const std::string &code = "");

        /**
         * Gets the error message.
         *
         * @return The error message.
         */
        std::string get_message() const;

        /**
         * Gets the error code (if provided).
         *
         * @return The error code.
         */
        std::string get_code() const;

    private:
        std::string message_; ///< The error message.
        std::string code_;    ///< The error code.
    };

    /**
     * \class Config
     * \brief Class representing a configuration.
     *
     * This class represents database configuration. This class is used to configure database objects.
     */
    class Config
    {
    private:
        /**
         * The port on which the server should listen for incoming connections.
         */
        int port_ = 1234;

        /**
         * The number of threads to use for handling requests.
         */
        int thread_count_ = 4;

        /**
         * The number of seconds between periodic data dumps.
         */
        int dump_period_ = 10;

        /**
         * The file name to use for persistent storage of server data.
         */
        std::string persistence_file_ = "server.config";

    public:
        /**
         * Returns the port on which the server should listen for incoming connections.
         */
        int get_port() const { return port_; }

        /**
         * Returns the number of threads to use for handling requests.
         */
        int get_thread_count() const { return thread_count_; }

        /**
         * Returns the file name to use for persistent storage of server data.
         */
        std::string get_persistence_file() const { return persistence_file_; }

        /**
         * Returns the number of seconds between periodic data dumps.
         */
        int get_dump_period() const { return dump_period_; }

        /**
         * Sets the port on which the server should listen for incoming connections.
         */
        void set_port(int port) { port_ = port; }

        /**
         * Sets the number of threads to use for handling requests.
         */
        void set_thread_count(int thread_count) { thread_count_ = thread_count; }

        /**
         * Sets the number of seconds between periodic data dumps.
         */
        void set_dump_period(int dump_period) { dump_period_ = dump_period; }

        /**
         * Sets the file name to use for persistent storage of server data.
         */
        void set_persistence_file(const std::string &persistence_file) { persistence_file_ = persistence_file; }
    };

    /**
     * \class ConfigParser
     * \brief Reads a configuration file and returns a Config object with the specified parameters.
     *
     * The ConfigParser class provides a method for reading a configuration file and returning a Config object with the parameters specified in the file.
     */
    class ConfigParser
    {
    public:
        /**
         * Reads a configuration file and returns a Config object with the specified parameters.
         *
         * @param filename the name of the configuration file to read
         * @return a Config object with the parameters specified in the configuration file
         */
        Config parse(const std::string &filename);
    };
}
