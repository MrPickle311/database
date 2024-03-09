#pragma once
#include <string>

namespace db
{
    class Logger{
    public:
        virtual void println(const std::string& message) = 0;
    };

}
