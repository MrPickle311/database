#include <command.hpp>
#include <string>
#include <vector>
#include <repository.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace db
{
    // STRING

    std::string CreateStringCommand::execute()
    {
        StringRepository::get_instance().create(string_name_);
        return "OK";
    }

    std::string StringGetCommand::execute()
    {
        return StringRepository::get_instance().get(str_name_);
    }

    std::string StringExistsCommand::execute()
    {
        std::stringstream ss;
        ss << std::boolalpha << StringRepository::get_instance().exists(str_name_);
        return ss.str();
    }

    std::string StringLenCommand::execute()
    {
        return std::to_string(StringRepository::get_instance().length(str_name_));
    }

    std::string StringSubCommand::execute()
    {
        return StringRepository::get_instance().substring(str_name_, start_pos_, end_pos_);
    }

    std::string StringAppendCommand::execute()
    {
        StringRepository::get_instance().append(str_name_, value_);
        return "OK";
    }

    std::string StringPrependCommand::execute()
    {
        StringRepository::get_instance().prepend(str_name_, value_);
        return "OK";
    }

    std::string StringInsertCommand::execute()
    {
        StringRepository::get_instance().insert(str_name_, value_, pos_);
        return "OK";
    }

    std::string StringTrimCommand::execute()
    {
        StringRepository::get_instance().trim(str_name_, start_pos_, end_pos_);
        return "OK";
    }

    std::string StringLtrimCommand::execute()
    {
        StringRepository::get_instance().ltrim(str_name_, char_count_);
        return "OK";
    }

    std::string StringRtrimCommand::execute()
    {
        StringRepository::get_instance().rtrim(str_name_, char_count_);
        return "OK";
    }

    boost::shared_ptr<Command> CreateCommandFactory::create_command(const std::vector<std::string> &input)
    { 
        std::vector<std::string> new_command(input.begin() + 1, input.end());
        return children_factories_[input[0]]->create_command(new_command);
    }

    boost::shared_ptr<Command> GenericCommandFactory::create_command(const std::vector<std::string> &input)
    {
        std::vector<std::string> new_command(input.begin() + 1, input.end());
        return children_factories_[input[0]]->create_command(new_command);
    }

    // STRING FACTORIES

    boost::shared_ptr<Command> CreateStringCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateStringCommand>(input[0]);
    }

    boost::shared_ptr<Command> StringGetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringGetCommand>(input[0]);
    }

    boost::shared_ptr<Command> StringExistsCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringExistsCommand>(input[0]);
    }

    boost::shared_ptr<Command> StringLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringLenCommand>(input[0]);
    }

    boost::shared_ptr<Command> StringSubCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringSubCommand>(input[0],
                                                    boost::lexical_cast<unsigned int>(input[1]),
                                                    boost::lexical_cast<unsigned int>(input[2]));
    }

    boost::shared_ptr<Command> StringAppendCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringAppendCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> StringPrependCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringPrependCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> StringInsertCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringInsertCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]), input[2]);
    }

    boost::shared_ptr<Command> StringTrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringTrimCommand>(input[0],
                                                    boost::lexical_cast<unsigned int>(input[1]),
                                                    boost::lexical_cast<unsigned int>(input[2]));
    }

    boost::shared_ptr<Command> StringLtrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringLtrimCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]));
    }

    boost::shared_ptr<Command> StringRtrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<StringRtrimCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]));
    }

    boost::shared_ptr<Command> StringCommandFactory::create_command(const std::vector<std::string> &input)
    {
        std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        return children_factories_[input[1]]->create_command(new_command);
    }



    boost::shared_ptr<Command> DeleteCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> KeysCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> HashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> QueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> SetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> CreateSetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> CreateHashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> CreateQueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    std::string CreateSetCommand::execute()
    {
        return std::string();
    }

    std::string CreateHashCommand::execute()
    {
        return std::string();
    }

    std::string CreateQueueCommand::execute()
    {
        return std::string();
    }


}