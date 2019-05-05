/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Windows.h"
#include <detours.h>
#include <Poco/Exception.h>


enum class CallConvention
{
    stdcall_t,
    cdecl_t
};

template <CallConvention cc, typename retn, typename convention, typename ...args>
struct convention;

template <typename retn, typename ...args>
struct convention<CallConvention::stdcall_t, retn, args...>
{
    typedef retn (__stdcall *type)(args ...);
};

template <typename retn, typename ...args>
struct convention<CallConvention::cdecl_t, retn, args...>
{
    typedef retn (__cdecl *type)(args ...);
};

template <CallConvention cc, typename retn, typename ...args>
class Hook
{
    typedef typename convention<cc, retn, args...>::type type;

    size_t _orig;
    type _detour;

    bool _isApplied;

public:
    Hook() : _orig(0), _detour(0), _isApplied(false)
    {
    }

    ~Hook()
    {
        remove();
    }

    template <typename T>
    void apply(T pFunc, type detour)
    {
        _detour = detour;
        _orig = static_cast<size_t>(pFunc);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        const auto result = DetourAttach(reinterpret_cast<void **>(&_orig), reinterpret_cast<void *>(_detour));
        DetourTransactionCommit();

        if (result != NO_ERROR)
        {
            switch (result)
            {
            case ERROR_INVALID_BLOCK:
                throw Poco::ApplicationException("The function referenced is too small to be detoured.", result);

            case ERROR_INVALID_HANDLE:
                throw Poco::ApplicationException("The ppPointer parameter is null or points to a null pointer.", result);

            case ERROR_INVALID_OPERATION:
                throw Poco::ApplicationException("No pending transaction exists.", result);

            case ERROR_NOT_ENOUGH_MEMORY:
                throw Poco::ApplicationException("Not enough memory exists to complete the operation.", result);

            default:
                throw Poco::ApplicationException("Unknown error", result);
            }
        }

        _isApplied = true;
    }

    bool remove()
    {
        if (!_isApplied)
            return false;

        _isApplied = false;

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        const auto ret = DetourDetach(reinterpret_cast<void **>(&_orig), reinterpret_cast<void *>(_detour));
        DetourTransactionCommit();

        return ret == NO_ERROR;
    }

    retn callOrig(args ... p)
    {
        return type(_orig)(p...);
    }

    bool isApplied() const
    {
        return _isApplied;
    }
};

