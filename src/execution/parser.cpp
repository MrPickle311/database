#include <parser.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

namespace db
{


    std::vector<boost::shared_ptr<Command>> DefaultParser::extract_commands(const std::string &input)
    {
        if(!this->validator_->validate(input)){

        }
        auto tokens = this->tokenizer_->tokenize(input);

        std::vector<boost::shared_ptr<Command>> result{};
        result.push_back(boost::make_shared<StringCommand>("Just a result"));
        return result;
    }

    std::vector<std::string> DefaultTokenizer::tokenize(const std::string &input)
    {
        std::vector<std::string> result;
        std::istringstream stream{input};
        std::string token;

        while (std::getline(stream, token, this->delimeter))
        {
            result.push_back(token);
        }

        return result;
    }
    
    bool DefaultValidator::validate(const std::string &input)
    {
        return false;
    }
}