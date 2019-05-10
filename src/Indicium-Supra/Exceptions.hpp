#pragma once

#include <stdexcept>

class DetourException : public std::runtime_error
{
public:
    explicit DetourException(const std::string &msg) :
        std::runtime_error(msg.c_str())
    { }

    explicit DetourException(const char *msg) :
        std::runtime_error(msg)
    { }
};
