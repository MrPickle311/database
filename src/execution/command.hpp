#pragma once

#include <string>

namespace db
{
    class Command
    {
    public:
        virtual std::string execute() = 0;
    };

    class CreateStringCommand
    {
    private:
        std::string string_name_;

    public:
        std::string execute();
    };

    class StringCommand : public Command
    {
    private:
        std::string str_;

    public:
        StringCommand(const std::string &str);
        std::string execute();
    };

    class CommandFactory
    {
    };
}