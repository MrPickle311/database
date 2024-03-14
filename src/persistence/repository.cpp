#include "repository.hpp"
#include <iostream>

namespace db
{

    // STRING
    void StringRepository::create(const std::string &name, const std::string &value)
    {
        // Use an accessor to safely insert the string into the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        data.insert(a, name);
        // Initialize the value to an empty string
        a->second = value;
    }

    std::string StringRepository::get(const std::string &name)
    {
        // Use an accessor to safely retrieve the string from the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            return a->second;
        }
        else
        {
            // Handle the case where the string is not found (e.g., throw an exception or return a default value)
            return ""; // Example: return an empty string if not found
        }
    }

    bool StringRepository::exists(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        return data.find(a, name);
    }

    unsigned int StringRepository::length(const std::string &name)
    {
        // Use an accessor to safely retrieve the string and get its length
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            return a->second.length();
        }
        else
        {
            // Handle the case where the string is not found
            return 0; // Example: return 0 if not found
        }
    }

    std::string StringRepository::substring(const std::string &name, const unsigned int start, const unsigned int end)
    {
        // Use an accessor to safely retrieve the string and perform the substring operation
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            return a->second.substr(start, end - start);
        }
        else
        {
            // Handle the case where the string is not found
            return ""; // Example: return an empty string if not found
        }
    }

    void StringRepository::append(const std::string &name, const std::string &postfix)
    {
        // Use an accessor to safely modify the string within the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            a->second.append(postfix);
        }
        else
        {
            // Handle the case where the string is not found
            // (e.g., create the string with the given suffix or throw an exception)
        }
    }

    void StringRepository::prepend(const std::string &name, const std::string &prefix)
    {
        // Use an accessor to safely modify the string within the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            a->second = prefix + a->second;
        }
        else
        {
            // Handle the case where the string is not found
            // (e.g., create the string with the given prefix or throw an exception)
        }
    }

    void StringRepository::insert(const std::string &name, const std::string &value, unsigned int index)
    {
        // Use an accessor to safely modify the string within the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            // Check if index is within bounds
            if (index <= a->second.length())
            {
                a->second.insert(index, value);
            }
            else
            {
                // Handle the case where index is out of bounds (e.g., throw an exception)
            }
        }
        else
        {
            // Handle the case where the string is not found
            // (e.g., create the string with the inserted value or throw an exception)
        }
    }

    void StringRepository::trim(const std::string &name, const unsigned int start, const unsigned int end)
    {
        // Pobierz element mapy
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            // Sprawdź poprawność zakresu
            if (start <= end && end <= a->second.length())
            {
                // Utwórz kopię oryginalnego stringa
                std::string original = a->second;

                // Usuń fragment stringa
                a->second.erase(start, end - start);

                // Wyślij zdarzenie o usunięciu fragmentu
                // (opcjonalne, w zależności od potrzeb)
                // onTrimmed(name, start, end, original.substr(start, end - start));
            }
            else
            {
                // Obsłuż błąd niepoprawnego zakresu
                // (np. wyrzuć wyjątek)
            }
        }
        else
        {
            // Obsłuż błąd braku elementu
            // (np. wyrzuć wyjątek)
        }
    }

    void StringRepository::ltrim(const std::string &name, const unsigned int count)
    {
        // Use an accessor to safely modify the string within the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            // Check if count is within bounds
            if (count <= a->second.length())
            {
                a->second.erase(0, count);
            }
            else
            {
                // Handle the case where count exceeds string length (e.g., reset string to empty or throw an exception)
            }
        }
        else
        {
            // Handle the case where the string is not found
            // (e.g., create the string with the trimmed content or throw an exception)
        }
    }

    void StringRepository::rtrim(const std::string &name, const unsigned int count)
    {
        // Use an accessor to safely modify the string within the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data.find(a, name))
        {
            // Check if count is within bounds
            if (count <= a->second.length())
            {
                a->second.erase(a->second.length() - count);
            }
            else
            {
                // Handle the case where count exceeds string length (e.g., reset string to empty or throw an exception)
            }
        }
        else
        {
            // Handle the case where the string is not found
            // (e.g., create the string with the trimmed content or throw an exception)
        }
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
        std::cout << "Setting " << name << " to " << key << std::endl;
    }

    uint HashRepository::len(const std::string &name)
    {
        return 333;
    }

    std::vector<std::string> HashRepository::search(const std::string &name, const std::string &query)
    {
        return std::vector<std::string>{"1", "2", "3", "4"};
    }

    std::vector<std::string> GlobalRepository::keys(std::string &pattern)
    {
        return std::vector<std::string>{"1", "2", "3", "4", "5"};
    }

    void GlobalRepository::del(std::string &key)
    {
        std::cout << "Deleting key: " << key << std::endl;
    }
}
