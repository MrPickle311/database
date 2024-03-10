#pragma once
#include <command.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace db
{
    class Parser
    {
    public:
        virtual std::vector<boost::shared_ptr<Command>> extract_commands(const std::string& input) = 0;
    };

    class DefaultParser : public Parser
    {
    public:
        std::vector<boost::shared_ptr<Command>> extract_commands(const std::string& input) override;
    };
}