#pragma once
#include <string>

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

    class Logger
    {
    public:
        virtual void println(const std::string &message) = 0;
    };

    class DefaultLogger : public Logger
    {
    public:
        void println(const std::string &message) override;
    };

}
