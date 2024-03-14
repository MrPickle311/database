#pragma once
#include <string>
#include <vector>

namespace db
{
    class Repository
    {
    public:
        virtual void create(const std::string &name) = 0;
    };

    class StringRepository : public Repository
    {
    public:
        virtual ~StringRepository(){};
        void create(const std::string &name) override;
        std::string get(const std::string &name);
        bool exists(const std::string &substr);
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

    class SetRepository : public Repository
    {
    public:
        ~SetRepository(){};
        void create(const std::string &name);
        void add(const std::string &name, const std::string &value);
        unsigned int len(const std::string &name);
        std::vector<std::string> intersection(const std::vector<std::string> &names);
        std::vector<std::string> difference(const std::vector<std::string> &names);
        std::vector<std::string> union_(const std::vector<std::string> &names); // Note: underscore used to avoid conflict with C++ union keyword
        bool contains(const std::string &name, const std::string &value);
        std::vector<std::string> get_all(const std::string &name);
        void move(const std::string &name1, const std::string &value, const std::string &name2);
        std::string get(const std::string &name, const std::string &optionalValue = "");
        std::string pop(const std::string &name, const std::string &optionalValue = "");

        static SetRepository &get_instance()
        {
            static SetRepository instance;
            return instance;
        }
    };

    class QueueRepository : public Repository
    {
    public:
        virtual ~QueueRepository() {}
        void create(const std::string &name) override;
        void push(const std::string &name, const std::string &value);
        std::string pop(const std::string &name);
        std::string poll(const std::string &name);

        static QueueRepository &get_instance()
        {
            static QueueRepository instance;
            return instance;
        }
    };

    class HashRepository : public Repository
    {
    public:
        virtual ~HashRepository() {}
        void create(const std::string &name) override;
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

};
