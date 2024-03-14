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
        StringRepository::get_instance().create(key_name_);
        return "OK";
    }

    std::string StringGetCommand::execute()
    {
        return StringRepository::get_instance().get(key_name_);
    }

    std::string StringExistsCommand::execute()
    {
        std::stringstream ss;
        ss << std::boolalpha << StringRepository::get_instance().exists(key_name_);
        return ss.str();
    }

    std::string StringLenCommand::execute()
    {
        return std::to_string(StringRepository::get_instance().length(key_name_));
    }

    std::string StringSubCommand::execute()
    {
        return StringRepository::get_instance().substring(key_name_, start_pos_, end_pos_);
    }

    std::string StringAppendCommand::execute()
    {
        StringRepository::get_instance().append(key_name_, value_);
        return "OK";
    }

    std::string StringPrependCommand::execute()
    {
        StringRepository::get_instance().prepend(key_name_, value_);
        return "OK";
    }

    std::string StringInsertCommand::execute()
    {
        StringRepository::get_instance().insert(key_name_, value_, pos_);
        return "OK";
    }

    std::string StringTrimCommand::execute()
    {
        StringRepository::get_instance().trim(key_name_, start_pos_, end_pos_);
        return "OK";
    }

    std::string StringLtrimCommand::execute()
    {
        StringRepository::get_instance().ltrim(key_name_, char_count_);
        return "OK";
    }

    std::string StringRtrimCommand::execute()
    {
        StringRepository::get_instance().rtrim(key_name_, char_count_);
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

    // SETS

    std::string CreateSetCommand::execute()
    {
        SetRepository::get_instance().create(key_name_);
        return "OK";
    }

    std::string SetAddCommand::execute()
    {
        SetRepository::get_instance().add(key_name_, value_);
        return "OK";
    }

    std::string SetLenCommand::execute()
    {
        return std::to_string(SetRepository::get_instance().len(key_name_));
    }

    std::string SetIntersectionCommand::execute()
    {
        auto result = SetRepository::get_instance().intersection(set_names_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string SetDifferenceCommand::execute()
    {
        auto result = SetRepository::get_instance().difference(set_names_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string SetUnionCommand::execute()
    {
        auto result = SetRepository::get_instance().union_(set_names_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string SetContainsCommand::execute()
    {
        std::stringstream ss;
        ss << std::boolalpha << SetRepository::get_instance().contains(key_name_, value_);
        return ss.str();
    }

    std::string SetGetAllCommand::execute()
    {
        auto result = SetRepository::get_instance().get_all(key_name_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string SetMoveCommand::execute()
    {
        SetRepository::get_instance().move(key_name_, value_, dest_set_name_);
        return "OK";
    }

    std::string SetGetCommand::execute()
    {
        return SetRepository::get_instance().get(key_name_, value_.value_or(""));
    }

    std::string SetPopCommand::execute()
    {
        return SetRepository::get_instance().pop(key_name_, value_.value_or(""));
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

    // SETS

    boost::shared_ptr<Command> CreateSetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateSetCommand>(input[0]);
    }

    boost::shared_ptr<Command> SetAddCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetAddCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> SetLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetLenCommand>(input[0]);
    }

    boost::shared_ptr<Command> SetIntersectionCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetIntersectionCommand>(input);
    }

    boost::shared_ptr<Command> SetDifferenceCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetDifferenceCommand>(input);
    }

    boost::shared_ptr<Command> SetUnionCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetUnionCommand>(input);
    }

    boost::shared_ptr<Command> SetContainsCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetContainsCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> SetGetAllCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetGetAllCommand>(input[0]);
    }

    boost::shared_ptr<Command> SetMoveCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetMoveCommand>(input[0], input[1], input[2]);
    }

    boost::shared_ptr<Command> SetGetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetGetCommand>(input[0], input.size() > 1 ? std::optional<std::string>{input[1]} : std::optional<std::string>{});
    }

    boost::shared_ptr<Command> SetPopCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetPopCommand>(input[0], input.size() > 1 ? std::optional<std::string>{input[1]} : std::optional<std::string>{});
    }

    boost::shared_ptr<Command> SetCommandFactory::create_command(const std::vector<std::string> &input)
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

    boost::shared_ptr<Command> CreateHashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
    }

    boost::shared_ptr<Command> CreateQueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::shared_ptr<Command>();
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