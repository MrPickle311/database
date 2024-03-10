#pragma once

#include <string>

namespace db
{
    class Command
    {
    public:
        virtual std::string execute() = 0;
    };

    class StringCommand : public Command
    {
    private:
        std::string str_;

    public:
        std::string execute();
    };

    class CommandFactory{

    };
}