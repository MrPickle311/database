#include <parser.hpp>
#include <boost/make_shared.hpp>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace db
{
    DefaultParser::DefaultParser(Validator &validator,
                                 Tokenizer &main_tokenizer,
                                 Tokenizer &sub_tokenizer,
                                 CommandFactory &command_factory) : validator_(validator),
                                                                    main_tokenizer_(main_tokenizer),
                                                                    sub_tokenizer_(sub_tokenizer),
                                                                    command_factory_(command_factory) {}

    std::vector<boost::shared_ptr<Command>> DefaultParser::extract_commands(const std::string &input)
    {
        std::vector<boost::shared_ptr<Command>> result{};

        if (!validator_.validate(input))
        {
            throw std::runtime_error("Cannot parse input");
        }
        auto commandTokens = this->main_tokenizer_.tokenize(input);

        for (auto &&commandToken : commandTokens)
        {
            auto subcommandTokens = this->sub_tokenizer_.tokenize(commandToken);
            auto cmd = this->command_factory_.create_command(subcommandTokens);
            result.push_back(cmd);
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
        return tokens;
    }

    bool DefaultValidator::validate(const std::string &input)
    {
        return true;
    }
}