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

    std::vector<std::string> exports = { "Present", "Reset", "EndScene", "ResizeTarget" };

    ScopedLock<FastMutex> lock(mPlugins);

    File root(m_DllPath);
    std::vector<File> files;
    root.list(files);

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
                if (!plugin->hasSymbol(symbol))
                {
                    logger.error("Missing export %s from plugin %s", symbol, plugin->getPath());
                    plugin->unload();
                    break;
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
}

void PluginManager::unload()
{
    ScopedLock<FastMutex> lock(mPlugins);

    for (auto it = plugins.begin(); it != plugins.end();)
    {
        if ((*it)->isLoaded())
            (*it)->unload();
        delete *it;
        it = plugins.erase(it);
    }
}

void PluginManager::present(IID guid, LPVOID unknown)
{
    try
    {
        mPlugins.tryLock(50);

        for (auto& plugin : plugins)
        {
            static_cast<VOID(__cdecl*)(IID, LPVOID)>(plugin->getSymbol("Present"))(guid, unknown);
        }
    }
    catch (Poco::TimeoutException) {}

    mPlugins.unlock();
}

void PluginManager::reset(IID guid, LPVOID unknown)
{
    try
    {
        mPlugins.tryLock(50);

        for (auto& plugin : plugins)
        {
            static_cast<VOID(__cdecl*)(IID, LPVOID)>(plugin->getSymbol("Reset"))(guid, unknown);
        }
    }
    catch (Poco::TimeoutException) {}

    mPlugins.unlock();
}

void PluginManager::endScene(IID guid, LPVOID unknown)
{
    try
    {
        mPlugins.tryLock(50);

        for (auto& plugin : plugins)
        {
            static_cast<VOID(__cdecl*)(IID, LPVOID)>(plugin->getSymbol("EndScene"))(guid, unknown);
        }
    }
    catch (Poco::TimeoutException) {}

    mPlugins.unlock();
}

void PluginManager::resizeTarget(IID guid, LPVOID unknown)
{
    try
    {
        mPlugins.tryLock(50);

        for (auto& plugin : plugins)
        {
            static_cast<VOID(__cdecl*)(IID, LPVOID)>(plugin->getSymbol("ResizeTarget"))(guid, unknown);
        }
    }
    catch (Poco::TimeoutException) {}

    mPlugins.unlock();
}

