#pragma once
#include <string>

namespace db
{
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
