#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <map>
#include <boost/make_shared.hpp>
#include <optional>

namespace db
{
    class ExecutionException : public std::exception
    {
    private:
        std::string message_;
        std::string code_;

    public:
        ExecutionException(const std::string &message, const std::string &code) : message_(message), code_(code) {}
        std::string get_message() const
        {
            return message_;
        }
        std::string get_code() const
        {
            return code_;
        }
    };

    class Validator
    {

    public:
        virtual bool validate(const std::vector<std::string> &input) = 0;
    };

    class ArgumentsCountValidator : public Validator
    {
    private:
        uint count_;

    public:
        ArgumentsCountValidator(uint count) : count_(count) {}
        bool validate(const std::vector<std::string> &input) override
        {
            if (!(input.size() >= count_))
            {
                throw ExecutionException(" Invalid number of arguments. Expected " + std::to_string(count_) + " got " + std::to_string(input.size()) + ".", "BAD_ARG_LEN");
            }
            return true;
        }
    };

    class Command
    {
    public:
        virtual std::string execute() = 0;
    };

    class KeyedCommand : public Command
    {
    protected:
        std::string key_name_;

    public:
        KeyedCommand(const std::string &str_name) : key_name_(str_name) {}
    };

    // CREATE

    class CreateStringCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        CreateStringCommand(const std::string &string_name, const std::string &value) : KeyedCommand{string_name}, value_{value} {}
        std::string execute();
    };

    class CreateSetCommand : public KeyedCommand
    {
    public:
        std::string execute();
        CreateSetCommand(const std::string &set_name) : KeyedCommand{set_name} {}
    };

    class CreateHashCommand : public KeyedCommand
    {
    public:
        std::string execute();
        CreateHashCommand(const std::string &hash_name) : KeyedCommand{hash_name} {}
    };

    class CreateQueueCommand : public KeyedCommand
    {
    public:
        CreateQueueCommand(const std::string &queue_name) : KeyedCommand{queue_name} {}
        std::string execute();
    };

    // STRING

    class StringGetCommand : public KeyedCommand
    {
    public:
        StringGetCommand(const std::string &str_name) : KeyedCommand(str_name) {}
        std::string execute() override;
    };

    class StringExistsCommand : public KeyedCommand
    {
    public:
        StringExistsCommand(const std::string &str_name) : KeyedCommand(str_name) {}
        std::string execute() override;
    };

    class StringLenCommand : public KeyedCommand
    {
    public:
        StringLenCommand(const std::string &str_name) : KeyedCommand(str_name) {}
        std::string execute() override;
    };

    class StringSubCommand : public KeyedCommand
    {
    private:
        uint start_pos_;
        uint end_pos_;

    public:
        StringSubCommand(const std::string &str_name, uint start_pos, uint end_pos)
            : KeyedCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}
        std::string execute() override;
    };

    class StringAppendCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        StringAppendCommand(const std::string &str_name, const std::string &value)
            : KeyedCommand(str_name), value_(value) {}
        std::string execute() override;
    };

    class StringPrependCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        StringPrependCommand(const std::string &str_name, const std::string &value)
            : KeyedCommand(str_name), value_(value) {}
        std::string execute() override;
    };

    class StringInsertCommand : public KeyedCommand
    {
    private:
        uint pos_;
        std::string value_;

    public:
        StringInsertCommand(const std::string &str_name, uint pos, const std::string &value)
            : KeyedCommand(str_name), pos_(pos), value_(value) {}
        std::string execute() override;
    };

    class StringTrimCommand : public KeyedCommand
    {
    private:
        uint start_pos_;
        uint end_pos_;

    public:
        StringTrimCommand(const std::string &str_name, uint start_pos, uint end_pos)
            : KeyedCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}
        std::string execute() override;
    };

    class StringLtrimCommand : public KeyedCommand
    {
    private:
        uint char_count_;

    public:
        StringLtrimCommand(const std::string &str_name, uint char_count)
            : KeyedCommand(str_name), char_count_(char_count) {}
        std::string execute() override;
    };

    class StringRtrimCommand : public KeyedCommand
    {
    private:
        uint char_count_;

    public:
        StringRtrimCommand(const std::string &str_name, uint char_count)
            : KeyedCommand(str_name), char_count_(char_count) {}
        std::string execute() override;
    };

    // SETS

    class SetAddCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        SetAddCommand(const std::string &set_name, const std::string &value) : KeyedCommand(set_name), value_(value) {}
        std::string execute() override;
    };

    class SetLenCommand : public KeyedCommand
    {
    public:
        SetLenCommand(const std::string &set_name) : KeyedCommand(set_name) {}
        std::string execute() override;
    };

    class SetIntersectionCommand : public Command
    {
    private:
        std::vector<std::string> set_names_;

    public:
        SetIntersectionCommand(const std::vector<std::string> &set_names) : set_names_(set_names) {}
        std::string execute() override;
    };

    class SetDifferenceCommand : public Command
    {
    private:
        std::string set_name_1_;
        std::string set_name_2_;

    public:
        SetDifferenceCommand(const std::string &set_name_1, const std::string &set_name_2) : set_name_1_(set_name_1), set_name_2_(set_name_2) {}
        std::string execute() override;
    };

    class SetUnionCommand : public Command
    {
    private:
        std::vector<std::string> set_names_;

    public:
        SetUnionCommand(const std::vector<std::string> &set_names) : set_names_(set_names) {}
        std::string execute() override;
    };

    class SetContainsCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        SetContainsCommand(const std::string &set_name, const std::string &value) : KeyedCommand(set_name), value_(value) {}
        std::string execute() override;
    };

    class SetGetAllCommand : public KeyedCommand
    {
    public:
        SetGetAllCommand(const std::string &set_name) : KeyedCommand(set_name) {}
        std::string execute() override;
    };

    class SetPopCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        SetPopCommand(const std::string &set_name, const std::string value) : KeyedCommand(set_name), value_(value) {}
        std::string execute() override;
    };

    // QUEUE

    class QueuePushCommand : public KeyedCommand
    {
    private:
        std::string value_;

    public:
        QueuePushCommand(const std::string &queue_name, const std::string &value) : KeyedCommand(queue_name), value_(value) {}
        std::string execute() override;
    };

    class QueuePopCommand : public KeyedCommand
    {
    public:
        QueuePopCommand(const std::string &queue_name) : KeyedCommand(queue_name) {}
        std::string execute() override;
    };

    // HASHES

    class HashDelCommand : public KeyedCommand
    {
    private:
        std::string hash_key_;

    public:
        HashDelCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}
        std::string execute() override;
    };

    class HashExistsCommand : public KeyedCommand
    {
    private:
        std::string hash_key_;

    public:
        HashExistsCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}
        std::string execute() override;
    };

    class HashGetCommand : public KeyedCommand
    {
    private:
        std::string hash_key_;

    public:
        HashGetCommand(const std::string &hash_name, const std::string &hash_key) : KeyedCommand(hash_name), hash_key_(hash_key) {}
        std::string execute() override;
    };

    class HashGetAllCommand : public KeyedCommand
    {
    public:
        HashGetAllCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}
        std::string execute() override;
    };

    class HashKeysCommand : public KeyedCommand
    {
    public:
        HashKeysCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}
        std::string execute() override;
    };

    class HashSetCommand : public KeyedCommand
    {
    private:
        std::string hash_key_;
        std::string hash_value_;

    public:
        HashSetCommand(const std::string &hash_name, const std::string &hash_key, const std::string &hash_value) : KeyedCommand(hash_name), hash_key_(hash_key), hash_value_(hash_value) {}
        std::string execute() override;
    };

    class HashLenCommand : public KeyedCommand
    {
    public:
        HashLenCommand(const std::string &hash_name) : KeyedCommand(hash_name) {}
        std::string execute() override;
    };

    class HashSearchCommand : public KeyedCommand
    {
    private:
        std::string query_;

    public:
        HashSearchCommand(const std::string &hash_name, const std::string &query) : KeyedCommand(hash_name), query_(query) {}
        std::string execute() override;
    };

    // OTHER

    class KeysCommand : public Command
    {
    public:
        std::optional<std::string> pattern_;

    public:
        KeysCommand(const std::optional<std::string> pattern) : pattern_(pattern) {}
        std::string execute() override;
    };

    class DelCommand : public KeyedCommand
    {
    public:
        DelCommand(const std::string &key) : KeyedCommand(key) {}
        std::string execute() override;
    };

    //////FACTORY

    class CommandFactory
    {
    protected:
        boost::shared_ptr<Validator> validator_;

    public:
        CommandFactory(const boost::shared_ptr<Validator> &validator) : validator_(validator) {}
        boost::shared_ptr<Command> get_command(const std::vector<std::string> &input)
        {
            validator_->validate(input);
            return create_command(input);
        }

    private:
        virtual boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) = 0;
    };

    // CREATE

    class CreateStringCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        CreateStringCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class CreateSetCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        CreateSetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class CreateHashCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        CreateHashCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class CreateQueueCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        CreateQueueCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class CreateCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        CreateCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"STR", boost::make_shared<CreateStringCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"SET", boost::make_shared<CreateSetCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"HASH", boost::make_shared<CreateHashCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"QUEUE", boost::make_shared<CreateQueueCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))}};
    };

    // STRINGS

    class StringExistsCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringExistsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringGetCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringGetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringLenCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringSubCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringSubCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringAppendCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringAppendCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringPrependCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringPrependCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringInsertCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringInsertCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringTrimCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringTrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringLtrimCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringLtrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringRtrimCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        StringRtrimCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class StringCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        StringCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"EXISTS", boost::make_shared<StringExistsCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"GET", boost::make_shared<StringGetCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"LEN", boost::make_shared<StringLenCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"SUB", boost::make_shared<StringSubCommandFactory>(boost::make_shared<ArgumentsCountValidator>(3))},
            {"APPEND", boost::make_shared<StringAppendCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"PREPEND", boost::make_shared<StringPrependCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"INSERT", boost::make_shared<StringInsertCommandFactory>(boost::make_shared<ArgumentsCountValidator>(3))},
            {"TRIM", boost::make_shared<StringTrimCommandFactory>(boost::make_shared<ArgumentsCountValidator>(3))},
            {"LTRIM", boost::make_shared<StringLtrimCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"RTRIM", boost::make_shared<StringRtrimCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))}};
    };

    // SETS

    class SetAddCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetAddCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetLenCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetIntersectionCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetIntersectionCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetDifferenceCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetDifferenceCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetUnionCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetUnionCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetContainsCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetContainsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetGetAllCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetGetAllCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetPopCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetPopCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class SetCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        SetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"ADD", boost::make_shared<SetAddCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"LEN", boost::make_shared<SetLenCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"INTER", boost::make_shared<SetIntersectionCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"DIFF", boost::make_shared<SetDifferenceCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"UNION", boost::make_shared<SetUnionCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"CONTAINS", boost::make_shared<SetContainsCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"GETALL", boost::make_shared<SetGetAllCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"POP", boost::make_shared<SetPopCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))}};
    };

    // QUEUES

    class QueuePushCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        QueuePushCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class QueuePopCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        QueuePopCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class QueueCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        QueueCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"PUSH", boost::make_shared<QueuePushCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"POP", boost::make_shared<QueuePopCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))}};
    };

    // HASHES

    class HashDelCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashDelCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashExistsCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashExistsCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashGetCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashGetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashGetAllCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashGetAllCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashGetKeysCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashGetKeysCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashSetCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashSetCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashLenCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashLenCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashSearchCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;

    public:
        HashSearchCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class HashCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        HashCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"DEL", boost::make_shared<HashDelCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"EXISTS", boost::make_shared<HashExistsCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"GET", boost::make_shared<HashGetCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"GETALL", boost::make_shared<HashGetAllCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"GETKEYS", boost::make_shared<HashGetKeysCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"SET", boost::make_shared<HashSetCommandFactory>(boost::make_shared<ArgumentsCountValidator>(3))},
            {"LEN", boost::make_shared<HashLenCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"SEARCH", boost::make_shared<HashSearchCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))}};
    };

    // OTHER

    class DeleteCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        DeleteCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class KeysCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        KeysCommandFactory(const boost::shared_ptr<Validator> validator) : CommandFactory(validator) {}
    };

    class GenericCommandFactory : public CommandFactory
    {
    private:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    public:
        GenericCommandFactory(const boost::shared_ptr<Validator> &validator) : CommandFactory(validator) {}
        static CommandFactory &get_instance()
        {
            static GenericCommandFactory instance{boost::make_shared<ArgumentsCountValidator>(1)};
            return instance;
        }

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"CREATE", boost::make_shared<CreateCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"STR", boost::make_shared<StringCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"SET", boost::make_shared<SetCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"HASH", boost::make_shared<HashCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"QUEUE", boost::make_shared<QueueCommandFactory>(boost::make_shared<ArgumentsCountValidator>(2))},
            {"DEL", boost::make_shared<DeleteCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))},
            {"KEYS", boost::make_shared<KeysCommandFactory>(boost::make_shared<ArgumentsCountValidator>(1))}};
    };
}