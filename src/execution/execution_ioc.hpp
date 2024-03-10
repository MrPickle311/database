#pragma once
#include <parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace db
{

    template <typename T>
    concept ParserType = std::is_base_of<Parser, T>::value;

    template <ParserType PType>
    class ExecutionIoC
    {
    private:
        boost::shared_ptr<PType> parser;

    public:
        ExecutionIoC(){
            this->parser = boost::make_shared<PType>();
        }
        boost::shared_ptr<PType> getParser() { return parser; }
    };


}