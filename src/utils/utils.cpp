#include <utils.hpp>
#include <iostream>

namespace db
{
    DatabaseException::DatabaseException(const std::string &message, const std::string &code) : message_{message}, code_{code}
    {
    }

    std::string DatabaseException::get_message() const
    {
        return message_;
    }

    std::string DatabaseException::get_code() const
    {
        return code_;
    }

    Config ConfigParser::parse(const std::string &filename)
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

}