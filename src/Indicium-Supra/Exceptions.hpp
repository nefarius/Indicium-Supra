#pragma once

#include <Windows.h>
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

class GenericWinAPIException : public std::runtime_error
{
    DWORD last_error_;

public:
    explicit GenericWinAPIException(const std::string &msg) :
        std::runtime_error(msg.c_str()), last_error_(GetLastError())
    { }

    explicit GenericWinAPIException(const char *msg) :
        std::runtime_error(msg), last_error_(GetLastError())
    { }

    virtual DWORD get_last_error() const
    {
        return last_error_;
    }
};

class ModuleNotFoundException : public GenericWinAPIException
{
public:
    explicit ModuleNotFoundException(const std::string &msg) :
        GenericWinAPIException(msg)
    { }

    explicit ModuleNotFoundException(const char *msg) :
        GenericWinAPIException(msg)
    { }
};

class ProcAddressNotFoundException : public GenericWinAPIException
{
public:
    explicit ProcAddressNotFoundException(const std::string &msg) :
        GenericWinAPIException(msg)
    { }

    explicit ProcAddressNotFoundException(const char *msg) :
        GenericWinAPIException(msg)
    { }
};

// ProcAddress
