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

    class BigTokenizer : public Tokenizer
    {
    private:
        const std::string delimeter = ";";

    public:
        std::vector<std::string> tokenize(const std::string &input) override;

    public:
        static Tokenizer &get_instance()
        {
            static BigTokenizer instance;
            return instance;
        }
    };

    class SmallTokenizer : public Tokenizer
    {
    private:
        const std::string delimeter = " ";

    public:
        std::vector<std::string> tokenize(const std::string &input);
        static Tokenizer &get_instance()
        {
            static SmallTokenizer instance;
            return instance;
        }
    };

    class DefaultParser : public Parser
    {
    private:
        Tokenizer &main_tokenizer_;
        Tokenizer &sub_tokenizer_;
        CommandFactory &command_factory_;

    public:
        DefaultParser(Tokenizer &main_tokenizer, Tokenizer &sub_tokenizer, CommandFactory &command_factory);

        std::vector<boost::shared_ptr<Command>> extract_commands(const std::string &input) override;
    };
}