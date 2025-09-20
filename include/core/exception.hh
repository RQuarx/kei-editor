#pragma once
#include <exception>
#include <format>
#include <string>


class Exception : public std::exception
{
public:
    template<typename ...T_Args>
    Exception( std::string_view p_fmt, T_Args &&...p_args ) :
        m_msg(std::vformat(p_fmt, std::make_format_args(p_args...)))
    {}


    [[nodiscard]]
    auto
    what() const noexcept -> const char * override
    { return m_msg.c_str(); }


private:
    std::string m_msg;
};


class ParsingError : public Exception
{ public: using Exception::Exception; };