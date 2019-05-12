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

#include <string>
#include <Psapi.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace Indicium
{
    namespace Core
    {
        namespace Util
        {
            inline std::string expand_environment_variables(const std::string & str)
            {
                std::string expandedStr;
                const DWORD neededSize = ExpandEnvironmentStringsA(str.c_str(),
                    nullptr, 0);
                if (neededSize)
                {
                    expandedStr.resize(neededSize);
                    if (0 == ExpandEnvironmentStringsA(str.c_str(),
                        &expandedStr[0],
                        neededSize))
                    {
                        // pathological case requires a copy
                        expandedStr = str;
                    }
                }
                // RVO here
                return expandedStr;
            }

            inline std::string process_name()
            {
                const auto nSize = MAX_PATH + 1;
                char procName[nSize];
                GetProcessImageFileName(GetCurrentProcess(), procName, nSize);

                return std::string(procName);
            }
        };
    };
};
