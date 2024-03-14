#include "repository.hpp"
#include <iostream>

namespace db
{

    // STRING
    void StringRepository::create(const std::string &name)
    {
        std::cout << "Creating string: " << name << std::endl;
    }

    std::string StringRepository::get(const std::string &name)
    {
        return "just a string";
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

    // SETS
    void SetRepository::create(const std::string &name)
    {
        std::cout << "Creating set: " << name << std::endl;
    }

    void SetRepository::add(const std::string &name, const std::string &value)
    {
        std::cout << "Adding value: " << value << " to set: " << name << std::endl;
    }

    unsigned int SetRepository::len(const std::string &name)
    {
        return 2138;
    }

    std::vector<std::string> SetRepository::intersection(const std::vector<std::string> &names)
    {
        return std::vector<std::string>{"2", "3", "4"};
    }

    std::vector<std::string> SetRepository::difference(const std::vector<std::string> &names)
    {
        return std::vector<std::string>{"2", "1"};
    }

    std::vector<std::string> SetRepository::union_(const std::vector<std::string> &names)
    {
        return std::vector<std::string>{"5", "6"};
    }

    bool SetRepository::contains(const std::string &name, const std::string &value)
    {
        return true;
    }

    std::vector<std::string> SetRepository::get_all(const std::string &name)
    {
        return std::vector<std::string>{"1", "2", "3", "4", "5", "6"};
    }

    void SetRepository::move(const std::string &name1, const std::string &value, const std::string &name2)
    {
        std::cout << "Moving value: " << value << " from set: " << name1 << std::endl;
    }

    std::string SetRepository::get(const std::string &name, const std::string &optionalValue)
    {
        std::cout << "Getting value: " << optionalValue << " from set: " << name << std::endl;
        return "76";
    }

    std::string SetRepository::pop(const std::string &name, const std::string &optionalValue)
    {
        std::cout << "Popping value: " << optionalValue << " from set: " << name << std::endl;
        return "99";
    }
}
