#include <parser.hpp>
#include <boost/make_shared.hpp>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace db
{
    std::vector<std::string> cleanup(std::vector<std::string> vec, std::string element)
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(), [&element](const std::string &str)
                                 { return str == element || str.empty(); }),
                  vec.end());
        return vec;
    }

    bool is_all_whitespace(const std::string &str)
    {
        return std::all_of(str.begin(), str.end(), [](char c)
                           { return std::isspace(c); });
    }

    DefaultParser::DefaultParser(Tokenizer &main_tokenizer,
                                 Tokenizer &sub_tokenizer,
                                 CommandFactory &command_factory) : main_tokenizer_(main_tokenizer),
                                                                    sub_tokenizer_(sub_tokenizer),
                                                                    command_factory_(command_factory) {}

    std::vector<boost::shared_ptr<Command>> DefaultParser::extract_commands(const std::string &input)
    {
        std::vector<boost::shared_ptr<Command>> result{};

        auto commandTokens = this->main_tokenizer_.tokenize(input);

        for (auto &&commandToken : commandTokens)
        {
            if (!is_all_whitespace(commandToken))
            {
                auto subcommandTokens = this->sub_tokenizer_.tokenize(commandToken);
                auto cmd = this->command_factory_.get_command(subcommandTokens);
                result.push_back(cmd);
            }
        }

        return result;
    }

    std::vector<std::string> BigTokenizer::tokenize(const std::string &input)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of(this->delimeter));
        return tokens;
    }

    std::vector<std::string> SmallTokenizer::tokenize(const std::string &input)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, input, boost::is_any_of(this->delimeter));
        tokens = cleanup(tokens, " ");
        return tokens;
    }

}