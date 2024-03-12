#pragma once
#include <string>

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
    };
}