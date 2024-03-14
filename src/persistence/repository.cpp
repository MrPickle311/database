#include "repository.hpp"
#include <iostream>
#include <set>

namespace db
{

    // STRING
    void StringRepository::create(const std::string &name, const std::string &value)
    {
        // Use an accessor to safely insert the string into the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        data_.insert(a, name);
        // Initialize the value to an empty string
        a->second = value;
    }

    std::string StringRepository::get(const std::string &name)
    {
        // Use an accessor to safely retrieve the string from the map
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
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
        return data_.find(a, name);
    }

    unsigned int StringRepository::length(const std::string &name)
    {
        // Use an accessor to safely retrieve the string and get its length
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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
        if (data_.find(a, name))
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

    // Create a new set with the given name (empty initially)
    void SetRepository::create(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        data_.insert(a, name);
        a->second = tbb::concurrent_set<std::string>{};
    }

    // Add a value to the set identified by the name
    void SetRepository::add(const std::string &name, const std::string &value)
    {
        std::cout << "Adding value: " << value << " to set: " << name << std::endl;
        // Use an accessor to safely access and modify the set
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            a->second.insert(value);
        }
        else
        {
            // Handle the case where the set doesn'  t exist (e.g., create it or throw an exception)
        }
    }

    // Get the number of elements in the set identified by the name
    unsigned int SetRepository::len(const std::string &name)
    {
        // Use an accessor to safely access the set
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.size();
        }
        else
        {
            // Handle the case where the set doesn't exist (e.g., return 0 or throw an exception)
            return 0;
        }
    }

    // Find the intersection of elements in all sets identified by the names vector
    std::vector<std::string> SetRepository::intersection(const std::vector<std::string> &names)
    {
        if (names.empty())
        {
            return {}; // Empty intersection for empty list
        }

        // Use accessors for thread-safe access to sets
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        data_.find(a, names[0]);

        std::set<std::string> intersection;
        for (auto &&e : a->second)
        {
            intersection.insert(e);
        }

        for (size_t i = 1; i < names.size(); ++i)
        {
            tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor b;
            if (data_.find(b, names[i]))
            {
                std::set<std::string> result;
                // Use std::set_intersection for efficient intersection
                std::set_intersection(intersection.begin(), intersection.end(),
                                      b->second.begin(), b->second.end(),
                                      std::inserter(result, result.end()));
                intersection = result;
            }
            else
            {
                // Handle the case where a set doesn't exist (e.g., return empty or throw an exception)
                return {}; // Return empty in this case for clarity
            }
        }

        // Convert the intersection set to a vector
        return std::vector<std::string>(intersection.begin(), intersection.end());
    }

    std::vector<std::string> SetRepository::difference(const std::string &name_1, const std::string &name_2)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a, b;
        if (!data_.find(a, name_1) || !data_.find(b, name_2))
        {
            // Handle cases where one or both sets don't exist (e.g., return empty or throw an exception)
            std::cout << "One or both sets not found" << std::endl;
            return {}; // Return empty in this case for clarity
        }

        // Calculate the difference directly using std::set_difference
        std::set<std::string> difference;
        std::set_difference(a->second.begin(), a->second.end(),
                            b->second.begin(), b->second.end(),
                            std::inserter(difference, difference.end()));

        // Convert the difference set to a vector
        return std::vector<std::string>(difference.begin(), difference.end());
    }

    std::vector<std::string> SetRepository::union_(const std::vector<std::string> &names)
    {
        // Use a set to accumulate unique elements from all sets
        std::set<std::string> union_set;

        // Iterate through each set name
        for (const std::string &name : names)
        {
            tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
            if (data_.find(a, name))
            {
                // Insert elements from the current set into the union set
                union_set.insert(a->second.begin(), a->second.end());
            }
            else
            {
                // Handle the case where a set doesn't exist (e.g., log or throw an exception)
                std::cout << "Set " << name << " not found" << std::endl;
            }
        }

        // Convert the union set to a vector
        return std::vector<std::string>(union_set.begin(), union_set.end());
    }

    bool SetRepository::contains(const std::string &name, const std::string &value)
    {
        // Use accessor for thread-safe access
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            // Check if value exists in the set
            return a->second.count(value) > 0;
        }
        else
        {
            // Handle case where set doesn't exist
            return false; // Assuming a value cannot be in a non-existent set
        }
    }

    std::vector<std::string> SetRepository::get_all(const std::string &name)
    {
        // Use accessor for thread-safe access
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            // Copy elements from the set to a vector
            return std::vector<std::string>(a->second.begin(), a->second.end());
        }
        else
        {
            // Handle case where set doesn't exist
            return {}; // Possibly throw an exception or provide a default value
        }
    }

    std::string SetRepository::pop(const std::string &name, const std::string &value)
    {
        // Use accessor for thread-safe access
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            // Check if the value exists in the set
            if (a->second.count(value) > 0)
            {
                // Remove the value from the set
                a->second.unsafe_erase(value);
                return value; // Return the removed value
            }
            else
            {
                // Value not found, handle the case (e.g., return empty string, throw exception)
                return ""; // Replace with your desired behavior (e.g., throw std::runtime_error)
            }
        }
        else
        {
            // Set doesn't exist, handle the case
            return ""; // Replace with your desired behavior
        }
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
