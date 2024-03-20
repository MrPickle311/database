#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace db
{
    class DatabaseException : public std::exception
    {
    private:
        std::string message_;
        std::string code_;

    public:
        DatabaseException(const std::string &message, const std::string &code) : message_(message), code_(code) {}
        std::string get_message() const
        {
            return message_;
        }
        std::string get_code() const
        {
            return code_;
        }
    };

    class Config
    {
    private:
        int port_ = 1234;
        int thread_count_ = 4;
        int dump_period_ = 10;
        std::string persistence_file_ = "server.config";

    public:
        int get_port() const { return port_; }
        int get_thread_count() const { return thread_count_; }
        std::string get_persistence_file() const { return persistence_file_; }
        int get_dump_period() const { return dump_period_; }

        void set_port(int port) { port_ = port; }
        void set_thread_count(int thread_count) { thread_count_ = thread_count; }
        void set_dump_period(int dump_period) { dump_period_ = dump_period; }
        void set_persistence_file(const std::string &persistence_file) { persistence_file_ = persistence_file; }
    };

    class ConfigParser
    {
    public:
        Config parse(const std::string &filename)
        {
            std::ifstream file(filename);

            if (!file.is_open())
            {
                std::cerr << "Cannot open config file: " << filename << std::endl;
                exit(1);
            }

            std::string line;
            Config config;
            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == '#')
                {
                    continue;
                }
                auto pos = line.find('=');
                if (pos == std::string::npos)
                {
                    std::cerr << "Bad config file format: " << line << std::endl;
                    exit(1);
                }
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                if (key == "port")
                {
                    config.set_port(std::stoi(value));
                }
                else if (key == "thread_count")
                {
                    config.set_thread_count(std::stoi(value));
                }
                else if (key == "persistence_file")
                {
                    config.set_persistence_file(value);
                }
                else if (key == "dump_period")
                {
                    config.set_dump_period(std::stoi(value));
                }
            }
            return config;
        }
    };

}
