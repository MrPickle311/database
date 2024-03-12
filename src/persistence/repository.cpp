#include "repository.hpp"
#include <iostream>

namespace db
{

    void StringRepository::create(const std::string &name)
    {
        std::cout << "Creating string: " << name << std::endl;
    }
    std::string StringRepository::get(const std::string &name)
    {
        return std::string();
    }
    bool StringRepository::exists(const std::string &substr)
    {
        return false;
    }
    unsigned int StringRepository::length(const std::string &name)
    {
        return 2137;
    }
    std::string StringRepository::substring(const std::string &name, const unsigned int start, const unsigned int end)
    {
        return "substring";
    }
    void StringRepository::append(const std::string &name, const std::string &postifx)
    {
        std::cout << "Appending string: " << postifx << std::endl;
    }
    void StringRepository::prepend(const std::string &name, const std::string &preifx)
    {
        std::cout << "Prepending string: " << preifx << std::endl;
    }
    void StringRepository::insert(const std::string &name, const std::string &value, unsigned int index)
    {
        std::cout << "Inserting string: " << value << " at index: " << index << std::endl;
    }
    void StringRepository::trim(const std::string &name, const unsigned int start, const unsigned int end)
    {
        std::cout << "Trimming string" << std::endl;
    }
    void StringRepository::ltrim(const std::string &name, const unsigned int count)
    {
        std::cout << "Left trimming string" << std::endl;
    }
    void StringRepository::rtrim(const std::string &name, const unsigned int count)
    {
        std::cout << "Right trimming string" << std::endl;
    }
}
