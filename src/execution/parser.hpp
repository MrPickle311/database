#pragma once
#include <command.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace db
{
    /**
     * @brief The Parser interface provides a contract for parsing input text into commands.
     */
    class Parser
    {
    public:
        /**
         * @brief Extracts a list of commands from the given input string.
         *
         * @param input The input string to be parsed.
         * @return A vector of shared pointers to Command objects representing the extracted commands.
         * @return An empty vector if no commands are found in the input.
         */
        virtual std::vector<boost::shared_ptr<Command>> extract_commands(const std::string &input) = 0;
    };

    /**
     * @brief The Tokenizer interface provides a contract for splitting a string into individual tokens.
     */
    class Tokenizer
    {
    public:
        /**
         * @brief Splits the given input string into a vector of tokens.
         *
         * @param input The input string to be tokenized.
         * @return A vector of strings, where each element represents a token extracted from the input.
         */
        virtual std::vector<std::string> tokenize(const std::string &input) = 0;
    };

    /**
     * The BigTokenizer class inherits from the Tokenizer interface and splits a string into tokens separated by a semicolon (;).
     */
    class BigTokenizer : public Tokenizer
    {
    private:
        /**
         * The character used as a separator for tokenization.
         */
        const std::string delimeter = ";";

    public:
        /**
         * Splits the given input string `input` into tokens using the semicolon (`;`) as a separator.
         *
         * \param input The string to be split into tokens.
         * \return A vector of strings, where each element represents a token identified in the original string.
         */
        std::vector<std::string> tokenize(const std::string &input) override;

    public:
        /**
         * Gets a reference to the single instance of the BigTokenizer class.
         *
         * \return A reference to the BigTokenizer instance.
         */
        static Tokenizer &get_instance();
    };

    /**
     * The SmallTokenizer class inherits from the Tokenizer interface and splits a string into tokens separated by spaces (" ").
     */
    class SmallTokenizer : public Tokenizer
    {
    private:
        /**
         * The character used as a separator for tokenization.
         */
        const std::string delimeter = " ";

    public:
        /**
         * Splits the given input string `input` into tokens using spaces (" ") as separators.
         *
         * \param input The string to be split into tokens.
         * \return A vector of strings, where each element represents a token identified in the original string.
         */
        std::vector<std::string> tokenize(const std::string &input) override;

    public:
        /**
         * Gets a reference to the single instance of the SmallTokenizer class.
         *
         * \return A reference to the SmallTokenizer instance.
         */
        static Tokenizer &get_instance();
    };

    /**
     * The DefaultParser class implements the Parser interface and parses input text into commands.
     */
    class DefaultParser : public Parser
    {
    private:
        /**
         * Reference to the main tokenizer used for initial splitting of the input string.
         */
        Tokenizer &main_tokenizer_;

        /**
         * Reference to the sub-tokenizer used for further splitting of command tokens.
         */
        Tokenizer &sub_tokenizer_;

        /**
         * Reference to the command factory used to create concrete Command objects based on parsed tokens.
         */
        CommandFactory &command_factory_;

    public:
        /**
         * Constructor that takes references to the main tokenizer, sub-tokenizer, and command factory.
         *
         * \param main_tokenizer Reference to the main tokenizer object.
         * \param sub_tokenizer Reference to the sub-tokenizer object.
         * \param command_factory Reference to the command factory object.
         */
        DefaultParser(Tokenizer &main_tokenizer, Tokenizer &sub_tokenizer, CommandFactory &command_factory);

        /**
         * Implementation of the `extract_commands` method inherited from the Parser interface.
         *
         * This method parses the input string into commands:
         *
         * 1. Splits the input string using the main tokenizer.
         * 2. Iterates through the resulting tokens.
         * 3. Skips tokens containing only whitespace characters.
         * 4. For non-empty tokens, further splits them using the sub-tokenizer.
         * 5. Uses the command factory to create a Command object based on the sub-tokens.
         * 6. Adds the created Command object to the result vector.
         *
         * \param input The input string to be parsed.
         * \return A vector of shared pointers to Command objects representing the extracted commands.
         * \return An empty vector if no commands are found in the input.
         */
        std::vector<boost::shared_ptr<Command>> extract_commands(const std::string &input) override;
    };

}