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