#include "PluginManager.h"
#include "Misc.h"

#include <Shlwapi.h>
#include <algorithm>

#define POCO_NO_UNWINDOWS
#include <Poco/Logger.h>
#include <Poco/Path.h>
#include <Poco/File.h>

using Poco::Logger;
using Poco::Path;
using Poco::File;


PluginManager::PluginManager()
{
    GetModuleFileName(reinterpret_cast<HINSTANCE>(&__ImageBase), m_DllPath, _countof(m_DllPath));
    PathRemoveFileSpec(m_DllPath);
}

PluginManager::~PluginManager()
{
}

bool PluginManager::findStringIC(const std::string& strHaystack, const std::string& strNeedle) const
{
    auto it = std::search(
        strHaystack.begin(), strHaystack.end(),
        strNeedle.begin(), strNeedle.end(),
        [](char ch1, char ch2)
    {
        return ::toupper(ch1) == ::toupper(ch2);
    }
    );
    return (it != strHaystack.end());
}

void PluginManager::load()
{
    auto& logger = Logger::get(LOG_REGION());

    ScopedLock<FastMutex> lock(mPlugins);

    File root(m_DllPath);
    std::vector<File> files;
    root.list(files);

    logger.information("Enumerating plugins...");

    for (auto it = files.begin(); it != files.end(); ++it)
    {
        if ((*it).isFile() && findStringIC((*it).path(), ".Plugin.dll"))
        {
            logger.information("Found plugin %s", it->path());

            auto plugin = new SharedLibrary();

            try
            {
                plugin->load(it->path());
            }
            catch (Poco::LibraryLoadException lle)
            {
                logger.error("Couldn't load plugin %s: %s", it->path(), std::string(lle.what()));
                delete plugin;
                continue;
            }

            for (const auto& symbol : exports)
            {
                if (plugin->hasSymbol(symbol))
                {
                    _fpMap[symbol].push_back(plugin->getSymbol(symbol));
                }
                else
                {
                    logger.warning("Missing export %s from plugin %s", symbol, plugin->getPath());
                }
            }

            if (!plugin->isLoaded())
            {
                delete plugin;
                continue;
            }

            plugins.push_back(plugin);
        }
    }

    logger.information("Finished enumerating plugins");
}

void PluginManager::unload()
{
    auto& logger = Logger::get(LOG_REGION());

    logger.information("Unloading plugins...");

    ScopedLock<FastMutex> lock(mPlugins);

    // discard function pointers
    for (const auto& symbol : exports)
    {
        _fpMap[symbol].clear();
    }

    // unload libraries
    for (auto it = plugins.begin(); it != plugins.end();)
    {
        if ((*it)->isLoaded())
            (*it)->unload();
        delete *it;
        it = plugins.erase(it);
    }

    logger.information("Finished unloading plugins");
}

void PluginManager::present(IID guid, LPVOID unknown, Direct3DVersion version)
{
    static std::string key = "Present";

    for (auto& fp : _fpMap[key])
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID, Direct3DVersion)>(fp)(guid, unknown, version);
    }
}

void PluginManager::reset(IID guid, LPVOID unknown, Direct3DVersion version)
{
    static std::string key = "Reset";

    for (auto& fp : _fpMap[key])
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID, Direct3DVersion)>(fp)(guid, unknown, version);
    }
}

void PluginManager::endScene(IID guid, LPVOID unknown, Direct3DVersion version)
{
    static std::string key = "EndScene";

    for (auto& fp : _fpMap[key])
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID, Direct3DVersion)>(fp)(guid, unknown, version);
    }
}

void PluginManager::resizeTarget(IID guid, LPVOID unknown, Direct3DVersion version)
{
    static std::string key = "ResizeTarget";

    for (auto& fp : _fpMap[key])
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID, Direct3DVersion)>(fp)(guid, unknown, version);
    }
}

