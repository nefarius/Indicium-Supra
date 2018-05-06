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

#include <Poco/SingletonHolder.h>
#include <Poco/Buffer.h>
#include <Poco/Path.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class GlobalState
{
public:
    GlobalState() {}
    ~GlobalState() {}

    static GlobalState& instance()
    {
        static Poco::SingletonHolder<GlobalState> sh;
        return *sh.get();
    }

    /**
     * \fn  std::string GlobalState::processName() const
     *
     * \brief   Name of the process the library is currently loaded into.
     *
     * \author  Benjamin "Nefarius" Höglinger
     * \date    05.05.2018
     *
     * \return  The process name.
     */
    std::string processName() const
    {
        Poco::Buffer<char> procName(MAX_PATH + 1);
        GetProcessImageFileName(GetCurrentProcess(), procName.begin(), (DWORD)procName.size());

        return std::string(procName.begin());
    }

    /**
     * \fn  std::string GlobalState::rootPath() const
     *
     * \brief   Path where the main core library resides.
     *
     * \author  Benjamin "Nefarius" Höglinger
     * \date    05.05.2018
     *
     * \return  Path to core library.
     */
    std::string rootPath() const
    {
        Poco::Buffer<char> myPathBuf(MAX_PATH + 1);
        GetModuleFileNameA(
            reinterpret_cast<HINSTANCE>(&__ImageBase),
            myPathBuf.begin(),
            static_cast<DWORD>(myPathBuf.size())
        );
        Poco::Path myPath(myPathBuf.begin());
        Poco::Path myDir(myPath.parent());

        return myDir.toString();
    }
protected:

private:
};