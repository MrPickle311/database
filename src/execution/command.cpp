#include <command.hpp>

namespace db{
    StringCommand::StringCommand(const std::string &str):str_{str}
    {
    }

    std::string StringCommand::execute(){
        return this->str_ + " processed";
    }
}