#pragma once
#include <string>
#include <boost/format.hpp>

inline static std::string format_string_recurse(boost::format& message)
{
    return message.str();
}

template <typename TValue, typename... TArgs>
std::string format_string_recurse(boost::format& message, TValue&& arg, TArgs&&... args)
{
    message % std::forward<TValue>(arg);
    return format_string_recurse(message, std::forward<TArgs>(args)...);
}

template <typename... TArgs>
std::string format_string(const char* fmt, TArgs&&... args)
{
    using namespace boost::io;
    boost::format message(fmt);
    return format_string_recurse(message, std::forward<TArgs>(args)...);
}
