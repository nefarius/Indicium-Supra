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

PluginManager::PluginManager()
{
    GetModuleFileName(reinterpret_cast<HINSTANCE>(&__ImageBase), m_DllPath, _countof(m_DllPath));
    PathRemoveFileSpec(m_DllPath);
}


PluginManager::~PluginManager()
{
}

void PluginManager::refresh()
{
    auto& logger = Logger::get(LOG_REGION());
    std::lock_guard<std::mutex> lock(m_pluginPaths_m);

    m_pluginPaths.clear();

    File root(m_DllPath);
    std::vector<File> files;
    root.list(files);

    for (auto it = files.begin(); it != files.end(); ++it)
    {
        if ((*it).isFile() && findStringIC((*it).path(), ".Plugin.dll"))
        {
            logger.information("Found plugin %s", it->path());
            m_pluginPaths.push_back(it->path());
        }
    }
}

void PluginManager::load()
{
    auto& logger = Logger::get(LOG_REGION());
    this->refresh();

    std::lock_guard<std::mutex> pLock(m_pluginPaths_m), mLock(m_pluginMods_m);

    for (const auto& path : m_pluginPaths)
    {
        auto hMod = LoadLibrary(path.c_str());

        if (hMod == nullptr)
        {
            logger.error("Couldn't load Indicium plugin from file %s (%lu)", path, GetLastError());
            continue;
        }

        m_pluginMods.push_back(hMod);

        {
            std::lock_guard<std::mutex> lock(m_presentFuncs_m);
            auto present = static_cast<LPVOID>(GetProcAddress(hMod, "Present"));
            if (present != nullptr)
            {
                m_presentFuncs.push_back(present);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_resetFuncs_m);
            auto reset = static_cast<LPVOID>(GetProcAddress(hMod, "Reset"));
            if (reset != nullptr)
            {
                m_resetFuncs.push_back(reset);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_endSceneFuncs_m);
            auto endScene = static_cast<LPVOID>(GetProcAddress(hMod, "EndScene"));
            if (endScene != nullptr)
            {
                m_endSceneFuncs.push_back(endScene);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_resizeTargetFuncs_m);
            auto resize = static_cast<LPVOID>(GetProcAddress(hMod, "ResizeTarget"));
            if (resize != nullptr)
            {
                m_resizeTargetFuncs.push_back(resize);
            }
        }
    }
}

void PluginManager::unload()
{
    // TODO: implement!
}

void PluginManager::present(IID guid, LPVOID unknown)
{
    std::lock_guard<std::mutex> lock(m_presentFuncs_m);

    for (const auto& func : m_presentFuncs)
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
    }
}

void PluginManager::reset(IID guid, LPVOID unknown)
{
    std::lock_guard<std::mutex> lock(m_resetFuncs_m);

    for (const auto& func : m_resetFuncs)
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
    }
}

void PluginManager::endScene(IID guid, LPVOID unknown)
{
    std::lock_guard<std::mutex> lock(m_endSceneFuncs_m);

    for (const auto& func : m_endSceneFuncs)
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
    }
}

void PluginManager::resizeTarget(IID guid, LPVOID unknown)
{
    std::lock_guard<std::mutex> lock(m_resizeTargetFuncs_m);

    for (const auto& func : m_resizeTargetFuncs)
    {
        static_cast<VOID(__cdecl*)(IID, LPVOID)>(func)(guid, unknown);
    }
}

