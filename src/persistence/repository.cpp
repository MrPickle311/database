#include "repository.hpp"
#include <iostream>
#include <set>
#include <utils.hpp>

namespace db
{

    // STRING
    void StringRepository::create(const std::string &name, const std::string &value)
    {
        KeysStorage &storage = KeysStorage::get_instance();
        if (storage.contains(name))
        {
            throw DatabaseException(name + " already exists", "KEY_EXISTS");
        }
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        data_.insert(a, name);
        a->second = value;
        storage.add(name);
    }

    std::string StringRepository::get(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            return a->second;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    bool StringRepository::exists(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        return data_.find(a, name);
    }

    unsigned int StringRepository::length(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.length();
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::string StringRepository::substring(const std::string &name, const unsigned int start, const unsigned int end)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;

        if (start > end)
        {
            throw DatabaseException("Second parameter must be greater than first parameter", "INVALID_ARGUMENTS");
        }

        if (data_.find(a, name))
        {
            if (a->second.size() < end - start)
            {
                throw DatabaseException("Substring's range is greater than string's size", "INVALID_ARGUMENTS");
            }
            return a->second.substr(start, end - start);
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::append(const std::string &name, const std::string &postfix)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            a->second.append(postfix);
            return;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::prepend(const std::string &name, const std::string &prefix)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            a->second = prefix + a->second;
            return;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::insert(const std::string &name, const std::string &value, unsigned int index)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            if (index <= a->second.length())
            {
                a->second.insert(index, value);
                return;
            }

            throw DatabaseException("Index is out of range", "INVALID_ARGUMENTS");
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::trim(const std::string &name, const unsigned int start, const unsigned int end)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            if (start <= end && end <= a->second.length())
            {
                std::string original = a->second;
                a->second.erase(start, end - start);
                return;
            }

            throw DatabaseException("Invalid range", "INVALID_ARGUMENTS");
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::ltrim(const std::string &name, const unsigned int count)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            if (count <= a->second.length())
            {
                a->second.erase(0, count);
                return;
            }

            throw DatabaseException("Invalid range", "INVALID_ARGUMENTS");
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void StringRepository::rtrim(const std::string &name, const unsigned int count)
    {
        tbb::concurrent_hash_map<std::string, std::string>::accessor a;
        if (data_.find(a, name))
        {
            if (count <= a->second.length())
            {
                a->second.erase(a->second.length() - count);
                return;
            }

            throw DatabaseException("Invalid range", "INVALID_ARGUMENTS");
        }
        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    // SETS

    void SetRepository::create(const std::string &name)
    {
        KeysStorage &storage = KeysStorage::get_instance();
        if (storage.contains(name))
        {
            throw DatabaseException(name + " already exists", "KEY_EXISTS");
        }
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        data_.insert(a, name);
        a->second = tbb::concurrent_set<std::string>{};
        storage.add(name);
    }

    void SetRepository::add(const std::string &name, const std::string &value)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            a->second.insert(value);
            return;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    unsigned int SetRepository::len(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.size();
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::string> SetRepository::intersection(const std::vector<std::string> &names)
    {
        if (names.empty())
        {
            return {};
        }

        auto unique_names = std::set<std::string>(names.begin(), names.end());

        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        auto is_first_set_found = data_.find(a, names[0]);

        if (!is_first_set_found)
        {

            throw DatabaseException(names[0] + " does not exist", "KEY_NOT_FOUND");
        }

        std::set<std::string> intersection;
        for (auto &&e : a->second)
        {
            intersection.insert(e);
        }
        a.release();

        for (auto &&name : unique_names)
        {
            tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor b;
            if (data_.find(b, name))
            {
                std::set<std::string> result;
                std::set_intersection(intersection.begin(), intersection.end(),
                                      b->second.begin(), b->second.end(),
                                      std::inserter(result, result.end()));
                intersection = result;
            }
            else
            {
                throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
            }
        }

        return std::vector<std::string>(intersection.begin(), intersection.end());
    }

    std::vector<std::string> SetRepository::difference(const std::string &name_1, const std::string &name_2)
    {
        if (name_1 == name_2)
        {
            throw DatabaseException("Cannot make difference between two objects with the same name", "INVALID_ARGUMENTS");
        }
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a, b;
        if (!data_.find(a, name_1) || !data_.find(b, name_2))
        {
            throw DatabaseException("One of key does not exist", "KEY_NOT_FOUND");
        }

        std::set<std::string> difference;
        std::set_difference(a->second.begin(), a->second.end(),
                            b->second.begin(), b->second.end(),
                            std::inserter(difference, difference.end()));

        return std::vector<std::string>(difference.begin(), difference.end());
    }

    std::vector<std::string> SetRepository::union_(const std::vector<std::string> &names)
    {
        if (names.empty())
        {
            return {};
        }

        auto unique_names = std::set<std::string>(names.begin(), names.end());

        std::set<std::string> union_set;

        for (const std::string &name : names)
        {
            tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
            if (data_.find(a, name))
            {
                union_set.insert(a->second.begin(), a->second.end());
            }
            else
            {
                throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
            }
        }

        return std::vector<std::string>(union_set.begin(), union_set.end());
    }

    bool SetRepository::contains(const std::string &name, const std::string &value)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.count(value) > 0;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::string> SetRepository::get_all(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return std::vector<std::string>(a->second.begin(), a->second.end());
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::string SetRepository::pop(const std::string &name, const std::string &value)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            if (a->second.count(value) > 0)
            {
                a->second.unsafe_erase(value);
                return value;
            }

            throw DatabaseException("Value not found in set", "VALUE_NOT_FOUND");
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    // QUEUES

    void QueueRepository::create(const std::string &name)
    {
        KeysStorage &storage = KeysStorage::get_instance();
        if (storage.contains(name))
        {
            throw DatabaseException(name + " already exists", "KEY_EXISTS");
        }
        tbb::concurrent_hash_map<std::string, tbb::concurrent_queue<std::string>>::accessor a;
        data_.insert(a, name);
        storage.add(name);
    }

    void QueueRepository::push(const std::string &name, const std::string &value)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_queue<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            a->second.push(value);
            return;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::string QueueRepository::pop(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_queue<std::string>>::accessor a;
        if (data_.find(a, name))
        {
            std::string value;
            if (!a->second.try_pop(value))
            {
                throw DatabaseException("Queue is empty", "QUEUE_EMPTY");
            }

            return value;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    // HASHES

    void HashRepository::create(const std::string &name)
    {
        KeysStorage &storage = KeysStorage::get_instance();
        if (storage.contains(name))
        {
            throw DatabaseException(name + " already exists", "KEY_EXISTS");
        }
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        data_.insert(a, name);
        storage.add(name);
    }

    void HashRepository::del(const std::string &name, const std::string &key)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            if (!a->second.erase(key))
            {
                throw DatabaseException("Key not found in hash", "KEY_NOT_FOUND");
            }
            return;
        }
        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    bool HashRepository::exists(const std::string &name, const std::string &key)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.count(key) > 0;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::string HashRepository::get(const std::string &name, const std::string &key)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            if (a->second.count(key) > 0)
            {
                tbb::concurrent_hash_map<std::string, std::string>::accessor b;
                a->second.find(b, key);
                return b->second;
            }

            throw DatabaseException("Key not found in hash", "KEY_NOT_FOUND");
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::pair<std::string, std::string>> HashRepository::get_all(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            std::vector<std::pair<std::string, std::string>> all_data;
            for (auto it = a->second.begin(); it != a->second.end(); ++it)
            {
                all_data.push_back(*it);
            }
            return all_data;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::string> HashRepository::get_keys(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            std::vector<std::string> all_keys;
            for (auto it = a->second.begin(); it != a->second.end(); ++it)
            {
                all_keys.push_back(it->first);
            }
            return all_keys;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    void HashRepository::set(const std::string &name, const std::string &key, const std::string &value)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            a->second.insert(std::make_pair(key, value));
            return;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    uint HashRepository::len(const std::string &name)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            return a->second.size();
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::string> HashRepository::search(const std::string &name, const std::string &query)
    {
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
        if (data_.find(a, name))
        {
            std::vector<std::string> result;
            for (auto it = a->second.begin(); it != a->second.end(); ++it)
            {
                result.push_back(it->first);
            }
            result;
            std::erase_if(result, [&](const std::string &key)
                          { return key.find(query) == std::string::npos; });
            return result;
        }

        throw DatabaseException(name + " does not exist", "KEY_NOT_FOUND");
    }

    std::vector<std::string> GlobalRepository::keys(std::string &pattern)
    {
        auto &&result = keys_storage_.get_keys();

        if (pattern != "*")
        {
            std::erase_if(result, [&](const std::string &key)
                          { return key.find(pattern) == std::string::npos; });
        }

        return std::vector<std::string>{result.begin(), result.end()};
    }

    void GlobalRepository::del(std::string &key)
    {
        if (keys_storage_.contains(key))
        {
            if (string_repository_.data_.erase(key))
            {
                keys_storage_.remove(key);
                return;
            }
            if (set_repository_.data_.erase(key))
            {
                keys_storage_.remove(key);
                return;
            }
            if (queue_repository_.data_.erase(key))
            {
                keys_storage_.remove(key);
                return;
            }
            if (hash_repository_.data_.erase(key))
            {
                keys_storage_.remove(key);
                return;
            }
        }
    }


        bool DataExporter::save(const std::string &filename)
        {
            std::fstream file(filename, std::ios::binary | std::ios::out);
            if (!file.is_open())
            {
                std::cerr << "Error opening file: " << filename << std::endl;
                return false;
            }
            file.clear();

            file.write("[HEADER]\0", 9);

            save_string_data(file);

            save_set_data(file);

            save_hash_data(file);

            file.write("[FOOTER]\3", 9);

            file.close();
            return true;
        }

        void DataExporter::save_string_data(std::fstream &file)
        {
            auto &string_repository = StringRepository::get_instance();
            uint32_t string_count = string_repository.data_.size();
            file.write(reinterpret_cast<char *>(&string_count), sizeof(string_count));
            for (const auto &key_value : string_repository.data_)
            {
                uint32_t key_length = key_value.first.size();
                file.write(reinterpret_cast<char *>(&key_length), sizeof(key_length));
                file.write(key_value.first.data(), key_length);
                file.put('\0');
                uint32_t value_length = key_value.second.size();
                file.write(reinterpret_cast<char *>(&value_length), sizeof(value_length));
                file.write(key_value.second.data(), value_length);
                file.put('\0');
            }
        }

        void DataExporter::save_set_data(std::fstream &file)
        {
            auto &set_repository = SetRepository::get_instance();
            uint32_t set_count = set_repository.data_.size();
            file.write(reinterpret_cast<char *>(&set_count), sizeof(set_count));
            for (const auto &key_value : set_repository.data_)
            {
                uint32_t key_length = key_value.first.size();
                file.write(reinterpret_cast<char *>(&key_length), sizeof(key_length));
                file.write(key_value.first.data(), key_length);
                file.put('\0');

                uint32_t value_count = key_value.second.size();
                file.write(reinterpret_cast<char *>(&value_count), sizeof(value_count));
                for (const auto &value : key_value.second)
                {
                    uint32_t value_length = value.size();
                    file.write(reinterpret_cast<char *>(&value_length), sizeof(value_length));
                    file.write(value.data(), value_length);
                    file.put('\0');
                }
            }
        }

        void DataExporter::save_hash_data(std::fstream &file)
        {
            auto &hash_repository = HashRepository::get_instance();
            uint32_t map_count = hash_repository.data_.size();
            file.write(reinterpret_cast<char *>(&map_count), sizeof(map_count));
            for (const auto &key_value : hash_repository.data_)
            {
                uint32_t key_length = key_value.first.size();
                file.write(reinterpret_cast<char *>(&key_length), sizeof(key_length));
                file.write(key_value.first.data(), key_length);
                file.put('\0');

                uint32_t inner_map_count = key_value.second.size();
                file.write(reinterpret_cast<char *>(&inner_map_count), sizeof(inner_map_count));
                for (const auto &inner_key_value : key_value.second)
                {
                    uint32_t inner_key_length = inner_key_value.first.size();
                    file.write(reinterpret_cast<char *>(&inner_key_length), sizeof(inner_key_length));
                    file.write(inner_key_value.first.data(), inner_key_length);
                    file.put('\0');

                    uint32_t value_length = inner_key_value.second.size();
                    file.write(reinterpret_cast<char *>(&value_length), sizeof(value_length));
                    file.write(inner_key_value.second.data(), value_length);
                    file.put('\0');
                }
            }
        }

    bool DataImporter::load(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }

        char header[9];
        file.read(header, sizeof(header));
        if (std::string(header) != "[HEADER]\0")
        {
            std::cerr << "Invalid file format: missing header." << std::endl;
            return false;
        }

        load_string_data(file);

        load_set_data(file);

        load_hash_data(file);

        file.close();
        return true;
    }

    void DataImporter::load_string_data(std::ifstream &file)
    {
        uint32_t string_count;
        file.read(reinterpret_cast<char *>(&string_count), sizeof(string_count));
        for (int i = 0; i < string_count; ++i)
        {
            uint32_t key_length;
            file.read(reinterpret_cast<char *>(&key_length), sizeof(key_length));
            std::string key(key_length, '\0');
            file.read(&key[0], key_length);
            file.get(); // Discard null terminator

            uint32_t value_length;
            file.read(reinterpret_cast<char *>(&value_length), sizeof(value_length));
            std::string value(value_length, '\0');
            file.read(&value[0], value_length);
            file.get(); // Discard null terminator

            tbb::concurrent_hash_map<std::string, std::string>::accessor a;
            StringRepository::get_instance().data_.insert(a, key);
            a->second = value;
            KeysStorage::get_instance().add(key);
        }
    }

    void DataImporter::load_set_data(std::ifstream &file)
    {
        uint32_t set_count;
        file.read(reinterpret_cast<char *>(&set_count), sizeof(set_count));
        for (int i = 0; i < set_count; ++i)
        {
            uint32_t key_length;
            file.read(reinterpret_cast<char *>(&key_length), sizeof(key_length));
            std::string key(key_length, '\0');
            file.read(&key[0], key_length);
            file.get(); // Discard null terminator

            uint32_t value_count;
            file.read(reinterpret_cast<char *>(&value_count), sizeof(value_count));
            std::set<std::string> value_set;
            for (int j = 0; j < value_count; ++j)
            {
                uint32_t value_length;
                file.read(reinterpret_cast<char *>(&value_length), sizeof(value_length));
                std::string value(value_length, '\0');
                file.read(&value[0], value_length);
                file.get(); // Discard null terminator
                value_set.insert(value);
            }
            tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>>::accessor a;
            SetRepository::get_instance().data_.insert(a, key);
            a->second = tbb::concurrent_set<std::string>{value_set.begin(), value_set.end()};
            KeysStorage::get_instance().add(key);
        }
    }

    void DataImporter::load_hash_data(std::ifstream &file)
    {
        uint32_t map_count;
        file.read(reinterpret_cast<char *>(&map_count), sizeof(map_count));
        for (int i = 0; i < map_count; ++i)
        {
            uint32_t key_length;
            file.read(reinterpret_cast<char *>(&key_length), sizeof(key_length));
            std::string key(key_length, '\0');
            file.read(&key[0], key_length);
            file.get(); // Discard null terminator

            uint32_t inner_map_count;
            file.read(reinterpret_cast<char *>(&inner_map_count), sizeof(inner_map_count));
            std::unordered_map<std::string, std::string> inner_map;
            for (int j = 0; j < inner_map_count; ++j)
            {
                uint32_t inner_key_length;
                file.read(reinterpret_cast<char *>(&inner_key_length), sizeof(inner_key_length));
                std::string inner_key(inner_key_length, '\0');
                file.read(&inner_key[0], inner_key_length);
                file.get(); // Discard null terminator

                uint32_t value_length;
                file.read(reinterpret_cast<char *>(&value_length), sizeof(value_length));
                std::string value(value_length, '\0');
                file.read(&value[0], value_length);
                file.get(); // Discard null terminator
                inner_map[inner_key] = value;
            }
            tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>>::accessor a;
            HashRepository::get_instance().data_.insert(a, key);
            a->second = tbb::concurrent_hash_map<std::string, std::string>{inner_map.begin(), inner_map.end()};
            KeysStorage::get_instance().add(key);
        }
    }
}
