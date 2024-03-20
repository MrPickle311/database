#pragma once
#include <parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace db
{

    template <typename T>
    /// A concept that checks if a type is a base class of Parser
    concept ParserType = std::is_base_of<Parser, T>::value;

    template <ParserType PType>
    /// A class that provides an interface to a execution singletons
    class ExecutionIoC
    {
    private:
        PType parser;

    public:
        /// Constructs an instance of the ExecutionIoC class with a specific parser
        /// @param parser The parser to be used by the interface
        ExecutionIoC(PType parser) : parser(parser) {}

        /// Returns a reference to the internal parser
        PType &getParser() { return parser; }
    };

}