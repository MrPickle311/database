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

    // QUEUES

    void QueueRepository::create(const std::string &name)
    {
        std::cout << "Creating queue: " << name << std::endl;
    }

    void QueueRepository::push(const std::string &name, const std::string &value)
    {
        std::cout << "Pushing value: " << value << " to queue: " << name << std::endl;
    }

    std::string QueueRepository::pop(const std::string &name)
    {
        return std::string{"23"};
    }

    std::string QueueRepository::poll(const std::string &name)
    {
        return std::string{"67"};
    }

    // HASHES

    void HashRepository::create(const std::string &name)
    {
        std::cout << "Creating hash: " << name << std::endl;
    }

    void HashRepository::del(const std::string &name, const std::string &key)
    {
        std::cout << "Deleting key: " << key << " from hash: " << name << std::endl;
    }

    bool HashRepository::exists(const std::string &name, const std::string &key)
    {
        return true;
    }

    std::string HashRepository::get(const std::string &name, const std::string &key)
    {
        return std::string{"123"};
    }

    std::vector<std::pair<std::string, std::string>> HashRepository::get_all(const std::string &name)
    {
        return std::vector<std::pair<std::string, std::string>>{{"1", "2"}, {"3", "4"}};
    }

    std::vector<std::string> HashRepository::get_keys(const std::string &name)
    {
        return std::vector<std::string>{"1", "2", "3", "4"};
    }

    void HashRepository::set(const std::string &name, const std::string &key, const std::string &value)
    {
    }

    uint HashRepository::len(const std::string &name)
    {
        return 333;
    }

    std::vector<std::string> HashRepository::search(const std::string &name, const std::string &query)
    {
        return std::vector<std::string>{"1", "2", "3", "4"};
    }

}
