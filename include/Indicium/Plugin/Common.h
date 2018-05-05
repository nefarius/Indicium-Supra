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

#ifndef Common_h__
#define Common_h__

#include <Windows.h>

enum Direct3DVersion
{
    Direct3D9 = 0,
    Direct3D10 = 1 << 0,
    Direct3D11 = 1 << 1,
    Direct3D12 = 1 << 2
};

#ifdef __cplusplus

inline Direct3DVersion operator|(Direct3DVersion lhs, Direct3DVersion rhs)
{
    return static_cast<Direct3DVersion>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline Direct3DVersion operator&(Direct3DVersion lhs, Direct3DVersion rhs)
{
    return static_cast<Direct3DVersion>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

#endif

#define INDICIUM_EXPORT(_ret_) extern "C" __declspec(dllexport) _ret_ __cdecl

extern "C" __declspec(dllexport) BOOLEAN indicium_plugin_init(Direct3DVersion version);

#endif // Common_h__
