#pragma once
#include <string>
#include <vector>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_set.h>
#include <tbb/concurrent_queue.h>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

namespace db
{

    class GlobalRepository;
    class DataExporter;
    class DataImporter;

    class KeysStorage
    {
    private:
        tbb::concurrent_set<std::string> keys_;

    public:
        void add(const std::string &key)
        {
            keys_.insert(key);
        }
        bool contains(const std::string &key)
        {
            return keys_.find(key) != keys_.end();
        }
        void remove(const std::string &key)
        {
            keys_.unsafe_erase(key);
        }

        std::set<std::string> get_keys()
        {
            return std::set<std::string>(keys_.begin(), keys_.end());
        }

        static KeysStorage &get_instance()
        {
            static KeysStorage instance;
            return instance;
        }
    };

    class StringRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    private:
        tbb::concurrent_hash_map<std::string, std::string> data_;

    public:
        virtual ~StringRepository(){};
        void create(const std::string &name, const std::string &value);
        std::string get(const std::string &name);
        bool exists(const std::string &name);
        unsigned int length(const std::string &name);
        std::string substring(const std::string &name, const unsigned int start, const unsigned int end);
        void append(const std::string &name, const std::string &postifx);
        void prepend(const std::string &name, const std::string &preifx);
        void insert(const std::string &name, const std::string &value, unsigned int index);
        void trim(const std::string &name, const unsigned int start, const unsigned int end);
        void ltrim(const std::string &name, const unsigned int count);
        void rtrim(const std::string &name, const unsigned int count);

        static StringRepository &get_instance()
        {
            static StringRepository instance;
            return instance;
        }
    };

    class SetRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_set<std::string>> data_;

    public:
        ~SetRepository(){};
        void create(const std::string &name);
        void add(const std::string &name, const std::string &value);
        unsigned int len(const std::string &name);
        std::vector<std::string> intersection(const std::vector<std::string> &names);
        std::vector<std::string> difference(const std::string &name_1, const std::string &name_2);
        std::vector<std::string> union_(const std::vector<std::string> &names); // Note: underscore used to avoid conflict with C++ union keyword
        bool contains(const std::string &name, const std::string &value);
        std::vector<std::string> get_all(const std::string &name);
        std::string pop(const std::string &name, const std::string &value);

        static SetRepository &get_instance()
        {
            static SetRepository instance;
            return instance;
        }
    };

    class QueueRepository
    {
        friend class GlobalRepository;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_queue<std::string>> data_;

    public:
        virtual ~QueueRepository() {}
        void create(const std::string &name);
        void push(const std::string &name, const std::string &value);
        std::string pop(const std::string &name);

        static QueueRepository &get_instance()
        {
            static QueueRepository instance;
            return instance;
        }
    };

    class HashRepository
    {
        friend class GlobalRepository;
        friend class DataExporter;
        friend class DataImporter;

    private:
        tbb::concurrent_hash_map<std::string, tbb::concurrent_hash_map<std::string, std::string>> data_;

    public:
        virtual ~HashRepository() {}
        void create(const std::string &name);
        void del(const std::string &name, const std::string &key);
        bool exists(const std::string &name, const std::string &key);
        std::string get(const std::string &name, const std::string &key);
        std::vector<std::pair<std::string, std::string>> get_all(const std::string &name);
        std::vector<std::string> get_keys(const std::string &name);
        void set(const std::string &name, const std::string &key, const std::string &value);
        uint len(const std::string &name);
        std::vector<std::string> search(const std::string &name, const std::string &query);

        static HashRepository &get_instance()
        {
            static HashRepository instance;
            return instance;
        }
    };

    class GlobalRepository
    {
    private:
        StringRepository &string_repository_;
        SetRepository &set_repository_;
        QueueRepository &queue_repository_;
        HashRepository &hash_repository_;
        KeysStorage &keys_storage_;

        GlobalRepository(StringRepository &string_repository,
                         SetRepository &set_repository,
                         QueueRepository &queue_repository,
                         HashRepository &hash_repository,
                         KeysStorage &keys_storage) : string_repository_(string_repository),
                                                      set_repository_(set_repository),
                                                      queue_repository_(queue_repository),
                                                      hash_repository_(hash_repository),
                                                      keys_storage_{keys_storage} {}

    public:
        virtual ~GlobalRepository() {}
        std::vector<std::string> keys(std::string &pattern);
        void del(std::string &key);

        static GlobalRepository &get_instance()
        {
            static GlobalRepository instance{StringRepository::get_instance(), SetRepository::get_instance(), QueueRepository::get_instance(), HashRepository::get_instance(), KeysStorage::get_instance()};
            return instance;
        }
    };

    class DataExporter
    {
    public:
        static bool save(const std::string &filename)
        {
            std::fstream file(filename, std::ios::binary | std::ios::out);
            if (!file.is_open())
            {
                std::cerr << "Error opening file: " << filename << std::endl;
                return false;
            }
            file.clear();

            // Write header
            file.write("[HEADER]\0", 9);

            // Save StringRepository data
            saveStringData(file);

            // Save SetRepository data
            saveSetData(file);

            // Save HashRepository data
            saveHashData(file);

            // Write footer
            file.write("[FOOTER]\3", 9);

            file.close();
            return true;
        }

        static DataExporter &get_instance()
        {
            static DataExporter instance;
            return instance;
        }

    private:
        static void saveStringData(std::fstream &file)
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

        static void saveSetData(std::fstream &file)
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

        static void saveHashData(std::fstream &file)
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
    };

    class DataImporter
    {
    public:
        static bool load(const std::string &filename)
        {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open())
            {
                std::cerr << "Error opening file: " << filename << std::endl;
                return false;
            }

            // Read header
            char header[9];
            file.read(header, sizeof(header));
            if (std::string(header) != "[HEADER]\0")
            {
                std::cerr << "Invalid file format: missing header." << std::endl;
                return false;
            }

            // Load StringRepository data
            std::cout << "Loading string repository data..." << std::endl;
            loadStringData(file);

            // Load SetRepository data
            loadSetData(file);

            // Load HashRepository data
            loadHashData(file);

            file.close();
            return true;
        }

    private:
        static void loadStringData(std::ifstream &file)
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

        static void loadSetData(std::ifstream &file)
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

        static void loadHashData(std::ifstream &file)
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
        };
    };
}