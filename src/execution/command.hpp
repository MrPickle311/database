#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <map>
#include <boost/make_shared.hpp>

namespace db
{
    class Command
    {
    public:
        virtual std::string execute() = 0;
    };

    // CREATE

    class CreateStringCommand : public Command
    {
    private:
        std::string string_name_;

    public:
        CreateStringCommand(const std::string &string_name) : string_name_{string_name} {}
        std::string execute();
    };

    class CreateSetCommand : public Command
    {
    private:
        std::string set_name_;

    public:
        std::string execute();
    };

    class CreateHashCommand : public Command
    {
    private:
        std::string hash_name_;

    public:
        std::string execute();
    };

    class CreateQueueCommand : public Command
    {
    private:
        std::string queue_name_;

    public:
        std::string execute();
    };

    // STRING

    class StringCommand : public Command
    {
    protected:
        std::string str_name_;

    public:
        StringCommand(const std::string &str_name) : str_name_(str_name) {}
    };

    class StringGetCommand : public StringCommand
    {
    public:
        StringGetCommand(const std::string &str_name) : StringCommand(str_name) {}
        std::string execute() override;
    };

    class StringExistsCommand : public StringCommand
    {
    public:
        StringExistsCommand(const std::string &str_name) : StringCommand(str_name) {}
        std::string execute() override;
    };

    class StringLenCommand : public StringCommand
    {
    public:
        StringLenCommand(const std::string &str_name) : StringCommand(str_name) {}
        std::string execute() override;
    };

    class StringSubCommand : public StringCommand
    {
    private:
        uint start_pos_;
        uint end_pos_;

    public:
        StringSubCommand(const std::string &str_name, uint start_pos, uint end_pos)
            : StringCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}
        std::string execute() override;
    };

    class StringAppendCommand : public StringCommand
    {
    private:
        std::string value_;

    public:
        StringAppendCommand(const std::string &str_name, const std::string &value)
            : StringCommand(str_name), value_(value) {}
        std::string execute() override;
    };

    class StringPrependCommand : public StringCommand
    {
    private:
        std::string value_;

    public:
        StringPrependCommand(const std::string &str_name, const std::string &value)
            : StringCommand(str_name), value_(value) {}
        std::string execute() override;
    };

    class StringInsertCommand : public StringCommand
    {
    private:
        uint pos_;
        std::string value_;

    public:
        StringInsertCommand(const std::string &str_name, uint pos, const std::string &value)
            : StringCommand(str_name), pos_(pos), value_(value) {}
        std::string execute() override;
    };

    class StringTrimCommand : public StringCommand
    {
    private:
        uint start_pos_;
        uint end_pos_;

    public:
        StringTrimCommand(const std::string &str_name, uint start_pos, uint end_pos)
            : StringCommand(str_name), start_pos_(start_pos), end_pos_(end_pos) {}
        std::string execute() override;
    };

    class StringLtrimCommand : public StringCommand
    {
    private:
        uint char_count_;

    public:
        StringLtrimCommand(const std::string &str_name, uint char_count)
            : StringCommand(str_name), char_count_(char_count) {}
        std::string execute() override;
    };

    class StringRtrimCommand : public StringCommand
    {
    private:
        uint char_count_;

    public:
        StringRtrimCommand(const std::string &str_name, uint char_count)
            : StringCommand(str_name), char_count_(char_count) {}
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

    class SetCommandFactory : public CommandFactory
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