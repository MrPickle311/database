#include <utils.hpp>
#include <iostream>

namespace db
{
    void DefaultLogger::println(const std::string &message)
    {
        std::cout << message << std::endl;
    }
}