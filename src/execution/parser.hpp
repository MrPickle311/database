#pragma once
#include <command.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace db
{
    class Parser
    {
    public:
        virtual std::vector<boost::shared_ptr<Command>> extract_commands(const std::string &input) = 0;
    };

    class Tokenizer
    {
    public:
        virtual std::vector<std::string> tokenize(const std::string &input) = 0;
    };

    class Validator
    {

    public:
        virtual bool validate(const std::string &input) = 0;
    };

    class DefaultValidator : public Validator
    {
    public:
        bool validate(const std::string &input) override;
    };

    class DefaultTokenizer
    {
    private:
        const char delimeter = ';';

    public:
        virtual std::vector<std::string> tokenize(const std::string &input) = 0;
    };

    class DefaultParser : public Parser
    {
    private:
        boost::shared_ptr<Validator> validator_;
        boost::shared_ptr<Tokenizer> tokenizer_;

    public:
        std::vector<boost::shared_ptr<Command>> extract_commands(const std::string &input) override;
    };
}