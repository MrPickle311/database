#include <parser.hpp>
#include <boost/make_shared.hpp>

namespace db
{


    std::vector<boost::shared_ptr<Command>> DefaultParser::extract_commands(const std::string &input)
    {
        std::vector<boost::shared_ptr<Command>> result{1};
        result.push_back(boost::make_shared<StringCommand>("Just a result"));
        return result;
    }
}