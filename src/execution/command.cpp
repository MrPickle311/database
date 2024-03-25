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

  CreateStringCommand::CreateStringCommand(const std::string &string_name, const std::string &value) : KeyedCommand{string_name}, value_{value} {}

  std::string CreateStringCommand::execute()
  {
    StringRepository::get_instance().create(key_name_, value_);
    return "OK";
  }

  StringGetCommand::StringGetCommand(const std::string &str_name) : KeyedCommand(str_name) {}

  std::string StringGetCommand::execute()
  {
    return StringRepository::get_instance().get(key_name_);
    }

  StringExistsCommand::StringExistsCommand(const std::string &str_name) : KeyedCommand(str_name) {}

  std::string StringExistsCommand::execute()
  {
    std::stringstream ss;
    ss << std::boolalpha << StringRepository::get_instance().exists(key_name_);
        return ss.str();
    }

  StringLenCommand::StringLenCommand(const std::string &str_name) : KeyedCommand(str_name) {}

  std::string StringLenCommand::execute()
  {
    return std::to_string(StringRepository::get_instance().length(key_name_));
    }

  StringSubCommand::StringSubCommand(const std::string &str_name, uint start_pos, uint end_pos)
    : KeyedCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}

  std::string StringSubCommand::execute()
  {
    return StringRepository::get_instance().substring(key_name_, start_pos_, end_pos_);
  }

  StringAppendCommand::StringAppendCommand(const std::string &str_name, const std::string &value)
    : KeyedCommand(str_name), value_(value) {}

  std::string StringAppendCommand::execute()
  {
    StringRepository::get_instance().append(key_name_, value_);
    return "OK";
  }

  StringPrependCommand::StringPrependCommand(const std::string &str_name, const std::string &value)
    : KeyedCommand(str_name), value_(value) {}

  std::string StringPrependCommand::execute()
  {
    StringRepository::get_instance().prepend(key_name_, value_);
    return "OK";
  }

  StringInsertCommand::StringInsertCommand(const std::string &str_name, uint pos, const std::string &value)
    : KeyedCommand(str_name), pos_(pos), value_(value) {}

  std::string StringInsertCommand::execute()
  {
    StringRepository::get_instance().insert(key_name_, value_, pos_);
    return "OK";
  }

  StringTrimCommand::StringTrimCommand(const std::string &str_name, uint start_pos, uint end_pos)
    : KeyedCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}

  std::string StringTrimCommand::execute()
  {
    StringRepository::get_instance().trim(key_name_, start_pos_, end_pos_);
    return "OK";
  }

  StringLtrimCommand::StringLtrimCommand(const std::string &str_name, uint char_count)
    : KeyedCommand(str_name), char_count_(char_count) {}

  std::string StringLtrimCommand::execute()
  {
    StringRepository::get_instance().ltrim(key_name_, char_count_);
    return "OK";
  }

  StringRtrimCommand::StringRtrimCommand(const std::string &str_name, uint char_count)
    : KeyedCommand(str_name), char_count_(char_count) {}

  std::string StringRtrimCommand::execute()
  {
    StringRepository::get_instance().rtrim(key_name_, char_count_);
    return "OK";
  }

  boost::shared_ptr<Command> CreateCommandFactory::create_command(const std::vector<std::string> &input)
    {
        std::vector<std::string> new_command(input.begin() + 1, input.end());
        if (!children_factories_.contains(input[0]))
        {
            throw DatabaseException("Unknown command: " + input[0], "CMD_UNKNOWN");
        }
        return children_factories_[input[0]]->get_command(new_command);
  }

  CreateCommandFactory::CreateCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

  boost::shared_ptr<Command> GenericCommandFactory::create_command(const std::vector<std::string> &input)
  {
    std::vector<std::string> new_command(input.begin() + 1, input.end());
        if (!children_factories_.contains(input[0]))
        {
            throw DatabaseException("Unknown command: " + input[0], "CMD_UNKNOWN");
        }
        return children_factories_[input[0]]->get_command(new_command);
  }

  GenericCommandFactory::GenericCommandFactory(const boost::shared_ptr<Validator> &validator) : CommandFactory(validator) {}

  CommandFactory &GenericCommandFactory::get_instance()
  {
    static GenericCommandFactory instance{boost::make_shared<ArgumentsCountValidator>(1)};
    return instance;
  }

  // SETS

  std::string CreateSetCommand::execute()
  {
    SetRepository::get_instance().create(key_name_);
    return "OK";
  }

  CreateSetCommand::CreateSetCommand(const std::string &set_name) : KeyedCommand{set_name} {}

    SetAddCommand::SetAddCommand(const std::string &set_name, const std::string &value) : KeyedCommand(set_name), value_(value) {}

    std::string SetAddCommand::execute()
    {
      SetRepository::get_instance().add(key_name_, value_);
      return "OK";
    }

    SetLenCommand::SetLenCommand(const std::string &set_name) : KeyedCommand(set_name) {}

    std::string SetLenCommand::execute()
    {
      return std::to_string(SetRepository::get_instance().len(key_name_));
    }

    SetIntersectionCommand::SetIntersectionCommand(const std::vector<std::string> &set_names) : set_names_(set_names) {}

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

    SetDifferenceCommand::SetDifferenceCommand(const std::string &set_name_1, const std::string &set_name_2) : set_name_1_(set_name_1), set_name_2_(set_name_2) {}

    std::string SetDifferenceCommand::execute()
    {
      auto result = SetRepository::get_instance().difference(set_name_1_, set_name_2_);
      std::stringstream ss;
        ss << "[ ";
        for (const auto &element : result)
        {
            ss << element << " ";
        }
        ss << "]";
        return ss.str();
    }

    SetUnionCommand::SetUnionCommand(const std::vector<std::string> &set_names) : set_names_(set_names) {}

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

    SetContainsCommand::SetContainsCommand(const std::string &set_name, const std::string &value) : KeyedCommand(set_name), value_(value) {}

    std::string SetContainsCommand::execute()
    {
      std::stringstream ss;
      ss << std::boolalpha << SetRepository::get_instance().contains(key_name_, value_);
        return ss.str();
    }

    SetGetAllCommand::SetGetAllCommand(const std::string &set_name) : KeyedCommand(set_name) {}

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

    SetPopCommand::SetPopCommand(const std::string &set_name, const std::string value) : KeyedCommand(set_name), value_(value) {}

    std::string SetPopCommand::execute()
    {
      return SetRepository::get_instance().pop(key_name_, value_);
    }

    // QUEUES

    CreateQueueCommand::CreateQueueCommand(const std::string &queue_name) : KeyedCommand{queue_name} {}

    std::string CreateQueueCommand::execute()
    {
      QueueRepository::get_instance().create(key_name_);
        return "OK";
    }

    QueuePushCommand::QueuePushCommand(const std::string &queue_name, const std::string &value) : KeyedCommand(queue_name), value_(value) {}

    std::string QueuePushCommand::execute()
    {
      QueueRepository::get_instance().push(key_name_, value_);
      return "OK";
    }

    QueuePopCommand::QueuePopCommand(const std::string &queue_name) : KeyedCommand(queue_name) {}

    std::string QueuePopCommand::execute()
    {
      return QueueRepository::get_instance().pop(key_name_);
    }

    // HASHES

    std::string CreateHashCommand::execute()
    {
        HashRepository::get_instance().create(key_name_);
        return "OK";
    }

    CreateHashCommand::CreateHashCommand(const std::string &hash_name) : KeyedCommand{hash_name} {}

    HashDelCommand::HashDelCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}

    std::string HashDelCommand::execute()
    {
      HashRepository::get_instance().del(key_name_, hash_key_);
      return "OK";
    }

    HashExistsCommand::HashExistsCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}

    std::string HashExistsCommand::execute()
    {
      std::stringstream ss;
      ss << std::boolalpha << HashRepository::get_instance().exists(key_name_, hash_key_);
        return ss.str();
    }

    HashGetCommand::HashGetCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}

    std::string HashGetCommand::execute()
    {
      return HashRepository::get_instance().get(key_name_, hash_key_);
    }

    HashGetAllCommand::HashGetAllCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}

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

    HashKeysCommand::HashKeysCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}

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

    HashSetCommand::HashSetCommand(const std::string &hash_name, const std::string &hash_key, const std::string &hash_value) : KeyedCommand(hash_name), hash_key_(hash_key), hash_value_(hash_value) {}

    std::string HashSetCommand::execute()
    {
      HashRepository::get_instance().set(key_name_, hash_key_, hash_value_);
      return "OK";
    }

    HashLenCommand::HashLenCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}

    std::string HashLenCommand::execute()
    {
      return std::to_string(HashRepository::get_instance().len(key_name_));
    }

    HashSearchCommand::HashSearchCommand(const std::string &hash_name, const std::string &query) : KeyedCommand(hash_name), query_(query) {}

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

    KeysCommand::KeysCommand(const std::optional<std::string> pattern) : pattern_(pattern) {}

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

    DelCommand::DelCommand(const std::string &key) : KeyedCommand(key) {}

    std::string DelCommand::execute()
    {
      GlobalRepository::get_instance().del(key_name_);
        return "OK";
    }

    // STRING FACTORIES

    boost::shared_ptr<Command> CreateStringCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateStringCommand>(input[0], input[1]);
    }

    CreateStringCommandFactory::CreateStringCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringGetCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringGetCommand>(input[0]);
    }

    StringGetCommandFactory::StringGetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringExistsCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringExistsCommand>(input[0]);
    }

    StringExistsCommandFactory::StringExistsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringLenCommand>(input[0]);
    }

    StringLenCommandFactory::StringLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringSubCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringSubCommand>(input[0],
                                                    boost::lexical_cast<unsigned int>(input[1]),
                                                    boost::lexical_cast<unsigned int>(input[2]));
    }

    StringSubCommandFactory::StringSubCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringAppendCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringAppendCommand>(input[0], input[1]);
    }

    StringAppendCommandFactory::StringAppendCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringPrependCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringPrependCommand>(input[0], input[1]);
    }

    StringPrependCommandFactory::StringPrependCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringInsertCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringInsertCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]), input[2]);
    }

    StringInsertCommandFactory::StringInsertCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringTrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringTrimCommand>(input[0],
                                                     boost::lexical_cast<unsigned int>(input[1]),
                                                     boost::lexical_cast<unsigned int>(input[2]));
    }

    StringTrimCommandFactory::StringTrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringLtrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringLtrimCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]));
    }

    StringLtrimCommandFactory::StringLtrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringRtrimCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<StringRtrimCommand>(input[0], boost::lexical_cast<unsigned int>(input[1]));
    }

    StringRtrimCommandFactory::StringRtrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> StringCommandFactory::create_command(const std::vector<std::string> &input)
    {
      std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        if (!children_factories_.contains(input[1]))
        {
            throw DatabaseException("Unknown command: " + input[1], "CMD_UNKNOWN");
        }
        return children_factories_[input[1]]->get_command(new_command);
    }

    StringCommandFactory::StringCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    // SETS FACTORIES

    boost::shared_ptr<Command> CreateSetCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateSetCommand>(input[0]);
    }

    CreateSetCommandFactory::CreateSetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetAddCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetAddCommand>(input[0], input[1]);
    }

    SetAddCommandFactory::SetAddCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetLenCommand>(input[0]);
    }

    SetLenCommandFactory::SetLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetIntersectionCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<SetIntersectionCommand>(input);
    }

    SetIntersectionCommandFactory::SetIntersectionCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetDifferenceCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetDifferenceCommand>(input[0], input[1]);
    }

    SetDifferenceCommandFactory::SetDifferenceCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetUnionCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetUnionCommand>(input);
    }

    SetUnionCommandFactory::SetUnionCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetContainsCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetContainsCommand>(input[0], input[1]);
    }

    SetContainsCommandFactory::SetContainsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetGetAllCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetGetAllCommand>(input[0]);
    }

    SetGetAllCommandFactory::SetGetAllCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetPopCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<SetPopCommand>(input[0], input[1]);
    }

    SetPopCommandFactory::SetPopCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> SetCommandFactory::create_command(const std::vector<std::string> &input)
    {
      std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        if (!children_factories_.contains(input[1]))
        {
            throw DatabaseException("Unknown command: " + input[1], "CMD_UNKNOWN");
        }
        return children_factories_[input[1]]->get_command(new_command);
    }

    SetCommandFactory::SetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    // QUEUES FACTORIES

    boost::shared_ptr<Command> CreateQueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateQueueCommand>(input[0]);
    }

    CreateQueueCommandFactory::CreateQueueCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> QueueCommandFactory::create_command(const std::vector<std::string> &input)
    {
      std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        if (!children_factories_.contains(input[1]))
        {
            throw DatabaseException("Unknown command: " + input[1], "CMD_UNKNOWN");
        }
        return children_factories_[input[1]]->get_command(new_command);
    }

    QueueCommandFactory::QueueCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> QueuePushCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<QueuePushCommand>(input[0], input[1]);
    }

    QueuePushCommandFactory::QueuePushCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> QueuePopCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<QueuePopCommand>(input[0]);
    }

    QueuePopCommandFactory::QueuePopCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    // HASHES FACTORIES

    boost::shared_ptr<Command> CreateHashCommandFactory::create_command(const std::vector<std::string> &input)
    {
        return boost::make_shared<CreateHashCommand>(input[0]);
    }

    CreateHashCommandFactory::CreateHashCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashCommandFactory::create_command(const std::vector<std::string> &input)
    {
      std::vector<std::string> new_command(input);
        auto it = new_command.begin() + 1;
        new_command.erase(it);
        if (!children_factories_.contains(input[1]))
        {
            throw DatabaseException("Unknown command: " + input[1], "CMD_UNKNOWN");
        }
        return children_factories_[input[1]]->get_command(new_command);
    }

    HashCommandFactory::HashCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashDelCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashDelCommand>(input[0], input[1]);
    }

    HashDelCommandFactory::HashDelCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashExistsCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashExistsCommand>(input[0], input[1]);
    }

    HashExistsCommandFactory::HashExistsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashGetCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashGetCommand>(input[0], input[1]);
    }

    HashGetCommandFactory::HashGetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashGetAllCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashGetAllCommand>(input[0]);
    }

    HashGetAllCommandFactory::HashGetAllCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashGetKeysCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashKeysCommand>(input[0]);
    }

    HashGetKeysCommandFactory::HashGetKeysCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashSetCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashSetCommand>(input[0], input[1], input[2]);
    }

    HashSetCommandFactory::HashSetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashLenCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashLenCommand>(input[0]);
    }

    HashLenCommandFactory::HashLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> HashSearchCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<HashSearchCommand>(input[0], input[1]);
    }

    HashSearchCommandFactory::HashSearchCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    // OTHER

    boost::shared_ptr<Command> DeleteCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<DelCommand>(input[0]);
    }

    DeleteCommandFactory::DeleteCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    boost::shared_ptr<Command> KeysCommandFactory::create_command(const std::vector<std::string> &input)
    {
      return boost::make_shared<KeysCommand>(input.size() > 0 ? std::optional<std::string>{input[0]} : std::optional<std::string>{});
    }

    KeysCommandFactory::KeysCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    ArgumentsCountValidator::ArgumentsCountValidator(uint count) : count_(count) {}

    bool ArgumentsCountValidator::validate(const std::vector<std::string> &input)
    {
        if (input.size() < count_)
        {
            std::string message = "Invalid number of arguments. Expected " + std::to_string(count_) +
                                  " got " + std::to_string(input.size()) + ".";
            throw DatabaseException(message, "BAD_ARG_LEN");
        }
        return true;
    }

    KeyedCommand::KeyedCommand(const std::string &str_name) : key_name_(str_name) {}

    CommandFactory::CommandFactory(const boost::shared_ptr<Validator> &validator) : validator_(validator) {}

    boost::shared_ptr<Command> CommandFactory::get_command(const std::vector<std::string> &input)
    {
      validator_->validate(input);
      return create_command(input);
    }

}
