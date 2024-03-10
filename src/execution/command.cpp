#include <command.hpp>

namespace db{

    std::string StringCommand::execute(){
        return this->str_ + " processed";
    }
}