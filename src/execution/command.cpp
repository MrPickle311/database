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

    // QUEUES

    std::string CreateQueueCommand::execute()
    {
        QueueRepository::get_instance().create(key_name_);
        return "OK";
    }

    std::string QueuePushCommand::execute()
    {
        QueueRepository::get_instance().push(key_name_, value_);
        return "OK";
    }

    std::string QueuePopCommand::execute()
    {
        return QueueRepository::get_instance().pop(key_name_);
    }

    std::string QueuePollCommand::execute()
    {
        return QueueRepository::get_instance().poll(key_name_);
    }

    // HASHES

    std::string CreateHashCommand::execute()
    {
        HashRepository::get_instance().create(key_name_);
        return "OK";
    }

    std::string HashDelCommand::execute()
    {
        HashRepository::get_instance().del(key_name_, hash_key_);
        return "OK";
    }

    std::string HashExistsCommand::execute()
    {
        std::stringstream ss;
        ss << std::boolalpha << HashRepository::get_instance().exists(key_name_, hash_key_);
        return ss.str();
    }

    std::string HashGetCommand::execute()
    {
        return HashRepository::get_instance().get(key_name_, hash_key_);
    }

    std::string HashGetAllCommand::execute()
    {
        auto result = HashRepository::get_instance().get_all(key_name_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << "{" << element.first << " : " << element.second << "} ";
        }
        ss << "]";
        return ss.str();
    }

    std::string HashKeysCommand::execute()
    {
        auto result = HashRepository::get_instance().get_keys(key_name_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string HashSetCommand::execute()
    {
        HashRepository::get_instance().set(key_name_, hash_key_, hash_value_);
        return "OK";
    }

    std::string HashLenCommand::execute()
    {
        return std::to_string(HashRepository::get_instance().len(key_name_));
    }

    std::string HashSearchCommand::execute()
    {
        auto result = HashRepository::get_instance().search(key_name_, query_);
        std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    // OTHER

    std::string KeysCommand::execute()
    {
        std::stringstream ss;
        auto pattern = pattern_.value_or("");
        auto result = GlobalRepository::get_instance().keys(pattern);
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    std::string DelCommand::execute()
    {
        GlobalRepository::get_instance().del(key_name_);
        return "OK";
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

    // SETS FACTORIES

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

    // QUEUES FACTORIES

    boost::shared_ptr<Command> CreateQueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateQueueCommand>(input[0]);
    }

    boost::shared_ptr<Command> QueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        return children_factories_[input[1]]->create_command(new_command);
    }

    boost::shared_ptr<Command> QueuePushCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<QueuePushCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> QueuePopCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<QueuePopCommand>(input[0]);
    }

    boost::shared_ptr<Command> QueuePollCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<QueuePollCommand>(input[0]);
    }

    // HASHES FACTORIES

    boost::shared_ptr<Command> CreateHashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateHashCommand>(input[0]);
    }

    boost::shared_ptr<Command> HashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        return children_factories_[input[1]]->create_command(new_command);
    }

    boost::shared_ptr<Command> HashDelCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashDelCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> HashExistsCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashExistsCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> HashGetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashGetCommand>(input[0], input[1]);
    }

    boost::shared_ptr<Command> HashGetAllCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashGetAllCommand>(input[0]);
    }

    boost::shared_ptr<Command> HashGetKeysCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashKeysCommand>(input[0]);
    }

    boost::shared_ptr<Command> HashSetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashSetCommand>(input[0], input[1], input[2]);
    }

    boost::shared_ptr<Command> HashLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashLenCommand>(input[0]);
    }

    boost::shared_ptr<Command> HashSearchCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<HashSearchCommand>(input[0], input[1]);
    }

    // OTHER

    boost::shared_ptr<Command> DeleteCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<DelCommand>(input[0]);
    }

    boost::shared_ptr<Command> KeysCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<KeysCommand>(input.size() > 0 ? std::optional<std::string>{input[0]} : std::optional<std::string>{});
    }

}