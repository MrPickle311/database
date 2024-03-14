#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <map>
#include <boost/make_shared.hpp>
#include <optional>

namespace db
{
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
    public:
        CreateStringCommand(const std::string &string_name) : KeyedCommand{string_name} {}
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
    };

    class CreateQueueCommand : public KeyedCommand
    {
    public:
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
        std::vector<std::string> set_names_;

    public:
        SetDifferenceCommand(const std::vector<std::string> &set_names) : set_names_(set_names) {}
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

    class SetMoveCommand : public KeyedCommand
    {
    private:
        std::string value_;
        std::string dest_set_name_;

    public:
        SetMoveCommand(const std::string &set_name, const std::string &value, const std::string &dest_set_name) : KeyedCommand(set_name), value_(value), dest_set_name_(dest_set_name) {}
        std::string execute() override;
    };

    class SetGetCommand : public KeyedCommand
    {
    private:
        std::optional<std::string> value_;

    public:
        SetGetCommand(const std::string &set_name, const std::optional<std::string> value) : KeyedCommand(set_name), value_(value) {}
        std::string execute() override;
    };

    class SetPopCommand : public KeyedCommand
    {
    private:
        std::optional<std::string> value_;

    public:
        SetPopCommand(const std::string &set_name, const std::optional<std::string> value) : KeyedCommand(set_name), value_(value) {}
        std::string execute() override;
    };

    //////FACTORY

    class CommandFactory
    {
    public:
        virtual boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) = 0;
    };

    // CREATE

    class CreateStringCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class CreateSetCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class CreateHashCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class CreateQueueCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class CreateCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"STR", boost::make_shared<CreateStringCommandFactory>()},
            {"SET", boost::make_shared<CreateSetCommandFactory>()},
            {"HASH", boost::make_shared<CreateHashCommandFactory>()},
            {"QUEUE", boost::make_shared<CreateQueueCommandFactory>()}};
    };

    // STRINGS

    class StringExistsCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringGetCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringLenCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringSubCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringAppendCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringPrependCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringInsertCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringTrimCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringLtrimCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringRtrimCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input) override;
    };

    class StringCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"EXISTS", boost::make_shared<StringExistsCommandFactory>()},
            {"GET", boost::make_shared<StringGetCommandFactory>()},
            {"LEN", boost::make_shared<StringLenCommandFactory>()},
            {"SUB", boost::make_shared<StringSubCommandFactory>()},
            {"APPEND", boost::make_shared<StringAppendCommandFactory>()},
            {"PREPEND", boost::make_shared<StringPrependCommandFactory>()},
            {"INSERT", boost::make_shared<StringInsertCommandFactory>()},
            {"TRIM", boost::make_shared<StringTrimCommandFactory>()},
            {"LTRIM", boost::make_shared<StringLtrimCommandFactory>()},
            {"RTRIM", boost::make_shared<StringRtrimCommandFactory>()}};
    };

    // SETS

    class SetAddCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetLenCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetIntersectionCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetDifferenceCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetUnionCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetContainsCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetGetAllCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetMoveCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetGetCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetPopCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class SetCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"ADD", boost::make_shared<SetAddCommandFactory>()},
            {"LEN", boost::make_shared<SetLenCommandFactory>()},
            {"INTER", boost::make_shared<SetIntersectionCommandFactory>()},
            {"DIFF", boost::make_shared<SetDifferenceCommandFactory>()},
            {"UNION", boost::make_shared<SetUnionCommandFactory>()},
            {"CONTAINS", boost::make_shared<SetContainsCommandFactory>()},
            {"GETALL", boost::make_shared<SetGetAllCommandFactory>()},
            {"MOVE", boost::make_shared<SetMoveCommandFactory>()},
            {"GET", boost::make_shared<SetGetCommandFactory>()},
            {"POP", boost::make_shared<SetPopCommandFactory>()}};
    };

    // OTHER

    class DeleteCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class KeysCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class HashCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class QueueCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
    };

    class GenericCommandFactory : public CommandFactory
    {
    public:
        boost::shared_ptr<Command> create_command(const std::vector<std::string> &input);
        static CommandFactory &get_instance()
        {
            static GenericCommandFactory instance;
            return instance;
        }

    private:
        std::map<std::string, boost::shared_ptr<CommandFactory>> children_factories_{
            {"CREATE", boost::make_shared<CreateCommandFactory>()},
            {"STR", boost::make_shared<StringCommandFactory>()},
            {"SET", boost::make_shared<SetCommandFactory>()},
            {"HASH", boost::make_shared<HashCommandFactory>()},
            {"QUEUE", boost::make_shared<QueueCommandFactory>()},
            {"DEL", boost::make_shared<DeleteCommandFactory>()},
            {"KEYS", boost::make_shared<KeysCommandFactory>()}};
    };
}