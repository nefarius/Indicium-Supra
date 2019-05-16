#pragma once

#include <cstdarg>    // va_start, va_end, std::va_list
#include <cstddef>    // std::size_t
#include <stdexcept>  // std::runtime_error
#include <vector>     // std::vector

namespace Indicium
{
    namespace Core
    {
        namespace Logging
        {
            inline std::string format(const char* const format, ...)
            {
                auto temp = std::vector<char>{};
                auto length = std::size_t{63};
                std::va_list args;
                while (temp.size() <= length)
                {
                    temp.resize(length + 1);
                    va_start(args, format);
                    const auto status = std::vsnprintf(temp.data(), temp.size(), format, args);
                    va_end(args);
                    if (status < 0)
                        throw std::runtime_error{"string formatting error"};
                    length = static_cast<std::size_t>(status);
                }
                return std::string{temp.data(), length};
            }
        };
    };
};
