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
            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Error opening file: " << filename << std::endl;
                return false;
            }

            file.clear();
            // Write header
            file << "[HEADER]\n";

            // Save StringRepository data
            saveStringData(file);
            file << "\n";

            // Save SetRepository data
            saveSetData(file);
            file << "\n";

            // Save HashRepository data
            saveHashData(file);
            file << "\n";

            // Write footer
            file << "[FOOTER]\n";

            file.close();
            return true;
        }

        static DataExporter &get_instance()
        {
            static DataExporter instance;
            return instance;
        }

    private:
        static void saveStringData(std::ofstream &file)
        {
            auto &string_repository = StringRepository::get_instance();
            int string_count = string_repository.data_.size();
            file << "[" << string_count << "] ";
            for (const auto &key_value : string_repository.data_)
            {
                string_count++;
                file << key_value.first << " " << key_value.second.size() << " " << key_value.second << " ";
            }
        }

        static void saveSetData(std::ofstream &file)
        {
            auto &set_repository = SetRepository::get_instance();
            int set_count = set_repository.data_.size();
            file << "[" << set_count << "] ";
            for (const auto &key_value : set_repository.data_)
            {
                set_count++;
                std::stringstream set_data_stream;
                for (const auto &value : key_value.second)
                {
                    set_data_stream << value << " ";
                }
                file << key_value.first << " " << key_value.second.size() << " " << set_data_stream.str() << " ";
            }
        }

        static void saveHashData(std::ofstream &file)
        {
            auto &hash_repository = HashRepository::get_instance();
            int map_count = hash_repository.data_.size();
            file << "[" << map_count << "] ";
            for (const auto &key_value : hash_repository.data_)
            {
                map_count++;
                file << key_value.first << " " << key_value.second.size() << " ";
                for (const auto &inner_key_value : key_value.second)
                {
                    file << inner_key_value.first << " " << inner_key_value.second << " ";
                }
            }
        }
    };

};
