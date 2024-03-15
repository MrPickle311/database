#pragma once
#include <string>
#include <vector>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_set.h>
#include <tbb/concurrent_queue.h>

namespace db
{

    class StringRepository
    {
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
    public:
        virtual ~GlobalRepository() {}
        std::vector<std::string> keys(std::string &pattern);
        void del(std::string &key);

        static GlobalRepository &get_instance()
        {
            static GlobalRepository instance;
            return instance;
        }
    };

};
